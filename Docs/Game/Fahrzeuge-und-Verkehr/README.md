# Fahrzeuge und Verkehr: Dokumentation

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieser Ordner dokumentiert und prüft das geplante System aus Fahrzeugen, Straßen und Verkehr: Fahrzeugklassen, Fahrphysik, Zustand und Werkstatt, Straßennetz, Kreuzungen, Verkehrs-KI, NPC-Fahrer, Unfälle, Polizei, Besitz und Diebstahl, Ermittlung, Persistenz, Koop, die Schnittstelle zu WorldForge sowie Unreal-Integration, Simulationsstufen und Leistung. Er ist **kein Kanon** und **keine Spielbalance**. Er trennt streng zwischen dem, was entschieden ist, und dem, was Entwurf oder offen ist. Es gibt kein Verkehrsmodell, keine Fahrzeugliste, keine Verkehrsregeln und keine Zahlenwerte, weil die Quellen keine nennen. Auftrag: Dokumentation (24.10.2026), Implementierung nicht freigegeben.

Teil von [`Docs/Game/`](..). Er ergänzt [Gameplay-Systeme §6](../Gameplay-Systeme.md#6-fahrzeuge-und-fortbewegung) und die [Welt-Dokumentation](../World/README.md) (Module 09, 13, 14, 15, 16, 18) und stützt sich auf die Nachbarordner [NPCs](../NPCs/README.md), [Ermittlung](../Ermittlung/README.md), [Kreaturen](../Kreaturen/README.md) und [Kampfsystem](../Kampfsystem/README.md).

## Statuslegende

| Zeichen | Wort | Bedeutung |
|---|---|---|
| 📋 | BESCHLOSSEN | durch Entwickleraussage oder `M-PD` ohne Vorbehalt festgelegt (übernommen aus dem Register der Nachbarordner) |
| 🧭 | RICHTUNG | gebilligte Richtung, ausdrücklich nicht abgeschlossen |
| 🧩 | ABGELEITET | folgt zwingend aus beschlossenen Anforderungen (Prämissen genannt) |
| 💡 | VORSCHLAG | Entwurf oder Beispiel, nicht genehmigt |
| ❓ | OFFEN | nicht entschieden |
| ⚠️ | KONFLIKT | Quellen widersprechen einander, nicht aufgelöst |
| 🔍 | UNBELEGT | keine auffindbare Quelle |

Begründung und Vergaberegeln: [Modul 01](01-Quellenlage-und-Methodik.md) §3.

## Lesereihenfolge

1. [01 Quellenlage und Methodik](01-Quellenlage-und-Methodik.md): was gelesen wurde, was fehlt und wie Aussagen eingestuft sind.
2. [02 Vision und Grundprinzipien](02-Vision-und-Grundprinzipien.md) und [29 Konflikte und offene Fragen](29-Konflikte-und-offene-Fragen.md): die Grundhaltung und was ungeklärt ist.
3. Die Fachmodule nach Interesse (Tabelle unten).
4. [31 Testkatalog, Ausbaustufen und Abnahme](31-Testkatalog-Phasen-und-Abnahme.md) als Prüfhilfe.
5. [28 Register](28-Entscheidungsregister.md) und [30 Abdeckung](30-Abdeckungsprotokoll.md) als Nachschlagewerke.

## Die Module

| Nr. | Datei | Inhalt |
|---|---|---|
| 01 | [Quellenlage und Methodik](01-Quellenlage-und-Methodik.md) | Quellen, Schlüssel, Statuswörter, Lesenachweis, fehlende Quellen |
| 02 | [Vision und Grundprinzipien](02-Vision-und-Grundprinzipien.md) | Leitsätze, drei Verantwortungsbereiche, Eigentümer je Thema |
| 03 | [Fahrzeugklassen und Fahrzeugdaten](03-Fahrzeugklassen-und-Fahrzeugdaten.md) | Verkehrsrolle, Typ und Exemplar |
| 04 | [Fahrphysik und Fahrverhalten](04-Fahrphysik-und-Fahrverhalten.md) | Schichtung, Straße und Wetter als Eingaben, NPC-Physik |
| 05 | [Steuerung, Kamera und Anzeige](05-Steuerung-Kamera-und-Anzeige.md) | DualSense-Vorgabe, Lücken, Prüfplan L1 + L2 |
| 06 | [Einsteigen, Aussteigen, Sitze und Kofferraum](06-Einsteigen-Aussteigen-Sitze-und-Kofferraum.md) | Zugangszustände, Besatzung, Inventar |
| 07 | [Zustand, Verschleiß und Schaden](07-Zustand-Verschleiss-und-Schaden.md) | Drei Schichten, Optionen für den Schadensdetailgrad |
| 08 | [Reparatur, Wartung und Werkstätten](08-Reparatur-Wartung-und-Werkstaetten.md) | Werkstattauftrag, Akteure, Bergung |
| 09 | [Kraftstoff, Energie und Tankstellen](09-Kraftstoff-Energie-und-Tankstellen.md) | Energiezustand, Tankstellen, Ladeorte, Rastplätze |
| 10 | [Straßennetz und Straßentypen](10-Strassennetz-und-Strassentypen.md) | Klassen, Straßenprofil, Autobahn, Land- und Bergstraßen, Brücken |
| 11 | [Kreuzungen, Signale und Schilder](11-Kreuzungen-Signale-und-Schilder.md) | Kreuzungsdaten, Vorfahrt, Signalprogramme |
| 12 | [Stadtverkehr, Parken und Garagen](12-Stadtverkehr-Parken-und-Garagen.md) | Parkmodell, Depots, Valet (unbelegt) |
| 13 | [Verkehrs-KI und Navigation](13-Verkehrs-KI-und-Navigation.md) | Schichten, Routenwahl, Fahrstile, Verhaltenskatalog |
| 14 | [NPC-Fahrer und NPC-Fahrzeuge](14-NPC-Fahrer-und-NPC-Fahrzeuge.md) | Fahrzeuge je Persistenzstufe, Fahrten, Verkehrsmenge |
| 15 | [Verkehrsdichte, Stau, Baustellen und Sperrungen](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md) | Dichteprofil, Stau, Baustellen, Meldungen |
| 16 | [Unfälle, Pannen, Einsatz und Sonderverkehr](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md) | Unfallereignis, Einsatzkette, Treffen und Straßenrennen |
| 17 | [Fußgänger, Radfahrer und weitere Teilnehmer](17-Fussgaenger-Radfahrer-und-weitere-Teilnehmer.md) | Straßenzugang, Fußgänger, Zweiräder, Tiere |
| 18 | [Besitz, Zulassung und Diebstahl](18-Besitz-Zulassung-und-Diebstahl.md) | Eigentumsfakten, Kauf, Versicherung, Kennzeichen |
| 19 | [Polizei und Verkehrsüberwachung](19-Polizei-und-Verkehrsueberwachung.md) | Wahrnehmer, Kontrolle, Verfolgung |
| 20 | [Interaktionen, Transport und Jobs](20-Interaktionen-Transport-und-Jobs.md) | Transportaufträge, Fahrgäste, Fracht |
| 21 | [Fahrzeuge in Ermittlungen und Jagden](21-Fahrzeuge-in-Ermittlungen-und-Jagden.md) | Spuren, Fallablauf, Jagd, Ebenenregeln |
| 22 | [Persistenz und Lebenszyklen](22-Persistenz-und-Lebenszyklen.md) | Segmente, Lebenszyklus, Speichern im Verkehr |
| 23 | [Koop und Netzwerk](23-Koop-und-Netzwerk.md) | Anforderungen, Autoritätsoptionen, Besitz im Koop |
| 24 | [WorldForge-Schnittstelle](24-WorldForge-Schnittstelle.md) | Abgleich `RC-01`, Kandidatenfelder, Überlagerung, Validierung |
| 25 | [Unreal-Integration](25-Unreal-Integration.md) | Aufteilung Kern und Adapter, Kandidaten zur Prüfung |
| 26 | [Simulationsstufen, LOD und Streaming](26-Simulationsstufen-LOD-und-Streaming.md) | Stufen für Fahrzeuge, Materialisierung, Streaming |
| 27 | [Performance und Skalierung](27-Performance-und-Skalierung.md) | Kostentreiber, Maßnahmen, Messplan |
| 28 | [Entscheidungsregister](28-Entscheidungsregister.md) | erzeugtes Register aller Aussagen |
| 29 | [Konflikte und offene Fragen](29-Konflikte-und-offene-Fragen.md) | `CK`, `CU`, `CQ` mit Freitextoption, Klärungsreihenfolge |
| 30 | [Abdeckungsprotokoll](30-Abdeckungsprotokoll.md) | Themenabdeckung, Quellen, Prüfungen, Integration |
| 31 | [Testkatalog, Ausbaustufen und Abnahme](31-Testkatalog-Phasen-und-Abnahme.md) | Tests nach 14 Kategorien, Szenarien, Ausbaustufen |

## Zuordnung zu den Themen des Auftrags

Der Auftrag nannte 35 Themen in fünf Bereichen. Die Gliederung wurde nach den Quellen zusammengelegt (verteilt über 27 Fachmodule); kein Thema entfällt. Die Tabelle in [Modul 30](30-Abdeckungsprotokoll.md) §2 nennt zu jedem Thema Modul und Befund.

| Bereich des Auftrags | Module |
|---|---|
| A Fahrzeuggrundlagen (Themen 1 bis 8) | 02 bis 09 |
| B Verkehrsnetz (9 bis 15) | 09, 10, 11, 12 |
| C Dynamischer Verkehr (16 bis 22) | 13 bis 17 |
| D Gameplay und Weltintegration (23 bis 29) | 18 bis 23 |
| E Technische Architektur (30 bis 35) | 24 bis 27, 31 |

## Zuordnung zu den Nachbardokumenten

| Thema | Dieser Ordner | Nachbarordner |
|---|---|---|
| Fahrzeugklassen, Motorsport, Fahrphysik (Grundlagen) | Module 03 bis 05 | [Welt 13, 14, 15](../World/README.md) |
| Straßennetz als Karte, ÖPNV, Bahn, Streaming | Module 10, 24, 26 | [Welt 09, 17, 18](../World/README.md) |
| NPC-Identität, Tagesabläufe, Stufen, Persistenz | Module 14, 22, 26 | [NPCs 03, 13, 21, 22](../NPCs/README.md) |
| Spuren, Zeugen, Polizei, Haft | Module 19, 21 | [Ermittlung 05, 06, 14, 16](../Ermittlung/README.md) |
| Schießen aus Fahrzeugen, Unfallverletzung, Rettung | Module 07, 16 | [Kampfsystem 05, 13, 14](../Kampfsystem/README.md) |
| Wesen mit Identität, Ebenen mit eigenen Regeln | Modul 21 | [Kreaturen 07, 09](../Kreaturen/README.md) |
| Weltzustand, Speicherformat | Modul 22 | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) |
| Unreal-Integration, Kern | Modul 25 | [Decision 0006](../../Decisions/0006-unreal-core-integration.md) |

