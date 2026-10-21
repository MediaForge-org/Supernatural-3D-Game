# Adult 25: Spezifikation Tattoos, Piercings und Körpermodifikation

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register, Abschnitt TA](14-Entscheidungsregister.md#10-ta-tattoos-piercings-körpermodifikation)). Kein Kanon. **Zuordnung:** Der Tattoo-Editor, Studios und Körpermodifikation gehören zum **Hauptspiel** (E-TA-015), nicht zum Erwachsenen-Paket; das Paket ergänzt nur intime Detailoptionen. Alle Aussagen betreffen **Erwachsene**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachlich: [Modul 07](07-Charakteranpassung-Tattoos-Koerpermodifikation.md). Daten: [Modul 21, Abschnitt 9](21-Spezifikation-Datenmodell.md#9-tattoo--und-körpermodifikationsdaten).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Tattoo als persistentes Objekt](#3-tattoo-als-persistentes-objekt)
4. [Platzierung, Abdeckung, Komposition](#4-platzierung-abdeckung-komposition)
5. [Stile und Motive](#5-stile-und-motive)
6. [Künstler](#6-künstler)
7. [Studios, Preise, Termine, Internet](#7-studios-preise-termine-internet)
8. [Prozess, Heilung, Alterung](#8-prozess-heilung-alterung)
9. [Cover-up, Laser, Veränderung](#9-cover-up-laser-veränderung)
10. [Körperveränderung und Deformation](#10-körperveränderung-und-deformation)
11. [Piercings und Körpermodifikation](#11-piercings-und-körpermodifikation)
12. [Kosmetische Eingriffe und Kliniken](#12-kosmetische-eingriffe-und-kliniken)
13. [Bevölkerung und Verteilung](#13-bevölkerung-und-verteilung)
14. [Editor, Import, Mods](#14-editor-import-mods)
15. [Rendering und Leistung](#15-rendering-und-leistung)
16. [Symbolik, Recht, Plattform](#16-symbolik-recht-plattform)
17. [Technische Komponenten](#17-technische-komponenten)
18. [Randfälle](#18-randfälle)
19. [Tests und Abnahmekriterien](#19-tests-und-abnahmekriterien)
20. [Ungeklärtes](#20-ungeklärtes)

---

## 1. Ziel und Abgrenzung

Der Entwickler wünscht Tattoos **extrem detailliert** (Ziel: etwa 90 bis 95 Prozent des Planungsumfangs der Runde 17; 35#590) und viele NPCs mit vielen Tattoos (E-TA-001). Alles folgt der Grundregel: **Alles, was nicht eigens festgelegt ist, verhält sich realistisch** (E-TA-026, E-SK-033). Tattoos sind kein Erwachsenen-Feature; sie sind Teil von Charaktererstellung, Alltag und Welt (E-TA-015). Das Alter ist Voraussetzung für alle Studio-Dienstleistungen (Erwachsene; siehe [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) Abschnitt 4).

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Tattoos auf jeder Körperstelle, 0 % bis praktisch 100 % | 📋 | E-TA-004 |
| Tattoo-Editor und Nacktheit im Hauptspiel | 📋 | E-TA-015 |
| Realistischer Lebenszyklus (frisch → Heilung → stabil → Verblassen → Auffrischen/Cover-up/Laser) | 📋 / 🧭 | E-TA-006, E-TA-007 |
| Künstlerqualität variiert, Preise variieren, Standort beeinflusst Preis | 📋 | E-TA-006, E-TA-019 |
| Wartezeit 1–2 Spielwochen, nicht länger | 📋 | E-TA-019 |
| Zeitraffer statt 5 Stunden Echtzeit | 📋 | E-TA-020 |
| Eigene Bilder importieren ist **Pflichtfeature** | 📋 | E-TA-024 |
| Selbsterklärende reale Tattoo-Logik gilt automatisch | 📋 | E-TA-021 |
| Baseline-Prozentzahlen der Verteilung | 🧭 (Assistent) | E-TA-023 |
| Tattoo-Rendering-Technik | 🧭 (offen: „wie wir das machen, weiß ich nicht“) | E-TA-018 |
| Extremistische Symbole | ❓ | [K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik) |
| Studioanzahl in der Metropole ‚20–30 oder mehr‘ | 🧭 | E-TA-005 |

## 3. Tattoo als persistentes Objekt

📋 Jedes Tattoo bleibt im Spielzustand ein **eigenes Objekt** (E-TA-014), wird aber zum Rendern zu optimierten Körperschichten zusammengefasst (Frage 113, E-TA-024).

🧭 Felder (aus E-TA-014):

| Feld | Bedeutung |
|---|---|
| `id` | stabile Kennung |
| `motif_ref` | Motiv (Registry oder importiertes Design) |
| `style_tags` | Stilmerkmale (kombinierbar) |
| `placement` | Oberflächenkoordinaten, Rotation, Größe |
| `coverage` | Anteil/Region (kontinuierlich) |
| `layers[]` | Schichten, Überlappung, Reihenfolge, Verlauf |
| `pigments[]` | Farben, Pigmenteigenschaften (haut- und lichtabhängig) |
| `artist_id`, `studio_id` | Herkunft |
| `session_log[]` | Sitzungen mit Datum/Dauer |
| `quality` | Ausführungsqualität (Linie, Schatten, Farbe, Fehler) |
| `age`, `lifecycle_state`, `fade` | Lebenszyklus |
| `meaning` | Bedeutung für Figur (Erinnerung, Gruppe, Partner, Reue) |
| `composition_id` | Zugehörigkeit zu Sleeve/Patchwork/Ganzkörperplan |

Alle Felder sind Vorschläge; das Format ist offen.

## 4. Platzierung, Abdeckung, Komposition

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-PL-01 | Freie Platzierung auf **jeder** Körperstelle (auch Gesicht, Hände, Füße, Hals, Mundinnenseite; Augen sehr selten). | E-TA-004 |
| TA-PL-02 | Abdeckung **kontinuierlich** von 0 % bis ~100 % (‚bis auf den letzten Millimeter‘); Anteil pro Region. | E-TA-004 |
| TA-PL-03 | Überlappung mit Schichtenverlauf (Layer History); Negativraum; Füllmotive für Lücken. | E-TA-014 |
| TA-PL-04 | Sleeves entstehen aus einzelnen Motiven **oder** einem Gesamtentwurf; Patchwork möglich. | E-TA-004, E-TA-014 |
| TA-PL-05 | Ganzkörperprojekte erstrecken sich über mehrere Sitzungen und Jahre; Masterplan erlaubt. | E-TA-014 |
| TA-PL-06 | Symmetrie ist bewusst wählbar; Anatomie beeinflusst Designanpassung. | E-TA-014 |
| TA-PL-07 | Mehrere Künstler und Stile an einer Figur; Mischung sichtbar. | E-TA-004 |
| TA-PL-08 | Tattoo-Dichte ist ein **NPC-Merkmal**; sie verändert sich über das Leben. | E-TA-014, E-TA-002 |
| TA-PL-09 | Narben und Hautzustand beeinflussen Aussehen; Tattoo über Narben möglich. | E-TA-014 |
| TA-PL-10 | Vorschau am eigenen Körper (Größe, Platzierung, Wirkung neben anderen Tattoos) vor dem Stechen. | E-TA-017 |

## 5. Stile und Motive

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-ST-01 | Stile sind **kombinierbare Merkmale**, nicht ein Dropdown: Realism, Black & Grey, Farbe (Pigmente je Hautton), Blackwork, Blackout (eigener Typ), White-over-Black, Traditional/Neo-Traditional, Japanese (zusammenhängende Komposition), Fine Line, Geometrisch/Ornamental, Lettering, Porträts, Custom. | E-TA-017 |
| TA-ST-02 | **Motive sind praktisch frei** („wirklich alles soll tätowierbar sein“); der Spieler beschreibt den Wunsch, der Künstler macht die Vorlage; Interpretation vs. genaue Vorlage. | E-TA-017 |
| TA-ST-03 | Schrift: Fehler (Tippfehler) bei schlechten Künstlern. | E-TA-017 |
| TA-ST-04 | Porträts (auch nach Referenzbild/NPC) möglich. | E-TA-017 |
| TA-ST-05 | Fehlerbilder bei geringer Qualität: wackelige Linien, Ausbluten (Blowouts), ungleichmäßige Farbe. | 35#762 (Vorschlag) |
| TA-ST-06 | Schablone/Freihand; spontane Tattoos möglich (‚meist mit Termin‘). | E-TA-006 |
| TA-ST-07 | Motiv-Wiederverwendung (Vorlagen werden häufiger genutzt). | E-TA-024 |
| TA-ST-08 | Blackout ist möglich, ‚aber nicht schön‘ (Entwickler 35#761/21). | E-TA-017 |

## 6. Künstler

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-KN-01 | `ArtistProfile` mehrdimensional: Linienführung, Schattierung, Farbe, Komposition, Realismus, Blackwork, Cover-up, Narbenabdeckung, Tempo, Konstanz, Hygiene, Erfahrung, Ruf. | E-TA-008 |
| TA-KN-02 | Spezialisierung und **Handschrift**; gute Künstler sind nicht in jedem Stil gut. | E-TA-008, E-TA-019 |
| TA-KN-03 | Karriereleiter (Lehrling → Profi → Weltklasse); Weltklasse ist teuer und ausgebucht. | E-TA-019 |
| TA-KN-04 | Schlechte Künstler (z. B. im Gefängnis) stechen schlechter; Ergebnis sichtbar. | E-TA-006 |
| TA-KN-05 | Ein Tattoo kann von mehreren Künstlern nacheinander fertiggestellt werden; sichtbar. | Modul 07 |
| TA-KN-06 | Empfehlungen unter NPCs, Portfolios, berühmte Kunden. | E-TA-019 |
| TA-KN-07 | Ein **virtueller Künstler** erzeugt aus Wunsch + Stil + Skill einen Entwurf (Komposition zuerst). | E-TA-024 |

## 7. Studios, Preise, Termine, Internet

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-SD-01 | Studiotypen: Walk-in, renommiert mit Warteliste, Gefängnis, privat. | E-TA-008 |
| TA-SD-02 | Metropole: sicher 20–30 Studios oder mehr; Dörfer wenige; große, berühmte Studios. | E-TA-005, E-TA-022 |
| TA-SD-03 | **Preis** hängt von Künstler, Größe, Aufwand, Stil und **Standort** ab (Innenstadt viel teurer als Land); Preis ≠ Qualität. | E-TA-019 |
| TA-SD-04 | Termin über Website/Telefon/Walk-in; Beratung, Anzahlung; **Wartezeit 1–2 Spielwochen**. | E-TA-019 |
| TA-SD-05 | Jedes Studio hat eigene Regeln, Hygiene (getrennt von Skill), Reputation. | E-TA-019 |
| TA-SD-06 | Studios können scheitern, schließen, Konkurrenz und Trends beeinflussen sie. | E-NP-028 / 35#539 |
| TA-SD-07 | Spezialisierungen (Blackout, Farbe …) beeinflussen Standort und Kundschaft. | E-TA-022 |
| TA-SD-08 | Studio-Websites mit Portfolios, Preisen, Terminen, Bewertungen; Buchung im funktionalen Internet. | E-TA-009 |
| TA-SD-09 | Spieler kann Webseiten bauen (stark vereinfacht, eigener Website-Builder in C++, nicht Unreal-Blueprint). | E-TA-009 |
| TA-SD-10 | Nagelstudios (Hand/Fuß), Nagellack im Supermarkt: eigene Dienstleistung. | E-TA-012 |

## 8. Prozess, Heilung, Alterung

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-PH-01 | Große Stücke entstehen in **mehreren Sitzungen**; Sitzungen sind nicht zwingend, aber fast immer. | E-TA-020 |
| TA-PH-02 | Die Sitzung wird als **Zeitraffer** erlebt. | E-TA-020 |
| TA-PH-03 | Schmerz je Körperstelle; Pausen. | E-TA-020 |
| TA-PH-04 | Heilungsphasen: frisch, schorfend/schuppend, stabil; Aftercare; Kleidungskontakt, Wasser, Sonne wirken ein. | E-TA-020 |
| TA-PH-05 | Individuelles Heilen; unvollkommene Heilung möglich; Touch-up (mit Preis). | E-TA-020 |
| TA-PH-06 | Verblassen erst nach **Jahren**; UV beschleunigt; Farben altern verschieden; Haut verrunzelt. | E-TA-006, E-TA-020 |
| TA-PH-07 | Auffrischen früh nach Jahren statt nach zwei Wochen. | Modul 07 |
| TA-PH-08 | `Tattoo-History` pro Objekt. | E-TA-020 |

## 9. Cover-up, Laser, Veränderung

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-CL-01 | Cover-up über bestehende Motive; Erfolg hängt von Künstler und Altmotiv ab. | E-TA-006, E-TA-021 |
| TA-CL-02 | Laser: mehrere Sitzungen, nie perfekt, Vernarbung möglich. | E-TA-006 |
| TA-CL-03 | Blackout-Cover als Option. | E-TA-014 |
| TA-CL-04 | Überstechen von Narben. | E-TA-006 |
| TA-CL-05 | **Reue**: Cover-up/Laser als Ergebnis von Persönlichkeit und Lebensumständen. | E-TA-006 |
| TA-CL-06 | Partnernamen und Bedeutungen können Anlass zu Cover-up werden. | 35#500/98 |

## 10. Körperveränderung und Deformation

Tattoos folgen dem Körper (E-TA-014, E-SO-027):

| Ursache | Wirkung |
|---|---|
| Gewichtszunahme/-abnahme | sichtbare Verzerrung |
| Schwangerschaft | Bauchtattoos dehnen sich; ein während der Schwangerschaft gestochenes verzerrt nach Rückbildung |
| Muskelaufbau/-abbau | Form und Spannung ändern sich |
| Alter | Verblassen, Hautfalten |
| Narben | Überlagerung |
| Kosmetische Eingriffe | Verschiebung oder Verlust |

Technisch muss die Körperoberflächenabbildung **verformbar** sein (UV/Body-Space), damit Tattoos mitwandern (E-TA-018).

## 11. Piercings und Körpermodifikation

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-PC-01 | Piercings an jeder **plausiblen** Stelle, viele pro Figur, persistent; Intimpiercings sind normale Körpermodifikationskategorie. | E-TA-010, E-TA-025 |
| TA-PC-02 | Schmuck austauschbar: Form, Größe, Material, Gewicht, Zustand; Materialunverträglichkeit. | E-TA-010 |
| TA-PC-03 | Heilungszustand, Komplikationen, Narben. | E-TA-010 |
| TA-PC-04 | Physik/Kleidung: Hängenbleiben und Verletzung **selten**; Entfernen vor Sport/Kampf. | E-TA-010 |
| TA-PC-05 | Dehnung (Tunnel, Plugs) persistent. | E-TA-010 |
| TA-PC-06 | Seltene Modifikationen (gespaltene Zunge, Scarification, Branding, subdermale Implantate): selten und subkulturabhängig, Spezialkünstler. | E-TA-010, E-TA-025 |
| TA-PC-07 | Studio-Reputation persistent; Internet-Buchung. | E-TA-010 |
| TA-PC-08 | Piercings im äußeren Genitalbereich plausibel; innere vaginale Piercings sind **keine Standardkategorie**; nicht beliebige Stellen. | E-KO-025 |
| TA-PC-09 | Der Editor bietet Intim-Detailoptionen nur bei aktivem Erwachsenen-Paket. | Modul 07 |
| TA-PC-10 | Billig ≠ schlecht; teuer kann ebenfalls misslingen. | 35#512/109 |

## 12. Kosmetische Eingriffe und Kliniken

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-KL-01 | Kosmetische Chirurgie (z. B. Brust, Gesäß, Fettabsaugung, Nase/Gesicht) als **Weltmechanik** mit Kliniken, Ärzten, Terminen. | E-TA-027, E-TA-010 |
| TA-KL-02 | Ein Eingriff ändert Generatorparameter, hat Erholung, Arztqualität, Revisionsmöglichkeit. | E-TA-010 |
| TA-KL-03 | NPCs entscheiden aus Persönlichkeit, Geld, Umfeld, Körperbild. | E-TA-010 |
| TA-KL-04 | Gemeinsames Aussehens-/Körperzustandssystem (Tattoos, Eingriffe, Gewicht). | E-TA-010 |
| TA-KL-05 | Eingriff beeinflusst Tattoos (Verschiebung/Verlust) und Physik. | Modul 07 |

## 13. Bevölkerung und Verteilung

### 13.1 Aussagen des Entwicklers (📋)

- Auf dem Land selten; bei älteren Menschen, die nicht im Gefängnis waren, selten; bei **jungen Erwachsenen** modern und häufiger; in der Stadt mehr; wenige Ganzkörperanzüge („tattoo-süchtige NPCs“) (E-TA-016).
- Gruppen wie Biker, Gangs, Mafia, Yakuza haben eigene Symbolsprachen (E-TA-017).
- In der Metropole nicht einer, sondern **mehrere** Ganzkörperfälle (E-TA-022).
- Je mehr Tattoos, desto seltener zufällig, außer an passenden Orten (E-TA-009).

### 13.2 Baseline-Prozentzahlen (🧭 Assistent, 35#776, vom Entwickler mit „passt“ bestätigt)

Anteil der **erwachsenen Bevölkerung** nach Tätowierungsgrad. Es sind **Spawn-/Populationsgewichte, keine harten Quoten**; sie sind nicht ausdrücklich als Zahlen bestätigt (E-TA-023, [K20](19-Konflikt-und-Widerspruchsmatrix.md#k20-tattoo-prozentverteilung)).

| Gebiet | 0 % | 1–10 % | 10–30 % | 30–60 % | 60–90 % | 90–100 % |
|---|---:|---:|---:|---:|---:|---:|
| Metropole | ca. 45 | ca. 30 | ca. 14 | ca. 7 | ca. 3 | ca. 1 |
| Kleinere Städte | 55 | 27 | 11 | 5 | 1,6 | 0,4 |
| Land | 70 | 21 | 6 | 2,3 | 0,6 | 0,1 |

Modifikatoren (Vorschlag): Alter, Beruf, Subkultur, Haft, Einkommen, Region.

### 13.3 Entwicklung über die Zeit

Die Population verändert sich dynamisch: Manche NPCs lassen sich später plötzlich viel tätowieren (E-TA-026). Entfernte NPCs entwickeln sich weiter (E-LE-005).

## 14. Editor, Import, Mods

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| TA-ED-01 | Ein gemeinsamer Produktionsablauf für Spieler, NPCs, Mods, Werkzeuge: Entwurf/Import → Vorschau → Interpretation durch den Künstler → Platzierung → Sitzungen → Heilung → Langzeitzustand. | E-TA-024 |
| TA-ED-02 | **Import eigener Designs ist Pflicht** (nicht nur eine Auswahl von Motiven); hohe Auflösung. | E-TA-024 |
| TA-ED-03 | Mods dürfen Motive, Stile, Studios, Künstler, Pigmente hinzufügen (Registries). | E-TA-024 |
| TA-ED-04 | Asset-QA: Importierte Designs durchlaufen Prüfungen (Auflösung, Lizenz-Metadaten, Inhaltsfilter nach Einstellungen). | E-TA-024 |
| TA-ED-05 | Der Charaktereditor hat denselben Tattoo-Editor für Spielerfigur; im Spiel arbeitet der Spieler über den Künstler. | E-TA-015 |
| TA-ED-06 | Nacktheit im Editor ist nach Einstellung möglich (Voll/Reduziert/Aus). | E-PA-009 |
| TA-ED-07 | Eine **Inhaltsprüfung für importierte Designs** (Verhinderung rechtswidriger Symbole) ist ❓ offen. | [K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik) |

## 15. Rendering und Leistung

🧭 aus 35#762/778:

```
Tattoo Design → Body-Surface Mask → Pigment Layer → Skin Shader Integration
```

- Virtual Texturing / Body-Space Texture Layers; viele Tattoos werden zu wenigen Schichten **zusammengebacken**, damit Ganzkörperbedeckung nicht hunderte Draw Calls erzeugt (E-TA-024).
- LOD: aus 10–20 m soll ein Motiv noch klar erkennbar sein (E-TA-024).
- Pigment sitzt „in der Haut“: Hautton, Beleuchtung, Hautzustand beeinflussen die Wirkung (E-TA-017).
- Optimierung ist globales Kernziel (E-PR-011).
- **Offene Frage:** konkrete Technik (Decals, Virtual Texture, prozeduraler Generator) – der Entwickler: „wie wir das machen, weiß ich nicht“.

## 16. Symbolik, Recht, Plattform

- Gruppensymbolik bleibt an konkrete Fraktionen und Figuren gebunden, nicht zufällig verteilt (Modul 07).
- Der Entwickler nannte auch Symbole extremistischer Gruppen (35#775/103). Ob solche Symbole **in Zielmärkten** erlaubt sind, ist **ungeprüft** ([K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik), E-RC-011).
- Nacktheit im Tattoo-Editor betrifft Plattform-Deskriptoren; die Grenze ‚erst bei Penetration Adult Only‘ ist laut Assistent zu eng ([K17](19-Konflikt-und-Widerspruchsmatrix.md#k17-erst-bei-penetration-adult-only)).
- Alle Tattoo-Figuren sind Erwachsene; Minderjährige erhalten keine Tattoos aus der Dienstleistung (Altersgate des Studios, [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)).

## 17. Technische Komponenten

| Komponente | Aufgabe |
|---|---|
| `TattooRegistry` | Motive, Stile, Pigmente |
| `TattooObject` / `TattooComposition` | Zustände, Schichten |
| `SurfaceMapper` | Körperoberflächenabbildung, Verformung |
| `PigmentModel` | Farbe, Haut, Licht |
| `LifecycleSimulator` | Heilung, Fading, UV |
| `ArtistService` | Skill, Preis, Fehlerbilder |
| `StudioService` | Termine, Preise, Hygiene, Ruf |
| `TattooCompositor` | Bake zu Schichten, LOD |
| `ImportPipeline` | Designs importieren, prüfen |
| `PopulationTattooGenerator` | Verteilung nach Demografie |
| `ModificationService` | Piercings/Modifikationen |
| `ClinicService` | Eingriffe |

## 18. Randfälle

[Modul 31](31-Randfaelle-Katalog.md#6-tattoos-und-körpermodifikation): Tattoo über Tattoo über Narbe; Körperumbau nach dem Stechen; Importbild mit Transparenz; Studio schließt zwischen Sitzungen; Künstler stirbt; Laser auf großflächigem Blackout; Piercing in verletzter Haut; Mod entfernt Pigmentdefinition; Figur wird befördert (Tier-Wechsel) mit vielen Tattoos.

## 19. Tests und Abnahmekriterien

| Test | Erwartung |
|---|---|
| Abdeckung 0 %, 50 %, 100 % | Rendering ohne Brüche, Budget eingehalten |
| Gewichtsänderung ±30 % | Tattoos verformen plausibel |
| Schwangerschaft + Bauchtattoo | Dehnung und Rückbildung korrekt |
| Fading über 10 Jahre | gemäß Zeitparametern |
| Cover-up | Ergebnis abhängig von Skill |
| Laser | mehrere Sitzungen, Restfarbe |
| Save/Load mit 200 Tattoos | identisch |
| Import eines 4K-Designs | akzeptiert, LOD korrekt |
| Verteilungstest 100.000 NPCs | Histogramm nahe Baseline (Toleranz festzulegen) |
| Studiopreise | Standortfaktor wirksam |
| Altersgate im Studio | Minderjährige erhalten keine Leistung |

[Modul 29](29-QA-Testkatalog.md#6-tattoos-und-körpermodifikation), [Modul 30](30-Abnahmekriterien.md#6-tattoos-und-körpermodifikation).

## 20. Ungeklärtes

- Rendering-Technik und Auflösungen.
- Zahlen: Studio-Dichte, Preise, Heilungs- und Fading-Zeiten, Verteilung.
- [K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik), [K17](19-Konflikt-und-Widerspruchsmatrix.md#k17-erst-bei-penetration-adult-only), [K20](19-Konflikt-und-Widerspruchsmatrix.md#k20-tattoo-prozentverteilung).
- Augen- und Schleimhaut-Tattoos (selten, Spezialkörpermodifikation): Umfang.
- Inhaltsprüfung importierter Designs.
- Verhältnis von Studio-Internet und allgemeinem Internetsystem (Wirtschaft/Internet-Planung steht aus).
- Zeitliche Einordnung: Tattoos als späteres Ausbaupaket vs. frühe Charaktergrundlagen (Modul 07, offen).
