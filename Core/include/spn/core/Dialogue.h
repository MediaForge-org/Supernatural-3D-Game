#pragma once

// Minimal dialogue core (SPN-DEV-02, decisions DLG-01..04 and NPCA-01..03, Docs/Planning/DECISIONS.md section J).
//
// A dialogue is a graph of nodes. Each node shows one line of an NPC and offers options.
// An option is available when all its conditions hold on WorldFacts; choosing it applies its
// effects (WorldFacts writes) and moves to the next node, or ends the dialogue.
//
// Scope of the prototype (NPCA-02, DLG-02): fixed options only. No free text, no language model
// (DLG-01 names free text as a later part of the hybrid system; DLG-04 forbids a runtime LLM).
// Lines are referenced by text keys (strings), so localisation and a later dynamic source can
// replace them without changing the graph.
//
// Extension point (not implemented): a dynamic line source may supply the text key of a node
// or option at view time. The graph and the state keys below do not change for that.
//
// State lives only in WorldFacts (saves with the store):
//   placeholder.dialogue.<dialogue>.node   string  current node id; absent = not started
//   placeholder.dialogue.<dialogue>.done   bool    true after an option without a next node
// NPC knowledge is ordinary facts set by effects (NPCA-03: nothing is generated automatically):
//   npc.<npc>.knows.<topic>                bool
// Conditions read facts only, so this module does not depend on NpcMemory or on the case logic.

#include <cstdint>
#include <optional>
#include <spn/core/InvestigationIds.h>
#include <spn/core/Result.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace spn::core {

struct DialogueCondition {
    enum class Kind : std::uint8_t { FactTrue, FactAbsent, FactEquals };
    Kind kind = Kind::FactTrue;
    std::string key;   ///< a valid fact key
    FactValue value{}; ///< used by FactEquals only
};

struct DialogueEffect {
    std::string key;   ///< a valid fact key
    FactValue value;   ///< written with SetFact when the option is chosen
};

struct DialogueOption {
    std::string id;                          ///< local id, unique within its node
    std::string textKey;                     ///< text key, [a-z][a-z0-9_.]*
    std::vector<DialogueCondition> conditions; ///< all must hold for the option to be offered
    std::vector<DialogueEffect> effects;     ///< applied on choice
    std::optional<std::string> next;         ///< next node id; nullopt = the dialogue ends
};

struct DialogueNode {
    std::string id;       ///< local id, unique within the dialogue
    std::string speaker;  ///< NPC id (local id) or "player"
    std::string textKey;  ///< text key of the line
    std::vector<DialogueOption> options; ///< empty = the node ends the dialogue after its line
};

enum class DialogueDefError : std::uint8_t {
    InvalidId, InvalidTextKey, InvalidFactKey, EmptyGraph, DuplicateNode, DuplicateOption, UnknownStart, UnknownNext
};

/// A validated dialogue graph. Immutable after Create.
class DialogueDef {
public:
    [[nodiscard]] static Result<DialogueDef, DialogueDefError> Create(std::string id, std::string npc, std::string start,
                                                                      std::vector<DialogueNode> nodes);
    [[nodiscard]] const std::string& Id() const noexcept { return id_; }
    [[nodiscard]] const std::string& Npc() const noexcept { return npc_; }
    [[nodiscard]] const std::string& Start() const noexcept { return start_; }
    [[nodiscard]] const DialogueNode* FindNode(std::string_view nodeId) const noexcept;
    [[nodiscard]] const std::vector<DialogueNode>& Nodes() const noexcept { return nodes_; }

private:
    DialogueDef(std::string id, std::string npc, std::string start, std::vector<DialogueNode> nodes)
        : id_(std::move(id)), npc_(std::move(npc)), start_(std::move(start)), nodes_(std::move(nodes)) {}
    std::string id_, npc_, start_;
    std::vector<DialogueNode> nodes_;
};

enum class DialogueErrorCode : std::uint8_t {
    InvalidId,          ///< a local id argument is malformed
    NotStarted,         ///< a choice was made before the dialogue started (use DialogueStart)
    AlreadyDone,        ///< the dialogue has ended; no further choice is accepted
    UnknownOption,      ///< the option id does not exist on the current node
    OptionUnavailable,  ///< the option exists but its conditions do not hold
    StateCorrupt,       ///< a stored dialogue fact has the wrong type or refers to a missing node
    Store               ///< WorldFacts refused a write; nothing was changed
};

struct DialogueError {
    DialogueErrorCode code = DialogueErrorCode::StateCorrupt;
    std::optional<FactError> fact; ///< only for code == Store
    friend bool operator==(const DialogueError&, const DialogueError&) = default;
};

/// What the player sees now: the node, and the options available on it (definition order).
struct DialogueView {
    std::string nodeId;
    std::string speaker;
    std::string textKey;
    std::vector<std::string> availableOptions;
    bool ended = false;
    friend bool operator==(const DialogueView&, const DialogueView&) = default;
};

[[nodiscard]] std::string DialogueNodeKey(std::string_view dialogueId);
[[nodiscard]] std::string DialogueDoneKey(std::string_view dialogueId);
/// Fact key for an NPC's knowledge of a topic (the only NPC-knowledge convention in this module).
[[nodiscard]] Result<std::string, DialogueErrorCode> NpcKnowsKey(std::string_view npcId, std::string_view topic);

/// Starts the dialogue (sets the node to the start node). Idempotent: an already started dialogue
/// returns its current view and writes nothing. Refused (AlreadyDone) once it has ended.
[[nodiscard]] Result<DialogueView, DialogueError> DialogueStart(const DialogueDef& def, WorldFacts& facts,
                                                                std::string_view cause = {});
/// The current view, read-only. Refused (NotStarted) if the dialogue has not started.
[[nodiscard]] Result<DialogueView, DialogueError> DialogueCurrent(const DialogueDef& def, const WorldFacts& facts);
/// Chooses an option. Atomic: on any error the facts are unchanged. Returns the view after the choice.
[[nodiscard]] Result<DialogueView, DialogueError> DialogueChoose(const DialogueDef& def, WorldFacts& facts,
                                                                 std::string_view optionId, std::string_view cause = {});

[[nodiscard]] bool IsDialogueDone(const WorldFacts& facts, std::string_view dialogueId) noexcept;
[[nodiscard]] std::string_view ToString(DialogueErrorCode code) noexcept;
[[nodiscard]] std::string_view ToString(DialogueDefError code) noexcept;

} // namespace spn::core