## Werkzeuge

Das Register (28) und die Statistik in Modul 30 werden erzeugt; die Prüfungen laufen mit den Hilfsskripten in `Tools/world-audit/` (siehe dort `README.md`):

```text
python3 -I Tools/world-audit/render_vehicle_register.py Docs/Game/Fahrzeuge-und-Verkehr
SPN_GENERATED=28- SPN_MASTER=Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27 SPN_WORLD_DOCS=Docs/Game/Fahrzeuge-und-Verkehr SPN_ID_PREFIX=VEH-DEC SPN_REGISTER_FILE=28-Entscheidungsregister.md python3 -I Tools/world-audit/check_docs.py <Arbeitsordner mit Chat-JSON>
python3 -I Tools/world-audit/check_links.py Docs/Game
```

## Was dieser Ordner nicht tut

- Er legt **keine** Zahlen, Verkehrsregeln, Fahrzeuglisten, Marken, Schadensmodelle, Netzwerkarchitekturen oder Austauschformate fest.
- Er ändert **keinen** bestehenden Kanon, keine Master-Datei, keinen Code und keine WorldForge-Datei.
- Er führt **keine** Tests aus; der Katalog beschreibt künftige Prüfungen.
- Er erzeugt **keine** zweite NPC-Architektur: Verkehr nutzt die Persistenz- und Simulationsstufen der NPC-Dokumentation.
