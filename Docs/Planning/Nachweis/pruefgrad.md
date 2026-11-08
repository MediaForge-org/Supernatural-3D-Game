# Prüfgrad der Nachweisregister (NIGHT-AUDIT-03, Priorität 2)

- **Stand:** 08.11.2026
- **Zweck:** Dokumentiert, wie tief jede Anforderung tatsächlich geprüft ist. Eine gültige Zeilennummer gilt nicht als inhaltliche Prüfung.

## Stufen
- **S0** Zeile existiert (maschinell geprüft).
- **S1** Stichwort in der Zeile gefunden (kein Vergleich).
- **S2** Quelle und Ziel inhaltlich gelesen und verglichen (in dieser oder einer früheren Sitzung ausdrücklich bearbeitet).
- **S3** Zusätzlich gegen die unabhängige Masterquelle geprüft.

## Stand je Register

| Register | Anzahl | Ergebnis je Stufe | Anmerkung |
|---|---:|---|---|
| `welt.md` | 120 | 18 Zeilen mit „teilweise“ inhaltlich bearbeitet (S2); 102 übrige mit früherem Ergebnis, überwiegend S0/S1 | Die 18 Zeilen liegen in den Abschnitten „Konsolidierung“ (CONSOLIDATION-02). S3 nicht durchgeführt. |
| `systeme.md` | 151 | 15 Zeilen mit „teilweise“ inhaltlich bearbeitet (S2); 136 übrige S0/S1 | S3 nicht durchgeführt. |
| `adult.md` | 131 | 312 Zitierungen maschinell (S0), 6 Stichproben inhaltlich (S2): ADU-B-005, B-010, B-013, D-001, C-007, E-07 (Quellzeilen 01:33, 01:117–118, 02:92, 08:99) | Die übrigen 125 nur S0/S1. Keine Masterquelle geprüft (S3). |
| `tagesdeltas.md` | 44 | Die sechs CHANGELOGs vollständig gelesen; Einzelpunkte gegen Docs über Stichwort und Stichprobe (S1/S2) | Die `docs/`-Dateien der Deltas wurden nur punktuell gelesen. |
| `gameplay.md` | 222 | 222 inhaltlich verglichen mit Docs (S2); 222 gegen den Master geprüft (S3) — siehe `master-gegenpruefung-gameplay.md`: 28 bestätigt, 52 teilweise, 116 nur konsolidiert, 19 nur Delta, 2 widersprüchlich, 2 nicht geöffnet, 3 Master-offen | S3 ist damit für Gameplay nur **teilweise**: „bestätigt“ bzw. „teilweise“ ist ein Masterbeleg, „nur konsolidiert“ nicht. |

## Konsequenz

Die Aussage „vollständig dokumentiert“ in früheren Berichten meint Stufe S0 bis S1 für die Mehrzahl der Zeilen. Inhaltlich vollständig (S2) sind für Welt und Systeme nur die bearbeiteten Zeilen, für Adult nur 6 Stichproben. Ein Gegenprüfungslauf gegen den Master (S3) fehlt für Welt, Systeme und Adult vollständig. Das ist der nächste Schritt in dieser Priorität.
