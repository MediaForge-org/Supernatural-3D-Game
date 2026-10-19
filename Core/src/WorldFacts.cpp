#include <charconv>
#include <limits>
#include <spn/core/WorldFacts.h>
#include <system_error>
#include <utility>

namespace spn::core {

namespace {

constexpr std::string_view kMagic = "SPNFACTS";
constexpr char kTab = '\t';
constexpr std::size_t kChangeFieldCount = 8;

// --- Text encoding ----------------------------------------------------------
// Every token is canonical: decoding then re-encoding must reproduce the input
// exactly, so Serialize(Deserialize(x)) == x for every accepted x.

std::string EncodeString(std::string_view text) {
    constexpr std::string_view kHex = "0123456789abcdef";
    std::string out;
    out.reserve(text.size() + 2);
    out.push_back('"');
    for (const char ch : text) {
        const auto byte = static_cast<unsigned char>(ch);
        switch (ch) {
        case '\\':
            out += "\\\\";
            break;
        case '"':
            out += "\\\"";
            break;
        case '\n':
            out += "\\n";
            break;
        case '\r':
            out += "\\r";
            break;
        case '\t':
            out += "\\t";
            break;
        default:
            if (byte < 0x20 || byte == 0x7f) {
                out += "\\x";
                out.push_back(kHex[byte >> 4]);
                out.push_back(kHex[byte & 0x0f]);
            } else {
                out.push_back(ch);
            }
        }
    }
    out.push_back('"');
    return out;
}

int HexValue(char ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    }
    return -1;
}

std::optional<std::string> DecodeString(std::string_view token) {
    if (token.size() < 2 || token.front() != '"' || token.back() != '"') {
        return std::nullopt;
    }
    const std::string_view body = token.substr(1, token.size() - 2);
    std::string out;
    out.reserve(body.size());
    for (std::size_t i = 0; i < body.size(); ++i) {
        const char ch = body[i];
        if (ch == '"') {
            return std::nullopt;
        }
        if (ch != '\\') {
            out.push_back(ch);
            continue;
        }
        if (++i >= body.size()) {
            return std::nullopt;
        }
        switch (body[i]) {
        case '\\':
            out.push_back('\\');
            break;
        case '"':
            out.push_back('"');
            break;
        case 'n':
            out.push_back('\n');
            break;
        case 'r':
            out.push_back('\r');
            break;
        case 't':
            out.push_back('\t');
            break;
        case 'x': {
            if (i + 2 >= body.size()) { // needs two hex digits at i+1 and i+2
                return std::nullopt;
            }
            const int hi = HexValue(body[i + 1]);
            const int lo = HexValue(body[i + 2]);
            if (hi < 0 || lo < 0) {
                return std::nullopt;
            }
            out.push_back(static_cast<char>(hi * 16 + lo));
            i += 2;
            break;
        }
        default:
            return std::nullopt;
        }
    }
    if (EncodeString(out) != token) {
        return std::nullopt;
    }
    return out;
}

std::string EncodeValue(const FactValue& value) {
    if (const bool* b = std::get_if<bool>(&value)) {
        return *b ? "b:true" : "b:false";
    }
    if (const std::int64_t* i = std::get_if<std::int64_t>(&value)) {
        return "i:" + std::to_string(*i);
    }
    return "s:" + EncodeString(std::get<std::string>(value));
}

std::string EncodeOptionalValue(const std::optional<FactValue>& value) {
    return value ? EncodeValue(*value) : std::string("-");
}

template <typename Int> std::optional<Int> ParseCanonicalInteger(std::string_view text) {
    Int value{};
    const char* const end = text.data() + text.size();
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage): the end pointer is passed.
    const auto [ptr, ec] = std::from_chars(text.data(), end, value);
    if (ec != std::errc{} || ptr != end || std::to_string(value) != text) {
        return std::nullopt;
    }
    return value;
}

