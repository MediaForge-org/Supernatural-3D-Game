#include <doctest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <random>
#include <spn/core/SaveSlot.h>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "MapTestData.h"
#include "SaveTestFs.h"

using namespace spn::core;
using namespace spn::testdata;

namespace {

SaveSection Sec(std::string id, std::string payload, bool required = false, std::uint32_t version = 1) {
    return SaveSection{std::move(id), version, required, std::move(payload)};
}

std::string Hex8(std::uint32_t v) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%08x", v);
    return buf;
}

// Builds container bytes by hand, with CORRECT checksums, so tests can create structurally wrong but internally consistent files.
struct Raw {
    std::string text;
    explicit Raw(std::string_view header = "SPN-SAVESLOT 1\n", std::size_t declared = 0) : text(header) { text += "sections " + std::to_string(declared) + "\n"; }
    Raw& Section(std::string_view id, std::string_view payload, std::string_view version = "1", std::string_view flag = "opt") {
        text += "section " + std::string(id) + " " + std::string(version) + " " + std::string(flag) + " " + std::to_string(payload.size()) + " " +
                Hex8(Crc32(payload)) + "\n" + std::string(payload) + "\n";
        return *this;
    }
    std::string Finish() const { return text + "end " + Hex8(Crc32(text)) + "\n"; }
};

SaveSlot Sample() {
    SaveSlot s;
    REQUIRE_FALSE(s.Add(Sec("world.facts", "FACTS\n", true)).has_value());
    REQUIRE_FALSE(s.Add(Sec("player.inventory", "INV\nwith\nnewlines\n")).has_value());
    REQUIRE_FALSE(s.Add(Sec("a.empty", "")).has_value());
    return s;
}

SaveErrorCode ParseCode(std::string_view bytes) {
    const auto r = SaveSlot::Parse(bytes);
    REQUIRE_FALSE(r.HasValue());
    return r.Error().code;
}

std::string ReadFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    REQUIRE_MESSAGE(in.is_open(), "cannot open ", path);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

} // namespace

TEST_CASE("Crc32 matches the standard check value and is sensitive to every byte") {
    CHECK(Crc32("123456789") == 0xCBF43926u);
    CHECK(Crc32("") == 0u);
    CHECK(Crc32("a") != Crc32("b"));
    CHECK(Crc32(std::string(1, '\0')) == 0xD202EF8Du);
}

TEST_CASE("section ids: dotted lowercase segments only") {
    for (const char* ok : {"world.facts", "a", "a1_b.c2", "x.y.z"}) CHECK(IsValidSectionId(ok));
    for (const char* bad : {"", ".a", "a.", "a..b", "A", "a b", "a-b", "a/b", "ä"}) CHECK_FALSE(IsValidSectionId(bad));
    CHECK(IsValidSectionId(std::string(kMaxSectionIdLength, 'a')));
    CHECK_FALSE(IsValidSectionId(std::string(kMaxSectionIdLength + 1, 'a')));
}

TEST_CASE("round trip: several sections, empty payload, newlines and binary bytes survive") {
    SaveSlot slot = Sample();
    const std::string binary("x\0y\xff\n" "end deadbeef\nsection fake 1 opt 0 00000000\n", 45);
    REQUIRE_FALSE(slot.Add(Sec("z.binary", binary)).has_value());
    const std::string bytes = slot.Serialize();
    const auto parsed = SaveSlot::Parse(bytes);
    REQUIRE(parsed.HasValue());
    CHECK(parsed.Value() == slot);
    CHECK(parsed.Value().Find("z.binary")->payload == binary);
    CHECK(parsed.Value().Find("a.empty")->payload.empty());
    CHECK(parsed.Value().Count() == 4);
    CHECK(parsed.Value().Serialize() == bytes);
}

