# NPCs 23: Koop, Host-Speicherstand und Mods

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie sich NPC-Gedächtnis, Beziehungen, Nemesis, Fraktionen und die lebendige Welt im Koop mit bis zu vier Spielern verhalten und wie Mods den NPC-Kern erweitern, ohne ihn zu zerstören. Beschlossen sind: der Host-Speicherstand ist maßgeblich, Gastfortschritt ist an den Host-Spielstand gebunden, Entscheidungen im Koop sind situationsabhängig (persönlich individuell oder per Teamabstimmung), Modding ist additiv, anbieterunabhängig und pro Spielstand aktivierbar. **Das Subjektmodell, die Replikation und die Erweiterungspunkte sind Entwurf (💡).** Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Koop-Ermittlung: [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md). Speicher: [Modul 21](21-Weltpersistenz-und-Speicherentwurf.md). Simulation: [Modul 22](22-Simulationsstufen-Skalierung-und-Performance.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Autorität und Replikation](#2-entwurf-autorität-und-replikation)
3. [Entwurf: Subjekte – mehrere Spieler in einer NPC-Welt](#3-entwurf-subjekte--mehrere-spieler-in-einer-npc-welt)
4. [Entwurf: Teamentscheidungen mit NPC-Folgen](#4-entwurf-teamentscheidungen-mit-npc-folgen)
5. [Entwurf: Beitritt, Austritt, Unterbrechung](#5-entwurf-beitritt-austritt-unterbrechung)
6. [Entwurf: Mods und der NPC-Kern](#6-entwurf-mods-und-der-npc-kern)
7. [Entwurf: Erweiterungspunkte](#7-entwurf-erweiterungspunkte)
8. [Entwurf: Mods im Spielstand](#8-entwurf-mods-im-spielstand)
9. [Entwurf: Online](#9-entwurf-online)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-KO-001 | **Host-Spielstand ist maßgeblich;** der Fortschritt von Gästen gehört zum Host-Spielstand; die Welt ist die Welt des Hosts | 📋 | `CLAUDE.md` §6; „es können sich die leute immer ändern aber der fortschritt ist auf dem spielstand gebunden“ in `48#105E` ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) C6, C16) |
| N-KO-002 | Was ein Gast auf seinem eigenen Spielstand hat, bleibt dort; bei einem Freund hat er nur, was er auf **dessen** Spielstand hat | 📋 | `48#107E` ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) C7) |
| N-KO-003 | Koop: 2–4 Spieler, kampagnenbasiert, Mods und Cheats erlaubt, von der offiziellen Online-Progression getrennt | 📋 | `M-PD` ‚Multiplayer separation‘ |
| N-KO-004 | Offizielles Online: eine persistente Online-Slot-Progression; keine Mods und Cheats; die Zeit-/Serverlogik für Online ist später eigen („im online wird das anders sein“) | 📋 | `M-PD` ‚Multiplayer separation‘; „11 die welt läuft nur wenn man gerade spielt, im online wird das anders sein“ in `35#279E` |
| N-KO-005 | **Entscheidungen im Story-Koop sind situationsabhängig:** persönliche (Dialoge, Beziehungen zu NPCs, moralische Entscheidungen) trifft jeder Spieler selbst; gemeinsame (Ermittlungsentscheidungen, Schicksal eines gemeinsamen Falls, Entscheidungen mit Wirkung auf die gemeinsame Story) werden abgestimmt | 📋 (Wahl) / 💡 (Tabelle) | „Supernatural Story-Koop Entscheidungen: Situationsabhängig: persönlich individuell, gemeinsam per Abstimmung“ in `48#97E`; Tabelle `48#98A` |
| N-KO-006 | Bei Gleichstand gelten situationsabhängige Regeln | 📋 | `48#99E` ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md) C3) |
| N-KO-007 | Das gemeinsame Journal löst unterschiedliche Meinungen und Theorien der Spieler nicht automatisch auf | 💡 | `48#98A` |
| N-KO-008 | Individuelle Verhaftung; die Welt läuft weiter; ein Welt-Zeitsprung im Koop nur mit Zustimmung aller aktiven Spieler; bei Zustimmung entwickeln sich Monster, NPCs, Organisationen und Ermittlungen weiter | 📋 | `48#161E`, „Supernatural Koop-Haft-Zeitsprung: Gemeinsamer Welt-Zeitsprung mit Zustimmung aller“ in `48#163E`; Auslegung `48#164A` |
| N-KO-009 | Jeder Koop-Spieler erstellt seinen Hunter über die Charaktererstellung | 📋 | `48#109E` |
| N-KO-010 | Modding ist ausdrücklich unterstützt und soll keine Reverse-Engineering-Arbeit verlangen; Mods sind standardmäßig additiv und nicht zerstörend über APIs, Registries und Hooks; kontrollierte Überschreibungen laufen über explizite Mod-Layer-/Ladereihenfolge-Regeln | 📋 | `M-PD` ‚Modding‘ |
| N-KO-011 | Vorläufige Mod-Ebenen: Daten-/Inhaltsmods, Unreal-/Blueprint-/visuelle Mods, native C++20-Mods; native Mods sind vertrauter Code; öffentliche APIs nach dem Prinzip der geringsten Rechte | 📋 | `M-PD` ‚Modding‘ |
| N-KO-012 | Das technische Mod-Kern ist von keinem Anbieter abhängig (Steam Workshop ist nicht gewählt oder verpflichtend); die lokal installierte Menge bleibt ohne Online-Anbieter nutzbar; Standardort `<GameRoot>/mods/`; `.spnmod` als portables Paket; installiert und aktiviert sind getrennt; pro Profil und pro Spielstand aktivierbar | 📋 | `M-MOD` (Block 5, CONFIRMED / CANONICAL) |
| N-KO-013 | Kritisch fehlende Mods blockieren das Laden des Spielstands standardmäßig; eine ausdrückliche Expertenübersteuerung erlaubt das Laden auf eigenes Risiko erst nach automatischem Backup und Risikohinweis; deaktivierte Mods bleiben installiert, ihre Dateien und ihr Zustand bleiben erhalten; Deaktivierung im Spiel ist zuerst logisch (keine neuen Aktivierungen, sichere Neuladung/Abgleich) | 📋 | `M-MOR` (Rekonstruktion 2026-09-09) |
| N-KO-014 | Mods sollen leicht zugänglich sein: dokumentierte APIs, stabile IDs, Datenformate, Beispielmods, Dokumentation, Debugger, Validierung, einfaches Packaging, verständliche Fehlermeldungen; „ja ich will dass modding erlaubt und einfach zu machen ist“ | 📋 | `23#48A` Abschnitt 18; Antwort „18 ja ich will dass modding erlaubt und einfach zu machen ist“ in `23#49E` |
| N-KO-015 | Mögliche Mod-Inhalte (Beispiele des Assistenten): neue Kreaturen, Fälle, Waffen, Rituale, Items, Codex-Einträge, Orte, **NPCs**, **Weltereignisse**, Karten | 💡 | `23#48A` Abschnitt 18 |
| N-KO-016 | Wie weit Native-C++-Modding gehen soll, wird gesondert entschieden; Lua ist nur optional, wenn es eine echte Lücke schließt | 📋 / ❓ | `23#48A` Abschnitt 18 („später gesondert entscheiden“); `M-PD` ‚Modding‘; Entwickler: „so wie beim minecraft spiel, nur ohne lua alles in c++“ in `23#52E` Punkt 18 (früher als `M-PD`; dort ist Lua nur optional) |
| N-KO-017 | Neue Datensatzarten müssen additiv registriert werden; ein Spielstand mit unbekannten Arten muss unverändert laden und wieder speichern | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6; [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) |
| N-KO-018 | Die Simulation läuft nur auf der Autorität; die Replikation des Simulationszustands ist offen; `WorldFacts` hat noch kein Compare-and-Set | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §6; [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) |
| N-KO-019 | Die Mod-Erweiterungspunkte des NPC-Systems sind offen | ❓ | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11 |

---

## 2. Entwurf: Autorität und Replikation

| Aspekt | Entwurf |
|---|---|
| **Autorität** | Der Host führt den Weltzustand und die gesamte NPC-Simulation (S2/S3, Fortschreiben, Fraktionen, Nemesis, Gerüchte). Gäste simulieren nicht (N-KO-001). |
| **Was wird repliziert?** | Darstellung der materialisierten Akteure nahe jedem Spieler (S0/S1), Dialogzustände, sichtbare Ereignisse. Nicht repliziert: Fernsimulation, Journal, Datensätze (nur auf Anfrage und nach Relevanz). |
| **Wie?** | Replikation ist ein Technik- und Netzwerkthema; die Quellen und Dokumente legen kein Format fest (N-KO-018). |
| **Determinismus** | Gleicher Weltzustand und Seed liefern dieselben Entscheidungen (Modul 22 §7). Gäste müssen Zustände nicht neu berechnen. |
| **Konsistenz** | Ein NPC existiert pro Welt nur einmal; zwei Spieler sehen denselben NPC, nicht zwei Kopien. |
| **Gleichzeitige Änderungen** | Ereignisse werden auf dem Host geordnet (Modul 24 §3); `WorldFacts` hat keine Gleichzeitigkeits-Kontrolle (N-KO-018). |
| **Latenz** | Dialog und Beziehungen müssen bei Latenz nicht Frame-genau sein; Weltereignisse haben Reihenfolge, nicht Echtzeit. |
| **Ausfall des Hosts** | Verbindung bricht ab; Weltzustand liegt im Host-Speicherstand; Gäste bleiben ohne Welt. Hostwechsel ist offen (Q-N26). |

---

## 3. Entwurf: Subjekte – mehrere Spieler in einer NPC-Welt

Die Quellen: Persönliche Entscheidungen, Dialoge und **individuelle Beziehungen zu NPCs** gehören dem jeweiligen Spieler; gemeinsame Entscheidungen werden abgestimmt (N-KO-005; die Beziehungszeile steht in der Tabelle des Assistenten, 💡). Der Entwurf nutzt dafür ein **Subjektmodell** (💡):

```text
Subjekt = ein Spieler (Hunter) | das Team als Gruppe ("Hunter-Team")
```

| Datenart | Subjektbezug | Wirkung |
|---|---|---|
| **Beziehungskante** NPC → Spieler | je Spieler eigene Kante (N-KO-005) | NPC behandelt Spieler A anders als B |
| **Gruppenkante** NPC → Team | zusätzliche, aus Mitgliedern abgeleitete Kante | NPC reagiert auf „das Team“ (z. B. bei gemeinsamem Auftreten) |
| **Erinnerungseintrag** | kann mehrere Subjekte enthalten; Wirkung je Subjekt | Zeuge erinnert ‚zwei Fremde‘ mit Eigenschaften; Zuordnung bei Wiedererkennung je Person |
| **Ruf** (Gerüchte, Behörden) | je Person; Team-Ruf als Aggregat | Fahndung individuell (N-KO-008); Team-Ruf bei gemeinsamem Auftreten |
| **Archiv/Journal** | persönlich + gemeinsam (Companion-App zeigt gemeinsame Akten und Teamfortschritt: Ermittlung 18 C9) | Spielerwissen bleibt personenbezogen, wo nicht geteilt |
| **Nemesis-Ziel** | `targets[]` kann Spieler, Team oder Begleiter enthalten | Feindschaft entsteht gegen die handelnde Person, bei Teamentscheidungen gegen das Team |
| **Fraktionsmitgliedschaft** | je Spieler | zwei Spieler können in verschiedenen oder rivalisierenden Gruppen sein (Konflikt unter Mitspielern: Q-N26) |
| **Rekrutierung** | Rekrut gehört dem Host-Spielstand; wer ihn rekrutierte, hat die Bindung | Verteilung unter Spielern offen |
| **Crew/HQ** | gehört zur Host-Welt; Gäste sind Gäste | Hunter-HQ als Ort der Host-Welt |
| **Persönliche Wünsche der Crew** | Auslöser je Spieler denkbar | offen |

**Regeln (Entwurf):**

1. **Der NPC beobachtet Personen.** Wahrnehmung und Gedächtnis haben Spieler als Subjekte (Modul 05 §4 Punkt 4).
2. **Individuelle Entscheidungen haben individuelle Folgen.** Wer lügt, wird als Lügner erinnert; die anderen nicht (N-KO-005).
3. **Gemeinsame Entscheidungen haben gemeinsame Folgen.** Die Folgen treffen das Team, soweit die Abstimmung das ausgelöst hat (Abschnitt 4).
4. **Wissen wird nicht automatisch geteilt.** Was Spieler A erfährt, weiß B nur, wenn es geteilt wurde (Journal, Gespräch, Funk).
5. **Fahndung und Haft sind individuell** (N-KO-008).
6. **Der NPC-Zustand ist einzigartig.** Es gibt keine spielerspezifischen NPC-Welten; Unterschiede entstehen nur in den Kanten und Erinnerungen.

---

## 4. Entwurf: Teamentscheidungen mit NPC-Folgen

Die Quellen unterscheiden persönliche und gemeinsame Entscheidungen. Für die NPC-Welt (💡):

| Entscheidungsart | Beispiel | Folge für NPCs |
|---|---|---|
| **Persönlich** | Dialogoption, Gefallen annehmen, Geheimnis anvertrauen | Beziehungskante des Handelnden |
| **Persönlich-moralisch** | einen Zeugen bedrohen, eine Information weitergeben | Erinnerung und Ruf des Handelnden |
| **Team: Ermittlung** | Verdächtigen beschuldigen | Beschuldigter, Gemeinschaft, Behörde: Folgen für das Team (und Einzelne, die zustimmten, nach Regel) |
| **Team: Schicksal** | Wesen töten, verschonen, fangen | Nemesis-Entstehung (Modul 11): Ziele; Rache gegen Team oder Beteiligte |
| **Team: Story** | Bündnis mit Fraktion, Aufnahme eines Rekruten | Fraktionsbeziehung, Crew |
| **Gleichstand** | 2:2 | situationsabhängige Regel (N-KO-006); gewichtige Entscheidungen bleiben offen, bis Einigung (Vorschlag A des Assistenten, `48#98A`) |

**Fragen (Entwurf).** Wer trägt Verantwortung für eine Team-Entscheidung, die der Einzelne ablehnte? (Wer dagegen stimmte, steht nicht automatisch im Fokus der Rache, aber das Team schon.) Das Verhalten der NPCs hängt davon ab, **wen sie wahrnehmen** und **wem sie die Tat zuschreiben** (Modul 08 §6): Die Zuschreibung folgt der Wahrnehmung, nicht der Abstimmung.

**Offen.** Wie Teamentscheidungen im Datenmodell erscheinen (Ereignis mit Abstimmungsdaten, Beteiligte), und wie Wirkung auf Abwesende (Spieler, die nicht dabei waren) behandelt wird (Q-N26).

---

## 5. Entwurf: Beitritt, Austritt, Unterbrechung

Die Ermittlungs-Dokumentation behandelt Beitritt und Austritt im Koop (Ermittlung 18 §10). Für NPCs gilt (💡):

| Ereignis | Wirkung |
|---|---|
| **Gast tritt bei** | Gast ist Spieler im Host-Zustand; seine Beziehungskanten zu NPCs im Host-Spielstand beginnen leer oder aus früherer Teilnahme (N-KO-001, N-KO-002); NPCs erkennen ihn nur nach Begegnung |
| **Gast verlässt die Sitzung** | Kanten und Erinnerungen bleiben im Host-Spielstand (für Wiedereintritt); sein Charakter ist in der Welt abwesend; er beeinflusst nichts |
| **Gast kehrt zurück** | Kanten, Erinnerungen, Ruf bleiben; die Welt ist weitergelaufen (Weltzeit) |
| **Host wechselt** | Weltzustand bleibt beim ursprünglichen Host-Spielstand; ein Hostwechsel wäre ein Kopieren/Verschieben von Zustand (offen, Q-N26) |
| **Unterbrechung** | Pause ohne Weltzeit; Verbindungsabbruch lässt den Host-Zustand intakt |
| **Welt-Zeitsprung** | nur mit Zustimmung aller aktiven Spieler (N-KO-008) |
| **Charakter-Tausch** | jeder Spieler hat seinen Hunter (N-KO-009); Wechsel des Charakters ist offen |

**Regeln (Entwurf):**

1. **Kanten gehören dem Host-Spielstand,** nicht dem Gast-Profil (N-KO-001).
2. **Der Gast nimmt keine NPC-Zustände mit** in seinen eigenen Spielstand (N-KO-002).
3. **Ein NPC erinnert Gäste auch nach Ende der Sitzung,** solange der Host-Spielstand besteht.

---

## 6. Entwurf: Mods und der NPC-Kern

Modding ist beschlossen (N-KO-010 bis N-KO-014); die NPC-Erweiterungspunkte sind offen (N-KO-019). Der Entwurf legt Grundsätze fest (💡):

| Grundsatz | Inhalt |
|---|---|
| **Additiv** | Mods fügen NPC-Arten, Vorlagen, Ereignisse, Dialoge, Fraktionen hinzu, ohne den Kern zu ändern (N-KO-010) |
| **Registries** | NPC-Vorlagen, Berufe, Zeitpläne, Fraktionstypen, Beziehungsdimensionen, Ereignisarten, Erinnerungs-Tags, Gerüchtevorlagen, Taktikvorräte und Artregeln werden registriert; der Kern liest die Registries |
| **Namensräume** | IDs enthalten die Mod-Kennung; keine Kollision mit Kernobjekten (D2-Grammatik als technische Regel: [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md)) |
| **Überschreiben** | nur über explizite Layer- und Ladereihenfolge-Regeln (N-KO-010) |
| **Validierung** | Mods werden validiert (Bedingungen auflösbar, IDs gültig, Referenzen vorhanden); verständliche Fehler (N-KO-014) |
| **Budgets** | Mod-Inhalte unterliegen denselben Simulationsbudgets (Modul 22): Obergrenzen für Entitäten, Ereignisse, Gerüchte |
| **Rechte** | Daten-Mods laufen in der Sandbox der Datenformate; native Mods sind vertrauter Code mit minimalen API-Rechten (N-KO-011) |
| **Spielstand** | Mod-Zustand ist im Spielstand abgelegt und bleibt bei Deaktivierung erhalten (N-KO-013) |
| **Nicht-Mods** | Das Offizielle Online hat keine Mods (N-KO-004) |
| **Schutz des Kerns** | Kernobjekte (z. B. Hauptstory-NPCs) sind schreibgeschützt oder durch ‚Authored/Protected‘-Flags gesichert; Mods dürfen Hauptstory-Pfade nicht brechen |
| **Daten statt Code** | wo möglich: NPCs, Fraktionen, Ereignisse als Daten (Datenmods), nicht als Code |

---

## 7. Entwurf: Erweiterungspunkte

Die folgende Liste sammelt **mögliche** Erweiterungspunkte. Sie ist ein Vorschlag, kein API-Entwurf (💡; N-KO-019).

| Erweiterungspunkt | Was ein Mod liefern kann | Voraussetzungen |
|---|---|---|
| **NPC-Vorlage / Archetyp** | Berufe, Persönlichkeitsverteilungen, Zeitpläne | Registry (Modul 03, 04, 13) |
| **Benannte NPCs** | handgeschriebene Figuren mit Stufe A/B, Dialog, Beziehungen | stabile IDs, Datensatz |
| **Fraktion** | Typ, Struktur, Ränge, Ziele, Territorienzuordnung | Registry (Modul 16, 17) |
| **Ereignisarten** | Lebensereignisse, Weltereignisse, Gruppenereignisse | Ereignisschema (Modul 14, 15) |
| **Beziehungsdimensionen** | zusätzliche Dimensionen und deren Bänder | Registry (Modul 08) |
| **Gedächtnis-Tags** | neue Erinnerungs-Tags und Dialoghooks | Registry (Modul 05, 20) |
| **Dialogzeilen** | Zeilen mit Bedingungen, Sprecher, Stimme | Dialogformat (Modul 20) |
| **Gerüchtevorlagen** | typische Gerüchte und Verzerrungen | Modul 07 |
| **Nemesis-Taktikvorräte** | Taktik- und Gegentaktikdaten je Art | Modul 11 |
| **Aussehen** | Rezepte, Kleidung, Tattoos | [Decision 0005](../../Decisions/0005-character-appearance-foundation.md) |
| **Inspektoren** | Werkzeuge im Mod-/Debug-Modus | Modul 20 §9 |
| **Orte und Betriebe** | Geschäfte, Lokale, Clubhäuser | Welt |
| **Kommunikationsstile** | Kanäle und Verhalten | Modul 04 §7 (offen) |

**Nicht erweiterbar (Entwurf):** Kernregeln für Determinismus, Persistenz-Integrität, Schutzstufen der Hauptstory und Altersregeln ([Adult 01](../Adult/01-Grundregeln-Alter-Zustimmung.md)).

---

## 8. Entwurf: Mods im Spielstand

Aus N-KO-013 folgt für NPCs:

| Situation | Entwurf |
|---|---|
| **Mod aktiviert** | Mod-NPCs, Fraktionen, Ereignisse beginnen zu wirken; die Welt startet sie aus Seeds/Datensätzen |
| **Mod deaktiviert** | Mod-Daten bleiben im Spielstand (`unknown_kinds`-Segment, Modul 21 §2); Mod-NPCs sind **inaktiv** (nicht gespawnt, nicht fortgeschrieben); Beziehungen bleiben eingefroren |
| **Mod entfernt** | gleiche Behandlung wie deaktiviert; Beim Laden Hinweis; kritisch fehlend blockiert (N-KO-013) |
| **Mod aktualisiert** | Schemaänderung → Migration der Mod-Daten durch das Mod (Versionsfeld); eingefrorene NPCs bleiben unverändert |
| **Mod wieder aktiviert** | Zustand wird wiederaufgenommen; die Weltzeit, die verging, wird nachgeholt (Fortschreiben) |
| **Zwei Mods konkurrieren** | Ladereihenfolge/Layer; sichtbare Konfliktmeldung (N-KO-010) |
| **Mod-NPC ist Teil eines Falls** | Fall verweist auf ID; bei Deaktivierung wird der Fall gesperrt oder verlagert (Regel der Mod-Abhängigkeit) |
| **Mod-Fraktion beherrscht Gebiet** | Beim Deaktivieren fällt der Anspruch; andere Gruppen übernehmen (Modul 17) — Entscheidung offen |
| **Logisch-Deaktivierung im Spiel** | keine neuen Aktivierungen; bereits geladene Akteure laufen bis zu einem sicheren Übergang (N-KO-013) |

**Regel:** Ein Mod darf nie dazu führen, dass der NPC-Kern **inkonsistent** wird (verwaiste Verweise, doppelte IDs, Zeitwidersprüche); Validierung und Tombstones verhindern das (Modul 21 §8).

---

## 9. Entwurf: Online

Offizielles Online hat eigene Progression, keine Mods und Cheats (N-KO-004). Für die NPC-Welt im Online gibt es **keine** Entscheidung. Offene Fragen (❓ Q-N28):

1. Ob das Online eine eigene NPC-Welt hat (persistente Server-Welt) oder keine Lebenswelt im Sinn dieses Ordners.
2. Wie viel der Lebenssimulation dort läuft (64–100 Spieler als Ziel: `M-PD`).
3. Die Zeitlogik ("im online wird das anders sein").
4. Das Verhältnis zu Koop-Welten und Singleplayer.

Dieser Ordner beschreibt daher nur Singleplayer und Koop.

---

## 10. Sonderfälle

1. **Zwei Spieler töten denselben NPC gemeinsam.** Beide sind Täter; Gerüchte nennen beide oder eine Beschreibung; Folgen für beide und das Team.
2. **Spieler A hilft einem NPC, Spieler B bedroht ihn.** Der NPC hat zwei Kanten; er reagiert je nach Gegenüber.
3. **Der Host beendet die Sitzung während eines Nemesis-Angriffs.** Zustand bleibt in Ereignissen konsistent (Atomarität: Modul 21).
4. **Gäste mit anderen Mods.** Mods gelten für die Sitzung nach Host-Spielstand (Koop erlaubt Mods; Mod-Menge ist Teil des Host-Spielstands).
5. **Ein Gast bringt Wissen aus seinem Spielstand mit.** Er behält nur das, was er auf dem Host-Spielstand hat (N-KO-002).
6. **Teamabstimmung über Rekrutierung.** Ein Rekrut kann Kritik der Crew erzeugen (Modul 19).
7. **Zeitsprung gegen den Willen eines Spielers.** Nicht erlaubt (N-KO-008).
8. **Gast wird verhaftet.** Individuell; die Welt läuft (N-KO-008).
9. **Spieler erstellt identisch aussehenden Charakter wie ein NPC.** Verwechslung ist erlaubt und kann Gegenstand von Fällen sein (Ermittlung 13).
10. **Mod-NPC begleitet Spieler; Mod wird deaktiviert.** Begleiter wird inaktiv; Beziehung eingefroren; Rückkehr beim Wiederaktivieren.
11. **Mehrere Hosts mit demselben Spielstand-Backup.** Kein Abgleich (jeder Spielstand eigene Geschichte).
12. **Spieler mit verschiedenen Zeitstufen im Koop (Reiche/Zeitreise).** Offen; Zeitstand-Regeln gelten (Welt 26).

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-501 | Ein NPC existiert pro Welt genau einmal und ist für alle Spieler derselbe | Abschnitt 2 |
| N-EN-502 | Beziehungskanten zu verschiedenen Spielern sind unabhängig | N-KO-005 |
| N-EN-503 | Gastfortschritt (Beziehungen, Erinnerungen, Ruf) ist im Host-Spielstand gespeichert und wird nicht in den Gast-Spielstand übernommen | N-KO-001, N-KO-002 |
| N-EN-504 | Ein Welt-Zeitsprung im Koop findet nur mit Zustimmung aller aktiven Spieler statt | N-KO-008 |
| N-EN-505 | Individuelle Verhaftung eines Spielers hält die Welt nicht an | N-KO-008 |
| N-EN-506 | Ein Mod-Datensatz wird bei Deaktivierung nicht gelöscht und bei Reaktivierung unverändert wiederverwendet | N-KO-013 |
| N-EN-507 | Ein Spielstand mit unbekannten Datenarten lädt und speichert sie unverändert | N-KO-017 |
| N-EN-508 | Mod-IDs kollidieren nicht mit Kern-IDs | Abschnitt 6 |
| N-EN-509 | Kein Mod bricht Hauptstory-Pfade oder Schutzstufen | Abschnitt 6 |
| N-EN-510 | Mod-Inhalte unterliegen denselben Simulationsbudgets | Abschnitt 6 |
| N-EN-511 | Das Offizielle Online lädt keine Mod-Daten | N-KO-004 |
| N-EN-512 | Die Simulation läuft nur auf der Autorität | N-KO-018 |
| N-EN-513 | Ein Gast ohne Begegnung mit einem NPC hat bei diesem keine Erinnerung | N-GE-008 |
| N-EN-514 | Eine Teamentscheidung wirkt auf NPCs nur über wahrgenommene Handlungen | Abschnitt 4 |
| N-EN-515 | Die Altersregeln des Adult-Systems sind durch Mods nicht abschaltbar | Abschnitt 7 |

---

## 12. Was fehlt

- Kein Replikationsformat, kein Hostwechsel, kein Zusammenführen von Welten (Q-N26).
- Offen: Verantwortung für Team-Entscheidungen bei abweichender Stimme.
- Offen: wie Rekruten und HQ unter mehreren Spielern verteilt werden.
- Offen: Online-NPC-Welt (Q-N28).
- Offen: Umfang des Mod-Zugriffs auf den NPC-Kern (N-KO-019).
- Offen: Native-Mod-Reichweite (N-KO-016).
