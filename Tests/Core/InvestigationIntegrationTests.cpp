#include <doctest.h>
#include <spn/core/CasePrototype.h>
#include <spn/core/Dialogue.h>
#include <spn/core/Evidence.h>
#include <spn/core/WorldFacts.h>

using namespace spn::core;

// Integration of evidence, dialogue and the CasePrototype placeholder (SPN-DEV-02 section 6).
// The case rule stays placeholder (witness AND evidence); nothing here decides case content or
// automatic closure (EVD-04, conflict K-35).

namespace {

DialogueDef WitnessDialogue() {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.start";
    DialogueOption leave;
    leave.id = "leave";
    leave.textKey = "placeholder.text.leave";
    start.options = {leave};
    auto made = DialogueDef::Create("witness_talk", "witness", "start", {start});
    REQUIRE(made.HasValue());
    return std::move(made).Value();
}

EvidenceCatalog Catalog() {
    auto made = EvidenceCatalog::Create({{"note", "general"}});
    REQUIRE(made.HasValue());
    return std::move(made).Value();
}

} // namespace

TEST_CASE("Integration: adoption sets nothing before real progress exists") {
    WorldFacts facts;
    REQUIRE(CloseCaseProto(facts).Error().code == CaseProtoErrorCode::NotReady);
    CHECK_FALSE(AdoptInvestigationProgress(facts, "note", "witness_talk").Value());
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Open);
}

TEST_CASE("Integration: a learned clue alone never makes the case ready (no automatic progress)") {
    const auto catalog = Catalog();
    WorldFacts facts;
    REQUIRE(LearnEvidence(facts, catalog, "note").HasValue());
    CHECK_FALSE(AdoptInvestigationProgress(facts, "note", "witness_talk").Value());
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Open);
}

TEST_CASE("Integration: registered evidence and a finished witness dialogue make the case ready, closure stays manual") {
    const auto catalog = Catalog();
    const auto dialogue = WitnessDialogue();
    WorldFacts facts;

    REQUIRE(FindEvidence(facts, catalog, "note").HasValue());
    REQUIRE(CollectEvidence(facts, catalog, "note").HasValue());
    REQUIRE(DialogueStart(dialogue, facts).HasValue());
    REQUIRE(DialogueChoose(dialogue, facts, "leave").HasValue());

    CHECK(AdoptInvestigationProgress(facts, "note", "witness_talk").Value());
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Ready);
    // EVD-04: readiness is not resolution. Closing is an explicit step.
    CHECK(ReadCaseProtoStatus(facts).Value() != CaseProtoStatus::Resolved);
    CHECK(CloseCaseProto(facts).Value() == CaseProtoStatus::Resolved);
}

TEST_CASE("Integration: adoption is idempotent and the journal fact is the only evidence source") {
    const auto catalog = Catalog();
    const auto dialogue = WitnessDialogue();
    WorldFacts facts;
    REQUIRE(FindEvidence(facts, catalog, "note").HasValue());
    REQUIRE(CollectEvidence(facts, catalog, "note").HasValue());
    REQUIRE(DialogueStart(dialogue, facts).HasValue());
    REQUIRE(DialogueChoose(dialogue, facts, "leave").HasValue());
    REQUIRE(AdoptInvestigationProgress(facts, "note", "witness_talk").HasValue());
    const auto before = facts.Revision();
    CHECK_FALSE(AdoptInvestigationProgress(facts, "note", "witness_talk").Value());
    CHECK(facts.Revision() == before);
}

TEST_CASE("Integration: a found but not collected piece of evidence does not count") {
    const auto catalog = Catalog();
    const auto dialogue = WitnessDialogue();
    WorldFacts facts;
    REQUIRE(FindEvidence(facts, catalog, "note").HasValue());
    REQUIRE(DialogueStart(dialogue, facts).HasValue());
    REQUIRE(DialogueChoose(dialogue, facts, "leave").HasValue());
    CHECK(AdoptInvestigationProgress(facts, "note", "witness_talk").Value()); // witness flag only
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Open);
}