TEST_CASE("serialization is deterministic and independent of insertion order") {
    SaveSlot a, b;
    const std::vector<SaveSection> sections = {Sec("c.x", "1"), Sec("a.x", "2"), Sec("b.x", "3", true)};
    for (const auto& s : sections) REQUIRE_FALSE(a.Add(s).has_value());
    for (auto it = sections.rbegin(); it != sections.rend(); ++it) REQUIRE_FALSE(b.Add(*it).has_value());
    CHECK(a.Serialize() == b.Serialize());
    CHECK(a.Serialize() == a.Serialize());
    CHECK(SaveSlot().Serialize().starts_with("SPN-SAVESLOT 1\nsections 0\nend "));
    CHECK(SaveSlot::Parse(SaveSlot().Serialize()).HasValue()); // an empty slot is a valid container
}

TEST_CASE("frozen container format: an independently produced golden file parses and re-serializes byte-identically") {
    const std::string golden = ReadFile(std::string(SPN_TEST_DATA_DIR) + "/saveslot_v1_golden.spnsave"); // produced with Python zlib, not with this code
    const auto parsed = SaveSlot::Parse(golden);
    REQUIRE(parsed.HasValue());
    CHECK(parsed.Value().Count() == 4);
    CHECK(parsed.Value().Find("world.facts")->required);
    CHECK(parsed.Value().Find("future.thing")->version == 3);
    CHECK(parsed.Value().Find("future.thing")->payload == std::string("line1\nend deadbeef\nline3\0\xff", 26));
    CHECK(parsed.Value().Serialize() == golden);
}

TEST_CASE("add, put, remove and limits") {
    SaveSlot s;
    CHECK(s.Add(Sec("a", "1")) == std::nullopt);
    CHECK(s.Add(Sec("a", "2"))->code == SaveErrorCode::DuplicateSection);
    CHECK(s.Find("a")->payload == "1");
    CHECK(s.Put(Sec("a", "2")) == std::nullopt);
    CHECK(s.Find("a")->payload == "2");
    CHECK(s.Add(Sec("Bad Id", "x"))->code == SaveErrorCode::InvalidSectionId);
    CHECK(s.Add(SaveSection{"v0", 0, false, ""})->code == SaveErrorCode::MalformedLine);
    CHECK(s.Remove("a"));
    CHECK_FALSE(s.Remove("a"));
    CHECK(s.Count() == 0);
    for (std::size_t i = 0; i < kMaxSaveSections; ++i) REQUIRE_FALSE(s.Add(Sec("s" + std::to_string(i), "")).has_value());
    CHECK(s.Add(Sec("one.too.many", ""))->code == SaveErrorCode::BadLength);
    CHECK(s.Put(Sec("s0", "replacing is still fine when full")) == std::nullopt);
}

TEST_CASE("corrupt headers are rejected with a precise code") {
    CHECK(ParseCode("") == SaveErrorCode::BadHeader);
    CHECK(ParseCode("not a save\n") == SaveErrorCode::BadHeader);
    CHECK(ParseCode("SPN-SAVESLOT") == SaveErrorCode::Truncated);
    CHECK(ParseCode("SPN-SAVESLOT x\nsections 0\n") == SaveErrorCode::BadHeader);
    CHECK(ParseCode(Raw("SPN-SAVESLOT 2\n").Finish()) == SaveErrorCode::UnsupportedContainerVersion);
    CHECK(ParseCode(Raw("SPN-SAVESLOT 01\n").Finish()) == SaveErrorCode::BadHeader); // non-canonical number
    CHECK(ParseCode("SPN-SAVESLOT 1\n") == SaveErrorCode::Truncated);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections x\n") == SaveErrorCode::MalformedLine);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 01\n") == SaveErrorCode::MalformedLine);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections -1\n") == SaveErrorCode::MalformedLine);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1025\n") == SaveErrorCode::BadLength);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 99999999999999999999\n") == SaveErrorCode::MalformedLine);
}

