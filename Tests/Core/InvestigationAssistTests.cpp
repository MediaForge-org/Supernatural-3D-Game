#include <doctest.h>
#include <spn/core/Evidence.h>
#include <spn/core/InvestigationAssist.h>
#include <spn/core/WorldFacts.h>

using namespace spn::core;

// Investigation assistance (DIF-01..03): presets are data, overrides are independent, no
// knowledge changes, deterministic. Preset names here are test names, not game decisions.

namespace {

AssistPreset Preset(std::string id, bool marker, bool known, bool dialogue) {
    AssistPreset p;
    p.id = std::move(id);
    p.values[AssistOption::FoundMarker] = marker;
    p.values[AssistOption::KnownHint] = known;
    p.values[AssistOption::DialogueHint] = dialogue;
    return p;
}

AssistPresetTable Table() {
    auto made = AssistPresetTable::Create({Preset("preset_a", false, false, false), Preset("preset_b", true, true, false)});
    REQUIRE(made.HasValue());
    return std::move(made).Value();
}

} // namespace

TEST_CASE("Assist presets: validation of ids, duplicates and missing options") {
    CHECK(AssistPresetTable::Create({}).Error() == AssistPresetTable::Error::Empty);
    CHECK(AssistPresetTable::Create({Preset("Bad", true, true, true)}).Error() == AssistPresetTable::Error::InvalidId);
    CHECK(AssistPresetTable::Create({Preset("a", true, true, true), Preset("a", false, false, false)}).Error() ==
          AssistPresetTable::Error::Duplicate);
    AssistPreset partial;
    partial.id = "partial";
    partial.values[AssistOption::FoundMarker] = true;
    CHECK(AssistPresetTable::Create({partial}).Error() == AssistPresetTable::Error::MissingOption);
}

TEST_CASE("Assist: without a preset every option is off") {
    const auto table = Table();
    WorldFacts facts;
    CHECK(SelectedAssistPreset(facts).Value().empty());
    CHECK_FALSE(EffectiveAssist(facts, table, AssistOption::FoundMarker).Value());
    CHECK_FALSE(EffectiveAssist(facts, table, AssistOption::KnownHint).Value());
}

TEST_CASE("Assist: selecting a preset sets the effective values, and validates the id") {
    const auto table = Table();
    WorldFacts facts;
    CHECK(SelectAssistPreset(facts, table, "preset_b").Value());
    CHECK(EffectiveAssist(facts, table, AssistOption::FoundMarker).Value());
    CHECK(EffectiveAssist(facts, table, AssistOption::KnownHint).Value());
    CHECK_FALSE(EffectiveAssist(facts, table, AssistOption::DialogueHint).Value());
    CHECK(SelectAssistPreset(facts, table, "preset_b").Value() == false); // idempotent
    CHECK(SelectAssistPreset(facts, table, "ghost").Error().code == AssistErrorCode::UnknownPreset);
    CHECK(SelectAssistPreset(facts, table, "Bad Id").Error().code == AssistErrorCode::InvalidId);
}

TEST_CASE("Assist: an override is stored separately and wins over the preset") {
    const auto table = Table();
    WorldFacts facts;
    REQUIRE(SelectAssistPreset(facts, table, "preset_b").HasValue());
    CHECK(SetAssistOverride(facts, AssistOption::FoundMarker, false, "test").Value());
    CHECK_FALSE(EffectiveAssist(facts, table, AssistOption::FoundMarker).Value());
    CHECK(EffectiveAssist(facts, table, AssistOption::KnownHint).Value()); // other options still follow the preset
    CHECK(facts.FindAs<bool>(AssistOverrideKey(AssistOption::FoundMarker)) != nullptr);
}

