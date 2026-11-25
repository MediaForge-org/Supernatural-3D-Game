# Welt 22: Katalog offener Fragen

> [!IMPORTANT]
> **Dies sind Fragen, keine Entscheidungen.** Sie sind für spätere gemeinsame Entscheidungen gesammelt (keine Rückfrage bei Kleinigkeiten). Jede Frage nennt ihren Ursprung. Bei jeder Planungsfrage gilt laut D14-OQ: Freitext des Entwicklers hat Vorrang vor Optionen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)). Konflikte: [Modul 21](21-Konflikte-und-Widersprueche.md).

**Inhalt**

1. [Priorität A: blockiert die Karte](#1-priorität-a-blockiert-die-karte)
2. [Priorität B: Welt und Städte](#2-priorität-b-welt-und-städte)
3. [Priorität C: Untergrund und Gesellschaft](#3-priorität-c-untergrund-und-gesellschaft)
4. [Priorität D: Fahrzeuge, Verkehr, Motorsport](#4-priorität-d-fahrzeuge-verkehr-motorsport)
5. [Priorität E: Technik und Weltgenerierung](#5-priorität-e-technik-und-weltgenerierung)
6. [Quellenlücken](#6-quellenlücken)

---

## 1. Priorität A: blockiert die Karte

| Nr. | Frage | Ursprung | Register/Konflikt |
|---|---|---|---|
| OF-01 | Welche Kartenversion (falls eine) wird als `ACCEPTED_MASTER` bestätigt? Seit 19.10. keine. | D18-OQ Nr. 1 | W-KA-003 |
| OF-02 | Welche Größen-Presets (von ~25) werden unterstützt, und welche Skalierungsregeln gelten? | D19-OQ Nr. 1 | W-GE-015 |
| OF-03 | Welches CRS, welche Toleranzen, welche native Rasterauflösung, Georeferenz? | D18-OQ Nr. 3; D19-OQ Nr. 6 | [Modul 17](17-Weltgenerierung-und-GIS-Daten.md) |
| OF-04 | Form und Name der Ostinsel; welche Variante der Papierskizze gilt? | D19-OQ Nr. 2 | W-GE-014 |
| OF-05 | Wo liegen Glenhaven und Ashford (binnen), wie ist Greywater angelegt? | D18-OQ Nr. 6 | W-ST-014/015 |
| OF-06 | Kann der Alder–Metropolis-Fluss durchgängig sein, ohne Glenhaven/Downtown zu queren? | D19-OQ Nr. 4 | W-WA-004 |
| OF-07 | Sind Glenhaven/Ashford/Greywater bei 2.200 km² „substanziell“ genug, ohne Überfüllung mit Dörfern? | D19-OQ Nr. 5 | W-ST-013 |
| OF-08 | Können Rohdaten (`SPN_SCALE_MASTER_V1`, Kandidaten-PNG/GIS) für Hash-Prüfungen bereitgestellt werden? | D19-OQ Nr. 3, 13 | [Modul 19](19-Kartenreferenzen-und-Versionsgeschichte.md) |
| OF-09 | Wie werden Geheimorte intern geführt und aus öffentlichen Karten entfernt? | D18-OQ Nr. 7 | W-KA-008 |
| OF-10 | Welche Entscheidungen der Tage 20.–22.10. fehlen im Master? | Lücke | K12 |

---

## 2. Priorität B: Welt und Städte

| Nr. | Frage | Ursprung |
|---|---|---|
| OF-11 | ✔ **Geklärt am 22.10.2026 (`PE-E1022`):** Die 110 km² sind das zusammenhängende Stadtgebiet einschließlich Vororten, nicht nur die Innenstadt und nicht die Metropolregion. | W-ST-003 |
| OF-12 | Teilweise geklärt: Die Flächenabgrenzung steht fest (OF-11); die ältere Aussage ‚80 km² ist schon krass‘ ist überholt. Offen bleibt, wie die Darstellung die Kritik ‚Städte zu groß‘ vermeidet (Gestaltungsfrage). | K1 |
| OF-13 | Aufteilung der 90 km² außerhalb der Downtown; Bezirksnamen/-rollen der 20 Bezirke. | D18-MAP |
| OF-14 | Übersetzung von 110 km², 20 Bezirken, 20/12-km²-Kern und 700–1.000-m-Landmarken in Dichte, vertikalen Maßstab und Leistung. | D18-OQ Nr. 5 |
| OF-15 | Gibt es Bundesstaaten (sieben) im Oktober-Kandidaten? | K7 |
| OF-16 | Wie groß sind die übrigen Städte, wie viele Siedlungen, welche Stufen? | W-ST-012 |
| OF-63 | Wie viele Stripclubs, Tattoo-Studios und weitere Gewerbe je Stadt (Prozentsätze „stellen wir gleich auf“)? | W-ST-031; W-ST-032 |
| OF-64 | Antwort des Entwicklers auf den vorgeschlagenen Ablauf Makro-Zuordnung → Siedlung → Gebäudetypen (35#531A)? | W-ST-033 |
| OF-17 | Gipfelhöhe und Höhenprofil. | K4 |
| OF-18 | Flughafen- und Motorsportzahl für 2.200 km². | K8 |
| OF-19 | Stadtanteil an der Weltfläche (Obergrenze). | K11 |
| OF-20 | Zusammenhang Namen A/B und Glenhaven/Ashford; Rollen der Städte. | K6 |
| OF-21 | Rolle von WORLD_700 (Referenz, Benchmark, Testwelt). | W-GE-003 |
| OF-22 | Landnutzungsverteilung, Klimazonen, Windrichtung. | [Modul 04](04-Berge-Waelder-und-Wildnis.md) |
| OF-23 | Hat die Ostinsel Berge, welche Siedlungen, wie ist sie erreichbar (Fähre, Brücke, Flug)? | [Modul 09](09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) |

---

## 3. Priorität C: Untergrund und Gesellschaft

| Nr. | Frage | Ursprung |
|---|---|---|
| OF-24 | Footprint der größten Untergrundstadt innerhalb 50–100 km², Ebenen, Bezirke, Kavernenhöhe. **Historisch nach N-02** (Stadt entfällt als Pflichtinhalt; offen nur für Bunker und Höhlen). | D14-OQ Nr. 1; D15-OQ Nr. 3 |
| OF-25 | Bevölkerung und Dichten nach Footprint-Wahl. | D14-OQ Nr. 2 |
| OF-26 | Anzahl und relative Größe der Untergrundstädte; Netz durchgängig? | D14-OQ Nr. 3 |
| OF-27 | Verhältnis Untergrund-Bunker ↔ oberirdisches HQ. | K10 |
| OF-28 | Herkunft des Schlüssels (reserviert) – Story-Strang später. | D14-HB |
| OF-29 | Umstände der Bunkerentdeckung, Erneuerung des Bündnisses, Status der Bunkerbewohner. | D14-OQ Nr. 6, 7 |
| OF-30 | Ursprung und Grenzen der fiktiven Energie; warum die Elite sie nicht hat. | D14-OQ Nr. 8 |
| OF-31 | Anzahl, Rolle, Kognition der unsterblichen Erbauer. **Historisch nach N-02** (Erbauer-Zivilisation). | D14-OQ Nr. 9 |
| OF-32 | Wasser, Landwirtschaft, Luft, Wärme, Abfall und Ökologie in Stadtgröße. | D14-OQ Nr. 10 |
| OF-33 | Wer weiß von der Oberfläche; wie werden Agenten erkannt? | D14-OQ Nr. 11 |
| OF-34 | Weitere geheime Anlagen. | D14-OQ Nr. 12 |
| OF-35 | U37: interne Netze und Oberflächen-Internet. | D17-OQ |
| OF-36 | KI: Bewusstsein, Rechte, Aufsicht, Datenhaltung, Notaus. | D15-OQ Nr. 1, 5; D17-U |
| OF-37 | Medizin: Lebensdauer, Zugang, Nebenwirkungen, Gründerzustand. | D15-OQ Nr. 4 |
| OF-38 | Bildung: Absicherung der Grundfertigkeiten ohne Pflichtlehrplan. | D15-OQ Nr. 6 |
| OF-39 | Ressourcenverteilung, Konfliktlösung, Infrastrukturfinanzierung, Handel, Luxus. | W-GS-018, W-GS-002 |
| OF-40 | ‚Energetische Stabilisierung‘ und Tragstruktur der Höhle – im Master nicht belegt. | [Modul 11](11-Untergrund-Architektur-und-Infrastruktur.md) |

---

## 4. Priorität D: Fahrzeuge, Verkehr, Motorsport

| Nr. | Frage | Ursprung |
|---|---|---|
| OF-41 | Schadensmodell (Verformung, Antriebs-/Reifenfehler, Reparaturökonomie, Verschleiß). | D15-RV; D15-OQ Nr. 2 |
| OF-42 | Fahrzeugsortiment, fiktive Marken, Trims, Modelljahre, Wartungsökonomie, Vertrautheitsgrenze. | M-OD |
| OF-43 | Kupplungs-/Motor-/Getriebe-Konstanten; Schleifpunkt-Algorithmus; Tastaturbelegung; Assists. | M-OD; D15-RV |
| OF-44 | Prototyp für L1+L2 (analoge Kupplung) und Bremse. | D15-OQ Nr. 10 |
| OF-45 | Strecken, Spurweite, Strom, Signal, Blocklängen, Flotten, Stationen. | D15-OQ Nr. 7 |
| OF-46 | Anbindung Oberflächenbahn ↔ Untergrundnetz. | D15-OQ Nr. 8 |
| OF-47 | Offscreen-Leistungsstufen, deterministische Resynchronisation von Zügen/Verkehr/NPCs. | D15-OQ Nr. 9 |
| OF-48 | Motorsport: Streckenliste, Serienstruktur, Klassenregeln, Homologation, Belohnungen. | M-OD |
| OF-49 | ÖPNV-Größe für den 110-km²-Kandidaten. | W-VK-006 |
| OF-50 | Schnellreise (Camps/Safehouses). | 35#86A |
| OF-58 | Ist der ungesicherte Schotter-Hillclimb (35#568E) die Pikes-Peak-Bergrallye aus W-MO-004 oder eine zweite Strecke? | W-MO-014 |
| OF-59 | Wie viele Rallyes, Stadtkurse und temporäre Strecken gibt es, und wie verhalten sie sich zu den 3 bzw. 6 Ankern (K8)? | W-MO-015; W-MO-016 |
| OF-60 | Welcher als ‚neuester bestätigter Motorsport-Stand‘ bezeichnete Stand (24.09.) liegt vor, und wo? | W-MO-017 |

---

## 5. Priorität E: Technik und Weltgenerierung

| Nr. | Frage | Ursprung |
|---|---|---|
| OF-51 | Unreal-Version, Landscape-/World-Partition-Parameter, Streaming-Distanzen. | [Modul 18](18-Unreal-Landscape-World-Partition-und-Streaming.md) |
| OF-52 | Hardware-Stufen, Frame-Budgets, Nanite-Strategie, Asset-Budgets. | M-PD §Rendering |
| OF-53 | Innenraum-Streaming, Laufzeitbau von Spielerhäusern. | Block 4 |
| OF-54 | Zeitschichten im GIS; Ladeverfahren für Epochen. | W-RE-006 |
| OF-55 | Terrainquelle (Handzeichnung, Generator, digitalisierte Skizze). | [Modul 17](17-Weltgenerierung-und-GIS-Daten.md) |
| OF-56 | Phase-0-Testumgebung: welcher Ausschnitt (1–3 km²) dient dem Vertical Slice? | M-MS1013 |
| OF-57 | Leistungs- und Journal-Policy der Persistenz (D19-OQ Nr. 9, 10). | D19-OQ |
| OF-69 | Exakte Position des Hunter-HQ im 2.200-km²-Kandidaten (die Lagebeschreibung ist dokumentierbar, W-KA-017; Koordinaten sind nicht festgelegt). | W-ST-035; W-KA-017 |
| OF-68 | Darstellung religiöser Bauten und Glaubensgemeinschaften in der Weltfiktion (Abweichung von 23#1022A), Plattform- und Sensibilitätsprüfung. | W-UR-019 |
| OF-65 | Tod in Hölle, Himmel und Fegefeuer: Checkpoint-/Rettungsregeln (Entwickler: „vielleicht aber noch anders“). In `Spielwelt-und-Untergrund.md` §15 als offen geführt (N-18). | W-RE-012 |
| OF-66 | Zeitverhältnis Erde ↔ Fegefeuer und ↔ Himmel. Die Hölle ist geklärt (ungefähr 40 Jahre : 4 Monate, 22.10., `PE-E1022`); offen bleiben Synchronisierung, Zeitmessung, Übergänge zwischen Reichen und Online-Behandlung. | W-RE-011; W-RE-014 |
| OF-67 | Biologie, Optik, Hierarchie und Schwächen der Leviathane; Zeitpunkt ihres Auftretens. | W-RE-015 |
| OF-61 | Save-Größenziel bei ~500.000 NPC-Identitäten (K13). | W-TV-011 |
| OF-62 | Welche Simulationsebenen (Tier 0 bis 3) gelten, wenn nur Pauschalantworten vorliegen? | W-TV-010 |

---

## 6. Quellenlücken

1. Entscheidungen ab 20.10.2026 (mit Ausnahme der drei Primärentscheidungen vom 22.10., `PE-E1022`).
2. Rohdaten aller Kartenkandidaten (V3–V7, V1.x, `SPN_SCALE_MASTER_V1`), Papierskizze.
3. Bildliche Auswertung der 105-MB-Rasterkarten und der Referenzkarte „Meridian-Reich“.
4. Konsolidierte Dokumente zu Realm-Geografie (nur Planungsbereiche).
5. Antworten des Entwicklers auf 45#278A–284A (Städtefragen) nach 13.10.
6. Chats 38, 40 (Hauptstory/Weiteres) nicht auf Weltaussagen durchsucht; Chats 17 bis 19 nur über Stichwortzählung eingeordnet.
7. Das Adult-Material der Welt (Nachtleben) ist in `Docs/Game/Adult/` dokumentiert und hier nicht wiederholt.
8. Chat 35 (Nachrichten 99–875): alle Entwicklernachrichten gesichtet, aber 370 Assistentennachrichten nur per Stichwortzählung ([Modul 27](27-Abdeckungsprotokoll-Chat-35.md)).
9. Assistentenantworten der Chats 23, 30, 45, 48 und 54: nur Treffer mit Welt-Stichwörtern gelesen und nur dort, wo eine Entwicklerantwort folgt ([Modul 23 §10](23-Quellenabdeckung-und-Auditbericht.md#10-fortsetzung-chat-35-und-assistentenantworten)). Chat 45#62–284 und Chat 48 wurden nur nach Treffern durchsucht.
