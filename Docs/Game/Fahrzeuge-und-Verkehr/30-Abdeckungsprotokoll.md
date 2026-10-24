# Fahrzeuge und Verkehr 30: Abdeckungsprotokoll

> [!IMPORTANT]
> **Dieses Protokoll sagt, was berücksichtigt wurde und was nicht.** Es ist kein Nachweis der Vollständigkeit des Spiels, sondern der Auswertung. Zeilenzahlen und Statistik sind erzeugt (`Tools/world-audit/render_vehicle_register.py`). Ein Zeilenziel nannte der Auftrag nicht; die Qualität der Quellenlage bestimmt die Länge, und dünne Module sind kein Fehler, sondern ein Befund.

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Ausgangslage](#1-ausgangslage)
2. [Abdeckung der vorgesehenen Themen](#2-abdeckung-der-vorgesehenen-themen)
3. [Quellenabdeckung](#3-quellenabdeckung)
4. [Abdeckung der Wechselwirkungen und Tests](#4-abdeckung-der-wechselwirkungen-und-tests)
5. [Befunde, die Bestehendes betreffen](#5-befunde-die-bestehendes-betreffen)
6. [Prüfungen und Ergebnisse](#6-prüfungen-und-ergebnisse)
7. [Änderungen außerhalb des Ordners](#7-änderungen-außerhalb-des-ordners)
8. [Grenzen](#8-grenzen)
9. [Erzeugte Übersicht](#9-erzeugte-übersicht)

---

## 1. Ausgangslage

- Git-Stand bei Beginn: Branch `main`, Commit `9e8598f` (‚docs: expand NPC memory, relationships and living world‘). Der Arbeitsbaum enthielt nicht committete Änderungen aus der Kampfsystem-Arbeit (`Docs/Game/Kampfsystem/`, `Tools/world-audit/render_combat_register.py` sowie Änderungen an vier bestehenden Dateien). Sie blieben unangetastet und wurden nicht committet.
- Bestandsaufnahme vor dem Schreiben: Welt-Module 09, 13, 14, 15, 16, 17, 18 und Register der Gruppen FZ, FP, TV, VK, UR; Gameplay-Systeme §6, §8, §10, §16; Register von NPCs, Ermittlung, Kreaturen und Kampfsystem (Zeilen mit Fahrzeug-, Fahrer-, Kennzeichen-, Polizei-, Notdienst- und Simulationsbezug); Master (`M-PD`, `M-WB17`, `M-WB89`, `M-DC31`, `M-PS31`, `D15-RV`, `D15-OQ`, die Zusätze `M-A1015`, `M-A0906`, `M-EP1018`, `M-OC1018`, Master-Suche mit Tier 1 bis 3); Chat 23 Fragenblock 5 vollständig mit allen 50 Antworten; WorldForge (`RC-01`, Straßendokumente, Koordinaten, Stable IDs, Unreal-Import).
- Ergebnis: [Modul 01](01-Quellenlage-und-Methodik.md) §1 und §6 (was bereits eindeutig entschieden ist, was fehlt), [Modul 02](02-Vision-und-Grundprinzipien.md) (Grundlagen), [Modul 29](29-Konflikte-und-offene-Fragen.md) (Widersprüche, Fragen).
- Bewusst nicht getan: Quellcode, Unreal-Dateien, Assets, WorldForge-Dateien und der Master wurden nicht verändert; es gab keine Git-Schreiboperation.

---

## 2. Abdeckung der vorgesehenen Themen

Die 35 Themen des Auftrags sind auf 27 Fachmodule verteilt (Gliederung nach den Quellen, wie der Auftrag es erlaubt). Spalte „Quellenlage“: **gut** (überwiegend 📋), **mittel** (Teile 📋, Teile 💡), **dünn** (überwiegend 💡, ❓, 🔍).

| Nr. | Thema des Auftrags | Modul | Quellenlage | Befund |
|---:|---|---|---|---|
| 1 | Vision und Gestaltungsprinzipien | [02](02-Vision-und-Grundprinzipien.md) | gut | Prinzipien aus `N-LT-011`, `W-FP-002`, `W-TV-001` |
| 2 | Fahrzeugklassen | [03](03-Fahrzeugklassen-und-Fahrzeugdaten.md) | gut | Klassen in Welt 13; Modul ergänzt Verkehrsrolle und Datenmodell (💡) |
| 3 | Physik und Fahrverhalten | [04](04-Fahrphysik-und-Fahrverhalten.md) | gut (Anforderungen), dünn (Werte) | keine Konstanten |
| 4 | Steuerung und Kamera | [05](05-Steuerung-Kamera-und-Anzeige.md) | mittel | acht Funktionen belegt, Lücken `CU-03` |
| 5 | Einsteigen, Aussteigen, Sitze | [06](06-Einsteigen-Aussteigen-Sitze-und-Kofferraum.md) | dünn | nur automatisches Einsteigen und Kofferraum |
| 6 | Zustand, Verschleiß, Schaden | [07](07-Zustand-Verschleiss-und-Schaden.md) | mittel | Verschleißteile beschlossen, Schaden offen (`CK-06`) |
| 7 | Reparatur, Wartung, Werkstätten | [08](08-Reparatur-Wartung-und-Werkstaetten.md) | mittel | Phasenvertrag, keine Ökonomie |
| 8 | Kraftstoff, Energie, Versorgung | [09](09-Kraftstoff-Energie-und-Tankstellen.md) | gut | Sorten, Verbrauch, E85 |
| 9 | Straßenhierarchie und -typen | [10](10-Strassennetz-und-Strassentypen.md) | dünn | Klassen nur aus WorldForge |
| 10 | Kreuzungen, Signale, Schilder | [11](11-Kreuzungen-Signale-und-Schilder.md) | dünn | fast keine Quelle |
| 11 | Autobahnen, Schnellstraßen | [10](10-Strassennetz-und-Strassentypen.md) §4 | dünn | nur Fernverbindungen und Stau erwähnt |
| 12 | Stadtverkehr, Wohngebiete | [12](12-Stadtverkehr-Parken-und-Garagen.md) §2, [15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md) | dünn | aus NPC-Alltag abgeleitet |
| 13 | Land- und Bergstraßen, Gelände | [10](10-Strassennetz-und-Strassentypen.md) §5 | mittel | Entwicklerwünsche, Spannung `CK-05` |
| 14 | Parken, Garagen, Depots | [12](12-Stadtverkehr-Parken-und-Garagen.md) | mittel | Parkplatzsuche beschlossen, Valet unbelegt |
| 15 | Tankstellen, Rastplätze | [09](09-Kraftstoff-Energie-und-Tankstellen.md) §3 | dünn | Rastplätze unbelegt |
| 16 | Verkehrs-KI und Navigation | [13](13-Verkehrs-KI-und-Navigation.md) | mittel | Fahrerprofile belegt, Modell offen |
| 17 | NPC-Fahrer und Verhalten | [13](13-Verkehrs-KI-und-Navigation.md) §4, [14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md) | gut (NPC-Rahmen) | Einbindung in die NPC-Architektur |
| 18 | Dichte und Tageszeiten | [15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md) | mittel | Absichten ohne Kurven |
| 19 | Stau, Baustellen, Sperrungen | [15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md) | mittel | Absichten ohne Modell |
| 20 | Unfälle und Pannen | [16](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md) | mittel | selten beschlossen, keine Raten |
| 21 | Einsatzfahrzeuge, Sonderverkehr | [16](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md) | mittel | Dienste beschlossen, Tiefe offen |
| 22 | Fußgänger, Radfahrer, Teilnehmer | [17](17-Fussgaenger-Radfahrer-und-weitere-Teilnehmer.md) | mittel | Fahrzeugarten belegt, Fußgängerregeln nicht |
| 23 | Besitz und Zulassung | [18](18-Besitz-Zulassung-und-Diebstahl.md) | mittel | Versicherung und Zulassung aus Block-5-Antworten |
| 24 | Diebstahl und Kriminalität | [18](18-Besitz-Zulassung-und-Diebstahl.md) | dünn | nur Existenz belegt |
| 25 | Polizei und Verkehrsüberwachung | [19](19-Polizei-und-Verkehrsueberwachung.md) | mittel | Prinzipien belegt, Katalog offen |
| 26 | Interaktionen und Transport | [20](20-Interaktionen-Transport-und-Jobs.md) | mittel | Jobs belegt, Umfang offen |
| 27 | Fahrzeuge in Ermittlungen und Jagden | [21](21-Fahrzeuge-in-Ermittlungen-und-Jagden.md) | mittel | Beweisquelle belegt |
| 28 | Persistenz und Lebenszyklen | [22](22-Persistenz-und-Lebenszyklen.md) | mittel | Grundsätze belegt, Format offen |
| 29 | Koop und Netzwerk | [23](23-Koop-und-Netzwerk.md) | dünn | Rahmen belegt, Architektur offen |
| 30 | Unreal-Integration | [25](25-Unreal-Integration.md) | mittel | Decision 0006; Plugins ungeprüft |
| 31 | Verkehrssimulation und LOD | [26](26-Simulationsstufen-LOD-und-Streaming.md) | mittel | NPC-Stufen übernommen |
| 32 | Streaming und World Partition | [26](26-Simulationsstufen-LOD-und-Streaming.md) §4 | dünn | Grundsatz belegt, Details offen |
| 33 | Performance und Skalierung | [27](27-Performance-und-Skalierung.md) | dünn | keine Messwerte |
| 34 | Automatisierte Tests und Validierung | [31](31-Testkatalog-Phasen-und-Abnahme.md), [24](24-WorldForge-Schnittstelle.md) §7 | – (Eigenleistung) | Tests aus Register abgeleitet |
| 35 | Implementierungsphasen und Abnahme | [31](31-Testkatalog-Phasen-und-Abnahme.md) §17 | – (Eigenleistung) | Entwurf aus Phasenvertrag |
| Z | Zusatzthema: WorldForge-Schnittstelle (Pflichtthema des Auftrags) | [24](24-WorldForge-Schnittstelle.md) | dünn | `RC-01` ist Vorvertrag |

---

## 3. Quellenabdeckung

| Quellengruppe | Stand |
|---|---|
| Welt-Dokumentation (Module 09, 13, 14, 16 vollständig; 15, 17, 18 abschnittsweise) und Welt-Register | gelesen, zitiert |
| Gameplay-Systeme §6 vollständig; §8, §10, §16 abschnittsweise | gelesen |
| NPCs 21, 22, 23 (Abschnitte zu Persistenz, Stufen, Koop) und Register | gelesen, zitiert |
| Ermittlung (Register, Module 05, 07, 18) und Kreaturen (Register, Module 07, 09) | Zeilen mit Bezug gelesen |
| Kampfsystem (Register, Module 05, 06, 13, 14, 20) | Zeilen mit Bezug gelesen |
| Master: `M-PD`, `M-WB17`, `M-WB89`, `M-DC31` (§3, §12, §13, §30 bis §36, §38, §39, §47, §48), `M-PS31` (§1, §37 bis §54), `D15-RV`, `D15-OQ`, `M-CA0905`, `M-MS1013`, `M-A1015`, `M-A0906`, `M-EP1018`, `M-OC1018`, `M-OD` ‚Vehicles / racing‘ | die genannten Abschnitte gelesen; Master-Suche mit sieben Begriffen (Tier 1, 2) und fünf weiteren (Tier 1 bis 3) |
| Chat 23: Fragenblock 5 (`1024A`) und Antworten (`1025E`) vollständig; `613E`, `514E`, `575E`, `705E`, `822E`, `823E` | gelesen |
| Chat 35, 30, 45 | nur über Zitate der Welt-Dokumentation |
| Decisions 0004, 0006; Proposals 0004, 0007, 0009, 0010 | gelesen (soweit zitiert) |
| WorldForge: `RC-01`, `DOMAIN-03`, `Road Generator`, `DC-01`, `C-09`, Unreal-Import (Auszüge), Phasenplan, README | gelesen |
| `Core/` und `Unreal/SupernaturalProt/Source/` | Dateistruktur und die zitierten Header gelesen; Suche nach Fahrzeugcode ohne Treffer |
| Prompt-Bibliothek, Backlog `M-B38`, Romanmaterial, Adult-Material | nicht als Anforderung gelesen |

Fehlende Quellen: [Modul 01](01-Quellenlage-und-Methodik.md) §8.

---

## 4. Abdeckung der Wechselwirkungen und Tests

| Wechselwirkung | Wo |
|---|---|
| Kampf, Verletzung, Medizin | Module 07, 16, 17, 21 |
| Ermittlung, Beweise, Zeugen | Module 18, 19, 21 |
| Polizei, Verbrechen, Recht | Module 18, 19 |
| NPC-Verhalten und -Gesellschaft | Module 13, 14, 20 |
| Wetter, Jahreszeit, Tageszeit | Module 04, 10, 15 |
| Wirtschaft | Module 08, 09, 18, 20 |
| Koop | Modul 23 |
| Speichern und Laden | Modul 22 |
| Streaming und Leistung | Module 26, 27 |

Tests: 14 Kategorien und 6 Querschnittsszenarien in [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md); die Zahl der Register-IDs, die ein Test referenziert, steht in §9.2.

---

## 5. Befunde, die Bestehendes betreffen

> Keine bestehende Datei wurde zur Korrektur geändert. Die Befunde sind Vorschläge für die Projektleitung.

1. **Das Welt-Register erfasst einen Teil der Block-5-Antworten nicht oder nur teilweise.** Nicht oder nicht ausdrücklich als eigene Zeile geführt sind: Versicherung (`23#1025E` Nr. 16), Zulassung (Nr. 17), Abschleppdienst als Spielerjob (Nr. 41), Fahrzeugtransport (Nr. 42), Autotreffen, Straßenrennen und Trackdays (Nr. 43 bis 45), Parkplatzsuche (Nr. 46), mechanische Schäden zusätzlich zur Deformation (Nr. 15), Zweiräder, E-Bikes und E-Scooter (Nr. 24 bis 29). Dieser Ordner führt sie mit Verweis auf die Antwortnummer.
2. **`W-VK-013` übernimmt Valet-Parken als beschlossen**, während die Antwort des Entwicklers ‚was ist das‘ lautet (`CU-02`).
3. **`W-FZ-013` (Schaden offen) und `23#1025E` Nr. 15 (mechanische Schäden ja)** sind nicht abgeglichen (`CK-06`).
4. **`W-FZ-006` (‚manuelle Variante für jedes Fahrzeug‘)** gilt wörtlich auch für Fahrzeuge ohne Schaltgetriebe (`CU-01`).
5. **Fahrtrichtung (Rechts- oder Linksverkehr) ist nirgends festgelegt** (`CU-06`).
6. **Der NPC-Speicherentwurf hat kein Fahrzeugsegment**; [NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md) nennt Fahrzeuge nur im konzeptionellen Beispiel `N-PE-023`. Modul 22 schlägt Segmente vor.
7. **Decision 0004 §6** ordnet Fahrzeugzustand außerhalb von `WorldFacts` ein; `M-PD` führt Fahrzeuge im Weltzustand. Modul 22 liest das als Konkretisierung, nicht als Widerspruch.
8. **WorldForge `RC-01`** deckt die Kantenebene ab, nicht die Fahrspur- und Regelebene (Modul 24).
9. **Kampfsystem:** Die Tastenbelegung von Kreuz und Quadrat überschneidet sich mit der Fahrbelegung (`CK-01`); das Schießen aus Fahrzeugen ist dort offen.
10. **Zwei Master-Dokumente mit Fahrzeugabschnitten fehlen in den Quellenschlüsseln der Welt-Dokumentation:** `M-DC31` (‚CANONICAL DELTA‘, ersetzt widersprechende ältere Formulierungen in `M-PD`) und `M-PS31`. Sie wurden hier nachträglich eingearbeitet (unter anderem Getriebe an Variante gebunden, Trim-Familien, Fahrzeug-App, Reichweiten, Fahrzeuglager, Passagiere und Körper im Fahrzeug, Sicherheitssysteme, Reifenarten, Spezialisierung der Werkstätten, Alkohol und Fahren). Die Welt-Dokumentation zitiert sie nicht; ob sie dort ergänzt werden sollen, entscheidet die Projektleitung.
11. **Der Zusatz vom 18.10.2026 führt die Fahrzeugdetails des Oktober-Pakets als ‚review-required specification inputs‘**, `D15-RV` nennt Phasenvertrag und Belegung dagegen ‚user-approved‘ und ‚user specified‘ (`CK-07`). Die Welt-Dokumentation führt sie als 📋.
12. **Die Welt-Gruppe VK enthält Arbeitsaufträge an Kartenläufe (🕰️)** neben Planungsrichtung (`W-VK-007`, `W-VK-008`); sie werden hier nur als 🧭 geführt.

---

## 6. Prüfungen und Ergebnisse

Alle Prüfungen wurden lokal auf Fedora ausgeführt, nach der letzten Änderung des Ordners.

| Prüfung | Werkzeug | Ergebnis |
|---|---|---|
| Chat-Zitate (Schlüssel `NN#m` mit Rolle E oder A): Nachricht existiert, Rolle passt zum Suffix | `check_docs.py` | 144 Zitate, 0 Probleme |
| Wörtliche Zitate in „…“ stehen in Chats oder Master | `check_docs.py` | 9 Zitate, 0 nicht gefunden |
| Quellenschlüssel (`M-…`, `D1x-…`, `PE-…`) sind in Modul 01 definiert | `check_docs.py` | 25 Schlüssel definiert und benutzt, 0 undefinierte (die WorldForge-Schlüssel `WF-…` prüft das Werkzeug nicht; sie sind in Modul 01 §4.2 definiert) |
| Register-IDs eindeutig, jede Referenz aufgelöst | `check_docs.py`, `render_vehicle_register.py` | 407 Einträge, 0 Duplikate, 0 fehlende |
| Zellenzahl, Gruppe, Statuswort, Modulzuordnung | `render_vehicle_register.py` | 0 Probleme |
| Verweise auf Nachbarregister (`W-`, `N-`, `E-`, `K-`, `COMBAT-DEC-`) lösen auf | `render_vehicle_register.py` | 824 Verweise geprüft, 0 nicht auffindbar |
| Verweise `CQ`, `CK`, `CU` aufgelöst | `render_vehicle_register.py` | 118 Fragen, 7 Konflikte, 9 unbelegte Angaben definiert; 0 undefinierte Verweise; jede Frage wird von einem Modul referenziert |
| Test-IDs eindeutig | `render_vehicle_register.py` | 97 Tests, 0 Duplikate |
| Relative Links und Anker | `check_links.py` | 6.163 Links, 2 kaputt (beide bereits vorhanden, Adult-Dokumentation, nicht verändert); 0 kaputt im neuen Ordner |
| Leerzeichen am Zeilenende, CRLF | `grep` | keine |
| `git diff --check` | Git (lesend) | sauber (prüft nur verfolgte Dateien; für den neuen, nicht verfolgten Ordner wurde `grep` auf Leerzeichen am Zeilenende und CRLF genutzt) |

Die Prüfung `check_docs.py` wurde für diesen Ordner erweitert: Die Umgebungsvariable `SPN_GENERATED` legt fest, welche Modulpräfixe als erzeugt übersprungen werden (Standard `25-` wie bisher; hier `28-`). Ohne diese Erweiterung hätte das Werkzeug das Modul 25 dieses Ordners übersprungen.

Während der Arbeit wurden folgende Probleme **durch diesen Auftrag verursacht und behoben**: vier wörtliche Zitate in „…“, die nicht verbatim in den Quellen stehen (in einfache Anführungszeichen umgestellt oder umformuliert), ein nicht definierter Quellenschlüssel (`WF-RD`, `WF-EX` in Modul 01 ergänzt), zwei erst spät gefundene Quellen (`M-DC31`, `M-PS31`; nachgearbeitet in den Modulen 02, 03, 04, 05, 06, 07, 08, 09, 13, 19, 20, 21, 23, 25, 27, 31), ein Verweis auf eine nicht referenzierte Frage (`CQ-53`) und Verwechslungsgefahr mit Kennungen des Kampfsystems (`CK-01`, `CQ-18`, `CU-01`; Verweise umformuliert).

---

## 7. Änderungen außerhalb des Ordners

| Datei | Änderung | Grund |
|---|---|---|
| `Docs/MasterIndex.md` | neuer Abschnitt ‚Vehicles, roads and traffic‘ | Auffindbarkeit |
| `Docs/Game/Gameplay-Systeme.md` | ein Verweis unter der Überschrift §6 | Querverweis |
| `README.md` | eine Zeile in der Tabelle der Dokumentationen | Auffindbarkeit |
| `Tools/world-audit/README.md` | Abschnitt ‚Fahrzeuge und Verkehr‘ | Werkzeugbeschreibung |
| `Tools/world-audit/render_vehicle_register.py` | neu | Register-, Statistik- und Querverweisgenerator |
| `Tools/world-audit/check_docs.py` | eine Zeile: `SPN_GENERATED` | übersprungene erzeugte Module konfigurierbar |

Es wurden **keine** Dateien gelöscht und keine Dateien im Master, in `Unreal/`, in `Core/`, in WorldForge oder in `Docs/Setup/` verändert. Es wurde nicht committet, gepusht, getaggt oder released.

---

## 8. Grenzen

1. Die Auswertung der Chats ist auf die in Abschnitt 3 genannten Bereiche begrenzt. Weitere Fahrzeug- und Verkehrsaussagen in nicht gelesenen Bereichen sind möglich. Die Master-Suche (sieben Begriffe) ergab keine weiteren Verkehrstreffer in den aktuellen kanonischen Quellen; die Suche nach Rechts- und Linksverkehr in allen geladenen Chats und im Repository ergab keinen Treffer. Weitere Stichwortsuchen in den Chats außerhalb der gelesenen Bereiche wurden nicht flächendeckend durchgeführt.
2. Die Zuordnung nummerierter Antworten (Block 5, `23#1025E`) zu den Fragen (`23#1024A`) wurde Punkt für Punkt gegen die Fragenliste geprüft; die Nummerierung beider Nachrichten stimmt überein (50 Fragen, 50 Antworten).
3. Aussagen über Unreal-Systeme (Modul 25) stammen nicht aus den Projektquellen und sind in der Projektversion zu prüfen.
4. Statistiken zählen Mehrfachstatus je Zeile mehrfach (zum Beispiel ‚📋 / 🧭‘).
5. Dieses Protokoll bewertet keine Spielqualität und führt keine Tests aus.

---

## 9. Erzeugte Übersicht

### 9.1 Zeilenzahlen

<!-- BEGIN GENERATED LINE COUNTS -->
| Datei | Zeilen |
|---|---:|
| `01-Quellenlage-und-Methodik.md` | 226 |
| `02-Vision-und-Grundprinzipien.md` | 118 |
| `03-Fahrzeugklassen-und-Fahrzeugdaten.md` | 138 |
| `04-Fahrphysik-und-Fahrverhalten.md` | 145 |
| `05-Steuerung-Kamera-und-Anzeige.md` | 126 |
| `06-Einsteigen-Aussteigen-Sitze-und-Kofferraum.md` | 126 |
| `07-Zustand-Verschleiss-und-Schaden.md` | 131 |
| `08-Reparatur-Wartung-und-Werkstaetten.md` | 111 |
| `09-Kraftstoff-Energie-und-Tankstellen.md` | 96 |
| `10-Strassennetz-und-Strassentypen.md` | 150 |
| `11-Kreuzungen-Signale-und-Schilder.md` | 127 |
| `12-Stadtverkehr-Parken-und-Garagen.md` | 123 |
| `13-Verkehrs-KI-und-Navigation.md` | 146 |
| `14-NPC-Fahrer-und-NPC-Fahrzeuge.md` | 147 |
| `15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md` | 164 |
| `16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md` | 146 |
| `17-Fussgaenger-Radfahrer-und-weitere-Teilnehmer.md` | 108 |
| `18-Besitz-Zulassung-und-Diebstahl.md` | 122 |
| `19-Polizei-und-Verkehrsueberwachung.md` | 129 |
| `20-Interaktionen-Transport-und-Jobs.md` | 113 |
| `21-Fahrzeuge-in-Ermittlungen-und-Jagden.md` | 124 |
| `22-Persistenz-und-Lebenszyklen.md` | 145 |
| `23-Koop-und-Netzwerk.md` | 123 |
| `24-WorldForge-Schnittstelle.md` | 178 |
| `25-Unreal-Integration.md` | 118 |
| `26-Simulationsstufen-LOD-und-Streaming.md` | 146 |
| `27-Performance-und-Skalierung.md` | 132 |
| `28-Entscheidungsregister.md` | 711 |
| `29-Konflikte-und-offene-Fragen.md` | 259 |
| `30-Abdeckungsprotokoll.md` | 244 |
| `31-Testkatalog-Phasen-und-Abnahme.md` | 268 |
| `README.md` | 106 |
| **gesamt (32 Dateien)** | **5246** |
<!-- END GENERATED LINE COUNTS -->

### 9.2 Statistik

<!-- BEGIN GENERATED STATS -->
| Kennzahl | Wert |
|---|---:|
| Register-Einträge | 407 |
| Einträge mit Status 📋 BESCHLOSSEN | 151 |
| Einträge mit Status 🧭 RICHTUNG | 26 |
| Einträge mit Status 🧩 ABGELEITET | 20 |
| Einträge mit Status 💡 VORSCHLAG | 126 |
| Einträge mit Status ❓ OFFEN | 46 |
| Einträge mit Status ⚠️ KONFLIKT | 4 |
| Einträge mit Status 🔍 UNBELEGT | 34 |
| Konflikte CK | 7 |
| Unbelegte oder unklare Angaben CU | 9 |
| Offene Fragen CQ | 118 |
| Testfälle CT | 97 |
| Register-IDs, die ein Testfall referenziert | 104 von 407 |
| Geprüfte Verweise in Nachbarregister | 824 |
<!-- END GENERATED STATS -->
