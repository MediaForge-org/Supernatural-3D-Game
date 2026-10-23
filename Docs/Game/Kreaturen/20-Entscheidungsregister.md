# Kreaturen 20: Entscheidungsregister

> [!IMPORTANT]
> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 ist vom Entwickler bestätigt oder im Master als akzeptiert geführt; 🧭, 💡, ❓ sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Kürzel in den Quellen: `P<n>` = Punkt n in der genannten Nachricht (Frage in der A-Nachricht, Antwort in der E-Nachricht).

**Inhalt**

1. [GR: Leitsätze (Modul 02)](#gr-leitsätze-modul-02)
2. [IP: Umgang mit der Serie und IP (Modul 01 §4)](#ip-umgang-mit-der-serie-und-ip-modul-01-4)
3. [KL: Klassifikation (Modul 03)](#kl-klassifikation-modul-03)
4. [KO: Körper, Verletzung, Schwächen, Regeneration (Modul 04)](#ko-körper-verletzung-schwächen-regeneration-modul-04)
5. [GE: Geister, Besessenheit, Flüche, Verträge (Modul 05)](#ge-geister-besessenheit-flüche-verträge-modul-05)
6. [HW: Dämonen, Engel, Leviathane, Monsterseelen (Modul 06)](#hw-dämonen-engel-leviathane-monsterseelen-modul-06)
7. [KS: Kosmologie, Seelen, Reiche (Modul 07)](#ks-kosmologie-seelen-reiche-modul-07)
8. [OE: Ökologie, Populationen, Lebenszyklus (Modul 08)](#oe-ökologie-populationen-lebenszyklus-modul-08)
9. [GS: Intelligente Wesen und Gesellschaft (Modul 09)](#gs-intelligente-wesen-und-gesellschaft-modul-09)
10. [KI: Monster-KI (Modul 10)](#ki-monster-ki-modul-10)
11. [HU: Hunterwelt, Organisationen, Kulte (Modul 11)](#hu-hunterwelt-organisationen-kulte-modul-11)
12. [JA: Jagd, Rituale, Schutz, Objekte (Modul 12)](#ja-jagd-rituale-schutz-objekte-modul-12)
13. [CO: Lebendfang, Fallen, Containment (Modul 13)](#co-lebendfang-fallen-containment-modul-13)
14. [FO: Forschung und Bestiarium (Modul 14)](#fo-forschung-und-bestiarium-modul-14)
15. [VF: Vorfälle und lebendige Welt (Modul 15)](#vf-vorfälle-und-lebendige-welt-modul-15)
16. [OR: Übernatürliche Orte (Modul 16)](#or-übernatürliche-orte-modul-16)
17. [PD: Persistenz und Modding (Modul 17)](#pd-persistenz-und-modding-modul-17)
18. [TI: Tiere (Modul 18)](#ti-tiere-modul-18)
19. [LW: Lebendige Welt, Gedächtnis, Nemesis (Modul 10 §14)](#lw-lebendige-welt-gedächtnis-nemesis-modul-10-14)
20. [EN: Entwurfsregister (nur unverbindliche Vorschläge)](#en-entwurfsregister-nur-unverbindliche-vorschläge)

---

## GR: Leitsätze (Modul 02)

Gruppe zu [02-Leitsaetze-und-Abgrenzung.md](02-Leitsaetze-und-Abgrenzung.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-GR-001 | Keine sichtbaren Stufen oder Level für Kreaturen; Gefahr entsteht aus Wissen, Fähigkeiten, Umgebung, Vorbereitung, Anzahl, Verletzungen, Ausrüstung, Intelligenz | 📋 | `M-PD`; `23#293A` P16; `23#296E` |
| K-GR-002 | Die Welt ist normal; das Übernatürliche ist verborgen, selten, widersprüchlich dokumentiert | 📋 | `23#359A` P2; `23#360E` P2; `23#293A` P19 |
| K-GR-003 | Kein universelles Magiesystem; Rituale ohne Mana; der Hunter ist kein Magier | 📋 | `23#359A` P1; `23#360E` P1; `23#1075E` P99–106 |
| K-GR-004 | Der Spieler bleibt Mensch; temporäre übernatürliche Zustände mit Konsequenzen | 📋 | `23#359A` P28–29; `23#360E` P28, Antwort B |
| K-GR-005 | Regeln müssen beobachtbar sein; Fairness durch Hinweise | 📋 | `23#359A` P21; `23#360E` P21 |
| K-GR-006 | Nicht jede Begegnung ist ein Kampf | 📋 | `23#293A` P17; `23#294E` P17 |
| K-GR-007 | Nicht jedes Wesen ist böse; Ausgänge: töten, vertreiben, heilen, Fluch brechen, verhandeln, retten | 📋 | `23#293A` P15; `23#294E` P15; `23#1075E` P45–54 |
| K-GR-008 | Monster respawnen nicht; tot ist tot; Ausnahme nur über Befreiung aus dem Fegefeuer | 📋 | `23#1075E` P30, P37; `M-B09` |
| K-GR-009 | Wichtige Arten sind handgeschrieben; prozedural nur Alter, Temperament, Narben, Erfahrung, Größe, Tendenzen | 📋 | `23#307A` P28; `23#308E` P28; `M-PD` |
| K-GR-010 | Das Archiv zeigt Wissen des Charakters, keine internen Daten | 📋 | `23#295A`; `M-PD` |
| K-GR-011 | Kernziel: Monster sind Wesen dieser Welt, keine Gegnergruppen auf der Karte | 📋 | `23#1075E` P156 |
| K-GR-012 | 20–30 tiefe Kernarten statt 100 flacher bevorzugt (nicht gesperrt) | 🧭 | `23#1075E` P151; `Entscheidungsstatus` |
| K-GR-013 | Weitere Monster, Organisationen, Ebenen und Lore über DLC | 📋 | `23#1075E` P154 |
| K-GR-014 | Jede Art unterscheidet sich in Ökologie, Verhalten, Spuren, Schwäche, Jagdmethode, Kampf-KI, Lore, NPC-Auswirkung | 📋 | `23#1075E` P155 |
| K-GR-015 | Zahl und Liste der Basisarten sind offen | ❓ | `M-CA0905` §7; `M-B09`; `M-OD` |
| K-GR-016 | Creature Definition als Gliederung der Datenfelder | 📋 | `23#307A` P27; `23#308E` P27 |
| K-GR-017 | Kein Level-Gating, keine Elite-Klasse als HP-Multiplikator | 📋 | `M-PD`; `23#293A` P11; `23#294E` D |

---

## IP: Umgang mit der Serie und IP (Modul 01 §4)

Gruppe zu [01-Quellenlage-und-Methodik.md](01-Quellenlage-und-Methodik.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-IP-001 | Bei nicht festgelegten übernatürlichen Regeln gilt die Serienlogik als Standard | 📋 | `23#362E`; `23#371E` |
| K-IP-002 | Dämonen, Engel, Geister, Leviathane existieren; Hauptcharaktere sind angehaucht, aber eigenständig | 📋 | `23#377E`; `23#379E` |
| K-IP-003 | Eigene Hierarchien, Fähigkeiten, Designs, Namen statt serienspezifischer Ausgestaltung | 📋 | `23#378A`; `23#379E`; `M-CA0905` §3 |
| K-IP-004 | Die wissenszentrierte Organisation hat eigenen Namen, eigene Geschichte, eigene Mythologie | 📋 | `23#1075E` P5; `23#1076A`; `M-B09` |
| K-IP-005 | Eigenname der Organisation und der Ursprungswesen offen | ❓ | `M-B09` |
| K-IP-006 | Auch spätere Serienbereiche sollen verbessert statt 1:1 übernommen werden (Schreibprojekt-Kontext) | 📋 | `50#13E` |
| K-IP-007 | Serieninhalt wird dem Entwickler nicht oder nur ohne Spoiler genannt | 📋 | `23#1091E` |
| K-IP-008 | Herkunftsfeld folklore | genre | series_specific je Kreatur | 💡 | `23#376A` |

---

## KL: Klassifikation (Modul 03)

Gruppe zu [03-Klassifikation-Herkunft-und-Individuen.md](03-Klassifikation-Herkunft-und-Individuen.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-KL-001 | Mehrdimensionale Klassifikation statt Tier/Level | 📋 | `23#307A` P1; `23#308E` P1 |
| K-KL-002 | Existenzform und Herkunft getrennt; Unknown ist eine gültige Angabe | 📋 | `23#307A` P3; `23#308E` P3 |
| K-KL-003 | Existenzformen: körperlich, verwandelter Mensch, besessen, körperlos, zeitweise manifestiert, gebunden | 📋 | `23#307A` P2; `23#308E` P2; `M-PD` |
| K-KL-004 | Herkunftsfamilien (natürlich, verflucht, verwandelt, erschaffen, beschworen, gebunden, historisch, dimensional, künstlich, unbekannt) | 📋 | `23#307A` P3; `M-PD`; `M-CA0905` §3 |
| K-KL-005 | Intelligenz in sieben unabhängigen Dimensionen | 📋 | `23#307A` P4; `23#308E` P4 |
| K-KL-006 | Antriebe statt Aggro-Wert | 📋 | `23#307A` P5; `23#308E` P5 |
| K-KL-007 | Temperament wichtiger Individuen intern; Individualität gewünscht | 📋 | `23#307A` P6; `23#308E` P6; `23#294E` D |
| K-KL-008 | Alter und Erfahrung getrennt; Alter multipliziert keine Stärke; Alter soll spürbar, aber nicht direkt sichtbar sein | 📋 | `23#307A` P7; `23#308E` P7; `23#294E` A |
| K-KL-009 | Ausnahme-/Alpha-Individuen sind keine Elite-Klasse; Ursachenmenge; Schwäche kann teilweise versagen | 📋 | `23#307A` P8; `23#308E` P8; `23#295A`; `M-PD` |
| K-KL-010 | Keine Beschriftung wie ALPHA oder IMMUN; Entdeckung durch Beobachtung und Recherche | 📋 | `23#307A` P8; `23#295A`; `M-PD` |
| K-KL-011 | Individuen derselben Art unterscheiden sich | 📋 | `23#1075E` P152 |
| K-KL-012 | Benannte Einzelwesen mit eigener Geschichte und Archivseite | 📋 | `23#307A` P29; `23#308E` P29 |
| K-KL-013 | Es gibt vielleicht erste ihrer Art (Entwicklerbegriff Alphas) | 📋 | `23#1075E` P153 |
| K-KL-014 | Archiv trennt Art und bekannte Individuen | 📋 | `23#307A` P30; `23#308E` P30 |
| K-KL-015 | Arbeitsbegriff Ursprungswesen statt Alpha aus IP-Gründen | 💡 | `23#1076A` |
| K-KL-016 | Welche Arten Individuen und welche Population sind | ❓ | `23#1074A` P31 |

---

## KO: Körper, Verletzung, Schwächen, Regeneration (Modul 04)

Gruppe zu [04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-KO-001 | Physischer Schaden und übernatürliche Wirkung getrennt; Spezialmaterial nur bei passender Artregel | 📋 | `23#293A` P3; `23#294E` P3 |
| K-KO-002 | Manche Wesen sind verletzbar, aber nicht damit tötbar | 📋 | `23#293A` P4; `23#294E` P4 |
| K-KO-003 | Regeneration artabhängig und in Teilregeln zerlegt (Rate, Gewebe, Knochen/Organe/Glieder, Ressourcen, Umwelt, Unterdrücker, Grenzen) | 📋 | `23#293A` P5; `23#307A` P13; `M-PD` |
| K-KO-004 | Schmerzempfinden individuell; kein Schmerz heißt nicht keine Beeinträchtigung | 📋 | `23#293A` P6; `23#294E` P6 |
| K-KO-005 | Modulare, artspezifische Anatomie | 📋 | `23#307A` P11; `23#308E` P11; `M-PD` |
| K-KO-006 | Funktionale Verletzungen gelten auch für Kreaturen mit Körper | 📋 | `23#307A` P12; `23#308E` P12; `M-PD` |
| K-KO-007 | Bleibende Verletzungen und Narben machen Individuen wiedererkennbar | 📋 | `23#307A` P14; `23#308E` P14; `M-PD` |
| K-KO-008 | Schwächen haben Wirkungsarten (tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend, fähigkeitsstörend) | 📋 | `23#307A` P9; `23#308E` P9; `M-PD` |
| K-KO-009 | Resistenz intern gestuft; Oberfläche zeigt beobachtete Belege | 📋 | `23#307A` P10; `23#308E` P10; `M-PD` |
| K-KO-010 | Übernatürliche Verletzungen erweitern das Traumasystem | 📋 | `M-PD` |
| K-KO-011 | Verletzungen durch Wesen sind mythologieabhängig; Behandlung muss gefunden werden | 📋 | `48#122A`; `48#123E`; `48#124A` |
| K-KO-012 | Infektion/Verwandlung bei manchen Arten; Gegenmittel möglich; kein Game Over, nur Tod | 📋 | `23#1075E` P144–146 |
| K-KO-013 | Gore ursachenabhängig; Kinder ohne Gore; Arten mit eigenen Zerstörungssystemen | 📋 | `M-PD`; `23#1075E` P147–150 |
| K-KO-014 | Bestimmte Wesen sind ohne richtige Methode kaum endgültig tötbar; mehrere Methoden; Vorbereitung belohnt; Improvisation; Überreste; Gefangennahme; Verhör | 📋 | `23#1075E` P91–98 |
| K-KO-015 | Kein Kugelschwamm; richtige Waffe, Munition und Schwäche sind Kern | 📋 | `M-PD` |
| K-KO-016 | Seltene übernatürliche Sofortheilung (Wunsch) | 📋 | `35#26E` |
| K-KO-017 | Dreistufiges Heilungsmodell und Fähigkeitsliste (CanStabilize ... CanResurrect) | 💡 | `35#27A` |
| K-KO-018 | Beobachtungsbasierte Anzeige statt Resistenzzahl; Archivstatus 'tot, aber nicht endgültig' | ❓ | `M-OD`; Modul 04 §15 |

---

## GE: Geister, Besessenheit, Flüche, Verträge (Modul 05)

Gruppe zu [05-Geister-Besessenheit-und-Fluecher.md](05-Geister-Besessenheit-und-Fluecher.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-GE-001 | Phänomene mit eigenen Regeln (Geister, Flüche, Besessenheit, dämonische Wesen, Artefakte) | 📋 | `23#359A` P1; `23#360E` P1 |
| K-GE-002 | Geisterhypothese: 'Es sieht nach einem Geist aus' ist zunächst eine Hypothese | 📋 | `23#359A` P7; `23#360E` P7 |
| K-GE-003 | Geistzustände von nicht-körperlich bis voll manifestiert | 📋 | `23#359A` P8; `23#360E` P8 |
| K-GE-004 | Bei gebundenen Entitäten ist die Bindung wichtiger als Schaden | 📋 | `23#359A` P9; `23#360E` P9 |
| K-GE-005 | Flüche als eigener Systemtyp | 📋 | `23#359A` P10; `23#360E` P10 |
| K-GE-006 | Flüche reagieren auf Bedingungen; Frage nach dem Auslöser | 📋 | `23#359A` P11; `23#360E` P11 |
| K-GE-007 | Besessenheit trennt Wirt und Entität | 📋 | `23#359A` P12; `23#360E` P12; `M-PD` |
| K-GE-008 | Besessenheit ist graduell | 📋 | `23#359A` P13; `23#360E` P13 |
| K-GE-009 | Dämonische Wesen: intelligent, planend, vertragsfähig; Orientierung an der Serie | 📋 | `23#359A` P14; `23#360E` P14 |
| K-GE-010 | Verträge regelbasiert | 📋 | `23#359A` P15; `23#360E` P15 |
| K-GE-011 | Nicht jeder Tote wird zum Geist | 📋 | `23#458A` P2; `23#459E` P2 |
| K-GE-012 | Besessenheit hat echte Folgen und kann Spieler und NPCs treffen, nicht oft | 📋 | `23#1075E` P142–143 |
| K-GE-013 | Flüche auf dem Spieler halten an, bis eine Lösung gefunden ist | 📋 | `23#1075E` P144 |
| K-GE-014 | Host erinnert bei voller Besessenheit zumindest Fragmente | 📋 | `23#362E` C |
| K-GE-015 | Spieler kann kurz besessen sein; Begleiter rettet ihn | 📋 | `23#360E` P28, B |
| K-GE-016 | Geisterregeln der Serienlogik (Salz, Eisen, Überreste, Anker, Degeneration, Reaper-Funktion) | 💡 | `23#370A`; `23#361A` P13–15 |
| K-GE-017 | Besessenheitsmodelle Unterdrückung, Verdrängung, Teilbesitz, Vollkontrolle; Host kämpft von innen | 💡 | `23#361A` P16–17 |
| K-GE-018 | Eigene Bezeichnungen für Seelenbegleiter | ❓ | `23#370A` P12 |
| K-GE-019 | Exorzismus-/Bannregeln | ❓ | Modul 05 §10 |

---

## HW: Dämonen, Engel, Leviathane, Monsterseelen (Modul 06)

Gruppe zu [06-Daemonen-Engel-und-Leviathane.md](06-Daemonen-Engel-und-Leviathane.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-HW-001 | Dämonen, Engel, Geister und Leviathane existieren in der Welt | 📋 | `23#377E`; `23#379E` |
| K-HW-002 | Rangordnung bei Dämonen und Engeln; Mutationen | 📋 | `23#1075E` P87 |
| K-HW-003 | Hölle hat Gesellschaft mit Hierarchien, Deals, Gefangenen | 📋 | `23#458A` P19; `23#459E` P19 |
| K-HW-004 | Nicht jeder Dämon greift sofort an; Verhandlung möglich | 📋 | `23#458A` P20; `23#459E` P20 |
| K-HW-005 | Engel sind keine simple Gut-Fraktion | 📋 | `23#458A` P21; `23#459E` P21 |
| K-HW-006 | Leviathane: uralt, mit dem Fegefeuer verbunden; gewöhnliche Waffen töten nicht | 📋 | `23#465A` P20–21; `23#466E` P20–21 |
| K-HW-007 | Leviathane sind nicht von Anfang an auf der Welt | 📋 | `23#468E` |
| K-HW-008 | Leviathane haben persistente Individuen | 📋 | `23#1075E` P31 |
| K-HW-009 | Der Tod eines Anführers löscht seine Gruppe nicht; Überlebende verstecken sich | 📋 | `23#470E`; `23#471A` |
| K-HW-010 | Leviathane an die Serie angelehnt, aber mit eigener Biologie, Form, Hierarchie, Fähigkeiten, Schwächen, Story | 📋 | `23#466E` P18, D |
| K-HW-011 | Fegefeuer: zweite Tötung endgültig | 📋 | `23#465A` A; `23#466E` A |
| K-HW-012 | Fegefeuer: ausgewählte früher getötete Monster wiederfinden, nicht zu oft | 📋 | `23#465A` B; `23#466E` B |
| K-HW-013 | Fegefeuer: Zeit anders, weniger extrem als Hölle; Relation offen | 📋 | `23#465A` C; `23#466E` C |
| K-HW-014 | Monsterseelen enthalten Macht, aber kein Mana | 📋 | `23#465A` P14; `23#466E` P14 |
| K-HW-015 | Wahre Form: Concepts, Anatomiebögen, Blender-Prototypen, Abnahme durch den Entwickler | 📋 | `23#465A` P23; `23#466E` P23; `23#467A` P8 |
| K-HW-016 | Hauptbedrohungen über mehrere Akte | 📋 | `23#49E` P2; `23#294E` P21; `23#307A` P23 |
| K-HW-017 | Leviathan: Verletzbarkeit, Regeneration, Tarnung, Lernen, Gedächtnis, Ernährung, Archiv (Ausformulierung) | 💡 | `23#465A`; `23#467A` |
| K-HW-018 | Dämonen-Block (Besessenheit, Deals, Hierarchie, Arten, Exorzismus) | ❓ | `23#468E` |
| K-HW-019 | Struktur der Leviathane untereinander | ❓ | `23#465A` P28 |
| K-HW-020 | Ursache der Ankunft der Leviathane | ❓ | `23#469A` |

---

## KS: Kosmologie, Seelen, Reiche (Modul 07)

Gruppe zu [07-Kosmologie-Seelen-und-Reiche.md](07-Kosmologie-Seelen-und-Reiche.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-KS-001 | Übernatürliche Ebenen sind eigene Welträume; Open-World-Regionen, kleiner als die Hauptkarte | 📋 | `23#359A` P3; `23#360E` P3, A |
| K-KS-002 | Ebenen haben eigene Regeln (Zeit usw.); intern konsistent | 📋 | `23#359A` P4; `23#360E` P4 |
| K-KS-003 | Übergänge brauchen Regeln; erster Zugang kann storygebunden sein, danach Wissen und Ritual | 📋 | `23#359A` P5; `23#360E` P5, A |
| K-KS-004 | Nicht jeder Hunter kennt die wahre Kosmologie; ein Rest Mysterium bleibt | 📋 | `23#359A` P6; `23#360E` P6 |
| K-KS-005 | Todesablauf; Gameplay-Tod nicht kanonisch; NPC-Tod folgt Jenseitssystem | 📋 | `23#458A` P1; `23#459E` P1; `23#460A` |
| K-KS-006 | Himmel und Hölle sind reale Orte | 📋 | `23#458A` P3; `23#459E` P3 |
| K-KS-007 | Tote Seelen altern körperlich nicht | 📋 | `23#459E` P7 |
| K-KS-008 | Persönlicher Himmel je Mensch | 📋 | `23#459E` P17 |
| K-KS-009 | Wiederbelebung ist außergewöhnlich | 📋 | `23#458A` P15; `23#459E` P15 |
| K-KS-010 | Hin- und Rückweg mit unterschiedlichen Regeln | 📋 | `23#458A` P12; `23#459E` P12 |
| K-KS-011 | Seele ist aus einem lebenden Körper entfernbar | 📋 | `23#361A` B; `23#362E` B |
| K-KS-012 | Höllenzeit konstant (Zeitverhältnis ist kanonische Regel) | 📋 | `23#361A` A; `23#362E` A; `PE-E1022` |
| K-KS-013 | Tod in anderen Ebenen: Checkpoint mit offenem Zusatz | ❓ | `23#458A` P26; `23#459E` P26 |
| K-KS-014 | Reisekomponenten selten, ohne Grinding | 📋 | `23#458A` P10–11; `23#459E` P10–11; `23#466E` P13 |
| K-KS-015 | Mindestens Himmel, Hölle, Fegefeuer im Hauptspiel | 📋 | `23#1075E` P140; `23#1103E`; `M-CA0905` §5 |
| K-KS-016 | Reisen können Erdzeit kosten, aber viel weniger | 📋 | `23#459E` P22 |
| K-KS-017 | Körper, Bewusstsein, Seele als Dreiteilung; Seele ohne Trefferpunktwert | 💡 | `23#361A` P1, P9 |
| K-KS-018 | Natürliches Ziel der Seele nach Leben und Handlungen; Sonderregeln überschreiben; kein Karma-Balken | 💡 | `23#460A` |
| K-KS-019 | Kriterien für das Ziel einer Seele | ❓ | `23#459E` C |
| K-KS-020 | Mechanismus der Trennung von Körper und Seele; Zustand des Körpers | ❓ | `23#361A` P11, P21 |

---

## OE: Ökologie, Populationen, Lebenszyklus (Modul 08)

Gruppe zu [08-Oekologie-Populationen-und-Lebenszyklus.md](08-Oekologie-Populationen-und-Lebenszyklus.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-OE-001 | Keine Spawnpunkte; Individuen persistent, häufigere Arten populationsbasiert | 📋 | `23#1075E` P30–32 |
| K-OE-002 | Reale Jagd- und Lebensgebiete; Nester, Rudel, Familien, Kolonien | 📋 | `23#1075E` P33–34 |
| K-OE-003 | Wanderung bei Nahrungsmangel, Hunter-Druck, Feuer, Überschwemmung, Rivalen, Ereignissen | 📋 | `23#1075E` P35 |
| K-OE-004 | Fortpflanzung bei einigen Arten | 📋 | `23#1075E` P36 |
| K-OE-005 | Vernichtetes Nest bleibt weg; Wiederbesiedlung durch andere später | 📋 | `23#1075E` P37–38 |
| K-OE-006 | Population hängt von Nahrung und Opfern ab | 📋 | `23#1075E` P39 |
| K-OE-007 | Arten bevorzugen Lebensräume; Tageszeit; Wetter und Jahreszeit | 📋 | `23#1075E` P40–42 |
| K-OE-008 | Konkurrenz zwischen Arten; Meiden liefert Hinweise | 📋 | `23#1075E` P43–44 |
| K-OE-009 | Soziale Strukturen (einzeln bis Schwarm) | 📋 | `23#307A` P16; `23#308E` P16 |
| K-OE-010 | Gruppen sind nicht moralisch einheitlich | 📋 | `23#307A` P17; `23#308E` P17 |
| K-OE-011 | Artspezifische Entstehung | 📋 | `23#307A` P18; `23#308E` P18 |
| K-OE-012 | Lebensphasen bei geeigneten Arten | 📋 | `23#307A` P19; `23#308E` P19 |
| K-OE-013 | Aktivitätsbedingungen aus Zeit, Wetter, Temperatur, Mond, Ort, Sonderzustand | 📋 | `23#307A` P20; `23#308E` P20 |
| K-OE-014 | Fall entsteht aus Weltzustand; Übergang statt Sprung; keine Marker nach Einstellung | 📋 | `23#307A` P21; `23#308E` P21 |
| K-OE-015 | Zeitfenster vernichten keine Inhalte | 📋 | `23#307A` P22; `23#308E` P22; `M-PD` |
| K-OE-016 | Territoriumssimulation mit Ressourcen, Bevölkerung, Strategie, regionalen Veränderungen | 📋 | `48#142A`; `48#143E` |
| K-OE-017 | Monster-Kinder nur bei ausgewählten Arten | 📋 | `23#307A` B; `23#308E` B |
| K-OE-018 | Zeitrahmen der Wiederbesiedlung; Mindestzeit | ❓ | `23#1074A` P37–38 |
| K-OE-019 | Welche Arten sich wie vermehren | ❓ | `23#307A` P18 |
| K-OE-020 | Darstellungsregeln für Monster-Kinder | ❓ | Modul 08 §7.2 |

---

## GS: Intelligente Wesen und Gesellschaft (Modul 09)

Gruppe zu [09-Intelligente-Wesen-und-Monstergesellschaft.md](09-Intelligente-Wesen-und-Monstergesellschaft.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-GS-001 | Intelligente Wesen leben unerkannt unter Menschen mit Wohnung, Beruf, Konto, Fahrzeug, Beziehungen | 📋 | `23#1075E` P45 |
| K-GS-002 | Jahrzehnte alte Identitäten; wohlhabende und arme Wesen | 📋 | `23#1075E` P46–48 |
| K-GS-003 | Unterschiedliche Moral; Spieler kann verschonen; Überlebende und Raubtiere; Wesen können lügen; Beziehungen über Jahre | 📋 | `23#1075E` P49–54 |
| K-GS-004 | Ein friedliches Wesen lebt normal weiter; ein anderer Jäger darf es töten | 📋 | `23#308E` A; `M-PD` |
| K-GS-005 | Ein ungewollt erschaffenes Wesen kann einfach sein Leben leben; der Spieler entscheidet | 📋 | `23#294E` C |
| K-GS-006 | Familie von Wesen nicht gemeinsam böse | 📋 | `23#307A` P17; `23#308E` P17 |
| K-GS-007 | Wesen nutzen Menschen als Werkzeuge | 📋 | `23#293A` P14; `23#294E` P14 |
| K-GS-008 | Monsterorganisationen mit Hierarchien, Territorien, Machtkämpfen (Option C, nicht D) | 📋 | `48#140A`; `48#141E` |
| K-GS-009 | Wissensweitergabe unter Monstern | 📋 | `48#139E`; `48#140A` |
| K-GS-010 | Umfassende Gesellschaftssimulation der Menschen | 📋 | `48#144A`; `48#145E` |
| K-GS-011 | Öffentliches Wissen regional und dynamisch | 📋 | `48#146A`; `48#147E` |
| K-GS-012 | Seltene Gegner können Spielerstandorte herausfinden | 📋 | `23#1075E` P61 |
| K-GS-013 | Monster-Reputation im Rufsystem | 📋 | `48#153E`; `48#154A` |
| K-GS-014 | Ob Beziehungen zu Wesen wie zu Menschen möglich sind | ❓ | Modul 09 §2.3 |
| K-GS-015 | Ob der Spieler Monstergruppen gegeneinander beeinflussen darf | ❓ | Modul 09 §4.4 |

---

## KI: Monster-KI (Modul 10)

Gruppe zu [10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-KI-001 | Monster-KI variiert stark nach Art; keine humanoide Universal-KI | 📋 | `M-PD`; `23#1075E` P55–60 |
| K-KI-002 | Wahrnehmung mit Unsicherheit; kein globales Wissen | 📋 | `M-PD` |
| K-KI-003 | Vier Wissensgrade der KI | 📋 | `23#253A` P1; `23#254E` P1 |
| K-KI-004 | Letzte bekannte Position wird ungenauer | 📋 | `23#253A` P2; `23#254E` P2 |
| K-KI-005 | Suchphasen von Sofortverfolgung bis vorsichtiger Wachsamkeit | 📋 | `23#253A` P3; `23#254E` P3 |
| K-KI-006 | Taktikvorrat je Art, Wechsel bei Misserfolg | 📋 | `23#253A` P6; `23#254E` P6 |
| K-KI-007 | Taktikwahl bewertet die Lage und passt sich dem Spielerverhalten an | 📋 | `23#253A` P7; `23#254E` P7 |
| K-KI-008 | Bewusste Stille, aber nicht jedes Mal | 📋 | `23#253A` P8; `23#254E` P8; `23#240E` C |
| K-KI-009 | Drei Geräuscharten (Welt, vom Monster erzeugt, rein übernatürlich) | 📋 | `23#253A` P9; `23#254E` P9 |
| K-KI-010 | Kampf kann in Suche zurückfallen | 📋 | `23#253A` P13; `23#254E` P13 |
| K-KI-011 | Beleuchtung gehört zur Wahrnehmung | 📋 | `23#253A` P15; `23#254E` P15 |
| K-KI-012 | KI-Inspektor und viele Tests | 📋 | `23#253A` P16; `23#254E` P16 |
| K-KI-013 | Monster lernen nur aus Beobachtetem, abhängig von Art und Intelligenz | 📋 | `23#293A` P12; `23#294E` P12; `23#307A` P15 |
| K-KI-014 | Umfassendes individuelles Monster-Gedächtnis | 📋 | `48#138A`; `48#139E` |
| K-KI-015 | Taktische Reaktion auf Fallen (Option C) | 📋 | `48#136A`; `48#137E` |
| K-KI-016 | Ein in wirksamer Falle gefangenes Wesen befreit sich nicht durch KI | 💡 | `48#138A` |
| K-KI-017 | Flucht, Verstecken, Hinterhalt, Jagd auf den Spieler | 📋 | `23#1075E` P57–60 |
| K-KI-018 | Höhere Schwierigkeit: aufmerksamer, gnadenloser | 📋 | `M-PD`; `23#86E` |
| K-KI-019 | Lerneffekt je Individuum, Gruppe oder Art | ❓ | Modul 10 §6 |
| K-KI-020 | Einzelne Sinne, Taktikvorräte, Gedächtnisgrenzen je Art | ❓ | `23#253A` Schluss |

---

## HU: Hunterwelt, Organisationen, Kulte (Modul 11)

Gruppe zu [11-Hunterwelt-Organisationen-und-Kulte.md](11-Hunterwelt-Organisationen-und-Kulte.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-HU-001 | Hunter sind selten; ein bis zwei Begegnungen im Leben | 📋 | `23#1075E` P1 |
| K-HU-002 | Loses Hunter-Netz, keine Armee, Safehouses, verlassene Hunter-Orte | 📋 | `23#1075E` P2–6 |
| K-HU-003 | Hunter-Shops und normale Läden; Foren und analoge Quellen | 📋 | `23#1075E` P7–10 |
| K-HU-004 | Hunter haben Ruf, sterben, arbeiten Fälle vor dem Spieler, konkurrieren, kooperieren | 📋 | `23#1075E` P11–15 |
| K-HU-005 | Mehrere Organisationen mit verschiedenen Philosophien | 📋 | `23#1075E` P16 |
| K-HU-006 | Hauptorganisation weiß viel mehr, aber nicht alles; Politik; Korruption | 📋 | `23#1075E` P17–20 |
| K-HU-007 | Geheime Einheiten in Behörden; die meisten Polizisten wissen nichts; geheime Forschung | 📋 | `23#1075E` P21–23 |
| K-HU-008 | Übernatürliches gilt offiziell als unbewiesen; Videos; Viralität; Zusammenbruch bei Großereignissen; Glaube wächst; kein Gedächtnislöschen | 📋 | `23#1075E` P24–29 |
| K-HU-009 | Kulte mit verschiedenen Glaubenssystemen, Wachstum, Orten, Infiltration | 📋 | `23#1075E` P121–126 |
| K-HU-010 | Fälle an HQ oder Forum weitergeben, NPC-Hunter übernehmen | 📋 | `23#1075E` P62–68 |
| K-HU-011 | Hunter-Vault für gefährliche Artefakte | 📋 | `23#1075E` P107–114 |
| K-HU-012 | Staatliche Monsterforschung geheim | 📋 | `48#171E` |
| K-HU-013 | Elite im Hintergrund; Monster teilweise von ihr | 📋 | `48#179E` |
| K-HU-014 | Name und Struktur der Hauptorganisation | ❓ | `M-B09` |
| K-HU-015 | Zahl und Dichte der Hunter | ❓ | Modul 11 §2.1 |

---

## JA: Jagd, Rituale, Schutz, Objekte (Modul 12)

Gruppe zu [12-Jagd-Rituale-und-verfluchte-Objekte.md](12-Jagd-Rituale-und-verfluchte-Objekte.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-JA-001 | Rituale benötigen Zutaten, Vorbereitung, Zeit, Ort; Fehler haben Folgen; Mondphase, Jahreszeit | 📋 | `23#1075E` P99–105 |
| K-JA-002 | Rituale selbst vorbereiten; Zutaten schwer zu finden | 📋 | `23#359A` C; `23#360E` C |
| K-JA-003 | Ritualvorbereitung ist Gameplay mit Störungen | 📋 | `23#359A` P17; `23#360E` P17 |
| K-JA-004 | Keine Quick-Time-Rituale als Standard | 📋 | `23#359A` P18; `23#360E` P18 |
| K-JA-005 | Schutzmaßnahmen real, nicht absolut, zerstörbar | 📋 | `23#359A` P25–26; `23#360E` P25–26 |
| K-JA-006 | Artefakte persistent und individuell; können falsch verstanden werden | 📋 | `23#359A` P19–20; `23#360E` P19–20 |
| K-JA-007 | Wissenschaft und Übernatürliches treffen sich; moderne und klassische Hunter-Stile | 📋 | `23#359A` P23–24, D; `23#360E` P23–24, D |
| K-JA-008 | Verfluchte Gegenstände wandern durch die Wirtschaft; Vault; nicht zu häufig | 📋 | `23#1075E` P107–114 |
| K-JA-009 | Temporäre übernatürliche Kräfte mit Konsequenzen | 📋 | `23#359A` P29; `23#360E` P29 |
| K-JA-010 | Originale seltene Jägerwaffen mit IP-sicherer Lore | 📋 | `M-PD` |
| K-JA-011 | Kosten- und Risikomodell der Rituale im Detail | ❓ | Modul 12 §12 |

---

## CO: Lebendfang, Fallen, Containment (Modul 13)

Gruppe zu [13-Lebendfang-Fallen-und-Containment.md](13-Lebendfang-Fallen-und-Containment.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-CO-001 | Gefangennahme ist ein umfassendes System (Option D) | 📋 | `48#128A`; `48#129E` |
| K-CO-002 | Keine universelle Gefängniszelle | 📋 | `48#130A` |
| K-CO-003 | Fallen an beliebigen Orten; Bunker eigenes Planungsthema | 📋 | `48#131E` |
| K-CO-004 | Fallen bleiben persistent und verändern sich durch Umwelt, NPCs, Monster, Zeit | 📋 | `48#132A`; `48#133E`; `48#134A` |
| K-CO-005 | Vorgefertigte, anpassbare Vorlagen statt Freihandzeichnen | 📋 | `48#135E`; `48#136A` |
| K-CO-006 | Gefängnissicherheit passt sich übernatürlichen Bedrohungen an | 📋 | `48#169E` |
| K-CO-007 | Verhör intelligenter Wesen | 📋 | `23#1075E` P97–98 |
| K-CO-008 | Koop-Vorbereitung gemeinsamer Orte | 💡 | `48#132A` |
| K-CO-009 | Dauerhafte Einrichtungen und Netzwerke des Spielers | ❓ | `48#130A` Option D nicht gewählt |
| K-CO-010 | Rechtlicher Rahmen für Verwahrung | ❓ | Modul 13 §4.3 |

---

## FO: Forschung und Bestiarium (Modul 14)

Gruppe zu [14-Forschung-und-Bestiarium.md](14-Forschung-und-Bestiarium.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-FO-001 | Archiv zeigt das Wissen des Hunters, keinen Debugger | 📋 | `23#295A`; `M-PD` |
| K-FO-002 | Statuswörter Confirmed, Suspected, Disputed, Disproven, Historical/Unverified | 📋 | `M-PD` |
| K-FO-003 | Hunter-Wissen kann falsch sein; Begleiter irren | 📋 | `23#307A` P25–26, C; `23#308E` C; `M-PD` |
| K-FO-004 | Monsterwissen aus Vorwissen, Forschung, Notizen und unzuverlässigen Quellen (Option D) | 📋 | `48#124A`; `48#125E` |
| K-FO-005 | Umfassende freie Monsterforschung | 📋 | `48#126A`; `48#127E`; `48#128A` |
| K-FO-006 | Archiv beginnt mit Lücken; Einstieg über Begegnung | 🧭 | `23#1075E` P84 |
| K-FO-007 | Evidenzbasiertes Wissen, Notizen, Medien, Wissensgraph | 📋 | `23#1075E` P85–90 |
| K-FO-008 | Regionale Varianten und Mutationen | 📋 | `23#1075E` P87 |
| K-FO-009 | Spuren: normal zuerst, körperlich, Verhalten, Historie, Wetter, Hund, falsche Beweise, Forensik | 📋 | `23#1075E` P76–83 |
| K-FO-010 | Fortschrittsstufen des Monsters im Hunter Record | 📋 | `M-PD` |
| K-FO-011 | Companion nennt Unbestätigtes nicht als Tatsache | 💡 | `48#126A` |
| K-FO-012 | Schwellen für Hypothese und Bestätigung | ❓ | `M-OD` |

---

## VF: Vorfälle und lebendige Welt (Modul 15)

Gruppe zu [15-Vorfaelle-und-lebendige-Welt.md](15-Vorfaelle-und-lebendige-Welt.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-VF-001 | Ökologie erzeugt Vorfälle; Kette bis Hunter-Hinweis | 📋 | `23#1075E` P62 |
| K-VF-002 | Ignorierte Fälle laufen weiter; Opfer; Weiterziehen; Eingreifen anderer; Selbstlösung; Verschlimmerung | 📋 | `23#1075E` P63–68 |
| K-VF-003 | Nebenmissionen sehr wichtig; sehr viele dynamische Aufträge | 📋 | `23#1075E` P69–70 |
| K-VF-004 | Mehrdeutige Fälle; False Positives; gemischte Ursachen; mehrere Arten; menschlicher Täter | 📋 | `23#1075E` P71–75 |
| K-VF-005 | Vorlaufphasen statt MMO-Spawn | 📋 | `M-PD`; `23#308E` P21 |
| K-VF-006 | Zeitgebundene Inhalte werden nicht billig gelöscht | 📋 | `M-PD`; `23#307A` P22 |
| K-VF-007 | Die Welt erinnert sich, keine automatische Gedächtnislöschung | 📋 | `23#1075E` P29 |
| K-VF-008 | Tatorte behalten Blut und Schäden bis zur Reinigung | 📋 | `23#1075E` P150 |
| K-VF-009 | Auslegung von nur Haupt- und Nebenmissionen bei Mehrdeutigkeit | ❓ | `23#1075E` P71 |

---

## OR: Übernatürliche Orte (Modul 16)

Gruppe zu [16-Uebernatuerliche-Orte-und-Geografie.md](16-Uebernatuerliche-Orte-und-Geografie.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-OR-001 | Haunted Properties mit Geschichte; Immobilienmarkt reagiert indirekt; kaufbar; kehrt wieder; Abriss und Renovierung | 📋 | `23#1075E` P115–120 |
| K-OR-002 | Hotspots; Friedhöfe; Schlachtstätten; Wälder; Seen; Städte | 📋 | `23#1075E` P127, P129–133 |
| K-OR-003 | Metaphysisches Netz ist offen; Assistent empfahl, es nicht als universelle Kartenmechanik zu nutzen | ❓ | `23#1075E` P128; `23#1076A` |
| K-OR-004 | Kartenvorgabe: Übernatürliches nicht sichtbar einzeichnen, natürlich aus Geografie entstehen lassen | 🧭 | `30#245E` |

---

## PD: Persistenz und Modding (Modul 17)

Gruppe zu [17-Persistenz-Daten-und-Modding.md](17-Persistenz-Daten-und-Modding.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-PD-001 | Zentraler persistenter Weltzustand inklusive Monsteraktivität | 📋 | `M-PD` |
| K-PD-002 | Saves segmentiert, inkrementell, absturzsicher, migrierbar; Ziel 20–50 MB | 📋 | `M-PD` |
| K-PD-003 | Monster-Framework stark modbar, additiv, native C++ möglich | 📋 | `23#293A` P23; `23#294E` P23; `M-PD` |
| K-PD-004 | Online ohne Mods; privater Koop mit Cheats | 📋 | `23#551E`; `23#553E`; `23#440E` |
| K-PD-005 | Mods pro Spielstand ein- und ausschaltbar | 📋 | `35#71E` |
| K-PD-006 | Mods ohne externen Internetzugriff | 📋 | `35#137E` |
| K-PD-007 | Welt läuft während Haft und Zeitsprung weiter | 📋 | `48#159E` |
| K-PD-008 | Speichermodell für Gedächtnis und Beziehungen | ❓ | Modul 17 §3 |
| K-PD-009 | Kennzeichnung lore-widriger Mod-Kreaturen | ❓ | Modul 17 §6.3 |

---

## TI: Tiere (Modul 18)

Gruppe zu [18-Tiere-und-Abgrenzung.md](18-Tiere-und-Abgrenzung.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-TI-001 | 220–280 Tier-, Vogel- und Fischarten; Studium braucht bewusste Aktion | 📋 | `M-PD`; `M-B37` |
| K-TI-002 | Haustiere, Farmtiere, Pferde, Babytiere, Roadkill, Jagd, Angeln, Fotografie, Park-Gebiete, Höhlen, Unterwasserwelt | 📋 | `23#1026A` P22–34; `23#1027E` P22–34 |
| K-TI-003 | Normale Tiere reagieren auf Übernatürliches, ohne Detektor zu sein | 📋 | `23#1026A` P35; `23#1027E` P35 |
| K-TI-004 | Hund als Begleiter bei Fällen; Hundesystem | 📋 | `23#1027E` P22; `M-CA0905` §4 |
| K-TI-005 | Verwandelte oder übernatürlich veränderte Tiere | ❓ | Modul 18 §8 |

---

## LW: Lebendige Welt, Gedächtnis, Nemesis (Modul 10 §14)

Gruppe zu [10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md).

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-LW-001 | Verbindliche Produktvision: Living World, NPC-Gedächtnis, Nemesis/Rivalität (Anforderungen A–H); kein M1-Feature; Reihenfolge offen | 📋 | `PE-N1022` |
| K-LW-002 | Überlebende Gegner können zurückkehren, Feindschaften entwickeln, lernen und Taktiken anpassen; auch intelligente übernatürliche Kreaturen | 📋 | `PE-N1022` C |
| K-LW-003 | Autonome Fraktionen (auch Monstergruppen) mit Zielen, Allianzen, Gebietsansprüchen, Strategiewechsel | 📋 | `PE-N1022` D |
| K-LW-004 | Skalierbare Simulation mit Detailstufen; Integration ohne widersprüchliche Zustände | 📋 | `PE-N1022` G, H |
| K-LW-005 | Die Anpassung übernatürlicher Wesen ist durch Artregeln und Schwächen begrenzt, nie freie Erfindung | 💡 | Proposal 0010 §4.4 |
| K-LW-006 | Ereignisjournal als Quelle der Wahrheit; Entitätsdatensätze statt Actors; saatbasiertes Fortschreiben | 💡 | Proposal 0010 §4.1, §4.2, §4.7 |
| K-LW-007 | Ein Besitzer je Tatsache; Querwirkungen als Ereignisse; Invarianten als Tests | 💡 | Proposal 0010 §7 |
| K-LW-008 | Hochfrequenter oder abgeleiteter Simulationszustand läuft nicht durch WorldFacts | 💡 | Proposal 0010 §6 |
| K-LW-009 | Wie lernt ein Nemesis (Regeln oder Daten); was beobachtet der Spieler von Feindschaft | ❓ | Proposal 0010 §11 |

---

## EN: Entwurfsregister (nur unverbindliche Vorschläge)

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| K-EN-001 | Datenmodell der Klassifikation (SpeciesDefinition, IndividualCreature) | 💡 | Modul 03 §9 |
| K-EN-002 | Wirkungsmodell (AttackEvent, SpeciesRule, Beobachtung) | 💡 | Modul 04 §12 |
| K-EN-003 | Zustandsmodelle Geist, Besessenheit, Fluch | 💡 | Modul 05 §6 |
| K-EN-004 | Begegnung mit uralten Wesen als Prozess in sechs Phasen | 💡 | Modul 06 §8 |
| K-EN-005 | Zustandsmodell Person/Seele/Reise | 💡 | Modul 07 §8 |
| K-EN-006 | Simulationsmodell mit Ebenen A, B, C | 💡 | Modul 08 §11 |
| K-EN-007 | Beziehungs- und Fraktionsmodell | 💡 | Modul 09 §8 |
| K-EN-008 | KI-Architektur und Inspektor | 💡 | Modul 10 §11 |
| K-EN-009 | NPC-Hunter als Akteure | 💡 | Modul 11 §9 |
| K-EN-010 | Ritual-, Artefakt-Modell | 💡 | Modul 12 §9 |
| K-EN-011 | Containment-Modell | 💡 | Modul 13 §8 |
| K-EN-012 | KnowledgeClaim-Modell | 💡 | Modul 14 §12 |
| K-EN-013 | Vorfallmodell (IncidentSeed) | 💡 | Modul 15 §7 |
| K-EN-014 | Ortsgeschichte als Daten | 💡 | Modul 16 §6 |
| K-EN-015 | Schlüsselschema und Größenrechnung | 💡 | Modul 17 §4 |
| K-EN-016 | Artenvorlage, Reihenfolgekriterien, Prüfliste | 💡 | Modul 19 §4–6 |

---

**Summe:** 273 Einträge in 20 Gruppen.
