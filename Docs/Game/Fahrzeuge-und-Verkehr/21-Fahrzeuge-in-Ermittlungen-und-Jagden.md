# Fahrzeuge und Verkehr 21: Fahrzeuge in Ermittlungen und Jagden

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass Fahrzeuge und Kennzeichen Beweisquellen sind, dass der Kofferraum die mobile Ausrüstungsbasis ist, dass das Fahrzeug in der Einsatzplanung eine Rolle hat und dass Wesen mit menschlicher Identität Fahrzeuge besitzen können. Die Rolle des Fahrzeugs im Fallablauf, bei Observation und bei Monsterkontakt ist nicht ausgearbeitet. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Eigentümer der Themen: [Ermittlung](../Ermittlung/README.md) (Spuren, Zeugen, Fälle), [Kreaturen](../Kreaturen/README.md) (Wesen, Ebenen), [Kampfsystem](../Kampfsystem/README.md) (Jagd, Fallen, Rettung).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Fahrzeug als Spur](#2-fahrzeug-als-spur)
3. [Fahrzeug im Fallablauf](#3-fahrzeug-im-fallablauf)
4. [Fahrzeug in der Jagd](#4-fahrzeug-in-der-jagd)
5. [Fahrzeuge in anderen Ebenen](#5-fahrzeuge-in-anderen-ebenen)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Beweisquelle | Fahrzeug und Kennzeichen sind Beweisquellen | `E-RE-002` |
| Wiedererkennung | Spieler, Fahrzeug, Kleidung und Maskierung können wiedererkannt werden | `E-ZE-003`; `E-SP-011` |
| Fallakte | Verdachtsgrade und Fahrzeugwechsel in der Fallakte (Richtung) | `E-QA-020` |
| Durchsuchung | Fahrzeugdurchsuchung nach Regeln (Wunsch) | `E-PO-005` |
| Spuren | Reifenspuren, Reifenprofile bei Fahrzeugfällen | [Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md) (`M-CA0905`) |
| Ausrüstung | Kofferraum als mobile Ausrüstungsbasis, Safehouse als größeres Lager | `W-FZ-001`; `M-PD` |
| Ablauf | Zu den Folgen eines Fehlschlags können gehören: Zeitverlust, neue Opfer, Ausrüstung und Fahrzeug am Ort zurückgelassen, Krankenhaus | `M-PD` (Fallfolgen) |
| Verfolgung | Verfolgen mit Fahrzeugen ist eine Ermittlungsmethode | [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §6 (`M-LATE`) |
| Planung | Die Einsatzplanung mit Begleitern enthält Rollen, Eingänge, Positionen, Rückzug, Treffpunkt und die Fahrzeugrolle | `N-CO-010` |
| Wesen | Intelligente Wesen leben unter menschlicher Identität mit Wohnung, Beruf, Konto und Fahrzeug | `K-GS-001` |
| Reisen | Lange Fahrten enthalten dynamische Begleitergespräche | `M-PD` |
| Vertical Slice | Ankunft und Fahren stehen am Anfang des Vertical-Slice-Falls | `CLAUDE.md` §4 |

---

## 2. Fahrzeug als Spur

Das Fahrzeug ist ein Träger vieler möglicher Spuren. Welche das Spiel kennt, liegt bei der Ermittlungsdokumentation; dieses Modul nennt, was das **Fahrzeugsystem liefern muss**, damit sie existieren können.

> **💡 VORSCHLAG.**
>
> | Spur | Was das Fahrzeugsystem liefert | Eigentümer der Regel |
> |---|---|---|
> | Kennzeichen | Kennzeichenobjekt, Zuordnung zu Fahrzeug und Eigentümer (Modul 18) | Ermittlung 05, 06 |
> | Reifenspuren | Reifenprofil und Fahrzeugklasse als Daten des Fahrzeugs | Ermittlung 05 |
> | Schadens- und Lackspuren | Schadenszonen (Modul 07) | Ermittlung 05, 11 |
> | Innenraum | Inventar und Insassenhistorie | Ermittlung 05, 11 |
> | Bilder von Kameras | Standort, Zeit und Kennzeichen sichtbar | Ermittlung 27 |
> | Fahrtdaten | Fahrzeughistorie (Standorte, Ereignisse) | Ermittlung 27 |
> | Zeugen | Wiedererkennung über Auffälligkeit und Kennzeichen | Ermittlung 12 |
>
> Die **Fahrzeughistorie** (`history_events` in Modul 03) speist die Ermittlung: Besitzer, Halter, Standorte, Unfälle und Reparaturen. Wie tief das Spiel digitale Fahrtdaten simuliert (Navigation, Fahrzeugortung, Telematik), ist nicht belegt (`CQ-88`).

---

## 3. Fahrzeug im Fallablauf

Die Quellen beschreiben den Vertical-Slice-Fall als Folge: Ankunft und Fahren, Dialog, Ermittlung, Recherche, Vorbereitung, Konfrontation, Folgen (`CLAUDE.md` §4). Das Fahrzeug ist darin Transport, Lager und Ort.

> **💡 VORSCHLAG.** Aus den beschlossenen Teilen ergeben sich diese Fahrzeugrollen im Fall:
>
> - **Anreise:** Fahrzeit ist ein Teil des Falls und der Figurenentwicklung (Gespräche, `W-FZ-001`).
> - **Ausrüstungsbasis:** Der Kofferraum enthält, was für den Einsatz gewählt wurde (Modul 06). Was dort fehlt, fehlt im Einsatz.
> - **Abgestelltes Fahrzeug als Teil des Tatorts:** Es bleibt am Ort stehen (`W-FZ-016`, 🧭), kann beschlagnahmt, durchsucht oder vom Gegner genutzt werden.
> - **Verfolgung von Verdächtigen:** Fahren hinter einem Fahrzeug als Ermittlungsmethode (Welt 13 §6). Das hängt an Verkehrs-KI und Fahrphysik (Module 04, 13).
> - **Zeitdruck und Rückweg:** Fahrzeit macht Abwägungen zwischen Orten.
>
> Observation aus dem Fahrzeug, Ortung und ähnliche Methoden sind in den Quellen nicht erwähnt (`CQ-88`).

---

## 4. Fahrzeug in der Jagd

Die Einsatzplanung enthält die Fahrzeugrolle (`N-CO-010`). Daraus folgt, dass das Fahrzeug ein Treffpunkt, Rückzugsweg und Transportmittel sein kann.

> **💡 VORSCHLAG.**
>
> - **Treff- und Rückzugspunkt:** Begleiter fahren mit eigenem Fahrzeug (`N-CO-005`) und können ‚dort treffen‘.
> - **Transport:** Verletzte (Weg ins Krankenhaus, [Kampfsystem 14](../Kampfsystem/14-Bewusstlosigkeit-Tod-und-Rettung.md)), Gefangene und gefangene Wesen ([Kampfsystem 17](../Kampfsystem/17-Fallen-Fesselung-und-Lebendfang.md)), Leichen und Tierkörper (`VEH-DEC-EI-003`).
> - **Schutz und Beschuss:** Fahrzeuge haben ballistische Strukturen (`VEH-DEC-ZV-006`); ob man aus Fahrzeugen schießen kann und was Fahrzeuge Wesen entgegensetzen, ist offen (`VEH-DEC-ZV-007`, `CQ-87`).
>
> Dass Monster Fahrzeuge angreifen, verfolgen oder behindern, ist in den Quellen nicht beschrieben (`CQ-87`).

---

## 5. Fahrzeuge in anderen Ebenen

Die Kosmologie der Kreaturen lässt zu, dass Ebenen **eigene Regeln** für Zeit, Licht, Raum, Elektronik, Fahrzeuge, Waffen und Kräfte haben, intern konsistent ([Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md)). Fahrzeuge sind dort ausdrücklich in der Parameterliste genannt.

> **💡 VORSCHLAG.** Das Fahrzeugsystem liest ein **Regelprofil der Ebene** als Eingabe (zum Beispiel ob Motoren laufen, wie Elektronik wirkt, ob es Straßen gibt). In der Hauptwelt ist das Profil das Standardprofil. Damit werden andere Ebenen kein Sonderfall im Code. Welche Fahrzeuge in den Reichen existieren, ist nicht belegt ([Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §9).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-EJ-001 | Fahrzeug und Kennzeichen sind Beweisquellen; Spieler, Fahrzeug, Kleidung und Maskierung können wiedererkannt werden | 📋 BESCHLOSSEN | `E-RE-002`; `E-ZE-003` | ERM NPC | – |
| VEH-DEC-EJ-002 | Die Fallakte führt Verdachtsgrade und Fahrzeugwechsel | 🧭 RICHTUNG | `E-QA-020` | ERM | – |
| VEH-DEC-EJ-003 | Fahrzeugdurchsuchung nach Regeln ist ein Wunsch | 🧭 RICHTUNG | `E-PO-005` | POL ERM | Regeln offen (`COMBAT-DEC-PO-023`) |
| VEH-DEC-EJ-004 | Zu den Folgen eines Fehlschlags können Ausrüstung und Fahrzeug am Ort zurückgelassen sein | 📋 BESCHLOSSEN | `M-PD` (Fallfolgen) | ERM FZ | – |
| VEH-DEC-EJ-005 | Verfolgen mit Fahrzeugen ist eine Ermittlungsmethode | 🧭 RICHTUNG | [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §6 (`M-LATE`) | ERM KI | – |
| VEH-DEC-EJ-006 | Die Einsatzplanung enthält die Fahrzeugrolle; Begleiter fahren mit eigenem Fahrzeug und treffen sich ‚dort‘ | 📋 BESCHLOSSEN | `N-CO-010`; `N-CO-005` | NPC FZ | – |
| VEH-DEC-EJ-007 | Intelligente Wesen können unter menschlicher Identität Fahrzeuge besitzen | 📋 BESCHLOSSEN | `K-GS-001` | KRE NPC | – |
| VEH-DEC-EJ-008 | Ebenen können eigene Regeln für Fahrzeuge, Elektronik, Zeit und Raum haben; das Durchqueren von Reichen soll sich deutlich vom normalen Fahren in der materiellen Welt unterscheiden | 🧭 RICHTUNG | [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md); `M-A0906` | KRE FZ | Fahrzeuge in Reichen nicht belegt |
| VEH-DEC-EJ-009 | Das Fahrzeugsystem liefert den Spurenarten der Ermittlung Daten (Kennzeichen, Reifenprofil, Schadenszonen, Innenraum, Standorte, Historie); die Regeln liegen bei der Ermittlung | 💡 VORSCHLAG | Ableitung aus `E-RE-002`, `W-FZ-016` | FZ ERM | Tiefe: `CQ-89` |
| VEH-DEC-EJ-010 | Die Fahrzeughistorie (Besitzer, Halter, Standorte, Unfälle, Reparaturen) speist die Ermittlung | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-KL-010` | FZ ERM | – |
| VEH-DEC-EJ-011 | Das Fahrzeug ist in der Jagd Treffpunkt, Rückzugsweg und Transportmittel (Verletzte, Gefangene, Leichen) | 💡 VORSCHLAG | Ableitung aus `N-CO-010`, `VEH-DEC-EI-003` | FZ NPC | – |
| VEH-DEC-EJ-012 | Das Fahrzeugsystem liest ein Regelprofil der Ebene als Eingabe; die Hauptwelt hat das Standardprofil | 💡 VORSCHLAG | Ableitung aus [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) | FZ KRE | – |
| VEH-DEC-EJ-013 | Angriffe, Verfolgung und Behinderung von Fahrzeugen durch Monster und die Wirkung von Fahrzeugen gegen Wesen sind nicht beschrieben | 🔍 UNBELEGT | – | KRE KMP | `CQ-87` |
| VEH-DEC-EJ-014 | Observation aus dem Fahrzeug, Ortung und digitale Fahrtdaten (Telematik) sind nicht erwähnt | 🔍 UNBELEGT | – | ERM | `CQ-88` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Monster und Fahrzeuge | `CQ-87` |
| Observation, Ortung, digitale Fahrtdaten | `CQ-88` |
| Tiefe der Fahrzeugspuren | `CQ-89` |
