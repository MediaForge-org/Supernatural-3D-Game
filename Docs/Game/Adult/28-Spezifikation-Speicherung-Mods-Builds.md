# Adult 28: Spezifikation Speicherung, Modding, Builds, Leistung und Netzwerk

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register, Abschnitte PA, LE, SP, PR, RC](14-Entscheidungsregister.md#3-pa-pakete-builds-architektur)). Kein Kanon. Technische Festlegungen (Unreal-Version, Plugin-Layout, Datenformat) sind laut CLAUDE.md §5 **vor dem Festlegen zu validieren**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachlich: [Modul 02](02-Architektur-Pakete-Technik.md). Daten: [Modul 21](21-Spezifikation-Datenmodell.md). Repository-Bezug: [Modul 02, Abschnitt 9](02-Architektur-Pakete-Technik.md#9-einordnung-im-bestehenden-repository).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Pakete und Builds](#3-pakete-und-builds)
4. [Code-Module und Testbarkeit](#4-code-module-und-testbarkeit)
5. [Speichersystem](#5-speichersystem)
6. [Modding](#6-modding)
7. [Leistung, LOD, Streaming](#7-leistung-lod-streaming)
8. [Netzwerk und Mehrspieler](#8-netzwerk-und-mehrspieler)
9. [Sicherheit und Datenschutz](#9-sicherheit-und-datenschutz)
10. [Qualitätssicherung und Produktionsgates](#10-qualitätssicherung-und-produktionsgates)
11. [Recht und Plattform (technische Anforderungen)](#11-recht-und-plattform-technische-anforderungen)
12. [Technische Komponenten](#12-technische-komponenten)
13. [Randfälle](#13-randfälle)
14. [Tests und Abnahmekriterien](#14-tests-und-abnahmekriterien)
15. [Ungeklärtes](#15-ungeklärtes)

---

## 1. Ziel und Abgrenzung

Dieses Modul beschreibt, wie das Adult-Paket **technisch getrennt, optional, testbar und moddingfähig** gebaut werden soll, ohne das Hauptspiel zu beeinflussen. Es greift die Vorgaben aus R19/R20 und den früheren Locks auf.

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Basisspiel ist nicht explizit; Paket getrennt; nicht in Konsolen-Builds; kein `if AdultModInstalled` | 📋 | E-PA-002/003/004 |
| Entwickler-Mod bekommt keine Sonderrechte | 📋 | E-PA-034 |
| Offizielles Online ohne Mods; SP und Koop dürfen Mods | 📋 | E-PA-006/007 |
| Adult-Daten separat gespeichert | 📋 | E-PA-029/030 |
| Hauptspiel ohne Adult baubar | 📋 | E-PA-028 |
| C++-first, kaum Blueprints, eigenes Persistenzsystem | 📋 | E-PA-025, E-LE-004 |
| Mods ohne Internetzugriff, offline-first | 📋 | E-PA-036 |
| Builds: Basis / intern / intern+Adult / später Distribution | 📋 | E-PA-031 |
| Feature Flags, Hot Enable/Disable, Crash-Isolation, Sandbox, Versionsprüfung | 📋 | E-PA-030 |
| Registry-Freeze nach Missing-Family- und Qwen-Audit | 📋 | E-PR-009 |
| Vertriebsweg, Steam, Workshop, Recht | ❓ | E-PA-008 |
| Konkretes Datenformat, Unreal-Version, Plugins | ❓ | CLAUDE.md §5 |

## 3. Pakete und Builds

### 3.1 Paketstruktur (🧭)

```
Basisspiel (immer):   Human Core · Weltlogik · Beziehungs-/Romantik-Sandbox · Tattoos/Körper · Replay/Streaming · Mod-Schnittstellen · Statistik
Adult-Paket (optional): Interaktions-Engine-Erweiterung · Taxonomie-Registry · Präferenzen (erweitert) · Reaktions-Content · Intim-Detailoptionen · Adult-Venues-Inhalte · Medien-Produktion
Mods:                   eigene Partitionen, gleiche öffentliche Schnittstellen
```

- Orte (Clubs, Hotels, Wohnungen) gehören ins **Hauptspiel** (E-PA-017).
- Das Paket ist ein **separates Content-Paket**; es ist niemals Teil von Konsolen-Cooks (E-PA-004).
- Der Kern kennt keinen Check ‚Adult installiert‘; ein **Presentation Resolver** löst Assets nach Konfiguration auf (E-PA-004).
- Kein versteckter, nur per Flag deaktivierter Adult-Content im Basis-Build (E-PA-032).
- All-in-One-Paket als Standard, wenn nicht riesig; Teilpakete (Animation, Stimme, Orte, Charaktere) mit `requires/optional/incompatible/recommended` (E-PA-018/019).
- Größenbeispiele des Assistenten (15–25 GB bzw. 50–100+ GB) sind **keine** Planwerte.

### 3.2 Builds (📋 E-PA-031)

| Build | Zweck | Adult-Inhalt |
|---|---|---|
| Basisversion | Veröffentlichung/Konsole | **nein** (auch nicht versteckt) |
| Interne Entwicklung | Entwicklung ohne Paket | nein |
| Entwicklung + Adult | Entwicklung des Pakets | ja |
| Distribution (später) | öffentlicher Vertrieb des Pakets | je Store/Land/Altersfreigabe |

**Build-Validator** (🧭, E-SK-031, E-PA-033): prüft, dass das Basisbuild keine Adult-Assets/Referenzen enthält; Alterspflichtfelder; Paketmetadaten (Inhaltsbewertung, Warnungen); Vollständigkeit der Provenienz (KI-Herkunft pro Asset, E-RC-009); schlägt Release-Kanäle nach Store/Land/Freigabe zu.

## 4. Code-Module und Testbarkeit

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| SM-CM-01 | **Engine-neutraler Kern** in portablem C++20 (Regeln, Zustände, Zustimmung, Persistenz, Taxonomie, Präferenzen, Gedächtnis, Teile der Physik-Logik), ohne Unreal-Typen. | E-PA-025, E-SP-007 |
| SM-CM-02 | **Unreal-Integrationsschicht** für Actors, Components, Rig, IK, Animation, Rendering, Physik, Navigation, Eingabe. | E-PA-026 |
| SM-CM-03 | Adult-C++-Module **strikt getrennt**; das Hauptspiel ist ohne sie baubar und lauffähig. | E-PA-028 |
| SM-CM-04 | Kein Adult-Code im gemeinsamen Human Core; Erweiterungspunkte stattdessen. | E-PA-028 |
| SM-CM-05 | Blueprint nur optional für Modder; Zustimmung, Serverautorität, Sitzungslebenszyklus, Eligibility, Replikation, Persistenz, Blockregeln und Anfrageberechtigungen **nie** Blueprint-only (28#25). | E-PA-025 |
| SM-CM-06 | Sprachen: C++ (80–90 %), TypeScript nur Tools, Python nur Offline-Pipeline. | E-PA-027 |
| SM-CM-07 | Standalone-Tests ohne Unreal; Unreal-Kommandozeilenautomation getrennt (CLAUDE.md §5). | CLAUDE.md |
| SM-CM-08 | Datenbasis: Data Assets/Registries; Mods erweitern über Daten vor Skript vor Native (Reihenfolge Data → Blueprint → Sandbox-Skript → Native, 35#145). | E-SP-006 |
| SM-CM-09 | Ereignisgesteuert statt Tick; adaptive Frequenz. | E-SP-001 |
| SM-CM-10 | Threads mit Besitzerregeln (Ownership). | E-SP-001 |

Einordnung im Repository: das bestehende `Core`-Modul (portabel) und das Unreal-Plugin sind in [Modul 02, Abschnitt 9](02-Architektur-Pakete-Technik.md#9-einordnung-im-bestehenden-repository) beschrieben; eine **Entscheidung über Ordnerstruktur** ist hier nicht getroffen.

## 5. Speichersystem

### 5.1 Grundsätze (🧭)

| Nr. | Anforderung | Basis |
|---|---|---|
| SM-SP-01 | **Eigenes engine-neutrales C++20-Persistenzsystem**; Unreal-SaveGame wird nicht für große Weltzustände genutzt. | E-PA-025, E-LE-004 |
| SM-SP-02 | **Delta** gegenüber generiertem Ausgangszustand; Default-Welt wird nicht gespeichert. | E-LE-002 |
| SM-SP-03 | Eigenschaften: chunked, async, delta, versioniert, komprimiert, checksum, crash-sicher (Journaling/WAL), Migration, stabile IDs, Mod-Partitionen. | E-LE-007, 35#286 |
| SM-SP-04 | Ordnerstruktur (Vorschlag): `manifest`, `player`, `world`, `regions`, `npc`, `relationships`, `cases`, `vehicles`, `properties`, `economy`, `temporal`, `mods`. | E-LE-007 |
| SM-SP-05 | **Adult-Speicherbereich getrennt** von der Basis; verknüpft über dieselben IDs; getrennt sicher- und löschbar. | E-PA-029/030 |
| SM-SP-06 | Optional verschlüsselt/nicht menschenlesbar. | E-PA-030 |
| SM-SP-07 | **Hot Enable/Disable** des Pakets; Deaktivieren erhält kompatible Daten; verwaiste Referenzen werden markiert. | E-PA-030 |
| SM-SP-08 | Speichern darf Spielern keine langen Ladezeiten erzeugen (asynchron); Größenziel „eher kleiner“ (Vorschlag 50–150 MB, nicht gemessen). | E-LE-003 |
| SM-SP-09 | Persistiert wird nur Langzeitrelevantes, keine Frames/Loops/Kamera/IK. | E-PR-012/436 |
| SM-SP-10 | Zeitreise-/Temporal-World-State bleibt konsistent mit Beziehung/Gedächtnis. | E-NP-016 |

### 5.2 Speicher und Replay

Replay-Datenpakete gehören zum Adult-Bereich, wenn sie Adult-Sitzungen betreffen; sie sind **keine** Savegames (E-RP-008). Replays referenzieren Figuren per ID und enthalten Seeds; ein Replay ohne passende Mods ist nicht abspielbar und wird mit Begründung abgelehnt.

## 6. Modding

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| SM-MD-01 | **Provider-unabhängig**, lokal `<GameRoot>/mods/`, `.spnmod`; Workshop optional, nie Pflicht. | E-PA-036, CLAUDE.md §6 |
| SM-MD-02 | **Moddbar:** Definitionen, Animationssets, Positions-/Pose-Hinweise, Varianten-Tags, Inhaltsprofile, Übergänge, Kamera-/Audio-/Dialogprofile, Umgebungs-/Flächenprofile, Kompatibilitätsprofile, Items, Reaktionen, Präferenzen. | E-PA-035, E-SP-006 |
| SM-MD-03 | **Nicht moddbar (Sicherheitskern):** AdultEligibility, Zustimmungspflicht, Spieler-Opt-out, Serverautorität, Blockregeln, Anfrageberechtigungen, Sitzungssicherheit. | E-SK-004 |
| SM-MD-04 | **Keine Sonderrechte** für den Entwickler-Mod; er nutzt dieselben öffentlichen Schnittstellen. | E-PA-034 |
| SM-MD-05 | Community-Mods: **kein unbeschränkter Internetzugriff**, offline-first; Sandbox für Skripte. | E-PA-036 |
| SM-MD-06 | Mod-Abhängigkeiten, Konflikte und Versionsprüfung; Konflikterkennung. | E-SP-006, E-PA-019 |
| SM-MD-07 | **Capability-System** (was ein Mod darf) und kleine stabile Mod-API. | E-SP-001 |
| SM-MD-08 | Mods schreiben nie direkt in Basis-Speicherstrukturen; Save-Impact NONE/OPTIONAL/STATEFUL/CRITICAL. | E-SP-007 |
| SM-MD-09 | Mod-Manager mit Status (aktiv, fehlt, inkompatibel), Profile (Vanilla, Realism, Hardcore, Testing, Custom, evtl. Adult). | 35#58, E-PA-019 |
| SM-MD-10 | Crash-Isolation: ein fehlerhafter Mod darf das Spiel nicht beschädigen; Hot Disable. | E-PA-030 |
| SM-MD-11 | Mod-Editor (VS-Code-ähnlich, extern, headless `spn-tools`) mit Validator, Inspector, Pose/Contact-Debugger. | 35#176 (13), E-SO-022 |
| SM-MD-12 | Content-Rating-/Adult-Flag in Mod-Metadaten, Nutzerfilter. | 35#60/141 |
| SM-MD-13 | Eine Mod darf Reaktionen/Präferenzen registrieren, aber **nicht** den Reaktionsgraph oder den Regelkern ändern. | E-SP-006 |
| SM-MD-14 | Offiziell wird der Entwickler-Mod als solcher gekennzeichnet (keine Tarnung als Fan-Mod). | E-PA-005 |

## 7. Leistung, LOD, Streaming

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| SM-LP-01 | Feste **Performance-Budgets pro Subsystem** (CPU/GPU/RAM/VRAM) vor dem Merge geprüft („Performance Gate“). | E-PR-012, E-SP-001 |
| SM-LP-02 | **Simulations-LOD** und Importance-LOD; Graceful Degradation. | E-SP-001 |
| SM-LP-03 | Entity-Stufen: Simulated Entity / Active Simulation Entity / Fully Materialized Actor. Größenordnungen (500.000 → ~5.000 → ~100–300) sind Hypothesen „zu messen“. | E-LE-006 |
| SM-LP-04 | Physikqualität pro Stufe; Low darf nicht fake aussehen. | E-SO-014 |
| SM-LP-05 | Zielhardware: nicht nur Spitzen-PCs (‚nicht jeder hat 5090/64 GB‘). | E-PR-011 |
| SM-LP-06 | Hintergrundjobs (Re-Render) mit Priorität „Gameplay zuerst“. | E-RP-008 |
| SM-LP-07 | Tattoos: Bake zu Körperschichten, LOD. | [Modul 25](25-Spezifikation-Tattoo-und-Koerpermodifikation.md) |
| SM-LP-08 | Late-Stage-Features hinter Feature Flags (Flüssigkeiten, VR, erweiterte Physik). | E-PA-030 |

## 8. Netzwerk und Mehrspieler

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| SM-NW-01 | **Offizielles Online**: keine Mods; Adult-Paket nicht verfügbar. | E-PA-006 |
| SM-NW-02 | **Koop/privates Online** mit Freunden: Spielstand übernommen; Mods möglich. | 23#553–557, E-PA-007 |
| SM-NW-03 | Host-Speicherstand ist autoritativ; Gastfortschritt gilt innerhalb des Host-Speicherstands (CLAUDE.md §6). | CLAUDE.md |
| SM-NW-04 | Adult nur vom **Host** aktivierbar; Zustand autoritativ auf dem Server; Weichgewebe clientseitig. | E-SP-005 |
| SM-NW-05 | Zustimmung zwischen Spielern ist separat und serverseitig validiert; Anfrageberechtigungen (Alle, Freunde, Party, Vertraute, Niemand); Block; Anti-Spam. | 28#25, E-SK-004 |
| SM-NW-06 | Datenminimierung: Clients erhalten nur, was sie darstellen dürfen (Content-off-Client, Observer-Darstellung). | 28#25 |
| SM-NW-07 | Kein Adult-Matchmaking. | 28#25 |
| SM-NW-08 | Autoritativer Interaction-State; Prioritäten im Scheduler. | E-SO-037 |
| SM-NW-09 | Disconnect/Server-Crash: Sitzung wird sicher beendet (`SafeExit`), nicht eingefroren. | [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) |
| SM-NW-10 | Offen: Zustimmung und Altersprüfung für Gäste im privaten Koop. | [K21](19-Konflikt-und-Widerspruchsmatrix.md#k21-mehrspieler) |

## 9. Sicherheit und Datenschutz

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| SM-SD-01 | Privacy by Design; nur minimale Telemetrie (im Entwicklungsbuild Entwickler-Telemetrie). | 28#25, E-SP-001 |
| SM-SD-02 | Adult-Daten getrennt sicher-/löschbar; optionale Verschlüsselung. | E-PA-030 |
| SM-SD-03 | Keine Ausweisdaten gespeichert (nur Status). | E-SK-012 |
| SM-SD-04 | Server validiert Identität, Eligibility, Berechtigungen, Block, Zustimmung, Scope. | 28#25 |
| SM-SD-05 | Mod-Sandbox: keine Dateisystem-/Netzwerkrechte außerhalb der Capabilities. | E-PA-036 |
| SM-SD-06 | Debug-Inspector nur im Entwicklungsbuild. | E-NP-021 |

## 10. Qualitätssicherung und Produktionsgates

Vorgaben R19/R20 (E-PR-012):

| Gate | Inhalt |
|---|---|
| **Core Gate** | IK, Physik, Körperzustand, Reaction Graph, Speichern/Laden, stabiler NPC-Intent stabil |
| **Performance Gate** | Budgets vor Merge |
| **QA Gate** | Solver-, Kollisions-, Speicher-, Replay-Tests |
| **Visual Gate** | Qualitätsbewertung |
| **Registry Freeze** | erst nach Missing-Family-Audit und Qwen-Audit |
| **Production Lock** | final |

Weitere Praktiken: CI mit tausenden Kombinationen, Golden Tests, Stresstests, Developer-Telemetrie, zentraler Adult-Debugger, **Prototype First**, kurze Spezifikation pro Subsystem (Inputs, Outputs, Abhängigkeiten, Datenmodell, Ereignisse, Speicherzustand, Tests, Budget), weniger Features, aber sauber, **Adult V1 klein** (Basisinteraktionen, wenige Positionen/Übergänge, grundlegende Reaktionen, einfache Präferenzen, eine Testlocation). Reihenfolge: Ladebildschirm und kleiner Kartenteil **vor** Adult (E-PR-010).

## 11. Recht und Plattform (technische Anforderungen)

(🧭, ungeprüft; Details in [Modul 10](10-Produktion-Recht-Vertrieb.md))

| Nr. | Anforderung | Basis |
|---|---|---|
| SM-RC-01 | Inhalte nach Store/Land/Altersfreigabe ein-/ausschließbar bauen. | E-PA-033 |
| SM-RC-02 | Build-Validator gegen Adult-Spuren im Basisbuild. | E-PA-032 |
| SM-RC-03 | Provenienz-Register pro Asset (Mensch/KI-unterstützt/KI-generiert+bearbeitet/lizenziert/gekauft) für wahrheitsgemäße Plattformangaben. | E-RC-009, [K7](19-Konflikt-und-Widerspruchsmatrix.md#k7-ki-geheimhaltung) |
| SM-RC-04 | Keine Live-Generative-KI im fertigen Spiel. | E-RC-008 |
| SM-RC-05 | Nacktheits-/Sexualinhalt-Deskriptoren korrekt angeben. | E-RC-010 |
| SM-RC-06 | Fiktive Marken, eigene IP. | E-RC-005 |
| SM-RC-07 | Keine Lizenz, kein öffentliches Repository, keine Veröffentlichung ohne Entscheidung des Entwicklers (CLAUDE.md §9, §10). | CLAUDE.md |

## 12. Technische Komponenten

| Komponente | Aufgabe |
|---|---|
| `PackageLoader` | Paket laden/entladen, Feature Flags |
| `ModRegistry` | Registries, Abhängigkeiten, Capabilities |
| `ModSandbox` | Skripte, Rechte |
| `PersistenceEngine` | Delta-Saves, Journal, Migration |
| `AdultSaveStore` | getrennter Speicherbereich |
| `BuildValidator` | Basisbuild-Prüfung |
| `ProvenanceRegistry` | Herkunft von Assets |
| `NetworkSessionAuthority` | autoritative Sitzungen |
| `BudgetMonitor` | Budgets, Gates |
| `TelemetryDev` | Entwicklertelemetrie |

## 13. Randfälle

[Modul 31](31-Randfaelle-Katalog.md#9-speicherung-mods-und-builds): Speicherstand von Build mit Paket in Build ohne Paket; Modversion ändert Schema; Absturz beim Speichern; Gastspieler ohne Paket tritt bei; Paket wird mitten in einer Sitzung deaktiviert; zwei Mods registrieren dieselbe ID; Mod fordert Internetzugriff; Replay-Cache läuft voll; Reaktivierung nach langer Zeit (Schemaveraltung).

## 14. Tests und Abnahmekriterien

| Test | Erwartung |
|---|---|
| Basisbuild-Scan | keine Adult-Assets oder -Symbole |
| Laden ohne Paket | Basisspiel intakt |
| Hot Disable/Enable | Daten erhalten, keine verwaisten Verweise |
| Mod ändert Eligibility | abgelehnt |
| Mod fordert Netzwerk | verweigert |
| Absturz-Wiederherstellung | konsistenter Stand nach Neustart (Journal) |
| Migration v(n) → v(n+1) | Golden Files |
| Online-Modus | Mods deaktiviert |
| Host aktiviert Paket, Gast ohne Paket | definiert (Darstellung/Beschränkung) |
| Provenienz-Vollständigkeit | Release-Gate schlägt bei Lücken an |
| Performance-Budgets | je Subsystem erfüllt (Budgets noch festzulegen) |

[Modul 29](29-QA-Testkatalog.md#9-speicherung-mods-und-builds), [Modul 30](30-Abnahmekriterien.md#9-speicherung-mods-und-builds).

## 15. Ungeklärtes

- Unreal-Version, Plugin-Layout, Datenformat (CLAUDE.md §5).
- Verschlüsselungsumfang, Schlüsselverwaltung.
- Vertriebsweg und Recht (E-PA-008; [K18](19-Konflikt-und-Widerspruchsmatrix.md#k18-veröffentlichungsziel)).
- Gäste im Koop ([K21](19-Konflikt-und-Widerspruchsmatrix.md#k21-mehrspieler)).
- Preset-Bündel (E-PA-022), Stabilität der Frameworks (E-PA-023).
- Größen- und Leistungsziele.
- Konsole-Einschränkungen und Zertifizierung.
- Offene Frage: ob Modder Zugriff auf den Reaktionsgraph erhalten (aktuell: nein).
