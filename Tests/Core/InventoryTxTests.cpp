// Behaviour-preservation and rollback tests for the inventory transaction mechanism (M2B2). They are written against the public API only,
// so they hold for any correct implementation of "all-or-nothing transactions" (staged copy in M2B1, undo log in M2B2).
//
// The golden transcript (Tests/Core/Data/inventory_tx_golden.txt) was recorded with the M2B1 staged-copy implementation BEFORE the undo-log
// change. Reproducing it byte for byte shows that the new mechanism returns the same result, error code, failing command index, revision,
// instance-id counter and serialized state after every one of the batches.
#include <array>
#include <cstdlib>
#include <doctest.h>
#include <fstream>
#include <limits>
#include <set>
#include <spn/core/Inventory.h>
#include <spn/core/SaveSlot.h>
#include <sstream>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

// std::getenv is flagged by MSVC (C4996); _dupenv_s is its safe equivalent.
std::string EnvOrEmpty(const char* name) {
#if defined(_MSC_VER)
    char* value = nullptr;
    std::size_t size = 0;
    if (_dupenv_s(&value, &size, name) != 0 || value == nullptr) return {};
    std::string result(value);
    std::free(value);
    return result;
#else
    const char* value = std::getenv(name);
    return value != nullptr ? std::string(value) : std::string();
#endif
}

// Arbitrary test values, not game balance.
struct World {
    ItemCatalog catalog;
    ContentId coin = Cid("test:item.coin"), bandage = Cid("test:item.bandage"), knife = Cid("test:item.knife"), coat = Cid("test:item.coat"),
              brick = Cid("test:item.brick"), hand = Cid("test:slot.hand"), torso = Cid("test:slot.torso");
    World() {
        const auto add = [&](ItemDefinition d) { REQUIRE_FALSE(catalog.Add(std::move(d)).has_value()); };
        ItemDefinition c;
        c.id = coin;
        c.category = Cid("test:cat.currency");
        c.maxStack = 10;
        add(c);
        ItemDefinition b;
        b.id = bandage;
        b.category = Cid("test:cat.medical");
        b.maxStack = 5;
        b.weightGrams = 100;
        b.volumeMilliliters = 50;
        add(b);
        ItemDefinition k;
        k.id = knife;
        k.category = Cid("test:cat.tool");
        k.weightGrams = 300;
        k.equipSlots = {hand};
        add(k);
        ItemDefinition t;
        t.id = coat;
        t.category = Cid("test:cat.clothing");
        t.equipSlots = {torso};
        add(t);
        ItemDefinition br;
        br.id = brick;
        br.category = Cid("test:cat.junk");
        br.maxStack = 4;
        br.weightGrams = std::numeric_limits<std::uint64_t>::max() / 3; // 4 of them overflow uint64
        add(br);
    }
    ContainerRules Rules(std::optional<std::uint32_t> stacks = std::nullopt, std::optional<std::uint64_t> weight = std::nullopt,
                         std::optional<std::uint64_t> volume = std::nullopt) const {
        ContainerRules r;
        r.kind = Cid("test:container.generic");
        r.maxStacks = stacks;
        r.maxWeightGrams = weight;
        r.maxVolumeMilliliters = volume;
        r.equipSlots = {hand, torso};
        return r;
    }
};

std::string Hex8(std::uint32_t v) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%08x", v);
    return buf;
}

