# NPCs 28: Abdeckungsprotokoll

> [!IMPORTANT]
> **Dieses Protokoll sagt, was gelesen wurde, was nur durchsucht wurde und was fehlt.** Es behauptet keine vollständige Auswertung des 2,9-GB-Archivs (`CLAUDE.md` §2.6) und keine Erfüllung des Zielumfangs von 10.000 bis 15.000 Zeilen. Zahlen in den erzeugten Blöcken stammen aus dem Werkzeug `Tools/world-audit/render_npc_register.py` und werden bei jedem Lauf neu berechnet.

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Gelesene Quellen](#2-gelesene-quellen)
3. [Nur durchsuchte und nicht gelesene Quellen](#3-nur-durchsuchte-und-nicht-gelesene-quellen)
4. [Themenabdeckung nach Auftrag](#4-themenabdeckung-nach-auftrag)
5. [Umfang und Zählung](#5-umfang-und-zählung)
6. [Was die Quellen nicht hergeben](#6-was-die-quellen-nicht-hergeben)
7. [Hinweise für globale Indizes und Nachbardokumente](#7-hinweise-für-globale-indizes-und-nachbardokumente)
8. [Prüfung](#8-prüfung)

---

## 1. Befund in Kürze

1. **Die Quellenlage ist breit bei der Richtung und dünn bei der Ausführung.** Beschlossen sind Ziele, Grundsätze und Einzelregeln (Block 8, Runde 9B/9C, Fraktionsrunde, Block 10, Monster-Quiz). Es gibt weder Schemata noch Zahlen noch benannte Figuren oder Fraktionen.
2. **Die Hauptquellen sind Gespräche, nicht der Master.** Der Master kennt die Bausteine (`M-PD`), aber keine Nemesis-Mechanik; die verbindliche Anforderung kam am 22.10.2026 (`PE-N1022`).
3. **Der Zielumfang für substanzielle Zeilen wurde nicht erreicht** (Zählung in Abschnitt 5; der Gesamtumfang liegt knapp über 10.000 Zeilen, aber ein Sechstel davon sind aus den Modulen erzeugte Tabellen). Mehr Zeilen hätten mehr Entwurf bedeutet, nicht mehr belegte Entscheidungen; das hätte die Trennung zwischen 📋 und 💡 verwischt (Auftrag: ‚Keine künstliche Textverlängerung‘).
4. **Die Entwürfe sind groß, aber als Entwurf gekennzeichnet.** Sie stehen in eigenen Abschnitten und tragen die Kennung `N-EN-…` im Register.
5. **Konflikte** sind in [Modul 27](27-Konflikte-und-offene-Fragen.md) geführt, nicht entschieden.

---

## 2. Gelesene Quellen

**Vollständig oder in den genannten Bereichen gelesen** (Nachrichten in Chats nach dem Schema `<Chat>#<Nachricht>`):

| Quelle | Bereich | Verwendung |
|---|---|---|
| `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` | alle Abschnitte | Produktentscheidung, Vorschlag, Phasen |
| `Docs/Game/Gameplay-Systeme.md` | §8 | Überblick NPCs |
| `Docs/Game/Spielwelt-und-Untergrund.md` | §13, §14.1–14.2 | Persistenzstufen, Gesellschaft |
| `Docs/Game/Spielvision.md` | §6.2–6.4 | Crew, Begleiter, HQ |
| `Docs/Game/Entscheidungsstatus.md` | Zeile Living World | Einstufung |
| `Docs/Game/Ermittlung/` | Module 12, 13 (§1, §4), 18 (§1–§2), 19 (§1–§6), 26 (§2) | Zeugen, Ruf, Koop, Persistenz, Ereignisse |
| `Docs/Game/Kreaturen/` | Module 01, 03 (Struktur), 08 (§1), 09 (§8), 10 (§5–§9, §14), 11 (§3–§9), 15, 17 (§3–§5, §10), 21, 23 (Struktur) | Monsterseite, Speicher, Hunterwelt |
| `Docs/Game/World/` | Modul 16 (§4–§8), 18 (Struktur), W-TV-010, W-TV-011, K13 | Simulationsebenen, Save-Ziel |
| `Docs/Game/Adult/05-NPCs-Beziehungen-Praeferenzen.md` | §1–§7 | Beziehungsdimensionen (nur Verweis) |
| `Docs/Decisions/0004`, `0005`; `Docs/Proposals/0004` | Auszüge | Schlüsselgrenzen, Stabile IDs, Aussehen, Mods |
| `M-PD` (`PROJECT_DECISIONS.md`) | ‚NPCs and AI‘, ‚World state and persistence‘, ‚Human anatomy …‘, ‚Death / failure‘, ‚Multiple-opponent combat …‘, ‚Stealth, perception and horror AI‘, ‚Companion autonomy …‘, ‚Organic world-event / case emergence‘, ‚Peaceful supernatural beings …‘, ‚Long-term story threats‘, ‚Dialogue …‘, ‚Dialogue volume …‘, ‚Player-created protagonist …‘, ‚Acts, chapters …‘, ‚Smartphone / social media …‘, ‚Cash / law / evidence‘, ‚Modding‘, ‚Multiplayer separation‘ | Master-Grundlage |
| `M-OD`, `M-B22`, `M-B34`, `M-P05` (Titel und `TRACK_CONTEXT.md`), `M-B09` | Treffer | Offene Punkte, Inventar |
| `CANONICAL_ADDENDUM_2026-09-05.md` | NPC-, ‚rival‘-Treffer | Planungslinie |
| `M-MOD`, `M-MOR`, `MODDING_BLOCK_STATUS_2026-09-11.md` | vollständig | Modding |
| `M-PO`, `M-AS`, `M-OQ16`, `AI_AUTONOMY_BOUNDARIES.md`, `AI_RECOGNITION_AND_CONFLICTS.md` | vollständig bzw. NPC-Treffer | Simulationsvorschläge, KI-Bewohner (nicht übernommen) |
| Chat 23 | `47A`–`52E` (Treffer `164A`, `166A` nur als Stichwort), `117A`/`118E` (Punkt 17), `313A`, `603A`–`606A`, `676A`–`681E`, `1056A`–`1058A`, `1059E`–`1063E`, `1064A`–`1066A`, `1088A`–`1090A` | Gedächtnis, Block 8, Fraktionen, Block 10, Dialog, RDR2-Auswahl |
| Chat 35 | `258A`–`282A`, `296A`, `784A`–`786A` | Runde 9A/9B/9C, Aussehen, Tattoos |
| Chat 48 | `97E`–`98A`, `103E`–`104A`, `137E`–`148A`, `153E`/`154A`, `163E`/`164A` | Koop, Monster-KI und -gesellschaft, Rufsystem, Zeitsprung |
| Chat 19 | `3A`, `4E` (Treffer) | frühe Themenliste (Rivalität) |

---

## 3. Nur durchsuchte und nicht gelesene Quellen

| Quelle | Umfang | Ergebnis der Suche |
|---|---|---|
| Chats 23, 35, 48 (übrige Nachrichten) | Stichwortsuche nach NPC, Gedächtnis, Beziehung, Fraktion, Gerücht, Familie, Tagesablauf, Rivalität, Begleiter | Treffer in anderen Themenblöcken (Waffen, Fahrzeuge, Adult, Welt); nicht ausgewertet |
| Chat 40 | Stichwortsuche | Treffer zu ‚Tagesabläufen‘ betreffen die Arbeitsplanung, nicht NPCs |
| Chats 27, 29 | Stichwortsuche | RDR2-Feature-Auflistung; Auswahl in `23#603A` gelesen |
| Master Tier 1 (übrige Dateien) | `master-search.sh` mit ‚nemesis‘, ‚rivalry‘, ‚grudge‘, ‚living world‘, ‚npc memory‘, ‚faction‘, ‚NPC‘ | siehe [Modul 01](01-Quellenlage-und-Methodik.md) §6 |
| Master Tier 2 (Deltas 2026-10-14 bis 2026-10-19) | dieselbe Suche | NPC-Treffer in Population, KI-Grenzen, Kommunikation, offenen Fragen; nur die genannten gelesen |
| Master Tier 3 | `--tier 3 --count` | Prompt-Entwürfe der Spur 05: nur Titel und `TRACK_CONTEXT.md`; Inhalt boilerplate |
| Master Tier 4, Roman-Kapitel, Referenzseiten (RDR2-Modifikation) | nicht gelesen | – |
| Tier-1-Dateien `MASTER_STATUS_*`, `SOURCE_OF_TRUTH_*`, `CURRENT_CANON_2026-09-16.md` | nicht einzeln gelesen | Treffer der Suche enthielten keine NPC-Mechanik |
| `Docs/Game/Adult/` (übrige Module) | nicht gelesen | Adult-Inhalte sind Gegenstand eines eigenen Ordners |
| `Docs/Game/World/` (übrige Module) | nicht gelesen | Geometrie und Karte sind nicht Gegenstand |

---

## 4. Themenabdeckung nach Auftrag

| Auftragsbereich | Module | Quellenlage | Bemerkung |
|---|---|---|---|
| NPC-Gedächtnis: individuelle Erinnerungen, Konsequenzen, Wiedererkennung, Gerüchte, Vergessen, Verbindung zu Ermittlung und Identität | 05, 06, 07, 20 | **gut** für Prinzipien (Block 8C, Runde 9B), **keine** Zahlen | Datensatz, Verblassen, Verdichtung sind Entwurf |
| Beziehungen: Freundschaft, Feindschaft, Vertrauen, Familien und Netzwerke, Reaktionen auf Spielerhandlungen, Langzeitveränderung | 08, 09, 10, 19 | **gut** für Prinzipien (Block 10A/B, Runde 9B.2/9B.3), Adult-Details ausgelagert | Dimensionen benannt, nicht definiert |
| Nemesis: Rivalen, Überlebende, Anpassung, Verletzungen, Feindschaft, Entwicklung | 11, 12 | **dünn:** Produktanforderung C (22.10.2026) und Monsterbausteine; **keine** Mechanik in den Quellen | Modell, Lebenszyklus, Grenzen sind Entwurf |
| Autonomes NPC-Leben: Tagesabläufe, Arbeit, Freizeit, Reisen, Entscheidungen, Lebensereignisse, Weltereignisse | 13, 14, 15 | **gut** für Prinzipien (Block 8A/8B/8L/8M/8N) | Zeitplan- und Entscheidungsmodell Entwurf |
| Fraktionen und Gesellschaft: Gruppen, Territorien, Hierarchien, Konflikte, Machtverschiebung, Behörden, Hunter, Wesen | 16, 17, 18 | **mittel:** Fraktionsrunde (`23#676A`–`680A`, überwiegend Vorschläge des Assistenten mit Entwicklerantworten), Monster-Quiz (`48#141E`–`146A`) | keine Fraktionsliste |
| Weltpersistenz: Speicherung, Simulation über Zeit, Sessions, Koop und Host-Save, Performance und Skalierung | 21, 22, 23 | **mittel:** Master-Grundsätze, Runde 9C, Proposal 0010 §5–§6; **keine Budgets** | Größenrechnung ist Annahme |
| Integration und Konsistenz | 24, 25 | Anforderung H und Proposal 0010 §7 (Ingenieurvorschlag) | Besitzer, Ereignisse, Invarianten sind Entwurf |
| Quellen-/Entscheidungsregister, Konflikte, Abdeckung, Tests | 01, 26–29 | – | erzeugt bzw. geführt |

---

## 5. Umfang und Zählung

Die folgenden Blöcke werden von `Tools/world-audit/render_npc_register.py` erzeugt.

<!-- BEGIN GENERATED LINE COUNTS -->
| Datei | Zeilen |
|---|---:|
| `01-Quellenlage-und-Methodik.md` | 180 |
| `02-Leitsaetze-und-Abgrenzung.md` | 150 |
| `03-Identitaet-Persistenzstufen-und-Datensatztiefe.md` | 293 |
| `04-Persoenlichkeit-Zustaende-und-Zeitskalen.md` | 250 |
| `05-Gedaechtnis-Erinnerung-und-Vergessen.md` | 376 |
| `06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md` | 291 |
| `07-Informationsfluss-Geruechte-und-Kommunikation.md` | 307 |
| `08-Beziehungen-Modell-und-Wirkung.md` | 325 |
| `09-Beziehungsentwicklung-Freundschaft-Feindschaft-und-Vertrauen.md` | 306 |
| `10-Familien-Haushalte-und-soziale-Netzwerke.md` | 256 |
| `11-Nemesis-Modell-und-Datensatz.md` | 387 |
| `12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md` | 383 |
| `13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md` | 365 |
| `14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md` | 328 |
| `15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md` | 320 |
| `16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md` | 428 |
| `17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md` | 340 |
| `18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md` | 312 |
| `19-Begleiter-Crew-und-Rekrutierung.md` | 364 |
| `20-Dialog-und-Sichtbarkeit-der-Beziehungen.md` | 318 |
| `21-Weltpersistenz-und-Speicherentwurf.md` | 356 |
| `22-Simulationsstufen-Skalierung-und-Performance.md` | 346 |
| `23-Koop-Host-Speicherstand-und-Mods.md` | 262 |
| `24-Integration-und-Konsistenzregeln.md` | 348 |
| `25-Querschnittsszenarien.md` | 333 |
| `26-Entscheidungsregister.md` | 1123 |
| `27-Konflikte-und-offene-Fragen.md` | 181 |
| `28-Abdeckungsprotokoll.md` | 201 |
| `29-Testkatalog-und-Abnahmekriterien.md` | 632 |
| `README.md` | 95 |
| **gesamt (30 Dateien)** | **10156** |
<!-- END GENERATED LINE COUNTS -->

<!-- BEGIN GENERATED STATS -->
| Kennzahl | Wert |
|---|---:|
| Zeilen gesamt (alle Dateien des Ordners) | 10156 |
| davon aus den Modulen erzeugt (Register, Regeltests, Zählblöcke) | 1656 |
| Rest: von Hand geschriebene Zeilen (Zielgröße 10.000 bis 15.000 gilt für substanzielle Zeilen) | 8500 |
| Register-Einträge gesamt (Aussagen und Entwurfsregeln) | 843 |
| davon Aussagen (Gruppen LT bis KS) | 523 |
| davon Entwurfsregeln (Gruppe EN) | 320 |
| Aussagen mit Status 📋 (Mehrfachstatus zählt mehrfach) | 430 |
| Aussagen mit Status 🧭 (Mehrfachstatus zählt mehrfach) | 48 |
| Aussagen mit Status 💡 (Mehrfachstatus zählt mehrfach) | 37 |
| Aussagen mit Status 🔭 (Mehrfachstatus zählt mehrfach) | 0 |
| Aussagen mit Status ❓ (Mehrfachstatus zählt mehrfach) | 19 |
| Aussagen mit Status ⚠️ (Mehrfachstatus zählt mehrfach) | 3 |
| Konflikte C-N | 9 |
| Unklare Antworten U-N | 11 |
| Offene Fragen Q-N | 34 |
<!-- END GENERATED STATS -->

**Einordnung.** Die Vorgängerordner haben rund 8.050 (Ermittlung), 5.350 (Kreaturen) und 4.640 (Welt) Zeilen. Der Zielumfang von 10.000 bis 15.000 Zeilen je großem Themenbereich ist ein **langfristiges Ziel für substanzielle Zeilen**. Er wurde hier nicht erzwungen. Die erzeugten Tabellen (Register, Regeltests, Zählblöcke) zählen nicht dazu, weil sie die Modulinhalte nur neu ordnen. Ein erheblicher Teil der von Hand geschriebenen Zeilen ist Entwurf, Prüfregel oder Szenario und als solcher gekennzeichnet; er ist nicht als Beschluss zu lesen.

---

## 6. Was die Quellen nicht hergeben

| Thema | Befund |
|---|---|
| **Benannte NPCs, Crew, Antagonisten** | nicht festgelegt (`M-OD`; Proposal 0010 §12); der Ordner nennt keine |
| **Fraktionsliste, Namen, Ziele** | nicht festgelegt; Proposal 0010 §11 |
| **Nemesis-Mechanik im Detail** | keine Quelle vor dem 22.10.2026; Modell ist Entwurf |
| **Zahlen:** Gedächtnisgrenzen, Skalen, Schwellen, Zeitschritte, Budgets, Anteile, Dauer | nicht festgelegt; `M-OQ16` |
| **Spielertod, Niederlage** | offen (`M-PD`) |
| **Familiengründung, Schwangerschaft** | offen / niedrige Priorität |
| **Untergrundzivilisation und KI-Bewohner im NPC-System** | nicht übernommen; Q-N25 |
| **Online-NPC-Welt** | keine Entscheidung |
| **Mod-Erweiterungspunkte des NPC-Kerns** | offen (Q-N33) |
| **Replikation im Koop** | offen |
| **Kinder-NPCs (Gewalt, Darstellung)** | offen, rechtlich zu prüfen |
| **RDR2-Mod als Vergleich** | nicht geöffnet, nicht ausgewertet (Proposal 0010 §1) |

---

## 7. Hinweise für globale Indizes und Nachbardokumente

Die folgenden Änderungen sind **nicht ausgeführt** (Auftrag: nicht bearbeiten; Übernahme später kontrolliert) und gehören in den Abschlussbericht:

| Datei | Vorschlag |
|---|---|
| `README.md` (Projektwurzel) | Eintrag für `Docs/Game/NPCs/` neben Ermittlung, Kreaturen, World, Adult |
| `Docs/MasterIndex.md` | Themenzeile ‚NPCs, Gedächtnis, Beziehungen, Nemesis, Living World‘ mit Verweis auf diesen Ordner und auf die Suchbegriffe (‚nemesis‘ ohne Treffer; ‚rival organizations‘ als einzige Tier-1-Stelle) |
| `Docs/Game/Gameplay-Systeme.md` §8 | Verweis auf `NPCs/README.md` |
| `Docs/Game/Spielvision.md` §6, §9 | Verweis auf `NPCs/README.md` (Crew, Begleiter, Nemesis) |
| `Docs/Game/Entscheidungsstatus.md` | Zeile ‚Living World …‘: Verweis auf `NPCs/README.md` und auf [Modul 27](27-Konflikte-und-offene-Fragen.md) |
| `Docs/Game/Kreaturen/README.md`, `Docs/Game/Ermittlung/README.md`, `Docs/Game/World/README.md` | Rückverweis auf `NPCs/README.md` |
| `Docs/Game/World/21-Konflikte-und-Widersprueche.md` | Hinweis auf C-N2 (gleiche Spannung wie K13) und C-N5 (Einstufung der Simulationsebenen) |
| `Tools/world-audit/README.md` | Abschnitt ‚NPC-Dokumentation‘ (in diesem Arbeitsgang ergänzt) |
| `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` | optional: Verweis auf [Modul 01](01-Quellenlage-und-Methodik.md) §6 (Ergänzung zu ‚Rivalität‘ in Gesprächen und Tier 1) |

---

## 8. Prüfung

| Prüfung | Werkzeug | Ergebnis |
|---|---|---|
| Zitate, wörtliche Zitate, Quellenschlüssel, Registerkennungen | `check_docs.py` mit `SPN_ID_PREFIX=N` | siehe Abschlussbericht (der Lauf erfolgt nach dem letzten Edit) |
| Links und Anker | `check_links.py Docs/Game` | siehe Abschlussbericht |
| Register-Eindeutigkeit und Verweise | `render_npc_register.py` | siehe Abschlussbericht |
| Whitespace | `git diff --check` | siehe Abschlussbericht |

Die Ergebnisse stehen bewusst nicht in diesem Dokument, weil das Protokoll selbst Teil des geprüften Bestands ist; maßgeblich ist der Lauf nach dem jeweils letzten Edit.
