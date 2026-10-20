// Save compatibility and historical replay for WorldFacts.
//
// The golden file Data/worldfacts_v1_golden.spnfacts is FROZEN: it documents format
// v1 as it was released. Never regenerate or edit it to make a test pass. A format
// change needs a new version number, a new golden file and a decision record; the v1
// file and its tests stay (see Docs/Decisions/0004-world-facts-save-format-v1.md).

#include <cstddef>
#include <cstdint>
#include <doctest.h>
#include <fstream>
#include <iterator>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

#ifndef SPN_TEST_DATA_DIR
#error "SPN_TEST_DATA_DIR must point at Tests/Core/Data"
#endif

using namespace spn::core;

namespace {

static_assert(kFactsSaveFormatVersion == 1,
              "Format version changed: add a new golden file and keep the v1 tests.");

constexpr std::size_t kGoldenSize = 1272;
constexpr std::uint64_t kGoldenFnv1a = 0x45a2cfd414c2f015ULL;
constexpr std::uint64_t kGoldenRevision = 16;

std::string ReadGolden() {
    std::ifstream file(std::string(SPN_TEST_DATA_DIR) + "/worldfacts_v1_golden.spnfacts",
                       std::ios::binary);
    REQUIRE_MESSAGE(file.is_open(), "golden file not found in ", SPN_TEST_DATA_DIR);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

std::uint64_t Fnv1a(std::string_view bytes) {
    std::uint64_t hash = 0xcbf29ce484222325ULL;
    for (const char ch : bytes) {
        hash ^= static_cast<unsigned char>(ch);
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

void Do(WorldFacts& facts, const FactCommand& command, std::string_view cause = {}) {
    const auto result = facts.Apply(command, cause);
    REQUIRE_MESSAGE(result.HasValue(), "unexpected error: ", ToString(result.Error().code));
}

/// The command sequence the golden file was written from.
WorldFacts BuildGoldenStore() {
    WorldFacts f;
    Do(f, SetFact{"placeholder.case.door_open", false}, "scene_start");
    Do(f, SetFact{"placeholder.case.clues_found", std::int64_t{0}});
    Do(f, SetFact{"placeholder.witness.name", std::string("Ann \"A\" O'Neil")}, "dialogue");
    Do(f, AdjustInt{"placeholder.case.clues_found", 1}, "evidence");
    Do(f, SetFact{"placeholder.case.door_open", true}, "player_action");
    Do(f, AdjustInt{"placeholder.case.clues_found", 2}, "evidence");
    Do(f, SetFact{"placeholder.case.door_open", true});
    Do(f,
       SetFact{"placeholder.note.text",
               std::string("line1\nline2\ttab\\slash \x01 \x7f caf\xc3\xa9 \xe2\x82\xac")},
       "journal");
    Do(f, SetFact{"placeholder.limit.max", std::int64_t{9223372036854775807LL}});
    Do(f, SetFact{"placeholder.limit.min", std::int64_t{-9223372036854775807LL - 1}});
    Do(f, SetFact{"placeholder.empty.text", std::string()});
    Do(f, AdjustInt{"placeholder.limit.max", -1}, "clamp");
    Do(f, RemoveFact{"placeholder.witness.name"}, "witness_left");
    Do(f, SetFact{"placeholder.witness.name", std::int64_t{42}}, "type_change_after_remove");
    Do(f, AdjustInt{"placeholder.case.clues_found", -2}, "evidence_retracted");
    Do(f, RemoveFact{"placeholder.empty.text"});
    return f;
}

WorldFacts LoadGolden() {
    auto loaded = WorldFacts::Deserialize(ReadGolden());
    REQUIRE_MESSAGE(loaded.HasValue(), "golden load failed: ", ToString(loaded.Error().code));
    return std::move(loaded).Value();
}

FactError LoadError(std::string_view text) {
    const auto result = WorldFacts::Deserialize(text);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

/// Returns the golden text with the 1-based line `lineNumber` replaced by `replacement`
/// (which must carry its own '\n', or be empty to delete the line).
std::string ReplaceGoldenLine(std::size_t lineNumber, const std::string& replacement) {
    const std::string text = ReadGolden();
    std::size_t start = 0;
    for (std::size_t line = 1; line < lineNumber; ++line) {
        start = text.find('\n', start) + 1;
    }
    const std::size_t end = text.find('\n', start) + 1;
    return text.substr(0, start) + replacement + text.substr(end);
}

std::string GoldenLine(std::size_t lineNumber) {
    const std::string text = ReadGolden();
    std::size_t start = 0;
    for (std::size_t line = 1; line < lineNumber; ++line) {
        start = text.find('\n', start) + 1;
    }
    return text.substr(start, text.find('\n', start) + 1 - start);
}

} // namespace

// --- Golden file (format v1) -------------------------------------------------

TEST_CASE("Golden v1: the file itself is unchanged") {
    const std::string bytes = ReadGolden();
    CHECK(bytes.size() == kGoldenSize);
    CHECK(Fnv1a(bytes) == kGoldenFnv1a);
    CHECK(bytes.starts_with("SPNFACTS 1\n"));
    CHECK(bytes.find('\r') == std::string::npos);
}

TEST_CASE("Golden v1: loads and restores the expected state") {
    const WorldFacts facts = LoadGolden();
    CHECK(facts.Revision() == kGoldenRevision);
    CHECK(facts.Journal().size() == kGoldenRevision);
    CHECK(facts.Facts().size() == 6);

    CHECK(facts.FindAs<bool>("placeholder.case.door_open") != nullptr);
    CHECK(*facts.FindAs<bool>("placeholder.case.door_open") == true);
    CHECK(*facts.FindAs<std::int64_t>("placeholder.case.clues_found") == 1);
    CHECK(*facts.FindAs<std::int64_t>("placeholder.witness.name") == 42);
    CHECK(*facts.FindAs<std::int64_t>("placeholder.limit.max") == 9223372036854775806LL);
    CHECK(*facts.FindAs<std::int64_t>("placeholder.limit.min") == (-9223372036854775807LL - 1));
    CHECK(*facts.FindAs<std::string>("placeholder.note.text") ==
          "line1\nline2\ttab\\slash \x01 \x7f caf\xc3\xa9 \xe2\x82\xac");
    CHECK(facts.Find("placeholder.empty.text") == nullptr);

    // Journal details that a format change could silently break.
    const auto& journal = facts.Journal();
    CHECK(journal[2].cause == "dialogue");
    CHECK(journal[6].cause.empty()); // unchanged Set is still journaled
    CHECK(journal[6].previous == journal[6].current);
    CHECK(journal[12].current == std::nullopt); // remove
    CHECK(journal[13].previous == std::nullopt);
    CHECK(journal[13].current == FactValue{std::int64_t{42}});
}

TEST_CASE("Golden v1: loading is deterministic and the text is canonical") {
    const std::string bytes = ReadGolden();
    const WorldFacts first = LoadGolden();
    const WorldFacts second = LoadGolden();
    CHECK(first == second);
    CHECK(first.Serialize() == bytes);
    CHECK(second.Serialize() == bytes);

    // Saving a loaded save and loading it again changes nothing.
    auto again = WorldFacts::Deserialize(first.Serialize());
    REQUIRE(again.HasValue());
    CHECK(again.Value() == first);
}

TEST_CASE("Golden v1: today's writer still produces the golden bytes") {
    CHECK(BuildGoldenStore().Serialize() == ReadGolden());
    CHECK(BuildGoldenStore() == LoadGolden());
}

TEST_CASE("Golden v1: a loaded save keeps working and appends gaplessly") {
    WorldFacts facts = LoadGolden();
    const auto next = facts.Apply(AdjustInt{"placeholder.case.clues_found", 4}, "after_load");
    REQUIRE(next.HasValue());
    CHECK(next.Value().sequence == kGoldenRevision + 1);
    CHECK(facts.Serialize().ends_with("end\t17\n"));
    CHECK(WorldFacts::Deserialize(facts.Serialize()).HasValue());
}

// --- Damaged golden saves ----------------------------------------------------

TEST_CASE("Golden v1: every proper prefix is rejected") {
    const std::string bytes = ReadGolden();
    for (std::size_t length = 0; length < bytes.size(); ++length) {
        CHECK_FALSE(WorldFacts::Deserialize(std::string_view(bytes).substr(0, length)).HasValue());
    }
}

TEST_CASE("Golden v1: truncation reports a truncation or header error") {
    const std::string bytes = ReadGolden();
    // Cut inside the last change line (no trailer, no final newline).
    const std::string cutMidLine = bytes.substr(0, bytes.rfind("end\t") - 5);
    CHECK(LoadError(cutMidLine).code == FactErrorCode::TruncatedData);
    // Trailer missing entirely, last change line intact.
    const std::string noTrailer = bytes.substr(0, bytes.rfind("end\t"));
    CHECK(LoadError(noTrailer) == FactError{FactErrorCode::TruncatedData, 17});
    // Trailer cut after "end".
    CHECK(LoadError(bytes.substr(0, bytes.size() - 3)).code == FactErrorCode::TruncatedData);
}

TEST_CASE("Golden v1: any single-byte corruption is rejected or re-serializes identically") {
    const std::string bytes = ReadGolden();
    constexpr std::string_view kReplacements = "X\t\n\"\\0-:";
    std::size_t rejected = 0;
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        for (const char replacement : kReplacements) {
            if (bytes[i] == replacement) {
                continue;
            }
            std::string mutated = bytes;
            mutated[i] = replacement;
            const auto result = WorldFacts::Deserialize(mutated);
            if (result.HasValue()) {
                CHECK(result.Value().Serialize() == mutated);
            } else {
                ++rejected;
            }
        }
    }
    CHECK(rejected > 0);
}

TEST_CASE("Golden v1: structural damage is rejected with a precise code") {
    const std::string bytes = ReadGolden();

    // Other format versions are never guessed at.
    CHECK(LoadError("SPNFACTS 2" + bytes.substr(10)) ==
          FactError{FactErrorCode::UnsupportedVersion, 1});
    CHECK(LoadError("SPNFACTS 0" + bytes.substr(10)) ==
          FactError{FactErrorCode::UnsupportedVersion, 1});
    CHECK(LoadError("SPNFACTS 10" + bytes.substr(10)).code == FactErrorCode::UnsupportedVersion);
    CHECK(LoadError("SPNFACTS" + bytes.substr(9)).code == FactErrorCode::InvalidHeader);

    // Windows line endings (e.g. a text-mode transfer) are damage, not a variant.
    std::string crlf;
    for (const char ch : bytes) {
        if (ch == '\n') {
            crlf += '\r';
        }
        crlf += ch;
    }
    CHECK(LoadError(crlf) == FactError{FactErrorCode::InvalidHeader, 1});

    // Missing, duplicated and swapped change lines.
    CHECK(LoadError(ReplaceGoldenLine(5, "")) == FactError{FactErrorCode::SequenceMismatch, 5});
    CHECK(LoadError(ReplaceGoldenLine(5, GoldenLine(4) + GoldenLine(5))) ==
          FactError{FactErrorCode::SequenceMismatch, 5});
    CHECK(LoadError(ReplaceGoldenLine(3, GoldenLine(4))) ==
          FactError{FactErrorCode::SequenceMismatch, 3});

    // Trailer damage.
    CHECK(LoadError(bytes.substr(0, bytes.size() - 3) + "5\n") ==
          FactError{FactErrorCode::CountMismatch, 18});
    CHECK(LoadError(bytes + "change\t17\n") == FactError{FactErrorCode::TrailingData, 19});
    CHECK(LoadError(bytes + '\0') == FactError{FactErrorCode::TrailingData, 19});
    CHECK(LoadError(bytes + bytes) == FactError{FactErrorCode::TrailingData, 19});
}

TEST_CASE("Golden v1: tampered values are caught by the journal verification") {
    // Line 5 (seq 4): "adjust clues_found +1": claim the result was 7 instead of 1.
    std::string line = GoldenLine(5);
    REQUIRE(line.find("i:0\ti:1\t") != std::string::npos);
    line.replace(line.find("i:0\ti:1\t"), 8, "i:0\ti:7\t");
    CHECK(LoadError(ReplaceGoldenLine(5, line)) == FactError{FactErrorCode::JournalMismatch, 5});

    // A different command argument no longer replays to the recorded result.
    std::string adjust = GoldenLine(5);
    adjust.replace(adjust.find("\ti:1\ti:0"), 4, "\ti:2");
    CHECK(LoadError(ReplaceGoldenLine(5, adjust)) == FactError{FactErrorCode::JournalMismatch, 5});

    // A key that never existed is reported as a replay error on that line.
    std::string removal = GoldenLine(14);
    removal.replace(removal.find("witness.name"), 12, "witness.nope");
    CHECK(LoadError(ReplaceGoldenLine(14, removal)).line == 14);
}

// --- Historical replay (StateAt) ---------------------------------------------

TEST_CASE("StateAt: sequence 0 is the empty store") {
    const WorldFacts facts = LoadGolden();
    const auto start = facts.StateAt(0);
    REQUIRE(start.HasValue());
    CHECK(start.Value() == WorldFacts{});
    CHECK(start.Value().Revision() == 0);
    CHECK(start.Value().Serialize() == "SPNFACTS 1\nend\t0\n");
}

TEST_CASE("StateAt: the last sequence equals the store itself") {
    const WorldFacts facts = LoadGolden();
    const auto end = facts.StateAt(facts.Revision());
    REQUIRE(end.HasValue());
    CHECK(end.Value() == facts);
    CHECK(end.Value().Serialize() == ReadGolden());
}

TEST_CASE("StateAt: reconstructs states in the middle of the history") {
    const WorldFacts facts = LoadGolden();

    const auto s4 = facts.StateAt(4);
    REQUIRE(s4.HasValue());
    CHECK(s4.Value().Revision() == 4);
    CHECK(*s4.Value().FindAs<bool>("placeholder.case.door_open") == false);
    CHECK(*s4.Value().FindAs<std::int64_t>("placeholder.case.clues_found") == 1);
    CHECK(*s4.Value().FindAs<std::string>("placeholder.witness.name") == "Ann \"A\" O'Neil");
    CHECK(s4.Value().Find("placeholder.note.text") == nullptr);

    const auto s13 = facts.StateAt(13); // witness removed, empty text still present
    REQUIRE(s13.HasValue());
    CHECK(s13.Value().Find("placeholder.witness.name") == nullptr);
    CHECK(s13.Value().Find("placeholder.empty.text") != nullptr);
    CHECK(*s13.Value().FindAs<std::int64_t>("placeholder.limit.max") == 9223372036854775806LL);

    const auto s14 = facts.StateAt(14); // witness comes back with another type
    REQUIRE(s14.HasValue());
    CHECK(*s14.Value().FindAs<std::int64_t>("placeholder.witness.name") == 42);
}

TEST_CASE("StateAt: every historical state is the exact prefix of the history") {
    const WorldFacts facts = LoadGolden();
    for (std::uint64_t sequence = 0; sequence <= facts.Revision(); ++sequence) {
        const auto state = facts.StateAt(sequence);
        REQUIRE(state.HasValue());
        CHECK(state.Value().Revision() == sequence);

        // Journal is the prefix of the original journal.
        const auto& journal = state.Value().Journal();
        REQUIRE(journal.size() == sequence);
        for (std::size_t i = 0; i < journal.size(); ++i) {
            CHECK(journal[i] == facts.Journal()[i]);
        }

        // It is a valid save of its own: serializing and loading is lossless.
        const std::string text = state.Value().Serialize();
        const auto reloaded = WorldFacts::Deserialize(text);
        REQUIRE(reloaded.HasValue());
        CHECK(reloaded.Value() == state.Value());

        // And it equals a store built from the first `sequence` commands only.
        WorldFacts rebuilt;
        for (std::size_t i = 0; i < journal.size(); ++i) {
            Do(rebuilt, facts.Journal()[i].command, facts.Journal()[i].cause);
        }
        CHECK(rebuilt == state.Value());
    }
}

TEST_CASE("StateAt: invalid sequences are errors") {
    const WorldFacts facts = LoadGolden();
    const FactError outOfRange{FactErrorCode::SequenceOutOfRange, 0};

    const auto past = facts.StateAt(facts.Revision() + 1);
    REQUIRE_FALSE(past.HasValue());
    CHECK(past.Error() == outOfRange);

    const auto huge = facts.StateAt(UINT64_MAX);
    REQUIRE_FALSE(huge.HasValue());
    CHECK(huge.Error() == outOfRange);

    // An empty store only knows sequence 0.
    CHECK(WorldFacts{}.StateAt(0).HasValue());
    CHECK(WorldFacts{}.StateAt(1).Error() == outOfRange);
}

TEST_CASE("StateAt: historical queries never change the queried store") {
    const WorldFacts facts = LoadGolden();
    const std::string textBefore = facts.Serialize();

    for (std::uint64_t sequence = 0; sequence <= facts.Revision() + 1; ++sequence) {
        (void)facts.StateAt(sequence);
    }
    CHECK(facts == LoadGolden());
    CHECK(facts.Serialize() == textBefore);

    // A reconstructed state is independent: changing it does not touch the original.
    auto past = facts.StateAt(7);
    REQUIRE(past.HasValue());
    WorldFacts branch = std::move(past).Value();
    Do(branch, SetFact{"placeholder.branch.only", true});
    CHECK(branch.Revision() == 8);
    CHECK(facts == LoadGolden());
    CHECK(facts.Find("placeholder.branch.only") == nullptr);
}

TEST_CASE("FactErrorCode: the historical-query code is named") {
    CHECK(ToString(FactErrorCode::SequenceOutOfRange) == "sequence out of range");
}

// --- Limits are part of format v1 ---------------------------------------------
//
// Data/worldfacts_v1_limits.spnfacts is FROZEN like the golden file: a save at the
// exact maximum key (128), string (4096 bytes) and cause (128 bytes) lengths. If a
// limit is tightened in code, these saves stop loading, which is a format change.

namespace {

constexpr std::size_t kLimitsSize = 24920;
constexpr std::uint64_t kLimitsFnv1a = 0xe54d868e5873ad16ULL;

std::string ReadLimits() {
    std::ifstream file(std::string(SPN_TEST_DATA_DIR) + "/worldfacts_v1_limits.spnfacts",
                       std::ios::binary);
    REQUIRE_MESSAGE(file.is_open(), "limits file not found in ", SPN_TEST_DATA_DIR);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

std::string LimitKey() {
    return "limit." + std::string(kMaxFactKeyLength - 6, 'k');
}

/// One-record document written by hand (independent of Serialize).
std::string SingleSet(const std::string& key, const std::string& value, const std::string& cause) {
    const std::string token = "s:\"" + value + "\"";
    return "SPNFACTS 1\nchange\t1\tset\t" + key + "\t" + token + "\t-\t" + token + "\t\"" + cause +
           "\"\nend\t1\n";
}

} // namespace

TEST_CASE("Limits v1: the limits file itself is unchanged") {
    const std::string bytes = ReadLimits();
    CHECK(bytes.size() == kLimitsSize);
    CHECK(Fnv1a(bytes) == kLimitsFnv1a);
    CHECK(bytes.find('\r') == std::string::npos);
}

TEST_CASE("Limits v1: a save at the maximum key, string and cause lengths loads") {
    static_assert(kMaxFactKeyLength == 128 && kMaxFactStringLength == 4096 &&
                      kMaxCauseLength == 128,
                  "A limit changed: that is a format change (see 0004). Keep this test and file.");
    const std::string bytes = ReadLimits();
    const auto loaded = WorldFacts::Deserialize(bytes);
    REQUIRE_MESSAGE(loaded.HasValue(), "load failed: ", ToString(loaded.Error().code));
    const WorldFacts& facts = loaded.Value();
    CHECK(facts.Revision() == 2);

    const std::string* longText = facts.FindAs<std::string>(LimitKey());
    REQUIRE(longText != nullptr);
    CHECK(*longText == std::string(kMaxFactStringLength, 'x'));
    CHECK(facts.Journal()[0].cause == std::string(kMaxCauseLength, 'c'));

    // The limit counts decoded bytes: 4096 newlines occupy 8192 bytes on disk.
    const std::string* escaped = facts.FindAs<std::string>("limit.escaped");
    REQUIRE(escaped != nullptr);
    CHECK(*escaped == std::string(kMaxFactStringLength, '\n'));

    CHECK(facts.Serialize() == bytes); // canonical: byte-identical re-save
}

TEST_CASE("Limits v1: today's writer reproduces the limits file") {
    WorldFacts facts;
    Do(facts, SetFact{LimitKey(), std::string(kMaxFactStringLength, 'x')},
       std::string(kMaxCauseLength, 'c'));
    Do(facts, SetFact{"limit.escaped", std::string(kMaxFactStringLength, '\n')});
    CHECK(facts.Serialize() == ReadLimits());
}

TEST_CASE("Limits v1: one unit over each limit is rejected on load") {
    const std::string key = LimitKey();
    const std::string text(kMaxFactStringLength, 'x');
    const std::string cause(kMaxCauseLength, 'c');

    // Control: the same hand-written shape at the exact limits is accepted.
    CHECK(WorldFacts::Deserialize(SingleSet(key, text, cause)).HasValue());

    CHECK(LoadError(SingleSet(key + "k", text, cause)) == FactError{FactErrorCode::InvalidKey, 2});
    CHECK(LoadError(SingleSet(key, text + "x", cause)) ==
          FactError{FactErrorCode::ValueTooLong, 2});
    CHECK(LoadError(SingleSet(key, text, cause + "c")) ==
          FactError{FactErrorCode::CauseTooLong, 2});
    // Escaped form: 4097 newlines are over the limit although each is 2 bytes on disk.
    std::string escapedOver;
    for (std::size_t i = 0; i <= kMaxFactStringLength; ++i) {
        escapedOver += "\\n";
    }
    CHECK(LoadError(SingleSet(key, escapedOver, cause)) ==
          FactError{FactErrorCode::ValueTooLong, 2});
}
