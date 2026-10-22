# Welt 16: NPC-Verkehr und Verkehrssimulation

> [!IMPORTANT]
> **Nichts davon ist implementiert; es gibt kein Verkehrsmodell.** Belegt sind Verhaltensanforderungen und das langfristige Bahnziel. Alle technischen Zerlegungen sind 💡. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (TV).

Teil der Welt-Dokumentation ([Index](README.md)). Infrastruktur: [Modul 09](09-Strassen-Schiene-und-Verkehrsinfrastruktur.md).

**Inhalt**

1. [Straßenverkehr](#1-straßenverkehr)
2. [Fahrer-NPCs](#2-fahrer-npcs)
3. [Öffentlicher Verkehr und Bahnbetrieb](#3-öffentlicher-verkehr-und-bahnbetrieb)
4. [Simulationsebenen](#4-simulationsebenen)
5. [NPC-Alltag und Weltzeit](#5-npc-alltag-und-weltzeit)
6. [Notdienste und Zwischenfälle](#6-notdienste-und-zwischenfälle)
7. [Persistenz](#7-persistenz)
8. [Leistung und Budgets](#8-leistung-und-budgets)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Straßenverkehr

📋 (M-WB17 Block 5, 23#1025E): dynamisch, nicht absurd; Stoßzeiten; Verkehrsmeldungen im Radio bei Stau; seltene NPC-Unfälle (‚außer du rammst sie weg‘); Pannen; Baustellen; Blitzer und Polizeikontrollen mit Warn-Apps (23#1025E Nr. 18/19). Bei Veranstaltungen Straßensperren (W-UR-010). Verkehr muss lokal rekonstruiert werden, wenn der Spieler eintritt (M-WB89 Block 8).

---

## 2. Fahrer-NPCs

Fahrer unterscheiden sich in Können und Persönlichkeit; Anfänger würgen manuelle Autos ab; Wetter ändert Verhalten (M-WB89 Block 8). Die gleiche Reifen-, Schlupf- und Kupplungsphysik für NPCs ist nicht festgelegt.

---

## 3. Öffentlicher Verkehr und Bahnbetrieb

📋 (D15-RW): Bahn-Langfristziel mit Fahrplänen, Signalen, Weichen, Stellwerken, Blöcken, Zugsicherung, Bremskurven, Verspätungsfortpflanzung, Rangieren, Stromversorgung, Instandhaltung, Umläufen, Personal, Offscreen-Betrieb; NPC-Betreiber halten das Netz unabhängig vom Spieler; Spieler fährt nur mit Job/Berechtigung. Bus und Zug sind für lange Strecken zu bevorzugen (23#1025E Nr. 36). Details und Anlagenzahlen: [Modul 09](09-Strassen-Schiene-und-Verkehrsinfrastruktur.md).

---

## 4. Simulationsebenen

📋 Prinzip: Welt läuft nur, wenn man spielt (Offline) (35#279E). Nicht geladene Welt simuliert abstrahiert weiter (Paket 11M, Antwort ‚ja‘ auf 147–158, 35#41E). 💡 Mehrere Ebenen: Full / Reduced / Statistical (Frage 148 im Paket 11M; die genaue Antwort steht nur als ‚ja‘).

💡 Engineering-Zerlegung der Bahn (D15-RW): statischer Infrastrukturgraph, Betriebsgraph (Blöcke, Signale, Disposition), Dienstgraph (Linien, Fahrpläne, Umläufe, Personal), Güter-Graph, Streaming/Offscreen-Simulation mit persistenten Zugidentitäten und sicherer Zusammenführung an Ladegrenzen, interaktive Rollen hinter Berechtigungen. Keine Spurweite, kein Signalstandard, kein Dateiformat gewählt.

**Ergänzung aus Chat 35 (11.09.):** Der Entwickler nennt ~500.000 NPCs als Planungsgröße (35#227E). Der Assistent schlägt vor: Tier 0 (Hero), 1 (lokal aktiv), 2 (regional im Hintergrund), 3 (global ruhend), ereignisbasierte Hintergrundsimulation und Zufall aus NPC-ID, Welt-Seed und Zeitfenster (35#278A). Die Antwort „12-35 ja“ (35#279E) ist eine Sammelzustimmung; einzelne Tier-Regeln stützen sich darauf nicht (W-TV-010). Das Save-Ziel ist umstritten (K13).

---

## 5. NPC-Alltag und Weltzeit

📋 (M-WB89 Block 8): Routinen nach Wochentag, Arbeit, Wetter, Krankheit, Feiertag, Ereignis; reale Arbeitsplätze und Wohnorte; Altern; Jobwechsel; Persistenzstufen A (Story) bis D (Menge); Menschen sprechen miteinander und nutzen Telefon/Social Media. Zeit: Vollkalender, beschleunigt, ‚ca. 2 Echtstunden je Spieltag‘ nicht gesperrt (M-WB17 Block 6).

---

## 6. Notdienste und Zwischenfälle

Feuerwehr, Rettung, Polizei, Abschleppdienst, Bergrettung, Wasserrettung, Suchaktionen in angemessener Tiefe; Zwischenfälle verursachen dauerhafte Schäden, NPC-Trupps reparieren über glaubhafte Zeit (M-WB89 Block 8). Polizei braucht Zeugen und Beweise.

---

## 7. Persistenz

Stabile IDs für wichtige Entitäten; segmentierte, absturzsichere Saves (M-PD §World state). Für Verkehr: Fahrzeuge persistieren (Fahrzeugzustand), Menge/Zugidentitäten sind Vorschlag. Host-Save-autoritativ im Koop (CLAUDE.md §6).

---

## 8. Leistung und Budgets

Crowd/Verkehr nutzen LOD und Abstraktion (M-WB89 Block 8). Die Untergrundbevölkerung muss unabhängig von der Lore-Zahl budgetiert werden (D14-PS). Budgets, Performance-Stufen und deterministische Resynchronisation sind offen (D15-OQ Nr. 9, 11).

---

## 9. Offene Punkte

Verkehrsmodell; Dichtekurven; ÖPNV-Betrieb; Bahn-Offscreen-Stufen; Resynchronisation; Netzkonsistenz beim Laden; Kollisionen NPC ↔ Spieler; Mehrspieler-Verkehr.
