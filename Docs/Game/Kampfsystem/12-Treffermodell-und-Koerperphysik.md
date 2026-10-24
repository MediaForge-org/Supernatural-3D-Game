# Kampfsystem 12: Treffermodell und Körperphysik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul beschreibt, was die Quellen über Trefferzonen, den Weg eines Treffers durch Kleidung, Gewebe, Knochen, Organe und Gefäße, die Folgen im Körper und die physikalische Plausibilität entscheiden. Datenmodelle sind **Entwurf (💡)**. Es gibt keine Schadens-, Gewebe-, Knochen- oder Rüstungswerte, weil `M-OD` sie ausdrücklich als offen führt. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Die Folgen im Körper (Blutung, Schmerz, Schock, Behandlung) stehen in [Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md); Animation und Ragdoll in [Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Trefferzonen und Körperschichten](#3-trefferzonen-und-körperschichten)
4. [Der Weg eines Treffers (Entwurf)](#4-der-weg-eines-treffers-entwurf)
5. [Wunden und Körperzustand (Entwurf)](#5-wunden-und-körperzustand-entwurf)
6. [Ursachen und Mechanismen](#6-ursachen-und-mechanismen)
7. [Körperphysik und Plausibilität](#7-körperphysik-und-plausibilität)
8. [Gegenüberstellung offener Lösungsansätze](#8-gegenüberstellung-offener-lösungsansätze)
9. [Schnittstellen und Fehlerfälle](#9-schnittstellen-und-fehlerfälle)
10. [Offene Punkte dieses Moduls](#10-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Anatomie ist detailliert, aber auf Spielrelevantes begrenzt:** etwa 25+ Trefferregionen, Weichgewebe, relevante Knochen, Gelenke, kritische Organe, große Arterien und kritische Gefäßzonen, vereinfachte Nerven und Sehnen (`M-PD` ‚Human anatomy, injury and medicine‘, `23#101E` P3, `23#104E` A).
2. **Der Entwickler wählte ausdrücklich die ausführlichere Variante** (Körperregionen plus wichtige Anatomie, `23#101E` P3: ‚das ist besser, es sollen wirklich viele Sachen angegeben werden, damit die Treffer realistisch sind und man somit auch besser verarzten kann‘). Er will aber keine vollständige medizinische Simulation jedes Organs (`23#98A`, Empfehlung des Assistenten, vom Entwickler mit `23#101E` P3 angenommen).
3. **Ein Treffer hat einen Weg,** keinen Schadenswert. Die Verletzung folgt dem Restzustand, der Durchdringung und der Anatomie entlang des Weges (`M-PD` ‚Ballistics‘, `COMBAT-DEC-ZA-001`).
4. **Menschen sind einheitlich:** Spieler und menschliche NPCs verwenden dieselbe Anatomie; Monster brauchen teilweise eigene Anatomie, die einzeln festgelegt wird (`23#101E` P18, P19; `23#104E` P18).
5. **Gore und Alter:** Die Darstellung ist ursachenabhängig; die Schwellen sind offen und von Plattform-, Alters- und Rechtsprüfung abhängig (`M-OD`, Gameplay-Systeme §4).
6. **Körperphysik:** Eine gemeinsame Sekundärphysik für Rückstoß, Laufen, Kampf, Fahrzeugbeschleunigung, Stürze und Tragen ist beschlossen; Zahlen und Technik sind offen (Gameplay-Systeme §9.3, `M-PD`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-TK-001 | **Anatomie mit etwa 25+ Trefferregionen,** Weichgewebe, relevanten Knochen, Gelenken, kritischen Organen, großen Arterien und kritischen Gefäßzonen sowie vereinfachten Nerven und Sehnen | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy, injury and medicine‘; `23#101E` P3; `23#104E` A | TRF MED | Liste der Regionen als Inventar: Abschnitt 3 |
| COMBAT-DEC-TK-002 | **Projektile folgen einem Weg** durch Kleidung, Materialien, Gewebe, Knochen, Organe und Gefäße; sie können austreten oder stecken bleiben; ein steckengebliebenes Projektil muss nicht zwingend entfernt werden (Behandlung hängt von Lage und Zustand ab) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#102A` (medizinische Korrektur); `23#103A` P2–P3 (Entwurf); `23#104E` | TRF MED | Wundklassen: Abschnitt 5 |
| COMBAT-DEC-TK-003 | **Traumaarten:** Schuss/Schrot/Splitter, Schnitt, Stich, stumpf/Quetschung, Bruch und offener Bruch, innere Verletzung, Verbrennung/Kälte/Elektrizität, Bisse/Krallen/Reißen, Explosion/Sturz/Fahrzeug/Trümmer, übernatürliche Modifikatoren | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; Gameplay-Systeme §4; `23#103A` P13 (Entwurf der Gliederung); `23#104E` P13 (‚machen wir später genauer‘) | TRF MED | Feingliederung offen |
| COMBAT-DEC-TK-004 | **Hauptarterien und kritische Gefäße müssen simuliert werden,** sonst kann man nicht ausbluten | 📋 BESCHLOSSEN | `23#104E` P1; `M-PD` ‚Human anatomy…‘ (‚major arterial injury can cause rapid exsanguination‘) | MED | Gefäßkategorien (Minor, Major, Critical): Entwurf von `23#103A` |
| COMBAT-DEC-TK-005 | **Schmerz ist vom physischen Schaden getrennt** (ein gebrochener Finger ist schmerzhaft und wenig gefährlich, eine innere Blutung gefährlich und zunächst weniger offensichtlich) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#100A` §6 (Beispiel); `23#101E` P6 | MED | Modul 13 |
| COMBAT-DEC-TK-006 | **Verletzungen summieren sich;** der Körper wird als Ganzes bewertet (Gesamtblutverlust, Schmerz, Schock, Beweglichkeit, Gliedmaßenfunktion, Bewusstsein, Ermüdung); der Tod entsteht aus dem Körperzustand, nicht aus der sechsten Wunde | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘ (‚injuries accumulate…‘); `23#101E` P4 (‚zu viele Treffer und man ist unfähig sich zu bewegen oder sogar sterben‘); `23#103A` P6 | MED | – |
| COMBAT-DEC-TK-007 | **Offene Brüche mit sichtbarem Knochen sind möglich,** auch beim Spieler | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#101E` P9 (‚ja schlimmer, es können auch Knochen rausschauen möglicherweise‘), P16 | TRF ANI | Darstellung: Modul 22 |
| COMBAT-DEC-TK-008 | **Gore ist primär bei Gegnern stärker sichtbar,** aber der Spieler ist nicht immun gegen schwere sichtbare Verletzungen | 📋 BESCHLOSSEN | `23#101E` P16 (‚ich dachte eher bei den Gegnern, aber es kann sein, dass ein Knochen bei dir raussteht‘); `M-PD` (‚Player can also show visible wounds…‘) | ANI | – |
| COMBAT-DEC-TK-009 | **Verzögerte oder innere Verletzungen bleiben bis zur Diagnose unsicher;** Symptome treten später deutlicher auf | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` P4; `23#103A` P11–P12 | MED UI | Modul 13 |
| COMBAT-DEC-TK-010 | **Übernatürliche Verletzungen erweitern das normale System durch Modifikatoren** (zum Beispiel Heilungshemmung, Fluch), statt ein getrenntes Gesundheitssystem zu bilden | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#103A` P15; `23#104E` P15; `K-KO-010` | MED KRE | Modul 15 |
| COMBAT-DEC-TK-011 | **Menschliche NPCs und der Spieler teilen dieselbe Anatomie;** Monster brauchen teilweise eigene Anatomie, die einzeln festgelegt wird (‚das machen wir dann einzeln, ist eher komplex‘) | 📋 BESCHLOSSEN | `23#101E` P18, P19; `23#104E` P18, P19; `K-KO-006` | KRE MED | Modul 15 |
| COMBAT-DEC-TK-012 | **Feuer kann sich ausbreiten;** ein früher kleiner Kleidungsbrand lässt sich ersticken (zum Beispiel mit der Jacke) | 📋 BESCHLOSSEN | `M-PD` ‚Human anatomy…‘; `23#104E` P14 | TRF MED | Verbrennungsgrade: Entwurf |
| COMBAT-DEC-TK-013 | **Gemeinsame Sekundärphysik** für Rückstoß, Laufen, Kampf, Fahrzeugbeschleunigung und -unfälle, Stürze, Tragen, Interaktion, Haar, Kleidung und Zubehör; realistische Dämpfung und Masse, kein übertriebenes Wackeln | 📋 BESCHLOSSEN | Gameplay-Systeme §9.3; `M-PD` ‚Body / physical response / recoil‘ | ANI TRF | Technik: `CQ-48` |
| COMBAT-DEC-TK-014 | **Die Wirkung eines Schlags, Stichs oder Hiebes wird anatomisch ausgewertet** (Region, Tiefe, Gewebe, möglicherweise Gefäß oder Organ) | 📋 BESCHLOSSEN | `23#162A` P3; `23#163E` P3; `COMBAT-DEC-NK-027` | TRF | – |
| COMBAT-DEC-TK-015 | **Gegner sind keine Hitpoint-Puppen:** sichtbare Folgen am Gegner (Arm schützen, humpeln); später auch humpeln und sich das Bein halten | 📋 BESCHLOSSEN | `23#86E` P3; `COMBAT-DEC-NK-046` | KI ANI | ‚in späteren Versionen‘ (`23#86E` P3) |
| COMBAT-DEC-TK-016 | **Ursachenabhängige Wundeigenschaften** (Eindringen, Energie, Schneiden, Quetschen, Brennen, Zerreißen, Geschossdurchmesser, Fragmentierung, übernatürliche Wirkung) leiten Verletzungen und Darstellung ab | 💡 VORSCHLAG | `23#103A` P10 (Entwurf des Assistenten); `M-PD` (Gore ist ursachenabhängig) | TRF | Abschnitt 6 |
| COMBAT-DEC-TK-017 | **Wundklassen für Projektile** (Durchschuss, unproblematisch steckend, problematisch steckend, kontaminiert/kompliziert) | 💡 VORSCHLAG | `23#103A` P3 | MED | `CQ-45` |
| COMBAT-DEC-TK-018 | **Wundeintrag als Datensatz** mit Region, Mechanismus, betroffenen Strukturen, Austritt, Blutungsgrad, Schmerz- und Schockbeitrag | 💡 VORSCHLAG | `23#103A` P2 (Beispiel ‚Wound #18427‘); Abschnitt 5 | MED PER | `CQ-46` |
| COMBAT-DEC-TK-019 | **Materialdurchschlag, Gewebe-, Knochen- und Rüstungsparameter** (Zahlen, Messwerte, Kalibrierung) sind nicht entschieden | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘; Entscheidungsstatus §5.6 | TRF | `CQ-45` |
| COMBAT-DEC-TK-020 | **Geometrie der Trefferzonen** (Hit-Volumina, Abbildung von Collision auf die 25+ Regionen, Anpassung an kontinuierlich veränderte Körper) ist nicht entschieden | ❓ OFFEN | Gameplay-Systeme §9.2 (kontinuierliche Morph-Ziele); `M-P14` (Themeninventar ‚human-body-hit-region-rig-mapping‘) | TRF ANI | `CQ-46`, `CQ-50` |
| COMBAT-DEC-TK-021 | **Gore-Schwellen, visuelle Familien, regionale Profile und Darstellung bei minderjährigen NPCs** sind offen und von Plattform-, Alters- und Rechtsprüfung abhängig | ❓ OFFEN | `M-OD` ‚Combat / injury / gore‘; Gameplay-Systeme §4 (Darstellung); Entscheidungsstatus §5.6 | ANI | `CQ-47` |
| COMBAT-DEC-TK-022 | **Aufprall- und Impulsreaktion am Ziel** (Zurückstoßen, Stolpern, Niederreißen bei starken Treffern) ist in den Quellen nicht beschrieben; nur Rückstoß der eigenen Waffe und Körperreaktionen als Verletzungsfolge | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | ANI TRF | `CQ-48` |
| COMBAT-DEC-TK-023 | **Verletzung von Tieren und Hunden** (Hund als Begleiter mit Verletzung, Altern, Tierarzt; wilde Tiere als Ziel) nutzt eine tierartige Anatomie; Einzelheiten sind nicht beschlossen | ❓ OFFEN | Spielvision §6.3 (Hund: Verletzung, Tierarzt); [Kreaturen 18](../Kreaturen/18-Tiere-und-Abgrenzung.md) | TRF MED KRE | `CQ-49` |

---

## 3. Trefferzonen und Körperschichten

**Aufteilung der Quellen (Inventar, keine Entscheidung der Anzahl):** Der Entwickler übernimmt die Anzahl ‚25+‘ aus `M-PD`; die konkrete Liste ist ein Entwurf des Assistenten (`23#103A` P1) und wurde vom Entwickler als Richtung bestätigt (`23#104E` P21 ‚passt‘).

| Bereich | Regionen (Entwurf `23#103A`) | Darunter liegende Strukturen (Quelle) |
|---|---|---|
| Kopf | Schädel, Gesicht, Kiefer | Gehirn, Schädelknochen |
| Hals | Hals | große Gefäße, Atemwege (Entwurf) |
| Rumpf | Brust links/rechts, Bauch, Becken | Rippen, Lunge, Herz, Leber, eventuell Nieren und weitere Bauchorgane; Wirbelsäule |
| Arm | Schulter, Oberarm, Ellenbogen, Unterarm, Hand (je links/rechts) | Humerus, Unterarmknochen, wichtige Gefäße, Nerven, Sehnen |
| Bein | Oberschenkel, Knie, Unterschenkel, Knöchel, Fuß (je links/rechts) | Femur, Unterschenkelknochen, große Gefäße |
| Schichten | Kleidung, Haut, Weichgewebe/Muskel, Sehnen, Nerven, Knochen, Organe, Gefäße | `M-PD`; `23#103A` P1 |

Die genannten Regionen ergeben mehr als 25 Zonen, wenn links und rechts getrennt gezählt werden; die Quelle `M-PD` bleibt bei ‚roughly 25+ hit regions‘.

**Gefäßkategorien (Entwurf):** Minor, Major, Critical plus anatomische Lage (`23#103A` P1). Der Entwickler bestätigt die Notwendigkeit der Hauptarterien (`COMBAT-DEC-TK-004`).

**Nerven und Sehnen:** Der Assistent führt sie als einzigen weiteren offenen Punkt (`23#103A` P1); der Entwickler antwortet ‚ja das auch‘ (`23#104E` A). Ein Nerven- oder Sehnenschaden kann Kontrolle und Griff beeinträchtigen (`M-PD`).

---

## 4. Der Weg eines Treffers (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-TK-017`, `-018`).** Die Kette setzt `COMBAT-DEC-ZA-001` und `COMBAT-DEC-FK-016` um. Sie enthält keine Zahlen.

```text
Treffer-Ereignis (Projektil oder Schlag)
  1. Eintrittsregion bestimmen          Hit-Volumen -> Region
  2. Schichten durchlaufen              Kleidung -> Haut -> Weichgewebe -> Knochen? -> Organ? -> Gefäß?
  3. Je Schicht: Energie abgeben        Material, Winkel, Restenergie -> Verformung, Stopp, Durchdringung
  4. Strukturen markieren               beschädigte Struktur + Grad
  5. Austritt oder Verbleib             Austrittswunde | Projektil steckt (Lage: harmlos | problematisch)
  6. Wundeintrag anlegen                Region, Mechanismus, Strukturen, Austritt, Kontamination, Zeit
  7. Körperzustand aktualisieren        Blutung, Schmerz, Schock, Funktion, Bewusstsein (Modul 13)
```

**Beispiel der Quelle (`23#100A` §4, `23#103A` P2, Entwurf des Assistenten):** Dreimal derselbe Beinbereich, dreimal anderes Ergebnis: durch Muskelgewebe (Weichteilverletzung, mäßige Blutung, Humpeln, behandelbar), mit Femurtreffer (Bein kaum belastbar, Bewegung stark eingeschränkt), mit Gefäßtreffer (starke Blutung, Schock, Bewusstseinsverlust, Tod möglich). Der Entwickler: ‚ja genau so soll das sein‘ (`23#101E` P4).

**Prüfbare Eigenschaften (🧩 ABGELEITET aus `COMBAT-DEC-ZA-001`, `COMBAT-DEC-TK-002`):**

1. Die Restenergie nach jeder Schicht ist höchstens gleich der Energie davor (keine Energiezunahme im Körper).
2. Zwei Treffer auf dieselbe Region mit anderem Weg führen zu verschiedenen Wundeinträgen.
3. Austrittswunde und Verbleib schließen einander aus.
4. Dieselbe Eingabe (Seed, Weg, Material) liefert dasselbe Ergebnis.

---

## 5. Wunden und Körperzustand (Entwurf)

> **💡 VORSCHLAG.** Die Felder folgen der Beispielstruktur in `23#103A` P2 und den Systemzuständen aus `M-PD`.

**Wundeintrag (konzeptionell):**

| Feld | Inhalt |
|---|---|
| Kennung | stabile Wund-ID |
| Region | Eintrittsregion |
| Mechanismus | Projektil, Schnitt, Stich, stumpf, Biss, Hitze, Fall, Explosion, übernatürlich |
| Strukturen | beschädigte Haut, Muskel, Knochen, Organ, Gefäß, Nerv, Sehne mit Grad |
| Austritt | ja oder nein, zweite Wunde |
| Projektilverbleib | keiner, harmlos, problematisch |
| Blutungsgrad | je Wunde (Rate) |
| Kontamination | Fremdmaterial, Tiefe (für späteres Infektionsrisiko) |
| Zeit | Zeitpunkt der Verletzung |
| Behandlungszustand | unbehandelt, gestoppt, gereinigt, verbunden, professionell |

**Körperzustand (Mindestmenge nach `M-PD`):** Blutvolumen, Blutungsrate, Schmerz, Schock, Bewusstsein, Atmung, Adrenalin, Beweglichkeit, Gliedmaßenfunktion, Infektionsrisiko. Der Gesamtzustand ist die **Zusammenfassung aller Wunden plus Allgemeinzustand**; einzelne Wunden bleiben unabhängig (`23#103A` P6).

**Besitz:** Die Wunden gehören dem Körper des Kämpfers ([Modul 03](03-Kampfzustaende-und-Systemarchitektur.md) §5). Das Treffermodell liefert nur Eingaben.

---

## 6. Ursachen und Mechanismen

Quellenlage: `M-PD` verlangt eine ursachenabhängige Gore- und Wundlogik (Pistole ≠ Schrotflinte aus der Nähe ≠ Messer ≠ Axt ≠ Kralle). Der Entwurf des Assistenten (`23#103A` P10) schlägt Eigenschaften je Schadensmechanismus vor.

| Eigenschaft (Entwurf) | Bedeutung | Beispiele (aus den Quellen) |
|---|---|---|
| Eindringen | Tiefe und Durchdringung | Projektil, Stich |
| Energie | übertragene kinetische Energie | Schuss, Hieb, Sturz, Explosion |
| Schneiden | Durchtrennen von Gewebe | Messer, Klaue |
| Quetschen | stumpfes Trauma | Hammer, Sturz, Fahrzeug |
| Brennen | Hitze | Feuer, Verbrennung |
| Zerreißen | Gewebezerstörung | Biss, Klaue, Schrot aus der Nähe |
| Durchmesser, Fragmentierung | Wundkanal | Slug, Hohlspitz |
| Übernatürliche Wirkung | Modifikator | Heilungshemmung, Fluch |

**Nahkampf:** Messerstich (Region, Eindringtiefe, Gewebe, möglicherweise Gefäß oder Organ, Blutung), Axt (Schneiden plus Energie plus Quetschung), Hammer (wenig Schneiden, viel stumpfes Trauma, Bruchrisiko), Schlag ins Gesicht (Schmerz, Benommenheit), harter Kopftreffer (Gehirnerschütterung, Bewusstlosigkeit möglich) – alle aus `23#162A` P3–P4, vom Entwickler bestätigt (`23#163E` P3–P4).

**Elementare Traumaarten:** Verbrennung hat mehrere Schweregrade und wirkt zugleich auf Schmerz, Kleidung und gegebenenfalls Atem (`23#103A` P14, Entwurf; Entwickler `23#104E` P14: Feuer breitet sich aus, früh löschbar).

---

## 7. Körperphysik und Plausibilität

**Was beschlossen ist (`COMBAT-DEC-TK-013`):** eine gemeinsame Schicht der Sekundärphysik und Körperdynamik für Rückstoß, Laufen, Kampf, Fahrzeugbeschleunigung und Unfälle, Stürze, Tragen, Interaktion, Haar, Kleidung und Zubehör; realistische Dämpfung und Masse.

**Was nicht beschrieben ist:**

- ob Treffer Impuls auf das Ziel übertragen (`COMBAT-DEC-TK-022`);
- ob Körper bei Tod oder Bewusstlosigkeit in ein physikbasiertes Verhalten wechseln (Ragdoll, [Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md));
- wie Kollision (Körper gegen Körper) im Griffkampf gelöst wird (Paarsystem, `CQ-08`).

**Plausibilitätsregeln als Prüfziele (🧩 ABGELEITET aus `COMBAT-DEC-GP-006`, `COMBAT-DEC-FK-016`, `COMBAT-DEC-TK-013`):**

| Regel | Begründung |
|---|---|
| Energie nimmt entlang eines Weges nicht zu | Physik, keine Quelle verlangt das Gegenteil |
| Eine Waffe überträgt beim Schuss einen Rückstoßimpuls, der mit dem Geschossimpuls zusammenhängt | `M-PD`: Rückstoß physikalisch, Training verbessert Kontrolle nicht den Impuls |
| Schwerere Körper reagieren träger auf denselben Impuls | ‚realistische Dämpfung und Masse‘ (Gameplay-Systeme §9.3) |
| Ein Sturz aus größerer Höhe verletzt mehr als aus geringer | Gameplay-Systeme §2: Höhe beeinflusst das Verletzungsrisiko |
| Kleidung schützt nicht automatisch, aber sie liegt im Weg | `M-PD` ‚Ballistics‘ |

Die Regeln sind Plausibilitätsprüfungen, keine Balancewerte.

---

## 8. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Trefferzonengeometrie (`CQ-46`) | A: Ein Hit-Volumen je Region (Kapseln/Boxen am Skelett) | schnell, stabil | grob für Schichten (Rippe, Lunge) |
| | B: Mehrschichtige Trefferauflösung (Hülle plus Strukturen) | trägt den Weg durch Anatomie | Mehraufwand, Synchronisierung |
| | C: Vollständiges Körpervolumen (physikbasiert) | höchste Treue | sehr teuer; widerspricht ‚leistungsbewusst‘ (`M-PD`) |
| Morphende Körper (`CQ-50`) | A: Hit-Volumina folgen Morph-Zielen | konsistent mit kontinuierlichen Körpern | Aufwand, Prüfung |
| | B: Wenige Standardkörper für Treffer | einfach | Abweichung zwischen Darstellung und Treffer |
| Gore (`CQ-47`) | A: Ein Profil | einfach | rechtlich riskant |
| | B: Regionale und altersbezogene Profile | rechtlich flexibler | Pflegeaufwand, Tests je Profil |
| Trefferimpuls (`CQ-48`) | A: Rein animierte Reaktion | kontrollierbar | wirkt künstlich |
| | B: Physikalisch übertragener Impuls | glaubwürdig | Instabilität, Synchronisierung |
| | C: Hybrid | Balance | Komplexität |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 9. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Ballistik ([Modul 05](05-Fernkampf-und-Schusswaffen.md)) | Projektilzustand, Materialien |
| zum Nahkampf ([Modul 04](04-Nahkampf-und-Kampfstile.md)) | Schläge, Würfe, Aufgabegriffe als Eingaben |
| zu Verletzungen ([Modul 13](13-Verletzungen-Schmerz-und-Kampffolgen.md)) | Wundeinträge, Blutung, Behandlung |
| zur Animation ([Modul 22](22-Animationen-Trefferreaktionen-und-Ragdoll.md)) | Trefferreaktion, Verletzungsschichten, Ragdoll |
| zu Kreaturen ([Modul 15](15-Kreaturenkampf-und-Spezialfaehigkeiten.md)) | eigene Anatomie, Regeneration |
| zu Beweisen ([Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md)) | Einschüsse, Blutspuren, Verletzungsbefund |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Autorität über Treffer |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Treffer ohne gültige Region | Standardregion mit Protokoll, kein Absturz | 💡 |
| Zwei Treffer im selben Schritt | deterministische Reihenfolge, Summe im Körperzustand | 💡 |
| Projektil verlässt die Welt im Körper (Streaming) | Wundeintrag bleibt vollständig; Projektil-Restweg endet | 💡 |
| Kleidungsschicht fehlt (nackt, Mod) | Weg beginnt bei Haut | 💡 |
| Kreatur ohne Anatomiedatensatz | Ablehnung mit Meldung statt Standardmensch | 💡 |

---

## 10. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-45` | Datengrundlage von Materialdurchschlag, Gewebe, Knochen und Rüstung: Zahlen, Messwerte, Kalibrierung; Wundklassen der Projektile | `COMBAT-DEC-TK-017`, `-019`, `COMBAT-DEC-FK-016` | ja |
| `CQ-46` | Geometrie der Trefferzonen und Abbildung auf 25+ Regionen; Wundeintrag als gespeicherter Datensatz | `COMBAT-DEC-TK-018`, `-020` | ja |
| `CQ-47` | Gore-Schwellen, visuelle Familien, regionale Profile, Darstellung bei Minderjährigen | `COMBAT-DEC-TK-021`, `COMBAT-DEC-GP-020` | ja |
| `CQ-48` | Trefferimpuls und Körperreaktion: Zurückstoßen, Niederreißen, Technik der Sekundärphysik | `COMBAT-DEC-TK-013`, `-022` | ja |
| `CQ-49` | Verletzung von Tieren und Hunden: Anatomie, Behandlung, Tierarzt | `COMBAT-DEC-TK-023` | ja |
| `CQ-50` | Trefferzonen bei kontinuierlich morphenden Körpern (Größe, Gewicht, Muskulatur) | `COMBAT-DEC-TK-020` | ja |
