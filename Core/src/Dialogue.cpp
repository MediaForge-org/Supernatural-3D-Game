#include <spn/core/Dialogue.h>

#include <set>

namespace spn::core {

namespace {

constexpr std::size_t kMaxTextKeyLength = 96;
constexpr std::string_view kPlayer = "player";
constexpr std::string_view kCauseStart = "dialogue:start";
constexpr std::string_view kCauseChoose = "dialogue:choose";

// Fact namespaces that belong to other investigation modules. A dialogue effect writing them would
// bypass their rules (EVD-01: journal registration only after collecting; the dialogue state machine).
constexpr std::string_view kReservedPrefixes[] = {
    "placeholder.journal.", "placeholder.dialogue.", "placeholder.evidence.", "placeholder.assist.", "placeholder.case.",
};

bool IsReservedEffectKey(std::string_view key) noexcept {
    for (const auto prefix : kReservedPrefixes) {
        if (key.substr(0, prefix.size()) == prefix) return true;
    }
    return false;
}

bool IsValidTextKey(std::string_view key) noexcept {
    if (key.empty() || key.size() > kMaxTextKeyLength) return false;
    if (!(key.front() >= 'a' && key.front() <= 'z')) return false;
    for (const char c : key) {
        const bool ok = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '.';
        if (!ok) return false;
    }
    return true;
}

std::string NodeKey(std::string_view dialogueId) { return "placeholder.dialogue." + std::string(dialogueId) + ".node"; }

// Reads the dialogue state from the facts.
// Returns the current node id (empty = not started) and the done flag, or StateCorrupt.
struct State {
    std::string node;
    bool started = false;
    bool done = false;
};

Result<State, DialogueError> ReadState(const WorldFacts& facts, std::string_view dialogueId) {
    State state;
    const FactValue* nodeValue = facts.Find(NodeKey(dialogueId));
    if (nodeValue != nullptr) {
        const std::string* text = std::get_if<std::string>(nodeValue);
        if (text == nullptr) return DialogueError{DialogueErrorCode::StateCorrupt, std::nullopt};
        state.node = *text;
        state.started = true;
    }
    const FactValue* doneValue = facts.Find(DialogueDoneKey(dialogueId));
    if (doneValue != nullptr) {
        const bool* flag = std::get_if<bool>(doneValue);
        if (flag == nullptr) return DialogueError{DialogueErrorCode::StateCorrupt, std::nullopt};
        state.done = *flag;
    }
    return state;
}

bool ConditionHolds(const DialogueCondition& condition, const WorldFacts& facts) {
    const FactValue* value = facts.Find(condition.key);
    switch (condition.kind) {
        case DialogueCondition::Kind::FactTrue: {
            const bool* flag = value != nullptr ? std::get_if<bool>(value) : nullptr;
            return flag != nullptr && *flag;
        }
        case DialogueCondition::Kind::FactAbsent: return value == nullptr;
        case DialogueCondition::Kind::FactEquals: return value != nullptr && *value == condition.value;
    }
    return false;
}

bool OptionAvailable(const DialogueOption& option, const WorldFacts& facts) {
    for (const auto& condition : option.conditions) {
        if (!ConditionHolds(condition, facts)) return false;
    }
    return true;
}

DialogueView MakeView(const DialogueNode& node, const WorldFacts& facts, bool ended) {
    DialogueView view;
    view.nodeId = node.id;
    view.speaker = node.speaker;
    view.textKey = node.textKey;
    view.ended = ended;
    if (!ended) {
        for (const auto& option : node.options) {
            if (OptionAvailable(option, facts)) view.availableOptions.push_back(option.id);
        }
    }
    return view;
}

} // namespace

Result<DialogueDef, DialogueDefError> DialogueDef::Create(std::string id, std::string npc, std::string start,
                                                          std::vector<DialogueNode> nodes) {
    if (!IsValidLocalId(id) || !IsValidLocalId(npc) || npc == kPlayer) return DialogueDefError::InvalidId;
    if (nodes.empty()) return DialogueDefError::EmptyGraph;
    std::set<std::string> nodeIds;
    for (const auto& node : nodes) {
        if (!IsValidLocalId(node.id)) return DialogueDefError::InvalidId;
        if (node.speaker != kPlayer && !IsValidLocalId(node.speaker)) return DialogueDefError::InvalidId;
        if (!IsValidTextKey(node.textKey)) return DialogueDefError::InvalidTextKey;
        if (!nodeIds.insert(node.id).second) return DialogueDefError::DuplicateNode;
    }
    if (!nodeIds.contains(start)) return DialogueDefError::UnknownStart;
    for (const auto& node : nodes) {
        std::set<std::string> optionIds;
        for (const auto& option : node.options) {
            if (!IsValidLocalId(option.id)) return DialogueDefError::InvalidId;
            if (!optionIds.insert(option.id).second) return DialogueDefError::DuplicateOption;
            if (!IsValidTextKey(option.textKey)) return DialogueDefError::InvalidTextKey;
            for (const auto& condition : option.conditions) {
                if (!IsValidFactKey(condition.key)) return DialogueDefError::InvalidFactKey;
            }
            for (const auto& effect : option.effects) {
                if (!IsValidFactKey(effect.key) || IsReservedEffectKey(effect.key)) return DialogueDefError::InvalidFactKey;
            }
            if (option.next.has_value() && !nodeIds.contains(*option.next)) return DialogueDefError::UnknownNext;
        }
    }
    return DialogueDef(std::move(id), std::move(npc), std::move(start), std::move(nodes));
}

const DialogueNode* DialogueDef::FindNode(std::string_view nodeId) const noexcept {
    for (const auto& node : nodes_) {
        if (node.id == nodeId) return &node;
    }
    return nullptr;
}

std::string DialogueNodeKey(std::string_view dialogueId) { return NodeKey(dialogueId); }
std::string DialogueDoneKey(std::string_view dialogueId) {
    return "placeholder.dialogue." + std::string(dialogueId) + ".done";
}

Result<std::string, DialogueErrorCode> NpcKnowsKey(std::string_view npcId, std::string_view topic) {
    if (!IsValidLocalId(npcId) || !IsValidLocalId(topic)) return DialogueErrorCode::InvalidId;
    return "npc." + std::string(npcId) + ".knows." + std::string(topic);
}

Result<DialogueView, DialogueError> DialogueStart(const DialogueDef& def, WorldFacts& facts, std::string_view cause) {
    const auto state = ReadState(facts, def.Id());
    if (!state) return state.Error();
    if (state.Value().done) return DialogueError{DialogueErrorCode::AlreadyDone, std::nullopt};
    if (state.Value().started) return DialogueCurrent(def, facts);
    const DialogueNode* start = def.FindNode(def.Start());
    if (start == nullptr) return DialogueError{DialogueErrorCode::StateCorrupt, std::nullopt};
    const auto applied = facts.Apply(SetFact{NodeKey(def.Id()), std::string(def.Start())}, cause.empty() ? kCauseStart : cause);
    if (!applied) return DialogueError{DialogueErrorCode::Store, applied.Error()};
    return MakeView(*start, facts, false);
}

Result<DialogueView, DialogueError> DialogueCurrent(const DialogueDef& def, const WorldFacts& facts) {
    const auto state = ReadState(facts, def.Id());
    if (!state) return state.Error();
    if (!state.Value().started) return DialogueError{DialogueErrorCode::NotStarted, std::nullopt};
    const DialogueNode* node = def.FindNode(state.Value().node);
    if (node == nullptr) return DialogueError{DialogueErrorCode::StateCorrupt, std::nullopt};
    return MakeView(*node, facts, state.Value().done);
}

Result<DialogueView, DialogueError> DialogueChoose(const DialogueDef& def, WorldFacts& facts, std::string_view optionId,
                                                   std::string_view cause) {
    if (!IsValidLocalId(optionId)) return DialogueError{DialogueErrorCode::InvalidId, std::nullopt};
    const auto state = ReadState(facts, def.Id());
    if (!state) return state.Error();
    if (state.Value().done) return DialogueError{DialogueErrorCode::AlreadyDone, std::nullopt};
    if (!state.Value().started) return DialogueError{DialogueErrorCode::NotStarted, std::nullopt};
    const DialogueNode* node = def.FindNode(state.Value().node);
    if (node == nullptr) return DialogueError{DialogueErrorCode::StateCorrupt, std::nullopt};

    const DialogueOption* chosen = nullptr;
    for (const auto& option : node->options) {
        if (option.id == optionId) chosen = &option;
    }
    if (chosen == nullptr) return DialogueError{DialogueErrorCode::UnknownOption, std::nullopt};
    if (!OptionAvailable(*chosen, facts)) return DialogueError{DialogueErrorCode::OptionUnavailable, std::nullopt};

    // Atomic: all writes go to a copy; the caller's store changes only if every write succeeds.
    WorldFacts next = facts;
    const std::string useCause = cause.empty() ? std::string(kCauseChoose) : std::string(cause);
    for (const auto& effect : chosen->effects) {
        const auto applied = next.Apply(SetFact{effect.key, effect.value}, useCause);
        if (!applied) return DialogueError{DialogueErrorCode::Store, applied.Error()};
    }
    if (chosen->next.has_value()) {
        const auto applied = next.Apply(SetFact{NodeKey(def.Id()), *chosen->next}, useCause);
        if (!applied) return DialogueError{DialogueErrorCode::Store, applied.Error()};
    } else {
        const auto applied = next.Apply(SetFact{DialogueDoneKey(def.Id()), true}, useCause);
        if (!applied) return DialogueError{DialogueErrorCode::Store, applied.Error()};
    }
    facts = std::move(next);
    return DialogueCurrent(def, facts);
}

bool IsDialogueDone(const WorldFacts& facts, std::string_view dialogueId) noexcept {
    const bool* flag = facts.FindAs<bool>(DialogueDoneKey(dialogueId));
    return flag != nullptr && *flag;
}

std::string_view ToString(DialogueErrorCode code) noexcept {
    switch (code) {
        case DialogueErrorCode::InvalidId: return "invalid_id";
        case DialogueErrorCode::NotStarted: return "not_started";
        case DialogueErrorCode::AlreadyDone: return "already_done";
        case DialogueErrorCode::UnknownOption: return "unknown_option";
        case DialogueErrorCode::OptionUnavailable: return "option_unavailable";
        case DialogueErrorCode::StateCorrupt: return "state_corrupt";
        case DialogueErrorCode::Store: return "store";
    }
    return "unknown";
}

std::string_view ToString(DialogueDefError code) noexcept {
    switch (code) {
        case DialogueDefError::InvalidId: return "invalid_id";
        case DialogueDefError::InvalidTextKey: return "invalid_text_key";
        case DialogueDefError::InvalidFactKey: return "invalid_fact_key";
        case DialogueDefError::EmptyGraph: return "empty_graph";
        case DialogueDefError::DuplicateNode: return "duplicate_node";
        case DialogueDefError::DuplicateOption: return "duplicate_option";
        case DialogueDefError::UnknownStart: return "unknown_start";
        case DialogueDefError::UnknownNext: return "unknown_next";
    }
    return "unknown";
}

} // namespace spn::core
