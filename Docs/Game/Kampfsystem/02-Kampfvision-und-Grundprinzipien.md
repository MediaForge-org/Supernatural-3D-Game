# Kampfsystem 02: Kampfvision und Grundprinzipien

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul hält fest, was der Entwickler über die **Grundhaltung** des Kampfsystems entschieden hat: Realismus, Spielbarkeit, Risiko, Konsequenzen und Schwierigkeit. Es erfindet keine Werte. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene Grundprinzipien](#2-beschlossene-grundprinzipien)
3. [Realismus, Spielbarkeit und Komplexität](#3-realismus-spielbarkeit-und-komplexität)
4. [Risiko und Konsequenzen](#4-risiko-und-konsequenzen)
5. [Schwierigkeit und Einstellungen](#5-schwierigkeit-und-einstellungen)
6. [Systemübergreifende Wechselwirkungen](#6-systemübergreifende-wechselwirkungen)
7. [Was das Kampfsystem ausdrücklich nicht sein soll](#7-was-das-kampfsystem-ausdrücklich-nicht-sein-soll)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Grundhaltung ist eindeutig und mehrfach bestätigt:** tiefe, körperliche, konsequenzreiche Simulation im Hintergrund, verständliche und konfigurierbare Bedienung vorne, keine Kugelschwämme, keine sichtbaren Level (`M-PD`, `23#85A`–`23#88E`).
2. **Der Entwickler bindet Schwierigkeit an Gegnerverhalten und Verletzungsfolgen, nicht an Lebenspunkte.** Grounded heißt aufmerksamere, aggressivere, schneller zuschlagende Gegner, schwerere Verletzungen, knappe Ressourcen und hohe Letalität (`M-PD` ‚Difficulty / configuration‘, Spielvision §14.1).
3. **Zahlen fehlen bewusst.** Entscheidungsstatus §5.6 und `M-OD` ‚Combat / injury / gore‘ führen die Parameter als offen. Dieses Modul entscheidet sie nicht.
4. **Zwei Spannungen werden hier nur benannt:** der Widerspruch zwischen ‚keine Plot-Rüstung‘ und dem Schutz bestimmter Figuren ([NPC-Konflikt C-N1](../NPCs/27-Konflikte-und-offene-Fragen.md)) und die Frage, wie weit der Gameplay-Tod schon festgelegt ist ([Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md), `CK-01`).

---

## 2. Beschlossene Grundprinzipien

Die Tabelle fasst die Prinzipien zusammen, auf die sich alle weiteren Module stützen. Spalten: siehe [Modul 01 §9](01-Quellenlage-und-Methodik.md#9-konventionen-für-ids-und-systemkürzel).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
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

## 3. Realismus, Spielbarkeit und Komplexität

### 3.1 Was die Quellen sagen

Der Entwickler verlangt gleichzeitig zwei Dinge, die sich nur durch Schichtung vereinbaren lassen:

1. **Eine tiefe, körperlich ernst genommene Simulation.** Beispiele aus den Quellen: 25+ Trefferregionen mit Knochen, Organen und Gefäßen (`M-PD`), Blutvolumen und Schock, Projektilpfad durch Gewebe, realistische Magazine (`23#86E` P12: nicht wie in einem bekannten Shooter, in dem man nach einem Schuss nachlädt und das Magazin ‚einfach im letzten gespawnt‘ ist), Positionsübergänge im Bodenkampf.
2. **Eine Bedienung, die nicht überfordert.** Beispiele: ‚nicht zu viele Tastenkombinationen‘ (`23#157E`), ‚sonst wird es zu komplex und der Spieler soll nicht eine Tabelle an Tastenkombinationen auswendig lernen‘ (`23#171E`), eine kontextabhängige Eingabe für Störungsbeseitigung (`M-PD`), Anzeige ‚ein oder aus‘, nichts dazwischen (`23#118E` P22).

Der Mittelweg ist in `M-PD` als Prinzip formuliert (‚Deep systems should exist underneath, while many assists/realism options are independently configurable‘) und im Chat so begründet, dass tiefe Systeme **nicht für Gelegenheitsspieler vereinfacht**, sondern **abschwächbar** sein sollen (`23#61A`, bestätigt in `23#62E` P11). Das ist die Leitentscheidung dieses Moduls: **Vereinfachung ist eine Option über der Simulation, nicht ein Ersatz für sie.**

### 3.2 Bewertungsrahmen für Mechaniken (💡 VORSCHLAG)

Der Auftrag verlangt, bei jeder Mechanik zu unterscheiden, ob eine Vereinfachung physikalisch, medizinisch, bedienungsbezogen, übernatürlich oder technisch begründet ist (`AUF` §6.1). Die Module 04 bis 25 benutzen dafür dasselbe Raster:

| Dimension | Frage | Typische Antwort in den Quellen |
|---|---|---|
| Physikalische Glaubwürdigkeit | Verhält sich die Mechanik so, dass ein aufmerksamer Spieler keinen offensichtlichen Bruch sieht? | ‚nicht so unrealistisch‘ (`23#124E`); Rückstoß physikalisch (`M-PD`); keine künstliche Maximalreichweite |
| Medizinische Plausibilität | Folgt die Verletzung einem plausiblen Pfad und Verlauf? | Projektilpfad, Blutvolumen, Schmerz ≠ Schaden, verzögerte Symptome, Diagnoseunsicherheit (`M-PD`) |
| Spielerische Bedienbarkeit | Kann der Spieler die Mechanik ohne Tabelle auslösen? | kontextabhängige Eingabe, wenige konsistente Inputs (`23#171E`) |
| Übernatürliche Ausnahme | Darf die Mechanik hier bewusst Regeln brechen? | Monster dürfen Regeln brechen (`23#86E` P13), übernatürliche Verletzungen sind Modifikatoren (`M-PD`) |
| Technische Umsetzbarkeit | Ist sie mit Unreal-C++-Mitteln und den Performance-Zielen vertretbar? | Schwierigkeit anerkannt (‚schwer umzusetzen wahrscheinlich‘, `23#101E` P7), nicht als Ausschluss, sondern als spätere Prüfung |

**Regel für Lücken (💡):** Wo eine Entscheidung fehlt, stehen in den Modulen die denkbaren Lösungen nebeneinander (Gegenüberstellung) und nicht eine Vorauswahl. Die Wahl bleibt der Projektleitung ([Modul 28](28-Konflikte-und-offene-Fragen.md)).

### 3.3 Beispiel: Wie die Schichtung bei einer Mechanik aussieht

Am Beispiel ‚Waffenstörung‘ (Quellen: `M-PD`, `23#155E` P16, `23#157E`–`23#160A`):

| Schicht | Inhalt | Status |
|---|---|---|
| Simulation | Störungen sind selten; wahrscheinlicher bei schlechter Wartung, Verschmutzung, Schaden oder problematischer Munition; robuste Konstruktionen vertragen mehr | 📋 BESCHLOSSEN (`M-PD`, `23#155E` P16, P19) |
| Bedienung | eine kontextabhängige Eingabe löst die richtige realistische Handlung aus | 📋 BESCHLOSSEN (`M-PD`; `23#157E`: nicht zu viele Tastenkombinationen) |
| Option | echte Fehlerbehebung je Störungsart statt Automatik | 🧭 RICHTUNG: der Assistent schlägt B (vereinfacht) als Standard und A als Realismusmodus vor (`23#156A`), der Entwickler antwortet ‚das weiß ich noch nicht‘ (`23#157E`) und nimmt die kontextabhängige Lösung an (`23#159E`) |

Die Schichtung ist damit erkennbar: Die Simulation ist beschlossen, die Bedienung ist beschlossen, die Frage ‚Automatik oder Handgriff‘ bleibt eine Option.

---

## 4. Risiko und Konsequenzen

Die Quellen verbinden Risiko mit **Folgen in der Welt**, nicht mit einem Game-Over-Bildschirm:

- Eine schwere Verletzung führt zu Behandlungsbedarf, Heilzeit, Einschränkungen und gegebenenfalls Krankenhaus (`M-PD`; `23#61A`/`23#62E`).
- Ein Fehler gegen ein starkes Monster darf im härtesten Modus ‚ins Krankenhaus‘ führen, und der Tod ist dort leichter möglich (`23#64E` P1, `E-FA-017`).
- Folgen können Zeitverlust, neue Opfer, einen Wechsel des Monsterortes, einen veränderten Fallzustand, zurückgelassene Ausrüstung, ein am Ort zurückgebliebenes Fahrzeug, Krankenhaus und bleibende Verletzungen sein (Spielvision §14.2 als Richtung, `M-PD` ‚Death / failure — OPEN‘).
- Kampffolgen sind **weltpersistent**: Wichtige Konsequenzen verschwinden nicht, weil der Spieler das Gebiet verlässt (`PE-N1022` E; `M-PD` ‚World state and persistence‘).
- Tod und Wiederherstellung im Detail sind **nicht** entschieden; der Chat enthält Stellen, die Spielvision §14.2 nicht erwähnt (`23#364E`, [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md)).

Für Tests bedeutet das: Ein Kampfsystem-Test prüft nicht nur ‚Gegner stirbt‘, sondern die **Kette** ‚Treffer → Zustand → Verhalten → Weltfolge → Persistenz‘ ([Modul 30](30-Testkatalog-und-Abnahmekriterien.md)).

---

## 5. Schwierigkeit und Einstellungen

| Aspekt | Beschlossen | Offen |
|---|---|---|
| Presets | viele Voreinstellungen plus ‚Eigene‘ (`COMBAT-DEC-GP-012`) | Namen, Anzahl, genaue Belegung der Regler |
| Unabhängigkeit | Kampf, Verletzung, Ermittlung, Fahren, Navigation, Folgen getrennt (`COMBAT-DEC-GP-011`) | Liste der Regler (`CQ-04`) |
| Grounded | Gegnerverhalten, Verletzungsfolgen, Ressourcen, Letalität (`COMBAT-DEC-GP-013`); Komfortelemente standardmäßig aus (`COMBAT-DEC-GP-015`) | welche Elemente genau |
| Barrierefreiheit | eigene Ebene (`COMBAT-DEC-GP-016`) | Umfang |
| Kampf-Hilfen | Zielhilfe optional, soft (`COMBAT-DEC-NK-006`) | Stärke, Standardwerte je Stufe |

Aus den Schwierigkeitsentscheidungen folgen **testbare Unabhängigkeiten** (Modul 30): Der Wechsel des Kampf-Schwierigkeitsgrades verändert die Ermittlungsführung nicht, und Barrierefreiheitsoptionen bleiben auch in Grounded verfügbar.

---

## 6. Systemübergreifende Wechselwirkungen

Der Auftrag nennt sieben Wechselwirkungen, die jedes Modul berücksichtigen muss (`AUF` §6.2). Die Tabelle zeigt, welche Quellenentscheidungen sie tragen und wo sie behandelt werden.

| Wechselwirkung | Tragende Entscheidungen | Behandelt in |
|---|---|---|
| Kampf ↔ Bewegung | Realistische Sprünge, ‚Animation und Bewegung müssen übereinstimmen‘, Verletzung und Erschöpfung beeinflussen später Traversal (Proposal 0009 §3a); Verletzungen beeinflussen Hinken, Sprint, Waffeneinsatz (`M-PD`) | [Modul 08](08-Kampfbewegung-und-Traversal-Integration.md), [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md) |
| Kampf ↔ Medizin | Gleiche Anatomie, Projektilpfad, Blutvolumen, Schock, Genesungsstufen (`M-PD`) | [Modul 12](12-Treffermodell-und-Koerperphysik.md), [13](13-Verletzungen-Schmerz-und-Kampffolgen.md), [14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| Kampf ↔ NPCs | Langzeitgedächtnis über Verletzungen und Konflikte, Nemesis, Begleiter (`PE-N1022`, `N-NE-001`, `N-CO-006`) | [Modul 18](18-Gegner-KI-und-Kampftaktik.md), [19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| Kampf ↔ Kreaturen | Schwächen, Anatomie, Regeneration, bleibende Narben (`K-KO-002`, `K-KO-003`, `K-KO-007`, `K-KO-008`) | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md), [16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md), [17](17-Fallen-Fesselung-und-Lebendfang.md) |
| Kampf ↔ Ermittlungen | Zeugen, Spuren, Waffen und Ballistik als Beweise, Tatorte bleiben bestehen (`E-RE-002`, `E-SP-013`, `E-QA-023`) | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| Kampf ↔ Persistenz | Zentraler Weltzustand, segmentierte Saves, Reload-Zustand persistent (`M-PD` ‚World state and persistence‘, ‚Gunplay / weapons‘) | [Modul 24](24-Speicherstaende-und-Weltpersistenz.md) |
| Kampf ↔ Koop | Welt des Hosts, Fortschritt am Spielstand des Hosts, Krankenhaus im Koop überspringbar (`E-KO-004`, `E-KO-007`, `E-WZ-005`) | [Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md) |

---

## 7. Was das Kampfsystem ausdrücklich nicht sein soll

Alle Punkte sind in den Quellen belegt; die Liste dient als Negativprüfung (Modul 30).

| Nicht-Ziel | Quelle |
|---|---|
| Kugelschwamm-Gegner, Gegner mit künstlich hohen Lebenspunkten | `COMBAT-DEC-GP-001` |
| Energy-Drink-Heilung, Sofortheilung | `M-PD` ‚Human anatomy…‘; `23#60E` |
| Plot-Rüstung für Spieler und menschliche NPCs | `COMBAT-DEC-GP-007` |
| Soulslike-Wiederherstellung und Rollen-Spam | `M-PD` ‚Death / failure — OPEN‘; ‚Melee / grappling‘; `23#88E` (F: ‚soll kein Souls game sein‘) |
| UFC-Klon, ununterbrechbare Kombo-Ketten, universeller Betäubungs-Lock | `M-PD` ‚Melee / grappling‘, ‚Multiple-opponent combat…‘ |
| Harte Zielaufschaltung als Standard | `M-PD` ‚Melee / grappling‘; `23#163E` P21 |
| Universeller Blockzustand mit Pauschalschutz | `M-PD` ‚Melee / grappling‘ |
| Gegner, die höflich nacheinander angreifen | `M-PD` ‚Multiple-opponent combat…‘; `23#222E` P1 |
| Gegner mit Allwissen, Wandblick, globaler Anti-Spieler-Telemetrie | `M-PD` ‚Stealth, perception and horror AI‘; `23#242E` |
| Zeitlupen-Fähigkeit mit Wandblick | `COMBAT-DEC-GP-021` |
| Kosmetisch erzeugte Standard-Splitter als Dauerschaden | `M-PD` ‚Ballistics‘; `23#130E` |
| Magische Rückkehr ins Inventar nach Entwaffnung | `23#162A` (Vorschlag), nicht Entwicklerzitat; siehe [Modul 09](09-Waffenklassen-und-Ausruestung.md) |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-03` | Schadenszahlen und Trefferanzeigen: Gibt es Rückmeldungen wie Zahlen, Trefferindikatoren, Kill-Anzeigen, und in welchem Schwierigkeitsgrad? | `COMBAT-DEC-GP-003` | ja |
| `CQ-04` | Katalog der Einstellbereiche und Realismusstufen je Kampfmechanik (Presets, Standardwerte, Grounded-Werte) | `COMBAT-DEC-GP-010` bis `-015`, `-033` | ja |
| `CQ-05` | Combat Focus: Aktivierung, Eingabe, Dauer, Erholung, Rückmeldung | `COMBAT-DEC-GP-021`, `-022` | ja |
| `CQ-06` | Gesamtmodell der Kampffertigkeiten: Nutzung, Training, Lehrer, abnehmende Erträge, Zusammenhang mit dem Körpermodell | `COMBAT-DEC-GP-024`, `-025` | ja |
| `CQ-07` | Todes- und Scheiterschleife: Verhältnis von `23#364E` zu `M-PD` ‚Death / failure — OPEN‘ | `COMBAT-DEC-GP-026`, `-027` | ja (`CK-01`) |
