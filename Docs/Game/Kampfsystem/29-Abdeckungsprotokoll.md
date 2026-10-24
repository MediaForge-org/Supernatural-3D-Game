# Kampfsystem 29: Abdeckungsprotokoll

> [!IMPORTANT]
> **Dieses Protokoll sagt, was berücksichtigt wurde und was nicht.** Es ist kein Nachweis der Vollständigkeit des Spiels, sondern der Auswertung. Zeilenzahlen und Statistik sind erzeugt (`Tools/world-audit/render_combat_register.py`). Ein Zeilenziel nannte der Auftrag nicht; die Qualität der Quellenbindung hat Vorrang vor dem Umfang.

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Ausgangslage](#1-ausgangslage)
2. [Abdeckung der vorgesehenen Themen](#2-abdeckung-der-vorgesehenen-themen)
3. [Quellenabdeckung](#3-quellenabdeckung)
4. [Abdeckung der Wechselwirkungen, Szenarien und Tests](#4-abdeckung-der-wechselwirkungen-szenarien-und-tests)
5. [Befunde, die Bestehendes betreffen](#5-befunde-die-bestehendes-betreffen)
6. [Prüfungen und Ergebnisse](#6-prüfungen-und-ergebnisse)
7. [Änderungen außerhalb des Ordners](#7-änderungen-außerhalb-des-ordners)
8. [Grenzen](#8-grenzen)
9. [Erzeugte Übersicht](#9-erzeugte-übersicht)

---

## 1. Ausgangslage

- Git-Stand bei Beginn: Branch `main`, Commit `9e8598f` (‚docs: expand NPC memory, relationships and living world‘), Arbeitsbaum sauber. Das entspricht dem erwarteten Stand; es gab keine Abweichung.
- Bestandsaufnahme (Phase A) vor dem Schreiben: Repository-Dokumente zu Kampf (Gameplay-Systeme §4, §5, §7, §8, §10, §16; Spielvision §9, §13, §14; Entscheidungsstatus §5; Proposal 0008, 0009, 0010; Register der Ordner Kreaturen, NPCs, Ermittlung), `M-PD` und `M-OD` im Master, Prompt-Titel der Tracks 06, 07, 08, 10, 14 und die Chats 23, 35, 48 (Treffer zu Kampf, DualSense, Heilfähigkeiten).
- Ergebnis der Bestandsaufnahme (Frage 1 bis 5 des Auftrags): [Modul 01](01-Quellenlage-und-Methodik.md) §1 und §6 (eindeutig getroffene Entscheidungen), [Modul 02](02-Kampfvision-und-Grundprinzipien.md) (mehrfach erwähnte Anforderungen), [Modul 28](28-Konflikte-und-offene-Fragen.md) (Widersprüche, unzureichend dokumentierte Bereiche, offene Fragen).
- Bewusst nicht getan: Quellcode, Unreal-Dateien, Build-Artefakte, M1-Release-Daten und der Master wurden nicht verändert; es gab keine Git-Schreiboperation.

---

## 2. Abdeckung der vorgesehenen Themen

| Vorgesehene Datei des Auftrags | Datei | Stand |
|---|---|---|
| README | `README.md` | vorhanden |
| 01 Quellenlage und Methodik | `01-…` | vorhanden |
| 02 Kampfvision | `02-…` | vorhanden |
| 03 Zustände und Architektur | `03-…` | vorhanden (Entwurf) |
| 04 Nahkampf | `04-…` | vorhanden |
| 05 Fernkampf | `05-…` | vorhanden |
| 06 Steuerung | `06-…` | vorhanden (DualSense-Vorgaben 🔍) |
| 07 Defensive | `07-…` | vorhanden |
| 08 Traversal-Integration | `08-…` | vorhanden |
| 09 Waffenklassen | `09-…` | vorhanden |
| 10 Munition | `10-…` | vorhanden |
| 11 Ausdauer | `11-…` | vorhanden |
| 12 Treffermodell | `12-…` | vorhanden |
| 13 Verletzungen | `13-…` | vorhanden |
| 14 Bewusstlosigkeit, Tod | `14-…` | vorhanden (Konflikt `CK-01`) |
| 15 Kreaturenkampf | `15-…` | vorhanden |
| 16 Schwächen | `16-…` | vorhanden |
| 17 Fallen | `17-…` | vorhanden |
| 18 Gegner-KI | `18-…` | vorhanden |
| 19 Nemesis | `19-…` | vorhanden |
| 20 Fraktionen, Polizei | `20-…` | vorhanden |
| 21 Vermeidung, Flucht | `21-…` | vorhanden |
| 22 Animationen | `22-…` | vorhanden |
| 23 Koop | `23-…` | vorhanden |
| 24 Speicherstände | `24-…` | vorhanden |
| 25 Performance | `25-…` | vorhanden |
| 26 Szenarien | `26-…` | zwölf Szenarien |
| 27 Entscheidungsregister | `27-…` | erzeugt |
| 28 Konflikte, offene Fragen | `28-…` | 7 Konflikte, 4 unbelegte Angaben, 109 Fragen |
| 29 Abdeckungsprotokoll | diese Datei | – |
| 30 Testkatalog | `30-…` | Tests nach zwölf Kategorien plus Szenarien |

Der Auftrag erlaubt eine sinnvolle Erweiterung der Struktur; sie war **nicht nötig**. Alle 31 Dateien entsprechen der vorgesehenen Liste einschließlich README.

---

## 3. Quellenabdeckung

| Quelle | Umfang der Auswertung | Verwendet in |
|---|---|---|
| `M-PD` (Master, 31.08.2026) | die Kampfabschnitte vollständig | alle Module |
| `M-OD` | Abschnitte zu Kampf, Tod, Körper, Recht vollständig | Offene Fragen |
| Master Tier 1 und 2 | per Suche (Kampf, Waffen, Ballistik, Quadrat, DualSense); Randtreffer gelesen | Module 01, 13 |
| Prompt-Bibliothek | Dateititel von 415 Entwurfs-Prompts (Tracks 06, 07, 08, 10, 14) und `TRACK_CONTEXT` von 06, 07, 08 | Themeninventar (nie Anforderung) |
| Chat 23 | `60E`–`69A`, `85A`–`113A`, `115A`–`160A`, `162A`–`174A`, `221A`–`226E`, `239A`–`243A`, `253A`–`256E`, `360E`–`372A` (Tod und Speichern); Stichproben `589A`, `515A` | Kern der Entscheidungen |
| Chat 35 | `790A`–`799E` (Kampf-Lernsystem, Control-Fragen), Treffer zu DualSense und Heilfähigkeiten | Module 04, 06 |
| Chat 48 | über die Register der Kreaturen- und NPC-Dokumentation | Module 15 bis 19 |
| Repository | Gameplay-Systeme, Spielvision, Entscheidungsstatus, Proposals 0007 bis 0010, Decision 0004, README §26 | alle Module |
| Nachbarregister | Kreaturen (`K-…`), NPCs (`N-…`), Ermittlung (`E-…`), World (`W-RE-…`) | Verweise, keine Neubewertung |
| Auftrag (`AUF`) | DualSense-Vorgaben, Szenarien, Kategorien | Modul 06, 26, 30 |

**Nicht gelesen oder nur durchsucht:** Master Tier 3 und 4 außerhalb von `M-PD`, `M-OD`, Backlog- und Prompt-Titeln; Chat 23 außerhalb der genannten Bereiche; Chats 27 und 29 (GTA/RDR2-Berichte) als Mechanikquelle; Romankapitel; Unreal-Code. Eine vollständige Prüfung des 2,9-GB-Archivs wird nicht behauptet.

---

## 4. Abdeckung der Wechselwirkungen, Szenarien und Tests

| Anforderung des Auftrags | Erfüllt durch |
|---|---|
| Sieben Wechselwirkungen (Bewegung, Medizin, NPCs, Kreaturen, Ermittlungen, Persistenz, Koop) | [Modul 02](02-Kampfvision-und-Grundprinzipien.md) §6, jeweilige Fachmodule |
| Zwölf Szenarien mit Trennung beschlossen und hypothetisch | [Modul 26](26-Querschnittsszenarien.md) |
| Technische Spezifikation (Zustände, Ereignisse, Daten, Schnittstellen, Persistenz, Netzwerk, Fehlerfälle, Testbarkeit, Performance) | Module 03, 12, 22, 23, 24, 25 und je Modul ‚Schnittstellen und Fehlerfälle‘ |
| Entscheidungsregister mit ID, Titel, Beschreibung, Status, Quelle, Systemen, Konsequenzen, offenen Punkten, Alternativen | [Modul 27](27-Entscheidungsregister.md) (Alternativen in den Gegenüberstellungen der Fachmodule und in Modul 28) |
| Offene Fragen mit Begründung, Anforderungen, Optionen, Vor- und Nachteilen, Vorbehalt | [Modul 28](28-Konflikte-und-offene-Fragen.md) §3 |
| Testkatalog mit zwölf Kategorien, je Test ID, Voraussetzungen, Ablauf, Ergebnis, Systeme, Bezug, Status | [Modul 30](30-Testkatalog-und-Abnahmekriterien.md) |
| Unbeschlossenes nicht als Abnahmekriterium | Teststatus `bedingt` oder `Vorschlag` |

---

## 5. Befunde, die Bestehendes betreffen

Die folgenden Befunde ändern **nichts** an bestehenden Dokumenten; sie sind Hinweise für die Projektleitung.

| Nr. | Befund | Betroffene Dokumente |
|---|---|---|
| B1 | Der Chat 23 (`364E`, `366E`) enthält eine spätere Entwicklerantwort zum Gameplay-Tod, die `M-PD` ‚Death / failure — OPEN‘, Spielvision §14.2 und die NPC-Dokumentation nicht erwähnen, die Kreaturen-Dokumentation aber als 📋 führt (`CK-01`) | Spielvision §14.2, NPCs 14 und 27, `M-PD` |
| B2 | Das Kampf-Lernsystem (Chat 35, `791E`–`798A`) fehlt in `M-PD`; Gameplay-Systeme §5.3 führt es nur als 🧭 ‚gewünscht, gehört zu einem späteren Kampf-Block‘ | Gameplay-Systeme §5.3 |
| B3 | Die DualSense-Kampfbelegung des Auftrags ist in keiner Quelle belegt (`CU-01`) | Auftrag, Gameplay-Systeme §3.2 (Controller-Belegung ❓) |
| B4 | Gameplay-Systeme §4 nennt den Verlust von Gliedmaßen als plausibles Gore-Ergebnis, das Addendum vom 05.09.2026 lässt Amputation nur über erzählte Story zu (`CK-04`) | Gameplay-Systeme §4, `M-CA0905` |
| B5 | `E-ZE-007` (‚Wichtige NPCs sterben nur durch den Spieler‘) und die Aussage ‚kein Plot-Armor‘ stehen in Spannung (`CK-02`) | Ermittlung, NPCs |
| B6 | Zwischen ‚keine Tastentabelle‘ und ‚Aktionsbibliothek mit Tastenkombination‘ besteht eine Spannung (`CK-03`) | `M-PD`, Chat 35 |
| B7 | Bereits vorhandene Fehler: Die Links `README.md#21-…` in `Adult-System.md` und `README.md#27-…` in `Adult/10-…` zeigen auf Anker, die es nicht gibt (nicht durch diesen Auftrag verursacht, nicht verändert) | Adult-Dokumentation |

---

## 6. Prüfungen und Ergebnisse

Alle Prüfungen wurden lokal auf Fedora ausgeführt. Stand der Ergebnisse nach der letzten Änderung des Ordners.

| Prüfung | Werkzeug | Ergebnis |
|---|---|---|
| Chat-Zitate (Schlüssel `NN#m` mit Rolle E oder A): Nachricht existiert, Rolle passt zum Suffix | `check_docs.py` | 1.412 Zitate, 0 Probleme |
| Wörtliche Zitate in „…“ stehen in Chats oder Master | `check_docs.py` | 10 Zitate, 0 nicht gefunden |
| Quellenschlüssel (`M-…`, `PE-…`) sind in Modul 01 definiert | `check_docs.py` | 19 Schlüssel definiert und benutzt, 0 undefinierte |
| Register-IDs eindeutig, jede Referenz aufgelöst | `check_docs.py`, `render_combat_register.py` | 562 Einträge, 0 Duplikate, 0 fehlende |
| Zellenzahl, Gruppe, Statuswort, Modulzuordnung | `render_combat_register.py` | 0 Probleme |
| Verweise `CQ`, `CK`, `CU`, `CS` aufgelöst | `render_combat_register.py` | 0 undefinierte; alle 109 Fragen werden von Modulen referenziert |
| Test-IDs eindeutig | `render_combat_register.py` | 175 Tests, 0 Duplikate |
| Relative Links und Anker | `check_links.py` | 5.408 Links, 2 kaputt (beide bereits vorhanden, Adult-Dokumentation, nicht verändert); 0 kaputt im neuen Ordner |
| Leerzeichen am Zeilenende, CRLF | `grep` | keine |
| `git diff --check` | Git (lesend) | sauber |

Während der Arbeit wurden folgende Probleme **durch diesen Auftrag verursacht und behoben**: sieben wörtliche Zitate, die nicht verbatim waren (in einfache Anführungszeichen umgestellt), drei fehlende Quellenschlüssel in Modul 01, zwei Anker im Inhaltsverzeichnis von Modul 26, ein fehlendes Statuswort in einer Registerzeile, mehrere Platzhalter und Formatierungsreste.

---

## 7. Änderungen außerhalb des Ordners

| Datei | Änderung | Grund |
|---|---|---|
| `Docs/MasterIndex.md` | neuer Abschnitt ‚Combat, injury, weapons and difficulty‘ (Verweise auf `PD`, `OD`, Prompt-Tracks, Kampfsystem-Dokumentation) | Auffindbarkeit (Auftrag Phase G Punkt 9) |
| `Docs/Game/Gameplay-Systeme.md` | ein Verweis unter der Überschrift §5 | Querverweis |
| `README.md` | eine Zeile in der Tabelle der Dokumentationen | Auffindbarkeit |
| `Tools/world-audit/README.md` | Abschnitt ‚Kampfsystem-Dokumentation‘ | Werkzeugbeschreibung |
| `Tools/world-audit/render_combat_register.py` | neu | Register- und Statistikgenerator |

Es wurden **keine** Dateien gelöscht und keine Dateien im Master, in `Unreal/`, in Build-Verzeichnissen oder in `Docs/Setup/` verändert. Es wurde nicht committet, gepusht, getaggt oder released.

---

## 8. Grenzen

1. Die Auswertung der Chats ist auf die in Abschnitt 3 genannten Bereiche begrenzt. Weitere Kampfaussagen in nicht gelesenen Bereichen sind möglich; die Suche nach Schlüsselwörtern (Kampf, Nahkampf, Waffe, DualSense, Quadrat, Ragdoll und weitere) ergab außerhalb der gelesenen Bereiche keine Entwicklerentscheidung.
2. Die Zuordnung nummerierter Antworten zu Fragen (`P<n>`) ist stichprobenartig gegen die Fragen geprüft; bei Abweichungen steht `CU-04`.
3. Statistiken zählen Mehrfachstatus je Zeile mehrfach (zum Beispiel ‚📋 BESCHLOSSEN / 🧭 RICHTUNG‘).
4. Dieses Protokoll bewertet keine Spielqualität und führt keine Tests aus.

---

## 9. Erzeugte Übersicht

### 9.1 Zeilenzahlen

<!-- BEGIN GENERATED LINE COUNTS -->
| Datei | Zeilen |
|---|---:|
| `01-Quellenlage-und-Methodik.md` | 213 |
| `02-Kampfvision-und-Grundprinzipien.md` | 184 |
| `03-Kampfzustaende-und-Systemarchitektur.md` | 214 |
| `04-Nahkampf-und-Kampfstile.md` | 249 |
| `05-Fernkampf-und-Schusswaffen.md` | 231 |
| `06-Steuerung-und-Eingabekontext.md` | 223 |
| `07-Ausweichen-Blocken-Parieren-und-Konter.md` | 168 |
| `08-Kampfbewegung-und-Traversal-Integration.md` | 163 |
| `09-Waffenklassen-und-Ausruestung.md` | 194 |
| `10-Munition-Nachladen-und-Waffenzustand.md` | 209 |
| `11-Ausdauer-Erschoepfung-und-Belastung.md` | 131 |
| `12-Treffermodell-und-Koerperphysik.md` | 231 |
| `13-Verletzungen-Schmerz-und-Kampffolgen.md` | 186 |
| `14-Bewusstlosigkeit-Tod-und-Rettung.md` | 212 |
| `15-Kreaturenkampf-und-Spezialfaehigkeiten.md` | 186 |
| `16-Schwaechen-Vorbereitung-und-Jagdmethoden.md` | 136 |
| `17-Fallen-Fesselung-und-Lebendfang.md` | 132 |
| `18-Gegner-KI-und-Kampftaktik.md` | 189 |
| `19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md` | 148 |
| `20-Fraktionen-Polizei-und-Kampfkonsequenzen.md` | 163 |
| `21-Kampfvermeidung-Flucht-und-Deeskalation.md` | 134 |
| `22-Animationen-Trefferreaktionen-und-Ragdoll.md` | 156 |
| `23-Koop-Synchronisierung-und-Autoritaet.md` | 148 |
| `24-Speicherstaende-und-Weltpersistenz.md` | 158 |
| `25-Performance-Skalierung-und-Simulationsstufen.md` | 148 |
| `26-Querschnittsszenarien.md` | 249 |
| `27-Entscheidungsregister.md` | 844 |
| `28-Konflikte-und-offene-Fragen.md` | 238 |
| `29-Abdeckungsprotokoll.md` | 224 |
| `30-Testkatalog-und-Abnahmekriterien.md` | 333 |
| `README.md` | 89 |
| **gesamt (31 Dateien)** | **6483** |
<!-- END GENERATED LINE COUNTS -->

### 9.2 Statistik

<!-- BEGIN GENERATED STATS -->
| Kennzahl | Wert |
|---|---:|
| Register-Einträge (Anforderungen und Aussagen) | 562 |
| Einträge mit Status 📋 BESCHLOSSEN | 367 |
| Einträge mit Status 🧭 RICHTUNG | 58 |
| Einträge mit Status 🧩 ABGELEITET | 9 |
| Einträge mit Status 💡 VORSCHLAG | 44 |
| Einträge mit Status ❓ OFFEN | 79 |
| Einträge mit Status ⚠️ KONFLIKT | 3 |
| Einträge mit Status 🔍 UNBELEGT | 4 |
| Konflikte CK | 7 |
| Unbelegte oder unklare Angaben CU | 4 |
| Offene Fragen CQ | 109 |
| Szenarien CS | 12 |
| Testfälle CT | 175 |
| Register-IDs, die ein Testfall referenziert | 188 von 562 |
<!-- END GENERATED STATS -->
