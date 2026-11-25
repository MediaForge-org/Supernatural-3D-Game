#include <doctest.h>
#include <spn/core/Dialogue.h>
#include <spn/core/WorldFacts.h>

using namespace spn::core;

// Dialogue core (DLG-02, NPCA-02): options, conditions, effects, transitions, persistence.

namespace {

DialogueOption Option(std::string id, std::string text, std::optional<std::string> next) {
    DialogueOption option;
    option.id = std::move(id);
    option.textKey = std::move(text);
    option.next = std::move(next);
    return option;
}

// witness_talk: start -> (ask_room | ask_evidence if the note is known | leave)
DialogueDef WitnessTalk() {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.witness_start";
    DialogueOption askRoom = Option("ask_room", "placeholder.text.ask_room", "room");
    askRoom.effects.push_back({"npc.witness.knows.room", true});
    DialogueOption askEvidence = Option("ask_evidence", "placeholder.text.ask_evidence", "evidence");
    askEvidence.conditions.push_back({DialogueCondition::Kind::FactTrue, "placeholder.evidence.note.known", false});
    start.options = {askRoom, askEvidence, Option("leave", "placeholder.text.leave", std::nullopt)};

    DialogueNode room;
    room.id = "room";
    room.speaker = "witness";
    room.textKey = "placeholder.text.room";
    room.options = {Option("back", "placeholder.text.back", "start")};

    DialogueNode evidence;
    evidence.id = "evidence";
    evidence.speaker = "witness";
    evidence.textKey = "placeholder.text.evidence";
    DialogueOption thanks = Option("thanks", "placeholder.text.thanks", std::nullopt);
    thanks.effects.push_back({"npc.witness.knows.evidence", true});
    evidence.options = {thanks};

    auto made = DialogueDef::Create("witness_talk", "witness", "start", {start, room, evidence});
    REQUIRE(made.HasValue());
    return std::move(made).Value();
}

std::vector<std::string> Ids(const DialogueView& view) { return view.availableOptions; }

} // namespace

TEST_CASE("Dialogue definition: invalid graphs are refused") {
    DialogueNode node;
    node.id = "start";
    node.speaker = "witness";
    node.textKey = "placeholder.text.a";
    CHECK(DialogueDef::Create("Bad", "witness", "start", {node}).Error() == DialogueDefError::InvalidId);
    CHECK(DialogueDef::Create("d", "witness", "start", {}).Error() == DialogueDefError::EmptyGraph);
    CHECK(DialogueDef::Create("d", "witness", "missing", {node}).Error() == DialogueDefError::UnknownStart);
    DialogueNode dup = node;
    CHECK(DialogueDef::Create("d", "witness", "start", {node, dup}).Error() == DialogueDefError::DuplicateNode);
    DialogueNode bad = node;
    bad.textKey = "Bad Key";
    CHECK(DialogueDef::Create("d", "witness", "start", {bad}).Error() == DialogueDefError::InvalidTextKey);
    DialogueNode dupOpt = node;
    dupOpt.options = {Option("o", "placeholder.text.o", std::nullopt), Option("o", "placeholder.text.p", std::nullopt)};
    CHECK(DialogueDef::Create("d", "witness", "start", {dupOpt}).Error() == DialogueDefError::DuplicateOption);
    DialogueNode toMissing = node;
    toMissing.options = {Option("o", "placeholder.text.o", "nowhere")};
    CHECK(DialogueDef::Create("d", "witness", "start", {toMissing}).Error() == DialogueDefError::UnknownNext);
    DialogueNode badEffect = node;
    badEffect.options = {Option("o", "placeholder.text.o", std::nullopt)};
    badEffect.options[0].effects.push_back({"Bad.Key!", true});
    CHECK(DialogueDef::Create("d", "witness", "start", {badEffect}).Error() == DialogueDefError::InvalidFactKey);
    CHECK(DialogueDef::Create("d", "witness", "start", {node}).HasValue());
}

TEST_CASE("Dialogue: the view before the note is known hides the evidence option") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    CHECK(DialogueCurrent(def, facts).Error().code == DialogueErrorCode::NotStarted);
    const auto started = DialogueStart(def, facts);
    REQUIRE(started.HasValue());
    CHECK(started.Value().nodeId == "start");
    CHECK(Ids(started.Value()) == std::vector<std::string>{"ask_room", "leave"});
    CHECK_FALSE(started.Value().ended);
}

TEST_CASE("Dialogue: starting twice writes nothing the second time") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(DialogueStart(def, facts).HasValue());
    const auto before = facts.Revision();
    const auto again = DialogueStart(def, facts);
    REQUIRE(again.HasValue());
    CHECK(again.Value().nodeId == "start");
    CHECK(facts.Revision() == before);
}

