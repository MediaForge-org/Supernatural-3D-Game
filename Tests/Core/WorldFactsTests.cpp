#include <array>
#include <cstdint>
#include <doctest.h>
#include <limits>
#include <optional>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using namespace spn::core;

namespace {

constexpr auto kInt64Max = std::numeric_limits<std::int64_t>::max();
constexpr auto kInt64Min = std::numeric_limits<std::int64_t>::min();

// A string literal must select std::string, never bool (C++20 variant rules).
static_assert(std::is_same_v<decltype(FactValue{"text"}.index()), std::size_t>);

SetFact Set(std::string key, FactValue value) {
    return SetFact{std::move(key), std::move(value)};
}

/// Reads the value of a journal slot; fails the test if it is empty or of another type.
template <typename T> T Extract(const std::optional<FactValue>& slot) {
    const T* typed = slot ? std::get_if<T>(&*slot) : nullptr;
    REQUIRE(typed != nullptr);
    return *typed;
}

/// Applies a command that must succeed and returns its journal record.
ChangeRecord Do(WorldFacts& facts, const FactCommand& command, std::string_view cause = {}) {
    auto result = facts.Apply(command, cause);
    REQUIRE_MESSAGE(result.HasValue(), "unexpected error: ", ToString(result.Error().code));
    return std::move(result).Value();
}

/// Applies a command that must fail and returns its error code.
FactErrorCode Fails(WorldFacts& facts, const FactCommand& command, std::string_view cause = {}) {
    const auto result = facts.Apply(command, cause);
    REQUIRE_FALSE(result.HasValue());
    return result.Error().code;
}

/// Test texts use "<T>" for the tab character to stay readable.
std::string Tabs(std::string text) {
    for (std::size_t pos = text.find("<T>"); pos != std::string::npos;
         pos = text.find("<T>", pos)) {
        text.replace(pos, 3, "\t");
    }
    return text;
}

FactError LoadError(std::string_view text) {
    const auto result = WorldFacts::Deserialize(text);
    REQUIRE_FALSE(result.HasValue());
    return result.Error();
}

std::string Header() {
    return "SPNFACTS 1\n";
}

/// A representative store touching every command, type and escape class.
WorldFacts MakeSampleStore() {
    WorldFacts facts;
    Do(facts, Set("case.door_open", true), "inspect");
    Do(facts, Set("case.clues", std::int64_t{0}));
    Do(facts, AdjustInt{"case.clues", 2});
    Do(facts, Set("witness.name", std::string("Ann \"A\"\n")));
    Do(facts, RemoveFact{"case.door_open"});
    return facts;
}

} // namespace

// --- Basic state ------------------------------------------------------------

TEST_CASE("WorldFacts: new store is empty") {
    const WorldFacts facts;
    CHECK(facts.Facts().empty());
    CHECK(facts.Journal().empty());
    CHECK(facts.Revision() == 0);
    CHECK(facts.Find("anything") == nullptr);
}

TEST_CASE("WorldFacts: stores and reads bool, integer and string facts") {
    WorldFacts facts;
    Do(facts, Set("flag.on", true));
    Do(facts, Set("count.neg", std::int64_t{-7}));
    Do(facts, Set("note.text", std::string("hello")));

    REQUIRE(facts.FindAs<bool>("flag.on") != nullptr);
    CHECK(*facts.FindAs<bool>("flag.on") == true);
    REQUIRE(facts.FindAs<std::int64_t>("count.neg") != nullptr);
    CHECK(*facts.FindAs<std::int64_t>("count.neg") == -7);
    REQUIRE(facts.FindAs<std::string>("note.text") != nullptr);
    CHECK(*facts.FindAs<std::string>("note.text") == "hello");

    // Wrong type or missing key yields nullptr instead of a conversion.
    CHECK(facts.FindAs<std::int64_t>("flag.on") == nullptr);
    CHECK(facts.FindAs<bool>("missing") == nullptr);
    CHECK(facts.Facts().size() == 3);
}

TEST_CASE("WorldFacts: string literals become string facts") {
    WorldFacts facts;
    Do(facts, Set("note.text", "literal"));
    CHECK(facts.FindAs<std::string>("note.text") != nullptr);
    CHECK(facts.FindAs<bool>("note.text") == nullptr);
}

