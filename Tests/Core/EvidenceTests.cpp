#include <doctest.h>
#include <spn/core/Evidence.h>
#include <spn/core/WorldFacts.h>

using namespace spn::core;

// Evidence stages (EVD-01): known, found, registered. Registration only after collecting.

namespace {

EvidenceCatalog Catalog() {
    auto made = EvidenceCatalog::Create({{"note", "general"}, {"photo", "general"}});
    REQUIRE(made.HasValue());
    return std::move(made).Value();
}

} // namespace

TEST_CASE("Evidence catalog: validation of ids, categories and duplicates") {
    CHECK(EvidenceCatalog::Create({}).Error() == EvidenceCatalog::Error::Empty);
    CHECK(EvidenceCatalog::Create({{"Bad", "general"}}).Error() == EvidenceCatalog::Error::InvalidId);
    CHECK(EvidenceCatalog::Create({{"note", "Bad.Cat"}}).Error() == EvidenceCatalog::Error::InvalidCategory);
    CHECK(EvidenceCatalog::Create({{"note", "general"}, {"note", "general"}}).Error() == EvidenceCatalog::Error::Duplicate);
    CHECK(EvidenceCatalog::Create({{"note", "general"}}).HasValue());
}

TEST_CASE("Evidence: the stages run known, found, registered in this order") {
    const auto catalog = Catalog();
    WorldFacts facts;
    CHECK(ReadEvidenceStage(facts, catalog, "note").Value() == EvidenceStage::Unknown);
    CHECK(LearnEvidence(facts, catalog, "note").Value());
    CHECK(ReadEvidenceStage(facts, catalog, "note").Value() == EvidenceStage::Known);
    CHECK(FindEvidence(facts, catalog, "note").Value());
    CHECK(ReadEvidenceStage(facts, catalog, "note").Value() == EvidenceStage::Found);
    CHECK(CollectEvidence(facts, catalog, "note").Value());
    CHECK(ReadEvidenceStage(facts, catalog, "note").Value() == EvidenceStage::Registered);
}

TEST_CASE("Evidence: collecting before finding is refused and writes nothing") {
    const auto catalog = Catalog();
    WorldFacts facts;
    REQUIRE(LearnEvidence(facts, catalog, "note").HasValue());
    const auto before = facts.Revision();
    const auto collected = CollectEvidence(facts, catalog, "note");
    REQUIRE_FALSE(collected.HasValue());
    CHECK(collected.Error().code == EvidenceErrorCode::NotFound);
    CHECK(facts.Revision() == before);
    CHECK(RegisteredEvidence(facts, catalog).Value().empty());
}

TEST_CASE("Evidence: known and found alone never register the journal entry") {
    const auto catalog = Catalog();
    WorldFacts facts;
    REQUIRE(LearnEvidence(facts, catalog, "photo").HasValue());
    CHECK(facts.Find(EvidenceJournalKey("photo")) == nullptr);
    REQUIRE(FindEvidence(facts, catalog, "photo").HasValue());
    CHECK(facts.Find(EvidenceJournalKey("photo")) == nullptr);
    CHECK(ReadEvidenceStage(facts, catalog, "photo").Value() == EvidenceStage::Found);
}

TEST_CASE("Evidence: no second registration, and no change after registration") {
    const auto catalog = Catalog();
    WorldFacts facts;
    REQUIRE(LearnEvidence(facts, catalog, "note").HasValue());
    REQUIRE(FindEvidence(facts, catalog, "note").HasValue());
    REQUIRE(CollectEvidence(facts, catalog, "note").HasValue());
    const auto before = facts.Revision();

    const auto again = CollectEvidence(facts, catalog, "note");
    REQUIRE_FALSE(again.HasValue());
    CHECK(again.Error().code == EvidenceErrorCode::AlreadyRegistered);
    CHECK(FindEvidence(facts, catalog, "note").Error().code == EvidenceErrorCode::AlreadyRegistered);
    CHECK_FALSE(LearnEvidence(facts, catalog, "note").Value()); // already known: nothing written
    CHECK(facts.Revision() == before);
}

TEST_CASE("Evidence: invalid and unknown ids are rejected") {
    const auto catalog = Catalog();
    WorldFacts facts;
    CHECK(LearnEvidence(facts, catalog, "Bad Id").Error().code == EvidenceErrorCode::InvalidId);
    CHECK(LearnEvidence(facts, catalog, "").Error().code == EvidenceErrorCode::InvalidId);
    CHECK(FindEvidence(facts, catalog, "ghost").Error().code == EvidenceErrorCode::UnknownEvidence);
    CHECK(ReadEvidenceStage(facts, catalog, "ghost").Error().code == EvidenceErrorCode::UnknownEvidence);
    CHECK(facts.Revision() == 0);
}

TEST_CASE("Evidence: registered list is in catalog order and survives save and load") {
    const auto catalog = Catalog();
    WorldFacts facts;
    REQUIRE(FindEvidence(facts, catalog, "photo").HasValue());
    REQUIRE(CollectEvidence(facts, catalog, "photo").HasValue());
    REQUIRE(FindEvidence(facts, catalog, "note").HasValue());
    REQUIRE(CollectEvidence(facts, catalog, "note").HasValue());
    CHECK(RegisteredEvidence(facts, catalog).Value() == std::vector<std::string>{"note", "photo"});

    const auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(RegisteredEvidence(loaded.Value(), catalog).Value() == std::vector<std::string>{"note", "photo"});
}

TEST_CASE("Evidence: a wrong-typed stored fact is reported as state corruption") {
    const auto catalog = Catalog();
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{EvidenceFoundKey("note"), std::string("yes")}).HasValue());
    CHECK(ReadEvidenceStage(facts, catalog, "note").Error().code == EvidenceErrorCode::StateCorrupt);
    CHECK(CollectEvidence(facts, catalog, "note").Error().code == EvidenceErrorCode::StateCorrupt);
}

TEST_CASE("Evidence: fact keys follow the documented contract") {
    CHECK(EvidenceKnownKey("note") == "placeholder.evidence.note.known");
    CHECK(EvidenceFoundKey("note") == "placeholder.evidence.note.found");
    CHECK(EvidenceJournalKey("note") == "placeholder.journal.evidence.note");
    CHECK(ToString(EvidenceStage::Registered) == "registered");
    CHECK(ToString(EvidenceErrorCode::NotFound) == "not_found");
}