TEST_CASE("Dialogue: a choice moves to the next node and applies its effect") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(DialogueStart(def, facts).HasValue());
    const auto after = DialogueChoose(def, facts, "ask_room");
    REQUIRE(after.HasValue());
    CHECK(after.Value().nodeId == "room");
    CHECK(facts.FindAs<bool>("npc.witness.knows.room") != nullptr);
    CHECK(*facts.FindAs<bool>("npc.witness.knows.room"));
    const auto back = DialogueChoose(def, facts, "back");
    REQUIRE(back.HasValue());
    CHECK(back.Value().nodeId == "start");
}

TEST_CASE("Dialogue: an unavailable option is refused and the facts stay unchanged") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(DialogueStart(def, facts).HasValue());
    const auto before = facts.Revision();
    const auto refused = DialogueChoose(def, facts, "ask_evidence");
    REQUIRE_FALSE(refused.HasValue());
    CHECK(refused.Error().code == DialogueErrorCode::OptionUnavailable);
    CHECK(facts.Revision() == before);
}

TEST_CASE("Dialogue: the evidence option appears once the note is known") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{"placeholder.evidence.note.known", true}).HasValue());
    const auto view = DialogueStart(def, facts);
    REQUIRE(view.HasValue());
    CHECK(Ids(view.Value()) == std::vector<std::string>{"ask_room", "ask_evidence", "leave"});
    REQUIRE(DialogueChoose(def, facts, "ask_evidence").HasValue());
    CHECK(DialogueChoose(def, facts, "thanks").HasValue());
    CHECK(*facts.FindAs<bool>("npc.witness.knows.evidence"));
}

TEST_CASE("Dialogue: unknown and malformed option ids are rejected") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(DialogueStart(def, facts).HasValue());
    CHECK(DialogueChoose(def, facts, "nope").Error().code == DialogueErrorCode::UnknownOption);
    CHECK(DialogueChoose(def, facts, "Bad Id").Error().code == DialogueErrorCode::InvalidId);
}

TEST_CASE("Dialogue: ending the dialogue sets done, and nothing else is accepted afterwards") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(DialogueStart(def, facts).HasValue());
    const auto ended = DialogueChoose(def, facts, "leave");
    REQUIRE(ended.HasValue());
    CHECK(ended.Value().ended);
    CHECK(ended.Value().availableOptions.empty());
    CHECK(IsDialogueDone(facts, "witness_talk"));
    CHECK(DialogueChoose(def, facts, "ask_room").Error().code == DialogueErrorCode::AlreadyDone);
    CHECK(DialogueStart(def, facts).Error().code == DialogueErrorCode::AlreadyDone);
    CHECK(DialogueCurrent(def, facts).Value().ended);
}

TEST_CASE("Dialogue: a failing effect leaves the store and the node unchanged (atomic choice)") {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.a";
    DialogueOption huge = Option("big", "placeholder.text.b", std::nullopt);
    huge.effects.push_back({"placeholder.big.value", std::string(kMaxFactStringLength + 1, 'x')});
    start.options = {huge};
    auto made = DialogueDef::Create("big_talk", "witness", "start", {start});
    REQUIRE(made.HasValue());
    WorldFacts facts;
    REQUIRE(DialogueStart(made.Value(), facts).HasValue());
    const auto before = facts.Revision();
    const auto failed = DialogueChoose(made.Value(), facts, "big");
    REQUIRE_FALSE(failed.HasValue());
    CHECK(failed.Error().code == DialogueErrorCode::Store);
    CHECK(facts.Revision() == before);
    CHECK_FALSE(IsDialogueDone(facts, "big_talk"));
}

TEST_CASE("Dialogue: a conversation survives save and load mid-way") {
    const auto def = WitnessTalk();
    WorldFacts facts;
    REQUIRE(DialogueStart(def, facts).HasValue());
    REQUIRE(DialogueChoose(def, facts, "ask_room").HasValue());
    const auto view = DialogueCurrent(def, facts);
    REQUIRE(view.HasValue());

    const auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(DialogueCurrent(def, loaded.Value()).Value() == view.Value());
}

TEST_CASE("Dialogue: corrupt stored state is reported, not guessed") {
    const auto def = WitnessTalk();
    WorldFacts wrongType;
    REQUIRE(wrongType.Apply(SetFact{DialogueNodeKey("witness_talk"), true}).HasValue());
    CHECK(DialogueCurrent(def, wrongType).Error().code == DialogueErrorCode::StateCorrupt);

    WorldFacts missingNode;
    REQUIRE(missingNode.Apply(SetFact{DialogueNodeKey("witness_talk"), std::string("gone")}).HasValue());
    CHECK(DialogueCurrent(def, missingNode).Error().code == DialogueErrorCode::StateCorrupt);
}

