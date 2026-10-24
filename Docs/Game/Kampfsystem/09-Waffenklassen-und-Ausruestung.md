# Kampfsystem 09: Waffenklassen und Ausrüstung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul sammelt, was die Quellen über Waffen, Trageweise, Zugriff, Rarität und Ausrüstung entscheiden, und trennt es von Listen und Ordnungsentwürfen. **Es gibt keine beschlossene Waffenliste, keine Kaliberliste und keine Waffenwerte.** Die Klassen unten sind Themeninventar. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Munition, Nachladen und Waffenzustand: [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md); Fernkampf: [Modul 05](05-Fernkampf-und-Schusswaffen.md); Nahkampf: [Modul 04](04-Nahkampf-und-Kampfstile.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Waffenklassen als Themeninventar](#3-waffenklassen-als-themeninventar)
4. [Trageorte und Zugriff (Entwurf)](#4-trageorte-und-zugriff-entwurf)
5. [Vorbereitung und Ausrüstung vor der Jagd](#5-vorbereitung-und-ausrüstung-vor-der-jagd)
6. [Seltene und legendäre Jägerwaffen](#6-seltene-und-legendäre-jägerwaffen)
7. [Waffen in der Welt: Verlust, Beweis, Besitz](#7-waffen-in-der-welt-verlust-beweis-besitz)
8. [Gegenüberstellung offener Lösungsansätze](#8-gegenüberstellung-offener-lösungsansätze)
9. [Schnittstellen und Fehlerfälle](#9-schnittstellen-und-fehlerfälle)
10. [Offene Punkte dieses Moduls](#10-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Auswahl ist gewollt, Zahlen sind es nicht.** Der Entwickler verlangt viel Auswahl, ‚auf jeden Fall mehr als nur die Standardwaffen‘, und realistisch gebaute Waffen (`23#86E` P8, P9). Die Quellen enthalten keine Waffenliste.
2. **Wirkung ist nicht gleich Rang.** Eine alte Pumpgun kann gegen das richtige Monster mehr wert sein als ein taktisches Gewehr (`23#85A` P8; `23#86E` P8 ‚ja‘).
3. **Tragen ist begrenzt, Vorbereitung zählt.** Ein paar Waffen sind tragbar, der Rest liegt im Fahrzeug oder im Unterschlupf; der Zugriff hängt vom Trageort ab (`M-PD`, `23#86E` P10).
4. **Seltene Hunterwaffen sind eigene Erfindungen.** Der Entwickler wünscht Anspielungen auf bekannte Waffen der Serie, akzeptiert aber ‚sicherer‘ mit ‚etwas Ähnlichkeit‘ (`23#88E`, `23#91E`); die rechtliche Absicherung ist Pflicht (`K-JA-010`).
5. **Offen:** Waffenliste, Kaliber und Herstellernamen (real oder fiktiv), Körperschutz, Zahl und Beschaffung seltener Waffen, Inventarmodell (Slots, Gewicht, Zugriffszeiten) und das Verhältnis zum Recht (offenes Tragen).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-WA-001 | **Es gibt viel Waffenauswahl, ‚auf jeden Fall mehr als nur die Standardwaffen‘;** jede Waffe wird in den Assets realistisch gebaut | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P8, P9 | WAF | Liste: `CQ-33` |
| COMBAT-DEC-WA-002 | **Nicht jede Waffe ist ein höherer Schadenswert.** Eine alte Pumpgun kann gegen das richtige Monster wertvoller sein als ein taktisches Gewehr | 📋 BESCHLOSSEN | `23#85A` P8; `23#86E` P8; `COMBAT-DEC-GP-001` | WAF KRE | – |
| COMBAT-DEC-WA-003 | **Waffenklassen (Entwurf des Assistenten):** Handfeuerwaffen (Pistolen, Revolver), Langwaffen (Schrotflinten, Jagdgewehre, Repetierer, halbautomatische Gewehre), Nahkampf (Messer, Machete, Axt, improvisiert, spezielle Hunter-Waffen), Spezialausrüstung (Salz-, Silber- und Eisenmunition, Brandmittel, besondere Projektile, Fallen, Bann- und Ritualgegenstände) | 🧭 RICHTUNG | `23#85A` P8 (Liste); `23#86E` P8 (‚viel Auswahl‘) | WAF | Abschnitt 3; Liste ist Inventar |
| COMBAT-DEC-WA-004 | **Der Spieler trägt mehrere Waffen, aber nicht alles;** die Vorbereitung vor der Jagd zählt; Kofferraum und Unterschlupf sind größere Lager | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P10 (‚ein paar Waffen kann man tragen, deswegen muss man vor dem Kampf gut mitdenken‘) | WAF | – |
| COMBAT-DEC-WA-005 | **Der Zugriff hängt vom Trageort ab:** Holster oder Gürtel schnell, Rucksack langsamer, Kofferraum nicht als unmittelbarer Kampfzugriff | 📋 BESCHLOSSEN | `M-PD` ‚Multiple-opponent combat…‘; Gameplay-Systeme §5.1 | WAF STE | Zeiten: `CQ-36` |
| COMBAT-DEC-WA-006 | **Der Körper hat begrenzten Stauraum;** Fahrzeuge bieten Lager; das Hauptquartier kann sehr große, nachgefüllte Vorräte halten | 📋 BESCHLOSSEN | Gameplay-Systeme §9.3; Proposal 0007 §2 (I1: akzeptierte Richtung, keine Zahlen) | WAF | Kein Kapazitätsmodell (`CQ-36`) |
| COMBAT-DEC-WA-007 | **Waffen und andere Gegenstände sind lose physische Objekte;** sie können physisch verloren und aufgehoben werden, mit Bereinigung und Persistenz | 📋 BESCHLOSSEN | Gameplay-Systeme §2; `M-PD` ‚Cash / law / evidence‘ | WAF PER | – |
| COMBAT-DEC-WA-008 | **Wichtige abgelegte Waffen bleiben physisch und können Beweise werden;** eine intelligente Bereinigung verhindert unbegrenzten Müll | 📋 BESCHLOSSEN | `M-PD` ‚Cash / law / evidence‘; `E-QA-023`; Proposal 0007 §2 (I2) | WAF POL | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-WA-009 | **Seltene, legendäre Jägerwaffen existieren** (Beispiele des Entwicklers: ein berühmter Revolver und ein Dämonenmesser der Serie), sind ‚richtig schwer‘ zu bekommen und werden als **eigene** Waffen mit eigener Geschichte, eigenem Design und eigenen Regeln umgesetzt; ‚etwas Ähnlichkeit‘ bleibt gewollt | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P8; `23#88E`; `23#91E`; `K-JA-010` | WAF KRE | Zahl und Beschaffung: `CQ-35` |
| COMBAT-DEC-WA-010 | **Rechtliche Absicherung der Waffen- und Requisitengestaltung ist Pflicht;** eine IP-Clearance vor der Veröffentlichung wird eingeplant | 📋 BESCHLOSSEN | `23#88E` (‚ich will auf jeden Fall abgesichert sein‘); `23#90A`; `K-IP-003` (Eigene Namen und Designs statt serienspezifischer Ausgestaltung) | WAF | Clearance ist Prozess, nicht Spielregel |
| COMBAT-DEC-WA-011 | **Waffen sind mechanisch korrekt gebaut** (Magazine, Patronen, Verschluss, Ladeanimation, Auswurf, Abzug, Sicherung, Rückstoß, Länge, Gewicht, Sound) | 📋 BESCHLOSSEN | `M-PD` ‚Gunplay / weapons‘; `23#86E` P8; Entwurfsliste `23#87A` | WAF ANI AUD | Assetpflichten: [Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md) |
| COMBAT-DEC-WA-012 | **Begleiter besitzen echte Waffen, Kleidung und Inventar** | 📋 BESCHLOSSEN | `N-CO-005` | NPC WAF | – |
| COMBAT-DEC-WA-013 | **Keine farbcodierte Seltenheit** und keine Schadenszahlen als Fortschrittsmotor; Seltenheit entsteht aus Geschichte, Beschaffung und Regeln | 📋 BESCHLOSSEN | `M-PD` ‚Genre and progression identity‘; `COMBAT-DEC-GP-003` | WAF UI | – |
| COMBAT-DEC-WA-014 | **Lichtquellen (Taschenlampe, Waffenlicht, Fahrzeuglicht, Lampen, Generator) sind taktische Werkzeuge und Risiken;** manche Kreaturen können Licht und Strom manipulieren | 📋 BESCHLOSSEN | `M-PD` ‚Stealth, perception and horror AI‘; `23#117A` (Beispiele) | WAF KI | Waffenlicht als Anbauteil: Entwurf |
| COMBAT-DEC-WA-015 | **Bei Niederlage kann Ausrüstung teilweise verloren gehen oder am Einsatzort zurückbleiben** (Fahrzeug, Waffen, Rucksack) | 🧭 RICHTUNG | Spielvision §14.2; `23#61A` (Folgenliste, Assistent); `M-PD` ‚Death / failure — OPEN‘ | WAF PER | [Modul 14](14-Bewusstlosigkeit-Tod-und-Rettung.md) |
| COMBAT-DEC-WA-016 | **Kleidung liegt im Projektilweg und hat eine Schutzfunktion** (Kleidungsschaden); Ausmaß der Schutzwirkung von Kleidung und besonderem Körperschutz (Westen, Helme) und die Rüstungsparameter sind nicht entschieden | ❓ OFFEN | `M-PD` ‚Ballistics‘ (Projektilpfad durch Kleidung); Gameplay-Systeme §9.3 (Kleidung hat Wetter-, Schutz-, Stauraum- und soziale Funktion, kann nass, schmutzig, blutig oder beschädigt werden); `M-OD` ‚Combat / injury / gore‘ (Rüstungsparameter) | TRF WAF | `CQ-34` |
| COMBAT-DEC-WA-017 | **Konkrete Waffenliste, Kaliber, Modelle und Herstellernamen (real oder fiktiv) sind nicht festgelegt** | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle); `M-OD` | WAF | `CQ-33` |
| COMBAT-DEC-WA-018 | **Inventarmodell für Waffen** (Slots, Gewicht, Stauraumgrenzen, Zugriffszeiten, Schnellwechsel) ist nicht entschieden | ❓ OFFEN | Proposal 0007 §2 (‚Searched, not found‘); `23#96A` (Aufgabe ‚Inventar‘) | WAF UI | `CQ-36` |
| COMBAT-DEC-WA-019 | **Weitere Waffenarten (Bogen, Armbrust, Wurfwaffen, Brandmittel, Sprengstoff)** sind nicht beschrieben; beschlossen sind Brand und Explosion als Traumaarten, nicht ihre Waffen | ❓ OFFEN | `M-PD` ‚Human anatomy…‘ (Traumaarten); `23#85A` P8 (‚Brandmittel‘, ‚besondere Projektile‘, Entwurf); `23#103A` P13 (‚Arrow/Projectile‘, Entwurf) | WAF TRF | `CQ-39` |
| COMBAT-DEC-WA-020 | **Waffenbesitz und offenes Tragen im Recht** (Tragen, Genehmigungen, illegale Waffen) sind nicht entschieden | ❓ OFFEN | `23#96A` (‚Waffen offen tragen‘, Assistent); `M-OD` ‚Crime / law / evidence‘ | POL | `CQ-81` |
| COMBAT-DEC-WA-021 | **Kauf, Wartung und Reparatur von Waffen sind Teil der Wirtschaft** (Ausgaben für Waffen, Munition, Jäger-Material) | 📋 BESCHLOSSEN | Gameplay-Systeme §13 (Single-Player-Wirtschaft) | WAF | Preise offen |

---

## 3. Waffenklassen als Themeninventar

> **Das ist Inventar, keine Entscheidung.** Die Tabelle zeigt, welche Klassen die Quellen nennen und was zu jeder Klasse offen ist.

| Klasse | Nennung | Aussagen der Quellen | Offen |
|---|---|---|---|
| Pistolen | `23#85A` P8; `23#115A` P9 | Schlitten, Slide Lock, Magazin, Patronenlager, schwache Hand, enge Haltung | Modelle, Kaliber |
| Revolver | `23#85A` P8; `23#115A` P9 | Trommel, Kammern, Auswurf, Speedloader je Modell (Entwurf) | Modelle |
| Schrotflinten | `23#85A` P8; `23#155E` P3–P4, P14 | Röhrenmagazin, Patrone für Patrone, Buckshot, Birdshot, Slug; Nahdistanz schwere Wunden | Modelle, Laufarten |
| Jagdgewehre und Repetierer | `23#85A` P8; `23#133A`–`23#141E` | Long Range, Optik, Zweibein | Kaliber, Optiken |
| Halbautomatische Gewehre | `23#85A` P8 | Magazinwechsel, Wartung | Modelle |
| Messer, Klingen | `23#162A` P2 | klein, schnell, eng brauchbar | Liste |
| Mittlere Nahkampfwaffen | `23#162A` P2 | Machete, Beil, Hammer, Brecheisen: mehr Wirkung, langsamer | Liste |
| Große Nahkampfwaffen | `23#162A` P2 | Axt, langer Hammer: hohe Wirkung, schwer, langsam, schlecht in engen Räumen, Ausdauerverbrauch | Liste |
| Improvisiert | `23#222E` P5 | Flasche, Dose, Werkzeug, Stein, Stuhlbein, Metallstange | Gegenstandsliste |
| Hunter-Spezialwaffen | `23#85A` P8; `K-JA-010` | selten, eigene Lore | Liste |
| Spezialmunition | `23#85A` P9; `23#155E` P5 | Salz, Silber, Eisen, Brandmittel (Entwurf) | Modul 10 |
| Fallen, Bann-/Ritualgegenstände | `23#85A` P8 | Kreaturenmodul | [Modul 17](17-Fallen-Fesselung-und-Lebendfang.md) |

---

## 4. Trageorte und Zugriff (Entwurf)

Die Quellen beschreiben nur drei Trageorte ausdrücklich (`M-PD` ‚Multiple-opponent combat…‘): Holster oder Gürtel, Rucksack, Kofferraum. Weitere Orte sind Entwurf.

| Trageort | Zugriff im Kampf | Status |
|---|---|---|
| Holster, Gürtel | schnell | 📋 |
| Rucksack | langsamer | 📋 |
| Kofferraum | nicht als unmittelbarer Kampfzugriff | 📋 |
| Unterschlupf, Hauptquartier | nicht im Einsatz; Vorbereitungsort | 📋 (Gameplay-Systeme §5.1) |
| Schulter, Sling, Rücken (Langwaffe) | Zugriff mittel (Sling-Übergänge als Idee) | 💡 (`M-P07`, ‚sling-holster-transition-hooks‘) |
| Hand, Boden | sofort bzw. aufheben | 📋 (lose Objekte, `COMBAT-DEC-WA-007`) |
| Begleiter, Fahrzeugkabine | nicht beschrieben | ❓ |

**Rechenmodell (💡):** Zugriffszeit als Funktion von Trageort, Gegenstandsklasse und Körperzustand (Verletzung der Zugriffshand verlangsamt). Werte fehlen absichtlich; die Zeit ist Konfiguration (`CQ-36`).

---

## 5. Vorbereitung und Ausrüstung vor der Jagd

Der Entwickler bestätigt den Zusammenhang Archiv → Hypothese → Ausrüstung (`23#85A` P10, `23#86E` P10). Aus den Quellen (`COMBAT-DEC-WA-004`, `K-KO-014`, `K-JA-001`) folgt der Ablauf:

```text
Hunter Archive / Recherche
  -> Hypothese über das Wesen und seine Schwäche
  -> Kofferraum / Unterschlupf: Auswahl der Ausrüstung (Waffe, Munition, Material)
  -> Einsatz mit begrenztem Körperinventar
  -> Merkt man im Einsatz, dass etwas fehlt (Eisen, Salz): Fahrzeug 800 m entfernt = echtes Gameplay
```

Die letzte Zeile ist ein Beispiel des Assistenten (`23#85A` P10) und vom Entwickler mit ‚ja ein paar Waffen kann man tragen‘ beantwortet. Sie ist **Richtung**, nicht Zahl.

**Verknüpfung (🧩 ABGELEITET):** Die Schwäche eines Wesens ist Wissen aus dem Archiv, nicht aus einem Wert im Spiel (`K-FO-001`); die Auswahl der Ausrüstung entsteht aus unvollständigem, möglicherweise falschem Wissen (`K-FO-003`). Das Kampfsystem darf deshalb keine „richtige“ Waffe anzeigen, die der Spieler nicht erforscht hat.

---

## 6. Seltene und legendäre Jägerwaffen

Quellenlage: `M-PD` verlangt ‚original rare/legendary hunter weapons with original IP-safe lore‘; der Entwickler nannte als Wünsche einen berühmten Revolver der Serie und das Dämonenmesser, ‚richtig schwer‘ zu bekommen (`23#86E` P8), und akzeptierte nach der Warnung des Assistenten, ‚das sicherer‘ zu machen, ‚es sollte nur noch etwas Ähnlichkeit dabei sein‘ (`23#91E`).

**Regeln aus den Quellen:**

1. Eigene Namen, Designs, Gravuren, Herkunft, Hersteller, Funktionsregeln und Questlinien (`23#90A`, Empfehlung des Assistenten, vom Entwickler mit `23#91E` übernommen).
2. Der Entwurf des Assistenten für die Mechanik (💡): extrem selten, besondere Herstellung, sehr wenige Schüsse, außergewöhnliche Wirkung; Messer mit besonderen Materialien, wirkt gegen bestimmte Wesen (`23#87A`).
3. Gameplay-Systeme §5.1: ‚Das Arsenal enthält … originale, seltene oder legendäre Jägerwaffen mit IP-sicherem Hintergrund‘.
4. Prüffrage für Gestaltung (Empfehlung des Assistenten, `23#92A`): Funktioniert das Element weiter, wenn der Serienvergleich entfernt wird? (💡, interne Prüfhilfe.)

**Offen:** Anzahl, Beschaffung (Questreihen, Funde, Handel), Munitionsknappheit, Zusammenhang mit der Hauptgeschichte und mit dem Archiv (`CQ-35`).

---

## 7. Waffen in der Welt: Verlust, Beweis, Besitz

| Aspekt | Aussage der Quellen | Modul |
|---|---|---|
| Entwaffnung | Waffe fällt, liegt am Boden, muss aufgenommen werden; keine „magische Rückkehr ins Inventar“ (Entwurf `23#162A` P14, vom Entwickler mit ‚passt‘ aufgenommen) | [Modul 04](04-Nahkampf-und-Kampfstile.md) `COMBAT-DEC-NK-045` |
| Ablage am Tatort | wichtige Waffen bleiben physisch und können Beweise werden | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| Beschlagnahme | Durchsuchung, Beschlagnahme, Fahrzeugdurchsuchung nach Regeln (Wunsch) | `E-PO-005` |
| Ballistik als Beweis | Waffen und Ballistik sind Beweisquellen; Forensikkatalog nennt Ballistik | `E-RE-002`, `E-SP-007` |
| Flohmärkte, Pfandläden, Hehler | wechselnder Bestand, gelegentlich Jäger-Relevantes | Gameplay-Systeme §10 |
| Waffenpflege | moderat, siehe Modul 10 | [Modul 10](10-Munition-Nachladen-und-Waffenzustand.md) |

---

## 8. Gegenüberstellung offener Lösungsansätze

| Offene Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Herstellernamen und Modelle (`CQ-33`) | A: reale Marken und Modelle | maximaler Wiedererkennungswert | Markenrecht und Lizenz vor Veröffentlichung (`23#90A`); Kosten |
| | B: fiktive Hersteller mit realen Bauformen | rechtlich sicherer, Tiefe der Mechanik bleibt | Fans erkennen weniger sofort; Gestaltungsaufwand |
| | C: Mischung (generische Bezeichnungen wie Typ und Kaliber, ohne Markenlogo) | pragmatisch | Konsistenz der Namen |
| Inventar für Waffen (`CQ-36`) | A: freie Slots am Körper (Holster, Gürtel, Rücken, Sling) | physisch lesbar, passt zu Zugriff nach Trageort | UI-Aufwand |
| | B: Gewichtsbasiert | realistisch | Verwaltung, Gefahr von „Excel“ |
| | C: Gruppenslots (Schnellwahl) | schnell | weniger Physik |
| Körperschutz (`CQ-34`) | A: keiner (nur Kleidung) | einfach, passt zu ‚kein Panzer‘ | keine Antwort auf Westen-tragende Gegner |
| | B: Kleidungsschichten mit Schutzwirkung | passt zum Projektilpfad durch Kleidung | Datenmodell, Balance |
| | C: spezielle Schutzausrüstung | taktische Tiefe | Gefahr, „Level“-Rüstung zu erzeugen (widerspricht `COMBAT-DEC-GP-003`) |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 9. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Inventar | Gegenstände, Trageorte, Zugriffszeit |
| zur Munition ([Modul 10](10-Munition-Nachladen-und-Waffenzustand.md)) | Kaliber, Magazine |
| zu Kreaturen ([Modul 16](16-Schwaechen-Vorbereitung-und-Jagdmethoden.md)) | Wirksamkeit gegen Schwäche |
| zur Wirtschaft | Kauf, Wartung, Reparatur |
| zur Polizei ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) | Besitz, Beschlagnahme, Beweise |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Waffenbesitz je Spieler, Inventar im Host-Spielstand |
| zu Mods ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | neue Waffen additiv, kein Eingriff in das Basisspiel (`N-KO-010`) |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Waffe im Rucksack wird im Kampf angefordert | Zugriff dauert; Ablehnung nur, wenn der Zustand es verbietet | 💡 |
| Waffe fällt in unzugängliche Zone (Wasser, Spalt) | definiertes Verhalten (nicht auffindbar, geborgen oder verloren) mit Protokoll | 💡 |
| Mod-Waffe mit unbekanntem Kaliber | Ablehnung mit Meldung, Basisspiel unverändert | 💡 |
| Speichern mit Waffe in der Hand während eines Wechsels | Zustand wird gespeichert und fortgesetzt | 📋 (Reload-Zustand persistent, `M-PD`) |
| Beschlagnahme während Ausrüstungsvorbereitung | Beweisregeln (Modul 20) haben Vorrang | 💡 |

---

## 10. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-33` | Waffenliste, Kaliber, Herstellernamen (real oder fiktiv) und Nahkampfwaffenliste | `COMBAT-DEC-WA-001`, `-003`, `-017` | ja |
| `CQ-34` | Körperschutz und Schutzwirkung von Kleidung; Rüstungsparameter | `COMBAT-DEC-WA-016` | ja |
| `CQ-35` | Seltene Hunterwaffen: Zahl, Beschaffung, Munitionsknappheit, Verknüpfung zur Hauptgeschichte | `COMBAT-DEC-WA-009` | ja |
| `CQ-36` | Inventarmodell für Waffen: Slots, Gewicht, Zugriffszeiten, Schnellwechsel | `COMBAT-DEC-WA-005`, `-006`, `-018` | ja |
| `CQ-39` | Weitere Waffenarten (Bogen, Armbrust, Wurfwaffen, Brandmittel, Sprengstoff) | `COMBAT-DEC-WA-019` | ja |