// --- SPN-DEV-03: unlocks by evidence, invalid input, and the prototype flow -------------------

namespace {

DialogueDef UnlockDialogue() {
    DialogueNode start;
    start.id = "start";
    start.speaker = "witness";
    start.textKey = "placeholder.text.start";
    DialogueOption leave;
    leave.id = "leave";
    leave.textKey = "placeholder.text.leave";
    DialogueOption showNote;
    showNote.id = "show_note";
    showNote.textKey = "placeholder.text.show_note";
    showNote.conditions.push_back({DialogueCondition::Kind::FactTrue, "placeholder.journal.evidence.note", false});
    showNote.next = std::nullopt;
    start.options = {showNote, leave};
    auto made = DialogueDef::Create("witness_talk", "witness", "start", {start});
    REQUIRE(made.HasValue());
    return std::move(made).Value();
}

} // namespace

TEST_CASE("Integration: a registered evidence unlocks a dialogue option only after collecting, not after finding") {
    const auto catalog = Catalog();
    const auto dialogue = UnlockDialogue();
    WorldFacts facts;
    REQUIRE(DialogueStart(dialogue, facts).HasValue());
    REQUIRE(FindEvidence(facts, catalog, "note").HasValue());
    CHECK(DialogueCurrent(dialogue, facts).Value().availableOptions == std::vector<std::string>{"leave"});
    REQUIRE(CollectEvidence(facts, catalog, "note").HasValue());
    CHECK(DialogueCurrent(dialogue, facts).Value().availableOptions == std::vector<std::string>{"show_note", "leave"});
}

TEST_CASE("Integration: invalid ids are refused by the adoption and change nothing") {
    WorldFacts facts;
    CHECK(AdoptInvestigationProgress(facts, "Bad Id", "witness_talk").Error().code == CaseProtoErrorCode::InvalidId);
    CHECK(AdoptInvestigationProgress(facts, "note", "Bad.Dialogue").Error().code == CaseProtoErrorCode::InvalidId);
    CHECK(facts.Revision() == 0);
}

TEST_CASE("Integration: the prototype flow runs end to end, survives save and load, and closes only on request") {
    const auto catalog = Catalog();
    const auto dialogue = WitnessDialogue();
    WorldFacts facts;

    // Clue, then the room and the evidence, then the witness conversation.
    REQUIRE(LearnEvidence(facts, catalog, "note", "clue").HasValue());
    REQUIRE(FindEvidence(facts, catalog, "note", "room").HasValue());
    REQUIRE(CollectEvidence(facts, catalog, "note", "player").HasValue());
    REQUIRE(DialogueStart(dialogue, facts).HasValue());

    // Save and load mid-way.
    auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    WorldFacts store = std::move(loaded).Value();
    CHECK(RegisteredEvidence(store, catalog).Value() == std::vector<std::string>{"note"});

    REQUIRE(DialogueChoose(dialogue, store, "leave").HasValue());
    CHECK(AdoptInvestigationProgress(store, "note", "witness_talk").Value());
    CHECK(ReadCaseProtoStatus(store).Value() == CaseProtoStatus::Ready);
    CHECK(ReadCaseProtoStatus(store).Value() != CaseProtoStatus::Resolved); // EVD-05: no automatic closure

    // A second save and load keeps the state, and only the explicit closure resolves the case.
    auto again = WorldFacts::Deserialize(store.Serialize());
    REQUIRE(again.HasValue());
    WorldFacts final = std::move(again).Value();
    CHECK(ReadCaseProtoStatus(final).Value() == CaseProtoStatus::Ready);
    CHECK(CloseCaseProto(final).Value() == CaseProtoStatus::Resolved);
    CHECK(CloseCaseProto(final).Error().code == CaseProtoErrorCode::AlreadyResolved);
}