TEST_CASE("every strict prefix of a valid file is rejected, never accepted as a smaller save") {
    const std::string bytes = Sample().Serialize();
    for (std::size_t n = 0; n < bytes.size(); ++n) {
        INFO("prefix length " << n);
        CHECK_FALSE(SaveSlot::Parse(std::string_view(bytes).substr(0, n)).HasValue());
    }
    CHECK(SaveSlot::Parse(bytes).HasValue());
}

TEST_CASE("flipping any single byte of a valid file is detected") {
    const std::string bytes = Sample().Serialize();
    std::size_t detected = 0;
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        std::string bad = bytes;
        bad[i] = static_cast<char>(bad[i] ^ 0x01);
        INFO("flipped byte " << i);
        const auto r = SaveSlot::Parse(bad);
        if (!r.HasValue()) ++detected;
        else CHECK_FALSE(r.Value() == SaveSlot::Parse(bytes).Value()); // if it parses at all it must not pretend to be the original
    }
    CHECK(detected == bytes.size()); // CRC-32 detects every single-bit error
}

TEST_CASE("truncated and oversized payload lengths, bad checksums and structure errors") {
    // payload shorter than declared
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1\nsection a 1 opt 100 00000000\nshort\n") == SaveErrorCode::Truncated);
    // length absurd
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1\nsection a 1 opt 99999999999 00000000\n") == SaveErrorCode::BadLength);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1\nsection a 1 opt 268435457 00000000\n") == SaveErrorCode::BadLength);
    // wrong declared length (payload continues past it): the byte after the payload is not the newline
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1\nsection a 1 opt 2 00000000\nabcd\nend 00000000\n") == SaveErrorCode::MalformedLine);
    // section checksum
    std::string text = Raw("SPN-SAVESLOT 1\n", 1).Section("a", "payload").Finish();
    const auto at = text.find("payload");
    text[at] = 'P';
    CHECK(ParseCode(text) == SaveErrorCode::SectionChecksumMismatch);
    // trailer checksum: a header-only change keeps the section checksums valid
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p", "1", "opt").text + "end 00000000\n") == SaveErrorCode::TrailerChecksumMismatch);
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p", "1", "opt").text + "end ABCDEF01\n") == SaveErrorCode::MalformedLine); // uppercase hex
    // trailing data after the trailer
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p").Finish() + "x") == SaveErrorCode::TrailingData);
    // counts
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 2).Section("a", "p").Finish()) == SaveErrorCode::CountMismatch);
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p").Section("b", "q").Finish()) == SaveErrorCode::CountMismatch);
    // header grammar
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p", "0").Finish()) == SaveErrorCode::MalformedLine);       // version 0
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p", "01").Finish()) == SaveErrorCode::MalformedLine);      // non-canonical
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("a", "p", "1", "maybe").Finish()) == SaveErrorCode::MalformedLine);
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("Bad Id", "p").Finish()) == SaveErrorCode::MalformedLine);       // 7 tokens
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 1).Section("BAD", "p").Finish()) == SaveErrorCode::InvalidSectionId);
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1\nsection a 1 opt 1\np\nend 00000000\n") == SaveErrorCode::MalformedLine); // too few tokens
    CHECK(ParseCode("SPN-SAVESLOT 1\nsections 1\nsection  a 1 opt 1 00000000\np\nend 00000000\n") == SaveErrorCode::MalformedLine); // double space
}

TEST_CASE("duplicate and misordered section ids are rejected") {
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 2).Section("a", "1").Section("a", "2").Finish()) == SaveErrorCode::DuplicateSection);
    CHECK(ParseCode(Raw("SPN-SAVESLOT 1\n", 2).Section("b", "1").Section("a", "2").Finish()) == SaveErrorCode::SectionOrder);
    CHECK(SaveSlot::Parse(Raw("SPN-SAVESLOT 1\n", 2).Section("a", "1").Section("b", "2").Finish()).HasValue());
}

