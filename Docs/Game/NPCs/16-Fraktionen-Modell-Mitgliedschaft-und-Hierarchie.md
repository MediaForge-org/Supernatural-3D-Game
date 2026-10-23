# NPCs 16: Fraktionen – Modell, Mitgliedschaft und Hierarchie

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt Gruppen, Organisationen und Szenen als Akteure der Welt: ihre Struktur, ihre Mitglieder, ihre Hierarchie, die Wege des Spielers hinein und hinaus. Beschlossen sind die Richtung (ein tiefes, optionales Fraktionssystem, unabhängig von der Hauptstory, mit Aufstieg bis zur Führung, Infiltration, Rekrutierung und unbegrenztem Zusatzinhalt) und die Anforderung D (`PE-N1022`: Gruppen verfolgen eigene Ziele, bilden Allianzen, beanspruchen Gebiete, kämpfen, ändern Strategien); **Datenmodell, Typen, Ränge und Ereignisgeneratoren sind Entwurf (💡)**. **Es gibt keine Liste benannter Fraktionen.** Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Territorien, Konflikte und Machtverschiebung: [Modul 17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md). Behörden, Hunter-Organisationen und Wesen: [Modul 18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md). Hunterwelt, Kulte und Organisationen aus Sicht der Kreaturen: [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Der Fraktionsdatensatz](#2-entwurf-der-fraktionsdatensatz)
3. [Entwurf: Typen von Fraktionen](#3-entwurf-typen-von-fraktionen)
4. [Entwurf: Hierarchie und Rang](#4-entwurf-hierarchie-und-rang)
5. [Entwurf: Mitglieder als Individuen](#5-entwurf-mitglieder-als-individuen)
6. [Entwurf: Der Spieler und die Fraktionen](#6-entwurf-der-spieler-und-die-fraktionen)
7. [Entwurf: Innenpolitik](#7-entwurf-innenpolitik)
8. [Entwurf: Beziehungen zwischen Fraktionen](#8-entwurf-beziehungen-zwischen-fraktionen)
9. [Entwurf: Symbole, Orte, Fahrzeuge](#9-entwurf-symbole-orte-fahrzeuge)
10. [Entwurf: Inhalte und Ereignisse](#10-entwurf-inhalte-und-ereignisse)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-FR-001 | **Autonome Fraktionen und Organisationen (D):** Monstergruppen, Hunter-Organisationen, kriminelle Netzwerke, Behörden und andere Gruppen verfolgen eigene Ziele, bilden Allianzen, beanspruchen Gebiete, bekämpfen einander und ändern ihre Strategie | 📋 | `PE-N1022` D |
| N-FR-002 | Optionale Mitgliedschaften neben der Hunter-Crew: Motorradclub, Racing Club, Gun Club, Jagdverein, Kampfszene, kriminelle Organisation, religiöse oder okkulte Gruppe, politische oder ideologische Gruppe, extremistische Organisation, weitere lokale Gruppierungen. Die Hunter-Crew bleibt das eigentliche Zuhause | 🧭 | `23#676A` (Vorschlag des Assistenten); Entwickler: „ich würde die ganze mc oder extremisten sachen alle optional machen“ in `23#677E` |
| N-FR-003 | Die optionalen Fraktionen sind für die Hauptstory **nicht nötig**; „es können sich nur die dialoge etwas ändern“; man kann auch ein Spitzel sein; nach Abschluss der Hauptstory kann man dort weitermachen | 📋 | `23#677E`; `23#678A` (‚Faction-System klar getrennt von der Hauptstory‘) |
| N-FR-004 | Clubs, Sekten und Extremisten können rekrutiert werden und dann im Hunter-HQ sein: „die clubs, sekten extremisten können möglicherweise auch rekrutiert werden und dann in der richtigen hq sein“; das kann etwas zur Story beitragen, wenn man den Charakter bei einer Mission zusätzlich verfügbar hat | 📋 | `23#677E` |
| N-FR-005 | Die Fraktionen werden **sehr realistisch** nachgebildet; es soll unbegrenzten Zusatzinhalt geben (Wortlaut mit Tippfehler: ‚uimimitiert extra content‘), auch nach der Story, mit Aufgaben und der Möglichkeit, zum Leader aufzusteigen | 📋 | „ich würde das sehr realistisch nachmachen“ und „man kann vielleicht zum leader aufsteigen“ in `23#679E` |
| N-FR-006 | Das Fraktionssystem ist **extrem tief**, die Hauptstory davon praktisch unabhängig; Fraktionen schalten optional frei: Nebencontent, World Events, spezielle Jobs, Beziehungen, Shops, Fahrzeuge, Kleidung, Safehouses/Clubhäuser, Informationen, Kontakte | 🧭 | `23#676A` Abschnitt ‚Sehr wichtig für unsere Hauptstory‘; `23#678A` (Vorschlag des Assistenten) |
| N-FR-007 | Es gibt **mehrere eigene kriminelle Organisationen** statt generischer Gang-NPCs, einschließlich extremistischer Gruppen (nach dem Planungsstand von Block 8) | 📋 | `23#1056A` Frage 52; Antwort in `23#1057E`; `23#1058A` ‚Kriminelle Organisationen‘ |
| N-FR-008 | Kriminelle Gruppen haben Einflussbereiche, aber nicht zwingend bunte Gangflächen; auf der Karte bunt nur dann, wenn man selbst in einer solchen Gruppe ist („nur wenn man selber in einer dieser gangs ist, dann kann man mehr gebiet erobern“) | 📋 | `23#1056A` Frage 53; Antwort in `23#1057E`; `23#1058A` ‚Territorien‘ |
| N-FR-009 | Illegale Geschäfte (Chop Shops, Underground-Casinos, illegale Rennen, Fight Clubs, Schmuggel) gehören zur Welt | 📋 | `23#1056A` Frage 54; „ja“ in `23#1057E` |
| N-FR-010 | Nicht jeder Motorradclub ist eine kriminelle Gang: normaler MC, Outlaw MC, extremistischer MC, Veteranen- oder lokaler MC unterscheiden sich | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-FR-011 | Hierarchie ist mehr als ein Ruf-Balken: je Gruppe eigene Ränge (Beispiel MC: Prospect, Member, Patch Member, Sergeant-at-Arms, Road Captain, Vice President, President); **Rang ≠ nur Level**; Aufstieg hängt ab von Vertrauen, geleisteten Jobs, Loyalität, Beziehungen zu Mitgliedern, internen Abstimmungen, Fähigkeiten, Verhalten, Entscheidungen | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-FR-012 | Führung hängt ab von langer Mitgliedschaft, Vertrauen, Beziehungen, Aufgaben, wichtigen Entscheidungen, Einfluss und Unterstützung; nicht „1000 XP gesammelt → jetzt Präsident“; manche Gruppen stimmen demokratisch ab, andere sind autoritär oder folgen Nachfolge, Machtkampf oder Ernennung | 🧭 | `23#680A` (Vorschlag des Assistenten) |
| N-FR-013 | Der Spieler bekommt in der Gruppe **tatsächliche Jobs**, keine kosmetische Mitgliedschaft | 🧭 | `23#676A` |
| N-FR-014 | Mitglieder haben untereinander Beziehungen: Leader mag den Spieler, der VP misstraut ihm, Mitglied A ist enger Freund, Mitglied B hasst ihn, Mitglied C schuldet einen Gefallen; daraus entsteht interne Politik | 🧭 | `23#676A` |
| N-FR-015 | Hierarchiekämpfe sind vorgesehen: Führungswechsel, interne Streitigkeiten, Ausschlüsse, Verrat, Abspaltungen, Austritte, neue Chapter, Fusionen, rivalisierende Führungsfraktionen; ein Teil davon passiert über den World State, nicht über Hauptmissionen | 🧭 | `23#676A` |
| N-FR-016 | Der Spieler kann mehreren Gruppen angehören, aber nicht folgenlos (zwei rivalisierende MCs, Polizei-nahe Organisation und Gang) | 🧭 | `23#676A` |
| N-FR-017 | Beziehungen zwischen Fraktionen: Allied, Friendly, Neutral, Tense, Hostile, War; sie verändern sich | 🧭 | `23#676A` |
| N-FR-018 | Mitgliedschaft kann enden: freiwillig, Rauswurf, Suspendierung, Verrat, Zerfall der Organisation, später eventuell eigene Gruppe; nicht jede falsche Dialogantwort zerstört 50 Stunden Inhalt | 🧭 | `23#676A` |
| N-FR-019 | Drei Wege: ehrliche Mitgliedschaft, Infiltration/Spitzel (mit Konflikt zwischen Loyalitäten), lose Verbindung ohne formelle Mitgliedschaft | 🧭 | `23#678A` (Vorschlag des Assistenten); „man kann theoretisch auch ein spitzel sein“ in `23#677E` |
| N-FR-020 | Eine Faction-Questline kann Dutzende Stunden zusätzlichen Inhalt haben und vollständig optional sein | 🧭 | `23#678A` |
| N-FR-021 | Rekrutierte Faction-NPCs brauchen echte Fähigkeiten, nicht nur Aussehen (Ex-MC-Mechaniker, ehemaliges Sektenmitglied, Racer, Arzt …); nicht jeder Rekrut passt automatisch ins HQ | 🧭 | `23#678A` |
| N-FR-022 | Der Spieler kann bei geeigneten Gruppen die Führung übernehmen und bekommt dann Mitgliederverwaltung, Clubhaus, Jobs, Ressourcen, Events, Rivalitäten – **ohne Strategiespiel-Micromanagement** | 🧭 | `23#678A`; `23#680A` |
| N-FR-023 | Fraktion-Reputation ist nicht dasselbe wie individuelle Beziehung (siehe [Modul 08](08-Beziehungen-Modell-und-Wirkung.md) N-BE-020) | 🧭 | `23#678A` |
| N-FR-024 | Extremistische Gruppen: Menschen mit unterschiedlichen Motiven (Fanatiker, Opportunist, Familie/Freunde, Krimineller, junger Rekrut, älteres Mitglied, Aussteiger, Zweifler, Führungsperson, die Ideologie für Macht benutzt); das Spiel zeigt Glauben, Tun, Behandlung anderer und Folgen und überlässt die Bewertung dem Spieler; die Strukturen sind eigene fiktive | 🧭 | `23#676A`, `23#678A`, `23#680A` (Vorschläge des Assistenten); siehe N-LT-029, N-LT-030 |
| N-FR-025 | Monster bilden Rudel, Clans, Hierarchien mit Anführern, Untergebenen, Rivalen und Nachfolgern, Territorien, Machtkämpfe, Allianzen; sie tauschen Wissen über Hunter aus; Veränderungen bleiben im Spielstand | 📋 | „Supernatural Monster-Gesellschaft: Organisationen, Hierarchien, Territorien und Machtkämpfe“ in `48#141E`; Tabelle `48#142A` (💡) |
| N-FR-026 | Es gibt mehrere Hunter-Organisationen, ein loses Netz und eine wissenszentrierte Hauptorganisation mit eigenem Namen; kleine Behördeneinheiten kennen Teile der Wahrheit; die konkrete Fraktionsliste ist offen | 📋 / ❓ | `M-B09`; [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) §4–§5 |
| N-FR-027 | Die Hunter-Crew lebt im HQ; optionale Rekruten kommen von außen; die Hauptstory bleibt unabhängig | 📋 | `M-PD` ‚Player-created protagonist, Hunter crew and HQ‘; `23#677E` |
| N-FR-028 | Online/Koop: Fraktionsmitgliedschaft und Fortschritt des Spielers gehören zum Host-Speicherstand | 📋 | `CLAUDE.md` §6 |
| N-FR-029 | Die konkrete Liste der Fraktionen, ihre Namen, Ziele und Besetzung sind nicht festgelegt | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11; `M-B09`; `M-OD` |

---

## 2. Entwurf: Der Fraktionsdatensatz

Die Quelle (`23#676A`, `23#678A`) nennt als Bestandteile eines Fraktionssystems: Identität, Typ, Glaubensinhalte, Hierarchie, Ränge, Mitgliedschaftsregeln, Reputation, individuelle Beziehungen, interne Politik, Fraktionsbeziehungen, Jobs, Vorteile, Pflichten, Symbole, Immobilien, Fahrzeuge, Zeitpläne, dynamische Ereignisse, Führungswechsel, Austritt und Rauswurf, Geschichte, Spielermitgliedschaft, Rekrutierung, Infiltration, Informantenstatus, Tarnidentität, Postgame-Fortsetzung. Der Entwurf (💡) legt daraus einen Datensatz an:

```text
Faction
├── id, kind, name_ref (Mod/Lore; keine Namen in dieser Dokumentation)
├── identity         Selbstbild, Werte, Glaube/Ideologie (falls vorhanden), Kultur, Rituale
├── legality         legal | grau | illegal | geheim
├── structure        Hierarchievorlage, Rangliste, Rollen (Ämter), Entscheidungsform, Nachfolgeregel
├── chapters[]       Ortsgruppen (Chapter), je mit eigener Führung und Gebiet
├── members[]        NPC-IDs mit Rang, Rolle, Status, Loyalität (Modul 05)
├── norms            interne Regeln, Tabus, Strafen, Aufnahmeverfahren
├── resources        Geld, Immobilien, Fahrzeuge, Waffen, Betriebe, Informanten, Verbindungen
├── territory        Einflussbereiche, Orte, Routen (Modul 17)
├── relations[]      Haltung zu anderen Fraktionen / Behörden / Menschen / Hunter (Allied ... War)
├── goals, strategy  aktuelle Ziele, Strategiezustand, Pläne (Modul 17)
├── knowledge        was die Gruppe weiß (geteilter Wissenspool mit Rollenrechten, Modul 05 §10)
├── events           laufende interne Ereignisse (Konflikte, Wahlen, Krisen)
├── history          Chronik: Gründung, Führungswechsel, Spaltungen, Kriege, Verluste
├── player           Beziehung zum Spieler (Mitglied? Rang? Jobs? Ruf der Gruppe?)
├── offer            Jobs, Dienste, Ressourcen, die die Gruppe dem Spieler bieten kann
└── flags            optional_for_story | authored | protected | mod_defined
```

**Entwurfsregeln:**

1. **Die Gruppe ist ein Akteur mit eigenem Datensatz**, nicht nur eine Menge von Mitgliedern. Sie hat Ziele, Mittel, Gedächtnis und Beziehungen (N-FR-001).
2. **Mitglieder sind NPC-Datensätze mit Zugehörigkeit**; die Gruppe verweist auf sie, sie auf die Gruppe (kein doppelter Besitz: Modul 24).
3. **Gruppenwissen ist ein Pool mit Zugriffsrechten.** Nicht jedes Mitglied weiß, was die Führung weiß (Modul 05 §10).
4. **Gruppenhaltung zum Spieler** ist aus Mitgliedern und Führung abgeleitet, nicht einfach eine Zahl (N-BE-020).
5. **Alle Quellen für Inhalte sind datengetrieben** (Mods dürfen Fraktionen registrieren, Modul 23).
6. **Optionale Fraktionen haben `optional_for_story`.** Kein Story-Pfad darf von ihnen abhängen (N-FR-003).
7. **Protected/authored.** Fraktionen der Hauptstory (und ihre Anführer) unterliegen Autorenregeln (Modul 11 §12).

---

## 3. Entwurf: Typen von Fraktionen

Die Quellen nennen viele Gruppenarten; der Entwurf ordnet sie nach Struktur und typischen Dynamiken. Er legt **keine Arten** fest (keine Liste benannter Gruppen).

| Typ | Beispiele aus den Quellen | Struktur (Entwurf) | Typische Dynamik |
|---|---|---|---|
| **Club (legal)** | Motorrad-, Racing-, Gun-, Jagdverein, Veteranenclub | Vorsitz, Ämter, Mitglieder, Clubhaus | Ausfahrten, Treffen, Wettbewerb, Streit |
| **Club (Grauzone)** | Outlaw MC, Fight-Szene, Underground-Rennszene | Ränge, Chapter, ungeschriebene Regeln | Geschäft, Gewalt, Rivalität, Polizei |
| **Straßengang** | Bandenkonflikte | lose Hierarchie, Territorium | Revierkämpfe, Rekrutierung, Verrat |
| **Organisierte Kriminalität** | Mafia-ähnliche Organisationen, Drogenorganisationen, Schmuggler | feste Rangordnung, Familien/Zellen | Geschäft, Bestechung, Gewalt, Geldwäsche |
| **Kult, Sekte** | okkulte und religiöse Gruppen | Führer, innerer Kreis, Anhänger | Rituale, Rekrutierung, Abschottung, Spaltung |
| **Religiöse Gemeinde** | Kirchen, Gemeinden | Pfarrer, Gemeinde | Feiertage, Hilfe, Konflikte |
| **Politische Gruppe** | Parteien, Bürgerinitiativen | Vorstand, Mitglieder | Wahlen, Proteste, Kampagnen |
| **Extremistische Gruppe** | gewalttätige Extremisten (eigene fiktive Strukturen) | Führungsfiguren, Zellen | Gewalt, Anwerbung (nicht als Handbuch), Spaltung |
| **Berufsgruppe, Behörde** | Polizeidienststellen, Sheriff, Feuerwehr, Krankenhaus | Dienstgrade, Zuständigkeiten | Verfahren, Korruption, Konflikte (Modul 18) |
| **Unternehmen** | Firmen, Medien, Sicherheitsdienste | Leitung, Abteilungen | Geschäft, Einfluss, Vertuschung |
| **Hunter-Organisation** | wissenszentrierte Hauptorganisation, weitere Organisationen, loses Netz | siehe Kreaturen 11 | Wissen, Missionen, Streit |
| **Monstergruppe** | Rudel, Clans, Zirkel (`48#141E`) | Anführer, Untergebene, Nachfolger | Territorien, Machtkämpfe (Modul 17) |
| **Familienclan** | Hunter-Familien, Verwandtschaft | Haupt, Linien | Erbe, Fehden |
| **Nachbarschaft, Verein** | informell | Sprecher | Hilfe, Klatsch |

**Regel:** Ein Typ ist eine **Vorlage von Standardwerten** (Strukturen, Aufgaben, Konflikte), keine feste Klasse. Jede Fraktion kann davon abweichen; Mischformen sind erlaubt (ein Club mit kriminellen Zweigen).

**Typ ≠ Wertung.** Das Spiel führt keine Typen als ‚gut‘ oder ‚böse‘ (N-LT-030). Ein normaler MC und ein Outlaw MC unterscheiden sich in Tun und Regeln, nicht in einer Plakette.

**Eigenständige fiktive Strukturen.** Gerade bei extremistischen und kriminellen Gruppen entwirft das Spiel eigene Organisationsformen und kopiert keine realen Handbücher (N-LT-029).

---

## 4. Entwurf: Hierarchie und Rang

**Beschlossene Richtung:** Eine Gruppe hat eine echte Hierarchielogik; der Rang ist nicht nur ein Level; manche Gruppen sind demokratisch, andere autoritär, hereditär oder von Machtkämpfen geprägt (N-FR-011, N-FR-012).

### 4.1 Rangvorlagen (Beispiele der Quelle)

Die Quelle nennt folgende Ranglisten als Beispiele (`23#676A`, `23#678A`); sie sind **nicht** festgelegt:

| Gruppenart | Beispielreihenfolge (Quelle) |
|---|---|
| Motorradclub | Prospect → Member → (Patch Member) → Enforcer / Road Captain / Sergeant-at-Arms → Vice President → President |
| Allgemeine Organisation | Recruit → Member → Trusted Member → Organizer → Lieutenant → Senior Leadership → Leader |
| Sekte | Outsider → Initiate → Follower → Trusted Follower → Inner Circle → Senior Figure → Leader |
| Kriminelle Organisation | Associate → Member → Enforcer → Lieutenant → Underboss → Boss |
| Extremistische Organisation | eigene fiktionale Struktur statt realer Handbücher |

### 4.2 Hierarchie-Modell (Entwurf)

```text
Hierarchie
├── Rang (Status):     formale Stufe  (Prospect, Member, Officer ...)
├── Amt (Funktion):    Kassenwart, Sergeant-at-Arms, Road Captain, Beauftragter ...
├── Einfluss:          informelle Macht (Alter, Verdienst, Beziehungen, Geld)
├── Befehlskette:      wem untersteht wer
├── Entscheidungsform: autoritär | Rat | Abstimmung | Erbfolge | Ernennung | Machtkampf
└── Nachfolgeregel:    wer rückt nach, wenn die Führung ausfällt
```

**Regeln (Entwurf):**

1. **Rang, Amt und Einfluss sind getrennt.** Ein Mitglied kann hohen Einfluss ohne Amt haben, ein Amt ohne Einfluss.
2. **Ränge haben Rechte und Pflichten** (Zugang zu Clubhaus, Jobs, Waffen, Wissen).
3. **Entscheidungen werden nach der Form der Gruppe gefällt**, nicht nach Spielerwunsch.
4. **Auf- und Abstieg haben Ursachen** (Abschnitt 4.3), nie nur Zeit.
5. **Chapter.** Große Gruppen haben Ortsgruppen mit eigener Führung; Konflikte zwischen Chaptern sind möglich (`23#680A`: unterschiedliche Chapters/Ortsgruppen).

### 4.3 Aufstieg und Abstieg (Entwurf)

| Einflussfaktor | Quelle der Idee |
|---|---|
| Vertrauen der Mitglieder und Führung | N-FR-011 |
| Geleistete Jobs und deren Ergebnis | N-FR-011 |
| Loyalität (auch in Krisen) | N-FR-011 |
| Beziehungen zu Mitgliedern (Fürsprecher, Gegner) | N-FR-014 |
| Interne Abstimmungen | N-FR-011 |
| Fähigkeiten, die die Gruppe braucht | N-FR-011 |
| Verhalten gegenüber der Gruppe und Außenstehenden | N-FR-011 |
| Entscheidungen in Konflikten | N-FR-011 |
| Zeit in der Gruppe | N-FR-012 |
| Stand der Führung (stabil, schwach, in Krise) | N-FR-012 |
| Politische Lage der Gruppe (Krieg, Frieden) | 💡 |

**Aufstiegsereignis (Entwurf).** Aufstieg ist ein Ereignis mit Zeugen und Abstimmungskontext, kein automatischer Schwellensprung. Beförderung, Prüfung, Mutprobe, Wahl, Ernennung, Erbe: je Gruppe verschieden.

**Abstieg und Strafe.** Degradierung, Suspendierung, Ausschluss, Strafen sind nach Regeln der Gruppe möglich (N-FR-018). Gewalt als Strafe ist Teil der Gruppenwirklichkeit, nicht Standardmechanik des Spiels.

**Die Schwelle ist offen.** Wie der Aufstieg technisch bewertet wird, entscheidet die Umsetzung (❓ Q-N14).

---

## 5. Entwurf: Mitglieder als Individuen

Die Quelle fordert, dass Mitglieder Menschen sind: Eine Faction ist nicht ein ‚Mitglied-Typ‘ (`23#676A`: ‚Faction Reputation ≠ Individual Relationship‘; ‚Charakterkomplexität statt Autorenkommentar‘). Für jeden NPC, der Mitglied ist, gilt (💡):

```text
Membership (im NPC-Datensatz, Modul 03 §3 'affiliations')
├── faction_id, chapter
├── rank, office, status       aktiv | Probezeit | suspendiert | Ruhestand | Spitzel | übergelaufen | ausgeschlossen
├── since, history             Eintritt, Beförderungen, Strafen
├── motive                     Familie | Geld | Überzeugung | Schutz | Zugehörigkeit | Nervenkitzel | Zwang | Karriere | Rache
├── loyalty                    zur Gruppe / zur Führung / zu Einzelnen (getrennt)
├── beliefs                    persönliche Überzeugung (kann von der Gruppe abweichen)
├── visibility                 offen | verdeckt (trägt Symbole nicht immer; höhere Mitglieder wirken durchschnittlich)
├── doubts                     Zweifel, Reue, Ausstiegswunsch
└── recruitable                kommt für Hunter-Crew in Frage (Modul 19) — nur ausgewählte Kandidaten
```

**Vielfalt innerhalb einer Gruppe (Quellenliste für Extremisten, übertragbar):** Fanatiker, Opportunist, Mitglied wegen Familie und Freunden, Krimineller, dem die Ideologie egal ist, junger Rekrut, älteres Mitglied, Aussteiger, Zweifler, Führungsperson, die Ideologie instrumentalisiert (N-FR-024). Der Entwurf verallgemeinert: **Jede Gruppe hat ein Spektrum von Motiven und Haltungen.**

**Person ≠ Gruppe ≠ Ideologie** (`23#676A` Einleitung, 🧭): Ein Mitglied kann loyal, hilfsbereit, ein guter Vater sein und dennoch Überzeugungen haben, die andere bedrohen; ein gesellschaftlich akzeptierter Mensch kann ein schlechter Mensch sein. Das Spiel soll keine GOOD/EVIL-Schablonen vergeben (N-LT-030).

**Individuelle Beziehungen zum Spieler** laufen über Beziehungskanten (Modul 08), nicht über die Gruppe. Das Spiel nutzt dafür keine Gruppenzahl.

**Verdeckte Zugehörigkeit.** Nicht jedes Mitglied trägt Zeichen offen (`23#676A`: ‚Aber nicht jeder trägt alles offen‘). Der Spieler muss Zugehörigkeit erkennen, ableiten oder erfragen (Ermittlung 13 §9).

**Mitglieder erinnern.** Sie haben Gedächtnis; sie erzählen innerhalb der Gruppe (Modul 07 §6, plausibler Kanal, N-WA-006).

---

## 6. Entwurf: Der Spieler und die Fraktionen

### 6.1 Die drei Wege

Die Quelle benennt drei Wege (N-FR-019). Der Entwurf formuliert sie als **Beziehungsformen** zwischen Spieler und Gruppe:

| Weg | Beschreibung | Konflikt, Chancen |
|---|---|---|
| **Ehrliche Mitgliedschaft** | Eintritt als Anwärter, Arbeit für die Gruppe, Aufstieg | Pflichten, Loyalität, Rang, Zugang |
| **Infiltration (Spitzel)** | im Auftrag (Polizei, andere Organisation, eigene Motivation) in die Gruppe, Vertrauen aufbauen, Information sammeln, entscheiden, was man weitergibt | Doppelloyalität; Entdeckung; Bindung an Mitglieder |
| **Lose Verbindung** | einzelne Jobs, Bekanntschaften, Clubhaus besuchen, Handel, Events | geringere Rechte, geringere Pflichten |

### 6.2 Statuswerte des Spielers (Entwurf)

```text
Fremder ──► Kontakt ──► Anwärter ──► Mitglied ──► Vertrauter ──► Offizier ──► Führung
   │            │           │            │           │            │
   │            └─ lose Verbindung bleibt möglich ───┘            │
   │                                                              ▼
   └─── Spitzel (verdeckt)  ──────────────────────────► Aussteiger | Ausgeschlossener | Überläufer | Gründer (🔭)
```

Mitgliedszustände sind Daten der Mitgliedschaft (Abschnitt 5), die **Zustände der Kante** zwischen Spieler und Gruppe. Wechsel haben Ursachen.

### 6.3 Pflichten und Vorteile (Entwurf)

| Vorteil (Quelle N-FR-006) | Pflicht |
|---|---|
| Nebencontent, World Events, spezielle Jobs | Aufgaben ausführen; Regeln einhalten |
| Beziehungen, Kontakte, Informationen | Loyalität; Verschwiegenheit |
| Shops, Fahrzeuge, Kleidung | Symbole tragen; Treffen besuchen |
| Safehouses, Clubhäuser | Clubhaus schützen; Beiträge |
| Zugang zu Orten und Szenen | Schutz der Mitglieder |

**Keine Pflicht zum Erfüllen.** Ein Spieler kann Aufgaben ablehnen; Folgen sind Ruf, Rang, Beziehungen (N-LT-023: große Ursachen für große Folgen).

### 6.4 Mehrfachmitgliedschaft und Loyalitäten

Eine Mitgliedschaft in zwei Gruppen ist möglich, aber nicht folgenlos (N-FR-016). Der Entwurf lässt Konflikte aus **Beziehungen zwischen Fraktionen** entstehen (Abschnitt 8): Gehört der Spieler zu A, die B bekriegt, und zu B, entstehen Verdacht, Prüfung, Entscheidung. Dreifachloyalität (Hunter-Crew, MC, Polizeiinformant: `23#678A`) ist ausdrücklich optionaler Inhalt, kein Spionagedrama der Hauptstory.

### 6.5 Austritt und Folgen

Mögliche Wege (N-FR-018): freiwillig, Rauswurf, Suspendierung, Verrat, Zerfall der Gruppe. Folgen (Entwurf): Verlust von Zugang und Besitz, Veränderung der Kanten, **sichtbare Spuren der Vergangenheit** (Tattoo, Patch; `23#676A`: „Tattoo verschwindet natürlich nicht magisch“), Gerüchte, mögliche Rache. Nicht jeder Austritt ist feindlich.

### 6.6 Führung

Wer die Führung übernimmt (N-FR-022), bekommt Mitgliederverwaltung, Clubhaus, Jobs, Ressourcen, Events, Rivalitäten, **aber kein Strategiespiel**: Der Spieler bleibt in der Welt, erledigt Aufgaben selbst; Delegation ist kontextbezogen (Mitglieder auswählen, Jobs vergeben, Events planen, Konflikte lösen, Chapter unterstützen, Mitglieder befördern, Probleme beantworten: `23#680A`). Führung erzeugt **neuen Inhalt** statt des Endes der Fraktion.

### 6.7 Postgame

Fraktionsinhalte laufen nach der Hauptstory weiter (N-FR-003, N-FR-005, N-LT-025). Das Hunter-HQ bleibt aktiv; neue Rekruten können kommen (`23#678A`, `23#680A`).

### 6.8 Gründung eigener Gruppen

`23#676A` nennt als möglichen Weg ‚Spieler gründet eventuell später eine eigene Gruppe‘. Das ist eine **Vision (🔭)** ohne Entscheidung. Technisch wäre es eine Fraktion mit `player`-Leitung (Mod-Erweiterbarkeit: Modul 23).

---

## 7. Entwurf: Innenpolitik

N-FR-014 und N-FR-015 verlangen interne Politik: Konflikte, Führungswechsel, Ausschlüsse, Verrat, Abspaltungen, neue Chapter, Fusionen. Das ist Quelle von **endlosem Inhalt** (N-FR-005) und von Lebendigkeit ohne Spieler (N-LT-006).

### 7.1 Ereignisgeneratoren (Entwurf)

Aus dem Zustand der Gruppe erzeugt die Welt Konflikte (Quelle `23#680A`: ‚Faction-Simulation erzeugt selbst Probleme‘ mit den Beispielen Mitgliederkonflikt, Rivalen, Verhaftung, Verschwinden des Anführers). Der Entwurf:

| Auslöser | Bedingung | Mögliches Ereignis |
|---|---|---|
| **Konflikt A–B** | Streit, Eifersucht, Schuld | Eskalation, Verweis, Duell, Spaltung |
| **Territorialer Druck** | rivalisierende Gruppe | Krieg, Verhandlung (Modul 17) |
| **Verhaftung** | Polizeiaktion | Machtvakuum, Informantenverdacht |
| **Führungskrise** | Führer stirbt, verschwindet, schwächelt | Machtkampf, Wahl, Nachfolge |
| **Geldprobleme** | Verluste, Schulden | Rückzug, Ausstieg, Diebstahl |
| **Verrat, Spitzel** | Informant, Doppelagent | Säuberung, Misstrauen |
| **Wachstum** | viele Rekruten | neues Chapter, interne Fraktionen |
| **Ideologiekonflikt** | verschiedene Überzeugungen | Spaltung |
| **Äußere Ereignisse** | Gesetz, Polizeikampagne, Katastrophe | Anpassung |
| **Spielerhandlung** | Spieler unterstützt, sabotiert, verrät | Folgen für Rang, Beziehungen |

### 7.2 Mechanik (Entwurf)

1. **Gruppenzustände** (Zusammenhalt, Disziplin, Wohlstand, Furcht, Außenwirkung) sind interne Größen mit Trägheit; nur Ereignisse verändern sie.
2. **Ereignisse haben Auslöser, Beteiligte und Auflösung** und laufen in Phasen (Anzeichen, Eskalation, Entscheidung, Folgen).
3. **Der Spieler kann eingreifen**, wenn er Kenntnis hat und Mitglied oder Beobachter ist; er kann auch fernbleiben.
4. **Die Welt bleibt konsistent** (N-LT-008): Ein Führer, der stirbt, hat einen Nachfolger oder die Gruppe zerfällt.

### 7.3 Typische Verläufe (Entwurf)

- **Führungswechsel:** Tod, Rücktritt, Absetzung, Wahl, Ernennung; Folge: Neuordnung der Ämter; Spieler kann Kandidat oder Wähler sein.
- **Spaltung:** Teil der Gruppe verlässt sie, nimmt Mitglieder und Besitz; zwei Gruppen entstehen.
- **Fusion:** zwei Gruppen verschmelzen; Konflikte bei Ämtern.
- **Neues Chapter:** Expansion in neue Gebiete.
- **Säuberung:** nach Verrat verlieren Mitglieder Rang oder Leben.
- **Auflösung:** Zerfall; Mitglieder verstreuen sich.

---

## 8. Entwurf: Beziehungen zwischen Fraktionen

Die Quelle schlägt ein Raster vor: Allied, Friendly, Neutral, Tense, Hostile, War (N-FR-017, 🧭). Der Entwurf behandelt sie als **gerichtete Kante zwischen Fraktionen** (wie Modul 08, aber auf Gruppenebene):

```text
FactionRelation (von A nach B)
├── stance       Verbündet | Freundlich | Neutral | Angespannt | Feindlich | Krieg
├── drivers      Gründe: Gebiet, Geschäft, Ehre, Geschichte, Personen, Ideologie
├── incidents[]  Ereignisse (Verrat, Hilfe, Grenzverletzung, Heirat, Mord)
├── agreements   Bündnisse, Abkommen, Waffenstillstand, Tribute
├── secrecy      offen | geheim
└── trend        steigend | stabil | fallend
```

**Regeln (Entwurf):**

1. **Stufen wechseln durch Ereignisse und Zeit**, mit Hysterese; ein einzelner Zwischenfall führt nicht zum Krieg (N-LT-023).
2. **Asymmetrie.** A kann B für einen Freund halten, B A für einen Gegner.
3. **Dritte.** Bündnisse und Gegnerschaften sind transitiv nur teilweise (der Feind meines Feindes ist nicht automatisch ein Freund).
4. **Stellvertreter.** Gruppen führen Konflikte über Dritte (Handlanger, Menschen als Werkzeuge: Kreaturen 09 §6).
5. **Spielerwirkung.** Spielerhandlungen verschieben Beziehungen; die Kette der Folgen ist nachvollziehbar.
6. **Stufen als Daten.** Die Anzahl und Namen der Stufen sind Entwurf; Mods können feinere Stufen definieren.

**Beziehungen zu Menschen, Behörden, Wesen.** Gruppen haben eine Haltung zu der Zivilbevölkerung (Schutzgeld, Wohltaten), zu Behörden (Korruption, Feindschaft, Kooperation), zu Huntern (Misstrauen, Schutz), zu Wesen (Duldung, Jagd, Kooperation). Detail: Modul 18.

---

## 9. Entwurf: Symbole, Orte, Fahrzeuge

Die Quellen nennen sichtbare Zeichen der Zugehörigkeit: Club-Patches, Tattoos, Gefängnistattoos, Rangabzeichen, Symbole, Farben, Schmuck, bestimmte Fahrzeuge, Kleidungsstile; und dass Spieler und NPCs solche Zeichen tragen können, wobei das Verschwinden aus der Gruppe nicht magisch ist (`23#676A`). Der Entwurf (💡):

| Zeichen | Rolle |
|---|---|
| **Patch, Kutte, Farben** | Zugehörigkeit und Rang; kann entzogen werden |
| **Tattoo** | dauerhaft; Gefängnis- und Clubtattoos als Erkennungsmerkmal (Ermittlung 13 §13) |
| **Symbolik** | Zeichen auf Fahrzeugen, Wänden, Kleidung |
| **Fahrzeuge** | gruppentypische Fahrzeuge, Umbauten, Farben |
| **Clubhaus, Treffpunkt** | Ort der Gruppe mit Regeln und Zeitplänen (Modul 13) |
| **Verdeckte Zugehörigkeit** | höhere Mitglieder tragen nichts offen (Abschnitt 5) |
| **Falsche Zugehörigkeit** | Täuschung (Infiltration, Verkleidung) |

**Konsequenz:** Erkennbare Zeichen wirken auf Wahrnehmung (Modul 06), Ruf (Ermittlung 13) und Beziehungen (Modul 08). Sie können falsch gelesen werden.

**Zuständigkeit.** Tattoos und Aussehen: [Decision 0005](../../Decisions/0005-character-appearance-foundation.md) und Adult 07/25 (Technik); dieses Modul legt nur die Bedeutung fest.

---

## 10. Entwurf: Inhalte und Ereignisse

Das Ziel ist ‚nicht dasselbe Missionssystem mit anderem Skin‘ (`23#680A`). Der Entwurf lässt Inhalte aus Typ und Zustand entstehen (Beispiele der Quelle, 🧭):

| Gruppentyp | Jobs und Ereignisse (Quelle) |
|---|---|
| Motorradclub | Club-Events, Ausfahrten, Transporte, Fahrzeugjobs, Schutz-/Hilfsaufträge, Rivalitäten, Clubhaus-Events, Rennen, Rekrutierung |
| Racing Club | Track Days, Championships, Car Meets, Tuning Events, Rallyes, Rivalitäten, Time Trials |
| Kriminelle Organisation | andere Jobs (Geschäft, Schutz, Beschaffung, Eintreibung) |
| Sekte | andere Aufgaben (Rituale, Anwerbung, Abschottung) |
| Hunter-Netz | dynamische Hunter-Fälle, Recherche, Rettung, Hilferufe, Rekruten (`23#680A`) |

**Dynamische Faction-Ereignisse** (Abschnitt 7) laufen im Hintergrund und treten auf, wenn der Spieler Teil der Gruppe ist oder in der Region ist. Sie sind kein Pflichtinhalt.

**Kombination aus Handarbeit und System** (`23#680A`: ‚handgeschriebener hochwertiger Content + systemisch generierte Fälle + dynamische Weltzustände‘): Die Hauptfiguren der Gruppen sind handgeschrieben, ihre Aufgaben systemisch.

---

## 11. Sonderfälle

1. **Die Gruppe wird durch den Spieler zerschlagen.** Mitglieder zerstreuen sich; Racheketten (Modul 10 §9); Territorium wird frei (Modul 17).
2. **Der Spieler führt eine Gruppe, die in Konflikt mit der Hunter-Crew gerät.** Dreifachloyalität, siehe 6.4.
3. **Ein Mitglied ist Wesen mit menschlicher Identität.** Das Wesen tarnt sich in der Gruppe; Aufdeckung hat Folgen (Ermittlung 13).
4. **Polizei infiltriert die Gruppe.** NPC-Spitzel; Zweifel; Aufdeckung (Ermittlung 13 §9).
5. **Der Spieler wird als Spitzel enttarnt.** Rang verloren; Gewalt droht; Chance der Aufklärung.
6. **Eine Gruppe zerfällt ohne Spielerhandlung.** Ereignis aus Innenpolitik (Abschnitt 7).
7. **Gruppen ohne Geld.** Verlust von Clubhaus, Fahrzeugen, Besitz.
8. **Mitglied stirbt.** Ersatz, Beerdigung, Rache; Gruppe verändert sich.
9. **Gruppenkrieg trifft Zivilisten.** Opferzahl, Polizeieinsatz, öffentliche Reaktion (Modul 15).
10. **Mod-Fraktionen.** Registrierung additiv; unbekannte Fraktionsdaten bleiben unverändert (Modul 23).
11. **Hauptstory-Fraktionen.** Autorenschutz (Modul 11 §12).
12. **Fraktionen im Untergrund.** Offen (Q-N25).
13. **Koop.** Fraktionsmitgliedschaft je Spieler im Host-Speicherstand (N-FR-028, Q-N15).
14. **Spieler tritt aus der Hunter-Crew aus?** Die Quellen sehen das nicht vor; offen.

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-306 | Keine Hauptmission ist von Fraktionsmitgliedschaft oder Rekrutierung abhängig | N-FR-003 |
| N-EN-307 | Eine Fraktionsentscheidung folgt der Entscheidungsform der Gruppe, nicht dem Spielerwunsch | Abschnitt 4 |
| N-EN-308 | Ein Rang ändert sich nur durch ein Ereignis mit Ursache | N-FR-011 |
| N-EN-309 | Der Rang ist nicht allein durch Zeit oder Erfahrungspunkte bestimmt | N-FR-012 |
| N-EN-310 | Gruppenruf und individuelle Beziehung können verschieden sein | N-FR-023 |
| N-EN-311 | Eine Fraktion hat jederzeit eine gültige Führung oder einen dokumentierten Führungsausfall | Abschnitt 7 |
| N-EN-312 | Ein Mitglied verlässt die Gruppe mit einem Ereignis; Besitz, Rechte und Beziehungen ändern sich nachvollziehbar | N-FR-018 |
| N-EN-313 | Eine verdeckte Mitgliedschaft ist für Nichteingeweihte nicht erkennbar, aber im Datensatz vorhanden | Abschnitt 5 |
| N-EN-314 | Fraktionsbeziehungen wechseln nur über Ereignisse und mit Hysterese | Abschnitt 8 |
| N-EN-315 | Zwei rivalisierende Mitgliedschaften des Spielers erzeugen Prüfung und Folgen | N-FR-016 |
| N-EN-316 | Eine Fraktion verwendet nur Mittel aus dem Weltzustand | N-LT-008 |
| N-EN-317 | Gruppenwissen ist durch Rollen begrenzt; ein Mitglied weiß nicht automatisch, was die Führung weiß | Modul 05 §10 |
| N-EN-318 | Ein Führungswechsel erzeugt Nachfolge, Machtkampf oder Zerfall, nie Leere | N-FR-015 |
| N-EN-319 | Der Austritt löscht keine Tattoos, Narben oder Erinnerungen | N-FR-018 |
| N-EN-320 | Ein Spieler in Führungsposition löst Aufgaben selbst; es gibt kein erzwungenes Mikromanagement | N-FR-022 |
| N-EN-321 | Fraktionen reagieren auf Spielerhandlungen nur, wenn sie davon erfahren haben | N-GE-008 |

---

## 13. Was fehlt

- **Keine Fraktionsliste**, keine Namen, Ziele oder Besetzungen (N-FR-029). Das ist eine bewusste Grenze des Proposals 0010.
- Offen: wie der Aufstieg technisch bewertet wird (Q-N14).
- Offen: Zahl und Namen der Beziehungsstufen zwischen Fraktionen (N-FR-017).
- Offen: Umgang mit Gewalt als Gruppenstrafe in der Darstellung.
- Offen: die Gründung eigener Gruppen (🔭).
- Offen: Verhältnis der Untergrundzivilisation zu diesem Modell (Q-N25).