TEST_CASE("Dialogue: NPC knowledge keys are validated and follow the convention") {
    CHECK(NpcKnowsKey("witness", "room").Value() == "npc.witness.knows.room");
    CHECK(NpcKnowsKey("Witness", "room").Error() == DialogueErrorCode::InvalidId);
    CHECK(NpcKnowsKey("witness", "").Error() == DialogueErrorCode::InvalidId);
}

TEST_CASE("Dialogue: equality conditions read exact fact values") {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.a";
    DialogueOption onlyWhenOne = Option("one", "placeholder.text.one", std::nullopt);
    onlyWhenOne.conditions.push_back({DialogueCondition::Kind::FactEquals, "placeholder.count", FactValue{std::int64_t{1}}});
    DialogueOption onlyWhenAbsent = Option("none", "placeholder.text.none", std::nullopt);
    onlyWhenAbsent.conditions.push_back({DialogueCondition::Kind::FactAbsent, "placeholder.flag", false});
    start.options = {onlyWhenOne, onlyWhenAbsent};
    auto made = DialogueDef::Create("eq_talk", "witness", "start", {start});
    REQUIRE(made.HasValue());
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{"placeholder.count", std::int64_t{2}}).HasValue());
    CHECK(Ids(DialogueStart(made.Value(), facts).Value()) == std::vector<std::string>{"none"});
    REQUIRE(facts.Apply(SetFact{"placeholder.count", std::int64_t{1}}).HasValue());
    CHECK(Ids(DialogueCurrent(made.Value(), facts).Value()) == std::vector<std::string>{"one", "none"});
}

// --- Regression tests for risks found in SPN-DEV-03 review -----------------------------------

namespace {

Result<DialogueDef, DialogueDefError> WithEffectKey(std::string key) {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.a";
    DialogueOption option = Option("o", "placeholder.text.o", std::nullopt);
    option.effects.push_back({std::move(key), true});
    start.options = {option};
    return DialogueDef::Create("d", "witness", "start", {start});
}

} // namespace

TEST_CASE("Regression: a dialogue effect cannot write another module's own state (journal, dialogue, evidence, assist, case)") {
    // Each of these would bypass EVD-01 (journal only after collecting) or the dialogue state machine.
    CHECK(WithEffectKey("placeholder.journal.evidence.note").Error() == DialogueDefError::InvalidFactKey);
    CHECK(WithEffectKey("placeholder.dialogue.other.done").Error() == DialogueDefError::InvalidFactKey);
    CHECK(WithEffectKey("placeholder.dialogue.d.node").Error() == DialogueDefError::InvalidFactKey);
    CHECK(WithEffectKey("placeholder.evidence.note.found").Error() == DialogueDefError::InvalidFactKey);
    CHECK(WithEffectKey("placeholder.assist.preset").Error() == DialogueDefError::InvalidFactKey);
    CHECK(WithEffectKey("placeholder.case.proto.resolved").Error() == DialogueDefError::InvalidFactKey);
    // Knowledge facts of NPCs stay allowed (NPCA-03 keeps them as ordinary facts).
    CHECK(WithEffectKey("npc.witness.knows.room").HasValue());
}

TEST_CASE("Regression: the NPC id 'player' is reserved for the player speaker") {
    DialogueNode node;
    node.id = "start";
    node.speaker = "player";
    node.textKey = "placeholder.text.a";
    CHECK(DialogueDef::Create("d", "player", "start", {node}).Error() == DialogueDefError::InvalidId);
    CHECK(DialogueDef::Create("d", "witness", "start", {node}).HasValue()); // the player may speak; a witness npc is fine
}

TEST_CASE("Regression: a choice with several effects is atomic (no earlier effect survives a later failure)") {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.a";
    DialogueOption twoEffects = Option("two", "placeholder.text.b", std::nullopt);
    twoEffects.effects.push_back({"npc.witness.knows.room", true});
    twoEffects.effects.push_back({"placeholder.big.value", std::string(kMaxFactStringLength + 1, 'x')});
    start.options = {twoEffects};
    auto made = DialogueDef::Create("two_talk", "witness", "start", {start});
    REQUIRE(made.HasValue());
    WorldFacts facts;
    REQUIRE(DialogueStart(made.Value(), facts).HasValue());
    const auto before = facts.Revision();
    REQUIRE_FALSE(DialogueChoose(made.Value(), facts, "two").HasValue());
    CHECK(facts.Find("npc.witness.knows.room") == nullptr); // the first effect was not kept
    CHECK(facts.Revision() == before);
    CHECK(DialogueCurrent(made.Value(), facts).Value().nodeId == "start");
}
