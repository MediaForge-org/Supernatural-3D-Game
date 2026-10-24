# Kampfsystem: Dokumentation

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieser Ordner dokumentiert und prüft das geplante Kampfsystem des Spiels: Nahkampf, Fernkampf, Verteidigung, Waffen, Verletzung, Tod und Rettung, Kreaturenkampf, Fallen, Gegner-KI, Nemesis, Polizei und Folgen, Koop, Speicherung und Leistung. Er ist **kein Kanon** und **keine Spielbalance**. Er trennt streng zwischen dem, was entschieden ist, und dem, was nur Entwurf oder offen ist. Es gibt keine Schadens-, Reichweiten- oder Zeitwerte, weil die Quellen keine nennen. Auftrag: Dokumentation und Review (23.10.2026), Implementierung nicht freigegeben.

Teil von [`Docs/Game/`](..). Er ergänzt [Gameplay-Systeme §4 und §5](../Gameplay-Systeme.md#4-verletzungen-und-medizin), [Spielvision §14](../Spielvision.md#14-schwierigkeit-scheitern-und-tod) und [Entscheidungsstatus §5.6](../Entscheidungsstatus.md) und stützt sich auf die Nachbarordner [Kreaturen](../Kreaturen/README.md), [NPCs](../NPCs/README.md), [Ermittlung](../Ermittlung/README.md) und [World](../World/README.md).

## Statuslegende

| Zeichen | Wort | Bedeutung |
|---|---|---|
| 📋 | BESCHLOSSEN | durch Entwickleraussage oder `M-PD` ohne Vorbehalt festgelegt |
| 🧭 | RICHTUNG | gebilligte Richtung, ausdrücklich nicht abgeschlossen |
| 🧩 | ABGELEITET | folgt zwingend aus beschlossenen Anforderungen (Prämissen genannt) |
| 💡 | VORSCHLAG | Entwurf oder Beispiel, nicht genehmigt |
| ❓ | OFFEN | nicht entschieden |
| ⚠️ | KONFLIKT | Quellen widersprechen einander, nicht aufgelöst |
| 🔍 | UNBELEGT | keine auffindbare Quelle für die Entwicklerentscheidung |

Begründung und Vergaberegeln: [Modul 01](01-Quellenlage-und-Methodik.md) §3.

## Lesereihenfolge

1. [01 Quellenlage und Methodik](01-Quellenlage-und-Methodik.md): was gelesen wurde und wie Aussagen eingestuft sind.
2. [02 Kampfvision und Grundprinzipien](02-Kampfvision-und-Grundprinzipien.md) und [28 Konflikte und offene Fragen](28-Konflikte-und-offene-Fragen.md): die Grundhaltung und was ungeklärt ist.
3. Die Fachmodule nach Interesse (Tabelle unten).
4. [26 Querschnittsszenarien](26-Querschnittsszenarien.md) und [30 Testkatalog](30-Testkatalog-und-Abnahmekriterien.md) als Prüfhilfe.
5. [27 Register](27-Entscheidungsregister.md) und [29 Abdeckung](29-Abdeckungsprotokoll.md) als Nachschlagewerke.

## Die Module

| Nr. | Datei | Inhalt |
|---|---|---|
| 01 | [Quellenlage und Methodik](01-Quellenlage-und-Methodik.md) | Quellen, Schlüssel, Statuswörter, Lesenachweis, Grenzen |
| 02 | [Kampfvision und Grundprinzipien](02-Kampfvision-und-Grundprinzipien.md) | Realismus, Spielbarkeit, Risiko, Schwierigkeit, Wechselwirkungen |
| 03 | [Kampfzustände und Systemarchitektur](03-Kampfzustaende-und-Systemarchitektur.md) | Schichten, Zustandsmodell, Datenverantwortung, Ereignisse |
| 04 | [Nahkampf und Kampfstile](04-Nahkampf-und-Kampfstile.md) | Striking, Clinch, Boden, Aufgabegriffe, Fight Club, Lernen |
| 05 | [Fernkampf und Schusswaffen](05-Fernkampf-und-Schusswaffen.md) | Handhabung, Zielen, Ballistik, Long Range |
| 06 | [Steuerung und Eingabekontext](06-Steuerung-und-Eingabekontext.md) | Controller, Tastatur, Maus, DualSense-Vorgaben, Kontexte |
| 07 | [Ausweichen, Blocken, Parieren und Konter](07-Ausweichen-Blocken-Parieren-und-Konter.md) | Verteidigung |
| 08 | [Kampfbewegung und Traversal-Integration](08-Kampfbewegung-und-Traversal-Integration.md) | Springen, Hürden, Kamera, Schnittstellen |
| 09 | [Waffenklassen und Ausrüstung](09-Waffenklassen-und-Ausruestung.md) | Klassen, Trageorte, seltene Waffen |
| 10 | [Munition, Nachladen und Waffenzustand](10-Munition-Nachladen-und-Waffenzustand.md) | Munition, Magazine, Störungen, Pflege |
| 11 | [Ausdauer, Erschöpfung und Belastung](11-Ausdauer-Erschoepfung-und-Belastung.md) | Körper und Belastung |
| 12 | [Treffermodell und Körperphysik](12-Treffermodell-und-Koerperphysik.md) | Trefferzonen, Weg, Wunden, Physik |
| 13 | [Verletzungen, Schmerz und Kampffolgen](13-Verletzungen-Schmerz-und-Kampffolgen.md) | Blutung, Schmerz, Behandlung, Heilung |
| 14 | [Bewusstlosigkeit, Tod und Rettung](14-Bewusstlosigkeit-Tod-und-Rettung.md) | Niederlage, Gameplay-Tod, Rettung (Konflikt `CK-01`) |
| 15 | [Kreaturenkampf und Spezialfähigkeiten](15-Kreaturenkampf-und-Spezialfaehigkeiten.md) | Kampfprofile, Existenzformen |
| 16 | [Schwächen, Vorbereitung und Jagdmethoden](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) | Recherche, Vorbereitung, Wirkung |
| 17 | [Fallen, Fesselung und Lebendfang](17-Fallen-Fesselung-und-Lebendfang.md) | Fallen, Gefangennahme |
| 18 | [Gegner-KI und Kampftaktik](18-Gegner-KI-und-Kampftaktik.md) | Wahrnehmung, Wissen, Taktik, Moral |
| 19 | [Nemesis, Erinnerung und persistente Kampffolgen](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) | Rückkehr, Narben, Lernen |
| 20 | [Fraktionen, Polizei und Kampfkonsequenzen](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) | Zeugen, Beweise, Fahndung, Recht |
| 21 | [Kampfvermeidung, Flucht und Deeskalation](21-Kampfvermeidung-Flucht-und-Deeskalation.md) | Alternativen zum Kampf |
| 22 | [Animationen, Trefferreaktionen und Ragdoll](22-Animationen-Trefferreaktionen-und-Ragdoll.md) | Animation und Physik |
| 23 | [Koop, Synchronisierung und Autorität](23-Koop-Synchronisierung-und-Autoritaet.md) | Host-Spielstand, Netzwerkfragen |
| 24 | [Speicherstände und Weltpersistenz](24-Speicherstaende-und-Weltpersistenz.md) | Speichern, Checkpoints, Segmente |
| 25 | [Performance, Skalierung und Simulationsstufen](25-Performance-Skalierung-und-Simulationsstufen.md) | Kostentreiber, Stufen, Messplan |
| 26 | [Querschnittsszenarien](26-Querschnittsszenarien.md) | zwölf Szenarien |
| 27 | [Entscheidungsregister](27-Entscheidungsregister.md) | erzeugtes Register aller Aussagen |
| 28 | [Konflikte und offene Fragen](28-Konflikte-und-offene-Fragen.md) | `CK`, `CU`, `CQ`, Klärungsreihenfolge |
| 29 | [Abdeckungsprotokoll](29-Abdeckungsprotokoll.md) | Quellen, Prüfungen, Integration |
| 30 | [Testkatalog und Abnahmekriterien](30-Testkatalog-und-Abnahmekriterien.md) | Tests nach zwölf Kategorien plus Szenarien |

## Zuordnung zu den Querverweisen

| Thema | Dieser Ordner | Nachbarordner |
|---|---|---|
| Kreaturenregeln, Schwächen, Fallen | Module 15 bis 17 | [Kreaturen 04, 10, 12, 13](../Kreaturen/README.md) |
| Gedächtnis, Nemesis, Begleiter, Tod von NPCs | Modul 19, 18 | [NPCs 05, 11, 12, 14, 19](../NPCs/README.md) |
| Spuren, Zeugen, Polizei, Haft, Koop-Ermittlung | Modul 20, 23 | [Ermittlung 05, 06, 14, 16, 18](../Ermittlung/README.md) |
| Weltzustand, Speicherung, Leistung | Module 24, 25 | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md), [NPCs 21, 22](../NPCs/README.md) |
| Bewegung und Traversal | Modul 08 | [Proposal 0009](../../Proposals/0009-traversal-jump-and-vault-direction.md) |

## Werkzeuge

Das Register (27) und die Statistik in Modul 29 werden erzeugt; die Prüfungen laufen mit den Hilfsskripten in `Tools/world-audit/` (siehe dort `README.md`):

```text
python3 -I Tools/world-audit/render_combat_register.py Docs/Game/Kampfsystem
SPN_MASTER=Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27 SPN_WORLD_DOCS=Docs/Game/Kampfsystem SPN_ID_PREFIX=COMBAT-DEC SPN_REGISTER_FILE=27-Entscheidungsregister.md python3 -I Tools/world-audit/check_docs.py <Arbeitsordner mit Chat-JSON>
python3 -I Tools/world-audit/check_links.py Docs/Game
```

## Was dieser Ordner nicht tut

- Er legt **keine** Zahlen, Eingabebelegungen (außer Kreuz = Springen), Waffenlisten, Artenlisten oder Todesloops fest.
- Er ändert **keinen** bestehenden Kanon, keine Master-Datei und keinen Code.
- Er führt **keine** Tests aus; der Katalog beschreibt künftige Prüfungen.
