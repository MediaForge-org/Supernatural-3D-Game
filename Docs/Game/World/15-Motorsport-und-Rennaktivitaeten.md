# Welt 15: Motorsport und Rennaktivitäten

> [!IMPORTANT]
> **Es gibt keine Serienliste, keine Regelwerke und keine Streckenverläufe.** Belegt sind Disziplinen, Tiefenanspruch, Mindestzahlen und Standort-Punkte der Referenzwelt. Nichts ist implementiert. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (MO).

Teil der Welt-Dokumentation ([Index](README.md)). Fahrphysik: [Modul 14](14-Fahrphysik-und-Fahrzeuginteraktion.md); Orte: [Modul 08](08-Raeumliche-Allokation-Orte-und-Aktivitaeten.md).

**Inhalt**

1. [Stellenwert](#1-stellenwert)
2. [Disziplinen Auto](#2-disziplinen-auto)
3. [Disziplinen Motorrad](#3-disziplinen-motorrad)
4. [Tiefe der Simulation](#4-tiefe-der-simulation)
5. [Strecken und Standorte](#5-strecken-und-standorte)
6. [Straßenrennen, Treffen, Track Days](#6-straßenrennen-treffen-track-days)
7. [Veranstaltungen im Stadtraum](#7-veranstaltungen-im-stadtraum)
8. [Wetten, Karriere, Wirtschaft](#8-wetten-karriere-wirtschaft)
9. [Standorte der Referenzwelt](#9-standorte-der-referenzwelt)
10. [Nicht erfinden: Serien und Namen](#10-nicht-erfinden-serien-und-namen)
11. [Offene Punkte](#11-offene-punkte)

---

## 1. Stellenwert

„Very high priority“ (M-WB17 Block 7). Autorennen sind ‚sehr wichtig‘ (23#511E); Motorsport soll ‚so genau oder noch genauer als Fußball/Eishockey‘ sein (23#1035E Nr. 32). Autotreffen, Straßenrennen und Track Days sind wichtig (M-WB17 Block 5).

---

## 2. Disziplinen Auto

(M-WB17 Block 7, 📋 Anforderung) Formel-/Top-Rundkurs, GT, Tourenwagen, Sportwagen, Rallye, Bergrennen, Drag, Oval/Stock Car, Dirt Oval, Offroad, Rallye Raid, Kart, Drift, Autocross, Time Attack, Track Days.

---

## 3. Disziplinen Motorrad

(M-WB17 Zusatz, 📋; auf Entwickler-Hinweis ‚bei Motorsport soll es auch sowas wie MotoGP geben‘, 23#1037E): MotoGP-artige GP-Meisterschaft, Prototyp-Zubringerklassen, Superbike, Supersport, nationale/regionale Straßenrennen, Motocross, Supercross, Enduro, Hard Enduro, Rallye Raid, Hillclimb, Drag Bikes, Flat/Dirt Track, Supermoto, Club-/Amateurrennen, Track Days. Spieler nimmt später teil; Tiefe vergleichbar mit Auto-Motorsport.

---

## 4. Tiefe der Simulation

Teams, Hersteller, Fahrer/Fahrerinnen, Qualifying, Reifen, Wetter, Setups, Schaden, Boxenstopps, Entwicklung, Zuverlässigkeit, Strafen, Rekorde, Wetten (M-WB17). Rennphysik realistisch statt Arcade, nicht pay-to-win (M-PD). Regen und Gewitter wichtig. Entwickler: Rallyes sind **mehrere getrennte Veranstaltungen mit 10–20 Etappen**, nicht eine einzige (35#566E).

---

## 5. Strecken und Standorte

| Stand | Aussage | Status | Quelle |
|---|---|:---:|---|
| Anforderung | mindestens fünf Rennstrecken, realistische Klassen und Homologation | 📋 | M-PD; M-B38 |
| Entwickler 28.08.–01.09. | F1-Strecke plus fünf weitere Rennstrecken; Rallye bergauf wie Pikes Peak | 📋 | 23#511E; 23#823E; 23#822E |
| Entwickler 16.09. (Chat 35) | Ungesicherte Schotter-Bergstrecke „wie Pikes Peak“ ohne Absperrungen (Wunsch). Ob sie mit der Pikes-Peak-Bergrallye identisch ist, ist offen (OF-58; W-MO-014). Offen: Sicherheit und Absperrung; die Ausgestaltung (Gefahrenstufen, Schadenssystem) ist 💡 und nicht festgelegt. Siehe §7. | 📋 Wunsch / ❓ | 35#568E; 35#570E; W-MO-014; OF-58 |
| WORLD_700 | sechs Standorte | 🕰️ | G7-GPKG |
| 09.09. WORLD_2000 | 16–22 Standorte (Hierarchie) | 🕰️ | 35#96A; 35#97E |
| Oktober-Aufträge | ‚sechs Motorsportsysteme‘ | 🕰️ | 30#265E |
| Kandidat 18./19.10. | **keine Aussage** | ❓ | – |

Die 09.09.-Hierarchie: 1 internationaler GP-Circuit, 2 größere Road Circuits, 2 Club-Circuits, 3 Ovale, 2 Drag Strips, 2 Rally Parks, Offroad-Komplex, Hillclimb und kleinere Eventstrecken, Motorrad-Anlagen (Motocross, Supercross, Enduro, Flat Track, Supermoto), Drift, Autocross, Time Attack, Test Park, Kartstrecken. Viele Anlagen tragen mehrere Disziplinen; Rallye, Straßenrennen, Autotreffen und manche Hillclimbs nutzen bestehende Straßen. Konflikt: [K8](21-Konflikte-und-Widersprueche.md#k8-flughäfen-und-motorsportstandorte).

Flächen (💡, 35#92A): GP-Circuit 3–4 km², Road Circuit 2–3, Rally Park 3–4, Hillclimb-Anlage 1–2, Drag Strip 0,5–1, Test Park 2–3.

---

## 6. Straßenrennen, Treffen, Track Days

Illegale/halblegale Rennen, Straßenrennen und Autotreffen sind Teil des Spiels (M-WB17 Block 5 und 8: Untergrund-Rennen, Wetten auf Untergrundkämpfe). Rennen mit Polizei und Zeugen folgen den Rechts- und Beweisregeln (M-PD §Cash/law).

---

## 7. Veranstaltungen im Stadtraum

Formel-1-Stadtkurse und Rallyes in der Stadt, Straßen werden am Wochenende oder zu Ereigniszeiten abgesperrt (35#562E). Konsequenzen: NPC-Verkehr, Parkplätze, ÖPNV, Hotels, Wetten (M-WB17 Block 7).

**Ergänzung aus Chat 35 (16.09.):** Der Entwickler will „mehrere strecken“, einige „durch den wald mit mehreren etappen“, und findet Stadtkurse mit Absperrungen passend (35#564E, W-MO-015). Für eine Schotter-Bergstrecke „wie pikespeak“ **ohne Absperrungen** gilt: Wer zu schnell ist, riskiert das Leben (35#568E, W-MO-014). Der Assistent entwirft dazu ein temporäres Event-System (Aufbau über Tage, Umleitungen, Folgen für Geschäfte und Hotels, Abbau), mehrere benannte Rallyes mit Service Park und Verbindungsetappen sowie eine Hillclimb mit Gefahrenstufen (35#563A bis 35#569A). Diese Ausgestaltung ist 💡; die Rallye-Namen sind Platzhalter. Ob der Hillclimb dieselbe Strecke wie der Pikes-Peak-Anstieg aus 28.08. ist, bleibt offen (OF-58). Die Aussage des Assistenten, die „6 gelockten Anlagen“ blieben bestehen, ist in K8 vermerkt und ist kein Beleg für einen Lock.

---

## 8. Wetten, Karriere, Wirtschaft

Der Spieler kann später teilnehmen (M-WB17). Motorsport hat Wettmärkte; Buchmacher sharp/soft (M-WB17 Block 3). Karriere und Ligenstruktur sind nicht spezifiziert.

---

## 9. Standorte der Referenzwelt

Gemessen im GeoPackage ([Modul 24](24-WORLD700-GIS-Referenz.md)): F1 Circuit (State D), Road Circuit (E), Rally Park (A), Mountain Hillclimb (C), Drag Strip (G), Motorsport Test Park (G). Zugehörige Zufahrten: F1 5,9 km, Road Circuit 6,2, Rally 5,2, Drag 1,0, Test Park 3,9 km. Hillclimb 18,9 km mit 17 Kehren.

---

## 10. Nicht erfinden: Serien und Namen

Der Master nennt **keine** benannten Rennserien, Teams, Hersteller oder Meisterschaftsstrukturen; ‚MotoGP-artig‘ und ‚Pikes Peak‘ sind Vergleiche. Namen realer Serien dürfen nicht als Spielinhalt übernommen werden (fiktive Marken: M-OD).

---

## 11. Offene Punkte

Streckenzahl und -lage; Serienstruktur; Klassenregeln; Homologation; Belohnungsökonomie; Fahrzeugliste je Klasse; Zuschauerbereiche; Rennwochenenden im Kalender; Teilnahmebedingungen; Sicherheitsausrüstung und Verletzungen auf der Strecke.
