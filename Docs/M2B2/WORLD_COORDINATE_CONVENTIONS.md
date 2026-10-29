# M2B2 — World Coordinate Conventions

Status legend used in this document and in the other M2B2 documents:

| Word | Meaning |
|---|---|
| **Implemented** | code exists in this repository |
| **Tested** | an automated test in this repository ran and passed (see `VALIDATION.md` for the counts) |
| **Proposed** | our engineering proposal; not approved, not master canon |
| **Unverified** | written but never executed or checked against the real counterpart (Unreal, Windows, WorldForge) |

Code: `Core/include/spn/core/Coordinates.h`, `Core/src/Coordinates.cpp`. Tests: `Tests/Core/CoordinateTests.cpp`.

## 1. Master basis (consultation protocol, CLAUDE.md section 11)

| Finding | Source | Status in the source |
|---|---|---|
| WorldForge calculates in **metres** | `Docs/Game/Fahrzeuge-und-Verkehr/24-WorldForge-Schnittstelle.md` (row "Einheiten", `VEH-DEC-WF-006`, master ref `WF-DC01`) | 🧭 RICHTUNG (direction), "Architekturvertrag (geplant)" |
| The **axis convention is not fixed** | same file; open question `CQ-101` ("Achsenkonvention und Einheit im Spiel") | OPEN |
| Mesh/spline export references the same RoadEdge/Crossing ids **and height coordinates** | same file, `VEH-DEC-WF-004` / `WF-RC01` | 🧭 RICHTUNG |
| Map acceptance wants one authoritative CRS with documented units, bounds and scale | `99_DAILY_DELTAS/2026-10-19/.../MAP_ACCEPTANCE_CHECKS_2026-10-19.md` | review-only delta, not canon |
| Nothing found in the current canonical state about a world origin, Unreal axis mapping or Unreal-side units | `bash Tools/master-search.sh "world origin" "coordinate system" "Unreal axis" WF-DC01` (tier 1: no hits) | — |

Consequence: **metres is the only part with a master direction. Every axis statement below is a proposal.** The WorldForge repository was not available and was not modified.

## 2. What the existing code already used (compatibility review)

| System | Convention found | Source |
|---|---|---|
| `Vec2` / `Vec3` | "Units are metres unless a function says otherwise; the Unreal adapters convert from/to centimetres" | `Vec.h` header comment |
| World map (M2B1) | metres, `double`, x = east, y = north, hard limit ±10,000 km | `WorldMap.h`, `SpatialGrid.h` (`kMaxMapCoordinateMetres = 1e7`) |
| Minimap (M2B1) | heading = compass bearing (clockwise from north); screen space y-down, origin at the viewport centre | `Minimap.h` |
| Navigation (M2B1) | same planar metres as the map; segments carry a layer | `Navigation.h` |
| Movement / interaction | SI units, metres; `z` is height | `Movement.h`, `InteractionReach.h` (`Vec3`) |
| Unreal side | cm, no Core adapter yet. `SpnMovement.cpp` scales cm↔m locally | `Unreal/.../SpnMovement.cpp` |

**No conflict.** The canonical model below is exactly what the Core already did, so no stored value is reinterpreted (no save file contains world positions in another unit: `SPN-MAPSTATE 1` stores metres x/y, which keep their meaning).

**Discrepancy documented:** the M2B1 doc says the Unreal axis mapping is "not defined yet". It still is not *decided*; M2B2 adds two selectable presets (section 4) and leaves the decision open.

## 3. Canonical model — **Implemented, Tested**

```
unit      metre, IEEE double
x         east-positive          (Vec3::x)
y         north-positive         (Vec3::y)
z         up-positive            elevation in metres inside one layer
origin    none baked in; every origin is an explicit parameter of an adapter
bearing   compass bearing, clockwise from north, radians
layer     independent plane (surface, underground, interior floor, realm) — NOT an elevation
```

* `WorldPoint = Vec3` (alias). `IsValidWorldPoint` / `ValidateWorldPoint`: all components finite, |x|,|y| ≤ `kMaxMapCoordinateMetres` (10,000 km), |z| ≤ `kMaxWorldElevationMetres` (100 km). The two bounds are **arithmetic safety limits, not game rules** (they keep cell indices and squared distances exact and reject garbage); they do not describe the world size (~2,200 km² is an open planning scenario, about 47 km across).
* Layer semantics: a layer is a separate coordinate plane (M2B1). Moving between layers is a *transition* (navigation `LayerTransition` segment), never a change of `z`. `ToMapPosition(layer, point)` drops `z`; `FromMapPosition(position, elevation)` adds it back.
* Errors are values (`CoordinateErrorCode`: `NonFinite`, `OutOfRange`, `InvalidFrame`); nothing wraps, clamps or throws.

### Why doubles and what precision means

float32 has a 24-bit mantissa. `Float32StepMetres(m)` returns the spacing of float32 values at distance `m` (measured, not assumed):

| distance from the float's origin | float32 step |
|---|---|
| 1,000 m | < 0.1 mm (`< 1e-4 m`, tested) |
| 9,000,000 m | ≥ 1 m (tested) |