InventoryCommand RandomCommand(const World& w, const InventoryState& s, Lcg& rng) {
    const auto container = [&] { return ContainerId(rng.Below(12) == 0 ? 4 + rng.Below(2) : 1 + rng.Below(3)); }; // 4 and 5 mostly do not exist
    const auto stackOf = [&](ContainerId id) {
        ItemInstanceId any(1 + rng.Below(60));
        if (const Container* c = s.FindContainer(id); c != nullptr && !c->stacks.empty() && rng.Below(10) != 0) {
            auto it = c->stacks.begin();
            std::advance(it, rng.Below(static_cast<std::uint32_t>(c->stacks.size())));
            any = it->first;
        }
        return any;
    };
    const ContainerId c1 = container();
    const ContainerId c2 = container();
    const ItemInstanceId pick = stackOf(c1);
    const std::vector<ContentId> defs{w.coin, w.bandage, w.knife, w.coat, w.brick};
    const ContentId& def = defs[rng.Below(5)];
    const std::uint32_t qty = 1 + rng.Below(12);
    const auto sideContainer = [&] { return ContainerId(4 + rng.Below(2)); }; // 4 and 5 are created and destroyed over and over
    const auto equippable = [&](ContainerId id) { // a knife or coat stack of the container, if any
        ItemInstanceId found = stackOf(id);
        if (const Container* c = s.FindContainer(id)) {
            for (const auto& [sid, st] : c->stacks) {
                if (!w.catalog.Find(st.definition)->equipSlots.empty() && rng.Below(3) == 0) found = sid;
            }
        }
        return found;
    };
    switch (rng.Below(22)) {
    case 0: case 1: case 2: return AddItem{c1, def, w.catalog.Find(def)->Stackable() ? qty : 1, {}};
    case 3: case 4: case 5: return RemoveItem{c1, pick, 1 + rng.Below(3)};
    case 6: case 7: case 8: return TransferItem{c1, c2, pick, 1 + rng.Below(5)};
    case 9: case 10: return SplitStack{c1, pick, 1 + rng.Below(3)};
    case 11: case 12: return MergeStacks{c1, pick, stackOf(c1), rng.Below(4)};
    case 13: case 14: { const ItemInstanceId item = equippable(c1); return EquipItem{c1, item, rng.Below(2) == 0 ? w.hand : w.torso}; }
    case 15: return UnequipSlot{c1, rng.Below(2) == 0 ? w.hand : w.torso};
    case 16: case 17: return CreateContainer{sideContainer(), rng.Below(2) == 0 ? w.Rules() : w.Rules(3, 900, 300)};
    case 18: case 19: return DestroyContainer{sideContainer()};
    case 20: return AddItem{c1, defs[rng.Below(2)], qty, {}};
    default: return AddItem{c1, w.brick, 1 + rng.Below(4), {}}; // weight overflow candidates
    }
}

struct Run {
    std::string transcript;
    std::size_t accepted = 0, rejected = 0, multiCommandRejected = 0, rolledBackAfterProgress = 0;
    std::array<std::size_t, 9> acceptedByType{}; ///< commands inside accepted batches, by variant index
};

// Applies `batches` random batches of 1..4 commands. After EVERY batch: invariants hold, a rejected batch left the state bit-identical, an
// accepted batch equals the same commands applied one by one (apart from the revision), and the transcript line is recorded.
Run Play(std::uint32_t seed, int batches) {
    World w;
    InventoryState s(w.catalog);
    REQUIRE(s.ApplyAll({CreateContainer{ContainerId(1), w.Rules()}, CreateContainer{ContainerId(2), w.Rules(6, 2000, 400)},
                        CreateContainer{ContainerId(3), w.Rules(std::nullopt, 800)}}).HasValue());
    Lcg rng(seed);
    Run run;
    for (int step = 0; step < batches; ++step) {
        std::vector<InventoryCommand> cmds;
        const std::uint32_t roll = rng.Below(20);
        const std::uint32_t n = roll < 11 ? 1 : roll < 16 ? 2 : roll < 19 ? 3 : 4; // mostly short batches, so that many of them succeed
        for (std::uint32_t i = 0; i < n; ++i) cmds.push_back(RandomCommand(w, s, rng));
        const InventoryState snapshot = s;
        const auto result = s.ApplyAll(cmds);
        std::ostringstream line;
        line << step << ' ';
        if (result.HasValue()) {
            ++run.accepted;
            line << "OK";
            for (const auto& c : cmds) ++run.acceptedByType[c.index()];
            // the same commands one at a time on a copy reach the same state (only the revision differs)
            InventoryState single = snapshot;
            for (const auto& c : cmds) REQUIRE(single.Apply(c).HasValue());
            CHECK(single.Serialize().substr(single.Serialize().find("\ncontainers")) == s.Serialize().substr(s.Serialize().find("\ncontainers")));
            CHECK(single.NextInstanceId() == s.NextInstanceId());
            CHECK(s.Revision() == snapshot.Revision() + 1);
        } else {
            ++run.rejected;
            line << 'E' << static_cast<int>(result.Error().code) << '@' << result.Error().commandIndex;
            CHECK(s == snapshot); // not one field, id counter or revision changed
            CHECK(s.Serialize() == snapshot.Serialize());
            if (cmds.size() > 1) ++run.multiCommandRejected;
            if (result.Error().commandIndex > 0) ++run.rolledBackAfterProgress; // earlier commands had already been applied and are undone
            // the failing command alone fails the same way on the state its predecessors produced
            InventoryState prefix = snapshot;
            for (std::size_t i = 0; i < result.Error().commandIndex; ++i) REQUIRE(prefix.Apply(cmds[i]).HasValue());
            const auto again = prefix.Apply(cmds[result.Error().commandIndex]);
            REQUIRE_FALSE(again.HasValue());
            CHECK(again.Error().code == result.Error().code);
        }
        line << ' ' << Hex8(Crc32(s.Serialize())) << " r" << s.Revision() << " n" << s.NextInstanceId().Value() << '\n';
        run.transcript += line.str();
        if (const auto bad = s.CheckInvariants()) FAIL("invariant broken at batch " << step << ": " << ToString(bad->code) << " " << bad->detail);
    }
    return run;
}

