#pragma once

// Deterministic, versioned fingerprint of the gameplay-relevant part of an ItemCatalog (M2B3.4). ENGINEERING PROPOSAL, not master canon.
//
// WHY: a saved inventory (SPN-INVENTORY 1) refers to item definitions by id but does not store them. If the definitions change between
// the save and the load (stack limit, weight, equipment slots ...), the inventory may still parse - and silently mean something else, or
// fail in a confusing way. The fingerprint is written next to the inventory ("slot.catalog" section, see GameSave.h) so that a load can tell
// "same definitions" from "changed definitions" instead of guessing.
//
// WHAT IS HASHED (gameplay-relevant only): for every definition, ordered by id: id, category, maxStack, weightGrams, volumeMilliliters and the
// equip slots (ordered). NOT hashed: anything visual or presentational (names, icons, meshes, sounds - they live in Unreal data assets and
// never reach the Core), container presets (a saved container carries its own rules), and the file the catalog was read from. The input is the
// parsed, validated ItemCatalog, so JSON formatting, key order and whitespace cannot matter.
//
// FORMAT (version 1): canonical text
//     SPN-CATALOG-FP 1\n
//     def <id> <category> <maxStack> <weightGrams> <volumeMilliliters> <slot,slot,...|->\n     (one line per definition, ascending id)
// hashed with SHA-256; the fingerprint is "fp1:" + 64 lowercase hex digits. The "1" is the version of the canonical form AND of the hash
// choice: any change to the canonical text or the hash must introduce "fp2:" and keep "fp1:" readable. Integers only, no floating point,
// no locale, no endianness dependence: identical on every platform.

#include <string>
#include <string_view>

#include "spn/core/Inventory.h"

namespace spn::core {

inline constexpr std::string_view kCatalogFingerprintPrefix = "fp1:";

/// SHA-256 (FIPS 180-4) of the bytes, as 64 lowercase hex digits. Exposed for the tests (known-answer vectors).
[[nodiscard]] std::string Sha256Hex(std::string_view data);

/// The text that is hashed (see above). Exposed for diagnostics and tests.
[[nodiscard]] std::string CanonicalCatalogText(const ItemCatalog& catalog);

/// "fp1:<64 hex digits>".
[[nodiscard]] std::string CatalogFingerprint(const ItemCatalog& catalog);

/// True for "fp<version>:" followed by exactly 64 lowercase hex digits (any version number: a newer fingerprint is syntactically fine but
/// not comparable; see IsComparableFingerprint).
[[nodiscard]] bool IsWellFormedFingerprint(std::string_view text) noexcept;

/// True if this build can compute the given fingerprint's version (currently only fp1).
[[nodiscard]] bool IsComparableFingerprint(std::string_view text) noexcept;

} // namespace spn::core
