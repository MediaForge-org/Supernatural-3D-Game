# Fahrzeuge und Verkehr 13: Verkehrs-KI und Navigation

> [!IMPORTANT]
> **Es gibt kein Verkehrsmodell.** Beschlossen sind Verhaltensanforderungen: Fahrer unterscheiden sich in Können und Wesen, passen sich dem Wetter an, bilden Rettungsgassen, und die Straße ist weder leer noch unglaubwürdig voll. Alles, was beschreibt, **wie** das erreicht wird, ist Vorschlag oder Option. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Die NPC-Architektur (Identität, Tagesablauf, Persistenz) gehört den [NPCs](../NPCs/README.md); dieses Modul ergänzt nur die Fahrseite.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Schichten der Verkehrs-KI](#2-schichten-der-verkehrs-ki)
3. [Navigation und Routenwahl](#3-navigation-und-routenwahl)
4. [Fahrstile](#4-fahrstile)
5. [Katalog glaubwürdigen Verhaltens](#5-katalog-glaubwürdigen-verhaltens)
6. [Determinismus](#6-determinismus)
7. [Wichtige NPCs und Reaktionen auf den Spieler](#7-wichtige-npcs-und-reaktionen-auf-den-spieler)
8. [Register](#8-register)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Fahrer | Fahrer-NPCs unterscheiden sich in Können und Persönlichkeit; Anfänger können manuelle Autos abwürgen; das Wetter ändert das Verhalten | `W-FP-009` |
| Stile | Anfänger, vorsichtig, normal, aggressiv, schlecht; Anpassung an Wetter; Rettungsgassen; NPC-Kriminelle können von der Polizei verfolgt werden | `N-LE-024` |
| Rekonstruktion | Die Straße ist beim Betreten eines Gebiets nicht leer, sondern voll; Menschenmengen und Verkehr werden aus abstraktem Zustand rekonstruiert | `N-LE-013`, `N-LE-026`, `W-TV-009` |
| Eigene Fahrzeuge | NPCs benutzen wiederholt dasselbe eigene Fahrzeug | `N-ID-005` |
| Wiedererkennung | NPCs können den Spieler und auffällige Fahrzeuge wiedererkennen | `N-GE-018` |
| Schutz | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI | `N-LT-033` |
| Navigation | Smartphone-Ökosystem kann Navigation und Verkehr enthalten (Wortlaut ‚may include‘) | `M-CA0905` |
| Offen | Verkehrsmodell, UI, Mehrspieler-Synchronisation | `W-FP-010`; `D15-RV` |

---

## 2. Schichten der Verkehrs-KI

> **💡 VORSCHLAG.** Die Schichtung trennt Zuständigkeiten, damit jede einzeln testbar bleibt. Sie ist eine Arbeitsstruktur, keine Entscheidung über Algorithmen.

| Schicht | Aufgabe | Eingaben | Ausgabe |
|---|---|---|---|
| **Strategie** | Ziel und Route: wohin, wann, auf welchen Straßen | Tagesablauf des NPC ([NPCs 13](../NPCs/13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md)), Straßengraph, Verkehrslage, Sperrungen | Folge von Straßenkanten |
| **Taktik** | Spurwahl, Überholen, Abbiegen, Vorfahrt, Lücken, Anhalten | Kreuzungsdaten (Modul 11), Nachbarfahrzeuge, Fahrstil | Manöver (z. B. ‚Spur wechseln‘, ‚Vortritt gewähren‘) |
| **Operation** | Gas, Bremse, Lenkung, Kupplung, Gang | Manöver, Fahrzeugzustand, Straße, Wetter | Fahrerabsicht an das Fahrzeug (Modul 04) |

Die Schicht ‚Strategie‘ gehört zur NPC-Architektur (Tagesablauf wählt das Ziel), nicht zu einer eigenen Verkehrspopulation. Die Schichten ‚Taktik‘ und ‚Operation‘ gibt es nur für Fahrzeuge in Spielernähe; in der Ferne gilt ein abstrakter Zustand (Modul 26).

---

## 3. Navigation und Routenwahl

> **💡 VORSCHLAG.**

- **Ein Graph, eine Wahrheit:** NPC-Routenwahl, die Navigation des Spielers und die Radiomeldungen lesen denselben Straßengraph (Modul 24) und dieselbe Verkehrslage (Modul 15). Eine Sperrung, die der Spieler im Navigationsgerät sieht, ist dieselbe, die NPCs umfahren.
- **Dynamische Kosten:** Eine Kante kostet Zeit; Stau, Baustelle, Sperrung, Wetter, Straßenklasse und Zugangsrecht der Verkehrsrolle verändern das Gewicht. Neuplanung geschieht bei Ereignissen, nicht in jedem Bild.
- **Netzgröße:** Das Arbeitsszenario ist rund 2.200 km² (`N-LT-007`, nicht beschlossen). Eine Routensuche über das ganze Netz kann teuer werden. Hierarchische Verfahren (grobe Fernrouten, feine lokale Routen) sind eine **zu prüfende Option**, keine Entscheidung (`CQ-50`).
- **Spielerseite:** Navigation über Karte, Smartphone und Radio ist vorgesehen (`M-CA0905`, `W-VK-014`). Ob es Zielführung, Straßenschilder, Weitsicht oder nur eine Karte gibt, ist nicht belegt (`CQ-54`).

---

## 4. Fahrstile

Die beschlossene Liste (`N-LE-024`): Anfänger, vorsichtig, normal, aggressiv, schlecht.

> **💡 VORSCHLAG.** Ein Fahrstil ist ein **Profil aus Parametern**, kein Fest-Schalter. Parameter (Arbeitsnamen, ohne Werte): Abstandsverhalten, Tempoüberschreitung, Reaktionszeit, Regeltreue, Risikobereitschaft, Geduld, Spurwechselfreude, Anfahrkönnen (Abwürgen, `W-FP-009`). Das Profil wird dem NPC zugeordnet (aus Persönlichkeit und Gewohnheit, [NPCs 04](../NPCs/04-Persoenlichkeit-Zustaende-und-Zeitskalen.md)) und kann sich mit Wetter, Zeitdruck und Stimmung verändern.
>
> Die Werte sind nicht belegt und sollen datengetrieben bleiben (`VEH-DEC-GP-014`).

Belegt ist für die Spielfigur: Alkohol beeinflusst Reaktion, Gleichgewicht, Wahrnehmung und Fahren; Drogen haben eigene Wirkungen und Risiken (`M-CA0905`). Nicht belegt sind Rechtsfolgen des Fahrens unter Einfluss, das Verhalten von NPC-Fahrern unter Einfluss sowie Müdigkeit und Ablenkung als Fahrfaktor (`CU-07`, `CQ-53`).

---

## 5. Katalog glaubwürdigen Verhaltens

Der Auftrag nennt eine Reihe von Verhaltensweisen, die Verkehr glaubwürdig machen. Die Tabelle trennt, was die Quellen verlangen, von dem, was Vorschlag oder offen ist. Die Tabelle ist zugleich die Grundlage der Eigenschaftstests in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md).

| Verhalten | Quelle verlangt | Status | Hinweis |
|---|---|---|---|
| Unterschiedliche Fahrstile | ja | 📋 | `N-LE-024`, `W-FP-009` |
| Regeln befolgen (Vorfahrt, Signale) | indirekt (Rotlicht-Vergehen, `VEH-DEC-KR-001`) | 🧩 | Regeln selbst: Modul 11 |
| Spurwechsel, Überholen, Abbiegen | nein | 💡 | durch Fahrstil und Straße bestimmt |
| Signalphasen beachten | indirekt | 🧩 | Signalzustand aus Weltzeit (Modul 11) |
| Parkplatzsuche | nein | 💡 | Modul 12 |
| Stau und Verdichtung | ja | 📋 | `W-TV-001`; Modul 15 |
| Unfälle (selten) und Pannen | ja | 📋 | Modul 16 |
| Einsatzfahrzeuge, Rettungsgassen | ja | 📋 | `N-LE-024`; Modul 16 |
| Wetter- und Straßenzustandsanpassung | ja | 📋 | `N-LE-024`, `23#1025E` Nr. 48 |
| Reaktion auf Unfälle und Sperrungen | indirekt (Welt reagiert auf Ereignisse) | 🧩 | `N-LE-025`; Modul 16 |
| Abwürgen bei Anfängern | ja | 📋 | `W-FP-009` |
| Reaktion auf den Spieler (Hupen, Drängeln, Rammen) | nein | ❓ | `CQ-51` |

---

## 6. Determinismus

Das Welt-Register nennt als Assistentenvorschlag, Zufall deterministisch aus NPC-ID, Welt-Seed und Zeitfenster abzuleiten (`W-TV-010`, 💡); der Entwickler hat darauf keine Einzelentscheidung gestützt. Für Verkehr gilt entsprechend:

> **💡 VORSCHLAG.** Jede Zufallsentscheidung der Verkehrs-KI (Aufbruchzeit, Routenvariante, Fahrstilschwankung, Panne) bekommt ihren Zufall aus (NPC-ID, Welt-Seed, Zeitfenster). Damit sind Verkehrsszenen reproduzierbar, rekonstruierbar und testbar. Ob exakte Reproduzierbarkeit auch zwischen verschiedenen Plattformen gefordert wird, ist offen (`CQ-112`).

---

## 7. Wichtige NPCs und Reaktionen auf den Spieler

**Schutz:** Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI (`N-LT-033`). Gleichzeitig sind seltene NPC-Unfälle beschlossen (`W-TV-001`) und Verletzungen von NPCs sind persistent (`M-WB89` Block 8).

> **💡 VORSCHLAG.** Die Schutzregel gilt für den **Tod** durch Simulationsunfälle, nicht zwingend für jede Verletzung. Technisch: Unfälle, an denen ein wichtiger NPC im Fahrzeug sitzt, werden von der Verkehrs-KI mit einer Sicherheitsgrenze behandelt; schwere Folgen entstehen nur durch den Spieler oder durch geskriptete Ereignisse. Ob auch Verletzungen verhindert werden, ist offen (`CQ-52`). Die Spannung zur unabhängigen Lebensentwicklung ist als `CK-02` geführt.

**Reaktionen auf den Spieler:** Beschlossen ist, dass NPCs den Spieler und auffällige Fahrzeuge wiedererkennen (`N-GE-018`) und dass der Spieler NPC-Unfälle verursachen kann, indem er rammt (`W-TV-001`). Wie NPC-Fahrer auf Hupen, Drängeln, Blockieren, Verfolgung oder Schüsse reagieren, ist nicht beschrieben.

---

## 8. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-KI-001 | Fahrer-NPCs unterscheiden sich in Können und Persönlichkeit; Anfänger können manuelle Autos abwürgen; das Wetter ändert das Verhalten | 📋 BESCHLOSSEN | `W-FP-009` | NPC KI | – |
| VEH-DEC-KI-002 | Fahrstile: Anfänger, vorsichtig, normal, aggressiv, schlecht; Anpassung an das Wetter; Rettungsgassen | 📋 BESCHLOSSEN | `N-LE-024` | NPC KI | Parameter: 💡 `VEH-DEC-KI-006` |
| VEH-DEC-KI-003 | NPCs können den Spieler und auffällige Fahrzeuge wiedererkennen | 📋 BESCHLOSSEN | `N-GE-018` | NPC ERM | Schwellen offen |
| VEH-DEC-KI-004 | Navigation und Verkehrsanzeige können Teil des Smartphone-Ökosystems sein | 🧭 RICHTUNG | `M-CA0905` (‚may include‘) | UI | `CQ-54` |
| VEH-DEC-KI-005 | Die Verkehrs-KI hat drei Schichten: Strategie (Ziel und Route), Taktik (Manöver), Operation (Gas, Bremse, Lenkung); die Strategie gehört zur NPC-Architektur | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `N-LE-013` | KI NPC | Keine zweite NPC-Architektur |
| VEH-DEC-KI-006 | Ein Fahrstil ist ein Parameterprofil (Abstand, Tempoüberschreitung, Reaktionszeit, Regeltreue, Risiko, Geduld, Spurwechsel, Anfahrkönnen), das sich mit Wetter und Stimmung ändern kann | 💡 VORSCHLAG | Ableitung aus `N-LE-024`, `W-FP-009` | NPC KI | Werte nicht belegt |
| VEH-DEC-KI-007 | NPC-Routenwahl, Spielernavigation und Radiomeldungen lesen denselben Straßengraph und dieselbe Verkehrslage | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-015` | KI UI | – |
| VEH-DEC-KI-008 | Routenkosten sind dynamisch (Stau, Baustelle, Sperrung, Wetter, Klasse, Zugangsrecht); Neuplanung erfolgt bei Ereignissen | 💡 VORSCHLAG | Ableitung aus `W-TV-001` | KI | – |
| VEH-DEC-KI-009 | Hierarchische Routensuche ist wegen der Netzgröße eine zu prüfende Option | 💡 VORSCHLAG | – | KI PF | `CQ-50` |
| VEH-DEC-KI-010 | Zufall der Verkehrs-KI wird deterministisch aus NPC-ID, Welt-Seed und Zeitfenster abgeleitet | 💡 VORSCHLAG | `W-TV-010` (Assistentenvorschlag) | KI PER | `CQ-112` |
| VEH-DEC-KI-011 | Schutzregel für wichtige NPCs: kein tödlicher Simulationsunfall; ob auch Verletzungen verhindert werden, ist offen | 💡 VORSCHLAG | Ableitung aus `N-LT-033` | KI NPC | `CQ-52`, `CK-02` |
| VEH-DEC-KI-012 | Das Verkehrsmodell (Agenten, Fluss oder Mischform) ist nicht beschlossen | ❓ OFFEN | `W-FP-010`; `W-TV-008` | KI | `CQ-49` |
| VEH-DEC-KI-013 | Wie NPC-Fahrer auf Hupen, Drängeln, Blockieren, Verfolgung und Schüsse reagieren, ist nicht beschrieben | ❓ OFFEN | – | KI KMP | `CQ-51` |
| VEH-DEC-KI-014 | Alkohol beeinflusst die Spielfigur in Reaktion, Gleichgewicht, Wahrnehmung und Fahren | 📋 BESCHLOSSEN | `M-CA0905` | KI MED | Rechtsfolgen und NPC-Verhalten: `VEH-DEC-KI-016` |
| VEH-DEC-KI-015 | Zielführung, Straßenschilder und Karte des Spielers sind nicht belegt | 🔍 UNBELEGT | – | UI KI | `CQ-54` |
| VEH-DEC-KI-016 | Rechtsfolgen des Fahrens unter Einfluss, Fahren von NPCs unter Einfluss sowie Müdigkeit und Ablenkung als Fahrfaktor sind nicht belegt | 🔍 UNBELEGT | – | KI POL | `CU-07`, `CQ-53` |

---

## 9. Offene Punkte

| Frage | Verweis |
|---|---|
| Verkehrsmodell (Agenten, Fluss, Mischform) | `CQ-49` |
| Hierarchische Routensuche | `CQ-50` |
| Reaktion der NPC-Fahrer auf den Spieler | `CQ-51` |
| Schutz wichtiger NPCs bei Unfällen (Tod oder auch Verletzung) | `CQ-52` |
| Zielführung und Karte des Spielers | `CQ-54` |
| Fahren unter Einfluss | `CQ-53` |
| Reproduzierbarkeit zwischen Plattformen | `CQ-112` |
