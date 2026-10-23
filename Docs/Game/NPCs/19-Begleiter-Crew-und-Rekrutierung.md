# NPCs 19: Begleiter, Crew und Rekrutierung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die Hunter-Crew, menschliche Begleiter, den Hund und die Rekrutierung neuer Figuren aus der Welt. Beschlossen sind die Richtung (eigenständige Begleiter mit Persönlichkeit, Moral, Ausrüstung, Fahrzeug; Zwei-Jäger-Kern; permanenter Tod mit Schutz der Kernfiguren; Rekrutierung aus optionalen Fraktionen und der Welt; HQ als lebendiger Ort) und viele Einzelregeln; **Datenmodell, Entscheidungslogik und Abläufe sind Entwurf (💡)**. **Besetzung, Biografien und Spezialisierungen der Crew sind offen** (`M-OD`); es werden keine Figuren festgelegt. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Überblick: [Spielvision](../Spielvision.md) §6.2–§6.4. Hund und Tiere: [Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md). Fraktionen und Rekrutierungswege: [Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Das Crew-Mitglied als Figur](#2-entwurf-das-crew-mitglied-als-figur)
3. [Entwurf: Begleiter im Einsatz](#3-entwurf-begleiter-im-einsatz)
4. [Entwurf: Verletzung, Rettung, Tod](#4-entwurf-verletzung-rettung-tod)
5. [Entwurf: Crew-Alltag und Hauptquartier](#5-entwurf-crew-alltag-und-hauptquartier)
6. [Entwurf: Der Hund](#6-entwurf-der-hund)
7. [Entwurf: Rekrutierung](#7-entwurf-rekrutierung)
8. [Entwurf: Integration ins Hauptquartier](#8-entwurf-integration-ins-hauptquartier)
9. [Entwurf: Entwicklung der Crew ohne den Spieler](#9-entwurf-entwicklung-der-crew-ohne-den-spieler)
10. [Entwurf: Koop](#10-entwurf-koop)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-CO-001 | Es gibt einen frei erstellbaren Hauptcharakter und eine feste, familienähnliche **Hunter-Crew** aus ausgearbeiteten Figuren mit Persönlichkeit, Vergangenheit, Beziehungen, Konflikten, Tagesabläufen, Fähigkeiten, Fahrzeugen, Zimmern, Ausrüstung, Meinungen, Kontakten und Erinnerungen | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-002 | Das Spiel beginnt mit einem **Zwei-Jäger-Kern**; später werden weitere Jäger getroffen, gewonnen und können als Begleiter verfügbar werden | 📋 | `23#117A` Punkt 17; „17 ja genau“ in `23#118E`; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-003 | Menschliche Begleiter sind eigenständige intelligente Akteure, keine ferngesteuerten Puppen; sie unterscheiden sich in Kampf, Recherche, Medizin, Technik, Tracking, Persönlichkeit, Moral, Ausrüstung, Fahrzeug, Erfahrung | 📋 | `M-PD` ‚Companion autonomy, planning and field commands‘; `23#1088A` Fragen 16–18; „16-24 ja“ in `23#1089E`; `23#1090A` ‚Companions‘ |
| N-CO-004 | Begleiter haben eigene Persönlichkeit, kommentieren Entscheidungen, handeln nicht wie stumme Bots; sie können Handlungen verweigern („Nein, da mache ich nicht mit.“); Vertrauen beeinflusst Zusammenarbeit ohne sichtbaren Balken | 📋 | `23#1088A` Fragen 18–20; „16-24 ja“ in `23#1089E`; `23#1066A` Abschnitt 2 |
| N-CO-005 | Begleiter besitzen echte Waffen, Kleidung und Inventar; einige fahren mit eigenem Auto oder Motorrad; man kann sich ‚dort treffen‘ (im Wortlaut der Frage: ‚Triff mich dort‘) statt immer gemeinsam zu reisen | 📋 | `23#1088A` Fragen 21–23; „16-24 ja“ in `23#1089E` |
| N-CO-006 | Begleiter können ernsthaft verletzt werden und ausfallen; sie können den Spieler retten (Erste Hilfe, Deckung, Herausziehen) und vom Spieler gerettet werden | 📋 | `23#1088A` Fragen 24, 27–28; „16-24 ja“, „27-29 ja“ in `23#1089E` |
| N-CO-007 | **Tod:** Normale und rekrutierbare Begleiter können dauerhaft sterben („würde ich schon machen außer bei den richtigen aus dem hunter hq“); Kernfiguren des Hunter-HQ haben keinen zufälligen Sandbox-Tod; stirbt eine Hauptfigur laut Story, bleibt sie tot („ja wenn es ein story tod ist dann ist er tod“) | 📋 | `23#1088A` Fragen 25–26; Antworten in `23#1089E`; `23#1090A` ‚Tod‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-008 | Mehrere Begleiter (2–3) sind für manche Einsätze möglich, aber kein ständiges Riesen-Squad; Solo bleibt möglich; Storymissionen dürfen mit Begleiter geschrieben sein: „die story missionen können mit begleiter sein“ | 📋 | `23#1088A` Fragen 29–30; „27-29 ja“ und Antwort 30 in `23#1089E` |
| N-CO-009 | Der Spieler kann entscheiden, ob er einen verfügbaren Begleiter mitnimmt, auch einen schwächeren oder unerfahrenen nicht | 📋 | `M-PD` ‚Companion autonomy …‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-010 | Vor dem Einsatz kann der Spieler mit dem Begleiter planen (Rollen, Eingänge, Positionen, laut oder leise, Rückzug, Treffpunkt, Fahrzeugrolle); im Einsatz sind Befehle möglich (Folgen, Warten, Decken, Beobachten, Flankieren, Ausgang sichern, Vorrücken, Zurückziehen, Leise sein); Handzeichen, ersatzweise Stimme, Funk, Telefon mit hörbaren Folgen | 📋 | `M-PD` ‚Companion autonomy …‘; [Spielvision](../Spielvision.md) §6.3 |
| N-CO-011 | Begleiter folgen Plan und Befehl, dürfen aber abweichen, wenn unmittelbare Gefahr oder neue Information die Anweisung offensichtlich unsicher oder unmöglich machen; unerfahrene Begleiter machen plausible Fehler, sabotieren aber nie zufällig | 📋 | `M-PD` ‚Companion autonomy …‘ |
| N-CO-012 | Ein erfahrener Hunter kann eine bessere spontane Entscheidung treffen als der Plan; er bewertet Plan, Befehl, Wahrnehmung, Gefahr, Persönlichkeit, Erfahrung, Vertrauen und Verletzungen | 🧭 | `23#255A` (Beschreibung des Assistenten); [Spielvision](../Spielvision.md) §6.3 |
| N-CO-013 | Eigenfeuer auf Begleiter ist physikalisch möglich und konfigurierbar; im härtesten Modus standardmäßig voll aktiv | 📋 | `M-PD` ‚Multiple-opponent combat …‘ |
| N-CO-014 | Der Hund ist ein besonderer Begleiter: adoptierbar, Rassen, Persönlichkeit, Training (Gehorsam, Rückruf, Suchen, Anzeigen, Warten), Geruchsverfolgung ohne magische Navigation, Reaktion auf Tiere und teils Übernatürliches, Verletzung, Alterung, Tierarzt; Impfungen optional; er kann zuhause bleiben | 📋 | `23#1088A` Fragen 31–40; „31-40 ja, impfungen sind optional“ in `23#1089E`; `23#1090A` ‚Hund‘ |
| N-CO-015 | Crewmitglieder leben im Hauptquartier: schlafen, essen, trainieren, recherchieren, Fahrzeuge reparieren, Waffen pflegen, telefonieren, fernsehen, reden, streiten, feiern, gehen und kommen zurück | 📋 | [Spielvision](../Spielvision.md) §6.4; `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; Ausformulierung `23#603A` Abschnitt 2 (💡) |
| N-CO-016 | Das Hunter-HQ ist der primäre soziale Ort; Versorgung, Beiträge, Aufgaben und Feiern bleiben organisch und nicht erdrückend; die Crew kann selbst Geld, Vorräte, Ersatzteile und Informationen beisteuern | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; [Spielvision](../Spielvision.md) §6.4 |
| N-CO-017 | Persönliche Anfragen der Crew haben mehrere faire Auslöser und sind nur unverfügbar, wenn die Figur stirbt oder dauerhaft geht | 📋 | `M-PD` ‚Acts, chapters and controlled story reactivity‘; [Spielvision](../Spielvision.md) §6.2 |
| N-CO-018 | Crew-Aktivitäten (Training, Essen, Billard, Bar, Rennen, Schießstand, Angeln, Jagd, Kino, Roadtrip, Reparatur, Recherche, kleinere Jagd, einkaufen, Sport) sollen nicht unnötig verpassbar sein | 🧭 | `23#603A` Abschnitt 3 (Vorschlag des Assistenten) |
| N-CO-019 | Ein Crewmitglied recherchiert tatsächlich und informiert den Spieler, repariert Fahrzeuge mit echtem Fortschritt, liegt verletzt mehrere Tage im HQ, und andere Crew übernimmt Aufgaben | 🧭 | `23#603A` Abschnitt 2 (Vorschlag des Assistenten) |
| N-CO-020 | Ein Ledger der Beiträge im HQ (Aufträge, Munition, Vorräte), Hausarbeiten ohne XP, spontane Feiern nach Erfolgen, persönliche Item-Wünsche | 🧭 | `23#606A` Abschnitte 1–5 (Vorschläge des Assistenten); [Spielvision](../Spielvision.md) §6.4 |
| N-CO-021 | **Rekrutierung:** Optionale Faction-NPCs (Club, Sekte, kriminelle Gruppe, Extremisten) können rekrutiert werden und dann im HQ sein; Rekruten stehen für Missionen optional zur Verfügung; die Mission funktioniert immer ohne sie | 📋 | `23#677E`; `23#678A`; „Optional Recruit → mehr Optionen / Dialog / Unterstützung“ (Regel des Assistenten) |
| N-CO-022 | Nur spezielle, definierte Kandidaten sind rekrutierbar; mögliche Herkünfte: MC, Sekte, kriminelle Organisation, extremistische Gruppe, Polizei, Militär, Arzt/Paramedic, Mechaniker, Racer, Wissenschaftler, Okkultist, ehemalige Monsteropfer, normale Zivilisten | 🧭 | `23#678A` (Vorschlag des Assistenten) |
| N-CO-023 | Rekruten bringen echte Fähigkeiten mit (Fahrzeuge, Insiderwissen, Symbolerkennung, Netzwerke, Driving, Tuning, Medizin, Forensik); nicht jeder Rekrut passt automatisch ins HQ; es entstehen Reaktionen der Crew, Konflikte und Vertrauensentwicklung | 🧭 | `23#678A` |
| N-CO-024 | Das Recruitment-System bleibt nach dem Storyende aktiv; neue Leute können ins HQ kommen; das HQ ist kein eingefrorenes Museum | 📋 | `23#679E`; `23#680A` |
| N-CO-025 | Auch NPC-Hunter und Freunde können Begleiter werden (andere Hunter treffen → zusammenarbeiten → Vertrauen → Freundschaft oder Rivalität → später möglicherweise als Begleiter verfügbar) | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-CO-026 | Die Besetzung der Crew, Biografien, Spezialisierungen, Todesfälle und Beziehungen sind nicht festgelegt | ❓ | [Spielvision](../Spielvision.md) §6.2; `M-OD` |
| N-CO-027 | Feldbasen (Motels, Hütten, Häuser, Unterschlupfe, Bunker) dienen entfernten Handlungsbögen; ein mobiles Wildnislager ist möglich | 📋 | [Spielvision](../Spielvision.md) §6.4; `M-PD` |
| N-CO-028 | Partner können in Gefahr geraten, aber nicht ständig als Entführungswerkzeug | 📋 | `23#1088A` Frage 15; „11-15 genau“ in `23#1089E` |
| N-CO-029 | Mehr Hunter außerhalb des Kerns erscheinen im Laufe des Spiels; das Hunter-Netz ist selten | 📋 | `M-B09`; [Spielvision](../Spielvision.md) §6.2 |

---

## 2. Entwurf: Das Crew-Mitglied als Figur

Ein Crew-Mitglied ist ein **A-NPC** (Modul 03) mit zusätzlichen Bausteinen (💡):

```text
CrewMember  (Erweiterung des NPC-Datensatzes)
├── role            Kernhunter | Crew | Rekrut | Gastbegleiter | Hund
├── skills          Kampf, Waffen, Recherche, Medizin, Technik, Fahren, Tracking, Forensik, Okkultismus, Verhandeln ...
├── doctrine        Haltung zu Töten, Gnade, Wesen, Gesetz, Geheimhaltung, Risiko  (Moral, N-CO-004)
├── gear            Waffen, Kleidung, Inventar, Fahrzeug(e)         (persistent)
├── room            Zimmer im HQ, persönliche Gegenstände
├── routines        Tagesabläufe im HQ (Schlaf, Training, Arbeit, Freizeit)
├── ties            Beziehungen zu anderen Crewmitgliedern, Familie, Freunden, früheren Gruppen
├── requests        persönliche Wünsche / Gefallen  (N-CO-017)
├── availability    verfügbar | im Einsatz | verletzt | abwesend | gestorben | gegangen
├── protection      sim_safe | story_safe | narrative_only   (Modul 03 §8)
└── arc             Entwicklungsbogen (handgeschrieben) + systemische Veränderungen
```

**Entwurfsregeln:**

1. **Handgeschriebene Tiefe, systemisches Leben.** Biografie und Schlüsselmomente sind geschrieben; Tagesablauf, Gedächtnis, Beziehungsdynamik und Reaktionen laufen systemisch (Dialogtiefe nach Stufe: `M-PD`).
2. **Doktrin statt Gut/Böse.** Begleiter urteilen verschieden (N-LT-027): Ein Begleiter, der ein Wesen töten würde, das der Spieler verschonen würde, ist ein erwarteter Konflikt, kein Fehler (siehe auch die Entscheidungsfreiheit anderer Hunter in `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘).
3. **Verfügbarkeit ist Zustand.** Ein Begleiter, der verletzt oder weg ist, ist nicht verfügbar; der Grund steht im Journal.
4. **Eigene Ziele.** Crew haben Wünsche und Pläne außerhalb des Spielers (Modul 13 §9).
5. **Keine Pflicht zur Besetzung.** Das Datenmodell trägt beliebige Crew, nicht eine feste Liste.

---

## 3. Entwurf: Begleiter im Einsatz

### 3.1 Zusage und Absage

Vor einem Einsatz entscheidet der Begleiter, ob er mitkommt. Die Quelle nennt Vertrauen, Gedächtnis und Moral als Einflüsse (N-CO-004, `23#117A`: „Kommt er mit, wenn du ihn mitten in der Nacht anrufst?“). Entwurf:

| Einfluss | Wirkung |
|---|---|
| **Beziehung zum Spieler** (Vertrauen, Respekt, Schuld) | wer gerettet wurde, kommt eher mit; wer im Stich gelassen wurde, zögert |
| **Erinnerung** | Lüge, Verlust, Verletzung wegen Entscheidungen (N-BV-004) |
| **Moral** | Einsatz verstößt gegen die Doktrin → Verweigerung (N-CO-004) |
| **Lage** | verletzt, müde, abwesend, in einem anderen Einsatz |
| **Zeit und Ort** | Anreise, eigenes Fahrzeug, Treffpunkt (N-CO-005) |
| **Gruppenlage** | Konflikte mit anderen Crewmitgliedern, die mitkommen |
| **Verpflichtung** | Auftrag, Familie, Arbeit |

**Ablehnung ist kein Fehler** (N-CO-004). Der Spieler kann alleine gehen (N-CO-008), außer die Story schreibt Begleiter vor.

### 3.2 Planung und Befehle

Planung und Befehle sind Spielsystem ([Spielvision](../Spielvision.md) §6.3, `M-PD`, `M-B22`). Für die NPC-Dokumentation zählt, **wie der Begleiter damit umgeht**:

1. **Ausführung nach Kompetenz.** Erfahrene führen sauberer aus; Unerfahrene machen plausible Fehler (N-CO-011).
2. **Abweichung nach Lage.** Er weicht ab bei unmittelbarer Gefahr oder neuer Information (N-CO-011) und **sagt es**, wenn möglich (Funk, Zeichen), damit der Spieler es nachvollziehen kann.
3. **Persönlichkeit färbt Ausführung.** Mutige drängen vor, Vorsichtige halten zurück.
4. **Hörbare Folgen.** Funk, Stimme, Telefon haben Geräuschkonsequenzen ([Spielvision](../Spielvision.md) §6.3).
5. **Tarnverhalten** achtet auf Absicht, Tempo, Licht, Gegner, Schusslinien, Lärm.
6. **Kein Zufallssabotieren.** Fehler sind nachvollziehbar.
7. **Eigenfeuer** möglich (N-CO-013).

### 3.3 Kommentare, Gespräche, Barks

Begleiter sprechen im Einsatz, beim Fahren und im Alltag (`M-PD` ‚Dialogue …‘: Road-Trip-Gespräche sind ein wichtiger Kanal der Charakterentwicklung). Ihre Kommentare folgen Persönlichkeit, Doktrin, Beziehung und Erinnerung (Modul 20).

### 3.4 Eigenes Leben

Begleiter warten nicht ewig auf den Spieler (`23#510A`, Bewertungstabelle: „Begleiter hat eigenes Leben“, 💡). Sie haben Zeitpläne (Modul 13), erreichbar per Telefon (Kommunikationsstil: Modul 04 §7) und treffen den Spieler an vereinbarten Orten.

---

## 4. Entwurf: Verletzung, Rettung, Tod

Die Quellen sind klar (N-CO-006, N-CO-007). Entwurf der Einbettung:

### 4.1 Verletzung

Begleiter folgen derselben Trauma- und Medizinlogik wie der Spieler (N-LB-013). Ein verletzter Begleiter **fällt aus** (N-CO-006) und ist verfügbar, sobald er genesen ist (Genesungsstufen: Modul 14 §5). Eine Verletzung hat Folgen für Fähigkeiten, Aussehen (Narben), Persönlichkeit (Trauma), Beziehung (Schuld, Vorwurf).

### 4.2 Rettung

Rettung (Erste Hilfe, Deckung, Herausziehen) geht in beide Richtungen (N-CO-006). Eine Rettung erzeugt einen **Anker** im Gedächtnis (Modul 05) und verschiebt Vertrauen, Schuld, Bindung (Modul 08). Das Rettungs-Szenario ist ein Standardszenario des Gedächtnissystems (Modul 05 §8).

### 4.3 Tod

| Gruppe | Regel (Quelle) |
|---|---|
| **Normale und rekrutierbare Begleiter** | können dauerhaft sterben (N-CO-007) |
| **Kernfiguren des HQ** | kein zufälliger Sandbox-Tod; Tod nur laut Story; dann endgültig (N-CO-007) |
| **Hund** | kann verletzt werden; Tod ist möglich (N-CO-014 nennt Verletzung, Alterung; der Tod des Hundes ist nicht ausdrücklich geregelt, ❓ Q-N22) |
| **Gast-Hunter** | wie normale Begleiter, abhängig von ihrer Schutzstufe |

**Folgen (Entwurf).** Ein Todesfall ist Weltereignis (Modul 14 §8): Trauer der Crew, Beerdigung, Veränderung von HQ und Beziehungen, Erinnerungen, Racheketten (wenn Täter bekannt: Nemesis, Modul 11). Persönliche Anfragen entfallen (N-CO-017). Vertretung: Rekruten, andere Crew.

**Spannung zum Schutz.** `M-PD` sagt „no plot armor“ für Menschen (N-LB-013); die Quellen schützen aber Kernfiguren des HQ vor zufälligem Tod. Das ist kein Widerspruch, wenn der Schutz nur **Simulations- und Zufallstode** betrifft und die Story selbst Tod zulässt; die Grenze (wer außer Spieler/Story töten darf) ist offen (⚠️ C-N1).

### 4.4 Beinahe-Tod

Bei Beinahe-Tod-Zuständen des Spielers („contextual rescue“: `M-PD` ‚Death / failure — OPEN‘) können Begleiter, Rettungskräfte oder Passanten retten. Der Ablauf ist offen. Für die NPC-Dokumentation gilt: Wer rettet, entsteht aus Lage und Beziehung, nicht aus einem Skript (Modul 05 §8).

---

## 5. Entwurf: Crew-Alltag und Hauptquartier

Die Quellen verlangen ein lebendiges HQ (N-CO-015, N-CO-016). Der Entwurf legt Alltagsbausteine fest (💡):

| Baustein | Beschreibung |
|---|---|
| **Zeitplan** | Schlaf, Mahlzeiten, Training, Arbeit, Freizeit (Modul 13) |
| **Orte im HQ** | Küche, Wohnzimmer, Garage, Archiv, Werkstatt, Training, Krankenzimmer, Lager, Zimmer ([Spielvision](../Spielvision.md) §6.4) |
| **Zufallsgespräche** | Der Spieler kann Gespräche mithören (Lauschen: N-IN-004; der Gedanke, im HQ nicht inszenierte Gespräche mitzubekommen, stammt aus `23#603A` Abschnitt 2, 💡); sie nutzen Gedächtnis und Beziehungen der Sprecher |
| **Arbeit** | Recherche mit echtem Ergebnis (N-CO-019), Reparatur mit Fortschritt, Waffenpflege |
| **Beiträge** | Crewmitglieder bringen Geld, Vorräte, Ersatzteile, Informationen (N-CO-016); abstraktes Ledger (N-CO-020) |
| **Versorgung** | organisch; leere Regale als Zeichen, keine Warnmeldungen (N-CO-020) |
| **Feiern, Spontanes** | nach Erfolgen, Geburtstagen, Ausbau, abgeschlossenen Fällen (N-CO-020) |
| **Aktivitäten** | gemeinsame Aktivitäten (N-CO-018) |
| **Persönliche Wünsche** | Items, Gefallen; faire Auslöser (N-CO-017) |
| **Konflikte** | Streit, Eifersucht, Lagerbildung (Modul 16 §7, Modul 09) |
| **Verletzte** | liegen im Krankenzimmer; andere übernehmen (N-CO-019) |
| **Besucher** | Rekruten, Freunde, Familie, Kontakte |
| **Sicherheit** | HQ nicht dauerhaft sicher (N-NE-007) |

**Regeln (Entwurf):**

1. **Nichts ist Pflicht.** Optionale Arbeiten haben keine XP; sie haben allenfalls kleine natürliche Effekte (schnellere Vorbereitung, Kommentar der Crew: `23#606A` Abschnitt 3).
2. **Nichts ist verpassbar wegen Zeitfenstern** (N-CO-018, N-LT-035).
3. **Die Crew lebt auch ohne den Spieler** (Abschnitt 9).
4. **Das HQ wächst und verändert sich** (Spielvision §6.4); der Zustand der Crew ist Teil davon.

---

## 6. Entwurf: Der Hund

Der Hund hat nach den Quellen eine besondere Stellung (N-CO-014). Die Verhaltens- und Tiermechanik gehört in die Tierdokumentation ([Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md), [Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md)). Aus NPC-Sicht gilt (💡):

| Aspekt | Entwurf |
|---|---|
| **Datensatz** | Tier-NPC mit ID, Rasse, Alter, Persönlichkeit, Training, Gesundheit, Narben, Besitzer; Persistenz A |
| **Beziehung** | Bindung zum Spieler (und zu Crew); Vertrautheit, Vertrauen; der Hund hat keine „Dialoge“, aber Ausdruck |
| **Gedächtnis** | Erkennt Menschen, Orte, Gerüche; einfaches Gedächtnis (Ereignisse, die ihn betreffen) |
| **Training** | Fähigkeiten wachsen durch Training und Wiederholung (organisch, nicht per Level); `23#1088A` Frage 33 |
| **Einsatz** | kann zuhause bleiben (Frage 37); nicht jeder Fall ist sicher |
| **Tracking** | umweltbezogen (Regen, Wasser, Zeit, Verkehr, Gerüche: Frage 35; kein magischer Zielpfeil) |
| **Reaktion** | auf Tiere und teils Übernatürliches, früher als Menschen aber nicht perfekt (Frage 36; N-WE-006) |
| **Verletzung, Alter, Tierarzt** | Fragen 38–40; Gesundheit und Lebensdauer folgen der Weltzeit |
| **Soziale Wirkung** | Hundebesitzer sind sozial vernetzt (Gassigehen, Hundeschule) 💡 |
| **Tod** | offen (Q-N22) |

**Hund und Beziehungen.** Hunde beeinflussen Menschen (Zuneigung, Misstrauen, Angst). Ein NPC kann den Hund des Spielers mögen; ein Feind kann ihn bedrohen (Modul 11 §6: Nahestehende).

---

## 7. Entwurf: Rekrutierung

Die Quellen verlangen Rekrutierung aus optionalen Fraktionen und der Welt (N-CO-021 bis N-CO-025). Der Entwurf legt die Mechanik fest (💡), als Kette aus Beziehung, Ereignis und Gelegenheit.

### 7.1 Kette

```text
Kontakt  ──►  gemeinsame Jobs / Erlebnisse  ──►  Vertrauen (Modul 08, 09)
                                                     │
                                       übernatürliches Ereignis / Einblick in die Hunter-Welt
                                                     │
                                          Recruitment Opportunity  (Gelegenheit)
                                                     │
                       Entscheidung des Kandidaten  +  Entscheidung des Spielers
                                                     │
                                   ┌─ Ablehnung (Kandidat bleibt, Beziehung ändert sich)
                                   └─ Aufnahme  ──►  Umzug ins HQ ──►  Integration (Abschnitt 8)
```

Dieser Verlauf folgt dem Beispiel in `23#678A` (Biker-Mitglied → mehrere Jobs → Vertrauen → übernatürliches Ereignis → Recruitment Opportunity → Hunter-HQ).

### 7.2 Kandidatenvoraussetzungen

| Voraussetzung | Beschreibung |
|---|---|
| **Definierter Kandidat** | nur ausgewählte NPCs sind rekrutierbar (N-CO-022); im Datensatz das Flag `recruitable` (Modul 16 §5) |
| **Beziehung** | ausreichendes Vertrauen und Bindung, **gemessen als Verhalten**, nicht als Zahl |
| **Wissen** | der Kandidat erfährt (oder ahnt) etwas von der Hunter-Welt; sonst keine Gelegenheit |
| **Motiv** | Rache, Schuld, Bindung, Neugier, Überzeugung, Not |
| **Verfügbarkeit** | frei, nicht eingesperrt, nicht tot; Gruppenbindung lösbar (Austritt, Abschied) |
| **Fähigkeiten** | etwas, das die Crew braucht (N-CO-023) |
| **HQ-Kompatibilität** | Chance auf Konflikt mit Crew (N-CO-023) |
| **Freiheit** | der Kandidat kann ablehnen |

### 7.3 Herkünfte (Quelle `23#678A`, 🧭)

Mögliche Herkunft: Motorradclub, Sekte, kriminelle Organisation, extremistische Gruppe, Polizei, Militär, Arzt oder Paramedic, Mechaniker, Racer, Wissenschaftler, Occultist, ehemalige Monsteropfer, normale Zivilisten. Jede Herkunft bringt **Fähigkeiten, Wissen und Konflikte** mit (Beispiele der Quelle, 🧭: Ex-MC-Mechaniker mit Fahrzeugen, Bikes, Kontakten, Straßenwissen; ehemaliges Sektenmitglied mit Insiderwissen und Symbolerkennung; ehemaliger Extremist mit Netzwerkkenntnis und innerem Konflikt; Racer mit Fahren, Tuning, Motorsportkontakten; Arzt mit Medizin, Trauma, Forensik).

### 7.4 Zeitpunkt und Menge

Rekrutierung geschieht über die ganze Spielzeit und nach der Hauptstory (N-CO-024). Die Zahl der Rekruten ist nicht festgelegt (❓ Q-N23). Das HQ hat Kapazität (`M-PD`: HQ wächst); ob Platz begrenzt ist, ist offen.

### 7.5 Rekrutierung ist optional und nie notwendig

Die Regel der Quelle: „Optional Recruit → mehr Optionen / Dialog / Unterstützung“, aber **niemals** „einzige Möglichkeit, Hauptstory fortzusetzen“ (`23#678A`). Prüfregel N-EN-406.

---

## 8. Entwurf: Integration ins Hauptquartier

Ein Rekrut wird Mitglied der Crew-Gemeinschaft; die Integration ist ein **sozialer Verlauf** (N-CO-023, `23#678A` ‚HQ Integration → Crew Reactions → Conflicts → Trust Development‘).

| Phase | Merkmale (Entwurf) |
|---|---|
| **Ankunft** | Zimmer, Ausrüstung, Vorstellung; Crew-Reaktionen nach Doktrin und Erinnerung |
| **Beobachtung** | Misstrauen oder Neugier; Prüfungen; Gerüchte im HQ |
| **Konflikt** | Spannung zwischen Rekrut und einzelnen Crew-Mitgliedern (frühere Zugehörigkeit, Ideologie, Taten) |
| **Vertrauensaufbau** | gemeinsame Einsätze, Hilfe, Ehrlichkeit |
| **Platzfindung** | Rolle in der Crew (Mechaniker, Fahrer, Arzt) |
| **Dauerhaft** | Teil der Gemeinschaft, oder scheitert (verlässt das HQ) |

**Offenes Ende.** Die Quelle lässt ausdrücklich offen, ob eine Crew jemanden je vollständig akzeptiert (`23#678A`: „Vielleicht akzeptieren manche ihn nie vollständig. Vielleicht verändert er sich. Vielleicht bleibt er bei einigen seiner Ansichten. Vielleicht stellt sich heraus, dass er trotzdem in bestimmten Situationen extrem loyal ist.“). Das Spiel entscheidet es nicht; der Spieler beurteilt (N-LT-030).

**Rückfall und Verrat.** Ein Rekrut kann das HQ verraten (alte Gruppe, Druck, Erpressung). Das ist eine Weltmöglichkeit, kein Zwang (Modul 16, 17).

**Skills im Einsatz.** Rekruten können in optionalen Situationen helfen (zusätzlicher Dialog, besondere Vorbereitung, Wissen, Route: `23#678A` ‚Mission 47‘-Beispiel). Das ändert nie die Hauptstory-Spine (N-CO-021).

---

## 9. Entwurf: Entwicklung der Crew ohne den Spieler

Auch die Crew lebt weiter (N-LT-006, N-CO-015). Entwurf:

| Aspekt | Beschreibung |
|---|---|
| **Aufträge** | Crewmitglieder bearbeiten Fälle, bezahlte Aufträge, Recherche, Reparatur, Einkäufe (N-CO-016, N-CO-019) |
| **Beziehungen** | Freundschaften, Streit, Paare, Trennungen innerhalb der Crew (Modul 09) |
| **Verletzung, Genesung** | Verletzte pausieren (Modul 14) |
| **Lebensereignisse** | Familie, Umzug, Krankheit (Modul 14) |
| **Weggehen** | dauerhaftes Gehen ist möglich (N-CO-017) |
| **Wachstum** | Fähigkeiten durch Benutzung (kein Level) |
| **Erinnerungen** | Crew erinnert gemeinsame Erlebnisse; Dialoge nutzen sie |
| **Kommunikation** | Anrufe, Nachrichten (Kommunikationsstil: Modul 04 §7) |
| **Außenkontakte** | Kontakte zu Behörden, Fraktionen, Familien |

**Schutz.** Die Entwicklung der Crew geschieht innerhalb der Schutzstufen (Modul 03 §8). Kernfiguren erleben nur geschriebene Wendepunkte.

---

## 10. Entwurf: Koop

Die Quellen sehen Koop mit bis zu vier Spielern vor (`CLAUDE.md` §6). Die in Chat 48 besprochene externe Companion-App (Spielarchiv, Karte, KI-Assistent) ist eine eigene Plattform und nicht Teil dieses Ordners. Für **KI-Begleiter im Koop** sind keine Entscheidungen gefunden. Entwurfsfragen (❓ Q-N15, Q-N26):

- Was geschieht mit KI-Begleitern, wenn mehrere Menschen spielen?
- Wessen Begleiter ist ein NPC-Hunter, wenn zwei Spieler ihn kennen?
- Wie werden Beziehungen eines Begleiters zu mehreren Spielern geführt?

Die Grundregel: Der **Host-Speicherstand** ist maßgeblich (N-KO-001 in Modul 23); Gast-Fortschritt ist an den Host-Speicherstand gebunden.

---

## 11. Sonderfälle

1. **Begleiter wird getötet, weil der Spieler ihn nicht schützte.** Schuldfrage; Crew-Reaktion; Trauer (Modul 14 §8).
2. **Begleiter stirbt durch Eigenfeuer.** Folge von Spielerhandlung; Eigenfeuer-Einstellung beachten (N-CO-013).
3. **Zwei Begleiter hassen einander.** Planung und Stimmung leiden; Spieler muss vermitteln.
4. **Begleiter weigert sich, einen Befehl auszuführen.** Dialog, Konsequenz, Konflikt (N-CO-004).
5. **Begleiter verrät den Spieler.** Möglichkeit (Erpressung, Rekrutierung durch Gegner); Modul 16, 17.
6. **Begleiter wird besessen.** Der Spieler muss ihn retten oder ihn bekämpfen (Kreaturen 05).
7. **Rekrut wird vom Hunter-Netz abgelehnt.** Er geht oder bleibt im Randbereich.
8. **Der Spieler verlässt die Crew nach einem Streit.** Offen (Q-N17 verwandt).
9. **Begleiter ist Wesen mit menschlicher Identität.** Tarnidentität; Aufdeckung (Modul 18 §6).
10. **Hund wird gestohlen oder entführt.** Fall, Rache, Nemesis-Möglichkeit.
11. **Begleiter und Nemesis haben Vorgeschichte.** Nemesis kennt den Begleiter (Modul 11 §6).
12. **Begleiter altert.** Alterung und Ruhestand (Modul 14 §3, Q-N21).
13. **Begleiter wurde in der Vergangenheit eines Zeitreise-Abschnitts getroffen.** Zeitstände (N-GE-028).
14. **Neu rekrutierter NPC hat bereits eine Geschichte mit anderen Crewmitgliedern.** Beziehungskanten existieren vorab; die Integration berücksichtigt sie.

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-391 | Ein Begleiter lehnt eine Handlung nur aus Persönlichkeit, Doktrin oder Lage ab, mit sichtbarem Grund | N-CO-004 |
| N-EN-392 | Ein Begleiter weicht nur bei unmittelbarer Gefahr oder neuer Information vom Plan ab | N-CO-011 |
| N-EN-393 | Ein Begleiter sabotiert nie zufällig | N-CO-011 |
| N-EN-394 | Ein verletzter Begleiter steht in der Zeit seiner Genesung nicht zur Verfügung | N-CO-006 |
| N-EN-395 | Ein gewöhnlicher Begleiter bleibt nach dem Tod tot | N-CO-007 |
| N-EN-396 | Ein Kernmitglied des HQ stirbt nicht durch Sandbox-Zufall | N-CO-007 |
| N-EN-397 | Ein Todesfall in der Crew führt zu Reaktionen von Crew, HQ und Beziehungen | N-CO-007, N-LB-016 |
| N-EN-398 | Persönliche Anfragen entfallen nur durch Tod oder dauerhaftes Gehen | N-CO-017 |
| N-EN-399 | Crew-Aktivitäten verschwinden nicht durch ein verpasstes Zeitfenster | N-CO-018 |
| N-EN-400 | Nur als rekrutierbar definierte NPCs können rekrutiert werden | N-CO-022 |
| N-EN-401 | Rekrutierung folgt einer Kette von Beziehung, Wissen, Gelegenheit und Entscheidung | Abschnitt 7 |
| N-EN-402 | Ein Rekrut bringt die Fähigkeiten, Beziehungen und Konflikte seiner Herkunft mit | N-CO-023 |
| N-EN-403 | Die Crew reagiert auf einen Rekruten nach Persönlichkeit und Erinnerung, nicht einheitlich | N-CO-023 |
| N-EN-404 | Crewmitglieder haben Abläufe außerhalb der Anwesenheit des Spielers | N-CO-015 |
| N-EN-405 | Der Hund ist umweltbezogener Spurenfolger, kein Zielpfeil | N-CO-014 |
| N-EN-406 | Kein Hauptmissionsabschnitt hängt von einem Rekruten ab | N-CO-021 |
| N-EN-407 | Ein Begleiter, der abwesend ist, wird bei Rückkehr aus seinem Zustand konsistent dargestellt | N-LE-013 |
| N-EN-408 | Ein Begleiter erinnert Rettung, Verlassen, Lüge und Verletzungen wegen Entscheidungen | N-BV-004 |

---

## 13. Was fehlt

- Keine Crew-Liste, keine Namen, Biografien, Spezialisierungen (N-CO-026).
- Offen: Wie Begleiter im Koop funktionieren (Q-N26).
- Offen: Tod des Hundes (Q-N22).
- Offen: Zahl der möglichen Rekruten und HQ-Kapazität (Q-N23).
- Offen: wer außer Spieler und Story Kernfiguren töten darf (C-N1).
- Offen: der Ablauf bei Beinahe-Tod und Spielertod (`M-PD` OFFEN).
