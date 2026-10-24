# Kampfsystem 01: Quellenlage und Methodik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sagt, **woher** die Kampfsystem-Dokumentation stammt, welche Schlüssel und Statuswörter sie benutzt, was tatsächlich gelesen und was nur durchsucht wurde und wo die Quellen enden. Es erfindet keine Spielerentscheidungen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Das Register steht in [Modul 27](27-Entscheidungsregister.md), Widersprüche und offene Fragen in [Modul 28](28-Konflikte-und-offene-Fragen.md), die Abdeckung in [Modul 29](29-Abdeckungsprotokoll.md), die Tests in [Modul 30](30-Testkatalog-und-Abnahmekriterien.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Auftrag und Arbeitsregeln](#2-auftrag-und-arbeitsregeln)
3. [Statuswörter](#3-statuswörter)
4. [Zitierschlüssel](#4-zitierschlüssel)
5. [Quellengewicht und Rangfolge](#5-quellengewicht-und-rangfolge)
6. [Master-Konsultation nach CLAUDE.md §11](#6-master-konsultation-nach-claudemd-11)
7. [Lesenachweis](#7-lesenachweis)
8. [Grenzen der Rekonstruktion](#8-grenzen-der-rekonstruktion)
9. [Konventionen für IDs und Systemkürzel](#9-konventionen-für-ids-und-systemkürzel)

---

## 1. Befund in Kürze

1. **Die Richtung ist breit und tief beschlossen, die Zahlen fehlen.** Fast alle Kampfentscheidungen stammen aus einer zusammenhängenden Planungsfolge im Chat 23 vom 18. bis 23.08.2026 (Verletzung und Schwierigkeit `60E`–`69A`, Körper und Tod `85A`–`113A`, Waffen und Ballistik `115A`–`160A`, Nahkampf `162A`–`174A`, mehrere Gegner `221A`–`224E`, Stealth und Wahrnehmung `225A`–`258E`) und werden im Master als `M-PD` (Stand 2026-08-31) zusammengefasst. Die Quellen legen **Prinzipien, Zustandsfolgen und Optionen** fest, **keine** Schadenswerte, Reichweiten, Zeiten oder Schwellen.
2. **Der Master führt nur `M-PD` als Entscheidungsquelle für Kampf.** Die Tier-1-Suche (aktueller kanonischer Stand, Addenda 2026-10-15 und 2026-10-18) liefert zu Kampf kaum Treffer (Abschnitt 6); die Tier-3-Prompt-Bibliothek enthält 400+ Entwurfs-Prompts zu Kampf, Medizin und Animation (`M-P06`, `M-P07`, `M-P08`, `M-P10`, `M-P14`), die **alle** den Status `DRAFT_REWRITE_REQUIRED` tragen und hier nur als **Themeninventar** benutzt werden.
3. **Die DualSense-Belegung für Kampf steht nicht in den Quellen.** Kreuz = Springen ist eine Entwickleraussage vom 22.10.2026 ([Proposal 0009](../../Proposals/0009-traversal-jump-and-vault-direction.md) §3a). Quadrat = Kontextinteraktion beziehungsweise Angriff, Kreis = Ausweichen und Dreieck = schwerer Angriff (unsicher) stehen **nur im Auftrag der Projektleitung vom 23.10.2026** (Schlüssel `AUF`). In Chats, Master und Repo wurde keine Entsprechung gefunden ([Modul 06](06-Steuerung-und-Eingabekontext.md), Frage `CQ-01`). Der Chat 35 vermerkt ausdrücklich, dass eine eigene Control-Runde mit konkreten DualSense-Belegungen **noch aussteht** (`35#465A`); dort geht es um die Steuerung der Adult-Interaktionen, eine Control-Runde für das allgemeine Kämpfen ist in den Quellen nicht verzeichnet.
4. **Der Tod ist teils entschieden, teils offen, und die Quellen widersprechen sich dabei.** `M-PD` führt ‚Death / failure — OPEN‘, `M-OD` und Spielvision §14.2 ebenso; festgelegt ist dort nur die **Richtung** (kritische Zustände, kontextabhängige Rettung, artspezifische Monsterausgänge, medizinisch plausibles Todesrisiko; optionaler Checkpoint; Permadeath nur optional; kein Soulslike). Der Chat 23 enthält jedoch eine spätere Entwicklerantwort (`23#364E` P2, P3, P16, 25.08.2026): Ein Gameplay-Tod führt zum letzten Checkpoint mit nur einem **minimal überlebensfähigen** Gesundheitszustand, Permadeath ist aktivierbar, ein Story-Tod ist erzählerisch bindend. Die Nachbarordner behandeln das uneinheitlich: [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) §3.1 führt es als 📋, die NPC-Dokumentation (`N-NL-013`, `N-LB-023`) als ❓. [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) legt beide Aussagen nebeneinander (`CK-01`) und erfindet keinen Todesloop.
5. **Es gibt echte Spannungen zwischen Quellen**, die nicht entschieden sind (Modul 28): zum Beispiel die Darstellung bei Minderjährigen gegenüber der Aussage ‚keine Plot-Rüstung‘, Friendly Fire gegenüber den Schutzregeln für Crew-Figuren, Abpraller und Splitter, Quadrat als Fahr- und als Kampftaste.
6. **Die Serie und andere Spiele sind Vergleich, keine Vorlage.** Namen wie ‚Last of Us‘, ‚Sniper Elite‘ oder ‚UFC‘ in den Quellen sind Gefühls- und Tiefenvergleiche des Entwicklers, keine Übernahme von Mechaniken. Konkrete Requisiten der Serie sind ausdrücklich ausgeschlossen (`23#90A`, `23#91E`, `M-PD` Gunplay ‚original rare/legendary hunter weapons‘).

---

## 2. Auftrag und Arbeitsregeln

Auftrag der Projektleitung vom 23.10.2026 (Schlüssel `AUF`): Dokumentation und Review des Kampfsystems unter `Docs/Game/Kampfsystem/`, Implementierung nicht freigegeben, keine Git-Operationen außer lesenden. Ausgangsstand: Branch `main`, Commit `9e8598f`, Arbeitsbaum sauber (geprüft am 23.10.2026; keine Abweichung vom erwarteten Stand).

| Regel aus dem Auftrag | Umsetzung in diesem Ordner |
|---|---|
| Bestehende Entscheidungen haben Vorrang | Jede Aussage trägt Status und Quelle ([Register](27-Entscheidungsregister.md)); Vorschläge stehen nur mit Status `💡 VORSCHLAG` |
| Sechs Aussagearten (beschlossen, abgeleitet, Vorschlag, offen, Konflikt, unbelegt) | Abschnitt 3; zusätzlich `🧭 RICHTUNG` für ausdrücklich nicht abgeschlossene Entwicklerrichtungen |
| Keine erfundenen Quellen | Alle Chat-Zitate werden von `Tools/world-audit/check_docs.py` gegen die Transkripte geprüft ([Modul 29](29-Abdeckungsprotokoll.md)) |
| Widersprüche nicht eigenmächtig lösen | [Modul 28](28-Konflikte-und-offene-Fragen.md): beide Aussagen mit Quelle, Entscheidung bleibt der Projektleitung |
| Bestehende Dokumente schützen | Dieser Ordner ist neu; Änderungen an bestehenden Dateien siehe Modul 29 §7 |
| Kein produktiver Code | Nur Tabellen, Pseudocode und konzeptionelle Datenmodelle, alle als `💡 VORSCHLAG` gekennzeichnet |

Zusätzlich gelten die Regeln aus `CLAUDE.md`: Vorschläge dürfen nie stillschweigend zu Beschlüssen werden, Zahlenwerte für Balance, Fahrverhalten, KI und Simulation sind **nicht** freigegeben, und `Planning/Master` bleibt schreibgeschützt.

---

## 3. Statuswörter

Die Kampfsystem-Dokumentation benutzt die sechs Aussagearten des Auftrags und eine siebte für ausdrücklich unabgeschlossene Entwicklerrichtungen. Zeichen und Wort stehen **immer zusammen** im Register; in Fließtexten genügt das Wort.

| Zeichen | Wort | Bedeutung | Vergabe |
|---|---|---|---|
| 📋 | BESCHLOSSEN | Durch eine konkrete Entwickleraussage oder eine in `M-PD` ohne OPEN-Vermerk geführte Regel festgelegt | Entwickler-Antwort auf eine konkrete Frage (‚ja‘, ‚genau‘, eigene Formulierung) oder `M-PD`-Zeile ohne Vorbehalt |
| 🧭 | RICHTUNG | Der Entwickler hat eine Richtung gebilligt, aber ausdrücklich nicht abgeschlossen | ‚noch nicht festgelegt‘, ‚da sollten wir noch nichts festnageln‘, pauschale Zustimmung zu einem langen Assistentenblock, `M-PD` ‚Favored direction‘ |
| 🧩 | ABGELEITET | Folgt zwingend aus beschlossenen Anforderungen; die Ableitung steht daneben | Zwei oder mehr 📋-Aussagen, die zusammen etwas erzwingen; die Quellen der Prämissen stehen in der Quellenspalte |
| 💡 | VORSCHLAG | Neue Idee, Entwurf oder Beispiel; nicht genehmigt | Alles, was von einem Assistenten oder von dieser Dokumentation stammt und keine Entwicklerbestätigung trägt |
| ❓ | OFFEN | Noch nicht entschieden, dokumentiert als Frage | `M-PD`/`M-OD`-Vermerk OPEN, Entscheidungsstatus-Liste, fehlende Antwort |
| ⚠️ | KONFLIKT | Zwei Quellen widersprechen einander; beide stehen mit Quelle in Modul 28 | Keine Auflösung durch die Dokumentation |
| 🔍 | UNBELEGT | Behauptung oder Zuordnung ohne auffindbare Quelle | Beispiel: die Dreieck-Belegung (Abschnitt 8) |

**Regeln der Vergabe**

1. **Assistentenaussagen sind 💡.** Tabellen, Beispiele, Zustandsleitern und Zahlen aus einer `A`-Nachricht zählen erst dann als 📋, wenn die folgende `E`-Nachricht den Punkt ausdrücklich bejaht. Beispiel: Die Leiter ‚Gesund → leicht verletzt → verletzt → schwer verletzt → kritisch → kampfunfähig‘ stammt aus `63A`; der Entwickler nimmt in `62E` ‚die Folgen‘ an, nicht aber die Leiter als Datenmodell. Sie ist deshalb als Richtung geführt.
2. **Pauschale Zustimmung.** Antwortet der Entwickler auf einen langen Assistentenblock nur mit ‚passt‘, gilt die Zustimmung für die Richtung (🧭), nicht für jede einzelne Beispielzahl. Nummerierte Antworten (‚3. ja genau‘) werden dem nummerierten Punkt zugeordnet; die Zuordnung steht als `P<n>` in der Quellenspalte.
3. **Vorbehalte des Entwicklers gewinnen.** Wo der Entwickler bei der Zustimmung einschränkt oder korrigiert (‚aber nur…‘, ‚nicht so…‘), gilt die Einschränkung. Beispiel: Splitter nicht als Dauerschaden (`130E`).
4. **`M-PD` ist maßgeblich, aber nicht allwissend.** Eine `M-PD`-Zeile ohne OPEN-Vermerk gilt als 📋; dieselbe Stelle in einer Abschnittsüberschrift mit ‚OPEN‘ (‚Death / failure — OPEN‘, ‚Progression — OPEN FOR DEDICATED DESIGN‘) ist ❓ beziehungsweise 🧭.
5. **Spätere Entscheidungen gehen vor.** Bei Widerspruch zwischen älterem `M-PD` (2026-08-31) und einer späteren ausdrücklichen Entwickleraussage gilt die spätere; der Widerspruch steht trotzdem in Modul 28 (`CLAUDE.md` §2.1).
6. **Auftragsvorgaben sind Eingaben der Projektleitung, keine Spielentscheidungen.** Was nur im Auftrag steht (Schlüssel `AUF`), wird als 🔍 UNBELEGT geführt, solange keine Quelle im Repo, Master oder Chat die Entwicklerentscheidung trägt. Das ist ausdrücklich so vom Auftrag gefordert (‚Dreieck… nicht als endgültig beschlossen zu behandeln‘, Auftrag §5 Modul 06).

---

## 4. Zitierschlüssel

Alle Master-Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation bleibt ohne Zugriff darauf lesbar; die Schlüssel dienen der Nachprüfbarkeit.

### 4.1 Chats

Alle Chats liegen unter `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/`. Schlüssel `<Chat>#<Nachricht><Rolle>`; `E` = Rolle ‚user‘ (Entwickler), `A` = Rolle ‚assistant‘. `P<n>` meint Punkt n der nummerierten Frage in der `A`-Nachricht; die Antwort steht in der folgenden `E`-Nachricht. Dieselbe Schreibweise wie in [Ermittlung](../Ermittlung/01-Quellenlage-und-Methodik.md#2-zitierschlüssel), [Kreaturen](../Kreaturen/01-Quellenlage-und-Methodik.md#2-zitierschlüssel) und [NPCs](../NPCs/01-Quellenlage-und-Methodik.md#2-zitierschlüssel).

| Schlüssel | Chat | Zeitraum | Inhalt für Kampf |
|---|---|---|---|
| `23#` | SPN Full Master ZIP (1.109 Nachrichten) | 18.08.–05.09.2026 | Verletzung, Tod und Schwierigkeit (`60E`–`69A`, `85A`–`96A`), Körper- und Traumasystem (`98A`–`111A`), Waffenhandling und Fertigkeiten (`115A`–`123A`), Ballistik und Long Range (`125A`–`148A`), Munition und Störungen (`152A`–`160A`), Nahkampf (`162A`–`174A`), mehrere Gegner (`221A`–`224E`), Stealth, Wahrnehmung, Begleiterplanung (`225A`–`258E`), Monster-Rahmen (`293A`–`308E`), Block 8 und Block 10 (`1056A`–`1058A`, `1088A`–`1090A`) |
| `35#` | Projektwissen nutzen (951 Nachrichten) | 06.09.–04.10.2026 | Hinweis auf noch ausstehende Control-Runde (`465A`), Notwehr und Selbstverteidigung (`532E`–`534E`), Heilfähigkeiten (`26E`, `27A`) |
| `48#` | Planung ZIP Downloads (297 Nachrichten) | 05.10.–13.10.2026 | Quiz: Verletzungen durch Wesen, Gefangennahme, Fallen, Monster-KI und -Gedächtnis (`122A`–`140A`), Koop und Krankenhaus (`117E`–`121E`) |
| `19#` | Frühe SPN-Projektdatei | 12.08.2026 | nur Themenliste |

**Nicht als Quelle verwendet:** Chat 18 (Minecraft-/Voxelprojekt, getrennt vom Spiel), die Roman-Kapitel des Schreibprojekts, die Adult-Runden (sexuelle Inhalte; ihre Körper- und Reaktionsspezifikation in [Adult 23](../Adult/23-Spezifikation-Reaktions-und-Koerpersystem.md) wird nur dort verlinkt, wo sie allgemeine Körpersysteme berührt).

### 4.2 Entscheidungs- und Planungsdokumente

| Schlüssel | Datei (relativ zum kumulativen Master) |
|---|---|
| `M-PD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` (Stand 2026-08-31, ‚living source of truth‘; Regel im Kopf: ‚items explicitly marked OPEN are not implementation decisions‘). Benutzte Abschnitte: ‚Human anatomy, injury and medicine‘, ‚Death / failure — OPEN‘, ‚Difficulty / configuration‘, ‚Gunplay / weapons‘, ‚Ballistics‘, ‚Melee / grappling‘, ‚Multiple-opponent combat, improvised actions and surrender‘, ‚Stealth, perception and horror AI‘, ‚Companion autonomy, planning and field commands‘, ‚Genre and progression identity‘, ‚Progression — OPEN FOR DEDICATED DESIGN‘, ‚Body / physical response / recoil‘, ‚Cash / law / evidence‘, ‚Gore / age presentation‘, ‚Multiplayer separation‘, ‚Creature / supernatural framework — expanded‘ |
| `M-OD` | `…/docs/reference/OPEN_DECISIONS.md` (Stand 2026-08-31); Abschnitte ‚Product / world / story‘ (Tod), ‚Progression / body / survival‘, ‚Combat / injury / gore‘, ‚Crime / law / evidence‘ |
| `M-B19` | `…/docs/planning-backlog/19-stealth-horror-ai.md` (Planungsinventar, nur Themenliste) |
| `M-B22` | `…/docs/planning-backlog/22-companions-planning-orders.md` (Planungsinventar) |
| `M-B35` | `…/docs/planning-backlog/35-online-coop-lobby-persistence.md` (Planungsinventar) |
| `M-B39` | `…/docs/planning-backlog/39-law-evidence-jurisdiction-smartphone.md` (Planungsinventar) |
| `M-B41` | `…/docs/planning-backlog/41-body-adaptation-physical-response-focus.md` (Planungsinventar) |
| `M-B44` | `…/docs/planning-backlog/44-injury-gore-presentation-age-policy.md` (Planungsinventar) |
| `M-P06` | `…/docs/prompts/06-survival-anatomy-medical/` (73 Entwurfs-Prompts), nur Themeninventar |
| `M-P07` | `…/docs/prompts/07-gunplay-ballistics-ammo/` (122 Entwurfs-Prompts), nur Themeninventar |
| `M-P08` | `…/docs/prompts/08-melee-grappling/` (131 Entwurfs-Prompts), nur Themeninventar |
| `M-P10` | `…/docs/prompts/10-difficulty-hud-accessibility/` (30 Entwurfs-Prompts), nur Themeninventar |
| `M-P14` | `…/docs/prompts/14-assets-animation-gore/` (59 Entwurfs-Prompts; Waffen-, Wund- und Animationsanteile), nur Themeninventar |
| `M-MOD` | `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md` (CONFIRMED / CANONICAL: Mods ändern das Basisspiel nicht, anbieterunabhängig, `.spnmod`) |
| `M-AS` | `99_DAILY_DELTAS/2026-10-16/SPN_DAILY_DELTA_2026-10-16/docs/specs/AI_SIMULATION_BOUNDARIES.md` (Kandidatenarchitektur 6: Detailverhalten in Spielernähe, begrenzte Fernaktualisierung; **Vorschlag, nicht Kanon**) |
| `M-OQ16` | `99_DAILY_DELTAS/2026-10-16/SPN_DAILY_DELTA_2026-10-16/docs/open_questions/OPEN_QUESTIONS_2026-10-16.md` (Punkt 9: KI-/NPC-LOD, Speicherschema, deterministische Offscreen-Simulation, Tests und Leistungsbudgets als offen) |
| `M-CA0905` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (Begleiter, Spielergesundheit, Forensik, Stealth) |
| `M-B09` | `01_CURRENT_CANONICAL_STATE/BLOCK_09_STATUS.md` |
| `PE-N1022` | Entwickleraussage vom 22.10.2026 zu Living World, NPC-Gedächtnis und Nemesis (acht Anforderungen A–H, [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §1); nicht im Master |
| `AUF` | Auftrag der Projektleitung vom 23.10.2026 (Dokumentationsauftrag Kampfsystem); Eingabe, keine Spielentscheidung (Abschnitt 3 Regel 6) |

Die Prompt-Bibliothek (`M-P…`, `M-B…`) trägt laut Master-Governance den Status `DRAFT_REWRITE_REQUIRED` und ist nach `CLAUDE.md` §11 Punkt 4 ‚nicht Anforderung‘. Die Planungsinventare (`M-B…`) sind selbst als ‚PLANNING INVENTORY‘ geführt: Sie nennen zu planende Themen, keine Entscheidungen.

### 4.3 Repository-Dokumente

| Kurzform | Datei |
|---|---|
| Gameplay-Systeme §n | [`Docs/Game/Gameplay-Systeme.md`](../Gameplay-Systeme.md) (Planungsstand; §4 Verletzungen, §5 Waffen und Nahkampf, §6.3 Schaltung und Kupplung, §7 Tarnung und Horror-KI, §8 NPCs und Gedächtnis, §10 Recht und Beweise, §14.3 Animation, §16 Persistenz) |
| Spielvision §n | [`Docs/Game/Spielvision.md`](../Spielvision.md) (§9 Kreaturen, §13 Spielmodi, §14 Schwierigkeit und Tod) |
| Entscheidungsstatus §n | [`Docs/Game/Entscheidungsstatus.md`](../Entscheidungsstatus.md) (§5.6 Kampf, Verletzung, Gore als offene Liste) |
| Proposal 0008 / 0009 | [`Docs/Proposals/`](../../Proposals/) (M1-Testkarte; Traversal-Richtung) |
| `K-XX-nnn` | Register der [Kreaturen](../Kreaturen/20-Entscheidungsregister.md) |
| `E-XX-nnn` | Register der [Ermittlung](../Ermittlung/20-Entscheidungsregister.md) |
| `N-XX-nnn` | Register der [NPCs](../NPCs/26-Entscheidungsregister.md) |
| `W-XX-nnn` | Register der [Welt](../World/20-Entscheidungsregister.md) |

---

## 5. Quellengewicht und Rangfolge

Angewendet wird die Rangfolge aus `CLAUDE.md` §2 und `Docs/MasterIndex.md` §1:

1. **Spätere ausdrückliche Entwickleraussagen** (Chat-`E`-Nachrichten, Proposal-Abschnitte ‚developer, 2026-10-22‘, Auftragsvorgaben) gehen vor.
2. **`M-PD`** (2026-08-31) fasst die Chat-23-Ergebnisse zusammen. Es ist hier der Anker für Kampfaussagen; die Chat-Nachrichten dahinter werden als Belegstellen genannt.
3. **Repository-Zusammenfassungen** (Gameplay-Systeme, Spielvision, Entscheidungsstatus) sind abgeleitet. Sie werden nur dann als alleinige Quelle geführt, wenn weder `M-PD` noch ein Chat die Aussage belegt; das ist in der Quellenspalte vermerkt.
4. **Assistentenvorschläge** (`A`-Nachrichten) und **Prompt-Entwürfe** (`M-P…`) sind 💡.
5. **Nachbar-Dokumentation** (Kreaturen, NPCs, Ermittlung) ist autoritativ für ihr Themengebiet; das Kampfsystem **verweist**, statt neu zu entscheiden (Registerkennungen `K-`, `N-`, `E-`).

---

## 6. Master-Konsultation nach CLAUDE.md §11

Durchgeführt am 23.10.2026 mit `Tools/master-search.sh` (Tier 1 und 2, Regex `melee|firearm|combat|grapple|weapon|injur|ballist|ragdoll`, ergänzend `Quadrat`, `DualSense`). Der Master wurde nicht verändert.

| Nr. | Befund | Klasse | Verwendung |
|---|---|---|---|
| F1 | Tier 1 enthält zu Kampf nur Randtreffer: `PLANNING_STATUS_2026-09-05.md` (Block 10: ‚injury‘, ‚stealth‘, ‚accepted direction‘), `CANONICAL_ADDENDUM_2026-09-05.md` (Begleiter unterscheiden sich im Kampf; Spielergesundheit: Blutverlust, Brüche, Gehirnerschütterung, innere Verletzungen, Verbrennungen, Vergiftung, selektive Infektion; ‚no full rehabilitation simulator‘; Amputation und Prothesen nur über erzählte Story), `BLOCK_09_STATUS.md` (individueller Tod von Wesen bleibt dauerhaft) | CONFIRMED beziehungsweise ‚accepted direction‘ | Module 13, 14, 15, 19 |
| F2 | Tier 2 (Tagesdeltas, Audits) enthält **keine** Kampftreffer; Treffer für ‚Quadrat‘/‚DualSense‘ liegen nur im Fahrzeug-Delta vom 15.10.2026 (u. a. `ROAD_VEHICLES_2026-10-15.md`) | – | Modul 06 (Abgrenzung zu Fahren) |
| F3 | `M-PD` enthält die maßgeblichen Kampfabschnitte (Abschnitt 4.2) | CONFIRMED ohne OPEN-Vermerk, sonst OPEN | alle Module |
| F4 | `M-PD` ‚Death / failure — OPEN‘ und `M-OD` ‚Exact final death / near-death / rescue / hospital / checkpoint behavior…‘ | OPEN | Modul 14 |
| F5 | `M-OD` ‚Combat / injury / gore‘: Combat Focus (Aktivierung, Eingabe, Dauer, Erholung), ballistische, Gewebe-, Knochen- und Rüstungsparameter, Gore-Schwellen, Darstellung bei minderjährigen NPCs, Tiefe von Nahkampf, Ringen und Aufgabegriffen und Animationsabdeckung | OPEN | Module 04, 07, 12, 13, 22; Fragenliste |
| F6 | `M-OD` ‚Crime / law / evidence‘: Straftatenkatalog, Festnahmeschwellen, Strafmaß, Zuständigkeiten, Verdachts- und Beweisbewertung, Durchsuchung | OPEN | Modul 20 |
| F7 | Prompt-Bibliothek: Tracks 06 (Medizin), 07 (Schusswaffen und Ballistik), 08 (Nahkampf), 10 (Schwierigkeit), 14 (Waffen- und Animationsanteile) listen 415 Themen | DRAFT_REWRITE_REQUIRED | Themeninventar je Modul; nie Anforderung |
| F8 | Zu ‚Dreieck‘, ‚Quadrat‘, ‚Kreis‘ als Kampftasten: **keine Fundstelle** in Master, Chats oder Repo | UNBELEGT | Modul 06 |
| F9 | Zu Ragdoll, Active Ragdoll, Motion Matching: **keine Entscheidung** im Master; Animationsanforderungen stehen nur als Schichten, IK, Motion Warping und prozedurale Anpassung (Gameplay-Systeme §14.3, `M-PD`) | – | Modul 22 |
| F10 | Zu Kampf im Koop: Planungsinventar `M-B35`, Spielvision §13, `M-PD` ‚Multiplayer separation‘; keine Kampf-spezifischen Netzwerkentscheidungen | PROVISIONAL / OPEN | Modul 23 |

---

## 7. Lesenachweis

**Vollständig gelesen:**

- `M-PD`: die Abschnitte aus 4.2, einschließlich der Zeilen zu Anatomie, Tod, Schwierigkeit, Schusswaffen, Ballistik, Nahkampf, mehreren Gegnern, Stealth, Begleitern, Körper und Rückstoß, Recht und Beweisen, Gore und Alter.
- `M-OD`: die Abschnitte aus 4.2.
- Chat 23: `60E`–`69A`, `85A`–`113A` (die Antworten `E` vollständig, die langen `A`-Nachrichten `103A` und `105A` in Auszügen), `115A`–`160A` (Antworten vollständig, `A` in Auszügen), `162A`–`174A`, `221A`–`226E`, `239A`–`243A`, `253A`–`256E`.
- Repository: Gameplay-Systeme §2 bis §5, §6.3, §7, §8, §10, §14.3, §16 (soweit Kampf, KI, Recht, Animation, Persistenz), Spielvision §9, §12 bis §14, Entscheidungsstatus §5, Proposal 0009 vollständig, Proposal 0008 (Abschnitt Springen), die Register der Nachbarordner (Kreaturen, NPCs, Ermittlung) soweit Kampf berührt.
- Prompt-Bibliothek: die Dateititel der Tracks 06, 07, 08, 10, 14 und die `TRACK_CONTEXT.md` von 06, 07, 08.

**Nur durchsucht oder in Auszügen gelesen:**

- Chat 23 außerhalb der genannten Bereiche (Ergebnis: Fahrzeuge, Modbrowser, Website, Audio, Rendering, Prompt-Organisation, Roman- und Plattformthemen sind für Kampf ohne Entscheidung); `589A` (Rückstoß als ‚ausgeprägte physische Simulation‘) nur in der Trefferliste.
- Chats 35 und 48: Treffer zu Kampf, DualSense, Heilfähigkeiten; die Quiz-Ergebnisse zu Fallen, Gefangennahme und Monster-KI werden über die Register [Kreaturen](../Kreaturen/20-Entscheidungsregister.md) und [NPCs](../NPCs/26-Entscheidungsregister.md) zitiert und dort nicht neu bewertet.
- Master-Tier 3 und 4 außerhalb von `M-PD`, `M-OD`, Backlog-Titeln und Prompt-Dateinamen: nicht gelesen.

**Nicht gelesen:** RDR2-/GTA-Zusammenfassungen (Chats 27, 29) als Mechanikquelle (reine Referenzberichte), Romankapitel, Unreal-Quelltext außer den in Proposal 0009 beschriebenen Eingabebindungen.

---

## 8. Grenzen der Rekonstruktion

1. **Mehrere Entscheidungen existieren nur als Zusammenfassung.** `M-PD` und Gameplay-Systeme verdichten die Chats; die Rückverfolgung auf einzelne Nachrichten ist dort möglich, wo `23#…` genannt ist. Wo eine `M-PD`-Zeile in den gelesenen Chatbereichen **keine** eindeutige Entwickleraussage hat, steht nur `M-PD` in der Quellenspalte.
2. **Nummerierte Antworten sind schmal.** Viele Entwicklerantworten bestehen aus Zeilen wie ‚3. ja genau‘ ohne Wiederholung der Frage. Die Zuordnung zur Frage beruht auf der Nummerierung der `A`-Nachricht und kann bei Verschiebungen falsch sein; deshalb sind Fragenummern als `P<n>` ausgewiesen und die Fragenummern der `A`-Nachricht mit den Registerzeilen abgeglichen.
3. **Die DualSense-Belegung ist nur im Auftrag belegt.** Quadrat = Kontextinteraktion beziehungsweise Angriff bei Gegnernähe, Kreis = Ausweichen und Dreieck = schwerer Angriff (möglicherweise) sind Vorgaben des Auftrags ‚auf Grundlage der bisherigen DualSense-Planung‘. Eine **frühere** Planung im Repo gibt es nur für Kreuz (Springen, Proposal 0009) und für die Fahrbelegung (Gameplay-Systeme §6.3: Kreuz/X Hochschalten, Quadrat Herunterschalten). Die Dokumentation übernimmt die Auftragsvorgaben als Eingaben (🔍 UNBELEGT) und stellt sie in Modul 06 neben die Fahrbelegung.
4. **Realistische Zahlen fehlen.** Die Quellen nennen genau drei Zahlen zu Kampf: ‚25+ Trefferregionen‘ (`M-PD`), typische Gefechte ‚meist unter etwa 100 m‘ mit seltenen Schüssen von 500 bis 1000 m und mehr (`M-PD`, `23#138E` P21) und 2 bis 3 Begleiter (`N-CO-008`). Alle anderen Zahlen in diesem Ordner sind Beispiele (💡).
5. **Die Dokumentation ist keine Spielbalance.** Tabellen mit Schwellen, Zeiten und Reichweiten sind Rechenbeispiele für Datenmodelle und Tests und tragen den Hinweis ‚Beispielwert, nicht beschlossen‘.
6. **Kein Test wurde ausgeführt.** Der Testkatalog beschreibt künftige Prüfungen; die Prüfungen dieser Dokumentation (Zitate, Register, Links) stehen in Modul 29 §6.

---

## 9. Konventionen für IDs und Systemkürzel

| Art | Form | Beispiel | Ort |
|---|---|---|---|
| Entscheidung / Anforderung | `COMBAT-DEC-<Gruppe>-<nnn>` | `COMBAT-DEC-NK-004` | Tabellen der Module 02 bis 25, erzeugtes [Register](27-Entscheidungsregister.md) |
| Konflikt | `CK-nn` | `CK-03` | [Modul 28](28-Konflikte-und-offene-Fragen.md) |
| Offene Frage | `CQ-nn` | `CQ-07` | Modul 28 |
| Unklare oder unbelegte Angabe | `CU-nn` | `CU-02` | Modul 28 |
| Szenario | `CS-nn` | `CS-08` | [Modul 26](26-Querschnittsszenarien.md) |
| Test | `CT-<Kategorie>-nnn` | `CT-VM-012` | [Modul 30](30-Testkatalog-und-Abnahmekriterien.md) |

**Gruppen der Entscheidungs-IDs:** GP Grundprinzipien (Modul 02), ZA Zustände und Architektur (03), NK Nahkampf (04), FK Fernkampf (05), ST Steuerung (06), DF Defensive Aktionen (07), BT Bewegung und Traversal (08), WA Waffen und Ausrüstung (09), MU Munition und Waffenzustand (10), AU Ausdauer (11), TK Treffermodell (12), VL Verletzungen (13), BW Bewusstlosigkeit, Tod, Rettung (14), KS Kreaturenkampf (15), SJ Schwächen und Jagd (16), FF Fallen und Fang (17), KI Gegner-KI (18), NM Nemesis (19), PO Polizei und Fraktionen (20), DE Deeskalation (21), AN Animation (22), KO Koop (23), SP Speicherung (24), PF Performance (25).

**Spalten einer Registerzeile:** Aussage (**Titel.** Beschreibung) | Status | Quelle | Systeme | Folge oder offener Punkt.

**Systemkürzel (Spalte ‚Systeme‘):** BEW Bewegung und Traversal, STE Steuerung und Eingabe, WAF Waffen und Ausrüstung, MUN Munition, TRF Treffer und Körper, MED Medizin und Verletzung, KRE Kreaturen, KI Gegner-KI, NPC NPCs und Gedächtnis, ERM Ermittlung, POL Polizei und Recht, PER Persistenz, KOO Koop, ANI Animation, UI Oberfläche und HUD, SCH Schwierigkeit und Einstellungen, AUD Audio, PRF Performance, TST Tests, MOD Modding.

**Zitierformat in Zellen:** Chat `23#86E P5`, Master `M-PD` ‚Abschnitt‘, Repo `Gameplay-Systeme §5.3`, Nachbarregister `K-KO-002`.
