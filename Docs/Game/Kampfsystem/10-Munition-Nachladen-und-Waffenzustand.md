# Kampfsystem 10: Munition, Nachladen und Waffenzustand

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul sammelt die Entscheidungen zu Munitionsarten, Magazinen, Patronenlager, Nachladeabläufen, Störungen und Waffenpflege. Zustandsautomaten und Datenmodelle sind **Entwurf (💡)**. Es gibt keine Zahlen: keine Magazingrößen, keine Wartungsintervalle, keine Störungswahrscheinlichkeiten. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Waffenhandhabung und Ballistik: [Modul 05](05-Fernkampf-und-Schusswaffen.md); Waffenklassen: [Modul 09](09-Waffenklassen-und-Ausruestung.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Munitionsarten und Wirkung](#3-munitionsarten-und-wirkung)
4. [Magazine, Patronenlager und Nachladen](#4-magazine-patronenlager-und-nachladen)
5. [Entwurf: Nachladezustände und Unterbrechung](#5-entwurf-nachladezustände-und-unterbrechung)
6. [Störungen und Waffenzustand](#6-störungen-und-waffenzustand)
7. [Gegenüberstellung offener Lösungsansätze](#7-gegenüberstellung-offener-lösungsansätze)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Munition unterscheidet sich physikalisch,** nicht nur über einen Schadenswert: Vollmantel, Hohlspitz, Buckshot, Birdshot und Slug verhalten sich unterschiedlich (`23#154A`, `23#155E` P1–P4). Spezialmunition gegen Übernatürliches wirkt **nur dort stärker, wo ihre Schwäche passt** (`23#155E` P5).
2. **Das Magazin ist ein Objekt.** Es behält seine tatsächliche Schusszahl; es gibt keinen globalen Munitionspool; Patronenlager und Magazin sind getrennte Zustände; ein angebrochenes Magazin bleibt angebrochen (`M-PD`, `23#86E` P12).
3. **Nachladen ist eine echte, unterbrechbare, persistente Aktion** mit Geräusch, das die KI hören kann (`M-PD`, `23#86E` P1).
4. **Störungen sind selten, werden aber bei Schmutz, Beschädigung, schlechter Wartung oder problematischer Munition wahrscheinlicher;** die Beseitigung bleibt bedienungsfreundlich (eine kontextabhängige Eingabe). Der Entwickler hat sich zum Realismusmodus der Beseitigung nicht festgelegt (`23#157E`).
5. **Wartung ist moderat und realistisch,** nicht ständig; robuste Konstruktionen vertragen mehr (`M-PD`, `23#155E` P19, P21).
6. **Offen:** Magazingrößen und Munitionsbestand, Komfortvariante der Verwaltung, Munitionswirtschaft, Zustandsgrößen der Waffe, Wartungshandlungen und der Grad der Störungsbeseitigung (`CQ-37`, `CQ-38`, `CQ-40`, `CQ-41`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-MU-001 | **Munitionsarten unterscheiden sich physikalisch,** nicht nur durch Schaden: Material, Aufbau, Geschwindigkeit, Wirkung im Gewebe und im Material | 📋 BESCHLOSSEN | `23#154A` P1; `23#155E` P1 (‚genau, das wurde schon öfters besprochen‘); `M-PD` ‚Ballistics‘ | MUN TRF | Werte offen |
| COMBAT-DEC-MU-002 | **Vollmantel- und Hohlspitzmunition** verhalten sich unterschiedlich; expandierende Munition kann eine schwere Wunde reißen (‚das ist dann böse, wenn der Gegner eine riesige Wunde hat‘) | 📋 BESCHLOSSEN | `23#154A` P2–P3; `23#155E` P2–P3 | MUN TRF | – |
| COMBAT-DEC-MU-003 | **Schrotmunition ist komplexer:** Buckshot, Birdshot und Slug verhalten sich klar unterschiedlich; Treffer aus der Nähe können schwerste Verletzungen verursachen (Schrotflinte aus der Nähe ≠ Pistole) | 📋 BESCHLOSSEN | `23#154A` P4; `23#155E` P3–P4; `M-PD` ‚Human anatomy…‘ (Gore-Aussage) | MUN TRF | Streuung: `CQ-19` |
| COMBAT-DEC-MU-004 | **Hunter-Munition baut auf echter Ballistik auf und fügt eine übernatürliche Wirkung hinzu;** die Arten richten nur bei den bestimmten Gegnern mehr Schaden an | 📋 BESCHLOSSEN | `23#154A` P5; `23#155E` P5; `M-PD` ‚Gunplay / weapons‘ | MUN KRE | Wirkungsarten: [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) |
| COMBAT-DEC-MU-005 | **Munition muss zur konkreten Waffe passen,** aber ‚nicht zu komplex‘ | 🧭 RICHTUNG | `23#154A` P6; `23#155E` P6 (‚ok ja aber nicht zu komplex‘) | MUN WAF | Kompatibilitätsregeln offen |
| COMBAT-DEC-MU-006 | **Magazine behalten ihre tatsächliche Schusszahl;** es gibt keinen globalen Munitionspool. Ein vorzeitiges Nachladen lässt das angebrochene Magazin bestehen (nicht ‚im letzten gespawnt‘) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P12; `23#155E` P11 | WAF | – |
| COMBAT-DEC-MU-007 | **Magazine sind physische Objekte** (können liegen, aufgehoben, verloren, abgeworfen werden) | 📋 BESCHLOSSEN | `23#154A` P11, P13; `23#155E` P11 (‚passt gut‘), P13 | WAF PER | Weltobjekt-Regeln: Modul 24 |
| COMBAT-DEC-MU-008 | **Patronenlager und Magazin sind getrennte Zustände** (eine Patrone im Lauf bei leerem Magazin ist möglich) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P12; `23#155E` P12 (‚ja das ist smart‘) | WAF | – |
| COMBAT-DEC-MU-009 | **Taktisches Nachladen behält ein angebrochenes Magazin** (wird verstaut); **Notnachladen** gilt vor allem bei leerem Magazin und wirft das leere Magazin zugunsten der Geschwindigkeit ab. Der Entwickler: ‚emergency reload sollte man nur machen, wenn das Magazin leer ist‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P13; `23#155E` P13 | WAF STE | – |
| COMBAT-DEC-MU-010 | **Ein getrennter ‚Drop Reload‘ zum bewussten Wegwerfen eines halbvollen Magazins** wäre möglich | 💡 VORSCHLAG | `23#156A` (Assistent, ‚eine Ausnahme könnten wir später noch überlegen‘); keine Entwicklerantwort | WAF STE | – |
| COMBAT-DEC-MU-011 | **Röhrenmagazin-Schrotflinten werden Patrone für Patrone geladen,** und das Nachladen ist nach jeder Patrone unterbrechbar | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P14; `23#155E` P14 | WAF | – |
| COMBAT-DEC-MU-012 | **Der Nachladezustand ist persistent und unterbrechbar** | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ | PER WAF | Entwurf: Abschnitt 5 |
| COMBAT-DEC-MU-013 | **Nachladen ist eine echte Aktion mit Geräusch** (Magazin fällt, Waffe nicht feuerbereit); die KI hört es abhängig von Entfernung, Raum, Wänden, Hintergrund, Monstertyp und Schwierigkeit; auf höheren Stufen sind Gegner gnadenlos und hören es ‚auf jeden Fall‘ | 📋 BESCHLOSSEN | `23#86E` P1 (‚auf jeden Fall hören sie es, auf leichteren vielleicht nicht‘); `M-PD` ‚Stealth, perception and horror AI‘; `23#115A` P10 (Entwurf) | AUD KI | [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-MU-014 | **Revolver mit Trommel und Kammern** sind Teil der Waffenbedienung (Auswurf, einzelne Patronen oder Speedloader je Modell) | 🧭 RICHTUNG | `23#115A` P9 (Entwurf); `M-PD` (‚mechanically correct manipulation‘) | WAF | Detail ist Asset-Aufgabe |
| COMBAT-DEC-MU-015 | **Störungen sind selten,** aber wahrscheinlicher bei schlechter Wartung, Verschmutzung, Schaden oder problematischer Munition. Der Entwickler wählt Variante B: ‚selten, aber wenn die Waffe nicht gut gereinigt ist, kann es leichter passieren‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P16; `23#155E` P16 | WAF | – |
| COMBAT-DEC-MU-016 | **Robuste Konstruktionen vertragen Vernachlässigung und Umwelt besser,** sind aber nicht unverwundbar. ‚Manche Waffen funktionieren ohne Probleme, wie die AK47 in echt auch‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (robust designs…); `23#155E` P19 | WAF | – |
| COMBAT-DEC-MU-017 | **Die Störungsbeseitigung bleibt bedienungsfreundlich:** eine kontextabhängige Eingabe löst die passende realistische Handlung aus; keine Waffenstörungs-Kombinationen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#157E`; `23#158A`; `23#159E` | STE WAF | Taste offen |
| COMBAT-DEC-MU-018 | **Grad der Störungsbeseitigung** (vereinfachte Automatik gegenüber echter Fehlerbehebung je Störungsart, auf Grounded als Option) | ❓ OFFEN | `23#156A` (Optionen A und B); `23#157E` (‚das weiß ich noch nicht‘) | STE SCH | `CQ-40` |
| COMBAT-DEC-MU-019 | **Der Waffenzustand wird intern detailliert simuliert** (Sauberkeit, Verschleiß, Beschädigung, Schmierung, Feuchtigkeit), aber nicht als unnötiges Mikromanagement | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (maintenance); `23#154A` P17; `23#155E` P17; `23#156A` | WAF | Größen: `CQ-41` |
| COMBAT-DEC-MU-020 | **Die Wartung ist moderat und realistisch, nicht ständig** (Variante A, ‚realistisch, nicht übertrieben‘) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#154A` P21; `23#155E` P21 | WAF | – |
| COMBAT-DEC-MU-021 | **Nasse und verschmutzte Waffen sind ein Faktor;** Waffen unterscheiden sich in ihrer Toleranz | 📋 BESCHLOSSEN | `23#154A` P19; `23#155E` P19 | WAF | – |
| COMBAT-DEC-MU-022 | **Waffeninspektion:** Waffe und Magazin werden körperlich geprüft; normale Modi dürfen zusätzlich eine Anzeige nutzen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (‚physical weapon and magazine inspection‘); `23#154A` P20; `23#155E` P20 | WAF UI | Anzeige binär: `COMBAT-DEC-FK-012` |
| COMBAT-DEC-MU-023 | **Auf Grounded gibt es weniger Munition und weniger medizinische Ressourcen** | 📋 BESCHLOSSEN | `23#85A` P7; `23#86E` P7 (‚ja das meinte ich auch mit grounded‘); `M-PD` ‚Difficulty / configuration‘ (scarce resources) | SCH | Mengen offen (`CQ-38`) |
| COMBAT-DEC-MU-024 | **Munition und Hülsen sind Spuren und Beweise** (Ballistik, abgelegte Waffen) | 📋 BESCHLOSSEN | `M-PD` ‚Cash / law / evidence‘; `E-RE-002`; `E-SP-007` | ERM POL | Modul 20 |
| COMBAT-DEC-MU-025 | **Auf Grounded kann die Störungsbeseitigung länger dauern, nicht automatisch gelingen und von Verletzung und Waffenkenntnis abhängen** | 💡 VORSCHLAG | `23#158A` (Assistent) | SCH WAF | `CQ-40` |
| COMBAT-DEC-MU-026 | **Komfortvariante der Munitionsverwaltung** (Gesamtzahl der Patronen, das Spiel verwaltet Magazine) als einstellbare Option | ❓ OFFEN | `23#85A` P12 (Option Komfortabel, Assistent); `23#86E` P12 (‚nicht so wie in cod…‘, realistische Verwaltung gewünscht) | SCH UI | `CQ-37` |
| COMBAT-DEC-MU-027 | **Munitionswirtschaft** (Preise, Gewicht, Herstellung, Wiederladen, Bestandsgrenzen, Knappheit) ist nicht entschieden | ❓ OFFEN | Gameplay-Systeme §13 (nennt Ausgaben, keine Preise); Entscheidungsstatus §5 | WAF | `CQ-38` |
| COMBAT-DEC-MU-028 | **Magazingrößen, Patronenzahlen, Kaliberliste und Verträglichkeit** sind nicht beschlossen | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | WAF | `CQ-33` |

---

## 3. Munitionsarten und Wirkung

Die Quellen führen vier Gruppen. Die Tabelle trennt die Aussage der Quelle vom Entwurf.

| Gruppe | Aussage der Quelle | Entwurf | Modul |
|---|---|---|---|
| Vollmantel (FMJ) | Standardmunition mit Durchdringung | – | 05 |
| Hohlspitz | expandiert, reißt große Wunde | – | 12 |
| Schrot | Buckshot, Birdshot, Slug unterschiedlich; Nähe schwerste Wunde | Streuung als Entwurf | 05, 12 |
| Hunter-Munition | physisch plus übernatürlich; nur wirksam, wo die Schwäche passt | Salz-, Silber-, Eisen-ähnliche Wirkungen als Erweiterungspunkte | 16 |

**Hinweis zu Namen:** Die Prompt-Entwürfe `M-P07` nennen ‚silver-like‘, ‚iron-like‘ und ‚salt load‘ ausdrücklich ‚without copied lore‘. Das entspricht `K-IP-003`. Ob ein Wesen Silber, Eisen oder Salz fürchtet, ist Sache der Kreaturenregeln ([Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md)), nicht der Munition.

**Wirkungsarten (🧩 ABGELEITET aus `K-KO-008`):** Hunter-Munition kann tödlich, schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, enthüllend oder fähigkeitsstörend wirken. Die Munitionsdefinition trägt eine **Wirkungsliste**, die nur gegen Wesen mit passender Schwäche greift.

---

## 4. Magazine, Patronenlager und Nachladen

**Datenverantwortung (💡):**

| Objekt | Zustand | Besitzer |
|---|---|---|
| Magazin | stabile Identität, Füllstand, Munitionsart pro Patrone (falls gemischt) | Magazinobjekt |
| Patronenlager | leer, gefüllt, Hülse, Fehlzündung | Waffeninstanz |
| Verschluss | vor, zurück, verriegelt (Slide Lock) | Waffeninstanz |
| Röhrenmagazin | Anzahl und Art der Patronen | Waffeninstanz |
| Trommel | Kammern (Zustand je Kammer) | Waffeninstanz |
| Waffenzustand | Sauberkeit, Verschleiß, Schaden, Schmierung, Feuchtigkeit | Waffeninstanz |

**Nachladearten:**

| Art | Ablauf (Quelle) | Status |
|---|---|---|
| Taktisch | angebrochenes Magazin wird verstaut, neues eingesetzt | 📋 |
| Notnachladen | leeres Magazin abwerfen, neues einsetzen so schnell wie möglich | 📋 |
| Drop Reload | halbvolles Magazin bewusst abwerfen | 💡 |
| Einzelpatronen (Röhre) | Patrone für Patrone, jederzeit abbrechbar | 📋 |
| Revolver | Auswurf, Kammern einzeln oder Speedloader | 🧭 |
| Verschluss bedienen | nach Nachladen bei leerem Lager | 🧭 (mechanisch korrekt, `M-PD`) |

Alle Zeiten sind Konfiguration; die Quellen nennen keine.

---

## 5. Entwurf: Nachladezustände und Unterbrechung

> **💡 VORSCHLAG.** Das Modell beantwortet die Anforderung ‚persistent und unterbrechbar‘ (`COMBAT-DEC-MU-012`) so, dass jeder Zustand speicher- und prüfbar ist.

```text
Bereit (Magazin drin, Patrone im Lager oder nicht)
  -> NachladenStart(Art)
       Taktisch/Not:  MagazinAus  ->  Waffe ohne Magazin
                      MagazinEin  ->  (Lager leer?) VerschlussBedienen -> Bereit
       Röhre:         PatroneEin(n) -> Bereit   (Abbruch nach jeder Patrone möglich)
  -> Abbruch(Zeitpunkt)
       vor MagazinAus : Bereit (unverändert)
       nach MagazinAus: Waffe ohne Magazin (Magazin in Hand oder am Boden, je nach Art)
       nach MagazinEin: Bereit, Verschluss ggf. nicht bedient (Lager leer)
       Röhre          : Bereit mit n+1 Patronen
  -> Speichern/Laden in jedem Zwischenzustand: Zustand + Objekte wie oben, Handlung geht weiter
```

**Prüfbare Eigenschaften (🧩 ABGELEITET):**

1. Die Summe aller Patronen in Welt, Magazinen und Waffen bleibt über jede Nachladefolge konstant (außer Verbrauch durch Schuss).
2. Jeder Abbruchpunkt führt in einen definierten Waffenzustand; es gibt keinen Zustand ‚Magazin verschwunden‘.
3. Ein Notnachladen bei vollem Magazin ist nicht der Normalfall (`COMBAT-DEC-MU-009`); wird es angefordert, wird die Ausführung abgelehnt oder in ein taktisches Nachladen umgewandelt; welche Variante gilt, ist Konfiguration (Entwurf).
4. Abgeworfene Magazine sind Weltobjekte mit Füllstand (`COMBAT-DEC-MU-007`) und erzeugen ein Geräusch (`COMBAT-DEC-MU-013`).

---

## 6. Störungen und Waffenzustand

### 6.1 Störungsklassen (💡 VORSCHLAG)

Die Quellen verlangen realistische Störungen, aber kein Verzeichnis. Der Entwurf des Assistenten nennt drei Klassen (`23#156A`): Fehlzündung (prüfen, erneut bedienen), Zuführstörung (Magazin oder Verschluss), Auswurfstörung (Verschluss). Die Zuordnung zu Waffenfamilien (Pistole, Revolver, Schrotflinte, Gewehr) und zu Behebungsanimationen ist offen.

### 6.2 Einflussgrößen

| Größe | Wirkung | Quelle |
|---|---|---|
| Sauberkeit | Störwahrscheinlichkeit | `M-PD`; `23#155E` P16 |
| Verschleiß, Beschädigung | Störwahrscheinlichkeit, Funktion | `M-PD` |
| Feuchtigkeit, Schlamm | Störwahrscheinlichkeit | `23#154A` P19 (Entwurf), `23#155E` P19 |
| Munitionsqualität | Störwahrscheinlichkeit | `M-PD` (problematische Munition) |
| Schmierung | Reibung, Verschleiß | `M-P07` (Themeninventar ‚weapon-lubrication-state‘) |
| Robustheitsprofil | Toleranz | `M-PD`; `23#155E` P19 |
| Verletzung des Schützen | Behebungsgeschwindigkeit, Erfolgsaussicht | `23#158A` (Entwurf) |

### 6.3 Pflege

Wartung ist moderat und realistisch, nicht ständig (`COMBAT-DEC-MU-020`). Die Handlung ‚Pflege‘ existiert; ihr Ort (Unterschlupf, Werkbank, unterwegs), Material und Dauer sind offen (`CQ-41`).

---

## 7. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Munitionsverwaltung (`CQ-37`) | A: Nur realistische Verwaltung (einzelne Magazine, Patronenlager) | konsistent mit `COMBAT-DEC-MU-006` | Einstieg schwerer |
| | B: Zusätzlich Komfortmodus (Gesamtzahl, das Spiel verwaltet die Magazine) | zugänglich | zwei Datenwege; Gefahr, dass der Komfort die Physik verdeckt |
| Störungsbeseitigung (`CQ-40`) | A: Echte Fehlerbehebung je Störungsart | Realismus, Waffenkenntnis zählt | mehr Animationen, mehr Eingaben, Konflikt zu ‚nicht zu viele Tastenkombinationen‘ |
| | B: Eine kontextabhängige Eingabe, intern die richtige Behebung | passt zu `COMBAT-DEC-MU-017` | weniger Waffenkenntnis als Spielinhalt |
| | C: B als Standard, A als Realismusmodus (Vorschlag des Assistenten) | trennt Zugänglichkeit von Tiefe | Der Entwickler hat sich nicht festgelegt |
| Wartung (`CQ-41`) | A: Wenige Zustandsgrößen (Sauberkeit, Schaden) | geringer Aufwand | weniger Unterschiede zwischen Waffen |
| | B: Viele Größen (Sauberkeit, Verschleiß, Schmierung, Feuchtigkeit, Schaden) | trägt Robustheitsprofil | Gefahr von Mikromanagement (`COMBAT-DEC-MU-019`) |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Waffe ([Modul 09](09-Waffenklassen-und-Ausruestung.md)) | Kaliber, Magazinbindung |
| zum Fernkampf ([Modul 05](05-Fernkampf-und-Schusswaffen.md)) | Projektildefinition |
| zur KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)) | Nachladegeräusch, Magazinwechsel als Signatur |
| zur Persistenz ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | Magazine, Lager, Nachladezustand |
| zu Beweisen ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) | Hülsen, Ballistik |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Magazinbesitz je Spieler, Autorität des Schusses |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Magazin passt nicht zur Waffe | Ablehnung mit Grund | 💡 |
| Nachladen bei Bewusstlosigkeit oder Griff | Ablehnung mit Grund | 💡 |
| Magazin wird beim Abwurf in unzugänglicher Zone verloren | definiertes Verhalten mit Protokoll | 💡 |
| Speichern während des Verschlussbedienens | Zustand gespeichert, Handlung fortgesetzt | 📋 |
| Hülse und Waffe aus verschiedenen Spielständen (Mod-Fall) | Ablehnung mit Meldung | 💡 |
| Doppelte Patronenerzeugung durch Netzwerkwiederholung | verhindert (Eindeutigkeit der Magazin-ID, Modul 23) | 💡 |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-37` | Komfortmodus der Munitionsverwaltung als Option | `COMBAT-DEC-MU-026` | ja |
| `CQ-38` | Munitionswirtschaft: Knappheit, Preise, Gewicht, Herstellung, Wiederladen, Grounded-Mengen | `COMBAT-DEC-MU-023`, `-027` | ja |
| `CQ-40` | Störungsbeseitigung: Automatik gegenüber Handgriff, Standard je Schwierigkeit | `COMBAT-DEC-MU-018`, `-025` | ja |
| `CQ-41` | Waffenzustand: Zustandsgrößen, Pflegehandlung, Ort und Material | `COMBAT-DEC-MU-019`, `-020` | ja |
