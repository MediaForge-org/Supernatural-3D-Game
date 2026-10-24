# Kampfsystem 08: Kampfbewegung und Traversal-Integration

> [!IMPORTANT]
> **Nichts davon ist implementiert außer Sprung, Sprint und der Testgeometrie von M1.** Das Modul verbindet das Kampfsystem mit der Bewegung zu Fuß (Sprinten, Springen, Hürden, Aufsteigen) und legt fest, was die **Traversal-Richtung** vom 22.10.2026 für den Kampf bedeutet. Es erfindet keine Sprunghöhen, Hindernisklassen oder Animationsdaten. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Die Hauptquelle ist [Proposal 0009](../../Proposals/0009-traversal-jump-and-vault-direction.md); die Steuerung steht in [Modul 06](06-Steuerung-und-Eingabekontext.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Bestand der Bewegung](#2-bestand-der-bewegung)
3. [Beschlossene und offene Anforderungen](#3-beschlossene-und-offene-anforderungen)
4. [Was das Kampfsystem von der Traversal-Arbeit braucht](#4-was-das-kampfsystem-von-der-traversal-arbeit-braucht)
5. [Gegenüberstellung: Hindernisse im Kampf](#5-gegenüberstellung-hindernisse-im-kampf)
6. [Bewegung, Verletzung und Erschöpfung](#6-bewegung-verletzung-und-erschöpfung)
7. [Kamera und Nähe zu Wänden](#7-kamera-und-nähe-zu-wänden)
8. [Schnittstellen zum Traversal-Meilenstein und Fehlerfälle](#8-schnittstellen-zum-traversal-meilenstein-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Richtung ist gesetzt, die Mechanik fehlt.** Springen wird aktiv ausgelöst und nie automatisch, die Sprungstärke bleibt menschlich, für niedrige Hindernisse soll später eine höhenpassende Bewegung erscheinen, und Animation muss der tatsächlichen Bewegung entsprechen (Proposal 0009 §2, §3a).
2. **Der Bestand widerspricht der Richtung.** Der Prototyp-Sprung (Sprunggeschwindigkeit 500, Scheitelhöhe etwa 127 cm) liegt über dem, was die Richtung ‚realistisch menschlich‘ verlangt; die 1-m-Testwand von M1 ist nur wegen dieses Wertes überquerbar (Proposal 0009 §3). Das ist eine bekannte Folgearbeit, kein Fehler.
3. **Verletzung und Erschöpfung sollen Traversal später beeinflussen** (Proposal 0009 §3a); der Kampf liefert die Gründe dafür (Hinken, Balance, Blutverlust, Atmung).
4. **Der Kampf braucht Zusagen vom Traversal-Meilenstein,** die hier als Anforderungen formuliert sind (Abschnitt 4). Sie sind Entwurf.
5. **Nicht in den Quellen:** Angriffe in der Luft, Kampf beim Klettern oder Schwimmen, Hindernisse als Fluchtmittel im Kampf (`CQ-31`, `CQ-32`).

---

## 2. Bestand der Bewegung

Quelle: Proposal 0009 §3, §3a (gelesen am 22.10.2026 aus dem Code), README §26.1.

| Größe | Wert (Bestand) | Bemerkung |
|---|---|---|
| Grundgeschwindigkeit | 500 cm/s | Platzhalter |
| Sprint | 800 cm/s (Shift halten) | Platzhalter |
| Sprunggeschwindigkeit | `JumpZVelocity` 500 | Scheitelhöhe etwa 127 cm bei Standardgravitation; höher als ein realistischer Standsprung |
| Luftkontrolle | 0,35 | Vorlagenwert |
| Stufenhöhe | 45 cm | Vorlagenwert |
| begehbarer Bodenwinkel | 44,8 Grad | Vorlagenwert |
| Sprung | aktiv über Eingabe | nie automatisch |
| Vault, Mantle, Hinderniserkennung, Haltezeit | **nicht vorhanden** | M1 enthält kein System |
| Testgeometrie M1 | Rampe 15 Grad, Treppe mit 10 Stufen und Podest, 1-m-Wand | Platzhalter hinter den Türen |

Alle Werte sind Platzhalter der Unreal-Vorlage, keine Spielentscheidung.

---

## 3. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
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

## 4. Was das Kampfsystem von der Traversal-Arbeit braucht

> **💡 VORSCHLAG.** Die Liste formuliert Anforderungen, die aus beschlossenen Kampfaussagen folgen. Sie legt die Traversal-Arbeit nicht fest und macht keine Zusagen für M1.

| Nr. | Anforderung des Kampfes | Ableitung |
|---|---|---|
| T1 | Eine Hinderniserkennung liefert Höhe und Tiefe als Daten, die auch für Deckung und Hürden-Fluchten nutzbar sind (kein zweites System) | `COMBAT-DEC-BT-015`, `COMBAT-DEC-FK-031` |
| T2 | Jede Traversal-Bewegung hat Verletzungs- und Erschöpfungsparameter (Beinverletzung verlängert oder verhindert die Handlung) | `COMBAT-DEC-BT-006`, `-011` |
| T3 | Jede Traversal-Bewegung kennt ihre Kompatibilität mit einer gehaltenen Waffe (Waffe weggesteckt, gesenkt, behalten) | `COMBAT-DEC-NK-023`, `COMBAT-DEC-FK-004` |
| T4 | Motion Warping und Animation folgen der gemessenen Höhe, nicht einer festen Klasse | `COMBAT-DEC-BT-005`, Gameplay-Systeme §14.3 |
| T5 | Ein Abbruch einer Traversal-Bewegung (Treffer, Griff, Betäubung) ist definiert und führt zu einem körperlich plausiblen Zustand | [Modul 03](03-Kampfzustaende-und-Systemarchitektur.md) §7 |
| T6 | Die Kamera bleibt bei Traversal-Bewegungen in Wandnähe konsistent (Wiederholung der Prüfung aus Proposal 0009 §3a) | Abschnitt 7 |
| T7 | Gegner und Monster benutzen dieselbe Hinderniserkennung oder eine bewusst andere Regel je Art | `COMBAT-DEC-GP-018` |
| T8 | Die Bewegungsentscheidung ist in portabler Logik prüfbar (Höhenklassifikation), unabhängig von Unreal | Proposal 0009 §4 (Tests), `COMBAT-DEC-ZA-009` |

---

## 5. Gegenüberstellung: Hindernisse im Kampf

Die Quellen sagen nicht, ob und wie Hindernisse im Kampf genutzt werden. Die Gegenüberstellung hilft bei der Entscheidung `CQ-31`.

| Ansatz | Inhalt | Vorteil | Nachteil | Bezug |
|---|---|---|---|---|
| A: Nur Fortbewegung | Hürden und Aufstiege sind Fortbewegung; im Kampf gelten sie wie sonst, ohne eigene Kampfbezüge | einfach | Fluchten über Hindernisse wirken blass | Proposal 0009 §2 |
| B: Fluchtmittel | Hürde und Mantle sind ausdrücklich ein Mittel, einen Verfolger zu verlangsamen oder zu entkommen; Gegner können es ebenso | taktisch reich, passt zu ‚Flucht‘ ([Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md)) | Balance, Animationsaufwand, KI-Aufwand | `COMBAT-DEC-GP-019` |
| C: Mit Kampfaktionen | zusätzlich Angriffe aus der Hürde, Sprungangriffe | mehr Ausdruck | widerspricht ‚keine Arcade-Bewegung‘; fliegende Techniken nur situativ (`23#166A`, Entwurf) | `COMBAT-DEC-NK-003` |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 6. Bewegung, Verletzung und Erschöpfung

Die Quellen verbinden Bewegung eng mit dem Körper (`M-PD`, `23#101E`, `23#116E`). Die folgende Tabelle ordnet die beschlossenen Einflüsse (Quelle jeweils `M-PD` ‚Human anatomy…‘ sofern nicht anders angegeben):

| Körperzustand | Einfluss auf Bewegung | Status |
|---|---|---|
| Beinverletzung | Hinken, eingeschränkter Sprint, schlechtere Ausweichbewegung | 📋 |
| Bruch, offener Bruch | starke Funktionsstörung, sichtbare Verletzung, hohe Komplikationsgefahr | 📋 (`23#103A` P8 Entwurf, `23#101E` P9 bestätigt) |
| Blutverlust, Schock | schwächere Koordination, Tunnelblick, Bewusstlosigkeit | 📋 |
| Atemproblem | Ausdauer sinkt, Zielen unruhiger | 📋 |
| Adrenalin | unterdrückt Schmerz zeitweise; danach Verschlechterung | 📋 |
| Erschöpfung | nicht nach wenigen Sekunden, aber wirksam; Training senkt sie | 📋 / 🧭 |
| Kälte, Hitze, Dehydrierung | Körpermodell (Gameplay-Systeme §9.2) | 🧭 (Formeln offen) |

**Hinweis:** Das Körpermodell (Gewicht, Fett, Muskelgruppen, Training, Erholung) ist Teil einer eigenen Planung; die Formeln sind ❓ offen (Gameplay-Systeme §9.2). Dieses Modul benutzt nur die Richtung (‚Kraft ist nicht identisch mit sichtbarer Muskelgröße‘).

---

## 7. Kamera und Nähe zu Wänden

Der zweite M1-Audit hat im Nahbereich einer Wand ein Kameraproblem gefunden und teilweise behoben (Proposal 0008 §11 und §12). Für den Kampf sind drei Ergebnisse relevant:

1. **Die Kamera kann bei Wandnähe sehr nahe an den Charakter rücken** (33 bis 60 cm vom Drehpunkt). Der Charakter-Mesh wird in der Ego-Sicht des Besitzers ausgeblendet, solange die Kamera unter einer Platzhalterdistanz von 70 cm liegt (`HideMeshCameraDistance`).
2. **Das Wachstum des Kameraarms ist begrenzt** (Platzhalter 300 cm/s), damit die Kamera nicht zwischen ‚an den Charakter gedrückt‘ und ‚weit entfernt‘ springt.
3. **Die Gefühlsprüfung im laufenden Spiel steht aus;** die Werte sind Platzhalter.

**Folgerung für den Kampf (🧩 ABGELEITET):** Nahkampf und Handgemenge finden oft an Wänden statt (`COMBAT-DEC-NK-008`, `-033`); die Kamera muss dort konsistent bleiben. Der Traversal-Meilenstein soll die Kamera erneut prüfen (Proposal 0009 §3a), und das Kampfsystem fügt eine Prüfung für Wandgriff und Boden hinzu ([Modul 30](30-Testkatalog-und-Abnahmekriterien.md)).

---

## 8. Schnittstellen zum Traversal-Meilenstein und Fehlerfälle

Im Auftrag heißt der nächste Meilenstein ‚M2 Movement & Traversal‘. Im Repository ist er nur als ‚later traversal milestone‘ beschrieben (Proposal 0009 §2 (4)); die Bezeichnung ‚M2‘ trägt der Auftrag, nicht eine Entscheidung im Repo. Die folgenden Schnittstellen sind Entwurf.

| Richtung | Schnittstelle (Entwurf) |
|---|---|
| Traversal → Kampf | Höhenklassifikation, Kompatibilität mit Waffenzustand, Abbruchereignisse |
| Kampf → Traversal | Verletzungs- und Erschöpfungsparameter, Verbotszustände (Griff, Kampfunfähigkeit) |
| Beide → Animation | Motion Warping, Montagen, IK ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) |
| Beide → Koop | lokale Autorität, Synchronisierung von Pose ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Sprung während Griff | Ablehnung mit Grund | 💡 |
| Hindernishöhe liegt zwischen zwei Klassen | die Entscheidung folgt der gemessenen Höhe und der Konfiguration; es gibt keinen Zwischenzustand ohne Bewegung | 💡 |
| Hindernis wird während der Bewegung entfernt (Tür geöffnet) | Bewegung bricht körperlich plausibel ab | 💡 |
| Landung auf ungeeignetem Untergrund | Verletzungsrisiko nach Höhe, Technik, Untergrund (`COMBAT-DEC-BT-009`) | 📋 |
| Treffer während des Absprungs | Zustand wird neu aufgelöst, Hauptmodus kann wechseln | 💡 |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-31` | Sprungstärke (Scheitelhöhe), Zuordnung der Hindernishöhen zu Aufstieg, Hürde und Mantle, Asset-Herkunft (eigene, gekaufte, generierte); Rolle von Hindernissen im Kampf und Kampfaktionen in der Luft | `COMBAT-DEC-BT-002`, `-003`, `-015`, `-017` | ja |
| `CQ-32` | Kampf beim Klettern, Schwimmen, im Wasser; Verletzungsrisiko bei Sprüngen ins Wasser | `COMBAT-DEC-BT-009`, `-017` | ja |
