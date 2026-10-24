# Kampfsystem 06: Steuerung und Eingabekontext

> [!IMPORTANT]
> **Nichts davon ist implementiert, bis auf die in Abschnitt 2 genannten Tastenbelegungen der Testkarte.** Dieses Modul trennt drei Dinge scharf: (1) was die Quellen über **Steuerung als Prinzip** entscheiden, (2) was nur der **Auftrag der Projektleitung vom 23.10.2026** über die DualSense-Kampfbelegung sagt (Status 🔍 UNBELEGT, weil keine Quelle im Repo, Master oder Chat die Entwicklerentscheidung trägt), und (3) was als **Entwurf** neben die offenen Fragen gestellt wird. **Die Dreieck-Belegung ist ausdrücklich nicht beschlossen.** Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Bestand: was heute belegt ist](#2-bestand-was-heute-belegt-ist)
3. [Beschlossene und offene Anforderungen](#3-beschlossene-und-offene-anforderungen)
4. [Die DualSense-Vorgaben des Auftrags](#4-die-dualsense-vorgaben-des-auftrags)
5. [Kontextabhängige Tasten: Gegenüberstellung](#5-kontextabhängige-tasten-gegenüberstellung)
6. [Eingabekontexte und Prioritäten (Entwurf)](#6-eingabekontexte-und-prioritäten-entwurf)
7. [Belegungsrahmen nach Aktionen](#7-belegungsrahmen-nach-aktionen)
8. [Maus, Tastatur, weitere Geräte](#8-maus-tastatur-weitere-geräte)
9. [Schnittstellen und Fehlerfälle](#9-schnittstellen-und-fehlerfälle)
10. [Offene Punkte dieses Moduls](#10-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Als Prinzip steht viel fest:** vollständige Controller-Unterstützung ohne Zweitklassigkeit, vollständiges Remapping, kontextabhängige und lernbare Eingaben statt Tastentabellen, analoges Schleichen über die Stickauslenkung, Hilfen einzeln abschaltbar (`23#63A`, `23#64E`, `M-PD`).
2. **Die konkrete Kampfbelegung steht nirgends in den Quellen.** Belegt sind: Kreuz = Springen (Entwickler, 22.10.2026), eine Fahrbelegung mit R2, L2, L1, Kreuz und Quadrat (Entwickler, 15.10.2026) und die Tastaturbelegung der Testkarte. Quadrat, Kreis und Dreieck für den Kampf stehen **nur im Auftrag** (`CU-01`).
3. **Quadrat trägt im Auftrag zwei Bedeutungen** (Kontextinteraktion außerhalb des Kampfes, normaler Angriff bei Gegnernähe) und in der Fahrbelegung eine dritte (Herunterschalten). Das ist kein Widerspruch zwischen Quellen, aber ein **ungeklärtes Umschaltverhalten** (`CQ-25`).
4. **Der Kampf hat eine eigene Steuerungslinie.** Die Hybrid-Steuerung aus Aktionsbibliothek plus analoger Feinkontrolle wurde für das **Adult-System** gelockt; für den Kampf bleibt die Frage Direct oder Assisted ausdrücklich offen (`35#792A`, `35#797E`; `CQ-02`).
5. **Eine Spannung ist unaufgelöst:** Der Entwickler will keine Tastentabelle (`23#171E`), aber auch eine Aktionsbibliothek mit Tastenkombinationen, Beschreibung und Vorschau (`35#791E` P3, `35#798A`). Siehe `CK-03`.

---

## 2. Bestand: was heute belegt ist

| Bindung | Wert | Quelle | Status |
|---|---|---|---|
| Springen, Tastatur | Leertaste | Proposal 0009 §3a (`IMC_Default`) | ✅ implementiert (Testkarte) |
| Springen, Gamepad | `Gamepad_FaceButton_Bottom` (DualSense-Kreuz auf Windows) | Proposal 0009 §3a | ✅ implementiert; Entwicklerentscheidung vom 22.10.2026: Kreuz ist die geplante Sprungtaste |
| Sprinten, Tastatur | Shift halten (Platzhalterwert) | README §26.1; Proposal 0009 §3a | ✅ implementiert |
| Sprinten, Gamepad | **keine Belegung** | Proposal 0009 §3a | ✅ Bestand: fehlt |
| Interagieren, Tastatur | E | README §26.1 | ✅ implementiert |
| Interagieren, Gamepad | **keine Belegung** | Proposal 0009 §3a | ✅ Bestand: fehlt |
| Fahren (DualSense) | R2 Gas, L2 Bremse, L1 + L2 analoge Kupplung, linker Stick Lenkung, rechter Stick Kamera, Kreuz/X Hochschalten, Quadrat Herunterschalten, alles frei belegbar | Gameplay-Systeme §6.3 (Entwickler 15.10.2026) | 📋 beschlossen, nicht implementiert; Prototyp muss L2-Doppelrolle prüfen |
| Fahren (Xbox-artig, früher) | RT Gas, LT Bremse, A Hochschalten, X Herunterschalten, B Handbremse, Y öffnen, LB Kupplungsmodifikator | Gameplay-Systeme §6.3; `23#69A` | 📋 beschlossen als früheres Konzept |

Alle anderen Kampfbelegungen sind **nicht implementiert und nicht belegt**. Die Testkarte kennt weder Angriff, Ausweichen, Zielen noch Schießen.

---

## 3. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-ST-001 | **Vollständige Controller-Unterstützung ohne Zweitklassigkeit:** Menüs, Inventar, Archiv, Ermittlung, Karten, Dialoge, Fahrzeuge, Kampf, Oberfläche, Modbrowser, Zugänglichkeit, Tastensymbole, Neubelegung, Vibration und Haptik, sinnvolle Radialmenüs funktionieren ohne Maus | 📋 BESCHLOSSEN | `23#63A` (Abschnitt 10); `23#64E` P10 (‚ja genau‘); `M-PD` ‚Vehicles‘ (full controller support) | STE UI | Radialmenüs: Entwurf; Haptik: `CQ-23` |
| COMBAT-DEC-ST-002 | **Jede Belegung ist frei neu belegbar;** das Spiel ist ‚generell alles konfigurierbar‘ | 📋 BESCHLOSSEN | `23#64E` P8 (Fahrbelegung ‚alles konfigurierbar‘) und Nachsatz; `M-PD` ‚Vehicles‘ (fully remappable) | STE | Die Aussage ist für die Fahrbelegung explizit, für den Kampf aus dem Nachsatz abgeleitet |
| COMBAT-DEC-ST-003 | **Steuerung ist kontextabhängig und lernbar;** der Spieler lernt keine großen Kombotabellen. Position, Distanz, Griff, Richtung und Kontext bestimmen die sinnvollen Aktionen | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#171E`; `COMBAT-DEC-NK-031` | STE | Spannung zu `35#798A`: `CK-03` |
| COMBAT-DEC-ST-004 | **Eine kontextabhängige Eingabe löst die richtige realistische Handlung aus** (zum Beispiel Nachladen und Störungsbeseitigung auf derselben Eingabe); keine Waffenstörungs-Kombinationen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#157E`; `23#159E` | STE WAF | Taste offen |
| COMBAT-DEC-ST-005 | **Langsames, leises Gehen entsteht kontinuierlich durch geringe Stickauslenkung,** nicht durch eine Pflicht-Schleichtaste | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#240E` A (‚ja indem man einfach den Stick weniger bewegt geht man langsamer‘) | STE KI | Tastatur-Entsprechung offen (`CQ-24`) |
| COMBAT-DEC-ST-006 | **Befehle und Handzeichen an Begleiter** (folgen, warten, dorthin, Deckung, beobachten, flankieren, Tür sichern, vor, zurück, leise) sind kontextabhängig; ohne Sichtkontakt dienen Stimme, Funk oder Telefon und können hörbar sein | 📋 BESCHLOSSEN | `M-PD` ‚Companion autonomy…‘; `23#254E` P17 | STE NPC | Eingabeform (Radialmenü, Taste, Stimme) offen |
| COMBAT-DEC-ST-007 | **Tarn-HUD und Erkennungsanzeigen sind optional;** Grounded schaltet sie standardmäßig ab | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#226E` P27 (‚A: ich würde es wieder einschaltbar machen, aber Grounded aus im Normalfall‘) | UI SCH | – |
| COMBAT-DEC-ST-008 | **Hilfen sind einzeln einstellbar und gehören zur Schwierigkeit ODER zur Zugänglichkeit;** Barrierefreiheit ist eine eigene Ebene (Aim-Hilfe und Eingabehilfen für Motorik sind nicht dasselbe wie eine Schwierigkeitsstufe) | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `COMBAT-DEC-GP-016` | SCH UI | Liste: `CQ-04` |
| COMBAT-DEC-ST-009 | **Springen liegt auf Kreuz (DualSense)** | 📋 BESCHLOSSEN | Proposal 0009 §3a (Entwickler, 22.10.2026) | BEW STE | Hat Vorrang vor jeder Entwurfsbelegung in diesem Modul |
| COMBAT-DEC-ST-010 | **Fahren belegt Kreuz und Quadrat anders als zu Fuß** (Kreuz Hochschalten, Quadrat Herunterschalten im Fahrzeug) | 📋 BESCHLOSSEN | Gameplay-Systeme §6.3 (Entwickler, 15.10.2026) | STE | Dass dieselben Tasten je Modus unterschiedlich wirken, ist nicht ausdrücklich entschieden (`CQ-25`) |
| COMBAT-DEC-ST-011 | **Quadrat: Kontextinteraktion außerhalb des Kampfes beziehungsweise normaler Angriff bei Gegnernähe** | 🔍 UNBELEGT | nur `AUF` §5 (Modul 06) | STE | `CU-01`, `CQ-01`, `CQ-25` |
| COMBAT-DEC-ST-012 | **Kreis: Ausweichen** | 🔍 UNBELEGT | nur `AUF` §5 (Modul 06) | STE | `CU-01`, `CQ-01`; Xbox-Fahrkonzept nutzte B (auf der DualSense Kreis) für die Handbremse (Gameplay-Systeme §6.3) |
| COMBAT-DEC-ST-013 | **Dreieck: möglicherweise schwerer Angriff;** ausdrücklich **nicht** als endgültig beschlossen zu behandeln | 🔍 UNBELEGT | `AUF` §5 (Modul 06): ‚Die Dreieck-Belegung ist nicht als endgültig beschlossen zu behandeln‘ | STE | `CU-01`, `CQ-01` |
| COMBAT-DEC-ST-014 | **Direct Control gegenüber Assisted Control im Kampf** ist offen | ❓ OFFEN | `35#791E` P1, P2, P4, P6; `35#792A` | STE | `CQ-02`; siehe `COMBAT-DEC-NK-040` |
| COMBAT-DEC-ST-015 | **Spannung: keine Tastentabelle (`23#171E`, `M-PD`) gegenüber Aktionsbibliothek mit Tastenkombination (`35#791E` P3, `35#798A`)** | ⚠️ KONFLIKT | `23#171E`; `M-PD` ‚Melee / grappling‘; `35#791E` P3; `35#798A` | STE | `CK-03` |
| COMBAT-DEC-ST-016 | **Haptik und adaptive Trigger im Kampf** (Rückstoß, Treffer, Nachladen, Verletzung) sind nicht entschieden; die Quellen führen nur Vibration und Haptik als Controller-Anforderung und beim Fahren Rückmeldungen über R2, L2 und Oberflächen | ❓ OFFEN | `23#63A` (Abschnitt 10); `23#515A` (Entwurf des Assistenten, Fahren) | STE AUD | `CQ-23` |
| COMBAT-DEC-ST-017 | **Belegung für Maus und Tastatur im Kampf** (Angriff, Block, Ausweichen, Zielen, Schießen, Greifen) ist nicht entschieden; belegt sind nur Leertaste, Shift, E | ❓ OFFEN | Proposal 0009 §3a; README §26.1 | STE | `CQ-24` |
| COMBAT-DEC-ST-018 | **Eingabeprioritäten bei gleichzeitigen Absichten** (Springen gegenüber Ausweichen gegenüber Interagieren, Angriff gegenüber Interaktion bei Gegnernähe) sind nicht entschieden | ❓ OFFEN | `AUF` §5; `AUF` §8 (‚Verhalten kontextabhängiger Tasten‘) | STE | `CQ-25` |
| COMBAT-DEC-ST-019 | **Eingabekontext-Modell:** Die Absichtsauflösung entscheidet nach Kontext (Fortbewegung, Fernkampfbereitschaft, Schlagabtausch, Griff/Boden, Fahrzeug, Menü, Dialog, Behandlung); jede Ablehnung nennt einen Grund | 💡 VORSCHLAG | Abschnitt 6 (Entwurf); `COMBAT-DEC-ZA-012` | STE | – |
| COMBAT-DEC-ST-020 | **Kampf in VR** ist nicht beschrieben; VR soll dieselbe Interaktions-, Physik- und Körperbasis nutzen und später kommen | ❓ OFFEN | `35#795E`, `35#796A` (für Interaktionen; Kampf nicht erwähnt) | STE | `CQ-26` |

---

## 4. Die DualSense-Vorgaben des Auftrags

Der Auftrag der Projektleitung (§5, Datei 06) nennt als ‚bisherige DualSense-Planung‘:

| Taste | Vorgabe des Auftrags | Belegt in einer Quelle? |
|---|---|---|
| Quadrat | Kontextinteraktion außerhalb des Kampfes beziehungsweise normaler Angriff bei Gegnernähe | **nein** (Fahrzeug: Herunterschalten) |
| Kreis | Ausweichen | **nein** |
| Kreuz | Springen | **ja**, Proposal 0009 §3a |
| Dreieck | möglicherweise schwerer Angriff, nicht als beschlossen zu behandeln | **nein**; Auftrag sagt selbst ‚nicht beschlossen‘ |

**Umgang in dieser Dokumentation:**

1. Kreuz = Springen ist 📋 BESCHLOSSEN (`COMBAT-DEC-ST-009`).
2. Quadrat, Kreis und Dreieck sind als **Eingaben der Projektleitung** übernommen, aber 🔍 UNBELEGT, weil keine auffindbare Entwicklerentscheidung in Repo, Master oder Chats existiert (Suche am 23.10.2026, [Modul 01](01-Quellenlage-und-Methodik.md) §6 F8, §8).
3. Die Dokumentation baut **keine** weitere Mechanik auf diesen Belegungen auf, ohne den Vorbehalt zu nennen; wo Module 04 und 07 auf Angriff und Ausweichen verweisen, steht die Taste nur als ‚laut Auftrag‘.
4. Falls die Projektleitung die Belegung bestätigt, ändert sich nur der Status der drei Zeilen; keine andere Zeile hängt davon ab (`CQ-01`).

**Wechselwirkungen, die bei Bestätigung zu klären sind:**

| Punkt | Hinweis |
|---|---|
| Kreis = Ausweichen und Handbremse | Im früheren Xbox-Fahrkonzept lag die Handbremse auf B (DualSense: Kreis, Gameplay-Systeme §6.3); beim Fahren gilt ein anderer Kontext, die Doppelbelegung ist moduspezifisch |
| Quadrat = Herunterschalten beim Fahren | Im Fahrzeug gilt die Fahrbelegung (`COMBAT-DEC-ST-010`); ob beim Aus- und Einsteigen oder beim Fahren mit Waffe ein Wechsel nötig ist, ist offen (`CQ-18`) |
| Quadrat = Interaktion und Angriff | Gegnernähe als Umschaltkriterium ist nicht definiert (Abschnitt 5) |
| Dreieck schwer | Der Entwickler hat leichte und schwere Angriffe beschlossen (`COMBAT-DEC-NK-002`), aber nie die Taste |
| Ausweichen auf Kreis | Der Entwickler hat Ausweichen als Schritt, Seitwärtsbewegung, Ducken, Körperdrehung beschlossen (keine Roll-Spam-Taste, [Modul 07](07-Ausweichen-Blocken-Parieren-und-Konter.md)); die Taste ist nicht beschlossen |

---

## 5. Kontextabhängige Tasten: Gegenüberstellung

Der Auftrag verlangt, das ‚Verhalten kontextabhängiger Tasten‘ als offene Frage zu führen. Die konkrete Frage (am Beispiel Quadrat): **Wann wirkt eine Taste als Interaktion, wann als Angriff?**

| Ansatz | Umschaltkriterium | Vorteil | Nachteil | Bezug zu Quellen |
|---|---|---|---|---|
| A: Bedrohungszustand | Angriff, sobald ein Gegner den Spieler entdeckt hat (bestätigtes Ziel) oder der Spieler Kampfbereitschaft hergestellt hat | stimmt mit der Speichersperre überein (`COMBAT-DEC-GP-028`); keine Fehlauslösung durch freundliche NPCs | Kriterium hängt an KI-Zuständen und deren Millisekunden-Übergang (`CQ-09`) | `23#370A` (Detected-Zustand, Entwurf) |
| B: Entfernung zu feindlichem Ziel | Angriff, wenn ein als feindlich bekannter Körper unter einer Distanz ist | einfach verständlich | Fehlauslösung bei Nähe zu neutralen NPCs; Distanz ist ein Balancewert | `AUF` §5 (‚bei Gegnernähe‘) |
| C: Expliziter Modus | Kampfhaltung wird gezielt eingenommen (Waffe ziehen, Hände heben); in ihr gilt Angriff | klar für den Spieler | zusätzliche Eingabe vor dem Angriff; langsam im Überfall | `M-PD` (fließende Übergänge) |
| D: Zielpriorität | die Taste wirkt auf das wahrscheinlichste Ziel (Gegner, Tür, Gegenstand), mit sichtbarem Hinweis | wenig Moduswechsel | Mehrdeutigkeit bei Gedränge | `M-PD` (Tarn-HUD optional, Kontexthinweise) |
| E: Mehrere Tasten | getrennte Tasten für Interaktion und Angriff | eindeutig | braucht mehr Tasten; widerspricht ‚weniger Tasten‘ (`23#157E`) | – |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen. Die Wahl bleibt der Projektleitung (`CQ-25`).

**Prüfbare Mindestanforderung (🧩 ABGELEITET aus `COMBAT-DEC-ST-003` und `COMBAT-DEC-GP-009`):** Gleich welcher Ansatz gilt, muss jede Eingabe entweder eine Handlung auslösen oder mit einem **benannten Grund** abgelehnt werden, und eine Handlung darf nicht zwei Bedeutungen im selben Zustand haben.

---

## 6. Eingabekontexte und Prioritäten (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-ST-019`).** Der Entwurf folgt dem bestehenden Eingabemuster: Rohgeräte werden zu engine-neutralen Aktionen, die Absichtsauflösung (Schicht L1, [Modul 03](03-Kampfzustaende-und-Systemarchitektur.md) §3) entscheidet.

### 6.1 Kontexte

| Kontext | Eingaben, die gelten (Beispiele aus den Quellen) |
|---|---|
| Fortbewegung | Bewegung, Kamera, Springen, Sprinten, Schleichen (Stick), Interaktion |
| Fernkampfbereitschaft | Zielen/Anschlag, Schießen, Nachladen/Störung (eine Eingabe), Waffenwechsel |
| Schlagabtausch | leichter/schwerer Angriff, Block, Ausweichen, Fußarbeit, Greifen |
| Griff und Boden | Grapple-Absicht (Position bestimmt die Handlung), Befreiung, Aufstehen |
| Fahrzeug | Fahrbelegung (`COMBAT-DEC-ST-010`) |
| Behandlung | Verband, Reinigung, Medikament (Interaktion, [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md)) |
| Menü, Radial, Dialog | gesperrte Kampfeingaben, Modalität |

### 6.2 Vorschlag für Vorrangregeln

1. Eine modale Oberfläche (Menü, Dialog) verbraucht alle Kampfeingaben.
2. Eine zwingende laufende Aktion (Griff-Befreiung, Gegriffen, Bewusstlosigkeit) schränkt die verfügbaren Absichten ein.
3. Kampfaktionen haben Vorrang vor Traversal, solange ein Bedrohungszustand besteht.
4. Traversal (Springen) hat Vorrang vor Interaktion, wenn eine Sprungabsicht vorliegt (Proposal 0009: der Sprung wird **aktiv** ausgelöst und nie automatisch).
5. Bewegung läuft immer weiter, außer ein Zustand blockiert sie (Griff, Kampfunfähigkeit).

**Eingebaute Prüffragen:** Welche Absicht gewinnt, wenn zwei gleichzeitig erlaubt sind? Kann der Spieler sie vorhersagen? Ist die Ablehnung erklärbar (Grund, Rückmeldung)?

### 6.3 Rückmeldung bei Verletzung

Verletzungen verändern Zielen, Greifen und Waffeneinsatz (`COMBAT-DEC-FK-002`, `COMBAT-DEC-NK-026`). Die Rückmeldung soll über Animation, Audio, Atmung, Sicht, Zittern, Stimme und Handlungsfähigkeit kommen, nicht über Zahlenanzeigen (`M-PD` ‚Human anatomy…‘). Die Steuerung selbst bleibt konsistent: Eine Handlung ist verfügbar, eingeschränkt oder abgelehnt (mit Grund), nie unsichtbar verändert.

---

## 7. Belegungsrahmen nach Aktionen

Die Tabelle zeigt, welche Aktionen die Quellen verlangen und wo eine Belegung belegt ist. **Keine leere Zelle ist ein Vorschlag.**

| Aktion | Anforderung (Quelle) | DualSense | Maus/Tastatur | Belegungsstatus |
|---|---|---|---|---|
| Bewegen, Kamera | Standard | linker/rechter Stick | – | Bestand (Template) |
| Springen | `COMBAT-DEC-ST-009` | Kreuz | Leertaste | 📋 |
| Sprinten | Bestand | offen | Shift | Gamepad offen |
| Interagieren | Bestand | Auftrag: Quadrat (außerhalb des Kampfes) | E | Gamepad 🔍 |
| Leichter Angriff | `COMBAT-DEC-NK-002` | Auftrag: Quadrat (bei Gegnernähe) | offen | 🔍 |
| Schwerer Angriff | `COMBAT-DEC-NK-002` | Auftrag: Dreieck (möglicherweise) | offen | 🔍 |
| Ausweichen | `COMBAT-DEC-DF-006` ([Modul 07](07-Ausweichen-Blocken-Parieren-und-Konter.md)) | Auftrag: Kreis | offen | 🔍 |
| Blocken | `M-PD` | offen | offen | ❓ |
| Greifen (Grapple) | `COMBAT-DEC-NK-031` | offen | offen | ❓ |
| Zielen (ADS) | `COMBAT-DEC-FK-004` | offen | offen | ❓ |
| Schießen | `COMBAT-DEC-FK-002` | offen (R2 beim Fahren ist Gas) | offen | ❓ |
| Nachladen/Störung | `COMBAT-DEC-ST-004` | offen | offen | ❓ (eine Eingabe) |
| Waffenwechsel | `COMBAT-DEC-NK-023` | offen | offen | ❓ |
| Combat Focus | `COMBAT-DEC-GP-021` | offen | offen | ❓ |
| Behandeln | Medizin ([Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md)) | offen | offen | ❓ |
| Befehle an Begleiter | `COMBAT-DEC-ST-006` | offen | offen | ❓ |
| Schleichen | `COMBAT-DEC-ST-005` | Stickauslenkung | offen | 📋 (Prinzip) |
| Fahrzeug | Fahrbelegung | R2, L2, L1, Kreuz, Quadrat, Sticks | offen | 📋 (Gameplay-Systeme §6.3) |

---

## 8. Maus, Tastatur, weitere Geräte

1. **Maus und Tastatur:** Die Quellen verlangen volle Unterstützung (implizit durch ‚Controller darf nicht zweite Klasse sein‘) und Neubelegung, nennen aber keine Kampfbelegung (`COMBAT-DEC-ST-017`).
2. **Zielhilfe:** Optionale weiche Zielhilfe ist beschlossen (`COMBAT-DEC-NK-006`), Stärke und Gerätunterschiede nicht (`CQ-15`). Eine Unterscheidung nach Gerät (Controller gegenüber Maus) ist plausibel, aber nicht beschlossen.
3. **Lenkrad, Pedale, H-Schalter:** Enthusiasten-Option für Fahrzeuge (Gameplay-Systeme §6.3), nicht für den Kampf.
4. **VR:** nicht für den Kampf entschieden (`COMBAT-DEC-ST-020`).
5. **Zugänglichkeit:** Barrierefreiheit ist eigene Ebene, Eingabehilfen werden in Grounded nicht gesperrt (`M-P10`: Themeninventar ‚input-accessibility-not-blocked-by-grounded‘, nur Entwurf; der Grundsatz ‚nicht mit Schwierigkeit verwechseln‘ ist 📋, `COMBAT-DEC-GP-016`).

---

## 9. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Absichtsauflösung ([Modul 03](03-Kampfzustaende-und-Systemarchitektur.md)) | Rohaktion + Kontext → Handlung oder Ablehnung mit Grund |
| zur Oberfläche | Kontexthinweise (optional), Symbole je Gerät, Einstellungen |
| zur Animation ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) | gewählte Handlung → Animation |
| zum Fahrzeug | Wechsel Fuß ↔ Fahrzeug |
| zu Accessibility | Eingabehilfen unabhängig von Schwierigkeit |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Zwei Absichten gleichzeitig möglich | deterministische Vorrangregel, Ablehnung der anderen mit Grund | 💡 |
| Taste ist doppelt belegt (Kampf/Fahrzeug) | der aktive Kontext entscheidet; beim Moduswechsel keine „hängende“ Eingabe | 💡 |
| Eingabe während Bewusstlosigkeit | keine Wirkung, kein Fehler | 💡 |
| Verletzung macht Handlung unmöglich (Arm gebrochen) | Ablehnung mit Grund und Rückmeldung, nicht stumme Wirkungslosigkeit | 💡 |
| Neubelegung erzeugt Konflikt | Hinweis beim Speichern der Belegung | 💡 (`35#790A` P15 als Entwurf für Adult) |

---

## 10. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-01` | Gesamtbelegung für den Kampf (DualSense): Bestätigung der Auftragsvorgaben Quadrat, Kreis, Dreieck und Festlegung der übrigen Aktionen | `COMBAT-DEC-ST-009` bis `-013` | ja |
| `CQ-02` | Steuerungsphilosophie im Kampf: Direct Control, Assisted Control oder etwas Drittes | `COMBAT-DEC-ST-014`, `COMBAT-DEC-NK-040` | ja |
| `CQ-23` | Haptik und adaptive Trigger im Kampf (Rückstoß, Treffer, Nachladen, Verletzung), Stärke einstellbar | `COMBAT-DEC-ST-016` | ja |
| `CQ-24` | Belegung von Maus und Tastatur im Kampf einschließlich Schleichen und Zielen | `COMBAT-DEC-ST-017` | ja |
| `CQ-25` | Umschaltverhalten kontextabhängiger Tasten (Interaktion gegenüber Angriff, gleiche Taste im Fahrzeug) und Eingabeprioritäten | `COMBAT-DEC-ST-010`, `-011`, `-018` | ja |
| `CQ-26` | Kampf in VR: Eingabe, Körperbasis, Zeitpunkt | `COMBAT-DEC-ST-020` | ja |
