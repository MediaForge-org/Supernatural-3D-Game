# Aufgabenregister

Stand: 08.11.2026. Statuswerte: `geplant`, `bereit`, `in Arbeit`, `blockiert`, `abgeschlossen`, `verworfen`.

Regeln:
- Die ID ist stabil (`<THEMA>-<SLUG>`, siehe `NAMING_PROPOSAL.md`). Sie wird nie umgedeutet.
- Die Phase ist eine Eigenschaft. `Alias` nennt alte Bezeichnungen. Der Alias bleibt erhalten.
- Jeder Eintrag verweist auf eine Quelle. Einträge ohne Quelle sind als `Vorschlag` gekennzeichnet.
- Akzeptanzkriterien stehen in der Quelle (oder in `Milestones/`), nicht hier kopiert.

## Phase 0 (`P0-FOUNDATION`)

| ID | Titel | Status | Alias | Quelle | Bemerkung |
|---|---|---|---|---|---|
| FOUNDATION-CORE | Build-Basis, StrongId, Result, Tests | abgeschlossen | Phase 0.1 | `Docs/Decisions/0001-phase-0-1-core-foundation.md` | |
| FOUNDATION-WORLDFACTS | WorldFacts-Store, Journal, Format v1, Golden-Datei | abgeschlossen | Phase 0.2, Phase 0.3 (WorldFacts) | `Docs/Decisions/0002`, `0004` | Zwei Dinge hießen „Phase 0.3“ (K-18). |
| FOUNDATION-APPEARANCE | Aussehensgrundlage, Generator, Rezepte | abgeschlossen | Phase 0.4 | `Docs/Decisions/0005-character-appearance-foundation.md` | |
| FOUNDATION-UNREAL-CORE | Modul `SpnCore`, Unreal-Prototyp, Windows-Build | abgeschlossen | Windows/Unreal | `Docs/Decisions/0006-unreal-core-integration.md` | |
| PROTOTYPE-DOOR | Interaktion, persistente Testtür, Inkremente 1–4 | in Arbeit | Phase 0.3 (Interaktion) | `Docs/Proposals/0004-interaction-and-persistent-door.md` | Manueller Editor-Test durch Entwickler offen. |
| PROTOTYPE-PICKUP | Aufnehmbare Gegenstände, Inventarzähler | in Arbeit | Phase 0.5 | `Docs/Proposals/0007-pickup-items-and-minimal-inventory.md` | Automationstests bestanden, PIE-Test offen. |
| PROTOTYPE-TESTMAP | Testkarte, nur technische Erzeugung und technische Abnahme (Rampe, Treppe, Wand, Sprint, Kamera) | abgeschlossen | M1 | `Docs/Proposals/0003`, `0008` §13 | Scope: technisch. Technical Acceptance PASS 23.10.2026 (Entwicklerbericht, Proposal 0008 §13, nicht erneut ausgeführt); Packaged Build getestet. **Nicht** Editor-abgenommen: manuelle PIE-Prüfung offen (siehe `P0-ACCEPT-EDITOR`). Performance nicht gemessen. Release vorbereitet, nicht veröffentlicht. |
| PROTOTYPE-MOVEMENT | Gangarten, Sprung, Traversal, Bewegungsregression | in Arbeit | M2-A, M2A2–M2A4.1 | `Docs/Proposals/0011-m2a-movement-traversal-and-test-map.md`, `Docs/M2A4/MOVEMENT_REGRESSION_REPORT.md` | Regressionsbericht 28.10.2026. Abnahme durch Entwickler offen. |
| FOUNDATION-COORDINATES | Weltkoordinaten, Einheiten, Adapter | abgeschlossen | M2-B2 | `Docs/M2B2/WORLD_COORDINATE_CONVENTIONS.md`, `Core/include/spn/core/Coordinates.h` | Achsenzuordnung zu Unreal: `UnrealAxes` offen (Wahl durch Entwickler). |
| WORLD-MAP-CORE | Kartenmodell, Minimap-Berechnung | abgeschlossen | M2-B1 | `Docs/M2B1/MAP_AND_MINIMAP.md` | Keine Unreal-Integration.; Scope: Core |
| FOUNDATION-INVENTORY | Inventar-Kern, Transaktionen, Ansicht, Katalog | in Arbeit | M2-B3 (Inventar) | `Docs/M2B1/INVENTORY_FOUNDATION.md`, `Docs/M2B2/INVENTORY_PERFORMANCE.md`, `Docs/M2B3/INVENTORY_UI.md` | Core abgeschlossen; Unreal-Integration dokumentiert. |
| FOUNDATION-SAVE | Speichern/Laden, Sektionen, atomares Schreiben | in Arbeit | M2-B3 (Speichern) | `Docs/M2B2/UNIFIED_SAVE_ARCHITECTURE.md`, `Docs/M2B3/SAVE_WINDOWS_VALIDATION.md` | Haltbarkeit unter Windows unbestätigt. |
| NAV-ROUTING-CORE | Straßennetz, Routing, Navigationssitzung, Kartenframe | abgeschlossen | NightBuild02 | `Docs/NightBuild02/MapNavigation/ROUTING.md`, `DATA_MODEL.md` | Fedora-Tests, keine Unreal-Anbindung.; Scope: Core |
| NAV-DEMO-REGION | Synthetische Testregion, Demo-Navigation in Unreal | abgeschlossen | NightBuild03 | `Docs/NightBuild03/KNOWN_ISSUES.md` | Platzhaltergeometrie, Demo-Autopilot.; Scope: Demo |
| PROTOTYPE-DOORKEY-RULES | Schlüssel, Aufbrechen, Platzhalterregeln | abgeschlossen | NightBuild04 | `Docs/NightBuild04/KNOWN_ISSUES.md` | Keine Kanonregeln.; Scope: Platzhalter |
| SIM-NPC-MEMORY | Gedächtnis, Dispositionen, Speicherformat | abgeschlossen | Proposal 0010 L1, NightBuild02 | `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` §10, `Core/include/spn/core/NpcMemory.h` | Keine Spielanbindung.; Scope: Core |
| PROTAGONIST-METAHUMAN | Protagonist-Vorschau (MetaHuman) | in Arbeit | Protagonist V1 | `Docs/ProtagonistV1/README.md` | Vorschau, nicht freigegeben, nicht integriert. |

