# Adult 40: Frage-Antwort-Matrix, Runden 17 bis 20 und Pre-Audit

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dies ist ein **Quellenprotokoll**, keine Spezifikation und kein Kanon. Es verzeichnet je Interviewfrage die (gekürzte) Frage, die Antwort des Entwicklers, die Fundstelle, einen **heuristischen** Status und den Registerbezug. Explizite Formulierungen der Quellen sind **automatisch abstrahiert**; der Wortlaut steht nur in der Primärquelle. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Methode, Grenzen und Gesamtzahlen: [Modul 41](41-Audit-Etappe-2-2.md). Quellenschlüssel: [Modul 13](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Entscheidungen: [Register (Modul 14)](14-Entscheidungsregister.md). Weitere Matrizen: [36](36-Frage-Antwort-Matrix-Runden-1-bis-9.md), [37](37-Frage-Antwort-Matrix-Runden-10-bis-13.md), [38](38-Frage-Antwort-Matrix-Runden-14-und-15.md), [39](39-Frage-Antwort-Matrix-Runde-16.md), [40](40-Frage-Antwort-Matrix-Runden-17-bis-20.md).

## Lesehilfe

| Spalte | Bedeutung |
|---|---|
| **Nr.** | Fragenummer **innerhalb des Pakets** (Paket = eine Fragenliste des Assistenten; Nummern wiederholen sich zwischen Paketen). |
| **Frage** | erste ca. 120 Zeichen der Frage; bei expliziter Formulierung durch einen Hinweis ersetzt. |
| **Antwort** | Antwort des Entwicklers, auf ca. 130 Zeichen gekürzt. „Bereichsantworten“ (z. B. „1-9 ja, …“) gelten für den ganzen Bereich; dieselbe Antwort erscheint dann mehrfach. `A → B` = spätere Nachreichung. `–` = in den Nachrichten keine Antwort gefunden. |
| **Quelle** | `35#Frage-Nachricht → 35#Antwort-Nachricht`. |
| **Status** | **heuristisch** aus dem Antworttext abgeleitet (siehe unten), nie eine Entscheidung dieser Dokumentation. |
| **Register** | Register-Einträge, deren Text diese Frage **ausdrücklich nennt** (Fragenummer oder Bereich in Aussage/Vermerk) und deren Quelle die Antwortnachricht ist. `–` heißt: **kein Eintrag nennt diese Frage namentlich**. Das bedeutet **nicht**, dass die Aussage fehlt; Paketebene steht im Kopf jedes Abschnitts. |

**Status-Werte:** `📋 ja` / `📋 ja mit Zusatz` / `📋 Freitext` = Zustimmung bzw. inhaltliche Antwort des Entwicklers, **nicht** automatisch zu Zahlen oder Details, die der Assistent als Beispiel nannte. `📋 Rückfrage → pauschal bestätigt` = Rückfrage des Entwicklers, danach ein knappes „Ja/Ok passt“ direkt nach der Erklärung. `🧭 bedingt/unverbindlich` = „kann sein“ o. ä. `🧭 delegiert` = „mach, was du gut findest“. `❓ offen/Rückfrage` = Rückfrage ohne belegte Bestätigung. `❌ nein`. `⚪ ohne Antwort`. Zusätze nach `·` verweisen auf Konflikte ([Modul 19](19-Konflikt-und-Widerspruchsmatrix.md)) oder Register-Einträge.

**Module:** 01 [Grundregeln](01-Grundregeln-Alter-Zustimmung.md) · 02 [Architektur](02-Architektur-Pakete-Technik.md) · 03 [Interaktion](03-Interaktion-Taxonomie-Ablauf.md) · 04 [Steuerung](04-Steuerung-Darstellung-Kamera-Audio.md) · 05 [NPCs](05-NPCs-Beziehungen-Praeferenzen.md) · 06 [Körper](06-Koerper-Physik-Gesundheit.md) · 07 [Charakteranpassung](07-Charakteranpassung-Tattoos-Koerpermodifikation.md) · 08 [Welt](08-Welt-Orte-Branche-Wirtschaft.md) · 10 [Produktion/Recht](10-Produktion-Recht-Vertrieb.md). Die Modulzuordnung gilt **auf Paketebene** (heuristisch, nicht je Frage belegt).

**Inhalt**

- [Pre-Audit A1–A15 (35#742)](#pre-audit-a1a15-35742)
- [Runde 17A: Tattoo-Grundmodell (35#748)](#runde-17a-tattoo-grundmodell-35748)
- [Runde 17B: Stile, Motive (35#760)](#runde-17b-stile-motive-35760)
- [Runde 17C: Künstler, Studios (35#766)](#runde-17c-künstler-studios-35766)
- [Runde 17D: Prozess, Heilung (35#768)](#runde-17d-prozess-heilung-35768)
- [Runde 17E: Cover-up, Laser (35#770)](#runde-17e-cover-up-laser-35770)
- [Runde 17F: NPC-Tattoo-Kultur (35#774)](#runde-17f-npc-tattoo-kultur-35774)
- [Runde 17G: Tattoo-Technik (35#778)](#runde-17g-tattoo-technik-35778)
- [Runde 17H: Piercings, Körpermodifikation (35#782)](#runde-17h-piercings-körpermodifikation-35782)
- [Runde 17I: Abschluss (35#784)](#runde-17i-abschluss-35784)
- [Runde 18A: Steuerung (35#790)](#runde-18a-steuerung-35790)
- [Runde 18B: VR (35#794)](#runde-18b-vr-35794)
- [Runde 18C: Bedienung (35#800)](#runde-18c-bedienung-35800)
- [Runde 19A: Technik (35#804)](#runde-19a-technik-35804)
- [Runde 19B: Speicher, Builds (35#806)](#runde-19b-speicher-builds-35806)
- [Runde 20A: Production Lock (35#810)](#runde-20a-production-lock-35810)

---


## Pre-Audit A1–A15 (35#742)

**Datum:** 2026-09-25 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#743, 35#745 · **Module (Paketebene):** 03 · **Status:** ❌ 1, ❓ 1, 📋 13  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-038, E-IA-039, E-WE-019

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Core Action Families: Sind diese Hauptfamilien als Basis vollständig genug: | ja | 35#742 → 35#743 | 📋 ja | – |
| 2 | Body-Focus als eigene Dimension: Sollen Dinge wie Brust-, Gesäß-, Fuß-, Haar-, Hand-/Finger-, Mund-/Gesichts-Fokus … | ja | 35#742 → 35#743 | 📋 ja | – |
| 3 | Clothing/Fashion Content: Brauchen wir als eigene Familie Clothing/Fabric Interaction, weil Kleidung, Teilentkleidung, … | ok | 35#742 → 35#743 | 📋 ja | – |
| 4 | Power/Dynamic Family: Sollen Dominanz, Unterordnung, Initiative, Kontrolle und Rollenwechsel eine eigene … | ja | 35#742 → 35#743 | 📋 ja | – |
| 5 | Sensation Family: Haben wir mit Pain, Temperatur, Textur, Druck, Sicht, Sound, Geruch usw. genug, oder sollen wir … | genauer sagen | 35#742 → 35#743 | ❓ offen/Rückfrage | – |
| 6 | Fantasy vs. Activity: Soll Fantasy/Roleplay weiterhin strikt getrennt werden von der tatsächlich ausgeführten Action? … | ja | 35#742 → 35#743 | 📋 ja | – |
| 7 | Media/Content Production: Reicht Recording/Observation + Creator/Professional Context, oder brauchen wir eine größere … | ja genau, anfangs klein und immer wachsend | 35#742 → 35#743 | 📋 ja mit Zusatz | – |
| 8 | Commerce/Service: Long-term Companion, Escort, Bordell, Creator, Performer usw. — sollen die nicht als … | ja | 35#742 → 35#743 | 📋 ja | – |
| 9 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Pre-Audit A1–A15] | nein noch genauer | 35#742 → 35#743 | ❌ nein | – |
| 10 | Social Outcomes: Soll es zusätzlich eine gemeinsame Outcome-Familie geben für Nähe, Zufriedenheit, Enttäuschung, … | ja | 35#742 → 35#743 | 📋 ja | – |
| 11 | Location Context: Haben wir mit Private Home · Hotel/Motel · Vehicle · Outdoor · Workplace · Adult Venue · Studio · … | ja | 35#742 → 35#743 | 📋 ja | – |
| 12 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Pre-Audit A1–A15] | ja | 35#742 → 35#743 | 📋 ja | – |
| 13 | Participant Structure: Reicht Solo · Pair · Group + normaler Interaction Graph, oder brauchen wir weitere … | ja | 35#742 → 35#743 | 📋 ja | – |
| 14 | Content Intensity: Sollen soft / rough / extreme niemals Kategorien werden, sondern weiterhin nur Resultat aus … | ja | 35#742 → 35#743 | 📋 ja | – |
| 15 | Missing-Family-Frage: Gibt es aus deiner Sicht jetzt irgendeine komplette Art von Adult-Content, bei der du sagst: „Das … | fällt mir gerade nicht ein aber schlage was vor aber ich dachte wir machen das erst mit qwen (35#743) → die sachen passen gut (35#745) | 35#742 → 35#743,35#745 | 📋 Freitext (nachgereicht/geändert) | – |

## Runde 17A: Tattoo-Grundmodell (35#748)

**Datum:** 2026-09-25 · **Fragen:** 1–20 (20) · **Antwortnachrichten:** 35#749, 35#751 · **Module (Paketebene):** 07 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-RC-010, E-TA-014, E-TA-015, E-TA-016

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Tattoo als echter Character-State: Soll jedes Tattoo ein persistentes Objekt am Charakter sein mit Motiv, Position, … | ja sicher, es kann auch erneuert, gelasert, geblackouted, coverup, … | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 2 | Freies Placement: Soll Tattoos grundsätzlich fast überall auf der äußeren Haut platzierbar sein, statt nur auf … | ja auf jedenfall, der spieler kann dem tätoowierer sagen was er will der stellt das zusammen und das schaut man ob es passt und … | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 3 | Surface Coordinates: Soll die Position intern über Körperoberflächen-Koordinaten gespeichert werden, damit das Tattoo … | ja aber wenn man ein tattoo am bauch hat und dann schwanger wird dann dehnt sich das aus, und wenn man sich schwanger tattoowiert … | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 4 | Coverage kontinuierlich: Soll Tattoo-Coverage wirklich von 0 bis nahezu 100 % kontinuierlich möglich sein, ohne feste … | ja natürlich, jedes tattoo bestimmt das dazu | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 5 | Coverage pro Körperregion: Soll zusätzlich gespeichert werden, wie stark einzelne Bereiche bedeckt sind, z. B. linker … | ja kann es schon | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 6 | Überlappungen: Sollen Tattoos sich überlappen dürfen, wenn jemand später bewusst etwas darüber stechen lässt? | ja sicher es kann auch über tattoos komplette tattoos geben zb Blackouts oder blackouts und dann noch weiße motive darüber,… | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 7 | Layer History: Soll dabei erhalten bleiben, welches Tattoo zuerst da war, damit Cover-ups, Fading und spätere … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 8 | Negative Space: Soll Negative Space – bewusst freie Haut zwischen Tattoos – als geplantes Gestaltungselement erkannt … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 9 | Filler Tattoos: Sollen kleine Motive, Ornamente, Muster oder Blackwork gezielt verwendet werden können, um … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 10 | Sleeve als Komposition: Soll ein Sleeve nicht nur „viele Tattoos am Arm“ sein, sondern optional als zusammenhängende … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 11 | Patchwork Sleeve: Soll gleichzeitig auch ein Patchwork Sleeve möglich sein – viele getrennte Tattoos verschiedener … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 12 | Full-Body-Projekte: Soll ein Charakter große Projekte planen können, die über mehrere Sessions/Jahre wachsen – z. B. … | natürlich das ist sogar gewünscht oder jedenfalls sehr gut möglich, es werden ja auch einige npcs full body tattoos haben, … | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 13 | Symmetrie: Soll ein Tattoo-Design symmetrisch sein können, aber niemals automatisch gespiegelt werden müssen? … | Achso ja | 35#748 → 35#751 | 📋 ja | – |
| 14 | Anatomie beeinflusst Design: Soll ein guter Artist das Motiv an Muskeln, Knochenlinien, Körperform und Bewegung … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 15 | Körperveränderung: Soll Gewichtszunahme/-abnahme, Muskelaufbau, Schwangerschaft und Alter Tattoos tatsächlich … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 16 | Narben und Hautzustand: Soll Placement berücksichtigen, ob dort Narben, Muttermale, Hautverletzungen oder ältere … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 17 | Tattoos über Narben: Soll über verheilte Narben tätowiert werden können, wobei Narbenart/-alter beeinflusst, wie gut … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 18 | Tattoo-Dichte als NPC-Merkmal: Soll die Weltpopulation unterschiedliche Tattoo-Coverage-Verteilungen haben – wenig … | ja sicher, auf den land sind tattoos selten, bei alten leuten die nicht im gefängnis waren auch. Bei jungen Personen … | 35#748 → 35#749 | 📋 ja mit Zusatz | E-TA-014 |
| 19 | NPC entwickelt sich weiter: Soll ein NPC über Jahre selbst neue Tattoos bekommen, bestehende erweitern, Cover-ups … | ja | 35#748 → 35#749 | 📋 ja | E-TA-014 |
| 20 | Character Creator: Soll der Character Creator bei aktivierter Adult-/Extended-Character-Mod deutlich mehr … | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#748 → 35#749 | 📋 Freitext · K17 | E-TA-014 |

## Runde 17B: Stile, Motive (35#760)

**Datum:** 2026-09-26 · **Fragen:** 21–40 (20) · **Antwortnachrichten:** 35#761, 35#763 · **Module (Paketebene):** 07 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-TA-017, E-TA-018

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 21 | Stil-System: Soll Tattoos nicht nur ein simples style = realism bekommen, sondern mehrere kombinierbare Stilmerkmale, … | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#760 → 35#761 | 📋 Freitext | E-TA-017 |
| 22 | Realism: Soll Realism extrem detailliert sein – Portraits, Tiere, Objekte, Horror-Motive usw. – wobei Artist-Skill … | ja genau, ein schlechter tattowierer kann weniger gut das tätowieren als ein weltweit bekannter profi, dafür sind die preise auch … | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 23 | Black & Grey: Soll Schwarz/Grau eine eigene große Richtung sein, mit feinen Abstufungen, Washes und unterschiedlichen … | 26. es soll alles geben können was man sich vorstellen kann, blackout kann man auswählen aber beim tättowierer sieht man eine … | 35#760 → 35#761 | 📋 Freitext | E-TA-017 |
| 24 | Color Tattoos: Soll Farbe echte Pigmenteigenschaften besitzen – Farbton, Sättigung, Helligkeit, Deckkraft – und je nach … | (Block 23–26) es soll alles geben können was man sich vorstellen kann, Blackout wählbar, beim Tätowierer Vorschau von sich: … | 35#760 → 35#761 | 📋 Freitext | E-TA-017 |
| 25 | Blackwork: Soll Blackwork von kleinen schwarzen Flächen bis zu sehr großen massiven schwarzen Bereichen reichen können, … | (Block 23–26) es soll alles geben können was man sich vorstellen kann, Blackout wählbar, beim Tätowierer Vorschau von sich: … | 35#760 → 35#761 | 📋 Freitext | E-TA-017 |
| 26 | Blackout: Soll Blackout ausdrücklich ein eigener Tattoo-Typ sein, mit dem bestehende Tattoos vollständig überdeckt … | (Block 23–26) es soll alles geben können was man sich vorstellen kann, Blackout wählbar, beim Tätowierer Vorschau von sich: … | 35#760 → 35#761 | 📋 Freitext | E-TA-017 |
| 27 | White-over-Black: Soll Weiß über verheilter schwarzer Fläche möglich sein, aber abhängig von Technik, Artist, Heilung … | ja (in 761 versehentlich gelöscht, in 763 nachgereicht) | 35#760 → 35#763 | 📋 ja mit Zusatz | – |
| 28 | Traditional / Neo-Traditional: Sollen klassische Traditional- und modernere Neo-Traditional-Stile getrennt erkennbar … | ja (in 761 versehentlich gelöscht, in 763 nachgereicht) | 35#760 → 35#763 | 📋 ja mit Zusatz | – |
| 29 | Japanese: Soll japanisch inspirierte Tätowierung als großer Stil mit zusammenhängender Körperkomposition, … | ja viele yakuza haben das, mafia soll es bei uns auch geben. | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 30 | Fine Line / Minimal: Soll es auch sehr feine, kleine Tattoos geben, deren Linienqualität besonders stark von … | ja soll es öfters bei jungen frauen glaub ich, solche schmetterlinge und sowas | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 31 | Geometric / Ornamental: Sollen geometrische, Mandala-, ornamental-/pattern-basierte Tattoos echte … | ja | 35#760 → 35#761 | 📋 ja | E-TA-017 |
| 32 | Lettering: Soll Text/Schrift eine eigene Unterkategorie haben – Font/Stil, Sprache, Größe, Ausrichtung und frei … | ja | 35#760 → 35#761 | 📋 ja | E-TA-017 |
| 33 | Portraits: Soll ein Portrait auf Basis eines vorhandenen NPCs, Fotos oder Referenzbilds erstellt werden können, sodass … | auf jeden fall, das kann sehr gut werden | 35#760 → 35#761 | 📋 ja | E-TA-017 |
| 34 | Custom Motif Composition: Soll man mehrere Motive kombinieren können – z. B. Portrait + Rosen + Schädel + Schrift + … | ja wie gesagt soll es eine preview geben und dann sieht man halt wo welches tattoos hinkommt und wie es ausschaut neben den … | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 35 | Artist Interpretation: Soll der Spieler zwischen „genau wie meine Vorlage“ und „Artist darf kreativ interpretieren“ … | ja, der spieler sagt was er will und der artist macht die vorlage, wenn es passt gut, sonst macht der spieler oder die npcs … | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 36 | Placement Preview: Soll der Artist vor dem Stechen eine echte Stencil-/Preview-Version auf dem Körper zeigen, die man … | ja das meine ich mit preview in paar vorigen punkten | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 37 | Freehand Tattoos: Soll es Artists geben, die bestimmte Designs direkt freehand auf den Körper zeichnen, statt mit … | ja kann es sein aber kommt auf die person an | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 38 | Mixed Styles: Soll ein Körper problemlos viele komplett unterschiedliche Tattoo-Stile von verschiedenen Artists tragen … | ja definitiv, wenn es der person gefällt sicher | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 39 | Cohesive Full-Body Design: Soll jemand alternativ einen langfristigen Full-Body-Masterplan erstellen lassen können, bei … | ja 100% sehr wichtig, es sollen einige leute einen fullbodysuit haben, in der stadt mehr als am land, aber das haben wir schon … | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |
| 40 | Motive praktisch frei: Soll grundsätzlich gelten, dass nahezu jedes visuell darstellbare Motiv tätowiert werden kann – … | ja genau, man kann selber auswählen was man will oder man sagt dem tättowierer ein motiv und der stellt das zusammen wenn er es … | 35#760 → 35#761 | 📋 ja mit Zusatz | E-TA-017 |

## Runde 17C: Künstler, Studios (35#766)

**Datum:** 2026-09-26 · **Fragen:** 41–60 (20) · **Antwortnachrichten:** 35#767 · **Module (Paketebene):** 07 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-TA-019

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 41 | Artist-Skill als mehrere Werte: Soll ein Tätowierer nicht einfach Skill = 87 haben, sondern getrennte Fähigkeiten für … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 42 | Spezialisierung: Soll ein Artist in einem Stil weltklasse sein können, aber in einem anderen nur mittelmäßig? Also z. … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 43 | Artist-Handschrift: Soll jeder gute Artist zusätzlich eine eigene erkennbare Handschrift haben, sodass zwei Top-Artists … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 44 | Nachwuchs bis Weltstar: Soll es eine echte Karriereleiter geben – Apprentice → Junior → erfahrener Artist → bekannter … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 45 | Preise: Soll Preis aus Artist-Reputation + Skill + Stil + Größe + Komplexität + Körperstelle + benötigten Stunden + … | ja schon aber auch die location, in der downtown wird es viel mehr kosten als auf dem land und so weiter | 35#766 → 35#767 | 📋 ja mit Zusatz | E-TA-019 |
| 46 | Stundensatz vs. Fixpreis: Soll es beides geben – manche Artists rechnen pro Stunde/Sitzung, andere geben für bestimmte … | damit kenne ich mich nicht aus aber klingt plausibel | 35#766 → 35#767 | 📋 Freitext | E-TA-019 |
| 47 | Weltklasse extrem teuer: Soll ein weltbekannter Artist sehr teuer sein und möglicherweise monatelange oder sogar … | ja, aber die wartezeiten machen wir nicht so lange | 35#766 → 35#767 | 📋 ja mit Zusatz | E-TA-019 |
| 48 | Terminplanung: Soll man Tattoos real über Website, Telefon, Walk-in oder persönlich im Studio buchen können, abhängig … | ja genau, jedes studio hat eigene regeln und planung | 35#766 → 35#767 | 📋 ja mit Zusatz | E-TA-019 |
| 49 | Consultation: Soll bei größeren Tattoos vorher ein Beratungstermin stattfinden können, bei dem Motiv, Placement, Stil, … | ja das meinte ich ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 50 | Deposits: Soll bei guten Studios häufig eine Anzahlung verlangt werden, die bei Nichterscheinen teilweise/verloren sein … | ja kann man machen | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 51 | Walk-ins: Soll es Studios geben, die kleine Tattoos spontan ohne Termin machen, während große Projekte fast immer … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 52 | Studio-Reputation: Soll ein Studio eine eigene Reputation haben, getrennt von den einzelnen Artists – z. B. Hygiene, … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 53 | Studio-Typen: Soll es unterschiedliche Studiotypen geben – kleine lokale Shops, High-End-Studios, Spezialisten-Studios, … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 54 | Hygiene separat von Skill: Soll ein technisch guter Artist trotzdem schlechte Hygiene haben können – und umgekehrt ein … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 55 | Schlechte Studios: Soll es auch richtig schlechte/billige Studios geben, bei denen Ergebnis, Hygiene, Beratung oder … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 56 | Preis ≠ Qualität: Soll ausdrücklich gelten: teurer heißt nicht automatisch besser und billig heißt nicht automatisch … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 57 | NPC-Empfehlungen: Sollen NPCs bestimmte Artists empfehlen können, weil sie selbst dort waren, Freunde dort kennen oder … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 58 | Social Media / Portfolio: Soll jeder relevante Artist ein Portfolio im In-Game-Internet haben können, mit Fotos … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 59 | Famous Clients / Culture: Soll ein Artist durch bekannte Musiker, Sportler, Kriminelle, Influencer oder andere NPCs … | ja | 35#766 → 35#767 | 📋 ja | E-TA-019 |
| 60 | Artist-Wechsel: Soll ein langfristiges Tattoo-Projekt auch von mehreren Artists weitergeführt werden können – entweder … | ja schon, aber vielleicht nicht immer sinnvoll, wenn das tattoo schon einen teil fertig hat und dann ein anderer weitermacht kann … | 35#766 → 35#767 | 📋 ja mit Zusatz | E-TA-019 |

## Runde 17D: Prozess, Heilung (35#768)

**Datum:** 2026-09-26 · **Fragen:** 61–80 (20) · **Antwortnachrichten:** 35#769 · **Module (Paketebene):** 07 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-TA-020

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 61 | Tattoo-Sessions: Soll ein großes Tattoo automatisch in mehrere Sitzungen zerlegt werden, abhängig von Größe, … | nicht zwingend aber so gut wie immer | 35#768 → 35#769 | 📋 Freitext | E-TA-020 |
| 62 | Session-Dauer: Soll eine Sitzung unterschiedlich lang sein können, z. B. kurze 30–60-Minuten-Termine bis zu mehreren … | ja sicher, aber das ist dann eine timelapse weil keiner will 5h zuschauen wie er tättowiert wird | 35#768 → 35#769 | 📋 ja mit Zusatz | E-TA-020 |
| 63 | Schmerz nach Körperstelle: Soll jede Körperregion eine eigene typische Schmerzempfindlichkeit besitzen, zusätzlich zur … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 64 | Individuelle Reaktion: Soll dieselbe Körperstelle bei zwei NPCs völlig unterschiedlich empfunden werden können, statt … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 65 | Schmerz während langer Sessions: Soll Schmerz/Empfindlichkeit im Verlauf einer Sitzung steigen können, weil Haut … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 66 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Prozess, Heilung] | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 67 | Artist-Pausen: Soll auch der Tätowierer Pausen brauchen können, insbesondere bei langen Sessions, weil … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 68 | Fresh Tattoo State: Soll ein frisch gestochenes Tattoo einen eigenen sichtbaren Zustand haben: Rötung, Schwellung, … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 69 | Healing Phases: Soll Heilung mehrere sichtbare Phasen haben – frisch → gereizt → trockener/Schuppen → weitgehend … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 70 | Aftercare: Soll Pflege nach dem Tattoo eine Rolle spielen, z. B. reinigen, geeignete Creme, Schutzfolie, nicht unnötig … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 71 | Kleidungskontakt: Soll enge/reibende Kleidung ein frisches Tattoo beeinflussen können, wenn sie ständig daran scheuert? | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 72 | Wasser/Sonne: Soll frische Tattoo-Haut auf langes Baden, starke Sonne usw. empfindlicher reagieren, während nach … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 73 | Healing individuell: Soll Heilungsdauer von Körperstelle, Haut, Größe, Technik, Pflege und Person abhängen statt … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 74 | Imperfect Healing: Soll ein Tattoo bei schlechter Heilung einzelne Stellen verlieren, fleckig werden oder kleine Narben … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 75 | Touch-up: Soll man nach vollständiger Heilung gezielt einzelne schwache Stellen nachstechen lassen können, statt das … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 76 | Touch-up-Preis: Soll ein Artist kleine Nachbesserungen manchmal kostenlos bzw. günstig anbieten, während größere … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 77 | Langzeit-Fading: Soll Tattoo-Fading über Jahre erfolgen, abhängig von Pigment, Hautstelle, Sonne, Artist-Qualität und … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 78 | Unterschiedliche Farben altern anders: Sollen verschiedene Pigmentfarben unterschiedlich stabil sein und sich dadurch … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 79 | Refresh nach Jahren: Soll man ältere Tattoos gezielt auffrischen lassen können, sodass Schwarz/Farben wieder kräftiger … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |
| 80 | Tattoo-History: Soll all das dauerhaft in der Tattoo-Historie stehen: ursprünglicher Artist, Sessions, Heilung, … | ja | 35#768 → 35#769 | 📋 ja | E-TA-020 |

## Runde 17E: Cover-up, Laser (35#770)

**Datum:** 2026-09-26 · **Fragen:** 81–100 (20) · **Antwortnachrichten:** 35#771 · **Module (Paketebene):** 07 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-TA-021

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 81 | Cover-up als echte Neugestaltung: Soll ein Cover-up nicht einfach „altes Tattoo ausblenden“ sein, sondern der Artist … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 82 | Cover-up braucht oft mehr Fläche: Soll ein Cover-up häufig größer und/oder dunkler werden müssen als das ursprüngliche … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 83 | Schwierige Cover-ups: Soll manche Arbeit so schwer zu covern sein, dass der Artist eher Laser + späteres neues Tattoo … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 84 | Blackout als Cover-up: Soll kompletter Blackout eine Option sein, um sehr große oder chaotische bestehende Tattoos zu … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 85 | Partial Blackout: Soll man nur einzelne Regionen blackouten können, z. B. Unterarm, Oberarm, Brustteil, Beinsegment … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 86 | Blackout-Qualität: Soll ein gutes Blackout extrem gleichmäßig und satt wirken, während schlechte Arbeit fleckig oder … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 87 | Blackout braucht mehrere Sessions: Soll großflächiges Blackwork häufig mehrere Sitzungen brauchen, besonders wenn hohe … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 88 | White-over-Black Refresh: Soll weißes/helles Motiv über Blackout nach Jahren erneut nachgestochen werden können, wenn … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 89 | Laser als Prozess: Soll Tattooentfernung durch Laser mehrere Sessions brauchen und nicht instant verschwinden? | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 90 | Laser-Ergebnis individuell: Soll Laser je nach Farbe, Pigmenttiefe, Alter, Haut, Tattoo-Qualität und Anzahl Sessions … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 91 | Restpigment: Soll nach Laser ein Tattoo teilweise sichtbar bleiben können, statt immer komplett weg zu sein? | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 92 | Laser-Narben/Veränderung: Soll schlechte Behandlung oder problematische Haut sichtbare Veränderungen/Narben … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 93 | Laser + Cover-up: Soll man ein Tattoo erst teilweise aufhellen und danach leichter covern können? | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 94 | Gewichtszunahme/-abnahme: Soll ein Tattoo sich entsprechend echter Hautverformung mit verändern und bei starken … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 95 | Muskelaufbau: Soll starker Muskelaufbau Tattoos ebenfalls sichtbar verändern, besonders an Armen, Schultern, Brust und … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 96 | Schwangerschaft: Soll Schwangerschaft besonders bei Bauch-/Flanken-Tattoos starke temporäre Dehnung verursachen und das … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 97 | Alterung der Haut: Soll ein Tattoo im hohen Alter mit Falten, Hauterschlaffung und verändertem Gewebe mit altern statt … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 98 | Narben später über Tattoo: Soll eine spätere Verletzung/Narbe ein bestehendes Tattoo tatsächlich beschädigen oder … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 99 | Tattoo über alter Verletzung: Soll ein Artist bei alten Narben anders planen müssen, weil Pigmentaufnahme und … | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |
| 100 | Tattoo-State-History: Soll die Engine jede wichtige Änderung als Verlauf speichern: | ja passt alles, aber vieles davon ist selbsterklärend (Pauschalantwort) | 35#770 → 35#771 | 📋 ja mit Zusatz | E-TA-021 |

## Runde 17F: NPC-Tattoo-Kultur (35#774)

**Datum:** 2026-09-26 · **Fragen:** 101–112 (12) · **Antwortnachrichten:** 35#775 · **Module (Paketebene):** 07 · **Status:** 📋 12  
**Register (Paketebene, Quelle überschneidet sich):** E-RC-011, E-TA-022, E-TA-023

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 101 | Tattoo-Verteilung nach Alter: Soll die Tattoo-Häufigkeit klar altersabhängig sein — jüngere Erwachsene deutlich … | ja das habe ich schon mal angedeutet | 35#774 → 35#775 | 📋 ja | E-TA-022 |
| 102 | Stadt vs. Land: Soll Tattoo-Coverage in Großstädten deutlich häufiger/extremer sein als auf dem Land, während ländliche … | ja auch das habe ich schon angedeutet | 35#774 → 35#775 | 📋 ja | E-TA-022 |
| 103 | Subkulturen: Sollen bestimmte Gruppen deutlich höhere Tattoo-Raten haben, z. B. Biker, Musiker, Gang-/Mafia-Milieus, … | [ja …; Details explizit, nicht wiedergegeben] | 35#774 → 35#775 | 📋 ja mit Zusatz · K16 | E-TA-022 |
| 104 | Berufseinfluss: Soll Beruf/Branche beeinflussen, wie häufig sichtbare Tattoos sind, ohne harte Verbote? Also z. B. … | ja teilweise schon, aber heutzutage nicht mehr so wie früher | 35#774 → 35#775 | 📋 ja mit Zusatz | E-TA-022 |
| 105 | Full-Body Seltenheit: Soll Full-Body-Coverage in der Gesamtbevölkerung selten bleiben, aber in passenden Szenen/Orten … | ja aber auch nicht einer in der ganzen metropolis, sondern schon mehrere, stellen wir gleich auf wie viele prozent die jeweiligen … | 35#774 → 35#775 | 📋 ja mit Zusatz | E-TA-022 |
| 106 | Studio-Dichte: Soll Metropolis sehr viele Studios haben, kleinere Städte deutlich weniger und ländliche Gebiete … | ja genau so ist es geplant, metropolis hat auch riesige studios mit mehreren stöcken und auch die berühmten... dörfliche sind … | 35#774 → 35#775 | 📋 ja mit Zusatz | E-TA-022 |
| 107 | Studio-Identität: Soll jedes relevante Studio eine erkennbare Spezialisierung/Atmosphäre haben, statt nur anderer Name … | ja genau, manche auf blackout spezialisiert manche auf farbe,... | 35#774 → 35#775 | 📋 ja mit Zusatz | E-TA-022 |
| 108 | Konkurrenz: Sollen Studios um Kunden, Reputation, Artists und Trends konkurrieren können, sodass manche aufsteigen und … | ja sicherlich | 35#774 → 35#775 | 📋 ja | E-TA-022 |
| 109 | Trends: Sollen Tattoo-Trends über Jahre wechseln können — bestimmte Motive, Styles oder Placements werden plötzlich … | ja | 35#774 → 35#775 | 📋 ja | E-TA-022 |
| 110 | NPC-Entscheidung: Soll ein NPC neue Tattoos wirklich aus Persönlichkeit, Geld, Szene, Freunden, Trends, Erinnerungen … | ja definitiv, aber manche sind danach süchtig und haben keine gründe mehr sondern einfach weil sie es wollen | 35#774 → 35#775 | 📋 ja mit Zusatz | E-TA-022 |
| 111 | Tattoo-Reputation: Soll extreme Coverage bei NPCs Reaktionen beeinflussen können — positiv, neutral oder negativ je … | ja kann es, manche mögen es manche hassen es und so weiter halt | 35#774 → 35#775 | 📋 ja mit Zusatz | E-TA-022 |
| 112 | Famous Artists/Studios: Soll es wenige legendäre Artists/Studios geben, die in der Welt richtig bekannt sind und Leute … | ja genau | 35#774 → 35#775 | 📋 ja | E-TA-022 |

## Runde 17G: Tattoo-Technik (35#778)

**Datum:** 2026-09-26 · **Fragen:** 113–122 (10) · **Antwortnachrichten:** 35#779 · **Module (Paketebene):** 07 · **Status:** 📋 10  
**Register (Paketebene, Quelle überschneidet sich):** E-TA-024

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 113 | Tattoo-Rendering als Skin-Layer: Soll jedes Tattoo logisch einzeln gespeichert bleiben, fürs Rendering aber zu … | wie genau meinst du das | 35#778 → 35#779 | 📋 Freitext | E-TA-024 |
| 114 | Pigment statt Sticker-Look: Soll der Shader Tattoos so darstellen, als lägen Pigmente in der Haut – also mit Poren, … | ja genau, aber das wird nicht einfach zum umsetzen sein nehme ich an | 35#778 → 35#779 | 📋 ja mit Zusatz | E-TA-024 |
| 115 | LOD-System: Soll Tattoo-Detail mit Distanz automatisch reduziert werden, während aus der Nähe Linien, Fading und … | ja teilweise aber von 10m oder 20m soll es noch klar erkennbar sein | 35#778 → 35#779 | 📋 ja mit Zusatz | E-TA-024 |
| 116 | Generator für NPCs: Soll der NPC-Generator Tattoos aus Demografie + Persönlichkeit + Subkultur + Beruf + Region + Geld … | ja bei den meiste schon aber manche machen sich tattoos einfach weil sie bock haben und es juckt sie nicht | 35#778 → 35#779 | 📋 ja mit Zusatz | E-TA-024 |
| 117 | Große Designs zuerst planen: Soll der Generator bei Sleeves/Bodysuits zuerst eine Komposition planen und danach … | ja | 35#778 → 35#779 | 📋 ja | E-TA-024 |
| 118 | Artist-Simulation: Soll beim Generieren immer ein konkreter virtueller Artist/Studio-Kontext dahinterstehen, damit … | ja | 35#778 → 35#779 | 📋 ja | E-TA-024 |
| 119 | Einzigartigkeit vs. Wiederverwendung: Dürfen Basis-Motive wiederverwendet werden, aber durch Placement, Scale, … | ja kann es wie gesagt gibt es vorlagen bei den tattoowierern und diese werden häufiger benutzt | 35#778 → 35#779 | 📋 ja mit Zusatz | E-TA-024 |
| 120 | Custom Player Tattoo Import: Soll später grundsätzlich ein System möglich sein, bei dem Spieler eigene Bilder/Designs … | ja das sowieso, das ist sogar pflicht, und nicht nur ein paar motive auswählen | 35#778 → 35#779 | 📋 ja mit Zusatz | E-TA-024 |
| 121 | Modding: Sollen Mods neue Tattoo-Stile, Motive, Pigmente, Artists, Studios, Generator-Regeln und Body-Surface-Templates … | ja | 35#778 → 35#779 | 📋 ja | E-TA-024 |
| 122 | Asset/QA-Pipeline: Soll jedes Tattoo-Asset vor Nutzung automatisch auf Dinge wie Auflösung, Seam-Probleme, falsche … | ja sicher aber sollte schon eine hohe auflösung sein | 35#778 → 35#779 | 📋 ja mit Zusatz | E-TA-024 |

## Runde 17H: Piercings, Körpermodifikation (35#782)

**Datum:** 2026-09-26 · **Fragen:** 123–134 (12) · **Antwortnachrichten:** 35#783 · **Module (Paketebene):** 07 · **Status:** 📋 12  
**Register (Paketebene, Quelle überschneidet sich):** E-TA-025

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 123 | Freie Piercing-Platzierung: Sollen Piercings grundsätzlich an allen anatomisch plausiblen Stellen möglich sein, statt … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 124 | Piercing-Arten: Sollen Typen wie Ring, Stud, Barbell, Surface Bar, Dermal usw. eigene Geometrie-/Materialeigenschaften … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 125 | Material: Soll Schmuckmaterial relevant sein, z. B. Titan, Stahl, Gold, Kunststoff, mit unterschiedlichen Preis-, … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 126 | Healing: Soll jedes Piercing eine realistische Heilphase haben, inklusive Reizung, Schwellung, möglicher Infektion und … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 127 | Entfernung / Wechsel: Soll man Schmuck später jederzeit wechseln oder entfernen können, wobei das Loch je nach … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 128 | Stretching: Soll gezieltes Dehnen von Piercings möglich sein, z. B. Ohrläppchen/Gauges, mit schrittweiser Größe statt … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 129 | Scar / Damage: Soll schlechtes Piercing, Unfall oder zu schnelle Dehnung Narben, Ausrisse oder dauerhafte Veränderungen … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 130 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Piercings, Körpermodifikation] | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 131 | Cosmetic Body Mods: Sollen weitere Mods wie Scarification, Branding, Split Tongue, Subdermal Implants oder ähnliche … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 132 | Extremere Mods selten: Soll extreme Body Modification in der Gesamtbevölkerung deutlich seltener sein, aber in … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 133 | Studios / Spezialisten: Sollen manche Studios auf Piercing/Body Mods spezialisiert sein, mit eigenem Skill, Hygiene, … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |
| 134 | Creator/Character Generator: Soll der normale Character Creator Piercings und Body Mods direkt anbieten, während … | ja | 35#782 → 35#783 | 📋 ja | E-TA-025 |

## Runde 17I: Abschluss (35#784)

**Datum:** 2026-09-26 · **Fragen:** 135–141 (7) · **Antwortnachrichten:** 35#785 · **Module (Paketebene):** 07 · **Status:** 📋 7  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-030, E-PR-016, E-SK-033, E-TA-026

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 135 | Tattoo-/Body-Mod-Defaultregel: Soll grundsätzlich gelten: Alles, was wir nicht einzeln abgefragt haben, orientiert sich … | ja so wie das komplette spiel nicht du das speziell | 35#784 → 35#785 | 📋 ja mit Zusatz | E-TA-026 |
| 136 | Keine künstlichen Grenzen: Soll der Spieler bei Tattoos/Piercings/Body Mods grundsätzlich sehr frei sein, solange es … | ja | 35#784 → 35#785 | 📋 ja | E-TA-026 |
| 137 | NPC-Autonomie: Sollen NPCs Tattoos/Piercings/Body Mods auch komplett unabhängig vom Spieler bekommen, verändern oder … | ja sowieso, die npcs sollen ein leben haben | 35#784 → 35#785 | 📋 ja mit Zusatz | E-TA-026 |
| 138 | Tattoo-Population dynamisch: Sollen Tattoo-Raten nicht nur einmal beim Spawn festgelegt werden, sondern sich über … | ja genau, manche leute die man vor paar jahren getroffen hat können auf einmal ein fullbodysuit haben oder was auch ich | 35#784 → 35#785 | 📋 ja mit Zusatz | E-TA-026 |
| 139 | Performance-Priorität: Soll Tattoo-Rendering hohe Nahqualität behalten, aber technisch aggressiv gebatcht/LOD-optimiert … | ja genau, das ganze spiel muss super optimiert sein | 35#784 → 35#785 | 📋 ja mit Zusatz | E-TA-026 |
| 140 | Content-Pipeline: Soll für Tattoos ein eigener Editor-/Production-Workflow entstehen: | ja | 35#784 → 35#785 | 📋 ja | E-TA-026 |
| 141 | Runde-17-Abschlussregel: Soll Runde 17 danach als ausreichend vollständig geplant gelten und nur später erneut geöffnet … | ja | 35#784 → 35#785 | 📋 ja | E-TA-026 |

## Runde 18A: Steuerung (35#790)

**Datum:** 2026-09-26 · **Fragen:** 1–20 (20) · **Antwortnachrichten:** 35#791, 35#793 · **Module (Paketebene):** 04 · **Status:** ❓ 3, 📋 17  
**Register (Paketebene, Quelle überschneidet sich):** E-ST-001, E-ST-002, E-ST-003, E-ST-004, E-ST-005

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Zwei Steuerungsmodi: Soll es grundsätzlich Direct Control und Assisted Control geben? | ich weiß noch nicht, ich finde das ufc system etwas erweitert ist schon extrem krass, was ist direkt dann genau | 35#790 → 35#791 | ❓ offen/Rückfrage | E-ST-002 |
| 2 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung] | schaue auf 1 weiß nicht genau noch | 35#790 → 35#791 | ❓ offen/Rückfrage | – |
| 3 | UFC-artige Action Library: Soll es ein Menü geben, in dem alle aktuell möglichen Actions/Transitions mit Beschreibung, … | ja aber erst wenn man diese gelernt hat, manche sind automatisch freigeschaltet wie ein haken oder ein jab aber spinning … | 35#790 → 35#791 | 📋 ja mit Zusatz | – |
| 4 | Kontextabhängige Liste: Soll dieses Menü automatisch nur Dinge zeigen, die mit aktueller Pose, freien Händen, Items, … | 4? | 35#790 → 35#791 | ❓ offen/Rückfrage | – |
| 5 | Vollständige Bibliothek separat: Soll man außerhalb einer laufenden Situation trotzdem die gesamte … | ja wenn man diese freigeschaltet hat ja, wenn noch nicht freigschaltet weiß man vielleicht nur den namen, aber beim mma training … | 35#790 → 35#791 | 📋 ja mit Zusatz | – |
| 6 | Freie Bewegung wichtiger als Combos: Sollen Combos nur Abkürzungen/Hilfen sein, während echte Freeform-Bewegung niemals … | wie gesagt frage 1 | 35#790 → 35#791 | 📋 Freitext | – |
| 7 | Analogsteuerung: Soll Geschwindigkeit/Bewegungsrichtung primär über Sticks/Trigger analog steuerbar sein, statt über … | worüber reden wir, über autos oder mma? (35#791) → ja stick bewegen ist schneller, spammen wäre zu gta (35#793) | 35#790 → 35#791,35#793 | 📋 ja mit Zusatz (nachgereicht/geändert) | – |
| 8 | Intensitäts-Modifier: Soll ein zusätzlicher Trigger/Button verändern, wie stark/schnell eine aktuelle Bewegung … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 9 | Körperbereich auswählen: Brauchen wir eine schnelle Methode, um festzulegen, welchen Körperbereich oder welches … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 10 | Smart Context: Soll das Spiel automatisch erraten, welche Hand/Körperregion sinnvoll gemeint ist, aber der Spieler kann … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 11 | Mehrere Actions verwalten: Soll das UI zeigen können, welche Actions aktuell parallel laufen, damit man eine einzelne … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 12 | HUD-Stufen: Soll es ungefähr Off / Minimal / Standard / Detailed / Developer geben, statt nur HUD an/aus? | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 13 | Minimal HUD: Soll Minimal praktisch nur wichtige Context-Hints zeigen und ansonsten möglichst filmisch aussehen? | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 14 | Vollständig remappable: Soll jede relevante Taste/Combo komplett neu belegbar sein, inklusive Controller, Maus/Tastatur … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 15 | Combo-Konflikte erkennen: Soll das Settings-Menü automatisch warnen, wenn zwei Actions dieselbe Kombination bekommen … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 16 | Haptics: Soll Controller-Vibration/Haptik aus tatsächlichem Contact/Impact/Movement entstehen und in Stärke separat … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 17 | Kamera: Soll die Kamera frei zwischen normaler Third Person, näherer Third Person, First Person und Cinematic View … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 18 | First Person nicht zwingend Kopfkamera: Soll First Person stabilisiert werden dürfen, damit starke Körperbewegungen … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 19 | Accessibility: Soll es Assists geben wie Hold statt Button-Spam, reduzierte komplexe Inputs, größere UI, längere … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |
| 20 | Settings-Profil: Soll der Spieler eigene komplette Control/UI-Presets speichern können, z. B. Controller Direct, … | sag mir das genauer ich kenne mich nicht aus (35#791) → ja (35#793) | 35#790 → 35#791,35#793 | 📋 ja (nachgereicht/geändert) | E-ST-005 |

## Runde 18B: VR (35#794)

**Datum:** 2026-09-26 · **Fragen:** 21–30 (10) · **Antwortnachrichten:** 35#795 · **Module (Paketebene):** 04 · **Status:** 📋 10  
**Register (Paketebene, Quelle überschneidet sich):** E-ST-002, E-ST-006

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 21 | VR als gleiche Engine: Soll VR exakt dieselben Actions/Physics/Body-States wie Controller nutzen und nur die Eingabe … | vr soll sich anfühlen von den movements und allem wie echt ok | 35#794 → 35#795 | 📋 Freitext | E-ST-006 |
| 22 | VR-Handtracking: Soll man Hände frei bewegen können und das System daraus passende Interaktionen ableiten, statt nur … | vr soll sich anfühlen von den movements und allem wie echt ok | 35#794 → 35#795 | 📋 Freitext | E-ST-006 |
| 23 | Full-Body-Tracking optional: Soll Hüft-/Fußtracking später unterstützt werden, aber nicht Pflicht sein? | vr soll sich anfühlen von den movements und allem wie echt ok | 35#794 → 35#795 | 📋 Freitext | E-ST-006 |
| 24 | Fehlende Körperbewegung: Wenn nur Headset + Hände getrackt werden, soll IK Beine/Hüfte realistisch ergänzen? | vr soll sich anfühlen von den movements und allem wie echt ok | 35#794 → 35#795 | 📋 Freitext | E-ST-006 |
| 25 | VR-Menü: Soll die Action-Bibliothek in VR als kleines räumliches Menü/Overlay erscheinen können, aber komplett … | ja | 35#794 → 35#795 | 📋 ja | E-ST-006 |
| 26 | VR-Haptik: Soll Haptik später über Controller/Handschuhe unterstützt werden, aber nie Voraussetzung sein? | ja | 35#794 → 35#795 | 📋 ja | E-ST-006 |
| 27 | Motion Sickness: Soll VR standardmäßig Kamera-/Kopfbewegung glätten können, damit starke Animationen nicht sofort … | ja | 35#794 → 35#795 | 📋 ja | E-ST-006 |
| 28 | Sitzend/stehend: Soll VR sowohl sitzend als auch stehend funktionieren? | ja | 35#794 → 35#795 | 📋 ja | E-ST-006 |
| 29 | VR-Calibration: Soll Körpergröße, Armreichweite und Tracking-Raum kalibriert werden, damit IK sauber zum echten Spieler … | ja | 35#794 → 35#795 | 📋 ja | E-ST-006 |
| 30 | VR später: Soll VR technisch von Anfang an mitgedacht werden, aber erst deutlich später implementiert werden, wenn … | ja definitiv VR kann man so viel erweitern das es sich wirklich anfühlt und ausschaut wie in echt mit bewegbaren händen, füßen, … | 35#794 → 35#795 | 📋 ja mit Zusatz | E-ST-006 |

## Runde 18C: Bedienung (35#800)

**Datum:** 2026-09-27 · **Fragen:** 31–42 (12) · **Antwortnachrichten:** 35#801 · **Module (Paketebene):** 04 · **Status:** 📋 12  
**Register (Paketebene, Quelle überschneidet sich):** E-ST-007

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 31 | Context-Sensitive Inputs: Soll dieselbe Taste je nach aktueller Situation unterschiedliche sinnvolle Funktionen haben … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 32 | Action Search: Soll man die Action-Bibliothek nach Namen, Kategorie, Körperbereich, Item oder Situation durchsuchen … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 33 | Quick Favorites: Soll man häufig verwendete Actions/Transitions auf ein kleines Schnellmenü legen können, ohne dass das … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 34 | Learning UI: Soll das Spiel neue Controls/Actions schrittweise erklären, statt beim ersten Start 200 … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 35 | Preview-System: Soll eine Action-Vorschau zeigen, was ungefähr passieren wird, aber nicht eine exakt vorgerenderte … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 36 | Invalid Action Feedback: Wenn etwas gerade physisch nicht möglich ist, soll das UI kurz erklären warum – z. B. zu wenig … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 37 | Kamera unabhängig steuerbar: Soll der Spieler die Kamera möglichst unabhängig von der laufenden Interaction bewegen … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 38 | Cinematic Camera getrennt von Gameplay: Soll die filmische Kamera eher für Beobachtung/Replay gedacht sein, während … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 39 | UI vollständig modular: Soll man fast jedes HUD-Element einzeln deaktivieren, skalieren, verschieben oder transparenter … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 40 | VR-Handpräzision: Soll VR-Hand-/Fingertracking später möglichst echte Fingerbewegungen übernehmen können, wenn Hardware … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 41 | VR-Body Ownership: Soll der sichtbare In-Game-Körper exakt an die kalibrierte Spielerposition angepasst werden, damit … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |
| 42 | Runde-18-Abschlussregel: Soll Runde 18 danach als ausreichend geplant gelten und nur wieder geöffnet werden, wenn … | ja | 35#800 → 35#801 | 📋 ja | E-ST-007 |

## Runde 19A: Technik (35#804)

**Datum:** 2026-09-27 · **Fragen:** 1–20 (20) · **Antwortnachrichten:** 35#805 · **Module (Paketebene):** 02 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-PA-028, E-PA-029, E-SP-001

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Ein gemeinsamer Human Core: Soll Adult ausschließlich Erweiterungen auf Human/Body/Interaction registrieren und niemals … | ja | 35#804 → 35#805 | 📋 ja | E-PA-028 |
| 2 | Modulgrenzen: Sollen Adult-spezifische C++-Module strikt von allgemeinen Modulen getrennt sein, damit das Hauptspiel … | ja | 35#804 → 35#805 | 📋 ja | E-PA-028 |
| 3 | Plugin-Struktur: Soll Adult technisch als optional ladbares Unreal-/Game-Modul funktionieren, statt tief im Base Game … | ja | 35#804 → 35#805 | 📋 ja | E-PA-028 |
| 4 | Keine Adult-Daten ohne Mod: Soll ein Save ohne installierte Adult-Mod grundsätzlich sauber funktionieren, auch wenn … | ja die daten von adult werden extra gespeichert | 35#804 → 35#805 | 📋 ja mit Zusatz | – |
| 5 | Save-Daten erhalten: Wenn man die Mod deaktiviert und später wieder aktiviert, sollen vorhandene kompatible Adult-Daten … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 6 | Schema-Migration: Soll jede größere Datenversion automatische Save-Migrationen besitzen, damit Updates ältere … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 7 | Event-driven statt Tick: Soll möglichst viel über Events/State Changes laufen und nicht jeder Adult-/Human-State jede … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 8 | Simulation LOD: Soll nur der aktuell relevante/nahe Charakter volle Interaction-/Physics-Qualität bekommen, während … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 9 | GPU/CPU Budgets: Soll Claude später für jedes große Subsystem konkrete Frame-Time-, RAM- und VRAM-Budgets bekommen, … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 10 | Graceful Degradation: Wenn Hardware schwächer ist, soll zuerst Detail reduziert werden – Solver-Iterations, Secondary … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 11 | Determinismus: Sollen wichtige Gameplay-/Body-/Reaction-States deterministisch genug sein, dass Save/Load und Replay … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 12 | Visual Physics getrennt: Sollen rein kosmetische Details teilweise nondeterministisch sein dürfen, solange … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 13 | Threading: Soll schwere Physik-/Reaction-/Body-State-Arbeit soweit möglich über Jobs/Worker verteilt werden, aber mit … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 14 | Data Registry: Soll Adult-Content primär über registrierte Data Assets/Definitions geladen werden, sodass neue Inhalte … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 15 | Mod API stabil: Soll die öffentliche Modding-API bewusst kleiner und stabiler sein als interne APIs, damit Updates … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 16 | Capability-System: Soll eine Mod deklarieren müssen, welche Features sie braucht, z. B. HumanInteraction, CustomItems, … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 17 | Automated QA: Soll CI tausende generierte Kombinationen automatisch testen – unterschiedliche Körper, Größen, … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 18 | Golden Tests: Sollen besonders wichtige Referenzfälle als Golden Tests existieren, deren Ergebnis bei Engine-Updates … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 19 | Developer Telemetry: Soll der Dev-Build Performance, Solver-Fehler, ungültige States, Save-Migrationen und … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |
| 20 | Early Production Scope: Soll die allererste Adult-Implementierung bewusst nur einen kleinen vertikalen Kern verwenden – … | ja | 35#804 → 35#805 | 📋 ja | E-SP-001 |

## Runde 19B: Speicher, Builds (35#806)

**Datum:** 2026-09-27 · **Fragen:** 21–32 (12) · **Antwortnachrichten:** 35#807 · **Module (Paketebene):** 02 · **Status:** 📋 12  
**Register (Paketebene, Quelle überschneidet sich):** E-PA-030, E-SP-001

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 21 | Gemeinsame IDs: Soll Adult immer dieselben persistenten CharacterID, ItemID, LocationID usw. wie das Hauptspiel … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 22 | Fehlende Referenzen: Wenn ein NPC/Item durch Mod-Deinstallation oder World-Änderung nicht mehr existiert, soll … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 23 | Separate Adult-Save-Datei: Soll pro Spielstand eine eigene Adult-State-Datei bzw. ein eigener Datenbereich existieren, … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 24 | Encryption/Privacy: Soll Adult-spezifischer Save-State optional verschlüsselt bzw. zumindest getrennt und nicht direkt … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 25 | Hot Enable/Disable: Soll die Adult-Mod möglichst ohne neuen Spielstand aktivierbar/deaktivierbar sein, solange Version … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 26 | Feature Flags: Soll jedes größere Adult-Subsystem separat aktivierbar sein, z. B. Core Interaction, Advanced Physics, … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 27 | Late-Stage Features: Sollen schwere Polish-Systeme wie detaillierte Fluids, extrem komplexe Secondary Physics und … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 28 | Crash Isolation: Soll ein Fehler in einem optionalen Adult-Subsystem möglichst nicht den kompletten Spielstand … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 29 | Mod Sandboxing: Sollen externe Adult-Mods nur über definierte APIs/Registries zugreifen dürfen und nicht beliebig … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 30 | Version Compatibility: Soll der Mod Manager bereits vor dem Laden prüfen, ob Adult-Mod, Base Game, Save Schema und … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 31 | Production Build Split: Soll es klar getrennte Builds geben: | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |
| 32 | Runde-19-Abschlussregel: Soll Runde 19 danach als technisch ausreichend geplant gelten, solange später beim echten … | ja | 35#806 → 35#807 | 📋 ja | E-PA-030 |

## Runde 20A: Production Lock (35#810)

**Datum:** 2026-09-27 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#811 · **Module (Paketebene):** 10, 02 · **Status:** 📋 15  
**Register (Paketebene, Quelle überschneidet sich):** E-PR-012, E-PR-013, E-RC-012

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Adult V1 Scope: Soll die erste spielbare Adult-Version bewusst klein starten: Basisinteraktionen, wenige … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 2 | Prioritätsklassen: Sollen alle Adult-Features final in Core, Important, Late Polish einsortiert werden, damit … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 3 | Core Gate: Soll Adult V1 erst als „funktionierend“ gelten, wenn IK, Physics, Body State, Reaction Graph, Save/Load und … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 4 | Kein Fake-Feature-Count: Soll ausdrücklich gelten: lieber weniger Features, aber sauber, statt hundert halbfertige … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 5 | Performance Gate: Soll jedes neue Adult-Subsystem vor Merge einen festen CPU/GPU/RAM/VRAM-Budgetcheck bestehen? | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 6 | QA Gate: Soll jede neue Action/Interaction mindestens durch automatisierte Solver-/Collision-/Save-/Replay-Tests … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 7 | Visual Quality Gate: Soll ein Feature erst als final gelten, wenn Animation/IK, Contact, Skin/Soft Tissue, Audio und … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 8 | Registry Freeze: Soll die finale Adult-Category-Registry erst nach unserem späteren Missing-Family-Audit + Qwen-Audit … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 9 | Private/Internal Content: Soll klar getrennt bleiben zwischen Base Release, Adult-enabled Dev Build und … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 10 | Public Adult Release optional: Soll ausdrücklich gelten, dass eine öffentliche Adult-Veröffentlichung kein Pflichtziel … | ja aber ich will es schon das es veröffentlicht wird nur für mich brauche ich nicht | 35#810 → 35#811 | 📋 ja mit Zusatz · K18 | – |
| 11 | Shared-Core-Regel: Soll Adult dauerhaft auf Human/Physics/Health/Memory/World/Replay/Stats/Modding-Core aufsetzen und … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 12 | Modding First-Class: Soll Adult-Modding von Anfang an als ernsthafte Erweiterungsschicht geplant werden, mit stabilen … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 13 | Production Documentation: Soll für jedes große Adult-Subsystem vor Implementierung eine kurze technische Spezifikation … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 14 | Prototype First: Soll Claude/Codex zuerst einen kleinen Vertical Slice bauen und erst nach erfolgreichem Test die … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
| 15 | Finaler Production Lock: Soll Adult nach Runde 20 als ausreichend produktionsreif geplant gelten, wobei nur echte neue … | ja | 35#810 → 35#811 | 📋 ja | E-PR-012 |
