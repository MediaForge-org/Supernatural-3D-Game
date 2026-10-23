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
