# world-audit

Reproduzierbare Hilfsskripte für Etappe 3 (Welt-Dokumentation). Nur Python-Standardbibliothek; der Master wird ausschließlich gelesen. Aufrufe: `Docs/Game/World/23-Quellenabdeckung-und-Auditbericht.md` §6.

| Skript | Aufgabe |
|---|---|
| `inventory.py` | Themenzähler je Master-Datei (`topic_files.tsv`) |
| `split_chats.py` | zerlegt Chat-Transkripte in JSON-Nachrichten |
| `gpkg_summary.py`, `gpkg_metrics.py`, `gpkg_metrics_lib.py` | lesen das WORLD_700-GeoPackage, berechnen Flächen/Längen |
| `render_w700.py` | erzeugt Modul 24 |
| `render_index.py` | erzeugt Modul 25 (Weltaussagen-Index) aus den Modulen 20 bis 22 |
| `render_chat35.py`, `chat35_read_log.json` | erzeugen Modul 27 (Abdeckungsprotokoll Chat 35) aus dem Lesenachweis |
| `check_docs.py` | prüft Zitate, wörtliche Zitate, Quellenkürzel, Register-IDs |
| `check_links.py` | prüft relative Links und Anker |

## Ermittlungs-Dokumentation

`check_docs.py` prüft auch `Docs/Game/Ermittlung` (Register-Kennungen `E-…` statt `W-…`):

```text
python3 Tools/world-audit/split_chats.py <Master> <Arbeitsordner> 23 35 27 29 40
SPN_MASTER=<Master> SPN_WORLD_DOCS=Docs/Game/Ermittlung SPN_ID_PREFIX=E python3 Tools/world-audit/check_docs.py <Arbeitsordner>
python3 Tools/world-audit/check_links.py Docs/Game
```

## Kreaturen-Dokumentation

`check_docs.py` prüft auch `Docs/Game/Kreaturen` (Register-Kennungen `K-…`). Zusätzlich zu den Chats 23, 35 und 48 werden die Chats 19, 30 und 50 benötigt:

```text
python3 Tools/world-audit/split_chats.py <Master> <Arbeitsordner> 23 35 48 19 30 50
SPN_MASTER=<Master> SPN_WORLD_DOCS=Docs/Game/Kreaturen SPN_ID_PREFIX=K python3 Tools/world-audit/check_docs.py <Arbeitsordner>
python3 Tools/world-audit/check_links.py Docs/Game
```

## NPC-Dokumentation

`check_docs.py` prüft auch `Docs/Game/NPCs` (Register-Kennungen `N-…`, Registerdatei `26-Entscheidungsregister.md`). Zusätzlich zu den Chats 23, 35 und 48 wird Chat 19 benötigt:

```text
python3 Tools/world-audit/split_chats.py <Master> <Arbeitsordner> 23 35 48 19
SPN_MASTER=<Master> SPN_WORLD_DOCS=Docs/Game/NPCs SPN_ID_PREFIX=N SPN_REGISTER_FILE=26-Entscheidungsregister.md python3 Tools/world-audit/check_docs.py <Arbeitsordner>
python3 Tools/world-audit/check_links.py Docs/Game
```

| Skript | Aufgabe |
|---|---|
| `render_npc_register.py` | erzeugt `Docs/Game/NPCs/26-Entscheidungsregister.md` aus den Kennungstabellen der Module 02 bis 24, prüft Eindeutigkeit, Zellenzahl und alle Verweise `N-XX-nnn`, schreibt die erzeugten Zählblöcke in Modul 28 |
| `render_npc_tests.py` | erzeugt den Regeltestteil von `Docs/Game/NPCs/29-Testkatalog-und-Abnahmekriterien.md` aus den Entwurfsregeln `N-EN-nnn` |

Reihenfolge nach Änderungen an Modulen: erst `render_npc_tests.py`, dann `render_npc_register.py`, dann die Prüfungen.

## Kampfsystem-Dokumentation

`check_docs.py` prüft auch `Docs/Game/Kampfsystem` (Register-Kennungen `COMBAT-DEC-XX-nnn`, Registerdatei `27-Entscheidungsregister.md`). Benötigt werden die Chats 23, 35, 48 und 19 (und die übrigen bereits zerlegten):

```text
python3 Tools/world-audit/split_chats.py <Master> <Arbeitsordner> 23 35 48 19
SPN_MASTER=<Master> SPN_WORLD_DOCS=Docs/Game/Kampfsystem SPN_ID_PREFIX=COMBAT-DEC SPN_REGISTER_FILE=27-Entscheidungsregister.md python3 Tools/world-audit/check_docs.py <Arbeitsordner>
python3 Tools/world-audit/check_links.py Docs/Game
```

| Skript | Aufgabe |
|---|---|
| `render_combat_register.py` | erzeugt `Docs/Game/Kampfsystem/27-Entscheidungsregister.md` aus den Tabellen der Module 02 bis 25, prüft Eindeutigkeit, Zellenzahl, Statuswörter, Gruppen-Modulzuordnung, alle Verweise `COMBAT-DEC-XX-nnn`, `CQ-nn`, `CK-nn`, `CU-nn`, `CS-nn`, Eindeutigkeit der Test-IDs `CT-XX-nnn` und schreibt die erzeugten Blöcke in Modul 29 |

## Dokumentation Fahrzeuge und Verkehr

`check_docs.py` prüft auch `Docs/Game/Fahrzeuge-und-Verkehr` (Register-Kennungen `VEH-DEC-XX-nnn`, Registerdatei `28-Entscheidungsregister.md`). Die Umgebungsvariable `SPN_GENERATED` (kommagetrennte Modulpräfixe, Standard `25-`) legt fest, welche erzeugten Module bei den Zitat-, Schlüssel- und Anführungsprüfungen übersprungen werden; für diesen Ordner `28-`. Benötigt werden die Chats 23, 30, 35, 45 und 48:

```text
python3 Tools/world-audit/split_chats.py <Master> <Arbeitsordner> 23 30 35 45 48
python3 -I Tools/world-audit/render_vehicle_register.py Docs/Game/Fahrzeuge-und-Verkehr
SPN_GENERATED=28- SPN_MASTER=<Master> SPN_WORLD_DOCS=Docs/Game/Fahrzeuge-und-Verkehr SPN_ID_PREFIX=VEH-DEC SPN_REGISTER_FILE=28-Entscheidungsregister.md python3 -I Tools/world-audit/check_docs.py <Arbeitsordner>
python3 -I Tools/world-audit/check_links.py Docs/Game
```

| Skript | Aufgabe |
|---|---|
| `render_vehicle_register.py` | erzeugt `Docs/Game/Fahrzeuge-und-Verkehr/28-Entscheidungsregister.md` aus den Tabellen der Module 02 bis 27, prüft Eindeutigkeit, Zellenzahl, Statuswörter, Gruppen-Modulzuordnung, alle Verweise `VEH-DEC-XX-nnn`, `CQ-nn`, `CK-nn`, `CU-nn`, die Eindeutigkeit der Test-IDs `CT-XX-nnn`, löst Verweise auf die Register von Welt, NPCs, Ermittlung, Kreaturen und Kampfsystem gegen deren Registerdateien auf und schreibt die erzeugten Blöcke in Modul 30 |
