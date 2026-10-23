# Kreaturen 22: Abdeckungsprotokoll

> [!IMPORTANT]
> **Dieses Modul sagt ehrlich, wie weit die Kreaturen-Dokumentation reicht, woher ihr Inhalt stammt und was fehlt.** Es ist kein Nachweis der Vollständigkeit. Der Zielumfang von **10.000 bis 15.000 substanziellen Zeilen** ist **nicht erreicht**; Abschnitt 4 nennt den Stand. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen und Schlüssel: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Umfang](#1-umfang)
2. [Herkunft des Inhalts: Zählung der Statuszeichen](#2-herkunft-des-inhalts-zählung-der-statuszeichen)
3. [Gelesene Quellen und Lesetiefe](#3-gelesene-quellen-und-lesetiefe)
4. [Stand gegenüber dem Zielumfang](#4-stand-gegenüber-dem-zielumfang)
5. [Abdeckung nach Teilthema](#5-abdeckung-nach-teilthema)
6. [Nicht gelesene oder nicht ausgewertete Quellen](#6-nicht-gelesene-oder-nicht-ausgewertete-quellen)
7. [Prüfungen](#7-prüfungen)
8. [Nächste Etappen](#8-nächste-etappen)

---

## 1. Umfang

Stand der Dateien in `Docs/Game/Kreaturen/` (gemessen mit `wc -l`; Zeilen inklusive Tabellen und Leerzeilen):

| Datei | Zeilen |
|---|---:|
| 01-Quellenlage-und-Methodik.md | 158 |
| 02-Leitsaetze-und-Abgrenzung.md | 149 |
| 03-Klassifikation-Herkunft-und-Individuen.md | 245 |
| 04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md | 309 |
| 05-Geister-Besessenheit-und-Fluecher.md | 266 |
| 06-Daemonen-Engel-und-Leviathane.md | 229 |
| 07-Kosmologie-Seelen-und-Reiche.md | 220 |
| 08-Oekologie-Populationen-und-Lebenszyklus.md | 236 |
| 09-Intelligente-Wesen-und-Monstergesellschaft.md | 221 |
| 10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md | 289 |
| 11-Hunterwelt-Organisationen-und-Kulte.md | 225 |
| 12-Jagd-Rituale-und-verfluchte-Objekte.md | 246 |
| 13-Lebendfang-Fallen-und-Containment.md | 210 |
| 14-Forschung-und-Bestiarium.md | 281 |
| 15-Vorfaelle-und-lebendige-Welt.md | 228 |
| 16-Uebernatuerliche-Orte-und-Geografie.md | 167 |
| 17-Persistenz-Daten-und-Modding.md | 222 |
| 18-Tiere-und-Abgrenzung.md | 119 |
| 19-Artennennungen-und-Artenvorlage.md | 165 |
| 20-Entscheidungsregister.md | 485 |
| 21-Konflikte-und-offene-Fragen.md | 136 |
| 23-Testkatalog-und-Abnahmekriterien.md | 310 |
| README.md | 59 |
| 22-Abdeckungsprotokoll.md (dieses Modul) | 199 |
| **Gesamt** | **5.350** |

Dies ist der **erste Durchgang** für diesen Themenbereich. Vor dem Auftrag gab es keinen Ordner `Docs/Game/Kreaturen/`; Überblickstexte standen in [Spielvision §9](../Spielvision.md#9-kreaturen-und-das-übernatürliche) (etwa 40 Zeilen) und in [Gameplay-Systeme](../Gameplay-Systeme.md).

---

## 2. Herkunft des Inhalts: Zählung der Statuszeichen

Gemessen, nicht geschätzt: Anzahl der Vorkommen der Zeichen je Datei (Tabellenzellen, Fließtext und Überschriften; Zeichen in der Legende zählen mit). Die Zählung zeigt die **Verteilung der Zusagen**, nicht Zeilenanteile.

| Datei | 📋 | 🧭 | 💡 | ❓ |
|---|---:|---:|---:|---:|
| 01-Quellenlage-und-Methodik.md | 3 | 2 | 8 | 1 |
| 02-Leitsaetze-und-Abgrenzung.md | 33 | 1 | 4 | 5 |
| 03-Klassifikation-Herkunft-und-Individuen.md | 20 | 0 | 6 | 6 |
| 04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md | 28 | 0 | 16 | 3 |
| 05-Geister-Besessenheit-und-Fluecher.md | 23 | 0 | 22 | 6 |
| 06-Daemonen-Engel-und-Leviathane.md | 40 | 0 | 42 | 10 |
| 07-Kosmologie-Seelen-und-Reiche.md | 31 | 0 | 23 | 12 |
| 08-Oekologie-Populationen-und-Lebenszyklus.md | 31 | 0 | 14 | 9 |
| 09-Intelligente-Wesen-und-Monstergesellschaft.md | 20 | 0 | 12 | 6 |
| 10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md | 35 | 0 | 16 | 8 |
| 11-Hunterwelt-Organisationen-und-Kulte.md | 25 | 0 | 6 | 7 |
| 12-Jagd-Rituale-und-verfluchte-Objekte.md | 20 | 0 | 7 | 4 |
| 13-Lebendfang-Fallen-und-Containment.md | 15 | 0 | 14 | 8 |
| 14-Forschung-und-Bestiarium.md | 21 | 3 | 15 | 5 |
| 15-Vorfaelle-und-lebendige-Welt.md | 20 | 0 | 8 | 3 |
| 16-Uebernatuerliche-Orte-und-Geografie.md | 13 | 2 | 4 | 6 |
| 17-Persistenz-Daten-und-Modding.md | 14 | 0 | 14 | 12 |
| 18-Tiere-und-Abgrenzung.md | 9 | 1 | 5 | 5 |
| 19-Artennennungen-und-Artenvorlage.md | 4 | 4 | 3 | 5 |
| 20-Entscheidungsregister.md | 209 | 4 | 32 | 32 |
| 21-Konflikte-und-offene-Fragen.md | 0 | 2 | 5 | 9 |
| 23-Testkatalog-und-Abnahmekriterien.md | 1 | 0 | 1 | 0 |
| README.md | 1 | 1 | 2 | 1 |
| **Summe** | **616** | **20** | **279** | **163** |

Die Module 20 bis 23 enthalten naturgemäß viele Zeichen (Register, Tests). Eine ehrliche Lesart: Die **Quellenlage trägt die Richtung**, die **Mechanik ist überwiegend Entwurf** (💡 und ❓). Die Entwürfe sind in den Modulen als „Entwurf“ gekennzeichnet; sie sind keine Entscheidungen und keine Quellenbefunde. Eine Aussage der Spalte 'Status' ist eine **Lesart** der Quelle.

---

## 3. Gelesene Quellen und Lesetiefe

| Quelle | Umfang | Lesetiefe |
|---|---|---|
| `M-PD` (Entscheidungsstand 31.08.2026) | Abschnitte Story and cases, Hunter Archive, NPCs and AI, World state, Human anatomy/injury, Death/failure, Gunplay, Multiple-opponent combat, Stealth/perception/horror AI, Modding, Genre, Creature framework (expanded), Organic world-event, Peaceful beings, Hunter Archive epistemology, Long-term story threats, Acts and chapters, Wildlife/monsters | vollständig für die genannten Abschnitte |
| `M-OD`, `M-B24`, `M-B37` | Abschnitt Wildlife/monster knowledge; Planungsinventare Kreaturen und Wildtiere | vollständig |
| `M-CA0905`, `M-CA0906`, `M-CU0908`, `M-B09`, `M-RS`, `M-PS0905` | Kanonische Zusätze, Block-9-Status, Reiche | vollständig für die genannten Abschnitte |
| Chat 23 | `253A`/`254E`, `293A`–`298E`, `307A`/`308E`, `359A`–`362E`, `365A`–`372A`, `375E`–`379E`, `440E`–`446E` (nur Randbezug), `458A`–`460A`, `465A`–`471A`, `1074A`–`1076A`, `1091E`, `1103E`/`1104A`, `1026A`/`1027E` (Natur) | vollständig in den genannten Bereichen |
| Chat 35 | `26E`/`27A` (Heilung), `41E` (Hölle, Zeitreise), `71E`, `137E` (Mods) | vollständig in den genannten Bereichen |
| Chat 48 | `122A`–`158A` (Quiz zu Verletzung, Monsterwissen, Forschung, Gefangennahme, Fallen, KI, Gedächtnis, Gesellschaft, Territorien, Ruf), `159E`, `169E`–`171E`, `179E`, `209E` | vollständig für die genannten Bereiche; Optionstexte aus den Nachrichten selbst |
| Chat 19, 30, 50 | `19#4E`; `30#245E` (Abschnitt 38); `50#13E` | je ein Ausschnitt |
| Repository | `Core/include/spn/core/WorldFacts.h`; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) vollständig; Überblicksdokumente (Zeilen zu Kreaturen, Fraktionen, Nemesis, Reiche) | gelesen |
| Suche | Stichwortsuche über alle 56 Chats (Monster, Kreatur, Geist, Dämon, Vampir, Werwolf, Wendigo, Formwandler, Bestiarium, Ritual, Fluch, Leviathan, Engel, Alpha); Ergebnis: Treffer außerhalb der gelesenen Bereiche betreffen Romankapitel, Kartenarbeit, Dateiarbeit oder das Minecraft-Projekt | durchgeführt, nicht im Volltext gelesen |

---

## 4. Stand gegenüber dem Zielumfang

Das Ziel lautet 10.000 bis 15.000 **substanzielle** Zeilen. Der Stand beträgt **5.350 Zeilen**, also etwa **54 %** der unteren Grenze von 10.000.

**Was die Lücke bedeutet.** Die Quellen für diesen Themenbereich sind im gelesenen Umfang ausgeschöpft; sie enthalten **keine einzige konkrete Art**. Zusätzliche Zeilen entstehen künftig aus drei Quellen:

1. **Neue Entwicklerentscheidungen** (Antworten auf die offenen Fragen in [Modul 21](21-Konflikte-und-offene-Fragen.md)), vor allem zur Artenliste (Q-K01), zum Speichermodell (Q-K25), zu Dämonen/Engeln (Q-K07) und zu Seelen (Q-K08).
2. **Weitere Quellenarbeit**: Chat 23 außerhalb der gelesenen Bereiche (Waffen- und Munitionsblöcke mit Bezug zu Schwächen), Prompt-Entwürfe `docs/prompts/*` (nur Titel), die Roman-Kapitel als Beispielquelle (keine Spielentscheidung).
3. **Weitere Entwürfe** (Artenblätter, Fallbeispiele). Mehr Entwurf ohne neue Anforderungen erhöht den Anteil an Vorschlag und die Gefahr der Wiederholung und ist **ausdrücklich nicht das Ziel**.

Empfehlung: Vor einer zweiten Etappe die Reihenfolge der Klärung in Modul 21 §6 beantworten. Jede entschiedene Art erzeugt dann ein Artenblatt nach [Modul 19](19-Artennennungen-und-Artenvorlage.md) §4.

---

## 5. Abdeckung nach Teilthema

Die Einschätzung bezieht sich auf die **Quellenlage**, nicht auf die Zahl der Zeilen. ‚Quellen ausgeschöpft‘ heißt: keine weitere Aussage in den gelesenen Quellen bekannt, die nicht dokumentiert wäre.

| Teilthema (Auftrag) | Module | Quellenlage | Stand |
|---|---|---|---|
| Spezies / Arten | 03, 19 | keine Artenliste; nur Nennungen und Rahmen | Rahmen und Vorlage; keine Art dokumentiert |
| Klassifikation, Individuen, Ausnahmen | 03 | gut (Aug. und Sep. 2026) | Quellen ausgeschöpft |
| Übernatürliche Fähigkeiten | 04, 05, 06 | Prinzipien (getrennt von physischem Schaden), keine Fähigkeitenliste | nur Rahmen |
| Schwächen, Widerstände, Regeneration | 04 | gut als Prinzip, keine Zuordnung | Quellen ausgeschöpft; Zuordnung offen |
| Verhalten, Monster-KI, Taktik, Lernen | 10 | gut (Stealth-Block, Quiz Oktober) | Quellen ausgeschöpft; Artregeln offen |
| Persistente Erinnerungen, Rivalitäten, Nemesis | 10, 17 | Quiz Oktober und Entwickleraussage 22.10.2026 (Proposal 0010) | Richtung bestätigt; Mechanik offen |
| Fraktionen, Monstergesellschaft | 09, 11 | Quiz Oktober, Block 9 | Quellen ausgeschöpft |
| Ökologie, Territorien, Lebenszyklus | 08 | gut (Block 9D, Quiz) | Quellen ausgeschöpft; Zahlen offen |
| Geister, Besessenheit, Flüche | 05 | gut (Aug. 2026), Serienlogik als Standard (Inhalt vom Assistenten) | Quellen ausgeschöpft; Inhalte 💡 |
| Dämonen, Engel, Leviathane | 06 | dünn für Dämonen/Engel, mittel für Leviathane | Dämonenblock fehlt (Quellenlücke) |
| Kosmologie, Seelen, Reiche | 07 | mittel; Reiche in Welt-Dokumentation | Quellen ausgeschöpft; Kriterien offen |
| Jagd, Rituale, verfluchte Objekte | 12 | gut (Aug./Sep. 2026) | Quellen ausgeschöpft; Inhalte offen |
| Lebendfang, Fallen, Containment | 13 | Quiz Oktober | Quellen ausgeschöpft; Einrichtungen offen |
| Forschung, Bestiarium | 14 | gut (Aug., Sep., Okt.) | Quellen ausgeschöpft |
| Welt-Auswirkungen, Vorfälle | 15 | gut (Block 9G–H) | Quellen ausgeschöpft |
| Orte, Haunted Properties | 16 | mittel (Block 9N/P) | Quellen ausgeschöpft |
| Persistenz, Modding | 17 | Master, Proposal 0010 | Linien beschrieben; Entscheidung offen |
| Tiere | 18 | Master, Block 6 | Abgrenzung; Details in Spielwelt-Dokumentation |

---

## 6. Nicht gelesene oder nicht ausgewertete Quellen

| Quelle | Möglicher Beitrag | Hinweis |
|---|---|---|
| Chat 23, Nachrichten außerhalb der gelesenen Bereiche | Waffen-/Munitionsblöcke (Schwächen und Spezialmunition), Stealth-Blöcke 225–252, Begleiter | nur stichwortweise; Entwicklerseite der Stealth-Blöcke durchgesehen (`23#226E`, `23#240E`) |
| Chat 35, außerhalb der gelesenen Bereiche | Adult-/Körper-/Karten-/Dateiarbeit | nur Stichwortfilter |
| Chat 48, Nachrichten 160–200 und 200–297 (Rest) | Companion-Plattform, Tesla, Dateiarbeit | Treffer nur zu Tesla/HQ (`48#209E`) |
| Chats 11, 12, 13, 15, 42, 50 (Roman und Schreibprojekt) | Fall- und Monsterbeispiele einer Romanfassung | **nicht gelesen**; Romanmaterial ist keine Spielentscheidung |
| Chat 18 (Minecraft/Voxel) | – | ausdrücklich getrennt vom Spiel (`19#1A`) |
| Chat 38 (Serienfolgenliste) | Serien-Handlung | **bewusst nicht gelesen**: Spoilerregel (`23#1091E`) |
| Master Tier 3 (Prompt-Bibliothek `docs/prompts/*`) | Arbeitspakete zu Monstern (Titel) | Schablonentext, `DRAFT_REWRITE_REQUIRED` |
| Master Tier 4 (Historie, Rekonstruktionsquellen) | Entwicklungsverlauf | nur über Chats |
| Entscheidungen ab 20.10.2026 außerhalb von `PE-E1022` und `PE-N1022` | – | sind nicht im Master und nur dann berücksichtigt, wenn in den Überblicksdokumenten oder Proposals festgehalten |

---

## 7. Prüfungen

| Prüfung | Ergebnis (zum Zeitpunkt der Erstellung) |
|---|---|
| `check_docs.py` (Chat-Zitate) | 1.777 Zitate, 0 Probleme (Nachricht existiert, Rolle E/A stimmt) |
| `check_docs.py` (wörtliche Zitate in „…“) | 381 Zitate, 0 nicht gefunden; 17 nicht wörtliche Wendungen wurden in ‚…‘ umgesetzt |
| `check_docs.py` (Quellenschlüssel) | 12 definiert, alle verwendeten definiert (Modul 01) |
| `check_docs.py` (Register-Kennungen `K-…`) | 273 Einträge, keine Duplikate, keine fehlenden |
| Zusatzprüfung Zitat in zitierter Nachricht (`qcheck.py`) | 0 Abweichungen: jedes wörtliche Zitat auf einer Zeile mit Chat-Zitat steht in einer der dort zitierten Nachrichten |
| `check_links.py` (Links und Anker) | alle Links der Kreaturen gültig; zwei fremde Altlasten in `Adult-System.md` und `Adult/10-…` (Anker in der Haupt-README) |
| `git diff --check` | ohne Befund |

Grenzen der Prüfung: Sie belegt, dass **zitierte** Nachrichten existieren und die Rolle (E/A) stimmt, dass **wörtliche Zitate** irgendwo in den Quellen vorkommen und dass in einer Zusatzprüfung Zitate auf einer Zeile in einer der dort zitierten Nachrichten stehen; sie belegt **nicht**, dass jede Deutung richtig ist. Jede Aussage der Spalte ‚Status‘ ist eine **Lesart** der Quelle. Wo eine Antwort mehrdeutig war, steht es in [Modul 21](21-Konflikte-und-offene-Fragen.md) (unklare Antworten).

---

## 8. Nächste Etappen

Entscheidungen, die Entwurf in Anforderung verwandeln würden (Auswahl aus [Modul 21](21-Konflikte-und-offene-Fragen.md) §6), geordnet nach Wirkung:

1. **Artenliste und Reihenfolge** (Q-K01).
2. **Speichermodell** für Gedächtnis, Beziehungen, Verdichtung (Q-K25).
3. **Individuum gegen Population** (Q-K02) und **Lerneffekt** (Q-K15, Q-K33).
4. **Dämonen- und Engelblock** (Q-K07), **Ziel der Seele** (Q-K08), **Körper und Seele** (Q-K09).
5. **Einrichtungen und Recht des Containments** (Q-K19, Q-K20).
6. **Bestiarium-Schwellen** (Q-K21, Q-K04).

Quellenarbeit, die weitere beschlossene Inhalte liefern könnte:

1. Waffen-/Munitionsblöcke und Stealth-Blöcke in Chat 23 auf Aussagen zu Schwächen und Kreaturenverhalten (Volltext).
2. Chat 48 Nachrichten 160–297 auf Kreaturenbezug.
3. Die Romankapitel nur als **Beispielquelle** für Spuren, Fälle und Verhalten, ohne daraus Entscheidungen abzuleiten.

Themenbereiche, die dieser Ordner nicht behandelt und die eigene Dokumentation bräuchten: Verletzung und Medizin im Detail, Kampf, Companion/NPC-Beziehungen, Hunter-HQ und Bunker, Reichsbibeln (Hölle, Fegefeuer, Himmel), Story und Hauptbedrohungen, Charaktererstellung. Ihr Überblicksstand steht in [Entscheidungsstatus](../Entscheidungsstatus.md).
