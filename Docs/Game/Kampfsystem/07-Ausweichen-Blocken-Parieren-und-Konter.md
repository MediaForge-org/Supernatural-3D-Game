# Kampfsystem 07: Ausweichen, Blocken, Parieren und Konter

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt, was der Entwickler über die Verteidigung im Nahkampf entschieden hat, und stellt die offenen Zeitfenster- und Eingabefragen gegenüber, statt sie zu entscheiden. Es enthält keine Zeitfenster, keine Prozentwerte und keine Tastenbelegung. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Angriff und Griffkampf: [Modul 04](04-Nahkampf-und-Kampfstile.md); Ausdauer: [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md); Eingabe: [Modul 06](06-Steuerung-und-Eingabekontext.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Verteidigungsarten im Überblick](#3-verteidigungsarten-im-überblick)
4. [Angriffseigenschaften statt Anzeigen](#4-angriffseigenschaften-statt-anzeigen)
5. [Gegenüberstellung der Auswertungsmodelle](#5-gegenüberstellung-der-auswertungsmodelle)
6. [Übergänge: Konter, Öffnung, Wiederaufnahme](#6-übergänge-konter-öffnung-wiederaufnahme)
7. [Verteidigung gegen Griffe, Waffen und Monster](#7-verteidigung-gegen-griffe-waffen-und-monster)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Quellen legen die Haltung fest, nicht die Mechanik.** Blocken ist physisch und kein Pauschalschutz, Parieren gibt es nur, wenn es plausibel ist, Ausweichen ist kein Rollen-Spam (`M-PD` ‚Melee / grappling‘, `23#162A` P6–P8, `23#163E` P6–P8).
2. **Der Spieler soll die Verteidigungsmöglichkeiten aus Animation, Monsterwissen und Erfahrung lernen,** nicht aus bunten Indikatoren (`23#162A` P6, `23#163E` P6 ‚passt gut‘).
3. **Zeitfenster, Voraussetzungen, Ausdauerkosten und Eingabe sind nicht festgelegt.** Der Auftrag nennt Kreis als Ausweichtaste (🔍 UNBELEGT, `COMBAT-DEC-ST-012`).
4. **Der Kampf hängt am Körperzustand:** Verletzter Arm schwächt Blocken, Beinverletzung die Ausweichbewegung, Blutverlust die Reaktion (`COMBAT-DEC-NK-026`).
5. **Verteidigung gegen Schusswaffen ist keine Nahkampfverteidigung:** Gegen Projektile gelten Deckung, Distanz, Sichtbruch und die Durchschlagsregeln (`COMBAT-DEC-FK-016`); ob es Ausweichmechaniken gegen Projektile gibt, steht nicht in den Quellen (`CQ-29`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-DF-001 | **Blocken ist physisch,** kein universeller Schadensminderungszustand | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#162A` P6; `23#163E` P6 (‚passt gut‘) | TRF STE | Zeitfenster: `CQ-27` |
| COMBAT-DEC-DF-002 | **Wirksamkeit hängt vom Angreifer ab:** ein menschlicher Faustschlag kann geblockt werden, ein Messer vielleicht abgeleitet oder pariert, aber gefährlich; die übernatürliche Kraft einer riesigen Kreatur wird nicht durch beidhändiges Blocken aufgehalten | 📋 BESCHLOSSEN | `23#162A` P6; `23#163E` P6 | KRE TRF | Grundlage der Angriffseigenschaften (DF-003) |
| COMBAT-DEC-DF-003 | **Jeder Angriff hat Eigenschaften** (blockierbar, parierbar, Ausweichen empfohlen, nicht blockierbar), die **nicht über bunte Videospielindikatoren** angezeigt werden; der Spieler lernt sie aus Animation, Monsterwissen und Erfahrung | 📋 BESCHLOSSEN | `23#162A` P6; `23#163E` P6 | KRE UI ANI | Hinweise je Schwierigkeit: `CQ-30` |
| COMBAT-DEC-DF-004 | **Parieren gilt nur, wenn es physikalisch plausibel ist** (menschlicher Messerangriff mit richtigem Timing ja; ein Monster mit sehr großer Masse nicht mit einem kleinen Messer) | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘; `23#162A` P7; `23#163E` P7 (‚genau‘) | TRF ANI | – |
| COMBAT-DEC-DF-005 | **Eine gelungene Parade kann den Gegner kurz öffnen, einen Griff lösen, Distanz schaffen oder einen Gegenangriff ermöglichen;** kein ‚Perfect Parry‘, das den Gegner mehrere Sekunden einfriert | 📋 BESCHLOSSEN | `23#162A` P7; `23#163E` P7 | STE KI | Dauer der Öffnung: Konfiguration, keine Quelle |
| COMBAT-DEC-DF-006 | **Ausweichen besteht aus körperlich plausiblen Bewegungen** (Schritt zurück, Seitschritt, Ducken, Körper wegdrehen, kurzer Sprung zur Seite, in extremer Lage zu Boden werfen); eine Rolle existiert nur, wo sie physisch sinnvoll ist, und ist nicht die primäre Verteidigung | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (‚Avoid Soulslike roll spam‘); `23#162A` P8; `23#163E` P8 (‚passt‘) | BEW ANI | Katalog und Kosten: `CQ-28` |
| COMBAT-DEC-DF-007 | **Ausweichen liegt laut Auftrag auf Kreis (DualSense)** | 🔍 UNBELEGT | `AUF` §5 (Modul 06); `COMBAT-DEC-ST-012` | STE | `CQ-01` |
| COMBAT-DEC-DF-008 | **Konter nach Block oder Ausweichen** sind Teil des Schlagabtauschs; eine Kombination, die gekontert wird, zerfällt | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (counters and transitions); `23#168A` (‚Konter nach Block/Dodge‘); `COMBAT-DEC-NK-004` | STE ANI | Zeitfenster: `CQ-27` |
| COMBAT-DEC-DF-009 | **Finten existieren** (Angriff starten und abbrechen, um eine Reaktion zu erzwingen) | 📋 BESCHLOSSEN | `M-PD` ‚Melee / grappling‘ (‚feints‘); `23#168A` (Beispiel Feint-Cross) | STE KI | – |
| COMBAT-DEC-DF-010 | **Verletzungen schwächen die Verteidigung:** verletzter Arm schlechteres Blocken, Beinverletzung schlechtere Ausweichbewegungen, Blutverlust schwächere Reaktion | 📋 BESCHLOSSEN | `23#162A` P15; `23#163E` P15; `COMBAT-DEC-NK-026` | MED | – |
| COMBAT-DEC-DF-011 | **Schwere Monster zwingen zum Ausweichen statt zum Tanken** (zerstören Deckung, lassen sich kaum aufhalten) | 📋 BESCHLOSSEN | `23#85A` P13 (Beispiel); `23#86E` P13 (‚ja genau, das ist wichtig‘) | KRE | [Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md) |
| COMBAT-DEC-DF-012 | **Gegner verteidigen sich ebenfalls** (blocken, weichen aus, kontern), sonst haben Block, Konter und zerfallende Kombinationen keine Wirkung | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-NK-004`, `COMBAT-DEC-NK-044` (Gegner nutzen Öffnungen) | KI | Schwierigkeitsabhängige Qualität: Modul 18 |
| COMBAT-DEC-DF-013 | **Block-Aufprall auf den Arm (Armverletzung durch harten Treffer auf die Deckung) ist als Mechanik denkbar** | 💡 VORSCHLAG | `M-P08` (Themeninventar ‚block-impact-and-arm-injury-integration‘); keine Entwicklerquelle | MED TRF | Prüfen in `CQ-27` |
| COMBAT-DEC-DF-014 | **Abwehr von Projektilen geschieht über Deckung, Distanz und Sichtbruch;** ob ein Ausweichen gegen Schüsse oder Monsterfernangriffe (Wurf, Speien, Entladung) existiert, ist nicht entschieden | ❓ OFFEN | `COMBAT-DEC-FK-016`; `23#127A` (‚Verstecken ist nicht automatisch dasselbe wie geschützt sein‘) | TRF KRE | `CQ-29` |
| COMBAT-DEC-DF-015 | **Zeitfenster, Voraussetzungen, Ausdauerkosten und Erholung von Block, Parade, Ausweichen und Konter sind nicht festgelegt** | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘ (Tiefe von Nahkampf); Entscheidungsstatus §5.6 | STE AU | `CQ-27`, `CQ-28` |

---

## 3. Verteidigungsarten im Überblick

| Art | Quellenaussage | Voraussetzung (Quelle) | Offen |
|---|---|---|---|
| Hohe/niedrige Deckung der Arme | Block ist physisch; kein Pauschalschutz (`M-PD`) | Angreifer und Wucht müssen die Handlung zulassen | Zeitfenster; welche Region geschützt wird (`M-P08`: Themeninventar ‚high-guard‘, ‚body-guard‘) |
| Ablenken, Parieren | nur wenn plausibel (`M-PD`) | passender Angriff, Timing | Zeitfenster, Voraussetzung, Eingabe |
| Schritt, Seitschritt, Ducken, Drehen | Ausweichen (`23#162A` P8) | Raum, Gleichgewicht, Verletzung | Ausdauerkosten, Eingabe |
| Sprung zur Seite, Boden | Extremsituation (`23#162A` P8) | physisch sinnvoll | Zustand danach (am Boden) |
| Rolle | nur wenn physisch sinnvoll (`23#162A` P8) | Raum, Kontext | Zulässigkeit je Situation |
| Takedown-Verteidigung | Sprawl, Unterhaken, Winkelwechsel (`COMBAT-DEC-NK-009`) | Griffkontakt | Zeitfenster, Eingabe |
| Befreiung aus dem Griff | kein Quick-Time-Spam (`COMBAT-DEC-NK-048`) | Position, Richtung, Kraft | Eingabe |
| Deckung und Distanz | gegen Fernkampf (`COMBAT-DEC-FK-016`) | Material, Winkel | Deckungssystem (`CQ-17`) |

---

## 4. Angriffseigenschaften statt Anzeigen

Der Entwickler bestätigt (`23#163E` P6), dass jeder Angriff Eigenschaften trägt, aber nicht über bunte Indikatoren angezeigt wird. Daraus folgt eine Anforderung an die **Lesbarkeit** (🧩 ABGELEITET aus `COMBAT-DEC-DF-003`, `COMBAT-DEC-GP-003`):

1. Die Lesbarkeit eines Angriffs entsteht aus **Animation und Audio** (Anlauf, Körperhaltung, Waffe) und aus **Wissen** (Hunter Archive, Erfahrung), nicht aus einer Schadenszahl oder einem farbigen Ring.
2. Das Monsterwissen des Archivs kann Eigenschaften enthüllen (bekannte Angriffsmuster); unbekannte Wesen sind schwerer zu lesen (`K-FO-004`, `K-FO-005`).
3. Schwierigkeitsabhängige Hinweise (zum Beispiel optionale Kontexthinweise) sind **nicht beschlossen** (`CQ-30`); nach `COMBAT-DEC-ST-007` sind Anzeigen im Normalfall optional und in Grounded standardmäßig aus.

**Entwurf der Angriffseigenschaften (💡 VORSCHLAG):**

| Eigenschaft | Bedeutung | Quelle der Idee |
|---|---|---|
| blockierbar | durch Armhaltung teilweise abfangbar | `23#162A` P6 |
| parierbar | durch Ablenken zu öffnen | `23#162A` P7 |
| Ausweichen empfohlen | Wucht oder Reichweite machen Blocken sinnlos | `23#162A` P6 |
| nicht blockierbar | Griff, Biss, Wurf, Besitz | `23#162A` P6; `M-PD` (Griffe) |
| Wucht | Energie des Angriffs (kein Schadenswert) | `COMBAT-DEC-NK-027` |
| Reichweite und Dauer | Zeit bis zur Wirkung | Entwurf |

Alle Felder sind Entwurf; Werte fehlen.

---

## 5. Gegenüberstellung der Auswertungsmodelle

Die Quellen verlangen physische Plausibilität (`COMBAT-DEC-DF-001`, `-004`) und eine Steuerung ohne Tastentabelle. Wie Block, Parade und Konter ausgewertet werden, ist offen (`CQ-27`).

| Modell | Beschreibung | Vorteil | Nachteil |
|---|---|---|---|
| A: Zeitfenster-basiert | Verteidigung gelingt, wenn die Eingabe in einem Fenster vor der Wirkung liegt | gut lesbar, einfach zu testen | wirkt arcadig; widerspricht ‚physisch, kein Zustand‘ nur halb; gefährdet ‚Perfect Parry‘-Muster (`COMBAT-DEC-DF-005`) |
| B: Physikbasiert | Block und Parade entstehen aus Armhaltung, Position, Wucht, Gleichgewicht; Auswertung über Körpermodell | passt zu `COMBAT-DEC-DF-001`, `-004`, `-010` | schwer vorhersagbar, schwer zu balancieren, hoher Animations- und Rechenaufwand |
| C: Hybrid | Eingabe öffnet eine kurze Haltung; die Physik entscheidet über Wirkung | verbindet beides | doppelte Komplexität; Fehlerquellen an der Grenze |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen. Für alle Modelle gilt: kein Quick-Time-Event, keine Pauschalimmunität, Auswertung deterministisch testbar.

---

## 6. Übergänge: Konter, Öffnung, Wiederaufnahme

Nach den Quellen verändert jede Verteidigung den Zustand (`COMBAT-DEC-NK-004`, `-007`):

```text
Angriff            -> getroffen | geblockt | pariert | ausgewichen | verfehlt
(neue Position)    -> neue Distanz, neues Gleichgewicht, mögliche Öffnung
(nächste Aktion)   -> Konter | Wiederaufnahme der Distanz | Clinch | Rückzug
```

**Prüfbare Eigenschaften (🧩 ABGELEITET):**

1. Ein geblockter Schlag setzt dieselbe Kombination nicht automatisch fort; der Folgeschlag ist neu auszuwählen.
2. Eine gelungene Verteidigung verändert den Zustand des Angreifers (Öffnung, Distanz), nicht nur des Verteidigers.
3. Ein ausgewichener Angriff geht ins Leere; es gibt keine Zielaufschaltung, die ihn nachträgt (`COMBAT-DEC-NK-006`).
4. Kein Zustand erzeugt eine dauerhafte Unverwundbarkeit (kein Betäubungs-Lock, `COMBAT-DEC-NK-019`).

---

## 7. Verteidigung gegen Griffe, Waffen und Monster

| Gegner oder Angriff | Quellenstand |
|---|---|
| Menschlicher Schlag oder Tritt | Block möglich, Parade bei Plausibilität; Ausweichen (`COMBAT-DEC-DF-002`, `-004`, `-006`) |
| Messer, Klingenwaffe | Ablenken oder Parieren möglich, aber gefährlich (`23#162A` P6) |
| Stumpfe oder schwere Nahkampfwaffe | Keine Aussage der Quellen; Wirksamkeit von Block folgt aus Wucht und Waffe (Entwurf) |
| Griff, Takedown | Verteidigung über Sprawl, Unterhaken, Winkelwechsel; Befreiung aus dem Griff (`COMBAT-DEC-NK-009`, `-048`) |
| Schusswaffe im Nahbereich | Wegdrücken, Mündungsrichtung ändern, Entwaffnen; Verteidigung ist körperlich (`COMBAT-DEC-NK-024`, `-045`) |
| Schweres Monster | Ausweichen statt Tanken (`COMBAT-DEC-DF-011`); Blocken wirkungslos (`COMBAT-DEC-DF-002`) |
| Geisterhafte oder körperlose Wesen | Block und Parade nicht sinnvoll; Schutz über Gegenmaßnahmen ([Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md), `K-GE-016` ist 💡) |
| Mehrere Gegner | Verteidigung gegen den Hauptangreifer lässt andere Angriffe offen; ‚nicht höflich nacheinander‘ (`COMBAT-DEC-NK-018`) |
| Fernangriff eines Wesens | nicht entschieden (`COMBAT-DEC-DF-014`) |

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Körpermodell ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) | welche Region wird durch Block geschützt, welche trifft |
| zur Ausdauer ([Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md)) | Kosten und Erholung der Verteidigung |
| zur KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)) | Gegner nutzen Öffnungen; Angriffe tragen Eigenschaften |
| zur Animation ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) | Lesbarkeit der Angriffe, Reaktionsanimationen |
| zur Steuerung ([Modul 06](06-Steuerung-und-Eingabekontext.md)) | Eingabe und Ablehnung mit Grund |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Verteidigung wird bei Bewusstlosigkeit oder Griff angefordert | Ablehnung mit Grund | 💡 |
| Ausweichen in eine Wand | Handlung gelingt eingeschränkt oder wird abgelehnt, kein Durchlaufen | 💡 |
| Block auf Angriff, der nicht blockierbar ist | Block ohne Wirkung; Rückmeldung über Animation | 💡 (`COMBAT-DEC-DF-003`) |
| Zwei Angriffe treffen im selben Schritt | Reihenfolge deterministisch, jede Verteidigung gilt gegen genau einen Angriff | 💡 |
| Verletzung macht Verteidigung unmöglich | Ablehnung mit Grund und sichtbarer Folge | 💡 |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-27` | Auswertung von Block, Parade und Konter: Zeitfenster, physikbasiert oder hybrid; Voraussetzungen, Öffnungsdauer | `COMBAT-DEC-DF-001`, `-004`, `-005`, `-008`, `-013`, `-015` | ja |
| `CQ-28` | Ausweichkatalog: welche Bewegungen, Ausdauerkosten, Eingabe, Zulässigkeit der Rolle | `COMBAT-DEC-DF-006`, `-015` | ja |
| `CQ-29` | Verteidigung gegen Projektile und Monsterfernangriffe | `COMBAT-DEC-DF-014` | ja |
| `CQ-30` | Darstellung der Angriffseigenschaften: reine Animation und Audio oder optionale Hinweise je Schwierigkeit | `COMBAT-DEC-DF-003` | ja |
