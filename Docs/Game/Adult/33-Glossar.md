# Adult 33: Glossar

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Glossar erklärt die in der Adult-Dokumentation verwendeten Begriffe. Begriffe aus den Quellen sind **Arbeitsbegriffe der Planungsgespräche**, keine festgelegten Bezeichner im Code. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)).

---

## A

**Abnahmekriterium (AK):** messbare Bedingung für die Fertigstellung eines Systembereichs ([Modul 30](30-Abnahmekriterien.md)).

**Adaptationsmodell:** Körperzusammensetzung als Modell vieler Zustände (statt Gewicht/Muskel-Schiebern); Kanon K§20.

**Adult Debug Inspector:** Entwicklungswerkzeug, das Präferenzen, Reaktionsgraph und Gesundheit zeigt; nur Entwicklung (E-NP-021).

**Adult-Paket:** optionales, separates Content-Paket mit expliziteren Inhalten; nicht im Basisspiel, nicht in Konsolen, nicht im offiziellen Online (E-PA-002 ff.).

**AdultEligibility / Eligibility:** abgeleiteter Zustand `Unknown` / `NotEligible` / `AdultEligible` aus Geburtsdatum und Weltdatum; nicht moddbar.

**Aktion (Action):** `InteractionType + TargetBodyRegion + Tool/BodyPart + Parameters`; läuft als Schicht (Layer).

**Aktionsbibliothek:** kontextabhängige, durchsuchbare Liste verfügbarer Aktionen/Kombinationen; Hilfe, keine Grenze.

**Anker-Erinnerung (Anchor Memory):** besonders starke, dauerhafte Erinnerung.

**Arousal:** kontinuierlicher, verborgener physiologischer Erregungszustand; kein Zustimmungssignal.

**Awareness / Responsiveness:** Bewusstseinszustand und Ansprechbarkeit, Grundlage des Fähigkeitsgates.

## B

**Basisspiel:** Spiel ohne Adult-Paket; konsistent nichtexplizit, Nacktheit konfigurierbar.

**Beobachter (Observer):** echte Rolle mit eigener KI/Gedächtnis; kann zusehen, beitreten, gehen.

**Body Profile Seed:** Seed, aus dem ein Körperparametersatz entsteht.

**Boundary Violation (Grenzverletzung):** Handlung ohne gültige Zustimmung; nur als Konsequenz modelliert.

**Build-Validator:** Prüfwerkzeug, das das Basisbuild auf Adult-Spuren untersucht.

## C

**Capacity Gate (Fähigkeitsgate):** Prüfung, ob eine Person zustimmungsfähig ist.

**Check-in:** kurze Rückfrage/Anpassung bei unklaren Signalen, kein Dauer-Popup.

**Contact Graph (Kontaktgraph):** Graph der Kontakte mit Fläche, Kraft, Reibung, Material, Temperatur.

**Content-Toggle:** Schalter, der Darstellung (z. B. starke Körperreaktionen, Blut) reduziert, ohne Physiologie abzuschalten.

**ContentOrigin:** Herkunft von Inhalt: BaseGame, OfficialDLC, DeveloperMod, CommunityMod.

**Core / Important / Late Polish:** Prioritätsklassen von Features (R16R).

## D

**Delta-Save:** Speicherung nur der Abweichungen vom aus Seed erzeugten Ausgangszustand.

**Default-Lock Realismus:** Was nichts anderes festlegt, verhält sich wie in der Realität.

**DNA (Präferenz-DNA):** einmal bei Erzeugung aus Seed erzeugter Präferenzansatz einer Figur.

## E

**Eligibility Gate:** siehe AdultEligibility.

**Entity-Stufen:** Simulated Entity / Active Simulation Entity / Fully Materialized Actor.

**Envelope (Session Intent Envelope):** vorab etablierter Sitzungsrahmen (Führung, Überraschung, Korridore, Stopp-Signal).

## F

**Fail closed:** Bei Fehler/Unwissen wird nicht gestartet bzw. sicher beendet.

**Fallback-Leiter:** Mikrokorrektur → IK-Lockerung → Neuberechnung → Ende (kein Teleport).

**Feature Flag:** Schalter für Subsysteme (Kerninteraktion, erweiterte Physik, Replay, Flüssigkeiten, VR).

**Freeform:** Positionen entstehen aus kontinuierlichem Ganzkörperzustand, nicht aus einer Liste.

## G

**Gate-Kette (GateChain):** feste Prüfreihenfolge Paket → Alter → Fähigkeit → Verfügbarkeit → Hard Limit → Zustimmung → Ort → Kapazität.

**Golden Test:** Test mit festem Referenzergebnis.

## H