TEST_CASE("schema: missing required, unknown optional, unknown required, newer versions") {
    const SaveSchema schema{{{"world.facts", 2, true}, {"player.inventory", 1, false}}};
    SaveSlot ok;
    REQUIRE_FALSE(ok.Add(Sec("world.facts", "F", true, 2)).has_value());
    CHECK_FALSE(ok.Validate(schema).has_value());

    SaveSlot missing;
    REQUIRE_FALSE(missing.Add(Sec("player.inventory", "I")).has_value());
    CHECK(missing.Validate(schema)->code == SaveErrorCode::MissingRequiredSection);
    CHECK(missing.Validate(schema)->detail == "world.facts");

    SaveSlot unknownOptional = ok;
    REQUIRE_FALSE(unknownOptional.Add(Sec("future.thing", "?", false, 9)).has_value());
    CHECK_FALSE(unknownOptional.Validate(schema).has_value());
    CHECK(unknownOptional.UnreadSections(schema) == std::vector<std::string>{"future.thing"});
    // the unknown section is kept: it survives a serialize/parse cycle
    CHECK(SaveSlot::Parse(unknownOptional.Serialize()).Value().Find("future.thing")->payload == "?");

    SaveSlot unknownRequired = ok;
    REQUIRE_FALSE(unknownRequired.Add(Sec("future.must", "?", true)).has_value());
    CHECK(unknownRequired.Validate(schema)->code == SaveErrorCode::UnsupportedRequiredSection);

    SaveSlot newerRequired;
    REQUIRE_FALSE(newerRequired.Add(Sec("world.facts", "F", true, 3)).has_value());
    CHECK(newerRequired.Validate(schema)->code == SaveErrorCode::UnsupportedSectionVersion);

    SaveSlot newerOptional = ok;
    REQUIRE_FALSE(newerOptional.Add(Sec("player.inventory", "I", false, 2)).has_value());
    CHECK_FALSE(newerOptional.Validate(schema).has_value()); // optional + newer: ignored, not an error
    CHECK(newerOptional.UnreadSections(schema) == std::vector<std::string>{"player.inventory"});
}

TEST_CASE("randomized slots always round-trip and never parse after damage") {
    Lcg rng(0x5AFE);
    for (int round = 0; round < 200; ++round) {
        SaveSlot slot;
        const auto count = rng.Next() % 6;
        for (std::uint64_t i = 0; i < count; ++i) {
            std::string payload(rng.Next() % 40, '\0');
            for (auto& c : payload) c = static_cast<char>(rng.Next() % 256);
            REQUIRE_FALSE(slot.Put(Sec("s" + std::to_string(rng.Next() % 8) + ".x", payload, rng.Next() % 2 == 0, static_cast<std::uint32_t>(1 + rng.Next() % 3))).has_value());
        }
        const std::string bytes = slot.Serialize();
        const auto back = SaveSlot::Parse(bytes);
        REQUIRE(back.HasValue());
        CHECK(back.Value() == slot);
        std::string damaged = bytes;
        damaged[rng.Next() % damaged.size()] ^= static_cast<char>(1 + rng.Next() % 255);
        CHECK_FALSE(SaveSlot::Parse(damaged).HasValue());
        CHECK_FALSE(SaveSlot::Parse(bytes.substr(0, rng.Next() % bytes.size())).HasValue());
    }
}

// --- store: publication, recovery, fault injection ------------------------------------------------------------------------------------

namespace {

const SaveSchema kSchema{{{"world.facts", 1, true}, {"player.inventory", 1, false}}};

SaveSlot Generation(int n) {
    SaveSlot s;
    REQUIRE_FALSE(s.Add(Sec("world.facts", "facts generation " + std::to_string(n) + std::string(200, 'x'), true)).has_value());
    REQUIRE_FALSE(s.Add(Sec("player.inventory", "inventory generation " + std::to_string(n))).has_value());
    return s;
}

const std::filesystem::path kBase = "slots/slot1.sav";

} // namespace

