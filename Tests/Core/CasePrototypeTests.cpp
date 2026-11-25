#include <doctest.h>
#include <spn/core/CasePrototype.h>
#include <spn/core/WorldFacts.h>

using namespace spn::core;

// PROTOTYPE-CASE-01 placeholder rules (see CasePrototype.h). Tests cover the state
// machine, idempotence, refusals, the journal and save/load through WorldFacts.

namespace {

std::uint64_t Rev(const WorldFacts& facts) { return facts.Revision(); }

} // namespace

TEST_CASE("CasePrototype: an empty case is open and cannot be closed") {
    WorldFacts facts;
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Open);
    const auto before = Rev(facts);
    const auto closed = CloseCaseProto(facts);
    REQUIRE_FALSE(closed.HasValue());
    CHECK(closed.Error().code == CaseProtoErrorCode::NotReady);
    CHECK(Rev(facts) == before); // a refusal writes nothing
}

TEST_CASE("CasePrototype: the witness alone or the evidence alone is not enough") {
    WorldFacts witnessOnly;
    REQUIRE(InterviewCaseProtoWitness(witnessOnly).HasValue());
    CHECK(ReadCaseProtoStatus(witnessOnly).Value() == CaseProtoStatus::Open);
    CHECK_FALSE(CloseCaseProto(witnessOnly).HasValue());

    WorldFacts evidenceOnly;
    REQUIRE(FindCaseProtoEvidence(evidenceOnly).HasValue());
    CHECK(ReadCaseProtoStatus(evidenceOnly).Value() == CaseProtoStatus::Open);
    CHECK_FALSE(CloseCaseProto(evidenceOnly).HasValue());
}

TEST_CASE("CasePrototype: witness and evidence together make the case ready, then closure resolves it") {
    WorldFacts facts;
    CHECK(InterviewCaseProtoWitness(facts).Value());
    CHECK(FindCaseProtoEvidence(facts).Value());
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Ready);

    const auto closed = CloseCaseProto(facts);
    REQUIRE(closed.HasValue());
    CHECK(closed.Value() == CaseProtoStatus::Resolved);
    CHECK(ReadCaseProtoStatus(facts).Value() == CaseProtoStatus::Resolved);
    CHECK(facts.FindAs<bool>(CaseProtoResolvedKey()) != nullptr);
}

TEST_CASE("CasePrototype: repeated steps write nothing and are reported as unchanged") {
    WorldFacts facts;
    CHECK(InterviewCaseProtoWitness(facts).Value());
    const auto after = Rev(facts);
    CHECK_FALSE(InterviewCaseProtoWitness(facts).Value());
    CHECK(Rev(facts) == after);
    CHECK(FindCaseProtoEvidence(facts).Value());
    const auto afterEvidence = Rev(facts);
    CHECK_FALSE(FindCaseProtoEvidence(facts).Value());
    CHECK(Rev(facts) == afterEvidence);
}

TEST_CASE("CasePrototype: a resolved case refuses every further change") {
    WorldFacts facts;
    REQUIRE(InterviewCaseProtoWitness(facts).HasValue());
    REQUIRE(FindCaseProtoEvidence(facts).HasValue());
    REQUIRE(CloseCaseProto(facts).HasValue());
    const auto before = Rev(facts);

    const auto again = CloseCaseProto(facts);
    REQUIRE_FALSE(again.HasValue());
    CHECK(again.Error().code == CaseProtoErrorCode::AlreadyResolved);
    CHECK(InterviewCaseProtoWitness(facts).Error().code == CaseProtoErrorCode::AlreadyResolved);
    CHECK(FindCaseProtoEvidence(facts).Error().code == CaseProtoErrorCode::AlreadyResolved);
    CHECK(Rev(facts) == before);
}

TEST_CASE("CasePrototype: the journal records exactly the three placeholder changes in order") {
    WorldFacts facts;
    REQUIRE(FindCaseProtoEvidence(facts).HasValue());
    REQUIRE(InterviewCaseProtoWitness(facts).HasValue());
    REQUIRE(CloseCaseProto(facts).HasValue());
    REQUIRE(facts.Journal().size() == 3);
    CHECK(std::get<SetFact>(facts.Journal()[0].command).key == CaseProtoEvidenceKey());
    CHECK(std::get<SetFact>(facts.Journal()[1].command).key == CaseProtoWitnessKey());
    CHECK(std::get<SetFact>(facts.Journal()[2].command).key == CaseProtoResolvedKey());
}

TEST_CASE("CasePrototype: a wrong-typed case fact is reported as state corruption") {
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{std::string(CaseProtoWitnessKey()), std::string("yes")}).HasValue());
    const auto status = ReadCaseProtoStatus(facts);
    REQUIRE_FALSE(status.HasValue());
    CHECK(status.Error().code == CaseProtoErrorCode::StateCorrupt);
    CHECK_FALSE(CloseCaseProto(facts).HasValue());
}

TEST_CASE("CasePrototype: the state survives a save and load round trip") {
    WorldFacts facts;
    REQUIRE(InterviewCaseProtoWitness(facts).HasValue());
    REQUIRE(FindCaseProtoEvidence(facts).HasValue());
    REQUIRE(CloseCaseProto(facts).HasValue());

    const auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(ReadCaseProtoStatus(loaded.Value()).Value() == CaseProtoStatus::Resolved);
    CHECK(loaded.Value().Revision() == facts.Revision());
}

TEST_CASE("CasePrototype: keys are valid fact keys and the status names are stable") {
    CHECK(IsValidFactKey(CaseProtoWitnessKey()));
    CHECK(IsValidFactKey(CaseProtoEvidenceKey()));
    CHECK(IsValidFactKey(CaseProtoResolvedKey()));
    CHECK(ToString(CaseProtoStatus::Open) == "open");
    CHECK(ToString(CaseProtoStatus::Ready) == "ready");
    CHECK(ToString(CaseProtoStatus::Resolved) == "resolved");
    CHECK(ToString(CaseProtoErrorCode::NotReady) == "not_ready");
}
