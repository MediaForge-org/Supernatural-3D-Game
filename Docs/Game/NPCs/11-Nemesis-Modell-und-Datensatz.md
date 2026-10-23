# NPCs 11: Nemesis und Rivalität – Modell und Datensatz

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt, was die Quellen zu wiederkehrenden Gegnern, Überlebenden, Feindschaft und Rivalität festlegen, und entwirft darauf ein gemeinsames Modell für Menschen und Wesen. **Verbindlich ist die Produktanforderung C** (`PE-N1022`): Überlebende Gegner können zurückkehren, persönliche Feindschaften entwickeln, aus früheren Begegnungen lernen und Taktiken anpassen, auch intelligente übernatürliche Kreaturen. **Alles Weitere (Begriffe, Datensatz, Lernmodell, Grenzen) ist Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Den zeitlichen Ablauf beschreibt [Modul 12](12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md). Artregeln, Schwächen und Monsterwahrnehmung stehen in [Kreaturen](../Kreaturen/README.md), insbesondere [Modul 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §5, §6, §8, §9 und §14.

**Inhalt**

1. [Befund zu den Quellen](#1-befund-zu-den-quellen)
2. [Was beschlossen ist](#2-was-beschlossen-ist)
3. [Entwurf: Begriffe](#3-entwurf-begriffe)
4. [Entwurf: Wer Nemesis werden kann](#4-entwurf-wer-nemesis-werden-kann)
5. [Entwurf: Der Nemesis-Datensatz](#5-entwurf-der-nemesis-datensatz)
6. [Entwurf: Das gelernte Modell des Spielers](#6-entwurf-das-gelernte-modell-des-spielers)
7. [Entwurf: Ziele und Motive](#7-entwurf-ziele-und-motive)
8. [Entwurf: Grenzen der Anpassung](#8-entwurf-grenzen-der-anpassung)
9. [Entwurf: Lernen durch Regeln oder durch Daten](#9-entwurf-lernen-durch-regeln-oder-durch-daten)
10. [Entwurf: Nemesis und Gruppen](#10-entwurf-nemesis-und-gruppen)
11. [Entwurf: Lesbarkeit und Fairness](#11-entwurf-lesbarkeit-und-fairness)
12. [Abgrenzung zu Hauptbedrohungen und geschützten Figuren](#12-abgrenzung-zu-hauptbedrohungen-und-geschützten-figuren)
13. [Sonderfälle](#13-sonderfälle)
14. [Entwurf: Prüfregeln](#14-entwurf-prüfregeln)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Befund zu den Quellen

1. **Nemesis-Mechanik ist neuer Input vom 22.10.2026.** Weder der Master noch frühere Gespräche beschreiben ein ‚Nemesis-System‘ (Modul 01 §1 und §6). Die Anforderung C entstand am 22.10.2026 (`PE-N1022`). Der Entwickler nannte als Gedankenvergleich eine RDR2-Modifikation; das Proposal 0010 hielt fest, dass sie weder Abhängigkeit noch Codebasis ist und dass die Seite nicht geöffnet wurde.
2. **Die Bausteine standen vorher da.** Der Entwickler hatte schon festgelegt: Monster erinnern sich individuell (`48#139E`), reagieren taktisch auf Fallen (`48#137E`), können fliehen und später zurückkehren (`23#1075E`), Hunter-NPCs können Rivalen werden (`19#4E`), friedliche Wesen können ein normales Leben führen (`M-PD`). Das Nemesis-Modell **verallgemeinert** diese Bausteine auf Menschen, Hunter, Gruppenführer und Wesen.
3. **Der ‚Nemesis‘ ist ein Arbeitsbegriff.** Die Quellen sagen ‚Überlebende Gegner‘, ‚wiederkehrende Rivalen‘, ‚persönliche Feindschaften‘. Diese Dokumentation verwendet *Nemesis* als Kurzform (Abschnitt 3); es ist kein Begriff für Menüs oder Dialoge.
4. **Die RDR2-Mod-Idee ist nicht Spezifikation.** Das Proposal 0010 sagt: ‚Our systems are specified independently.‘

---

## 2. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-NE-001 | **Nemesis und Rivalität (C):** Überlebende Gegner können zurückkehren, persönliche Feindschaften entwickeln, aus früheren Begegnungen lernen und ihre Taktik anpassen; das gilt ausdrücklich auch für intelligente übernatürliche Kreaturen | 📋 | `PE-N1022` C |
| N-NE-002 | Intelligente Monster haben ein **umfassendes individuelles Gedächtnis** mit Beziehungen, Rache, Angst und Informationsaustausch | 📋 | „Supernatural Monster-Gedächtnis: Umfassendes individuelles Gedächtnis mit Beziehungen, Rache, Angst und Informationsaustausch“ in `48#139E` |
| N-NE-003 | Das Monstergedächtnis umfasst Begegnungen, Kampferfahrungen (Waffen, Fallen, Strategien), emotionale Reaktionen, Beziehungen, Informationsaustausch und langfristige Entwicklung | 💡 (Auslegung der 📋 Wahl) | `48#140A` |
| N-NE-004 | Monster reagieren taktisch auf vorbereitete Fallen: erkennen, untersuchen, umgehen, sabotieren, täuschen, Gegenstrategien entwickeln; die Mythologie gilt weiter (ein vollständig gefangener Dämon entkommt nicht durch KI) | 📋 / 💡 | „Supernatural Monster-KI bei Fallen: Taktische Monster mit Sabotage und Gegenstrategien“ in `48#137E`; Auslegung `48#138A` |
| N-NE-005 | Ein überlebendes oder geflohenes Wesen kann später mit Erinnerung und früheren Verletzungen wiederkehren; „es kann auftauchen aber es muss nicht“ | 📋 | `23#293A` Punkt 20; „es kann auftauchen aber es muss nicht“ in `23#294E` |
| N-NE-006 | Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf; manche beobachten den Spieler und greifen zu einem besseren Zeitpunkt an; sehr gefährliche Wesen können den Spieß umdrehen und den Spieler jagen | 📋 | `23#1074A` Punkte 58–60; „55-60 ja“ in `23#1075E` |
| N-NE-007 | Das HQ und die Safehouses sind nicht absolut sicher; sehr seltene Gegner können Spielerstandorte herausfinden, wenn man unvorsichtig ist; selten, sonst nervt es | 📋 | `23#1074A` Punkt 61; „61 ja das kann etwa so sein wie bei rdr2, mit dem umziehen“ in `23#1075E` |
| N-NE-008 | Monster lernen nur aus tatsächlich **beobachteten** Informationen, nicht aus globaler Telemetrie; es hängt von Monster und Intelligenz ab | 📋 | `23#293A` Punkt 12; „12. ja aber das kommt auf das monster und die intelligenz ab“ in `23#294E`; `M-PD` ‚Stealth, perception and horror AI‘ |
| N-NE-009 | Die Monster sollen sich **anhand des Spielerverhaltens** schlau anpassen | 📋 | „7. genau, die monster sollen sich anhand von meinem verhalten schlau anpassen“ in `23#254E` |
| N-NE-010 | Besonders intelligente Gegner passen sich an wiederholte Taktiken an | 📋 | „55-60 ja“ in `23#1075E` |
| N-NE-011 | Einzelwesen haben persistente Identität: Name, Alter, Gebiet, Beziehungen, Opfer, Verletzungen, Feinde, Vergangenheit; benannte Wesen haben eigene Archivseite, mehrere Fälle und Storyrelevanz | 📋 | `23#1076A`; „31. ja kann sein, sowie bei leviathanen“ in `23#1075E`; „29. cool“ in `23#308E` |
| N-NE-012 | Monster respawnen nicht; tot ist tot (Ausnahme: Befreiung aus dem Fegefeuer) | 📋 | `23#1075E` Punkte 30, 37; `M-B09` |
| N-NE-013 | Narben und Verletzungen von Wesen bleiben bestehen; zurückkehrende Individuen zeigen die Folgen früherer Kämpfe | 📋 | [Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §6; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §2 L4 |
| N-NE-014 | Menschliche Gegner können fliehen, erstarren oder sich ergeben, wenn sie genug eingeschüchtert sind | 📋 | `M-PD` ‚Multiple-opponent combat, improvised actions and surrender‘ |
| N-NE-015 | Hunter-NPCs können Rivalen werden; sie haben Ruf, können vor dem Spieler an Fällen arbeiten, Fälle falsch behandeln oder konkurrieren | 📋 | `19#4E`; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §3.1 |
| N-NE-016 | Ein verschontes friedliches Wesen wird nicht automatisch zum Feind; andere Hunter können es dennoch töten | 📋 | `M-PD` ‚Peaceful supernatural beings and independent hunter agency‘ |
| N-NE-017 | Lernen ist Wissensaufbau, nicht Stärkerwerden; kein Wert steigt mit jeder Begegnung; keine sichtbaren Stufen | 📋 | `M-PD` ‚Genre and progression identity‘; [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §6 |
| N-NE-018 | Bestimmte kriminelle Gruppen können wissen, wer der Spieler ist; wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | 📋 | `23#1056A` Fragen 43–44; „ja“ in `23#1057E` |
| N-NE-019 | Monster können dem Spieler Verbündete angreifen (Beispiel des Assistenten: der Dämon könnte ‚sich zu rächen, dich zu meiden oder gezielt deine Verbündeten anzugreifen‘) | 💡 | `48#140A` |
| N-NE-020 | Die Anpassung von Wesen ist durch Artregeln und Schwächen des Bestiariums begrenzt, nie durch freie Erfindung | 💡 (Vorschlag, deckt sich mit den Quellen der Kreaturen) | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §4 Punkt 4; [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §14 |
| N-NE-021 | Wiederkehrende Rivalen können im Underground-Fight-Club entstehen (zusammen mit Beziehungen, Trainern, Reputation, Hunter-Kontakten) | 💡 | `23#164A`, `23#166A` (Vorschläge des Assistenten); „ja das es nicht ufc 2.0 ist ist klar.genau so will ich das haben“ in `23#165E` |
| N-NE-022 | Wie ein Nemesis lernt (Regeln oder Daten), welche Historie gespeichert wird, wie häufig er auftritt, was der Spieler von der Feindschaft erkennen kann | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11 |

---

## 3. Entwurf: Begriffe

Damit die Dokumentation nicht verschiedene Dinge mit demselben Wort bezeichnet, trennt der Entwurf sechs Begriffe (💡). Der Spieler sieht keinen dieser Begriffe; es sind Arbeitsbezeichnungen für Datenmodell und Tests.

| Begriff | Bedeutung | Dauer | Beispiel |
|---|---|---|---|
| **Gegner** | Akteur in einer Konfrontation | Dauer der Begegnung | Wache, Wolf, Straßenräuber |
| **Überlebender** | Gegner, der die Begegnung überlebt hat **und** einen Datensatz behält | bis zum Tod oder Vergessen | geflohener Gangster, verwundeter Geist-Gebundener |
| **Rivale** | Akteur im Wettbewerb um Rang, Fall, Gebiet, Anerkennung; nicht zwingend tödlich | langfristig | konkurrierender Hunter, Kämpfer aus der Kampfszene |
| **Feind** | Akteur mit **persönlicher Feindschaft** und Schädigungsabsicht | langfristig | Rächer, Verräter, Gangführer |
| **Nemesis** | Überlebender oder Feind mit anhaltender Absicht gegen den Spieler **und** einem Entwicklungsverlauf (lernt, passt an, sammelt Mittel, kehrt zurück) | langfristig, begrenzt | wiederkehrender Gegner |
| **Hauptbedrohung** | handgeschriebene, langlaufende Gegner der Hauptstory (`M-PD` ‚Long-term story threats‘) | ein oder mehrere Akte | ein Akt-Gegner |

**Eigenschaften der Begriffe (Entwurf):**

1. **Ein Gegner wird nicht automatisch Überlebender.** Nur Begegnungen, die ein Ereignis mit Folge hinterlassen, erzeugen einen Datensatz (N-ID-016). Ein Tier ohne Intelligenz, das flieht, ist Ökologie.
2. **Ein Überlebender wird nicht automatisch Nemesis.** Er wird es, wenn Motiv, Mittel, Gedächtnis und Gelegenheit zusammenkommen (Abschnitt 4).
3. **Ein Nemesis kann zu einem Rivalen oder Verbündeten werden.** Der Verlauf ist nicht unumkehrbar (Modul 12 §8).
4. **Gruppen können als Ganzes Nemesis sein** (Abschnitt 10).
5. **Hauptbedrohungen sind handgeschrieben** und folgen eigenen Regeln (Abschnitt 12).

---

## 4. Entwurf: Wer Nemesis werden kann

Anforderung C ist breit (‚überlebende Gegner‘). Nicht jeder Gegner kann sinnvoll lernen, sich erinnern und planen. Die Eignung ergibt sich aus Eigenschaften (💡):

| Voraussetzung | Beschreibung | Quelle der Grundlage |
|---|---|---|
| **Intelligenz** | kann Beobachtung zu Wissen verarbeiten und planen | `23#293A` Punkt 12 (hängt von Monster und Intelligenz ab) |
| **Gedächtnis** | hat die Art, sich zu erinnern (Individuum mit Datensatz) | `48#139E` |
| **Beobachtung** | hat den Spieler tatsächlich wahrgenommen | N-WA-005 |
| **Überleben** | wurde nicht getötet; ist im Weltzustand `alive`, `missing` oder `captured` | N-NE-012 |
| **Motiv** | hat Grund (Rache, Angst, Gier, Pflicht, Neugier) | Modul 08, 09 |
| **Mittel** | Fähigkeiten, Verbündete, Waffen, Verstecke, Geld | Modul 16, 17 |
| **Gelegenheit** | räumliche und zeitliche Möglichkeit, dem Spieler zu begegnen | Modul 12 |
| **Artregeln** | Art lässt dieses Verhalten zu | [Kreaturen 03](../Kreaturen/03-Klassifikation-Herkunft-und-Individuen.md), 10 |
| **Persistenzstufe** | A oder B (N-ID-016) | Modul 03 |

**Wer in Frage kommt (Entwurf, nicht abschließend):**

| Gruppe | Beispiele (abstrakt) | Besonderheit |
|---|---|---|
| Menschliche Kriminelle | geflohener Täter, Gangmitglied, Anführer, Auftragnehmer | Menschenfraktionen (Modul 16, 17), Polizei (Modul 18) |
| Hunter | Rivale, abgesprungener Hunter, korrumpierter Hunter | Ruf, Netz, Taktik (Kreaturen 11) |
| Kultisten, Sektenführer | Verblendete, Fanatiker | Ideologie (Modul 16) |
| Intelligente Wesen | Dämonen, Vampire, Werwölfe in Gruppen, Hexen u. a. | Artregeln; Territorium, Rudel |
| Wesen mit menschlicher Identität | getarnte Wesen, besessene Menschen | Tarnidentität (Ermittlung 13) |
| Besessene, Verfluchte | Menschen mit übernatürlichem Einfluss | Heilung möglich (Kreaturen 05) |
| Behörden und Beamte | wiederholt konfliktbeladene Polizisten, Ermittler | Verfahren statt Gewalt (Modul 18) |
| Fight-Club-Rivalen | wiederkehrende Gegner | Sport/Szene (N-NE-021) |
| Tiere | nur in Sonderfällen (zum Beispiel Problemtiere) | Ökologie, keine Intelligenz |

**Nicht in Frage kommen** (Entwurf): Wesen ohne die Fähigkeit zur Planung; Wesen, die ihre Art grundsätzlich nicht erinnern lässt; Wesen, die von der Art-Regel her nicht überleben können; Figuren der Stufen C/D, solange sie nicht aufsteigen (Modul 03 §9).

**Selektion statt Flut (Entwurf).** Die Zahl gleichzeitig aktiver Nemesis-Figuren ist begrenzt (Abschnitt 11). Der Weltzustand wählt Figuren nach Fall, Region, Beziehung und Bedeutung; zufällige Gegner mit Datensatz sind die Ausnahme.

---

## 5. Entwurf: Der Nemesis-Datensatz

> 💡 **Vorschlag.** Er besteht aus dem normalen NPC-Datensatz (Modul 03) plus einem Nemesis-Teil, der nur existiert, wenn die Voraussetzungen erfüllt sind. Er übernimmt den Rahmen des Proposals 0010 §4 Punkt 4 (‚Begegnungshistorie, gelerntes Modell der Spielermethoden, Verletzungen und Narben, Ressourcen und Verbündete, Ziele‘) und füllt ihn mit Feldern aus den Kreaturenquellen.

```text
NemesisState  (Teil des NPC-/Wesen-Datensatzes)
├── status           candidate | active | dormant | recovering | hunting | allied |
│                    redeemed | captured | dead
├── role             enemy | rival | stalker | avenger | hunted | turncoat
├── origin_event     Ereignis, das ihn zum Überlebenden machte
├── encounters[]     Begegnungen mit dem Spieler (Zeit, Ort, Umstände, Ausgang,
│                    Verletzungen, wer anwesend, was wirkte, was versagte, was er sah)
├── injuries[]       persistente Verletzungen, Narben, Behinderungen (+ Heilungsstand)
├── losses[]         verlorene Mitglieder, Begleiter, Besitz, Gebiet
├── player_model     gelerntes Modell der Methoden des Spielers (Abschnitt 6)
├── relations        Kanten (Modul 08): Spieler, Begleiter, Familie, Gruppe, Verbündete
├── emotions         Angst, Hass, Respekt, Besessenheit, Pragmatismus  (intern)
├── goals[]          aktuelle Ziele mit Priorität (Abschnitt 7)
├── resources        Verbündete, Verstecke, Waffen, Geld, Fahrzeuge, Informanten
├── plan             aktueller Plan, Zeitfenster, Voraussetzungen
├── adaptations[]    vorgenommene Anpassungen mit Ursache (aus Ereignis) und Art
├── species_ref      Artregeln / Schwächen aus dem Bestiarium (Grenzen)
├── pacing           Mindestabstände, letzter Auftritt, Cooldown, Auftrittsbudget
├── tells            Spuren, Gerüchte, Handschrift, die der Spieler wahrnehmen kann
├── protection       none | authored | story_safe     (Modul 03 §8)
└── archive_view     was im Hunter Archive über ihn steht (Spielerwissen, siehe Ermittlung 04)
```

**Entwurfsentscheidungen:**

1. **Zustand der Entwicklung, nicht Stärke.** Das Feld `status` beschreibt Lage und Absicht, nicht ein Level (N-NE-017). Kein Feld steigt ‚mit jeder Begegnung‘; Anpassungen sind Änderungen von Wissen, Taktik, Mitteln und Verbündeten, die jeweils eine Ursache haben.
2. **Begegnungen sind einzelne Datensätze.** Sie bleiben einzeln erhalten (Anker, Modul 05), damit Dialog, Rache und Archiv konkret darauf Bezug nehmen können.
3. **Verletzungen sind Teil der Figur.** Eine Narbe, ein verlorener Arm, ein Humpeln sind sichtbar und wirken auf Taktik (Modul 12 §4).
4. **Das gelernte Modell ist ein Glaube.** Es kann falsch sein und veraltet (Abschnitt 6).
5. **`tells` ist Spielerwissen-Brücke.** Ohne Spuren, die der Spieler lesen kann, ist ein Nemesis unlesbar (Abschnitt 11).
6. **Pacing ist Teil des Datensatzes.** Das System verhindert Dauerpräsenz (Abschnitt 11).

---

## 6. Entwurf: Das gelernte Modell des Spielers

Der Kern von Anforderung C ist, dass der Nemesis **aus früheren Begegnungen lernt**. Die Quellen sagen was er lernen kann (N-NE-003) und was er **nicht** darf (N-NE-008: nur Beobachtetes, kein Metawissen).

**Das Modell ist eine Menge von Überzeugungen** (wie ein Eintrag im Gedächtnis, Modul 05), jeweils mit Quelle, Sicherheit und Alter:

| Bereich | Mögliche Überzeugungen (Entwurf) | Quelle der Beobachtung |
|---|---|---|
| **Methoden** | bevorzugte Waffen, Munition, Fallen, Rituale, Salz, Eisen, Schutzkreise | Begegnung selbst, Spuren, Berichte |
| **Taktik** | schleicht, frontal, nutzt Licht, plant Fallen, nutzt Fahrzeug | beobachtetes Verhalten |
| **Schwächen und Gewohnheiten** | Nachladen, Verletzungsfolgen, Müdigkeit, Wege, Zeiten | Beobachtung |
| **Begleiter** | wer begleitet; wer ist verletzlich | Begegnung, Aufklärung |
| **Orte** | Fahrzeug, Safehouse, HQ, Wohnort, Stammorte | Aufklärung, Informanten |
| **Nahestehende** | Partner, Familie, Freunde, Hund | Aufklärung, Gerüchte |
| **Ressourcen** | Geld, Kontakte, Verbündete | Gerüchte |
| **Ruf und Ruhm** | was andere über ihn erzählen | Gespräche (Modul 07) |
| **Bereitschaft** | gnädig oder rücksichtslos; verschont er? | Begegnung mit Ausgang |

**Eigenschaften des Modells (Entwurf):**

1. **Nur Beobachtetes oder Erschlossenes.** Jede Überzeugung hat eine Quelle im Weltzustand (N-WA-005). Es gibt keine Datensammlung über das Spielerverhalten abseits der Welt.
2. **Unsicherheit.** Die Sicherheit steigt mit Wiederholung, sinkt mit Zeit; eine Überzeugung kann falsch sein (Fehldeutung, Täuschung durch den Spieler; Kreaturen 10 §7: Wiederholte Ablenkungen werden erkannt).
3. **Veralten.** Der Spieler wechselt Methoden. Ein Nemesis, der nur die Methoden von vor zehn Jahren kennt, ist überrascht.
4. **Informationsaustausch.** Das Modell kann an andere weitergegeben werden (N-NE-002), aber nur über plausible Wege (N-WA-006); es kann durch Weitergabe verzerrt werden (Modul 07).
5. **Täuschbarkeit.** Der Spieler kann das Modell absichtlich füttern (Köder, Fehlinformation, Gerüchte) — das ist eine Gegenmaßnahme, die die Quellen für Ablenkungen ausdrücklich zulassen (N-WA-008).
6. **Ableitung ohne Zahlen.** Das Modell ist eine Menge diskreter Aussagen, kein Zahlenprofil. Es erzeugt Entscheidungen im Rahmen der Taktikauswahl (`M-PD`: ‚Tactic selection should be contextual and adaptive rather than simple random rotation‘).

**Wirkung (Entwurf):**

| Modellinhalt | Mögliche Anpassung |
|---|---|
| bevorzugte Waffe bekannt | trägt Schutz, wählt Ort/Zeit, bereitet Gegenmaßnahme vor |
| Fallen bekannt | untersucht Räume genauer, sabotiert, umgeht (N-NE-004) |
| Begleiter bekannt | greift Begleiter an, isoliert den Spieler |
| Safehouse bekannt | überwacht, überfällt, zerstört (selten, N-NE-007) |
| Familie bekannt | droht, rächt, entführt (nicht als billiges Plotwerkzeug: N-LT-032 gilt für Partner) |
| Gewohnheiten bekannt | lauert auf Strecken, nutzt Zeiten |
| Schwäche vermutet | versucht sie zu schützen oder zu verbergen |
| Verletzungen bekannt | exploitet sie (Kreaturen 10: „exploit reload/injury“) |

---

## 7. Entwurf: Ziele und Motive

Ziele sind der Grund für Rückkehr und Verhalten. Anforderung C spricht von ‚persönlichen Feindschaften‘; die Quellen nennen Rache, Angst, Hass, Respekt (N-NE-002) und die Option, den Spieler zu meiden. Der Entwurf (💡):

| Ziel | Motiv | Typischer Plan |
|---|---|---|
| **Rache** | Groll, Verlust, Schande | den Spieler oder seine Nahestehenden schädigen, töten, bloßstellen |
| **Überleben** | Angst | untertauchen, fliehen, Schutz suchen, Gruppe wechseln |
| **Wiedergutmachung** | Schuld, Scham | Gegenseitigkeit, Hilfe (selten) |
| **Wettbewerb** | Stolz, Rang | übertreffen, bloßstellen, Fall wegschnappen (Rivale) |
| **Beute, Macht** | Gier | Mittel besorgen, Gebiet übernehmen |
| **Wissen** | Neugier | den Spieler beobachten, studieren |
| **Auftrag** | Pflicht, Geld | im Dienst einer Gruppe handeln (Modul 16) |
| **Wiedererlangung** | Besitz, Gebiet, Familie | zurückholen |
| **Bekehrung, Anwerbung** | Respekt | den Spieler für die eigene Sache gewinnen (Kreaturen 09: Menschen als Werkzeuge) |
| **Vermeidung** | Angst, Respekt | den Spieler meiden (`48#140A`) |

**Mehrere Ziele, Prioritäten.** Ein Nemesis hat gleichzeitig mehrere Ziele (Überleben vor Rache, solange er verletzt ist). Prioritäten ergeben sich aus Lage (Modul 04) und Persönlichkeit.

**Ziele ändern sich.** Aus Rache kann Vermeidung werden, nachdem der Nemesis eine zweite Niederlage erlitten hat; aus Vermeidung kann Bündnis werden, wenn ein größerer Feind erscheint (Modul 12 §8).

**Ziel ≠ Auftrag an das Spiel.** Das Spiel soll den Spieler nicht zwingen, den Nemesis zu verfolgen. Er kann ihn ignorieren, die Folgen tragen und sich anderem zuwenden (N-LT-024).

---

## 8. Entwurf: Grenzen der Anpassung

Die Quellen verlangen, dass die Anpassung begrenzt bleibt (N-NE-017, N-NE-020). Der Entwurf nennt **Grenzen**, die beim Entwurf jedes Nemesis zu prüfen sind (💡):

| Grenze | Beschreibung | Grundlage |
|---|---|---|
| **Artregeln** | keine Immunität, keine Fähigkeit, die die Art nicht hat | [Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md), 10 |
| **Schwächen bleiben** | Eine Schwäche der Art bleibt Schwäche; der Nemesis lernt, sie zu **schützen**, nicht abzulegen | Kreaturen 04 §7 |
| **Keine freie Erfindung** | Anpassungen kommen aus dem Taktikvorrat der Art | Kreaturen 10 §5 |
| **Nur Beobachtetes** | keine Metadaten | N-WA-005 |
| **Keine Stat-Skalierung** | keine unsichtbaren Zähler, die Werte erhöhen | N-NE-017 |
| **Plausible Mittel** | Waffen, Verbündete, Verstecke müssen im Weltzustand existieren | N-LT-008 |
| **Kommunikation** | Weitergabe nur über artspezifische oder plausible Wege | N-WA-006 |
| **Tote bleiben tot** | kein Respawn | N-NE-012 |
| **Mythologische Regeln** | gefangene Wesen bleiben gefangen, Schutzkreise wirken | N-NE-004 |
| **Verletzungen wirken** | eine Narbe ist mehr als Kosmetik | Kreaturen 04 §6 |
| **Seltenheit** | Auftritte begrenzt (Abschnitt 11) | Proposal 0010 §9 |

**Beispiele erlaubter Anpassung (abstrakt, 💡):** trägt Schutz, wo der Spieler vorher traf; kommt mit Verbündeten; wartet auf Gelegenheit; zieht das Gefecht in Dunkelheit oder Lärm; schickt Menschen vor; zerstört Licht oder Strom (wenn die Art es kann); versteckt sich in einem anderen Gebiet; wechselt den Wohnort; lernt Namen der Begleiter; greift ein ungeschütztes Ziel an.

**Beispiele unerlaubter Anpassung:** entwickelt plötzlich eine nicht vorhandene Resistenz; weiß ohne Quelle, wo der Spieler ist; erneuert Leben, weil er „besiegt“ wurde; hat nach jeder Begegnung mehr Lebenspunkte; erscheint nach dem Tod.

---

## 9. Entwurf: Lernen durch Regeln oder durch Daten

Das Proposal 0010 §11 hält als offene Frage fest: ‚how a nemesis learns (rules vs data)‘. Der Entwurf stellt zwei Wege und eine Mischung vor, ohne zu entscheiden (💡):

| Weg | Beschreibung | Vorteile | Nachteile |
|---|---|---|---|
| **Regelbasiert** | Fest codierte Regeln: wenn der Spieler X einsetzt und der Nemesis es überlebte, dann bereitet er Gegenmaßnahme Y vor | testbar, deterministisch, erklärbar | wirkt mit der Zeit mechanisch; viel Handarbeit je Art |
| **Datenbasiert** | Tabellen je Art und Taktik: ‚Gegenmaßnahmen‘, die ein Wesen kennt und wählen kann (Taktikvorrat), und Auswahlgewichte aus dem Modell | Erweiterbar durch Mods, wenig Code, Designerkontrolle | verlangt gute Daten; Gefahr von Inkohärenz |
| **Mischform (empfohlen als Prüfgegenstand)** | Regeln wählen aus einem **datengetriebenen Taktikvorrat** der Art; das gelernte Modell liefert Bedingungen; Zufall nur über deterministischen Seed | passt zu `M-PD` (‚StateTree …; bespoke C++ handles perception, memory, combat and special rules‘) und zu Kreaturen 10 §5 | Aufwand für Taktikvorrat je Art |

**Beobachtbare Eigenschaften, die jeder Weg erfüllen muss (Entwurf):**

1. **Erklärbar.** Für jede Anpassung ist nachvollziehbar, welche Begegnung sie veranlasste (Debug-Werkzeug, Proposal 0010 §4 Punkt 8).
2. **Deterministisch.** Dasselbe Modell und derselbe Weltzustand liefern dieselbe Anpassung (Seed; Modul 22).
3. **Begrenzt.** Pro Zeitraum nur begrenzte Anzahl Anpassungen.
4. **Mischbar mit Handarbeit.** Handgeschriebene Nemesis (Hauptbedrohungen) können Anpassungen vorgeschrieben bekommen (Abschnitt 12).
5. **Testbar.** Szenarien: Spieler setzt dreimal dieselbe Methode ein → Gegenmaßnahme; Spieler wechselt → Nemesis ist unvorbereitet (Modul 29).

**Frage an den Entwickler:** Soll das Nemesis-Verhalten eher **handgeschrieben pro Wesen** (hohe Qualität, wenig Fälle) oder **systemisch aus Daten** (breit, aber gleichförmiger) sein? Die Quellen legen sich nicht fest (❓ Q-N08).

---

## 10. Entwurf: Nemesis und Gruppen

Anforderung D (Fraktionen) und C (Nemesis) überlappen: Eine Gruppe kann Gegner sein, ein Anführer kann Nemesis sein.

| Konstellation | Beschreibung | Folge |
|---|---|---|
| **Einzelner Nemesis** | eine Figur, eigene Mittel | persönliche Jagd |
| **Nemesis mit Gruppe** | Anführer mit Rudel/Bande | Gruppenressourcen, Hierarchie (Modul 16) |
| **Gruppe als Nemesis** | Gruppe als Akteur mit eigener Beziehung zum Spieler | Gruppenhaltung (Modul 08 §8) |
| **Nachfolger** | Anführer fällt, Nachfolger übernimmt (Beispiel `48#142A`) | Groll vererbt sich oder nicht |
| **Lieutenants** | Untergebene mit eigenen Zielen | Machtkämpfe, Verrat |
| **Racheketten** | Verwandte und Freunde eines Getöteten werden Feinde | Modul 10 §9 |
| **Allianz gegen den Spieler** | mehrere Feinde verbünden sich | Modul 17 |

**Regeln (Entwurf):**

1. **Der Tod eines Anführers löst die Gruppe nicht auf** (`48#142A`, 📋 Beispiel des Assistenten; Kreaturen 09 §9).
2. **Groll ist an Personen gebunden.** Wer ihn erbt, tut es über eine Kante (Familie, Loyalität, Eid), nicht automatisch.
3. **Informationen innerhalb der Gruppe** werden über plausible Wege geteilt (N-WA-006). Ein Mitglied, das den Spieler sah, kann es erzählen; ein Fremder weiß nichts.
4. **Gruppe ≠ Hive Mind** (N-WA-006).

---

## 11. Entwurf: Lesbarkeit und Fairness

Das Proposal 0010 §9 nennt als Risiko: ‚Feindschaft, die der Spieler nicht lesen oder beeinflussen kann‘. Die Gegenmaßnahme ‚verdeckte Werte mit sichtbaren Folgen‘ passt zu N-LT-012. Der Entwurf formuliert Leitplanken (💡):

| Leitplanke | Inhalt |
|---|---|
| **Vorzeichen** | Ein Nemesis kündigt sich an: Spuren, Gerüchte, Nachrichten, Tiere, Zeichen, Begegnung mit Boten (Modul 07, 15) |
| **Handschrift** | Seine Methoden sind erkennbar (Hinweise im Archiv), sodass der Spieler ihn beim Namen nennen kann |
| **Gegenmaßnahmen** | Der Spieler hat Möglichkeiten: Köder, Beseitigung der Spur, Schutz der Nahestehenden, Hilfe suchen, Flucht |
| **Verhältnismäßigkeit** | Die Antwort des Nemesis entspricht der Provokation (N-LT-023) |
| **Frequenz** | Mindestabstände, Auftrittsbudget je Region und Zeit; keine Dauerpräsenz; ‚selten, sonst nervt es‘ (N-NE-007) |
| **Anzahl** | Begrenzung gleichzeitig aktiver Nemesis-Figuren; Zahl offen (Q-N09) |
| **Wahlfreiheit** | Der Spieler kann ignorieren, verhandeln, töten, verschonen, gefangen nehmen |
| **Schwierigkeit** | Höhere Schwierigkeit (Grounded) macht Nemesis aufmerksamer und weniger nachsichtig, aber nie allwissend (`M-PD`) |
| **Keine Straf-Spirale** | Misslingen darf keine unausweichliche Abwärtsspirale auslösen; Spielende nicht durch einen Nemesis blockieren |
| **Spielerzeit** | Auftritte respektieren Spielphasen: kein Auftritt während Story-Sequenzen, wenn dies die Erzählung stört |

**Kein Marker.** Es gibt keine Kartenmarkierung ‚Nemesis hier‘ (vgl. `M-PD` zu Karten- und Questmarkern in Ermittlungsmodi, Ermittlung 03). Sichtbar sind nur Folgen, Hinweise und im Archiv Eintragungen im Wissensstand des Spielers.

---

## 12. Abgrenzung zu Hauptbedrohungen und geschützten Figuren

`M-PD` ‚Long-term story threats‘: Große Akt-Bedrohungen sind **handgeschriebene, langlaufende Entitäten**, keine Ein-Mission-Bosse; sie wirken über Agenten, Hinweise, Frühbegegnungen, gescheiterte Versuche und wechselnde Strategien; der Spieler kann mehrere Begegnungen überleben, bevor er die Lösung versteht.

| Aspekt | Nemesis (systemisch) | Hauptbedrohung (authored) |
|---|---|---|
| Ursprung | Begegnung im Spielverlauf | Story, vorab geschrieben |
| Datensatz | aus dem Weltzustand erzeugt | handgeschrieben, einzigartig |
| Lernen | Regeln/Daten (Abschnitt 9) | vorgegebene Strategien plus Reaktivität |
| Schutz | `none` bis `story_safe` | `authored`, Tod nur laut Story |
| Wiederkehr | Zufall/Plan im System | Erzählstruktur |
| Zahl | mehrere | wenige, pro Akt |
| Beziehung zu Story | optional | zentral |

**Überschneidungen:** Eine Hauptbedrohung kann **dieselben Datenfelder** nutzen (Begegnungen, gelerntes Modell), damit sie reaktiv wirkt; ihr Ablauf wird aber von der Erzählung geschützt. Umgekehrt kann ein systemischer Nemesis zu einer wichtigen Figur werden, wenn die Handlung es nutzt (Modul 03 §9: Aufstieg).

**Hauptstory-Spine bleibt kontrolliert.** Nemesis-Ereignisse dürfen die Hauptstory nicht blockieren oder verbiegen (N-LT-024; `M-PD` ‚Acts, chapters and controlled story reactivity‘: Spielerentscheidungen ändern Wege, Informationen, lokale Ausgänge und Beziehungen stärker als das Überleben vieler kritischer Hauptfiguren).

---

## 13. Sonderfälle

1. **Der Nemesis wird von jemand anderem getötet** (Hunter, Rivalengruppe, Polizei, Unfall): Der Weltzustand ändert sich ohne den Spieler (F, E). Der Spieler erfährt davon über Wege (Modul 07). Ob er ‚Genugtuung‘ erlebt, hängt nicht vom System ab.
2. **Der Nemesis wird vom Spieler gerettet.** Eine Rettung ändert die Kante (Schuld, Respekt) und kann Feindschaft in Bündnis wandeln (Modul 12).
3. **Der Nemesis wird gefangen.** Er bleibt Individuum im Gewahrsam (Verhör, Kreaturen 13); er kann entkommen, bestochen werden oder sterben.
4. **Der Nemesis ist verwandelt oder besessen.** Der Kern ist die Person; die Ursache (Besessenheit) kann behandelt werden (Kreaturen 05). Heilung kann Feindschaft beenden oder Schuld erzeugen.
5. **Ein Wesen mit menschlicher Identität** wird zum Feind: Seine Tarnidentität wird Teil der Jagd; Familien und Kollegen sind betroffen (Ermittlung 13).
6. **Zwei Spieler im Koop.** Die Feindschaft gilt dem Spieler, dem das Ereignis zugeschrieben wird; im Host-Speicherstand kann sie sich auf mehrere Spieler beziehen (Q-N15).
7. **Spielerwechsel der Methoden** (Berufswechsel, Wohnortwechsel): Das Modell wird veraltet; der Nemesis muss neu beobachten.
8. **Der Spieler verliert eine Konfrontation.** Der Nemesis kann verschonen, gefangen nehmen, töten (Spielende ohne Game Over, siehe `M-PD` ‚Death / failure — OPEN‘) — das ist eine offene Designfrage (Q-N10).
9. **Der Nemesis spielt keine Rolle mehr.** Nach langer Zeit ohne Kontakt kann er herabgestuft werden; Historie bleibt (N-ID-015).
10. **Nemesis nach DLC oder Mod.** Fremde Datensätze müssen mit dem Kern kompatibel bleiben (Modul 23).
11. **Der Nemesis ist ein Hund oder anderes Tier.** Kein Nemesis im Sinn dieses Moduls (keine Planung); wohl aber ein Problemtier als Ökologiefall.
12. **Nemesis und Fahndung.** Ein menschlicher Nemesis kann die Polizei nutzen (Anzeige, falsche Beweise, Zeugenbeeinflussung); die Fahndungslogik bleibt in Ermittlung 14.

---

## 14. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-181 | Ein Nemesis lernt nur Dinge, für die es im Weltzustand eine Beobachtung oder plausible Weitergabe gibt | N-NE-008, N-WA-005 |
| N-EN-182 | Kein Wert des Nemesis steigt allein durch die Zahl der Begegnungen | N-NE-017 |
| N-EN-183 | Eine Anpassung hat eine Ursache in einem Begegnungs-Datensatz | Abschnitt 5 |
| N-EN-184 | Eine Anpassung bleibt innerhalb der Artregeln und Schwächen | N-NE-020 |
| N-EN-185 | Ein toter Nemesis erscheint nicht wieder (außer artspezifische Ausnahme laut Bestiarium) | N-NE-012 |
| N-EN-186 | Eine zurückkehrende Figur zeigt ihre früheren Verletzungen | N-NE-013 |
| N-EN-187 | Dasselbe Modell und derselbe Weltzustand liefern dieselbe Anpassung | Abschnitt 9 |
| N-EN-188 | Ein Nemesis verwendet keine Information über den Spieler, die er nie erhielt | N-WA-005 |
| N-EN-189 | Die Zahl gleichzeitig aktiver Nemesis-Figuren und ihre Auftrittsabstände sind begrenzt | Abschnitt 11 |
| N-EN-190 | Ein Nemesis kann keine Hauptmission blockieren | Abschnitt 12 |
| N-EN-191 | Der Tod eines Anführers löst die Gruppe nicht auf | Abschnitt 10 |
| N-EN-192 | Ein Nemesis hat keinen Marker in der Spieler-Oberfläche | Abschnitt 11 |
| N-EN-193 | Die Wirkung einer Rettung oder Verschonung ändert die Beziehungskante nachvollziehbar | Modul 08 |
| N-EN-194 | Jeder Nemesis hat mindestens ein Vorzeichen (Spur, Gerücht, Handschrift), bevor er auftritt | Abschnitt 11 |
| N-EN-195 | Ein Nemesis in Gefangenschaft nimmt nicht an Außenaktionen teil | Modul 12 |
| N-EN-196 | Ein Nemesis mit `story_safe` oder `authored` kann nur durch Spieler oder Geschichte sterben | Modul 03 §8 |

---

## 15. Was fehlt

- Kein konkretes Schema, keine Zahlen für Häufigkeit, Anzahl, Dauer, Cooldown.
- Die Fragen aus Proposal 0010 §11: Wie lernt ein Nemesis (Regeln oder Daten, Q-N08), welche Historie bleibt, was sieht der Spieler.
- Keine Entscheidung, welche Wesen und welche Menschen Nemesis werden können (Q-N09).
- Keine Aussage, wie der Entwickler eine Niederlage des Spielers durch einen Nemesis behandelt wissen will (Q-N10).
- Keine benannten Antagonisten. Das Proposal 0010 §12 schließt sie als Nicht-Ziel aus.
