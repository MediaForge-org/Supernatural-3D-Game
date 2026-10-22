# Welt 07: Weitere Städte und Siedlungen

> [!IMPORTANT]
> **Nichts davon ist implementiert; keine Siedlungsfläche außer Metropolis (110 km²) ist für den Kandidaten vom 18./19.10. festgelegt.** Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (ST).

Teil der Welt-Dokumentation ([Index](README.md)). Metropolis: [Modul 06](06-Metropolis.md); Größenchronik: [Modul 03](03-Massstab-Flaechen-Proportionen.md).

**Inhalt**

1. [Siedlungshierarchie](#1-siedlungshierarchie)
2. [Glenhaven, Ashford, Greywater](#2-glenhaven-ashford-greywater)
3. [Stadtcharaktere aus der September-Planung](#3-stadtcharaktere-aus-der-september-planung)
4. [Mittlere Städte, Kleinstädte, Dörfer](#4-mittlere-städte-kleinstädte-dörfer)
5. [Bundesstaaten und Regionen](#5-bundesstaaten-und-regionen)
6. [Siedlungen in Gebirge, Küste, Insel](#6-siedlungen-in-gebirge-küste-insel)
7. [Zahlenvorschläge im Vergleich](#7-zahlenvorschläge-im-vergleich)
8. [Funktionen jeder Siedlungsstufe](#8-funktionen-jeder-siedlungsstufe)
9. [WORLD_700 als Beispiel](#9-world_700-als-beispiel)
10. [Offene Punkte](#10-offene-punkte)

---

## 1. Siedlungshierarchie

📋 (D19-REQ): Metropolis > mehrere Großstädte > weitere Städte > Kleinstädte > Dörfer/Weiler. Größere Städte dürfen nicht als winzige Punkte erscheinen, zugleich soll die Karte nicht mit Kleinstsiedlungen überladen sein. Der Entwickler wollte nie nur eine große Stadt pro Staat (23#723E) und fand 09.09. „zu wenige große städte“ im Plan (35#93E).

Prinzip (Entwickler 35#89E): Größe folgt dem Ort – ‚auf einem Berg keine 20-km²-Stadt, nur ein 1–2 km² großes Dorf‘. Assistentenformulierung dazu: Entscheidend ist, was geografisch und historisch passt (35#90A, 💡).

---

## 2. Glenhaven, Ashford, Greywater

| Stadt | Vorgabe | Status | Quelle |
|---|---|:---:|---|
| Glenhaven | Binnenstadt, optisch trocken; nicht an Fluss, See, Meer, Kanal; Fluss führt nicht hindurch; „substanziell“ (nicht winzig) | 📋 | D18-MAP; D19-REQ; D19-OQ |
| Ashford | wie Glenhaven, ohne große Kanäle | 📋 | D18-MAP |
| Greywater | einzige klar maritime Hafenstadt; Küste und Hafen glaubwürdig, nicht venezianisch, keine Kanäle, kein überflutetes Sumpfland | 📋 | D18-MAP |
| Fläche aller drei | **nicht genannt** | ❓ | – |
| Frühere Werte | Glenhaven/Ashford je 9 km² (Testwelt 11.10.), je ~30 km² (Vorschlag 13.10.) | 🕰️ / 💡 | 45#216E; 45#282A |
| Stadtmuster-Entwürfe V1.1 | Metropolis dichte Blockstruktur, Glenhaven geschwungene Straßenzüge, Ashford unregelmäßiger historischer Kern | 🕰️ Visualisierungsentwurf | 45#216E |
| Städtetypen (Assistent) | Glenhaven regionales Wirtschaftszentrum mit älteren Vierteln; Ashford anders in Architektur/Wirtschaft, an der Küste denkbar | 💡 | 45#282A |

**Widerspruch Ashford/Küste:** Der Assistent sah Ashford 13.10. an der Ostküste (💡); die Oktober-Vorgabe macht es zur Binnenstadt und gibt Greywater die Küste ([K6](21-Konflikte-und-Widersprueche.md#k6-namen-der-großstädte)). Die Oktober-Vorgabe gilt.

Ob Glenhaven/Ashford die früheren „Major City A/B“ sind, ist nicht ausdrücklich belegt (W-ST-016).

---

## 3. Stadtcharaktere aus der September-Planung

(M-WB17 Block 1, M-W15R; 🧭, nicht neu bestätigt für den Oktober-Kandidaten)

- **Großstadt mit Industrie-Identität:** Automobil, Fertigung, Bahnlogistik, ältere Industrie, sichtbare Altfabriken, Leerstand, Umnutzung.
- **Großstadt mit Forschungs-Identität:** Ingenieurwesen, Forschung, Hochtechnologie, Berg- und Wintertourismus.
- Beide Städte: reiche Wasser-/Hanglagen, Mittelstandsvororte, Arbeiter-/Industrieviertel, schwächere Gebiete, historische Gemeinschaftsviertel.
- Vorbilder des Assistenten für den Oktober (💡): San Francisco-artig hügelig, Boston-artig historische Flussstadt (45#62A) – beides im Widerspruch zu ‚binnen, ohne Fluss‘ (D18-MAP) und deshalb nicht übernommen.

---

## 4. Mittlere Städte, Kleinstädte, Dörfer

- Der Entwickler bestätigt gestufte Größen: „20km2 15 12 und so weiter“ (35#89E).
- Kleinere Städte erhalten Stripclubs, Tattoo-Studios usw. (23#705E).
- Kleinstädte sind klassische amerikanische Orte (Main Street, Sheriff, Diner, Kirche, Tankstelle) – Assistentenbild 45#284A, 💡.
- Bergorte: wenige, charakteristisch, abgelegen.
- Dörfer: bis 0,5 km² oder kleiner (35#87E).
- Abgelegene Einzelorte: Motels, Tankstellen, Farmen, Fabriken, Häfen, Campingplätze, Jagdhütten (35#92A/96A, 💡 mit Richtungszustimmung 35#97E).
- Anzahl der Siedlungen: **offen**; Vorschlag 58 (45#282A) ist 💡, D19-OQ fragt, ob große Städte genügend Substanz haben ‚ohne Überfüllung mit Dörfern‘.

---

## 5. Bundesstaaten und Regionen

- 31.08.: ‚5–10 Bundesstaaten mit Untergebieten‘ (23#721E), dann „machen wir es mal mit 7 bundesstaaten“ (23#729E); Gesetze unterscheiden sich je Staat – in einem wird man gesucht, im anderen nicht (23#719E).
- 09.09.: sieben Staaten als Arbeitsrichtung der ~2.000-km²-Welt, jeder Staat mit eigener bedeutender Stadt und eigener Mischung (35#96A, 35#97E).
- Oktober-Kandidat: **keine Aussage**. Siehe [K7](21-Konflikte-und-Widersprueche.md#k7-staatenstruktur).
- Folgen für Systeme: Rechtssystem mit mehreren Gerichtsbarkeiten (M-WB89 Block 8), Polizeitypen, Kennzeichen, Karten-Unterregionen für Sammelobjekte.

---

## 6. Siedlungen in Gebirge, Küste, Insel

| Raum | Siedlungstyp | Quelle |
|---|---|---|
| Gebirge | wenige Bergdörfer an Pässen/Seen, Skiresorts, Lodges, Rangerstationen | 35#89E; 35#96A |
| Küste | Greywater (Hafenstadt), Küstenorte; Marinas | D18-MAP; 35#96A |
| Ostinsel | bewohnt; mindestens eine kleine Stadt und mehrere Ortschaften (Assistentenbild) | D19-REQ; 45#278A |
| Binnenland | Landwirtschaftsstädte, regionale Zentren | 45#282A |

---

## 7. Zahlenvorschläge im Vergleich

| Quelle | Metropolis | Große Städte | Mittlere | Kleine/Dörfer | Anteil Stadt |
|---|---|---|---|---|---|
| Entwickler 09.09. | 70–80 | 30–40 | 20, 15, 12 … | bis 0,5 | ≤ 12–18 % |
| Assistent 09.09. (35#96A) | 70–80 | 6 × 20–35 | 3–5 × 10–18, 5–8 × 5–10 | 10–20 Towns 1,5–5; Dörfer | 12–18 % |
| Assistent 13.10. (45#282A) | 70 | 2 × 30 | 6 (4–8) | 14 Kleinstädte, 35 Dörfer | 8,8 % |
| Test 09.10. | 20–25 | 8–10 | – | – | ~1–2 % |
| Kandidat 18./19.10. | 110 | – | – | – | ≥ 5 % (nur Metropolis) |

Alle Werte außer der letzten Zeile sind 🕰️ oder 💡.

---

## 8. Funktionen jeder Siedlungsstufe

Aus Blocks 4, 7, 8 (📋 als Planung): Wohnen und Immobilien (Kaufen, Mieten), Gewerbe, Gastronomie, Nachtleben, Dienste (Polizei, Feuerwehr, Rettung), Schulen/Colleges, Krankenhäuser, Sportstätten, Verkehr, lokale Medien. Kleinere Orte haben weniger Tiefe: Betretbarkeit ≥ 90 % gilt für normale Gebäude, Innenräume in Stufen. Jede Stadt braucht ‚eigene Gründe, dort zu leben oder hinzufahren‘ (35#96A, 💡).

---

## 9. WORLD_700 als Beispiel

Sieben Siedlungspolygone (Metropolis 30,0 km², Major A 10,0, Major B 9,3, vier Mid Cities ~1,9–2,1 km²), gemessen in [Modul 24](24-WORLD700-GIS-Referenz.md). Die frühe V3-Spezifikation hatte 8 Mid Cities und 22 Small Towns (G7-V3). Zahlen gehören der gesperrten Welt.

---

## 10. Offene Punkte

1. Flächen und Lagen von Glenhaven, Ashford, Greywater.
2. Anzahl und Verteilung weiterer Siedlungen.
3. Staatenstruktur.
4. Namen der Städte unterhalb der Großstädte.
5. Rolle der Ostinsel-Siedlungen.
6. Bevölkerungszahlen aller Siedlungen.
