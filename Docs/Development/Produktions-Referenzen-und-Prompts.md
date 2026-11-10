# Produktions-Referenzen und Produktionsprompts

> [!IMPORTANT]
> **Stand 08.11.2026. Das Produktionsprompt-System ist nicht abgeschlossen.** Die Master-Quellen legen die Richtung fest, nicht die konkreten Tutorials, Prompt-Vorlagen oder Arbeitsschritte. Alles, was hier nicht als 📋 markiert ist, ist offen.

Statuszeichen: siehe [Spielwelt und Untergrund §1](../Game/Spielwelt-und-Untergrund.md#1-statuskennzeichnung).

## 1. Quellen und Status

| Master-Quelle | Inhalt | Status |
|---|---|---|
| `01_CURRENT_CANONICAL_STATE/CURRENT_CANON_2026-09-16.md` | „Final Blender/environment/asset prompts should incorporate high-quality practical external tutorials/references and concrete visual-reference/scene-setup guidance.“ | 📋 Kanon (laut Master) |
| `01_CURRENT_CANONICAL_STATE/VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10.md` | Externe Referenzen für Blender-Szenen, Umgebungsbau und Asset-Erstellung; praktische Sichtreferenz- und Szenenaufbau-Hinweise | 📋 Kanon (laut Master) |
| `01_CURRENT_CANONICAL_STATE/PLANNING_BLOCK_STATUS_2026-09-16.md` | „Complete production-prompt system: unfinished.“ | Offen (laut Master) |
| `02_EMBEDDED_BASE_ARCHIVES/…` (Prompt-Bibliothek, 3.673 Dateien) | Entwurfsmaterial. Laut `Docs/Planning/SOURCES.md`: „Entwurf, Neuschreibung nötig“. | Nicht Anforderung |

**Regel:** Die Prompt-Bibliothek des Masters ist ein Entwurf (`DRAFT_REWRITE_REQUIRED`). Ihre Inhalte gelten nicht als Anforderung, bevor sie neu geschrieben und geprüft sind.

## 2. Gesicherte Anforderungen (📋)

1. **Externe Referenzen.** Produktionsprompts für Blender, Umgebungen und Assets sollen hochwertige, praxisnahe externe Tutorials und Referenzen einbeziehen, wo sie nützlich sind (Quelle: CURRENT_CANON_2026-09-16).
2. **Konkrete Szenenvorgaben.** Prompts sollen Sichtreferenzen und Hinweise zum Szenenaufbau enthalten, damit sie nicht nur textuell bleiben (Quelle: CURRENT_CANON_2026-09-16 und VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10).
3. **Entwickler-Abnahme.** Wahre Formen und Anatomien brauchen Konzept, Anatomiebogen, Blender-Prototyp und Abnahme durch den Entwickler, bevor eine Darstellung freigegeben wird (`Docs/Game/Kreaturen/20-Entscheidungsregister.md`, K-HW-015; Konzept in `Docs/Game/Kreaturen/19-Artennennungen-und-Artenvorlage.md`).
4. **Stimmproduktion** läuft nach einem eigenen Referenz-Workflow: freiwillige Freunde oder Familie liefern saubere Referenzaufnahmen; Einwilligung, kommerzielle Rechte, Nutzungsumfang, Aufbewahrung und Löschung sind Pflicht (`Docs/Game/Spielvision.md` §15.2 und Ergänzung „Stimmen“; `Docs/Planning/DECISIONS.md` C-40).

## 3. Offen

- Auswahl der konkreten externen Tutorials und Referenzquellen. Dies ist eine Entwicklerentscheidung; die Quellen nennen keine.
- Gliederung und Pflichtfelder einer Produktionsprompt-Vorlage (Asset, Szene, Referenz, Stil, Abnahme).
- Zuständigkeit: wer schreibt, prüft und pflegt die Prompts; Versionierung.
- Verhältnis zwischen Prompt-Vorlagen und den Asset-Pipelines (Blender, Unreal-Import).
- Neuschreibung der Prompt-Bibliothek des Masters: nicht begonnen. Sie ist Entwurf und keine Quelle für Anforderungen.

## 4. Abgrenzung

- Dieses Dokument ist keine Asset-Spezifikation. Konkrete Gestalt, Anatomie und Figuren stehen in `Docs/Game/Kreaturen/`.
- Keine Produktionsprompts werden hier festgeschrieben. Das System ist nicht freigegeben.

## 5. Zugehörige Dokumente

- [Spielvision](../Game/Spielvision.md): Stimmen und Referenzaufnahmen.
- [Gameplay-Systeme](../Game/Gameplay-Systeme.md): Produktionsanforderungen für Animation und Verletzung.
- [Kreaturen 19](../Game/Kreaturen/19-Artennennungen-und-Artenvorlage.md) und [Kreaturen 20](../Game/Kreaturen/20-Entscheidungsregister.md): Blender-Prototyp und Abnahme.
- `Docs/Planning/Nachweis/master-zuordnung-01-current.md`: Zuordnung der Master-Quellen.

## 6. KI-gestützte 3D-Asset-Pipeline (Recherche, Stand 19.10.2026)

> [!IMPORTANT]
> **Nur Recherche und Vorschlag.** Die Quelle entscheidet nicht über die Pipeline des Spiels. Herstellerangaben sind nicht unabhängig geprüft. Es gibt keinen Vergleich mit identischen SPN-Assets.

**Gemeldete Herstellerstände (Quelle: Herstellerdokumentation, am 19.10. abgerufen; bitte erneut prüfen):**

- Tripo Rigging v3.0 ist eine **ausdrücklich gewählte** Option für bessere und schnellere humanoide Rigs. Standard und Nicht-Humanoide bleiben unverändert auf v2.5. Neue Skelettvorgaben: `unreal`, `unity`, `actorcore`, `vrm`; `rig_type=others` ist verfügbar.
- Tripo API v3: Retopologie v3.0 und Smart UV mit Ausgabe- und Topologiesteuerung.
- Das offizielle ComfyUI-Paket von Tripo wurde für die Migration auf API v3 geprüft. Alte Workflows müssen vor einem Upgrade getestet werden. Eine Quelle nennt eine **Abschaltung der v2-API zum 31.10.2026**; dieses Datum ist vor einer Produktionsentscheidung neu zu bestätigen. Veraltete Refine- und Stylize-Knoten dürfen als Kompatibilitätsplatzhalter bleiben.
- Weitere Kandidaten mit unterschiedlichen Stärken und Lizenzbedingungen: Tripo H3.1/P2, Meshy 7.1, Rodin Gen-2.5, TRELLIS.2, Pixal3D. Pixal3D-Abhängigkeiten brauchen eine eigene Lizenzprüfung.
- Qualitäts-, Auflösungs- und Polygonangaben der Anbieter sind **Herstellerangaben**, keine neutralen SPN-Messungen.

**Vorschläge für SPN (💡):**

1. Ein versioniertes, engine-neutrales Austauschformat mit Netz, UV, PBR, Rig, Animation und Lizenzmetadaten.
2. Vergleich aller Kandidaten am **selben Referenzcharakter**: Skelettkompatibilität, Retargeting, Trennung von Kleidung, Materialqualität.
3. Keine Charakter-Editor-Architektur um einen einzelnen Anbieter herum. Lokale und entfernte Erzeugung müssen austauschbar bleiben. Herkunft jedes Assets wird festgehalten.
4. Erzeugte oder fremde Netze werden nicht automatisch veröffentlicht. Rechte und Trainingsbedingungen werden vor einer Open-Source-Veröffentlichung geprüft.

**Offen (aus den Fragen vom 19.10.):** Lizenzprüfung für KI-erzeugte und fremde 3D-Assets und Abhängigkeiten vor jeder Veröffentlichung; identische Testassets für alle Kandidaten mit Rigging, Kleidungstrennung, PBR und LOD-Leistung, erst danach eine Rangfolge.

## 7. Produktions- und Werkzeug-Anforderungen aus dem Backlog (Planungsinventar, 📋 Richtung)

Quelle: `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/planning-backlog/` (Stand 31.08./04.09.2026, ältere Momentaufnahme, Status **„PLANNING INVENTORY, not yet final Claude prompts“**). Diese Liste ist keine bestätigte Entscheidung.

**Blender- und Szenenproduktion (Track 29, 💡 Pipeline-Vorschlag):** Ablauf Grundriss oder Blueprint → Vermessungs- und Blickspezifikation → Greybox → modulares Set → Montage → eindeutige Architektur → Materialien → Lichtvarianten → Requisiten und Umweltgeschichte → Gameplay- und Audio-Anbindung → Optimierung und Prüfung → menschliche Abnahme. Dazu gehören eine bpy-Brücke, vermessene Set-Blätter, Pivots, Snap-, Kollisions-, Nanite- und LOD-Metadaten, mehrere Konzeptvarianten sowie die Trennung von Netz, Rig und Animation.

**Werkzeuge und Prozesse (Tracks 23, 27, 28, 30, 31; Nicht-Spielinhalt):**
- Prompt- und Planungsverwaltung (Track 23): Epics, Features, Arbeitspakete mit stabilen IDs, Abhängigkeitsgraph ohne Zyklen, Definition of Ready und Done, Qualitätsgate, Rückwirkungsindex. Diese Vorgaben betreffen die alte Prompt-Bibliothek und sind keine Spielanforderung.
- Dokumenten-Website (Track 27): rendert `Docs/` durchsuchbar und mit Abhängigkeitsnavigation. Sie ist **kein** zweiter Wahrheitsspeicher.
- Visuelles Referenzsystem (Track 28): stabile Referenz-IDs; unterscheidet verbindliche Referenzen, Stimmungsbilder, technische Diagramme und Vorschauen. Metadaten, Herkunft und Versionierung.
- Testorchestrierung (Track 30): Bot-gesteuerte Laufzeittests, Zustandsprüfungen, Screenshot-Basislinien, Leistungsspuren, deterministische Seeds, Regressionsläufe, nächtliche Ausführung.
- Menschliche Freigabe (Track 31): Unterscheidung **TECHNICALLY_VALIDATED** und **APPROVED**; Freigabepakete mit Annehmen, Änderungen anfordern und Ablehnen; Versionsverlauf und Prüfpfad.

**Nicht übernommen:** die Scrum-Sprint-Felder und Token-Vorgaben aus Track 23. Sie gehören zu dieser Prompt-Bibliothek und nicht zu `Docs/`.

## 8. Plattform-Grundlagen (Backlog Track 17, 📋 Richtung; plattformneutral)

- Steam, PlayStation und Xbox sind langfristig geplant. Dazu gehören Erfolge und Trophäen, Cloud-Speicher, Controller-Unterstützung mit Tastensymbolen, Builds, DLC und Zertifizierung.
- **Jetzt** werden nur plattformneutrale Grundlagen angelegt. Die exakte SDK-Umsetzung und die Erfolgsliste sind **offen**.
- Trophäen und Erfolge folgen der Regel der Medaillen (siehe [Spielvision](../Game/Spielvision.md), Abschnitt Medaillen): Replay ändert den Story-Zustand nicht; Online, Grounded und Permadeath sind getrennte Gruppen.
- Ein Controller-Symbolsystem (Glyphen) ist noch nicht spezifiziert. Es wird als offener Punkt geführt und nicht als Anforderung.

## 9. Audioproduktion und Audio-Qualitätssicherung (Entscheidungen 23.08.2026, 📋 Richtung)

- Verlustfreie Masterbibliothek mit 48 kHz / 24 Bit; für stark bearbeitetes Sound-Design eine Quelle mit 96 kHz / 24 Bit. Es gibt keine Pflicht, alles in höchster Auflösung zu speichern.
- Ziel ist **transparente Qualität** zur Laufzeit. Selektives PCM ist zulässig, gestufte Qualität und Bitrate werden getrennt bewertet. Hörbare Kompressionsartefakte sind ein **Abnahmekriterium**, kein Ziel, das nur auf dem Papier erreicht wird.
- Raum- und Portalakustik, 3D-Audio, Verdeckung, Nachhall und die KI-Wahrnehmung nutzen dieselben kanonischen Geräuschereignisse (siehe [Gameplay-Systeme](../Game/Gameplay-Systeme.md) §14.2).
- Eine eigene Audioproduktions- und Audio-QA-Linie ist vorgesehen. Sie umfasst Blind- und ABX-Tests, Prüfung von Codecs, Streaming- und Cache-Budgets sowie Regressionstests der Leistung.

**Bitraten-Planungsbereiche (PROJECT_DECISIONS, Entscheidungen 23.08.; 🧭 keine Codec-Zusage):** kritische Mono-Quellen etwa 192–256+ kbps oder PCM; hochwertige Gameplay-Mono etwa 160–224 kbps; Standard-Mono etwa 112–160 kbps; niedrig priorisierte Mono etwa 80–128 kbps; Musik und wichtige Stereo-Umgebung mit deutlich höheren Raten oder gleichwertigem transparentem Codec. Die endgültigen Werte entscheiden **Blindtests** pro Codec und Plattform. Wichtige Horrorgeräusche (Atem, Flüstern, Kratzen, ferne Schritte, feiner Regen) bekommen eine höhere Qualitätsstufe, weil Kompressionsartefakte dort auffallen.

## 10. Dialogsystem, Stimmproduktion und Werkzeug-Details (PROJECT_DECISIONS, Stand 09.11.2026)

**Dialog-Architektur (📋 Richtung):**
- Dialog ist ein **datengesteuertes Inhaltssystem**, keine direkte Verknüpfung einzelner WAV-Dateien. Jede gesprochene Zeile hat eine stabile **Dialog-ID** mit Revision. Sie verweist auf Sprecher, kanonischen Text, Gesprächskontext, Emotions- und Performance-Metadaten, Teilnehmer, Fall- und Weltzustand, Wissens- und Beziehungsbedingungen, Auswahlzweige, Stimmasset mit Revision, Lippensynchronisation, Gesichts- und Körperperformance, Untertitel und Lokalisierung sowie Prüfstatus.
- Ändert sich der Text, wird die bisherige Aufnahme als **neu aufzunehmen oder neu zu erzeugen** markiert. Veraltete Text-, Audio-, Gesichts- und Lokalisierungsassets müssen erkennbar sein.
- Stimmassets sind **austauschbar und versioniert** hinter der Dialog-ID. Ein Austausch (etwa Temp-Stimme durch Schauspielerstimme) verändert weder Quest-Logik noch Untertitel noch Speicherstände. Lippensynchronisation und Gesichtstiming werden aus der **endgültigen** Aufnahme neu erzeugt, nicht übernommen.
- Die **Stimmen-Bibel** (Voice/Character Bible) hält für wichtige Figuren Alter, Region, Akzent, Wortschatz, Bildung, Persönlichkeit, Humor, Stressreaktion und Tabuthemen fest. Sie sichert konsistentes Schreiben und Sprechen.
- **Kontextuelle Zurufe (Barks):** Wichtige Figuren brauchen große Mengen an Zurufen für Kampf, Schleichen, Erkunden, Verletzung, Fahrt, Weltereignisse und Begleiter. Die Variation muss so groß sein, dass keine auffällige Wiederholung entsteht (`PROJECT_DECISIONS` §34, Richtung).
- **Laden nur bei Bedarf:** Die Dialoglaufzeit lädt nur die kontextuell relevanten Blöcke und nicht das gesamte Dialogkorpus beim Start (`PROJECT_DECISIONS` §34, Richtung).
- **Schreiben und Spieltest vor der Aufnahme:** Dialoglogik wird geschrieben und spielgetestet, bevor die Sprachaufnahme beginnt. Die Sprachproduktion darf das Questdesign nicht vorzeitig festlegen.
- Modelle von Stimmen werden **nicht** im Spiel ausgeliefert. Das Spiel erhält gerenderte oder gecookte Audiodateien. Zur Laufzeit wird nichts live über Cloud-TTS erzeugt.
- Signalprofile für Telefon, Funk, Polizeifunk, Walkie-Talkie, Mailbox und Überwachungsaudio sind eigene Profile mit erhaltener Verständlichkeit.

**Hunter-Archiv-Explorer (Vorproduktion, 🧭 technische Richtung, nicht festgelegt):**
- Das vollständige `/docs`-Verzeichnis bleibt die kanonische Wahrheit. Der Explorer ist **nur** eine Anzeige und ein Index darüber, kein zweites Inhaltssystem.
- Markdown bleibt das Speicherformat. Gelesen wird über die gerenderte Website.
- Mögliche Technik: React mit TypeScript; für die Abhängigkeitsdarstellung ein Graphwerkzeug wie React Flow; für die Suche ein generierter Volltextindex (etwa SQLite FTS5). Bei 10.000 bis 100.000+ Aufgaben müssen Index und Suche serverseitig oder generiert laufen; das Laden aller Dateien im Browser ist ausgeschlossen.
- **Mindestfunktionen vor dem Codieren:** ansprechende Darstellung der Prompts und Spezifikationen; Hierarchie-Navigation (Epic, Feature, Story, Track); Volltextsuche; Detailseiten; Bild- und Diagrammanzeige; Verweise auf Pflicht-, Bezugs- und Folgeabhängigkeiten; Sichtbarkeit von Status, Qualitätsgate und offenen Entscheidungen; Breadcrumbs und Inhaltsverzeichnis (`PROJECT_DECISIONS` §38, Richtung).
- **Erwünscht vor dem Codieren:** interaktiver Abhängigkeitsgraph, Scrum- und Sprintansichten, Auswirkungsansicht mit Rückabhängigkeiten, Diff und Historie, Fortschrittsdashboards, visueller Asset-Browser und später die Ansicht des Claude-Ausführungsprompts.
- Der Explorer wird **vor** dem ersten großen Scrum-Sprint der Spielentwicklung fertig, damit der Plan prüfbar ist.

**Visuelle Referenzen (📋 Richtung):**
- Jede wichtige Referenz hat eine stabile **Visual-ID**, einen repositoryrelativen Pfad, Typ, Titel, Zweck, Version und Status, Herkunft und Rückverweise auf alle Aufgaben, die sie nutzen.
- Zu unterscheiden sind **verbindliche Referenzen** (kanonische Gestaltung, Geometrie, Layout) und **nicht verbindliche Stimmungs- oder Inspirationsreferenzen**. In einer Ausführungsaufgabe dürfen sie nicht verwechselt werden.
- Versionen sind eindeutig; eine Aufgabe nennt die maßgebliche Version.
- Der Explorer zeigt Vorschaubilder, Vergrößerung, Metadaten, Rückverweise und Vergleichsansichten. Eine Ansicht lässt nach Kategorien filtern (Monster, UI, Waffen, Orte, Gore, Stealth, Hunter-Archiv).

## 11. Produktionsregeln für Assets, Tests und Freigabe (PROJECT_DECISIONS, Stand 09.11.2026)

**Blender und Umgebungsbau (📋 Arbeitsregeln):**
- Eine komplexe Umgebung oder ein komplexes Asset wird **vor** der Blender-Arbeit spezifiziert.
- Zwei getrennte Quellen der Wahrheit: **MESSEN** (physische Maße, Proportionen, Wandstärke, Decken- und Türgrößen, Rasterraster, Kollisionsfreiräume, realer Maßstab) und **LOOK** (Materialien, Architektur und Stil, Verschleiß, Farbe, Lichtstimmung, visuelle Sprache).
- Ablauf für begehbare Umgebungen: Grundriss → Greybox → modulares Set → Montage → eigene Einzelarchitektur → Materialien → Licht → Ausstattung und Umweltgeschichte → Gameplay- und Audioanbindung → Optimierung und Prüfung.
- Greybox prüft Raumgrößen, Wege, Türen, Sichtlinien, Kamera, Stealth, Kampf und Navigation, bevor teure Kunst entsteht.
- Modulare Teile brauchen eine Spezifikationsbogen je Teil: ID, Maße, Pivot, Anschlusspunkte, Verbindungsart, Materialslots, Kollision, Nanite- und LOD-Eignung, Varianten und Schadensvarianten. Wichtige Orte erhalten genug Einzelarchitektur, damit sie nicht wie kopierte Bausätze wirken.
- Charaktere und Kreaturen werden in Teilaufgaben zerlegt: Netz, Varianten, Rig, Skinning und Gewichte, IK, Animation, Material, Verletzungs- und Gore-Kompatibilität, Interaktionspunkte, Export und Tests. Ein gut aussehendes Netz ist **nicht** automatisch animationsfähig.
- Wichtige Designs werden in mehreren Varianten erkundet, bevor eine kanonische Richtung gewählt wird. Generierte Konzeptbilder werden geprüft; das erste Bild wird nie automatisch übernommen.
- Beleuchtung ist ein eigener Arbeitsschritt mit Zuständen für Tag, Nacht, Stromausfall, Notbeleuchtung, Sturm, Taschenlampe und übernatürliche Lichter.
- Schleife je Asset: Erzeugen oder Bauen → technisch prüfen → Vorschau-Render → visuelle Prüfung → Korrektur → erneut prüfen. Ein erzeugtes `.blend` allein gilt nie als fertig.
- Claude darf Blender später über kontrollierte Werkzeuge oder Python (`bpy`) bzw. eine MCP-artige Brücke steuern, wenn das stabil funktioniert. Ein erzeugtes `.blend` allein gilt auch dann nie als fertig.
- Automatische Testpawns oder Bots führen Eingaben und Szenarien aus. Gebäudetests gehen durch Türen und Räume, prüfen Verstecke, Navigation, Kampf und Stealth und erfassen Performance- und Streaming-Fehler. Screenshot-Tests ergänzen die maschinelle Prüfung von UI, Rendering, Assets, Gore und Materialien; die visuelle Qualität bleibt menschlich zu prüfen (`PROJECT_DECISIONS` §40–41, Richtung).
- Ein externes Video ist als Arbeitsanregung erhalten (`https://www.youtube.com/watch?v=Z9fyPTQVEdI`). SPN wendet einen deutlich strengeren Prüfmaßstab an.

**Claude-Ausführung und Laufzeittests (📋):**
- Eine Aufgabe ist **nicht** fertig, weil Code geschrieben oder ein Asset erzeugt wurde. Standardablauf: Ziel und genaue Abhängigkeiten lesen → Repository prüfen → umsetzen → Tests schreiben → bauen → testen → Unreal-/Spiel-Laufzeit starten → kontrolliertes Szenario ausführen → Logs, Zustände, Screenshots, Traces und Replays sammeln → Fehler analysieren → korrigieren → erneut prüfen → Regressionstests → zur Prüfung vorlegen.
- Wichtiges Laufzeitverhalten wird in einer **gestarteten** Spielsitzung getestet, nicht nur durch Kompilierung.
- Kleine deterministische Testkarten je Fachgebiet (Ballistik, Nahkampf, Verletzung, KI und Stealth, Audio, Fahrzeuge, Streaming und Rendering) vermeiden, dass für jeden Test die offene Welt geladen werden muss.
- KI-Tests prüfen auch die **inneren** Zustände: bestätigtes und vermutetes Wissen, letzte bekannte Position, Hörschätzung, Suchphase, geteilte Information und Grund der Taktik.
- Schlägt ein Test fehl, werden Seed, Eingaben, Weltzustand, Logs, KI-Zustand und Replay gesichert, damit der Fehler reproduzierbar ist.
- Eine **SPN-Test-Orchestrierung** soll Builds erstellen, Sitzungen starten, Suiten ausführen und maschinenlesbare Ergebnisse zurückgeben.
- Claude korrigiert gewöhnliche Fehler selbstständig und wartet nicht auf die nächste Anweisung. Eine **offene Designfrage** wird dagegen nie erfunden, nur um einen Test grün zu machen.
- Scrum-Storys enthalten Umsetzung, Pflichttests, Laufzeitprüfung und Regression. Sie gelten nur dann als fertig, wenn alle drei vorliegen.

**Freigabe (📋):**
- Automatische Tests und Selbstprüfung ergeben **technische Validierung**, aber keine kreative Freigabe. Für subjektive Ergebnisse (Modelle, Charaktere, Monster, Waffen, Fahrzeuge, Umgebungen, Materialien, Animation, Gore, UI, Musik, Klang, Stimmen, Zwischensequenzen, Licht, VFX, Story und Fälle) entscheidet der Nutzer.
- Ein technisch gelungenes Asset kann abgelehnt werden. Zustände: PLANNED → READY → IN_PROGRESS → AUTOMATED_TESTING → TECHNICALLY_VALIDATED → READY_FOR_HUMAN_REVIEW → APPROVED, mit FAILED, CHANGES_REQUESTED, REJECTED und REWORK_IN_PROGRESS.
- 💡 **Hierarchie und Arbeitspaket-Felder** (Projektzusammenfassung §80, MASTER-MIGRATION-18, Vorschlag): Source of Truth → Architektur-Spezifikationen → Inhaltsspezifikationen → ADRs → Epics → Stories → Arbeitspakete → Claude-Umsetzung → Build → Test → Laufzeitprüfung → menschliche Freigabe → Commit. Ein Arbeitspaket enthält ID, Ziel, Umfang, Nicht-Ziele, Abhängigkeiten, Quellenverweise, erlaubte Dateien, Schnittstellen, Akzeptanzkriterien, Tests, Leistungsziele, Fehlerfälle, Migrationsauswirkungen, Definition of Done und Commit-Grenze. Visuelle und animierte Inhalte brauchen menschliche Freigabe. Commits bleiben beim Entwickler.
- 💡 **Schätzung, keine Messung:** Die Projektzusammenfassung (`PROJECT_STATE_2026-08-31_COMPACT.md` §80) nennt grob 800–1.200 Arbeitspakete für den Interaktionsrahmen und 3.000–5.000+ mit Inhalten, Animation und Integration. Diese Zahlen haben keine nachvollziehbare Berechnungsgrundlage und sind kein Fortschrittswert.
- Der Prompt/Spec-Explorer soll später prüfbare Vorschauen, Renderings, Screenshots, Tests, Leistungswerte und Referenzbilder zeigen und die Aktionen **Freigeben**, **Änderungen anfordern** und **Ablehnen** bieten (`PROJECT_DECISIONS` §42, Richtung).
- `TECHNICALLY_VALIDATED` ist **nicht** gleich `APPROVED`. Änderungswünsche gehen zurück, die technische Prüfung läuft erneut.

**Reihenfolge der Vorproduktion (📋):**
- Das Projekt nutzt bewusst eine umfangreiche wasserfallartige Vorplanung vor der großen Umsetzung, danach Scrum-Sprints. Der Nutzer will den vollständigen Plan und die Prompt-Bibliothek abschließen, bevor der erste große Entwicklungssprint beginnt.
- Die endgültige globale Umsetzungsreihenfolge ist **noch nicht festgelegt**; die Nummerierung der Tracks ist nur eine Navigationshilfe. Sie entsteht nach dieser Kette: vollständige Planung → kanonische Spezifikationen → Arbeitspakete → Abhängigkeitsgraph → Epics, Features, Storys → validierte Reihenfolge → Qualitätsgate → Ausführungs-Prompts → Umsetzung.

**Fortschrittsmessung (📋):** Bei jedem größeren Planungsabschnitt werden zwei getrennte Näherungswerte berichtet: der Gesamtfortschritt der Spielplanung und der Anteil des fertigen Claude-Produktionsplans. Ein Prompt zählt erst als fertig, wenn er individuell hochwertig, mit Quellenbezug, vollständigen Abhängigkeiten und Zeilenbezügen, in der Scrum- und Graphenordnung, mit bestandenem Qualitätsgate und ausführbar ist.

## 12. Entwicklerwerkzeuge, Tests und Prompt-Governance (PROJECT_DECISIONS §19–20, Stand 09.11.2026)

Quelle: `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md`, Abschnitte „Testing, tools and quality“ und „Prompt-library governance“. Status 📋 Richtung. Nachweis: `Planning/Nachweis/entscheidungsabgleich-project-decisions.md`, Zeilen 19 und 20 (MASTER-MIGRATION-17).

**Tests (📋):**
- Tests sollen praktisch jedes wichtige System abdecken; wichtige Fehler bekommen Regressionstests.
- Pflicht sind Tests für Speichern, Laden, Migration, Weltzustand, Fälle, KI, Kampf, Fahrzeuge, Mods und Streaming. Die Testarchitektur steht in `Game/Gameplay-Systeme.md` §19 (Einheits-, Integrations-, Gameplay-, Regressions- und Randfalltests).

**Entwicklerwerkzeuge (📋):** Fall-Debugger, KI-Debugger (der KI-Inspektor ist in `Game/Gameplay-Systeme.md` §7 belegt), Weltzustands-Inspektor, Performance-Werkzeuge, Spawn- und Encounter-Werkzeuge sowie domänenspezifische Validierung. 💡 Zusätzlich Waffen-, Ballistik- und Verletzungs-Debugger (Entwurf `SPN_Prompt_Pack/05_Quality`). Keine Werkzeugliste ist als Umfang festgelegt; die Einzelwerkzeuge bleiben ❓ offen.

💡 **Fall-Wiederholungs- und Tag-Werkzeug** (aus dem älteren Prompt-Entwurf `SPN_Prompt_Pack/02_Narrative_Investigation`, nicht bestätigt): hilft, früh zu ähnliche Fälle zu erkennen.

**Optimierung (📋):** Performance ist Pflicht und keine Politur am Schluss (siehe `Game/Gameplay-Systeme.md` §Performance).

**Prompt-Governance (📋 Richtung, nicht freigegeben):**
- Die Anzahl der Prompts ist nicht begrenzt. Vollständigkeit ist wichtiger als Anzahl; komplexe Themen werden feiner geteilt, einfache bleiben kompakt.
- Jede bekannte Entscheidung, Anforderung, Abhängigkeit, Variante, offene Frage und wichtige Beispiel muss vertreten sein.
- Jeder Umsetzungsprompt soll ohne Rückfragen direkt umsetzbar sein. Pflichtbestandteile: Ziel, Kontext, verbindliche Anforderungen, betroffene Architektur, Umfang und Nicht-Umfang, Umsetzungshinweise, Randfälle, Fehlerfälle, Auswirkungen auf Persistenz, Modding, Plattform und Performance, Tests, Akzeptanzkriterien und Definition of Done.
- Gemeinsame Regeln dürfen aus Governance-Dateien referenziert werden; aufgabenkritische Information darf aber nicht hinter einem unklaren Verweis versteckt sein.
- Der Geltungsbereich umfasst Blender und DCC, Animation, Kunst, Audio, VFX, UI/UX, Plattform, Trophäen, Zertifizierung, QA, Release, Support, Recht, Webseite und App, KI, Backend, Marketing und Nachbetreuung.
- Kanonische Information wird **einmal** definiert; abhängige Prompts verweisen über stabile ID, Pfad und Abschnitt, statt die Spezifikation zu kopieren. Eine Abhängigkeits- und Wirkungsübersicht ist maschinenlesbar zu führen.
- Die Umsetzungsreihenfolge ergibt sich aus einem geprüften Abhängigkeitsgraphen (DAG), nicht aus der Ordnerzahl. Grundlagen, Verträge und Tests kommen vor abhängigen Systemen; jeder Sprint-Zuwachs bleibt baubar und getestet.
- Ein Arbeitspaket gilt erst als fertig, wenn seine Pflichttests bestehen. Nach Abschluss der vollständigen Planung wird für jedes Arbeitspaket ein eigener, knapp gehaltener Ausführungs-Prompt erzeugt, der nur die nötigen Quellabschnitte benennt (`PROJECT_DECISIONS` §27, MASTER-MIGRATION-17).
- Die bestehende Prompt-Bibliothek des Masters bleibt Entwurf (`DRAFT_REWRITE_REQUIRED`) und keine Anforderungsquelle; diese Governance-Regeln gelten für eine künftige Neufassung.