std::optional<FactValue> DecodeValue(std::string_view token) {
    if (token == "b:true") {
        return FactValue{true};
    }
    if (token == "b:false") {
        return FactValue{false};
    }
    if (token.starts_with("i:")) {
        if (const auto value = ParseCanonicalInteger<std::int64_t>(token.substr(2))) {
            return FactValue{*value};
        }
        return std::nullopt;
    }
    if (token.starts_with("s:")) {
        if (auto text = DecodeString(token.substr(2))) {
            return FactValue{std::move(*text)};
        }
    }
    return std::nullopt;
}

/// Result of decoding a previous/current slot: valid syntax, and either a value
/// or the "-" (none) token.
struct OptionalValueToken {
    bool valid = false;
    std::optional<FactValue> value;
};

OptionalValueToken DecodeOptionalValue(std::string_view token) {
    if (token == "-") {
        return {.valid = true, .value = std::nullopt};
    }
    if (auto value = DecodeValue(token)) {
        return {.valid = true, .value = std::move(value)};
    }
    return {};
}

std::vector<std::string_view> SplitTabs(std::string_view line) {
    std::vector<std::string_view> fields;
    std::size_t start = 0;
    while (true) {
        const std::size_t tab = line.find(kTab, start);
        if (tab == std::string_view::npos) {
            fields.push_back(line.substr(start));
            return fields;
        }
        fields.push_back(line.substr(start, tab - start));
        start = tab + 1;
    }
}

// --- Record encoding --------------------------------------------------------

struct EncodedCommand {
    std::string_view op;
    std::string_view key;
    std::string arg;
};

EncodedCommand EncodeCommand(const FactCommand& command) {
    if (const auto* set = std::get_if<SetFact>(&command)) {
        return {"set", set->key, EncodeValue(set->value)};
    }
    if (const auto* remove = std::get_if<RemoveFact>(&command)) {
        return {"remove", remove->key, "-"};
    }
    const auto& adjust = std::get<AdjustInt>(command);
    return {"adjust", adjust.key, EncodeValue(FactValue{adjust.delta})};
}

std::optional<FactCommand> DecodeCommand(std::string_view op, std::string_view key,
                                         std::string_view arg) {
    if (op == "set") {
        if (auto value = DecodeValue(arg)) {
            return FactCommand{SetFact{std::string(key), std::move(*value)}};
        }
    } else if (op == "remove") {
        if (arg == "-") {
            return FactCommand{RemoveFact{std::string(key)}};
        }
    } else if (op == "adjust") {
        if (const auto value = DecodeValue(arg)) {
            if (const auto* delta = std::get_if<std::int64_t>(&*value)) {
                return FactCommand{AdjustInt{std::string(key), *delta}};
            }
        }
    }
    return std::nullopt;
}

FactError Fail(FactErrorCode code, std::size_t line) {
    return FactError{code, line};
}

} // namespace

bool IsValidFactKey(std::string_view key) noexcept {
    if (key.empty() || key.size() > kMaxFactKeyLength) {
        return false;
    }
    const auto isLower = [](char ch) { return ch >= 'a' && ch <= 'z'; };
    if (!isLower(key.front())) {
        return false;
    }
    for (const char ch : key) {
        const bool isDigit = ch >= '0' && ch <= '9';
        if (!isLower(ch) && !isDigit && ch != '_' && ch != '.') {
            return false;
        }
    }
    return true;
}

std::string_view ToString(FactErrorCode code) noexcept {
    switch (code) {
    case FactErrorCode::InvalidKey:
        return "invalid key";
    case FactErrorCode::ValueTooLong:
        return "value too long";
    case FactErrorCode::CauseTooLong:
        return "cause too long";
    case FactErrorCode::KeyNotFound:
        return "key not found";
    case FactErrorCode::TypeMismatch:
        return "type mismatch";
    case FactErrorCode::IntegerOverflow:
        return "integer overflow";
    case FactErrorCode::InvalidHeader:
        return "invalid header";
    case FactErrorCode::UnsupportedVersion:
        return "unsupported version";
    case FactErrorCode::TruncatedData:
        return "truncated data";
    case FactErrorCode::TrailingData:
        return "trailing data";
    case FactErrorCode::MalformedLine:
        return "malformed line";
    case FactErrorCode::InvalidToken:
        return "invalid token";
    case FactErrorCode::SequenceMismatch:
        return "sequence mismatch";
    case FactErrorCode::JournalMismatch:
        return "journal mismatch";
    case FactErrorCode::CountMismatch:
        return "count mismatch";
    case FactErrorCode::SequenceOutOfRange:
        return "sequence out of range";
    }
    return "unknown error";
}

