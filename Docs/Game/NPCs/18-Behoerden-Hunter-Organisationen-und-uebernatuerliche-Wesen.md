# NPCs 18: Behörden, Hunter-Organisationen und übernatürliche Wesen als Akteure

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die drei großen Akteursgruppen der Welt, die zwischen Bevölkerung, Fraktionen und Spieler stehen: **Behörden**, **Hunter-Organisationen** und **übernatürliche Wesen unter Menschen**. Beschlossen sind die Grundzüge (mehrere Behörden mit Zuständigkeiten, keine Allwissenheit, Fahndung bleibt lange bestehen; mehrere Hunter-Organisationen, kleine Einheiten mit Teilwissen; Wesen mit eigenem Leben); **die Akteursmodelle, Wissensverteilung und Dreiecksbeziehungen sind Entwurf (💡)**. Die Detailregeln stehen in den verlinkten Ordnern. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Behörden: [Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md), [14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md), [15](../Ermittlung/15-Gerichte-und-Verfahren.md), [16](../Ermittlung/16-Haft-und-Strafvollzug.md). Hunterwelt: [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md). Wesen unter Menschen: [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Behörden als Akteure](#2-entwurf-behörden-als-akteure)
3. [Entwurf: Beamte als Individuen](#3-entwurf-beamte-als-individuen)
4. [Entwurf: Hunter-Organisationen](#4-entwurf-hunter-organisationen)
5. [Entwurf: NPC-Hunter im Alltag](#5-entwurf-npc-hunter-im-alltag)
6. [Entwurf: Wesen unter Menschen](#6-entwurf-wesen-unter-menschen)
7. [Entwurf: Wer weiß was](#7-entwurf-wer-weiß-was)
8. [Entwurf: Dreiecksbeziehungen und Konflikte](#8-entwurf-dreiecksbeziehungen-und-konflikte)
9. [Sonderfälle](#9-sonderfälle)
10. [Entwurf: Prüfregeln](#10-entwurf-prüfregeln)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-BH-001 | Es gibt mehrere Behörden (City Police, County Sheriff, State Police/Highway Patrol, Rangers usw.) mit unterschiedlichen geografischen Zuständigkeiten (Jurisdiktionen) | 📋 | `23#1056A` Fragen 57–58; „ja sicherlich“ und „ja“ in `23#1057E` |
| N-BH-002 | Die Polizei weiß nicht magisch alles; bei Verdacht kann es zu einem Verhör kommen | 📋 | `23#1056A` Frage 59; „ja genau, wenn man verdächtigt wird kann es zu einem verhör kommen und so weiter“ in `23#1057E` |
| N-BH-003 | Zeugen rufen Notruf; Täterbeschreibung (Kleidung, Fahrzeug, Kennzeichen, Richtung); Kennzeichenabfrage; Verkehrskontrollen (normale Stops oder Verfolgung, je nach Lage); Verhalten des Spielers entscheidet, ob eine Situation harmlos endet oder eskaliert | 📋 | `23#1056A` Fragen 60–64; Antworten in `23#1057E` |
| N-BH-004 | Schwere Fahndung bleibt lange bestehen, bis das Kopfgeld bezahlt, die Klage zurückgezogen oder der Beschuldigte freigesprochen wird: „sicher nicht, sie bleiben lange bestehen bis man kopfgeld bezahlt, oder die klage zurückgezogen oder man freigesprochen wird“ | 📋 | `23#1056A` Frage 65; Antwort in `23#1057E`; `23#1058A` ‚Man kann tatsächlich zum landesweit Gesuchten werden‘ |
| N-BH-005 | Bei schweren Taten folgen Ermittlungen und Detectives; Beweise (Kameras, Zeugen, Kennzeichen, Spuren) werden selektiv simuliert; Festnahme führt zu Verhör, Geldstrafe, Kaution, Gerichtsverfahren, Haft, Zeitsprung statt Game Over; Anwälte und Kaution später ausbaubar | 📋 | `23#1056A` Fragen 66–69; Antworten in `23#1057E`; `23#1058A` |
| N-BH-006 | Das Rechtssystem verfolgt tatsächliche Information und Beweissicherheit über Jurisdiktionen hinweg statt allwissender Fahndungslogik; Beweise sind Zeugen, Kameras, Kleidung, Gesicht, Fahrzeuge, Kennzeichen, Waffen/Ballistik, Fingerabdrücke/DNA, digitale Beweise; Festnahme ist eine echte Alternative zur tödlichen Polizeigewalt; Polizei-K9 und Geruchs-Wind-Modell erwünscht | 📋 | `M-PD` ‚Cash / law / evidence‘ |
| N-BH-007 | Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen; bestimmte kriminelle Gruppen können wissen, wer der Spieler ist | 📋 | `23#1056A` Fragen 43–44; „ja“ in `23#1057E` |
| N-BH-008 | Die Polizei erklärt Dinge zunächst normal; die meisten Polizisten wissen nichts vom Übernatürlichen; kleine geheime Einheiten in Polizei und Bundesbehörden wissen teilweise davon | 📋 | `23#1056A` Frage 116; „ja so wie in der serie“ in `23#1057E`; „21. ja“, „22. genau“ in `23#1075E` ([Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §5) |
| N-BH-009 | Die Polizei reagiert auch ohne den Spieler: Notruf → Beamte kommen → Täter kann fliehen oder festgenommen werden; nach größeren Verbrechen können Orte abgesperrt sein | 📋 | `23#1056A` Fragen 49–50; „ja sicher“ und „ja genau“ in `23#1057E` |
| N-BH-010 | Polizeifahndung und Strafverfolgung sind vollständig dynamisch: Zuständigkeiten, verdeckte Ermittlungen, Identitätsdiebstahl, Beweismanipulation, Korruption, Langzeitfolgen | 📋 | „D“ in `48#155E`; Auffächerung `48#154A`, `48#156A` (💡); [Ermittlung 14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md) |
| N-BH-011 | Behörden, Hunter-Organisationen und private Sicherheitsorganisationen reagieren auf Ereignisse und passen ihr Verhalten langfristig an; es gibt geheime Hunter-Netzwerke, staatliche Sondergruppen und private Sicherheitsorganisationen | 📋 | `48#145E`; Tabelle `48#146A` (💡) |
| N-BH-012 | Mehrere Hunter-Organisationen (pragmatische Hunter, Forscher, religiöse Orden, alte Geheimorganisationen, kleine staatliche Programme, private Gruppen); eine wissenszentrierte Hauptorganisation mit eigener IP; sie weiß mehr als der normale Hunter, aber nicht alles; interne Fraktionen, Korruption, Verrat | 📋 | `23#1075E` Punkte 16–20; `M-B09`; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §4 |
| N-BH-013 | NPC-Hunter haben Ruf, können sterben, Fälle vor dem Spieler bearbeiten, falsch behandeln, konkurrieren oder kooperieren; ignorierte Fälle werden manchmal von ihnen gelöst; sie haben eigene Handlungsfreiheit | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘; `23#1074A` Punkte 11–15, 66; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §3 |
| N-BH-014 | Das Hunter-Netz ist selten; Hunter sind kein Massenphänomen | 📋 | `M-B09` (‚rare hunter network‘) |
| N-BH-015 | Menschenähnliche übernatürliche Wesen, die niemandem schaden, können ein normales, dauerhaftes Leben mit Beruf, Haus, Familie, Freunden, Routinen führen, wenn der Spieler sie verschont | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |
| N-BH-016 | Ein verschontes friedliches Wesen wird nicht automatisch zum Feind; ein anderer Hunter kann es töten, wenn es zu seinem Wissen, Charakter, seiner Ideologie und Lage passt | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |
| N-BH-017 | Die Gesellschaft reagiert auf das Übernatürliche mit Behördenermittlungen, Absperrungen, Sondereinheiten, politischen Entscheidungen, geheimen Programmen, Medienberichten, Vertuschung; die Welt entwickelt sich auch ohne den Spieler | 📋 | `48#145E`; `48#146A` (💡) |
| N-BH-018 | Staatliche Monsterforschung existiert (unterirdische Labore u. a.) und wird geheim gehalten, weil öffentliches Wissen Angst erzeugt | 📋 | `48#171E` ([Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §5; [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md)) |
| N-BH-019 | Eine Elite im Hintergrund verfolgt viele verschiedene Ziele und will manches um keinen Preis öffentlich machen; der Entwickler hält es für möglich, dass Monster teilweise von ihr stammen. Nach dem Vorschlag des Assistenten wirkt sie fast allmächtig, ist es aber nicht, und ihre wahre Struktur wird nicht von Anfang an erklärt | 📋 Richtung / 💡 | `48#179E`; Vorschlag `48#178A` |
| N-BH-020 | Bestimmte NPCs, darunter Polizisten, sind Teil der Hunter-Crew oder des Netzes (Rekrutierung aus Polizei, Militär, Medizin, Werkstatt, Wissenschaft, Okkultismus, Opfern, Zivilisten) | 🧭 | `23#678A` (Vorschlag des Assistenten) |

---

## 2. Entwurf: Behörden als Akteure

Behörden sind Fraktionen im Sinn von [Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md): Sie haben Struktur, Mitglieder, Mittel, Ziele, Wissen und Beziehungen. Sie unterscheiden sich in **Rechtsmacht**, **Verfahren** und **Rechenschaft**. Die Ermittlungs-Dokumentation beschreibt ihr Verhalten (Fahndung, Verfahren, Haft). Dieses Modul legt fest, wie sie im **Akteursgefüge** stehen (💡).

### 2.1 Behörden als Datensätze

```text
Agency (Behörde)  — Spezialfall einer Faction (Modul 16)
├── kind            City Police | County Sheriff | State Police | Rangers | Feuerwehr | Rettung |
│                   Gericht | Gefängnis | Bundesbehörde | Sondereinheit (geheim) | Kommunalverwaltung
├── jurisdiction    Gebiet, Delikte, Befugnisse (Zuständigkeitskarte)
├── structure       Dienstgrade, Dienststellen, Abteilungen (Streife, Kripo, Spezial, Verwaltung)
├── members[]       Beamte (NPC-Stufe A-C), Dienstplan, Standort
├── resources       Personal, Fahrzeuge, Labor, Datenbanken, Budget
├── knowledge       Akten, Fahndungen, Informanten, Berichte  (Pool mit Zugriffsrechten)
├── relations       zu anderen Behörden, Gruppen, Medien, Politik
├── culture         Korruptionsniveau, Gewaltbereitschaft, Professionalität, Außenwirkung
├── procedures      Verfahrensregeln (Festnahme, Verhör, Durchsuchung, Weitergabe)
└── supernatural    kennt Teilwahrheit?  (Sondereinheit | einzelne Mitwisser | nichts)
```

### 2.2 Zuständigkeit und Zusammenarbeit

Behörden haben **geografisch und sachlich unterschiedliche Zuständigkeiten** (N-BH-001). Konsequenzen (Entwurf):

1. **Der Weg einer Information folgt dem Dienstweg.** Zeuge → Notruf → zuständige Stelle → Streife → Ermittler; Zuständigkeitswechsel dauern (Ermittlung 14).
2. **Zusammenarbeit und Konkurrenz.** Behörden teilen Wissen unvollständig; Kompetenzstreit, Informationslücken und Übergaben erzeugen Reibung, aber auch Rettungsketten (Ermittlung 26 §4).
3. **Jurisdiktionswechsel.** Ein Täter, der die Grenze überschreitet, entzieht sich teilweise der Verfolgung; Informationen werden nur nach Dienstweg geteilt (`M-PD`: ‚across jurisdictions‘).
4. **Lokale Besonderheiten.** Eine kleine Sheriff-Dienststelle handelt anders als ein großes Stadtdezernat (Ressourcen, Kultur, Bekanntheit).
5. **Behörden haben Gedächtnis.** Akten und Fahndungen halten Wissen fest (Modul 05 §10); sie können veralten, verloren gehen, manipuliert werden (`48#154A`: Beweismanipulation).

### 2.3 Verhalten gegenüber dem Spieler

Die Quellen verlangen, dass das Verhalten der Polizei vom Spielerverhalten abhängt (N-BH-003, N-BH-007). Entwurf:

| Einflussgröße | Wirkung |
|---|---|
| **Eigene Vorgeschichte mit der Dienststelle** | wiederholte Probleme ändern Haltung (N-BH-007) |
| **Verhalten bei Kontakt** | höflich, aggressiv, bewaffnet; Eskalation (N-BH-003) |
| **Bekanntheit, Ruf** | lokale Polizei kennt oder erkennt den Spieler (Modul 08 §8) |
| **Beweislage** | je mehr Beweise, desto härter (N-BH-006) |
| **Zeugen und Aussagen** | Menge und Qualität (Modul 06 §7) |
| **Einzelne Beamte** | persönliche Beziehung, Sympathie, Schuld (Modul 08, 09) |
| **Fahndungsstatus** | schwere Fahndung bleibt lange bestehen (N-BH-004) |
| **Lokale Lage** | Konflikt, Katastrophe, Medienlage |
| **Korruption** | manche Beamte sind bestechlich oder in Gruppen eingebunden (N-BH-010) |

**Entscheidungsweg der Polizei (Entwurf):** Ereignis → Meldung → Bewertung durch Dienst-/Einsatzleitung → Einsatz (Streife/Ermittler) → Sachverhalt (Beweise) → Hypothese → Maßnahmen (Befragung, Fahndung, Festnahme). Jeder Schritt arbeitet mit tatsächlich vorliegender Information (N-BH-006).

### 2.4 Behörden und das Übernatürliche

Die Mehrheit der Beamten weiß nichts und sucht normale Erklärungen (N-BH-008). Kleine Einheiten wissen teilweise Bescheid. Der Entwurf lässt dies über **Wissensflaggen** im Behördendatensatz laufen (siehe Abschnitt 7). Eine Behörde kann Wissen **verwenden, verbergen, weitergeben oder missbrauchen**.

---

## 3. Entwurf: Beamte als Individuen

Behörden bestehen aus Personen. Die Beziehungen zum Spieler laufen über Beamte (Modul 08), nicht über „die Polizei“ als Zahl.

| Rolle | Merkmale (Entwurf) | Persistenz (Modul 03) |
|---|---|---|
| **Streifenbeamter** | viele, wechselnd; erste Anlaufstelle | C, bei Begegnung B |
| **Sheriff, Chef** | Leiter einer Dienststelle; Beziehungen, Politik | A/B |
| **Detective, Ermittler** | verfolgt Fälle; Hypothesen; zeitliche Kontinuität | A/B |
| **Spezialeinheiten** | Spurensicherung, K9, Taucher, Einsatzgruppe | B |
| **Rechtsmediziner, Labor** | Befunde, Zeitaufwand ([Ermittlung 11](../Ermittlung/11-Forensik-und-Labor.md)) | B |
| **Verwaltung, Archiv** | Akten, Zugang | C |
| **Verbindungsbeamte** | Kontakt zu Hunter-Netz (selten) | A |
| **Korrupte Beamte** | Gefälligkeiten, Schweigen | B |
| **Verdeckte Ermittler** | Infiltration (Ermittlung 13 §9) | A |

**Eigenschaften (Entwurf):**

1. **Beamte haben Persönlichkeit, Gedächtnis, Beziehungen und Ziele** (Modul 04, 05, 08).
2. **Pflicht und Gewissen.** Dienstvorschrift und persönliche Haltung können kollidieren (Schweigen, Wegschauen, Gefallen).
3. **Der Sheriff-Fall der Quelle** (`23#51A`): Wer einen Sheriff rettet, kann Jahre später Zugang zu Polizeidaten bekommen („Nach dem, was damals auf der Farm passiert ist, schulde ich Ihnen ohnehin etwas.“). Das ist ein Muster, kein Zahlenwert.
4. **Karriere.** Beamte steigen auf, wechseln, werden versetzt, scheiden aus (Modul 14).
5. **Nemesis-Möglichkeit.** Ein Ermittler kann dem Spieler als beharrlicher Gegner begegnen (Verfahren statt Gewalt: Modul 11 §4); er ist ein legaler Nemesis, der Fakten sammelt.
6. **Koexistenz.** Einzelne Beamte können Verbündete, Informanten oder Mitwisser sein, ohne dass die Behörde als Ganzes so handelt.

---

## 4. Entwurf: Hunter-Organisationen

Die Quellen beschreiben die Hunter-Welt in [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md). Hier die Einbindung in das Akteursmodell (💡).

### 4.1 Organisationen als Fraktionen

| Merkmal | Entwurf |
|---|---|
| **Plural** | mehrere Organisationen mit unterschiedlichen Philosophien (N-BH-012) |
| **Wissen** | wissenszentrierte Hauptorganisation: viel Wissen, aber nicht alles; Fragmente bei anderen (N-TE-017) |
| **Struktur** | Hierarchie, Fraktionen innerhalb, Korruption, Verrat (N-BH-012) |
| **Mitgliedschaft** | selten, Vertrauensbasis; Zugang zu Archiv, Ausrüstung, Safehouses (Modul 16 §6) |
| **Netz** | lose Hunter, Familien, Einzelgänger |
| **Zusammenarbeit** | Austausch von Fällen, Wissen, Hilfen; Konkurrenz um Fälle und Status |
| **Gegner** | Monstergruppen, Kulte, Behörden bei Eingriffen, Nemesis |
| **Rekrutierung** | selten; Kandidaten aus Faction-NPCs, Opfern, Berufen (N-BH-020) |
| **Sicherheit** | HQ und Standorte nicht dauerhaft sicher (N-NE-007) |

### 4.2 Interne Dynamik

Die Hunter-Organisation folgt den Regeln der Innenpolitik (Modul 16 §7): Streit um Methoden, Wissen, Verrat, Führung. Das macht die Hunter-Welt zu einer lebendigen Gesellschaft statt einer einheitlichen Fraktion (`23#1075E` Punkte 19–20: „19. ja“, „20. ja sowie überall“).

### 4.3 Verhältnis zu Behörden

Hunter bewegen sich außerhalb des Gesetzes, wo sie Beweise manipulieren, Tatorte betreten, Zeugen beeinflussen. Beziehungen (Entwurf): Misstrauen, Duldung, Zusammenarbeit über Mitwisser, Konflikt, Bestechung. Hunter müssen Spuren vermeiden (Ermittlung 17, Geheimhaltung).

### 4.4 Verhältnis zu Wesen

Hunter sehen Wesen unterschiedlich: Jäger, Forscher, Bewahrer (Kreaturen 11 §3.2, N-BH-016). Ein NPC-Hunter kann ein friedliches Wesen töten, wenn es zu seinem Wissen, Charakter, seiner Ideologie und Lage passt; das ist die Quelle von Konflikten, die der Spieler nicht kontrolliert (Modul 10 §9 Racheketten).

### 4.5 Verhältnis zu Fraktionen und Gesellschaft

Hunter treffen auf Clubs, Kulte, Gangs; sie rekrutieren aus ihnen oder kämpfen gegen sie (N-FR-004, Modul 16). Hunter-Familien geben Wissen und Feindschaften weiter.

---

## 5. Entwurf: NPC-Hunter im Alltag

Hunter-NPCs sind Menschen mit Leben (Modul 13), die zusätzlich ihr verborgenes Gewerbe betreiben. Aus den Quellen (N-BH-013) folgt (💡):

| Aspekt | Entwurf |
|---|---|
| **Doppelleben** | Beruf, Familie, Wohnung, plus Hunter-Tätigkeit; Tarnung (Ermittlung 13 §10) |
| **Fälle** | Hunter bearbeiten Fälle in der Welt; ein Fall kann vor dem Spieler erledigt werden (`M-PD`) |
| **Ruf** | Ruf unter Huntern (zuverlässig, rücksichtslos, inkompetent, gefährlich) |
| **Konkurrenz** | um Fälle, Status, Wissen, Ressourcen |
| **Kooperation** | Hilfe, Warnung, Tipp, gemeinsame Jagd (Companion: Modul 19) |
| **Gefahr** | Hunter sterben (N-BH-013); Folge: Erinnerung, Racheketten, Archivlücken |
| **Wissen** | Familienwissen kann falsch sein (Kreaturen 11 §3.2) |
| **Moral** | Hunter handeln nach Überzeugung; Kollision mit dem Spieler möglich |
| **Weitergegebene Fälle** | Der Spieler kann Fälle im HQ oder Forum melden; NPC-Hunter übernehmen (`23#1075E`) |
| **Verletzung** | Verletzte Hunter brauchen Heilung (Modul 14); Sie fehlen in Fällen |

**Hunter und Spieler.** Der Spieler trifft Hunter nach Spuren, Gerüchten und Netzen; das Vertrauen baut sich wie bei anderen Beziehungen auf (N-BV-002).

---

## 6. Entwurf: Wesen unter Menschen

Die Quellen: Wesen mit menschlicher Identität können ein normales Leben führen (N-BH-015); sie sind keine Pflichtfeinde (N-BH-016). Kreaturen 09 behandelt Identität, Verschonen, Gruppen. Hier die Einbindung in die NPC-Welt (💡):

| Aspekt | Entwurf |
|---|---|
| **Datensatz** | normaler NPC-Datensatz (Modul 03) plus Wesenanteil (Art, Fähigkeiten, Schwächen, Bedürfnisse) |
| **Tarnung** | Name, Beruf, Haushalt, Beziehungen; Spuren dieser Identität auffindbar (Kreaturen 09 §2) |
| **Bedürfnisse** | artabhängig (Nahrung, Schlaf, Orte); sie prägen den Zeitplan (Modul 13) |
| **Beziehungen** | Freundschaften, Familie, Kollegen; sie kennen die Wahrheit oder nicht |
| **Risiko der Entdeckung** | durch Zeugen, Hunter, Ermittlung; Folgen für Netz und Familie |
| **Verhältnis zu Gruppen** | Wesen-Gruppen (Rudel, Zirkel) und menschliche Fraktionen (Kulte, Gangs) |
| **Nützlichkeit** | als Informant, Berater, Verbündeter, Gegner (Kreaturen 09 §4.4) |
| **Verschont** | lebt weiter; kann Dank oder Misstrauen empfinden (Modul 12 §8) |
| **Gefährdet** | Hunter, Kultisten, Behörden, andere Wesen können es bedrohen |

**Gruppen und Gesellschaft der Wesen.** Monster haben Rudel, Clans, Hierarchien, Allianzen, Territorien, Machtkämpfe, Informationsaustausch (Modul 16 §3, 17; N-FR-025). Sie sind Akteure mit eigenen Strategien (N-TE-001).

**Menschen als Werkzeuge.** Wesen können Menschen benutzen und täuschen (Kreaturen 09 §6); in der NPC-Welt sind das Beziehungen mit Machtgefälle, Schuld, Angst, Verblendung (Modul 08).

---

## 7. Entwurf: Wer weiß was

Die Welt hat unterschiedliche Wissensstände über das Übernatürliche. Ein Wissensmatrix-Entwurf (💡) für Akteure (Wissen ist ein Pool, nicht ein Schalter; Modul 05 §10):

| Akteur | Wissen über das Übernatürliche (Entwurf) | Quelle der Grundlage |
|---|---|---|
| **Durchschnittsbevölkerung** | nichts oder Gerüchte, Legenden, persönlicher Glaube | N-WA-016 |
| **Lokale Traditionsträger** | Überlieferung, Teilwissen | `48#148A` |
| **Polizei (Mehrheit)** | nichts; normale Erklärungen | N-BH-008 |
| **Sondereinheiten, Mitwisser** | Teilwissen | N-BH-008 |
| **Staatliche Programme** | Forschung, geheim | N-BH-018 |
| **Medien** | Berichte ohne Anerkennung; Verschwörungen | N-IN-019 |
| **Hunter (lose)** | praktisches Wissen, Aberglauben, Irrtümer | Kreaturen 11 |
| **Wissenszentrierte Organisation** | breites Wissen, nicht alles | N-BH-012 |
| **Kulte** | je nach Kult: Täuschung, Teilwissen, Kontakte | Kreaturen 11 §6 |
| **Wesen** | eigenes Wissen, Hunter-Wissen über Beobachtung | N-NE-002 |
| **Elite im Hintergrund** | Vielschichtig, nicht allwissend | N-BH-019 |

**Regeln (Entwurf):**

1. **Wissen fließt entlang von Beziehungen und Wegen** (Modul 07); Behörden geben Wissen nicht frei weiter.
2. **Wissen kann falsch sein** (N-GE-009).
3. **Der Wissensstand ändert sich langsam** (Kreaturen 11 §8: Über Jahre kann die Bevölkerung stärker glauben).
4. **Keine automatische Gedächtnislöschung** (Kreaturen 11 §8 Punkt 29): Wer etwas sah, erinnert es.
5. **Ereignisse ändern das Wissen:** große Vorfälle, Videos, Enthüllungen (N-WE-014).

---

## 8. Entwurf: Dreiecksbeziehungen und Konflikte

Behörden, Hunter, Wesen, Kulte und Medien bilden ein Spannungsfeld. Typische Konstellationen (💡):

| Konstellation | Dynamik | Spielerwirkung |
|---|---|---|
| **Polizei ↔ Hunter** | Duldung, Misstrauen, einzelne Verbündete; Beweise; Vertuschung | Spieler muss Spuren managen |
| **Polizei ↔ Wesen** | Wesen nutzt Polizei oder wird von ihr verfolgt; Falschzeugen | Fälle mit Verdächtigen, die keine Menschen sind |
| **Hunter ↔ Wesen** | Jagd, Verschonen, Verträge, Informanten | Moralische Konflikte |
| **Kult ↔ Wesen** | Anbetung, Beschwörung, Ausnutzung | Fälle, Opfer |
| **Kult ↔ Polizei** | Verdeckte Ermittlung, Razzia | Infiltration (Ermittlung 13) |
| **Medien ↔ alle** | Berichte, Enthüllungen, Falschmeldungen | Öffentliches Wissen (Modul 07) |
| **Politik ↔ Behörden** | Aufträge, Vertuschung, Druck | Sondermaßnahmen |
| **Elite ↔ Staat** | Einfluss | Verdeckte Programme (N-BH-018) |
| **Fraktionen ↔ Behörden** | Korruption, Kampf, Duldung | Kriminalität (Modul 17) |
| **Nemesis ↔ Behörden** | Anzeigen, Zeugenbeeinflussung, falsche Beweise | legale Gegner (Modul 11) |

**Mechanik (Entwurf).**

1. **Alle Beziehungen sind gerichtete Kanten** zwischen Akteuren (Modul 08 §8, Modul 16 §8).
2. **Ereignisse verschieben die Kanten**; die Verschiebungen sind nachvollziehbar.
3. **Wissen entscheidet über Möglichkeiten:** Wer nichts weiß, kann nicht handeln (N-WA-005).
4. **Geheimnisse sind Ressourcen und Risiken.** Ein Geheimnis kann erpresst, verraten, verloren werden.
5. **Der Spieler kann Kanten verändern** (Enthüllungen, Informationen, Vermittlung, Sabotage).

---

## 9. Sonderfälle

1. **Der Spieler wird von einem Hunter-Rivalen bei der Polizei gemeldet.** Intrigen zwischen Hunter und Behörden; Beweislage.
2. **Ein Beamter ist Mitglied einer Gang oder eines Kults.** Doppelloyalität; Korruptionsfall.
3. **Ein Wesen ist Beamter.** Tarnidentität in der Behörde; Aufdeckung ist Skandal.
4. **Die Polizei verhaftet einen Hunter.** Folgen für Netz, Fälle, Gerüchte; Befreiung.
5. **Sondereinheit tötet ein Wesen und vertuscht.** Der Spieler findet Spuren; Verhältnis zu Staat.
6. **Ein Behördenmitarbeiter ist Informant des Spielers.** Zugangsmöglichkeit; Risiko.
7. **Mehrere Zuständigkeiten und widersprüchliche Berichte.** Der Spieler muss die Wahrheit herausfinden.
8. **Hunter-Organisation wird von innen zerlegt.** Innenpolitik (Modul 16 §7).
9. **Hunter-Rekrut aus dem Polizeidienst.** Rekrutierung (Modul 19).
10. **Ein verschontes Wesen kommt ins Visier der Behörden.** Es bittet den Spieler um Hilfe.
11. **Medienkampagne gegen einen Hunter.** Verdacht, öffentliche Meinung (Modul 07).
12. **Untergrund und Behörden.** **historisch (N-02, 08.11.2026)** für die Untergrundzivilisation; Restfrage für Bunker, Höhlen und kleinere Untergrundbereiche bleibt offen (Q-N25).

---

## 10. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-366 | Eine Behörde handelt nur auf Grundlage tatsächlich vorliegender Information | N-BH-006 |
| N-EN-367 | Eine Zuständigkeit bestimmt, welche Behörde eine Meldung bearbeitet; Informationsweitergabe zwischen Behörden braucht einen Weg | N-BH-001 |
| N-EN-368 | Wiederholte Konflikte mit einer Dienststelle verändern deren Verhalten gegenüber dem Spieler nachvollziehbar | N-BH-007 |
| N-EN-369 | Eine schwere Fahndung endet nur durch einen definierten Abschluss (Kopfgeld, Rücknahme, Freispruch, Verfahren) | N-BH-004 |
| N-EN-370 | Die Polizei kennt das Übernatürliche nur, wenn der Datensatz es erlaubt | N-BH-008 |
| N-EN-371 | Ein Beamter handelt nach Persönlichkeit, Gedächtnis und Beziehung, nicht nur nach Dienstregel | Abschnitt 3 |
| N-EN-372 | Ein NPC-Hunter folgt eigenen Zielen; er löst keine Fälle zum Vorteil des Spielers, nur weil dieser sie ignoriert | N-BH-013 |
| N-EN-373 | Ein NPC-Hunter kann sterben und hinterlässt Spuren (Erinnerungen, Archivlücken, Fälle) | N-BH-013 |
| N-EN-374 | Ein verschontes Wesen wird nicht gezwungen, Feind zu werden | N-BH-016 |
| N-EN-375 | Das Wissen einer Behörde ist ein Pool mit Rollenrechten | Modul 05 §10 |
| N-EN-376 | Staatliche Geheimprogramme sind Spielinhalt der Welt, nicht Pflicht für Hauptstory-Abläufe | N-BH-018 |
| N-EN-377 | Medienberichte und Behördenmitteilungen widersprechen sich nur mit erkennbarem Grund | N-BH-017 |
| N-EN-378 | Wissenszentrierte Organisation weiß nicht alles | N-BH-012 |
| N-EN-379 | Ein Wesen mit menschlicher Identität hat auffindbare Spuren dieser Identität | Kreaturen 09 |
| N-EN-380 | Die Aufdeckung eines Geheimnisses ändert Beziehungskanten nachvollziehbar | Abschnitt 8 |

---

## 11. Was fehlt

- Keine benannte Behörde, Organisation, Dienststelle, Person (Proposal 0010 §12).
- Keine Zahlen: Korruptionsrate, Wissensanteil, Anzahl Hunter.
- Offen: Ausmaß der Staatsprogramme und der Elite (Spoiler- und Lore-Grenze; `48#178A`–`48#179E`).
- Offen: Behörden im Untergrund und die Rolle der Infrastruktur-KI (Q-N25).
- Offen: Verfahren gegen NPC-Hunter und Wesen (Rechtsrahmen).