TEST_CASE("WorldFacts: Set overwrites a value of the same type") {
    WorldFacts facts;
    Do(facts, Set("a.b", std::int64_t{1}));
    const ChangeRecord record = Do(facts, Set("a.b", std::int64_t{2}));
    CHECK(*facts.FindAs<std::int64_t>("a.b") == 2);
    REQUIRE(record.previous.has_value());
    CHECK(Extract<std::int64_t>(record.previous) == 1);
    CHECK(facts.Facts().size() == 1);
}

TEST_CASE("WorldFacts: Remove deletes a fact and allows a new type afterwards") {
    WorldFacts facts;
    Do(facts, Set("a.b", true));
    Do(facts, RemoveFact{"a.b"});
    CHECK(facts.Find("a.b") == nullptr);
    Do(facts, Set("a.b", std::string("now text")));
    CHECK(facts.FindAs<std::string>("a.b") != nullptr);
}

TEST_CASE("WorldFacts: AdjustInt changes an integer fact") {
    WorldFacts facts;
    Do(facts, Set("clues", std::int64_t{5}));
    const ChangeRecord up = Do(facts, AdjustInt{"clues", 3});
    const ChangeRecord down = Do(facts, AdjustInt{"clues", -10});
    CHECK(*facts.FindAs<std::int64_t>("clues") == -2);
    CHECK(Extract<std::int64_t>(up.current) == 8);
    CHECK(Extract<std::int64_t>(down.previous) == 8);
}

TEST_CASE("WorldFacts: facts are iterated in key order regardless of insertion order") {
    WorldFacts facts;
    Do(facts, Set("zeta", true));
    Do(facts, Set("alpha", true));
    Do(facts, Set("mid.key", true));
    std::vector<std::string> keys;
    for (const auto& [key, value] : facts.Facts()) {
        keys.push_back(key);
    }
    CHECK(keys == std::vector<std::string>{"alpha", "mid.key", "zeta"});
}

// --- Validation and atomicity -----------------------------------------------

TEST_CASE("WorldFacts: key validation") {
    CHECK(IsValidFactKey("a"));
    CHECK(IsValidFactKey("case.mill_2.door"));
    CHECK(IsValidFactKey(std::string(kMaxFactKeyLength, 'a')));

    CHECK_FALSE(IsValidFactKey(""));
    CHECK_FALSE(IsValidFactKey("Upper"));
    CHECK_FALSE(IsValidFactKey("1starts.with.digit"));
    CHECK_FALSE(IsValidFactKey(".leading.dot"));
    CHECK_FALSE(IsValidFactKey("has space"));
    CHECK_FALSE(IsValidFactKey("tab\tkey"));
    CHECK_FALSE(IsValidFactKey("new\nline"));
    CHECK_FALSE(IsValidFactKey("dash-key"));
    CHECK_FALSE(IsValidFactKey("umlaut\xc3\xa4"));
    CHECK_FALSE(IsValidFactKey(std::string("nul\0byte", 8)));
    CHECK_FALSE(IsValidFactKey(std::string(kMaxFactKeyLength + 1, 'a')));
}

TEST_CASE("WorldFacts: every command rejects invalid keys") {
    WorldFacts facts;
    CHECK(Fails(facts, Set("Bad Key", true)) == FactErrorCode::InvalidKey);
    CHECK(Fails(facts, RemoveFact{""}) == FactErrorCode::InvalidKey);
    CHECK(Fails(facts, AdjustInt{"X", 1}) == FactErrorCode::InvalidKey);
    CHECK(facts.Journal().empty());
}

TEST_CASE("WorldFacts: size limits for strings and causes") {
    WorldFacts facts;
    Do(facts, Set("text", std::string(kMaxFactStringLength, 'x')));
    CHECK(Fails(facts, Set("text", std::string(kMaxFactStringLength + 1, 'x'))) ==
          FactErrorCode::ValueTooLong);
    Do(facts, Set("flag", true), std::string(kMaxCauseLength, 'c'));
    CHECK(Fails(facts, Set("flag", false), std::string(kMaxCauseLength + 1, 'c')) ==
          FactErrorCode::CauseTooLong);
    CHECK(*facts.FindAs<bool>("flag") == true);
}

