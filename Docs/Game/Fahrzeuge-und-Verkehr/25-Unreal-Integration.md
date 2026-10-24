# Fahrzeuge und Verkehr 25: Unreal-Integration

> [!IMPORTANT]
> **Es gibt weder Fahrzeug- noch Verkehrscode im Repository** (Suche in `Core/` und `Unreal/SupernaturalProt/Source/` am 24.10.2026: keine Treffer). Beschlossen ist die Richtung Unreal Engine + C++20 mit portablen Kernen und Unreal-Adaptern. Die Unreal-Version und alle Plugins sind nicht festgelegt. Dieses Modul zeichnet die **Aufteilung** und nennt **Kandidaten zur Prüfung**; es wählt keine Technik. Aussagen über Unreal-Systeme stammen nicht aus den Projektquellen und sind in der Version des Projekts zu prüfen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Bestehend: [Decision 0006](../../Decisions/0006-unreal-core-integration.md), [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md), [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md).

**Inhalt**

1. [Was vorhanden ist](#1-was-vorhanden-ist)
2. [Aufteilung zwischen Kern und Unreal](#2-aufteilung-zwischen-kern-und-unreal)
3. [Kandidaten zur Prüfung](#3-kandidaten-zur-prüfung)
4. [Straßen in Unreal](#4-straßen-in-unreal)
5. [Testumgebung und Maschinenrollen](#5-testumgebung-und-maschinenrollen)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was vorhanden ist

| Gegenstand | Befund | Quelle |
|---|---|---|
| Technologierichtung | Unreal Engine + C++20, C++-first; Version, Plugins und Layout sind vor dem Festlegen zu prüfen | `CLAUDE.md` §5 |
| Portable Kerne | Engine-unabhängige Logik in portablen C++20-Bibliotheken, Unreal-Spezifisches hinter Unreal-Modulen und Adaptern; engine-neutrale, maßgebliche Simulationsgrenzen, wo sie nützlich sind | `CLAUDE.md` §5; `M-A1015`; `M-PS31` §1 |
| Erste Integration | Unreal 5.8.3 (Launcher-Build) mit dem Modul `SpnCore`, das `Core/src/*.cpp` direkt über UBT kompiliert; Kerntypen sind privat, andere Module nutzen exportierte Bridge-Funktionen | [Decision 0006](../../Decisions/0006-unreal-core-integration.md) (Engineering-Notizen, kein Kanon) |
| Spielmodul | `SupernaturalProt` mit `Spn/` (Interaktion, Testtür, Weltzustands-Subsystem) | Decision 0006 |
| Bausteine im Kern | `StrongId` (typisierte IDs), `DeterministicRandom` (SplitMix64, FNV-1a, plattformunabhängig), `DetailTierTable` (Distanz zu Stufe, konfigurierbar), `WorldFacts`, `Interaction`, `Pickup` | `Core/include/spn/core/` |
| Fahrzeug- und Verkehrscode | keiner | Suche 24.10.2026 |
| Testkarten | Kleine deterministische Testkarten für komplexe Bereiche, darunter Fahrzeuge, sind vorgesehen | `M-PD` (Testing) |

Die Verträge für Interaktion, Weltzustand und Zufall sind bereits so gebaut, dass Fahrzeuge sie wiederverwenden können, ohne neue Grundlagen zu erfinden.

---

## 2. Aufteilung zwischen Kern und Unreal

> **💡 VORSCHLAG.** Die Aufteilung folgt `CLAUDE.md` §5 (kein künstliches Erzwingen von Engine-Unabhängigkeit).

| Teil | Kern (portabel, Standalone-Tests auf Fedora) | Unreal-Adapter (Windows-Rechner) |
|---|---|---|
| Fahrzeugdaten | Typ und Exemplar, Eigentums- und Zulassungsfakten, Historie, Fahrzeug-IDs (`StrongId`) | Spawn und Darstellung, Verknüpfung mit Actors |
| Zustand | Verschleiß, Defekt, Schaden (Zustandsschichten), Reparaturaufträge | Visuelle Schäden, Partikel |
| Antriebsstrang | Motorkennlinie, Kupplung, Getriebe, Differenzial (fester Zeitschritt) | Räder, Federung, Kollision, Aufstandsfläche |
| Straße | Straßenprofile, Kreuzungsdaten, Signalprogramme, Schilder als Regelträger | Meshes, Splines, Materialien, Beleuchtung |
| Navigation | Routenplanung auf dem Straßengraph, dynamische Kosten | Wegefolgen, Hindernisse, Wahrnehmung im Nahbereich |
| Verkehrs-KI | Fahrstilprofile, Regelwerk, Taktik-Entscheidungen | Darstellung und Steuerung der Fahrzeuge |
| Dichte und Stufen | Dichtekurven, Stufenzuordnung (`DetailTierTable`), Rekonstruktion | Materialisierung als Actors oder Instanzen |
| Persistenz | Segmente, Ereignisse, Migration | Dateizugriff, Ladevorgang |
| Zufall | `DeterministicRandom` | – |

**Grundsatz:** Der Kern entscheidet, der Adapter stellt dar und führt aus. Der Kern kennt keine Unreal-Typen; der Adapter hält keine Spielregeln. Diese Grenze entspricht dem NPC-Entwurf (Kern, Adapter, Daten, Werkzeuge; [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §9).

---

## 3. Kandidaten zur Prüfung

> **💡 VORSCHLAG.** Die Liste enthält Bausteine, die für diese Aufgaben üblich sind. Sie stammt **nicht** aus den Projektquellen, kein Baustein ist gewählt, und jede neue Abhängigkeit braucht die Genehmigung nach `CLAUDE.md` §7. Ob ein Baustein in der eingesetzten Unreal-Version verfügbar, stabil und passend ist, muss ein Spike zeigen.

| Aufgabe | Kandidaten | Anmerkung |
|---|---|---|
| Fahrzeugphysik | die eingebaute Fahrzeugphysik von Unreal (Chaos Vehicles), Anpassung davon, eigene Lösung | `CQ-10`; Antriebsstrang bleibt im Kern (`VEH-DEC-PH-012`) |
| Verkehr in großer Menge | datenorientierte Entitäten (Mass Entity) und darauf aufbauende Verkehrs- und Menschenmengen-Beispiele; eigener Kern für die Ferne | steht schon als Kandidat im [NPC-Entwurf](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §9; `CQ-107` |
| Verhaltenslogik | StateTree | in den Quellen für wichtige NPCs genannt (`M-PD`) |
| Orte und Aktivitäten | Smart Objects, Zonen | NPC-Entwurf |
| Straßenzugriff | eigener Graph aus WorldForge-IDs gegenüber dem Navigationsmesh von Unreal | `CQ-108` |
| Streaming | World Partition, Data Layers | [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) |
| Wahrnehmung im Verkehr | Physik-Traces gegenüber Graphnachbarschaft | `CQ-109`; Kostenfrage (Modul 27) |

---

## 4. Straßen in Unreal

WorldForge nennt als Vorgabe: ‚UE-Seite entscheidet pro Profil zwischen Landscape Spline (wenn passend), Spline Mesh/Static Mesh oder instanzierten Segmenten‘, diese Wahl solle durch einen Spike und Pipeline-Tests geklärt werden (`WF-EX`). Brücken sind eigene Actors und Meshes, Tunnel brauchen Terrain-Maske, Mesh-Korridor und Kollisions- und Lichtplan (`WF-EX`).

> **💡 VORSCHLAG.** Das Spiel liest aus der Unreal-Darstellung der Straße nur **IDs und Geometrieverweise** (`VEH-DEC-WF-004`), niemals Spielregeln. Welche Darstellungsart für Fahrphysik gut ist (Kollisionsgeometrie, Oberflächenmaterial für die Haftung), ist im Spike zu klären: Die Fahrphysik braucht Kollision und Materialkennung der Fahrbahn, die Darstellung braucht Optik und LOD. Beide müssen auf derselben Geometrie beruhen.

---

## 5. Testumgebung und Maschinenrollen

`CLAUDE.md` §5: Der Fedora-Laptop (16 GB, integrierte Grafik) ist für Code und schnelle Standalone-Tests, der Windows-Desktop (RTX 3090, 32 GB) für Unreal-Integration; der Editor ist auf dem Laptop unter Speicherdruck eventuell unpraktisch.

> **💡 VORSCHLAG.** Aus den Rollen folgen drei Teststufen für Fahrzeuge und Verkehr (Details in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md)): Standalone-Tests der Kerne (Antriebsstrang, Zustand, Regeln, Signalprogramme, Routen, Persistenz), Unreal-Kommandozeilen-Automation auf einer **kleinen deterministischen Fahrzeug-Testkarte** (Physik, Kreuzungen, wenige Fahrzeuge) und manuelle Abnahme von Fahrgefühl und Optik. Ein gepackter Build ist für Leistungsmessungen nötig.

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-UE-001 | Die technologische Richtung ist Unreal Engine + C++20, C++-first; Version, Plugins und Layout sind vor dem Festlegen zu prüfen | 📋 BESCHLOSSEN | `CLAUDE.md` §5 | UE | `CQ-106` |
| VEH-DEC-UE-002 | Engine-unabhängige Logik liegt in portablen C++20-Kernen; Unreal-Spezifisches hinter Unreal-Modulen und Adaptern | 📋 BESCHLOSSEN | `CLAUDE.md` §5 | UE | – |
| VEH-DEC-UE-003 | Die erste Integration benutzt Unreal 5.8.3 mit dem Modul `SpnCore`, das den Kern direkt über UBT kompiliert; der Engine-Stand ist ein Engineering-Vermerk, kein Beschluss | 🧩 ABGELEITET | [Decision 0006](../../Decisions/0006-unreal-core-integration.md) | UE | Version nicht gelockt |
| VEH-DEC-UE-004 | Im Kern gibt es wiederverwendbare Bausteine: `StrongId`, `DeterministicRandom`, `DetailTierTable`, `WorldFacts`, `Interaction`, `Pickup` | 🧩 ABGELEITET | `Core/include/spn/core/` | UE | – |
| VEH-DEC-UE-005 | Das Repository enthält keinen Fahrzeug- oder Verkehrscode | 🧩 ABGELEITET | Suche in `Core/` und `Unreal/SupernaturalProt/Source/`, 24.10.2026 | UE | – |
| VEH-DEC-UE-006 | Kleine deterministische Testkarten für komplexe Bereiche, darunter Fahrzeuge, sind vorgesehen | 📋 BESCHLOSSEN | `M-PD` (Testing) | TST | Karte für Fahrzeuge: 💡 `VEH-DEC-UE-010` |
| VEH-DEC-UE-007 | Aufteilung: Der Kern hält Fahrzeugdaten, Zustand, Antriebsstrang, Straßen- und Regeldaten, Routenplanung, Fahrstilprofile, Stufenzuordnung und Persistenz; der Adapter stellt dar und führt aus | 💡 VORSCHLAG | Ableitung aus `CLAUDE.md` §5 | UE | – |
| VEH-DEC-UE-008 | Für Fahrzeuge und Verkehr entstehen eigene Unreal-Module neben `SpnCore` (Arbeitsnamen `SpnVehicles`, `SpnTraffic`) mit Bridge-Funktionen nach dem Muster von `SpnCore` | 💡 VORSCHLAG | Ableitung aus Decision 0006 | UE | Genehmigung nach `CLAUDE.md` §7 |
| VEH-DEC-UE-009 | Kandidaten zur Prüfung: eingebaute Fahrzeugphysik, Mass Entity und darauf aufbauende Beispiele, StateTree, Smart Objects und Zonen, Navigationsmesh gegenüber eigenem Graph, World Partition; keiner gewählt | 💡 VORSCHLAG | – (nicht aus den Projektquellen) | UE | `CQ-107`, `CQ-108` |
| VEH-DEC-UE-010 | Für Fahrzeuge und Verkehr gibt es eine kleine deterministische Testkarte; Kerne werden standalone getestet, Physik und Verkehr per Unreal-Kommandozeilen-Automation | 💡 VORSCHLAG | Ableitung aus `M-PD` (Testing), `CLAUDE.md` §5 | TST UE | – |
| VEH-DEC-UE-011 | Das Spiel liest aus der Unreal-Darstellung der Straße nur IDs und Geometrieverweise, keine Spielregeln; Fahrphysik und Darstellung beruhen auf derselben Geometrie | 💡 VORSCHLAG | Ableitung aus `WF-EX`, `VEH-DEC-WF-004` | UE WF | Spike nötig |
| VEH-DEC-UE-012 | Unreal-Version und Plugins sind nicht gelockt | ❓ OFFEN | `CLAUDE.md` §5 | UE | `CQ-106` |
| VEH-DEC-UE-013 | Ob der Verkehr auf Mass Entity, einer eigenen Lösung oder einer Mischung beruht, ist offen | ❓ OFFEN | [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §9 | UE LS | `CQ-107` |
| VEH-DEC-UE-014 | Die Wahrnehmung der Verkehrs-KI (Physik-Traces oder Graphnachbarschaft) und die Wahl zwischen Navigationsmesh und eigenem Graph sind offen | ❓ OFFEN | – | UE KI | `CQ-108`, `CQ-109` |
| VEH-DEC-UE-015 | Systemkritische Teile werden möglichst modular und engine-neutral gehalten, damit ein späterer Wechsel auf eine eigene Engine langfristig realistischer wird; die kanonische Basis nennt engine-neutrale, maßgebliche Simulationsgrenzen, wo sie nützlich sind, und eine eigene Weltzustands- und Persistenzarchitektur | 📋 BESCHLOSSEN | `M-PS31` §1; `M-A1015` | UE | Stützt `VEH-DEC-UE-007` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Unreal-Version und Plugins | `CQ-106` |
| Verkehrsrahmen (Mass Entity, eigener Kern, Mischung) | `CQ-107` |
| Navigation in Unreal (eigener Graph oder Navigationsmesh) | `CQ-108` |
| Wahrnehmung im Verkehr (Traces oder Graph) | `CQ-109` |
| Fahrzeugphysik-Grundlage | `CQ-10` |