## Phase 0, Restarbeiten

| ID | Titel | Status | Alias | Quelle | Bemerkung |
|---|---|---|---|---|---|
| P0-ACCEPT-EDITOR | Manuelle Editor-Abnahme: Türen, Pickups, Speichern/Laden, M1-Karte | bereit | – | `README.md` Abschnitt 27.3 | Aufgabe des Entwicklers am Editor. Nicht automatisierbar. |
| P0-PLAYABLE-PLACEHOLDER | Erster Platzhalter mit Dialog, Ermittlung, einfachem NPC und Gegner | geplant | – | `README.md` Abschnitt 27.1 (Phase 0) | Reihenfolge laut 27.3 nicht festgelegt. |

## Phase 0 offene Systeme (aus README 27.1, ohne Details)

| ID | Titel | Status | Quelle | Bemerkung |
|---|---|---|---|---|
| CASE-EVIDENCE | Beweissystem | geplant | README 27.1, `Docs/Game/Ermittlung/` | Nicht im Code vorhanden. Details in den Ermittlungsdokumenten. |
| CASE-DIALOG | Dialogsystem | geplant | README 27.1 | Nicht im Code vorhanden. |
| NPC-BASIC | Einfache NPCs | geplant | README 27.1 | Nur `NpcMemory` im Core, keine Spielfigur. |
| CREATURE-BASIC | Einfaches Gegnerverhalten | geplant | README 27.1, `Docs/Game/Kreaturen/` | Nicht im Code vorhanden. |
| VEHICLE-DRIVING | Fahren mit Kupplung | geplant | README 27.1, `Docs/Game/Fahrzeuge-und-Verkehr/` | Nur Demo-Autopilot (NightBuild03). |

