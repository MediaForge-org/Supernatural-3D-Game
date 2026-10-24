# Kampfsystem 17: Fallen, Fesselung und Lebendfang

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul beschreibt die **Kampfseite** von Fallen, Fesselung, Eindämmung, Transport und Gefangennahme. Die Entscheidungen liegen in der [Kreaturen-Dokumentation, Modul 13](../Kreaturen/13-Lebendfang-Fallen-und-Containment.md); hier werden sie nur verwiesen und auf den Kampf bezogen. Der Bunker ist ein eigenes Planungsthema und hier nicht festgelegt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Fallen im Kampf](#3-fallen-im-kampf)
4. [Fesselung und nichttödliche Überwältigung](#4-fesselung-und-nichttödliche-überwältigung)
5. [Transport, Verwahrung, Verhör](#5-transport-verwahrung-verhör)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Gefangennahme ist ein umfassendes System** mit maximaler Freiheit (Fallen, Fesseln, Betäubung, Rituale, Transport, Verwahrung, Forschung, Verhandlung); der Entwickler wählte Option D (`K-CO-001`, `48#129E`).
2. **Fallen sind frei platzierbar und persistent;** sie verändern sich durch Wetter, NPCs, Monster, Spieler und Zeit, auch wenn niemand dort ist (`K-CO-003`, `K-CO-004`).
3. **Es gibt keine universelle Zelle;** die Wirksamkeit hängt von der Mythologie ab (`K-CO-002`).
4. **Monster reagieren taktisch auf Fallen,** ein in einer wirksamen Falle gefangenes Wesen befreit sich nicht durch KI allein (`K-KI-015`, `K-KI-016` 💡).
5. **Für Menschen gilt die Festnahme,** nicht der Lebendfang (Polizei, Haft: Ermittlung 14 bis 16); Fesselung eines Menschen ist ein rechtlich relevanter Akt.
6. **Nicht in den Quellen:** Fesselmechanik im Nahkampf, Betäubungswaffen, Zeitfenster der Überwältigung (`CQ-70`, `CQ-71`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
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

## 3. Fallen im Kampf

Aus den Quellen folgt (🧩 ABGELEITET aus `COMBAT-DEC-FF-004`, `-006`; Kreaturen 13 §3):

1. **Eine Falle ist ein persistentes Weltobjekt** mit Zustand (intakt, beschädigt, unwirksam, ausgelöst), Besitzer und Vorbereitungsstand. Sie gehört der Welt, nicht dem Kampf.
2. **Vor dem Kampf vorbereitet:** Die Falle wird nicht im Gefecht gezeichnet (Vorlagen, Zeit, Material).
3. **Im Kampf wirkt sie nur, wenn sie wirksam ist** und das Wesen zur Mythologie passt (`K-CO-002`).
4. **Das Wesen kann sie erkennen, umgehen oder sabotieren** (`COMBAT-DEC-FF-006`); wiederholte Tricks können durchschaut werden (`M-PD` ‚Stealth…‘).
5. **Sie kann durch den Kampf beschädigt werden** (Staub, Blut, Wegschieben, Verwischen) – Entwurf (`COMBAT-DEC-FF-011`).

Beispiel der Quelle: Eine Falle in einer Scheune kann Wochen später intakt oder durch Reinigung zerstört sein (`48#134A`, Auslegung).

---

## 4. Fesselung und nichttödliche Überwältigung

**Beschlossen:** Aufgabegriffe können zu Abklopfen, Bewusstlosigkeit oder echter Verletzung führen; im tödlichen Straßenkampf kann anhaltendes Würgen töten (`COMBAT-DEC-NK-011`, `-012`). Kein eigener „Execute“-Befehl (`23#168A`, 💡).

**Nicht beschrieben:** Handschellen, Kabelbinder, Seile, Betäubungswaffen, Beruhigungsmittel, Zeit der Bewusstlosigkeit, Wiedererwachen, Fesselung von Wesen (Material gegen Mythologie), Fesselung im Koop.

**Prüfbare Mindestregel (🧩 ABGELEITET aus `COMBAT-DEC-NK-012`):** Eine Überwältigung, die bewusstlos machen soll, darf bei fortgesetztem Griff den Tod nicht ausschließen; die Grenze ist Körperzustand, nicht Skript.

---

## 5. Transport, Verwahrung, Verhör

Quellen: `K-CO-001`, `K-CO-007`, Kreaturen 13 §2 bis §5.

| Phase | Aussage | Offen |
|---|---|---|
| Transport | gefangene Kreaturen sicher zu geeigneten Einrichtungen bringen (Auslegung) | Fahrzeug, Risiko, Ausbruch |
| Verwahrung | an beliebigen Orten möglich; Bunker später; staatliche Stellen forschen geheim | `CQ-72` |
| Verhör | intelligente Wesen können verhört werden; Verhandlung möglich | Zeitaufwand, Wahrheitsgehalt |
| Ausbruch | systemische Fluchtversuche, Sabotage (Auslegung) | Regeln |
| Folgen | Story, Beziehungen, Moral, Spielwelt | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Bedienung des Fallenbaus (`CQ-70`) | A: Platzierungsmodus mit Vorlagen und Vorschau | passt zu `COMBAT-DEC-FF-005` | UI-Aufwand |
| | B: Kontextinteraktion am Ort (Kreide aufnehmen, Vorlage wählen) | nahe an der Handlung | wenig Kontrolle über Größe |
| Nichttödliche Mittel (`CQ-71`) | A: Nur Körpergriffe | einfach | wenig Optionen |
| | B: Zusätzlich Fesseln und Betäubungsmittel | taktisch reich | Rechts- und Balancefragen |
| Dauerhafte Verwahrung (`CQ-72`) | A: Nur ad-hoc-Fallen | geringer Aufwand | Trägt Forschung und Verhör nicht |
| | B: Ausbaubare Einrichtungen | trägt das Zielbild | Planung des Bunkers vorausgesetzt |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Persistenz | Falle als Weltobjekt mit Zustand |
| zur KI | Erkennen und Sabotieren |
| zum Nahkampf | Überwältigung, Fesselung |
| zum Recht | Festhalten, Festnahme |
| zum Koop | gemeinsame Vorbereitung, Autorität |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Falle wird durch NPC beschädigt | Zustand ändert sich, Ereignis in der Welt | 📋 (`K-CO-004`) |
| Wesen bricht aus | Folgen laut Gefängnis-/Fallenregel | 💡 |
| Falle in entladener Region | Zustand wird nachgeführt, nicht verloren | 📋 (`K-CO-004`) |
| Fesselung eines Wesens ohne passendes Material | wirkungslos mit Rückmeldung | 💡 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-70` | Bedienung und Umfang des Fallenbaus und der Fesselung im Einsatz | `COMBAT-DEC-FF-005`, `-011` | ja |
| `CQ-71` | Nichttödliche Mittel gegen Menschen und Wesen: Fesseln, Betäubung, Festnahme durch den Spieler | `COMBAT-DEC-FF-012`, `-013` | ja |
| `CQ-72` | Dauerhafte Verwahrung, Bunker, Netzwerk, rechtlicher Rahmen | `COMBAT-DEC-FF-014`, `-015` | ja |