So positions of a 2,200 km² world (about 47 km across) in float32 *world* coordinates would be coarse at the edges; doubles for world data and **float32 only inside a local frame** (section 5) is the rule. Tested: a round trip of points at ±10,000 km through the Unreal adapter loses < 1 µm; for points in a 47 km world < 1 nm (`CoordinateTests.cpp`, "round trip at the edge of the world").

## 4. Unreal adapter — **Implemented, Tested (math only); Unverified in Unreal**

Unreal: left-handed, X forward, Y right, Z up, **centimetres**. The canonical frame (east, north, up) is right-handed, so a correct conversion **must mirror one horizontal axis** (determinant −1; this is tested and is *not* a bug). Two presets, chosen per project through `UnrealFrame::axes`:

| preset | UE +X | UE +Y | UE +Z | UE yaw 0 faces | bearing → yaw |
|---|---|---|---|---|---|
| `EastSouthUp` (default of the struct) | east | **south** (−north) | up | east | `yaw = bearing − 90°` |
| `NorthEastUp` | north | east | up | north | `yaw = bearing` |

`UnrealFrame::originMetres` is the canonical point that sits at UE (0,0,0). Default zero = no offset; no arbitrary offsets are introduced.

API (all `noexcept`, `Result<…, CoordinateError>`):

```cpp
WorldToUnreal(frame, worldPoint)  -> UnrealPoint   // (world - origin) in double, axis map, * 100
UnrealToWorld(frame, unrealPoint) -> WorldPoint    // exact inverse; result must be a valid world point
WorldDeltaToUnreal(axes, delta)   -> UnrealPoint   // offsets / velocities: axes and scale, no origin
UnrealDeltaToWorld(axes, delta)   -> Vec3
BearingToUnrealYawDegrees(axes, bearingRad) -> degrees in (-180, 180]
UnrealYawDegreesToBearing(axes, yawDeg)     -> radians in [0, 2π)
```

**Proposed, not decided:** which preset the game uses. `EastSouthUp` keeps "UE +X = canonical x" (east) and only flips y; `NorthEastUp` makes UE "forward at yaw 0" = north, which is the usual map-reading direction but swaps x and y relative to the canonical frame. Either choice needs to match the WorldForge export (`CQ-101`). **Do not ship a preset before WorldForge's frame is known.** Changing the preset later changes data (a config value), not code.

Not claimed: that Unreal behaves as the table says. The table is the author's belief, tested only for self-consistency (position map ↔ yaw map ↔ determinant). Unreal runtime compatibility must be verified on Windows (section 8).

## 5. Local (region-relative) frames — **Implemented, Tested**

`LocalFrame{originMetres}`; `WorldToLocal` / `LocalToWorld`: pure translation with canonical axes, subtraction in double (exact for the tested values). No rotated frames — a rotation would be an arbitrary choice; a region that needs one composes it on top. Use: store/stream world positions as double, hand float32 offsets relative to a region origin to rendering/physics code.

## 6. Map and minimap adapters — **Implemented, Tested**

* `ToMapPosition` / `FromMapPosition`: canonical point ↔ `MapPosition` (layer + x east, y north).
* `WorldToMinimapScreen(transform, point)` / `MinimapScreenToWorld(transform, screen, elevation)`: thin wrappers over `MinimapTransform` that add input validation. Screen space is y-**down** with the origin at the viewport centre; world north is **never** a screen axis (the north-up case maps +y world to −y screen; this is tested to guard the classic mix-up between geographic and screen coordinates).

## 7. Invariants (all tested)

* round trip `UnrealToWorld(WorldToUnreal(p)) == p` within 1e-12 relative, both presets, zero and non-zero origin
* origin ↦ UE (0,0,0); +1 m east ↦ 100 cm along the preset's east axis
* NaN, ±∞, beyond-limit and overflow-scale inputs are rejected (`1e300`, `1.7e308`, `nextafter(limit)`)
* a result that falls outside the world after adding the origin is rejected
* invalid frame (non-finite origin) is rejected
* yaw: N/E/S/W for both presets; west is +180, never −180; many full turns normalize

## 8. Requirements (not done here)

**Windows / Unreal (Unverified until done):**
1. Decide the preset together with WorldForge, then record it in `Docs/Decisions/`.
2. Write the Unreal adapter module that calls `WorldToUnreal` / `UnrealToWorld` (Core stays Unreal-free); use `FVector` double (LWC) and test a known object at a known position on both axes.
3. Check in the editor that +X/+Y really are east/south (or north/east) in the *placed* test level, and that yaw 0 faces the documented direction.
4. Decide the world origin (`UnrealFrame::originMetres`) and whether World Partition / origin rebasing is used; Core's `LocalFrame` is the portable counterpart.

**SPN WorldForge (separate Rust project; not modified):**
1. Export in metres, with the same east/north/up meaning, or publish its own frame so that it can be expressed as an `UnrealFrame`/adapter.
2. State handedness and the vertical datum (sea level? terrain-relative?) — the Core only fixes "z = up, metres, inside a layer".
3. Layer ids and region bounds as data (the Core has no geography).