constexpr std::uint32_t kGoldenSeed = 20261029;
constexpr int kGoldenBatches = 3000;

} // namespace

TEST_CASE("transactions: a recorded transcript of 3000 random batches is reproduced exactly (behaviour preserved across implementations)") {
    const Run run = Play(kGoldenSeed, kGoldenBatches);
    CHECK(run.accepted > 300);
    CHECK(run.rejected > 600);
    CHECK(run.multiCommandRejected > 100);
    for (std::size_t t = 0; t < run.acceptedByType.size(); ++t) CHECK_MESSAGE(run.acceptedByType[t] >= 8, "command type ", t, " is exercised successfully");
    CHECK(run.rolledBackAfterProgress > 100); // the interesting case: a later command fails after earlier ones changed the state
    if (const std::string out = EnvOrEmpty("SPN_WRITE_INVENTORY_GOLDEN"); !out.empty()) {
        std::ofstream(out, std::ios::binary) << run.transcript; // maintenance hook: re-record deliberately, never to make a failing test pass
        return;
    }
    std::ifstream in(std::string(SPN_TEST_DATA_DIR) + "/inventory_tx_golden.txt", std::ios::binary);
    REQUIRE_MESSAGE(in.is_open(), "golden transcript missing");
    const std::string golden((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    REQUIRE(golden.size() > 1000);
    CHECK(run.transcript == golden);
    if (run.transcript != golden) { // pinpoint the first differing line
        std::istringstream a(run.transcript), b(golden);
        std::string la, lb;
        while (std::getline(a, la) && std::getline(b, lb)) {
            if (la != lb) FAIL_CHECK("first difference: got '" << la << "' expected '" << lb << "'");
            if (la != lb) break;
        }
    }
}

TEST_CASE("transactions: other seeds also keep every invariant, rollback and batch-equals-single property") {
    for (const std::uint32_t seed : {1u, 7u, 99u, 12345u}) {
        const Run run = Play(seed, 800);
        CHECK(run.accepted > 40);
        CHECK(run.rejected > 100);
    }
}

TEST_CASE("transactions: rollback restores every kind of change made earlier in the same batch") {
    World w;
    InventoryState s(w.catalog);
    const ContainerId a(1), b(2);
    REQUIRE(s.ApplyAll({CreateContainer{a, w.Rules()}, CreateContainer{b, w.Rules(2)}, AddItem{a, w.coin, 25, {}}, AddItem{a, w.knife, 1, {}},
                        AddItem{a, w.coat, 1, {}}, AddItem{b, w.bandage, 3, {}}}).HasValue());
    const ItemInstanceId knife = s.FindContainer(a)->stacks.rbegin()->first; // coat is last
    ItemInstanceId coat = knife, knifeId;
    for (const auto& [id, st] : s.FindContainer(a)->stacks) {
        if (st.definition == w.knife) knifeId = id;
        if (st.definition == w.coat) coat = id;
    }
    REQUIRE(s.ApplyAll({EquipItem{a, knifeId, w.hand}}).HasValue());
    const InventoryState before = s;

    // Each prefix touches something different, then the last command fails; everything must be undone.
    const std::vector<std::vector<InventoryCommand>> batches = {
        {AddItem{a, w.coin, 7, {}}, RemoveItem{ContainerId(77), knifeId, 1}},                                  // quantity changes + new stack, then unknown container
        {RemoveItem{a, coat, 1}, UnequipSlot{a, w.hand}, SplitStack{a, ItemInstanceId(999), 1}},               // erased stack + erased equipment, then unknown item
        {TransferItem{a, b, coat, 1}, TransferItem{a, b, knifeId, 1}},                                          // moves a unique item, then equipped item / stack limit
        {SplitStack{a, ItemInstanceId(1), 3}, MergeStacks{a, ItemInstanceId(1), ItemInstanceId(2), 0}, AddItem{b, w.knife, 1, {}}, AddItem{b, w.knife, 1, {}}}, // capacity of b
        {CreateContainer{ContainerId(9), w.Rules()}, AddItem{ContainerId(9), w.coin, 3, {}}, DestroyContainer{ContainerId(9)}, DestroyContainer{ContainerId(9)}},
        {DestroyContainer{b}, DestroyContainer{a}},                                                              // destroys b's... b is not empty: fails at once
        {UnequipSlot{a, w.hand}, EquipItem{a, coat, w.torso}, EquipItem{a, coat, w.torso}},                    // equipment changes, then AlreadyEquipped
        {AddItem{a, w.brick, 4, {}}},                                                                            // arithmetic overflow in the capacity check
    };
    for (std::size_t i = 0; i < batches.size(); ++i) {
        INFO("batch " << i);
        const auto r = s.ApplyAll(batches[i]);
        REQUIRE_FALSE(r.HasValue());
        CHECK(s == before);
        CHECK(s.Serialize() == before.Serialize());
        CHECK_FALSE(s.CheckInvariants().has_value());
        CHECK(s.TotalWeightGrams(a) == before.TotalWeightGrams(a));
        CHECK(s.TotalVolumeMilliliters(a) == before.TotalVolumeMilliliters(a));
    }
    // and the state is fully usable afterwards: ids continue where they were, a valid batch commits
    const auto ok = s.ApplyAll({AddItem{a, w.knife, 1, {}}});
    REQUIRE(ok.HasValue());
    CHECK(ok.Value().createdStacks.front() == before.NextInstanceId());
}

TEST_CASE("transactions: instance ids are never reused, also not after a rolled-back batch") {
    World w;
    InventoryState s(w.catalog);
    REQUIRE(s.ApplyAll({CreateContainer{ContainerId(1), w.Rules()}}).HasValue());
    std::set<ItemInstanceId> seen;
    Lcg rng(5);
    for (int i = 0; i < 400; ++i) {
        std::vector<InventoryCommand> cmds{AddItem{ContainerId(1), w.knife, 1, {}}};
        if (rng.Below(3) == 0) cmds.push_back(RemoveItem{ContainerId(42), ItemInstanceId(1), 1}); // makes the batch fail after the add
        const auto r = s.ApplyAll(cmds);
        if (r.HasValue())
            for (const auto id : r.Value().createdStacks) CHECK(seen.insert(id).second);
        if (r.HasValue() && rng.Below(2) == 0) REQUIRE(s.ApplyAll({RemoveItem{ContainerId(1), r.Value().createdStacks.front(), 1}}).HasValue());
    }
    CHECK(seen.size() > 200);
    CHECK_FALSE(s.CheckInvariants().has_value());
}

TEST_CASE("transactions: cached totals always equal the recomputed sums (weight and volume)") {
    // Public view of the cached totals: TotalWeightGrams / TotalVolumeMilliliters must equal the sum over the stacks after every batch.
    World w;
    InventoryState s(w.catalog);
    REQUIRE(s.ApplyAll({CreateContainer{ContainerId(1), w.Rules()}, CreateContainer{ContainerId(2), w.Rules(std::nullopt, 5000, 1000)}}).HasValue());
    Lcg rng(77);
    for (int step = 0; step < 1500; ++step) {
        const auto r = s.ApplyAll({RandomCommand(w, s, rng), RandomCommand(w, s, rng)});
        (void)r;
        for (const auto& [cid, c] : s.Containers()) {
            std::uint64_t weight = 0, volume = 0;
            for (const auto& [sid, st] : c.stacks) {
                weight += w.catalog.Find(st.definition)->weightGrams * st.quantity;
                volume += w.catalog.Find(st.definition)->volumeMilliliters * st.quantity;
            }
            if (weight < (1ull << 60)) { // the brick can legitimately exceed what a plain sum can show; skip those states
                CHECK(s.TotalWeightGrams(cid) == weight);
                CHECK(s.TotalVolumeMilliliters(cid) == volume);
            }
        }
    }
}
