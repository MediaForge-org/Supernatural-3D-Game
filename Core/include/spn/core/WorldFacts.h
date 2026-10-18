#pragma once

// WorldFacts: deterministic store of named facts with an explicit command API,
// a change journal and a versioned text save format.
//
// ENGINEERING PROPOSAL, not master canon. Key rules, size limits, the format and
// the "type is fixed per key" rule are proposals for review. The store makes no
// assumption about who is allowed to issue commands (co-op authority is OPEN).

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <spn/core/Result.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace spn::core {

// Proposed limits (placeholders, not tuned values).
inline constexpr std::size_t kMaxFactKeyLength = 128;
inline constexpr std::size_t kMaxFactStringLength = 4096;
inline constexpr std::size_t kMaxCauseLength = 128;
inline constexpr std::uint64_t kFactsSaveFormatVersion = 1;

/// Fact values. Note: a string literal selects std::string (C++20), never bool.
using FactValue = std::variant<bool, std::int64_t, std::string>;

/// Valid key: 1..kMaxFactKeyLength chars, first [a-z], rest [a-z0-9_.].
[[nodiscard]] bool IsValidFactKey(std::string_view key) noexcept;

// --- Commands: the only way to change the store ----------------------------

/// Creates the fact or replaces its value. The value type of an existing key
/// cannot change (Remove it first).
struct SetFact {
    std::string key;
    FactValue value;
    friend bool operator==(const SetFact&, const SetFact&) = default;
};

struct RemoveFact {
    std::string key;
    friend bool operator==(const RemoveFact&, const RemoveFact&) = default;
};

/// Adds `delta` to an existing integer fact (overflow is an error).
struct AdjustInt {
    std::string key;
    std::int64_t delta = 0;
    friend bool operator==(const AdjustInt&, const AdjustInt&) = default;
};

using FactCommand = std::variant<SetFact, RemoveFact, AdjustInt>;

// --- Journal ----------------------------------------------------------------

/// One applied command. Only successful commands are journaled.
struct ChangeRecord {
    std::uint64_t sequence = 0; // 1-based, gapless
    FactCommand command;
    std::optional<FactValue> previous; // value before (nullopt: fact did not exist)
    std::optional<FactValue> current;  // value after (nullopt: fact was removed)
    std::string cause;                 // free-form audit text, no semantics
    friend bool operator==(const ChangeRecord&, const ChangeRecord&) = default;
};

// --- Errors -----------------------------------------------------------------

enum class FactErrorCode : std::uint8_t {
    // Applying a command
    InvalidKey,
    ValueTooLong,
    CauseTooLong,
    KeyNotFound,
    TypeMismatch,
    IntegerOverflow,
    // Loading saved data
    InvalidHeader,
    UnsupportedVersion,
    TruncatedData,
    TrailingData,
    MalformedLine,
    InvalidToken,
    SequenceMismatch,
    JournalMismatch,
    CountMismatch,
};

[[nodiscard]] std::string_view ToString(FactErrorCode code) noexcept;

struct FactError {
    FactErrorCode code;
    std::size_t line = 0; // 1-based line in saved data; 0 when not applicable
    friend bool operator==(const FactError&, const FactError&) = default;
};

// --- Store ------------------------------------------------------------------

class WorldFacts {
public:
    /// Validates and applies a command atomically: on error the store and the
    /// journal are unchanged. On success returns the journaled record.
    Result<ChangeRecord, FactError> Apply(const FactCommand& command, std::string_view cause = {});

    [[nodiscard]] const FactValue* Find(std::string_view key) const;

    /// Typed lookup; nullptr if the fact is missing or has another type.
    template <typename T> [[nodiscard]] const T* FindAs(std::string_view key) const {
        const FactValue* value = Find(key);
        return value != nullptr ? std::get_if<T>(value) : nullptr;
    }

    /// All facts, ordered by key (deterministic).
    [[nodiscard]] const std::map<std::string, FactValue, std::less<>>& Facts() const noexcept {
        return facts_;
    }
    [[nodiscard]] const std::vector<ChangeRecord>& Journal() const noexcept { return journal_; }
    /// Sequence number of the latest change; 0 for an empty store.
    [[nodiscard]] std::uint64_t Revision() const noexcept { return journal_.size(); }

    /// Deterministic text form of the journal; the state is derived by replay.
    [[nodiscard]] std::string Serialize() const;
    /// Replays and verifies saved data. Never partially succeeds.
    [[nodiscard]] static Result<WorldFacts, FactError> Deserialize(std::string_view text);

    friend bool operator==(const WorldFacts&, const WorldFacts&) = default;

private:
    std::map<std::string, FactValue, std::less<>> facts_;
    std::vector<ChangeRecord> journal_;
};

} // namespace spn::core