## Erster spielbarer Prototyp (`P1-FIRST-PLAYABLE`)

| ID | Titel | Status | Quelle | Bemerkung |
|---|---|---|---|---|
| PROTOTYPE-CASE-01 | Ein Raum, ein Zeuge, ein Beweis, ein Abschluss | in Arbeit | – | `NAMING_PROPOSAL.md` §4; `Core/include/spn/core/CasePrototype.h`, `Evidence.h`, `Dialogue.h`; `Nachweis/dialog-beweis-grundlagen.md` | Core-Bausteine fertig (Beweis, Dialog, Ermittlungshilfe, Integration; 37 neue Tests). Unreal-Actor für Zeuge und Beweis, Journal-UI und PIE offen. Abschlusslogik bleibt manuell (K-35). |

## Vertical Slice und folgende Phasen

| ID | Titel | Status | Phase | Quelle | Bemerkung |
|---|---|---|---|---|---|
| SLICE-CASE-01 | Ein vollständiger übernatürlicher Fall | geplant | `P2-VERTICAL-SLICE` | README Kapitel 3, 27.1; `Nachweis/vertical-slice-kriterien.md` | Belegte Kriterien (Ablauf in 7 Schritten, Konsequenzen) siehe Nachweis. Offen: Akt, Figuren, Fall, Auslöser, Messregeln für 30–60 min und 1–3 km² (`OPEN_QUESTIONS.md`). |
| WORLD-REALM-BIBLE-HELL | Realm-Bibel Hölle | geplant | offen | `Docs/Proposals` (Kanon C-06 bis C-08) | Nächster Schritt laut Kanon: Realm-Bibeln, dann räumliche Zuordnung. |
| WORLD-REALM-BIBLE-PURGATORY | Realm-Bibel Purgatorium | geplant | offen | C-06, C-09 | wie oben |
| WORLD-REALM-BIBLE-HEAVEN | Realm-Bibel Himmel | geplant | offen | C-06, C-10 | wie oben |
| MOD-CORE | Mod-Kern, lokale Pakete, Aktivierung pro Spielstand | geplant | offen | `DECISIONS.md` T-04, C-36, C-37 | Kein Code vorhanden. |
| COOP-HOST-SAVE | Koop mit Host-Speicherstand | geplant | offen | T-05, C-52 | Netzwerk-Autorität offen. |
| SIM-NPC-PERSONA | Persistente Identitäten und Beziehungen (Proposal 0010 L2) | geplant | P3-WORLD-BUILDOUT | Proposal 0010 §10 | Nach Ermittlungssystem. |
| SIM-NEMESIS | Wiederkehrende Gegner (L3) | geplant | P3-WORLD-BUILDOUT | Proposal 0010 §10 | Nach Monster-KI. |
| SIM-TIERED | Stufen der Simulation und Ereignisfortschreibung (L4) | geplant | P3-WORLD-BUILDOUT | Proposal 0010 §10 | |
| SIM-FACTIONS | Fraktionen und Gebiete (L5) | geplant | P3-WORLD-BUILDOUT | Proposal 0010 §10 | |

## Entscheidungen (keine Aufgaben, aber blockierend)

| ID | Titel | Status | Quelle |
|---|---|---|---|
| DEC-WORLD-SIZE | Endgültige Größe der Hauptwelt | blockiert | N-01 |
| DEC-UNDERGROUND-SCOPE | Umfang Untergrund nach N-02 (Bunker, Höhlen, Labore) | bereit | N-02, `OPEN_QUESTIONS.md` 0.3; Formulierung der N-02-Fassung ausstehend (Bestätigung) |
| DEC-UNREAL-AXES | Achsenzuordnung Unreal (`EastSouthUp` oder `NorthEastUp`) | geplant | `Core/include/spn/core/Coordinates.h` |
| DEC-P4-DEFINITION | Bedeutung von P4-RELEASE und POST-LAUNCH | geplant | `ROADMAP.md` |

