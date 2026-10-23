# NPC-Dokumentation: Index

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieser Ordner dokumentiert den **Planungsstand** von NPCs, Gedächtnis, Beziehungen, Nemesis/Rivalität, autonomem Leben, Fraktionen und Weltpersistenz aus dem externen Planungs-Master, den Gesprächsverläufen und den Entwickleraussagen. Er erfindet keine Entscheidungen, keine Figuren, keine Fraktionen und keine Zahlen. **Verbindlich ist die Produktvision vom 22.10.2026** ([Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §1, Anforderungen A–H); alle Datenmodelle, Verläufe, Rechenbeispiele und Prüfregeln sind **Entwurf (💡)**. Der Gesamtumfang liegt bei etwa 10.100 Zeilen, davon rund 1.650 aus den Modulen erzeugte Register- und Testtabellen; die **von Hand geschriebenen, substanziellen Zeilen** (rund 8.500) liegen unter dem langfristigen Zielbereich von 10.000 bis 15.000. Genaue Zahlen: [Modul 28](28-Abdeckungsprotokoll.md) §5.

Teil der Spiel-Dokumentation: [Spielvision](../Spielvision.md) (Überblick §6, §9, §10), [Gameplay-Systeme](../Gameplay-Systeme.md) (§8), [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) (§13), [Entscheidungsstatus](../Entscheidungsstatus.md), [Ermittlungs-Dokumentation](../Ermittlung/README.md), [Kreaturen-Dokumentation](../Kreaturen/README.md), [Welt-Dokumentation](../World/README.md), [Adult-System](../Adult-System.md).

## Statuslegende

| Zeichen | Bedeutung |
|---|---|
| ✅ | implementiert und getestet (in diesem Ordner nirgends) |
| 📋 | bestätigt: vom Entwickler festgelegt oder im Master als akzeptiert geführt, nicht implementiert |
| 🧭 | vorläufig: Arbeitsstand, Szenario, Kandidat |
| 💡 | Vorschlag oder Beispiel des Assistenten oder dieser Dokumentation, nicht beschlossen |
| 🔭 | langfristige Vision ohne Spezifikation |
| ❓ | offen: gestellt, nicht beantwortet |
| 🕰️ | historisch ersetzt: bleibt als Geschichte stehen |
| ⚠️ | widersprüchlich: keine Ersetzung belegt |

„📋 Richtung“ bedeutet: das Prinzip ist bestätigt, Einzelheiten (Namen, Schwellen, Zahlen) sind es nicht. Das gilt besonders für die Sammelantwort im Block 9C ([Modul 01](01-Quellenlage-und-Methodik.md) §3 Regel 7).

## Die acht Produktanforderungen (22.10.2026)

| | Anforderung | Schwerpunkt-Module |
|---|---|---|
| A | Persistente NPC-Identitäten | [03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md), [04](04-Persoenlichkeit-Zustaende-und-Zeitskalen.md) |
| B | Langzeitgedächtnis | [05](05-Gedaechtnis-Erinnerung-und-Vergessen.md), [06](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md), [07](07-Informationsfluss-Geruechte-und-Kommunikation.md) |
| C | Nemesis und Rivalität | [11](11-Nemesis-Modell-und-Datensatz.md), [12](12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md) |
| D | Autonome Fraktionen und Organisationen | [16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md), [17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md), [18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md) |
| E | Persistente Konsequenzen | [14](14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md), [15](15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md) |
| F | Unabhängige Lebensentwicklung | [09](09-Beziehungsentwicklung-Freundschaft-Feindschaft-und-Vertrauen.md), [10](10-Familien-Haushalte-und-soziale-Netzwerke.md), [13](13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md) |
| G | Skalierbare Weltsimulation | [21](21-Weltpersistenz-und-Speicherentwurf.md), [22](22-Simulationsstufen-Skalierung-und-Performance.md) |
| H | Integration ohne widersprüchliche Zustände | [24](24-Integration-und-Konsistenzregeln.md), [25](25-Querschnittsszenarien.md) |

## Leseempfehlung

1. [Modul 01](01-Quellenlage-und-Methodik.md): Quellen, Zitierschlüssel, Statusregeln, die drei Achsen (Persistenz, Simulation, Datentiefe).
2. [Modul 02](02-Leitsaetze-und-Abgrenzung.md): die acht Anforderungen und die Leitsätze aus den Planungsrunden.
3. Danach nach Interesse; jedes Modul beginnt mit einer Tabelle ‚Was beschlossen ist‘ mit Quelle und Status, danach folgen Entwurfsabschnitte (💡) und Prüfregeln.
4. [Modul 25](25-Querschnittsszenarien.md): zwölf Abläufe, die die Module zusammenführen.
5. [Modul 27](27-Konflikte-und-offene-Fragen.md): was noch entschieden werden muss (zuerst Abschnitt 7).

## Module

| Nr. | Dokument | Inhalt |
|---|---|---|
| 01 | [Quellenlage und Methodik](01-Quellenlage-und-Methodik.md) | Schlüssel, Statusregeln, Master-Konsultation, drei Achsen, Grenzen |
| 02 | [Leitsätze und Abgrenzung](02-Leitsaetze-und-Abgrenzung.md) | Anforderungen A–H, Leitsätze, Nicht-Ziele, Umfang, Sicht des Spielers |
| 03 | [Identität, Persistenzstufen, Datensatztiefe](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md) | ID, Seed, Stufen A–D, Datensatztiefe, Lebenszustand, geschützte NPCs |
| 04 | [Persönlichkeit, Zustände, Zeitskalen](04-Persoenlichkeit-Zustaende-und-Zeitskalen.md) | Kern, schnelle/mittlere/lange Zustände, Ereigniswirkung, Kommunikationsstil |
| 05 | [Gedächtnis, Erinnerung, Vergessen](05-Gedaechtnis-Erinnerung-und-Vergessen.md) | Erinnerungsarten, Eintrag, Verblassen, Verdichten, Wiedererkennung, Szenarien |
| 06 | [Wahrnehmung, Wissen, Glaube, Unsicherheit](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md) | Wissensgrade, Verdacht, Zeugen, falsche Überzeugungen, Übernatürliches |
| 07 | [Informationsfluss, Gerüchte, Kommunikation](07-Informationsfluss-Geruechte-und-Kommunikation.md) | Kanäle, Gerüchteobjekt, Verzerrung, Kreise, Medien |
| 08 | [Beziehungen: Modell und Wirkung](08-Beziehungen-Modell-und-Wirkung.md) | Kante, Dimensionen, Typen, Stufen, Aggregation, Verhaltensbänder, Gruppenruf |
| 09 | [Beziehungsentwicklung](09-Beziehungsentwicklung-Freundschaft-Feindschaft-und-Vertrauen.md) | Ereigniskatalog, Reaktionsbilder, Freundschaft, Feindschaft, Versöhnung, Schuld |
| 10 | [Familien, Haushalte, soziale Netzwerke](10-Familien-Haushalte-und-soziale-Netzwerke.md) | Haushalt, Verwandtschaft, Kreise, Orte, Familienereignisse, Generationen (vorbereitet) |
| 11 | [Nemesis: Modell und Datensatz](11-Nemesis-Modell-und-Datensatz.md) | Begriffe, Eignung, Datensatz, gelerntes Modell, Ziele, Grenzen, Lernweg |
| 12 | [Nemesis: Lebenszyklus](12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md) | Ausgänge, Verletzungen, Erholung, Rückkehr, Anpassung, Wendepunkte, Tod |
| 13 | [Autonomes Leben](13-Autonomes-Leben-Tagesablaeufe-Arbeit-Freizeit-und-Reisen.md) | Zeitplan, Orte, Abweichungen, Entscheidung, Reisen, Arbeit, Hintergrundzustände |
| 14 | [Lebensereignisse, Alterung, Tod](14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md) | Ereigniskatalog, Alterung, äußere Veränderung, Verletzung, Umzug, Beruf, Tod |
| 15 | [Reaktion auf Weltereignisse](15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md) | Reaktionsmodell, Gemeinschaften, Übernatürliches, veränderte NPCs, Meidung, Reparatur |
| 16 | [Fraktionen: Modell, Mitgliedschaft, Hierarchie](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md) | Datensatz, Typen, Ränge, Mitglieder, Spieler und Fraktionen, Innenpolitik |
| 17 | [Fraktionen: Territorien, Konflikte, Macht](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md) | Territorium, Ressourcen, Strategiezustände, Konflikt, Machtverschiebung |
| 18 | [Behörden, Hunter-Organisationen, Wesen](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md) | Akteursmodelle, Beamte, NPC-Hunter, Wesen unter Menschen, Wissensverteilung |
| 19 | [Begleiter, Crew, Rekrutierung](19-Begleiter-Crew-und-Rekrutierung.md) | Crew-Figur, Einsatz, Tod, HQ-Alltag, Hund, Rekrutierung, Integration |
| 20 | [Dialog und Sichtbarkeit](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md) | Dialogkontext, Bedingungen, Dialogtiefe, Hooks, Signalpalette, Barks, Inspektoren |
| 21 | [Weltpersistenz und Speicherentwurf](21-Weltpersistenz-und-Speicherentwurf.md) | Segmente, Lebenszyklus, Journal, Größenrechnung, Migration, Integrität |
| 22 | [Simulationsstufen, Skalierung, Performance](22-Simulationsstufen-Skalierung-und-Performance.md) | S0–S3, Fortschreiben, Materialisierung, Übergänge, Budgets, Determinismus |
| 23 | [Koop, Host-Speicherstand, Mods](23-Koop-Host-Speicherstand-und-Mods.md) | Autorität, Subjekte, Teamentscheidungen, Mod-Erweiterungspunkte |
| 24 | [Integration und Konsistenzregeln](24-Integration-und-Konsistenzregeln.md) | Besitzer je Tatsache, Ereignisse, Invariantenkatalog, Querwirkungen |
| 25 | [Querschnittsszenarien](25-Querschnittsszenarien.md) | zwölf Abläufe über mehrere Module |
| 26 | [Entscheidungsregister](26-Entscheidungsregister.md) | erzeugt aus den Modulen: Aussagen mit Status und Quelle (Kennungen `N-…`) |
| 27 | [Konflikte und offene Fragen](27-Konflikte-und-offene-Fragen.md) | Konflikte C-N, unklare Antworten U-N, offene Fragen Q-N, Begriffskollisionen |
| 28 | [Abdeckungsprotokoll](28-Abdeckungsprotokoll.md) | gelesene Quellen, Abdeckungsgrad, Zeilenumfang, fehlende Themen, Hinweise für globale Indizes |
| 29 | [Testkatalog und Abnahmekriterien](29-Testkatalog-und-Abnahmekriterien.md) | abgeleitete Tests und Prüfregeln, Messungen, Abnahme nach Ausbaustufe |

## Was dieser Ordner nicht tut

- Er legt **keine benannten Figuren, Fraktionen oder Antagonisten** fest und keine Crew-Besetzung (Proposal 0010 §12).
- Er legt **keine Zahlen, Budgets oder Schwellen** fest; Rechenbeispiele sind als Annahmen markiert.
- Er ändert **nicht** den Master, `WorldFacts` v1, das Speicherformat oder bestehende Überblicksdokumente.
- Er behandelt **nicht** die sexuellen und romantischen Inhalte (siehe [Adult-System](../Adult-System.md)), nur das gemeinsame Beziehungsgerüst.
- Er entscheidet **keinen** Konflikt zwischen Quellen ([Modul 27](27-Konflikte-und-offene-Fragen.md)).

## Werkzeuge

| Werkzeug | Aufgabe |
|---|---|
| [`Tools/world-audit/check_docs.py`](../../../Tools/world-audit/README.md) | Zitate, wörtliche Zitate, Quellenschlüssel, Registerkennungen (für diesen Ordner mit `SPN_WORLD_DOCS=Docs/Game/NPCs SPN_ID_PREFIX=N SPN_REGISTER_FILE=26-Entscheidungsregister.md`) |
| `Tools/world-audit/check_links.py` | relative Links und Anker |
| `Tools/world-audit/render_npc_register.py` | erzeugt [Modul 26](26-Entscheidungsregister.md) aus den Tabellen der Module 02 bis 24 und prüft Eindeutigkeit und Verweise |
| `Tools/world-audit/render_npc_tests.py` | erzeugt den Regeltestteil von [Modul 29](29-Testkatalog-und-Abnahmekriterien.md) aus den Prüfregeln |

Vorbereitung der Quellen für die Prüfung: `python3 Tools/world-audit/split_chats.py <Master> <Arbeitsordner> 23 35 48 19` (nur Lesen im Master).