const FactValue* WorldFacts::Find(std::string_view key) const {
    const auto it = facts_.find(key);
    return it != facts_.end() ? &it->second : nullptr;
}

Result<ChangeRecord, FactError> WorldFacts::Apply(const FactCommand& command,
                                                  std::string_view cause) {
    if (cause.size() > kMaxCauseLength) {
        return Fail(FactErrorCode::CauseTooLong, 0);
    }

    // Validate everything and compute the outcome first; mutate only at the end.
    const std::string& key =
        std::visit([](const auto& c) -> const std::string& { return c.key; }, command);
    if (!IsValidFactKey(key)) {
        return Fail(FactErrorCode::InvalidKey, 0);
    }
    const auto existing = facts_.find(key);
    const bool exists = existing != facts_.end();

    std::optional<FactValue> previous;
    if (exists) {
        previous = existing->second;
    }
    std::optional<FactValue> current;

    if (const auto* set = std::get_if<SetFact>(&command)) {
        if (const auto* text = std::get_if<std::string>(&set->value);
            text != nullptr && text->size() > kMaxFactStringLength) {
            return Fail(FactErrorCode::ValueTooLong, 0);
        }
        if (exists && existing->second.index() != set->value.index()) {
            return Fail(FactErrorCode::TypeMismatch, 0);
        }
        current = set->value;
    } else if (std::holds_alternative<RemoveFact>(command)) {
        if (!exists) {
            return Fail(FactErrorCode::KeyNotFound, 0);
        }
    } else {
        const auto& adjust = std::get<AdjustInt>(command);
        if (!exists) {
            return Fail(FactErrorCode::KeyNotFound, 0);
        }
        const auto* value = std::get_if<std::int64_t>(&existing->second);
        if (value == nullptr) {
            return Fail(FactErrorCode::TypeMismatch, 0);
        }
        constexpr auto kMin = std::numeric_limits<std::int64_t>::min();
        constexpr auto kMax = std::numeric_limits<std::int64_t>::max();
        if ((adjust.delta > 0 && *value > kMax - adjust.delta) ||
            (adjust.delta < 0 && *value < kMin - adjust.delta)) {
            return Fail(FactErrorCode::IntegerOverflow, 0);
        }
        current = FactValue{*value + adjust.delta};
    }

    if (current) {
        facts_.insert_or_assign(key, *current);
    } else {
        facts_.erase(key);
    }
    journal_.push_back(ChangeRecord{journal_.size() + 1, command, std::move(previous),
                                    std::move(current), std::string(cause)});
    return journal_.back();
}

Result<WorldFacts, FactError> WorldFacts::StateAt(std::uint64_t sequence) const {
    if (sequence > Revision()) {
        return Fail(FactErrorCode::SequenceOutOfRange, 0);
    }
    WorldFacts state;
    for (std::size_t index = 0; index < sequence; ++index) {
        const ChangeRecord& record = journal_[index];
        const auto applied = state.Apply(record.command, record.cause);
        if (!applied) {
            return Fail(applied.Error().code, 0);
        }
        if (applied.Value() != record) {
            return Fail(FactErrorCode::JournalMismatch, 0);
        }
    }
    return state;
}

std::string WorldFacts::Serialize() const {
    std::string out;
    out += kMagic;
    out += ' ';
    out += std::to_string(kFactsSaveFormatVersion);
    out += '\n';
    for (const ChangeRecord& record : journal_) {
        const EncodedCommand encoded = EncodeCommand(record.command);
        out += "change";
        out += kTab;
        out += std::to_string(record.sequence);
        out += kTab;
        out += encoded.op;
        out += kTab;
        out += encoded.key;
        out += kTab;
        out += encoded.arg;
        out += kTab;
        out += EncodeOptionalValue(record.previous);
        out += kTab;
        out += EncodeOptionalValue(record.current);
        out += kTab;
        out += EncodeString(record.cause);
        out += '\n';
    }
    out += "end";
    out += kTab;
    out += std::to_string(journal_.size());
    out += '\n';
    return out;
}