## Konsolidierung (Planungsbereinigung, Stand 08.11.2026)

Quelle für alle Einträge: `Docs/Planning/CHANGELOG.md` (Pakete) und `Docs/Planning/Nachweis/KONSOLIDIERUNGSBERICHT.md`. Die Zählungen stehen in den Nachweisen, nicht hier.

| ID | Titel | Status | Alias | Quelle | Bemerkung |
|---|---|---|---|---|---|
| CONS-WELT | Weltanforderungen (120) gegen Docs prüfen | abgeschlossen | CONSOLIDATION-02 | `Nachweis/welt.md` | 91 vollständig, 17 bereits, 1 verworfen |
| CONS-WELT-BLOCK | Welt: blockierte und offene Punkte (10 blockiert, Konflikt K-33) | blockiert | – | `Nachweis/welt.md`, `CONFLICTS.md` K-20, K-28, K-29, K-33 | K-20 entschieden: neue Größe durch N-01 offen; K-28 nach N-02; K-29; WE-ZEIT-005 Quelle offen |
| CONS-SYSTEME | Systemanforderungen (151) gegen Docs prüfen | abgeschlossen | CONSOLIDATION-02 | `Nachweis/systeme.md` | 92 vollständig, 48 bereits |
| CONS-SYSTEME-BLOCK | Systeme: blockierte Punkte (10 blockiert, Konflikt K-34) | blockiert | – | `Nachweis/systeme.md`, `CONFLICTS.md` K-21, K-22, K-26, K-27, K-34 | Entwicklerentscheidungen nötig |
| CONS-ADULT | Adult-Anforderungen (131) gegen Docs prüfen | abgeschlossen | CONSOLIDATION-ADULT | `Nachweis/adult.md` Abschnitt G | Alle 131 belegt; Korrekturen G |
| CONS-ADULT-OFFEN | Adult: offene Entscheidungen D-01 bis D-18 | blockiert | – | `Nachweis/adult.md`, `CONFLICTS.md` K-10, K-23, K-31 | K-23 und K-31 bleiben offen; nicht genehmigte Inhalte markiert |
| CONS-DELTA | Tagesdeltas 14.–19.10. übernehmen | abgeschlossen | CONSOLIDATION-DELTA | `Nachweis/tagesdeltas.md` | Deltas sind Vorschläge; nicht Kanon |
| CONS-DELTA-FRAGEN | Delta-Fragen Q-DELTA-2 (2.200-km²-Messung) und Q-DELTA-3 (HQ auf Vorschaubildern) | blockiert | – | `OPEN_QUESTIONS.md` | Entwicklerantwort nötig |
| CONS-REGEL-P01 | Planungsregel: Freitextoption, Vorrang, keine eigenmächtige Konfliktauflösung | abgeschlossen | – | `PLANUNGSFRAGEN.md`, `DECISIONS.md` P-01 | Verbindlich (Entwicklerangabe 08.11.2026) |
| CONS-GAMEPLAY | Gameplay-Nachweis (222 Zeilen) inhaltlich gegen Gameplay-Dokumente prüfen | abgeschlossen | – | `Nachweis/gameplay.md`, `Nachweis/gameplay-inhaltspruefung.md` | 222/222 geprüft: 215 übereinstimmend, 7 blockiert |
| CONS-GAMEPLAY-MASTER | Im Gameplay-Nachweis zitierte Master-Stellen gegenprüfen | in Arbeit | – | `Nachweis/master-gegenpruefung-gameplay.md` | 11 Masterdateien gelesen; 28 gegen Master bestätigt, 52 teilweise, 116 nur konsolidiert, 19 nur Delta; 2 Master nicht geöffnet (AIS) |
| CONS-MASTER-REST | Master-Teile ohne inhaltliche Auswertung (Abschnitt 5 des Berichts) | geplant | – | `Nachweis/KONSOLIDIERUNGSBERICHT.md` §5 | Umfang nur mit Entwicklerfreigabe |
| CONS-K21-PE1022 | K-21: Hölle-Zeitdehnung; PE-E1022 in DECISIONS eintragen oder K-21 offen lassen | blockiert | – | `CONFLICTS.md` K-21, `World/26` | Entwicklerentscheidung nötig |

