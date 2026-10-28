# M2-A.2 Patchvorschläge für Windows-konfliktgefährdete Dateien

Nichts hiervon ist angewendet. Die Dateien `SpnCharacterMovementComponent.{h,cpp}` enthalten auf Windows die NetworkSprint-Korrektur
(`bSprintRequested`/`BaseGait` über `ClientUpdatePositionAfterServerUpdate()`); die Patches ändern diese Teile NICHT und müssen auf dem
Windows-Stand angewendet werden. Nichts ist gegen eine Engine kompiliert.

## 1. Sprint-Wenden (Core `GroundSteering`)
Ursache der 800 -> 462 cm/s bei 90 Grad: `GroundFriction = TurnFriction (8)` mischt den Geschwindigkeitsvektor linear zur Zielrichtung; dabei
sinkt der Betrag (bei 90 Grad auf bis zu etwa 0,7). Core-Referenz: `SteerVelocity` dreht den Kurs begrenzt (720 -> 300 °/s bis 8 m/s) und
verliert 0,15 Betrag je Radiant. Tests (Core): 45/90/135/180 Grad Mindesttempo > 6,5 / 5,5 / (monoton) / > 3,0 m/s, Wiederaufbau auf 8 m/s < 2 s.
Vorschlag (nur Walking-Modus, replay-sicher, weil nur von Velocity/Input/dt abhängig):
```cpp
// in USpnCharacterMovementComponent: bei MOVE_Walking und bOrientRotationToMovement statt des Friction-Blends
void USpnCharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
    if (!IsMovingOnGround() || !bUseSpnSteering) { Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration); return; }
    // Bridge (noch zu schreiben, wie SpnMovement.cpp): cm <-> m, Settings -> spn::core::SteeringConfig
    const FVector2D Wish(Acceleration.X, Acceleration.Y);
    const spn::core::Vec2 Next = spn::core::SteerVelocity(SteeringConfig, {Velocity.X * 0.01, Velocity.Y * 0.01},
        {{Wish.X, Wish.Y}, GetMaxSpeed() * 0.01, GetMaxAcceleration() * 0.01, Settings.BrakingDeceleration * 0.01}, DeltaTime);
    Velocity.X = Next.x * 100.0; Velocity.Y = Next.y * 100.0;
}
```
Offen: `Acceleration` ist in CalcVelocity bereits in cm/s² skaliert (Richtung benutzen, nicht den Betrag), analoger Stick (MinAnalogWalkSpeed),
Root Motion, `bUseSeparateBrakingFriction`. Zuerst hinter einem Flag `bUseSpnSteering = false` einbauen und mit 20x NetworkSprint testen.

## 2. Sprung gegen Kanten (Core `JumpModel`)
Kinematik (Apex 45 cm, g 9,81, v0 2,97 m/s): ein Absprung näher als `speed * riseTime` vor der Kante trifft die Kante noch im Aufstieg.
In der Luft gibt es kein Step-up; die Kante nimmt die horizontale Geschwindigkeit, der Charakter steigt senkrecht, dann folgt das Weiterschieben:
genau die gemeldete Beobachtung. Mindestabstand vor Kante (Run 5,5 m/s): 30 cm 0,70 m, 40 cm 1,11 m; bei Sprint 8 m/s: 30 cm 1,03 m.
**Hypothese, durch Kinematik gestützt, nicht durch Engine-Messung bestätigt.** Messen mit `USpnMovementDiagnostics` (`spn.MoveDiag 1`):
Ereignis 8 (Tempo-Einbruch in der Luft) direkt nach Ereignis 1 (Absprung) an einer 30/40-cm-Wand bestätigt sie.
Korrekturoptionen, keine ohne Messung anzuwenden und keine verändert die Apexhöhe:
- A. Gravitation hoch, Sprungimpuls passend (gleicher Apex): Mindestabstand fällt mit 1/sqrt(Gravitationsfaktor) (Core-Test: Faktor 2 -> -29 %). Anpassung `GravityScale` + `JumpHeight`-Bridge.
- B. Horizontale Geschwindigkeit beim Kantenkontakt in der Luft für kurze Zeit erhalten ("Ledge-Assist"), solange die Füße noch steigen und der Apex die Kante überragt (`EvaluateJump` == BlockedRising). Kein Teleport: Geschwindigkeit wird nur nicht gelöscht.
- C. Sprungpuffer/Absprungkorrektur im Input (kein Eingriff in die Physik).
Zur 25-cm-Stufe: das beobachtete automatische Überwinden von ca. 30 cm ist nicht erklärt; Messung der Ereignisse 4 (Step-up) mit `FloorDist` an der 30-cm-Wand liefert die Daten (Verdacht: Step-up-Toleranz der Engine über `MaxStepHeight`, Schwebe-/Perch-Logik; unbestätigt).

