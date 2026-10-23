# Kreaturen 01: Quellenlage und Methodik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sagt, **woher** die Kreaturen-Dokumentation stammt, welche Schlüssel sie benutzt, welche Regeln für Statuszeichen gelten und wo die Quellen enden. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Das Register steht in [Modul 20](20-Entscheidungsregister.md), Widersprüche und offene Fragen in [Modul 21](21-Konflikte-und-offene-Fragen.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Zitierschlüssel](#2-zitierschlüssel)
3. [Statusregeln](#3-statusregeln)
4. [Zwei Regeln des Entwicklers zum Umgang mit der Serie](#4-zwei-regeln-des-entwicklers-zum-umgang-mit-der-serie)
5. [Nachbardokumentation](#5-nachbardokumentation)
6. [Grenzen dieser Auswertung](#6-grenzen-dieser-auswertung)

---

## 1. Befund in Kürze

1. **Das Kreaturen-Framework ist als Richtung breit und tief festgelegt, aber ohne einzige konkrete Art.** Der Master führt eine ausführliche Liste von Prinzipien (`M-PD`, Abschnitt ‚Creature / supernatural framework — expanded‘), die auf einem Gespräch vom 24.08.2026 beruht (`23#293A`, `23#294E`, `23#307A`, `23#308E`). Eine feste Artenliste gibt es nicht: ‚Final base-game monster roster/count‘ steht als offen (`M-CA0905` §7, `M-B09`).
2. **Die Mechanik steht als offen.** `M-OD` führt als offen: ‚Exact monster taxonomy, anatomies, regeneration, weaknesses, resistances and harvesting rules‘ und ‚Exact monster hypothesis/confirmation thresholds and false-classification UX‘. Dieser Ordner führt deshalb **Zustandsmodelle, Datenfelder und Prüfregeln** als Entwurf (💡) neben den Quellenaussagen.
3. **Der Gesprächsverlauf ist Quelle für Details, die der Master nur zusammenfasst.** Block 9 (04.09.2026, `23#1074A`/`23#1075E`) hat 156 Entscheidungspunkte, der Master kennt davon die Auswahl in `M-B09` und `M-PD`. Der Entwickler antwortete großteils mit Bereichen („45-54 passt“); die inhaltliche Aussage steht in der **Frage** des Assistenten, die Zustimmung in der Antwort.
4. **Chat 48 (Entscheidungs-Quiz, 10.–11.10.2026)** liefert Entscheidungen zu Monsterwissen, Forschung, Gefangennahme, Fallen, Monster-KI, Gedächtnis, Gesellschaft und Territorien, die **nirgends im Master stehen** (Tier 1 und 2 ohne Treffer). Sie sind dokumentiert, aber als Quelle ‚nur im Gespräch‘ markiert.
5. **Reiche und Kosmologie** (Hölle, Fegefeuer, Himmel, Zeit, Zeitreise) sind bereits in der Welt-Dokumentation behandelt ([Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md)). Dieser Ordner führt sie nur, soweit sie **Kreaturen, Seelen, Geister und Tod** betreffen, und verweist sonst.
6. **Die Serie ist Ton- und Regelreferenz, nicht Textvorlage.** Der Entwickler hat beides gesagt (Abschnitt 4). Die Dokumentation vermeidet serienspezifische Namen als Entwurfsinhalt und trennt Mythologie, Genre-Konzept und serienspezifische Ausgestaltung (`IP_PROVENANCE`, Abschnitt 4.3).
7. **Spoilerregel.** Der Entwickler hat gebeten, ihm Serieninhalt „nicht direkt oder nur ohne spoiler“ zu nennen (`23#1091E`). Dieser Ordner enthält keine Handlungsdetails der Serie.

---

## 2. Zitierschlüssel

Alle Pfade sind relativ zu `Planning/Master/SPN_FULL_CUMULATIVE_MASTER_2026-09-27/` (schreibgeschützt, nicht im Git). Die Dokumentation bleibt ohne Zugriff darauf lesbar; die Schlüssel dienen der Nachprüfbarkeit.

### 2.1 Chats

Alle Chats liegen unter `99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE/`. Schlüssel `<Chat>#<Nachricht><Rolle>`; `E` = Rolle „user“ (Entwickler), `A` = Rolle „assistant“. Dieselbe Schreibweise wie in der [Ermittlungs-Dokumentation](../Ermittlung/01-Quellenlage-und-Methodik.md#2-zitierschlüssel) und der [Welt-Dokumentation](../World/01-Quellenlage-und-Methodik.md#2-zitierschlüssel).

| Schlüssel | Chat | Zeitraum | Inhalt für Kreaturen |
|---|---|---|---|
| `23#` | SPN Full Master ZIP (1.109 Nachrichten) | 18.08.–05.09.2026 | Monster-Framework (`293A`–`308E`), Kosmologie (`359A`–`360E`), Tod, Seele, Geister (`361A`–`372A`), Umgang mit der Serie (`375E`–`379E`), Jenseits (`458A`–`460A`), Fegefeuer und Leviathane (`465A`–`471A`), Block 9 (`1074A`–`1076A`), Realms (`1103E`–`1104A`) |
| `35#` | Projektwissen nutzen (951 Nachrichten) | 06.09.–04.10.2026 | übernatürliche Heilung (`26E`, `27A`), Höllenzeit (`41E`), Tattoo-/Körpersystem (nur Randbezug) |
| `48#` | Planung ZIP Downloads (297 Nachrichten) | 05.10.–13.10.2026 | **Quiz** vom 10.–11.10.2026: übernatürliche Verletzungen (`122A`–`124A`), Monsterwissen (`124A`–`126A`), Forschung (`127E`/`128A`), Gefangennahme (`129E`/`130A`), Einrichtungen und Fallen (`131E`–`136A`), Monster-KI und Gedächtnis (`137E`–`140A`), Monstergesellschaft und Territorien (`141E`–`144A`), Gesellschaftsreaktion (`145E`/`146A`), öffentliches Wissen (`147E`/`148A`), Ruf und Formwandler (`153E`/`154A`) |
| `30#` | Kartenerzeugung (Prompt-Dokumente) | 01.–30.09.2026 | nur Nachricht `245E`, Abschnitt 38 ‚Supernatural layer‘ (Prompt-Vorgabe an die Kartenerzeugung, Verfasser unklar, 🧭) |
| `19#` | Frühe SPN-Projektdatei | 12.08.2026 | Themenliste des ursprünglichen Konzepts (`4E`) |
| `50#` | Schreibprojekt (Roman) | 05.–10.10.2026 | nur eine Randaussage (`13E`), kein Spielentscheid |

**Nicht als Quelle verwendet:** Chat 18 (Minecraft-/Voxelprojekt, ausdrücklich getrennt vom Spiel, `19#1A`) und die Roman-Kapitel in den Chats 11, 12, 13, 15, 42, 50 (Schreibprojekt). Aussagen darin sind keine Spielentscheidungen.

### 2.2 Entscheidungs- und Planungsdokumente

| Schlüssel | Datei (relativ zum kumulativen Master) |
|---|---|
| `M-PD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/PROJECT_DECISIONS.md` (Stand 2026-08-31). Benutzte Abschnitte: ‚Story and cases‘, ‚Hunter Archive, collectibles and progress‘, ‚NPCs and AI‘, ‚World state and persistence‘, ‚Multiple-opponent combat…‘, ‚Stealth, perception and horror AI‘, ‚Genre and progression identity‘, ‚Creature / supernatural framework — expanded‘, ‚Organic world-event / case emergence‘, ‚Peaceful supernatural beings and independent hunter agency‘, ‚Hunter Archive epistemology‘, ‚Long-term story threats‘, ‚Wildlife / study / harvesting / monsters‘ |
| `M-OD` | `02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/OPEN_DECISIONS.md` (Stand 2026-08-31; Abschnitt ‚Wildlife / hunting / monster knowledge‘: Liste **offener** Punkte) |
| `M-B24` | `…/docs/planning-backlog/24-creature-supernatural-framework.md` (Planungsinventar, nur Themenliste) |
| `M-B37` | `…/docs/planning-backlog/37-wildlife-study-harvest-forensics.md` (Planungsinventar) |
| `M-CA0905` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-05.md` (§3 ‚Historical supernatural layer‘, §4 Hunde und Spurensuche, §5 Reiche, §7 ‚Still open‘) |
| `M-CA0906` | `01_CURRENT_CANONICAL_STATE/CANONICAL_ADDENDUM_2026-09-06.md` (Reiche, Bewegung, Persistenz, ‚Still open‘) |
| `M-CU0908` | `01_CURRENT_CANONICAL_STATE/CANONICAL_UPDATE_2026-09-08.md` (Reiche, Orte) |
| `M-B09` | `01_CURRENT_CANONICAL_STATE/BLOCK_09_STATUS.md` (akzeptierte Pfeiler und offene Punkte von Block 9) |
| `M-RS` | `01_CURRENT_CANONICAL_STATE/REALMS/STATUS_2026-09-10.md` (Stand der Reiche) |
| `M-PS0905` | `01_CURRENT_CANONICAL_STATE/PLANNING_STATUS_2026-09-05.md` |
| `PE-E1022` | Mitteilung ‚Kanon-Update: Metropolis, Höllenzeit und Hunter HQ‘ des Projektinhabers vom 22.10.2026; neue Primärentscheidung, nicht im Master (siehe [Welt 01](../World/01-Quellenlage-und-Methodik.md)) |
| `PE-N1022` | Entwickleraussage vom 22.10.2026 zu **Living World, NPC-Gedächtnis und Nemesis/Rivalität** (acht verbindliche Produktanforderungen A–H, `Docs/Proposals/0010-living-world-npc-memory-and-nemesis.md` §1; Zusammenfassung in [Entscheidungsstatus](../Entscheidungsstatus.md) und [Gameplay-Systeme](../Gameplay-Systeme.md) §8); **nicht im Master** (Suche nach ‚nemesis‘, ‚rivalry‘, ‚grudge‘, ‚living world‘ ohne Treffer, Proposal 0010 §2). Die übrigen Abschnitte des Proposals (§3–§11) sind **Ingenieurvorschlag**, nicht Entscheidung |

Die Backlog-Dateien `M-B24` und `M-B37` tragen selbst den Status ‚PLANNING INVENTORY‘: Sie nennen **zu planende Themen**, keine Entscheidungen.

### 2.3 Registerkennungen

Entscheidungsregister `K-<GRUPPE>-nnn` (Modul 20), Konflikte `C-K1…` und unklare Antworten `U-K1…` (Modul 21), offene Fragen `Q-K01…` (Modul 21), Tests `T-A…` (Modul 23, Gruppen siehe dort). Das Präfix `K` meint **Kreaturen**, nicht die Konflikte `K-E…` der Ermittlungs-Dokumentation.

---

## 3. Statusregeln

| Zeichen | Bedeutung | Vergabe |
|---|---|---|
| 📋 | bestätigt | vom Entwickler festgelegt (Antwort „ja“ auf eine konkrete Frage, eigene Aussage) oder im Master als akzeptiert geführt |
| 🧭 | vorläufig | Arbeitsstand, Szenario, Planungsbereich; im Master als ‚not locked‘ oder ‚planning favorite‘ geführt |
| 💡 | Vorschlag | Ausformulierung oder Beispiel des Assistenten, **nicht** beschlossen; Entwurfsabschnitte dieses Ordners |
| 🔭 | Vision | langfristig, ohne Spezifikation |
| ❓ | offen | gestellt, nicht beantwortet, oder im Master als offen geführt |
| 🕰️ | ersetzt | bleibt als Geschichte stehen |
| ⚠️ | widersprüchlich | zwei Quellen widersprechen sich, keine Ersetzung belegt |

**Regeln.**

1. Eine Zustimmung gilt nur für das, **was die Frage sagt**. Antwortete der Entwickler „ja“ auf eine Liste, übernimmt dieser Ordner den Inhalt der Frage als 📋 **nur**, soweit er nicht erkennbar Teil eines Vorschlags mit eigenem Zusatz war; sonst 💡 mit Hinweis.
2. **Nummernversatz.** Wo die Zählung einer Antwort nicht zur Zählung der Frage passt (zum Beispiel `23#362E` zu `23#361A`, `23#466E` zu `23#465A`), wird die Antwort nur dort zugeordnet, wo der Wortlaut sie eindeutig macht. Die übrigen Fälle stehen in Modul 21 (unklare Antworten).
3. **Beispiele und Tabellen** des Assistenten (zum Beispiel die Kreaturenbeispiele in `48#124A`) sind 💡, auch wenn der Entwickler das Gesamtkonzept bestätigte; der Text sagt dazu selbst: „Das sind mögliche Mechaniken. Welche Effekte tatsächlich zu welcher Kreatur gehören, legen wir anhand unserer endgültigen Monster-Mythologie fest.“
4. **Quiz-Antworten in Chat 48** haben die Form ‚Supernatural <Thema>: <Optionstext oder Freitext>‘ und sind Knopfdruck-Antworten auf Optionen A–E. Gewählte Optionen sind 📋; der erklärende Text des Assistenten danach ist Auslegung (💡), soweit er über die gewählte Option hinausgeht.
5. **Kein Auffüllen.** Wo eine Quelle nichts sagt, steht ‚keine Quelle‘ und ein Entwurf (💡) nur, wenn er eine Prüfregel oder einen Zustand für die Umsetzung liefert.

---

## 4. Zwei Regeln des Entwicklers zum Umgang mit der Serie

Der Entwickler hat dem Assistenten zwei Regeln gegeben, die zusammen gelten. Sie sind **keine Widersprüche**, sondern Rahmen und Schranke.

### 4.1 Rahmen: Serienlogik als Standard

| Aussage | Quelle |
|---|---|
| „viele sachen sollen von der serie übernommen werden, also wenn ich nicht alles genau weiß nimm es von der serie supernatural“ | `23#362E` |
| „ich sage dir schon das spiel wird in der supernatural welt spielen, wenn ich es dir nicht anders sage dann werden die dinge so sein wie dort“ | `23#371E` |
| Bei Geistern (Salz, Eisen, Bindung an Gegenstände, Verschlechterung über Zeit) richtet sich der Assistent „bewusst stärker an den Regeln aus Supernatural“ aus; der Entwickler bestätigt die Richtung | `23#369A`, `23#370A`, `23#371E` |

### 4.2 Schranke: eigene IP

| Aussage | Quelle |
|---|---|
| „ich meinte damit dass dämonen, engel, geister, leviathane existieren, die hauptcharaktere sind an sam und dean angehaucht aber eigenständig“ | `23#377E` |
| Auslegung des Assistenten: allgemeine Mythologie ist frei nutzbar, serienspezifische Figuren, Artefakte, Designs, Lore werden durch eigenständige Varianten ersetzt; Bestätigung: „ja so meine ich das“ | `23#378A`, `23#379E` |
| Die Organisation, in der „sam und deans opa“ Mitglied war, soll funktional vorkommen, aber mit eigenem Namen, eigener Geschichte, eigener Mythologie | `23#1075E` Punkt 5; `23#1076A` |
| Auch spätere Serienbereiche (genannt: Leviathane, Eve, Hallucifer, Staffel-8-Rückblenden) sollen **nicht stumpf 1:1** übernommen, sondern verbessert werden (Kontext: Schreibprojekt, Chat 50; Aussage des Entwicklers) | `50#13E` |
| Master: ‚original IP rather than copied characters, names, factions or plotlines‘ | `M-CA0905` §3 |

### 4.3 Folgerung für diese Dokumentation (💡)

Jede Kreatur und jeder Begriff bekommt im Bestiarium-Datensatz ein Herkunftsfeld `ip_provenance` mit den drei Werten, die der Assistent in `23#376A` vorschlug (💡, vom Entwickler nicht ausdrücklich bestätigt, aber in der Sache durch `23#379E` gedeckt):

| Wert | Beispiel | Behandlung |
|---|---|---|
| `folklore` | Geister, Dämonen, Besessenheit, Vampire, Werwölfe, Hexen, Schutzkreise, Salz | frei als Grundlage nutzbar |
| `genre` | Monsterjagd, Exorzismus, Recherche, Road-Trip-Duo | eigene Umsetzung |
| `series_specific` | konkrete Figuren, Artefakte, Symbole, Wesen, Storybögen der Serie | nur eigene Variante; ohne Lizenz nicht direkt übernommen |

Der Master nennt ein offenes Namensthema ausdrücklich: ‚original-IP names for Men-of-Letters-like institution and progenitor/first-of-species concepts‘ (`M-B09`, ‚Open‘). Die Dokumentation vergibt deshalb **keine Eigennamen** und benutzt Arbeitsbegriffe (‚wissenszentrierte Organisation‘, ‚Ursprungswesen‘).

---

## 5. Nachbardokumentation

| Thema | Wo geführt |
|---|---|
| Fallmodell, Hinweise, Beweislogik, Journal | [Ermittlung](../Ermittlung/README.md), vor allem [Modul 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md) und [Modul 10](../Ermittlung/10-Beweislogik-und-Wissensmodell.md) |
| Hunter Archive als Wissensgraph | [Ermittlung 04](../Ermittlung/04-Archiv-Journal-und-Hunter-Record.md) |
| Formwandler und Identitätsmissbrauch | [Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) |
| Geheimhaltung, öffentliches Wissen | [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md) |
| Reiche, Zeitreise, historische Weltzustände | [Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md) |
| Persistenter Weltzustand (technisch) | [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md), `Core/include/spn/core/WorldFacts.h` |
| Verletzung, Medizin, Tod | [Spielvision §14](../Spielvision.md#14-schwierigkeit-scheitern-und-tod), [Gameplay-Systeme](../Gameplay-Systeme.md) (Überblick); eine eigene Detaildokumentation fehlt |
| Stealth- und Horror-KI (Grundsystem) | [Gameplay-Systeme](../Gameplay-Systeme.md) (Überblick), `M-PD` ‚Stealth, perception and horror AI‘ |
| Überblick über Kreaturen | [Spielvision §9](../Spielvision.md#9-kreaturen-und-das-übernatürliche) |

Die Überblicksdokumente bleiben unverändert. Abweichungen stehen in [Modul 21](21-Konflikte-und-offene-Fragen.md).

---

## 6. Grenzen dieser Auswertung

- **Gelesen** wurden vollständig: Proposal 0010 (alle Abschnitte), die Zeile ‚Living World, NPC-Gedächtnis und Nemesis‘ in `Entscheidungsstatus`, [Gameplay-Systeme §8](../Gameplay-Systeme.md);  `M-PD` (alle genannten Abschnitte), `M-B09`, `M-RS`, `M-CA0905` (§1–§7), `M-CA0906` (Reiche, ‚Still open‘), `M-OD` (Abschnitt Wildlife / hunting / monster knowledge), `M-B24`, `M-B37`; Chat 23: `293A`–`298E`, `307A`/`308E`, `359A`–`362E`, `365A`–`372A`, `375E`–`379E`, `458A`–`460A`, `465A`–`471A`, `1074A`–`1076A`, `1091E`–`1096A` (nur Spoilerregel und Zeitreise-Randbezug), `1103E`/`1104A`, `253A`/`254E`; Chat 35: `26E`/`27A`, `41E`; Chat 48: `122A`–`158A`, `169E`–`171E`, `179E`.
- **Nur stichwortweise geprüft:** alle übrigen Nachrichten der Chats 23, 35, 48 und die Chats 6, 10, 17–19, 26, 30, 32, 38, 42, 50. Ergebnis: Treffer außerhalb der genannten Bereiche betreffen Roman-Kapitel, Kartenarbeit, Dateiarbeit oder das Minecraft-Projekt. Details: [Modul 22](22-Abdeckungsprotokoll.md).
- **Nicht gelesen:** die Roman-Kapitel (Chats 11, 12, 13, 15, 50), die Prompt-Entwürfe `docs/prompts/*` (nur Titel und Kontext), Master Tier 3 und 4 außer den genannten Dateien.
- Es gibt **keine Implementierung** zu Kreaturen, Bestiarium, Monster-KI, Ritualen, Fallen oder Containment. Das einzige gebaute Fundament, das Kreaturen betrifft, ist der persistente Weltzustand `WorldFacts` (siehe [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md)).
- **Referenzspiele und Serie** erscheinen als Vergleich. Sie sind keine Vorgaben (Abschnitt 4).