## Qualitätssicherung (NIGHT-AUDIT-03, 08.11.2026)

| ID | Titel | Status | Alias | Quelle | Bemerkung |
|---|---|---|---|---|---|
| QA-PRUEFGRAD | Prüfgrad je Anforderung in allen Nachweisregistern dokumentieren | in Arbeit | – | `Nachweis/pruefgrad.md` | Gesamtübersicht erstellt; Einzelgrad für Welt, Systeme, Adult offen |
| QA-TROCKENLAUF | Aufgabensteuerung über Kennungen testen | abgeschlossen | – | `Nachweis/trockenlauf-aufgaben.md` | 5 Aufgaben geprüft; PROTOTYPE-TESTMAP-Status offen (Entwicklerfrage) |
| QA-SCOPE | Architektur- und Scope-Audit | abgeschlossen | – | `Nachweis/scope-audit.md` | Befunde dokumentiert; Vorschläge nicht beschlossen |
| TECH-VARIANT-CLEANUP | Unreal-Vorlagenvarianten `Variant_Combat`, `Variant_Platforming`, `Variant_SideScrolling` entfernen | geplant | – | `Unreal/SupernaturalProt/Source/SupernaturalProt/Variant_*`, `Docs/Setup/Core-QA-Status-2026-11-08.md` | Im Build aktiv, nicht genutzt. Nur nach Entwicklerfreigabe. |

## Ermittlungs- und Dialoggrundlagen (SPN-DEV-02, 08.11.2026)

| ID | Titel | Status | Alias | Quelle | Bemerkung |
|---|---|---|---|---|---|
| CORE-EVIDENCE | Beweis: bekannt, gefunden, registriert | abgeschlossen | – | `Core/include/spn/core/Evidence.h`, `Tests/Core/EvidenceTests.cpp` | Core-Stand; Unreal-Objekt offen |
| CORE-DIALOGUE | Dialogkern: Knoten, Optionen, Bedingungen, Effekte | abgeschlossen | – | `Core/include/spn/core/Dialogue.h`, `Tests/Core/DialogueTests.cpp` | Core-Stand; Effekte dürfen fremde Zustandsnamensräume nicht schreiben (Regression); keine Freitexteingabe (DLG-01 offen) |
| CORE-ASSIST | Ermittlungshilfe: Presets als Daten, Überschreibungen | abgeschlossen | – | `Core/include/spn/core/InvestigationAssist.h`, `Tests/Core/InvestigationAssistTests.cpp` | Core-Stand; Schwierigkeitsnamen offen (DIF-01) |
| UNREAL-WITNESS-EVIDENCE | Zeugen-Actor, Beweisobjekt, Journal-UI in Unreal | geplant | – | `Nachweis/dialog-beweis-grundlagen.md` | Braucht PIE-Prüfung (`P0-ACCEPT-EDITOR`) |
| CASE-AUTO-CLOSE | Automatische Fallauflösung (T-F02) | abgeschlossen | – | `CONFLICTS.md` K-35, `DECISIONS.md` EVD-04, EVD-05 | Entschieden 08.11.2026 (SPN-DEV-03): kein automatischer Abschluss; T-F02 umformuliert. Core ohne automatische Abschlusslogik. |
