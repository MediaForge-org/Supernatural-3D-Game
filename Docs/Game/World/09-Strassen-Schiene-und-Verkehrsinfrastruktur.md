# Welt 09: Straßen, Schiene und Verkehrsinfrastruktur

> [!IMPORTANT]
> **Nichts davon ist implementiert; es gibt kein Netz für den 2.200-km²-Kandidaten.** Belegt sind Anforderungen, Bahn-Planungsbereiche (15.10.) und das Netz der Referenzwelt WORLD_700. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (VK).

Teil der Welt-Dokumentation ([Index](README.md)). Fahrzeuge: [Modul 13](13-Fahrzeuge-und-Fahrzeugklassen.md); Simulation: [Modul 16](16-NPC-Verkehr-und-Verkehrssimulation.md); GIS-Daten: [Modul 24](24-WORLD700-GIS-Referenz.md).

**Inhalt**

1. [Straßennetz](#1-straßennetz)
2. [Berg- und Sonderstraßen](#2-berg--und-sonderstraßen)
3. [Eisenbahn](#3-eisenbahn)
4. [Nahverkehr in Metropolis](#4-nahverkehr-in-metropolis)
5. [Flughäfen und Flugplätze](#5-flughäfen-und-flugplätze)
6. [Häfen und Marinas](#6-häfen-und-marinas)
7. [Unterirdisches Netz](#7-unterirdisches-netz)
8. [Verkehr und Weltgröße](#8-verkehr-und-weltgröße)
9. [Netz der Referenzwelt WORLD_700](#9-netz-der-referenzwelt-world_700)
10. [Offene Punkte](#10-offene-punkte)

---

## 1. Straßennetz

- Sparsames Makronetz; ländliche Straßendichte sinkt mit der Besiedlung; keine „Straßenspaghetti“ (Prototypauftrag 30#232E, Verfeinerung 30#241E: ländliche Straßen −30–40 %) – Arbeitsaufträge, 🕰️.
- Straßen, Schiene und Brücken respektieren Gelände, Stadthierarchie und große Brücken (D18-ACC, 📋 als Anforderung).
- Straßenhierarchie und Bahnhierarchie sind Teil der Metropolis-Anforderung (D18-MAP).
- Straßen und Logistik als Gameplay-Metadatenebene sind als Prompt-Entwurf genannt (`022_create-road-network-gameplay-metadata-layer`), nicht als Entscheidung.
- Straßensperrungen bei Stadtrennen und Rallye-Etappen (W-UR-010); Baustellen, Pannen, seltene Unfälle (W-TV-001).
- Der Prototypauftrag 29.09. nannte drei strategische Korridore (West–Ost-Autobahn, Nord–Süd über einen Pass, Küstenkorridor). Für den Kandidaten sind Verläufe offen.

---

## 2. Berg- und Sonderstraßen

- Straßen im Gebirge bis zu einer gewissen Höhe, Skigebiet, Dörfer an Pässen (35#947E, 23#705E).
- Gondel am Berg, Auto auf „nicht so hohe“ Berge (23#823E).
- Rallye- und Hillclimb-Straße „wie Pikes Peak“ (23#822E).
- Korridorstatus der Testwelt (berichtet): 2 PASS, 19 NEEDS_ENGINEERING, 2 FAIL, zwei Bergzugänge nicht freigegeben (45#216E).
- Referenzwerte WORLD_700: Hillclimb 18,9 km, 17 Kehren, Zufahrt mit kurzem Tunnel (G7-CLOSE).

---

## 3. Eisenbahn

📋 (D15-RW, „User-approved range“):

| Anlage | Bereich |
|---|---:|
| Große Güterknoten | 3–4 |
| Regionale Güterterminals | 8–10 |
| Lokale Ladestellen | 18–25 |
| Große Depots | 4–6 |
| Regionale Betriebsdepots | 9–13 |
| Hauptwerkstätten | 3–4 |
| Kleine Abstellanlagen | 18–26 |

Weitere Festlegungen:

- Alle Züge bleiben innerhalb der Karte.
- Elektrifizierung hoch (Haupt- und S-Bahn fast vollständig, Regional überwiegend; abgelegene Zweige Diesel/Batterie); keine Spannung, kein System.
- Langfristziel: nahezu vollständiger Betrieb (Fahrpläne, Signale, Weichen, Stellwerke, Blöcke, Zugsicherung, Bremskurven, Verspätungsfortpflanzung, Rangieren, Stromversorgung, Instandhaltung, Umläufe, Personal, Offscreen-Betrieb). Selbst fahren nur mit passendem Job/Berechtigung; NPC-Betreiber halten das Netz am Laufen.
- Dichten (aus früherer Entscheidung, 🧭): S-Bahn 3–5 min, RE 10–15, RB 10–20, Zweiglinien 15–30.
- Nicht festgelegt: Strecken, Längen, Spurweite, Signalstandard, Flottengrößen, Hersteller, Dateiformat.

**Rechnung:** Die Anlagenzahlen (zusammen 63–86) sind Planungsbereiche ohne Kartenbezug; für 110 km² Metropolis und die übrige Karte fehlt die Verteilung. Der Entwickler stufte Passagier- und Güterzüge, Schienenverkehr mit Zug- und Fernbus als Teil der Reise ein: ‚Zug oder Bus bevorzugt, weil schneller‘ (23#1025E Nr. 36).

**Engineering-Zerlegung (💡, nicht beschlossen, D15-RW):** statischer Infrastrukturgraph; Betriebsgraph; Dienstgraph; Güter-Graph; Streaming/Offscreen-Simulation; interaktive Rollen (Fahrer, Disponent) hinter Berechtigungen.

---

## 4. Nahverkehr in Metropolis

🧭 (für die 70-km²-Linie, 23#907E Nr. 9, M-W15R): 8–10 U-Bahn-Linien, 7–9 S-Bahn-Linien, Straßenbahn und Bus ‚noch mehr‘ („angelehnt an Wien aber noch mehr“), Zug und weite Busstrecken noch besser vernetzt „wie in Europa“; nutzbar zum Reisen. Für den 110-km²-Kandidaten nicht neu bestätigt. Der Entwickler wollte ÖPNV auch in und außerhalb der Stadt (23#822E). WORLD_700 hatte drei U-Bahn-Linien und einen Straßenbahnring.

---

## 5. Flughäfen und Flugplätze

| Stand | Aussage | Quelle |
|---|---|---|
| WORLD_700 | 3 Flughäfen (Metropolis International 6,4 km², West 2,4, Ost 2,7; Hüllgeometrien) | G7-GPKG |
| 09.09. (WORLD_2000) | ≥ 7 Flughäfen (einer je Staat), 8–10 evtl., 8–15 Flugplätze, Bergflugplatz, Heliports | 35#93E; 35#96A |
| Oktober-Aufträge | drei Flughäfen als Ankerelemente | 30#265E |
| 18./19.10. | Flughäfen nicht genannt | – |

Gemeinsam: Hauptflughafen **außerhalb des Kerns** (Schwechat), Flughafenanbindung per Straße und Bahn. Spielbezug: Airliner als Passagierverkehr, Kleinflugzeuge spielerflugbar, Hubschrauber besitzbar aber meist von Piloten geflogen (23#1025E Nr. 34). Konflikt: [K8](21-Konflikte-und-Widersprueche.md#k8-flughäfen-und-motorsportstandorte).

---

## 6. Häfen und Marinas

Boote liegen an Marinas, Tauchen/Erkunden per Boot (M-WB17); Häfen, Fischerei, Fracht gehören zur Wirtschaft; 5–10 Marinas als Arbeitsrichtung 09.09. (35#96A). Greywater ist der Haupthafen; Hafeninsel- und Küstenhafen in WORLD_700.

---

## 7. Unterirdisches Netz

> ⚠️ **historisch (N-02, 08.11.2026): Die Untergrundstädte und ihr Verbindungsnetz entfallen als Pflichtinhalt. Bunker, Höhlen und kleinere Untergrundbereiche bleiben möglich.** Mehrere Untergrundstädte verbindet ein unterirdisches Schienen- oder ähnliches Transportsystem (D14-UC, 📋 Richtung; Technik offen). Zugangskontrolle ist ein eigenes System (W-UG-022). Ob die Oberflächenbahn angebunden ist, ist offen (W-VK-016). Details: [Modul 11](11-Untergrund-Architektur-und-Infrastruktur.md).

---

## 8. Verkehr und Weltgröße

Reise-Richtwert (💡, 45#60A): 90 km/h, 20 % Umweg → West–Ost-Fahrt 54 min bei 2.200 km². Die Weltgröße soll aus echten Fahrzeiten mitkalibriert werden (W-GE-026). Das Straßennetz bestimmt Fahrzeiten stärker als Fläche (35#86A). Schnellreise über Camps/Safehouses optional (35#86A, 💡; Entwickler stimmte der Richtung ‚Durchquerung merkbar lang, normale Missionen regional‘ zu, 35#87E).

---

## 9. Netz der Referenzwelt WORLD_700

Berechnet aus dem GeoPackage ([Modul 24](24-WORLD700-GIS-Referenz.md)): I1-Kette West-Tor → Major A → Metropolis → Major B → Ost-Tor (zusammen 36,5 km), G1 Süd und Küste (13,2 km), Flughafenanbindung 4,4 km, Bergzufahrt 6,1 km, Hillclimb 8,6 km (Vektorlänge; Ingenieurstrecke 18,9 km); Bahn: Intercity A–Metropolis 16,5 km und Metropolis–B 9,7 km, drei Regionalstrecken, Pendler-Airport 4,7 km, Güterstrecke 2,6 km; drei U-Bahn-Linien je ~6,5 km, Straßenbahnring 9,1 km, vier Buskorridore. Elektrifizierung und Spurzahl ‚unspezifiziert‘.

---

## 10. Offene Punkte

1. Gesamtnetz für den Kandidaten (Autobahn, Bahn, Pässe).
2. Verteilung der Bahnanlagen.
3. ÖPNV-Umfang bei 110 km².
4. Anzahl der Flughäfen (K8).
5. Häfen und Fährverkehr zur Ostinsel.
6. Schnellreise.