**Hard Limit (harte Grenze):** unveränderliche Invariante der Figur für die aktuelle Situation; verschiebt sich nur durch Vorgänge der Figur (selten).

**Hard Stop:** vereinbartes oder deutliches Stoppsignal; beendet sofort.

**Hardcore:** Preset über mehrere Dimensionen (kein Schalter, keine Katalogklasse).

**HumanInteractionRuntime:** Architekturvorschlag für die gemeinsame Interaktionslaufzeit (35#473).

**Hybrid-Steuerung:** Aktions-/Kombinationshilfen + analoge Echtzeitsteuerung.

## I

**Intent (InteractionIntent):** Absicht (was gewollt/zulässig), getrennt von Physik.

**Interaction Event Stream:** Ereignisstrom, den Gedächtnis, Statistik, Replay, Audio, Animation, Gesundheit und Modding abonnieren.

**Interaction Session:** laufende Sitzung aus modularen Aktionen.

## K

**Kanon (K§n):** Entscheidungen im Dokument `DECISION_CHANGELOG_2026-08-31`.

**Konsequenzebene:** Darstellung von Folgen (Polizei, Gericht, Ruf) ohne Handlungsmechanik.

## L

**Layer (Action Layer):** gleichzeitig laufende Aktionsschicht.

**LOD (Level of Detail):** Detailgrad nach Entfernung/Wichtigkeit.

## M

**Master Category Registry:** Registry aller Kategorien (Schema in E-IA-030).

**Missing-Family-Audit:** Audit nach fehlenden Familien am Ende der Runde 16.

**Mod (.spnmod):** lokales, providerunabhängiges Erweiterungspaket.

## N

**Nachsorge (Aftercare):** Phase nach der Sitzung (Nähe, Gespräch, Duschen, Schlaf).

## O

**Outcome Layer (Unified Outcome Layer):** gemeinsame Ergebnisschicht über dem Körpersystem für Adult, Kampf, Sport, Medizin.

## P

**Pre-Audit (A1–A15):** vorläufiger Audit der Familienliste (25.09.).

**Presentation Resolver:** löst Assets nach Darstellungsprofil auf, ohne `if AdultModInstalled`.

**Preference Registry:** Registry der Präferenzdefinitionen (`PreferenceDefinition`/`PreferenceState`).

**Privacy Score:** dynamischer Wert für Privatheit eines Ortes.

**Provenienz-Register:** Herkunftsangabe pro Asset (Mensch/KI-unterstützt/KI-generiert+bearbeitet/lizenziert/gekauft).

## Q

**Qwen:** ungefiltertes lokales Sprachmodell, vom Entwickler als späterer Lückenfinder und Content-Helfer vorgesehen; kein Ergebnis im Archiv.

## R

**Reaction Graph:** Graph, der aus Handlung, Körper, Person und Situation Reaktionen berechnet.

**Registry-Freeze:** Einfrieren der Kategorienregistry nach den Audits.

**Replay-Datenpaket:** kleine Datei mit Seed, Zeitlinie, Zuständen; Quelle für Re-Render.

**Risk Pipeline (Unified Risk Pipeline):** Aktion → Last → Empfindung → Schaden → Reaktion → Erholung → Gedächtnis.

## S

**Safe Exit (sicherer Ausstieg):** kontrollierter Übergang in einen neutralen Zustand ohne Teleport.

**Save-Impact:** NONE / OPTIONAL / STATEFUL / CRITICAL (Mod-Auswirkung auf Speicherstände).

**Scope (Zustimmungsumfang):** Familie / Unterart / Variante / Profil.

**Session-Seed:** Seed für deterministische Mikrovariation einer Sitzung.

**Soft Tissue:** Weichgewebe-Simulation.

**Stable ID:** Kennung `namespace:kind:local-id` ohne Version.

## T

**Tier S / A / Population:** Fidelity-Stufen von Figuren (Story / wichtig / Masse).

**Tier 0–3:** Simulationsstufen (Held/lokal/regional/ruhend).

## U

**Unified Body State:** gemeinsamer Körperzustand für alle Systeme.

**USER-LOCK:** Formulierung des Assistenten für einen nicht freigegebenen Entwicklerwunsch (K2); **keine** Entscheidung dieser Dokumentation.

## V

**Venue Policy:** Hausregeln und Weltregeln eines Betriebs.

**Virtual Texturing / Body-Space-Layers:** Technikvorschlag für Tattoo-Rendering.

## W

**Widerruf (Withdrawn):** Rücknahme der Zustimmung; logisch sofort, sicherer Ausstieg getrennt.

**World Rules:** Weltregeln (Gesetze, Hausregeln, Normen) als Daten.