namespace {

/// Parses one "change" line and replays it into `facts`.
std::optional<FactError> ReplayChangeLine(WorldFacts& facts, std::string_view line,
                                          std::size_t lineNumber) {
    const std::vector<std::string_view> fields = SplitTabs(line);
    if (fields.size() != kChangeFieldCount) {
        return Fail(FactErrorCode::MalformedLine, lineNumber);
    }
    const auto sequence = ParseCanonicalInteger<std::uint64_t>(fields[1]);
    if (!sequence) {
        return Fail(FactErrorCode::InvalidToken, lineNumber);
    }
    if (*sequence != facts.Revision() + 1) {
        return Fail(FactErrorCode::SequenceMismatch, lineNumber);
    }
    const auto command = DecodeCommand(fields[2], fields[3], fields[4]);
    const auto previous = DecodeOptionalValue(fields[5]);
    const auto current = DecodeOptionalValue(fields[6]);
    const auto cause = DecodeString(fields[7]);
    if (!command || !previous.valid || !current.valid || !cause) {
        return Fail(FactErrorCode::InvalidToken, lineNumber);
    }

    const auto applied = facts.Apply(*command, *cause);
    if (!applied) {
        return Fail(applied.Error().code, lineNumber);
    }
    const ChangeRecord& record = applied.Value();
    if (record.previous != previous.value || record.current != current.value) {
        // No rollback needed: a failed load discards the whole partially built store.
        return Fail(FactErrorCode::JournalMismatch, lineNumber);
    }
    return std::nullopt;
}

} // namespace

Result<WorldFacts, FactError> WorldFacts::Deserialize(std::string_view text) {
    WorldFacts facts;
    std::size_t position = 0;
    std::size_t lineNumber = 0;

    while (position < text.size()) {
        const std::size_t newline = text.find('\n', position);
        ++lineNumber;
        if (newline == std::string_view::npos) {
            return Fail(lineNumber == 1 ? FactErrorCode::InvalidHeader
                                        : FactErrorCode::TruncatedData,
                        lineNumber);
        }
        const std::string_view line = text.substr(position, newline - position);
        position = newline + 1;

        if (lineNumber == 1) {
            const std::string prefix = std::string(kMagic) + ' ';
            if (!line.starts_with(prefix)) {
                return Fail(FactErrorCode::InvalidHeader, lineNumber);
            }
            const auto version = ParseCanonicalInteger<std::uint64_t>(line.substr(prefix.size()));
            if (!version) {
                return Fail(FactErrorCode::InvalidHeader, lineNumber);
            }
            if (*version != kFactsSaveFormatVersion) {
                return Fail(FactErrorCode::UnsupportedVersion, lineNumber);
            }
            continue;
        }

        if (line.starts_with("end")) {
            const std::vector<std::string_view> fields = SplitTabs(line);
            if (fields.size() != 2 || fields[0] != "end") {
                return Fail(FactErrorCode::MalformedLine, lineNumber);
            }
            const auto count = ParseCanonicalInteger<std::uint64_t>(fields[1]);
            if (!count) {
                return Fail(FactErrorCode::InvalidToken, lineNumber);
            }
            if (*count != facts.Revision()) {
                return Fail(FactErrorCode::CountMismatch, lineNumber);
            }
            if (position != text.size()) {
                return Fail(FactErrorCode::TrailingData, lineNumber + 1);
            }
            return facts;
        }

        if (!line.starts_with("change\t")) {
            return Fail(FactErrorCode::MalformedLine, lineNumber);
        }
        if (const auto error = ReplayChangeLine(facts, line, lineNumber)) {
            return *error;
        }
    }
    return Fail(lineNumber == 0 ? FactErrorCode::InvalidHeader : FactErrorCode::TruncatedData,
                lineNumber);
}

} // namespace spn::core
