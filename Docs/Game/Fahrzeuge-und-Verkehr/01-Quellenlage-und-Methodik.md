# Fahrzeuge und Verkehr 01: Quellenlage und Methodik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sagt, **woher** die Dokumentation zu Fahrzeugen und Verkehr stammt, welche Schlüssel und Statuswörter sie benutzt, was tatsächlich gelesen und was nur durchsucht wurde und wo die Quellen enden. Es erfindet keine Spielerentscheidungen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Das Register steht in [Modul 28](28-Entscheidungsregister.md), Widersprüche und offene Fragen in [Modul 29](29-Konflikte-und-offene-Fragen.md), die Abdeckung in [Modul 30](30-Abdeckungsprotokoll.md), Tests und Ausbaustufen in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Auftrag und Arbeitsregeln](#2-auftrag-und-arbeitsregeln)
3. [Statuswörter](#3-statuswörter)
4. [Zitierschlüssel](#4-zitierschlüssel)
5. [Quellengewicht und Rangfolge](#5-quellengewicht-und-rangfolge)
6. [Master-Konsultation nach CLAUDE.md §11](#6-master-konsultation-nach-claudemd-11)
7. [Lesenachweis](#7-lesenachweis)
8. [Nicht verfügbare Quellen](#8-nicht-verfügbare-quellen)
9. [Grenzen der Rekonstruktion](#9-grenzen-der-rekonstruktion)
10. [Konventionen für IDs](#10-konventionen-für-ids)

---

## 1. Befund in Kürze

1. **Fahrzeuge sind in der Richtung breit beschlossen, im Detail kaum.** Beschlossen sind: Fahren als Kernsystem, manuelles Getriebe mit Kupplung als Standard, eine manuelle Variante für jedes Fahrzeug, die DualSense-Belegung vom 15.10.2026, die Verschleißteile, die Kraftstoffarten, fiktive Fahrzeugnamen und die Persistenz von Fahrzeugen. Die [Welt-Dokumentation](../World/README.md) (Module 13 und 14) hat das bereits erfasst; dieser Ordner **wiederholt es nicht**, sondern verweist auf `W-FZ-…` und `W-FP-…`. Zusätzlich fanden sich im Master zwei Dokumente mit Fahrzeugabschnitten, die die Welt-Dokumentation nicht als Schlüssel führt: `M-DC31` (§30 bis §36, §47, §48; Getriebe gehört zur Variante, Trim-Familien, Reifenarten, Fahrzeug-App, Reichweiten, Passagiere und Körper im Fahrzeug, Sicherheitssysteme) und `M-PS31` (§38 bis §54). Beide bestätigen die Richtung und verfeinern Einzelheiten; sie sind im Register mit eigenen Zeilen geführt. Sie wurden erst nach der ersten Fassung der Module gefunden und nachgearbeitet.
2. **Der Verkehr ist als Verhaltensanforderung beschlossen, als System offen.** Beschlossen sind dynamischer, aber nicht absurder Verkehr, Stoßzeiten, Radiomeldungen bei Stau, seltene NPC-Unfälle, Pannen, Baustellen, Fahrer mit unterschiedlichem Können, Rettungsgassen und die Rekonstruktion einer glaubwürdigen lokalen Verkehrslage beim Betreten eines Gebiets (`W-TV-001`, `W-TV-009`, `N-LE-024`, `N-LE-026`). **Es gibt kein Verkehrsmodell** (`W-FP-010`, `W-TV-008`): keine Dichten, keine Fahrzeugzahlen, keine Regeln für Kreuzungen, Ampeln, Vorfahrt, Überholen oder Parken.
3. **Der Master enthält zu Verkehr fast nichts.** Die Master-Suche (Abschnitt 6) liefert in den aktuellen kanonischen Quellen einen einzigen Treffer (Navigations- und Verkehrs-Apps im Smartphone-Ökosystem) und in den Tagesdeltas zwei Verweise auf das offene Verkehrsmodell. Die Verkehrsanforderungen stehen in `M-WB17` Block 5 und `M-WB89` Block 8 und in den Antworten des Entwicklers im Chat 23 (`23#1025E`, `23#1057E`).
4. **Die Schadensfrage ist ausdrücklich unbeantwortet.** Das Gespräch vom 15.10.2026 endete bei der Frage nach dem Detailgrad des Schadensmodells (`W-FZ-013`). Alles in [Modul 07](07-Zustand-Verschleiss-und-Schaden.md), das darüber hinausgeht, ist ein Vorschlag.
5. **Zwischen der Karte und dem Verkehr fehlt eine Schnittstelle.** WorldForge (`SPN_WorldForge`, außerhalb dieses Repositories) beschreibt einen Straßengraph-Vorvertrag für den Meilenstein M2 (`RC-01`), aber keine Spuren, Geschwindigkeitsbegrenzungen, Schilder, Signale, Parkplätze oder Navigationsverknüpfungen. [Modul 24](24-WorldForge-Schnittstelle.md) trennt, was dort steht, von dem, was das Spiel braucht, und schlägt Felder nur als 💡 VORSCHLAG vor. Der Unreal-Import ist in WorldForge selbst als noch zu belegen geführt.
6. **Die Weltgröße von etwa 2.200 km² ist ein Arbeitsszenario**, nicht beschlossen (`CLAUDE.md` §3). Die Zahl taucht hier nur als Skalierungsannahme für Leistungsüberlegungen auf. Kein Fahrzeug- oder NPC-Wert wird daraus abgeleitet.

---

## 2. Auftrag und Arbeitsregeln

Auftrag der Projektleitung vom 24.10.2026 (Schlüssel `AUF`): Dokumentation der Bereiche Fahrzeuge und Verkehr unter `Docs/Game/Fahrzeuge-und-Verkehr/`. Nur Dokumentation und Dokumentationsprüfwerkzeuge; keine Spielimplementierung, keine Unreal-Dateien, keine Assets. Ausgangsstand: Branch `main`, Commit `9e8598f`, mit nicht committeten Änderungen aus der Kampfsystem-Arbeit (bleiben unangetastet).

| Regel aus dem Auftrag | Umsetzung in diesem Ordner |
|---|---|
| Bestehende Entscheidungen unverändert übernehmen | Beschlossenes steht mit Status und Verweis auf das Register der Nachbarordner; keine Neuformulierung als Entscheidung |
| Jede wesentliche Aussage mit Status | Siebenstufige Statusskala (Abschnitt 3), erzwungen vom Registergenerator |
| Neue Ideen nur als Vorschlag | `💡 VORSCHLAG`, in Tabellen und Abschnitten gekennzeichnet |
| Keine erfundenen technischen Details, keine verbindlichen Zahlen | Zahlen nur, wenn eine Quelle sie nennt; sonst keine oder ausdrücklich als Beispielwert gekennzeichnet |
| Keine Doppelverantwortung | Je Thema ein Eigentümer (Abschnitt 5 jedes Moduls), Verweis auf die Nachbardokumentation statt Kopie |
| Jede Entscheidungsfrage mit Freitextoption | Alle Fragen in [Modul 29](29-Konflikte-und-offene-Fragen.md) tragen die Option ‚F Freitext‘; deren Inhalt hat Vorrang vor den vorgegebenen Optionen |
| Git nur lesend | Kein Commit, Push, Tag, Release, Branchwechsel, Reset, keine Löschung |

Zusätzlich gelten die Regeln aus `CLAUDE.md`: Vorschläge dürfen nie stillschweigend zu Beschlüssen werden, Zahlenwerte für Fahrverhalten, KI und Simulation sind nicht freigegeben, und `Planning/Master` bleibt schreibgeschützt.

---

## 3. Statuswörter

| Zeichen | Wort | Bedeutung | Vergabe |
|---|---|---|---|
| 📋 | BESCHLOSSEN | Durch eine konkrete Entwickleraussage oder eine ohne OPEN-Vermerk geführte Regel festgelegt | Übernommen aus dem Register der Nachbarordner oder aus `M-PD`; die Quelle steht daneben |
| 🧭 | RICHTUNG | Gebilligte Richtung, ausdrücklich nicht abgeschlossen | Pauschale Zustimmung, Arbeitsauftrag an einen Kartenlauf, Planungsrichtung |
| 🧩 | ABGELEITET | Folgt zwingend aus beschlossenen Anforderungen; die Ableitung steht daneben | Zwei oder mehr 📋-Aussagen, die zusammen etwas erzwingen |
| 💡 | VORSCHLAG | Neue Idee, Entwurf oder Beispiel; nicht genehmigt | Alles, was von dieser Dokumentation oder einem Assistenten stammt |
| ❓ | OFFEN | Noch nicht entschieden, als Frage geführt | OPEN-Vermerk der Quelle oder fehlende Antwort |
| ⚠️ | KONFLIKT | Zwei Quellen widersprechen einander | Keine Auflösung durch die Dokumentation |
| 🔍 | UNBELEGT | Behauptung ohne auffindbare Quelle | Die Dokumentation hält den Mangel fest und füllt ihn nicht |

**Regeln der Vergabe**

1. **Eigene Aussagen sind 💡.** Alles, was diese Dokumentation über das hinaus ergänzt, was die Nachbarordner und der Master tragen (Zustandsmodelle, Datenfelder, Verfahren, Beispiele), ist 💡 VORSCHLAG.
2. **Übernahme heißt Übernahme.** Ein Register-Eintrag, der ein Nachbarregister zitiert (`W-FZ-005`), erbt dessen Status. Ändert sich der Status dort, ist hier nachzuführen; die Prüfung stellt nur sicher, dass die zitierte ID existiert.
3. **Arbeitsaufträge an Kartenläufe (🕰️ in der Welt-Dokumentation) sind keine Spielentscheidungen.** Sie erscheinen hier als 🧭 oder gar nicht.
4. **Zusammenfassungen sind keine Belege.** Wenn eine Aussage nur in einer Zusammenfassung steht und die Rückverfolgung zur Entwickleraussage fehlt, trägt sie 🧭 oder 🔍.
5. **Beim Zählen gilt:** Eine Zeile mit zwei Statuswörtern zählt für beide.

---

## 4. Zitierschlüssel

Alle Master-Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Chats: Schlüssel `<Chat>#<Nachricht><Rolle>`, `E` = Entwickler, `A` = Assistent, `P<n>` = Punkt n der nummerierten Frage. Chat-Nummern wie in [Welt 01](../World/01-Quellenlage-und-Methodik.md).

### 4.1 Planungs- und Entscheidungsdokumente

| Schlüssel | Datei (relativ zum kumulativen Master) |
|---|---|
| `M-PD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md`; benutzt: ‚Vehicles‘, ‚Vehicles/racing‘, ‚World state‘, ‚Cash / law / evidence‘ |
| `M-OD` | `…/docs/reference/OPEN_DECISIONS.md`; benutzt: ‚Vehicles / racing‘ |
| `M-DC31` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/DECISION_CHANGELOG_2026-08-31.md` (Status im Kopf: ‚CANONICAL DELTA‘, neuer als der Stand v0.4 und ersetzt widersprechende ältere Formulierungen in `M-PD`); benutzt: §3, §12, §13, §30 bis §36, §38, §39, §47, §48 |
| `M-PS31` | `…/docs/reference/PROJECT_STATE_2026-08-31_COMPACT.md` (‚Master Project State / Source of Truth‘, deutsch, Stand 2026-08-31); benutzt: §1, §37 bis §54 |
| `M-WB17` / `M-WB89` | `02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/WORLD_DESIGN_BLOCKS_1_TO_7_CURRENT.md` / `…_8_TO_9_CURRENT.md`; benutzt: Block 5 (Verkehr), Block 8 (Lebendige Welt) |
| `M-W15R` | `02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/WORLD_1500_CURRENT_REQUIREMENTS.md` |
| `M-LATE` | `02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/LATE_ADDITIONS_AND_CORRECTIONS_2026-09-04.md` |
| `M-MS1013` | `01_CURRENT_CANONICAL_STATE/MASTER_STATUS_2026-10-13.md` (‚Manual clutch / driving is an early gameplay ambition, subject to implementation planning‘) |
| `M-A1015` | `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-15/01_CANON_ADDENDUM.md` (Technologiebasis, Koop bis 4 Spieler, Stadtgrößen als PROVISIONAL) |
| `M-A0906` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` (Reichsdurchquerung unterscheidet sich vom normalen Fahren) |
| `M-EP1018` | `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/03_ENGINEERING_PROPOSALS.md` (‚Proposed engineering work — NOT ratified canon‘; Abschnitt ‚Rail and road vehicles‘: ‚review-required specification inputs‘) |
| `M-OC1018` | `01_CURRENT_CANONICAL_STATE/ADDENDA_2026/2026-10-18/02_OPEN_AND_CONFLICTS.md` (Zeile ‚Transport‘: Fahrzeugdetails sind Vorschläge neben etablierten Vorlieben, einzeln zu prüfen) |
| `M-CA0905` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (Smartphone-Ökosystem mit Navigation und Verkehr; Alkohol beeinflusst Reaktion, Gleichgewicht, Wahrnehmung und Fahren; Fahrzeuge als Lagerraum) |
| `M-B38` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/planning-backlog/38-vehicle-physics-trims-racing.md` (Planungsinventar, nur Themenliste) |
| `D15-RV` | `99_DAILY_DELTAS/2026-10-15/SPN_DAILY_DELTA_2026-10-15/docs/specs/ROAD_VEHICLES_2026-10-15.md` (Phasenvertrag und DualSense-Belegung; ‚user-approved‘ bzw. ‚user specified‘) |
| `D15-RW` | `…/2026-10-15/…/docs/specs/RAILWAY_SYSTEM_2026-10-15.md` |
| `D15-OQ` | `…/2026-10-15/…/docs/open_questions/OPEN_QUESTIONS_2026-10-15.md` (Punkt 9: Leistungsstufen der Offscreen-Simulation und deterministische Resynchronisation von Zügen, Verkehr und NPCs) |
| `D18-MAP` / `D18-ACC` | `99_DAILY_DELTAS/2026-10-18/…/docs/canon/WORLD_MAP_DECISIONS_2026-10-18.md` / `…/specs/MASTER_MAP_ACCEPTANCE_SPEC.md` |
| `D14-PS` | `…/docs/design/POPULATION_AND_SPATIAL_SCENARIOS.md` (Flächen- und Bevölkerungsrechnung) |
| `M-AS` | `99_DAILY_DELTAS/2026-10-16/SPN_DAILY_DELTA_2026-10-16/docs/specs/AI_SIMULATION_BOUNDARIES.md` (Kandidat 6; **Vorschlag, nicht Kanon**) |
| `M-OQ16` | `…/2026-10-16/…/docs/open_questions/OPEN_QUESTIONS_2026-10-16.md` (Punkt 9: KI-/NPC-LOD, Speicherschema, deterministische Offscreen-Simulation) |
| `M-MOD` | `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md` |
| `PE-N1022` | Entwickleraussage vom 22.10.2026 zu Living World, NPC-Gedächtnis und Nemesis ([Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §1) |
| `AUF` | Auftrag der Projektleitung vom 24.10.2026 (Dokumentationsauftrag Fahrzeuge und Verkehr); Eingabe, keine Spielentscheidung |

### 4.2 WorldForge (außerhalb dieses Repositories)

Pfade relativ zu `SPN_WorldForge/docs/` (Nachbarprojekt im Verzeichnis `Projekte`, nicht Teil dieses Repositories, nicht verlinkbar).

| Schlüssel | Datei | Statuswort der Quelle |
|---|---|---|
| `WF-RC01` | `19_V3_DOMAENENKONTRAKTE/02_ROAD_GRAPH_PROFILE_V1.md` (‚RC-01‘) | ‚M2-Vorvertrag‘ |
| `WF-D03` | `09_DOMAENEN/03_STRASSEN_HIERARCHIE_NETZ_GEOMETRIE.md` (‚DOMAIN-03‘) | Domänenbeschreibung, Phase M2 |
| `WF-RG` | `02_SPEZIFIKATIONEN/STRASSEN_INFRASTRUKTUR.md` (‚Road Generator‘) | Spezifikation |
| `WF-DC01` | `08_VERBINDLICHE_DATENVERTRAEGE/01_KOORDINATEN_UND_EINHEITEN.md` (‚DC-01‘) | ‚Architekturvertrag (geplant)‘ |
| `WF-C09` | `13_V3_SYSTEMVERTRAEGE/09_EDIT_OPERATIONS_STABLE_IDS_UNDO.md` (‚C-09‘) | Systemvertrag |
| `WF-UE` | `15_V3_IMPORT_UNREAL/01_OFFIZIELLE_UE_FAKTEN_VS_SPIKES.md`, `05_WP_REGION_REIMPORT_UND_PERFORMANCE.md` | Unreal-Fähigkeiten ‚noch nicht erhoben‘ bzw. zu belegen |
| `WF-PL` | `03_ROADMAP/PHASENPLAN_M0_BIS_M7.md` | Meilensteinplan |
| `WF-RD` | `README.md` im Wurzelverzeichnis von `SPN_WorldForge` | Stand ‚M0.1 … Der echte Unreal-Import bleibt für M0.2 offen und ist `UNVERIFIED`‘ |
| `WF-EX` | `15_V3_IMPORT_UNREAL/04_MESH_SPLINE_WASSER_MATERIALS.md` | Exportanforderungen für Straßen, Brücken, Tunnel |

### 4.3 Repository-Dokumente

| Kurzform | Datei |
|---|---|
| Gameplay-Systeme §n | [`Docs/Game/Gameplay-Systeme.md`](../Gameplay-Systeme.md) (§6 Fahrzeuge, §8 NPCs, §10 Recht und Beweise, §16 Persistenz) |
| `W-XX-nnn` | Register der [Welt](../World/20-Entscheidungsregister.md) (FZ, FP, TV, VK, UR) |
| `N-XX-nnn` | Register der [NPCs](../NPCs/26-Entscheidungsregister.md) |
| `E-XX-nnn` | Register der [Ermittlung](../Ermittlung/20-Entscheidungsregister.md) |
| `K-XX-nnn` | Register der [Kreaturen](../Kreaturen/20-Entscheidungsregister.md) |
| `COMBAT-DEC-XX-nnn` | Register des [Kampfsystems](../Kampfsystem/27-Entscheidungsregister.md) |

---

## 5. Quellengewicht und Rangfolge

Angewendet wird die Rangfolge aus `CLAUDE.md` §2 und `Docs/MasterIndex.md`:

1. **Spätere ausdrückliche Entwickleraussagen** (Chat-`E`-Nachrichten, Phasenvertrag und Eingabebelegung `D15-RV`) gehen vor.
2. **`M-WB17`, `M-WB89`, `M-PD`** tragen die Verkehrs- und Fahrzeuganforderungen; **`M-DC31`** (‚CANONICAL DELTA‘) ist neuer als `M-PD` und geht bei Widerspruch vor, `M-PS31` ist die deutsche Zusammenfassung desselben Stands.
3. **Die Register der Nachbarordner** sind autoritativ für ihr Gebiet. Die Fahrzeug- und Verkehrsdokumentation **verweist**, statt neu zu entscheiden.
4. **Prompt-Entwürfe** (`DRAFT_REWRITE_REQUIRED`) und **Assistentenvorschläge** sind 💡.
5. **WorldForge** ist ein Nachbarprojekt mit eigener Governance. Seine Verträge sind ‚Vorverträge‘ oder ‚geplant‘ und binden das Spiel nicht.

---

## 6. Master-Konsultation nach CLAUDE.md §11

Durchgeführt am 24.10.2026 mit `Tools/master-search.sh` (Tier 1 und 2, Begriffe einzeln: `traffic`, `Verkehr`, `parking`, `garage`, `gas station`, `Tankstelle`, `road graph`; der Master wurde nicht verändert).

| Nr. | Befund | Klasse | Verwendung |
|---|---|---|---|
| F1 | Tier 1, `traffic`: ein Treffer in `M-CA0905` (Smartphone-Ökosystem kann Navigation, Verkehr, Wetter, Bank und Weiteres enthalten) | PROVISIONAL (Addendum, Wortlaut ‚may include‘) | [Modul 13](13-Verkehrs-KI-und-Navigation.md), [Modul 15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md) |
| F2 | Tier 2, `traffic`: `D15-RV` (Verkehrsmodell nicht ratifiziert) und `D15-OQ` Punkt 9 (Leistungsstufen und Resynchronisation offen) | OPEN | [Modul 26](26-Simulationsstufen-LOD-und-Streaming.md), [Modul 29](29-Konflikte-und-offene-Fragen.md) |
| F3 | `Verkehr`: nur Treffer in einem Prüfbericht zu Kartenläufen (`99_CANON_AUDITS`), Bewertung von Verkehrskorridoren der Testwelt | Evidenz, nicht Kanon | [Modul 10](10-Strassennetz-und-Strassentypen.md) |
| F4 | `parking`, `garage`, `Tankstelle`, `gas station` (Tier 1 und 2): keine Treffer außer Fremdtreffern (Bahn, Koop). Tier 3 (`--tier 1,2,3`): Fahrzeug-App mit Parkposition in `M-DC31` §34 und `M-PS31` §48 | CANONICAL DELTA beziehungsweise Source-of-Truth-Dokument (Tier 3) | Parken und Tankstellen stehen sonst nur in `M-WB17` Block 5 (eingebettete Basis, von der Standardsuche nicht erfasst) und in der Welt-Dokumentation |
| F5 | `road graph`: Treffer betreffen WORLD_700 (Sperrvermerk der Makrogeografie) | CONFIRMED (Sperre), irrelevant für Verkehr | [Modul 24](24-WorldForge-Schnittstelle.md) (Abgrenzung) |
| F6 | Prompt-Bibliothek, Track 09 (Fahrzeuge, Fahrphysik) und `M-B38`: Themenliste, `DRAFT_REWRITE_REQUIRED` | DRAFT | nur Themeninventar |
| F7 | Tier 1, `vehicle|driving|clutch|Fahrzeug`: `M-MS1013` (Fahren mit Kupplung als frühe Ambition), `M-A1015` (Koop bis 4 Spieler, Replikation offen; engine-neutrale Grenzen), `M-A0906` (Reichsdurchquerung), `M-CA0905` (Alkohol und Fahren; Lager; Smartphone), `M-EP1018` und `M-OC1018` (Fahrzeugdetails des Oktober-Pakets als ‚review-required‘) | CANON DIRECTION / PROVISIONAL; `M-EP1018`: NOT ratified | Module 02, 05, 13, 21, 25; Konflikt `CK-07` |
| F8 | Tier 3, `DECISION_CHANGELOG_2026-08-31.md` (36 Treffer) und `PROJECT_STATE_2026-08-31_COMPACT.md` (32 Treffer): Fahrzeugabschnitte (siehe Abschnitt 4.1) | CANONICAL DELTA | Module 03 bis 09, 13, 19, 20, 23, 25 |

---

## 7. Lesenachweis

**Vollständig gelesen:**

- Welt-Module 09, 13, 14, 16 (vollständig); 15, 17, 18 (die für Straße, Verkehr, Streaming relevanten Abschnitte); die Zeilen der Gruppen FZ, FP, TV, VK, UR im [Welt-Register](../World/20-Entscheidungsregister.md).
- [Gameplay-Systeme](../Gameplay-Systeme.md) §6 vollständig; §8, §10, §16 soweit Fahrzeuge, Recht und Persistenz berührt sind.
- Aus `M-PD`/`M-WB17`/`M-WB89`: die Zeilen zu Fahrzeugen, Rennen, Verkehr, Parken, Pannen, Baustellen, Lebendige Welt.
- `M-DC31` §3, §12, §13, §30 bis §36, §38, §39, §47, §48; `M-PS31` §1, §37 bis §54; `OPEN_DECISIONS.md` Abschnitt ‚Vehicles / racing‘; `M-MS1013`, `M-A1015` (Technologie), `M-A0906` (Reiche), `M-EP1018` (Abschnitt Fahrzeuge), `M-OC1018` (Zeile Transport).
- `D15-RV` vollständig; `D15-OQ` Punkt 9.
- Chat 23: Fragenblock 5 (`1024A`) und Antworten (`1025E`); Antworten `613E`, `514E`, `575E`, `705E`, `822E`, `823E` vollständig; Blöcke `1056A`/`1057E` soweit in den NPC- und Ermittlungsregistern zitiert.
- WorldForge: `WF-RC01`, `WF-D03`, `WF-RG`, `WF-DC01`, `WF-C09` vollständig; `WF-UE` (Auszug der Abschnitte zu Heightmap, Prüfpunkten, Reimport).
- Die Register von NPCs, Ermittlung, Kreaturen und Kampfsystem: die Zeilen mit Bezug zu Fahrzeugen, Fahrern, Kennzeichen, Polizei, Notdiensten und Simulationsstufen.

**Nur durchsucht oder in Auszügen gelesen:**

- Chat 23 außerhalb der genannten Bereiche; Chat 35 (`35#86A`, `35#87E`, `35#699E`, `35#700A`, `35#717E`) nur über die Zitate der Welt-Dokumentation.
- WorldForge `13_V3_SYSTEMVERTRAEGE` ohne `09`, `15_V3_IMPORT_UNREAL` ohne `01` und `05`: nur Titel.
- Prompt-Bibliothek: nur Titel und Themen.

**Nicht gelesen:** Roman- und Adult-Material (kein Fahrzeugbezug festgestellt), Unreal-Quelltext (`Unreal/`), Master-Tier 3 und 4 außer den genannten Dateien.

---

## 8. Nicht verfügbare Quellen

Diese Quellen werden vom Auftrag erwartet und **existieren nicht oder liefern nichts** (Stand 24.10.2026):

| Erwartete Quelle | Befund |
|---|---|
| Verkehrsmodell, Verkehrsregeln, Dichtekurven | nicht vorhanden; `W-FP-010`, `W-TV-008` führen es als offen |
| Straßenverkehrsrecht der Spielwelt (Höchstgeschwindigkeiten, Verkehrsregeln, Strafen) | nicht vorhanden; `M-OD` ‚Crime / law / evidence‘ ist offen |
| Fahrzeugliste, Marken, Modelle, Preise | nicht vorhanden; fiktive Namen sind beschlossen, die Liste ist offen (`W-FZ-014`, `W-FZ-017`) |
| Spezifikation des Straßengraphen mit Spuren, Schildern, Signalen, Parkplätzen | nicht vorhanden; WorldForge hat nur `RC-01` mit Kanten, Spuren (Feld ‚lanes‘), Kreuzungen und Querungen |
| Unreal-Verkehrs-Plugin oder Fahrzeugphysik-Entscheidung | nicht vorhanden; es gibt keine Entscheidung zu Chaos Vehicles, MassEntity oder einer eigenen Lösung |
| Zahl der Fahrzeuge, NPCs im Straßenverkehr, Straßenkilometer | nicht vorhanden; nur die Planungsannahme von rund 500.000 NPC-Identitäten (`W-TV-010`, 🧭) und die Weltgröße als Arbeitsszenario |
| Netzwerkarchitektur für Fahrzeuge im Koop | nicht vorhanden (`E-KO-010`, `N-KO-018`) |
| Tankstellen-, Parkplatz- und Garagenkonzept | außer ‚Parken, Valet-Service an passenden Orten‘ nicht vorhanden (`W-VK-013`) |
| Straßenverkehr in der Untergrundzivilisation | nicht vorhanden (Untergrundnetz: Schiene oder ähnlich, Technik offen) |

---

## 9. Grenzen der Rekonstruktion

1. **Die meisten Module dieses Ordners sind dünn an Entscheidungen und dick an Vorschlägen.** Das ist der Quellenlage geschuldet, nicht Nachlässigkeit. Jedes Modul weist aus, welche Zeilen übernommen und welche Vorschläge sind.
2. **Realwelt-Wissen ist kein Beleg.** Aussagen über echtes Verkehrsrecht, echte Fahrphysik oder echte Verkehrssimulation (zum Beispiel typische Fahrzeugfolgemodelle) erscheinen nur als Hinweis auf Optionen, nicht als Anforderung.
3. **Keine Zahl ist eine Entscheidung.** Wo ein Beispielwert nötig ist, steht ‚Beispielwert, nicht beschlossen‘.
4. **Unreal-Fähigkeiten sind nicht verifiziert.** Die Unreal-Version ist nicht festgelegt (`CLAUDE.md` §5), und `WF-UE` führt die lokal installierte Version als ‚noch nicht erhoben‘. Aussagen über Unreal-Systeme (zum Beispiel für Massensimulation) sind **Optionen zum Prüfen**, keine Zusagen.
5. **Kein Test wurde ausgeführt.** Der Testkatalog beschreibt künftige Prüfungen; die Prüfungen dieser Dokumentation stehen in [Modul 30](30-Abdeckungsprotokoll.md).

---

## 10. Konventionen für IDs

| Art | Form | Beispiel | Ort |
|---|---|---|---|
| Entscheidung / Anforderung | `VEH-DEC-<Gruppe>-<nnn>` | `VEH-DEC-PH-004` | Tabellen der Module 02 bis 27, erzeugtes [Register](28-Entscheidungsregister.md) |
| Konflikt | `CK-nn` | `CK-01` | [Modul 29](29-Konflikte-und-offene-Fragen.md) |
| Unbelegte Angabe | `CU-nn` | `CU-01` | Modul 29 |
| Offene Frage | `CQ-nn` | `CQ-12` | Modul 29 (jede mit Freitextoption) |
| Testfall | `CT-<Kategorie>-<nnn>` | `CT-FU-001` | [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md) |

Gruppen: `GP` Grundprinzipien, `KL` Klassen, `PH` Physik, `ST` Steuerung, `EI` Einsteigen, `ZV` Zustand, `WE` Werkstatt, `KE` Kraftstoff und Energie, `SN` Straßennetz, `KR` Kreuzungen, `SP` Stadt und Parken, `KI` Verkehrs-KI, `NF` NPC-Fahrer, `VD` Verkehrsdichte, `UP` Unfälle und Pannen, `VT` Verkehrsteilnehmer, `BZ` Besitz und Zulassung, `PV` Polizei, `IT` Interaktion und Jobs, `EJ` Ermittlung, `PE` Persistenz, `KO` Koop, `WF` WorldForge, `UE` Unreal, `LS` Simulationsstufen, `PF` Performance. Die Zuordnung Gruppe zu Modul prüft der Generator.
