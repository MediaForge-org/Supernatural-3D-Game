# Welt 04: Berge, Wälder, Wildnis und Küste

> [!IMPORTANT]
> **Nichts davon ist implementiert, und keine Landschaft ist freigegeben.** Das Modul sammelt, was die Quellen zu Relief, Vegetation, Wildnis, Küste und natürlichen Aktivitätsräumen festlegen, und trennt es von älteren Prototypaufträgen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)). Gewässer: [Modul 05](05-Gewaesser-und-Hydrologie.md); Register: [Modul 20](20-Entscheidungsregister.md) (NA, GE).

**Inhalt**

1. [Gesamtbild der Landschaft](#1-gesamtbild-der-landschaft)
2. [Das Hauptgebirge im Norden](#2-das-hauptgebirge-im-norden)
3. [Wälder, Felder, Weiden, Täler](#3-wälder-felder-weiden-täler)
4. [Wildnis und Leerraum](#4-wildnis-und-leerraum)
5. [Küste und Ostinsel](#5-küste-und-ostinsel)
6. [Wetter, Jahreszeiten, Klima](#6-wetter-jahreszeiten-klima)
7. [Natürliche Aktivitätsräume](#7-natürliche-aktivitätsräume)
8. [Tiere und Ökologie (Weltbezug)](#8-tiere-und-ökologie-weltbezug)
9. [Höhlen und Unterwelt der Natur](#9-höhlen-und-unterwelt-der-natur)
10. [Ältere Landschaftsvorgaben und ihr Verbleib](#10-ältere-landschaftsvorgaben-und-ihr-verbleib)
11. [Offene Punkte](#11-offene-punkte)

---

## 1. Gesamtbild der Landschaft

Der Kandidat vom 18./19.10. verlangt (📋 als Entwicklervorgabe, Geometrie 🧭):

- **Ein** Hauptgebirge im Norden als klares Makromerkmal; sonst plausible Hügel, Wälder, Felder, Täler und Wildnis. Die Welt soll **nicht** wie eine Felslandschaft aussehen (D18-MAP).
- **Breite Räume** zwischen Siedlungen, **Wälder, Weiden und Ackerland**, die lange Fahrten glaubwürdig machen und die ‚Spielzeugwelt‘-Wirkung vermeiden (D19-REQ).
- **Natürliche Übergänge:** Gebirge, Wald und Täler wirken natürlich und nicht wie einheitliche Klippenränder (D19-REQ; Prüfpunkt D19-CHK „Visual checks“).
- Wasser soll das bewohnbare Land nicht dominieren; nicht jede Siedlung ist eine Wasserstadt (D18-MAP).

Die Entwicklerkorrekturen am 27.09.–01.10. nennen dieselben Punkte aus Sicht der Bildkritik: ‚nicht überall Berge, nur an einer Stelle‘ (35#879E), ‚nicht immer nur Berge, sondern auch anderes‘ (35#909E), ‚schaue bei großen AAA-Maps nach, wie dort die Karten sind‘ (35#911E).

---

## 2. Das Hauptgebirge im Norden

| Aspekt | Aussage | Status | Quelle |
|---|---|:---:|---|
| Lage | im Norden, ein einzelnes System, kann durch die Karte verlaufen | 📋 | D18-MAP; 35#879E |
| Form | verwinkelt, mit vielen Geheimnissen („Mysteries“), nicht alles sichtbar | 📋 | 23#780E |
| Täler und Pässe | mehrere große Täler und Pässe, nicht eine massive Wand (mindestens drei Querungen im Prototypauftrag) | 🧭 | D19-REQ; 30#232E (Auftrag) |
| Zugang | Straßen bis auf gewisse Höhe, Bergsiedlungen klein (1–2 km² Dorf), Hochgebirge nur mit Spezialfahrzeugen oder Gondeln | 📋 | 35#947E; 35#89E; 23#821E; 23#823E |
| Höhe | 4.000–5.000 m (31.08., 01.10.) · 2.000–2.500 m befahrbar (31.08.) · 2.500–3.000 m (Auftrag) · ~4.400–4.900 m, nur wenige Gipfel (04.10., 45#7A) | ⚠️ | [K4](21-Konflikte-und-Widersprueche.md#k4-gipfelhöhe) |
| Wintersport | Gebirge stärker in die Karte, Skigebiet mit Straße nach oben | 📋 | 35#947E |
| Schnee | höchste Gipfel mit Schnee; dynamische Schneedecke und Spuren | 📋 | M-WB17 Block 6 |
| Seen im Gebirge | in WORLD_700: Eagle Lake (1.380 m), Summit Lake (1.860 m) | 🕰️ | G7-GPKG |
| Gondel, Bergrennen | Gondel mit Tal- und Gipfelstation; Rallye-Anstieg ‚wie Pikes Peak‘ und Hillclimb | 📋 (Wunsch) / 🕰️ (WORLD_700-Technik) | 23#822E; 23#823E; G7-CLOSE |
| Rolle für den Hunter | Gebirge als Jagd- und Rückzugsraum; Straßen, Pässe, Wetterwechsel | 🧭 | M-WB17 Block 6/7 |

**Nicht belegt:** konkrete Gipfelnamen für den Kandidaten, Verlauf der Kämme, Position der Pässe, Lage eines Skiorts. Die Namen ‚West Crown‘, ‚North Divide‘ (Landschaftsnamen im berichteten Blueprint V1.3e.3, `45#274E`) sind Arbeitsnamen eines verworfenen Blueprints.

**Warum die Höhe offen bleibt:** 4.000–5.000 m fordert der Entwickler zweimal (23#803E, 35#935E), 2.000–2.500 m am 31.08. (23#820E: ‚braucht nicht 4800 m sein, nur 2000–2500 m, aber befahrbar‘). Beide Aussagen betreffen **unterschiedliche Karten**; die jüngere (01.10.) gilt der 2.200-km²-Linie. Der Oktober-Kandidat nennt keine Höhe. Ein Kompromiss (befahrbare Straße bis ~2.000 m, höhere Gipfel nur per Gondel/zu Fuß) folgt aus beiden Aussagen, ist aber **nicht beschlossen** (💡).

---

**Ergänzung 04.10. (Chat 45):** In der ‚Geografische Masterstruktur v1‘ schlägt der Assistent drei verbundene Massive vor (Alder Highlands, Meridian Crown, Eastern Range), nur ~8–12 % hochalpin, höchste Gipfel ~4.400–4.900 m, einen Ski-Arm 6–10 km nach Süden und eine Hauptstraße bis ~2.500–3.200 m; der Entwickler antwortet „der rest klingt gut“ (45#8E). Westen: Fluss-Schlucht statt Gebirgswand; Südwesten: offene Weite ohne Wüste; Süd-Zentrum: große Waldwildnis (W-NA-012, W-NA-013, 🧭). Die Koordinaten dieses Entwurfs (W-GE-037) gehören zur Hülle 65 × 34 km und nicht zum Kandidaten vom 18.10.

## 3. Wälder, Felder, Weiden, Täler

- **Große zusammenhängende Wälder** sind wesentlich (`M-W15R`; D18-MAP). Wälder sollen es auch in Karten geben, die der Entwickler als zu unwaldig kritisiert hat (23#822E: ‚es gibt keinen Wald‘).
- **Farmland in nordamerikanischem Maßstab** (Auftrag 29.09., 30#241E) ist eine Detailvorgabe eines Kartenlaufs, keine Weltentscheidung.
- **Industrie im Außenland:** Land- und Forstwirtschaft, Steinbruch, Bergbau, Lebensmittelverarbeitung, Fischerei (M-WB17 Block 2). Beispiel WORLD_700: Südwest-Tal mit Reservoir, kleinem Damm, historischem Ortskern, Steinbruch, Holzeinschlag, Landwirtschaft (G7-GPKG: `ROAD_SW_*`).
- **Täler:** Siedlungen und Straßen folgen Tälern; der Entwickler wünscht Steigung und Vegetation sogar in Städten (45#61E).
- **PCG-Einsatz:** Vegetation, Steine, Straßenbild und kontrollierte Variation; wichtige Orte bleiben authored ([W-TE-003](20-Entscheidungsregister.md#16-technik-und-weltgenerierung-te)).
- **Vegetationssukzession:** saisonale Vegetation und langfristige ökologische Entwicklung (M-WB17 Block 6).

Eine **Landnutzungsverteilung** für den Kandidaten gibt es nicht. Der Prototypauftrag vom 29.09. verwendete Planungsziele (Stadt 4–5 %, Acker ~25 %, Wald ~30 %, Gebirge 15–18 %, Feuchtgebiet ~4 %, Gewässer ~5 %; 30#232E); der Assistent berichtete für die Testwelt Anteile (Acker 27 %, Mischhügel 13 %, Vorgebirge 12 %, Hochalpin 9 %, Binnenwasser 5 %, Feuchtgebiet 4 %, Stadt 3 %; 45#3A). Beide sind 🕰️ und keine Vorgabe.

---

## 4. Wildnis und Leerraum

- ‚Soll auch nicht leer wirken, sondern nur abgelegen wie in real life‘ (35#87E, Frage 4).
- 5–10 km überwiegend ländliche Strecken sind im Prototypauftrag **absichtlich** (30#232E §30, Auftragstext).
- D19-REQ: ‚Settlement-size hierarchy readable‘, aber ‚avoid excessive small settlements‘.
- Wildnis ist der Hauptzuwachs der größeren Welt: mehr Wald, Seen, kleine Dörfer, Platz (23#881E Nr. 4; `M-LATE`; `23#1009E` Nr. 24).
- **Fauna** und Monsterökologie müssen Wildnis tragen (M-WB89 Block 9; [Gameplay-Systeme](../Gameplay-Systeme.md)); Wildnis ist kein bloßer Zwischenraum.
- Abgelegene Orte sind Teil des Spiels: Motels, Tankstellen, Farmen, Fabriken, Häfen, Campingplätze, Jagdhütten (Assistent 45#282A, 💡); Entwickler bestätigte nur die Richtung ‚Welt voll, nicht leer‘ (23#717E).

---

## 5. Küste und Ostinsel

| Aspekt | Aussage | Status | Quelle |
|---|---|:---:|---|
| Lage der Küste | Osten/Südosten | 📋 | D18-MAP; 45#279E |
| Charakter | ruhiger, organisch regelmäßig, kein Mündungs- und Inselirrgarten | 📋 | D18-MAP |
| Anteil | nicht „150 km Küste“, ein kleiner Teil, nicht Meer überall | 📋 / 🕰️ | 23#779E |
| Inspiration | abwechslungsreiche Küste ‚wie bei Horizon Forbidden West‘ (Idee des Entwicklers: Meer im Osten statt im Westen) | 💡 | 45#272E; 45#280A |
| Küstenformen (Assistent) | felsige Abschnitte, Strände, Buchten, Inselgruppen, Häfen, Küstenstädte | 💡 | 45#280A |
| Insel | eine auffällige große Insel im Osten/Südosten, 80–100 km², bewohnt, getrennt | 🧭 | D18-MAP; D19-REQ |
| Inselinhalt | Entwicklerwunsch ‚eine Insel im Meer‘; Assistent: Siedlung, Wälder, Küstenstraßen, Hafen, abgelegene Gebiete | 📋 / 💡 | 23#780E; 45#278A |
| Weitere Inseln | Alder Isle (im See), Harbor Isle (WORLD_700, 1,6 km²), kleine Inseln vor der Küste (Prototyp) | 🧭 / 🕰️ | D18-MAP; G7-CLOSE; 30#232E |
| Hafen | Greywater: maritime Hafenstadt, nicht venezianisch | 📋 | D18-MAP |
| Rolle für Aktivitäten | Segeln, Yachten, Wasserski, Surfen, Klippen-/Brückenspringen, Tauchen, Marinas | 📋 | M-WB17 Block 7 |
| Küste in WORLD_700 | kleine Bucht in der SO-Ecke, 18,9 km² (2,9 %) im V3-Entwurf; 30,0 km² Meer in der Endfassung | 🕰️ | G7-V3; G7-CLOSE |

Die Insel hat eine eigene Rolle im Prüfplan: ‚Form nicht beiläufig umzeichnen‘, getrennt von Festland, Lücke zum Festland prüfen (D19-REQ; D19-CHK Nr. 3).

---

## 6. Wetter, Jahreszeiten, Klima

Aus `M-WB17` Block 6 (📋; als Planung, nicht implementiert):

- Vollkalender (jeder Tag/Stunde/Minute); Zeit beschleunigt; ‚ca. 2 Echtstunden je Spieltag‘ als Idee, **nicht gesperrt**.
- Vier Jahreszeiten; hohes Gebirge ganzjährig kühl (Sommer ~10–15 °C, Winter −10 °C und kälter).
- Dynamischer Schnee (Aufbau, Schmelze, Spuren); Regen und Gewitter wichtig, besonders für Motorsport.
- Überschwemmungen, Stürme, Nebel, seltene Tornados, seltene Waldbrände bei Hitze und Trockenheit.
- Wetter beeinflusst NPC-Routinen, Verkehr, Spuren und Beweise (Block 8, Block 9).
- Feiertage und Saisonereignisse sind akzeptierte Layer (`M-CA0905` §4).

**Offen:** Klimazonen der Karte, Windrichtungen, Regenschatten, Temperaturprofile je Region, Tag/Nacht-Verhältnis (❓).

---

## 7. Natürliche Aktivitätsräume

(Vollständige Aufzählung der Aktivitäten: [Modul 08](08-Raeumliche-Allokation-Orte-und-Aktivitaeten.md).)

| Raum | Aktivitäten | Quelle |
|---|---|---|
| Wald und Berg | Wandern, Trekking, Camping, Klettern, Bouldern, Abseilen, MTB, Rennradfahren, Fotografie, Jagd | M-WB17 Block 7 |
| Gewässer | Kajak/Kanu, Rafting, Angeln und Turniere, Tauchen, Schnorcheln, Jetski, Boote, Segeln, Surfen | M-WB17 Block 7; 45#8E |
| Winter | Ski alpin, Snowboard, Langlauf, Schneeschuh, Skitouren, Freeride, Schneemobil, Eislaufen, Lawinenrisiko | M-WB17 Block 7 |
| Höhle | Höhlenforschung, Höhlentauchen | M-WB17 Block 6/7 |
| Küste | Strand, Segeln, Wassersport, Hafenaktivitäten | M-WB17 Block 7 |

---

## 8. Tiere und Ökologie (Weltbezug)

- Tiefe Tierwelt: große Tiere, Raubtiere, Kleintiere, Vögel, Wassertiere, Haustiere, Nutztiere, Pferde, Fortpflanzung, Kadaver/Aasfresser, Überfahrene (M-WB17 Block 6).
- Tiere reagieren auf Übernatürliches ohne perfekte Detektoren.
- Jagd in legalen/besonderen Gebieten, tiefe Angelmechanik, Tierstudium und Fotografie.
- **Artenzahl (Zielbereich, vorläufig, V-07):** etwa 220–280 Tier-, Vogel- und Fischarten auf der Hauptkarte; auf späteren Karten oder im DLC mehr (`Spielwelt-und-Untergrund.md` §9.2, 📋 Ziel; `Entscheidungsstatus.md` §3). Keine Festlegung.
- **Studium einer Art** braucht eine bewusste Studier-Aktion; bloßes Sehen genügt nicht für vollständige Dokumentation. Archivpflichtige gewöhnliche Arten sind nicht dauerhaft verpassbar (`Spielwelt-und-Untergrund.md` §9.2).
- Unterwasserwelt mit Schwärmen, Vegetation, Felsen, Wracks, Höhlen, Tiefzonen und überfluteten Ruinen.
- **Monsterökologie** (Territorien, Nester, Rudel, Wanderung) braucht Wildnis; **Fegefeuer** ist das stärkste Ökologie-Reich ([Modul 26](26-Reiche-Zeitreise-und-historische-Weltzustaende.md)).

Die vollständige Artenliste liegt **nicht** vor (Artenliste, Klassenabdeckung und Studienkriterien sind laut `Spielwelt-und-Untergrund.md` §9.2 ❓ offen); die Prompt-Entwürfe `37-wildlife-study-harvest-forensics` führen sie nur als Planungsinventar.

---

## 9. Höhlen und Unterwelt der Natur

- „Große Höhlen“ und ‚entwickelte Unterwasserwelt‘ gehören zu Block 6; Höhlentauchen zu Block 7.
- Von den **Untergrundstädten** zu trennen: Naturhöhlen sind Teil der Oberflächenwelt, die Untergrundzivilisation ist eigene Lore ([Modul 10](10-Untergrundzivilisation.md)).
- Die Untergrundzivilisation wünscht eigene Naturräume (Fluss, Seen, Höhlenökosysteme); ihre Herkunft ist offen (W-UG-011).
- Eingänge zum Untergrund (Bunker, Schlüssel) sind Story-gesperrt (W-UG-016).

---

## 10. Ältere Landschaftsvorgaben und ihr Verbleib

| Vorgabe | Datum | Verbleib |
|---|---|---|
| ‚Zwei Gebirgsketten über je drei Staaten, vier Flusssysteme, Hochplateau im Regenschatten‘ | 31.08. | WORLD_700-Linie (G7-V3) |
| Südwesten trocken, Ranch, Canyons | 29.09. | Prototypauftrag; nicht übernommen |
| Greywater-Feuchtgebiet 50–80 km² | 29.09. | durch D18-MAP (‚reduce marsh‘) überholt |
| „Rechteckige“ Spielwelt mit 5–10 km Randpuffer | 29.09.–01.10. | durch natürliche Außengrenze ersetzt (W-GE-021); Puffer 1,5–3 km in Assistentenauftrag 35#936A |
| Hoher Gebirgsanteil rund um die Karte | 01.10. | vom Entwickler kritisiert (35#909E, 35#911E) |
| Mehrere Gebirgsketten statt eines ‚Gebirgsstaats‘ | 31.08. | für WORLD_700; die Oktober-Linie wünscht **ein** Hauptsystem |

---

## 11. Offene Punkte

1. Gipfelhöhe und Höhenprofil (K4).
2. Lage und Form des Hauptgebirges, der Pässe, des Skigebiets.
3. Landnutzungsverteilung des Kandidaten.
4. Klimazonen, Windrichtung, Niederschlagsverteilung.
5. Anzahl und Lage von Naturhöhlen und deren Verhältnis zum Untergrund.
6. Fläche und Gestalt der Ostinsel; ob sie Berge hat.
7. Ob die Außengrenze Gebirge, Tal, Wald oder Straße ist (nur ‚natürlich‘ festgelegt).

---

## Nachtrag Master-Abgleich (08.11.2026)

Ergänzungen aus dem Abgleich mit dem Planungsmaster. Quellen in Klammern: Master-Dateien unter `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/`, Kürzel siehe `Docs/Planning/SOURCES.md`. Blockierte Punkte sind als offen markiert und nicht entschieden (`Docs/Planning/CONFLICTS.md`).

### Ergänzung: Ostinsel (08.11.2026)
- Die bevorzugte Variante der Ostinsel ist nicht eindeutig identifizierbar (Master: `D19-REQ:24`). Eine Festlegung fehlt.
