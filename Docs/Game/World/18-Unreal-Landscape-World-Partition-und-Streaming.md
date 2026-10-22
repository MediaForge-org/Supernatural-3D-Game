# Welt 18: Unreal Landscape, World Partition und Streaming

> [!IMPORTANT]
> **Keine Unreal-Weltarchitektur ist beschlossen oder implementiert.** Der Master legt nur Grundsätze fest (Unreal + C++20, World Partition, PCG für Dressing, stabile IDs, Weltzustand). Alles Weitere ist 💡 oder Prompt-Entwurf (`DRAFT_REWRITE_REQUIRED`). Der Repository-Stand ist ein Platzhalter-Prototyp. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (TE).

Teil der Welt-Dokumentation ([Index](README.md)). Daten: [Modul 17](17-Weltgenerierung-und-GIS-Daten.md).

**Inhalt**

1. [Bestätigte Grundsätze](#1-bestätigte-grundsätze)
2. [Landscape](#2-landscape)
3. [World Partition und Level Streaming](#3-world-partition-und-level-streaming)
4. [Data Layers](#4-data-layers)
5. [PCG und Vegetation](#5-pcg-und-vegetation)
6. [Straßen- und Verkehrsnetze, Stadtgenerierung](#6-straßen--und-verkehrsnetze-stadtgenerierung)
7. [Gebäude und Innenräume](#7-gebäude-und-innenräume)
8. [LOD, HLOD, Nanite, Budgets](#8-lod-hlod-nanite-budgets)
9. [Save/Load und Persistenz](#9-saveload-und-persistenz)
10. [NPC- und Fahrzeug-Streaming](#10-npc--und-fahrzeug-streaming)
11. [Untergrund und Reiche](#11-untergrund-und-reiche)
12. [Automatisierte Tests](#12-automatisierte-tests)
13. [Trennung: Vorhandenes, Bestätigtes, Vorschläge](#13-trennung-vorhandenes-bestätigtes-vorschläge)
14. [Offene Punkte](#14-offene-punkte)

---

## 1. Bestätigte Grundsätze

| Grundsatz | Status | Quelle |
|---|:---:|---|
| Unreal Engine + C++20; keine eigene Voll-Engine; kein stilles C++23 | 📋 | M-PD §Technology |
| Unreal für Rendering, World Partition/Streaming, Animation, Audio, UI, Editor, Asset-Pipeline; eigene Systeme für Ermittlung, KI, Weltzustand, Persistenz, Verletzung, Kampf, Fahrzeug, Modding, Tools | 📋 | M-PD |
| ‚Was bei Unreal besser ist, lassen wir bei Unreal; was besser selbst, machen wir selbst‘ | 📋 | 35#41E (Fragen 159–168) |
| World Partition lädt Laufzeitinhalt; Weltzustand bestimmt die persistente Variante | 📋 | M-PD §World state |
| PCG für Vegetation, Steine, Straßenbild; nicht für authored wichtige Orte | 📋 | M-PD §World |
| Stabile IDs, segmentierte Saves | 📋 | M-PD §World state |
| Performance-Budgets von Anfang an, gemeinsam geplant; Hardwarestufen offen | 📋 / ❓ | M-PD §Rendering |
| Engine-neutrale Simulationsgrenzen wo sinnvoll | 📋 | M-AD1015 |

---

## 2. Landscape

**Nicht belegt:** Landscape-Größe, Komponentenanzahl, Auflösung, Heightmap-Format, Streaming-Proxys. Die Referenzwelt hat 10,8-m-Planungs-DEM (G7-CLOSE); für Unreal Landscape wäre eine neue Ableitung nötig. Vorschlag (💡): Höhenmodell aus dem akzeptierten Master ableiten, nicht umgekehrt; Heightmap-Auflösung nach Prüfung der Fahrzeug- und Hangkollision festlegen; mehrere Größenpresets (25 Varianten) verlangen **parametrische** Landschaftsgrenzen statt festkodierter Zahlen (W-GE-015). Siehe Prompt `029_create-map-edge-and-inaccessible-area-validation` (Entwurf).

---

## 3. World Partition und Level Streaming

📋 World Partition ist das Streaming-Grundverfahren (M-PD). Entwürfe: `009_create-world-partition-integration-contract-for-persistent-locations`, `010_create-streaming-source-policy-for-player-and-vehicles`, `011_create-location-activation-deactivation-lifecycle-events`, `035_create-large-world-save-load-streaming-regression-suite`, `036_create-low-memory-streaming-stress-scenario`, Testharness `010_create-world-partition-streaming-test-harness`. Fahrzeuge brauchen eine eigene Streaming-Quelle (Geschwindigkeit; Entwurf 010). Der Entwickler: ‚wenn gut optimiert, läuft auch eine 20.000-km²-Welt, was ich nicht machen will‘ (35#87E).

**Hardware:** Laptop 16 GB mit integrierter Grafik: voller Editor unwahrscheinlich praktikabel; Windows-Desktop RTX 3090 für Unreal-Integration (CLAUDE.md §5). Dies bestimmt Testdesign: Standalone-C++-Tests, Unreal-Kommandozeile, gepackte Builds.

---

## 4. Data Layers

Der Master nennt Data Layers **nicht**. 💡 Mögliche Nutzung: Zeitschichten (Siedlungen je Epoche), Realm-Varianten, Geheimorte, Ereignis-/Sperrzustände, Jahreszeiten. Keine Entscheidung.

---

## 5. PCG und Vegetation

📋 Grundsatz s. o. Entwürfe: `020_create-pcg-boundary-rules-for-authored-case-locations`, `021_create-pcg-environment-seed-version-metadata`, `055_create-pcg-compatible-environmental-asset-metadata`. Zu beachten: saisonale Vegetation, Sukzession (M-WB17 Block 6), wichtige Orte bleiben authored; PCG-Seeds und Version werden gespeichert, damit Weltzustand reproduzierbar bleibt (Entwurf 021). Landnutzungs-Anteile des Kandidaten liegen nicht vor.

---

## 6. Straßen- und Verkehrsnetze, Stadtgenerierung

Entwurf `022_create-road-network-gameplay-metadata-layer`, `023_create-road-relevance-tagging-for-hunter-archive`. Stadtgenerierung: Der Master enthält **keine** Generator-Spezifikation; belegt sind Ziele (organische Form, Bezirke, Hierarchie, Parks). V1.1 berichtete drei Stadtmuster als **Visualisierungsentwürfe** (45#216E). 💡 Bezirke (20) als Datenobjekte mit Rolle/Dichte/Epoche, Straßennetz aus Topografie, Hero-Gebäude manuell.

---

## 7. Gebäude und Innenräume

📋 ≥ 90 % betretbar; Stufen Hero / Full Gameplay / Procedural / Limited; reine Fassaden Minderheit. Entwürfe `014` bis `019` (Variation, Betretbarkeit, Hero-/Fall-Pipeline, Fassaden-Budget-Validierung). Innenraum-Streaming (Laden beim Betreten, Wiedereintritt, persistente Änderungen) ist nicht spezifiziert; Immobilien- und Bau-Systeme (Spieler plant Haus) benötigen Laufzeitbau, der **nicht** entschieden ist.

---

## 8. LOD, HLOD, Nanite, Budgets

📋 Ziel 4K (8K optional), Upscaling optional, Ray Tracing Teil des Ziels, Pfad-Tracing nur Hochend, PS5 30/60-Modi, Series S skaliert (M-PD §Rendering). Asset-Budgets: Entwurf `012_create-asset-performance-budget-metadata`, `011_create-lod-nanite-decision-checklist`. **Offen:** konkrete Triangle-/Draw-/Speicherbudgets. Eine Skyline mit 3–5 Supertalls und 110-km²-Stadt erhöht die Nachfrage nach HLOD/Fernsicht (Folgerung, 💡).

---

## 9. Save/Load und Persistenz

📋 Saves: segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar, komprimiert; Ziel 20–50 MB langfristig; Korrektheit vor Größe. Weltzustand verfolgt Falllösung, NPC lebend/tot, Beziehungen, veränderte Orte, Wissen, Monsteraktivität, Ruf, Fahrzeuge, Safehouses, Ereignisse, Zeit (M-PD). Host-Save-autoritativ im Koop (CLAUDE.md). Zeitreise: Saves müssen Epochenvarianten tragen (M-CA0905 §2). Repository-Realität: WorldFacts/Save-Prototyp berichtet (D19, nicht geprüft); eigene Entscheidungen in `Docs/Decisions/`.

---

## 10. NPC- und Fahrzeug-Streaming

Mehrere Simulationsebenen (Voll, Reduziert, Statistisch) für nicht geladene Welt (Paket 11M, 35#41E zustimmend); LOD für Crowds; Bahn-Offscreen (D15-RW 💡). Fahrzeug-Streaming als Streaming-Quelle (Entwurf 010). Budgets offen.

---

## 11. Untergrund und Reiche

Vertikal gestapelte Untergrundräume brauchen GIS-Darstellung und Streaming/LOD (D14-OQ Nr. 13); Reiche sind **eigene Karten/Welten** mit Realm-ID im Weltzustand (35#40A Fragen 137–146, Entwurf `031_create-separate-world-map-registration-for-hell-purgatory-like-planes`, `032_create-inter-world-travel-gateway-interface-without-final-lore-rules`). DLC- und Erweiterungs-Registrierungspunkte: Entwürfe `033`, `034`.

---

## 12. Automatisierte Tests

📋 Testen ist Teil der Architektur (M-PD §Testing). Weltbezogene Entwürfe: Reisezeit-Instrumentierung (`024`, `025`), Stadt-/Innenraumdichte-Benchmarks (`026`, `027`), Content-Dichte-Heatmap (`028`), Karten-Rand-Validierung (`029`). 💡 Geografische Konsistenztests als Standalone-C++/Python-Prüfungen auf dem GIS-Export: Flächenbilanz, Flussgraph, Eindeutigkeit der Feature-IDs, Geheimnisfreiheit.

---

## 13. Trennung: Vorhandenes, Bestätigtes, Vorschläge

| Kategorie | Inhalt |
|---|---|
| **Vorhanden (laut Repository, nicht von dieser Etappe geprüft)** | Phase-0-Prototyp: Testkarte mit Tür-Interaktion, WorldFacts-Persistenz (README, `Docs/Decisions/`) |
| **Bestätigte Planung** | Abschnitt 1 |
| **Vorschläge (💡)** | Data Layers, Landscape-Ableitung, Stadtgenerator, Tests auf GIS-Export, Zeitlayer |
| **Prompt-Entwürfe** | alle genannten Nummern, `DRAFT_REWRITE_REQUIRED` |

---

## 14. Offene Punkte

Unreal-Version; Landscape-/Partition-Parameter; Streaming-Distanzen; Nanite-Strategie für Städte; Innenraum-Streaming; Laufzeitbau; Budgets je Plattform; Zeitreise-Ladeverfahren; Untergrund-Streaming; Moddability der Karte.
