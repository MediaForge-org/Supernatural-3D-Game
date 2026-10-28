#include <doctest.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <memory>
#include <map>
#include <sstream>
#include <spn/core/Devices.h>
#include <spn/core/InteractionSystem.h>
#include <spn/core/ObjectPersistence.h>
#include <spn/core/TestDoor.h>

// Runs the interactive objects and gate scenarios the Python enrichment layer generated for the enriched test map
// (Data/env_scenarios.txt, written by spn_env_enrichment.py --write-fixture) against the REAL Core: interaction system, devices, links,
// events and the object persistence. This proves the generated data is consistent with the Core rules on Fedora. It does NOT show anything about
// Unreal actors, collision or visuals (no Unreal adapter for switch/button/lever exists yet; doors are ASpnTestDoor).

using namespace spn::core;

namespace {

struct Obj {
    std::string id, kind, region;
    double x = 0, y = 0, z = 0, radius = 0;
    int priority = 0;
    std::int64_t cooldownMs = 0;
    std::map<std::string, FactValue, std::less<>> defaults;
};
struct LinkRec { std::string source, when, target, key; FactValue value; std::optional<FactValue> otherwise; };
struct Step { std::string scenario, target, expect; };
struct Fixture { std::vector<Obj> objects; std::vector<LinkRec> links; std::vector<Step> steps; std::vector<std::string> scenarioOrder; };

FactValue ParseValue(const std::string& type, const std::string& text) {
    if (type == "b") return text == "1";
    return static_cast<std::int64_t>(std::stoll(text));
}

Fixture Load() {
    std::ifstream in(std::string(SPN_TEST_DATA_DIR) + "/env_scenarios.txt");
    REQUIRE_MESSAGE(in.good(), "fixture missing: regenerate it with spn_env_enrichment.py --write-fixture");
    Fixture f;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream words(line);
        std::string tag;
        words >> tag;
        if (tag == "object") {
            Obj o;
            words >> o.id >> o.kind >> o.region >> o.x >> o.y >> o.z >> o.radius >> o.priority >> o.cooldownMs;
            f.objects.push_back(o);
        } else if (tag == "default") {
            std::string id, name, type, text;
            words >> id >> name >> type >> text;
            auto it = std::find_if(f.objects.begin(), f.objects.end(), [&](const Obj& o) { return o.id == id; });
            REQUIRE(it != f.objects.end());
            it->defaults[name] = ParseValue(type, text);
        } else if (tag == "link") {
            LinkRec l;
            std::string type, text, otype, otext;
            words >> l.source >> l.when >> l.target >> l.key >> type >> text >> otype;
            l.value = ParseValue(type, text);
            if (otype != "-") { words >> otext; l.otherwise = ParseValue(otype, otext); }
            f.links.push_back(l);
        } else if (tag == "step") {
            Step s;
            words >> s.scenario >> s.target >> s.expect;
            if (std::find(f.scenarioOrder.begin(), f.scenarioOrder.end(), s.scenario) == f.scenarioOrder.end()) f.scenarioOrder.push_back(s.scenario);
            f.steps.push_back(s);
        } else {
            FAIL("unknown fixture record: " << tag);
        }
    }
    return f;
}