TEST_CASE("Assist: changing the preset keeps the overrides, and clearing one returns it to the preset") {
    const auto table = Table();
    WorldFacts facts;
    REQUIRE(SelectAssistPreset(facts, table, "preset_b").HasValue());
    REQUIRE(SetAssistOverride(facts, AssistOption::FoundMarker, false).HasValue());
    REQUIRE(SelectAssistPreset(facts, table, "preset_a").HasValue());
    CHECK_FALSE(EffectiveAssist(facts, table, AssistOption::FoundMarker).Value()); // override kept
    CHECK(ClearAssistOverride(facts, AssistOption::FoundMarker).Value());
    CHECK_FALSE(EffectiveAssist(facts, table, AssistOption::FoundMarker).Value()); // now preset_a's value
    REQUIRE(SelectAssistPreset(facts, table, "preset_b").HasValue());
    CHECK(EffectiveAssist(facts, table, AssistOption::FoundMarker).Value());
    REQUIRE(SetAssistOverride(facts, AssistOption::FoundMarker, false).HasValue());
    CHECK(ClearAssistOverride(facts, AssistOption::FoundMarker).Value());
    CHECK_FALSE(ClearAssistOverride(facts, AssistOption::FoundMarker).Value()); // nothing left to clear
}

TEST_CASE("Assist: setting an unchanged override writes nothing") {
    WorldFacts facts;
    REQUIRE(SetAssistOverride(facts, AssistOption::KnownHint, true).HasValue());
    const auto before = facts.Revision();
    CHECK_FALSE(SetAssistOverride(facts, AssistOption::KnownHint, true).Value());
    CHECK(facts.Revision() == before);
}

TEST_CASE("Assist: assistance never changes what the character knows") {
    const auto table = Table();
    auto catalog = EvidenceCatalog::Create({{"note", "general"}});
    REQUIRE(catalog.HasValue());
    WorldFacts facts;
    REQUIRE(LearnEvidence(facts, catalog.Value(), "note").HasValue());
    REQUIRE(FindEvidence(facts, catalog.Value(), "note").HasValue());
    const auto knowledgeBefore = facts.Facts();

    REQUIRE(SelectAssistPreset(facts, table, "preset_b").HasValue());
    REQUIRE(SetAssistOverride(facts, AssistOption::DialogueHint, true).HasValue());
    REQUIRE(ClearAssistOverride(facts, AssistOption::FoundMarker).HasValue());

    // The only facts that changed are assist keys; the evidence stage is untouched.
    for (const auto& [key, value] : knowledgeBefore) {
        CHECK(facts.Find(key) != nullptr);
        CHECK(*facts.Find(key) == value);
    }
    CHECK(ReadEvidenceStage(facts, catalog.Value(), "note").Value() == EvidenceStage::Found);
}

TEST_CASE("Assist: presets and overrides survive save and load with the same effective values") {
    const auto table = Table();
    WorldFacts facts;
    REQUIRE(SelectAssistPreset(facts, table, "preset_b").HasValue());
    REQUIRE(SetAssistOverride(facts, AssistOption::DialogueHint, true).HasValue());
    REQUIRE(SetAssistOverride(facts, AssistOption::KnownHint, false).HasValue());

    const auto loaded = WorldFacts::Deserialize(facts.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(SelectedAssistPreset(loaded.Value()).Value() == "preset_b");
    for (std::size_t i = 0; i < kAssistOptionCount; ++i) {
        const auto option = static_cast<AssistOption>(i);
        CHECK(EffectiveAssist(loaded.Value(), table, option).Value() == EffectiveAssist(facts, table, option).Value());
    }
}

TEST_CASE("Assist: a wrong-typed stored value is reported as state corruption") {
    const auto table = Table();
    WorldFacts facts;
    REQUIRE(facts.Apply(SetFact{AssistOverrideKey(AssistOption::FoundMarker), std::string("yes")}).HasValue());
    CHECK(EffectiveAssist(facts, table, AssistOption::FoundMarker).Error().code == AssistErrorCode::StateCorrupt);
}

TEST_CASE("Assist: option names parse exactly and keys follow the contract") {
    CHECK(ParseAssistOption("found_marker") == AssistOption::FoundMarker);
    CHECK(ParseAssistOption("dialogue_hint") == AssistOption::DialogueHint);
    CHECK_FALSE(ParseAssistOption("Found_Marker").has_value());
    CHECK(AssistOverrideKey(AssistOption::KnownHint) == "placeholder.assist.known_hint.override");
    CHECK(AssistPresetKey() == "placeholder.assist.preset");
}
