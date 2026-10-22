# Entscheidungsstatus: Was bestätigt, vorläufig, offen oder überholt ist

> [!IMPORTANT]
> Dieses Dokument ist die **Gesamtübersicht über den Entscheidungsstand der Spielplanung**. Es trifft **keine** Entscheidungen. Es sammelt, was der Entwickler festgelegt hat, was nur ein Arbeitsstand ist und was offen bleibt. Wo sich Planungsdokumente widersprechen, steht der Widerspruch ausdrücklich dabei. Es wurde aus dem Planungsmaster aufbereitet und enthält keine Kopie und keine Verweise auf seine Dateien.

Es gehört zu den Spiel-Dokumenten in [`Docs/Game/`](.), neben [Spielvision](Spielvision.md), [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md), [Gameplay-Systeme](Gameplay-Systeme.md) und [Adult-System](Adult-System.md), und ergänzt die [README](../../README.md).

**Inhalt**

1. [Wie dieses Dokument zu lesen ist](#1-wie-dieses-dokument-zu-lesen-ist)
2. [Bestätigt und gesperrt](#2-bestätigt-und-gesperrt)
3. [Vorläufig](#3-vorläufig)
4. [Vorschläge, die nicht ratifiziert sind](#4-vorschläge-die-nicht-ratifiziert-sind)
5. [Offene Entscheidungen nach Themen](#5-offene-entscheidungen-nach-themen)
6. [Widersprüche und überholte Planung](#6-widersprüche-und-überholte-planung)
7. [Quellenlage und Grenzen](#7-quellenlage-und-grenzen)

---

## 1. Wie dieses Dokument zu lesen ist

### 1.1 Statusklassen

| Klasse | Bedeutung | Kennzeichnung in den Dokumenten |
|---|---|---|
| **Bestätigt / gesperrt** | Ausdrücklich vom Entwickler entschieden. Wird nicht ohne neue ausdrückliche Entscheidung geändert. | 📋 (bei „gesperrt“ zusätzlich im Text) |
| **Vorläufig** | Bevorzugter Arbeitsstand oder Testszenario. Darf nicht als Festlegung gelesen werden. | 🧭 |
| **Vorschlag** | Vorschlag eines Assistenten oder Entwurfs, den der Entwickler nicht bestätigt hat. | 🧭 |
| **Offen** | Bewusst nicht entschieden. | ❓ |
| **Überholt** | Frühere Planung, die später ersetzt wurde. | 🕰️ |
| **Vision** | Langfristiges Ziel ohne Spezifikation. | 🔭 |

### 1.2 Vorrangregeln

1. Spätere ausdrückliche Entscheidungen des Entwicklers gehen früheren vor.
2. Das aktuelle Kanon-Dokumentenpaket (Statusdokumente, Nachträge, Entscheidungsregister) geht älteren Archiven, Prompts und Experimenten vor.
3. Ein **neueres Datum** macht eine Aussage nicht automatisch verbindlich; maßgeblich sind die Statuswörter im jeweiligen Abschnitt.
4. Beispiele, Schätzungen, Annahmen und Vorschläge von Assistenten werden **nicht** zu Entscheidungen.
5. Verwaiste oder fehlende Rohdaten werden nicht erfunden.
6. Die ältere Welt (abgeschlossen) und die größere Hauptwelt (in Planung) werden getrennt behandelt; es werden keine Geometrien übertragen.

---

## 2. Bestätigt und gesperrt

Die Spalte *Quelle* nennt die Art und das Datum des Dokuments, nicht den Dateipfad.

### 2.1 Produkt und Technik

| Thema | Festlegung | Quelle |
|---|---|---|
| Marke | Originale IP; keine Übernahme von Namen, Figuren, Lore, Kosmologie, Symbolen, Waffen, Geschichten oder Designs bestehender Werke; rechtliche Markenprüfung vor Veröffentlichung | Entscheidungsregister, Stand 31.08.2026 |
| Engine und Sprache | Unreal Engine plus C++20; C++-first; engine-neutrale autoritative Simulationsgrenzen, wo sinnvoll; eigene Persistenz-/Weltzustandsarchitektur; keine eigene vollständige Engine; keine stille Migration auf C++23 | Kanon 16.09.2026, Nachtrag 15.10.2026 |
| Produktionsstrategie | KI-gestützte Entwicklung durch einen Entwickler (etwa 8–15 Stunden pro Woche), volle Langzeitvision bleibt erhalten, frühe sichtbare spielbare Ergebnisse, Kennzeichnung `[VS]`/`[EARLY]`/`[LATER]`/`[FUTURE/AI]` ist Priorität und keine Streichung | Statusdokument 13.10.2026, Nachtrag 15.10.2026 |
| Modding | Anbieterunabhängiger Kern; lokales `.spnmod` und `<GameRoot>/mods/`; Aktivierung pro Profil und Spielstand; Steam Workshop nicht Pflicht; Mods additiv; offizieller Online-Modus ohne Mods | Block-5-Bestätigung 11.09.2026 |
| Mehrspieler | Koop-Kampagne bis 4 Spieler (Richtung); Einzelspieler offline; Koop-Kampagne mit Mods/Cheats ohne Transfer in den Online-Fortschritt; offizieller Online-Modus ohne Mods/Cheats | Entscheidungsregister 31.08.2026, Nachtrag 15.10.2026 |
| Persistenz | Zentraler Weltzustand, stabile IDs, segmentierte, absturzsichere, migrierbare Spielstände | Entscheidungsregister 31.08.2026 |
| Testphilosophie | Tests sind Teil der Architektur, nicht Schlussphase | Entscheidungsregister 31.08.2026 |
| Distribution | Zusatzpakete (Stimme, Texturen, Sprachen) wählbar; Standardinstallation nicht aufblähen | Kanon 10.09.2026 |

### 2.2 Welt

| Thema | Festlegung | Quelle |
|---|---|---|
| Ältere Welt | Geografisch und optisch abgeschlossen; Makrogeografie gesperrt; additive Inhalte erlaubt; kein klassisches DLC; Referenz- und Technik-Master | Kanon 04.–10.09.2026, Nachtrag 15.10.2026 |
| Größere Hauptwelt | Standard-Hauptwelt des Basisspiels; **keine Endgenerierung vor vollständigen Anforderungen und räumlicher Zuordnung** | Kanon 05.–16.09.2026, Nachtrag 15.10.2026 |
| Weltgröße | ca. 2.000 km² weiches Ziel, 2.200 km² Szenario; kein Geometrie-Lock | Nachtrag 15.10.2026 |
| Städte | Vorbilder: Stimmung und Vielfalt von vier bekannten amerikanischen Großstädten, keine realen Flächen; die früher abgeleitete Gesamtstadtfläche von 570 km² als Entscheidung abgelehnt; plausible Nachbarschaften, Hügel, Vegetation, Parks, Flüsse, organische Straßen | Statusdokument 13.10.2026, Nachtrag 15.10.2026 |
| Reiche | Himmel, Hölle und Fegefeuer als eigene Open Worlds im Basisspiel; Persistenz auch dort | Kanon 05.–10.09.2026 |
| Zeitreise | Großes Spielsystem; jahresgenau etwa 1600 bis zur Gegenwart; keine harte Untergrenze | Kanon 05./06.09.2026 |
| Geheimhaltung | Geheimes Hauptquartier wird auf öffentlichen Spielerkarten nicht auffällig beschriftet | Kartenanforderung 18./19.10.2026 |
| Karte | Eine geografisch maßgebliche Karte; abgeleitete Ansichten; technisch bestandene Kandidaten sind nicht automatisch akzeptiert | Kartenanforderung 18./19.10.2026 |

### 2.3 Spielinhalt

| Thema | Festlegung | Quelle |
|---|---|---|
| Spielgattung | Open-World-Action-Adventure mit RPG-, Ermittlungs-, Immersive-Sim- und Survival-Horror-Tiefe; keine sichtbaren Monster- oder Charakterlevel | Entscheidungsregister 31.08.2026 |
| Ton | Düsterer Mystery-Horror nach dem Vorbild der frühen Phase des Genres; später großer Umfang durch diese Linse | Entwicklerinterview 13.09.2026 |
| Protagonist | Ein frei erstellbarer Hauptcharakter, männlich/weiblich ohne Sperren; feste Hunter-Crew | Entscheidungsänderungen 25.–31.08.2026 |
| Spielbeginn | Ländlich, persönliche/tragische Vergangenheit, erste lokale Fälle; großes Hauptquartier erst nach mehreren Stunden Hauptgeschichte entdeckt | Entwicklerinterview 13.09.2026 |
| Story | Akte und Kapitel; stabile Handlungsachse; Hauptmissionen nie dauerhaft verpassbar; Spielerfreiheit ändert vor allem das Wie | Entscheidungsänderungen 25.–31.08.2026 |
| Fälle | Mehrwege-Ermittlungen, frühes korrektes Lösen möglich, Journal, Detektivmodus ohne Hilfe | Entscheidungsregister 31.08.2026 |
| Archiv | Wissensgraph; qualitative Anzeige; Wissensstatus (bestätigt/vermutet/umstritten/widerlegt/historisch) | Entscheidungsregister 31.08.2026 |
| Monster | Keine Respawn-Spawnpunkte; mehrdimensionale Klassifikation; Schwächen nicht nur als Bonusschaden | Entscheidungsregister 31.08.2026 |
| Living World, NPC-Gedächtnis und Nemesis | **Verbindlicher Bestandteil der Langzeitvision, kein M1-Feature:** persistente NPC-Identitäten, Langzeitgedächtnis, Nemesis-/Rivalitätssystem (auch für intelligente übernatürliche Kreaturen), autonome Fraktionen, persistente Konsequenzen, eigenständige Lebensentwicklung, skalierbare Weltsimulation mit Detailstufen, Integration ohne widersprüchliche Zustände. Umsetzung, Datenmodell und Zahlen sind offen. Das Wort „Nemesis“ steht bisher nicht im Master (neue Eingabe, Canon-Addendum folgt nach Entwicklerwunsch); die zugrunde liegenden Ideen stehen in Gameplay-Systeme §8 und Spielvision §9 | Entwickleraussage 22.10.2026; [Proposal 0010](../Proposals/0010-living-world-npc-memory-and-nemesis.md) |
| Medaillen | Bronze, Silber, Gold, Platin, Diamant; verborgene Kriterien; Replay ändert Story-Zustand nicht | Entscheidungsänderungen 25.–31.08.2026 |
| Soziale Medien | Nie Pflicht für Hauptgeschichte oder normale Nebenfälle; nur optionale dynamische Weltereignisse | Entscheidungsänderungen 25.–31.08.2026 |
| Fahren | Manuelle Kupplung, Abwürgen, freie Gangwahl; frühe versus langfristige Version; Handschalt-Variante für jedes Fahrzeug als Ziel | Entscheidungsregister 31.08.2026, Spezifikation 15.10.2026 |
| Untergrund (Gesellschaft) | Gesellschaftsentscheidungen U24–U36: Vertrauen, Haushalte, Lernen, Medizin, Lebensdauer, Verjüngung, KI-Stufen und individuelle Anerkennung, KI-Aufsicht je nach Aufgabe, gemischte Kommunikation | Entscheidungsnachträge 15.–18.10.2026 |
| Rail | Alle Züge innerhalb der Karte; Facility-Bereiche; hohe Elektrifizierung; Fahren nur mit Job/Berechtigung; Langzeitziel | Spezifikation 15.10.2026 |
| Adult-Bereich | Rahmen: getrenntes optionales Paket, Zustimmungs- und Altersregeln, Basisspiel nicht explizit; Detailplanung im Entwicklerinterview | Entscheidungsänderungen 25.–31.08.2026; Entwicklerinterview 28.08.–27.09.2026 |

---

## 3. Vorläufig

| Thema | Arbeitsstand | Anmerkung |
|---|---|---|
| Fläche der Hauptwelt | 2.200 km² trockenes Festland plus Ostinsel (80–100 km², 90 als Test) | Arbeitsziel der nächsten Kartenversion; kein abgeschlossenes Karten-Master |
| Metropolis | 110 km², 20 Bezirke, ca. 20 km² Innenstadt, ca. 12 km² Hochhauskern, 3–5 Supertalls (700–1.000 m) | Arbeitsmaß; frühere Testwerte (20–25 km²; 70–73 km² in einer älteren Planung) sind abgelöst |
| Größte Untergrundstadt | Horizontale Ausdehnung 50–100 km²; Bevölkerung mehrere Hunderttausend bis etwa 1 Million | ausdrücklich keine Geometrie-Festlegung |
| Reichsgrößen | Hölle 350–500 km² (Favorit 420), Fegefeuer 500–750 (650), Himmel 250–400 (320) plus Taschendomänen | Planungsbereiche |
| Hauptgeschichte | Umfang etwa 80–100 Stunden | Stand 31.08.2026, Planungsziel |
| Zeitskala | Etwa 2 Echtstunden pro Spieltag | nur eine Idee |
| Tierarten | 220–280 auf der Hauptkarte | Zielbereich |
| Kernarten von Monstern | Grob 20–30 sehr tiefe statt 100 flacher | bevorzugt, nicht gesperrt |
| Phasenziele | Phase 0 2–4 Monate (kleiner Platzhalter in 2–4 Wochen); Phase 1 ca. 1–3 km² und 30–60 Minuten; Phase 2 ca. 5–10 km², 3–5 Stunden; Phase 3 ca. 25–50 km², 10–20+ Stunden | Schätzungen, keine Termine |
| Online | 64–100 Spieler pro großer Weltinstanz | langfristiges Ziel |
| Basisspiel-Größe | etwa 100 GB | Richtung |
| Bevölkerung | mehrere Hunderttausend NPCs | Planungsgröße |
| Rail-Frequenzen | S-Bahn 3–5 Minuten, RE 10–15, RB 10–20, Zweigstrecken 15–30 | Dichteziele, kein Fahrplan |
| Adult-Einstellungen | Tabelle von Einstellungen und Darstellungsmodi | Entwurf aus dem Rahmenentwurf |
| Rennsport | Rallyes mit 10–20 Etappen, Stadtkurse, ein gefährliches Schotter-Bergrennen, mindestens 5 Strecken | Planungsgespräch |
| Eingabe (DualSense) | R2/L2, L1 + L2 als analoge Kupplung | technische Machbarkeit ist im Prototyp zu prüfen |

---

## 4. Vorschläge, die nicht ratifiziert sind

Diese Punkte standen in Planungspaketen als **Ingenieursvorschläge** oder als Beispiele und wurden **nicht** bestätigt:

- **KI und Kommunikation im Untergrund:** Trennung von Aufgabenumfang, Autonomie, möglichem Bewusstsein, individueller Anerkennung und Aufsicht; kritische Infrastruktur mit risikoabhängigen Schutzvorkehrungen; Geräte, räumliche Projektionen, Raumsysteme und freiwillige neuronale Schnittstellen als austauschbare Frontends mit Einwilligung und Privatsphäre. Keine Annahme von Implantaten, Oberflächen-Internet, Latenzzielen oder perfekter Verfügbarkeit.
- **Eisenbahn und Straße:** Detaillierte Infrastruktur, Signalisierung, Fahrpläne, Wartung, NPC-Betrieb, Fahrphysik, Kupplung, Getriebe, Schäden und Tuning sind **Prüfeingaben**, keine bestätigten Zahlen.
- **Unterschiedliche Ingenieurmodelle:** Vier unabhängige Zustände für Alter, biologisches Alter, Behandlungen, Interessen, Haushalt und KI-Autonomieklasse; vier Story-Variablen für Schlüsselherkunft, Entdeckung, Vertragsstatus und Identitäts-Exposition; ein globaler Vertrauens-Score als Anti-Muster.
- **Erscheinungsbild (Phase 0.4):** Ein reiner Datenvorschlag für Figurenaussehen; keine endgültige Struktur.
- **Weltgrößen-Beispiele:** Rechenbeispiele zur Dichte der Untergrundstadt (z. B. 30 km² / 600.000) sind arithmetische Beispiele und keine Empfehlungen.
- **Prompt-Bibliothek:** Alle rund 850 katalogisierten Arbeitspakete-Entwürfe haben den Status „Entwurf, Neuschreibung nötig“ und sind **keine Anforderungen**.
- **Berichte über eigene Software:** In Planungspaketen berichtete Testergebnisse sind nicht reproduziert.

---

## 5. Offene Entscheidungen nach Themen

### 5.1 Produkt, Welt und Story

- Endgültiger Spieltitel, Logos, Marken.
- Endgültige Weltfläche und Geometrie; Karten-Master; Terrain, Gewässer, Gebirge, Küsten, Straßen, Siedlungszahl, Städte, Bezirke (und Namen), Regionen.
- Vollständige Hauptgeschichte: Akt-/Kapitelzahl, Gliederung, benannte Bedrohungen, Kosmologie, Besetzung der Crew mit Biografien, Todesfällen, Beziehungen und Spezialitäten, auslösendes Ereignis, Endspiel.
- Zahl und Art der Nebenfälle; konkrete Fälle.
- Hauptquartier (Ort, Aufbau, Ausbaufolge, Umzüge) und Feldbasen.
- Todes-/Beinahe-Tod-/Rettungs-/Krankenhaus-/Checkpoint-Verhalten und die Zuordnung zu Schwierigkeit und Permadeath.
- Kalender-/Jahreszeitenlogik und wie viel Story-Zeit vergeht.

### 5.2 Untergrund und Reiche

- Größe, Höhenstufen, Lage, Bevölkerung, Dichte, Kavernen, Machbarkeit der größten Untergrundstadt; Zugangsregeln, Verkehrsrechte; Untergrund-Netz und Anbindung an die Oberfläche; Beziehung der Jäger-Einrichtungen zu den Städten.
- Bewusst **reservierte** Story-Fragen (ihre Beantwortung ist Teil der Hauptgeschichte und wird nicht vorweggenommen).
- KI-Rechte, Bewusstsein, Notfallbefugnisse; Sicherheit und Identität neuronaler Schnittstellen.
- Reichsgrößen, Grenzen, Namen, Kosmologie, politische Strukturen, Zeitverhältnisse, Gedächtnisdomänen, Transport und Schnellreise, Jenseits-Ablauf.
- Zeitreise: früheste voll ausgearbeitete Zeit, Zukunftsreisen, Verzweigung von Zeitlinien.

### 5.3 Fortschritt, Körper und Überleben

- Vollständiges Fortschrittsmodell jenseits von Benutzung/Training und abnehmenden Erträgen.
- Formeln für Körperzusammensetzung, Muskelzuwachs/-verlust, Kraft, Kardio, Müdigkeit, Erholung, Flüssigkeit, Schlaf, Stress, Ernährung; Geschlechtsunterschiede ohne Pauschalmultiplikatoren; Medizin-, Drogen- und Diagnoselisten; Krankenhauswirtschaft; Tiefe von Rauchen/Alkohol/Abhängigkeit.

### 5.4 Tiere, Jagd, Monster

- Artenliste und Klassenverteilung; Studienkriterien; seltene Einzeltiere; Jagdwirtschaft; Alterung, Fortpflanzung, Wanderung; Monstertaxonomie, Anatomien, Regeneration, Schwächen, Resistenzen, Ernteregeln; Hypothesen-/Bestätigungsschwellen und falsche Klassifikation.

### 5.5 Fahrzeuge und Rennen

- Startfahrzeug-Liste, Marken, Baujahre, Ausstattung; Kupplungs-Eingabe auf Tastatur/Controller und Zugänglichkeits-Hilfen; Tiefe von Kupplungs-, Motor-, Getriebe-, Reifenverschleiß und Thermik; Kraftstoff-/Ladewirtschaft; Wartungswirtschaft; Fahrzeugvertrautheit; Rennklassen, Homologation, Strecken jenseits von fünf, Meisterschaftsstruktur, Rennbelohnungen; Abschlepp-/Seilwinden-Bedienung und Persistenz; Schadensmodell.

### 5.6 Kampf, Verletzung, Gore

- Combat-Focus-Aktivierung/Eingabe/Dauer/Erholung; ballistische, Gewebe-, Knochen- und Rüstungsparameter; Gore-Schwellen und visuelle Familien; Darstellung bei minderjährigen NPCs nach Plattform-/Rechtsprüfung; Tiefe von Nahkampf/Ringen/Aufgabegriffen und Animationsabdeckung.

### 5.7 Recht, Wirtschaft und Medien

- Straftatenkatalog, Festnahmeschwellen, Strafen, Kaution, Zuständigkeitskarte, Informationsaustausch-Verzögerungen, Verdachts- und Beweismodelle, Überwachungsdichte, Spürhund-Modell, Durchsuchungsregeln, Polizeikarte je Schwierigkeit.
- Gesamtbalance der Wirtschaft, Bargeld-Anzeige, Lagergrenzen, Pfand-/Hehlerbestand, Crafting, Dienstleistungspreise.
- Spielnetzwerk (Name, Oberfläche, Ranking), prozedurale Beiträge, Ereignisgenerator, Verhältnis handgeschrieben/generiert, Streaming-/Beweisfolgen, Telefon-Apps.

### 5.8 Erfolge und Online

- Gewichtung der Medaillen, Zielanzeige nach Abschluss, Zahl und Liste der Trophäen und Erfolge, Online-/Grounded-/Permadeath-/DLC-Gruppen, Platin-Anforderungen.
- Zahl der Online-Slots, Spielerzahl, Backend-Topologie (Sharding, Instanzen, Reisen), Persistenzschema, Matchmaking, Crossplay, Feindseligkeits-/Verteidigungsmodus/Kopfgeld, Geschäfte und Rollen, Sprachchat-Routing, Anti-Cheat.
- Kreativwirtschaft: Umsatzaufteilung, Förderfonds, Creator-Codes, Auszahlungsanbieter, kuratierter Weg für Online-Inhalte, Mod-Hosting, Moderation, Lua oder andere Skriptebene, Signierrichtlinie für native Mods.
- Echtgeld-Beschleunigung, Pakete, Preise und Schutz vor Pay-to-Win.

### 5.9 Grafik, Audio und Vertrieb

- Hardwareanforderungen, Auflösungen und Bildraten, Raytracing-Funktionen, Path-Tracing-Verfügbarkeit, 8K, PS5-Pro- und Series-S-Modi, Upscaling-/Frame-Generation-Matrix.
- Installationsgröße je Plattform, Disc-Aufteilung, Kompression, Sprachpaket-Politik.
- Codecs, Raum-/Portal-Architektur, Speicher-Budgets, Aufnahmeausrüstung, Dialogzahlen, Dialogdatenbank, Besetzung der Stimmen (Freunde/Familie/Profis/Mischung), Stimmklon-Anbieter und Verträge, Verhältnis von Darbietung zu Klon zu Sprache-zu-Sprache, Synchronsprachen.
- Preis und Zugang der Guide-Plattform.

### 5.10 Adult-Bereich

Siehe [Offene Fragen und Konflikte](Adult/11-Offene-Fragen-und-Konflikte.md) (Einstieg: [Adult-System](Adult-System.md)): Vertriebsweg und Altersfreigabe, Paketzuschnitt, Abschluss des Taxonomie-Audits und der externen Gegenprüfung, Inhaltskataloge, Stimmenproduktion, Steuerung (Direkt/Assistiert), Gesundheitswerte, zeitliche Einordnung.

### 5.11 Technik und Recht (Repository)

- Lizenz und Veröffentlichungsumfang; Prüfung der Vorlagen- und Engine-Inhalte; Menschen-Pipeline; Export-Makro oder Brückenschicht für Core-Typen; Speicherformat v2 (Kompaktierung), Metadaten, Reichstrennung, UTF-8-Prüfung, Compare-and-Set; Netzwerkarchitektur.

---

## 6. Widersprüche und überholte Planung

### 6.1 Weltgröße

| Datum | Aussage | Status |
|---|---|---|
| bis Ende August 2026 | ca. 200–300 km² | 🕰️ |
| 04.–08.09.2026 | Standardwelt 1.500–1.800 km² (Favorit ca. 1.650) | 🕰️ |
| 16.–27.09.2026 | ca. 2.000 km² weiches Ziel | 🕰️ |
| 13.–15.10.2026 | 2.000–2.200 km², offener Rahmen | 🧭 |
| 18.–19.10.2026 | 2.200 km² Festland plus Insel als Kartenkandidat | 🧭 aktuell |

Die Zahlen sind aufeinanderfolgende Planungsstände, **keine** gleichzeitigen Aussagen. Das Wort „Hauptwelt“ bezeichnet in allen Fällen die in Planung befindliche größere Welt, nicht die ältere abgeschlossene.

### 6.2 Metropolis-Fläche

70–73 km² (September, ältere Weltgröße) → 20–25 km² (13./15.10., Testszenario) → 110 km² (18.10., Arbeitsmaß der aktuellen Kandidatenkarte). Die 110 km² sind ausdrücklich keine dauerhafte Weltkonstante; bei anderen Karten-Größenvarianten ist die Stadtfläche neu zu planen.

### 6.3 Die Beziehung zwischen Untergrund-Einrichtungen und Hauptquartier

Mehrere Planungsdokumente erwähnen sowohl eine gesicherte unterirdische Einrichtung als auch ein oberirdisches Hauptquartier. Ob sie identisch, getrennt oder verbunden sind, ist **nicht belastbar dokumentiert** und bleibt ❓ offen. Es darf nicht stillschweigend zusammengeführt werden.

### 6.4 Größe der größten Untergrundstadt

Ein Tagesstand vom 14.10.2026 nennt die Größe als unbekannt; der Nachtrag vom 15.10.2026 nennt 50–100 km² als vorläufig. Das ist **kein Widerspruch**, sondern eine Verfeinerung; die Zahl bleibt vorläufig.

### 6.5 Hauptquartier-Beschriftung auf Karten

Einige erzeugte Kartenbilder vom 19.10.2026 zeigen noch eine auffällige „Hunter HQ“-Beschriftung. Das widerspricht der Geheimhaltungsregel und gilt als **Qualitätsmangel der Karte**, nicht als neue Freigabe.

### 6.6 Engine-Neutralität

Eine ältere Zielbestimmung (31.08.2026) wünschte systemkritische Teile möglichst engine-neutral, damit ein späterer Wechsel zu einer eigenen Engine realistischer würde. Der spätere Kanon (September/Oktober) legt dagegen fest: Unreal ist die Engine, es wird keine eigene Engine gebaut, und engine-neutrale Grenzen gelten **dort, wo sie nützen**. Die ältere Absicht wird als Option, nicht als Plan geführt.

### 6.7 Adult-Planung

- Ein Rahmenentwurf vom 28.08.2026 sah Darstellungsmodi inklusive „voll erlaubt“ und ein Online-Anfragesystem im Spielkern vor. Seit dem 31.08. und den September-Runden: erweiterter Teil ist ein **getrenntes optionales Paket**, das Basisspiel ist nicht explizit, der offizielle Online-Modus lädt keine Mods.
- Die Detailplanung ist vor allem im Entwicklerinterview dokumentiert (Runden 1–20) und wurde **noch nicht in die Kanon-Dokumente konsolidiert**.
- Frühe Entwickleräußerungen zu Zustimmung und Trunkenheit (10.–16.09.) sind durch spätere Festlegungen ersetzt; ein Wunsch nach einem internen Modus gegen den Willen von Figuren (20.09.) ist **nicht freigegeben** und wartet auf eine Entscheidung. Übersicht: [Offene Fragen und Konflikte](Adult/11-Offene-Fragen-und-Konflikte.md).

### 6.8 Sammelobjekte

Der frühere Wert „ca. 1.200–1.500 physische Sammelobjekte“ bezog sich auf ein älteres, kleineres Weltkonzept (nicht die abgeschlossene ältere Welt, wie zeitweise angenommen). Für die heutige Welt ist die Zahl offen und wird je Kategorie neu zugeordnet.

### 6.9 Weitere Hinweise

- Eine zusätzliche Eigenart ist die Spielerzahl-Angabe in einem Entwurf („64–100 Spieler“): Sie ist ein **Ambitionsziel**, kein Startversprechen.
- Zeitangaben und Planungsfortschritte, die in Planungsdokumenten genannt werden („Prozent der Planung abgeschlossen“), sind interne Schätzungen und werden hier nicht übernommen.
- Alle Ortsnamen auf Kartenentwürfen (Glenhaven, Ashford, Greywater, Lake Alder, Lake Meridian, South Reservoir, Greywater Bay) sind **Arbeitsbezeichnungen** und nicht endgültig.
- Prompt-Bibliotheken und Entwürfe sind keine Anforderungen.

---

## 7. Quellenlage und Grenzen

Dieses Dokument beruht auf dem Planungsmaster, soweit er für diese Aufbereitung gelesen wurde:

- **Gelesen:** der aktuelle Kanon-Stand (Statusdokumente, Nachträge vom 4. September bis 18. Oktober, Entscheidungsregister, Konfliktlisten), die Planungsdokumente der Entwicklungsphase Ende August (Entscheidungsregister, Offene-Entscheidungen-Liste, Zustandsdokumente, Planungs-Backlog), die Tagesstände vom 14. bis 19. Oktober (Kanon-, Spezifikations-, Design- und Offene-Fragen-Dokumente) sowie die Gesprächsprotokolle zur Adult-Planung (28. August bis 27. September 2026) und einzelne weitere Protokolle zu Spielwelt und Story.
- **Nicht gelesen:** der überwiegende Teil des Archivs (Gesprächs-Rohexporte, historische Zwischenstände, Rohdaten, Bilder, Geodaten, die Prompt-Entwürfe).
- Es wird **nicht behauptet**, das gesamte Archiv geprüft zu haben. Eine Aussage „nichts gefunden“ bezieht sich nur auf das Gelesene.
- Die Planungsdokumente sind teilweise Gesprächsprotokolle mit Assistenten. Dort stehende **Vorschläge der Assistenten** sind nicht übernommen, wenn der Entwickler sie nicht bestätigt hat.
- Der Planungsmaster wird bei dieser Aufbereitung **nicht verändert**.