TEST_CASE("store: save, load, and the previous generation is kept") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    CHECK(store.Load(kSchema).Error().code == SaveErrorCode::NoValidSave); // nothing saved yet
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    CHECK(fs.files.size() == 1); // no leftovers: tmp files are gone
    const auto first = store.Load(kSchema);
    REQUIRE(first.HasValue());
    CHECK(first.Value().source == SaveSource::Current);
    CHECK(first.Value().slot == Generation(1));

    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    CHECK(fs.files.size() == 2);
    CHECK(SaveSlot::Parse(fs.files.at(store.Previous().string())).Value() == Generation(1));
    CHECK(store.Load(kSchema).Value().slot == Generation(2));
}

TEST_CASE("store: a slot without its required sections is never published") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    SaveSlot bad;
    REQUIRE_FALSE(bad.Add(Sec("player.inventory", "only inventory")).has_value());
    CHECK(store.Save(bad, kSchema)->code == SaveErrorCode::MissingRequiredSection);
    CHECK(store.Load(kSchema).Value().slot == Generation(1));
    CHECK(fs.files.size() == 1);
}

TEST_CASE("store: corruption of the main file falls back to the previous generation, and a corrupt main file never replaces a good previous one") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());

    for (const auto damage : {0, 1, 2}) {
        MemoryFs copy = fs;
        std::string& cur = copy.files[store.Current().string()];
        if (damage == 0) cur.resize(cur.size() / 2);          // truncated
        else if (damage == 1) cur[cur.size() / 2] ^= 0x20;    // flipped byte
        else cur = "garbage";                                  // not a save
        SaveSlotStore s(copy, kBase);
        const auto loaded = s.Load(kSchema);
        REQUIRE(loaded.HasValue());
        CHECK(loaded.Value().source == SaveSource::Previous);
        CHECK(loaded.Value().slot == Generation(1));
        REQUIRE(loaded.Value().currentProblem.has_value());
        // saving again: the corrupt main file must not overwrite the good previous generation
        REQUIRE_FALSE(s.Save(Generation(3), kSchema).has_value());
        CHECK(SaveSlot::Parse(copy.files.at(s.Previous().string())).Value() == Generation(1));
        CHECK(s.Load(kSchema).Value().slot == Generation(3));
    }
    MemoryFs both = fs;
    both.files[store.Current().string()] = "garbage";
    both.files[store.Previous().string()] = "garbage";
    SaveSlotStore s(both, kBase);
    CHECK(s.Load(kSchema).Error().code == SaveErrorCode::NoValidSave);
}

TEST_CASE("store: an intact save from a NEWER build is refused, never replaced by an older generation") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    SaveSlot newer = Generation(3);
    REQUIRE_FALSE(newer.Put(Sec("world.facts", "from the future", true, 2)).has_value());
    fs.files[store.Current().string()] = newer.Serialize(); // an intact file the "old build" cannot read
    const auto loaded = store.Load(kSchema);
    REQUIRE_FALSE(loaded.HasValue());
    CHECK(loaded.Error().code == SaveErrorCode::UnsupportedSectionVersion);
    SaveSlot unknownRequired = Generation(3);
    REQUIRE_FALSE(unknownRequired.Add(Sec("future.must", "x", true)).has_value());
    fs.files[store.Current().string()] = unknownRequired.Serialize();
    CHECK(store.Load(kSchema).Error().code == SaveErrorCode::UnsupportedRequiredSection);
    fs.files[store.Current().string()] = "SPN-SAVESLOT 9\nsections 0\n";
    CHECK(store.Load(kSchema).Error().code == SaveErrorCode::UnsupportedContainerVersion);
}

TEST_CASE("store: a leftover temporary file is never loaded and is replaced by the next save") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    fs.files[store.Temporary().string()] = Generation(9).Serialize(); // complete but never published
    CHECK(store.Load(kSchema).Value().slot == Generation(1));
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    CHECK_FALSE(fs.files.contains(store.Temporary().string()));
}