TEST_CASE("WorldFacts: the type of an existing key cannot change") {
    WorldFacts facts;
    Do(facts, Set("a.b", true));
    CHECK(Fails(facts, Set("a.b", std::int64_t{1})) == FactErrorCode::TypeMismatch);
    CHECK(Fails(facts, Set("a.b", std::string("x"))) == FactErrorCode::TypeMismatch);
    CHECK(Fails(facts, AdjustInt{"a.b", 1}) == FactErrorCode::TypeMismatch);
    CHECK(*facts.FindAs<bool>("a.b") == true);
}

TEST_CASE("WorldFacts: missing keys cannot be removed or adjusted") {
    WorldFacts facts;
    CHECK(Fails(facts, RemoveFact{"nope"}) == FactErrorCode::KeyNotFound);
    CHECK(Fails(facts, AdjustInt{"nope", 1}) == FactErrorCode::KeyNotFound);
}

TEST_CASE("WorldFacts: AdjustInt detects overflow in both directions") {
    WorldFacts facts;
    Do(facts, Set("hi", kInt64Max));
    Do(facts, Set("lo", kInt64Min));
    CHECK(Fails(facts, AdjustInt{"hi", 1}) == FactErrorCode::IntegerOverflow);
    CHECK(Fails(facts, AdjustInt{"lo", -1}) == FactErrorCode::IntegerOverflow);
    // Exactly reaching the limit is fine, as is moving away from it.
    Do(facts, AdjustInt{"hi", -1});
    Do(facts, AdjustInt{"hi", 1});
    Do(facts, AdjustInt{"lo", kInt64Max});
    CHECK(*facts.FindAs<std::int64_t>("hi") == kInt64Max);
    CHECK(*facts.FindAs<std::int64_t>("lo") == -1);
    CHECK(Fails(facts, AdjustInt{"lo", kInt64Min}) == FactErrorCode::IntegerOverflow);
}

TEST_CASE("WorldFacts: failed commands change neither state nor journal") {
    WorldFacts facts = MakeSampleStore();
    const WorldFacts before = facts;

    (void)Fails(facts, Set("bad key", true));
    (void)Fails(facts, Set("case.clues", true));
    (void)Fails(facts, RemoveFact{"missing"});
    (void)Fails(facts, AdjustInt{"witness.name", 1});
    (void)Fails(facts, Set("x", true), std::string(kMaxCauseLength + 1, 'c'));

    CHECK(facts == before);
    CHECK(facts.Serialize() == before.Serialize());
}

// --- Journal ----------------------------------------------------------------

TEST_CASE("WorldFacts: journal records every successful command in order") {
    WorldFacts facts;
    const ChangeRecord created = Do(facts, Set("a.b", true), "created");
    const ChangeRecord removed = Do(facts, RemoveFact{"a.b"});

    REQUIRE(facts.Journal().size() == 2);
    CHECK(facts.Revision() == 2);
    CHECK(facts.Journal()[0] == created);
    CHECK(facts.Journal()[1] == removed);

    CHECK(created.sequence == 1);
    CHECK_FALSE(created.previous.has_value());
    REQUIRE(created.current.has_value());
    CHECK(Extract<bool>(created.current) == true);
    CHECK(created.cause == "created");
    CHECK(std::get<SetFact>(created.command).key == "a.b");

    CHECK(removed.sequence == 2);
    REQUIRE(removed.previous.has_value());
    CHECK(Extract<bool>(removed.previous) == true);
    CHECK_FALSE(removed.current.has_value());
    CHECK(removed.cause.empty());
}

TEST_CASE("WorldFacts: a Set that does not change the value is still journaled") {
    WorldFacts facts;
    Do(facts, Set("a.b", true));
    const ChangeRecord again = Do(facts, Set("a.b", true));
    CHECK(again.previous == again.current);
    CHECK(facts.Revision() == 2);
}

// --- Serialization ----------------------------------------------------------

TEST_CASE("WorldFacts: empty store serializes to the minimal document") {
    CHECK(WorldFacts{}.Serialize() == "SPNFACTS 1\nend\t0\n");
}

TEST_CASE("WorldFacts: serialized format is stable (golden text)") {
    const std::string expected =
        Tabs("SPNFACTS 1\n"
             "change<T>1<T>set<T>case.door_open<T>b:true<T>-<T>b:true<T>\"inspect\"\n"
             "change<T>2<T>set<T>case.clues<T>i:0<T>-<T>i:0<T>\"\"\n"
             "change<T>3<T>adjust<T>case.clues<T>i:2<T>i:0<T>i:2<T>\"\"\n"
             "change<T>4<T>set<T>witness.name<T>s:\"Ann \\\"A\\\"\\n\"<T>-<T>s:\"Ann "
             "\\\"A\\\"\\n\"<T>\"\"\n"
             "change<T>5<T>remove<T>case.door_open<T>-<T>b:true<T>-<T>\"\"\n"
             "end<T>5\n");
    CHECK(MakeSampleStore().Serialize() == expected);
}

