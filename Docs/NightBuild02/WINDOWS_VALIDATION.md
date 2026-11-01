# Was auf dem Windows-/Unreal-PC geprüft werden muss — NightBuild 02

Nichts hiervon wurde unter Windows, MSVC oder Unreal ausgeführt.

## 1. Build und Core-Tests (MSVC)
1. `cmake --preset` (MSVC/Ninja) und die ganze Core-Testsuite bauen und ausführen. Neue Dateien: `GeometryMath`, `MapGeometry`, `RoadNetwork`, `Routing`, `NavigationSession`, `MapRender`, `SyntheticMapRegion`, `NpcMemory` (+ Tests). Mögliche Stolpersteine: MSVC `/W4 /permissive-` mit Strukturinitialisierung, `std::erase_if`, `std::exp2`, `[[nodiscard]]`-Warnungen; die Quellen sind unter clang/GCC `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror` sauber.
2. Erwartung: alle Core-Tests grün; `unreal_stub_check` ist unter Windows nicht Teil des Laufs (nur UNIX).

## 2. Stub-Annahmen gegen die echte Engine prüfen
Der Stub nimmt für UE 5.x an: `MoveAlongFloor(const FVector&, float, FStepDownResult* = nullptr)`, `StepUp(const FVector& GravDir, const FVector& Delta, const FHitResult&, FStepDownResult* = nullptr)`, `AdjustFloorHeight()`, `SlideAlongSurface(const FVector&, float, const FVector&, FHitResult&, bool)`, `PhysFalling(float, int32)`, `FindFloor(const FVector&, FFindFloorResult&, bool, const FHitResult* = nullptr) const`, `ComputeFloorDist(..., float SweepRadius, const FHitResult* = nullptr) const`, `GetValidPerchRadius() const`, `IsWalkable(const FHitResult&) const`, `MIN_FLOOR_DIST`/`MAX_FLOOR_DIST`, `UE_KINDA_SMALL_NUMBER`, `TAutoConsoleVariable<int32>::GetValueOnGameThread()`. Die Unreal-Seite (`SpnCharacterMovementComponent.*`) wurde von mir **nicht verändert**; der echte Editor-Build ist die Entscheidung.

## 3. Karte / Navigation im Editor (noch zu bauen)
* Importer „WorldForge → `MapGeometryData`“ oder ein Editor-Werkzeug, das die synthetische Region als Demo-Level darstellt.
* `USpnMapSubsystem` um eine `NavigationSession` erweitern; `SpnMapScreen`/`SpnMapView` zeichnen `MapRenderFrame`; Klick → `ScreenToWorld` → `SetWaypoint`.
* Prüfen: Koordinatenrichtung `UnrealAxes` (EastSouthUp vs. NorthEastUp ist weiter offen), Y-Achse (Bildschirm y nach unten), Zoom/Rotation des Minimaps mit dem echten Pawn, Frame-Kosten von `BuildMapRenderFrame` im Spiel (Core-Messung: 0,17 ms Release auf dem Laptop).
* Prüfen: Route auf der synthetischen Region im Level abfahren; Abweichung, Neuberechnung, Sperrung (`SetClosedSegments`).

## 4. Savegame
* `SpnSaveSubsystem`: `GameSaveSources::npcMemory` / `LoadedGame::npcMemory` anschließen (Entscheidung des Entwicklers, ob `world.npcs` so bleibt).
* Bestehende Windows-Prüfung der Haltbarkeit (`Docs/M2B3/SAVE_WINDOWS_VALIDATION.md`) bleibt offen und unverändert.

## 5. Movement
Die bekannten Windows-/Editor-Läufe des Movement (Treppen, Türen, Kamera, Sprint-Wende) bleiben wie in `Docs/NightBuild01` beschrieben. NightBuild 02 hat dafür keine Engine-Seite geändert.
