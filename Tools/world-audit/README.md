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