TEST_CASE("WorldFacts: identical command sequences give identical bytes") {
    CHECK(MakeSampleStore().Serialize() == MakeSampleStore().Serialize());
}

TEST_CASE("WorldFacts: round trip restores state and journal exactly") {
    const WorldFacts original = MakeSampleStore();
    const std::string text = original.Serialize();

    auto loaded = WorldFacts::Deserialize(text);
    REQUIRE(loaded.HasValue());
    const WorldFacts& restored = loaded.Value();

    CHECK(restored == original);
    CHECK(restored.Facts() == original.Facts());
    CHECK(restored.Journal() == original.Journal());
    CHECK(restored.Revision() == 5);
    CHECK(restored.Serialize() == text);
}

TEST_CASE("WorldFacts: round trip of empty store") {
    auto loaded = WorldFacts::Deserialize(WorldFacts{}.Serialize());
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == WorldFacts{});
}

TEST_CASE("WorldFacts: round trip preserves awkward strings and extreme integers") {
    WorldFacts facts;
    const std::string awkward = std::string("quote\" backslash\\ tab\t cr\r nl\n nul") + '\0' +
                                " del\x7f ctrl\x01 utf8 \xc3\xa4\xe2\x82\xac";
    Do(facts, Set("text.awkward", awkward), awkward.substr(0, kMaxCauseLength));
    Do(facts, Set("text.empty", std::string()));
    Do(facts, Set("num.max", kInt64Max));
    Do(facts, Set("num.min", kInt64Min));
    Do(facts, Set("num.zero", std::int64_t{0}));
    Do(facts, Set("flag.false", false));

    const std::string text = facts.Serialize();
    // Only structural newlines may appear raw in the document.
    CHECK(text.find('\r') == std::string::npos);
    CHECK(text.find('\0') == std::string::npos);

    auto loaded = WorldFacts::Deserialize(text);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value() == facts);
    REQUIRE(loaded.Value().FindAs<std::string>("text.awkward") != nullptr);
    CHECK(*loaded.Value().FindAs<std::string>("text.awkward") == awkward);
    CHECK(loaded.Value().Serialize() == text);
}

TEST_CASE("WorldFacts: a loaded store keeps working") {
    auto loaded = WorldFacts::Deserialize(MakeSampleStore().Serialize());
    REQUIRE(loaded.HasValue());
    WorldFacts facts = std::move(loaded).Value();
    const ChangeRecord next = Do(facts, AdjustInt{"case.clues", 1});
    CHECK(next.sequence == 6);
    CHECK(*facts.FindAs<std::int64_t>("case.clues") == 3);
}

// --- Invalid saved data -----------------------------------------------------

TEST_CASE("WorldFacts: rejects invalid headers") {
    CHECK(LoadError("") == FactError{FactErrorCode::InvalidHeader, 0});
    CHECK(LoadError("hello\n") == FactError{FactErrorCode::InvalidHeader, 1});
    CHECK(LoadError("SPNFACTS 1") == FactError{FactErrorCode::InvalidHeader, 1});
    CHECK(LoadError("SPNFACTS x\nend\t0\n") == FactError{FactErrorCode::InvalidHeader, 1});
    CHECK(LoadError("SPNFACTS 01\nend\t0\n") == FactError{FactErrorCode::InvalidHeader, 1});
    CHECK(LoadError("SPNFACTS 1\r\nend\t0\r\n") == FactError{FactErrorCode::InvalidHeader, 1});
    CHECK(LoadError("spnfacts 1\nend\t0\n") == FactError{FactErrorCode::InvalidHeader, 1});
}

TEST_CASE("WorldFacts: rejects unsupported versions") {
    CHECK(LoadError("SPNFACTS 2\nend\t0\n") == FactError{FactErrorCode::UnsupportedVersion, 1});
    CHECK(LoadError("SPNFACTS 0\nend\t0\n") == FactError{FactErrorCode::UnsupportedVersion, 1});
}

