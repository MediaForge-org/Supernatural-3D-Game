# Welt 03: Maßstab, Flächen und Proportionen

> [!IMPORTANT]
> **Keine dieser Zahlen ist eine Geometrie-Festlegung.** Das Modul trennt die Flächenangaben der verschiedenen Planungsphasen, damit sie nicht unbemerkt vermischt werden. Gerechnete Werte sind als **Rechnung** gekennzeichnet und keine Quellenaussage. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md); Konflikte: [Modul 21](21-Konflikte-und-Widersprueche.md).

**Inhalt**

1. [Die Flächenbegriffe](#1-die-flächenbegriffe)
2. [Der aktuelle Flächenaufbau des Kandidaten (19.10.)](#2-der-aktuelle-flächenaufbau-des-kandidaten-1910)
3. [Chronik der Hauptweltgröße](#3-chronik-der-hauptweltgröße)
4. [Chronik der Metropolis-Fläche](#4-chronik-der-metropolis-fläche)
5. [Was die 110 km² bedeuten](#5-was-die-110-km²-bedeuten)
6. [Anteile im Vergleich (Rechnung)](#6-anteile-im-vergleich-rechnung)
7. [Chronik weiterer Städte und Gewässer](#7-chronik-weiterer-städte-und-gewässer)
8. [Die 25 Kartengrößen](#8-die-25-kartengrößen)
9. [Entfernungen und Fahrzeiten](#9-entfernungen-und-fahrzeiten)
10. [WORLD_700 als Maßstabsreferenz](#10-world_700-als-maßstabsreferenz)
11. [Verwechslungsfallen](#11-verwechslungsfallen)

---

## 1. Die Flächenbegriffe

| Begriff | Bedeutung in den Quellen | Nicht gemeint |
|---|---|---|
| **Trockenes zusammenhängendes Festland** | Landfläche des Hauptkontinents ohne Seen, Reservoir, Meer (`D19-REQ`: „Contiguous dry mainland“) | Gesamtkarte, Bildfläche, Weltausschnitt |
| **Spielbare Fläche** (WORLD_700: *playable land*) | Land ohne Meer und ohne Binnengewässer; Sumpf zählt als Land (`G7-CLOSE`) | Hüllfläche |
| **Hüllfläche / Envelope** | Rechteck der Karte einschließlich Meer (WORLD_700: 733,8 km²) | Spielfläche |
| **Planungshülle** | Rechteck, in dem ein Prototyp gezeichnet wird (66 × 34 km = 2.244 km²) | Spielfläche; ausdrücklich ‚kein physischer Rand‘ (`30#232E`) |
| **Siedlungs-/Stadtfläche** | Zusammenhängende bebaute Fläche **einschließlich Vororten**, Straßen und örtlicher Freiflächen | Verwaltungsgebiet; Gebäudegrundfläche |
| **Footprint (Untergrund)** | horizontale Projektion der Stadt, nicht die summierte Geschossfläche (`D14-PS`) | Nutzfläche über alle Ebenen |

---

## 2. Der aktuelle Flächenaufbau des Kandidaten (19.10.)

Quelle: `D19-REQ` (Tabelle „Area accounting“), Status 🧭 (Arbeitsvorgabe für ein vorläufiges 2.200-km²-Preset).

| Bereich | Wert | Bedeutung |
|---|---:|---|
| Zusammenhängendes trockenes Festland | **2.200 km²** | Zielfläche ohne Wasser |
| davon Metropolis | **110 km²** | Teilmenge, ca. 5 % |
| Festland außerhalb von Metropolis | **2.090 km²** | 2.200 − 110 |
| Östliche bewohnte Insel | **80–100 km²** | zusätzliches Land, 90 km² als Testgeometrie |
| Lake Alder | ca. 25 km² | Seefläche (mit Alder Isle), kein Trockenland |
| Lake Meridian | ca. 5 km² | Seefläche |
| South Reservoir | ca. 7 km² | Seefläche |

**Rechnung** (keine Quellenaussage): Die drei benannten Binnengewässer ergeben 37 km². Weil sie **nicht** im trockenen Festland enthalten sind, läge Land plus diese Seen bei rund 2.237 km²; die Insel (80–100 km²) käme zusätzlich, das Meer ebenfalls. Die Quelle gibt keine Gesamtkartenfläche an.

Weitere Randbedingungen derselben Quelle: ca. 25 Größenvarianten; die 2.200 km² sind ‚keine universelle Weltgröße‘; Skalierung von Stadtflächen, Geländedetail und Siedlungszahl **je Preset**, nicht linear.

---

## 3. Chronik der Hauptweltgröße

| Datum | Aussage | Status heute | Quelle |
|---|---|:---:|---|
| bis 31.08. | ~200–300 km², Endgröße aus Reisezeiten ableiten | 🕰️ | M-PD §World |
| 31.08. | 300 km² zu klein, wenn eine Stadt 15–20 % beansprucht; Assistent: ~620 km² (Standard) bis 900–1.200 km² (Premium) | 🕰️ | 23#697E; 23#788E |
| 01.09. | WORLD_700-Linie: ca. 690,9 km² Spielfläche, abgeschlossen | 🕰️ (gesperrt) | G7-CLOSE; 30#167E |
| 01.09. | Zweite, größere Karte: „1500km² spielbare fläche“ | 🕰️ | 23#865E; 23#881E |
| 04.09. | Frage „noch größer als 1500 km²?“ | – | 23#1067E |
| 06.09.–10.09. | Standardwelt 1.500–1.800 km², Favorit ~1.650 | 🕰️ | M-CA0906; M-CU0908; M-W1500 |
| 09.09. | ~2.000 km² für die Hauptstory (ohne DLC und Reiche); „wenn es gut optimiert ist kann man das auch mit 20000 km² gut laufen haben, was ich nicht machen will“ | 🕰️ | 35#87E (Fragen 9 und 10) |
| 16.–27.09. | ~2.000 km² als weiches Ziel („soft target“) | 🕰️ | M-AD1015 |
| 27.09. | Karte wirkt ‚sehr klein für ihre Größe‘, Städte nicht riesig machen | 📋 (Kritik) | 35#876E |
| 29.09. | Prototyp 66 × 34 km (2.244 km²), ca. 2.100+ km² spielbar | 🕰️ | 30#232E |
| 01.10. | 2.150–2.300 km², Ziel ~2.200, Form unregelmäßig | 🕰️ | 30#255E |
| 04.10. | Größen-Tests von 2.200 bis 15.000 km², dann bis 1 Mio. km² | 📋 (Testwunsch) | 45#22E; 45#24E |
| 13.10. | ca. 2.000–2.200 km² offener Planungsrahmen | 🧭 | M-MS1013 |
| 18.10. | ~2.200 km² Kandidat, Fläche und Gelände nicht freigegeben | 🧭 | D18-MAP |
| 19.10. | 2.200 km² trockenes Festland plus Insel 80–100 km² | 🧭 | D19-REQ |

---

## 4. Chronik der Metropolis-Fläche

Jede Zeile nennt die Weltgröße, auf die sie sich bezieht. **Das Vermischen der Zeilen ist der Hauptfehler, den dieses Modul verhindern soll.**

| Datum | Metropolis | Weltgröße | Wer sagte es | Status heute | Quelle |
|---|---|---|---|:---:|---|
| 31.08. | „kann ruhig 20–30 km² sein“, später ‚vielleicht 30–40 km²‘ | 300 km² | Entwickler | 🕰️ | 23#703E; 23#705E |
| 31.08. | „sicher 60 km² oder mehr“ (Kritik am Bild) | 600+ km² | Entwickler | 🕰️ | 23#819E |
| 01.09. | 70 km² Megacity, danach 20–25 km² | 1.500 km² | Entwickler | 🕰️ | 23#881E |
| 03.09. | „bisschen weniger“ als eine frühere Zahl; Wien (2 Mio., 415 km²) als Maßstab | 1.500 km² | Entwickler | 🕰️ | 23#1009E |
| 04.09. | Megacity ~70–73 km² | 1.500–1.800 km² | Dokument | 🕰️ | M-LATE; M-W15R |
| 09.09. | „70–80 km²“, andere große Städte 30–40, dann bis 0,5 km² | ~2.000 km² | Entwickler | 🕰️ | 35#87E |
| 27.09. | „eine 200 km² Stadt ist bodenlos, 80 km² ist schon krass“ | – | Entwickler | 🕰️ (Obergrenzen-Aussage; durch die spätere Festlegung 110 km² vom 22.10. überholt, soweit sie ihr widerspricht) | 35#876E |
| 29.09. | 40–50 km² (Ziel ~45), Downtown 6–8 km² | 2.244 km² | Auftragstext | 🕰️ | 30#232E |
| 01.10. | Zahlen „von der Größe, die du gesagt hast, passt“ (Städte im Bild aber „zu groß“) | ~2.200 km² | Entwickler | 📋 (Bildkritik) | 35#935E; 35#919E |
| 01.10. | Assistent: bei Maßstab ×0,591 → 8,7–12,2 km² (ideal ~10,5) | ~2.200 km² | Assistent | 💡 | 35#926A |
| 04.10. | Metropolis 20–25 km² (Downtown 4–5), Major City A/B je 8–10 km² als „festgelegte“ Zielwerte; Zustimmung „ok passt ja“ / „ja mache das“ | 2.200 km² (Hülle ~65 × 34, dann ~67 × 34 km) | Assistent / Entwickler | 🕰️ | 45#3A; 45#4E; 45#6E |
| 09.10. | Assistentenvorschlag nach der Städtevision: Metropolis 150–300+ km², A 60–120, B 40–90 (Chat 48), Weltgröße 12.000 km² als Favorit | 10.000–15.000 km² | Assistent | 🕰️ | 48#42A; 48#44A |
| 09.10. | 350 km² (NYC/Chicago-Szenario) vom Assistenten als Test; Entwickler: nur anfühlen, nicht so groß | 2.200 km² | Assistent / Entwickler | 💡 / 📋 | 45#60A; 45#61E |
| 09.10. | Offene Testbereiche: 2.200 km² → Metropolis 20–25 km², Major Cities 8–10 km² | 2.200 km² | Assistent | 🕰️ | 45#62A; M-MS1013 |
| 11.10. | Bestehende GIS-Testwelt: Metropolis 22,5 km², Glenhaven 9, Ashford 9 | 2.200 km² | berichtet | 🕰️ | 45#216E |
| 13.10. | Vorschlag 70 km² (Suburbs inklusive), dann Frage „110 / 160 / …?“ | 2.200 km² | Assistent | 💡 | 45#282A; 45#284A |
| 18.10. | **~110 km²**, 20 Bezirke, Downtown ~20, Kern ~12 | 2.200 km² | Entwickler (laut Delta) | 🧭 | D18-MAP |
| 19.10. | 110 km², ca. 5 % des Festlands | 2.200 km² | Entwickler (laut Delta) | 🧭 | D19-REQ |
| 22.10. | **110 km² bestätigt:** zusammenhängendes Stadtgebiet einschließlich Vororten; nicht nur Innenstadt, nicht die Metropolregion; Teil der 2.200 km² (nicht zusätzlich) | 2.200 km² | Projektinhaber (Primärentscheidung) | 📋 | PE-E1022 |

WORLD_700 (gesperrt) zählt hier nicht mit: Die dort **gemessene** Metropolis-Hüllfläche beträgt 30,0 km² von 690,9 km² Spielfläche ([Modul 24](24-WORLD700-GIS-Referenz.md)).

---

## 5. Was die 110 km² bedeuten

**Entscheidung vom 22.10.2026 (📋, `PE-E1022`, neue Primärentscheidung des Projektinhabers, nicht im Master):** Metropolis umfasst **110 km² zusammenhängendes Stadtgebiet einschließlich seiner Vororte**, also den urbanen Kern, weitere Stadtviertel und zugehörige Vorortbereiche. Die 110 km² bezeichnen **nicht** ausschließlich die dicht bebaute Innenstadt und **nicht** die weiter gefasste Metropolregion mit eigenständigen umliegenden Siedlungen. Sie sind Teil der 2.200 km² Festland und kommen nicht zusätzlich hinzu. Konkrete Stadtteilflächen oder Stadtgrenzen sind damit **nicht** festgelegt. Der Konflikt K1 ist hinsichtlich der Flächenabgrenzung geklärt (W-ST-003).

Die folgenden Punkte zeigen die **Herleitung vor dieser Entscheidung** und bleiben als Geschichte stehen:

1. **Wortlaut der Frage (13.10.):** „Wie groß soll Metropolis insgesamt werden?“ und „Gemeint ist die zusammenhängende Stadt inklusive aller Suburbs, Downtown und Industriegebiete.“ (`45#284A`, wörtlich in „…“ nur dieser Satz; Voreinstellung der Auswahl war 110).
2. **Ergebnis der Antwort:** Die Antwort des Entwicklers auf diese Frage steht **nicht** in `45#` (der Chat endet mit der Frage); sie liegt seit dem 22.10. als Primärentscheidung vor. Der Master belegt 110 nur über `D18-MAP` („~110 km² metropolitan footprint“) und `D19-REQ` („Metropolis 110 km², subset of mainland“).
3. **Frühere Folgerung (Rechnung, durch `PE-E1022` bestätigt):** Am wahrscheinlichsten war 110 km² = Gesamtgebiet der zusammenhängenden Metropole mit Vororten, weil `D18-MAP` Downtown (20 km²) und Hochhauskern (12 km²) als Teile davon führt und die Frage genau so formuliert war. Die Quellen sagten nicht ausdrücklich, ob **Verwaltungsgrenzen** oder **Einzugsgebiet** gemeint sind; beides wäre eine andere Größe (`45#60A` weist darauf hin, dass reale Stadtflächen Verwaltungsgebiete sind).
4. **Aufteilung innerhalb der 110 km²:** Nur Downtown (20) und Kern (12) sind genannt; die übrigen 90 km² sind **nicht** auf Bezirke, Vororte, Industrie oder Parks verteilt. Die frühere 70-km²-Tabelle des Assistenten (Downtown 5, innere Viertel 14, gemischte 17, Vororte 23, Industrie/Hafen 7, Parks 4) bezieht sich auf die 70-km²-Variante und ist 💡.
5. **Frühere Trennung Stadtgebiet/Metropolregion:** Am 01.09. beschrieb der Assistent eine Megacity mit 70 km² (City Proper) und einen Metropolitan Belt außerhalb, dessen zusammenhängende Region „deutlich über 100 km²“ umfasst; der Entwickler antwortete „ja genau“ (`23#910A`, `23#911E`). Die Entscheidung vom 22.10. stellt klar, dass die 110 km² **nicht** die Metropolregion bezeichnen (W-ST-037, W-ST-003).
6. **Zwei offene Anschlussfragen** (die Abgrenzung selbst ist geklärt): (a) Wie wirken 110 km² in der Darstellung angesichts der Kritik vom 27.09./01.10. („Städte zu groß“)? ([Konflikt K1](21-Konflikte-und-Widersprueche.md#k1-metropolis-fläche)) (b) Ist Metropolis allein 5 % der Landfläche zusammen mit den übrigen Städten noch tragbar? (offen, [Modul 22](22-Offene-Fragen-Katalog.md)).

---

## 6. Anteile im Vergleich (Rechnung)

Die Tabelle rechnet Quellenwerte nur durch; sie ist keine Entscheidung.

| Stand | Metropolis | Bezugsfläche | Anteil |
|---|---:|---:|---:|
| WORLD_700, gemessene Hüllfläche | 30,0 km² | 690,9 km² | 4,3 % |
| WORLD_700, frühe V3-Spezifikation | 35,0 km² | 650 km² | 5,4 % |
| WORLD_1500-Anforderung 04.09. | 70 km² | 1.500 km² | 4,7 % |
| Entwickler 09.09. (Mitte 75 km²) | 70–80 km² | ~2.000 km² | 3,5–4,0 % |
| Auftrag 29.09. | 45 km² | 2.244 km² | 2,0 % |
| Testwelt 11.10. | 22,5 km² | 2.200 km² | 1,0 % |
| Test 09.10. (20–25 km²) | 20–25 km² | 2.200 km² | 0,9–1,1 % |
| **Kandidat 18./19.10.** | **110 km²** | 2.200 km² | **5,0 %** |

Beobachtung (Rechnung): Der 110-km²-Wert liegt **proportional im Bereich der älteren Entwicklerwerte** (4,3–5,4 %), nicht im Bereich der verkleinerten Testwerte (1–2 %). Daraus folgt kein Beschluss; es zeigt, dass der Wechsel von 20–25 auf 110 eine Rückkehr zur Entwicklerlinie von September ist.

Geometrie als Orientierung (Rechnung): 110 km² entsprechen einem Quadrat mit 10,5 km Seitenlänge oder einem Kreis mit 11,8 km Durchmesser; die Quelle verlangt ausdrücklich eine **organische, nicht kreisförmige** Form (`D18-MAP`). Downtown mit 20 km² entspräche 4,5 km Seitenlänge, der Kern mit 12 km² etwa 3,5 km.

---

## 7. Chronik weiterer Städte und Gewässer

### 7.1 Zweite und dritte Großstadt

| Datum | Wert | Quelle | Status |
|---|---|---|:---:|
| 01.09. | 20–25 km² | 23#881E | 🕰️ |
| 09.09. | 30–40 km², dann „20 km² 15 12 und so weiter“ | 35#87E; 35#89E | 🕰️ |
| 29.09. | 15–22 km² (Ziel 18–20) | 30#232E | 🕰️ |
| 09.10. | 8–10 km² (Test bei 2.200 km²) | 45#62A; M-MS1013 | 🕰️ |
| 11.10. | Glenhaven 9, Ashford 9 (GIS-Testwelt) | 45#216E | 🕰️ |
| 13.10. | je ~30 km² (Vorschlag) | 45#282A | 💡 |
| 18./19.10. | **keine Fläche** für Glenhaven, Ashford, Greywater genannt; nur Rollen (Binnenstadt, Binnenstadt, Hafenstadt) | D18-MAP | ❓ |
| WORLD_700 | Major A 10,0 km², Major B 9,3 km² (gemessen) | G7-GPKG | 🕰️ |

### 7.2 Siedlungszahl

Der Vorschlag vom 13.10. (58 Siedlungen auf 2.200 km²: 1 Metropolis, 2 Major Cities, 6 Regionalstädte, 14 Kleinstädte, 35 Dörfer; Summe 193,5 km² = 8,8 %) ist 💡 und vom Entwickler weder bestätigt noch verworfen; er verlangte eine **genauere Auflistung** (`45#283E`). `D19-OQ` Nr. 5 stellt die Frage neu (Größe der Großstädte, Überfüllung mit Dörfern).

### 7.3 Seen und Feuchtgebiete

| Objekt | 09.2026 | 29.09. (Auftrag) | 04.10. | 19.10. | WORLD_700 (gemessen) |
|---|---|---|---|---|---|
| Lake Alder | – | 15–22 (max ~25) km² | „auf 25 km²“ | ca. 25 km² (mit Alder Isle) | 6,3 km² |
| Lake Meridian | – | 5–8 (max 10) km² | – | ca. 5 km² | 4,9 km² |
| South Reservoir | – | – | – | ca. 7 km² | Südwest-Talreservoir 2,1 km² |
| Greywater-Feuchtgebiet | – | 50–80 km² | – | **reduziert** (Anforderung), keine Zahl | 8,7 km² |

Quellen: `30#232E`, `45#8E`, `D19-REQ`, `G7-GPKG`. Die Zahlen der Spalten stammen aus **verschiedenen Welten**; nur die Spalte 19.10. gehört zum aktuellen Kandidaten.

---

## 8. Die 25 Kartengrößen

Der Entwickler wünscht ca. 25 Größenvarianten (`D19-REQ`). Die **Liste selbst** stammt vom Assistenten (`45#60A`, 2:1-Ausdehnung, 96 % bespielbar) und ist 💡; der Entwickler hatte am 04.10. Tests von 2.200 bis 15.000 km² verlangt und dann „bis 1mio km²“ (`45#22E`, `45#24E`):

2.200 · 2.500 · 3.000 · 3.500 · 4.000 · 5.000 · 7.000 · 10.000 · 15.000 · 20.000 · 25.000 · 30.000 · 40.000 · 50.000 · 75.000 · 100.000 · 150.000 · 200.000 · 250.000 · 300.000 · 400.000 · 500.000 · 600.000 · 750.000 · 1.000.000 km² (25 Einträge).

Die Städtewerte für sehr große Welten aus `45#62A` ("Metropolis 20–25 km² bei 2.200 bis 120–200 km² bei 1 Mio. km²") sind **Testbereiche des Assistenten** und wurden mit dem 110-km²-Wert für den 2.200-km²-Kandidaten überholt. Als Prinzip bestätigt sind nur zwei Dinge: Städte wachsen mit der Welt **sublinear** (`M-MS1013`, `D19-REQ`), und größere Karten brauchen mehr Orte, Seen, Gebirge, Einzugsgebiete, nicht nur größere Städte (`45#61E` Nr. 3 und 5).

Welche Presets wirklich unterstützt werden und welche Skalierungsregeln gelten, ist offen (`D19-OQ` Nr. 1).

---

## 9. Entfernungen und Fahrzeiten

- **Reisezeit-Richtwert (Assistent, 💡):** West–Ost-Fahrt bei 20 % Umweg und 90 km/h Durchschnitt: 2.200 km² (68 × 34 km) ≈ 54 min; 5.000 km² ≈ 1:22 h; 20.000 km² ≈ 2:43 h (`45#60A`).
- **Prototyp-Abstände (Auftragstext 29.09., 🕰️):** Major City A – Metropolis ~23–25 km, Metropolis – Major City B ~21–23 km, A – B ~44–46 km, mit echtem Land dazwischen (`30#232E`).
- **WORLD_700-Abstände (gesperrt):** Metropolis ↔ Major City A 15,5 km, A ↔ B 18,6 km, Metropolis ↔ B 4,4 km in der frühen V3-Spezifikation (`G7-V3`).
- Der 110-km²-Kandidat nennt **keine** Abstände. Sie sind erst nach einer akzeptierten Geometrie bindend (`D19-CHK` Nr. 5).

---

## 10. WORLD_700 als Maßstabsreferenz

| Größe | Wert | Quelle |
|---|---:|---|
| Raster | 12.288 × 8.192 px, 2,7 m/px | G7-LCS |
| Hüllmaß | 33.177,6 × 22.118,4 m | G7-LCS |
| Hüllfläche | 733,835428 km² | G7-CLOSE |
| Spielfläche | 690,903626 km² | G7-CLOSE |
| Meer innerhalb der Hülle | 30,046114 km² | G7-CLOSE |
| Binnengewässer gesamt | 12,885687 km² | G7-CLOSE |
| Feuchtgebiet (Teil der Spielfläche) | 8,690963 km² | G7-CLOSE |
| Harbor Isle (Land) | 1,606716 km² | G7-CLOSE |

Weitere Zahlen und die berechneten Stadt-, Flughafen- und Staatsflächen: [Modul 24](24-WORLD700-GIS-Referenz.md).

---

## 11. Verwechslungsfallen

1. **Festland vs. Gesamtkarte:** 2.200 km² sind trockenes Festland; Meer, Seen und Insel kommen hinzu (`D19-REQ`).
2. **Planungshülle vs. Spielwelt:** die 66 × 34 km (2.244 km²) waren ein Prototyp-Bildrahmen, ausdrücklich ‚kein physischer Rand‘ (`30#232E`); das Höhenraster der Testwelt mit 1.348 × 680 Zellen à 50 m deckt 67,4 × 34,0 km (≈ 2.292 km² Rechteck, **Rechnung**) (`45#253E`, berichtet).
3. **110 vs. 22,5 vs. 70 vs. 30:** vier Metropolis-Werte aus vier Weltgrößen (Abschnitt 4). Nur 110 gehört zum Kandidaten vom 18./19.10.
4. **Seen vs. Land:** Seeflächen zählen nicht zu den 2.200 km².
5. **Stadtfläche vs. Verwaltungsgebiet:** die Vergleichsstädte (New York 778 km², Chicago 590 km², San Francisco 121 km², Boston 125 km²; US-Census 2020, nach `45#60A`) sind Verwaltungsflächen und ausdrücklich **keine Vorgabe** (`M-MS1013`).
6. **Alter Kollektiv-Zähler:** ‚1.200–1.500 Sammelobjekte‘ gehörten zur alten ~200-km²-Idee (`M-LATE`), nicht zu einer der heutigen Karten.
7. **WORLD_700 vs. WORLD_1500:** Zahlen der gesperrten Welt dürfen nicht für die größere Welt übernommen werden und umgekehrt (`W-GE-004`).
