# Welt 02: Weltvision und geografische Grundstruktur

> [!IMPORTANT]
> **Die Karte ist nicht freigegeben, und nichts davon ist implementiert.** Alle Aussagen sind Planungsstand mit Statuszeichen ([Legende](README.md#statuslegende)). Namen von Orten sind Arbeitsbezeichnungen. Das Dokument erfindet keine Geografie: wo die Quellen schweigen, steht ❓.

Teil der Welt-Dokumentation ([Index](README.md)). Vertieft [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md); Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Die Welt in einem Absatz](#1-die-welt-in-einem-absatz)
2. [Die Schichten der Welt](#2-die-schichten-der-welt)
3. [Leitprinzipien](#3-leitprinzipien)
4. [Die Makrostruktur des Kandidaten](#4-die-makrostruktur-des-kandidaten)
5. [Wie der Entwickler die Welt will: Richtungsaussagen](#5-wie-der-entwickler-die-welt-will-richtungsaussagen)
6. [Was ausdrücklich nicht gewollt ist](#6-was-ausdrücklich-nicht-gewollt-ist)
7. [Abhängigkeiten zwischen Welt und Spielsystemen](#7-abhängigkeiten-zwischen-welt-und-spielsystemen)
8. [Phasen und Weltgröße](#8-phasen-und-weltgröße)
9. [Fehlende Entscheidungen](#9-fehlende-entscheidungen)

---

## 1. Die Welt in einem Absatz

Das Spiel spielt in einer **fiktiven, modernen, USA-geprägten Welt**, in der das Übernatürliche unter der gewöhnlichen Gesellschaft verborgen liegt (`M-PD` §World). Die Hauptwelt ist ein **zusammenhängendes Festland** mit einer riesigen Metropole, weiteren Großstädten, vielen Kleinstädten und Dörfern, großen Wäldern und einem Hochgebirge im Norden, einer Küste im Osten mit einer bewohnten Insel, wenigen zusammenhängenden Gewässersystemen und weiten Fahrstrecken. Darunter liegt eine **verborgene, uralte Zivilisation**; daneben stehen **drei übernatürliche Reiche** als eigene Karten. Die Welt soll sich **dicht und lebendig** anfühlen, aber ‚abgelegen wie im echten Leben‘ dort, wo wenig Menschen leben (35#87E).

Dieser Absatz fasst 📋-Richtungen zusammen. Die **Zahlen** (2.200 km², 110 km² …) sind 🧭 ([Modul 03](03-Massstab-Flaechen-Proportionen.md)). Seit N-01/K-20 (08.11.2026) ist 2.200 km² kein Richtwert mehr.

---

## 2. Die Schichten der Welt

| Schicht | Inhalt | Status | Modul |
|---|---|:---:|---|
| **Materielle Hauptwelt** | Festland, Städte, Natur, Verkehr, Aktivitäten | 🧭 Größe, 📋 Richtung | 03–09, 13–16 |
| **Ostinsel** | bewohnte, geografisch getrennte Insel (80–100 km²) | 🧭 | [04](04-Berge-Waelder-und-Wildnis.md), [07](07-Weitere-Staedte-und-Siedlungen.md) |
| **Untergrund** | verborgene Städte, Bunker, Netz | 📋 Lore, 🧭 Größe | [10](10-Untergrundzivilisation.md), [11](11-Untergrund-Architektur-und-Infrastruktur.md) |
| **Reiche** | Hölle, Fegefeuer, Himmel (eigene Karten) | 📋 Existenz, 🧭 Größen | [26](26-Reiche-Zeitreise-und-historische-Weltzustaende.md) |
| **Historische Weltzustände** | Zeitreise ab ~1600 jahrgenau | 📋 | [26](26-Reiche-Zeitreise-und-historische-Weltzustaende.md) |
| **WORLD_700** | gesperrte Referenzwelt | 🕰️ / 📋 | [24](24-WORLD700-GIS-Referenz.md) |

Die **Hauptwelt** soll laut `M-CA0905` die Standardkarte des Basisspiels sein; neue Planung gilt, wenn nichts anderes steht, für sie.

---

## 3. Leitprinzipien

1. **Eine maßgebliche Geografie** (W-KA-001). Alle Ansichten leiten sich daraus ab.
2. **Realismus und innere Konsistenz** gehören zu den höchsten Prioritäten des Projekts (`M-LATE`, „Global realism“).
3. **Größenunabhängigkeit.** Keine Weltgröße wird hartkodiert; Städte wachsen sublinear (W-GE-015).
4. **Einzigartige Orte.** Keine doppelten Schlüsselorte, Module dürfen wiederverwendet werden, ihr Ergebnis muss stark variieren (W-GE-025, W-UR-002).
5. **Leere ist erlaubt, Langeweile nicht.** Große, menschenleere Flächen brauchen Geografie, Ökologie, Erkundung und glaubwürdige Übergänge (Assistentenformulierung `45#62A`, Entwickler: ‚soll auch nicht leer wirken, sondern nur abgelegen wie im echten Leben‘, 35#87E).
6. **Persistenz.** Das zentrale Weltzustandsgedächtnis speichert zerstörte/veränderte Orte, NPC-Zustand, Wissen, Monsteraktivität, Fahrzeuge, Zeit (`M-PD` §World state; W-TE-004).
7. **Additive Erweiterung.** Neue Regionen, Reiche, DLC können später kommen; DLC-Struktur ist ❓ (`M-PD` §World).
8. **Geheimnisse bleiben geheim.** Das Hunter-HQ und der Bunker sind im Spiel Story-gesperrt und auf der Spielerkarte nicht auffällig beschriftet (W-KA-008, W-UG-015). Die allgemeine Weltdokumentation darf dagegen die geografische Lage des HQ beschreiben (W-KA-017, `PE-E1022`), nicht aber geheime Eingänge, Story-Enthüllungen oder interne Einrichtungen.

---

## 4. Die Makrostruktur des Kandidaten

Quelle: `D18-MAP`, `D19-REQ`, Skizze 13.10. (`45#277E`–`45#280A`). Status 🧭, ohne Geometrie.

```text
            N
   ┌──────────────────────────────────────────────┐
   │  Hauptgebirge (Norden, ein einzelnes System)  │
   │      Lake Alder (mit Alder Isle)              │
   │   Wälder, Täler, Weiden, Felder               │
   W  ........ Hauptfluss  ──► nördl. äußere        E
   │          Bezirke von Metropolis ──► Ostküste ~ ~ ~ Meer
   │   Metropolis (110 km², zentral)         ~ ~ ~  ~  ● große Insel
   │   Glenhaven, Ashford (binnen, Lage ❓)    Greywater (Küste)
   │      South Reservoir (weiter im Südosten)     │
   │  Land setzt sich nach W und S über den Rand fort │
   └──────────────────────────────────────────────┘
            S
```

**Warnung:** Das Schaubild ist eine **Textskizze der Anforderungen**, keine Karte. Lagen von Glenhaven, Ashford und Greywater zueinander sind nicht festgelegt, mit Ausnahme der Bedingungen (‚binnen‘, ‚Hafen an der Küste‘). Aus der Entwicklerskizze ist nur bekannt, dass zwei weitere Großstädte „A“ und „B“ westlich und östlich von Metropolis lagen (45#278A, Lesart des Assistenten); für die Namen ist das nicht belegt. Ob Glenhaven/Ashford = A/B ist, sagt keine Entwicklerstelle ([K6](21-Konflikte-und-Widersprueche.md#k6-namen-der-großstädte)).

**Feste Randbedingungen:**

| Bedingung | Status | ID |
|---|:---:|---|
| Meer nur im Osten/Südosten | 📋 | W-GE-017 |
| Hauptgebirge im Norden | 📋 | W-GE-019 |
| Festland setzt sich im Westen und Süden über den Rand fort | 📋 | W-GE-016 |
| Eine große, auffällige Insel im Osten/Südosten | 🧭 | W-GE-014, W-WA-005 |
| Hauptfluss von W/N-Seen an Metropolis (äußerer Bezirk) vorbei zur Ostküste | 📋 | W-WA-004 |
| Glenhaven, Ashford abseits großer Gewässer | 📋 | W-ST-014 |
| Greywater ist die einzige Hafenstadt | 📋 | W-ST-015 |
| Südreservoir weiter im Südosten | 📋 | W-WA-006 |
| Hunter HQ abseits, unauffällig, nicht in/bei einer Stadt | 📋 | W-UG-023 |
| Hunter-HQ-Lage: südlich/südwestlich von Metropolis am Übergang zur großen Waldwildnis, etwa 10–15 Straßenkilometer, nahe Wasser (Entwurf 04.10., **Hypothese einer früheren Hülle**; exakte Position ❓) | 🧭 Hypothese | W-ST-035, W-KA-017, OF-69 |
| Natürliche, unregelmäßige Weltgrenze | 📋 | W-GE-021 |

---

## 5. Wie der Entwickler die Welt will: Richtungsaussagen

Diese Stellen sind **Primärbelege** (Rolle Entwickler):

| Wunsch | Beleg |
|---|---|
| Welt einzigartig, keine doppelten Orte; Story 80–100 h, viele Nebenfälle, Sammelobjekte ohne Ubisoft-Spam | 23#46E |
| Viele Gebäude, damit die Welt nicht leer ist | 23#46E (Punkt 17) |
| Jede Stadt-/Funktionsvielfalt: Strip-Clubs, Tattoo-Studios auch in kleineren Städten | 23#705E |
| Berge brauchen Straßen mit kleinen Dörfern, sonst sind sie ‚unnötig‘ | 23#705E |
| Voll, nicht leer: sehr viele NPCs in der Metropole, wenige auf dem Land, ganz wenige im Gebirge | 23#717E |
| Fiktive Welt, nicht wie die USA, nie nur eine große Stadt pro Staat | 23#723E |
| Wenig Meer, kein langer Küstenstreifen, einige Seen groß und klein für Aktivitäten, Festland, aber Insel erlaubt | 23#779E |
| Verwinkeltes Gebirge mit vielen Geheimnissen | 23#780E |
| Metropolis weiter zentral, vielleicht an einem See; nur eine kleine Küstenstadt; weniger Meer | 23#821E |
| Wald, Festland-Metropolis, F1-Strecke, fünf weitere Rennstrecken, ÖPNV, Gondel am Berg | 23#822E; 23#823E |
| Nach der 700-km²-Karte eine größere Karte | 23#865E |
| Städte als Stadtgefühl: nicht groß, sondern abwechslungsreich; mit Fluss, Vegetation, Steigung | 45#61E |
| Karte gefällt nicht; hohe Ansprüche | 45#61E (Punkt 6) |
| Meer im Osten | 45#279E |
| Keine Fortnite-Map, kein Insel-Eindruck, Grenzen sind Berge oder Festland | 35#876E |
| Natürliche Kartenränder; Karte nicht rechteckig; nicht nur Berge | 35#905E; 35#907E; 35#909E; 35#911E |
| Wintersport: Gebirge stärker in die Karte, Straßen bis zu einer gewissen Höhe | 35#947E |

---

## 6. Was ausdrücklich nicht gewollt ist

| Nicht gewollt | Beleg |
|---|---|
| Insel oder „Fortnite-Map“ | 35#876E; 23#779E |
| Städte als „bodenlos“ große Flächen (200 km²) | 35#876E |
| 570 km² Gesamtstadtfläche als Projektannahme | M-MS1013; M-AD1015 |
| Gebirge als Wand ringsum, Gebirgslandschaft überall, Klippenränder | D18-MAP; 35#909E |
| Flüsse, Kanäle, Sumpfarme überall; Venedig-Hafen | D18-MAP; D19-REQ |
| Perspektivische/isometrische/3D-Karte als Hauptkarte | D19-REQ |
| Rechteckiger Spielfeldrand mitten in der Landschaft | 35#905E |
| Kreisförmige/ovale Stadtflächen, Voronoi-Bezirke | D18-ACC |
| Zusammenwachsende Städte ohne Zwischenland | 23#714E; 30#232E (Auftrag) |
| Spielzeugwelt-Wirkung; zu kurze Distanzen | D18-MAP; 30#227E (Auftrag, eingefügt) |
| Prominente „Hunter HQ“-Beschriftung auf der Spielerkarte | W-KA-008 |
| Stilles Umzeichnen der Ostinsel | D19-REQ |

---

## 7. Abhängigkeiten zwischen Welt und Spielsystemen

| Welt-Aussage | Berührt | Bemerkung |
|---|---|---|
| Fahrtdauer und Weltgröße | [Fahrzeuge](13-Fahrzeuge-und-Fahrzeugklassen.md), [Fahrphysik](14-Fahrphysik-und-Fahrzeuginteraktion.md) | Größe soll mit aus echten Fahrzeiten kalibriert werden (W-GE-026), noch kein Fahrprototyp mit Karte |
| Siedlungsdichte | NPC-Simulation, Persistenz ([Modul 16](16-NPC-Verkehr-und-Verkehrssimulation.md)) | Persistenzstufen A–D |
| Gebirge und Winter | Aktivitäten ([Modul 08](08-Raeumliche-Allokation-Orte-und-Aktivitaeten.md)), Straßen ([Modul 09](09-Strassen-Schiene-und-Verkehrsinfrastruktur.md)) | Gondel, Skigebiet, Lawinen |
| Wasser | Tauchen, Angeln, Boote, Hydrologie ([Modul 05](05-Gewaesser-und-Hydrologie.md)) | Unterwasserwelt als Hauptlayer |
| Untergrund | Story, Bunker, Netz, Streaming vertikaler Räume | [Modul 10](10-Untergrundzivilisation.md), [Modul 18](18-Unreal-Landscape-World-Partition-und-Streaming.md) |
| Hunter-HQ-Lage | Karte, Geheimnis, Story | Beschreibung dokumentierbar (W-KA-017); exakte Position ❓ (OF-69); Bunker-Verhältnis W-UG-017 offen |
| Zeitreise | Zeitversionierte Geografieschichten | [Modul 26](26-Reiche-Zeitreise-und-historische-Weltzustaende.md) |

---

## 8. Phasen und Weltgröße

Das Projekt plant **nicht**, die volle Welt sofort zu bauen. Laut Produktionsstrategie (`M-MS1013`, `M-AD1015`, CLAUDE.md §4):

| Phase | Ziel | Weltbezug |
|---|---|---|
| 0 Kerntechnik | winziges spielbares Platzhalter-Ergebnis | temporäre Testumgebung, **keine** getarnte Endkarte |
| 1 Vertical Slice | ca. 1–3 km², 30–60 min, ein Fall | ein Ausschnitt; Weltgrößen offen |
| 2 | ca. 5–10 km², 3–5 h | mehrere Fälle |
| 3 | ca. 25–50 km², 10–20+ h | erweiterte Welt |
| 4+ | iterativ zur Gesamtvision | volle Hauptwelt |

Der Prototyp-Stand im Repository (Testkarte mit Tür-Interaktion, WorldFacts) ist ein **Platzhalter** und kein Kartenkandidat ([Spielwelt §2](../Spielwelt-und-Untergrund.md#2-zwei-getrennte-linien-der-weltplanung)).

**Ergänzung aus Chat 35:** Die genaue Karte soll „später“ geplant werden, wenn viele Systeme stehen; zu Beginn zählen Ladebildschirm und ein kleiner Kartenteil (35#598E, 35#715E, W-GE-033). Die erste Prototypkarte vom 09.09. hielt der Entwickler für zu klein skaliert (35#100E, W-GE-032).

---

## 9. Fehlende Entscheidungen

Kompakt (volle Liste: [Modul 22](22-Offene-Fragen-Katalog.md)):

1. Endgültige Fläche, Terrain, Hydrologie, Küste, Insel, Städtezahl und -lagen.
2. Ob es Staaten gibt (W-ST-020).
3. Gipfelhöhe (W-GE-020).
4. Die Größe der Städte außer Metropolis.
5. Verhältnis Untergrund-Bunker ↔ oberirdisches Hunter-HQ (W-UG-017).
6. Verwendung von WORLD_700 (W-GE-003).
7. Welche der 25 Größenpresets unterstützt werden (D19-OQ).
