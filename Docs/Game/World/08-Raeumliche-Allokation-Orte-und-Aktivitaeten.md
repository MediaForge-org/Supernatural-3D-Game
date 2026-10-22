# Welt 08: Räumliche Allokation, Orte und Aktivitäten

> [!IMPORTANT]
> **Die räumliche Zuordnung (Spatial Allocation) ist nicht abgeschlossen; alle Mengen sind Arbeitsrichtungen.** Dieses Modul sammelt die Orts- und Aktivitätsanforderungen, die die Karte tragen muss. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (UR, AK).

Teil der Welt-Dokumentation ([Index](README.md)).

**Inhalt**

1. [Warum Allokation vor der finalen Karte](#1-warum-allokation-vor-der-finalen-karte)
2. [Sport und Stadien](#2-sport-und-stadien)
3. [Freizeit, Parks, Nachtleben](#3-freizeit-parks-nachtleben)
4. [Outdoor, Wasser, Winter](#4-outdoor-wasser-winter)
5. [Gebäude und Gewerbe](#5-gebäude-und-gewerbe)
6. [Dynamische Veranstaltungen](#6-dynamische-veranstaltungen)
7. [Flächenbudget 09.09. (Entwurf)](#7-flächenbudget-0909-entwurf)
8. [Sammelobjekte und Entdeckungsebenen](#8-sammelobjekte-und-entdeckungsebenen)
9. [Tiefenstufen](#9-tiefenstufen)
10. [Offene Punkte](#10-offene-punkte)

---

## 1. Warum Allokation vor der finalen Karte

Die finale Karte darf erst entstehen, wenn (1) Anforderungsblöcke ausreichend entschieden sind, (2) die Allokationsmatrix Anzahl, Größe und Lage festlegt, (3) Verkehr, Umwelt, Aktivitäten, Sport und Systeme ohne spätere Zwangsanpassung zusammenpassen (M-W15R „Final-map gate“). Der Entwickler: bevor die Map gemacht wird, muss ‚ALLES festgelegt werden, nicht nur die Sportarten‘ (23#984E).

---

## 2. Sport und Stadien

📋 (M-WB17 Block 7, 23#1035E):

- Voll simulierte nationale Ligen; **nur lokale Heimstätten** physisch (American Football, Basketball, Baseball, Eishockey, Fußball, College, Minor, High School). Auswärtsspiele per TV, Radio, Zeitung, Wetten verfolgt; eine Liga komplett simuliert und Stadion besuchbar (23#1035E Nr. 14).
- Mehrzweckarenen für Basketball, Eishockey, Kampf, Konzerte.
- Weitere: Boxen, MMA, Wrestling, Untergrund-/Backyard-Wrestling, Pferderennen, Pferdebesitz, Tennis, Golf, Darts, Pool, Bowling (definitiv spielbar, M-LATE).
- Der Entwickler sah das Stadionproblem: eine komplette Liga bräuchte 10–20 Stadien je Sportart (Eishockey/Basketball über 30) – darum die Simulationslösung (23#1033E).
- Motorsport: [Modul 15](15-Motorsport-und-Rennaktivitaeten.md).

---

## 3. Freizeit, Parks, Nachtleben

| Ort | Vorgabe | Status | Quelle |
|---|---|:---:|---|
| Wasserpark | mindestens einer, Rutschen/Becken benutzbar | 📋 | 23#1039E; M-LATE |
| Themen-/Achterbahnpark | mindestens einer, Fahrgeschäfte fahrbar | 📋 | 23#1039E |
| Eigene Achterbahn bauen | teuer, Strecke/Zug/Stützen/Sicherheit; niedrige Priorität, kein Start | 🔭 | M-WB17 |
| Kleinere Parks | später bei der Allokation | ❓ | M-WB17 |
| Zoo, Aquarium, Museen, Kino, Arcades, Konzerte, Festivals, Bars | vorhanden | 📋 | M-WB17 Block 7 |
| Kino/TV/Streaming mit Fiktivinhalten, viele Radiosender, Podcasts | vorhanden | 📋 | M-WB17 |
| Nachtleben | „ESSENTIAL“; Clubs, Stripclubs, optionale Erwachsenenbetriebe | 📋 | M-WB17; 23#1035E Nr. 100 |
| Kochen | nicht nötig, essen gehen reicht | 📋 | 23#1035E |

---

## 4. Outdoor, Wasser, Winter

Siehe [Modul 04 §7](04-Berge-Waelder-und-Wildnis.md#7-natürliche-aktivitätsräume) und [Modul 05 §6](05-Gewaesser-und-Hydrologie.md#6-wasser-und-gameplay). Ergänzend aus 09.09. (35#96A, 🕰️): 2–4 Skigebiete, 5–10 größere Marinas/Bootsanlagen, Bergflugplatz.

---

## 5. Gebäude und Gewerbe

Betretbarkeit ≥ 90 %; Hero-/Full-/Procedural-/Limited-Interiors; Handel, Gastronomie, Banken, Hotels, Krankenhäuser, Colleges, Bibliotheken, Museen, Händler, Tuning, Logistik, Friedhöfe, Rechenzentren, Marinas, Resorts, Campingplätze, Hunter-Fachgeschäfte (W-UR-001, W-UR-005). Pfand-/Second-Hand-/Fence-Läden mit wechselndem Bestand (M-PD §Cash).

---

## 6. Dynamische Veranstaltungen

📋 (M-WB17 Block 7): Die Welt plant Autotreffen, Sport, Clubnächte, Konzerte, Märkte, Kämpfe, Motorsport, Festivals, Pferderennen. NPCs besuchen sie und verändern Verkehr, ÖPNV, Parken, Gastronomie, Hotels, Wettmärkte. Gelegentlich entstehen Gerüchte oder kleine Aufträge, aber nicht als markierte Nebenmissionen (23#1035E Nr. 125: Aufträge, nicht Nebenmissionen, ‚weil diese auf der Karte markiert sind‘).

---

## 7. Flächenbudget 09.09. (Entwurf)

💡 Assistent (35#92A), für ~2.000 km², ausdrücklich ‚noch kein Lock‘:

| Gebiet | km² |
|---|---:|
| Städte/Siedlungen | 250–270 |
| Wälder/Forst | ~450 |
| Gebirge/alpin | ~320 |
| Farmland/offen | ~330 |
| Seen, Flüsse, Wetlands | ~180 |
| Grasland/Busch/sonstige Natur | ~160 |
| Küste/Shoreline/Hafen | ~70 |
| Industrie/Logistik außerhalb der Städte | ~55 |
| Straßen, Rail, Korridore | ~65 |
| Flughäfen | ~25 |
| Motorsportanlagen | ~15 |
| POI/Park/Mystery/Reserve | ~70 |
| **Summe (Rechnung)** | **~1.990 – 2.010** |

Überlappungen sind erlaubt; für die finale Allokation GIS-Layer statt Prozenttorte (35#92A). Flughäfen: Metropolis International 12–16 km², zwei Regionalflughäfen je 4–6. Motorsport: 6 Anlagen mit 12–17 km² (W-MO-012). Diese Tabelle gehört zur 2.000-km²-Linie und ist für den 2.200-km²-Kandidaten nicht fortgeschrieben.

---

## 8. Sammelobjekte und Entdeckungsebenen

1.200–1.500 physische Sammelobjekte gehörten zur alten ~200-km²-Idee, insgesamt ~5.000 Entdeckungsobjekte (M-LATE, 23#1079E). Mehr Welt bedeutet mehr Collectibles (23#1077E); Zahlen werden **je Kategorie** nach der Allokation festgelegt, nicht linear. Straßen, besondere Gebäude, Höhlen, Gewässer, Infrastruktur und Geschichte sind eigene Entdeckungs-/Archivebenen. Reiche erhalten eigene Planung.

---

## 9. Tiefenstufen

Tier A: Fahrzeuge, Motorsport, Sportwetten, Glücksspiel, Angeln, Jagd, Tauchen, Wintersport, Hunter-Erkundung; Tier B: Basketball, Hockey, Tennis, Golf, Bowling, Pool, Darts, Camping, Kajak/Rafting, MTB, Minigolf; Tier C: Sonnenbaden, Streicheln, kleine Arcade, TV, kleine Baraktivitäten. Tiefe darf wachsen (M-WB17).

---

## 10. Offene Punkte

1. Anzahl der Stadien, Parks, Skigebiete, Marinas für den Kandidaten.
2. Verteilung auf die Karte (benötigt akzeptierte Geografie).
3. Welche Aktivitäten Starttiefe haben.
4. Ob das Budget vom 09.09. für 2.200 km² neu gerechnet wird.
