# Adult 27: Spezifikation Steuerung, Oberfläche, VR, Kamera, Audio und Replay

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register, Abschnitte ST und RP](14-Entscheidungsregister.md#12-st-steuerung-oberfläche-vr-kamera-audio)). Kein Kanon. Eingabebelegungen (Tasten, Sticks) sind **nicht festgelegt**; der Entwickler: „weiß ich noch nicht“ (35#574, 35#791). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachlich: [Modul 04](04-Steuerung-Darstellung-Kamera-Audio.md). Engine: [Modul 22](22-Spezifikation-Interaktionsengine.md).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Hybrid-Steuerung](#3-hybrid-steuerung)
4. [Intensitäts- und Rhythmuseingabe](#4-intensitäts--und-rhythmuseingabe)
5. [Aktionsbibliothek und Oberfläche](#5-aktionsbibliothek-und-oberfläche)
6. [HUD und Hilfen](#6-hud-und-hilfen)
7. [Kamera](#7-kamera)
8. [Audio](#8-audio)
9. [VR](#9-vr)
10. [Barrierefreiheit und Einstellungen](#10-barrierefreiheit-und-einstellungen)
11. [Replay, Aufnahme, Streaming](#11-replay-aufnahme-streaming)
12. [Darstellungsprofile und Nacktheit](#12-darstellungsprofile-und-nacktheit)
13. [Statistik](#13-statistik)
14. [Technische Komponenten](#14-technische-komponenten)
15. [Randfälle](#15-randfälle)
16. [Tests und Abnahmekriterien](#16-tests-und-abnahmekriterien)
17. [Ungeklärtes](#17-ungeklärtes)

---

## 1. Ziel und Abgrenzung

Die Steuerung soll das Gefühl einer **freien, realistischen Interaktion** geben, nicht von Button-Spammen („spammen wäre zu GTA“, Entwickler 35#793). Sie ist in [Modul 22](22-Spezifikation-Interaktionsengine.md) eingespeist; sie darf den Regelkern nie umgehen ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)).

**Nicht Adult:** das Kampf-/MMA-Lernsystem (Basismoves früh, komplexe Techniken lernen, Trainer, kein normales Upgradesystem; E-ST-003). Es ist ein **separater Core-Block**.

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Analoge Stick-Steuerung statt Button-Spammen | 📋 | E-ST-001 |
| **Hybrid-Steuerung für Adult** (Aktions-/Kombinationshilfen + analoge Echtzeitsteuerung) | 📋 | E-ST-002 |
| Kampf behält separates UFC-artiges Lernsystem | 📋 | E-ST-003 |
| Aktionsbibliothek erst nach Freischaltung sichtbar | 📋 | E-ST-004 |
| Intensitätsmodifikator, Körperbereichswahl, Smart Context, parallele Aktionen, HUD-Stufen, Remapping, Haptik, Kamera, Barrierefreiheit, Profile | 📋 | E-ST-005 |
| VR: gleiches System, Handtracking, optional Ganzkörper | 📋 | E-ST-006, E-SO-031 |
| Favoriten | 🧭 („weiß ich noch nicht, aber ja, glaube schon“) | E-ST-009 |
| Content-Toggle für starke Körperreaktionen und Blut | 📋 | E-ST-011 |
| Replay als Datenpaket, Cinematic Re-Render | 📋 / 🧭 | E-RP-002, E-RP-003 |
| Streaming/Aufnahme im Hauptspiel | 📋 | E-RP-004 |
| Konkrete Tastenbelegung, Menüaufbau | ❓ | E-SO-017, E-SO-030 |
| Streamer-Filter/Namen der Nacktheitsstufen | ❓ | E-ST-012 |

## 3. Hybrid-Steuerung

📋 E-ST-002: Der Entwickler wollte in R18 zunächst zwei Konzepte nicht festlegen (Frage 1, 2, 4, 6 offen); der Assistent schlug **ein Hybrid-System** vor (35#796); der Entwickler: „bei Kampf soll das so sein wie ich gesagt habe; wenn du auf Adult meinst, das passt“ (35#797) und „ok dann weiter, das passt dann“ (35#799).

🧭 Ausprägung:

| Ebene | Funktion |
|---|---|
| **Absichtsebene (Hilfen)** | Aktionen/Kombinationen aus der Bibliothek (Name, Beschreibung, Eingabe, Vorschau, Voraussetzungen, Übergänge) – eine **Hilfe, keine Grenze** (35#611) |
| **Analogebene (Echtzeit)** | kontinuierliche Steuerung von Bewegungsrichtung, Tempo, Amplitude, Druck (Stick, Trigger) mit Modifikatoren |
| **Kontextfilter** | zeigt nur, was nach Haltung, freier Hand, Items, Möbeln, Rolle und Übergängen sinnvoll ist |

Die Wahl „direkt vs. unterstützt“ ist eine **Spielereinstellung** (35#790 Frage 1), nicht ein harter Moduswechsel.

## 4. Intensitäts- und Rhythmuseingabe

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| ST-IR-01 | **Stufenlose** Intensität; Geschwindigkeit ≠ Kraft ≠ Amplitude; Beschleunigung als eigene Achse. | E-SO-034 |
| ST-IR-02 | Direkte Controllersteuerung (Stick = Richtung); Modifikator für Feinsteuerung. | E-SO-034 |
| ST-IR-03 | **Rhythmus-Assistenz** (Halten, Wiederholen); NPC kann Bewegung übernehmen; **gemeinsame Bewegung**: Solver kombiniert Absichten. | E-SO-034, E-SO-019 |
| ST-IR-04 | Spamming ist nicht optimal; Analogeingaben sind schneller als Tastenhämmern. | E-ST-001 |
| ST-IR-05 | Eingaben werden zu **Absichten** (Intent), nie direkt zu Animationen. | E-SO-017 |
| ST-IR-06 | Eingabe kann nie eine Zustimmungs-/Fähigkeitsprüfung umgehen. | [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md) |
| ST-IR-07 | Intensität wird **von Sekunde zu Sekunde** anpassbar (Entwickler 35#574/5); die Eingabe dafür ist offen („X-Taste spammen … oder Steuerpad rechts“). | E-ST-005 |
| ST-IR-08 | Unbeabsichtigter Kontakt (z. B. falscher Körperbereich) wird als solcher erkannt; die Figur reagiert; keine Zustimmung zu neuer Handlung (siehe Regelkern ZU-LS-05). | 35#575/577 |

## 5. Aktionsbibliothek und Oberfläche

### 5.1 Bibliothek

📋 E-ST-010: Die Bibliothek ist **kontextabhängig** und **jederzeit einsehbar** (Entwickler 35#610: „alle verfügbaren im Menü anschauen“).

| Eigenschaft (🧭) | Beschreibung |
|---|---|
| Name, Beschreibung | als Hinweis |
| Eingabe | Kombination bzw. Analogvorschrift |
| Vorschau | **ungefähr**, ohne Exaktheitsversprechen |
| Voraussetzungen | Haltung, Platz, freie Hand, Items |
| Übergänge | wohin man von hier kommt |
| Freischaltung | Aktionen erst sichtbar, wenn gelernt/entdeckt (E-ST-004) |
| Suche, Favoriten | ja (E-ST-007, E-ST-009) |
| Filter | Kontextmenü filterbar |
| Eigene Kombinationen | als Hinweis speicherbar |

### 5.2 Oberfläche

| Nr. | Anforderung | Basis |
|---|---|---|
| ST-UI-01 | Beim ersten Start des Pakets: **Setup-Assistent** (Standard bei Überspringen). | E-ST-008 |
| ST-UI-02 | Begründung bei ungültiger Aktion („warum geht das nicht“). | E-ST-007 |
| ST-UI-03 | Smart Context: Vorschläge nach Situation; **parallele Aktionen** im UI sichtbar. | E-ST-005 |
| ST-UI-04 | Keine sichtbare Präferenz-Checkliste (Spieler lernt über Verhalten). | E-NP-004 |
| ST-UI-05 | Risiko-Anzeige subtil, keine Prozentwerte. | E-KO-028 |
| ST-UI-06 | Lern-UI (Hilfe für Neueinsteiger, einblendbar). | E-ST-007 |
| ST-UI-07 | Das Menü ist **modular**; UI enthält keine Spielregeln (ViewModel). | 35#473 |
| ST-UI-08 | Kein Minispiel. | E-IA-010 |

## 6. HUD und Hilfen

| Nr. | Anforderung | Basis |
|---|---|---|
| ST-HD-01 | HUD-Stufen **Off / Minimal / Standard / Detailed / Developer**. | E-ST-005 |
| ST-HD-02 | Minimal-HUD filmisch. | E-ST-005 |
| ST-HD-03 | UI-Hilfen in **vier Stufen** (aus/minimal/normal/ausführlich). | E-SP-003 |
| ST-HD-04 | Pose-Overlay optional, **standardmäßig aus**. | E-SO-018 |
| ST-HD-05 | Developer-Stufe zeigt Inspector-Daten nur in Entwicklungsbuilds. | E-NP-021 |
| ST-HD-06 | Streamer-Namen/Filter der Nacktheitsstufen sind offen. | E-ST-012 |

## 7. Kamera

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| ST-KM-01 | Kamera-Modi: Third Person, näher, First Person, Cinematic. | 35#790 (R18A) |
| ST-KM-02 | First-Person-Stabilisierung (Komfort). | E-ST-005 |
| ST-KM-03 | Kamera ist **unabhängig** von der Simulation; Cinematic nur zur Beobachtung/Replay, nicht zur Steuerung. | E-ST-007 |
| ST-KM-04 | Kamera zeigt keine Grenzverletzung der Darstellungsprofile (Basis: Kamera/FOV-Fehler zeigen nichts; K§17). | K§17 |
| ST-KM-05 | Beim Replay-Neurendern: Cine-Kamera 24–85 mm, Color Grading, Film Grain optional, ohne HUD. | E-RP-002 |
| ST-KM-06 | Freie Kamera nachträglich im Replay/Capture-Editor (Vorschlag). | 35#465 |

## 8. Audio

Siehe [Modul 04, Abschnitt 7](04-Steuerung-Darstellung-Kamera-Audio.md#7-audio-und-stimmen). Technisch (🧭):

| Nr. | Anforderung | Basis |
|---|---|---|
| ST-AU-01 | Atem- und Stimmaudio prozedural aus Reaktionsprofil und Physiologie. | E-KO-021/022 |
| ST-AU-02 | Stimm-Packs getrennt (Sprachen, Qualität, Reaktionssets). | E-PA-021 |
| ST-AU-03 | Lautstärke situationsabhängig (Ort, Nachbarn, Wände); Schall breitet sich realistisch aus (Zeugenerkennung). | E-IA-035 |
| ST-AU-04 | Audiobusse getrennt (für Aufnahme/Streaming). | E-RP-007 |
| ST-AU-05 | Hohe Bitrate für exportierten Ton einstellbar. | E-RP-001 |
| ST-AU-06 | Voice-Standard + optionaler Lossless-Pack (allgemein). | OPTIONAL_CONTENT_PACKS |

## 9. VR

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| ST-VR-01 | **Gleiche Simulation**, anderer Eingang; VR verändert nicht den Kern. | E-SO-031 |
| ST-VR-02 | Tracking-Stufen: Headset+Controller → Handtracking → Hüfte → Füße → Ganzkörper → Haptik. | 35#795 |
| ST-VR-03 | Fingertracking, Body Ownership, Teil-Körpertracking; IK ergänzt fehlende Tracker. | E-ST-007 |
| ST-VR-04 | VR-Intent-Prediction (Vorschlag). | 35#626 (R16H) |
| ST-VR-05 | Räumliches Menü, Komfort (Übelkeitsvermeidung), sitzend/stehend, Kalibrierung. | E-ST-006 |
| ST-VR-06 | Entwickler: VR „soll sich anfühlen wie echt“, bewegliche Hände/Füße/Hüfte. | E-ST-006 |
| ST-VR-07 | **VR wird später implementiert**, bleibt aber in der Architektur berücksichtigt. | E-ST-006 |

## 10. Barrierefreiheit und Einstellungen

| Nr. | Anforderung | Basis |
|---|---|---|
| ST-BF-01 | Vollständig remappable Eingaben; Konflikterkennung; Control-Profile. | E-ST-005 |
| ST-BF-02 | Haptik-Optionen; Eingabevereinfachungen; Untertitel; Kontrastoptionen. | E-ST-005 |
| ST-BF-03 | Settings-Kategorien: Gameplay, Controls, Visuals, Physics, Audio, Content, Replay, VR, Accessibility, Advanced; Presets + Advanced. | E-SP-003 |
| ST-BF-04 | Content-Einstellungen sind modular ohne Speicherschaden änderbar. | E-IA-037 |

## 11. Replay, Aufnahme, Streaming

### 11.1 Anforderungen des Entwicklers

📋 (35#464–470, 484):

- Szene speichern; Export in 4K/8K oder niedriger, extrem hohe Bitrate, einstellbar; Speicherverbrauch anzeigen (E-RP-001).
- Beim Neurendern soll es wie ein Film aussehen (ohne HUD), „fast echt“ (35#466).
- Re-Render im Hintergrund mit Fortschritt; Weiterspielen ohne großen Performanceverlust (E-RP-003).
- Aufnahme/Streaming direkt im Hauptspiel, unabhängig von Adult, „ein Button“ (E-RP-004).
- Jede Szene aufnehmbar, auch nachträglich; nur kleine Clips aus dem Replay-Puffer (E-RP-006).
- Fragen 21–100 (Replay-Editor, Kameras, Codecs, Audiobusse, Provider-Unabhängigkeit): „mach, was du gut findest“ (E-RP-007).

### 11.2 Modell (🧭 Vorschlag des Assistenten, 35#465/467/469/485)

| Teil | Beschreibung |
|---|---|
| **Replay-Datenpaket** | Figuren, Session-Seed, Zeitlinie, Pose/Eingabe, NPC-Entscheidungen, Physikereignisse, Kamera, Umwelt, Audioreferenzen (kleine Datei) |
| **Quick Export** | Direkt aus dem Spiel als Video |
| **Cinematic Re-Render** | aus dem Datenpaket in höherer Qualität, ohne HUD |
| **Replay-Puffer** | für kurze Clips |
| **Capture-Pipeline** | gemeinsame Aufnahme-/Streaming-Pipeline („Mini-OBS“), Encoding über NVENC/AMF/QuickSync, Audiobusse, Datenschutzmodus |
| **Scene-State-Track** | nur für Cinematic Re-Render, unsichtbar |
| **Adult-Filter** | Adult-Inhalte können separat als nicht streambar/versteckt konfiguriert werden, falls Plattformregeln es verlangen (Vorschlag) |

Regeln:

| Nr. | Anforderung | Basis |
|---|---|---|
| ST-RP-01 | Replay ≠ Savegame; deterministische Kernspur. | E-RP-008 |
| ST-RP-02 | Hintergrund-Rendering läuft mit „Gameplay zuerst“. | E-RP-008 |
| ST-RP-03 | HUD/Untertitel/Prompts/Debug getrennt schaltbar beim Re-Render. | E-RP-002 |
| ST-RP-04 | Qualitätsprofile (Small … Lossless), Codec/Bitrate/CRF/10-Bit/HDR; Speicherschätzung live. | 35#465 |
| ST-RP-05 | Ton: hohe Bitrate (AAC/Opus 320–512 kbps, FLAC, PCM 24 Bit), Stereo/5.1/7.1 (Vorschlag). | 35#465 |
| ST-RP-06 | Aufnahme/Veröffentlichung getrennte Zustimmungen in der Spielwelt (siehe [Modul 26](26-Spezifikation-Weltintegration-Branche-Wirtschaft.md#6-medien-plattformen-internet)). | E-WE-011 |
| ST-RP-07 | Replays von Adult-Sitzungen gehören zum **Adult-Speicherbereich**; bei Löschen der Daten werden sie entfernt. | E-PA-030 |
| ST-RP-08 | Der Replay-Pfad darf **keine** Inhalte erzeugen, die das Live-Spiel nicht zeigen könnte. | 28#25 Prinzip |
| ST-RP-09 | Streaming-Plattformregeln (Altersfreigabe, Nacktheit) sind ungeprüft. | E-ST-012 |

## 12. Darstellungsprofile und Nacktheit

| Aussage | Status | Basis |
|---|---|---|
| Offizielle Basis: konsistente nichtexplizite Darstellung; Nacktheit konfigurierbar (voll/reduziert/aus); Intimität angedeutet | 📋 | K§17 |
| Alle Plattformen im Basisspiel gleich | 📋 | E-PA-002 |
| Explizitere Darstellung nur als getrenntes Paket, nicht in Konsolen-Builds | 📋 | E-PA-003/004 |
| Presentation Resolver löst Assets nach Profil auf (kein `if AdultModInstalled`) | 📋 | E-PA-004 |
| Vier Profile (RomanceOnly, ConsoleExplicit, PCAdult, Concealed) | 🧭 Vorschlag 23#538 | – |
| Streamer-Filter, Namen der Stufen | ❓ | E-ST-012 |

Die Dokumentation beschreibt keine konkreten Darstellungsgrenzen; diese sind **Release-/Rechtsentscheidungen** ([Modul 10](10-Produktion-Recht-Vertrieb.md)).

## 13. Statistik

📋 E-LE-009 / E-SP-004: globales Lifetime-Statistiksystem, **unabhängig von Adult**, zentral über Ereignisse; Adult-Statistik nur bei aktivem Paket sichtbar und ausblendbar; Statistik ist keine Arcade-Mechanik und treibt kein Verhalten (E-IA-021).

## 14. Technische Komponenten

| Komponente | Aufgabe |
|---|---|
| `InputRouter` | Eingabegeräte → Absichten |
| `ControlProfile` | Remapping, Profile, Konflikte |
| `ActionLibraryService` | Bibliothek, Kontextfilter, Suche, Favoriten |
| `ViewModelLayer` | UI-Zustand ohne Spielregeln |
| `HudController` | Stufen |
| `CameraDirector` | Kameramodi, Cine-Kamera |
| `AudioDirector` | Busse, Prozedural |
| `VRInputAdapter` | Tracking, Intent-Prediction |
| `ReplayRecorder` | Datenpaket, Seeds |
| `ReplayRenderer` | Re-Render im Hintergrund |
| `CapturePipeline` | Aufnahme/Streaming |
| `ContentToggleFilter` | Darstellung nach Einstellung |
| `StatisticsService` | globale Statistik |

## 15. Randfälle

[Modul 31](31-Randfaelle-Katalog.md#8-steuerung-ui-vr-und-replay): Controllerwechsel mitten in der Sitzung; Remapping-Konflikte; VR-Tracker fällt aus; Replay eines Savegames mit entferntem Mod; Re-Render mit fehlendem Asset; Streamer-Modus während Adult-Szene; Content-Toggle-Änderung während Re-Render; Aufnahmespeicher voll; Auflösung/Bitrate unterstützt GPU nicht.

## 16. Tests und Abnahmekriterien

| Test | Erwartung |
|---|---|
| Eingabe → Absicht | kein Eingabepfad umgeht Gate |
| Remapping-Konflikte | erkannt, Warnung |
| Hybrid: gleiche Absicht über Analog und Bibliothek | gleiche Sitzung |
| Replay-Determinismus | Re-Render aus Paket identisch (innerhalb Toleranz) |
| HUD aus im Re-Render | kein HUD im Video |
| Content-Toggle | Darstellung verändert, Physiologie gleich |
| VR vs. Flat | gleiche Sitzungsergebnisse bei gleicher Absicht |
| Streaming-Filter | markierte Szenen werden ausgeblendet |
| Barrierefreiheit | alle Funktionen ohne feine Motorik erreichbar (Kriterien festzulegen) |

[Modul 29](29-QA-Testkatalog.md#8-steuerung-ui-vr-und-replay), [Modul 30](30-Abnahmekriterien.md#8-steuerung-ui-vr-und-replay).

## 17. Ungeklärtes

- Konkrete Eingabebelegung und Menüaufbau (Entwickler: „weiß noch nicht“).
- Verhältnis zum UFC-artigen Kampfsystem (separater Lock).
- Streamer-/Plattformfilter ([E-ST-012](14-Entscheidungsregister.md#12-st-steuerung-oberfläche-vr-kamera-audio)).
- Rendering-Techniken (Path Tracing im Re-Render, Formate AV1/HEVC/H.264/EXR) – delegiert (E-RP-007).
- Barrierefreiheitsziele (Standards).
- VR-Einführungszeitpunkt.
- Haptik-Hardware.