namespace {

// Runs "save generation B over generation A" with a fault at every mutating operation and every fault kind; after a crash the process
// "reboots" (Revive) and loads. Whatever happened, the result must be exactly A or exactly B.
void SweepFaults(int baseGenerations, bool expectNothingAllowed) {
    MemoryFs probe;
    {
        SaveSlotStore store(probe, kBase);
        for (int g = 1; g <= baseGenerations; ++g) REQUIRE_FALSE(store.Save(Generation(g), kSchema).has_value());
        probe.operations = 0;
        REQUIRE_FALSE(store.Save(Generation(baseGenerations + 1), kSchema).has_value());
    }
    const std::size_t total = probe.operations; // mutating operations of one clean save
    REQUIRE(total >= 4);

    const SaveSlot before = Generation(baseGenerations);
    const SaveSlot after = Generation(baseGenerations + 1);
    std::size_t cases = 0, sawBefore = 0, sawAfter = 0, sawNothing = 0;
    for (const auto mode : {FaultMode::Error, FaultMode::ErrorAfterEffect, FaultMode::Crash, FaultMode::TornCrash}) {
        for (std::size_t k = 1; k <= total; ++k) {
            MemoryFs fs;
            SaveSlotStore store(fs, kBase);
            for (int g = 1; g <= baseGenerations; ++g) REQUIRE_FALSE(store.Save(Generation(g), kSchema).has_value());
            fs.operations = 0;
            fs.plan = {mode, k};
            const auto saved = store.Save(after, kSchema);
            fs.Revive(); // power is back (or: the fault was only an error)
            SaveSlotStore rebooted(fs, kBase);
            const auto loaded = rebooted.Load(kSchema);
            ++cases;
            const bool failed = saved.has_value(); // Save returns the error, if any
            const std::string where = "mode " + std::to_string(static_cast<int>(mode)) + " fault at operation " + std::to_string(k) + " of " +
                                      std::to_string(total) + ", save " + (failed ? "reported an error" : "reported success");
            INFO(where);
            CHECK(failed); // a planned fault must be visible to the caller
            if (!loaded.HasValue()) {
                CHECK(expectNothingAllowed); // only legal when there was no earlier save
                CHECK(loaded.Error().code == SaveErrorCode::NoValidSave);
                ++sawNothing;
                REQUIRE_FALSE(rebooted.Save(Generation(99), kSchema).has_value());
                CHECK(rebooted.Load(kSchema).Value().slot == Generation(99));
                continue;
            }
            const bool isBefore = baseGenerations > 0 && loaded.Value().slot == before;
            const bool isAfter = loaded.Value().slot == after;
            CHECK((isBefore || isAfter)); // never a partial or mixed save
            isBefore ? ++sawBefore : ++sawAfter;
            // the leftovers of the interruption do not stop the next save
            REQUIRE_FALSE(rebooted.Save(Generation(99), kSchema).has_value());
            CHECK(rebooted.Load(kSchema).Value().slot == Generation(99));
        }
    }
    CHECK(cases == 4 * total);
    CHECK(sawBefore + sawAfter + sawNothing == cases);
    CHECK(sawAfter > 0); // faults after the publishing step really do leave the new save
    if (baseGenerations > 0) CHECK(sawBefore > 0);
}

} // namespace

TEST_CASE("fault injection: a save over an existing save leaves exactly the old or exactly the new save at every interruption point") {
    SweepFaults(1, false);
    SweepFaults(2, false); // with a previous generation present as well
}

TEST_CASE("fault injection: the very first save either completes or leaves no save, never a partial one") { SweepFaults(0, true); }

TEST_CASE("fault injection: a torn write of the main file by an external cause is recovered from the previous generation") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    // a torn write is what a non-atomic writer would have left: half the bytes
    fs.files[store.Current().string()] = Generation(3).Serialize().substr(0, 100);
    const auto loaded = store.Load(kSchema);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().source == SaveSource::Previous);
    CHECK(loaded.Value().slot == Generation(1)); // .prev holds the generation before the damaged main file was written
}

