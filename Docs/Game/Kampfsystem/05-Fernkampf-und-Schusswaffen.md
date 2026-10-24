# Kampfsystem 05: Fernkampf und Schusswaffen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul fasst die Entscheidungen zu Waffenhandhabung, Zielen, Schießen und Ballistik zusammen und stellt Entwürfe als **Entwurf (💡)** daneben. Es enthält keine Schadens-, Geschwindigkeits- oder Reichweitenwerte, weil die Quellen keine nennen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Munition, Magazine und Waffenzustand stehen in [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md), Waffenklassen in [Modul 09](09-Waffenklassen-und-Ausruestung.md), Körperwirkung in [Modul 12](12-Treffermodell-und-Koerperphysik.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene Anforderungen](#2-beschlossene-anforderungen)
3. [Waffenhandhabung und Zielen](#3-waffenhandhabung-und-zielen)
4. [Projektilverhalten und Ballistik](#4-projektilverhalten-und-ballistik)
5. [Entwurf: Schusspipeline](#5-entwurf-schusspipeline)
6. [Gegenüberstellung der Projektilmodelle](#6-gegenüberstellung-der-projektilmodelle)
7. [Long Range, Üben, Kill-Cam](#7-long-range-üben-kill-cam)
8. [Schnittstellen und Fehlerfälle](#8-schnittstellen-und-fehlerfälle)
9. [Offene Punkte dieses Moduls](#9-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Quellen verlangen eine physische, körperlich spürbare Schusswaffe,** deren Handhabung vom Körperzustand abhängt: Masse, Schwerpunkt, Länge, Rückstoß, Anschlagzustände, Raumbeschränkung, Ermüdung, Atmung, Verletzung, schwache Hand (`M-PD` ‚Gunplay / weapons‘, `23#116E`).
2. **Ballistik ist glaubwürdig, nicht wissenschaftlich perfekt.** Flugzeit, Fall, Luftwiderstand, Energieverlust, Wind, Vorhalten, Höhe, Schusswinkel, Visierhöhe, Cant; Coriolis ausdrücklich nicht (`M-PD` ‚Ballistics‘, `23#134E`, `23#138E`).
3. **Es gibt keine künstliche Maximalreichweite;** typische Gefechte liegen unter etwa 100 m, einzelne Schüsse von 500 bis 1000 m und mehr müssen funktionieren (`M-PD`, `23#138E` P21).
4. **Verletzung folgt dem Projektilzustand und dem anatomischen Weg,** nicht einer Abstandskurve (`COMBAT-DEC-ZA-001`).
5. **Nicht in den Quellen:** ein Deckungssystem (Anlehnen, Blindfeuer, Deckungswechsel), Schießen aus und auf Fahrzeuge als Kampfmechanik, Zahlen zur Streuung, eine Waffenliste (`CQ-17`, `CQ-18`, `CQ-33`).
6. **Eine Antwort des Entwicklers ist uneindeutig:** ‚auf Grounded würde ich aber alle solche Sachen disabled machen‘ (`23#116E` P7). Gemeint sind entweder die Abschaltoptionen (Auswirkungen bleiben zwingend an) oder die Auswirkungen selbst; der Assistent legte Ersteres aus (`23#117A` zu P7). Die Dokumentation übernimmt keine der Lesarten (`CU-02`).

---

## 2. Beschlossene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-FK-001 | **Eine Schusswaffe hat ein physisches Profil** (Masse, Schwerpunkt, Gesamtlänge, Rückstoß, Zustand, Befüllung) und ist kein Damage-Wert; eine objektiv stärkere Waffe kann situativ schlechter sein | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#115A` P1 (Beispiel); `23#116E` P1 | WAF | Felder und Werte: Entwurf (Abschnitt 3) |
| COMBAT-DEC-FK-002 | **Die Handhabung umfasst Masse, Schwerpunkt, Länge, Rückstoß, tiefe Bereitschaftsstellung, Punktschießen, Visieren (ADS), Raumbeschränkungen, Ermüdung, Atmung, Verletzungseinfluss, schwache Hand und mechanisch korrekte Bedienung** | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P3, P5, P7–P9, P13 | WAF ANI | – |
| COMBAT-DEC-FK-003 | **Schwere und lange Waffen können mehr Energie und Reichweite liefern,** haben aber Nachteile bei Handhabung, Platz und Ermüdung. Der Entwickler: ‚die große Waffe wird vermutlich mehr Schaden machen können‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P2; `23#117A` | WAF | – |
| COMBAT-DEC-FK-004 | **Anschlagzustände:** tiefe Bereitschaft, Punktschießen und Visieren (ADS) werden unterschieden; die Waffe wird in Third Person sichtbar korrekt angeschlagen | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#115A` P3 (Zustände, Entwurf des Assistenten); `23#116E` P13 | WAF ANI | Namen und Übergangszeiten: Entwurf |
| COMBAT-DEC-FK-005 | **Rückstoß ist physikalisch;** Training verbessert die Kontrolle, nicht den Impuls der Waffe | 📋 BESCHLOSSEN | `M-PD` ‚Body / physical response / recoil‘; `23#116E` P5 (‚Rückstoß soll realistisch sein‘); `23#115A` P5–P6 | WAF STE | Rückstoßmodell: Entwurf; Zielhilfe `CQ-15` |
| COMBAT-DEC-FK-006 | **Ermüdung, Atmung, Schmerz und Verletzung beeinflussen das Zielen,** abschaltbar in den Einstellungen. Auf Grounded ist die Aussage des Entwicklers uneindeutig | 🧭 RICHTUNG | `23#116E` P7 (‚kann aber ausgeschaltet werden. auf grounded würde ich aber alle solche sachen disabled machen‘); `23#117A` (Auslegung des Assistenten) | WAF SCH | Lesart: `CU-02` |
| COMBAT-DEC-FK-007 | **Mit verletztem Schussarm kann eine Pistole mit der schwachen Hand geführt werden,** mit reduzierter Kontrolle und langsamerer Handhabung. ‚Bei Pistole dann vielleicht in der schwachen Hand, aber Manövrierbarkeit eingeschränkt‘ | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P8; `23#117A` | WAF MED | Weitere Einschränkungen (Reloads, Sicherung): Entwurf von `23#117A` |
| COMBAT-DEC-FK-008 | **Der Charakter unterstützt viele Körperhaltungen** und ist nicht künstlich eingeschränkt | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P14 | BEW ANI | Haltungsliste offen |
| COMBAT-DEC-FK-009 | **Lange Haltezeiten ermüden moderat,** nicht nach wenigen Sekunden; wiederholtes Überlasten verlangsamt den Wiederaufbau der Ausdauer | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#116E` P21; `23#118E` P21 | AU | [Modul 11](11-Ausdauer-Erschoepfung-und-Belastung.md) |
| COMBAT-DEC-FK-010 | **Raumbeschränkungen wirken auf Waffen** (lange Waffe an der Wand); der Entwickler will außerdem richtig dunkle, gruselige Innenräume. Die Ausgestaltung (Waffe zurückziehen, Bereitschaftsposition ändern) ist ein Entwurf des Assistenten | 🧭 RICHTUNG | `M-PD` ‚Gunplay / weapons‘ (room constraints); `23#115A` P4 (Entwurf); `23#116E` P4 | WAF ANI | Details: Abschnitt 3.3 |
| COMBAT-DEC-FK-011 | **Waffenbedienung ist mechanisch korrekt** (Pumpgun: Vorderschaft, Hülsenauswurf, Zuführung; Pistole: Schlitten, Slide Lock, Magazin; Revolver: Trommel, Kammern, Auswurf) und gut animiert | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘ (‚mechanically correct manipulation‘); `23#115A` P9; `23#116E` P9 (‚ja das muss gut animiert werden‘) | WAF ANI | Asset-Anforderungen: `M-P14` (Inventar) |
| COMBAT-DEC-FK-012 | **Munitionsanzeige ist binär,** an oder aus (nichts dazwischen); ohne Anzeige zählt man Schüsse und prüft Waffe und Magazin körperlich; Grounded schaltet sie standardmäßig ab | 📋 BESCHLOSSEN | `M-PD` ‚Difficulty / configuration‘; `23#116E` P22; `23#118E` P22; `23#120E` | UI SCH | [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md) |
| COMBAT-DEC-FK-013 | **Flugbahn wird physikalisch berechnet:** Flugzeit, Fall, Luftwiderstand, Geschwindigkeitsverlust und Restenergie aus plausiblen Formeln, besonders bei Jagd- und Präzisionsgewehren; ‚nicht so komplexe Formeln‘ reichen | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#134E` P1–P2; `23#136E` | TRF PRF | Formeln: Entwurf |
| COMBAT-DEC-FK-014 | **Keine künstliche Maximalreichweite,** bei der Geschosse verschwinden | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#134E` P3 (‚das ist wichtig‘) | PRF | Lebensdauer eines Projektils: Entwurf (Modul 25) |
| COMBAT-DEC-FK-015 | **Projektildefinition** mit Mündungsgeschwindigkeit, Masse, ballistischem Koeffizienten beziehungsweise Luftwiderstandsverhalten und Aufbau | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P3 (Rückfrage des Entwicklers zum Koeffizienten, vom Assistenten erklärt `23#139A`) | MUN | Datenwerte offen |
| COMBAT-DEC-FK-016 | **Durchdringung hängt ab von** Projektil, Energie, Aufbau, Winkel, Material und Dicke; dünne Deckung (Holztür, dünne Wand, leichte Möbel) kann durchschlagen werden, massive Deckung (dicker Beton, schwere Metalltür, Pfeiler) stoppt und zeigt kosmetische Einschläge; mehrere Schichten verringern den Zustand schrittweise | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#125A`; `23#126E` (‚A, wenn die Tür nicht so massiv ist; sonst nur Einschusslöcher‘); `23#127A` | TRF | Materialtabelle: `CQ-45` |
| COMBAT-DEC-FK-017 | **Fahrzeuge haben unterschiedliche ballistische Strukturen** (Türblech ≠ Motorblock); Glas kann brechen und ablenken | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#154A` P9–P10 (Entwurf); `23#155E` P9–P10 | TRF | Schießen von und auf Fahrzeuge als Mechanik: `CQ-18` |
| COMBAT-DEC-FK-018 | **Gefährliche Abpraller sind selten** (Winkel, Geschwindigkeit, Energie, Geschoss, Material); auf Grounded standardmäßig aktiv, in anderen Voreinstellungen standardmäßig aus, aber einschaltbar | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#130E`; `23#131A` | TRF SCH | – |
| COMBAT-DEC-FK-019 | **Gewöhnliche Materialfragmente (Splitter) sind überwiegend kosmetisch;** nur in begründeten Sonderfällen gameplayrelevant. Der Entwickler: ‚das passiert einem in real life nicht mal‘ | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#130E` | TRF | – |
| COMBAT-DEC-FK-020 | **Wind wirkt mit wachsender Flugzeit;** bewegte Ziele brauchen echtes Vorhalten aus der Flugzeit; Höhe und Luftdichte können modelliert werden, Temperatur intern ohne großen Einfluss | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#134E` P7–P8; `23#138E` P5–P9 (P9 ‚ich glaub nicht, dass das so einen Unterschied macht‘) | TRF | – |
| COMBAT-DEC-FK-021 | **Schusswinkel bergauf und bergab, Visierhöhe über der Laufachse und Verkantung (Cant)** sind physikalisch korrekt; Transschall und Spin-Drift dürfen angenähert werden; **Coriolis ist nicht nötig** (‚nein brauchen wir nicht, ist zu klein‘) | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P10–P15 | TRF | – |
| COMBAT-DEC-FK-022 | **Anbauteile verändern relevante Eigenschaften physikalisch** (Optikgeometrie und Nullpunkt, Schalldämpfer, Lauflänge und Mündungsgeschwindigkeit, Zweibein, Magazinmasse) | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P16 (‚die verschiedenen Aufsätze machen einen Unterschied‘) | WAF | Liste offen |
| COMBAT-DEC-FK-023 | **Ein Schalldämpfer macht Überschallschüsse nicht lautlos;** Unterschallmunition hat eigene Kompromisse | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#133A` P9 (Entwurf); `23#134E` P9 (‚das ist auch gut‘) | WAF AUD KI | Geräuschmodell: [Modul 18](18-Gegner-KI-und-Kampftaktik.md) |
| COMBAT-DEC-FK-024 | **Nullpunkteinstellung ist echte Optik- und Lauf-Geometrie,** keine veränderte Geschossphysik | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P2 | WAF | – |
| COMBAT-DEC-FK-025 | **Typische Gefechte liegen meist unter etwa 100 m,** seltene Schüsse von 500 bis 1000 m und mehr müssen funktionieren | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P21 | TRF PRF | Prüfzahl für Tests (kein Balancewert) |
| COMBAT-DEC-FK-026 | **Langstrecken-Üben ist vorgesehen:** Schießstände, Unterschlupf-Training, Trainingsumgebungen und optionale Schießwettbewerbe; ‚viele Möglichkeiten, den Schuss zu üben und nicht nur an Gegnern‘ | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P2; `23#141E` P1–P4 (‚vielleicht auch Events, wo man schießen gegen andere Leute kann‘) | UI WAF | Umfang der Events: `CQ-22` |
| COMBAT-DEC-FK-027 | **Im härtesten Modus kann ein tatsächliches Entfernungsmessgerät nötig sein,** anstatt einer magischen Entfernungsanzeige. Der Entwickler: ‚vielleicht ein extra Gerät‘ | 🧭 RICHTUNG | `M-PD` ‚Ballistics‘ (‚may require‘); `23#138E` P1 | UI SCH | Hilfen je Stufe: `CQ-20` |
| COMBAT-DEC-FK-028 | **Kill-Cams für seltene, hochwertige Treffer:** filmische Projektilverfolgung, optional anatomische Röntgen-ähnliche Ansicht mit eigener visueller Sprache, die an die tatsächliche Verletzungssimulation gebunden ist; Häufigkeit konfigurierbar, standardmäßig selten. Der Entwickler: nicht nur bei richtig guten Kills, konfigurierbar, ‚man kann beides einstellen‘ | 📋 BESCHLOSSEN | `M-PD` ‚Ballistics‘; `23#138E` P21; `23#141E` P8 | UI ANI TRF | Kriterien: `CQ-21` |
| COMBAT-DEC-FK-029 | **Combat Focus** ersetzt eine Zeitlupen-Fähigkeit (siehe [Modul 02](02-Kampfvision-und-Grundprinzipien.md)); Einzelheiten sind offen | 📋 BESCHLOSSEN / ❓ OFFEN | `COMBAT-DEC-GP-021`, `COMBAT-DEC-GP-022` | STE | `CQ-05` |
| COMBAT-DEC-FK-030 | **Schusswaffen gegen Monster:** normale Ballistik ist die Grundlage; danach gelten Resistenz und Schwäche je Kreatur; manche Wesen sind durch Schusswaffen nur zu vertreiben, ihre eigentliche Lösung ist ein Ritual, ein Objekt, Verbrennung oder Bannung | 🧭 RICHTUNG | `23#85A` P2; `23#86E` P2 (‚ja das ist Pflicht‘); `K-KO-002`, `K-KO-014` | KRE | [Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md) |
| COMBAT-DEC-FK-031 | **Ein Deckungssystem (Anlehnen, Lehnen um die Ecke, Blindfeuer, Deckungswechsel, Gegnerverhalten in Deckung) ist in keiner Quelle entschieden;** die Quellen nennen Deckung nur als Material (Durchschlag) und als Stichwort der Combat-Planung | ❓ OFFEN | `23#96A` (Aufgabenliste ‚Deckung‘, Assistent); `M-PD` (nur Materialdurchschlag) | BEW ANI KI | `CQ-17` |
| COMBAT-DEC-FK-032 | **Schießen aus und auf Fahrzeuge** (Reifen, Motor, Fahrer, Innenraum, Fahrzeugkampf) ist als Kampfmechanik nicht beschrieben; beschlossen ist nur die unterschiedliche ballistische Struktur von Fahrzeugen | ❓ OFFEN | `M-PD` ‚Ballistics‘ (Strukturen); Gameplay-Systeme §6 (Fahrzeugschaden ❓) | WAF BEW | `CQ-18` |
| COMBAT-DEC-FK-033 | **Rückstoß-, Streuungs- und Schützenfehlermodell** (Hüftfeuer gegenüber ADS, Schrotstreuung, Mündungsanstieg, Erholung) sind nicht beschlossen; die Quellen nennen nur die Komponenten (Rückstoßimpuls, Mündungsanstieg, Rotation, Erholung, Schützenkontrolle) als Entwurf | ❓ OFFEN | `23#115A` P5 (Entwurf); `M-OD` ‚Combat / injury / gore‘ | WAF STE | `CQ-19` |

---

## 3. Waffenhandhabung und Zielen

### 3.1 Waffenprofil (💡 VORSCHLAG)

Die Quellen verlangen ein physisches Profil, nennen aber keine Felder. Die folgende Tabelle ist der Entwurf von `23#115A` P1, ergänzt um den Bezug zu den Anforderungen. **Alle Werte fehlen absichtlich.**

| Feld | Wirkung nach den Quellen | Bezug |
|---|---|---|
| Masse | Anhebezeit, Trägheit, Ermüdung | `COMBAT-DEC-FK-002`, `-009` |
| Schwerpunkt | kopflastig oder ruhig | `COMBAT-DEC-FK-002` |
| Gesamtlänge | Handling in engen Räumen | `COMBAT-DEC-FK-010` |
| Lauflänge | Mündungsgeschwindigkeit und Manövrierbarkeit | `COMBAT-DEC-FK-022` |
| Rückstoßprofil | Impuls, Mündungsanstieg, Erholung | `COMBAT-DEC-FK-005` |
| Ergonomie, Abzug, Visierung | Bedienung und Präzision | `COMBAT-DEC-FK-002` |
| Kaliber und Munition | Rückstoß, Ballistik, Wirkung | [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md) |
| Zustand | Zuverlässigkeit und Handling | Modul 10 |
| aktuelle Befüllung | tatsächliche Magazine und Patronen | `COMBAT-DEC-ZA-005` |

**Zusatzregel (nach `23#115A` P2, Entwurf des Assistenten, vom Entwickler mit ‚hört sich gut an‘ aufgenommen, `23#116E` P6):** Gewicht wird nicht durch künstliche Eingabeverzögerung simuliert. Die Kamera reagiert direkt; die Waffe hat Nachbewegung und braucht Zeit für ruhiges Zielen. Als Grundsatz gilt die Formulierung des Entwicklers nicht wörtlich; sie ist eine Auslegung.

### 3.2 Einflüsse des Körperzustands auf das Zielen

Die Quellen führen diese Einflüsse (`M-PD` ‚Human anatomy…‘, `23#115A` P7–P8, `23#116E` P7–P8):

| Einfluss | Wirkung (nach den Quellen) | Status |
|---|---|---|
| Sprint, Atmung | Visierung unruhiger; nach kurzer Ruhe ruhiger | 📋 (Richtung) |
| Schmerz, Blutverlust, Schock | Aim Sway, Koordination | 📋 (`M-PD`: Zielen) |
| Armverletzung (Schussarm) | schlechtere Kontrolle; Pistole mit schwacher Hand möglich | 📋 |
| Armverletzung (Stützhand bei Langwaffe) | stärkerer Sway, längere Anschlagzeit, langsamere Reloads (Entwurf) | 💡 |
| Nerven- und Sehnenschäden | Kontrolle und Griff | 📋 (`M-PD`) |
| Adrenalin | unterdrückt wahrgenommenen Schmerz zeitweise | 📋 (`M-PD`) |

### 3.3 Raumbeschränkungen (Entwurf des Assistenten, 🧭)

Der Vorschlag `23#115A` P4: Eine lange Waffe wird bei Wandnähe nicht „gesperrt“, sondern der Charakter zieht sie zurück, die Bereitschaftsposition ändert sich, das Zielen wird langsamer oder eingeschränkt; in extrem enger Umgebung wird die Pistole praktisch uneingeschränkt. Der Entwickler antwortet nicht ausdrücklich auf diesen Punkt (seine Antwort zu P4 betrifft dunkle Räume). `M-PD` führt ‚room constraints‘ als Handhabungsfaktor. Die Ausgestaltung bleibt Entwurf.

### 3.4 Spontaner Waffenwechsel und Handgemenge

Siehe [Modul 04](04-Nahkampf-und-Kampfstile.md) (`COMBAT-DEC-NK-023`, `-024`): Wechsel zwischen Schusswaffe, Messer und Improvisation sind fließend, Gegner können eine Waffe greifen, die Mündungsrichtung bleibt bedeutsam. Für den Fernkampf folgt daraus (🧩 ABGELEITET): **Der Schuss aus dem Handgemenge verlangt eine Richtungsprüfung** (wohin zeigt die Mündung, wer steht in der Schusslinie), weil Friendly Fire möglich ist (`COMBAT-DEC-GP-023`).

---

## 4. Projektilverhalten und Ballistik

### 4.1 Was die Quellen berechnen lassen (📋)

| Größe | Quelle | Bemerkung |
|---|---|---|
| Flugzeit, Fall, Luftwiderstand, Geschwindigkeits- und Energieverlust | `M-PD`, `23#134E` | plausible Formeln, nicht perfekte |
| Vorhalten bewegter Ziele aus der Flugzeit | `M-PD`, `23#134E` P8 | ‚das ist Pflicht‘ |
| Wind (stärker bei langer Flugzeit), böiger Wind | `M-PD`; `23#138E` P5–P6 (Antworten ‚passt‘) | Böen als Annäherung |
| Höhe, Luftdichte; Temperatur intern | `M-PD`; `23#138E` P8–P9 | Temperatur ohne großen Einfluss |
| Schusswinkel, Visierhöhe, Cant | `M-PD`; `23#138E` P10–P12 | Cant nur bei Fernschüssen relevant |
| Transschall, Spin-Drift (angenähert), Coriolis (nein) | `M-PD`; `23#138E` P13–P15 | – |
| Laufeinflüsse, Munitionsstreuung | `23#137A` P16–P17 (Entwurf); `23#138E` P16 | – |

### 4.2 Was die Quellen ausdrücklich nicht verlangen

Coriolis (`23#138E` P15), auf 0,1 Prozent genaue Luftdichte, Temperaturunterschiede der Munition bei normalen Kampfdistanzen, jede aerodynamische Geschossabweichung, militärische Long-Range-Simulation (`23#125A`, Aufzählung des Assistenten ‚Was ich nicht zwingend simulieren würde‘, vom Entwickler mit `23#126E` akzeptiert). Hitzeflimmern (Mirage) steht als Punkt 20 der Assistentenliste; eine Antwort des Entwicklers darauf ist nicht zuzuordnen.

### 4.3 Materialdurchschlag

Quellen: `COMBAT-DEC-FK-016`, `-017`. Der Entwickler wählt Variante A des Assistenten: Wenn jemand hinter einer dünnen Holzwand steht, kann man tatsächlich durchschießen, Gegner ebenfalls; **massive Deckung stoppt** (`23#126E`). Die Berechnung berücksichtigt (nach `23#127A`, Entwurf des Assistenten, vom Entwickler mit ‚ja genau‘ bestätigt, `23#128E`): Kaliber, Munitionstyp, Geschossenergie, Auftreffwinkel, Material, Materialdicke, mehrere Schichten, Restenergie.

| Deckung (Beispiele der Quellen) | Verhalten (Richtung) | Status |
|---|---|---|
| Holztür, dünne Wand, leichte Möbel | durchschlagbar mit Energieverlust | 📋 |
| dicker Beton, schwere Metalltür, massiver Pfeiler | stoppt; Einschussloch, Abplatzung, Verformung, Funken | 📋 |
| Fahrzeugblech | durchschlagbar nach Struktur | 📋 (Richtung) |
| Motorblock | schwere Deckung | 🧭 (`M-P07`: Themeninventar) |
| Glas | bricht, lenkt ab, verringert Energie | 📋 |

Zahlenwerte für Materialwiderstände fehlen (`CQ-45`).

### 4.4 Abpraller

Der Entwickler: relativ selten, erst auf Grounded, vorher einschaltbar (`23#130E`). Mehrere Bedingungen müssen zusammenkommen: harter Untergrund, ausreichend Restenergie, passender Auftreffwinkel, geeigneter Geschosstyp und geeignete Oberfläche (`23#131A`, Ausgestaltung des Assistenten, 💡). Sichtbare Splitter und Staub sind Darstellung; zusätzlicher Schaden entsteht nur in wenigen Sonderfällen (`COMBAT-DEC-FK-019`).

---

## 5. Entwurf: Schusspipeline

> **💡 VORSCHLAG.** Die Pipeline zeigt die Reihenfolge, nicht die Mathematik. Sie verwendet keine Zahlen.

```text
SCHUSS-ABSICHT (Abzug)
  1. Waffe prüft Zustand      Patronenlager gefüllt? Störung? -> Ablehnung mit Grund
  2. Streuung bestimmen       Körperzustand + Haltung + Waffe -> Abweichungsvektor (Zufall mit Seed)
  3. Projektil erzeugen       Mündungsgeschwindigkeit, Masse, Luftwiderstandsprofil, Aufbau
  4. Rückstoß anwenden        Impuls -> Waffenbewegung, Spielerreaktion (Körpermodell)
  5. Geräusch ausgeben        NoiseEmitted (Schalldämpfer, Überschallknall, Raum)
  6. Flug in Schritten        Zeitschritt: Gravitation, Luftwiderstand, Wind -> Zustandsänderung
  7. Segmenttest              Strecke vs Welt: Material, Winkel, Dicke -> durchdringen | stoppen | abprallen
  8. Körpertreffer            wenn Segment einen Körper trifft: Treffer an Verletzungsmodell (Modul 12)
  9. Restzustand weiter       Projektil mit verringertem Zustand weiter, bis gestoppt oder Lebensdauer erreicht
 10. Ereignisse               HitResolved, EvidenceCreated (Einschuss, Hülse), WorldConsequence bei Folgen
```

Jeder Schritt bekommt Zeit und Zufall von außen (deterministisch testbar, `COMBAT-DEC-ZA-008`). Schritt 7 ist der Ort, an dem die Unreal-Welt-Traces beginnen (Adapter); Schritte 2, 3, 6, 9 sind portable Logik.

---

## 6. Gegenüberstellung der Projektilmodelle

Die Quellen fordern eine leistungsbewusste Näherung und verbieten die künstliche Maximalreichweite. Wie das technisch umgesetzt wird, ist nicht entschieden (`CQ-19` bis `-20`, Modul 25).

| Ansatz | Beschreibung | Vorteil | Nachteil |
|---|---|---|---|
| A: Alle Projektile simuliert | jeder Schuss ein Projektil mit Zeitschritten | einheitlich, trägt Flugzeit, Wind, Vorhalten | teuer bei Dauerfeuer und Schrot, Kollisionsprüfung pro Schritt |
| B: Hybrid nach Entfernung | Nahdistanz sofortiger Treffer mit analytischem Korrektiv, Ferndistanz simuliert | günstig im Normalfall (unter etwa 100 m) | Übergang muss nahtlos sein; zwei Pfade zu testen |
| C: Analytische Flugbahn in Segmenten | geschlossene Näherung der Bahn, Test von Segment zu Segment | schnell, deterministisch | Näherungsfehler bei Wind und Böen; weniger flexibel |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen. Sie berühren die Leistungsziele (Modul 25) und die Netzwerksynchronisierung (Modul 23).

---

## 7. Long Range, Üben, Kill-Cam

1. **Long Range ist vereinzelt** (`COMBAT-DEC-FK-025`), aber vollständig: Entfernung, Nullpunkt, ballistischer Koeffizient, Luftwiderstand, Wind, Vorhalten, Höhe, Temperatur, Winkel, Optikhöhe, Cant (`23#137A`, vom Entwickler in `23#138E` beantwortet).
2. **Entfernung messen:** Im härtesten Modus ein tatsächliches Messgerät (`COMBAT-DEC-FK-027`, 🧭); ob andere Stufen eine Entfernungsanzeige haben, ist offen (`CQ-20`).
3. **Üben:** Schießstände, Safehouse-Training, freies Training, konfigurierbare Zielaufbauten (`23#140A` als Entwurf, Zustimmung `23#141E` P1–P4); Wettbewerbe gegen andere Leute (`COMBAT-DEC-FK-026`, Umfang `CQ-22`).
4. **Kill-Cam:** Filmische und anatomische Variante, beide konfigurierbar, selten als Standard (`COMBAT-DEC-FK-028`). Eine ausdrückliche Zuordnung der Option ‚B‘ in `23#141E` (‚B wäre wie in Sniper Elite‘) zur anatomischen Kameravariante ist wahrscheinlich, aber nicht durch die Quelle belegt, weil die Frage in `23#140A` zwei Optionen mit anderen Kennungen führt (`CU-03`).
5. **Akustik:** Schüsse sind laute Weltgeräusche; ein Schalldämpfer macht Überschallschüsse nicht lautlos. Folgen für die KI: [Modul 18](18-Gegner-KI-und-Kampftaktik.md).

---

## 8. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Munition ([Modul 10](10-Munition-Nachladen-und-Waffenzustand.md)) | Patronenlager, Magazin, Projektildefinition |
| zum Treffermodell ([Modul 12](12-Treffermodell-und-Koerperphysik.md)) | Trefferkette (Material, Körper) |
| zur KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)) | Geräusche, Mündungsfeuer, Sicht |
| zu Beweisen ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) | Hülsen, Einschüsse, Ballistikvergleich |
| zur Steuerung ([Modul 06](06-Steuerung-und-Eingabekontext.md)) | Anschlag, Zielhilfe, Combat Focus |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Autorität der Treffer, Projektilreplikation |
| zur Performance ([Modul 25](25-Performance-Skalierung-und-Simulationsstufen.md)) | Projektilzahl, Lebensdauer, Traces |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Schuss ohne Patrone im Lauf | Ablehnung mit Grund, Geräusch des Leerschlags, kein Projektil | 💡 |
| Schuss mit Störung | Störungszustand setzt ein (Modul 10) | 📋 (selten, `M-PD`) |
| Projektil verlässt die geladene Weltregion | Flug wird zu Ende gerechnet oder deterministisch beendet; kein stiller Verlust ohne Protokoll | 💡 (`CQ-19`) |
| Treffer auf Objekt mit unbekanntem Material | Standardmaterial mit Warnung | 💡 |
| Mehrere Projektile (Schrot) treffen dieselbe Region | Summe wird im Verletzungsmodell zusammengeführt | 💡 |

---

## 9. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-17` | Deckungssystem im Gunplay (Anlehnen, Blindfeuer, Deckungswechsel, Gegnerverhalten in Deckung) | `COMBAT-DEC-FK-031` | ja |
| `CQ-18` | Schießen aus und auf Fahrzeuge als Kampfmechanik | `COMBAT-DEC-FK-017`, `-032` | ja |
| `CQ-19` | Streuung, Rückstoß- und Schützenfehlermodell; Projektilmodell (simuliert, hybrid, analytisch) und Lebensdauer | `COMBAT-DEC-FK-013`, `-014`, `-033` | ja |
| `CQ-20` | Hilfen für Entfernung und Wind je Schwierigkeit (Entfernungsmesser, Anzeige, Windfahne) | `COMBAT-DEC-FK-027` | ja |
| `CQ-21` | Kill-Cam: Kriterien für ‚hochwertig‘, Standardhäufigkeit, Auslösung auch im Nahkampf | `COMBAT-DEC-FK-028` | ja |
| `CQ-22` | Schießwettbewerbe und Events ‚gegen andere Leute‘: NPC oder Spieler, Umfang | `COMBAT-DEC-FK-026` | ja |
