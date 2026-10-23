# NPCs 01: Quellenlage und Methodik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sagt, **woher** die NPC- und Living-World-Dokumentation stammt, welche Schlüssel sie benutzt, welche Regeln für Statuszeichen gelten und wo die Quellen enden. Es erfindet keine Spielerentscheidungen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Das Register steht in [Modul 26](26-Entscheidungsregister.md), Widersprüche und offene Fragen in [Modul 27](27-Konflikte-und-offene-Fragen.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Zitierschlüssel](#2-zitierschlüssel)
3. [Statusregeln](#3-statusregeln)
4. [Drei Achsen: Persistenz, Simulation, Datentiefe](#4-drei-achsen-persistenz-simulation-datentiefe)
5. [Abgrenzung zu den Nachbarordnern](#5-abgrenzung-zu-den-nachbarordnern)
6. [Master-Konsultation nach CLAUDE.md §11](#6-master-konsultation-nach-claudemd-11)
7. [Grenzen dieser Auswertung](#7-grenzen-dieser-auswertung)

---

## 1. Befund in Kürze

1. **Die Richtung ist breit und verbindlich, das Datenmodell ist offen.** Zwei Quellen tragen die Grundlage: die Entwickleraussage vom 22.10.2026 (`PE-N1022`, acht Anforderungen A–H, [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §1) und die Planungsrunde „Block 8 – NPC-Leben, soziale Simulation, Kriminalität & dynamische Welt“ (`23#1056A`, beantwortet in `23#1057E`, zusammengefasst in `23#1058A`; 125 Punkte). Beide legen fest, **was** die Welt können soll, nicht **wie** sie es technisch löst.
2. **Der Master kennt die Begriffe ‚Nemesis‘ und ‚Living World‘ nicht**, wohl aber die Bausteine: wichtige NPCs mit Gedächtnis, intern geführte Beziehungswerte, Persistenzstufen, Monster mit individuellem Gedächtnis, Hunter mit eigener Handlungsfreiheit, Fraktionen. Das Proposal 0010 §2 meldete für die Master-Tiers 1 und 2 keine Treffer zu ‚nemesis‘, ‚rivalry‘, ‚grudge‘, ‚living world‘ (Suche vom 22.10.2026). Diese Auswertung bestätigt das für alle vier Master-Tiers (Suche am 23.10.2026; einzige verwandte Tier-1-Stelle: ‚rival organizations‘ als Wissensträger, Abschnitt 6 F8) und **ergänzt** einen Befund aus den Gesprächen: ‚Rivalität‘ als Beziehungsform kommt dort mehrfach vor (`19#4E`: Hunter-NPCs können „Rivalen werden“; `23#117A`; `23#164A`; `23#467A`; `23#676A`). Eine eigene ‚Nemesis‘-Mechanik hat der Entwickler vor dem 22.10.2026 nicht beschrieben. Siehe [Modul 11](11-Nemesis-Modell-und-Datensatz.md) §1.
3. **Der Hauptteil der Detailentscheidungen steht in Gesprächen, nicht im Master.** Das gilt für Block 8 (`23#1056A`–`23#1058A`), die Runde 9B/9C (Gedächtnis, Beziehungsverlauf, Simulationsstufen, Save-Größe; `35#268A`–`35#280A`), die Fraktionsrunde (`23#676A`–`23#680A`), Block 10 (Beziehungen, Begleiter, Hund; `23#1088A`–`23#1090A`) und das Quiz zur Monstergesellschaft (`48#138A`–`48#148A`). Der Master fasst davon nur `M-PD` zusammen.
4. **Das Datenmodell ist durchgehend Entwurf.** Die Quellen nennen Dimensionen (Vertrauen, Angst, Respekt, Verdacht), Gedächtnisarten (kurz, Ereignis, Wissen, Beziehung), Stufen (A–D) und Beispiele (`23#51A`), legen aber **keine Felder, Formeln, Schwellen oder Zahlen** fest. Diese Dokumentation führt Zustandsmodelle, Übergangsregeln und Prüfregeln als **Entwurf (💡)** neben den Quellenaussagen.
5. **Es gibt Spannungen zwischen Quellen**, die nicht entschieden sind: zum Beispiel wer einen wichtigen NPC töten darf ([Modul 27](27-Konflikte-und-offene-Fragen.md) C-N1), welches Save-Größenziel gilt (C-N2) und wie die Weltzeit außerhalb des Spielens läuft (C-N3).
6. **Die Serie und andere Spiele sind Vergleich, keine Vorlage.** Das Proposal 0010 nennt eine RDR2-Modifikation ausdrücklich nur als Gedankenvergleich; die Seite wurde nicht geöffnet und wird hier nicht ausgewertet. Aus RDR2 selbst liegt eine Auswahl des Assistenten vor (`23#603A`), die **nicht** als Entscheidung geführt wird (Abschnitt 3).

---

## 2. Zitierschlüssel

Alle Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation bleibt ohne Zugriff darauf lesbar; die Schlüssel dienen der Nachprüfbarkeit.

### 2.1 Chats

Alle Chats liegen unter `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/`. Schlüssel `<Chat>#<Nachricht><Rolle>`; `E` = Rolle „user“ (Entwickler), `A` = Rolle „assistant“. Dieselbe Schreibweise wie in den Ordnern [Ermittlung](../Ermittlung/01-Quellenlage-und-Methodik.md#2-zitierschlüssel), [Kreaturen](../Kreaturen/01-Quellenlage-und-Methodik.md#2-zitierschlüssel) und [Welt](../World/01-Quellenlage-und-Methodik.md#2-zitierschlüssel).

| Schlüssel | Chat | Zeitraum | Inhalt für NPCs |
|---|---|---|---|
| `23#` | SPN Full Master ZIP (1.109 Nachrichten) | 18.08.–05.09.2026 | Gedächtnis- und Weltzustandsmodell (`47A`–`52E`), Companion-Beziehung (`117A`), Fraktions- und Mitgliedschaftssystem (`676A`–`681E`), Auswahl aus RDR2 (`603A`–`606A`), Block 8 (`1056A`–`1058A`), Block 10 (`1088A`–`1090A`), Ergänzungsliste (`1066A`), Dialog (`313A`) |
| `35#` | Projektwissen nutzen (951 Nachrichten) | 06.09.–04.10.2026 | Runde 9B/9C: Gedächtnisstufen, Beziehungsverlauf, Tod, Zeitreise, Simulationsstufen, Save-Größe (`262A`–`280A`); NPC-Leben bei Aussehen (`785E`/`786A`) |
| `48#` | Planung ZIP Downloads (297 Nachrichten) | 05.10.–13.10.2026 | Quiz: Monster-KI und -Gedächtnis (`137E`–`140A`), Monstergesellschaft, Territorien, Gesellschaftsreaktion, öffentliches Wissen (`141E`–`148A`) |
| `19#` | Frühe SPN-Projektdatei | 12.08.2026 | Themenliste des ursprünglichen Konzepts (`3A`, `4E`) |

**Nicht als Quelle verwendet:** Chat 18 (Minecraft-/Voxelprojekt, ausdrücklich getrennt vom Spiel, `19#1A`) und die Roman-Kapitel (Schreibprojekt). Die Adult-Runden sind für allgemeine NPC-Fragen nur dort einbezogen, wo sie **allgemeine** Systeme betreffen (Gedächtnis, Alterung, Tod, Skalierung); ihre sexuellen und romantischen Inhalte stehen im Ordner [Adult](../Adult-System.md) und werden hier nicht dupliziert.

### 2.2 Entscheidungs- und Planungsdokumente

| Schlüssel | Datei (relativ zum kumulativen Master) |
|---|---|
| `M-PD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` (Stand 2026-08-31). Benutzte Abschnitte: ‚NPCs and AI‘, ‚World state and persistence‘, ‚Multiple-opponent combat, improvised actions and surrender‘, ‚Companion autonomy, planning and field commands‘, ‚Organic world-event / case emergence‘, ‚Peaceful supernatural beings and independent hunter agency‘, ‚Long-term story threats‘, ‚Dialogue / conversation architecture‘, ‚Dialogue volume / scalability‘, ‚Player-created protagonist, Hunter crew and HQ‘, ‚Acts, chapters and controlled story reactivity‘, ‚Smartphone / social media / dynamic world events‘, ‚Cash / law / evidence‘ |
| `M-OD` | `…/docs/reference/OPEN_DECISIONS.md` (Stand 2026-08-31); NPC-relevante offene Punkte: Crew-Besetzung, NPC-Kommunikationsstil und Beziehungsfolgen |
| `M-B22` | `…/docs/planning-backlog/22-companions-planning-orders.md` (Planungsinventar, nur Themenliste) |
| `M-B34` | `…/docs/planning-backlog/34-hunter-crew-hq-acts-missions.md` (Planungsinventar, nur Themenliste) |
| `M-P05` | `…/docs/prompts/05-npc-ai-relations/` (35 Entwurfs-Prompts, alle `DRAFT ONLY`, plus `TRACK_CONTEXT.md`); nur als **Themeninventar** benutzt, nie als Anforderung |
| `M-MOD` | `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md` (Status CONFIRMED / CANONICAL: anbieterunabhängig, `.spnmod`, lokaler Standardort, Profile und Spielstand-Aktivierung) |
| `M-MOR` | `01_CURRENT_CANONICAL_STATE/MODDING/MODDING_ARCHITECTURE_V1_RECONSTRUCTED_2026-09-09.md` (rekonstruierter bestätigter Stand: fehlende kritische Mods blockieren das Laden, Expertenübersteuerung nach Backup, Deaktivierung pro Spielstand) |
| `M-B09` | `01_CURRENT_CANONICAL_STATE/BLOCK_09_STATUS.md` (akzeptierte Pfeiler: seltenes Hunter-Netz, mehrere Organisationen, intelligente übernatürliche NPCs, persistente Monsterökologie) |
| `M-PO` | `99_DAILY_DELTAS/2026-10-14/SPN_DAILY_DELTA_2026-10-14/docs/design/POPULATION_AND_SPATIAL_SCENARIOS.md` (Punkt 5: Simulationsstufen und Menschenmengen-Budgets unabhängig von der Lore-Bevölkerung; **Vorschlag, review-only**) |
| `M-AS` | `99_DAILY_DELTAS/2026-10-16/SPN_DAILY_DELTA_2026-10-16/docs/specs/AI_SIMULATION_BOUNDARIES.md` (Kandidatenarchitektur 6: Detailverhalten in Spielernähe, begrenzte Fernaktualisierung; **Vorschlag, nicht Kanon**) |
| `M-OQ16` | `99_DAILY_DELTAS/2026-10-16/SPN_DAILY_DELTA_2026-10-16/docs/open_questions/OPEN_QUESTIONS_2026-10-16.md` (Punkt 9: ‚AI/NPC LOD, save schema, deterministic offscreen simulation, tests and performance budgets‘ als offen) |
| `PE-N1022` | Entwickleraussage vom 22.10.2026 zu **Living World, NPC-Gedächtnis und Nemesis/Rivalität**: acht verbindliche Produktanforderungen A–H, festgehalten in `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` §1; Zusammenfassung in [Entscheidungsstatus](../Entscheidungsstatus.md) und [Gameplay-Systeme](../Gameplay-Systeme.md) §8. **Nicht im Master.** Die übrigen Abschnitte des Proposals (§3–§11) sind **Ingenieurvorschlag**, nicht Entscheidung |

Die Backlog-Dateien `M-B22` und `M-B34` und die Prompts `M-P05` tragen selbst den Status ‚PLANNING INVENTORY‘ beziehungsweise ‚DRAFT ONLY‘: Sie nennen **zu planende Themen**, keine Entscheidungen.

### 2.3 Eigene Dokumente als Querverweis

| Dokument | Rolle |
|---|---|
| [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) | Produktentscheidung (§1) und Architekturvorschlag (§3–§11) |
| [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) | Zeugen, Erinnerung, Verzerrung, Gerüchtekette (Entwurf) |
| [Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) | Ruf als Menge von Überzeugungen |
| [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) | Weltzustand (`WorldFacts`) und seine Grenzen |
| [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md), [10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md), [11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) | Monstergesellschaft, Monstergedächtnis, Hunterwelt |
| [Welt 12](../World/12-Gesellschaft-Wirtschaft-und-Ressourcen.md), [16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) | Gesellschaft, NPC-Verkehr |
| [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13 | Persistenzstufen A–D, NPC-Leben (Überblick) |
| [Gameplay-Systeme](../Gameplay-Systeme.md) §8 | NPCs, Gedächtnis, Beziehungen (Überblick) |

### 2.4 Registerkennungen

Entscheidungsregister `N-<GRUPPE>-nnn` (Modul 26), Konflikte `C-N1…` und unklare Antworten `U-N1…` (Modul 27), offene Fragen `Q-N01…` (Modul 27), Tests `T-…` (Modul 29, Gruppen siehe dort). Das Präfix `N` meint **NPCs**; es kollidiert nicht mit `K-…` (Kreaturen), `E-…` (Ermittlung) und `W-…` (Welt).

---

## 3. Statusregeln

| Zeichen | Bedeutung | Vergabe |
|---|---|---|
| 📋 | bestätigt | vom Entwickler festgelegt (Antwort „ja“ auf eine konkrete Frage, eigene Aussage) oder im Master als akzeptiert geführt |
| 🧭 | vorläufig | Arbeitsstand, Szenario, Planungsbereich; im Master als ‚not locked‘ oder ‚planning favorite‘ geführt |
| 💡 | Vorschlag | Ausformulierung oder Beispiel des Assistenten oder dieser Dokumentation, **nicht** beschlossen |
| 🔭 | Vision | langfristig, ohne Spezifikation |
| ❓ | offen | gestellt, nicht beantwortet, oder im Master als offen geführt |
| 🕰️ | ersetzt | bleibt als Geschichte stehen |
| ⚠️ | widersprüchlich | zwei Quellen widersprechen sich, keine Ersetzung belegt |

**Regeln.**

1. **Antworten gelten nur für den Inhalt der Frage.** Antwortete der Entwickler „ja“ auf eine nummerierte Frage, übernimmt dieser Ordner die Aussage der Frage als 📋 **nur**, soweit sie keinen erkennbaren Zusatz des Assistenten enthält. Fragen mit Vorschlagscharakter (‚Ich würde … verwenden. Passt?‘) werden als 📋 **Richtung** geführt; die Ausformulierung bleibt 💡.
2. **Beispiele des Assistenten** (Datensatzskizzen wie die Gedächtnisliste in `23#51A`, Werte wie ‚Trust 72/100‘) sind 💡, auch wenn der Entwickler das Konzept bestätigte. Der Text des Assistenten sagt dazu selbst, dass es sich um Beispiele handelt.
3. **Nummernversatz.** Die Antwort `23#1057E` ist eine Liste „1. … 125. …“ und deckt sich eins zu eins mit den Fragen in `23#1056A`. Die Antwort `35#279E` nennt Bereiche („1-4 ja“, „12-35 ja“) zu den Fragen in `35#278A`.
4. **Auswahlen des Assistenten** (`23#603A`: Welche RDR2-Systeme übernehmen? `23#606A`: Welche davon sind neu?) sind 💡. Der Entwickler antwortete mit der Bitte, nur noch Neues und genauer zu nennen (`23#604E`), nicht mit einer Zustimmung zu einzelnen Punkten.
5. **Quiz-Antworten in Chat 48** haben die Form ‚Supernatural <Thema>: <Optionstext>‘ und sind Knopfdruck-Antworten auf Optionen A–E. Gewählte Optionen sind 📋; der erklärende Text des Assistenten danach ist Auslegung (💡), soweit er über die Option hinausgeht.
6. **Kein Auffüllen.** Wo eine Quelle nichts sagt, steht ‚keine Quelle‘, und ein Entwurf (💡) nur, wenn er einen Zustand, eine Übergangsregel oder eine Prüfregel für die Umsetzung liefert.
7. **Sammelantwort im Block 9C.** Die Antwort „12-35 ja“ (`35#279E`) bündelt 24 verschiedene Fragen zu Simulationsebenen, Hintergrundsimulation, Relationship-Skalierung und Speicherung. Die [Welt-Dokumentation](../World/20-Entscheidungsregister.md) stützt darauf keine Einzelentscheidung (`W-TV-010`). Dieser Ordner führt die darauf beruhenden Aussagen als 📋 **Richtung**: Das Prinzip der Frage ist bestätigt, **nie** Stufennamen, Schwellen, Zahlen oder Formeln. Die einzeln beantworteten Fragen derselben Antwort („5 ja aber halt nicht der jetzige ort nicht der ganze weg und so weiter“, „11 die welt läuft nur wenn man gerade spielt“, „36 ja solange der npc sich an alles einzeln erinnern kann“, Punkt 40) werden einzeln geführt. Alle anderen Bereichsantworten (zum Beispiel „55-60 ja“ oder „1-50 passt alles“) werden wie in den Nachbarordnern als Antwort auf die einzelnen Fragen geführt. Die Abweichung gegenüber der Welt-Dokumentation (dort 🧭/💡 für die Simulationsebenen) steht in [Modul 27](27-Konflikte-und-offene-Fragen.md) C-N5.
8. **Wörtliche Zitate** in „…“ stammen aus Gesprächen oder dem Master und werden mit `Tools/world-audit/check_docs.py` gegen die Quelltexte geprüft. Zitate aus Dokumenten dieses Repositorys stehen in ‚…‘ und werden nicht automatisch geprüft.

---

## 4. Drei Achsen: Persistenz, Simulation, Datentiefe

Die Quellen benutzen drei Wörter für verwandte, aber verschiedene Ideen. Diese Dokumentation hält sie getrennt (💡 Ordnungsvorschlag; die Einzelteile sind in den Quellen belegt):

| Achse | Frage | Stufen in den Quellen | Quelle |
|---|---|---|---|
| **Persistenzstufe** | Wie wichtig ist der NPC für die Welt, wie viel muss dauerhaft erhalten bleiben? | A Story/Hunter/Nebenfall/enge Beziehungen, B Händler/Nachbarn/Arbeitgeber/wiederkehrende Kontakte, C normale Einwohner, D Menschenmengen | `23#1056A` Fragen 17, 121; Antwort „ja“ in `23#1057E`; `23#1058A` |
| **Simulationsstufe** | Wie genau wird der NPC gerade gerechnet? | im Beispiel `Tier 0` Held/direkt beim Spieler, `Tier 1` lokal aktiv, `Tier 2` regionale Hintergrundsimulation, `Tier 3` globale Dormant-Simulation; und die Zustände `AT_WORK`, `HOME`, `TRAVEL` | `35#278A` Fragen 2–4; „1-4 ja“ in `35#279E`; `23#1056A` Frage 122 |
| **Datensatztiefe** | Wie viel ist über diesen NPC gespeichert? | Minimal Record, Known NPC Record, Detailed Persistent Profile, Full Persistent Profile | `35#262A` (Zu 39–40, Vorschlag des Assistenten zu einer Ergänzung des Entwicklers) |

Ein NPC der Persistenzstufe C kann zeitweise in Simulationsstufe 0 laufen (der Spieler spricht mit ihm), und ein NPC der Stufe A kann in Simulationsstufe 3 ruhen (weit weg). Die Datensatztiefe wächst durch **Begegnungen**: Wer dem Spieler relevant begegnet, bekommt dauerhaft einen tieferen Datensatz (`35#262A`; `35#278A` Frage 8). Die Verknüpfung der drei Achsen steht in [Modul 03](03-Identitaet-Persistenzstufen-und-Datensatztiefe.md) §6; sie ist ein Entwurf.

Weitere Verwendungen von ‚Tier‘ und A–D in den Quellen (Tier S der visuellen Figurenqualität, A–D der Gebäudeklassen) bezeichnen andere Achsen; sie stehen in [Modul 27](27-Konflikte-und-offene-Fragen.md) §3. Der Master kennt ausschließlich die Persistenzstufen A–D (`M-PD`/Spielwelt §13). Die Simulationsstufen sind im Master nur als **Vorschlag ohne Budgets** geführt (`M-PO`, `M-AS`, `M-OQ16`).

---

## 5. Abgrenzung zu den Nachbarordnern

| Thema | Wo geführt | Was dieser Ordner dazu tut |
|---|---|---|
| Zeugenaussage, Verhör, Alibi, Gerüchtekette (Ermittlungssicht) | [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) | zeigt die **Gegenseite**: wie der NPC weiß, vergisst und weitergibt ([Modul 05](05-Gedaechtnis-Erinnerung-und-Vergessen.md), [06](06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md), [07](07-Informationsfluss-Geruechte-und-Kommunikation.md)) |
| Ruf, Identität, Formwandler | [Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) | Ruf ist dort eine **Überzeugungsmenge von Beobachtern**; hier die Beobachter als Personen ([Modul 08](08-Beziehungen-Modell-und-Wirkung.md) §8) |
| Monsterstruktur, Monstergedächtnis, Artregeln | [Kreaturen 03](../Kreaturen/03-Klassifikation-Herkunft-und-Individuen.md), [09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md), [10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) | **Artregeln bleiben dort**; hier das gemeinsame Nemesis- und Fraktionsmodell für Menschen und Wesen ([Modul 11](11-Nemesis-Modell-und-Datensatz.md), [17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md)) |
| Hunterwelt, Organisationen, Kulte | [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md) | Verweis; hier die Fraktionsmechanik und der Spieler als Mitglied ([Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md), [18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md)) |
| Polizei, Gerichte, Haft, Kriminalität | [Ermittlung 06](../Ermittlung/06-Kriminalitaet-Polizei-und-Justiz.md), [14](../Ermittlung/14-Polizei-Behoerden-und-Fahndung.md), [15](../Ermittlung/15-Gerichte-und-Verfahren.md), [16](../Ermittlung/16-Haft-und-Strafvollzug.md), [25](../Ermittlung/25-Kriminalitaet-in-der-Welt.md) | Verweis; hier die NPC-Seite (Täter, Opfer, Zeugen, Beamte als Personen) |
| Notfälle und Weltereignisse | [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md) | Verweis; hier die Reaktion einzelner NPCs ([Modul 15](15-Reaktion-auf-Weltereignisse-und-Uebernatuerliches.md)) |
| NPC-Verkehr | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) | Verweis; Fahrverhalten ist dort geführt |
| Persistenztechnik (`WorldFacts`) | [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md), [Kreaturen 17](../Kreaturen/17-Persistenz-Daten-und-Modding.md) | hier die **NPC-spezifische** Speicherfrage ([Modul 21](21-Weltpersistenz-und-Speicherentwurf.md)) |
| Romantik, Sexualität, Präferenzen | [Adult-System](../Adult-System.md), Ordner `Docs/Game/Adult/` | **nicht** hier; nur das gemeinsame Beziehungsgerüst |
| Aussehen, Tattoos | [Decision 0005](../../Decisions/0005-character-appearance-foundation.md), Adult 07/25 | nur der Zeitverlauf (Altern, Veränderung) in [Modul 14](14-Lebensereignisse-Alterung-Umzug-Beruf-und-Tod.md) |
| Dialogsystem, Stimmen | `M-PD` ‚Dialogue …‘ | nur die Schnittstelle zum Beziehungs- und Gedächtniszustand ([Modul 20](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md)) |

Die Überblicksdokumente (`Spielvision`, `Gameplay-Systeme`, `MasterIndex`, `README`) bleiben unverändert. Notwendige Ergänzungen stehen im Abschlussbericht und in [Modul 28](28-Abdeckungsprotokoll.md) §7.

---

## 6. Master-Konsultation nach CLAUDE.md §11

Die Suche erfolgte am 23.10.2026 mit `Tools/master-search.sh` (Tier 1 und 2; Tier 3 mit `--tier 3`).

| Begriffe | Tier 1 | Tier 2 | Tier 3 |
|---|---|---|---|
| ‚nemesis‘, ‚rivalry‘, ‚grudge‘, ‚living world‘ | kein Treffer | kein Treffer | ‚nemesis‘ ohne Treffer; ‚relationship‘, ‚NPC memory‘, ‚faction‘, ‚schedule‘ viele Treffer (siehe unten) |
| ‚npc memory‘, ‚faction‘ | `CANONICAL_ADDENDUM_2026-09-05.md` (eine Zeile zu Beziehungen, siehe unten) | keine Treffer zu ‚npc memory‘ | `PROJECT_DECISIONS.md`, `DECISION_CHANGELOG_2026-08-31.md`, `PROJECT_STATE_2026-08-31_COMPACT.md`, `PROMPT_ORDER.md`, Prompts `05-npc-ai-relations/` |
| ‚NPC‘ (zusammen mit ‚simulation LOD‘, ‚offscreen‘) | `ADDENDA_2026/2026-10-18/03_ENGINEERING_PROPOSALS.md`, `BLOCK_09_STATUS.md`, `CANONICAL_ADDENDUM_2026-09-05.md` | `POPULATION_AND_SPATIAL_SCENARIOS.md`, `AI_SIMULATION_BOUNDARIES.md`, `AI_AUTONOMY_BOUNDARIES.md`, `OPEN_QUESTIONS_2026-10-16.md` und weitere Randtreffer | – |

Gelesen und klassifiziert:

| # | Fund | Status im Quelltext | Wirkung in diesem Ordner |
|---|---|---|---|
| F1 | `CANONICAL_ADDENDUM_2026-09-05.md`: Romantik und Sexualität können mit erwachsenen NPCs breit verfügbar sein, wichtige Figuren erhalten handgeschriebene Tiefe, generische NPCs nutzen systemische Beziehungsinhalte | akzeptiert als Planungslinie | [Modul 20](20-Dialog-und-Sichtbarkeit-der-Beziehungen.md) §3: Dialogtiefe nach Stufe |
| F2 | `BLOCK_09_STATUS.md`: Pfeiler ‚multiple organizations‘, ‚intelligent supernatural NPCs‘, ‚individual death permanence‘, ‚territories/nests/migration/reproduction‘ | akzeptiert (Block 9) | [Modul 17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md), [18](18-Behoerden-Hunter-Organisationen-und-uebernatuerliche-Wesen.md) |
| F3 | `03_ENGINEERING_PROPOSALS.md` (2026-10-18): Kein finales GIS aus einem Flächenbereich; Bahn-Detail als Prüfmaterial | ‚review-required‘ | keine Wirkung; Weltgeometrie bleibt außerhalb |
| F4 | `M-PO` Punkt 5 und `M-AS` Kandidat 6: Simulationsstufen und Budgets unabhängig von der Lore-Bevölkerung | Vorschlag, nicht Kanon | [Modul 22](22-Simulationsstufen-Skalierung-und-Performance.md) §1 |
| F5 | `M-AS`, `AI_AUTONOMY_BOUNDARIES.md`, `AI_RECOGNITION_AND_CONFLICTS.md`: Rahmen für Roboter und fortgeschrittene KI als Bewohner der Untergrundzivilisation | Entwurf bzw. nur U32–U35 bestätigt | **nicht** übernommen; KI-Bewohner sind kein Teil des NPC-Kerns ([Modul 27](27-Konflikte-und-offene-Fragen.md) Q-N25) |
| F6 | `M-OQ16` Punkt 9: NPC-LOD, Save-Schema, deterministische Fernsimulation, Tests, Budgets | offen | [Modul 27](27-Konflikte-und-offene-Fragen.md) |
| F7 | `M-OD`: ‚Exact NPC communication-style data model and relationship consequences‘ und ‚Exact Hunter crew roster, biographies, deaths, relationships and companion specialties‘ | offen | [Modul 27](27-Konflikte-und-offene-Fragen.md) |
| F8 | `CANONICAL_ADDENDUM_2026-09-05.md` (Zeile 32): ‚rival organizations may hold different fragments of knowledge‘, also **rivalisierende Organisationen** als Wissensträger; die einzige Tier-1-Stelle mit ‚rival‘ im Sinn einer Beziehung. Keine Mechanik, kein Nemesis | akzeptiert als Planungslinie (Wissensverteilung) | [Modul 17](17-Fraktionen-Territorien-Konflikte-und-Machtverschiebung.md) §1 |

Nichts gefunden zu: einer Liste benannter NPCs, benannter Fraktionen oder benannter Antagonisten (außer Hunter-HQ-Begriffen, die hier **nicht** ausgeführt werden). Das Ergebnis ‚keine Fraktionsliste im Master‘ steht in [Modul 28](28-Abdeckungsprotokoll.md).

---

## 7. Grenzen dieser Auswertung

- **Gelesen** wurden vollständig: Proposal 0010; `Gameplay-Systeme` §8; `Spielwelt-und-Untergrund` §13; `M-PD` (die in Abschnitt 2.2 genannten Abschnitte); Chat 23: `47A`–`52E`, `603A`–`606A`, `676A`–`681E`, `1056A`–`1058A`, `1064A`–`1066A`, `1088A`–`1090A`, `117A`/`118E` (nur Punkt 17); Chat 35: `262A`–`282A`, `784A`–`786A`; Chat 48: `137E`–`148A`; die oben genannten Deltas und Statusdateien; die Titel der 35 Prompts in `05-npc-ai-relations/` und die Dateien `TRACK_CONTEXT.md`, `22-…`, `34-…`.
- **Nur stichwortweise geprüft:** die übrigen Nachrichten der Chats 23, 35, 48 und Chat 19. Details: [Modul 28](28-Abdeckungsprotokoll.md).
- **Nicht gelesen:** die Roman-Kapitel, Master Tier 3 und 4 außer den genannten Dateien, die Seite der RDR2-Modifikation (Proposal 0010 §1 sagt dasselbe).
- Die Beschreibung der Hunter-Crew im Master ist **nicht** Teil dieser Auswertung, weil Besetzung, Biografien und Relationen offen sind (`M-OD`); der Ordner führt nur das **System**, nie Figuren.
- Es gibt **keine Implementierung** zu NPC-Verhalten, Gedächtnis, Beziehungen, Fraktionen oder Nemesis. Das einzige gebaute Fundament, das NPCs betrifft, ist `WorldFacts` (siehe [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md)) und der Aussehensgenerator ([Decision 0005](../../Decisions/0005-character-appearance-foundation.md)).
