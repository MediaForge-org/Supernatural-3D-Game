# Welt 23: Quellenabdeckung und Auditbericht

> [!IMPORTANT]
> **Etappe 3 ist nicht vollständig abgeschlossen.** Dieses Modul sagt ehrlich, was gelesen, was nur per Stichwortzählung eingeordnet und was nicht ausgewertet wurde, und welche Prüfungen tatsächlich liefen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Welt-Dokumentation ([Index](README.md)).

**Inhalt**

1. [Zusammenfassung](#1-zusammenfassung)
2. [Abdeckungsmatrix nach Themen](#2-abdeckungsmatrix-nach-themen)
3. [Quellen: vollständig, teilweise, nicht ausgewertet](#3-quellen-vollständig-teilweise-nicht-ausgewertet)
4. [Nicht erledigter Umfang](#4-nicht-erledigter-umfang)
5. [Durchgeführte Prüfungen](#5-durchgeführte-prüfungen)
6. [Reproduzierbarkeit](#6-reproduzierbarkeit)
7. [Kennzahlen](#7-kennzahlen)
8. [Zeilenzahlen](#8-zeilenzahlen)
9. [Ehrliche Grenze der Quellen](#9-ehrliche-grenze-der-quellen)

---

## 1. Zusammenfassung

- 29 Dateien in `Docs/Game/World/` (24 Module von Hand einschließlich dieses Moduls, 3 Module generiert (24, 25, 27), README); Etappen 3A (Quellen, Register, Konflikte) und große Teile von 3B bis 3E sind bearbeitet.
- Der Umfang ist **durch die Quellen begrenzt**, nicht durch den Willen zu 10.000 Zeilen: Für Fahrzeuge, Untergrund-Architektur, Unreal-Technik und Wirtschaft liefert der Master nur Richtungen. Die Module sagen das jeweils ausdrücklich.
- Nicht erledigt: vollständige Durchsicht aller Chats, bildliche Auswertung der Karten, Abschluss 3F.

---

## 2. Abdeckungsmatrix nach Themen

Grad: ● gründlich gelesen und dokumentiert · ◐ gezielt gelesen, Lücken benannt · ○ nur eingeordnet · – nicht ausgewertet.

| Nr. | Themenbereich (Auftrag §4) | Wichtigste Quellen | Grad | Module |
|---|---|---|:---:|---|
| 1 | Weltkarte, Geografie | D18/D19, M-CA0906, Chats 23/30/35/45/54 | ◐ | 02–05, 17, 19 |
| 2 | Metropolis | D18-MAP, D19-REQ, 23#881E, 35#87E, 45#58E–45#284A | ● | 06 |
| 3 | Weitere Städte | D18-MAP, 35#87E–35#98A, 45#282A | ◐ | 07 |
| 4 | Regionen, Wälder, Berge, Gewässer | D18/D19, M-WB17 Block 6, Chat 23 | ◐ | 04, 05 |
| 5 | Straßen, Bahn, Verkehr | D15-RW, G7-GPKG, M-WB17 Block 5 | ● (Bahn), ◐ (Straße) | 09 |
| 6 | Unterirdische Stadt | D14–D17, M-AD1015/1018, Chat 43/48 | ● | 10, 11 |
| 7 | NPC-Gesellschaft, Wirtschaft | D15-U, M-WB17 Block 2/3, M-WB89 Block 8 | ◐ | 12, 16 |
| 8 | Fahrzeuge, Fahrphysik | M-PD, D15-RV, M-B38, 23#1025E | ◐ | 13, 14 |
| 9 | Motorsport, Nebenaktivitäten | M-WB17 Block 7, 35#96A, 23#1035E | ◐ | 15, 08 |
| 10 | Weltgenerierung, GIS, Heightmaps, Landscape | G7-*, Chat 45, M-PD | ◐ | 17, 18, 24 |
| 11 | Kartenvarianten, Referenzbilder | D18-REV, Chat 30/45/54 | ◐ (Berichte), – (Bilder) | 19 |
| 12 | Entscheidungen, Änderungsprotokolle, offene Fragen | alle | ◐ | 20–22 |

---

## 3. Quellen: vollständig, teilweise, nicht ausgewertet

### 3.1 Vollständig gelesen

- Alle Dokumente der Tages-Deltas 14.–19.10. zu Welt, Untergrund, Bahn, Straßenfahrzeugen, Karte (Canon-, Spec-, Status-, Open-Question-, Comparison- und Provenance-Dateien der genannten Verzeichnisse).
- `M-CA0905`, `M-CA0906`, `M-CU0908`, `M-W1500`, `M-W700`, `M-MS1013`, `M-AD1015`, `M-AD1018`, `M-DR1015` (Auszug bis D-015), `M-WB17`, `M-WB89`, `M-W15R`, `M-LATE`.
- `M-PD` Abschnitte World, Technology, World state, Vehicles, Vehicles/racing, Cash/law, Rendering; `M-B38`.
- `G7-LCS`, `G7-V3`, `G7-PA`-Dokumente, `G7-CLOSE`-Dokumente, GeoPackage (Tabelleninhalt per Skript).
- Chat 43 (5 Nachrichten).

### 3.2 Teilweise gelesen

| Quelle | Gelesen | Nicht gelesen |
|---|---|---|
| Chat 23 (1.109 Nachrichten) | Entwicklernachrichten mit mindestens zwei Weltstichwörtern; ca. 45 Nachrichten ganz; Assistentenantworten mit ≥ 4 Welt-Stichwörtern samt folgender Entwicklerantwort (rund 185 Paare gesichtet), 458A/460A/465A/467A/908A/910A/1022A inhaltlich | übrige Assistentenantworten und Entwicklernachrichten ohne Stichwort |
| Chat 35 (951) | 40–41, 83–98, 117, 876–950 (Auszüge); 99–875: alle 370 Entwicklernachrichten, 16 Assistentenantworten im Kontext, 21 nur Überschriften ([Modul 27](27-Abdeckungsprotokoll-Chat-35.md)) | 370 Assistentennachrichten im Bereich 99–875 nur per Stichwortzählung; Nachrichten 1–39, 42–82 nur Stichwortfilter |
| Chat 30 (271) | Entwicklernachrichten (Köpfe), vollständig 232 | Assistentenantworten |
| Chat 45 (284) | Entwicklerköpfe; 60, 62, 80, 216, 241, 253, 257, 265, 274, 278–284 gezielt; 04.10.-Block 1–57 (Assistentenantworten 3A, 5A, 7A, 13A, 17A, 19A, 21A bis 37A inhaltlich); Assistentenantworten ab 62 nach Treffern | Prozessberichte V3.1–V1.3 (Zahlen siehe Modul 19) |
| Chat 48 (297) | Stichwortfilter, Nachrichten 183–213 (Auszug); 39–45 und 131, 173–175 mit Assistentenkontext; alle Assistentenantworten mit ≥ 2 Treffern samt Entwicklerantwort gesichtet | Gameplay-Themen (Companion, Medizin, Gefängnis, Monster) nicht in die Weltdokumentation übernommen |
| Chat 54 (137) | Entwicklernachrichten; Assistentenantworten mit ≥ 5 Treffern (22) als Prozessberichte gesichtet, 3A und 7A inhaltlich | übrige Berichte; keine Entwicklerentscheidungen darin |
| `M-PD`, `M-OD` | genannte Abschnitte | übrige |
| Prompt-Bibliothek | Titel der Welt-/Fahrzeug-Prompts | Inhalte |

### 3.3 Nur eingeordnet (Stichwortzählung)

Chats 17, 18, 19, 34, 37, 38, 40, 47; `04_HISTORY` (mehrfach kopierte Kanon-Stände); `_SOURCE_ARCHIVE` (außer `World Current` und `World History Loose Early`: Spezifikation V3).

### 3.4 Nicht ausgewertet

Binärdateien (PNG, TIFF, SVG, GPKG-Geometrien außer maschinell gemessene Flächen/Längen); `06_NEW_REFERENCES` (Karte des Meridian-Reichs); Videos in `_SOURCE_RESEARCH`; Assistentenantworten zu den Entwicklerfragen vom 13.10.; alle Quellen außerhalb des Masters (Vault, Delta-Pakete) für die Welt.

### 3.5 Fehlende Quellen

Entscheidungen ab 20.10.2026; Rohdateien aller Kartenkandidaten und der Papierskizze; Antwort des Entwicklers auf die 110/160-Frage; Dokumente zu Realm-Geografie.

---

## 4. Nicht erledigter Umfang

| Offener Teil | Grund |
|---|---|
| Vollständiges Lesen der Assistentenantworten in Chats 23/30/45/48/54 (rund 4,8 MB) | Sie sind keine Belege für Entscheidungen; gelesen wurden nur Treffer mit folgender Entwicklerantwort ([Abschnitt 10](#10-fortsetzung-chat-35-und-assistentenantworten)) |
| Bildliche Auswertung der Karten | nicht möglich/ nicht beauftragt |
| Etappe 3F: vollständige Redundanzprüfung, Prüfung aller Flächenangaben maschinell | nur Stichprobe ([Abschnitt 5](#5-durchgeführte-prüfungen)) |
| Vertiefung der Module 13–18 | Quellen lassen wenig zu; weitere Vertiefung nur mit neuen Entwicklerentscheidungen sinnvoll |
| Abgleich mit `Docs/Game/Spielwelt-und-Untergrund.md` | Dort bleibt der Überblick stehen; Abweichungen siehe [Abschnitt 9](#9-ehrliche-grenze-der-quellen) |

---

## 5. Durchgeführte Prüfungen

Alle mit `Tools/world-audit/` ausgeführt; Ergebnisse stammen vom letzten Lauf.

| Prüfung | Ergebnis |
|---|---|
| Chat-Zitate `NN#m[E/A]` (Existenz, Rolle) | siehe Abschnitt 7 |
| Wörtliche Zitate „…“ gegen Chats und Master-Texte | siehe Abschnitt 7 |
| Quellenkürzel definiert | alle verwendeten Kürzel in Modul 01 definiert |
| Register-IDs | keine Duplikate; jede referenzierte ID existiert |
| Relative Links und Anker in `Docs/Game/World` | siehe Abschnitt 7 |
| Flächenangaben | Metropolis-/Festlandrechnungen in Modul 03 händisch nachgerechnet; WORLD_700-Flächen aus Geometrie berechnet (Summe der Staaten = 733,84 km² = Hüllfläche) |
| Verwechslung Festland/Gesamtfläche | in Modul 03 §11 und 21 ausgewiesen |
| Redundanz | Zahlen stehen mehrfach (Register, Modul 03, Fachmodul); Quelle der Wahrheit ist das Register |

---

## 6. Reproduzierbarkeit

```bash
SPN=/pfad/zu/SPN_FULL_CUMULATIVE_MASTER_2026-09-27
python3 -I Tools/world-audit/inventory.py <arbeitsverzeichnis>
python3 -I Tools/world-audit/split_chats.py $SPN <arbeitsverzeichnis>
python3 -I Tools/world-audit/render_w700.py "$SPN/_SOURCE_ARCHIVE/World Current/04_WORLD_700_CURRENT/vectors/open_world_master_iteration_8_3.gpkg" Docs/Game/World/24-WORLD700-GIS-Referenz.md
# Modul 25 (Weltaussagen-Index), nach Änderungen an Modul 20 bis 22 neu erzeugen:
(cd Docs/Game/World && python3 -I ../../../Tools/world-audit/render_index.py . 25-Weltaussagen-Index.md)
# Modul 27 (Abdeckungsprotokoll Chat 35):
(cd Docs/Game/World && python3 -I ../../../Tools/world-audit/render_chat35.py <arbeitsverzeichnis>/msgs35.json ../../../Tools/world-audit/chat35_read_log.json . 27-Abdeckungsprotokoll-Chat-35.md)
SPN_MASTER=$SPN SPN_WORLD_DOCS=Docs/Game/World python3 -I Tools/world-audit/check_docs.py <arbeitsverzeichnis>
python3 -I Tools/world-audit/check_links.py Docs/Game
```

`inventory.py` schreibt `topic_files.tsv` (Themenzähler je Datei). Alle Skripte nutzen nur die Python-Standardbibliothek und lesen den Master ausschließlich.

---

## 7. Kennzahlen

| Kennzahl | Wert |
|---|---:|
| Register-Einträge (`W-XX-nnn`) | 302 |
| davon 📋 (inkl. Mischstatus) | 194 |
| davon 🧭 | 31 |
| davon 💡 | 30 |
| davon ❓ | 24 |
| davon 🕰️ | 45 |
| davon ⚠️ | 5 |
| davon 🔭 | 2 |
| Konflikte | 13 (K1–K13), davon 8 offen (⚠️/❓); K1 am 22.10. geklärt; frühere Angabe ‚6 offen‘ war zu niedrig gezählt |
| Offene Fragen | 69 (OF-01 bis OF-69), davon OF-11 geklärt, OF-12 und OF-66 teilweise geklärt |
| Geprüfte Chat-Zitate | 1006 Stellen, 0 Probleme |
| Geprüfte wörtliche Zitate | 221 geprüft, 0 nicht auffindbar |
| Link-Prüfung `Docs/Game/World` | 0 Fehler (2 vorbestehende Fehler außerhalb des Ordners in `Docs/Game/Adult`) |

(Mehrfachstatus werden in mehreren Zeilen gezählt; die Summe ist größer als 302.)

---

## 8. Zeilenzahlen

Gemessen mit `wc -l` zum Abschlusslauf. Der Zuwachs ist **netto neu**; bestehende Dokumente (Spielwelt-und-Untergrund u. a.) wurden nicht geändert.

| Datei | Zeilen |
|---|---:|
| 01-Quellenlage-und-Methodik.md | 181 |
| 02-Weltvision-und-geografische-Grundstruktur.md | 185 |
| 03-Massstab-Flaechen-Proportionen.md | 223 |
| 04-Berge-Waelder-und-Wildnis.md | 175 |
| 05-Gewaesser-und-Hydrologie.md | 147 |
| 06-Metropolis.md | 189 |
| 07-Weitere-Staedte-und-Siedlungen.md | 125 |
| 08-Raeumliche-Allokation-Orte-und-Aktivitaeten.md | 115 |
| 09-Strassen-Schiene-und-Verkehrsinfrastruktur.md | 122 |
| 10-Untergrundzivilisation.md | 129 |
| 11-Untergrund-Architektur-und-Infrastruktur.md | 97 |
| 12-Gesellschaft-Wirtschaft-und-Ressourcen.md | 95 |
| 13-Fahrzeuge-und-Fahrzeugklassen.md | 107 |
| 14-Fahrphysik-und-Fahrzeuginteraktion.md | 93 |
| 15-Motorsport-und-Rennaktivitaeten.md | 99 |
| 16-NPC-Verkehr-und-Verkehrssimulation.md | 76 |
| 17-Weltgenerierung-und-GIS-Daten.md | 124 |
| 18-Unreal-Landscape-World-Partition-und-Streaming.md | 123 |
| 19-Kartenreferenzen-und-Versionsgeschichte.md | 168 |
| 20-Entscheidungsregister.md | 457 |
| 21-Konflikte-und-Widersprueche.md | 142 |
| 22-Offene-Fragen-Katalog.md | 133 |
| 23-Quellenabdeckung-und-Auditbericht.md | 246 |
| 24-WORLD700-GIS-Referenz.md | 258 |
| 25-Weltaussagen-Index.md | 193 |
| 26-Reiche-Zeitreise-und-historische-Weltzustaende.md | 78 |
| 27-Abdeckungsprotokoll-Chat-35.md | 106 |
| 28-Nachtragsvorschlag-Spielwelt-und-Untergrund.md | 395 |
| README.md | 62 |
| **Summe** | **4643** |

Davon generiert (Module 24, 25 und 27): 555 Zeilen; von Hand geschrieben: 3646 Zeilen. Hinzu kommen ca. 560 Zeilen Skripte in `Tools/world-audit/`.

**Hinweis:** Das Ziel von 10.000–15.000 Zeilen pro großem Themenbereich wurde nicht erreicht. Der Grund liegt in den Quellen: Der Master enthält für die Weltkarte etwa 190 KB an Delta-Dokumenten, davon wenige Dutzend KB mit Weltinhalt, und keine Spezifikation für Fahrzeuge, Untergrund-Architektur oder Unreal-Weltaufbau. Aufblähen wurde vermieden.

---

## 9. Ehrliche Grenze der Quellen

1. Die Weltkarte ist nicht akzeptiert; die Geometrie fehlt.
2. Ab 20.10. fehlen Entscheidungen, mit Ausnahme der drei Primärentscheidungen vom 22.10.2026 (`PE-E1022`, siehe [Abschnitt 11](#11-kanon-update-vom-22102026)).
3. `Docs/Game/Spielwelt-und-Untergrund.md` stützt sich auf denselben Master; zwei Ergänzungen dieses Ordners gehen darüber hinaus und sollten dort nachgetragen werden: (a) der 09.09.-Plan (WORLD_2000: sieben Staaten, ≥ 7 Flughäfen, 16–22 Motorsportanlagen), (b) die Entwicklerantwort zur Hölle-Zeit (40 Jahre : 4 Monate) und zur Geografie beim Zeitreisen (35#41E). Das Dokument wurde nicht angefasst.
4. Statusberichte aus Karten-Chats sind nicht überprüft.
5. Die Zuordnung ‚110 km² = Gesamtgebiet mit Vororten‘ war eine Folgerung und ist seit dem 22.10.2026 als Primärentscheidung bestätigt (W-ST-003, `PE-E1022`).

---

## 10. Fortsetzung: Chat 35 und Assistentenantworten

**Chat 35, Nachrichten 99–875** (777 Nachrichten, 370 vom Entwickler): Alle Entwicklernachrichten wurden angezeigt (318 vollständig, 52 gekürzt mit Stichwortprüfung des Rests). Der weit überwiegende Teil betrifft Adult-System, Mod-Architektur, Werkzeuge und Archivarbeit. Weltrelevante Funde und die Tiefe pro Nachricht stehen in [Modul 27](27-Abdeckungsprotokoll-Chat-35.md). Neue Register-Einträge daraus: W-GE-032 bis 034, W-ST-031 bis 033, W-WI-007, W-FZ-017/018, W-MO-014 bis 017, W-TV-010/011.

**Assistentenantworten der Chats 23, 30, 45, 48, 54:** Methode: Treffer mit Welt-Stichwörtern, jeweils zusammen mit der folgenden Entwicklernachricht. Als Beleg dient nur die Entwicklerantwort; Zahlen und Vorschläge des Assistenten bleiben 💡 oder 🕰️. Chat 30 (243 Assistentenantworten) und Chat 54 enthalten fast nur Prozessberichte zu Kartenläufen ohne Entscheidung.

| Fund | Wirkung |
|---|---|
| 04.10. „Geografische Masterstruktur v1“ (45#3A bis 45#21A) fehlte im Register | W-GE-035 bis 038, W-NA-012/013, W-ST-034/035; Metropolis-Chronik ergänzt (20–25 km² bereits am 04.10.) |
| 26.08. Jenseitsablauf, Höllenzeit mit Richtung, Fegefeuer, Leviathane (23#458A bis 23#468E) | W-RE-008 bis 015, OF-65 bis 67; Hölle-Zeitrichtung (W-RE-005/011) klärt sich 🧭 |
| 31.08./01.09. Metropolis, Abstände, Downtown, Metropolitan Belt (23#699E bis 23#911E) | W-ST-036/037; Hinweis zu K1 (110 km², Stadtgebiet gegenüber Region) |
| 09.10. Städtevision und Kartengrößenvorschlag (48#39E bis 48#45E) | W-ST-038, W-GE-039; Ursprung der Städtegrößen-Debatte (K1) |
| Block-4-Antworten (23#1023E) | W-UR-018/019, W-FZ-019, OF-68 |
| Geheime Forschungskomplexe, Bunker einzeln planen (48#131E, 48#173E, 48#175E) | W-UG-025/026 |
| Prototypberichte Chat 54 | W-KA-016; K4 ergänzt (4.400–4.900 m, Berichtswert 4.600 m) |
| Save-Größe (35#279E/280A) | K13, W-TV-011 |

**Nicht aufgelöst:** K1 (Metropolis), K4 (Gipfelhöhe), K8 (Anlagenzahlen) und K13 sind nur ergänzt, nicht entschieden. Eine Entscheidung des Entwicklers liegt für keinen davon vor.

**Grenzen:** Assistentennachrichten ohne Welt-Stichwort, Nachrichten ohne Entwicklerantwort und Bilder wurden nicht gelesen; weltanschauliche Aussagen im Adult-Kontext (zum Beispiel 35#500E, 35#532E) werden dort geführt und hier nur verwiesen.

**Nachtragsvorschlag:** [Modul 28](28-Nachtragsvorschlag-Spielwelt-und-Untergrund.md) sammelt 28 Vorschläge für `Docs/Game/Spielwelt-und-Untergrund.md`; die Datei selbst ist unverändert und jeder Vorschlag braucht deine Freigabe.

---

## 11. Kanon-Update vom 22.10.2026

Der Projektinhaber hat am 22.10.2026 drei bisher offene Fragen ausdrücklich entschieden (`PE-E1022`). Es sind **neue Primärentscheidungen**; sie stehen nicht im Master und werden nicht als dort enthalten ausgegeben.

| Entscheidung | Status | Geänderte Register-IDs | Konflikt/Frage | Geänderte Dateien |
|---|:---:|---|---|---|
| Metropolis: 110 km² zusammenhängendes Stadtgebiet einschließlich Vororten (nicht nur Innenstadt, nicht Metropolregion), Teil der 2.200 km² | 📋 | W-ST-002, W-ST-003, W-ST-037 (Anmerkung) | K1 (Abgrenzung) geklärt, OF-11 geklärt, OF-12 teilweise | Module 01, 03, 06, 19, 20, 21, 22, 23, Übersichtsdatei |
| Höllenzeit: 40 Jahre Hölle ≈ 4 Erdmonate; Zeit vergeht in der Hölle wesentlich schneller | 📋 | W-RE-005, W-RE-011 | OF-66 teilweise (Hölle geklärt; Technik und Fegefeuer/Himmel offen) | Module 20, 22, 26, Übersichtsdatei |
| Hunter HQ: geografische Lage in der öffentlichen Weltdokumentation erlaubt (keine Geheimeingänge, Story-Enthüllungen, Interna) | 📋 | W-KA-017 (neu), Anmerkungen W-KA-008, W-ST-035, W-UG-023 | OF-69 (exakte Position) neu | Module 02, 05, 17, 19, 20, 22, Übersichtsdatei |

**Prüfungen:** Die Metropolis-Chronik (Modul 03 §4) bewahrt alle historischen Werte und führt die neue Zeile 22.10.; die Obergrenze vom 27.09. ist als überholt gekennzeichnet. Die Höllenzeit steht in den Modulen 20, 26 und der Übersichtsdatei einheitlich (ungefähr 40 : 4 Monate, Technik offen). Die HQ-Lage enthält keine Koordinaten; die des Entwurfs vom 04.10. gehören zu einer früheren Hülle. Der Nachtragsvorschlag N-13, N-14 und N-17 wurde neu gefasst und übernommen; N-16 wurde am 22.10.2026 freigegeben und umgesetzt (20–25 km² als historischer Planungsstand, Chronologie erhalten). Für N-04, N-12, N-15, N-18 bis N-22 und N-26 bis N-28 lag eine Entscheidungsvorlage vor ([Modul 28 §11](28-Nachtragsvorschlag-Spielwelt-und-Untergrund.md#11-entscheidungsvorlage)); sie wurden am 22.10.2026 abschließend entschieden: übernommen N-12 (gekürzt), N-15, N-18 (nur offene Fragen), N-20, N-21, N-26, N-27, N-28; abgelehnt N-04, N-22; zurückgestellt N-19 (Save-System-Dokumentation). Details: [Modul 28 §10](28-Nachtragsvorschlag-Spielwelt-und-Untergrund.md#10-übernahmestatus).
