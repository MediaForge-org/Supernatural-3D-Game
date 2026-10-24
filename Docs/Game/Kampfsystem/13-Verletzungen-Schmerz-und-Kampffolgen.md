# Kampfsystem 13: Verletzungen, Schmerz und Kampffolgen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul sammelt die Entscheidungen zu Folgen von Verletzungen: Blutung, Schmerz, Schock, Adrenalin, Infektion, Behandlung, Heilung, Diagnose und langfristige Folgen. Verläufe und Zeitstufen sind **Entwurf (💡)**. Es gibt keine Heilzeiten, Blutungsraten oder Schwellen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Die Entstehung von Wunden steht in [Modul 12](12-Treffermodell-und-Koerperphysik.md); Bewusstlosigkeit, Tod und Rettung in [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Systemzustände und ihre Wahrnehmung](#3-systemzustände-und-ihre-wahrnehmung)
4. [Behandlung auf drei Ebenen](#4-behandlung-auf-drei-ebenen)
5. [Verlauf und Heilung (Entwurf)](#5-verlauf-und-heilung-entwurf)
6. [Langfristige Folgen und Spannung zur Gliedmaßendarstellung](#6-langfristige-folgen-und-spannung-zur-gliedmaßendarstellung)
7. [Gegenüberstellung offener Lösungsansätze](#7-gegenüberstellung-offener-lösungsansätze)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Verletzung ist ein Kernpfeiler,** kein optionales System (Gameplay-Systeme §4). Es gibt keinen Energy-Drink und keine Sofortheilung; Heilung bedeutet Behandlung und Zeit (`M-PD`, `23#60E`).
2. **Der Spieler muss Blutverlust, Schmerz, Schock und Adrenalin spüren,** über Animation, Audio, Atmung, Sicht, Zittern, Bewegung, Zielen, Stimme, Koordination und Handlungsfähigkeit, nicht über Zahlenanzeigen (`M-PD`, `23#104E` P5).
3. **Behandlung hat drei Ebenen:** Feldversorgung für leichte Verletzungen und Stabilisierung, ein medizinisch geschulter Jäger oder Arzt für manche mittelschweren Fälle, Krankenhaus für schwere (`M-PD`, `23#104E` C).
4. **Zeit vergeht in Stufen:** akut, stabilisiert, heilend, erholend, erholt; eine erneute Verletzung einer noch nicht ausgeheilten Region erreicht schwere Zustände schneller (`M-PD`, `23#104E` D).
5. **Die Zahlen, Medikamentenlisten und die Krankenhauswirtschaft sind offen** (Entscheidungsstatus §5.3, `M-OD`).
6. **Spannung:** Die Gore-Darstellung nennt den Verlust von Gliedmaßen als plausibles Ergebnis, das kanonische Addendum vom 05.09.2026 lässt Amputation und Prothesen nur über erzählte Story zu (`CK-04`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
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

## 3. Systemzustände und ihre Wahrnehmung

**Systemzustand (Mindestmenge nach `M-PD`):** Blutvolumen, Blutungsrate, Schmerz, Schock, Bewusstsein, Atmung, Adrenalin, Beweglichkeit, Gliedmaßenfunktion, Infektionsrisiko.

| Zustand | Wirkung nach den Quellen | Spürbar über (nach `M-PD` / Entwurf) |
|---|---|---|
| Blutverlust | Schwäche, Koordinationsprobleme, Tunnelblick, Bewusstseinsverlust, Tod (`23#103A` P5 Entwurf, `23#104E` P1 Pflicht der Arterien) | Sicht, Zittern, Atmung, Bewegung |
| Schmerz | beeinflusst Zielen, Nachladen, Sprint, Nahkampf, Konzentration, Geräusche, Interaktion, Fahren | Stimme, Atmung, Haltung |
| Schock | Brücke zwischen Verletzung und Bewusstlosigkeit; Stufen (Entwurf: Stabil → Gestresst → Schock → schwerer Schock → bewusstlos, `23#100A` §8) | Sicht, Atmung, Zittern |
| Adrenalin | unterdrückt Schmerz zeitweise; Rückschlag danach | Präzision, Zittern |
| Atmung | Atemprobleme (zum Beispiel Lunge) | Atemgeräusch, Ausdauer |
| Beweglichkeit, Gliedmaßenfunktion | Hinken, Sprintgrenze, Waffenführung, Balance | Animation |
| Infektionsrisiko | verzögerte Verschlechterung | zunehmende Beschwerden, später Befund |

**Wichtig für die Oberfläche:** Der Spieler sieht keine ‚Laborwerte‘. Das Spiel zeigt keine Meldung wie ‚rechte Lunge zu 34 Prozent beschädigt‘; der Charakter weiß nicht automatisch, was intern verletzt wurde (`23#103A` P11–P12, Zustimmung `23#104E` P11–P12). Ein verständliches medizinisches Interface ist geplant (`23#85A` §4 Entwurf, vom Entwickler nicht ausdrücklich bestätigt; Frage `CQ-55`).

---

## 4. Behandlung auf drei Ebenen

| Ebene | Typische Fälle (`M-PD`) | Mittel (Quelle) | Offen |
|---|---|---|---|
| Feld | leichte Verletzungen, Stabilisierung | Druck, Verband, Reinigung, Desinfektion, Schiene (`23#85A` §5 als Beispiele) | Gegenstandsliste, Dauer, Ort |
| Medizinisch geschulter Jäger oder Arzt | manche mittelschwere Fälle | Erfahrung, Ausrüstung, Medikamente | Verfügbarkeit, Ausbildung von Begleitern |
| Krankenhaus | schwere innere Verletzungen, komplizierte Schüsse, schwere offene Brüche, schwere Verbrennungen, kritische Infektionen, massiver Blutverlust | Operation, Diagnose, Überwachung | Kosten, Dauer, Wirtschaft, Rechtsfolgen (Schussverletzung wird gemeldet?) |

**Aus den Quellen ableitbar (🧩 ABGELEITET):**

1. Die Zuordnung eines Falls zur Ebene ist **nicht** an die Zahl der Treffer gebunden, sondern an Strukturen und Zustand (`COMBAT-DEC-TK-006`, `COMBAT-DEC-VL-003`).
2. Ein Krankenhausaufenthalt verbraucht Weltzeit und hat Weltfolgen (`COMBAT-DEC-VL-019`, [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md)).
3. Die Feldbehandlung verändert den Behandlungszustand der Wunde, aber nicht deren Tiefe (Wundkanal bleibt Teil des Wundeintrags, [Modul 12](12-Treffermodell-und-Koerperphysik.md)).
4. Eine Schussverletzung im Krankenhaus ist ein **Ermittlungs- und Rechtsereignis** (Meldung, Fragen), soweit das Rechtssystem dies vorsieht ([Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md)); die Regel ist nicht entschieden (`CQ-81`).

---

## 5. Verlauf und Heilung (Entwurf)

> **💡 VORSCHLAG.** Der Verlauf übersetzt `COMBAT-DEC-VL-005` in eine Zustandsfolge. Zeitdauern sind Konfiguration.

```text
Wundeintrag erzeugt (akut)
  -> Gefahr beseitigt? Blutung gestoppt?  --nein--> Blutung geht weiter (Blutvolumen sinkt)
  -> stabilisiert (Blutung kontrolliert, keine unmittelbare Lebensgefahr)
  -> heilend  (Behandlung vorhanden, Ruhe erforderlich; Infektionsrisiko aus Behandlungsstand)
  -> erholend (Funktionen kehren zurück; Rückfall bei Überlastung)
  -> erholt   (Region voll belastbar; Narben/Folgen nach Regel)
Rückwärtspfeile: erneute Verletzung derselben Region, Infektion, Überlastung -> schnellerer Abstieg
```

**Prüfbare Eigenschaften (🧩 ABGELEITET aus `COMBAT-DEC-VL-001`, `-005`):**

1. Kein Zustand springt von ‚akut‘ nach ‚erholt‘ ohne Zeit und Behandlung (kein Sofortheilen).
2. Eine erneute Verletzung in ‚heilend‘ führt schneller in schwere Zustände als dieselbe Verletzung in ‚erholt‘.
3. Gespeicherte Verläufe laufen nach dem Laden deterministisch weiter.
4. Zeit außerhalb des Spiels folgt der Regel der Weltzeit (offen, [NPC-Konflikt C-N3](../NPCs/27-Konflikte-und-offene-Fragen.md)).

---

## 6. Langfristige Folgen und Spannung zur Gliedmaßendarstellung

**Was feststeht:**

- Schwere Verletzungen können **temporäre** Funktionseinschränkungen verursachen; es gibt keinen vollständigen Rehabilitationssimulator (`M-CA0905`).
- Bleibende Narben und Verletzungen existieren bei Wesen (`K-KO-007`) und können bei Menschen eine Folge von Niederlagen sein (`COMBAT-DEC-VL-019`).
- Gore stellt für erwachsene Menschen auch Verlust von Gliedmaßen dar (`COMBAT-DEC-VL-013`).
- Amputation und Prothesen sind nur durch erzählte Story vorgesehen (`COMBAT-DEC-VL-012`).

**Die Spannung `CK-04`:** Wird dem Spieler im Kampf eine Hand abgetrennt, ist offen, was **nach** dem Kampf gilt: dauerhafter Funktionsverlust (widerspricht ‚nur über erzählte Story‘), eine Darstellung ohne bleibende Folge (widerspricht dem Anspruch ‚kein Plot-Armor und Gore ist an den tatsächlichen Verletzungszustand gebunden‘) oder eine Beschränkung der Gliedmaßen-Darstellung auf Gegner. Die Quellen lösen das nicht; beide Aussagen stehen in Modul 28.

---

## 7. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Gliedmaßenverlust beim Spieler (`CQ-53`) | A: Nur Gegner und NPCs können Gliedmaßen verlieren; der Spieler erleidet schwerste Funktionsverluste ohne Abtrennung | vermeidet Dauerfolge-Konflikt | Spieler ist anders als NPCs (`COMBAT-DEC-GP-007`) |
| | B: Spieler kann Gliedmaßen verlieren; Folge ist eine bleibende erzählte Verletzung | konsequent | Prothese/Amputation nur Story (`COMBAT-DEC-VL-012`) |
| | C: Kampf-Abtrennung ist Gore-Darstellung eines schweren Zustands, die Folge folgt dem Verletzungsmodell (temporäre Funktionseinschränkung) | passt zu `M-CA0905` | wirkt inkonsequent |
| Behandlungsoberfläche (`CQ-55`) | A: Menü mit Gegenständen und Körperregionen | klar | Menüaufwand (`COMBAT-DEC-VL-020`) |
| | B: Kontextinteraktion am Körper (Druck, Verband) mit wenigen Eingaben | passt zu ‚verständliche Bedienung‘ | schwer mit mehreren Wunden |
| | C: Hybrid mit Schnellbehandlung und Detailansicht | zugänglich | doppelter Aufwand |
| Infektionsmodell (`CQ-54`) | A: Wahrscheinlichkeit pro Wunde nach Faktoren | einfach | wenig Dramatik |
| | B: Verlauf über Zeitstufen mit Behandlungsfenster | glaubwürdig, spielbar | mehr Zustand |
| Zeitkompression (`CQ-51`) | A: Heilzeiten an Spielzeit gebunden | konsistent | lange Wartezeiten |
| | B: Heilzeiten an Weltzeit gebunden (Schlaf, Reisen, Zeitsprung) | glaubwürdig | Abhängigkeit von Zeitregeln (offen) |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Treffermodell ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) | Wundeinträge |
| zu Tod und Rettung ([Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md)) | Bewusstlosigkeit, Krankenhaus |
| zur Ausdauer ([Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md)) | Belastung, Adrenalin |
| zu NPCs ([Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md), [NPCs 14](../NPCs/14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md)) | Verletzungen von NPCs (genesen, Folgen bei wichtigen) |
| zu Ermittlung und Recht | Verletzungsbefund, Meldung im Krankenhaus ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) |
| zur Persistenz ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | Wundeinträge, Behandlungszustand, Verläufe |
| zur Animation ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) | Verletzungsschichten (Hinken, Arm, Rippen, Zittern, Atmung) |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Verletzungszustand je Spieler, Hilfe durch andere |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Behandlung einer nicht vorhandenen Wunde | Ablehnung mit Grund | 💡 |
| Mehrere Behandler gleichzeitig | eine Behandlung pro Wunde und Zeit, die anderen erhalten Grund | 💡 |
| Schmerzmittel und Adrenalin überlagern sich | Wahrnehmung verschoben, Zustand unverändert | 🧩 |
| Laden zwischen Verband und Wirkung | Wirkung läuft nach dem Laden weiter | 💡 |
| Mod fügt Wundtyp hinzu | additiv, Basisverhalten unverändert | 💡 (`N-KO-010`) |
| Zeitsprung während Heilung | Fortschreiben nach Weltzeitregel (offen) | ❓ |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-51` | Heilzeiten und Zeitkompression: Verhältnis von Spielzeit, Weltzeit, Schlaf, Reisen, Zeitsprüngen | `COMBAT-DEC-VL-005`, `-021`, `-023` | ja |
| `CQ-52` | Medikamente, Behandlungsgegenstände, Diagnose- und Krankenhauslisten, Krankenhauswirtschaft; Zuordnung einzelner Fälle zu den Behandlungsebenen | `COMBAT-DEC-VL-003`, `-004`, `-022` | ja |
| `CQ-53` | Gliedmaßenverlust und bleibende Folgen beim Spieler (Spannung zu `M-CA0905`) | `COMBAT-DEC-VL-012`, `-013`, `-019`, `CK-04` | ja |
| `CQ-54` | Tiefe des Infektionsmodells (Faktoren, Zeitstufen, Behandlungsfenster) | `COMBAT-DEC-VL-014` | ja |
| `CQ-55` | Behandlungsoberfläche und Anzeige des Befunds (Menü, Kontextinteraktion, Diagnoseanzeige, Aufwand) | `COMBAT-DEC-VL-006`, `-009`, `-020` | ja |