## 3. Bewertung der Sprung-Optionen (Phase 3, Core `SimulateFaceJump`)
Zahlen aus dem Kinematikmodell (Apex 45 cm, Run 5,5 m/s). Mindestabstand vor der Kante, ab dem ein Sprung die Kante nicht mehr im Aufstieg trifft:

| Gravitationsfaktor | 30 cm Kante | 40 cm Kante | Flugzeit |
|---|---|---|---|
| 1,0 (jetzt) | 0,70 m | 1,11 m | 0,61 s |
| 1,5 | 0,57 m | 0,91 m | 0,49 s |
| 2,0 | 0,50 m | 0,79 m | 0,43 s |

Kantenkontakt im Aufstieg (Absprung 0,2 m vor einer 30-cm-Kante): Die Füße passieren die Kantenhöhe nach 0,128 s und bleiben danach 0,35 s darüber. Mit Luftkontrolle 0,35 (1,2 m/s²) legt eine am Kontakt gestoppte Kapsel in dieser Zeit nur 0,073 m zurück; mit Ledge-Assist (Tempo bleibt) 1,9 m.
**Das Modell kann das beobachtete "springt dann abrupt auf das Hindernis" noch nicht von "fällt zurück" unterscheiden**: entscheidend ist, wie viel Vorwärtsweg die Engine nach Überschreiten der Kante verlangt (Perch/Floor-Detection). Bei <= 7 cm "rastet" der Sprung ein (passt zur Beobachtung), bei einem vollen Kapselradius (42 cm) fällt die Kapsel zurück (Core-Test zur Empfindlichkeit). Dieser Wert ist die wichtigste Messgröße der Windows-Diagnose: x-Position der Kapsel bei Ereignis 8 (Kontakt) und bei Ereignis 2 (Landung) bzw. wann `FloorDist` zum ersten Mal ~0 und `walkable` = 1 wird.

Bewertung (keine Option anwenden, bevor gemessen wurde):
- **A (Gravität erhöhen, Impuls anpassen):** verkleinert die tote Zone um 1/sqrt(Faktor), verkürzt aber die Flugzeit (0,61 -> 0,43 s bei Faktor 2) und verändert das Sprunggefühl insgesamt (Fallgeschwindigkeit, Landung). Betrifft Landing-Zeit, Fallschaden-Schwellen später. Reversibel, über Settings. Wirkt auf jeden Sprung, nicht nur an Kanten.
- **B (Ledge-Assist: Tempo am Kontakt nicht löschen):** löst genau den modellierten Fall, braucht aber Eingriff in die Kollisionsantwort der Komponente (Windows-Datei, Netzwerk-Replay-sicher gestalten), Gefahr "Hängenbleiben/Gleiten" an Wänden; nur bei `EvaluateJump == BlockedRising` UND Apex > Kante + Reserve.
- **C (Sprungpuffer/Absprungkorrektur im Input):** ändert die Geometrie nicht; hilft nur gegen "zu spät gedrückt".
Entscheidung erst nach der Messung (Abschnitt oben); bis dahin bleibt das Verhalten unverändert.

## 4. Nachtrag 2026-10-28 (Fedora): Sprint-Wenden, Messung erklärt
Die Windows-Messung (800 -> 461..464 cm/s, 95 % nach ca. 1,02 s) wird von der Engine-Reibungsformel allein reproduziert: `v -= (v - dir*|v|) * min(dt*Friction, 1)` verkleinert den Betrag bei jedem Schritt kumulativ (nicht nur bis 0,707). Modell in `Tests/Core/GroundSteeringTests.cpp` (Test "the engine's friction blend alone explains ..."): Minimum 58 %, 95 % nach 1,02 s. Das Verlieren der Sprint-Gangart (Sustain gilt nur über 6,75 m/s) verschärft den Einbruch nur um ca. 0,2 m/s. Eine Änderung der Gangart-Regel ist deshalb **nicht** die Lösung. Die gemessenen 41-44° waren der Restwinkel im Akteur-Frame, nicht der Eingabewinkel. Die Vorschläge in §1 bleiben die Optionen; Anwendung nur nach Entscheidung. Nicht erklärt: Sprint endet nur beim Linkswechsel.
