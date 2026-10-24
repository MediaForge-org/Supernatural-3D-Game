# Kampfsystem 18: Gegner-KI und Kampftaktik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul sammelt die Entscheidungen zu Wahrnehmung, Wissen, Suche, Taktik, Moral und Zusammenarbeit der Gegner im Kampf. Die tiefere Monster-KI steht in [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md); hier geht es um die Kampfseite. Zustandsmodelle sind **Entwurf (💡)**. Es gibt keine Reichweiten, Zeiten oder Schwellen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Wahrnehmung und Wissen](#3-wahrnehmung-und-wissen)
4. [Suche und Rückfall in die Tarnung](#4-suche-und-rückfall-in-die-tarnung)
5. [Bedrohungsbewertung, Taktikwahl und Anpassung (Entwurf)](#5-bedrohungsbewertung-taktikwahl-und-anpassung-entwurf)
6. [Menschliche Gegner: Moral, Kommunikation, Gruppen](#6-menschliche-gegner-moral-kommunikation-gruppen)
7. [Schwierigkeit](#7-schwierigkeit)
8. [Gegenüberstellung offener Lösungsansätze](#8-gegenüberstellung-offener-lösungsansätze)
9. [Schnittstellen und Fehlerfälle](#9-schnittstellen-und-fehlerfälle)
10. [Offene Punkte dieses Moduls](#10-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Gegner wissen nicht einfach, wo der Spieler ist.** Wahrnehmung beruht auf Unsicherheit; Geräusche liefern Richtung, Entfernung und Unsicherheit, keine Koordinate (`M-PD` ‚Stealth, perception and horror AI‘, `23#225A`, `23#239A`).
2. **Das Wissen der KI hat vier Grade** (bestätigt, starker Verdacht, schwacher Verdacht, unbekannt) und wird nur aus Beobachtetem oder plausibel Gefolgertem gespeist; keine globale Anti-Spieler-Telemetrie (`23#242E`, `23#243A`).
3. **Menschliche Gegner teilen Wissen nur über plausible Kommunikation;** es gibt kein Schwarmbewusstsein (`M-PD`).
4. **Intelligente Monster haben einen Taktikvorrat,** der kontextabhängig und adaptiv gewählt wird, nicht zufällig rotiert (`M-PD`, `23#253A`, `23#254E`).
5. **Kampf kann in Suche zurückfallen,** wenn die Sichtlinie bricht und die Position verloren ist (`M-PD`).
6. **Schwierigkeit verändert Aufmerksamkeit, Reaktion, Suche und Aggression, nicht ein Supergehör** (`23#239A`, `23#226E` P21).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-KI-001 | **Wahrnehmung mit Unsicherheit statt magischer Spielerkoordinaten;** ein Geräusch liefert Richtung, Entfernung, Unsicherheit, Lautstärke, Art und Zeit, keine Position | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#239A` §1 (Entwurf); `23#240E` | KI AUD | Modell: Abschnitt 3 |
| COMBAT-DEC-KI-002 | **Vier Wissensgrade** (bestätigt, starker Verdacht, schwacher Verdacht, unbekannt) für Position, Verletzung, Waffen, Fluchtwege, Ablenkungen, Begleiter, Verstecke | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#242E`; `23#253A` P1; `K-KI-003` | KI | – |
| COMBAT-DEC-KI-003 | **Adaptive Taktiken nutzen nur Beobachtetes oder plausibel Gefolgertes;** keine globale Anti-Spieler-Telemetrie, kein verborgenes Metawissen (‚KI darf intelligent schließen, aber nicht cheaten‘ als Formulierung des Assistenten) | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#242E`; `K-KI-013` | KI | – |
| COMBAT-DEC-KI-004 | **Wahrnehmungszustände** mindestens unaware, suspicious, investigating, alert, confirmed target, searching | 🧭 RICHTUNG | `M-PD` (‚may include‘); `COMBAT-DEC-ZA-003` | KI | Schwellen: `CQ-09` |
| COMBAT-DEC-KI-005 | **Die Sicherheit der letzten bekannten Position zerfällt mit der Zeit;** Gegner verlieren Genauigkeit, aber nicht das Wissen, dass eine bestätigte Bedrohung existiert | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` P2; `K-KI-004` | KI | – |
| COMBAT-DEC-KI-006 | **Suchphasen** von sofortiger Verfolgung über lokale und erweiterte Suche und Einkesselung bis zu vorsichtiger Wachsamkeit; Dauer und Intelligenz schwierigkeitsabhängig | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` P3; `23#226E` P11; `K-KI-005` | KI SCH | – |
| COMBAT-DEC-KI-007 | **Kampf kann in Suche und Tarnung zurückfallen,** wenn die Sichtlinie bricht und die genaue Position verloren ist | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` P13; `K-KI-010` | KI | – |
| COMBAT-DEC-KI-008 | **Schall:** Lautstärke, Dauer und Frequenz, Entfernung, Raumakustik, Wände, Türen, Materialien, Hintergrundlärm, Wetter, Böden und Hörvermögen; Kategorien (Schritt, Nachladen, Magazinabwurf, Tür, Glasbruch, Stimme, Schuss, Motor, geworfener Gegenstand) werden verschieden gedeutet | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; Gameplay-Systeme §7 | KI AUD | Raum-/Portalmodell: Audio-Planung |
| COMBAT-DEC-KI-009 | **Sicht:** Richtung, Reichweite, Licht, Bewegung, Deckung, Silhouette, Haltung, Wetter; Dunkelheit verringert die Sicht, ohne Unsichtbarkeit zu gewähren; ein im Dunkeln gehörter Spieler kann grob geortet werden, ohne identifiziert zu sein | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#226E` P7 | KI | – |
| COMBAT-DEC-KI-010 | **Hören und Sehen unterscheiden sich je Art;** Supergehör ist keine Schwierigkeitsmechanik | 📋 BESCHLOSSEN | `23#239A` §5–§6 (Entwurf); `23#240E` P5, P6; `23#226E` P21 (‚realistisch bleiben, nur gefährlicher‘) | KI SCH | – |
| COMBAT-DEC-KI-011 | **Intelligente Gegner kombinieren Spuren** (Geräusch, Blut, Türen, Fenster, letzte Position) zu Hypothesen und prüfen Verstecke auch ohne Gewissheit; Verstecke sind nie garantiert sicher | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#240E` B (Verstecke), P12 (‚er darf einfach so nachschauen, wenn er dich findet, ist es halt blöd‘) | KI | – |
| COMBAT-DEC-KI-012 | **Wiederholte Ablenkungstricks können erkannt werden;** der Gegner wird skeptisch, ignoriert, untersucht anders oder vermutet eine Falle | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#241A` | KI | – |
| COMBAT-DEC-KI-013 | **Menschliche Gegner teilen Information nur über plausible Kommunikation** (Stimme, Funk, Sichtzeichen); kein Schwarmbewusstsein; Kreaturen kommunizieren artspezifisch; wer jemanden vor dem Ruf ausschaltet, hält die anderen unwissend | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` §4–§5 (Entwurf) | KI NPC | – |
| COMBAT-DEC-KI-014 | **Blutspuren können Menschen visuell verfolgen, wo plausibel;** Geruchsverfolgung ist kreaturspezifisch | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘ | KI | – |
| COMBAT-DEC-KI-015 | **Taktikvorrat intelligenter Monster:** beobachten, auflauern, flankieren, Strom abschalten, Licht manipulieren, falsche Geräusche und Stimmen, absichtlich schweigen, Ausgänge blockieren, Jäger isolieren, verletzte Ziele verfolgen, Begleiter angreifen, zurückziehen, Nachladen und Verletzungen ausnutzen; nicht jede Kreatur kann alles | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` §6; `23#254E` P6 (‚kommt aufs Monster an‘); `K-KI-006` | KI KRE | – |
| COMBAT-DEC-KI-016 | **Die Taktikwahl bewertet die Lage** (Verletzung, Gruppe, Distanzvorteil, Strom, Verhalten des Spielers) und passt sich an; keine Zufallsrotation, mit etwas Variation | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#253A` §7; `23#254E` P7 (‚die Monster sollen sich anhand von meinem Verhalten schlau anpassen‘) | KI | Bewertungsmodell: Abschnitt 5 |
| COMBAT-DEC-KI-017 | **Absichtliches Schweigen ist nur eine Taktik unter vielen** und nicht so häufig, dass sie vorhersehbar wird | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#240E` C; `K-KI-008` | KI | – |
| COMBAT-DEC-KI-018 | **Fake-Geräusche** unterscheiden Weltgeräusch, vom Monster erzeugtes physisches Geräusch und rein übernatürliche Wahrnehmung nur des Ziels | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `K-KI-009` | KI AUD | – |
| COMBAT-DEC-KI-019 | **Taschenlampen und Lichtquellen sind Werkzeuge und Risiken;** kreaturspezifische KI kann Licht und Strom manipulieren; manche Wesen sehen Licht nicht, andere reagieren stark | 📋 BESCHLOSSEN | `M-PD` ‚Stealth…‘; `23#225A` §8–§9 (Entwurf) | KI WAF | – |
| COMBAT-DEC-KI-020 | **Mehrere Gegner üben gleichzeitig Druck aus** (flankieren, greifen, Fluchtweg abschneiden) und nutzen Gelegenheiten konsequenter, besonders auf Grounded | 📋 BESCHLOSSEN | `COMBAT-DEC-NK-018`, `-044` | KI | – |
| COMBAT-DEC-KI-021 | **Menschliche Gegner können fliehen, erstarren oder sich ergeben,** abhängig von Persönlichkeit, Kampferfahrung, Verletzung, Bewaffnung, Verbündeten, Gesehenem und der Haltung des Spielers; Furcht und Moral bleiben intern, ohne sichtbaren Balken | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#223A` (Entwurf der Faktoren); `23#222E` Zusatz | KI NPC | Schwellen: `CQ-73` |
| COMBAT-DEC-KI-022 | **Auf höheren Schwierigkeitsgraden sind Gegner aufmerksamer und gnadenlos;** sie hören Nachladen in der Nähe | 📋 BESCHLOSSEN | `23#86E` P1; `M-PD` ‚Difficulty…‘; `K-KI-018` | KI SCH | – |
| COMBAT-DEC-KI-023 | **Grounded verbessert Reaktionszeit, Aufmerksamkeit, Interpretation, Erinnerung, Suchlogik und Bereitschaft nachzugehen,** nur moderat die Wahrnehmungsschwellen (Entwurf des Assistenten, vom Entwickler mit ‚realistisch bleiben, nur gefährlicher‘ aufgenommen) | 🧭 RICHTUNG | `23#239A` §6; `23#240E` P6; `23#226E` P21 | KI SCH | – |
| COMBAT-DEC-KI-024 | **Ein KI-Inspektor zeigt Wissen, Verdacht, letzte Position, Hörschätzung, Suchphase, aktuelle Taktik und Begründung** | 📋 BESCHLOSSEN | Gameplay-Systeme §7 (Entwicklerwerkzeuge); `K-KI-012`; `23#254E` P16 | TST | – |
| COMBAT-DEC-KI-025 | **Begleiter handeln selbst und stimmen sich mit Plan und Befehl ab;** sie dürfen abweichen, wenn Gefahr die Anweisung unsicher macht; ihr Stealth versteht Absicht, Tempo, Licht, Feinde, Schusslinien und Lärmrisiko | 📋 BESCHLOSSEN | `M-PD` ‚Companion autonomy…‘; `23#254E` P17; `N-CO-010`, `N-CO-011` | NPC KI | – |
| COMBAT-DEC-KI-026 | **Friendly Fire trifft auch Gegner untereinander;** Gegner achten auf ihre Schusslinie, auf Grounded besser | 🧭 RICHTUNG | `23#221A` §3 (Entwurf); `23#222E` P3 (‚passt‘) | KI | – |
| COMBAT-DEC-KI-027 | **Bedrohungsbewertung als Modell** (Zahl und Ausrüstung des Spielers, Verletzungen, Gelände, eigene Verluste, Rückzugswege) für den Rückzug und Wiederkehr | 💡 VORSCHLAG | Abschnitt 5; Anwendung von `N-NL-001`, `K-KI-006` | KI | Werte offen |
| COMBAT-DEC-KI-028 | **Rückzug, Verstecken, Hinterhalt und Jagd auf den Spieler** sind Verhalten intelligenter Wesen | 📋 BESCHLOSSEN | `K-KI-017` | KI | – |
| COMBAT-DEC-KI-029 | **Lernen je Individuum, Gruppe oder Art** und Gedächtnisgrenzen je Art sind nicht festgelegt | ❓ OFFEN | `K-KI-019`, `K-KI-020` | KI NPC | `CQ-74` |
| COMBAT-DEC-KI-030 | **Schwellen von Furcht, Aufgabe und Flucht,** Gewichtung der Faktoren und Auslöser sind nicht festgelegt | ❓ OFFEN | `M-OD`; `23#223A` (Faktoren nur genannt) | KI NPC | `CQ-73` |
| COMBAT-DEC-KI-031 | **Deckungsnutzung der Gegner** (Anlehnen, Wechsel, Blindfeuer) ist nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-FK-031` | KI | `CQ-17` |
| COMBAT-DEC-KI-032 | **Leistungsgrenzen der KI** (gleichzeitig aktive Gegner, Wahrnehmungstraces je Tick) sind nicht entschieden | ❓ OFFEN | `N-SI-017`; Modul 25 | KI PRF | `CQ-75` |
| COMBAT-DEC-KI-033 | **Verhalten von Gegnern gegenüber Begleitern, Tieren und Zivilisten im Kampf** (Ziele, Geiseln, Flucht von Unbeteiligten) ist nicht beschrieben | ❓ OFFEN | `E-RE-005` (NPCs reagieren auf Verbrechen); Gameplay-Systeme §10 | KI NPC | `CQ-76` |

---

## 3. Wahrnehmung und Wissen

**Schall (nach `23#239A`, Entwurf des Assistenten; im Kern bestätigt durch `23#240E`):** Das Ereignis `HeardSound` enthält geschätzte Richtung, geschätzte Entfernung, Unsicherheit, Lautstärke, Geräuschart und Zeitpunkt. Die Genauigkeit der Ortung wächst mit Lautstärke, Dauer, Frequenzinhalt, kleiner Entfernung, Sinnesschärfe der Art; sie sinkt mit Hall und Raumakustik (‚Räume verändern die Richtung‘). Die Kernaussagen sind in `M-PD` ohne Zahlen geführt.

**Wissensgrade:** Je Information gibt es einen Grad; Ereignisse können ihn heben (Sichtung) oder senken (Zeitverlauf).

| Grad | Beispiel der Quelle (`23#242E`, `23#243A`) |
|---|---|
| bestätigt | der Gegner sah den Spieler in einen Schrank steigen |
| starker Verdacht | Blutspur endet vor dem Schrank, Tür bewegt |
| schwacher Verdacht | der Spieler verschwand im Raum, mehrere Verstecke plausibel |
| unbekannt | keinerlei Hinweis |

**Folgerung (🧩 ABGELEITET):** Eine KI, die das Muster ‚Spieler versteckt sich in Schränken‘ aus früheren, ihr unbekannten Begegnungen kennt, widerspricht `COMBAT-DEC-KI-003`; wiederkehrende Gegner dürfen nur aus **eigenen** Begegnungen lernen ([Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md)).

---

## 4. Suche und Rückfall in die Tarnung

```text
Entdeckung (confirmed target) -> Sofortverfolgung -> Sichtverlust
  -> Lokale Suche (letzte bekannte Position, Nachbarräume)
  -> Erweiterte Suche (wahrscheinliche Fluchtwege)
  -> Einkesselung (Ausgänge sichern, Räume aufteilen, nur mit Kommunikation)
  -> Vorsichtige Wachsamkeit (keine Entwarnung)
```

Stufen nach `M-PD` und `23#253A` P3. Der Entwickler: Die Dauer der Suche sei schwierigkeitsabhängig (`23#226E` P11). Die Speichersperre endet erst, wenn der Bedrohungszustand wirklich beendet ist (`COMBAT-DEC-GP-028`, `23#370A`). Den Übergang zwischen Investigating und Detected legt der Assistent als später technisch festzulegen fest (`CQ-09`).

---

## 5. Bedrohungsbewertung, Taktikwahl und Anpassung (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-KI-027`).** Das Modell konkretisiert `COMBAT-DEC-KI-016`, ohne Werte.

```text
Eingaben: Wissensgrade (Position, Waffen, Verletzung, Begleiter), Lage (Licht, Raum, Ausgänge),
          eigener Zustand (Verletzung, Verbündete), Art (erlaubte Taktiken), Schwierigkeit
Bewertung: je erlaubte Taktik ein Nutzen aus der Lage (kein Zufall); kleine Variation mit Seed
Wahl:      beste Taktik; wechselt, wenn sie wiederholt erfolglos war
Ausgabe:   Handlung + Begründung für den KI-Inspektor
```

**Prüfbare Eigenschaften (🧩 ABGELEITET):** (1) Dieselbe Lage mit demselben Seed ergibt dieselbe Taktik. (2) Taktiken, die nicht im Vorrat der Art stehen, werden nie gewählt. (3) Eine Information, die die KI nicht beobachtet hat, beeinflusst die Wahl nicht. (4) Die Begründung nennt die verwendeten Wissensgrade.

---

## 6. Menschliche Gegner: Moral, Kommunikation, Gruppen

- **Furcht und Moral** bleiben intern und zeigen sich über Stimme, Zittern, Zurückweichen, unsicheren Blick, Waffe senken, Fluchtversuch, Hände heben, tatsächliche Aufgabe (`23#223A`, Entwurf); kein sichtbarer Furchtbalken.
- **Beispielkette der Quelle (`23#223A`, 💡):** Mehrere Angreifer, einer ausgeschaltet, einer schwer verletzt, der dritte unter Kontrolle und bedroht: hohe Furcht, Aufgabe. Ein anderer Gegner kämpft trotz derselben Lage weiter.
- **Folgen für Ermittlung:** Jemanden festzusetzen und zur Aufgabe zu bringen ist etwas anderes, als jeden Gegner zu töten (`23#223A`, 💡).
- **Kommunikation** ist spielrelevant (Rufen, Funk, Handzeichen) und kann verhindert werden (`COMBAT-DEC-KI-013`).
- **Gruppen** teilen sich nur bei Kommunikation auf; Fraktionszugehörigkeit steuert Reaktion und Folgen ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)).

---

## 7. Schwierigkeit

| Aspekt | Aussage | Quelle |
|---|---|---|
| Aufmerksamkeit, Aggression, Reaktion | steigen auf höheren Stufen | `COMBAT-DEC-KI-022` |
| Suche | länger, intelligenter | `COMBAT-DEC-KI-006` |
| Wahrnehmungsschwellen | nur moderat | `COMBAT-DEC-KI-023` |
| Wissen | nie Wandblick oder Allwissen | `COMBAT-DEC-KI-003` |
| Anatomie | gleich | `COMBAT-DEC-GP-014` |
| Tarn-HUD | Grounded standardmäßig aus | `COMBAT-DEC-ST-007` |

---

## 8. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Furcht und Aufgabe (`CQ-73`) | A: Schwellenwert aus Faktoren | einfach, testbar | wirkt mechanisch |
| | B: Persönlichkeitsabhängige Verteilung (Streuung je Figur) | lebendig | Balance und Tests |
| Lernen (`CQ-74`) | A: Je Individuum | passt zu persistenter Identität | Speicher |
| | B: Je Gruppe (Wissensweitergabe) | passt zu Monstergesellschaft | Konsistenz |
| | C: Je Art (nur kosmetisch) | billig | widerspricht individuellem Gedächtnis |
| KI-Last (`CQ-75`) | A: Alle Gegner voll | Qualität | teuer |
| | B: Stufen nach Nähe und Bedrohung | skalierbar | Übergänge sichtbar |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 9. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zu Audio | Schallereignisse, Raumakustik |
| zu Waffen | Nachladen, Schuss, Schalldämpfer |
| zu Nemesis ([Modul 19](19-Nemesis-Erinnerung-und-persistente-Kampffolgen.md)) | Gedächtnis, Wiederkehr |
| zu NPCs ([NPCs 06](../NPCs/06-Wahrnehmung-Wissen-Glaube-und-Unsicherheit.md)) | Wahrnehmung und Überzeugung |
| zu Persistenz | Zustand von Gegnern, Speichersperre |
| zu Koop | Wahrnehmung bei mehreren Spielern (Ziel wählen) |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Mehrere Spieler in der Nähe | Ziel nach Wissen, nicht nach Distanz allein | 💡 |
| Gegner ohne Pfad zum Ziel | Ersatzverhalten (Positionswechsel, Rückzug), keine Starre | 💡 |
| Gleiche Geräuschquelle wiederholt | Skepsis (`COMBAT-DEC-KI-012`) | 📋 |
| Tod des einzigen Kommunikators | Information bleibt lokal | 📋 (`COMBAT-DEC-KI-013`) |
| Streaming entlädt den Gegner im Suchzustand | Zustand wird nachgeführt (Modul 25) | 💡 |

---

## 10. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-73` | Schwellen und Faktorgewichtung für Furcht, Flucht, Aufgabe und Erstarren | `COMBAT-DEC-KI-021`, `-030` | ja |
| `CQ-74` | Lernen und Gedächtnisgrenzen: je Individuum, Gruppe oder Art | `COMBAT-DEC-KI-029` | ja |
| `CQ-75` | Rechenbudget der KI im Kampf: gleichzeitige Gegner, Wahrnehmungsaufwand, Stufen | `COMBAT-DEC-KI-032` | ja |
| `CQ-76` | Gegner und Unbeteiligte: Zivilisten, Geiseln, Begleiter, Tiere im Kampf | `COMBAT-DEC-KI-033` | ja |
| `CQ-77` | Bewertungsmodell der Taktikwahl und Variation (Gewichtung, Seeds) | `COMBAT-DEC-KI-016`, `-027` | ja |
