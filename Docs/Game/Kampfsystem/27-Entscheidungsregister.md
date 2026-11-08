# Kampfsystem 27: Entscheidungsregister

> [!IMPORTANT]
> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 BESCHLOSSEN ist vom Entwickler festgelegt oder im Master ohne Vorbehalt geführt; 🧭, 🧩, 💡, ❓, ⚠️ und 🔍 sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuswörter: [Legende](README.md#statuslegende).
> **Dieses Dokument ist erzeugt** (`Tools/world-audit/render_combat_register.py`) aus den Tabellen der Module 02 bis 25. Änderungen gehören in die Module, nicht hierher.

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Konflikte und Fragen: [Modul 28](28-Konflikte-und-offene-Fragen.md).

**Inhalt**

1. [GP: Grundprinzipien](#gp-grundprinzipien)
2. [ZA: Zustände und Architektur](#za-zustände-und-architektur)
3. [NK: Nahkampf und Kampfstile](#nk-nahkampf-und-kampfstile)
4. [FK: Fernkampf und Schusswaffen](#fk-fernkampf-und-schusswaffen)
5. [ST: Steuerung und Eingabekontext](#st-steuerung-und-eingabekontext)
6. [DF: Ausweichen, Blocken, Parieren, Konter](#df-ausweichen-blocken-parieren-konter)
7. [BT: Kampfbewegung und Traversal](#bt-kampfbewegung-und-traversal)
8. [WA: Waffenklassen und Ausrüstung](#wa-waffenklassen-und-ausrüstung)
9. [MU: Munition, Nachladen, Waffenzustand](#mu-munition-nachladen-waffenzustand)
10. [AU: Ausdauer, Erschöpfung, Belastung](#au-ausdauer-erschöpfung-belastung)
11. [TK: Treffermodell und Körperphysik](#tk-treffermodell-und-körperphysik)
12. [VL: Verletzungen, Schmerz, Kampffolgen](#vl-verletzungen-schmerz-kampffolgen)
13. [BW: Bewusstlosigkeit, Tod, Rettung](#bw-bewusstlosigkeit-tod-rettung)
14. [KS: Kreaturenkampf und Spezialfähigkeiten](#ks-kreaturenkampf-und-spezialfähigkeiten)
15. [SJ: Schwächen, Vorbereitung, Jagdmethoden](#sj-schwächen-vorbereitung-jagdmethoden)
16. [FF: Fallen, Fesselung, Lebendfang](#ff-fallen-fesselung-lebendfang)
17. [KI: Gegner-KI und Kampftaktik](#ki-gegner-ki-und-kampftaktik)
18. [NM: Nemesis, Erinnerung, persistente Kampffolgen](#nm-nemesis-erinnerung-persistente-kampffolgen)
19. [PO: Fraktionen, Polizei, Kampfkonsequenzen](#po-fraktionen-polizei-kampfkonsequenzen)
20. [DE: Kampfvermeidung, Flucht, Deeskalation](#de-kampfvermeidung-flucht-deeskalation)
21. [AN: Animationen, Trefferreaktionen, Ragdoll](#an-animationen-trefferreaktionen-ragdoll)
22. [KO: Koop, Synchronisierung, Autorität](#ko-koop-synchronisierung-autorität)
23. [SP: Speicherstände und Weltpersistenz](#sp-speicherstände-und-weltpersistenz)
24. [PF: Performance, Skalierung, Simulationsstufen](#pf-performance-skalierung-simulationsstufen)

---

## GP: Grundprinzipien

Gruppe zu [Modul 02](02-Kampfvision-und-Grundprinzipien.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-GP-001 | **Kein Kugelschwamm.** Weder Menschen noch Monster sind durch große Lebenspunktmengen schwer. Die richtige Waffe, die richtige Munition und die Schwäche der Kreatur sind das Kernstück der Jagd | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#85A` P1; Spielvision §2.1 | WAF KRE KI | – |
| COMBAT-DEC-GP-002 | **Schwierigkeit entsteht aus Wissen, Vorbereitung, Zielen, Positionierung, KI, Kreaturenregeln, Ressourcen und Verletzungen** | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; Spielvision §2.1 | KI KRE MED | Messbar nur über Verhalten, nicht über Zahlen ([Modul 30](30-Testkatalog-und-Abnahmekriterien.md)) |
| COMBAT-DEC-GP-003 | **Keine sichtbaren Monster- oder Charakterlevel, keine farbcodierte Beute-Seltenheit, keine stufengebundenen Gegner, keine automatische Gegnerskalierung, keine abstrakten Schadenszahlen als Fortschrittsmotor** | 📋 BESCHLOSSEN | `M-PD` ‚Genre and progression identity‘; Spielvision §2.1 | KRE SCH UI | Gilt auch für Trefferanzeigen: keine Schadenszahlen als Fortschrittsmotor (Anzeige als reine Rückmeldung bleibt offen, `CQ-03`) |
| COMBAT-DEC-GP-004 | **Bedrohung entsteht aus Kreaturenregeln, Wissen und Vorbereitung, Umgebung, Verletzungen, Gegnerzahl und taktischer Lage**, nicht aus einem Monsterlevel | 📋 BESCHLOSSEN | `M-PD` ‚Genre and progression identity‘; `K-GR-001` | KRE KI | – |
| COMBAT-DEC-GP-005 | **Realismus-Grundregel:** Was nicht eigens anders festgelegt ist, verhält sich so realistisch und logisch wie in der echten Welt | 📋 BESCHLOSSEN | Gameplay-Systeme §2; `23#124E`; `23#136E` | alle | Ausnahmen sind übernatürliche Regeln ([Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md)) |
| COMBAT-DEC-GP-006 | **Glaubwürdig statt wissenschaftlich perfekt.** Physik wird getreu, aber leistungsbewusst genähert; nicht perfekt, aber ‚nicht so unrealistisch‘ | 📋 BESCHLOSSEN | `23#124E`; `23#125A`; `23#136E`; `M-PD` ‚Ballistics‘ | TRF WAF PRF | Konkrete Näherungen sind Entwurf ([Modul 05](05-Fernkampf-und-Schusswaffen.md), [Modul 12](12-Treffermodell-und-Koerperphysik.md)) |
| COMBAT-DEC-GP-007 | **Spieler und menschliche NPCs teilen dieselbe Anatomie und dieselben Traumaregeln. Keine Plot-Rüstung** | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy, injury and medicine‘; `23#104E` P9, P18 | MED NPC | Spannung zu geschützten Figuren: `N-LB-017`, `N-CO-007`, NPC-Konflikt C-N1; hier `CK-02` |
| COMBAT-DEC-GP-008 | **Der Spieler kann durch plausibel tödliche Treffer sterben**, zum Beispiel durch eine Pumpgun aus nächster Nähe | 📋 BESCHLOSSEN | `23#104E` P9; `M-PD` ‚Human anatomy, injury and medicine‘ | MED | Was nach dem Tod geschieht: [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| COMBAT-DEC-GP-009 | **Tiefe Simulation im Hintergrund, verständliche Bedienung vorne:** Waffenstörungen, Griffe und Positionen werden über kontextabhängige Eingaben gelöst, nicht über Tastenkombinationstabellen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (Störungen), ‚Melee / grappling‘ (Steuerung); `23#157E`; `23#171E` | STE | Spannung zu `35#798A`: `CK-03` |
| COMBAT-DEC-GP-010 | **Fast alles ist konfigurierbar.** Realismus- und Hilfsoptionen sind einzeln einstellbar; Voreinstellungen setzen sinnvolle Standards | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `23#64E` (Nachsatz); `23#66E` P1 | SCH | Katalog der Optionen: `CQ-04` |
| COMBAT-DEC-GP-011 | **Mehrere voneinander unabhängige Einstellbereiche** (Kampf, Verletzung und Überleben, Ermittlung, Fahren, Navigation, Folgen); die Ermittlungsführung ist unabhängig vom Kampf-Schwierigkeitsgrad | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `23#66E` P1; Spielvision §7.2; `E-MO-001` | SCH ERM | – |
| COMBAT-DEC-GP-012 | **Voreinstellungen** reichen von ‚Story/Leicht‘ über ‚Hart/Survivor‘ bis zu einer extremen Stufe im Stil von ‚Grounded‘, dazu ‚Eigene‘ | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; Spielvision §14.1; `23#64E` P1 | SCH | Endgültige Namen und Zahl der Stufen sind offen (`CQ-04`) |
| COMBAT-DEC-GP-013 | **Grounded bedeutet nicht mehr Lebenspunkte,** sondern aufmerksamere, aggressivere, schneller zuschlagende und weniger nachsichtige Gegner, schwerere Verletzungen, knappe Ressourcen, hohe Letalität | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; Spielvision §14.1; `23#64E` P1; `23#86E` P1, P7 | SCH KI MED | – |
| COMBAT-DEC-GP-014 | **Die Anatomie bleibt in allen Stufen gleich;** Parameter, KI und Folgen variieren | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `23#101E` P20 | SCH MED | – |
| COMBAT-DEC-GP-015 | **Grounded schaltet Komfort- und HUD-Hilfen standardmäßig ab;** einzelne Hilfen bleiben separat einschaltbar | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `23#120E`; `23#116E` P7, P22 | SCH UI | Auflistung der abgeschalteten Hilfen: [Modul 06](06-Steuerung-und-Eingabekontext.md) |
| COMBAT-DEC-GP-016 | **Barrierefreiheit ist eine eigene Ebene** und wird nicht mit Schwierigkeit verwechselt | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; Spielvision §14.1 | SCH UI | – |
| COMBAT-DEC-GP-017 | **Nahkampf ist tief und häufig nützlich,** kein flacher Notfallknopf und kein UFC-Klon | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#163E`; `23#165E` | STE ANI | Tiefe und Animationsabdeckung sind offen (`M-OD`, `CQ-11`) |
| COMBAT-DEC-GP-018 | **Monster dürfen die menschlichen Kampfregeln brechen;** Menschen folgen nachvollziehbarer Physik | 📋 BESCHLOSSEN | `23#85A` P13; `23#86E` P13 | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md) |
| COMBAT-DEC-GP-019 | **Nicht jede Begegnung mit einem Wesen ist ein Kampf** (beobachten, verfolgen, belauschen, fotografieren, Proben nehmen sind gleichwertige Zugänge) | 📋 BESCHLOSSEN | `K-GR-006`; `E-QA-009` | KRE ERM | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-GP-020 | **Gore ist gewollt und ursachenabhängig** (Pistole ≠ Schrotflinte aus der Nähe ≠ Messer ≠ Axt ≠ Kralle) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘, ‚Gore / age presentation‘; `23#86E` P3; Gameplay-Systeme §4 | TRF ANI | Schwellen: `CQ-47` ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) |
| COMBAT-DEC-GP-021 | **Combat Focus ist die bevorzugte Alternative zu einer Zeitlupen-Fähigkeit:** keine Zeitverlangsamung, kein Wandblick, nur trainierte Reaktion und Kontrolle mit fokussierter Präsentation | 📋 BESCHLOSSEN | `M-PD` ‚Body / physical response / recoil‘; Gameplay-Systeme §5.1 | STE UI | Aktivierung, Eingabe, Dauer, Erholung: ❓ (GP-022) |
| COMBAT-DEC-GP-022 | **Combat Focus: Aktivierung, Eingabe, Rückmeldung, Dauer, Erholung und Wechselwirkung mit der Schwierigkeit sind nicht festgelegt** | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘; Entscheidungsstatus §5.6 | STE SCH | `CQ-05` |
| COMBAT-DEC-GP-023 | **Friendly Fire gegen Begleiter ist körperlich möglich und konfigurierbar;** Grounded schaltet es standardmäßig voll ein; Begleiter werden von echten Projektilen getroffen | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` (Zusatz) | KI WAF NPC | Beziehungsfolgen sind Entwurf ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)); Spannung zu geschützten Crew-Figuren: `CK-02` |
| COMBAT-DEC-GP-024 | **Fertigkeiten wachsen vor allem durch Benutzung und Training,** nicht durch Punktekauf (Schrotflinte, Sprint, Medizin, Fahren, schwache Hand) | 🧭 RICHTUNG | `M-PD` ‚Progression — OPEN FOR DEDICATED DESIGN‘; `23#116E` P16; `23#120E` | alle | Gesamtmodell ist ein eigener Planungsblock (`CQ-06`) |
| COMBAT-DEC-GP-025 | **Starke abnehmende Erträge** sollen Ausnutzen verhindern (etwa stundenlanges Sprinten gegen eine Wand) | 🧭 RICHTUNG | `M-PD` ‚Progression — OPEN…‘; Spielvision §12.1; `23#119A` (Beispiel des Assistenten), `23#120E` | AU | `CQ-06` |
| COMBAT-DEC-GP-026 | **Schwere Verletzung ist nicht automatisch Niederlage.** Der Entwickler lehnt ‚gleich Niederlage‘ als zu hart ab und bejaht die Folgen | 📋 BESCHLOSSEN | `23#62E`; `23#63A`; `M-PD` ‚Death / failure — OPEN‘ | MED PER | [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| COMBAT-DEC-GP-027 | **Kampfunfähigkeit ist nicht automatisch Tod; Tod ist ein eigenes, deutlich ernsteres Ereignis** | 🧭 RICHTUNG | `23#85A` (Abschnitt ‚Tod‘); `23#86E` P14; `23#63A` | MED PER | Tod bleibt offen (`CK-01`, `CQ-07`) |
| COMBAT-DEC-GP-028 | **Kein Speichern im aktiven Kampf.** Die Sperre gilt erst, wenn Gegner den Spieler entdeckt haben | 📋 BESCHLOSSEN | `23#366E` P1; `23#368E`; `E-QA-039` | PER | Einzelheiten: [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| COMBAT-DEC-GP-029 | **Originale, seltene und legendäre Jägerwaffen** mit IP-sicherer Hintergrundgeschichte statt Serienrequisiten; rechtliche Clearance vor Veröffentlichung | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#88E`; `23#90A`; `23#91E`; `K-JA-010` | WAF MOD | [Modul 09](09-Waffenklassen-und-Ausruestung.md) |
| COMBAT-DEC-GP-030 | **Das normale Spiel soll fair ohne Echtgeld spielbar sein; keine käuflichen Kampfwerte** | 🧭 RICHTUNG | Spielvision §13.1 | SCH | Monetarisierung ist insgesamt nicht beschlossen |
| COMBAT-DEC-GP-031 | **Realismus wird nicht mit maximaler Komplexität gleichgesetzt:** Wo die Quellen Tiefe verlangen, verlangen sie dieselbe Stelle bedienbar; wo sie Vereinfachung erlauben, ist die tiefe Simulation trotzdem im Hintergrund | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-GP-009`, `COMBAT-DEC-GP-010`; `23#157E`; `23#171E`; Auftrag §6.1 (`AUF`) | alle | Dient als Prüffrage für jede Mechanik (Abschnitt 3.2) |
| COMBAT-DEC-GP-032 | **Fünf Bewertungsdimensionen** je Mechanik: physikalische Glaubwürdigkeit, medizinische Plausibilität, spielerische Bedienbarkeit, übernatürliche Ausnahmen, technische Umsetzbarkeit | 💡 VORSCHLAG | `AUF` §6.1 (Bewertungsrahmen der Dokumentation, keine Spielregel) | alle | Abschnitt 3.2 |
| COMBAT-DEC-GP-033 | **Der Realismusgrad jeder einzelnen Kampfmechanik** ist nicht in einem Katalog festgelegt (welche Stufen es gibt, was Standard ist) | ❓ OFFEN | Entscheidungsstatus §5.6; `M-OD` ‚Combat / injury / gore‘ | SCH | `CQ-04` |

---

## ZA: Zustände und Architektur

Gruppe zu [Modul 03](03-Kampfzustaende-und-Systemarchitektur.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-ZA-001 | **Zustand folgt aus Trefferpfad und Körperzustand,** nicht aus einem linearen Abstands-Schadenswert. Die Verletzung beruht auf Restzustand des Projektils, Durchdringung und anatomischem Weg | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#107A` (Grundsatz, vom Entwickler mit ‚Perfekt‘ im Block bestätigt, `23#106E`); Gameplay-Systeme §5.2 | TRF MED | Entwurf der Kette: [Modul 12](12-Treffermodell-und-Koerperphysik.md) |
| COMBAT-DEC-ZA-002 | **Ein klassischer HP-Wert darf intern existieren, ist aber nicht die Mechanik.** Der Körperzustand umfasst Blutvolumen, Blutungsrate, Schmerz, Schock, Bewusstsein, Atmung, Adrenalin, Beweglichkeit, Gliedmaßenfunktion und Infektionsrisiko | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (Systemzustand); `23#85A` P4 (HP optional intern); `23#86E` P4 | MED | Liste ist die Mindestmenge (Modul 13) |
| COMBAT-DEC-ZA-003 | **Wahrnehmungszustände** können mindestens unaware, suspicious, investigating, alert, confirmed target und searching sein; Kampf kann in Suche zurückfallen, wenn die Sichtlinie bricht und die genaue Position verloren ist | 🧭 RICHTUNG | `M-PD` ‚Stealth, perception and horror AI‘ (‚may include‘); `23#225A`–`23#226E`; `23#253A` P13; `K-KI-005`, `K-KI-010` | KI | Übergangsregeln und Detected-Schwelle: `CQ-09` |
| COMBAT-DEC-ZA-004 | **Wissensgrade der KI** sind getrennt: bestätigt, starker Verdacht, schwacher Verdacht, unbekannt; adaptive Taktiken nutzen nur Beobachtetes oder plausibel Gefolgertes | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#242E`; `K-KI-003` | KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-ZA-005 | **Lade- und Waffenzustände sind persistent und unterbrechbar:** Patronenlager und Magazin sind getrennte Zustände, der Nachladezustand überlebt Speichern und Laden | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ | WAF PER | [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md), [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| COMBAT-DEC-ZA-006 | **Bodenkampf kennt stabile Positionen und umkämpfte Übergangszustände** (Scrambles), in denen keiner volle Kontrolle hat | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P9 (‚nicht immer eindeutige Positionen, sondern so ein Mittelding‘); `23#169E` P8–9 | BEW ANI | [Modul 04](04-Nahkampf-und-Kampfstile.md) |
| COMBAT-DEC-ZA-007 | **Genesungsstufen** akut → stabilisiert → heilend → erholend → erholt; erneute Verletzung einer noch nicht ausgeheilten Region erreicht schwere Zustände schneller | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` D; `N-LB-014` | MED | [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| COMBAT-DEC-ZA-008 | **Kampf, Verletzung, Tarnung und KI sind Risikosysteme:** Sie brauchen deterministische Simulationen, Szenariomatrizen, Replay-Tests, Fuzzing und Leistungsregression; Zufall ist über Seeds reproduzierbar | 📋 BESCHLOSSEN | Gameplay-Systeme §17.2; `23#254E` P16; `23#256E`; `M-PD` ‚Testing philosophy‘ | TST | Verpflichtet die Architektur zur Determinismusfähigkeit (ZA-012) |
| COMBAT-DEC-ZA-009 | **Rein logische Kampfregeln liegen in portablem C++20;** Unreal-Aktoren, Animation, Physik und Eingabe liegen hinter Adaptern. Engine-abhängige Systeme werden nicht künstlich engine-unabhängig gemacht | 📋 BESCHLOSSEN | Gameplay-Systeme §17.1; `CLAUDE.md` §5 | alle | Grenzziehung im Entwurf: Abschnitt 3 |
| COMBAT-DEC-ZA-010 | **StateTree organisiert Zustände der KI; eigener C++-Code übernimmt Wahrnehmung, Gedächtnis, Kampf und Sonderregeln** | 📋 BESCHLOSSEN | Gameplay-Systeme §8 (erste Aufzählung); `N-SI-002` | KI | Gilt für KI-Zustände; ob auch Spielerzustände StateTree nutzen, steht nicht fest |
| COMBAT-DEC-ZA-011 | **Der Zustand während einer laufenden Interaktion bleibt konsistent über Speichern, Laden und Streaming** (Interaktionssitzungen sind persistent) | 🧩 ABGELEITET | Prämissen: Gameplay-Systeme §3.2 (Sitzungen, Netzwerk, Persistenz); `COMBAT-DEC-ZA-005` | PER KOO | Sperre im aktiven Kampf macht den Fall selten (`COMBAT-DEC-GP-028`) |
| COMBAT-DEC-ZA-012 | **Schichtenmodell** Eingabe → Absicht → Kämpferzustand → Aktion → Trefferauflösung → Verletzung → Folgeereignisse → Darstellung | 💡 VORSCHLAG | Abschnitt 3 (Entwurf dieses Ordners) | alle | Prüfung durch Projektleitung |
| COMBAT-DEC-ZA-013 | **Eine Tatsache hat einen Besitzer;** Querwirkungen laufen als Ereignisse, nicht als direkte Schreibzugriffe | 💡 VORSCHLAG | Übernommen aus [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7; `K-LW-007` | alle | Abschnitt 5 |
| COMBAT-DEC-ZA-014 | **Ein Zustandsmodell für Kämpfer** mit Hauptmodus (Fortbewegung, Fernkampfbereitschaft, Schlagabtausch, Clinch, Boden, Gegriffen, Kampfunfähig, Bewusstlos, tot) und überlagernden Aktivitäten (Nachladen, Störungsbeseitigung, Behandeln, Tragen, Fahren) | 💡 VORSCHLAG | Abschnitt 4 | BEW ANI KI | Namen, Zahl und Übergänge nicht beschlossen |
| COMBAT-DEC-ZA-015 | **Die Stufenleiter ‚gesund … kritisch … Kampfunfähigkeit … Tod‘ ist eine abgeleitete Zusammenfassung des Körperzustands,** kein Primärzustand | 💡 VORSCHLAG | Leiter aus `23#63A`, `23#87A` (G), `23#98A`; Entwickler bejaht die Folgen, nicht die Leiter (`23#62E`, `23#88E`) | MED UI | Schwellen: `CQ-10` |
| COMBAT-DEC-ZA-016 | **Hochfrequente Kampfdaten (Projektile im Flug, Trefferdetails, Animationszeit) laufen nicht durch das Weltzustandsjournal;** nur Folgen mit langfristiger Bedeutung werden dort festgehalten | 💡 VORSCHLAG | Anwendung von [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6; `K-LW-008` | PER PRF | Abschnitt 5; Modul 24 |

---

## NK: Nahkampf und Kampfstile

Gruppe zu [Modul 04](04-Nahkampf-und-Kampfstile.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-NK-001 | **Hybrider Straßen-/Jäger-Nahkampf:** Schlagen, Treten, Fußarbeit, Distanzmanagement, Finten, Block/Parade/Ausweichen, Clinch, Ringen, Takedowns, Griffkampf, bodennahe Kontrolle, Scrambles, Aufgabegriffe | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#165E`; `23#167E` P1 | STE ANI | – |
| COMBAT-DEC-NK-002 | **Leichte und schwere Angriffe** existieren | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#163E` P1 | STE | Die Belegung der schweren Angriffstaste ist offen (Dreieck, [Modul 06](06-Steuerung-und-Eingabekontext.md)) |
| COMBAT-DEC-NK-003 | **Schlagvarianten in großer Zahl,** mit vielen Kopf- und Körpertreffern, Winkeln, Konterschlägen und Übergängen; ‚mehr Körpertreffer und generell mehr Auswahl‘ | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P2; `23#169E` P1–P2 | ANI | Technikfamilien als Inventar: Abschnitt 4; Umfang: `CQ-11` |
| COMBAT-DEC-NK-004 | **Kombinationen sind keine ununterbrechbaren Animationsketten:** Fehlschläge, Blocks und Konter verändern den Zustand; eine Kombination kann mittendrin zerfallen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#164A`; `23#165E` | STE ANI | Verhältnis zur gelernten Combo-Bibliothek: `CK-03` |
| COMBAT-DEC-NK-005 | **Fußarbeit und Reichweite zählen** (Distanz schließen und öffnen, Winkel wechseln) | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ | BEW ANI | – |
| COMBAT-DEC-NK-006 | **Keine harte Zielaufschaltung als Standard.** Schläge können verfehlen; eine optionale weiche Zielhilfe ist konfigurierbar. Der Entwickler: kein Lock-on, ‚weil man auch Schläge nicht treffen kann‘, aber die Wahl soll der Spieler haben | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#163E` P21 | STE SCH | Stärke und Standardwerte je Stufe: `CQ-15` |
| COMBAT-DEC-NK-007 | **Das System berücksichtigt die Kampfdistanz automatisch** (weit, Boxdistanz, Nahdistanz, Clinch, Boden); es gibt nicht ‚die eine beste Kombination‘ | 📋 BESCHLOSSEN | `23#166A` P1; `23#167E` P1 | STE | Zonengrenzen sind Konfiguration |
| COMBAT-DEC-NK-008 | **Clinch:** Kontrolle über Kopf, Arm, Unterhaken, Überhaken, Körper; Wanddruck, kurze Schläge, Knie, Takedown-Versuche und Verteidigung | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (clinch control, under/overhooks, body/arm control, wall pressure); `23#167E` P5 | BEW ANI | – |
| COMBAT-DEC-NK-009 | **Takedowns aus mehreren Familien** (ringerartig, wurfartig, situative Straßenvarianten) mit Verteidigung (Sprawl, Winkelwechsel); kein Takedown gelingt garantiert, Erfolg hängt von Timing, Position, Balance, Kraft, Können, Verletzung und Gegnerreaktion ab | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (trips/throws/takedowns); `23#167E` P6–P7 (‚passt gut‘, ‚perfekt‘) | BEW ANI | Die Erfolgsfunktion ist Entwurf ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) |
| COMBAT-DEC-NK-010 | **Bodenkampf mit stabilen Positionen und umkämpften Übergängen** (Scrambles), in denen keiner volle Kontrolle hat; ‚nicht immer eindeutige Positionen, sondern so ein Mittelding‘, wie in echten Kämpfen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P8–P9; `23#168A` | BEW ANI | Stellenwert der Positionsliste: Abschnitt 4 |
| COMBAT-DEC-NK-011 | **Aufgabegriffe wirken auf den Körper** (Würgen, Armhebel, Schulterhebel, Beinhebel) und enden je nach Kontext mit Abklopfen, Bewusstlosigkeit oder echter Verletzung; kein Balken | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P17; `23#168A` | MED ANI | Tiefe: `CQ-11` |
| COMBAT-DEC-NK-012 | **Im tödlichen Straßenkampf kann anhaltendes Würgen nach der Bewusstlosigkeit töten;** auch eine Guillotine, aus der der Gegner nicht herauskommt, kann töten | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P17 | MED | Kein eigener Hinrichtungsbefehl (`23#168A`, Vorschlag) |
| COMBAT-DEC-NK-013 | **Fight Club:** gleiches physisches Kampfsystem; Ehrenkodex: niemand tötet oder verletzt schwer; Aufgabegriffe werden bei Abklopfen oder Bewusstlosigkeit gelöst; Verstöße haben ernste soziale, rechtliche und Spielfolgen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#169E` P17; `23#167E` P17 | NPC POL | Folgen im Detail: NK-014 |
| COMBAT-DEC-NK-014 | **Fight-Club-Ausgestaltung (Entwurf des Assistenten):** sofort loslassen bei Abklopfen, Stopp bei Bewusstlosigkeit, keine absichtlichen schweren Verletzungen, kein Weiterangreifen auf Wehrlose, Waffen in normalen Kämpfen verboten, Ringrichter oder Organisatoren greifen ein, Verstoß führt zu Ausschluss, Rufverlust, Feindschaften, Ermittlungen | 💡 VORSCHLAG | `23#170A` (Assistent; vom Entwickler nicht einzeln bestätigt, `23#171E` bezieht sich auf die Grapple-Frage) | NPC POL | Umfang: `CQ-13` |
| COMBAT-DEC-NK-015 | **Jeder Kämpfer und Jäger hat einen eigenen Stil und eigene Vorlieben,** keine starren Klassen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#167E` P11; `23#168A` | NPC KI | Datenmodell NK-016 |
| COMBAT-DEC-NK-016 | **Stilprofil als Vorschlag:** je Kämpfer Neigungen über Schlagen, Tritte, Ringen, Bodenkampf, Distanzverhalten (druckvoll/abwartend), keine Level | 💡 VORSCHLAG | `23#168A` (Beispielprofile des Assistenten); `M-P08` (Themeninventar ‚fighter-style-profile‘) | NPC KI | Abschnitt 5 |
| COMBAT-DEC-NK-017 | **Menschliche Gegner können fliehen, erstarren oder sich ergeben,** wenn sie genug eingeschüchtert sind; Furcht und Moral bleiben intern und zeigen sich im Verhalten | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘, ‚Multiple-opponent combat…‘; `23#222E` (Zusatz: ‚ja das wäre möglich wenn man den Gegner richtig Angst macht‘) | KI NPC | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-NK-018 | **Mehrere Gegner greifen nicht höflich nacheinander an:** gleichzeitiger Druck, Flankieren, Greifen, Ausgänge abschneiden, Chancen nutzen | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#221A` P1; `23#222E` P1 (‚das ist in tlou2 auch so glaub ich‘) | KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-NK-019 | **Körperkollision, Raumgeometrie und Engstellen gelten auch für Gegner;** sie behindern einander, Realismus wird nicht zum dauerhaften Betäubungs-Lock | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P2–P3 | BEW KI | – |
| COMBAT-DEC-NK-020 | **Griffkampf gegen mehrere Gegner ist erlaubt, aber systemisch gefährlich,** nicht künstlich gesperrt | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘, ‚Multiple-opponent combat…‘; `23#222E` P4; `23#167E` P13 | KI | – |
| COMBAT-DEC-NK-021 | **Improvisierte Gegenstände und die Umgebung** (Flasche werfen, Werkzeug, Wände, Möbel, Türen) können genutzt werden | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P5; `23#167E` P13 (‚wenn man was parat hat eine Flasche werfen, Messer zücken‘) | WAF BEW | Gegenstandsliste offen |
| COMBAT-DEC-NK-022 | **Eine normale Glasflasche zerbricht nach einem harten Schlag;** ein Rest kann als improvisierter Gegenstand bleiben | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P6 (‚Glas ist nach 1 Schlag kaputt‘) | WAF | – |
| COMBAT-DEC-NK-023 | **Wechsel zwischen Schusswaffe, Nahkampf, Messer und Improvisation sind fließend;** extrem enge Pistolenhaltung ist möglich | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P7 | WAF STE | Zugriffszeiten nach Trageort: [Modul 09](09-Waffenklassen-und-Ausruestung.md) |
| COMBAT-DEC-NK-024 | **Gegner können eine Schusswaffe physisch greifen und kontrollieren;** die Mündungsrichtung bleibt im Handgemenge physikalisch bedeutsam | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; `23#162A` P13; `23#163E` P13 (‚ja das wäre stark‘) | WAF ANI | Lange Waffen sind anfälliger (Vorschlag `23#162A`) |
| COMBAT-DEC-NK-025 | **Nahkampfwerkzeuge in Kategorien** (unbewaffnet, kleine, mittlere, große Waffen) mit Zielkonflikten: mehr Wirkung bedeutet langsamer, schwerer, schlechter in engen Räumen, mehr Ausdauerverbrauch; die konkrete Waffenliste folgt später | 🧭 RICHTUNG | `23#162A` P2; `23#163E` P2 (‚die ganzen Waffen werden wir wenn wir die Schusswaffen machen dazu besprechen‘); `M-PD` ‚Gunplay / weapons‘ (Zielkonflikt für Waffen allgemein) | WAF | Liste: [Modul 09](09-Waffenklassen-und-Ausruestung.md), `CQ-33` |
| COMBAT-DEC-NK-026 | **Verletzungen beeinflussen den Nahkampf:** verletzter Arm (schwächerer Schlag, schlechteres Blocken, große Zweihandwaffen kaum brauchbar), Beinverletzung (Balance, Ausweichen, Tritte), Blutverlust (Kraft, Reaktion, Erschöpfung) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (Gliedmaßenfunktion, Waffeneinsatz, Balance, Ausdauer); `23#162A` P15; `23#163E` P15 | MED | – |
| COMBAT-DEC-NK-027 | **Treffer werden anatomisch ausgewertet** (Messerstich: Region, Tiefe, Gewebe, möglicherweise Gefäß oder Organ; Axt: Schneiden plus Energie plus Quetschung; Hammer: stumpfes Trauma, Bruchrisiko) | 📋 BESCHLOSSEN | `23#162A` P3; `23#163E` P3 (‚ja genau‘); `M-PD` ‚Human anatomy…‘ (Traumaarten) | TRF MED | Entwurf: [Modul 12](12-Treffermodell-und-Koerperphysik.md) |
| COMBAT-DEC-NK-028 | **Schläge haben Folgen** (Gesicht: Schmerz, Benommenheit; harter Kopftreffer: Gehirnerschütterung, Bewusstlosigkeit möglich; Tritt gegen Knie: Balanceverlust, Gelenkverletzung; Schlag gegen verletzte Rippen besonders problematisch) | 📋 BESCHLOSSEN | `23#162A` P4; `23#163E` P4; `M-CA0905` (Gehirnerschütterung als Teil der Spielergesundheit) | MED | Wirkstärken sind offen |
| COMBAT-DEC-NK-029 | **Nahkampf braucht Ausdauer,** aber ein trainierter Hunter ermüdet nicht nach wenigen Schlägen; schwere Waffen belasten mehr, Verletzung und Blutverlust wirken mit | 📋 BESCHLOSSEN | `23#162A` P5; `23#163E` P5; `23#116E` P21 | AU | [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md) |
| COMBAT-DEC-NK-030 | **Finisher nur bei tatsächlicher Handlungsunfähigkeit** und passendem Kontext, Waffe und Wesen; kein permanenter Arcade-Knopf | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (‚Finishers only when target is effectively incapacitated…‘); `23#163E` P22 (‚B, ja warum nicht, aber nur wenn das Monster/Person sich nicht mehr wirklich bewegen kann‘) | ANI KRE | Umfang: `CQ-16` |
| COMBAT-DEC-NK-031 | **Steuerung ist kontextabhängig und lernbar,** der Spieler lernt keine riesigen Kombotabellen; Position, Distanz, Griff, Richtung und Kontext bestimmen die sinnvollen Aktionen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#171E`; `23#172A` (Ausgestaltung des Assistenten, 💡) | STE | Belegung: Modul 06 |
| COMBAT-DEC-NK-032 | **Monster nutzen die menschlichen Nahkampfregeln nur, wo Anatomie und Verhalten es erlauben;** humanoide übernatürliche Gegner können mehr davon nutzen; ein übernatürlich starkes Wesen lässt sich trotz perfekter Position nicht automatisch kontrollieren | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#166A` P14; `23#167E` P14 (‚genau‘) | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md) |
| COMBAT-DEC-NK-033 | **Untergrund und Umgebung verändern den Kampf** (Asphalt, Schlamm, Motelzimmer, Treppe, Flur, Auto); ein harter Sturz auf Beton verletzt; Gegner können gegen Kanten gestoßen werden | 📋 BESCHLOSSEN | `23#166A` P12; `23#167E` P12 (‚passt‘) | TRF BEW | – |
| COMBAT-DEC-NK-034 | **Leistung im Nahkampf folgt dem tatsächlichen Körper-, Trainings- und Verletzungszustand;** keine pauschalen Geschlechtsmultiplikatoren oder Sperren; Kraft ist nicht dasselbe wie sichtbare Muskelgröße | 📋 BESCHLOSSEN | Spielvision §6.1; Gameplay-Systeme §9.2; `M-PD` ‚Body / physical response / recoil‘ | MED NPC | Größe und Kraft als Kampfmerkmal: `23#166A` P11 (Vorschlag) |
| COMBAT-DEC-NK-035 | **Grundtechniken sind früh verfügbar** (zum Beispiel Haken, Jab), anspruchsvolle (zum Beispiel ein Spinning Roundhouse Kick) ‚wissen die wenigsten‘ | 📋 BESCHLOSSEN | `35#791E` P3; `35#792A`; `35#797E` | STE | Liste der Grundtechniken offen (`CQ-12`) |
| COMBAT-DEC-NK-036 | **Fortgeschrittene Techniken werden über Training, Gyms, Trainer und andere Kämpfer erlernt;** man kann diese Personen kennenlernen | 📋 BESCHLOSSEN | `35#791E` P5; `35#792A`; `35#798A` | NPC STE | `CQ-12` |
| COMBAT-DEC-NK-037 | **Gelernte Techniken können verbessert werden, aber nicht über ein normales Upgrade-System** (Entwickler: ‚ein krasseres‘); kein pauschales Prozent-Upgrade | 📋 BESCHLOSSEN | `35#791E` P3; `35#794A` (Assistentenzusammenfassung: ‚kein stumpfes +5%-Skilltree-System‘) | STE | Mechanik offen (`CQ-12`) |
| COMBAT-DEC-NK-038 | **Unbekannte Techniken sind nicht voll nutzbar;** im Menü erscheinen höchstens Name oder Hinweis. Nur freigeschaltete Techniken werden mit Beschreibung, Tastenkombination und Vorschau angezeigt | 📋 BESCHLOSSEN | `35#791E` P3, P5; `35#794A` | UI STE | Menü und Combo-Rolle: `CQ-12`, `CK-03` |
| COMBAT-DEC-NK-039 | **Das Kampf-Lernsystem ist vom Interaktionssystem der Adult-Module getrennt;** beide können denselben Eingabe- und Animationsunterbau teilen | 📋 BESCHLOSSEN | `35#797E` (‚bei Kampf soll das so sein wie ich das gesagt habe‘); `35#798A` | STE ANI | Die Hybrid-Steuerung (Aktion plus analoge Kontrolle) gilt dort, nicht im Kampf |
| COMBAT-DEC-NK-040 | **Direct Control gegenüber Assisted Control im Kampf** (freie Körpersteuerung oder aktionsgestützte Combos) ist nicht entschieden; die Fragen bleiben ausdrücklich offen | ❓ OFFEN | `35#791E` P1, P2, P4, P6; `35#792A` (‚1–2 bleiben OPEN‘); `35#794A` | STE | `CQ-02` |
| COMBAT-DEC-NK-041 | **Stealth-Takedowns sind kein Sofort-Tod-Knopf:** bei Menschen sehr wirksam, aber fehlbar; bei Monstern artabhängig, manche erlauben keinen normalen Stealth-Takedown | 📋 BESCHLOSSEN | `23#162A` P18; `23#163E` P18 (‚ja bei Monstern ist das eher schwierig, bei Dämonen oder so würde es gehen‘) | KI KRE | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-NK-042 | **Begleiter greifen im Nahkampf ein** (Monster vom Partner wegziehen, schießen, schlagen; Partner wird niedergerissen und man muss entscheiden); Vertrauen kann die Zusammenarbeit beeinflussen | 🧭 RICHTUNG | `23#162A` P19; `23#163E` P19 (‚passt‘) | NPC KI | Gesamtregeln für Begleiter: `N-CO-006` |
| COMBAT-DEC-NK-043 | **Gore im Nahkampf ist ursachenabhängig** (Messer ≠ Hammer ≠ Axt); sehr brutal und realistisch | 📋 BESCHLOSSEN | `23#162A` P17; `23#163E` P17 (‚ja sehr brutal und realistisch‘) | TRF ANI | Schwellen: `CQ-47` |
| COMBAT-DEC-NK-044 | **Auf Grounded reagieren Gegner im Nahkampf schneller, nutzen Öffnungen, verfolgen nach, haben weniger künstliche Pausen;** Grapples sind gefährlicher; keine künstlichen zehnfachen Lebenspunkte | 📋 BESCHLOSSEN | `23#162A` P20; `23#163E` P20 (‚ja sie sind einfach schlauer, aggressiver‘) | KI SCH | – |
| COMBAT-DEC-NK-045 | **Entwaffnung gilt für Spieler und Gegner** (harter Armtreffer senkt die Griffkraft, die Waffe fällt); die Waffe liegt wirklich auf dem Boden und muss aufgenommen werden | 📋 BESCHLOSSEN | `23#162A` P14; `23#163E` P14; Gameplay-Systeme §2 (lose physische Gegenstände) | WAF PER | Beweis- und Aufräumregeln: [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-NK-046 | **Gegner sind keine Hitpoint-Puppen:** ein Messer im Unterarm schützt der Mensch den Arm, ein Beintreffer lässt ihn humpeln; bei Monstern hängt es von Anatomie und Schmerzempfinden ab | 📋 BESCHLOSSEN | `23#162A` P16; `23#163E` P16; `23#86E` P3 (humpeln, Bein halten) | KI ANI | Darstellung in späteren Versionen (`23#86E` P3) |
| COMBAT-DEC-NK-047 | **Kämpfe gehen am Boden weiter:** wer niedergeworfen ist, kann treten, blocken, wegkriechen, aufzustehen versuchen oder eine Waffe ziehen; Monster können ihn dort bedrängen | 📋 BESCHLOSSEN | `23#162A` P10; `23#163E` P10 (‚genau‘) | BEW | Eignung der Waffenziehung am Boden: `M-P08` (Themeninventar) |
| COMBAT-DEC-NK-048 | **Befreiung aus einem Griff geschieht ohne Quick-Time-Spam** (Richtung, Position, Kraft, kontextabhängige Aktion, Messer, Tritt, Begleiter greift ein) | 📋 BESCHLOSSEN | `23#162A` P9 (Vorschlag); `23#163E` P9 (‚gut‘) | STE | Eingabe: Modul 06 |
| COMBAT-DEC-NK-049 | **Eine Langwaffe ist ein reales Objekt im Nahkampf:** wegdrücken, Handschutz blocken, Schaftschlag; das kann die Waffe beschädigen (teure Optik) | 📋 BESCHLOSSEN | `23#162A` P12; `23#163E` P12 (‚natürlich‘) | WAF | Waffenschaden: [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md) |

---

## FK: Fernkampf und Schusswaffen

Gruppe zu [Modul 05](05-Fernkampf-und-Schusswaffen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-FK-001 | **Eine Schusswaffe hat ein physisches Profil** (Masse, Schwerpunkt, Gesamtlänge, Rückstoß, Zustand, Befüllung) und ist kein Damage-Wert; eine objektiv stärkere Waffe kann situativ schlechter sein | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#115A` P1 (Beispiel); `23#116E` P1 | WAF | Felder und Werte: Entwurf (Abschnitt 3) |
| COMBAT-DEC-FK-002 | **Die Handhabung umfasst Masse, Schwerpunkt, Länge, Rückstoß, tiefe Bereitschaftsstellung, Punktschießen, Visieren (ADS), Raumbeschränkungen, Ermüdung, Atmung, Verletzungseinfluss, schwache Hand und mechanisch korrekte Bedienung** | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P3, P5, P7–P9, P13 | WAF ANI | – |
| COMBAT-DEC-FK-003 | **Schwere und lange Waffen können mehr Energie und Reichweite liefern,** haben aber Nachteile bei Handhabung, Platz und Ermüdung. Der Entwickler: ‚die große Waffe wird vermutlich mehr Schaden machen können‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P2; `23#117A` | WAF | – |
| COMBAT-DEC-FK-004 | **Anschlagzustände:** tiefe Bereitschaft, Punktschießen und Visieren (ADS) werden unterschieden; die Waffe wird in Third Person sichtbar korrekt angeschlagen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#115A` P3 (Zustände, Entwurf des Assistenten); `23#116E` P13 | WAF ANI | Namen und Übergangszeiten: Entwurf |
| COMBAT-DEC-FK-005 | **Rückstoß ist physikalisch;** Training verbessert die Kontrolle, nicht den Impuls der Waffe | 📋 BESCHLOSSEN | `M-PD` ‚Body / physical response / recoil‘; `23#116E` P5 (‚Rückstoß soll realistisch sein‘); `23#115A` P5–P6 | WAF STE | Rückstoßmodell: Entwurf; Zielhilfe `CQ-15` |
| COMBAT-DEC-FK-006 | **Ermüdung, Atmung, Schmerz und Verletzung beeinflussen das Zielen,** abschaltbar in den Einstellungen. Auf Grounded ist die Aussage des Entwicklers uneindeutig | 🧭 RICHTUNG | `23#116E` P7 (‚kann aber ausgeschaltet werden. auf grounded würde ich aber alle solche sachen disabled machen‘); `23#117A` (Auslegung des Assistenten) | WAF SCH | Lesart: `CU-02` |
| COMBAT-DEC-FK-007 | **Mit verletztem Schussarm kann eine Pistole mit der schwachen Hand geführt werden,** mit reduzierter Kontrolle und langsamerer Handhabung. ‚Bei Pistole dann vielleicht in der schwachen Hand, aber Manövrierbarkeit eingeschränkt‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P8; `23#117A` | WAF MED | Weitere Einschränkungen (Reloads, Sicherung): Entwurf von `23#117A` |
| COMBAT-DEC-FK-008 | **Der Charakter unterstützt viele Körperhaltungen** und ist nicht künstlich eingeschränkt | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P14 | BEW ANI | Haltungsliste offen |
| COMBAT-DEC-FK-009 | **Lange Haltezeiten ermüden moderat,** nicht nach wenigen Sekunden; wiederholtes Überlasten verlangsamt den Wiederaufbau der Ausdauer | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P21; `23#118E` P21 | AU | [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md) |
| COMBAT-DEC-FK-010 | **Raumbeschränkungen wirken auf Waffen** (lange Waffe an der Wand); der Entwickler will außerdem richtig dunkle, gruselige Innenräume. Die Ausgestaltung (Waffe zurückziehen, Bereitschaftsposition ändern) ist ein Entwurf des Assistenten | 🧭 RICHTUNG | `M-PD` ‚Gunplay / weapons‘ (room constraints); `23#115A` P4 (Entwurf); `23#116E` P4 | WAF ANI | Details: Abschnitt 3.3 |
| COMBAT-DEC-FK-011 | **Waffenbedienung ist mechanisch korrekt** (Pumpgun: Vorderschaft, Hülsenauswurf, Zuführung; Pistole: Schlitten, Slide Lock, Magazin; Revolver: Trommel, Kammern, Auswurf) und gut animiert | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (‚mechanically correct manipulation‘); `23#115A` P9; `23#116E` P9 (‚ja das muss gut animiert werden‘) | WAF ANI | Asset-Anforderungen: `M-P14` (Inventar) |
| COMBAT-DEC-FK-012 | **Munitionsanzeige ist binär,** an oder aus (nichts dazwischen); ohne Anzeige zählt man Schüsse und prüft Waffe und Magazin körperlich; Grounded schaltet sie standardmäßig ab | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `23#116E` P22; `23#118E` P22; `23#120E` | UI SCH | [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md) |
| COMBAT-DEC-FK-013 | **Flugbahn wird physikalisch berechnet:** Flugzeit, Fall, Luftwiderstand, Geschwindigkeitsverlust und Restenergie aus plausiblen Formeln, besonders bei Jagd- und Präzisionsgewehren; ‚nicht so komplexe Formeln‘ reichen | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#134E` P1–P2; `23#136E` | TRF PRF | Formeln: Entwurf |
| COMBAT-DEC-FK-014 | **Keine künstliche Maximalreichweite,** bei der Geschosse verschwinden | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#134E` P3 (‚das ist wichtig‘) | PRF | Lebensdauer eines Projektils: Entwurf (Modul 25) |
| COMBAT-DEC-FK-015 | **Projektildefinition** mit Mündungsgeschwindigkeit, Masse, ballistischem Koeffizienten beziehungsweise Luftwiderstandsverhalten und Aufbau | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P3 (Rückfrage des Entwicklers zum Koeffizienten, vom Assistenten erklärt `23#139A`) | MUN | Datenwerte offen |
| COMBAT-DEC-FK-016 | **Durchdringung hängt ab von** Projektil, Energie, Aufbau, Winkel, Material und Dicke; dünne Deckung (Holztür, dünne Wand, leichte Möbel) kann durchschlagen werden, massive Deckung (dicker Beton, schwere Metalltür, Pfeiler) stoppt und zeigt kosmetische Einschläge; mehrere Schichten verringern den Zustand schrittweise | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#125A`; `23#126E` (‚A, wenn die Tür nicht so massiv ist; sonst nur Einschusslöcher‘); `23#127A` | TRF | Materialtabelle: `CQ-45` |
| COMBAT-DEC-FK-017 | **Fahrzeuge haben unterschiedliche ballistische Strukturen** (Türblech ≠ Motorblock); Glas kann brechen und ablenken | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#154A` P9–P10 (Entwurf); `23#155E` P9–P10 | TRF | Schießen von und auf Fahrzeuge als Mechanik: `CQ-18` |
| COMBAT-DEC-FK-018 | **Gefährliche Abpraller sind selten** (Winkel, Geschwindigkeit, Energie, Geschoss, Material); auf Grounded standardmäßig aktiv, in anderen Voreinstellungen standardmäßig aus, aber einschaltbar | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#130E`; `23#131A` | TRF SCH | – |
| COMBAT-DEC-FK-019 | **Gewöhnliche Materialfragmente (Splitter) sind überwiegend kosmetisch;** nur in begründeten Sonderfällen gameplayrelevant. Der Entwickler: ‚das passiert einem in real life nicht mal‘ | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#130E` | TRF | – |
| COMBAT-DEC-FK-020 | **Wind wirkt mit wachsender Flugzeit;** bewegte Ziele brauchen echtes Vorhalten aus der Flugzeit; Höhe und Luftdichte können modelliert werden, Temperatur intern ohne großen Einfluss | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#134E` P7–P8; `23#138E` P5–P9 (P9 ‚ich glaub nicht, dass das so einen Unterschied macht‘) | TRF | – |
| COMBAT-DEC-FK-021 | **Schusswinkel bergauf und bergab, Visierhöhe über der Laufachse und Verkantung (Cant)** sind physikalisch korrekt; Transschall und Spin-Drift dürfen angenähert werden; **Coriolis ist nicht nötig** (‚nein brauchen wir nicht, ist zu klein‘) | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P10–P15 | TRF | – |
| COMBAT-DEC-FK-022 | **Anbauteile verändern relevante Eigenschaften physikalisch** (Optikgeometrie und Nullpunkt, Schalldämpfer, Lauflänge und Mündungsgeschwindigkeit, Zweibein, Magazinmasse) | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P16 (‚die verschiedenen Aufsätze machen einen Unterschied‘) | WAF | Liste offen |
| COMBAT-DEC-FK-023 | **Ein Schalldämpfer macht Überschallschüsse nicht lautlos;** Unterschallmunition hat eigene Kompromisse | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#133A` P9 (Entwurf); `23#134E` P9 (‚das ist auch gut‘) | WAF AUD KI | Geräuschmodell: [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-FK-024 | **Nullpunkteinstellung ist echte Optik- und Lauf-Geometrie,** keine veränderte Geschossphysik | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P2 | WAF | – |
| COMBAT-DEC-FK-025 | **Typische Gefechte liegen meist unter etwa 100 m,** seltene Schüsse von 500 bis 1000 m und mehr müssen funktionieren | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P21 | TRF PRF | Prüfzahl für Tests (kein Balancewert) |
| COMBAT-DEC-FK-026 | **Langstrecken-Üben ist vorgesehen:** Schießstände, Unterschlupf-Training, Trainingsumgebungen und optionale Schießwettbewerbe; ‚viele Möglichkeiten, den Schuss zu üben und nicht nur an Gegnern‘ | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P2; `23#141E` P1–P4 (‚vielleicht auch Events, wo man schießen gegen andere Leute kann‘) | UI WAF | Umfang der Events: `CQ-22` |
| COMBAT-DEC-FK-027 | **Im härtesten Modus kann ein tatsächliches Entfernungsmessgerät nötig sein,** anstatt einer magischen Entfernungsanzeige. Der Entwickler: ‚vielleicht ein extra Gerät‘ | 🧭 RICHTUNG | `M-PD` ‚Ballistics‘ (‚may require‘); `23#138E` P1 | UI SCH | Hilfen je Stufe: `CQ-20` |
| COMBAT-DEC-FK-028 | **Kill-Cams für seltene, hochwertige Treffer:** filmische Projektilverfolgung, optional anatomische Röntgen-ähnliche Ansicht mit eigener visueller Sprache, die an die tatsächliche Verletzungssimulation gebunden ist; Häufigkeit konfigurierbar, standardmäßig selten. Der Entwickler: nicht nur bei richtig guten Kills, konfigurierbar, ‚man kann beides einstellen‘ | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P21; `23#141E` P8 | UI ANI TRF | Kriterien: `CQ-21` |
| COMBAT-DEC-FK-029 | **Combat Focus** ersetzt eine Zeitlupen-Fähigkeit (siehe [Modul 02](02-Kampfvision-und-Grundprinzipien.md)); Einzelheiten sind offen | 📋 BESCHLOSSEN / ❓ OFFEN | `COMBAT-DEC-GP-021`, `COMBAT-DEC-GP-022` | STE | `CQ-05` |
| COMBAT-DEC-FK-030 | **Schusswaffen gegen Monster:** normale Ballistik ist die Grundlage; danach gelten Resistenz und Schwäche je Kreatur; manche Wesen sind durch Schusswaffen nur zu vertreiben, ihre eigentliche Lösung ist ein Ritual, ein Objekt, Verbrennung oder Bannung | 🧭 RICHTUNG | `23#85A` P2; `23#86E` P2 (‚ja das ist Pflicht‘); `K-KO-002`, `K-KO-014` | KRE | [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) |
| COMBAT-DEC-FK-031 | **Ein Deckungssystem (Anlehnen, Lehnen um die Ecke, Blindfeuer, Deckungswechsel, Gegnerverhalten in Deckung) ist in keiner Quelle entschieden;** die Quellen nennen Deckung nur als Material (Durchschlag) und als Stichwort der Combat-Planung | ❓ OFFEN | `23#96A` (Aufgabenliste ‚Deckung‘, Assistent); `M-PD` (nur Materialdurchschlag) | BEW ANI KI | `CQ-17` |
| COMBAT-DEC-FK-032 | **Schießen aus und auf Fahrzeuge** (Reifen, Motor, Fahrer, Innenraum, Fahrzeugkampf) ist als Kampfmechanik nicht beschrieben; beschlossen ist nur die unterschiedliche ballistische Struktur von Fahrzeugen | ❓ OFFEN | `M-PD` ‚Ballistics‘ (Strukturen); Gameplay-Systeme §6 (Fahrzeugschaden ❓) | WAF BEW | `CQ-18` |
| COMBAT-DEC-FK-033 | **Rückstoß-, Streuungs- und Schützenfehlermodell** (Hüftfeuer gegenüber ADS, Schrotstreuung, Mündungsanstieg, Erholung) sind nicht beschlossen; die Quellen nennen nur die Komponenten (Rückstoßimpuls, Mündungsanstieg, Rotation, Erholung, Schützenkontrolle) als Entwurf | ❓ OFFEN | `23#115A` P5 (Entwurf); `M-OD` ‚Combat / injury / gore‘ | WAF STE | `CQ-19` |

---

## ST: Steuerung und Eingabekontext

Gruppe zu [Modul 06](06-Steuerung-und-Eingabekontext.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-ST-001 | **Vollständige Controller-Unterstützung ohne Zweitklassigkeit:** Menüs, Inventar, Archiv, Ermittlung, Karten, Dialoge, Fahrzeuge, Kampf, Oberfläche, Modbrowser, Zugänglichkeit, Tastensymbole, Neubelegung, Vibration und Haptik, sinnvolle Radialmenüs funktionieren ohne Maus | 📋 BESCHLOSSEN | `23#63A` (Abschnitt 10); `23#64E` P10 (‚ja genau‘); `M-PD` ‚Vehicles‘ (full controller support) | STE UI | Radialmenüs: Entwurf; Haptik: `CQ-23` |
| COMBAT-DEC-ST-002 | **Jede Belegung ist frei neu belegbar;** das Spiel ist ‚generell alles konfigurierbar‘ | 📋 BESCHLOSSEN | `23#64E` P8 (Fahrbelegung ‚alles konfigurierbar‘) und Nachsatz; `M-PD` ‚Vehicles‘ (fully remappable) | STE | Die Aussage ist für die Fahrbelegung explizit, für den Kampf aus dem Nachsatz abgeleitet |
| COMBAT-DEC-ST-003 | **Steuerung ist kontextabhängig und lernbar;** der Spieler lernt keine großen Kombotabellen. Position, Distanz, Griff, Richtung und Kontext bestimmen die sinnvollen Aktionen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#171E`; `COMBAT-DEC-NK-031` | STE | Spannung zu `35#798A`: `CK-03` |
| COMBAT-DEC-ST-004 | **Eine kontextabhängige Eingabe löst die richtige realistische Handlung aus** (zum Beispiel Nachladen und Störungsbeseitigung auf derselben Eingabe); keine Waffenstörungs-Kombinationen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#157E`; `23#159E` | STE WAF | Taste offen |
| COMBAT-DEC-ST-005 | **Langsames, leises Gehen entsteht kontinuierlich durch geringe Stickauslenkung,** nicht durch eine Pflicht-Schleichtaste | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#240E` A (‚ja indem man einfach den Stick weniger bewegt geht man langsamer‘) | STE KI | Tastatur-Entsprechung offen (`CQ-24`) |
| COMBAT-DEC-ST-006 | **Befehle und Handzeichen an Begleiter** (folgen, warten, dorthin, Deckung, beobachten, flankieren, Tür sichern, vor, zurück, leise) sind kontextabhängig; ohne Sichtkontakt dienen Stimme, Funk oder Telefon und können hörbar sein | 📋 BESCHLOSSEN | `M-PD` ‚Companion autonomy…‘; `23#254E` P17 | STE NPC | Eingabeform (Radialmenü, Taste, Stimme) offen |
| COMBAT-DEC-ST-007 | **Tarn-HUD und Erkennungsanzeigen sind optional;** Grounded schaltet sie standardmäßig ab | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#226E` P27 (‚A: ich würde es wieder einschaltbar machen, aber Grounded aus im Normalfall‘) | UI SCH | – |
| COMBAT-DEC-ST-008 | **Hilfen sind einzeln einstellbar und gehören zur Schwierigkeit ODER zur Zugänglichkeit;** Barrierefreiheit ist eine eigene Ebene (Aim-Hilfe und Eingabehilfen für Motorik sind nicht dasselbe wie eine Schwierigkeitsstufe) | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `COMBAT-DEC-GP-016` | SCH UI | Liste: `CQ-04` |
| COMBAT-DEC-ST-009 | **Springen liegt auf Kreuz (DualSense)** | 📋 BESCHLOSSEN | Proposal 0009 §3a (Entwickler, 22.10.2026) | BEW STE | Hat Vorrang vor jeder Entwurfsbelegung in diesem Modul |
| COMBAT-DEC-ST-010 | **Fahren belegt Kreuz und Quadrat anders als zu Fuß** (Kreuz Hochschalten, Quadrat Herunterschalten im Fahrzeug) | 📋 BESCHLOSSEN | Gameplay-Systeme §6.3 (Entwickler, 15.10.2026) | STE | Dass dieselben Tasten je Modus unterschiedlich wirken, ist nicht ausdrücklich entschieden (`CQ-25`) |
| COMBAT-DEC-ST-011 | **Quadrat: Kontextinteraktion außerhalb des Kampfes beziehungsweise normaler Angriff bei Gegnernähe** | 🔍 UNBELEGT | nur `AUF` §5 (Modul 06) | STE | `CU-01`, `CQ-01`, `CQ-25` |
| COMBAT-DEC-ST-012 | **Kreis: Ausweichen** | 🔍 UNBELEGT | nur `AUF` §5 (Modul 06) | STE | `CU-01`, `CQ-01`; Xbox-Fahrkonzept nutzte B (auf der DualSense Kreis) für die Handbremse (Gameplay-Systeme §6.3) |
| COMBAT-DEC-ST-013 | **Dreieck: möglicherweise schwerer Angriff;** ausdrücklich **nicht** als endgültig beschlossen zu behandeln | 🔍 UNBELEGT | `AUF` §5 (Modul 06): ‚Die Dreieck-Belegung ist nicht als endgültig beschlossen zu behandeln‘ | STE | `CU-01`, `CQ-01` |
| COMBAT-DEC-ST-014 | **Direct Control gegenüber Assisted Control im Kampf** ist offen | ❓ OFFEN | `35#791E` P1, P2, P4, P6; `35#792A` | STE | `CQ-02`; siehe `COMBAT-DEC-NK-040` |
| COMBAT-DEC-ST-015 | **Spannung: keine Tastentabelle (`23#171E`, `M-PD`) gegenüber Aktionsbibliothek mit Tastenkombination (`35#791E` P3, `35#798A`)** | ⚠️ KONFLIKT | `23#171E`; `M-PD` ‚Melee / grappling‘; `35#791E` P3; `35#798A` | STE | `CK-03` |
| COMBAT-DEC-ST-016 | **Haptik und adaptive Trigger im Kampf** (Rückstoß, Treffer, Nachladen, Verletzung) sind nicht entschieden; die Quellen führen nur Vibration und Haptik als Controller-Anforderung und beim Fahren Rückmeldungen über R2, L2 und Oberflächen | ❓ OFFEN | `23#63A` (Abschnitt 10); `23#515A` (Entwurf des Assistenten, Fahren) | STE AUD | `CQ-23` |
| COMBAT-DEC-ST-017 | **Belegung für Maus und Tastatur im Kampf** (Angriff, Block, Ausweichen, Zielen, Schießen, Greifen) ist nicht entschieden; belegt sind nur Leertaste, Shift, E | ❓ OFFEN | Proposal 0009 §3a; README §26.1 | STE | `CQ-24` |
| COMBAT-DEC-ST-018 | **Eingabeprioritäten bei gleichzeitigen Absichten** (Springen gegenüber Ausweichen gegenüber Interagieren, Angriff gegenüber Interaktion bei Gegnernähe) sind nicht entschieden | ❓ OFFEN | `AUF` §5; `AUF` §8 (‚Verhalten kontextabhängiger Tasten‘) | STE | `CQ-25` |
| COMBAT-DEC-ST-019 | **Eingabekontext-Modell:** Die Absichtsauflösung entscheidet nach Kontext (Fortbewegung, Fernkampfbereitschaft, Schlagabtausch, Griff/Boden, Fahrzeug, Menü, Dialog, Behandlung); jede Ablehnung nennt einen Grund | 💡 VORSCHLAG | Abschnitt 6 (Entwurf); `COMBAT-DEC-ZA-012` | STE | – |
| COMBAT-DEC-ST-020 | **Kampf in VR** ist nicht beschrieben; VR soll dieselbe Interaktions-, Physik- und Körperbasis nutzen und später kommen | ❓ OFFEN | `35#795E`, `35#796A` (für Interaktionen; Kampf nicht erwähnt) | STE | `CQ-26` |

---

## DF: Ausweichen, Blocken, Parieren, Konter

Gruppe zu [Modul 07](07-Ausweichen-Blocken-Parieren-und-Konter.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-DF-001 | **Blocken ist physisch,** kein universeller Schadensminderungszustand | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#162A` P6; `23#163E` P6 (‚passt gut‘) | TRF STE | Zeitfenster: `CQ-27` |
| COMBAT-DEC-DF-002 | **Wirksamkeit hängt vom Angreifer ab:** ein menschlicher Faustschlag kann geblockt werden, ein Messer vielleicht abgeleitet oder pariert, aber gefährlich; die übernatürliche Kraft einer riesigen Kreatur wird nicht durch beidhändiges Blocken aufgehalten | 📋 BESCHLOSSEN | `23#162A` P6; `23#163E` P6 | KRE TRF | Grundlage der Angriffseigenschaften (DF-003) |
| COMBAT-DEC-DF-003 | **Jeder Angriff hat Eigenschaften** (blockierbar, parierbar, Ausweichen empfohlen, nicht blockierbar), die **nicht über bunte Videospielindikatoren** angezeigt werden; der Spieler lernt sie aus Animation, Monsterwissen und Erfahrung | 📋 BESCHLOSSEN | `23#162A` P6; `23#163E` P6 | KRE UI ANI | Hinweise je Schwierigkeit: `CQ-30` |
| COMBAT-DEC-DF-004 | **Parieren gilt nur, wenn es physikalisch plausibel ist** (menschlicher Messerangriff mit richtigem Timing ja; ein Monster mit sehr großer Masse nicht mit einem kleinen Messer) | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#162A` P7; `23#163E` P7 (‚genau‘) | TRF ANI | – |
| COMBAT-DEC-DF-005 | **Eine gelungene Parade kann den Gegner kurz öffnen, einen Griff lösen, Distanz schaffen oder einen Gegenangriff ermöglichen;** kein ‚Perfect Parry‘, das den Gegner mehrere Sekunden einfriert | 📋 BESCHLOSSEN | `23#162A` P7; `23#163E` P7 | STE KI | Dauer der Öffnung: Konfiguration, keine Quelle |
| COMBAT-DEC-DF-006 | **Ausweichen besteht aus körperlich plausiblen Bewegungen** (Schritt zurück, Seitschritt, Ducken, Körper wegdrehen, kurzer Sprung zur Seite, in extremer Lage zu Boden werfen); eine Rolle existiert nur, wo sie physisch sinnvoll ist, und ist nicht die primäre Verteidigung | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (‚Avoid Soulslike roll spam‘); `23#162A` P8; `23#163E` P8 (‚passt‘) | BEW ANI | Katalog und Kosten: `CQ-28` |
| COMBAT-DEC-DF-007 | **Ausweichen liegt laut Auftrag auf Kreis (DualSense)** | 🔍 UNBELEGT | `AUF` §5 (Modul 06); `COMBAT-DEC-ST-012` | STE | `CQ-01` |
| COMBAT-DEC-DF-008 | **Konter nach Block oder Ausweichen** sind Teil des Schlagabtauschs; eine Kombination, die gekontert wird, zerfällt | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (counters and transitions); `23#168A` (‚Konter nach Block/Dodge‘); `COMBAT-DEC-NK-004` | STE ANI | Zeitfenster: `CQ-27` |
| COMBAT-DEC-DF-009 | **Finten existieren** (Angriff starten und abbrechen, um eine Reaktion zu erzwingen) | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (‚feints‘); `23#168A` (Beispiel Feint-Cross) | STE KI | – |
| COMBAT-DEC-DF-010 | **Verletzungen schwächen die Verteidigung:** verletzter Arm schlechteres Blocken, Beinverletzung schlechtere Ausweichbewegungen, Blutverlust schwächere Reaktion | 📋 BESCHLOSSEN | `23#162A` P15; `23#163E` P15; `COMBAT-DEC-NK-026` | MED | – |
| COMBAT-DEC-DF-011 | **Schwere Monster zwingen zum Ausweichen statt zum Tanken** (zerstören Deckung, lassen sich kaum aufhalten) | 📋 BESCHLOSSEN | `23#85A` P13 (Beispiel); `23#86E` P13 (‚ja genau, das ist wichtig‘) | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md) |
| COMBAT-DEC-DF-012 | **Gegner verteidigen sich ebenfalls** (blocken, weichen aus, kontern), sonst haben Block, Konter und zerfallende Kombinationen keine Wirkung | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-NK-004`, `COMBAT-DEC-NK-044` (Gegner nutzen Öffnungen) | KI | Schwierigkeitsabhängige Qualität: Modul 18 |
| COMBAT-DEC-DF-013 | **Block-Aufprall auf den Arm (Armverletzung durch harten Treffer auf die Deckung) ist als Mechanik denkbar** | 💡 VORSCHLAG | `M-P08` (Themeninventar ‚block-impact-and-arm-injury-integration‘); keine Entwicklerquelle | MED TRF | Prüfen in `CQ-27` |
| COMBAT-DEC-DF-014 | **Abwehr von Projektilen geschieht über Deckung, Distanz und Sichtbruch;** ob ein Ausweichen gegen Schüsse oder Monsterfernangriffe (Wurf, Speien, Entladung) existiert, ist nicht entschieden | ❓ OFFEN | `COMBAT-DEC-FK-016`; `23#127A` (‚Verstecken ist nicht automatisch dasselbe wie geschützt sein‘) | TRF KRE | `CQ-29` |
| COMBAT-DEC-DF-015 | **Zeitfenster, Voraussetzungen, Ausdauerkosten und Erholung von Block, Parade, Ausweichen und Konter sind nicht festgelegt** | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘ (Tiefe von Nahkampf); Entscheidungsstatus §5.6 | STE AU | `CQ-27`, `CQ-28` |

---

## BT: Kampfbewegung und Traversal

Gruppe zu [Modul 08](08-Kampfbewegung-und-Traversal-Integration.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-BT-001 | **Springen wird aktiv durch die Spielerin oder den Spieler ausgelöst** und ist nie die automatische Reaktion auf ein Hindernis | 📋 BESCHLOSSEN | Proposal 0009 §2 (1) (Entwickler, 22.10.2026) | BEW STE | Das gilt auch im Kampf: kein automatisches Überspringen bei Verfolgung |
| COMBAT-DEC-BT-002 | **Die Sprungstärke bleibt realistisch menschlich** und wird nicht erhöht, um Hindernisse zu überwinden | 📋 BESCHLOSSEN | Proposal 0009 §2 (2) | BEW | Zahl offen (`CQ-31`) |
| COMBAT-DEC-BT-003 | **Für niedrige Hindernisse soll der Charakter später eine zur Höhe passende Bewegung ausführen** (Aufstieg, Hürde, Mantle) statt eines unnötig hohen Sprungs | 📋 BESCHLOSSEN | Proposal 0009 §2 (3) | BEW ANI | Umsetzung erst im Traversal-Meilenstein |
| COMBAT-DEC-BT-004 | **Das System kommt in einem späteren Traversal-Meilenstein;** M1 bekommt kein Vault- oder Mantle-System | 📋 BESCHLOSSEN | Proposal 0009 §2 (4) | BEW | Kampf darf nicht darauf warten (Abschnitt 4) |
| COMBAT-DEC-BT-005 | **Animation und tatsächliche Bewegung müssen übereinstimmen** | 📋 BESCHLOSSEN | Proposal 0009 §3a (Entwickler, 22.10.2026) | BEW ANI | Gilt für Kampfaktionen mit Bewegung (Ausweichen, Sprung zur Seite) |
| COMBAT-DEC-BT-006 | **Verletzungen und Erschöpfung dürfen Traversal später beeinflussen** | 📋 BESCHLOSSEN | Proposal 0009 §3a | MED AU BEW | Mechanik offen |
| COMBAT-DEC-BT-007 | **Eine 50-cm-Mauer darf keinen übertriebenen Sprung auslösen** | 📋 BESCHLOSSEN | Proposal 0009 §3a | BEW | Prüfbeispiel (Modul 30) |
| COMBAT-DEC-BT-008 | **Springen liegt auf Kreuz (DualSense)** | 📋 BESCHLOSSEN | Proposal 0009 §3a | STE | `COMBAT-DEC-ST-009` |
| COMBAT-DEC-BT-009 | **Springen und Wasser:** normaler Sprung, Kopfsprung, weiter Sprung, kontrollierter Sprung mit begrenzter Körperkorrektur; keine magische Luftsteuerung; Höhe, Technik und Wasser beeinflussen das Verletzungsrisiko | 📋 BESCHLOSSEN | Gameplay-Systeme §2 (‚Springen und Wasser‘) | BEW MED | Wasserkampf: `CQ-32` |
| COMBAT-DEC-BT-010 | **Stürze, Explosionen, Fahrzeugunfälle und Trümmer sind Traumaarten** des gemeinsamen Verletzungssystems | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (Traumaarten) | MED TRF | [Modul 12](12-Treffermodell-und-Koerperphysik.md), [13](13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| COMBAT-DEC-BT-011 | **Verletzte Gliedmaßen wirken sich auf Hinken, Sprinten, Waffeneinsatz, Nachladen, Gleichgewicht, Atmung und Ausdauer aus** | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ | MED BEW AU | – |
| COMBAT-DEC-BT-012 | **Sprinten verbessert die Ausdauer durch Benutzung;** Kondition und Belastung wirken auf das Zielen und die Handlungsfähigkeit | 🧭 RICHTUNG | `23#116E` P16 (‚sprintet man mehr hat man mehr Ausdauer‘); `23#116E` P7; `M-PD` ‚Progression — OPEN…‘ | AU | `COMBAT-DEC-GP-024`, `CQ-42` |
| COMBAT-DEC-BT-013 | **Fußarbeit, Distanzmanagement und Ausweichbewegungen sind Bewegung im Kampf** und folgen denselben Verletzungs- und Ausdauerregeln wie die Fortbewegung | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-NK-005`, `COMBAT-DEC-DF-006`, `COMBAT-DEC-BT-011` | BEW | – |
| COMBAT-DEC-BT-014 | **Third Person ist der wahrscheinliche Standard;** vollständige First-Person-Unterstützung ist ernsthaft geplant, besonders für Horror und Ermittlung | 📋 BESCHLOSSEN | Gameplay-Systeme §2 (‚Kamera‘) | ANI UI | Kampfkamera in First Person: nicht beschrieben (`CQ-26` für VR; allgemein offen) |
| COMBAT-DEC-BT-015 | **Eine Hinderniserkennung (Höhe und Tiefe per Traces) entscheidet später zwischen Gehen, Springen, Hürde und Mantle;** Schwellen und Sprungstärke sind Konfiguration, die mit der Projektleitung festzulegen ist | 💡 VORSCHLAG | Proposal 0009 §4 (Proposal, nichts entschieden) | BEW TST | Offene Fragen aus Proposal 0009 §5: `CQ-31` |
| COMBAT-DEC-BT-016 | **Bewegung bleibt lokal und autoritativ je Spieler und hat keinen Weltzustands-Einfluss** | 💡 VORSCHLAG | Proposal 0009 §4 | KOO | [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md) |
| COMBAT-DEC-BT-017 | **Kampfaktionen in der Luft und beim Klettern, Hürden und Aufstiege als Fluchtmittel im Kampf, Kampf im und unter Wasser** sind in keiner Quelle beschrieben | ❓ OFFEN | Gameplay-Systeme §2 (Klettern und Schwimmen sind im Prototyp nicht vorhanden); `M-PD` (Sprung nur als Verletzungsrisiko) | BEW ANI | `CQ-31`, `CQ-32` |

---

## WA: Waffenklassen und Ausrüstung

Gruppe zu [Modul 09](09-Waffenklassen-und-Ausruestung.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-WA-001 | **Es gibt viel Waffenauswahl, ‚auf jeden Fall mehr als nur die Standardwaffen‘;** jede Waffe wird in den Assets realistisch gebaut | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P8, P9 | WAF | Liste: `CQ-33` |
| COMBAT-DEC-WA-002 | **Nicht jede Waffe ist ein höherer Schadenswert.** Eine alte Pumpgun kann gegen das richtige Monster wertvoller sein als ein taktisches Gewehr | 📋 BESCHLOSSEN | `23#85A` P8; `23#86E` P8; `COMBAT-DEC-GP-001` | WAF KRE | – |
| COMBAT-DEC-WA-003 | **Waffenklassen (Entwurf des Assistenten):** Handfeuerwaffen (Pistolen, Revolver), Langwaffen (Schrotflinten, Jagdgewehre, Repetierer, halbautomatische Gewehre), Nahkampf (Messer, Machete, Axt, improvisiert, spezielle Hunter-Waffen), Spezialausrüstung (Salz-, Silber- und Eisenmunition, Brandmittel, besondere Projektile, Fallen, Bann- und Ritualgegenstände) | 🧭 RICHTUNG | `23#85A` P8 (Liste); `23#86E` P8 (‚viel Auswahl‘) | WAF | Abschnitt 3; Liste ist Inventar |
| COMBAT-DEC-WA-004 | **Der Spieler trägt mehrere Waffen, aber nicht alles;** die Vorbereitung vor der Jagd zählt; Kofferraum und Unterschlupf sind größere Lager | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P10 (‚ein paar Waffen kann man tragen, deswegen muss man vor dem Kampf gut mitdenken‘) | WAF | – |
| COMBAT-DEC-WA-005 | **Der Zugriff hängt vom Trageort ab:** Holster oder Gürtel schnell, Rucksack langsamer, Kofferraum nicht als unmittelbarer Kampfzugriff | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; Gameplay-Systeme §5.1 | WAF STE | Zeiten: `CQ-36` |
| COMBAT-DEC-WA-006 | **Der Körper hat begrenzten Stauraum;** Fahrzeuge bieten Lager; das Hauptquartier kann sehr große, nachgefüllte Vorräte halten | 📋 BESCHLOSSEN | Gameplay-Systeme §9.3; Proposal 0007 §2 (I1: akzeptierte Richtung, keine Zahlen) | WAF | Kein Kapazitätsmodell (`CQ-36`) |
| COMBAT-DEC-WA-007 | **Waffen und andere Gegenstände sind lose physische Objekte;** sie können physisch verloren und aufgehoben werden, mit Bereinigung und Persistenz | 📋 BESCHLOSSEN | Gameplay-Systeme §2; `M-PD` ‚Cash / law / evidence‘ | WAF PER | – |
| COMBAT-DEC-WA-008 | **Wichtige abgelegte Waffen bleiben physisch und können Beweise werden;** eine intelligente Bereinigung verhindert unbegrenzten Müll | 📋 BESCHLOSSEN | `M-PD` ‚Cash / law / evidence‘; `E-QA-023`; Proposal 0007 §2 (I2) | WAF POL | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-WA-009 | **Seltene, legendäre Jägerwaffen existieren** (Beispiele des Entwicklers: ein berühmter Revolver und ein Dämonenmesser der Serie), sind ‚richtig schwer‘ zu bekommen und werden als **eigene** Waffen mit eigener Geschichte, eigenem Design und eigenen Regeln umgesetzt; ‚etwas Ähnlichkeit‘ bleibt gewollt | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P8; `23#88E`; `23#91E`; `K-JA-010` | WAF KRE | Zahl und Beschaffung: `CQ-35` |
| COMBAT-DEC-WA-010 | **Rechtliche Absicherung der Waffen- und Requisitengestaltung ist Pflicht;** eine IP-Clearance vor der Veröffentlichung wird eingeplant | 📋 BESCHLOSSEN | `23#88E` (‚ich will auf jeden Fall abgesichert sein‘); `23#90A`; `K-IP-003` (Eigene Namen und Designs statt serienspezifischer Ausgestaltung) | WAF | Clearance ist Prozess, nicht Spielregel |
| COMBAT-DEC-WA-011 | **Waffen sind mechanisch korrekt gebaut** (Magazine, Patronen, Verschluss, Ladeanimation, Auswurf, Abzug, Sicherung, Rückstoß, Länge, Gewicht, Sound) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P8; Entwurfsliste `23#87A` | WAF ANI AUD | Assetpflichten: [Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md) |
| COMBAT-DEC-WA-012 | **Begleiter besitzen echte Waffen, Kleidung und Inventar** | 📋 BESCHLOSSEN | `N-CO-005` | NPC WAF | – |
| COMBAT-DEC-WA-013 | **Keine farbcodierte Seltenheit** und keine Schadenszahlen als Fortschrittsmotor; Seltenheit entsteht aus Geschichte, Beschaffung und Regeln | 📋 BESCHLOSSEN | `M-PD` ‚Genre and progression identity‘; `COMBAT-DEC-GP-003` | WAF UI | – |
| COMBAT-DEC-WA-014 | **Lichtquellen (Taschenlampe, Waffenlicht, Fahrzeuglicht, Lampen, Generator) sind taktische Werkzeuge und Risiken;** manche Kreaturen können Licht und Strom manipulieren | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#117A` (Beispiele) | WAF KI | Waffenlicht als Anbauteil: Entwurf |
| COMBAT-DEC-WA-015 | **Bei Niederlage kann Ausrüstung teilweise verloren gehen oder am Einsatzort zurückbleiben** (Fahrzeug, Waffen, Rucksack) | 🧭 RICHTUNG | Spielvision §14.2; `23#61A` (Folgenliste, Assistent); `M-PD` ‚Death / failure — OPEN‘ | WAF PER | [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| COMBAT-DEC-WA-016 | **Kleidung liegt im Projektilweg und hat eine Schutzfunktion** (Kleidungsschaden); Ausmaß der Schutzwirkung von Kleidung und besonderem Körperschutz (Westen, Helme) und die Rüstungsparameter sind nicht entschieden | ❓ OFFEN | `M-PD` ‚Ballistics‘ (Projektilpfad durch Kleidung); Gameplay-Systeme §9.3 (Kleidung hat Wetter-, Schutz-, Stauraum- und soziale Funktion, kann nass, schmutzig, blutig oder beschädigt werden); `M-OD` ‚Combat / injury / gore‘ (Rüstungsparameter) | TRF WAF | `CQ-34` |
| COMBAT-DEC-WA-017 | **Konkrete Waffenliste, Kaliber, Modelle und Herstellernamen (real oder fiktiv) sind nicht festgelegt** | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle); `M-OD` | WAF | `CQ-33` |
| COMBAT-DEC-WA-018 | **Inventarmodell für Waffen** (Slots, Gewicht, Stauraumgrenzen, Zugriffszeiten, Schnellwechsel) ist nicht entschieden | ❓ OFFEN | Proposal 0007 §2 (‚Searched, not found‘); `23#96A` (Aufgabe ‚Inventar‘) | WAF UI | `CQ-36` |
| COMBAT-DEC-WA-019 | **Weitere Waffenarten (Bogen, Armbrust, Wurfwaffen, Brandmittel, Sprengstoff)** sind nicht beschrieben; beschlossen sind Brand und Explosion als Traumaarten, nicht ihre Waffen | ❓ OFFEN | `M-PD` ‚Human anatomy…‘ (Traumaarten); `23#85A` P8 (‚Brandmittel‘, ‚besondere Projektile‘, Entwurf); `23#103A` P13 (‚Arrow/Projectile‘, Entwurf) | WAF TRF | `CQ-39` |
| COMBAT-DEC-WA-020 | **Waffenbesitz und offenes Tragen im Recht** (Tragen, Genehmigungen, illegale Waffen) sind nicht entschieden | ❓ OFFEN | `23#96A` (‚Waffen offen tragen‘, Assistent); `M-OD` ‚Crime / law / evidence‘ | POL | `CQ-81` |
| COMBAT-DEC-WA-021 | **Kauf, Wartung und Reparatur von Waffen sind Teil der Wirtschaft** (Ausgaben für Waffen, Munition, Jäger-Material) | 📋 BESCHLOSSEN | Gameplay-Systeme §13 (Single-Player-Wirtschaft) | WAF | Preise offen |

---

## MU: Munition, Nachladen, Waffenzustand

Gruppe zu [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-MU-001 | **Munitionsarten unterscheiden sich physikalisch,** nicht nur durch Schaden: Material, Aufbau, Geschwindigkeit, Wirkung im Gewebe und im Material | 📋 BESCHLOSSEN | `23#154A` P1; `23#155E` P1 (‚genau, das wurde schon öfters besprochen‘); `M-PD` ‚Ballistics‘ | MUN TRF | Werte offen |
| COMBAT-DEC-MU-002 | **Vollmantel- und Hohlspitzmunition** verhalten sich unterschiedlich; expandierende Munition kann eine schwere Wunde reißen (‚das ist dann böse, wenn der Gegner eine riesige Wunde hat‘) | 📋 BESCHLOSSEN | `23#154A` P2–P3; `23#155E` P2–P3 | MUN TRF | – |
| COMBAT-DEC-MU-003 | **Schrotmunition ist komplexer:** Buckshot, Birdshot und Slug verhalten sich klar unterschiedlich; Treffer aus der Nähe können schwerste Verletzungen verursachen (Schrotflinte aus der Nähe ≠ Pistole) | 📋 BESCHLOSSEN | `23#154A` P4; `23#155E` P3–P4; `M-PD` ‚Human anatomy…‘ (Gore-Aussage) | MUN TRF | Streuung: `CQ-19` |
| COMBAT-DEC-MU-004 | **Hunter-Munition baut auf echter Ballistik auf und fügt eine übernatürliche Wirkung hinzu;** die Arten richten nur bei den bestimmten Gegnern mehr Schaden an | 📋 BESCHLOSSEN | `23#154A` P5; `23#155E` P5; `M-PD` ‚Gunplay / weapons‘ | MUN KRE | Wirkungsarten: [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) |
| COMBAT-DEC-MU-005 | **Munition muss zur konkreten Waffe passen,** aber ‚nicht zu komplex‘ | 🧭 RICHTUNG | `23#154A` P6; `23#155E` P6 (‚ok ja aber nicht zu komplex‘) | MUN WAF | Kompatibilitätsregeln offen |
| COMBAT-DEC-MU-006 | **Magazine behalten ihre tatsächliche Schusszahl;** es gibt keinen globalen Munitionspool. Ein vorzeitiges Nachladen lässt das angebrochene Magazin bestehen (nicht ‚im letzten gespawnt‘) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P12; `23#155E` P11 | WAF | – |
| COMBAT-DEC-MU-007 | **Magazine sind physische Objekte** (können liegen, aufgehoben, verloren, abgeworfen werden) | 📋 BESCHLOSSEN | `23#154A` P11, P13; `23#155E` P11 (‚passt gut‘), P13 | WAF PER | Weltobjekt-Regeln: Modul 24 |
| COMBAT-DEC-MU-008 | **Patronenlager und Magazin sind getrennte Zustände** (eine Patrone im Lauf bei leerem Magazin ist möglich) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P12; `23#155E` P12 (‚ja das ist smart‘) | WAF | – |
| COMBAT-DEC-MU-009 | **Taktisches Nachladen behält ein angebrochenes Magazin** (wird verstaut); **Notnachladen** gilt vor allem bei leerem Magazin und wirft das leere Magazin zugunsten der Geschwindigkeit ab. Der Entwickler: ‚emergency reload sollte man nur machen, wenn das Magazin leer ist‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P13; `23#155E` P13 | WAF STE | – |
| COMBAT-DEC-MU-010 | **Ein getrennter ‚Drop Reload‘ zum bewussten Wegwerfen eines halbvollen Magazins** wäre möglich | 💡 VORSCHLAG | `23#156A` (Assistent, ‚eine Ausnahme könnten wir später noch überlegen‘); keine Entwicklerantwort | WAF STE | – |
| COMBAT-DEC-MU-011 | **Röhrenmagazin-Schrotflinten werden Patrone für Patrone geladen,** und das Nachladen ist nach jeder Patrone unterbrechbar | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P14; `23#155E` P14 | WAF | – |
| COMBAT-DEC-MU-012 | **Der Nachladezustand ist persistent und unterbrechbar** | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ | PER WAF | Entwurf: Abschnitt 5 |
| COMBAT-DEC-MU-013 | **Nachladen ist eine echte Aktion mit Geräusch** (Magazin fällt, Waffe nicht feuerbereit); die KI hört es abhängig von Entfernung, Raum, Wänden, Hintergrund, Monstertyp und Schwierigkeit; auf höheren Stufen sind Gegner gnadenlos und hören es ‚auf jeden Fall‘ | 📋 BESCHLOSSEN | `23#86E` P1 (‚auf jeden Fall hören sie es, auf leichteren vielleicht nicht‘); `M-PD` ‚Stealth, perception and horror AI‘; `23#115A` P10 (Entwurf) | AUD KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-MU-014 | **Revolver mit Trommel und Kammern** sind Teil der Waffenbedienung (Auswurf, einzelne Patronen oder Speedloader je Modell) | 🧭 RICHTUNG | `23#115A` P9 (Entwurf); `M-PD` (‚mechanically correct manipulation‘) | WAF | Detail ist Asset-Aufgabe |
| COMBAT-DEC-MU-015 | **Störungen sind selten,** aber wahrscheinlicher bei schlechter Wartung, Verschmutzung, Schaden oder problematischer Munition. Der Entwickler wählt Variante B: ‚selten, aber wenn die Waffe nicht gut gereinigt ist, kann es leichter passieren‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P16; `23#155E` P16 | WAF | – |
| COMBAT-DEC-MU-016 | **Robuste Konstruktionen vertragen Vernachlässigung und Umwelt besser,** sind aber nicht unverwundbar. ‚Manche Waffen funktionieren ohne Probleme, wie die AK47 in echt auch‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (robust designs…); `23#155E` P19 | WAF | – |
| COMBAT-DEC-MU-017 | **Die Störungsbeseitigung bleibt bedienungsfreundlich:** eine kontextabhängige Eingabe löst die passende realistische Handlung aus; keine Waffenstörungs-Kombinationen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#157E`; `23#158A`; `23#159E` | STE WAF | Taste offen |
| COMBAT-DEC-MU-018 | **Grad der Störungsbeseitigung** (vereinfachte Automatik gegenüber echter Fehlerbehebung je Störungsart, auf Grounded als Option) | ❓ OFFEN | `23#156A` (Optionen A und B); `23#157E` (‚das weiß ich noch nicht‘) | STE SCH | `CQ-40` |
| COMBAT-DEC-MU-019 | **Der Waffenzustand wird intern detailliert simuliert** (Sauberkeit, Verschleiß, Beschädigung, Schmierung, Feuchtigkeit), aber nicht als unnötiges Mikromanagement | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (maintenance); `23#154A` P17; `23#155E` P17; `23#156A` | WAF | Größen: `CQ-41` |
| COMBAT-DEC-MU-020 | **Die Wartung ist moderat und realistisch, nicht ständig** (Variante A, ‚realistisch, nicht übertrieben‘) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P21; `23#155E` P21 | WAF | – |
| COMBAT-DEC-MU-021 | **Nasse und verschmutzte Waffen sind ein Faktor;** Waffen unterscheiden sich in ihrer Toleranz | 📋 BESCHLOSSEN | `23#154A` P19; `23#155E` P19 | WAF | – |
| COMBAT-DEC-MU-022 | **Waffeninspektion:** Waffe und Magazin werden körperlich geprüft; normale Modi dürfen zusätzlich eine Anzeige nutzen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (‚physical weapon and magazine inspection‘); `23#154A` P20; `23#155E` P20 | WAF UI | Anzeige binär: `COMBAT-DEC-FK-012` |
| COMBAT-DEC-MU-023 | **Auf Grounded gibt es weniger Munition und weniger medizinische Ressourcen** | 📋 BESCHLOSSEN | `23#85A` P7; `23#86E` P7 (‚ja das meinte ich auch mit grounded‘); `M-PD` ‚Difficulty / configuration‘ (scarce resources) | SCH | Mengen offen (`CQ-38`) |
| COMBAT-DEC-MU-024 | **Munition und Hülsen sind Spuren und Beweise** (Ballistik, abgelegte Waffen) | 📋 BESCHLOSSEN | `M-PD` ‚Cash / law / evidence‘; `E-RE-002`; `E-SP-007` | ERM POL | Modul 20 |
| COMBAT-DEC-MU-025 | **Auf Grounded kann die Störungsbeseitigung länger dauern, nicht automatisch gelingen und von Verletzung und Waffenkenntnis abhängen** | 💡 VORSCHLAG | `23#158A` (Assistent) | SCH WAF | `CQ-40` |
| COMBAT-DEC-MU-026 | **Komfortvariante der Munitionsverwaltung** (Gesamtzahl der Patronen, das Spiel verwaltet Magazine) als einstellbare Option | ❓ OFFEN | `23#85A` P12 (Option Komfortabel, Assistent); `23#86E` P12 (‚nicht so wie in cod…‘, realistische Verwaltung gewünscht) | SCH UI | `CQ-37` |
| COMBAT-DEC-MU-027 | **Munitionswirtschaft** (Preise, Gewicht, Herstellung, Wiederladen, Bestandsgrenzen, Knappheit) ist nicht entschieden | ❓ OFFEN | Gameplay-Systeme §13 (nennt Ausgaben, keine Preise); Entscheidungsstatus §5 | WAF | `CQ-38` |
| COMBAT-DEC-MU-028 | **Magazingrößen, Patronenzahlen, Kaliberliste und Verträglichkeit** sind nicht beschlossen | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | WAF | `CQ-33` |

---

## AU: Ausdauer, Erschöpfung, Belastung

Gruppe zu [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-AU-001 | **Ausdauer ist eine interne Körpergröße** neben Fitness, Kraft, Beweglichkeit, Ermüdung und Erholung (Anpassungsmodell mit regionalen Muskelgruppen); Kraft ist nicht identisch mit sichtbarer Muskelgröße | 📋 BESCHLOSSEN | Gameplay-Systeme §9.2; `M-PD` ‚Body / physical response / recoil‘ | MED | Formeln offen (AU-012) |
| COMBAT-DEC-AU-002 | **Training, Ernährung, Schlaf, Stress, Alkohol, Rauchen, Müdigkeit und Verletzung beeinflussen den Körper;** Training, Klettern und Alltag wirken passiv | 📋 BESCHLOSSEN | Gameplay-Systeme §9.2 | MED | – |
| COMBAT-DEC-AU-003 | **Langes Halten einer Langwaffe ermüdet moderat,** nicht nach wenigen Sekunden | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P21 | WAF | `COMBAT-DEC-FK-009` |
| COMBAT-DEC-AU-004 | **Nahkampf kostet Ausdauer;** schwere Waffen und wiederholte Schläge belasten mehr, die Wirkung (Geschwindigkeit, Präzision, Kraft) sinkt; ein trainierter Hunter kämpft trotzdem länger | 📋 BESCHLOSSEN | `23#162A` P5; `23#163E` P5; `COMBAT-DEC-NK-029` | AU STE | – |
| COMBAT-DEC-AU-005 | **Ausdauer entsteht durch Benutzung** (Sprinten); **abnehmende Erträge** verhindern Ausnutzen; zu häufiges Überlasten baut sie langsamer auf | 🧭 RICHTUNG | `23#116E` P16; `23#118E` P21; `M-PD` ‚Progression — OPEN…‘ | AU | `COMBAT-DEC-GP-024`, `-025`, `CQ-06` |
| COMBAT-DEC-AU-006 | **Atmung, Ermüdung und Anstrengung beeinflussen das Zielen** (Visierung unruhiger nach einem Sprint, ruhiger nach kurzer Pause); abschaltbar, auf Grounded uneindeutig | 🧭 RICHTUNG | `23#115A` P7; `23#116E` P7; `COMBAT-DEC-FK-006` | WAF | `CU-02` |
| COMBAT-DEC-AU-007 | **Verletzte Gliedmaßen wirken sich auf Hinken, Sprinten, Waffeneinsatz, Nachladen, Gleichgewicht, Atmung und Ausdauer aus;** Blutverlust senkt Kraft und beschleunigt die Erschöpfung | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#162A` P15; `COMBAT-DEC-NK-026` | MED | – |
| COMBAT-DEC-AU-008 | **Adrenalin unterdrückt wahrgenommenen Schmerz zeitweise;** nach dem Ende der Gefahr verschlechtern sich Schmerz, Zittern, Beweglichkeit und Präzision | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#101E` P7 (‚ja das ist auch gut, aber schwer umzusetzen wahrscheinlich‘) | MED | Zeitverläufe: Modul 13 |
| COMBAT-DEC-AU-009 | **Erschöpfung darf Traversal später beeinflussen** (Springen, Aufstieg) | 📋 BESCHLOSSEN | Proposal 0009 §3a | BEW | `COMBAT-DEC-BT-006` |
| COMBAT-DEC-AU-010 | **Alkohol beeinflusst Reaktion, Gleichgewicht, Wahrnehmung, Dialog, Fahrfähigkeit, Schlaf und Erholung;** Drogen haben eigene Wirkungen und Risiken | 📋 BESCHLOSSEN | Gameplay-Systeme §9.3 | MED | Wirkung im Kampf: aus Reaktion und Gleichgewicht abgeleitet |
| COMBAT-DEC-AU-011 | **Schlaf hat Folgen,** ohne ein ständiges Meter-Management zu werden; Nahrung ist optionaler Lebensstil, kein Pflicht-Hungerkreis | 📋 BESCHLOSSEN | `M-CA0905` (Spielergesundheit, Schlaf); Gameplay-Systeme §9.3 | MED | – |
| COMBAT-DEC-AU-012 | **Formeln und Grenzen für Körperzusammensetzung, Muskelzuwachs und -verlust, Kraft, Kardio, Müdigkeit, Erholung, Flüssigkeit, Schlaf, Stress und Ernährung sind nicht festgelegt** | ❓ OFFEN | Gameplay-Systeme §9.2 (letzte Zeile); Entscheidungsstatus §5.3; `M-OD` ‚Progression / body / survival‘ | MED AU | `CQ-43` |
| COMBAT-DEC-AU-013 | **Die Oberfläche kommuniziert den Körperzustand,** ohne das Spiel zur Tabellenkalkulation zu machen; ob es eine Ausdaueranzeige gibt, ist nicht entschieden | ❓ OFFEN | Gameplay-Systeme §9.2; `M-PD` ‚Human anatomy…‘ (Spieler muss Zustand spüren); `23#118E` P22 (nur Munitions-HUD) | UI | `CQ-42` |
| COMBAT-DEC-AU-014 | **Traglast** (Gewicht und Menge der getragenen Ausrüstung) und ihr Einfluss auf Bewegung, Geräusch und Ausdauer sind nicht entschieden; die Quellen nennen nur ‚begrenzten Stauraum‘ | ❓ OFFEN | Gameplay-Systeme §9.3 (Inventar); Proposal 0007 §2 (I1); `23#225A` (Schrittgeräusch abhängig von ‚Körpergewicht/Ausrüstung‘, Entwurf) | AU WAF | `CQ-44` |
| COMBAT-DEC-AU-015 | **Kälte, Hitze und Dehydrierung** sind Teil der Spielergesundheit; ihre Wirkung im Kampf folgt aus Beweglichkeit und Konzentration | 📋 BESCHLOSSEN | `M-CA0905` (Spielergesundheit); Gameplay-Systeme §4 (Kälte, Elektrizität) | MED | Werte offen |
| COMBAT-DEC-AU-016 | **Körperliche Unterschiede (zum Beispiel Geschlecht) erzeugen keinen pauschalen Nachteil;** Leistung folgt dem tatsächlichen Körper-, Trainings- und Verletzungszustand | 📋 BESCHLOSSEN | Spielvision §6.1; `COMBAT-DEC-NK-034` | MED | – |

---

## TK: Treffermodell und Körperphysik

Gruppe zu [Modul 12](12-Treffermodell-und-Koerperphysik.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-TK-001 | **Anatomie mit etwa 25+ Trefferregionen,** Weichgewebe, relevanten Knochen, Gelenken, kritischen Organen, großen Arterien und kritischen Gefäßzonen sowie vereinfachten Nerven und Sehnen | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy, injury and medicine‘; `23#101E` P3; `23#104E` A | TRF MED | Liste der Regionen als Inventar: Abschnitt 3 |
| COMBAT-DEC-TK-002 | **Projektile folgen einem Weg** durch Kleidung, Materialien, Gewebe, Knochen, Organe und Gefäße; sie können austreten oder stecken bleiben; ein steckengebliebenes Projektil muss nicht zwingend entfernt werden (Behandlung hängt von Lage und Zustand ab) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#102A` (medizinische Korrektur); `23#103A` P2–P3 (Entwurf); `23#104E` | TRF MED | Wundklassen: Abschnitt 5 |
| COMBAT-DEC-TK-003 | **Traumaarten:** Schuss/Schrot/Splitter, Schnitt, Stich, stumpf/Quetschung, Bruch und offener Bruch, innere Verletzung, Verbrennung/Kälte/Elektrizität, Bisse/Krallen/Reißen, Explosion/Sturz/Fahrzeug/Trümmer, übernatürliche Modifikatoren | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; Gameplay-Systeme §4; `23#103A` P13 (Entwurf der Gliederung); `23#104E` P13 (‚machen wir später genauer‘) | TRF MED | Feingliederung offen |
| COMBAT-DEC-TK-004 | **Hauptarterien und kritische Gefäße müssen simuliert werden,** sonst kann man nicht ausbluten | 📋 BESCHLOSSEN | `23#104E` P1; `M-PD` ‚Human anatomy…‘ (‚major arterial injury can cause rapid exsanguination‘) | MED | Gefäßkategorien (Minor, Major, Critical): Entwurf von `23#103A` |
| COMBAT-DEC-TK-005 | **Schmerz ist vom physischen Schaden getrennt** (ein gebrochener Finger ist schmerzhaft und wenig gefährlich, eine innere Blutung gefährlich und zunächst weniger offensichtlich) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#100A` §6 (Beispiel); `23#101E` P6 | MED | Modul 13 |
| COMBAT-DEC-TK-006 | **Verletzungen summieren sich;** der Körper wird als Ganzes bewertet (Gesamtblutverlust, Schmerz, Schock, Beweglichkeit, Gliedmaßenfunktion, Bewusstsein, Ermüdung); der Tod entsteht aus dem Körperzustand, nicht aus der sechsten Wunde | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (‚injuries accumulate…‘); `23#101E` P4 (‚zu viele Treffer und man ist unfähig sich zu bewegen oder sogar sterben‘); `23#103A` P6 | MED | – |
| COMBAT-DEC-TK-007 | **Offene Brüche mit sichtbarem Knochen sind möglich,** auch beim Spieler | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#101E` P9 (‚ja schlimmer, es können auch Knochen rausschauen möglicherweise‘), P16 | TRF ANI | Darstellung: Modul 22 |
| COMBAT-DEC-TK-008 | **Gore ist primär bei Gegnern stärker sichtbar,** aber der Spieler ist nicht immun gegen schwere sichtbare Verletzungen | 📋 BESCHLOSSEN | `23#101E` P16 (‚ich dachte eher bei den Gegnern, aber es kann sein, dass ein Knochen bei dir raussteht‘); `M-PD` (‚Player can also show visible wounds…‘) | ANI | – |
| COMBAT-DEC-TK-009 | **Verzögerte oder innere Verletzungen bleiben bis zur Diagnose unsicher;** Symptome treten später deutlicher auf | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` P4; `23#103A` P11–P12 | MED UI | Modul 13 |
| COMBAT-DEC-TK-010 | **Übernatürliche Verletzungen erweitern das normale System durch Modifikatoren** (zum Beispiel Heilungshemmung, Fluch), statt ein getrenntes Gesundheitssystem zu bilden | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#103A` P15; `23#104E` P15; `K-KO-010` | MED KRE | Modul 15 |
| COMBAT-DEC-TK-011 | **Menschliche NPCs und der Spieler teilen dieselbe Anatomie;** Monster brauchen teilweise eigene Anatomie, die einzeln festgelegt wird (‚das machen wir dann einzeln, ist eher komplex‘) | 📋 BESCHLOSSEN | `23#101E` P18, P19; `23#104E` P18, P19; `K-KO-006` | KRE MED | Modul 15 |
| COMBAT-DEC-TK-012 | **Feuer kann sich ausbreiten;** ein früher kleiner Kleidungsbrand lässt sich ersticken (zum Beispiel mit der Jacke) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` P14 | TRF MED | Verbrennungsgrade: Entwurf |
| COMBAT-DEC-TK-013 | **Gemeinsame Sekundärphysik** für Rückstoß, Laufen, Kampf, Fahrzeugbeschleunigung und -unfälle, Stürze, Tragen, Interaktion, Haar, Kleidung und Zubehör; realistische Dämpfung und Masse, kein übertriebenes Wackeln | 📋 BESCHLOSSEN | Gameplay-Systeme §9.3; `M-PD` ‚Body / physical response / recoil‘ | ANI TRF | Technik: `CQ-48` |
| COMBAT-DEC-TK-014 | **Die Wirkung eines Schlags, Stichs oder Hiebes wird anatomisch ausgewertet** (Region, Tiefe, Gewebe, möglicherweise Gefäß oder Organ) | 📋 BESCHLOSSEN | `23#162A` P3; `23#163E` P3; `COMBAT-DEC-NK-027` | TRF | – |
| COMBAT-DEC-TK-015 | **Gegner sind keine Hitpoint-Puppen:** sichtbare Folgen am Gegner (Arm schützen, humpeln); später auch humpeln und sich das Bein halten | 📋 BESCHLOSSEN | `23#86E` P3; `COMBAT-DEC-NK-046` | KI ANI | ‚in späteren Versionen‘ (`23#86E` P3) |
| COMBAT-DEC-TK-016 | **Ursachenabhängige Wundeigenschaften** (Eindringen, Energie, Schneiden, Quetschen, Brennen, Zerreißen, Geschossdurchmesser, Fragmentierung, übernatürliche Wirkung) leiten Verletzungen und Darstellung ab | 💡 VORSCHLAG | `23#103A` P10 (Entwurf des Assistenten); `M-PD` (Gore ist ursachenabhängig) | TRF | Abschnitt 6 |
| COMBAT-DEC-TK-017 | **Wundklassen für Projektile** (Durchschuss, unproblematisch steckend, problematisch steckend, kontaminiert/kompliziert) | 💡 VORSCHLAG | `23#103A` P3 | MED | `CQ-45` |
| COMBAT-DEC-TK-018 | **Wundeintrag als Datensatz** mit Region, Mechanismus, betroffenen Strukturen, Austritt, Blutungsgrad, Schmerz- und Schockbeitrag | 💡 VORSCHLAG | `23#103A` P2 (Beispiel ‚Wound #18427‘); Abschnitt 5 | MED PER | `CQ-46` |
| COMBAT-DEC-TK-019 | **Materialdurchschlag, Gewebe-, Knochen- und Rüstungsparameter** (Zahlen, Messwerte, Kalibrierung) sind nicht entschieden | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘; Entscheidungsstatus §5.6 | TRF | `CQ-45` |
| COMBAT-DEC-TK-020 | **Geometrie der Trefferzonen** (Hit-Volumina, Abbildung von Collision auf die 25+ Regionen, Anpassung an kontinuierlich veränderte Körper) ist nicht entschieden | ❓ OFFEN | Gameplay-Systeme §9.2 (kontinuierliche Morph-Ziele); `M-P14` (Themeninventar ‚human-body-hit-region-rig-mapping‘) | TRF ANI | `CQ-46`, `CQ-50` |
| COMBAT-DEC-TK-021 | **Gore-Schwellen, visuelle Familien, regionale Profile und Darstellung bei minderjährigen NPCs** sind offen und von Plattform-, Alters- und Rechtsprüfung abhängig | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘; Gameplay-Systeme §4 (Darstellung); Entscheidungsstatus §5.6 | ANI | `CQ-47` |
| COMBAT-DEC-TK-022 | **Aufprall- und Impulsreaktion am Ziel** (Zurückstoßen, Stolpern, Niederreißen bei starken Treffern) ist in den Quellen nicht beschrieben; nur Rückstoß der eigenen Waffe und Körperreaktionen als Verletzungsfolge | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | ANI TRF | `CQ-48` |
| COMBAT-DEC-TK-023 | **Verletzung von Tieren und Hunden** (Hund als Begleiter mit Verletzung, Altern, Tierarzt; wilde Tiere als Ziel) nutzt eine tierartige Anatomie; Einzelheiten sind nicht beschlossen | ❓ OFFEN | Spielvision §6.3 (Hund: Verletzung, Tierarzt); [Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md) | TRF MED KRE | `CQ-49` |

---

## VL: Verletzungen, Schmerz, Kampffolgen

Gruppe zu [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-VL-001 | **Kein Energy-Drink- und kein Sofortheilungsmodell.** Heilung besteht aus Behandlung und Zeit; wer verwundet ist, muss verbinden, desinfizieren und warten, bis er weitermachen kann | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#60E` (‚du heilst dich nicht mit einem Energy Drink, sondern musst verbinden, desinfizieren und warten, bis du wieder weitermachen kannst‘) | MED | – |
| COMBAT-DEC-VL-002 | **Behandlung besteht aus tatsächlichen Schritten** (Gefahr beseitigen, Blutung stoppen, Wunde reinigen oder desinfizieren, verbinden oder stabilisieren, Ruhe und Zeit); ein Verband macht nicht sofort gesund, er verhindert zunächst weiteres Verbluten | 🧭 RICHTUNG | `23#61A` §1 (Entwurf); `23#62E` P1; `23#85A` P5 (Beispiele) | MED STE | Schrittliste und Reihenfolge: Entwurf |
| COMBAT-DEC-VL-003 | **Behandlung auf drei Ebenen:** Feldbehandlung (leichte Verletzungen, Stabilisierung), medizinisch geschulter Jäger oder Arzt (manche mittelschwere Fälle), Krankenhaus (schwere innere Verletzungen, komplizierte Schüsse, schwere offene Brüche, schwere Verbrennungen, kritische Infektionen, massiven Blutverlust). Der Entwickler: ‚schwere Verletzungen sollten im Spital behandelt werden, leichte gehen im Wald, und wenn ein guter Arzt-Jäger dabei ist, auch mittlere im Wald‘ | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` C; `23#105A` §10 | MED NPC | Einzelzuordnung der Fälle: `CQ-52` |
| COMBAT-DEC-VL-004 | **Schmerzmittel wirken verzögert und lindern Symptome,** sie heilen nicht; sie können das Weiterkämpfen ermöglichen, wirken aber nicht direkt | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` B | MED | Liste: `CQ-52` |
| COMBAT-DEC-VL-005 | **Erholungsstufen** akut → stabilisiert → heilend → erholend → erholt; erneute Verletzung einer noch nicht ausgeheilten Region erreicht schwere Zustände schneller | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` D (‚ja genau, und wenn man dort wieder getroffen wird, ist der gleiche Zustand schneller erreicht‘) | MED | `COMBAT-DEC-ZA-007`; Zeitdauern: `CQ-51` |
| COMBAT-DEC-VL-006 | **Die Wahrnehmung der Verletzung geschieht über Animation, Audio, Atmung, Sicht, Zittern, Bewegung, Zielen, Stimme, Koordination und Handlungsfähigkeit;** Blutverlust, Schmerz, Schock und Adrenalin müssen spürbar sein | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` P5 (‚genau, aber der Spieler muss es fühlen‘) | ANI AUD UI | Details: Abschnitt 3 |
| COMBAT-DEC-VL-007 | **Adrenalin unterdrückt wahrgenommenen Schmerz zeitweise;** Symptome werden später deutlicher | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#101E` P7; `COMBAT-DEC-AU-008` | MED | – |
| COMBAT-DEC-VL-008 | **Nerven- und Sehnenschäden beeinflussen Kontrolle und Griff;** verletzte Gliedmaßen wirken sich auf Hinken, Sprinten, Waffeneinsatz, Nachladen, Gleichgewicht, Atmung und Ausdauer aus | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` A | MED WAF | `COMBAT-DEC-NK-026` |
| COMBAT-DEC-VL-009 | **Eine Diagnose hat Unsicherheit:** direkt nach der Verletzung weiß der Charakter nur, was er plausibel wissen kann (rechte Brust verletzt, starke Schmerzen, Blutung, erschwerte Atmung); erst die professionelle Untersuchung liefert Befunde (Rippenfraktur, Lungenverletzung) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (delayed/internal injuries); `23#103A` P12 (Beispiel); `23#104E` P12 (‚passt genau‘) | MED UI | Darstellung: `CQ-55` |
| COMBAT-DEC-VL-010 | **Ein Arztbesuch ist sinnvoll nach Treffern, deren Wirkung unsicher ist:** ‚dann sollte man ins Spital fahren und untersuchen lassen‘ | 📋 BESCHLOSSEN | `23#104E` P11 | MED | – |
| COMBAT-DEC-VL-011 | **Schweregrade der Spielergesundheit:** Blutverlust, Brüche, Gehirnerschütterung, innere Verletzungen, Verbrennungen, Vergiftung, Kälte, Hitze, Dehydrierung und selektive Infektion; Krankenhäuser und Operationen sind möglich, es gibt keinen vollständigen Rehabilitationssimulator; schwere Verletzungen können temporäre Funktionseinschränkungen verursachen | 📋 BESCHLOSSEN | `M-CA0905` (Player health) | MED PER | – |
| COMBAT-DEC-VL-012 | **Amputation und Prothesen gibt es nur über erzählte Story, falls überhaupt** | 📋 BESCHLOSSEN | `M-CA0905` (‚Amputation/prosthetics only via authored story if used‘) | MED PER | Spannung zur Gliedmaßendarstellung: `CK-04` |
| COMBAT-DEC-VL-013 | **Gore umfasst plausible Ergebnisse bis zu Verlust von Fingern, Händen und Gliedmaßen und – unter sehr extremen, physikalisch passenden Bedingungen – Enthauptung** bei erwachsenen Menschen | 📋 BESCHLOSSEN (Darstellung) | Gameplay-Systeme §4 (Darstellung); `M-PD` ‚Gore / age presentation‘ | ANI TRF | Schwellen: `CQ-47`; Spannung `CK-04` |
| COMBAT-DEC-VL-014 | **Infektionsrisiko ist ein Systemzustand;** eine Infektion entwickelt sich zeitverzögert aus Kontamination, Wundtiefe, Fremdmaterial, Gewebeschaden, Behandlungsstand, Zeit und Immunzustand und nicht in Sekunden | 🧭 RICHTUNG | `M-PD` ‚Human anatomy…‘ (infection risk); `23#101E` P1 (Entwickler: Kugel drin → entzündet sich schneller; durch `23#102A` korrigiert); `23#103A` P4 (Entwurf der Faktoren) | MED | Modell: `CQ-54` |
| COMBAT-DEC-VL-015 | **Eine im Körper verbliebene Kugel muss nicht automatisch entfernt werden;** die Behandlung hängt von Lage und Zustand ab; eine problematische Lage kann professionelle Entfernung verlangen | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#102A`; `23#103A` P3 | MED | `COMBAT-DEC-TK-002` |
| COMBAT-DEC-VL-016 | **Schmerz beeinflusst Zielen, Nachladen, Sprint, Nahkampf, Konzentration, Geräusche des Spielers, Interaktionen und Fahren;** auf Grounded stärker | 🧭 RICHTUNG | `23#100A` §6 (Entwurf); `23#101E` P6 (‚ja das ist auch gut‘); `M-PD` ‚Human anatomy…‘ | MED WAF | – |
| COMBAT-DEC-VL-017 | **Verletzung erschwert Tarnung** (Hinken, schweres Atmen, Schmerzlaute, Blutspuren) | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘ | KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-VL-018 | **Begleiter können ernsthaft verletzt werden und ausfallen;** sie können erste Hilfe leisten, den Spieler retten und herausziehen; ein medizinisch kompetenter Begleiter erweitert die Feldbehandlung | 📋 BESCHLOSSEN | `N-CO-006`; `M-PD` ‚Human anatomy…‘ (medizinisch geschulter Jäger) | NPC MED | – |
| COMBAT-DEC-VL-019 | **Bleibende Verletzungen können Folge einer Niederlage sein** (zum Beispiel bleibende Verletzungen, Krankenhauskosten, Zeitverlust) | 🧭 RICHTUNG | Spielvision §14.2; `23#61A` §1 (Folgenliste, Entwurf) | MED PER | Modul 14 |
| COMBAT-DEC-VL-020 | **Behandlung soll nach einem kleinen Kampf nicht zwanzig Minuten Menüarbeit verlangen** | 💡 VORSCHLAG | `23#85A` §5 (Hinweis des Assistenten); keine Entwicklerantwort | UI STE | Prüfziel: `CQ-55` |
| COMBAT-DEC-VL-021 | **Eine mögliche Verlaufskette** (schwere Verletzung → nach Stunden stabilisiert → nach Tagen deutliche Einschränkung → weitgehend geheilt, ‚komprimiert auf sinnvolle Spielzeit‘) | 💡 VORSCHLAG | `23#61A` §1 (Beispiel des Assistenten) | MED | Abschnitt 5 |
| COMBAT-DEC-VL-022 | **Medikamente, Drogen, Diagnoselisten und Krankenhauswirtschaft** sind nicht festgelegt | ❓ OFFEN | Entscheidungsstatus §5.3; `M-OD` ‚Progression / body / survival‘ | MED | `CQ-52` |
| COMBAT-DEC-VL-023 | **Zeitdauern und Zeitkompression der Heilung** (Spielzeit gegenüber Weltzeit, Verhältnis zu Schlaf, Reisen, Zeitsprüngen) sind nicht entschieden | ❓ OFFEN | `23#61A` (‚natürlich komprimiert auf sinnvolle Spielzeit‘, Entwurf); Entscheidungsstatus §5.1 (Kalender und Story-Zeit offen) | MED PER | `CQ-51` |
| COMBAT-DEC-VL-024 | **Verletzungen im Koop:** Krankenhaus im Koop überspringbar, Ausfall wenige Minuten bis etwa 15 Minuten, klassischer Modus; detailliertes Rettungssystem mit vereinfachter Wiederbelebung als Option | 📋 BESCHLOSSEN | `E-KO-007`; `E-KO-008` | KOO MED | [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md) |

---

## BW: Bewusstlosigkeit, Tod, Rettung

Gruppe zu [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-BW-001 | **Kein folgenloser Checkpoint als Todesmodell:** ‚es darf nicht einfach ein Checkpoint sein und du startest von dort‘; man kann das ausschalten, aber es soll nicht Standard sein | 📋 BESCHLOSSEN | `23#60E`; `23#88E` A (‚kann man in den Einstellungen andrehen‘); `M-PD` ‚Death / failure — OPEN‘ | PER SCH | Verhältnis zu BW-009: `CK-01` |
| COMBAT-DEC-BW-002 | **Ein klassischer Checkpoint-Neustart kann als optionaler Komfortmodus existieren** | 📋 BESCHLOSSEN | `M-PD` ‚Death / failure — OPEN‘; Spielvision §14.2; `23#88E` A | PER SCH | – |
| COMBAT-DEC-BW-003 | **Kein Krankenhaus-Reset ohne Folgen ‚wie in GTA‘ als Standard** | 📋 BESCHLOSSEN | `23#88E` B (‚wie in GTA, also zu leicht‘); Spielvision §14.2 | PER | – |
| COMBAT-DEC-BW-004 | **Ein Soulslike-Wiederherstellungsmodell ist nicht das Ziel** (‚es soll kein Souls Game sein‘) | 📋 BESCHLOSSEN | `23#88E` F; `M-PD` ‚Death / failure — OPEN‘ | PER | – |
| COMBAT-DEC-BW-005 | **Der Tod wird medizinisch bestimmt** (Blutvolumen, Trauma, Organschaden, Schock, Bewusstsein), nicht durch ‚HP ≤ 0‘; der Entwickler: ‚ist sowieso klar, dass man das haben soll‘ | 📋 BESCHLOSSEN | `23#88E` H; `23#87A` H (Entwurf); `M-PD` ‚Death / failure — OPEN‘ (medically plausible death risk) | MED | – |
| COMBAT-DEC-BW-006 | **Kritische und sterbende Zustände** (Near Death) liegen vor dem Tod; der Entwickler findet das ‚auch nicht schlecht‘ | 🧭 RICHTUNG | `23#87A` G; `23#88E` G; `23#90A`; `M-PD` ‚Death / failure — OPEN‘ (favored direction) | MED UI | Ausgestaltung: BW-020; `CQ-60` |
| COMBAT-DEC-BW-007 | **Rettung hängt von der Lage ab:** Begleiter, Rettungsdienste, Passanten, Polizei und Ort | 🧭 RICHTUNG | `M-PD` ‚Death / failure — OPEN‘; Spielvision §14.2; `23#88E` C (‚hört sich gut an, nur weiß man nicht, was passiert, wenn man wirklich stirbt‘) | NPC POL | Zuordnung: `CQ-56` |
| COMBAT-DEC-BW-008 | **Der Spieler kann sich aus unmittelbarer Gefahr kämpfen, einen Freund oder Jäger anrufen, zusammenbrechen, gerettet werden und im Krankenhaus aufwachen** | 🧭 RICHTUNG | `23#88E` G (‚ruft man noch seinen Kumpel an… er rettet einen und man wird ohnmächtig und wacht im Spital auf‘); `23#90A`; Spielvision §14.2 | NPC STE | Ob der Anruf eine Taste, ein Menü oder ein Ereignis ist: `CQ-56` |
| COMBAT-DEC-BW-009 | **Monsterabhängige Ausgänge einer Niederlage:** das Wesen geht, entführt, verflucht, besitzt oder tötet, je nach seinen Regeln | 🧭 RICHTUNG | `23#87A` D; `23#88E` D (‚sehr interessant‘); `M-PD` ‚Death / failure — OPEN‘ | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md); `CQ-61` |
| COMBAT-DEC-BW-010 | **Folgen einer Niederlage:** Zeitverlust, neue Opfer, Wechsel des Monsterortes, veränderter Fallzustand, zurückgelassene Ausrüstung, am Ort zurückgebliebenes Fahrzeug, Krankenhaus, bleibende Verletzungen. Der Entwickler: ‚aber die Folgen sind gut‘ | 🧭 RICHTUNG | Spielvision §14.2; `M-PD` ‚Death / failure — OPEN‘; `23#61A` §1; `23#62E` | PER NPC | Welt entwickelt sich weiter (`23#61A`); keine ‚Mission Failed‘-Bildschirme (`23#63A`) |
| COMBAT-DEC-BW-011 | **Ein Fall scheitert nicht zwangsläufig bei einer Niederlage:** Fall und Kreatur bleiben aktiv, der Spieler behandelt, recherchiert und versucht es erneut; die Lage kann sich geändert haben (Versteck gewechselt, weiteres Opfer, Polizei sperrt ab, Zeuge verschwindet, Monster wird vorsichtiger) | 🧭 RICHTUNG | `23#63A` §1 (Entwurf); `23#62E` | ERM NPC | – |
| COMBAT-DEC-BW-012 | **Gameplay-Tod: Rückkehr zum letzten geeigneten Checkpoint mit nur einem minimal überlebensfähigen Gesundheitszustand,** ‚gerade dass man überlebt, das ist dann in Kämpfen extrem schwierig‘; keine kostenlose Vollheilung. Das Heilmodell bleibt unverändert | 📋 BESCHLOSSEN | `23#364E` P2; `23#365A` §1 (Ausformulierung); `23#366E` (‚der Rest passt‘); `K-KS-005`; Kreaturen 07 §3.1 | PER MED | Widerspruch zu `M-PD` OPEN: `CK-01`; Anatomie des Mindestzustands: `CQ-57` |
| COMBAT-DEC-BW-013 | **Permadeath ist aktivierbar, nie Standard** | 📋 BESCHLOSSEN | `23#364E` P3 (‚ja, permadeath ist aktivierbar‘); `23#88E` E; `M-PD` ‚Death / failure — OPEN‘ (nur optional) | PER SCH | Varianten: `CQ-59` |
| COMBAT-DEC-BW-014 | **Ein Story-Tod ist erzählerisch bindend** (‚bei Story-Tod ist das dann storybindend‘); ein Gameplay-Tod ist nicht kanonisch | 📋 BESCHLOSSEN | `23#364E` P16; `23#365A` §2–§3; `K-KS-005`; `W-RE-008` | PER NPC | Jenseitsregeln: [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) |
| COMBAT-DEC-BW-015 | **Es gibt einen Hauptcharakter plus Begleiter über das gesamte Spiel;** nach einem Tod wird nicht jedes Mal ein neuer Charakter erzeugt | 📋 BESCHLOSSEN | `23#362E` P1; `23#365A` §8 | PER | – |
| COMBAT-DEC-BW-016 | **Softlock-Schutz:** kein unendlicher Todes-Loop; im absoluten Ausnahmefall ein älterer sicherer Checkpoint oder der Spieler wählt einen früheren Spielstand, ohne kostenlose Heilung | 🧭 RICHTUNG | `23#365A` §1 (Vorschlag); `23#366E` P2–P13 (‚der Rest passt‘); `23#370A` | PER | Details: Modul 24 |
| COMBAT-DEC-BW-017 | **Safe-Checkpoint-Validator:** vor einem Checkpoint wird geprüft, dass kein Angriff unmittelbar eintrifft, keine tödliche Projektilbahn besteht, kein Griff oder Takedown läuft, keine unmittelbare Explosion, keine nicht reproduzierbare Transition und der Zustand reproduzierbar ist | 💡 VORSCHLAG | `23#367A` (Entwurf des Assistenten) | PER TST | [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| COMBAT-DEC-BW-018 | **Autosaves entstehen vor und nach Kämpfen, gelegentlich bei längeren Fahrten und in der Natur; bei wirklich langen Kämpfen kann ein Autosave zwischen den Phasen liegen** | 📋 BESCHLOSSEN | `23#368E`; `23#371E`; `E-QA-039` | PER | Phasenkriterien: Modul 24 |
| COMBAT-DEC-BW-019 | **Todesfolgen sind ein eigener Einstellbereich** (zum Beispiel Persistent, Reduziert, Checkpoint) und unabhängig von der Kampfschwierigkeit | 🧭 RICHTUNG | `23#61A` §1 (Auswahlfeld im Entwurf); `23#65A` §1; `23#66E` P1; `M-PD` ‚Difficulty / configuration‘ (independently configurable) | SCH PER | Voreinstellungen: `CQ-59` |
| COMBAT-DEC-BW-020 | **Beschreibung eines Near-Death-Zustands (Entwurf):** starke Sichtbeeinträchtigung, extrem eingeschränkte Bewegung, kaum Waffenhandling, Blutverlust, eingeschränkte Wahrnehmung, eventuell Kriechen, Notversorgung möglich, Begleiter kann helfen; kein Arcade-Timer, sondern Vermittlung über Bild, Geräusche, Atmung und Animation | 💡 VORSCHLAG | `23#87A` G | MED ANI UI | `CQ-60` |
| COMBAT-DEC-BW-021 | **Auf Grounded führt derselbe Treffer schneller in den Near-Death-Zustand, und ein weiterer Treffer kann töten;** der Tod ist dort leichter möglich | 📋 BESCHLOSSEN | `23#64E` P1; `23#88E` G (‚das mit Grounded auch nicht schlecht‘); `E-FA-017` | SCH MED | – |
| COMBAT-DEC-BW-022 | **Bewusstlosigkeit entsteht durch Blutverlust, Schock, Schläge gegen den Kopf, Würgegriffe** u. a. und ist ein Systemzustand (Bewusstsein) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (consciousness); `COMBAT-DEC-NK-011`, `-028` | MED | – |
| COMBAT-DEC-BW-023 | **Festnahme ist kein automatisches Game Over;** Verhör, Strafen, Gericht, Gefängnis und vergehende Zeit können auftreten | 📋 BESCHLOSSEN | `E-RE-015`; Gameplay-Systeme §10 | POL | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-BW-024 | **Begleiter können den Spieler retten** (erste Hilfe, Deckung, Herausziehen) und werden dabei selbst verwundbar | 📋 BESCHLOSSEN | `N-CO-006` | NPC | – |
| COMBAT-DEC-BW-025 | **Koop:** Krankenhaus ist überspringbar; der Ausfall dauert wenige Minuten bis etwa 15 Minuten; ein klassischer Modus existiert; es gibt ein detailliertes Rettungssystem mit vereinfachter Wiederbelebung als Option | 📋 BESCHLOSSEN | `E-KO-007`; `E-KO-008` | KOO | `CQ-62` |
| COMBAT-DEC-BW-026 | **Der Spielercharakter kann besessen werden;** dann geht die Kontrolle zum Begleiter, der den Hauptcharakter retten muss | 🧭 RICHTUNG | `23#364E` P8; `23#365A` §8 (Entwurf) | NPC KRE | [Kreaturen 05](../Kreaturen/05-Geister-Besessenheit-und-Fluecher.md) |
| COMBAT-DEC-BW-027 | **Was geschieht bei einem tatsächlichen Tod** (Todesloop, Verhältnis von Rettung, Krankenhaus, Checkpoint und Permadeath je Voreinstellung) ist nicht endgültig entschieden | ❓ OFFEN | `M-PD` ‚Death / failure — OPEN‘; `M-OD` (‚Exact final death / near-death / rescue / hospital / checkpoint behavior…‘); Entscheidungsstatus §5.1; `23#88E` C (‚nur weiß man nicht, was passiert, wenn man wirklich stirbt‘) | PER | `CQ-07` |
| COMBAT-DEC-BW-028 | **Widerspruch:** `M-PD`, `M-OD`, Spielvision §14.2 und die NPC-Dokumentation führen den Todesloop als OPEN; `23#364E` (später, 25.08.2026) und die Dokumentationen Kreaturen 07, Ermittlung 19 und Welt `W-RE-008` führen den Gameplay-Tod als Checkpoint mit Mindestzustand | ⚠️ KONFLIKT | `M-PD` ‚Death / failure — OPEN‘; `23#364E` P2, P16; `N-NL-013`; `K-KS-005` | PER | `CK-01` |
| COMBAT-DEC-BW-029 | **Der anatomische Mindestzustand nach einem Gameplay-Tod** (nicht ‚ein magischer roter HP-Balken‘) ist nicht definiert | ❓ OFFEN | `23#370A` (‚definieren wir beim finalen Death-/Medical-Integration-Block‘) | MED PER | `CQ-57` |
| COMBAT-DEC-BW-030 | **Krankenhaus als Ort** (Aufwachen, Kosten, Dauer, Wirtschaft, Meldung von Schussverletzungen) ist nicht entschieden | ❓ OFFEN | Entscheidungsstatus §5.3 (Krankenhauswirtschaft); `23#61A` (Folgenliste) | PER POL | `CQ-58` |
| COMBAT-DEC-BW-031 | **Abgrenzung zwischen Bewusstlosigkeit, Kampfunfähigkeit und Tod** (Schwellen, Dauer des Near-Death-Zustands) | ❓ OFFEN | `23#61A` (‚wie genau wir Tod von bloßer Bewusstlosigkeit… unterscheiden, sollten wir später noch ausführlich entscheiden‘) | MED | `CQ-60` |

---

## KS: Kreaturenkampf und Spezialfähigkeiten

Gruppe zu [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-KS-001 | **Physischer Schaden und übernatürliche Wirkung sind getrennt;** Spezialmaterial wirkt nur bei passender Artregel | 📋 BESCHLOSSEN | `K-KO-001` (`23#293A` P3, `23#294E` P3) | KRE MUN | `COMBAT-DEC-MU-004` |
| COMBAT-DEC-KS-002 | **Manche Wesen sind verletzbar, aber nicht damit tötbar;** ein Schuss kann nur vertreiben oder unterbrechen | 📋 BESCHLOSSEN | `K-KO-002`; `23#85A` P2 (Beispiel Monster C) | KRE | – |
| COMBAT-DEC-KS-003 | **Regeneration ist artabhängig** und in Teilregeln zerlegt (Rate, Gewebe, Knochen, Organe, Glieder, Ressourcen, Umwelt, Unterdrücker, Grenzen) | 📋 BESCHLOSSEN | `K-KO-003` | KRE MED | – |
| COMBAT-DEC-KS-004 | **Schmerzempfinden ist individuell;** kein Schmerz heißt nicht keine Beeinträchtigung | 📋 BESCHLOSSEN | `K-KO-004` | KRE KI | – |
| COMBAT-DEC-KS-005 | **Anatomie ist modular und artspezifisch;** funktionale Verletzungen gelten auch für Kreaturen mit Körper, außer die Art regeneriert sie oder ist körperlos | 📋 BESCHLOSSEN | `K-KO-005`, `K-KO-006`; Spielvision §9.2 | KRE TRF | Datensatz je Art: Abschnitt 3 |
| COMBAT-DEC-KS-006 | **Bleibende Verletzungen und Narben überdauern Begegnungen,** wenn die Art sie nicht regeneriert; wiederkehrende Individuen sind an den Folgen früherer Kämpfe erkennbar | 📋 BESCHLOSSEN | `K-KO-007`; Spielvision §9.2 | KRE NPC | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-KS-007 | **Schwächen haben Wirkungsarten:** tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend, fähigkeitsstörend | 📋 BESCHLOSSEN | `K-KO-008` | KRE MUN | [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) |
| COMBAT-DEC-KS-008 | **Resistenz wird intern gestuft;** die Oberfläche zeigt beobachtete Belege, keine Zahl | 📋 BESCHLOSSEN | `K-KO-009` | KRE UI | Archivdarstellung `K-KO-018` ❓ |
| COMBAT-DEC-KS-009 | **Bestimmte Wesen sind ohne die richtige Methode kaum endgültig tötbar;** es gibt mehrere Methoden, Vorbereitung wird belohnt, Improvisation ist möglich, Überreste, Gefangennahme und Verhör sind Optionen | 📋 BESCHLOSSEN | `K-KO-014`; `E-FA-013` | KRE ERM | – |
| COMBAT-DEC-KS-010 | **Existenzformen** (körperlich, verwandelter Mensch, besessen, körperlos, zeitweise manifestiert, gebunden) und Herkunft sind getrennte Angaben; ‚Unbekannt‘ ist gültig | 📋 BESCHLOSSEN | `K-KL-002`, `K-KL-003` | KRE | Kampfregeln je Form: Abschnitt 5 |
| COMBAT-DEC-KS-011 | **Bei gebundenen Entitäten ist die Bindung wichtiger als der Schaden;** Geister verschwinden, manipulieren die Umgebung, greifen aus unerwarteten Richtungen an und lassen sich nicht normal erschießen | 📋 BESCHLOSSEN / 🧭 | `K-GE-004`; `23#85A` P13 (Beispiel Geist); `23#86E` P13 | KRE | Serienlogik des Geistes (Salz, Eisen, Überreste) ist 💡 (`K-GE-016`) |
| COMBAT-DEC-KS-012 | **Bei einer Besessenheit sind Wirt und Entität getrennte Figuren;** die Besessenheit ist graduell und hat Folgen für Wirt und Umfeld | 📋 BESCHLOSSEN | `K-GE-007`, `K-GE-008`, `K-GE-012` | KRE NPC | Kampf gegen Besessene: `CQ-63` |
| COMBAT-DEC-KS-013 | **Dämonische Wesen sind intelligent, planend und vertragsfähig;** nicht jeder Dämon greift sofort an, Verhandlung ist möglich | 📋 BESCHLOSSEN | `K-GE-009`, `K-HW-004` | KRE KI | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-KS-014 | **Leviathane sind uralt;** gewöhnliche Waffen töten sie nicht; eine zweite Tötung im Fegefeuer ist endgültig | 📋 BESCHLOSSEN | `K-HW-006`, `K-HW-011` | KRE | Kampf: `CQ-66` |
| COMBAT-DEC-KS-015 | **Intelligenz hat mehrere unabhängige Dimensionen und die Antriebe ersetzen einen Aggro-Wert** | 📋 BESCHLOSSEN | `K-KL-005`, `K-KL-006` | KI KRE | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-KS-016 | **Monster müssen nicht bis zum Tod kämpfen:** Sie fliehen, wechseln den Bau oder das Opfer, verstecken sich oder holen Verstärkung | 📋 BESCHLOSSEN | `N-NL-001`; `23#293A` P13 | KI NPC | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-KS-017 | **Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf;** manche beobachten den Spieler und greifen später besser vorbereitet an | 📋 BESCHLOSSEN | `N-NL-002`; `23#1074A` P58–P59 | KI NPC | – |
| COMBAT-DEC-KS-018 | **Individuen derselben Art unterscheiden sich;** Ausnahme- und Alpha-Individuen sind keine Elite-Klasse (Ursachenmenge; die Schwäche kann teilweise versagen); es gibt keine Anzeige wie ‚ALPHA‘ oder ‚IMMUN‘, sondern Entdeckung durch Beobachtung und Recherche | 📋 BESCHLOSSEN | `K-KL-009`, `K-KL-010`, `K-KL-011` | KRE UI | – |
| COMBAT-DEC-KS-019 | **Der Tod eines Anführers löscht seine Gruppe nicht;** die Überlebenden verstecken sich | 📋 BESCHLOSSEN | `K-HW-009`; `N-TE-001` | KRE KI | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-KS-020 | **Verletzungen durch Wesen sind mythologieabhängig,** die Behandlung muss gefunden werden; Infektion oder Verwandlung ist bei manchen Arten möglich, ein Gegenmittel ist denkbar, es gibt kein Game Over, nur Tod | 📋 BESCHLOSSEN | `K-KO-011`, `K-KO-012` | MED KRE | [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| COMBAT-DEC-KS-021 | **Gore bei Wesen ist ursachenabhängig;** manche Arten haben eigene Zerstörungssysteme | 📋 BESCHLOSSEN | `K-KO-013` | TRF ANI | `CQ-47` |
| COMBAT-DEC-KS-022 | **Humanoide übernatürliche Gegner können den größten Teil des menschlichen Nahkampfsystems nutzen;** andere haben falsche Anatomie für Griffe, sind zu groß, besitzen Klauen, beißen, bewegen sich ungewöhnlich oder regenerieren | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#166A` P14; `23#167E` P14 | KRE ANI | `COMBAT-DEC-NK-032` |
| COMBAT-DEC-KS-023 | **Kampfarchetypen als Beispiele** (Geist, sehr schneller Jäger, intelligenter Gegner, schweres Monster; tierartig, humanoid-intelligent) | 💡 VORSCHLAG | `23#85A` P13; `23#98A` §8 (Entwurf des Assistenten) | KRE | Abschnitt 4 |
| COMBAT-DEC-KS-024 | **Kampfprofil einer Art als Datensatz** (Anatomie, Regeneration, Schmerz, Resistenz, Schwächen, Fähigkeiten, Taktikvorrat, Wahrnehmung, Verhalten bei Verletzung) | 💡 VORSCHLAG | Abschnitt 3 (Entwurf); Vorlage in [Kreaturen 19](../Kreaturen/19-Artennennungen-und-Artenvorlage.md) §4 | KRE TST | `CQ-64` |
| COMBAT-DEC-KS-025 | **Eine beschlossene Artenliste und Kampfprofile der Kernarten gibt es nicht;** die Quellen wünschen 20 bis 30 tiefe Kernarten statt 100 flacher | ❓ OFFEN | [Kreaturen 19](../Kreaturen/19-Artennennungen-und-Artenvorlage.md) §1; `M-OD` | KRE | `CQ-64` |
| COMBAT-DEC-KS-026 | **Konfrontation mit körperlosen und kaum tötbaren Wesen im Kampf** (Manifestation stören, Bindung lösen, Schutz, Flucht) ist mechanisch nicht beschrieben | ❓ OFFEN | `K-GE-003`, `K-GE-019` (Exorzismus-/Bannregeln ❓) | KRE STE | `CQ-65` |
| COMBAT-DEC-KS-027 | **Große Kämpfe gegen Hauptbedrohungen** (mehrere Akte, lange Kampfdauer, Phasen, Zwischen-Checkpoints) sind nicht beschrieben; beschlossen ist nur, dass Hauptbedrohungen über mehrere Akte laufen und ein sehr langer Kampf Zwischen-Autosaves erlauben darf | ❓ OFFEN | `K-HW-016`; `23#371E`, `23#372A` (Beispiel eines 30-Minuten-Kampfes, Assistent) | KRE PER | `CQ-66` |
| COMBAT-DEC-KS-028 | **Wilde Tiere und Hunde im Kampf** (Angriff, Abwehr, Jagd) sind Teil der Tierdokumentation und nicht der Monsterregeln | 🧭 RICHTUNG | [Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md); `K-TI-003` | KRE | `CQ-49` |

---

## SJ: Schwächen, Vorbereitung, Jagdmethoden

Gruppe zu [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-SJ-001 | **Die Kernschleife ist Ankunft → Dialog → Ermittlung → Recherche → Vorbereitung → Konfrontation unter Ausnutzung der Schwäche → dauerhafte Konsequenzen** | 🧭 RICHTUNG | Spielvision §2.2 (Zielwerte vorläufig) | ERM KRE | – |
| COMBAT-DEC-SJ-002 | **Die richtige Waffe, Munition und Kreaturschwäche sind Kernstück der Jagd;** ein unbekanntes Wesen kann extrem gefährlich sein, dasselbe mit guter Vorbereitung kontrollierbar | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#85A` P1 | KRE WAF | – |
| COMBAT-DEC-SJ-003 | **Schwächen haben Wirkungsarten** (tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend, fähigkeitsstörend) | 📋 BESCHLOSSEN | `K-KO-008` | KRE MUN | – |
| COMBAT-DEC-SJ-004 | **Eine Schwäche kann bei Ausnahme-Individuen teilweise versagen** | 📋 BESCHLOSSEN | `K-KL-009` | KRE | Prüfziel für Tests |
| COMBAT-DEC-SJ-005 | **Das Archiv zeigt das Wissen des Hunters, keinen Debugger;** Statuswörter Confirmed, Suspected, Disputed, Disproven, Historical/Unverified | 📋 BESCHLOSSEN | `K-FO-001`, `K-FO-002` | UI ERM | – |
| COMBAT-DEC-SJ-006 | **Wissen kann falsch sein, Begleiter irren;** Monsterwissen stammt aus Vorwissen, Forschung, Notizen und unzuverlässigen Quellen | 📋 BESCHLOSSEN | `K-FO-003`, `K-FO-004` | ERM NPC | – |
| COMBAT-DEC-SJ-007 | **Umfassende freie Monsterforschung** (Beobachten, Spuren, Proben, Tests) ist vorgesehen | 📋 BESCHLOSSEN | `K-FO-005` | ERM KRE | – |
| COMBAT-DEC-SJ-008 | **Methoden zum Töten verlangen Recherche oder eigenes Testen;** es gibt mehrere Methoden, Fangen und Verhör sind möglich | 📋 BESCHLOSSEN | `E-FA-013`; `K-KO-014` | ERM KRE | – |
| COMBAT-DEC-SJ-009 | **Auflösungsarten:** töten, vertreiben, bannen, fangen, Fluch brechen, retten, Artefakt zerstören oder sichern, verhandeln, überleben, evakuieren | 📋 BESCHLOSSEN | `E-FA-009`; `M-PD` | ERM KRE | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-SJ-010 | **Rituale brauchen Zutaten, Vorbereitung, Zeit und Ort;** Fehler haben Folgen; keine Quick-Time-Rituale als Standard | 📋 BESCHLOSSEN | `K-JA-001`, `K-JA-003`, `K-JA-004` | KRE STE | Kosten: `K-JA-011` ❓ |
| COMBAT-DEC-SJ-011 | **Schutzmaßnahmen sind real, nicht absolut und zerstörbar** | 📋 BESCHLOSSEN | `K-JA-005` | KRE | – |
| COMBAT-DEC-SJ-012 | **Vorbereitung vor der Jagd:** Archiv → Hypothese → Ausrüstung aus Kofferraum oder Unterschlupf → Einsatz mit begrenztem Körperinventar | 🧭 RICHTUNG | `23#85A` P10; `23#86E` P10; `COMBAT-DEC-WA-004` | WAF ERM | [Modul 09](09-Waffenklassen-und-Ausruestung.md) |
| COMBAT-DEC-SJ-013 | **Hunter-Munition wirkt nur bei passender Schwäche** | 📋 BESCHLOSSEN | `23#155E` P5; `COMBAT-DEC-MU-004` | MUN KRE | – |
| COMBAT-DEC-SJ-014 | **Resistenz ist intern gestuft; die Oberfläche zeigt beobachtete Belege** (zum Beispiel ‚Kugeln wirkten kaum‘), keine Prozentwerte | 📋 BESCHLOSSEN | `K-KO-009`; Spielvision §9.2 | UI KRE | `K-KO-018` ❓ |
| COMBAT-DEC-SJ-015 | **Eine Falschklassifikation kann zu einer wirkungslosen Vorbereitung führen** (Folge der Unsicherheit des Wissens) | 🧩 ABGELEITET | Prämissen: `K-FO-003`, `COMBAT-DEC-SJ-002` | KRE ERM | Prüfziel: Szenario 5 ([Modul 26](26-Querschnittsszenarien.md)) |
| COMBAT-DEC-SJ-016 | **Der Kampf selbst liefert Wissen:** Reaktionen des Wesens auf Waffen und Methoden fließen in Archiv und Notizen, ohne Schwellen zu behaupten | 💡 VORSCHLAG | Anwendung von `K-FO-004` und `K-KI-013` | ERM KI | Schwellen: `CQ-67` |
| COMBAT-DEC-SJ-017 | **Schwellen für Hypothese und Bestätigung sowie der Umgang mit falscher Klassifikation** sind nicht festgelegt | ❓ OFFEN | `K-FO-012`; `M-OD` | ERM | `CQ-67` |
| COMBAT-DEC-SJ-018 | **Kosten- und Risikomodell der Rituale** ist nicht festgelegt | ❓ OFFEN | `K-JA-011` | KRE | `CQ-68` |
| COMBAT-DEC-SJ-019 | **Rückmeldung im Kampf über die Wirksamkeit** (Wirkungsanzeige, Hinweise, Audio) ist nicht entschieden | ❓ OFFEN | `K-KO-018`; `COMBAT-DEC-ST-007` | UI | `CQ-69` |

---

## FF: Fallen, Fesselung, Lebendfang

Gruppe zu [Modul 17](17-Fallen-Fesselung-und-Lebendfang.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-FF-001 | **Gefangennahme ist ein umfassendes System** (Fallen, Fesseln, Betäubung, Rituale, Transport, sichere Verwahrung, selbst gebaute Sicherheitsmaßnahmen, Forschung, Verhandlungen, Konsequenzen je nach Kreatur) | 📋 BESCHLOSSEN | `K-CO-001` (`48#128A`, `48#129E`) | KRE | Reihenfolge der Ausarbeitung offen |
| COMBAT-DEC-FF-002 | **Es gibt keine universelle Gefängniszelle;** die Wirksamkeit hängt von der Mythologie ab | 📋 BESCHLOSSEN | `K-CO-002` | KRE | – |
| COMBAT-DEC-FF-003 | **Fallen können an beliebigen Orten gebaut werden;** der Bunker bleibt ein eigenes Planungsthema | 📋 BESCHLOSSEN | `K-CO-003` (`48#131E`) | KRE | – |
| COMBAT-DEC-FF-004 | **Fallen bleiben persistent** und verändern sich durch Umwelt, NPCs, Monster, Spieler und Zeit; die Entwicklung läuft ohne ständige Vollsimulation weiter | 📋 BESCHLOSSEN | `K-CO-004` (`48#133E`) | KRE PER | Speicherung: [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| COMBAT-DEC-FF-005 | **Fallen entstehen aus vorgefertigten, anpassbaren Vorlagen** (Größe, Material, Ausrichtung, Position), kein Freihandzeichnen als Pflicht | 📋 BESCHLOSSEN | `K-CO-005` (`48#135E`) | KRE UI | Bedienung: `CQ-70` |
| COMBAT-DEC-FF-006 | **Monster reagieren taktisch auf Fallen** (erkennen, untersuchen, umgehen, sabotieren, täuschen, Gegenstrategien) | 📋 BESCHLOSSEN | `K-KI-015`; `N-NE-004` | KI KRE | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-FF-007 | **Ein in einer wirksamen Falle gefangenes Wesen befreit sich nicht durch KI allein** | 💡 VORSCHLAG | `K-KI-016` (`48#138A`) | KI | Prüfziel |
| COMBAT-DEC-FF-008 | **Kreaturen können lebend gefangen und verhört werden;** Verhandlung und Kooperation sind möglich | 📋 BESCHLOSSEN | `K-CO-007`; `N-NL-005` | KRE NPC | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-FF-009 | **Die Gefängnissicherheit passt sich dynamisch an übernatürliche Bedrohungen an** | 📋 BESCHLOSSEN | `K-CO-006` | POL KRE | Ermittlung 16 |
| COMBAT-DEC-FF-010 | **Mehrere Hunter bereiten gemeinsam einen Ort vor** (Koop) | 💡 VORSCHLAG | `K-CO-008` | KOO | – |
| COMBAT-DEC-FF-011 | **Das Aufrechterhalten einer Falle im Kampf** (Wesen steht im Zeichen, Zeichen wird verwischt, Hunter stört) | 💡 VORSCHLAG | Ableitung aus `K-CO-004`, `K-JA-005` (Schutzmaßnahmen zerstörbar) | KRE | Abschnitt 3 |
| COMBAT-DEC-FF-012 | **Nichttödliche Überwältigung von Menschen:** Würgegriff bis Bewusstlosigkeit ohne Tod ist als Aufgabegriff-Ausgang möglich; Fesselung und Betäubungswaffen sind nicht beschrieben | 🧭 RICHTUNG | `COMBAT-DEC-NK-011`, `-012`; `23#166A` (Frage der nichttödlichen Ausschaltung); `23#167E` P17 | MED POL | `CQ-71` |
| COMBAT-DEC-FF-013 | **Festnahme (durch Polizei) ist kein Game Over;** die Festnahme durch den Spieler ist nicht beschrieben | 📋 BESCHLOSSEN (Polizei) / ❓ OFFEN (Spieler) | `E-RE-015`; `E-QA-021` (💡 Festnahme statt Flucht) | POL | `CQ-71` |
| COMBAT-DEC-FF-014 | **Dauerhafte Einrichtungen des Spielers (Netzwerk, Ausbau, Bunker) sind nicht entschieden** | ❓ OFFEN | `K-CO-009`; `48#131E` | KRE PER | `CQ-72` |
| COMBAT-DEC-FF-015 | **Rechtlicher Rahmen des Festhaltens** (Freiheitsberaubung, Besessene) ist nicht entschieden | ❓ OFFEN | `K-CO-010` | POL | `CQ-72` |

---

## KI: Gegner-KI und Kampftaktik

Gruppe zu [Modul 18](18-Gegner-KI-und-Kampftaktik.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-KI-001 | **Wahrnehmung mit Unsicherheit statt magischer Spielerkoordinaten;** ein Geräusch liefert Richtung, Entfernung, Unsicherheit, Lautstärke, Art und Zeit, keine Position | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#239A` §1 (Entwurf); `23#240E` | KI AUD | Modell: Abschnitt 3 |
| COMBAT-DEC-KI-002 | **Vier Wissensgrade** (bestätigt, starker Verdacht, schwacher Verdacht, unbekannt) für Position, Verletzung, Waffen, Fluchtwege, Ablenkungen, Begleiter, Verstecke | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#242E`; `23#253A` P1; `K-KI-003` | KI | – |
| COMBAT-DEC-KI-003 | **Adaptive Taktiken nutzen nur Beobachtetes oder plausibel Gefolgertes;** keine globale Anti-Spieler-Telemetrie, kein verborgenes Metawissen (‚KI darf intelligent schließen, aber nicht cheaten‘ als Formulierung des Assistenten) | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#242E`; `K-KI-013` | KI | – |
| COMBAT-DEC-KI-004 | **Wahrnehmungszustände** mindestens unaware, suspicious, investigating, alert, confirmed target, searching | 🧭 RICHTUNG | `M-PD` (‚may include‘); `COMBAT-DEC-ZA-003` | KI | Schwellen: `CQ-09` |
| COMBAT-DEC-KI-005 | **Die Sicherheit der letzten bekannten Position zerfällt mit der Zeit;** Gegner verlieren Genauigkeit, aber nicht das Wissen, dass eine bestätigte Bedrohung existiert | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` P2; `K-KI-004` | KI | – |
| COMBAT-DEC-KI-006 | **Suchphasen** von sofortiger Verfolgung über lokale und erweiterte Suche und Einkesselung bis zu vorsichtiger Wachsamkeit; Dauer und Intelligenz schwierigkeitsabhängig | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` P3; `23#226E` P11; `K-KI-005` | KI SCH | – |
| COMBAT-DEC-KI-007 | **Kampf kann in Suche und Tarnung zurückfallen,** wenn die Sichtlinie bricht und die genaue Position verloren ist | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` P13; `K-KI-010` | KI | – |
| COMBAT-DEC-KI-008 | **Schall:** Lautstärke, Dauer und Frequenz, Entfernung, Raumakustik, Wände, Türen, Materialien, Hintergrundlärm, Wetter, Böden und Hörvermögen; Kategorien (Schritt, Nachladen, Magazinabwurf, Tür, Glasbruch, Stimme, Schuss, Motor, geworfener Gegenstand) werden verschieden gedeutet | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; Gameplay-Systeme §7 | KI AUD | Raum-/Portalmodell: Audio-Planung |
| COMBAT-DEC-KI-009 | **Sicht:** Richtung, Reichweite, Licht, Bewegung, Deckung, Silhouette, Haltung, Wetter; Dunkelheit verringert die Sicht, ohne Unsichtbarkeit zu gewähren; ein im Dunkeln gehörter Spieler kann grob geortet werden, ohne identifiziert zu sein | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#226E` P7 | KI | – |
| COMBAT-DEC-KI-010 | **Hören und Sehen unterscheiden sich je Art;** Supergehör ist keine Schwierigkeitsmechanik | 📋 BESCHLOSSEN | `23#239A` §5–§6 (Entwurf); `23#240E` P5, P6; `23#226E` P21 (‚realistisch bleiben, nur gefährlicher‘) | KI SCH | – |
| COMBAT-DEC-KI-011 | **Intelligente Gegner kombinieren Spuren** (Geräusch, Blut, Türen, Fenster, letzte Position) zu Hypothesen und prüfen Verstecke auch ohne Gewissheit; Verstecke sind nie garantiert sicher | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#240E` B (Verstecke), P12 (‚er darf einfach so nachschauen, wenn er dich findet, ist es halt blöd‘) | KI | – |
| COMBAT-DEC-KI-012 | **Wiederholte Ablenkungstricks können erkannt werden;** der Gegner wird skeptisch, ignoriert, untersucht anders oder vermutet eine Falle | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#241A` | KI | – |
| COMBAT-DEC-KI-013 | **Menschliche Gegner teilen Information nur über plausible Kommunikation** (Stimme, Funk, Sichtzeichen); kein Schwarmbewusstsein; Kreaturen kommunizieren artspezifisch; wer jemanden vor dem Ruf ausschaltet, hält die anderen unwissend | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` §4–§5 (Entwurf) | KI NPC | – |
| COMBAT-DEC-KI-014 | **Blutspuren können Menschen visuell verfolgen, wo plausibel;** Geruchsverfolgung ist kreaturspezifisch | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘ | KI | – |
| COMBAT-DEC-KI-015 | **Taktikvorrat intelligenter Monster:** beobachten, auflauern, flankieren, Strom abschalten, Licht manipulieren, falsche Geräusche und Stimmen, absichtlich schweigen, Ausgänge blockieren, Jäger isolieren, verletzte Ziele verfolgen, Begleiter angreifen, zurückziehen, Nachladen und Verletzungen ausnutzen; nicht jede Kreatur kann alles | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` §6; `23#254E` P6 (‚kommt aufs Monster an‘); `K-KI-006` | KI KRE | – |
| COMBAT-DEC-KI-016 | **Die Taktikwahl bewertet die Lage** (Verletzung, Gruppe, Distanzvorteil, Strom, Verhalten des Spielers) und passt sich an; keine Zufallsrotation, mit etwas Variation | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` §7; `23#254E` P7 (‚die Monster sollen sich anhand von meinem Verhalten schlau anpassen‘) | KI | Bewertungsmodell: Abschnitt 5 |
| COMBAT-DEC-KI-017 | **Absichtliches Schweigen ist nur eine Taktik unter vielen** und nicht so häufig, dass sie vorhersehbar wird | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#240E` C; `K-KI-008` | KI | – |
| COMBAT-DEC-KI-018 | **Fake-Geräusche** unterscheiden Weltgeräusch, vom Monster erzeugtes physisches Geräusch und rein übernatürliche Wahrnehmung nur des Ziels | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `K-KI-009` | KI AUD | – |
| COMBAT-DEC-KI-019 | **Taschenlampen und Lichtquellen sind Werkzeuge und Risiken;** kreaturspezifische KI kann Licht und Strom manipulieren; manche Wesen sehen Licht nicht, andere reagieren stark | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#225A` §8–§9 (Entwurf) | KI WAF | – |
| COMBAT-DEC-KI-020 | **Mehrere Gegner üben gleichzeitig Druck aus** (flankieren, greifen, Fluchtweg abschneiden) und nutzen Gelegenheiten konsequenter, besonders auf Grounded | 📋 BESCHLOSSEN | `COMBAT-DEC-NK-018`, `-044` | KI | – |
| COMBAT-DEC-KI-021 | **Menschliche Gegner können fliehen, erstarren oder sich ergeben,** abhängig von Persönlichkeit, Kampferfahrung, Verletzung, Bewaffnung, Verbündeten, Gesehenem und der Haltung des Spielers; Furcht und Moral bleiben intern, ohne sichtbaren Balken | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#223A` (Entwurf der Faktoren); `23#222E` Zusatz | KI NPC | Schwellen: `CQ-73` |
| COMBAT-DEC-KI-022 | **Auf höheren Schwierigkeitsgraden sind Gegner aufmerksamer und gnadenlos;** sie hören Nachladen in der Nähe | 📋 BESCHLOSSEN | `23#86E` P1; `M-PD` ‚Difficulty…‘; `K-KI-018` | KI SCH | – |
| COMBAT-DEC-KI-023 | **Grounded verbessert Reaktionszeit, Aufmerksamkeit, Interpretation, Erinnerung, Suchlogik und Bereitschaft nachzugehen,** nur moderat die Wahrnehmungsschwellen (Entwurf des Assistenten, vom Entwickler mit ‚realistisch bleiben, nur gefährlicher‘ aufgenommen) | 🧭 RICHTUNG | `23#239A` §6; `23#240E` P6; `23#226E` P21 | KI SCH | – |
| COMBAT-DEC-KI-024 | **Ein KI-Inspektor zeigt Wissen, Verdacht, letzte Position, Hörschätzung, Suchphase, aktuelle Taktik und Begründung** | 📋 BESCHLOSSEN | Gameplay-Systeme §7 (Entwicklerwerkzeuge); `K-KI-012`; `23#254E` P16 | TST | – |
| COMBAT-DEC-KI-025 | **Begleiter handeln selbst und stimmen sich mit Plan und Befehl ab;** sie dürfen abweichen, wenn Gefahr die Anweisung unsicher macht; ihr Stealth versteht Absicht, Tempo, Licht, Feinde, Schusslinien und Lärmrisiko | 📋 BESCHLOSSEN | `M-PD` ‚Companion autonomy…‘; `23#254E` P17; `N-CO-010`, `N-CO-011` | NPC KI | – |
| COMBAT-DEC-KI-026 | **Friendly Fire trifft auch Gegner untereinander;** Gegner achten auf ihre Schusslinie, auf Grounded besser | 🧭 RICHTUNG | `23#221A` §3 (Entwurf); `23#222E` P3 (‚passt‘) | KI | – |
| COMBAT-DEC-KI-027 | **Bedrohungsbewertung als Modell** (Zahl und Ausrüstung des Spielers, Verletzungen, Gelände, eigene Verluste, Rückzugswege) für den Rückzug und Wiederkehr | 💡 VORSCHLAG | Abschnitt 5; Anwendung von `N-NL-001`, `K-KI-006` | KI | Werte offen |
| COMBAT-DEC-KI-028 | **Rückzug, Verstecken, Hinterhalt und Jagd auf den Spieler** sind Verhalten intelligenter Wesen | 📋 BESCHLOSSEN | `K-KI-017` | KI | – |
| COMBAT-DEC-KI-029 | **Lernen je Individuum, Gruppe oder Art** und Gedächtnisgrenzen je Art sind nicht festgelegt | ❓ OFFEN | `K-KI-019`, `K-KI-020` | KI NPC | `CQ-74` |
| COMBAT-DEC-KI-030 | **Schwellen von Furcht, Aufgabe und Flucht,** Gewichtung der Faktoren und Auslöser sind nicht festgelegt | ❓ OFFEN | `M-OD`; `23#223A` (Faktoren nur genannt) | KI NPC | `CQ-73` |
| COMBAT-DEC-KI-031 | **Deckungsnutzung der Gegner** (Anlehnen, Wechsel, Blindfeuer) ist nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-FK-031` | KI | `CQ-17` |
| COMBAT-DEC-KI-032 | **Leistungsgrenzen der KI** (gleichzeitig aktive Gegner, Wahrnehmungstraces je Tick) sind nicht entschieden | ❓ OFFEN | `N-SI-017`; Modul 25 | KI PRF | `CQ-75` |
| COMBAT-DEC-KI-033 | **Verhalten von Gegnern gegenüber Begleitern, Tieren und Zivilisten im Kampf** (Ziele, Geiseln, Flucht von Unbeteiligten) ist nicht beschrieben | ❓ OFFEN | `E-RE-005` (NPCs reagieren auf Verbrechen); Gameplay-Systeme §10 | KI NPC | `CQ-76` |

---

## NM: Nemesis, Erinnerung, persistente Kampffolgen

Gruppe zu [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-NM-001 | **Überlebende Gegner können zurückkehren, Feindschaften entwickeln, aus Begegnungen lernen und Taktiken anpassen** | 📋 BESCHLOSSEN | `PE-N1022` C; `N-NE-001` | NPC KI | – |
| COMBAT-DEC-NM-002 | **Intelligente Monster haben ein umfassendes individuelles Gedächtnis** (Begegnungen, Kampferfahrungen mit Waffen, Fallen, Strategien, Beziehungen, Rache, Angst, Informationsaustausch) | 📋 BESCHLOSSEN | `N-NE-002`; `K-KI-014` (`48#139E`) | NPC KRE | Ausformulierung `N-NE-003` ist 💡 |
| COMBAT-DEC-NM-003 | **Ein Gegner, der nur vertrieben wurde oder entkam, kann später mit Erinnerung und früheren Verletzungen auftauchen,** muss aber nicht | 📋 BESCHLOSSEN | `N-NL-003`; `23#294E` (‚es kann auftauchen aber es muss nicht‘ in der Quelle) | NPC | Häufigkeit: `N-NE-022` |
| COMBAT-DEC-NM-004 | **Narben und bleibende Verletzungen überdauern,** wo die Art nicht regeneriert | 📋 BESCHLOSSEN | `K-KO-007`; `N-NE-013`, `N-NL-006` | NPC KRE | – |
| COMBAT-DEC-NM-005 | **Gegner kehren verändert zurück** (Narben, besser vorbereitet, mit Verbündeten, wissend, was gewirkt hat) | 🧭 RICHTUNG | `N-NL-010` (📋 Richtung); `PE-N1022` C | NPC KI | – |
| COMBAT-DEC-NM-006 | **Lernen ist Wissensaufbau, nicht Stärkerwerden;** keine sichtbaren Stufen, kein Wert steigt mit jeder Begegnung | 📋 BESCHLOSSEN | `N-NE-017`; `M-PD` ‚Genre and progression identity‘ | KI SCH | – |
| COMBAT-DEC-NM-007 | **Die Anpassung übernatürlicher Wesen ist durch Artregeln und Schwächen begrenzt,** nie freie Erfindung | 💡 VORSCHLAG | `N-NE-020`; `K-LW-005` | KI KRE | Wie ein Nemesis lernt: `N-NE-022` ❓ |
| COMBAT-DEC-NM-008 | **Hunter-NPCs können Rivalen werden;** sie haben Ruf, bearbeiten Fälle vor dem Spieler, behandeln sie falsch, konkurrieren oder kooperieren | 📋 BESCHLOSSEN | `N-NE-015`; `N-BH-013` | NPC | – |
| COMBAT-DEC-NM-009 | **Ein verschontes friedliches Wesen wird nicht automatisch zum Feind;** ein anderer Hunter kann es töten | 📋 BESCHLOSSEN | `N-NE-016`; `K-GS-004` | NPC KRE | – |
| COMBAT-DEC-NM-010 | **Rivalen können im Fight Club entstehen** (Beziehungen, Trainer, Reputation, Hunter-Kontakte) | 💡 VORSCHLAG | `N-NE-021`; `23#164A`, `23#166A` | NPC | `CQ-13` |
| COMBAT-DEC-NM-011 | **Kriminelle Gruppen können wissen, wer der Spieler ist;** wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | 📋 BESCHLOSSEN | `N-NE-018`; `N-BH-007` | NPC POL | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-NM-012 | **Normale NPCs können verletzt werden und genesen;** schwere Verletzungen haben bei wichtigen NPCs dauerhafte Folgen; Krankenhaus und Rettung sind Teil der Welt | 📋 BESCHLOSSEN | `N-LB-012`, `N-NL-007` | NPC MED | – |
| COMBAT-DEC-NM-013 | **Spieler und menschliche NPCs teilen dieselbe Trauma-Regel;** kein Plot Armor | 📋 BESCHLOSSEN | `N-LB-013`; `COMBAT-DEC-GP-007` | NPC MED | Spannung: `CK-02` |
| COMBAT-DEC-NM-014 | **Wichtige NPCs sterben nicht durch banale Simulationsfehler;** Kernfiguren des Hunter-HQ haben keinen zufälligen Sandbox-Tod, ein geschriebener Story-Tod bleibt | 📋 BESCHLOSSEN | `N-LB-017`; `N-CO-007`; `M-CA0905` (Z. 44) | NPC | Wer sie töten darf: NPC-Konflikt C-N1, `CK-02` |
| COMBAT-DEC-NM-015 | **Normale und rekrutierbare Begleiter können dauerhaft sterben** | 📋 BESCHLOSSEN | `N-CO-007`; `M-CA0905` | NPC | – |
| COMBAT-DEC-NM-016 | **Der Tod eines NPCs bleibt dauerhaft im Weltzustand;** Identität und Historie bleiben, nur unnötiger technischer Detailzustand darf verdichtet werden | 📋 BESCHLOSSEN | `N-LB-018`, `N-LB-016` | PER NPC | – |
| COMBAT-DEC-NM-017 | **Wer dem Spieler relevant begegnet (Beziehung, Story, bedeutende Begegnung), steigt in eine höhere Persistenzstufe auf** | 📋 BESCHLOSSEN | `N-ID-015` | PER NPC | Kampf kann Aufstieg auslösen: Abschnitt 3 |
| COMBAT-DEC-NM-018 | **Ein Kampf meldet Ereignisse an das Gedächtnis,** keine Direktschreibzugriffe | 💡 VORSCHLAG | `COMBAT-DEC-ZA-013` | NPC | Abschnitt 3 |
| COMBAT-DEC-NM-019 | **Rückkehrhäufigkeit, Lernmodell (Regeln oder Daten) und was der Spieler von der Feindschaft sieht** sind offen | ❓ OFFEN | `N-NE-022`; `K-LW-009` | NPC KI | `CQ-78` |
| COMBAT-DEC-NM-020 | **Wer wichtige NPCs töten darf (Spieler, Fraktionen, Hunter, Wesen)** ist nicht entschieden | ❓ OFFEN | NPC-Konflikt C-N1; `N-LB-017` | NPC | `CQ-79` |
| COMBAT-DEC-NM-021 | **Ruhm und Bekanntheit durch Kämpfe** (Medien, Social Media) sind selten, aber wirksam; Häufigkeit offen | 🧭 RICHTUNG | NPC-Konflikt C-N8; `23#1057E` P45 | NPC POL | `CQ-80` |

---

## PO: Fraktionen, Polizei, Kampfkonsequenzen

Gruppe zu [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-PO-001 | **Das Gesetz folgt tatsächlicher Information und Beweiskonfidenz über Zuständigkeiten hinweg** (lokal, Bezirk, Bundesstaat, Bund), nicht omniszienter Fahndungslogik | 📋 BESCHLOSSEN | `E-RE-001`; `M-PD` ‚Cash / law / evidence‘; Gameplay-Systeme §10 | POL ERM | – |
| COMBAT-DEC-PO-002 | **Beweisquellen:** Zeugen, Überwachungskameras, Kleidung, Gesicht, Fahrzeug und Kennzeichen, Waffen und Ballistik, Fingerabdrücke, DNA wo passend, digitale Beweise | 📋 BESCHLOSSEN | `E-RE-002`; `M-PD` | POL ERM | Katalog der Spuren: [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) §3 |
| COMBAT-DEC-PO-003 | **Wichtige abgelegte Waffen und Gegenstände bleiben physisch und können Beweise werden;** eine intelligente Bereinigung verhindert unbegrenzten Müll | 📋 BESCHLOSSEN | `E-RE-024`; `E-QA-023`; `M-PD` | PER POL | Bereinigungsregel offen |
| COMBAT-DEC-PO-004 | **Tatorte bleiben bis zur Reinigung oder Reparatur bestehen** | 📋 BESCHLOSSEN | `E-SP-013` | PER ERM | – |
| COMBAT-DEC-PO-005 | **Zeugenaussagen sind keine garantierte Wahrheit;** NPCs wissen nur, was sie erfahren haben; Spieler, Fahrzeug und Kleidung können wiedererkannt werden | 📋 BESCHLOSSEN | `E-SP-010`, `E-SP-011`; `E-ZE-002`, `E-ZE-003` | NPC ERM | – |
| COMBAT-DEC-PO-006 | **Der Spieler kann Zeuge werden** (schweigen, melden, bestochen oder bedroht werden) | 📋 BESCHLOSSEN | `E-RE-005` | NPC POL | – |
| COMBAT-DEC-PO-007 | **Notruf mit Latenz, Täterbeschreibung, Kennzeichenabfrage;** Polizei reagiert auch ohne den Spieler | 📋 BESCHLOSSEN | `E-RE-010`; `N-BH-009` | POL NPC | – |
| COMBAT-DEC-PO-008 | **Schwere Fahndung bleibt bis zur glaubwürdigen Auflösung;** man kann landesweit gesucht werden; kein Sternesystem; Kopfgeld als Bedingung für das Ende | 📋 BESCHLOSSEN | `E-RE-012`, `E-RE-013`, `E-PO-007` | POL | Widerspruch Quiz-Text: `E-PO-003` ⚠️ |
| COMBAT-DEC-PO-009 | **Festnahme ist kein automatisches Game Over;** Strafkette von Tat über Schweregrad, Vorsatz, Opfer, Zeugen und Beweise, Notwehr, Vorstrafen, Festnahme, Staatsanwaltschaft, Prozess oder Absprache, Urteil | 📋 BESCHLOSSEN | `E-RE-014`, `E-RE-015`; Gameplay-Systeme §10 | POL | [Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md) |
| COMBAT-DEC-PO-010 | **Aufgeben und Flucht sind Wege bei Kontakt mit der Polizei;** Festnahme statt Flucht ist ein Vorschlag, nicht widersprochen | 🧭 RICHTUNG | Gameplay-Systeme §10; `E-QA-021` | POL | – |
| COMBAT-DEC-PO-011 | **Selbstverteidigung ist unabhängig vom Geschlecht;** Handlungen werden bewertet | 📋 BESCHLOSSEN | `E-RE-018` (`35#533A`, `35#532E`); Gameplay-Systeme §10 | POL | Bewertungsregeln offen (`CQ-82`) |
| COMBAT-DEC-PO-012 | **NPCs, die ein schweres Verbrechen beobachten, können Polizei rufen, eingreifen, festhalten, fliehen, angreifen oder massiv eskalieren,** je nach Persönlichkeit, Fraktion und Moral | 📋 BESCHLOSSEN | Gameplay-Systeme §10; `E-RE-019` | NPC KI | – |
| COMBAT-DEC-PO-013 | **Die Polizei ist nicht allwissend;** bei Verdacht kann ein Verhör des Spielers erfolgen | 📋 BESCHLOSSEN | `N-BH-002`; `E-ZE-009` | POL | – |
| COMBAT-DEC-PO-014 | **Die Polizei erklärt Vorfälle zunächst normal;** die meisten Polizisten wissen nichts vom Übernatürlichen; kleine geheime Einheiten kennen Teile | 📋 BESCHLOSSEN | `E-RE-022`; `N-BH-008` | POL KRE | Folge: Wesen- und Monsterkämpfe haben „normale“ Erklärungen |
| COMBAT-DEC-PO-015 | **Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen;** bestimmte kriminelle Gruppen können wissen, wer der Spieler ist | 📋 BESCHLOSSEN | `N-NE-018`; `N-BH-007` | POL NPC | – |
| COMBAT-DEC-PO-016 | **Kriminalität läuft unabhängig vom Spieler;** Gangkonflikte, Festnahmen, Absperrungen nach größeren Verbrechen | 📋 BESCHLOSSEN | `E-RE-003`; `E-PO-008`; `N-TE-007` | NPC POL | – |
| COMBAT-DEC-PO-017 | **Fight-Club-Verstöße** (absichtliches Töten, schwere vermeidbare Verletzung, Weiterwürgen) haben ernste soziale, rechtliche und Spielfolgen | 📋 BESCHLOSSEN | `COMBAT-DEC-NK-013`; `M-PD` ‚Melee / grappling‘ | POL NPC | Details 💡 (`COMBAT-DEC-NK-014`) |
| COMBAT-DEC-PO-018 | **Schwere Straftaten sind nur Konsequenzsystem** (Zeugen, Spuren, Ermittlung, Festnahme, Prozess, Haft, Ruf), keine eigene Spielmechanik | 📋 BESCHLOSSEN | `E-RE-020`; Gameplay-Systeme §10 | POL | – |
| COMBAT-DEC-PO-019 | **Nur interne Entwicklerbuilds dürfen Crime-Pfade bis zur Haft testen,** nicht die Veröffentlichung | 📋 BESCHLOSSEN | `E-HA-011` | TST | Testkatalog beachten |
| COMBAT-DEC-PO-020 | **Wichtige NPCs sterben nur durch den Spieler** (Aussage des Entwicklers zur Welt); Spannung zur unabhängigen Lebensentwicklung und zu Fraktionskämpfen | ⚠️ KONFLIKT | `E-ZE-007`; NPC-Konflikt C-N1; `COMBAT-DEC-GP-007` | NPC | `CK-02`, `CQ-79` |
| COMBAT-DEC-PO-021 | **Fraktionen, Wesen und Hunter können Rache nehmen** (Gedächtnis, Rufweitergabe) | 🧭 RICHTUNG | `N-NE-002`, `N-NE-018`; `PE-N1022` C, D | NPC KI | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-PO-022 | **Kampfspuren als Katalog** (Blut, Hülsen, Einschüsse, Waffen, Kleidungsschäden, Kampfspuren am Körper, Zeugen, Kameras) | 💡 VORSCHLAG | Abschnitt 3 (Ableitung aus `E-SP-002`, `E-SP-007`, `E-RE-002`) | ERM | – |
| COMBAT-DEC-PO-023 | **Straftatenkatalog, Festnahmeschwellen, Strafmaß, Kaution, Zuständigkeitskarte, Überwachungsdichte, Beweiswertung, Durchsuchungsregeln** sind nicht entschieden | ❓ OFFEN | `M-OD` ‚Crime / law / evidence‘; Gameplay-Systeme §10 (letzte Zeile) | POL | `CQ-83` |
| COMBAT-DEC-PO-024 | **Waffenbesitz, offenes Tragen, illegale Waffen und die Meldung von Schussverletzungen** im Recht sind nicht entschieden | ❓ OFFEN | `23#96A` (Aufgabe ‚Waffen offen tragen‘); `M-OD` | POL WAF | `CQ-81` |
| COMBAT-DEC-PO-025 | **Polizeireaktion auf Gewalt im öffentlichen Raum** (Eskalationsstufen, Kontaktregeln, tödliche Gewalt der Polizei gegenüber dem Spieler) ist nicht beschrieben; beschlossen ist nur, dass Festnahme und Aufgabe Alternativen zu tödlicher Polizeilösung sind | ❓ OFFEN | `M-PD` ‚Cash / law / evidence‘ (arrest/jail/surrender); `E-EN-027` (💡 Eskalationsstufen) | POL KI | `CQ-84` |
| COMBAT-DEC-PO-026 | **Verhältnis zwischen Monsterkampf und öffentlichem Wissen** (Videos, Viralität, Geheimhaltung; kein Gedächtnislöschen) ist offen entschieden bis auf Grundsätze | 🧭 RICHTUNG | `K-HU-008`; Kreaturen 11 | POL KRE | `CQ-85` |
| COMBAT-DEC-PO-027 | **Gefangene Menschen** (Festsetzen durch den Spieler) und die Folgen sind nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-FF-013` | POL | `CQ-86` |

---

## DE: Kampfvermeidung, Flucht, Deeskalation

Gruppe zu [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-DE-001 | **Nicht jede Begegnung mit einem Wesen ist ein Kampf;** Beobachten, Verfolgen, Belauschen, Fotografieren und Proben sind gleichwertige Zugänge | 📋 BESCHLOSSEN | `K-GR-006`; `E-QA-009`; `COMBAT-DEC-GP-019` | KRE ERM | – |
| COMBAT-DEC-DE-002 | **Ausgänge einer Begegnung:** töten, vertreiben, bannen, fangen, heilen, Fluch brechen, retten, verhandeln, überleben, evakuieren | 📋 BESCHLOSSEN | `E-FA-009`; `K-GR-007`; `N-NL-004` | ERM KRE | – |
| COMBAT-DEC-DE-003 | **Nicht jedes Wesen ist böse;** ein friedliches Wesen lebt normal weiter, und ein anderer Hunter darf es töten | 📋 BESCHLOSSEN | `K-GR-007`; `K-GS-004` | KRE NPC | – |
| COMBAT-DEC-DE-004 | **Nicht jeder Dämon greift sofort an;** Verhandlung ist möglich | 📋 BESCHLOSSEN | `K-HW-004` | KRE | Mechanik: `CQ-90` |
| COMBAT-DEC-DE-005 | **Menschliche Gegner können fliehen, erstarren oder sich ergeben,** wenn sie genug eingeschüchtert sind; kein sichtbarer Furchtbalken | 📋 BESCHLOSSEN | `COMBAT-DEC-NK-017`; `M-PD` | KI | Schwellen: `CQ-73` |
| COMBAT-DEC-DE-006 | **Ein Spieler kann eine bedrohliche Lage durch Tarnung und Ablenkung vermeiden;** der Kampf kann zur Suche zurückkehren, Verstecke sind nie garantiert sicher | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `COMBAT-DEC-KI-007`, `-011` | KI | – |
| COMBAT-DEC-DE-007 | **Geworfene Gegenstände erschrecken oder lenken ab und erzeugen Aufmerksamkeit** | 📋 BESCHLOSSEN | `23#221A` P5 (Beispiel); `23#222E` P5; `COMBAT-DEC-NK-021` | KI WAF | – |
| COMBAT-DEC-DE-008 | **Stealth-Takedowns sind kein Sofort-Tod;** bei Monstern artabhängig | 📋 BESCHLOSSEN | `COMBAT-DEC-NK-041` | KI KRE | – |
| COMBAT-DEC-DE-009 | **Der Spieler kann fliehen;** bei schwerer Verletzung kann er sich aus der Gefahr kämpfen, einen Freund anrufen und gerettet werden | 🧭 RICHTUNG | Spielvision §14.2; `23#88E` G; `23#63A` | BEW NPC | Mechanik: `CQ-88` |
| COMBAT-DEC-DE-010 | **Gegner können den Spieler verfolgen;** ein verletzter Spieler ist leichter zu verfolgen (Hinken, Atmung, Blutspur); manche Wesen verfolgen gezielt verletzte Jäger | 📋 BESCHLOSSEN | `COMBAT-DEC-VL-017`; `K-KI-017`; `M-PD` ‚Stealth…‘ | KI | – |
| COMBAT-DEC-DE-011 | **Eine Einsatzplanung mit Begleitern kann Rückzugspunkt, Treffpunkt bei Kontaktverlust, Verhalten bei Verletzung und Entdeckung und die Entscheidung ‚Gefangennahme statt Töten‘ enthalten** | 🧭 RICHTUNG | `23#255A` (Liste, Entwurf); `23#256E` (‚das passt so gut‘); `M-PD` ‚Companion autonomy…‘ (escape/retreat logic) | NPC | – |
| COMBAT-DEC-DE-012 | **Nichttödliche Ausschaltung:** Aufgabegriffe können Bewusstlosigkeit ohne Tod erzeugen; Fesseln, Betäubung und Festnahme durch den Spieler sind nicht beschrieben | 🧭 RICHTUNG | `COMBAT-DEC-NK-011`, `COMBAT-DEC-FF-012` | MED POL | `CQ-14`, `CQ-71` |
| COMBAT-DEC-DE-013 | **Fluchtmechanik** (Sprint, Ausdauer, Türen schließen, Hindernisse, Fahrzeug, Verlust von Verfolgern, Untertauchen) ist nicht beschrieben | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | BEW KI | `CQ-88` |
| COMBAT-DEC-DE-014 | **Soziale Deeskalation mit Menschen** (Drohen, Überzeugen, Beruhigen, Lügen) im Konflikt ist nicht beschrieben; das Dialogsystem startet klassisch | ❓ OFFEN | `E-ZE-012`; `E-ZE-013` | NPC | `CQ-89` |
| COMBAT-DEC-DE-015 | **Kampfvermeidung und Fortschritt:** ob ein Fall ohne Kampf gelöst werden kann und wie das belohnt wird, ist teils beschlossen (mehrere Lösungswege) und nicht auf Kampf bezogen entschieden | 🧭 RICHTUNG | `E-QA-005`; `M-PD` ‚Story and cases‘ | ERM | `CQ-91` |
| COMBAT-DEC-DE-016 | **Grounded macht Flucht und Verstecken schwerer** (Gegner suchen länger, besser, hören Nachladen), ohne Wandblick | 🧭 RICHTUNG | `COMBAT-DEC-KI-006`, `-022`, `-023` | KI SCH | – |
| COMBAT-DEC-DE-017 | **Festnahme statt Flucht** bei Kontakt mit der Polizei ist ein Vorschlag, nicht widersprochen | 🧭 RICHTUNG | `E-QA-021`; Gameplay-Systeme §10 | POL | – |

---

## AN: Animationen, Trefferreaktionen, Ragdoll

Gruppe zu [Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-AN-001 | **Komplexe Verletzungsanimation (Schichten, IK, prozedurale Anpassung) ist vor der Veröffentlichung nötig,** auch wenn frühe Builds einfacher sind | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; Gameplay-Systeme §4 | ANI | – |
| COMBAT-DEC-AN-002 | **Komplexe Verletzungsanimation, Ganzkörper-IK, Motion Warping, prozedurale Anpassung und Gesichtsanimation** (Augen, Blick, Brauen, Wangen, Kopf, Atmung, Mikroausdruck) | 📋 BESCHLOSSEN | Gameplay-Systeme §14.3 | ANI | – |
| COMBAT-DEC-AN-003 | **Verletzungen wirken in Schichten:** Basisbewegung plus Beinverletzung (Hinken) plus Armverletzung plus Schmerz und Atmung (Beispiel des Assistenten); der Entwickler: ‚das wird nicht leicht‘ | 🧭 RICHTUNG | `23#103A` P17 (Entwurf); `23#101E` P17; `23#104E` P17 | ANI MED | Parameter: Abschnitt 3 |
| COMBAT-DEC-AN-004 | **Animation und tatsächliche Bewegung müssen übereinstimmen** | 📋 BESCHLOSSEN | Proposal 0009 §3a | ANI BEW | – |
| COMBAT-DEC-AN-005 | **Waffenbedienung ist mechanisch korrekt animiert** (Pumpgun, Pistole, Revolver) | 📋 BESCHLOSSEN | `COMBAT-DEC-FK-011` | ANI WAF | – |
| COMBAT-DEC-AN-006 | **Verletzte Gegner zeigen Folgen** (Arm schützen, humpeln, das Bein halten) in späteren Versionen | 📋 BESCHLOSSEN | `23#86E` P3; `COMBAT-DEC-TK-015` | ANI KI | – |
| COMBAT-DEC-AN-007 | **Sichtbare Wunden, offene Brüche, beschädigte Kleidung und Blut sind am Spieler und an Gegnern darstellbar** | 📋 BESCHLOSSEN | `COMBAT-DEC-TK-007`, `-008`; `M-PD` ‚Human anatomy…‘ | ANI TRF | Technik: Gameplay-Systeme §14.3 |
| COMBAT-DEC-AN-008 | **Gore-Einstellungen voll, reduziert, minimal** und einstellbare Kill-Cams | 📋 BESCHLOSSEN | Gameplay-Systeme §4 (Darstellung) | UI ANI | Schwellen: `CQ-47` |
| COMBAT-DEC-AN-009 | **Gemeinsame Sekundärphysik** für Rückstoß, Laufen, Kampf, Fahrzeug, Stürze, Tragen, Haar, Kleidung; realistische Dämpfung und Masse | 📋 BESCHLOSSEN | `COMBAT-DEC-TK-013` | ANI | Technik offen (`CQ-48`) |
| COMBAT-DEC-AN-010 | **Gemeinsames Interaktions-Framework** mit Teilnehmern, Sitzungen, stabilen IDs, Anfragen, Reservierungen, Annäherung, Ausrichtung, Motion Warping, IK, Synchronisation, Übergängen, Netzwerk und Persistenz; kein Monolith | 📋 BESCHLOSSEN | Gameplay-Systeme §3.2 | ANI KOO | Nutzung für Griffe: `CQ-08` |
| COMBAT-DEC-AN-011 | **Manuelle QA bleibt für Gefühl, Animation, Audio und Bildqualität wichtig;** der Entwickler ist der endgültige Entscheider für Subjektives | 📋 BESCHLOSSEN | Gameplay-Systeme §17.2, §17.3 | TST | – |
| COMBAT-DEC-AN-012 | **Zustandsparameter statt Animationsaufrufe:** Der portable Kern liefert Parameter (Hinkfaktor, Armschutz, Atemrate, Zittern, Haltung), die Animation interpretiert; so bleiben Kern und Engine getrennt | 💡 VORSCHLAG | Abschnitt 3; `COMBAT-DEC-ZA-009` | ANI | – |
| COMBAT-DEC-AN-013 | **Mindestmenge einer Paaranimations-Bibliothek und ein Plan gegen die Kombinationsexplosion** (Positionen mal Techniken mal Körpergrößen) | 💡 VORSCHLAG | `M-P14` (Themeninventar ‚animation-combination-explosion-mitigation-plan‘); `23#171E` (Position bestimmt Aktion) | ANI | `CQ-87` |
| COMBAT-DEC-AN-014 | **Vollständigkeitsprüfung der Verletzungsanimationen vor dem Release** (Validator) | 💡 VORSCHLAG | `M-P14` (‚pre-release-injury-animation-completeness-validator‘) | TST | – |
| COMBAT-DEC-AN-015 | **Ragdoll, aktive Ragdoll, physikalische Animation, Motion Matching und Anim-Blueprint-Ansatz** sind nicht entschieden | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | ANI | `CQ-87` |
| COMBAT-DEC-AN-016 | **Trefferreaktionen** (Reaktion auf Region und Wucht, Stolpern, Niederreißen) sind nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-TK-022` | ANI | `CQ-92` |
| COMBAT-DEC-AN-017 | **Herkunft der Animationen und Assets** (eigene, gekauft, generiert) ist nicht entschieden | ❓ OFFEN | Proposal 0009 §5 (3); `M-OD` | ANI | `CQ-93` |
| COMBAT-DEC-AN-018 | **Darstellung von Verletzungen in der Egoperspektive** (Hände, Atem, Sichtfeld) und in der Third-Person-Kamera ist nicht beschrieben | ❓ OFFEN | Gameplay-Systeme §2 (Kamera); `M-PD` ‚Human anatomy…‘ (Sicht, Zittern) | ANI UI | `CQ-94` |
| COMBAT-DEC-AN-019 | **Animationskosten im Gedränge und aus der Ferne** (LOD, Gruppen) sind nicht entschieden | ❓ OFFEN | Gameplay-Systeme §14.3; Modul 25 | ANI PRF | `CQ-95` |

---

## KO: Koop, Synchronisierung, Autorität

Gruppe zu [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-KO-001 | **Koop ist die Kampagne für primär 2 bis 4 Spieler;** keine eigene Koop-Geschichte nötig; Mods ja, Cheats ja; kein Transfer zur offiziellen Online-Progression | 📋 BESCHLOSSEN | `M-PD` ‚Multiplayer separation‘; `N-KO-003`; Spielvision §13 | KOO | – |
| COMBAT-DEC-KO-002 | **Der Host-Spielstand ist maßgeblich;** Gast-Fortschritt ist an den Host-Spielstand gebunden; die Welt ist die Welt des Hosts; was ein Gast auf seinem eigenen Spielstand hat, bleibt dort | 📋 BESCHLOSSEN | `N-KO-001`, `N-KO-002`; `E-KO-004`; `E-WZ-005`; `CLAUDE.md` §6 | KOO PER | – |
| COMBAT-DEC-KO-003 | **Jeder Koop-Spieler erstellt seinen eigenen Hunter** | 📋 BESCHLOSSEN | `E-KO-006` | KOO | Folge: Körperzustand und Ausrüstung je Spieler |
| COMBAT-DEC-KO-004 | **Netzwerkarchitektur, Replikation, Autorität und Zeitpunkt der Umsetzung sind nicht entschieden** | ❓ OFFEN | Spielvision §13; `E-KO-010`; `N-KO-018`; `CLAUDE.md` §6 | KOO | `CQ-97` |
| COMBAT-DEC-KO-005 | **Der offizielle Online-Modus ist serverautoritativ mit Anti-Cheat, ohne Mods und Cheats** und hat eine gemeinsame persistente Online-Progression je Slot; langfristig 64 bis 100 Spieler pro große Instanz als technisches Ziel, keine Zusage | 🧭 RICHTUNG | `M-PD` ‚Multiplayer separation‘ (keine Mods, keine Cheats); Spielvision §13 (🔭) | KOO PRF | Kampfmodus im Online-Modus: `CQ-96` |
| COMBAT-DEC-KO-006 | **Eine Verhaftung ist individuell;** die Welt läuft weiter; ein Welt-Zeitsprung im Koop nur mit Zustimmung aller aktiven Spieler | 📋 BESCHLOSSEN | `N-KO-008`; `E-HA-004` | KOO POL | – |
| COMBAT-DEC-KO-007 | **Im Koop ist das Krankenhaus überspringbar** (Ausfall wenige Minuten bis ca. 15 Minuten); es gibt einen klassischen Modus; ein detailliertes Rettungssystem mit vereinfachter Wiederbelebung als Option | 📋 BESCHLOSSEN | `E-KO-007`, `E-KO-008` | KOO MED | `COMBAT-DEC-BW-025`; `CQ-62` |
| COMBAT-DEC-KO-008 | **Friendly Fire gegen Begleiter ist konfigurierbar;** gegenüber anderen Spielern im Koop ist es nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-GP-023`; Suche am 23.10.2026 (keine Fundstelle) | KOO KI | `CQ-96` |
| COMBAT-DEC-KO-009 | **Mods und Cheats sind im Koop erlaubt;** Mods sind pro Spielstand ein- und ausschaltbar; kritisch fehlende Mods blockieren das Laden | 📋 BESCHLOSSEN | `K-PD-004`, `K-PD-005`; `N-KO-013` | MOD KOO | Mod-Synchronisierung im Koop: `CQ-99` |
| COMBAT-DEC-KO-010 | **Die Simulation läuft nur auf der Autorität;** die Replikation des Simulationszustands ist offen; `WorldFacts` hat noch kein Compare-and-Set | ❓ OFFEN | `N-KO-018`; Gameplay-Systeme §16 (letzte Zeile) | KOO PER | `CQ-97` |
| COMBAT-DEC-KO-011 | **Bewegung bleibt lokal und autoritativ je Spieler** und hat keinen Weltzustands-Einfluss (für Traversal) | 💡 VORSCHLAG | Proposal 0009 §4 | KOO BEW | – |
| COMBAT-DEC-KO-012 | **Körperzustand, Ausrüstung und Wundeinträge gehören dem Spieler,** aber im Host-Spielstand gespeichert | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-KO-002`, `COMBAT-DEC-KO-003`, `COMBAT-DEC-ZA-013` | KOO PER | – |
| COMBAT-DEC-KO-013 | **Fallen, abgelegte Waffen, Tatorte und tote Körper sind Weltzustand des Hosts,** nicht Eigentum eines Spielers | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-KO-002`, `COMBAT-DEC-FF-004`, `COMBAT-DEC-WA-007` | KOO PER | – |
| COMBAT-DEC-KO-014 | **Autoritätsaufteilung für den Kampf** (Eingabe lokal, Körperzustand autoritativ, Projektile autoritativ mit Vorschau, Animation lokal aus Zustand, KI nur Autorität) | 💡 VORSCHLAG | Abschnitt 3 | KOO | – |
| COMBAT-DEC-KO-015 | **Netzwerkereignisse sind idempotent** (eindeutige Projektil-, Magazin-, Wund-IDs, damit Wiederholung nichts doppelt erzeugt) | 💡 VORSCHLAG | Abschnitt 4; `COMBAT-DEC-ZA-016` | KOO | – |
| COMBAT-DEC-KO-016 | **Treffer-Autorität und Latenzausgleich** (wer entscheidet einen Treffer, wie wird Verzögerung ausgeglichen) | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | KOO | `CQ-97` |
| COMBAT-DEC-KO-017 | **Gedächtnis von Gegnern gegenüber mehreren Spielern** (wen merken sie sich, wer wird Ziel) | ❓ OFFEN | `COMBAT-DEC-NM-002`; Suche (keine Fundstelle) | KI NPC | `CQ-98` |
| COMBAT-DEC-KO-018 | **Verbindungsabbruch, Beitritt und Verlassen im Kampf** (Körper, Ausrüstung, Zustand des Gastes) | ❓ OFFEN | Suche (keine Fundstelle) | KOO | `CQ-99` |
| COMBAT-DEC-KO-019 | **Speichersperre im aktiven Kampf bei mehreren Spielern** (gilt für alle, wenn einer entdeckt ist) | ❓ OFFEN | `COMBAT-DEC-GP-028`; Suche (keine Fundstelle) | PER KOO | `CQ-100` |
| COMBAT-DEC-KO-020 | **Schwierigkeit und Hilfen im Koop** (pro Spieler oder durch Host; Zielhilfe, Anzeige, Ressourcen) | ❓ OFFEN | `COMBAT-DEC-GP-010`; Suche (keine Fundstelle) | SCH KOO | `CQ-101` |
| COMBAT-DEC-KO-021 | **Kampfmodus im Online-Modus:** Spielergegen-Spieler, ‚Feindseligkeits-/Verteidigungsmodus‘ und Kopfgelder als mögliche Elemente; weniger Griefing | 🧭 RICHTUNG | Spielvision §13 (Online-Zeile, 🔭 / ❓) | KOO | `CQ-96` |

---

## SP: Speicherstände und Weltpersistenz

Gruppe zu [Modul 24](24-Speicherstaende-und-Weltpersistenz.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-SP-001 | **Ein zentraler persistenter Weltzustand ist das ‚Gedächtnis der Welt'** (Fallergebnisse, NPC lebt oder tot, Beziehungen, veränderte Orte, Wissen, Monsteraktivität, Ruf, Fahrzeuge, Unterschlupfe, Weltereignisse, Zeit) mit stabilen IDs | 📋 BESCHLOSSEN | `E-WZ-001`; `K-PD-001`; Gameplay-Systeme §16 | PER | – |
| COMBAT-DEC-SP-002 | **Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert;** Größenband etwa 20–50 MB bei langer Laufzeit, wo vernünftig; Zuverlässigkeit vor Größe | 📋 BESCHLOSSEN | Gameplay-Systeme §16; `E-WZ-004`; `K-PD-002` | PER | Widerspruch zum Größenziel: NPC-Konflikt C-N2; `CQ-104` |
| COMBAT-DEC-SP-003 | **Der Nachladezustand ist persistent und unterbrechbar;** Patronenlager und Magazine werden getrennt gespeichert | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `COMBAT-DEC-MU-012` | PER WAF | – |
| COMBAT-DEC-SP-004 | **Kein Speichern mitten im aktiven Kampf,** um nicht mit hoher Gesundheit zu speichern und einen bekannten Treffer zurückzusetzen; die Sperre gilt erst, wenn Gegner den Spieler entdeckt haben | 📋 BESCHLOSSEN | `23#366E` P1; `23#368E`; `E-QA-039`; `COMBAT-DEC-GP-028` | PER | Übergang Investigating zu Detected: `CQ-09` |
| COMBAT-DEC-SP-005 | **Autosaves liegen vor und nach Kämpfen, gelegentlich bei längerer Fahrt und in der Natur;** bei wirklich langen Kämpfen kann ein Autosave zwischen den Phasen liegen | 📋 BESCHLOSSEN | `23#368E`; `23#371E`; `COMBAT-DEC-BW-018` | PER | Kriterien der Phasen: `CQ-103` |
| COMBAT-DEC-SP-006 | **Ein Autosave vor einem Kampf entsteht, bevor der Spieler entdeckt wurde;** nach dem Kampf erst, wenn der Bedrohungszustand wirklich beendet ist | 🧭 RICHTUNG | `23#370A` (Auslegung des Assistenten); `23#368E` | PER KI | – |
| COMBAT-DEC-SP-007 | **Nach einem Gameplay-Tod kehrt das Spiel zum letzten geeigneten Checkpoint zurück** mit einem minimal überlebensfähigen Gesundheitszustand; der Spieler kann einen älteren Spielstand laden | 📋 BESCHLOSSEN | `23#364E` P2, P16; `COMBAT-DEC-BW-012`; `23#370A` | PER MED | Widerspruch zu `M-PD`: `CK-01` |
| COMBAT-DEC-SP-008 | **Safe-Checkpoint-Validator** (kein unmittelbarer Angriff, keine tödliche Projektilbahn, kein Griff oder Takedown, keine unmittelbare Explosion, keine nicht reproduzierbare Transition, Encounter-Zustand reproduzierbar) | 💡 VORSCHLAG | `23#367A` | PER TST | – |
| COMBAT-DEC-SP-009 | **Softlock-Schutz:** kein unendlicher Todes-Loop; ein älterer sicherer Checkpoint ohne kostenlose Heilung | 🧭 RICHTUNG | `COMBAT-DEC-BW-016`; `23#365A` | PER | – |
| COMBAT-DEC-SP-010 | **Fallen bleiben persistent** und verändern sich durch Umwelt, NPCs, Monster, Spieler und Zeit | 📋 BESCHLOSSEN | `K-CO-004` | PER KRE | – |
| COMBAT-DEC-SP-011 | **Wichtige abgelegte Waffen, Magazine, Leichen und Tatorte bleiben als Weltobjekte** bis zur Bereinigung | 📋 BESCHLOSSEN | `COMBAT-DEC-WA-007`, `COMBAT-DEC-PO-003`, `-004`; `E-QA-023` | PER | Bereinigungsregel offen |
| COMBAT-DEC-SP-012 | **Individuelle Wesen sind persistent** (Name, Gebiet, Beziehungen, Verletzungen, Feinde, Vergangenheit, Gedächtnis); der Tod eines NPCs bleibt dauerhaft | 📋 BESCHLOSSEN | `K-OE-001`; `N-NE-011`; `N-LB-018` | PER NPC | – |
| COMBAT-DEC-SP-013 | **Körperzustand, Wundeinträge und Behandlungsstand von Spieler und persistenten NPCs bleiben im Spielstand;** nicht jeder Zwischenzustand der Ermüdung | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-VL-005`, `COMBAT-DEC-NM-012`, `COMBAT-DEC-ZA-016` | PER MED | Umfang: `CQ-104` |
| COMBAT-DEC-SP-014 | **Hochfrequenter Kampfzustand (Projektile im Flug, Animationszeit, Trefferdetails) wird nicht im Weltzustandsjournal geführt,** nur langfristig bedeutsame Folgen | 💡 VORSCHLAG | `E-WZ-002`; `K-LW-008`; `COMBAT-DEC-ZA-016` | PER PRF | – |
| COMBAT-DEC-SP-015 | **Zufallszustand und Seeds** werden mitgespeichert, damit Wiederholungen und Fehlerberichte reproduzierbar sind | 💡 VORSCHLAG | Gameplay-Systeme §17.2 (Zufall über Seeds und Replays reproduzierbar); `COMBAT-DEC-ZA-008` | PER TST | `CQ-105` |
| COMBAT-DEC-SP-016 | **Neue Datensatzarten (Mods) werden additiv registriert;** ein Spielstand mit unbekannten Arten lädt unverändert und speichert wieder | 💡 VORSCHLAG | `N-KO-017` (Proposal 0010); `E-WZ-006` | MOD PER | – |
| COMBAT-DEC-SP-017 | **Journal-Kompaktierung, Metadaten, Reichstrennung und Compare-and-Set für Koop** sind nicht entschieden | ❓ OFFEN | Gameplay-Systeme §16; `E-WZ-003`; `N-KO-018` | PER KOO | `CQ-104` |
| COMBAT-DEC-SP-018 | **Permadeath-Speicherstände:** Behandlung von Backups, Zeitpunkt des Löschens, Mehrfachspeicher | ❓ OFFEN | `COMBAT-DEC-BW-013`; Suche (keine Fundstelle) | PER | `CQ-102` |
| COMBAT-DEC-SP-019 | **Was in einem Zwischen-Checkpoint eines langen Kampfes gespeichert wird** (Gegnerzustand, Taktikzustand, Projektile) ist nicht beschrieben | ❓ OFFEN | `23#372A` (Beispiel, Entwurf) | PER KI | `CQ-103` |
| COMBAT-DEC-SP-020 | **Kampf-Replays und Fehlerberichte im Spielstand** (Seed, Eingaben, Zustand) sind gewünscht, aber nicht festgelegt | 🧭 RICHTUNG | Gameplay-Systeme §17.2; `M-PD` ‚Testing philosophy‘ | TST PER | `CQ-105` |

---

## PF: Performance, Skalierung, Simulationsstufen

Gruppe zu [Modul 25](25-Performance-Skalierung-und-Simulationsstufen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| COMBAT-DEC-PF-001 | **Optimierung ist Architekturanforderung von Anfang an;** CPU, GPU, VRAM, RAM, Streaming, KI, Physik, Animation, Audio und Simulation werden gemeinsam budgetiert | 📋 BESCHLOSSEN | Gameplay-Systeme §14.1; `N-SI-015` (`35#785E` P139) | PRF | – |
| COMBAT-DEC-PF-002 | **Physik mit leistungsbewusster Näherung,** keine offensichtlich unrealistischen Abkürzungen | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘ | PRF TRF | `COMBAT-DEC-GP-006` |
| COMBAT-DEC-PF-003 | **Keine künstliche Maximalreichweite** für Projektile | 📋 BESCHLOSSEN | `COMBAT-DEC-FK-014` | PRF | Lebensdauer ohne Entscheidung (`CQ-19`, `CQ-106`) |
| COMBAT-DEC-PF-004 | **Skalierbare Weltsimulation:** hohe Genauigkeit nahe am Spieler, reduzierte Simulation in der Ferne, ereignisbasiertes Fortschreiben nicht geladener NPCs und Gebiete, Persistenz wichtiger Ergebnisse | 📋 BESCHLOSSEN | `N-SI-001` (`PE-N1022` G) | PRF PER | – |
| COMBAT-DEC-PF-005 | **Nicht jeder NPC braucht die volle Simulation;** wichtige Figuren sind vollständige Akteure, Hintergrund und Ferne sind leichter | 📋 BESCHLOSSEN | `N-SI-002`; Gameplay-Systeme §8 | KI PRF | – |
| COMBAT-DEC-PF-006 | **Persistenzstufen A bis D** legen fest, wie viel Persistenz und Simulation eine Figur bekommt | 🧭 RICHTUNG | `N-SI-003`, `N-ID-022` | KI PER | – |
| COMBAT-DEC-PF-007 | **Hintergrundsimulation arbeitet ereignisbasiert,** Zufall deterministisch aus Figur-ID, Weltseed und Zeitfenster | 🧭 RICHTUNG | `N-SI-011`, `N-SI-012` | PRF | – |
| COMBAT-DEC-PF-008 | **Crowd-LOD:** in der Ferne vereinfachte Simulation, in Spielnähe detaillierte NPCs; Menschenmengen werden aus dem Simulationszustand rekonstruiert | 📋 BESCHLOSSEN | `N-SI-007` | PRF | Kampf in Menschenmengen: `CQ-107` |
| COMBAT-DEC-PF-009 | **Zufall ist über Seeds reproduzierbar;** Risikosysteme bekommen deterministische Simulationen und Leistungsregressionstests | 📋 BESCHLOSSEN | Gameplay-Systeme §17.2 | TST PRF | – |
| COMBAT-DEC-PF-010 | **Kampfsystem-Kostentreiber** (Projektile, Trefferauflösung, Verletzungsmodell, KI-Wahrnehmung, Animationen, Ragdoll, Netzwerk, Audio) werden einzeln gemessen, bevor Budgets festgelegt werden | 💡 VORSCHLAG | Abschnitt 3, 5; `N-SI-020` | PRF TST | – |
| COMBAT-DEC-PF-011 | **Kampf-Detailstufen:** voller Kampf in Spielnähe, vereinfachte Auflösung außerhalb (zum Beispiel Kämpfe zwischen NPC-Gruppen als Ergebnisereignis, nicht als Schuss für Schuss) | 💡 VORSCHLAG | Abschnitt 4; `N-SI-005`, `N-SI-011` | PRF KI | `CQ-106` |
| COMBAT-DEC-PF-012 | **Hardware-Mindest-, Empfohlen- und Ultra-Anforderungen, Bildraten, Raytracing-Umfang und Profile für PS5 Pro und Series S** sind offen | ❓ OFFEN | Gameplay-Systeme §14.1 (letzte Zeile) | PRF | `CQ-108` |
| COMBAT-DEC-PF-013 | **Budgets für Simulation und Kampf** (Gegner gleichzeitig, Projektile, Wahrnehmungstraces, Ragdolls, Wundberechnungen) sind nicht festgelegt und hängen am Prototyp | ❓ OFFEN | `N-SI-017` (`M-OQ16`, `M-AS`); `N-SI-020` | PRF | `CQ-106` |
| COMBAT-DEC-PF-014 | **Kampf in dicht bevölkerten Gebieten** (Leistung, Zeugen, Zivilisten) ist nicht entschieden | ❓ OFFEN | Auftrag §8 (Performance in dicht bevölkerten Gebieten); `N-SI-007` | PRF NPC | `CQ-107` |
| COMBAT-DEC-PF-015 | **Offscreen-Kämpfe** (Fraktionskonflikte, Monsterterritorien) und ihr Fortschreiben sind nicht im Detail festgelegt; beschlossen sind Territoriumssimulation und Konflikte unabhängig vom Spieler | 🧭 RICHTUNG | `N-TE-001`, `N-TE-002`; `K-OE-016` | PRF KI | `CQ-109` |
| COMBAT-DEC-PF-016 | **Leistung beeinflusst nie die Spielregeln:** Detailstufen verändern Darstellung und Genauigkeit der Auflösung, nicht die Gültigkeit beschlossener Regeln (Anatomie, Beweise) | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-GP-014` (Anatomie gleich), `PE-N1022` H (Integration ohne Widerspruch) | PRF | Prüfziel |

---

## Zählung

| Gruppe | Anzahl |
|---|---:|
| GP | 33 |
| ZA | 16 |
| NK | 49 |
| FK | 33 |
| ST | 20 |
| DF | 15 |
| BT | 17 |
| WA | 21 |
| MU | 28 |
| AU | 16 |
| TK | 23 |
| VL | 24 |
| BW | 31 |
| KS | 28 |
| SJ | 19 |
| FF | 15 |
| KI | 33 |
| NM | 21 |
| PO | 27 |
| DE | 17 |
| AN | 19 |
| KO | 21 |
| SP | 20 |
| PF | 16 |
| **gesamt** | **562** |

## Konsolidierung 08.11.2026 (Systemintegration)

| Lücke | Inhalt | Status |
|---|---|---|
| L-01 Todesmodell (BW-001, BW-012, BW-028) | Checkpoint nach echtem Tod widerspricht C-21 | **blockiert (K-26)** |
| L-02 Todesursache wichtiger NPCs (PO-020, ⚠️) | siehe Ermittlung/20 | **blockiert (K-27)** |
| L-05 Gehen/Fahren-Kreuz (ST-009, BT-008) | Belegung nicht entschieden (W-03) | Entscheidung offen |