InteractStatus ParseStatus(const std::string& name) {
    for (int i = 0; i <= static_cast<int>(InteractStatus::InvalidInput); ++i) {
        std::string label(ToString(static_cast<InteractStatus>(i)));
        label.erase(std::remove(label.begin(), label.end(), ' '), label.end());
        std::transform(label.begin(), label.end(), label.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        std::string wanted = name;
        std::transform(wanted.begin(), wanted.end(), wanted.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (label == wanted) return static_cast<InteractStatus>(i);
    }
    FAIL("unknown status in fixture: " << name);
    return InteractStatus::InvalidInput;
}

// The revision counts the journal records of the SAVING session (informational, see ObjectRecord::revision); state equality is about the fields.
std::vector<ObjectRecord> WithoutRevision(std::vector<ObjectRecord> records) {
    for (ObjectRecord& r : records) r.revision = 0;
    return records;
}

struct Session {
    InteractionRegistry registry;
    WorldFacts facts;
    GameEventBus bus;
    std::unique_ptr<InteractionSystem> system;
    WorldObjectCatalog catalog;
    std::map<std::string, Obj> objects;
    std::vector<GameEvent> events;
    ScopedSubscription recorder;

    explicit Session(const Fixture& f) {
        REQUIRE_FALSE(RegisterTestDoor(registry).has_value());
        REQUIRE_FALSE(RegisterDevices(registry).has_value());
        system = std::make_unique<InteractionSystem>(registry, facts, &bus);
        recorder = ScopedSubscription(bus, bus.Subscribe([this](const GameEvent& e) { events.push_back(e); }));
        for (const Obj& o : f.objects) {
            const auto kind = ContentId::Parse(o.kind);
            REQUIRE(kind.has_value());
            InteractableDescriptor d;
            d.id = o.id; d.kind = *kind; d.centre = {o.x / 100.0, o.y / 100.0, o.z / 100.0}; d.radius = o.radius / 100.0; d.priority = o.priority;
            d.cooldownMs = o.cooldownMs; d.defaults.values = {o.defaults.begin(), o.defaults.end()};
            REQUIRE_FALSE(system->AddInteractable(d).has_value());
            ObjectConfig c;
            c.id = o.id; c.kind = *kind; c.region = o.region; c.defaults = {o.defaults.begin(), o.defaults.end()};
            REQUIRE_FALSE(catalog.Add(c).has_value());
            objects[o.id] = o;
        }
        for (const LinkRec& l : f.links) {
            Link link{l.source, l.when == "-" ? std::nullopt : std::optional<std::int64_t>(std::stoll(l.when)), l.target, l.key, l.value, l.otherwise};
            const auto error = system->AddLink(link);
            REQUIRE_MESSAGE(!error.has_value(), "link " << l.source << ">" << l.target << ": " << (error ? ToString(error->code) : ""));
        }
    }
    InteractionRequest Request(const std::string& target, std::int64_t nowMs) const {
        const Obj& o = objects.at(target);
        InteractionRequest r;
        r.actor.eye = {o.x / 100.0 - 1.0, o.y / 100.0, o.z / 100.0};   // one metre in front of the object, looking at it
        r.actor.viewDirection = {1.0, 0.0, 0.0};
        r.targetId = target;
        r.nowMs = nowMs;
        return r;
    }
};

} // namespace

TEST_CASE("the fixture is consistent: valid ids, known kinds, regions, links, at least two scenarios") {
    const Fixture f = Load();
    CHECK(f.objects.size() >= 4);
    CHECK(f.scenarioOrder.size() >= 2);
    CHECK(f.links.size() == f.scenarioOrder.size());
    for (const Obj& o : f.objects) {
        CHECK(IsValidInteractableId(o.id));
        CHECK(IsValidInteractableId(o.region));
        CHECK(ContentId::Parse(o.kind).has_value());
    }
    Session s(f); // builds registry, system, catalog and links: every validation of the Core accepts the generated data
    CHECK(s.system->Count() == f.objects.size());
    CHECK(s.catalog.Size() == f.objects.size());
}

TEST_CASE("every generated gate scenario passes against the real interaction system, with events, and its state survives a save and load") {
    const Fixture f = Load();
    for (const std::string& scenario : f.scenarioOrder) {
        SUBCASE(scenario.c_str()) {
            Session s(f);
            std::int64_t now = 0;
            std::string deviceId;
            for (const Step& step : f.steps) {
                if (step.scenario != scenario) continue;
                now += 1000;
                const auto result = s.system->Interact(s.Request(step.target, now));
                CHECK_MESSAGE(result.status == ParseStatus(step.expect), scenario << ": " << step.target << " gave " << ToString(result.status) << ", expected " << step.expect);
                if (deviceId.empty() && step.target.find("_gate") == std::string::npos) deviceId = step.target;
            }
            // events: the link fired and the gate changed
            const bool linked = std::any_of(s.events.begin(), s.events.end(), [](const GameEvent& e) { return e.type == GameEventType::LinkedMechanismActivated; });
            CHECK(linked);
            // standing at the device selects the device (priority 2) and not the nearby gate (priority 1)
            ActorView actor;
            actor.eye = {s.objects.at(deviceId).x / 100.0 - 1.0, s.objects.at(deviceId).y / 100.0, s.objects.at(deviceId).z / 100.0};
            CHECK(s.system->SelectTarget(actor).value_or("") == deviceId);

            // persistence: only changed objects are stored, a new session restores exactly this state
            const auto records = CaptureChanged(s.facts, s.catalog);
            CHECK(records.size() >= 2);
            auto parsed = ParseRecords(SerializeRecords(records));
            REQUIRE(parsed.HasValue());
            Session restored(f);
            const auto report = ApplyRecords(restored.facts, restored.catalog, parsed.Value(), RestoreMode::Replace);
            CHECK(report.rejected.empty());
            CHECK(report.missing.empty());
            CHECK_MESSAGE(report.unknownFields.empty(), "state fields without an authored default would be lost: " << (report.unknownFields.empty() ? "" : report.unknownFields.front()));
            CHECK_MESSAGE(WithoutRevision(CaptureChanged(restored.facts, restored.catalog)) == WithoutRevision(records),
                          "saved:\n" << SerializeRecords(records) << "restored:\n" << SerializeRecords(CaptureChanged(restored.facts, restored.catalog)));
            // region unload and reload keeps the state
            RegionStateCache cache;
            const std::string region = s.objects.at(deviceId).region;
            CHECK(cache.Unload(region, restored.facts, restored.catalog) >= 1);
            CHECK(CaptureChanged(restored.facts, restored.catalog, region).empty());
            CHECK(cache.Load(region, restored.facts, restored.catalog).rejected.empty());
            CHECK(WithoutRevision(CaptureChanged(restored.facts, restored.catalog)) == WithoutRevision(records));
        }
    }
}

TEST_CASE("gates start locked: opening a gate without its device is refused for every gate") {
    const Fixture f = Load();
    Session s(f);
    std::int64_t now = 0;
    int gates = 0;
    for (const Obj& o : f.objects) {
        if (o.kind != "placeholder:interactable.door") continue;
        ++gates;
        const auto result = s.system->Interact(s.Request(o.id, now += 1000));
        CHECK(result.status == InteractStatus::Locked);
        CHECK(s.facts.Find(TestDoorOpenKey(o.id)) == nullptr);
    }
    CHECK(gates >= 2);
}