TEST_CASE("fault injection: a read-back mismatch aborts before anything is published") {
    struct LyingFs final : MemoryFs {
        spn::core::Result<std::string, SaveError> Read(const std::filesystem::path& path) override {
            auto r = MemoryFs::Read(path);
            if (lie && r.HasValue() && path.string().ends_with(".tmp")) return std::string(r.Value()).substr(0, 10);
            return r;
        }
        bool lie = false;
    };
    LyingFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    fs.lie = true;
    CHECK(store.Save(Generation(2), kSchema)->code == SaveErrorCode::VerifyFailed);
    fs.lie = false;
    CHECK(store.Load(kSchema).Value().slot == Generation(1));
    CHECK(fs.files.size() == 1); // temp file cleaned up
}

TEST_CASE("fault injection: a failed directory flush reports 'not durable' but the new save is visible") {
    MemoryFs fs;
    SaveSlotStore store(fs, kBase);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    fs.operations = 0;
    // operations of a save over an existing one: remove tmp, write tmp, write prev.tmp, rename prev, rename tmp, sync dir = 6
    fs.plan = {FaultMode::Error, 6};
    const auto saved = store.Save(Generation(2), kSchema);
    REQUIRE(saved.has_value());
    CHECK(saved->detail.starts_with("not durable"));
    CHECK(store.Load(kSchema).Value().slot == Generation(2));
}

// --- the real filesystem ------------------------------------------------------------------------------------------------------------

namespace {
struct TempDir {
    std::filesystem::path path;
    TempDir() {
        path = std::filesystem::temp_directory_path() / ("spn_saveslot_" + std::to_string(std::random_device{}()));
        std::filesystem::create_directories(path);
    }
    ~TempDir() {
        std::error_code ec;
        std::filesystem::remove_all(path, ec);
    }
};
} // namespace

TEST_CASE("real filesystem: save, overwrite, recover and clean up with StdSaveFileSystem") {
    TempDir dir;
    StdSaveFileSystem fs;
    SaveSlotStore store(fs, dir.path / "slot.sav");
    CHECK(store.Load(kSchema).Error().code == SaveErrorCode::NoValidSave);
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    CHECK(store.Load(kSchema).Value().slot == Generation(2));
    CHECK(std::filesystem::exists(store.Previous()));
    CHECK_FALSE(std::filesystem::exists(store.Temporary()));
    // damage the main file on disk
    {
        std::ofstream out(store.Current(), std::ios::binary | std::ios::trunc);
        out << "SPN-SAVESLOT 1\nsections 1\nsection world.facts 1 req 5 00";
    }
    const auto loaded = store.Load(kSchema);
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().source == SaveSource::Previous);
    CHECK(loaded.Value().slot == Generation(1));
    // errors of the platform are reported, not thrown
    SaveSlotStore nowhere(fs, dir.path / "missing_dir" / "slot.sav");
    CHECK(nowhere.Save(Generation(1), kSchema)->code == SaveErrorCode::Io);
}

TEST_CASE("real filesystem: the publish primitives behave as the protocol needs on the temp volume") {
    TempDir dir;
    StdSaveFileSystem fs;
    const auto caps = ProbePublishCapabilities(fs, dir.path);
    INFO("filesystem: " << caps.filesystem << " notes: " << caps.notes);
    CHECK(caps.renameReplacesExisting);
    CHECK(caps.fileSyncSucceeded);
    CHECK(caps.directorySyncSucceeded);
    CHECK(caps.AllChecksPassed());
    CHECK(std::filesystem::is_empty(dir.path)); // the probe cleans up after itself
    const auto bad = ProbePublishCapabilities(fs, dir.path / "does_not_exist");
    CHECK_FALSE(bad.AllChecksPassed());
    CHECK_FALSE(bad.notes.empty());
}

