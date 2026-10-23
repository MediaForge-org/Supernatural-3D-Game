# Ermittlung 22: Abdeckungsprotokoll

> [!IMPORTANT]
> **Dieses Modul sagt ehrlich, wie weit die Ermittlungs-Dokumentation reicht, woher ihr Inhalt stammt und was fehlt.** Es ist kein Nachweis der Vollständigkeit. Der Zielumfang von **10.000 bis 15.000 substanziellen Zeilen** für diesen Themenbereich ist **nicht erreicht**; Abschnitt 4 nennt den Stand und die Gründe. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Quellen und Schlüssel: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Umfang](#1-umfang)
2. [Herkunft des Inhalts: Quelle gegen Entwurf](#2-herkunft-des-inhalts-quelle-gegen-entwurf)
3. [Gelesene Quellen und Lesetiefe](#3-gelesene-quellen-und-lesetiefe)
4. [Stand gegenüber dem Zielumfang](#4-stand-gegenüber-dem-zielumfang)
5. [Abdeckung nach Teilthema](#5-abdeckung-nach-teilthema)
6. [Nicht gelesene oder nicht ausgewertete Quellen](#6-nicht-gelesene-oder-nicht-ausgewertete-quellen)
7. [Prüfungen](#7-prüfungen)
8. [Nächste Etappen](#8-nächste-etappen)

---

## 1. Umfang

Stand der Dateien in `Docs/Game/Ermittlung/` (Zeilen inklusive Tabellen und Leerzeilen):

| Gruppe | Module | Zeilen |
|---|---|---:|
| Grundmodule 01 bis 07 (inklusive Nachträge der Etappe 4) | 01 bis 07 | 1230 |
| Vertiefung Etappe 2 (inklusive Nachträge) | 08 bis 19 | 4457 |
| Vertiefung Etappe 3 | 24 bis 27 | 1104 |
| Register, Konflikte, Protokoll, Tests | 20 bis 23 | 1201 |
| Index | README | 62 |
| **Gesamt** | alle | **8054** |

Der Ordner enthielt vor dem Auftrag zur Vertiefung **1.401** Zeilen. Zugewachsen sind damit 6653 Zeilen; in Etappe 4 (7.511 auf 8054 Zeilen) kamen 543 Zeilen hinzu, überwiegend Quellenbefunde. Die Zahl wächst mit jeder Etappe; sie ist ein Messwert, kein Nachweis von Qualität.

Zum Vergleich der Gesamtdokumentation: `Docs/Game/` hatte vor der Arbeit 16.432 Zeilen; davon der Ermittlungsbereich als Überblick in etwa 100 Zeilen verteilt auf [Spielvision](../Spielvision.md) und [Gameplay-Systeme](../Gameplay-Systeme.md).

---

## 2. Herkunft des Inhalts: Quelle gegen Entwurf

Ein Maß dafür, wie viel der Zeilen auf **Quellen** beruht und wie viel **Vorschlag** ist. Gezählt wird nach Modulgruppe (grobe Schätzung, keine exakte Zählung):

| Modulgruppe | Zeilen | Überwiegend Quelle (📋) | Überwiegend Entwurf (💡) |
|---|---:|:---:|:---:|
| 01 bis 07 | 1.123 | ja | – |
| 08 | 263 | etwa 45 % | etwa 55 % |
| 09 | 412 | etwa 25 % | etwa 75 % |
| 10 | 393 | etwa 25 % | etwa 75 % |
| 11 | 310 | etwa 25 % | etwa 75 % |
| 12 | 391 | etwa 20 % | etwa 80 % |
| 13 | 364 | etwa 30 % | etwa 70 % |
| 14 | 468 | etwa 25 % | etwa 75 % |
| 15 | 307 | etwa 15 % | etwa 85 % |
| 16 | 399 | etwa 45 % | etwa 55 % |
| 17 | 291 | etwa 40 % | etwa 60 % |
| 18 | 278 | etwa 40 % | etwa 60 % |
| 19 | 332 | etwa 20 % (Fakten aus dem Code) | etwa 80 % |
| 24 bis 27 | 1.059 | etwa 25 % | etwa 75 % |

Das ist eine **ehrliche Auskunft**: Der größte Teil der Etappen 2 und 3 ist **Entwurf**. Entwürfe sind in den Modulen als solche gekennzeichnet („Entwurf“ im Abschnittstitel, 💡 im Status, Hinweisfeld am Abschnittsanfang). Sie sind **Anwendungen der beschlossenen Anforderungen** auf Fragen, die die Quellen offenlassen (Zustandsmaschinen, Tabellen von Fällen, Abhängigkeiten, Sonderfälle), keine Entscheidungen und keine Quellenbefunde.

Warum so viel Entwurf? Weil die Quellen für diesen Themenbereich **knapp** sind. Der Master kennt die Richtung, nicht die Mechanik (`M-OD` führt die Mechanik als offen). Wer Tiefe verlangt, bekommt sie derzeit nur, indem die offenen Fragen **sichtbar gemacht und strukturiert** werden. Der Wert liegt in der Struktur, den Randfällen, den Abhängigkeiten und den Prüfregeln; er ersetzt keine Entscheidung.

---

## 3. Gelesene Quellen und Lesetiefe

| Quelle | Umfang | Lesetiefe |
|---|---|---|
| `M-PD` (Entscheidungsstand 31.08.2026) | Abschnitte Story/Cases, Investigation modes, Hunter Archive, Cash/law/evidence, Organic emergence, Archive epistemology, Smartphone/social, Wildlife/monsters, Melee/Multiple opponents, Stealth | vollständig für die genannten Abschnitte |
| `M-OD` (Offene Entscheidungen) | Abschnitte Crime/law/evidence, Smartphone, Product/story | vollständig für die genannten Abschnitte |
| `M-CA0905`, `M-PS0905`, `M-B09`, `M-MS1013` (Tier 1) | ganze Dateien | vollständig |
| Planungsinventare `M-B25`, `M-B37`, `M-B39`, `M-B40` | ganze Dateien | vollständig (kurz, nur Themenlisten) |
| Prompt-Entwürfe `M-P03` (53 Dateien) | Titel und `TRACK_CONTEXT.md` | Titel und Kontext gelesen; Schablonentext **nicht** als Inhalt gewertet |
| Chat 23 (Etappe 1–3) | Nachrichten 45–65, 294, 308, 598–606 (nur Feature-Analyse als Randhinweis), 675–679, 1056–1063, 1074–1077, 1088–1090 | vollständig für die genannten Bereiche |
| Chat 23 (Etappe 4) | Übersicht **aller Entwicklernachrichten 66–1055** (Anfangszeilen); Volltext der relevanten Bereiche: 361–372 (Tod, Speichern), 427–446 und 551–561 (Koop, Online), 587–629 (Medaillen, Fallgenerator, Aufräumen), 667–679, 719–729 (Bundesstaaten), 984–1039 (Fragenblöcke 3–7 mit Antworten) und 1056–1063 | Entwicklerantworten vollständig in den genannten Bereichen; Assistentennachrichten nur stichwortweise und zu den beantworteten Fragen. **Nicht im Volltext gelesen**: Karten- und Codex-Berichte (etwa 861–977, 988–997), Dateiarbeit (999–1055 außer den genannten), Adult-Anteile |
| Chat 35 | Nachrichten 530–538, 634–636 (Etappe 1–3); in Etappe 4 alle 449 Entwicklernachrichten per Stichwortfilter (Polizei, Fahndung, Gefängnis, Gericht, Beweis, Zeuge, Ermittlung, Detektiv, Koop, Geheim, Verbrechen, Speicher) geprüft, 31 Treffer, Volltext von 635, 733, 749 | **Stichwortdurchgang, kein Volltext aller 951 Nachrichten**; Treffer außerhalb von Adult betreffen Tattoos als Erkennungsmerkmal (749) und die interne Adminversion (635, 636A) |
| Chat 48 | Nachrichten 40–200 (Quiz) | vollständig; Rest (200–297) nur durchsucht (Tesla, Dateiarbeit) |
| Chat 25 | Nachricht 13 | ein Ausschnitt |
| Repository | `Core/include/spn/core/WorldFacts.h`, [Decision 0002](../../Decisions/0002-world-facts.md), [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md), [Proposal 0007](../../Proposals/0007-pickup-items-and-minimal-inventory.md) | gelesen |
| Suche im Master | `Tools/master-search.sh` auf `forensic`, `witness`, `journal`, `evidence`, `detective`, `investigat`, `prison`, `jail`, `justice`, `wanted`, `shapeshifter`, `skinhead`, `motorcycle club` (Tier 1 und 2) | durchgeführt |

---

## 4. Stand gegenüber dem Zielumfang

Das Ziel lautet 10.000 bis 15.000 **substanzielle** Zeilen. Der Stand beträgt **8054 Zeilen**, also etwa **81 %** der unteren Grenze von 10.000. Davon ist nach Abschnitt 2 nur rund ein Drittel quellenbasiert; die Etappe 4 hat den Quellenanteil erhöht, die Schätzung in Abschnitt 2 aber nicht neu berechnet.

**Was die Lücke bedeutet.** Die Quellen für diesen Themenbereich sind weitgehend ausgeschöpft, soweit sie auf den Pfaden liegen, die die Suche im Master und in den Chats ergeben hat. Zusätzliche Zeilen entstehen künftig aus drei Quellen:

1. **Neue Entwicklerentscheidungen** (Antworten auf die offenen Fragen in [Modul 21](21-Konflikte-und-offene-Fragen.md)). Diese würden aus Entwurfsabschnitten beschlossene Anforderungen machen, ohne Fülltext.
2. **Weitere Quellenarbeit**: die nicht ausgewerteten Teile von Chat 23 und Chat 35, andere Chats (Abschnitt 6) und der Tier-3-Bestand des Masters.
3. **Weitere Entwürfe**, etwa Fallvorlagen-Beispiele, Dialogbausteine oder Tabellen. Diese sind erlaubt, aber ohne Entscheidung des Entwicklers nur Vorschläge. Mehr davon ohne neue Anforderungen würde den Anteil an Entwurf weiter erhöhen und die Gefahr von Wiederholung steigern. Das ist ausdrücklich **nicht** das Ziel (Auftrag: keine Wiederholungen, kein Fülltext).

Empfehlung: Vor einer weiteren Etappe der Entwurfsvertiefung zuerst Abschnitt 8 klären.

---

## 5. Abdeckung nach Teilthema

Die Einschätzung bezieht sich auf die **Quellenlage**, nicht auf die Menge an Zeilen. ‚Quellen ausgeschöpft‘ heißt: Es ist keine weitere Aussage in den gelesenen Quellen bekannt, die nicht dokumentiert wäre.

| Teilthema | Module | Quellenlage | Stand |
|---|---|---|---|
| Fallmodell, Falltypen | 02, 09 | Richtung bekannt, keine Mechanik | Quellen ausgeschöpft; Mechanik nur Entwurf |
| Ermittlungsmodi, Hilfen, Einstellungen | 03, 08 | gut (Quiz Oktober) | Quellen weitgehend ausgeschöpft |
| Archiv, Journal, Record | 04 | gut (August und September) | Quellen ausgeschöpft |
| Spuren, Tracking | 05 | mittel | Quellen ausgeschöpft |
| Forensik, Labor | 05, 11 | Katalog bekannt, keine Regeln | Quellen ausgeschöpft |
| Zeugen, Verdächtige, Verhör | 05, 12 | Grundsätze bekannt | Quellen ausgeschöpft |
| Beweislogik | 10 | kaum Quellen | fast nur Entwurf |
| Identität, Ruf, Formwandler | 13 | Quiz Oktober (kurz) | Quellen ausgeschöpft |
| Polizei, Fahndung | 06, 14 | Richtung und Offenlisten | Quellen ausgeschöpft |
| Gerichte, Strafen | 06, 15 | sehr dünn | fast nur Entwurf |
| Haft, Strafvollzug | 16 | gut (Quiz Oktober) | Quellen ausgeschöpft |
| Geheimhaltung, öffentliches Wissen | 17 | gut (Quiz Oktober) | Quellen für Ermittlungsaspekte ausgeschöpft; Story nicht behandelt |
| Koop | 18 | gut (Quiz Oktober) | Quellen ausgeschöpft |
| Weltzustand, Persistenz | 19 | Code und Decisions | sehr gut belegt |
| Detektivpfad | 07, 24 | dünn | Entwurf |
| Kriminalität in der Welt | 06, 25 | mittel (Block 8) | Quellen ausgeschöpft |
| Ereignisse, Notfälle | 07, 26 | mittel (Block 8) | Quellen ausgeschöpft |
| Medien, Smartphone | 05, 27 | mittel (Block 10) | Quellen ausgeschöpft |
| Dynamische Aufträge, Generator | 07, 09 | Wirkung bekannt, kein Generator | Entwurf |

---

## 6. Nicht gelesene oder nicht ausgewertete Quellen

| Quelle | Möglicher Beitrag | Hinweis |
|---|---|---|
| Chat 23, Assistentennachrichten außerhalb der genannten Bereiche | Langtexte zu Welt, Fahrzeugen, Waffen, Wirtschaft | nur stichwortweise; Entwicklerseite ist per Übersicht durchgesehen |
| Chat 23, Nachricht 307 bis 359 (Monster- und Übernatürliches-Framework) | Kreaturen; Berührung mit Wissensstatus und Forschung | im Master-Stand `M-PD` zusammengefasst; Original nicht gelesen |
| Chat 35, außerhalb der genannten Bereiche | Runden zu NPC, Adult, Karten, Dateiarbeit | Adult ist in `Docs/Game/Adult` dokumentiert; Rest nur per Stichwortfilter |
| Chat 40, 45, 54 | Karten, Städte, Werkzeuge | für Ermittlung wenig; nicht gelesen |
| Chat 17, 19, 22, 25 (bis auf 13), 28, 32, 34, 37, 38, 42, 47, 52, 53 | Sonstiges | nur nach Suchwörtern geprüft |
| Chats 11, 12, 13, 15, 50 (Roman und Schreibprojekt) | Mögliche Fall- und Ermittlungsbeispiele einer Romanfassung | **nicht gelesen**; Romanmaterial ist keine Spielentscheidung |
| Chat 48, Nachrichten 200–297 | Tesla, Dateiarbeit | durchsucht, kein Ermittlungsbezug |
| Master Tier 3 (Prompt-Bibliothek) außer `reference/`, `planning-backlog/` und Titel der Fall-Arbeitspakete | Entwurfsprompts der Spuren 03, 04, 05, 11 | Schablonentext; Titel gelesen |
| Master Tier 4 (Historie, Rekonstruktionsquellen) | Entwicklungsverlauf | nur über Chats |
| Dateien des Masters zu Hunter-Archiv-Prompts (Spur 04: 48 Arbeitspakete) | Entwurfsliste des Archivs | nur indirekt über `M-PD` |
| Weitere Quiz-Entscheidungen in Chat 48 (Companion-Plattform, Medizin, Monsterwissen, Territorien) | eigene Themenbereiche | nur soweit für Ermittlung relevant |

---

## 7. Prüfungen

| Prüfung | Ergebnis (zum Zeitpunkt der Erstellung) |
|---|---|
| `check_docs.py` (Chat-Zitate) | 1.655 Zitate, 0 Probleme (Stand Etappe 4) |
| `check_docs.py` (wörtliche Zitate in „…“) | 563 Zitate, 0 nicht gefunden; 116 nicht wörtliche Wendungen wurden in ‚…‘ umgesetzt |
| `check_docs.py` (Quellenschlüssel) | alle definiert (Modul 01) |
| `check_docs.py` (Register-Kennungen `E-…`) | 296 Einträge, keine Duplikate, keine fehlenden |
| `check_links.py` (Links und Anker) | alle Links der Ermittlung gültig (2.840 Links geprüft); zwei fremde Altlasten in `Adult-System.md` und `Adult/10-…` (Anker in der Haupt-README) |
| `git diff --check` | ohne Befund |

Grenzen der Prüfung: Sie belegt, dass **zitierte** Stellen existieren und **wörtliche Zitate** stimmen, nicht, dass jede Deutung richtig ist. Jede Aussage der Spalte „Status“ ist eine **Lesart** der Quelle. Wo eine Antwort mehrdeutig war, steht es in [Modul 21](21-Konflikte-und-offene-Fragen.md) (Abschnitt ‚Unklare Antworten‘).

---

## 8. Nächste Etappen

Entscheidungen, die Entwurf in Anforderung verwandeln würden (Auswahl aus [Modul 21](21-Konflikte-und-offene-Fragen.md)), geordnet nach Wirkung auf den Umfang der Dokumentation:

1. **Wer setzt den Wissensstatus** (Q-E39)? Entscheidet über das Journalmodell.
2. **Wie wird Persistenz aufgeteilt** (Q-E67, Q-E71)? Entscheidet über Weltzustand, Fallbericht und Speichergröße.
3. **Fahndungsstufen oder Intensität** (K-E8)? Entscheidet über Anzeige und HUD.
4. **Hilfe und Medaillen** (Q-E33)? Entscheidet über Erfolgsmodell.
5. **Fallabschluss durch Befehl oder Handlung** (Q-E41)?
6. **Schutzstufen für Fälle** (Q-E36)?
7. **Kriminalitätsursachen und gesellschaftliche Prägung** (K-E3, K-E4)?
8. **Gerichte: Umfang im Basisspiel** (Modul 15 §8) und Jury (Q-E59)?

Quellenarbeit, die weitere beschlossene Inhalte liefern könnte (Stand nach Etappe 4):

1. Volltextdurchgang der **Assistentennachrichten** in Chat 23 (66–1055) und der Chat-35-Nachrichten außerhalb von Adult.
2. **Master Tier 3**: Arbeitspakete der Spuren 04 (Archiv) und 05 (NPC-Beziehungen) auf Kontextzeilen.
3. Chats 11, 12, 13, 15, 50 (Roman/Schreibprojekt) und 40, 45, 54 (Karten): bisher nicht gelesen.
4. **Neue Entwicklerentscheidungen** zu den offenen Fragen (Modul 21), insbesondere Q-E39, Q-E67, Q-E71, K-E8, Q-E33: keine Quelle beantwortet sie, nur Entscheidungen des Entwicklers können das.

Themenbereiche, die dieser Ordner nicht behandelt und die eigene Dokumentation bräuchten: Kreaturen und Monster-KI, NPC-Gedächtnis und Nemesis, Fraktionen, Kampf und Medizin, Fahrzeuge, Companion-App und Karten, Charaktererstellung. Ihr Überblicksstand steht in [Entscheidungsstatus](../Entscheidungsstatus.md).