TEST_CASE("WorldFacts: rejects truncated, counted-wrong and trailing data") {
    CHECK(LoadError(Header()) == FactError{FactErrorCode::TruncatedData, 1});
    CHECK(LoadError(Header() + "end\t0") == FactError{FactErrorCode::TruncatedData, 2});
    CHECK(LoadError(Header() + "end\t1\n") == FactError{FactErrorCode::CountMismatch, 2});
    CHECK(LoadError(Header() + "end\t0\nextra\n") == FactError{FactErrorCode::TrailingData, 3});
    CHECK(LoadError(Header() + "end\t0\n\n") == FactError{FactErrorCode::TrailingData, 3});
}

TEST_CASE("WorldFacts: every proper prefix of a valid document is rejected") {
    const std::string text = MakeSampleStore().Serialize();
    for (std::size_t length = 0; length < text.size(); ++length) {
        const auto result = WorldFacts::Deserialize(std::string_view(text).substr(0, length));
        CHECK_FALSE(result.HasValue());
    }
    CHECK(WorldFacts::Deserialize(text).HasValue());
}

TEST_CASE("WorldFacts: rejects malformed lines") {
    CHECK(LoadError(Header() + "bogus\n") == FactError{FactErrorCode::MalformedLine, 2});
    CHECK(LoadError(Header() + "change\t1\tset\n") == FactError{FactErrorCode::MalformedLine, 2});
    CHECK(LoadError(Header() + Tabs("change<T>1<T>set<T>a<T>b:true<T>-<T>b:true<T>\"\"<T>x\n")) ==
          FactError{FactErrorCode::MalformedLine, 2});
    CHECK(LoadError(Header() + "end\t0\textra\n") == FactError{FactErrorCode::MalformedLine, 2});
    CHECK(LoadError(Header() + "endx\t0\n") == FactError{FactErrorCode::MalformedLine, 2});
}