TEST_CASE("real filesystem: non-ASCII directory and file names round trip (UTF-8 paths, no ANSI code page)") {
    TempDir dir;
    StdSaveFileSystem fs;
    const std::filesystem::path folder = dir.path / std::filesystem::path(u8"Spielstände 日本 Übergänge");
    std::filesystem::create_directories(folder);
    SaveSlotStore store(fs, folder / std::filesystem::path(u8"slot äöü.sav"));
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    CHECK(store.Load(kSchema).Value().slot == Generation(2));
    CHECK(std::filesystem::exists(store.Previous()));
    CHECK_FALSE(std::filesystem::exists(store.Temporary()));
    CHECK(std::distance(std::filesystem::directory_iterator(folder), std::filesystem::directory_iterator()) == 2); // current + previous only
}

TEST_CASE("real filesystem: a path longer than 260 characters works") {
    TempDir dir;
    StdSaveFileSystem fs;
    std::filesystem::path folder = dir.path;
#if defined(_WIN32)
    folder = std::filesystem::path(std::wstring(L"\\\\?\\") + std::filesystem::absolute(dir.path).wstring()); // extended-length prefix: the MAX_PATH limit does not apply
#endif
    std::error_code ec;
    for (int i = 0; i < 6; ++i) { // one level at a time: the failing level is reported
        folder /= std::string(60, static_cast<char>('a' + i));
        std::filesystem::create_directory(folder, ec);
        REQUIRE_MESSAGE(!ec, "cannot create level " << i << " (" << folder.string().size() << " characters): " << ec.message());
    }
    REQUIRE(folder.native().size() > 300);
    SaveSlotStore store(fs, folder / "slot.sav");
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    CHECK(store.Load(kSchema).Value().slot == Generation(2));
    CHECK(store.Load(kSchema).Value().source == SaveSource::Current);
}

#if defined(_WIN32)
TEST_CASE("windows: a save file that another process keeps open without sharing is not replaced and the old generation survives") {
    TempDir dir;
    StdSaveFileSystem fs;
    SaveSlotStore store(fs, dir.path / "slot.sav");
    REQUIRE_FALSE(store.Save(Generation(1), kSchema).has_value());
    REQUIRE_FALSE(store.Save(Generation(2), kSchema).has_value());
    // an antivirus scanner or indexer holding the file looks like this: open with no FILE_SHARE_DELETE
    const HANDLE held = ::CreateFileW(store.Current().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(held != INVALID_HANDLE_VALUE);
    const auto error = store.Save(Generation(3), kSchema);
    ::CloseHandle(held);
    REQUIRE(error.has_value()); // the failure is reported, the caller can retry
    CHECK(error->code == SaveErrorCode::Io);
    CHECK(store.Load(kSchema).Value().slot == Generation(2)); // nothing was lost or half-written
    CHECK_FALSE(std::filesystem::exists(store.Temporary()));   // the temporary file was cleaned up
    REQUIRE_FALSE(store.Save(Generation(3), kSchema).has_value()); // once released, saving works again
    CHECK(store.Load(kSchema).Value().slot == Generation(3));
}
#endif

#if defined(_WIN32)
TEST_CASE("windows: a path with an unpaired UTF-16 surrogate is reported as an error and never throws") {
    // path::u8string() throws for such a path (MSVC); the store builds its error messages from paths, and its host (Unreal) expects no exception
    std::wstring name = L"slot_";
    name.push_back(static_cast<wchar_t>(0xD800));
    const std::filesystem::path bad = std::filesystem::temp_directory_path() / name / "slot.sav";
    StdSaveFileSystem fs;
    SaveSlotStore store(fs, bad);
    std::optional<SaveError> saveError;
    CHECK_NOTHROW(saveError = store.Save(Generation(1), kSchema));
    REQUIRE(saveError.has_value());
    CHECK(saveError->code == SaveErrorCode::Io);
    CHECK_FALSE(saveError->detail.empty());
    CHECK_NOTHROW((void)fs.Read(bad));
    CHECK_NOTHROW((void)fs.WriteSynced(bad, "x"));
    CHECK_NOTHROW((void)fs.Rename(bad, bad));
    CHECK_NOTHROW((void)fs.Remove(bad));
    CHECK_NOTHROW((void)store.Load(kSchema));
}
#endif
