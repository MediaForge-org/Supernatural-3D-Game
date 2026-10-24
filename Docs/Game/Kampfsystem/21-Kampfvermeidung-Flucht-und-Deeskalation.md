# Kampfsystem 21: Kampfvermeidung, Flucht und Deeskalation

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul sammelt die Alternativen zum Kampf: Beobachten, Tarnung, Ablenkung, Einschüchterung, Verhandlung, Flucht, nichttödliche Lösungen. Die Quellen beschreiben diese Alternativen als gleichwertige Zugänge, aber kaum als Mechanik. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Optionen im Überblick](#3-optionen-im-überblick)
4. [Flucht](#4-flucht)
5. [Einschüchterung, Verhandlung, Ablenkung](#5-einschüchterung-verhandlung-ablenkung)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Nicht jede Begegnung ist ein Kampf.** Beobachten, Verfolgen, Belauschen, Fotografieren und Probennehmen sind gleichwertige Zugänge (`E-QA-009`, `K-GR-006`).
2. **Viele Ausgänge sind beschlossen:** töten, vertreiben, bannen, fangen, Fluch brechen, retten, verhandeln, überleben, evakuieren (`E-FA-009`).
3. **Menschen können sich ergeben, Dämonen verhandeln,** nicht jeder greift sofort an (`COMBAT-DEC-NK-017`, `COMBAT-DEC-KS-013`).
4. **Der Spieler kann fliehen;** schwere Verletzung erlaubt Flucht oder Anruf (`COMBAT-DEC-BW-008`). Wie Flucht mechanisch aussieht, ist **nicht beschrieben**.
5. **Es gibt keine Mechanik für Überredung, Drohung oder Verhandlung im Konflikt;** das Dialogsystem ist klassisch angelegt, erweiterbar (`E-ZE-012`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
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

## 3. Optionen im Überblick

| Option | Wirkung (nach Quellen) | Risiko | Status der Mechanik |
|---|---|---|---|
| Beobachten, Verfolgen, Belauschen | Wissen ohne Kontakt | Entdeckung | beschlossen als Zugang |
| Tarnen und Ablenken | Kampf vermeiden | Verstecke nie sicher | beschlossen (Prinzipien) |
| Einschüchtern | Gegner fliehen, erstarren, ergeben sich | Gegenreaktion | Prinzip beschlossen, Schwellen offen |
| Verhandeln | Dämonen, intelligente Wesen | Täuschung | Prinzip beschlossen, Mechanik offen |
| Überwältigen ohne Töten | Bewusstlosigkeit, Gefangennahme | Rechtsfolgen, Tod bei Fortsetzung | Teile beschlossen |
| Fangen | Fallen, Lebendfang | Ausbruch | beschlossen (Modul 17) |
| Fliehen | Distanz, Sichtbruch | Verfolgung | Prinzip, Mechanik offen |
| Rückzug mit Hilfe | Anruf, Begleiter | Zeitverlust, Folgen | Richtung |

---

## 4. Flucht

**Was die Quellen sagen:** Man kann fliehen; ein starker Gegner kann verfolgen; Verletzung erleichtert die Verfolgung; manche Gegner geben nicht auf. Eine Flucht über Fahrzeug oder Gelände ist nicht beschrieben.

**Prüfbare Eigenschaften (🧩 ABGELEITET):**

1. Eine Flucht verlangt Ausdauer und Bewegung; Verletzung (Bein) schränkt sie ein (`COMBAT-DEC-NK-026`, `COMBAT-DEC-BT-011`).
2. Gegner folgen nur der Information, die sie haben (letzte bekannte Position, Spuren); ein Sichtbruch kann die Verfolgung zur Suche machen (`COMBAT-DEC-KI-007`).
3. Auf Grounded ist die Suche länger, aber nicht allwissend (`COMBAT-DEC-KI-023`).
4. Fliehen ist nie unmöglich gemacht durch einen künstlichen Sperrzustand (Prinzip ‚kein permanenter Betäubungs-Lock‘, `COMBAT-DEC-NK-019`).

---

## 5. Einschüchterung, Verhandlung, Ablenkung

- **Einschüchterung** (Mensch): abhängig von Persönlichkeit, Kampferfahrung, Verletzungen, Bewaffnung, Verbündeten, Gesehenem, Haltung des Spielers und eventuell Ruf (`23#223A`, Entwurf, vom Entwickler bejaht: `23#222E` Zusatz).
- **Zeichen der Furcht** (Entwurf): Stimme, Zittern, Zurückweichen, unsicherer Blick, Waffe senken, Fluchtversuch, Hände heben, tatsächliche Aufgabe.
- **Verhandlung** (Wesen): Prinzip beschlossen (`K-HW-004`, `K-CO-007`, `N-NL-005`); wer was weiß und was ein Wesen anbietet, ist Sache der Art.
- **Ablenkung:** Geräusch und Wurf lenken auf Positionen; wiederholte Tricks werden erkannt (`COMBAT-DEC-KI-012`).

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Fluchtmechanik (`CQ-88`) | A: Nur Bewegung und Sichtbruch | einfach, passt zu `COMBAT-DEC-NK-019` | wenig Tiefe |
| | B: Zusatzwerkzeuge (Türen verbarrikadieren, Hindernisse, Fahrzeug) | taktisch | Aufwand, Balance |
| | C: Fluchtwege mit Kosten (Ausrüstung zurücklassen) | Konsequenz | Frust |
| Soziale Deeskalation (`CQ-89`) | A: Dialogwahl vor Kampfbeginn | einfach | begrenzt |
| | B: Kontinuierlich mit Drohung und Haltung | tief | komplex, Animations- und Audiobedarf |
| Verhandlung mit Wesen (`CQ-90`) | A: Skriptbare Fälle | kontrolliert | wenig systemisch |
| | B: Regelbasierte Angebote je Art | systemisch | Daten und Tests |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)) | Furcht, Verfolgung, Suche |
| zu Dialog ([NPCs 20](../NPCs/20-Dialog-und-Sichtbarkeit-der-Beziehungen.md)) | Gespräche im Konflikt |
| zu Polizei ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) | Aufgabe, Festnahme |
| zu Begleitern | Einsatzplan, Handzeichen |
| zu Fallen ([Modul 17](17-Fallen-Fesselung-und-Lebendfang.md)) | Fangen |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Gegner ergibt sich, Spieler greift weiter an | Folgen (Zeugen, Ruf, Recht) | 💡 |
| Verhandlung scheitert | Kampf oder Flucht, keine Sackgasse | 💡 |
| Flucht in unentladene Region | Verfolger-Zustand wird nachgeführt | 💡 |
| Begleiter flieht gegen Plan | nur bei offensichtlicher Gefahr (`N-CO-011`) | 📋 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-88` | Fluchtmechanik: Mittel, Ausdauer, Fahrzeug, Verlust von Verfolgern | `COMBAT-DEC-DE-009`, `-013` | ja |
| `CQ-89` | Soziale Deeskalation im Konflikt: Drohen, Überzeugen, Beruhigen | `COMBAT-DEC-DE-014` | ja |
| `CQ-90` | Verhandlung mit Wesen als Mechanik | `COMBAT-DEC-DE-004` | ja |
| `CQ-91` | Fortschritt und Belohnung ohne Kampf; Wert eines gewaltfreien Ausgangs | `COMBAT-DEC-DE-015` | ja |