TEST_CASE("WorldFacts: rejects invalid and non-canonical tokens") {
    const auto record = [](const std::string& sequence, const std::string& op,
                           const std::string& arg, const std::string& prev, const std::string& cur,
                           const std::string& cause) {
        return Header() + Tabs("change<T>" + sequence + "<T>" + op + "<T>a.b<T>" + arg + "<T>" +
                               prev + "<T>" + cur + "<T>" + cause + "\nend<T>1\n");
    };
    const FactError invalid{FactErrorCode::InvalidToken, 2};

    CHECK(WorldFacts::Deserialize(record("1", "set", "i:1", "-", "i:1", "\"\"")).HasValue());

    CHECK(LoadError(record("01", "set", "i:1", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("x", "set", "i:1", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "frob", "i:1", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "-", "-", "-", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "i:007", "-", "i:7", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "i:-0", "-", "i:0", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "i:+1", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "i:", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "i:9223372036854775808", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "b:True", "-", "b:true", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "x:1", "-", "i:1", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:abc", "-", "s:\"abc\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:\"a\"b\"", "-", "s:\"ab\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:\"a\\\"", "-", "s:\"a\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:\"\\q\"", "-", "s:\"\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:\"\\x4\"", "-", "s:\"\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:\"\\x4G\"", "-", "s:\"\"", "\"\"")) == invalid);
    // Valid escape syntax but not the canonical spelling ("A" must be written plain).
    CHECK(LoadError(record("1", "set", "s:\"\\x41\"", "-", "s:\"A\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "s:\"\\x0A\"", "-", "s:\"\\n\"", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "set", "i:1", "-", "i:1", "unquoted")) == invalid);
    CHECK(LoadError(record("1", "remove", "i:1", "i:1", "-", "\"\"")) == invalid);
    CHECK(LoadError(record("1", "adjust", "s:\"1\"", "i:1", "i:2", "\"\"")) == invalid);
}

TEST_CASE("WorldFacts: rejects a broken sequence") {
    const auto line = [](const std::string& seq) {
        return Tabs("change<T>" + seq + "<T>set<T>a.b<T>b:true<T>-<T>b:true<T>\"\"\n");
    };
    CHECK(LoadError(Header() + line("2") + "end\t1\n") ==
          FactError{FactErrorCode::SequenceMismatch, 2});
    CHECK(LoadError(Header() + line("1") + line("1") + "end\t2\n") ==
          FactError{FactErrorCode::SequenceMismatch, 3});
    CHECK(LoadError(Header() + line("1") + line("3") + "end\t2\n") ==
          FactError{FactErrorCode::SequenceMismatch, 3});
}

TEST_CASE("WorldFacts: rejects journals that do not replay to the recorded values") {
    CHECK(
        LoadError(Header() + Tabs("change<T>1<T>set<T>a.b<T>i:1<T>i:0<T>i:1<T>\"\"\nend<T>1\n")) ==
        FactError{FactErrorCode::JournalMismatch, 2});
    CHECK(LoadError(Header() + Tabs("change<T>1<T>set<T>a.b<T>i:1<T>-<T>i:2<T>\"\"\nend<T>1\n")) ==
          FactError{FactErrorCode::JournalMismatch, 2});
    CHECK(LoadError(Header() + Tabs("change<T>1<T>set<T>a.b<T>i:1<T>-<T>i:1<T>\"\"\n"
                                    "change<T>2<T>adjust<T>a.b<T>i:1<T>i:1<T>i:5<T>\"\"\n"
                                    "end<T>2\n")) == FactError{FactErrorCode::JournalMismatch, 3});
}

TEST_CASE("WorldFacts: command errors during replay report the line") {
    const auto set = [](const std::string& seq, const std::string& key, const std::string& value,
                        const std::string& prev) {
        return Tabs("change<T>" + seq + "<T>set<T>" + key + "<T>" + value + "<T>" + prev + "<T>" +
                    value + "<T>\"\"\n");
    };
    CHECK(LoadError(Header() + set("1", "Bad", "i:1", "-") + "end\t1\n") ==
          FactError{FactErrorCode::InvalidKey, 2});
    CHECK(LoadError(Header() + set("1", "a.b", "i:1", "-") + set("2", "a.b", "s:\"x\"", "i:1") +
                    "end\t2\n") == FactError{FactErrorCode::TypeMismatch, 3});
    CHECK(LoadError(Header() + Tabs("change<T>1<T>remove<T>a.b<T>-<T>-<T>-<T>\"\"\nend<T>1\n")) ==
          FactError{FactErrorCode::KeyNotFound, 2});
    CHECK(LoadError(Header() + set("1", "a.b", "i:9223372036854775807", "-") +
                    Tabs("change<T>2<T>adjust<T>a.b<T>i:1<T>i:9223372036854775807<T>"
                         "i:0<T>\"\"\nend<T>2\n")) == FactError{FactErrorCode::IntegerOverflow, 3});
    CHECK(LoadError(Header() + Tabs("change<T>1<T>set<T>a.b<T>b:true<T>-<T>b:true<T>") + '"' +
                    std::string(kMaxCauseLength + 1, 'c') + "\"\nend\t1\n") ==
          FactError{FactErrorCode::CauseTooLong, 2});
}

TEST_CASE("WorldFacts: any single-byte corruption is rejected or re-serializes identically") {
    // Canonical-form property: whatever is accepted must be exactly what we would
    // have written ourselves, so a loader can never silently normalize damaged data.
    const std::string text = MakeSampleStore().Serialize();
    constexpr std::string_view kReplacements = "X\t\n\"\\0-";
    std::size_t rejected = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        for (const char replacement : kReplacements) {
            if (text[i] == replacement) {
                continue;
            }
            std::string mutated = text;
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

TEST_CASE("FactErrorCode: every code has a distinct readable name") {
    constexpr std::array kAll = {FactErrorCode::InvalidKey,       FactErrorCode::ValueTooLong,
                                 FactErrorCode::CauseTooLong,     FactErrorCode::KeyNotFound,
                                 FactErrorCode::TypeMismatch,     FactErrorCode::IntegerOverflow,
                                 FactErrorCode::InvalidHeader,    FactErrorCode::UnsupportedVersion,
                                 FactErrorCode::TruncatedData,    FactErrorCode::TrailingData,
                                 FactErrorCode::MalformedLine,    FactErrorCode::InvalidToken,
                                 FactErrorCode::SequenceMismatch, FactErrorCode::JournalMismatch,
                                 FactErrorCode::CountMismatch};
    std::vector<std::string_view> names;
    for (const FactErrorCode code : kAll) {
        const std::string_view name = ToString(code);
        CHECK_FALSE(name.empty());
        CHECK(name != "unknown error");
        for (const std::string_view other : names) {
            CHECK(name != other);
        }
        names.push_back(name);
    }
}
