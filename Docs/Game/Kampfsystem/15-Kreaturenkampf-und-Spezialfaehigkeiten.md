# Kampfsystem 15: Kreaturenkampf und Spezialfähigkeiten

> [!IMPORTANT]
> **Nichts davon ist implementiert, und es gibt keine beschlossene Artenliste.** Dieses Modul verbindet das Kampfsystem mit den Entscheidungen der [Kreaturen-Dokumentation](../Kreaturen/README.md). Es **verweist** auf deren Register (`K-…`), statt Kreaturenregeln neu zu entscheiden, und fasst zusammen, was daraus für den **Kampf** folgt. Kampfarchetypen sind Beispiele des Assistenten (💡), keine Arten. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Schwächen und Jagdmethoden: [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md); Fallen und Fang: [Modul 17](17-Fallen-Fesselung-und-Lebendfang.md); KI: [Modul 18](18-Gegner-KI-und-Kampftaktik.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Was ein Kampfprofil einer Art enthalten muss](#3-was-ein-kampfprofil-einer-art-enthalten-muss)
4. [Kampfarchetypen als Beispiele](#4-kampfarchetypen-als-beispiele)
5. [Existenzformen und Kampfregeln](#5-existenzformen-und-kampfregeln)
6. [Menschenregeln und Monsterregeln](#6-menschenregeln-und-monsterregeln)
7. [Gegenüberstellung offener Lösungsansätze](#7-gegenüberstellung-offener-lösungsansätze)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Monster dürfen die Kampfregeln brechen, Menschen nicht** (`23#85A` P13, `23#86E` P13): Dieselbe Waffe kann gegen ein Wesen Schaden machen, gegen ein anderes nichts und gegen ein drittes nur vertreiben (`23#85A` P2, `23#86E` P2 ‚ja das ist Pflicht‘).
2. **Das Menschensystem ist die Grundlage, nicht die Grenze:** ‚Human Close Combat System plus Creature-specific Rules‘, keine isolierten Systeme (`23#166A` P14, `23#167E` P14).
3. **Es gibt keine Monsterlevel und keine Aggro-Werte:** Gefahr entsteht aus Regeln, Wissen, Umgebung, Zahl, Verletzung; Antriebe statt Aggro (`K-GR-001`, `K-KL-006`).
4. **Kreaturen mit Körper unterliegen funktionalen Verletzungen:** ein zerstörtes Bein beeinträchtigt die Bewegung, wo es zur Art passt; Narben bleiben (`K-KO-006`, `K-KO-007`).
5. **Keine Artenliste:** Die Quellen wünschen 20 bis 30 tiefe Kernarten (🧭, `K-…` Kreaturen 19), nennen aber keine als Spielentscheidung. Dieses Modul beschreibt deshalb **Kampfprofile als Vorlage**, nicht Gegner.

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-KS-001 | **Physischer Schaden und übernatürliche Wirkung sind getrennt;** Spezialmaterial wirkt nur bei passender Artregel | 📋 BESCHLOSSEN | `K-KO-001` (`23#293A` P3, `23#294E` P3) | KRE MUN | `COMBAT-DEC-MU-004` |
| COMBAT-DEC-KS-002 | **Manche Wesen sind verletzbar, aber nicht damit tötbar;** ein Schuss kann nur vertreiben oder unterbrechen | 📋 BESCHLOSSEN | `K-KO-002`; `23#85A` P2 (Beispiel Monster C) | KRE | – |
| COMBAT-DEC-KS-003 | **Regeneration ist artabhängig** und in Teilregeln zerlegt (Rate, Gewebe, Knochen, Organe, Glieder, Ressourcen, Umwelt, Unterdrücker, Grenzen) | 📋 BESCHLOSSEN | `K-KO-003` | KRE MED | – |
| COMBAT-DEC-KS-004 | **Schmerzempfinden ist individuell;** kein Schmerz heißt nicht keine Beeinträchtigung | 📋 BESCHLOSSEN | `K-KO-004` | KRE KI | – |
| COMBAT-DEC-KS-005 | **Anatomie ist modular und artspezifisch;** funktionale Verletzungen gelten auch für Kreaturen mit Körper, außer die Art regeneriert sie oder ist körperlos | 📋 BESCHLOSSEN | `K-KO-005`, `K-KO-006`; Spielvision §9.2 | KRE TRF | Datensatz je Art: Abschnitt 3 |
| COMBAT-DEC-KS-006 | **Bleibende Verletzungen und Narben überdauern Begegnungen,** wenn die Art sie nicht regeneriert; wiederkehrende Individuen sind an den Folgen früherer Kämpfe erkennbar | 📋 BESCHLOSSEN | `K-KO-007`; Spielvision §9.2 | KRE NPC | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-KS-007 | **Schwächen haben Wirkungsarten:** tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend, fähigkeitsstörend | 📋 BESCHLOSSEN | `K-KO-008` | KRE MUN | [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) |
| COMBAT-DEC-KS-008 | **Resistenz wird intern gestuft;** die Oberfläche zeigt beobachtete Belege, keine Zahl | 📋 BESCHLOSSEN | `K-KO-009` | KRE UI | Archivdarstellung `K-KO-018` ❓ |
| COMBAT-DEC-KS-009 | **Bestimmte Wesen sind ohne die richtige Methode kaum endgültig tötbar;** es gibt mehrere Methoden, Vorbereitung wird belohnt, Improvisation ist möglich, Überreste, Gefangennahme und Verhör sind Optionen | 📋 BESCHLOSSEN | `K-KO-014`; `E-FA-013` | KRE ERM | – |
| COMBAT-DEC-KS-010 | **Existenzformen** (körperlich, verwandelter Mensch, besessen, körperlos, zeitweise manifestiert, gebunden) und Herkunft sind getrennte Angaben; ‚Unbekannt‘ ist gültig | 📋 BESCHLOSSEN | `K-KL-002`, `K-KL-003` | KRE | Kampfregeln je Form: Abschnitt 5 |
| COMBAT-DEC-KS-011 | **Bei gebundenen Entitäten ist die Bindung wichtiger als der Schaden;** Geister verschwinden, manipulieren die Umgebung, greifen aus unerwarteten Richtungen an und lassen sich nicht normal erschießen | 📋 BESCHLOSSEN / 🧭 | `K-GE-004`; `23#85A` P13 (Beispiel Geist); `23#86E` P13 | KRE | Serienlogik des Geistes (Salz, Eisen, Überreste) ist 💡 (`K-GE-016`) |
| COMBAT-DEC-KS-012 | **Bei einer Besessenheit sind Wirt und Entität getrennte Figuren;** die Besessenheit ist graduell und hat Folgen für Wirt und Umfeld | 📋 BESCHLOSSEN | `K-GE-007`, `K-GE-008`, `K-GE-012` | KRE NPC | Kampf gegen Besessene: `CQ-63` |
| COMBAT-DEC-KS-013 | **Dämonische Wesen sind intelligent, planend und vertragsfähig;** nicht jeder Dämon greift sofort an, Verhandlung ist möglich | 📋 BESCHLOSSEN | `K-GE-009`, `K-HW-004` | KRE KI | [Modul 21](21-Kampfvermeidung-Flucht-und-Deeskalation.md) |
| COMBAT-DEC-KS-014 | **Leviathane sind uralt;** gewöhnliche Waffen töten sie nicht; eine zweite Tötung im Fegefeuer ist endgültig | 📋 BESCHLOSSEN | `K-HW-006`, `K-HW-011` | KRE | Kampf: `CQ-66` |
| COMBAT-DEC-KS-015 | **Intelligenz hat mehrere unabhängige Dimensionen und die Antriebe ersetzen einen Aggro-Wert** | 📋 BESCHLOSSEN | `K-KL-005`, `K-KL-006` | KI KRE | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-KS-016 | **Monster müssen nicht bis zum Tod kämpfen:** Sie fliehen, wechseln den Bau oder das Opfer, verstecken sich oder holen Verstärkung | 📋 BESCHLOSSEN | `N-NL-001`; `23#293A` P13 | KI NPC | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-KS-017 | **Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf;** manche beobachten den Spieler und greifen später besser vorbereitet an | 📋 BESCHLOSSEN | `N-NL-002`; `23#1074A` P58–P59 | KI NPC | – |
| COMBAT-DEC-KS-018 | **Individuen derselben Art unterscheiden sich;** Ausnahme- und Alpha-Individuen sind keine Elite-Klasse (Ursachenmenge; die Schwäche kann teilweise versagen); es gibt keine Anzeige wie ‚ALPHA‘ oder ‚IMMUN‘, sondern Entdeckung durch Beobachtung und Recherche | 📋 BESCHLOSSEN | `K-KL-009`, `K-KL-010`, `K-KL-011` | KRE UI | – |
| COMBAT-DEC-KS-019 | **Der Tod eines Anführers löscht seine Gruppe nicht;** die Überlebenden verstecken sich | 📋 BESCHLOSSEN | `K-HW-009`; `N-TE-001` | KRE KI | [Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md) |
| COMBAT-DEC-KS-020 | **Verletzungen durch Wesen sind mythologieabhängig,** die Behandlung muss gefunden werden; Infektion oder Verwandlung ist bei manchen Arten möglich, ein Gegenmittel ist denkbar, es gibt kein Game Over, nur Tod | 📋 BESCHLOSSEN | `K-KO-011`, `K-KO-012` | MED KRE | [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| COMBAT-DEC-KS-021 | **Gore bei Wesen ist ursachenabhängig;** manche Arten haben eigene Zerstörungssysteme | 📋 BESCHLOSSEN | `K-KO-013` | TRF ANI | `CQ-47` |
| COMBAT-DEC-KS-022 | **Humanoide übernatürliche Gegner können den größten Teil des menschlichen Nahkampfsystems nutzen;** andere haben falsche Anatomie für Griffe, sind zu groß, besitzen Klauen, beißen, bewegen sich ungewöhnlich oder regenerieren | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#166A` P14; `23#167E` P14 | KRE ANI | `COMBAT-DEC-NK-032` |
| COMBAT-DEC-KS-023 | **Kampfarchetypen als Beispiele** (Geist, sehr schneller Jäger, intelligenter Gegner, schweres Monster; tierartig, humanoid-intelligent) | 💡 VORSCHLAG | `23#85A` P13; `23#98A` §8 (Entwurf des Assistenten) | KRE | Abschnitt 4 |
| COMBAT-DEC-KS-024 | **Kampfprofil einer Art als Datensatz** (Anatomie, Regeneration, Schmerz, Resistenz, Schwächen, Fähigkeiten, Taktikvorrat, Wahrnehmung, Verhalten bei Verletzung) | 💡 VORSCHLAG | Abschnitt 3 (Entwurf); Vorlage in [Kreaturen 19](../Kreaturen/19-Artennennungen-und-Artenvorlage.md) §4 | KRE TST | `CQ-64` |
| COMBAT-DEC-KS-025 | **Eine beschlossene Artenliste und Kampfprofile der Kernarten gibt es nicht;** die Quellen wünschen 20 bis 30 tiefe Kernarten statt 100 flacher | ❓ OFFEN | [Kreaturen 19](../Kreaturen/19-Artennennungen-und-Artenvorlage.md) §1; `M-OD` | KRE | `CQ-64` |
| COMBAT-DEC-KS-026 | **Konfrontation mit körperlosen und kaum tötbaren Wesen im Kampf** (Manifestation stören, Bindung lösen, Schutz, Flucht) ist mechanisch nicht beschrieben | ❓ OFFEN | `K-GE-003`, `K-GE-019` (Exorzismus-/Bannregeln ❓) | KRE STE | `CQ-65` |
| COMBAT-DEC-KS-027 | **Große Kämpfe gegen Hauptbedrohungen** (mehrere Akte, lange Kampfdauer, Phasen, Zwischen-Checkpoints) sind nicht beschrieben; beschlossen ist nur, dass Hauptbedrohungen über mehrere Akte laufen und ein sehr langer Kampf Zwischen-Autosaves erlauben darf | ❓ OFFEN | `K-HW-016`; `23#371E`, `23#372A` (Beispiel eines 30-Minuten-Kampfes, Assistent) | KRE PER | `CQ-66` |
| COMBAT-DEC-KS-028 | **Wilde Tiere und Hunde im Kampf** (Angriff, Abwehr, Jagd) sind Teil der Tierdokumentation und nicht der Monsterregeln | 🧭 RICHTUNG | [Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md); `K-TI-003` | KRE | `CQ-49` |

---

## 3. Was ein Kampfprofil einer Art enthalten muss

> **💡 VORSCHLAG (`COMBAT-DEC-KS-024`).** Die Liste leitet aus beschlossenen Kreaturenregeln ab, welche **Kampfdaten** pro Art nötig sind. Sie legt keine Art fest.

| Feld | Inhalt | Quelle der Anforderung |
|---|---|---|
| Existenzform und Herkunft | körperlich, besessen, körperlos, gebunden … | `K-KL-003`, `K-KL-004` |
| Anatomie | modular; Strukturen, die funktional verletzt werden können | `K-KO-005`, `K-KO-006` |
| Regeneration | Rate, Gewebe, Grenzen, Unterdrücker | `K-KO-003` |
| Schmerz | Empfindlichkeit, Beeinträchtigung trotz Schmerzlosigkeit | `K-KO-004` |
| Schwächen mit Wirkungsart | Material, Methode, Ort, Zustand | `K-KO-008` |
| Resistenz (intern gestuft) | gegen Munition, Feuer, Eisen, Salz … | `K-KO-009` |
| Fähigkeiten | Licht manipulieren, Strom abschalten, Stimmen imitieren, teleportartiges Auftauchen u. a. | `23#253A` P6 (Taktikvorrat, Beispiele des Assistenten) |
| Wahrnehmung | Sicht, Gehör, Geruch, weitere Sinne je Art | `23#240E`, `K-KI-001` |
| Kognition | Dimensionen der Intelligenz, Antriebe | `K-KL-005`, `K-KL-006` |
| Taktikvorrat | Liste möglicher Taktiken, Wechsel bei Misserfolg | `K-KI-006`, `K-KI-007` |
| Verhalten bei Verletzung | Rückzug, Verstecken, Rache, Wiederkehr | `N-NL-001`, `N-NL-002` |
| Gruppenverhalten | Rudel, Hierarchie, Führerverlust | `K-OE-009`, `K-HW-009` |
| Reaktion auf Fallen | erkennen, untersuchen, umgehen, sabotieren | `K-KI-015` |
| Gedächtnis | Individuelles Gedächtnis | `K-KI-014` |
| Zerstörungs-/Gore-System | eigene Darstellung | `K-KO-013` |
| Leitsatz | Warum diese Art anders kämpft als die anderen | `K-GR-014` |

**Gedächtnis- und Lernregeln:** Monster lernen nur aus Beobachtetem, abhängig von Art und Intelligenz (`K-KI-013`); ob das Lernen je Individuum, Gruppe oder Art gilt, ist offen (`K-KI-019`).

---

## 4. Kampfarchetypen als Beispiele

> **💡 VORSCHLAG.** Die Beispiele stammen vom Assistenten (`23#85A` P13, `23#98A` §8). Der Entwickler bestätigt die **Idee** (‚ja genau, das ist wichtig‘, `23#86E` P13), nicht die Arten.

| Archetyp (Beispiel) | Kampfverhalten laut Beispiel | Kampfregel, die daraus folgt |
|---|---|---|
| Geist | verschwindet, manipuliert Umgebung, nicht normal erschießbar, greift aus unerwarteten Richtungen an | Konventionelle Treffer kaum relevant; Gegenmaßnahmen und Bindung zählen (`COMBAT-DEC-KS-011`) |
| Sehr schneller Jäger | schließt Distanz, flankiert, zieht sich nach Verletzung zurück | Distanzwaffe hat wenig Zeit; Rückzug und Rückkehr (`COMBAT-DEC-KS-016`, `-017`) |
| Intelligenter Gegner | erkennt die Waffe, sabotiert Licht, lockt, stellt Fallen | Taktikvorrat; Anpassung nur aus Beobachtetem (`K-KI-013`) |
| Schweres Monster | zerstört Deckung, kaum aufzuhalten, zwingt zum Ausweichen statt Tanken | Deckung ist relativ; Verteidigung über Bewegung (`COMBAT-DEC-DF-011`) |
| Tierartige Kreatur | extreme Geschwindigkeit und Tracking | Verfolgung, Sichtlinie brechen |
| Humanoid-intelligenter Gegner | Deckung, Flanking, Täuschung | menschliches Kampfsystem plus Zusatzregeln (`COMBAT-DEC-KS-022`) |
| Besonderes Wesen | spezielle Munition erforderlich | Passende Schwäche (`COMBAT-DEC-MU-004`) |

Alle Beispiele sind Archetypen. Sie dürfen nicht zu einer Typenliste mit Leveln werden (`COMBAT-DEC-GP-003`).

---

## 5. Existenzformen und Kampfregeln

Aus `K-KL-003` und den Kampfsystem-Entscheidungen lässt sich ableiten (🧩 ABGELEITET), welche Teile des Kampfsystems je Existenzform greifen. Die Tabelle ist eine Prüfhilfe, keine Regeldefinition.

| Existenzform (`K-KL-003`) | Treffermodell (Modul 12) | Nahkampf (Modul 04) | Schusswaffen (Modul 05) | Besonderheit |
|---|---|---|---|---|
| körperlich | volle Anatomie, artspezifisch | je nach Anatomie | normale Ballistik, dann Resistenz | Regeneration artabhängig |
| verwandelter Mensch | Menschenanatomie plus Modifikatoren | Menschenregeln plus Zusatz | normale Ballistik, dann Resistenz | Verwandlung als Zustand (`K-KO-012`) |
| besessen | Wirtskörper (Menschenanatomie) | Menschenregeln | normale Ballistik | Wirt und Entität getrennt (`K-GE-007`); Wirt kann sterben |
| körperlos | keine physische Anatomie | Griffe unmöglich | normale Treffer kaum relevant | Manifestation stören (`K-GE-003`, `K-GE-004`) |
| zeitweise manifestiert | Anatomie nur im manifestierten Zustand | wie körperlich nur manifestiert | wie körperlich nur manifestiert | Zustandswechsel (`K-GE-003`) |
| gebunden | nach Form | nach Form | nach Form | Bindung ist das Ziel (`K-GE-004`) |

---

## 6. Menschenregeln und Monsterregeln

Der Entwickler will **ein** Nahkampfsystem mit Zusatzregeln pro Wesen, nicht zwei isolierte Systeme (`23#166A` P14, `23#167E` P14).

1. **Gemeinsame Grundlagen:** Körperzustand, Verletzungsmodell, Ausdauer, Wahrnehmung, Gedächtnis.
2. **Artspezifische Zusatzregeln:** Anatomie, Regeneration, Fähigkeiten, Schwächen, Verhalten.
3. **Nie:** Level, farbcodierte Rarität, automatische Skalierung (`COMBAT-DEC-GP-003`).
4. **Schwierigkeit verändert die Anatomie nicht** (`COMBAT-DEC-GP-014`), sie verändert das Verhalten der Wesen (aufmerksamer, aggressiver, Taktikvorrat).

**Menschliche Gegner** (Kriminelle, Kultisten, Jäger, Polizei) kämpfen nach Menschenregeln mit Persönlichkeit, Erfahrung, Bewaffnung, Gruppe, Moral (`COMBAT-DEC-NK-017`, `-018`). Ihre Fraktionszugehörigkeit steuert Reaktion und Folgen ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)).

---

## 7. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Kampf gegen Besessene (`CQ-63`) | A: Der Wirt ist nur ein Körper; Tötung als Normalfall | einfach | widerspricht ‚Wirt und Entität getrennt‘ |
| | B: Der Wirt wird geschont; nichttödliche Optionen (Fesseln, Exorzismus) bevorzugt | passt zur Serienlogik | Aufwand, verlangt Fesselung und Ritual |
| | C: Beides, Wahl hat Folgen (Ruf, Gedächtnis, Story) | konsequent | Datenmodell, Tests |
| Kernarten-Reihenfolge (`CQ-64`) | A: Eine Art zuerst vollständig | zeigt Machbarkeit | Gefahr der Einseitigkeit |
| | B: Mehrere Archetypen minimal | trägt Vielfalt | alle halbfertig |
| | C: Nach Spielphase (Phase-1-Fall) | passt zu `CLAUDE.md` §4 | Verzug bei später Auswahl |
| Körperlose Wesen (`CQ-65`) | A: Sonderspiel (Rätsel, Ritual) statt Kampf | passt zu ‚Bindung wichtiger als Schaden‘ | Kampfgefühl fehlt |
| | B: Spezialwaffen mit Schaden gegen Manifestation | Kampf bleibt | Gefahr, in Level/„Spezialwaffe“ zu münden |
| | C: Beides | tragfähig | Aufwand |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zu Kreaturen ([Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md), [10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md)) | Artdaten, KI, Gedächtnis |
| zum Treffermodell ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) | artspezifische Anatomie |
| zu Schwächen ([Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md)) | Wirkungsarten |
| zu Fallen ([Modul 17](17-Fallen-Fesselung-und-Lebendfang.md)) | Fesselung und Eindämmung |
| zu Nemesis ([Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md)) | Rückkehr, Narben, Lernen |
| zu Mods | additive Arten (`N-KO-010`) |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Art ohne Anatomiedatensatz | Ablehnung mit Meldung | 💡 |
| Widersprüchliche Schwächen (Mod-Fall) | additiv, mit Konfliktmeldung | 💡 |
| Unverwundbare Art in einem Zustand | definierte Regel (Bindung/Manifestation), keine stille Unverwundbarkeit | 💡 |
| Individuum mit Narben, aber Art regeneriert | Regeneration der Art entscheidet (`K-KO-007`) | 📋 |
| Besessener Wirt stirbt | Konsequenzen für Entität und Umfeld | ❓ |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-63` | Kampf gegen Besessene: Wirt schonen oder töten, nichttödliche Optionen, Folgen | `COMBAT-DEC-KS-012` | ja |
| `CQ-64` | Kampfprofile der Kernarten: Auswahl, Reihenfolge, Mindestumfang | `COMBAT-DEC-KS-024`, `-025` | ja |
| `CQ-65` | Konfrontation mit körperlosen und kaum tötbaren Wesen als Kampfmechanik | `COMBAT-DEC-KS-026` | ja |
| `CQ-66` | Große Kämpfe gegen Hauptbedrohungen: Phasen, Länge, Zwischen-Checkpoints, Gruppenkampf | `COMBAT-DEC-KS-014`, `-027` | ja |
