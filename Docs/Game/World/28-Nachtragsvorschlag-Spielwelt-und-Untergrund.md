# Welt 28: Nachtragsvorschlag zu ‚Spielwelt und Untergrund‘

> [!IMPORTANT]
> **Abschließend entschieden (Stand 22.10.2026).** Ursprünglich ein reiner Vorschlag zu [`Docs/Game/Spielwelt-und-Untergrund.md`](../Spielwelt-und-Untergrund.md) (damals 395 Zeilen, SHA-256 beginnt mit `7b5b66f6faa74a83`, letzter Commit `37553b6`). Übernommen sind N-01 bis N-03, N-05 bis N-11, N-12 (gekürzt), N-13 bis N-17 (13, 14 und 17 nach `PE-E1022`, 16 nach Freigabe), N-18 (nur offene Fragen), N-20, N-21, N-26, N-27 und N-28. **Abgelehnt:** N-04, N-22. **Zurückgestellt:** N-19 (spätere technische Save-System-Dokumentation). N-23 bis N-25 sind Dubletten und erledigt oder durch N-15 abgedeckt. Die Primärentscheidungen `PE-E1022` blieben unverändert. Der Vorschlag entscheidet keinen Konflikt. Details: [Abschnitt 10](#10-übernahmestatus).

Teil der Welt-Dokumentation ([Index](README.md)). Grundlage: [Register](20-Entscheidungsregister.md), [Konflikte](21-Konflikte-und-Widersprueche.md), [offene Fragen](22-Offene-Fragen-Katalog.md).

**Inhalt**

1. [Lesehilfe und Änderungsarten](#1-lesehilfe-und-änderungsarten)
2. [Bereits abgedeckt: kein Nachtrag nötig](#2-bereits-abgedeckt-kein-nachtrag-nötig)
3. [Vorschläge A: bestätigte Entscheidungen](#3-vorschläge-a-bestätigte-entscheidungen)
4. [Vorschläge B: historische Varianten](#4-vorschläge-b-historische-varianten)
5. [Vorschläge C: Folgerungen und vorläufiger Stand](#5-vorschläge-c-folgerungen-und-vorläufiger-stand)
6. [Vorschläge D: offene Fragen und Konflikte](#6-vorschläge-d-offene-fragen-und-konflikte)
7. [Bestehende Formulierungen mit Korrekturbedarf](#7-bestehende-formulierungen-mit-korrekturbedarf)
8. [Bewusst nicht vorgeschlagen](#8-bewusst-nicht-vorgeschlagen)
9. [Freigabe-Übersicht](#9-freigabe-übersicht)
10. [Übernahmestatus](#10-übernahmestatus)
11. [Entscheidungsvorlage](#11-entscheidungsvorlage)

---

## 1. Lesehilfe und Änderungsarten

Jeder Vorschlag hat eine Nummer `N-xx`, einen Zielabschnitt der bestehenden Datei (nach deren Gliederung), eine **Art**, einen **Status** und **Quellen**. Der eingerückte Text ist der wörtliche Einfügetext.

| Art | Bedeutung |
|---|---|
| **E** | Ergänzung: neuer Satz, neue Zeile oder neuer Absatz; bestehender Text bleibt |
| **K** | Korrektur: ein bestehender Satz wird ersetzt oder präzisiert |
| **R** | rein redaktionell (Legende, Verweise), ohne neue Weltaussage |

Quellenkürzel: [Modul 01 §2](01-Quellenlage-und-Methodik.md#2-zitierschlüssel). Ein Verweis wie `W-ST-034` führt ins [Register](20-Entscheidungsregister.md). Wörtliche Zitate stehen in „…“, Paraphrasen in ‚…‘.

---

## 2. Bereits abgedeckt: kein Nachtrag nötig

Der Abgleich mit der bestehenden Datei ergab für diese Punkte keine Lücke und keinen Widerspruch:

| Inhalt | Wo in der Datei | Register |
|---|---|---|
| Meer im Osten, Norden und Süden nicht vom Meer umschlossen | §5 | W-GE-016, W-GE-017 |
| Lake Alder ca. 25 km², Meridian ca. 5, Südreservoir ca. 7 | §4 | W-WA-002 |
| 110 km², 20 Bezirke, Innenstadt 20, Kern 12, 3–5 Landmarken von 700–1.000 m | §6.1 | W-ST-002 ff. |
| 25 Kartengrößen, nichts fest einkodieren, sublineares Wachstum der Stadtflächen | §3 | W-GE-015 |
| Rallyes mit 10–20 Etappen, Stadtkurse, ungesicherte Schotter-Bergstrecke | §10 | W-UR-010, W-MO-014 bis 016 (Feinheiten: N-04, N-15) |
| Wasserpark und Freizeitpark | §10 | W-UR-006 |
| Mindestens 90 % betretbare Gebäude | §12 | W-UR-001 |
| Zeitreise: jedes Jahr, ca. 1600 bis heute, versionierte Schichten | §7 | W-RE-004 |
| Untergrund 50–100 km² tentativ **(historisch nach N-02)**, Gesellschaftstabelle U24 bis U36 (Geltung offen, K-29) | §14 | W-UG-008, W-GS-001 ff. |
| Reichsgrößen Hölle/Fegefeuer/Himmel | §15 | W-RE-002 |
| Karten werden in Qualitätsstufen abgenommen, keine Karte akzeptiert | §16 | W-KA-003 |

---

## 3. Vorschläge A: bestätigte Entscheidungen

Status 📋 oder Entwickleraussage mit wörtlichem Beleg.

### N-01 · §5 „Geografie“, Abschnitt ‚Weitere Anforderungen‘ · Art E

> 📋 **Abstände zwischen Siedlungen:** Zwischen größeren Siedlungsräumen liegen überwiegend 5–10 km Landschaft („eher 5-10km“), nicht 20 km; die Welt soll nicht leer wirken, sondern abgelegen wie im echten Leben.

Status 📋. Quellen: 23#709E, 35#87E, D19-REQ; Register W-GE-023, W-ST-036. Bezug zur Datei: §5 sagt nur ‚breite Räume‘, ohne Zahl.

### N-02 · §6.2 „Weitere Städte“ · Art E

> 📋 Schon am 31.08.2026 wünschte der Entwickler eine riesige Metropole mit „2 größere städte“. Die Struktur Metropolis plus zwei Major Cities ist damit älter als der Kandidat vom Oktober.

Status 📋. Quellen: 23#707E, 23#711E; W-ST-036. Prüfung: kein Widerspruch zu §6.2 (dort drei benannte Städte Glenhaven, Ashford, Greywater); die Zuordnung der Namen bleibt ❓ (K6).

### N-03 · §7 „Zeitreise“ · Art E

> 📋 **Geografie ist die stabile Schicht:** Beim Sprung ins Jahr 1600 bleiben „nur seen berge flüsse“; die Stadt ist ‚wahrscheinlich komplett weg‘. Siedlungen und Bauten sind zeitversioniert. Handlungen in der Vergangenheit können die Gegenwart verändern: ‚theoretisch ja, aber nicht übertreiben‘.

Status 📋. Quellen: 35#41E (Fragen 125 und 128); W-RE-006, W-RE-007. Bezug: §7 erwähnt diese Antworten nicht.

### N-04 · §10 „Sport“, Punkt Motorsport · Art E (siehe auch N-15)

> 📋 Der Entwickler will mehrere Strecken, einige „durch den wald mit mehreren etappen“, und Stadtkurse mit Absperrungen; außerdem eine Schotterstrecke „wie pikespeak“ **ohne Absperrungen**, bei der zu schnelles Fahren tödlich enden kann.

Status 📋 für den Wunsch. Quellen: 35#564E, 35#568E; W-MO-014, W-MO-015. Die Ausgestaltung (Event-Aufbau, Gefahrenstufen, benannte Rallyes) bleibt 💡 (35#563A bis 35#569A).

### N-05 · §13 ‚Gesellschaft und dynamische Welt‘ · Art E

> 🧭 Als Größenordnung der NPC-Bevölkerung nennt der Entwickler „500000 npcs“ als Annahme für den Handbau-Aufwand; Simulationsebenen und Budgets sind ❓.

Status 🧭. Quellen: 35#227E, 35#278A; W-TV-010. Bezug: ersetzt keinen bestehenden Satz, präzisiert „mehrere Hunderttausend“ (Zeile 302 der Datei). Die Save-Größe ist ein Konflikt (N-19).

### N-06 · §14 „Untergrund“, Spoiler-Block · Art E

> 📋 Es gibt mehrere geheime Forschungskomplexe „von sehr klein bis riesig“; wer sie in der Story freigeschaltet hat, weiß, dass es mehrere gibt, und kann sie suchen. Die kleineren sind eher freundlich, die großen der Elite feindselig. Der Hunter-Bunker wird einzeln geplant, weil er „so super sein soll“.

Status 📋. Quellen: 48#173E, 48#175E, 48#131E; W-UG-025, W-UG-026. Bezug: ergänzt ‚weitere Anlagen sind in der Planung‘ (§14.1) und den offenen Bunker–HQ-Konflikt K10.

### N-07 · §15 „Himmel, Hölle und Fegefeuer“ · Art E

> 📋 **Persönlicher Himmel:** „in der serie hat jeder mensch seine eigenen himmel“; der Himmel soll individuell und teils subjektiv sein, ohne Kopie der Serie.
> 📋 **Hölle:** eine echte, kleinere Open-World-Ebene mit Sammelobjekten, frei erkundbar; Reisen dorthin sind zunächst storygebunden, später durch Entdeckung von Ritual und seltenen Zutaten möglich (ohne Grinding).
> 📋 **Leviathane:** „man kann sie mit gewöhnlichen waffen auch nicht töten“; sie sollen „nicht von anfang an auf der welt sind“.
> 📋 **DLC** können Ebenen erweitern („ja das ist fix“).

Status 📋 für die zitierten Entwicklersätze, 🧭 für das Umfeld aus Kurzzustimmungen. Quellen: 23#459E, 23#466E, 23#468E, 23#458A, 23#465A; W-RE-009 bis 011, W-RE-013, W-RE-015. Bezug: §15 enthält Größen und Orte, aber keinen Ablauf.

### N-08 · §14.3 „Offen“ · Art E

> ❓ Eine ‚energetische Stabilisierung‘ oder Tragstruktur der zentralen Höhle ist in den Planungsquellen **nicht belegt**; ebenso gibt es dort weder Handels- noch Luxusregeln. Sie dürfen nicht als Kanon behandelt werden.

Status ❓ (Quellenlage). Quellen: [Modul 11 §1](11-Untergrund-Architektur-und-Infrastruktur.md#1-befund-zur-quellenlage), OF-39, OF-40.

---

## 4. Vorschläge B: historische Varianten

Status 🕰️: frühere Planung, die ausdrücklich als überholt gekennzeichnet bleibt.

### N-09 · §3 Tabelle ‚Wie sich die Größenplanung entwickelt hat‘ · Art E (neue Zeilen)

| Zeitraum | Planungsstand zur Größe der Hauptwelt | Status |
|---|---|---|
| 31.08.–1.09.2026 | Die 300-km²-Karte ist zu klein, wenn die Hauptstadt 15–20 % ausmacht (Entwickler); am 01.09. werden **1.500 km²** spielbar genannt: Megacity 70 km², nächstgrößte Städte 20–25 km² | 🕰️ |
| 9.9.2026 | ca. **2.000 km²** weiches Ziel für die Hauptstory (Reiche und DLC zusätzlich); dazu ein Planungsentwurf „WORLD_2000“ mit 7 Staaten, mindestens 7 Flughäfen und 16–22 Motorsportstandorten (Entwurf des Assistenten, vom Entwickler als besser bezeichnet) | 🕰️ |
| 4.10.2026 | **~2.200 km²** mit Hüllrechteck zunächst 65 × 34 km, dann ~67 × 34 km (Entwickler: 95–97 % des Rechtecks Spielfläche); nur Südostküste; „Geografische Masterstruktur v1“ (siehe N-10) | 🕰️ / 🧭 |
| 9.10.2026 | Nach der Städtevision schlägt der Assistent 10.000–15.000 km² (Favorit 12.000) vor; der Entwickler will stattdessen viele Größen testen | 🕰️ (Vorschlag, nicht angenommen) |

Quellen: 23#697E, 23#881E, 35#83E bis 35#88A, 45#18E, 45#19A, 48#44A, 48#45E; W-GE-007, W-GE-009, W-GE-028, W-GE-035, W-GE-039, W-ST-028, W-VK-018, W-MO-011.
Einordnung: Die Zeile „9.9.“ entspricht der bestehenden Zeile „16.–27. September“ (siehe N-22 in §7).

### N-10 · §5 „Geografie“, neuer Unterabschnitt vor ‚Karten-Geheimhaltung‘ · Art E

> **Frühere Makrostruktur vom 4. Oktober 2026 (historische Variante, 🕰️/🧭).** Zu einer früheren Planungshülle (65 × 34 km) entwarf der Assistent die „Geografische Masterstruktur v1“; der Entwickler änderte nur den See auf 25 km² und schrieb „der rest klingt gut“. Sie gehört zu einer früheren Hülle und ist durch die Aufträge vom 18./19.10. nicht ersetzt, aber auch nicht bestätigt:
>
> - sieben ineinander übergehende Makroregionen (nördliches Hochgebirge, Alder-Becken, westliche Schlucht, zentrales Meridian-Becken, Südwest-Weite, Südliche Wildnis, östliche Tiefebene mit Greywater-Küste); das Meridian-Wassersystem als Gestaltungsgrundlage; Meer nur im Südosten (🧭, W-GE-036)
> - Nordgebirge aus drei verbundenen Massiven, nur ~8–12 % hochalpin, höchste Gipfel ~4.400–4.900 m (nur wenige), ein Ausläufer 6–10 km nach Süden; Hauptstraße plausibel bis ~2.500–3.200 m (🧭, W-NA-012)
> - Westen als Fluss-Schlucht statt Gebirgswand; Südwesten offen (ohne Wüste); Süd-Zentrum große Waldwildnis (🧭, W-NA-013)
> - Koordinaten dieses Entwurfs (Raum SW = (0,0), NE = (65,34)) sind 💡 und dürfen weder auf WORLD_700 noch auf den kommenden Kandidaten übertragen werden (W-GE-037)

Status 🧭 (Zustimmung ohne Einzelzahlen). Quellen: 45#7A, 45#8E, 45#13A, 45#3A; W-GE-036, W-GE-037, W-NA-012, W-NA-013. Bezug: die Datei kennt diese Stufe nicht; ihre Aussagen (Gebirge im Norden, Meer im Osten) bleiben vereinbar.

### N-11 · §6.1 „Metropolis“, Liste „Ältere Planung“ · Art E

> - 🕰️ 04.10.2026 (Hülle 65 × 34): Metropolis 20–25 km², Downtown 4–5, Major City A/B je 8–10, Coastal Town 1,5–2 km² als ‚festgelegte‘ Zielwerte; Zustimmung des Entwicklers „ok passt ja“ und „ja mache das“ (W-ST-034).
> - 🕰️ 01.09.2026 (1.500-km²-Linie): Downtown/Kern 7–8 km², Hochhäuser 350–500+ m, 70 km² City Proper, dazu ein Metropolitan Belt außerhalb mit einer Metropolregion „deutlich über 100 km²“ (W-ST-037).
> - 🕰️ 09.10.2026: Nach der Städtevision (New York, Chicago, San Francisco, Boston) schlug der Assistent 150–300+ km² für Metropolis vor; der Entwickler stellte klar, die Städte sollten sich nur so anfühlen (W-ST-038).

Status 🕰️. Quellen: 45#3A, 45#4E, 45#6E, 23#908A, 23#910A, 23#911E, 48#39E, 48#42A, 45#61E. Bezug: ergänzt die bestehende Liste; ersetzt nichts.

### N-12 · §11.1 ‚Straße, Luft und Wasser‘, Zeile zu Flughäfen · Art K

Bestehend: ‚… ihre Zahl und Lage sind ❓ offen.‘ Vorschlag:

> 🧭 Ein internationaler Flughafen außerhalb des Stadtkerns und regionale Flughäfen sind vorgesehen. Die Zahl ist ⚠️ widersprüchlich: WORLD_700 hat 3; die Planung vom 09.09. nannte mindestens 7 (einen pro Staat, 🕰️); die Aufträge vom Oktober verankern 3 Flughäfen (🕰️ Auftragstext). Eine Entscheidung für den 2.200-km²-Kandidaten liegt nicht vor.

Status ⚠️/🕰️. Quellen: 35#93E, G7-GPKG, 30#265E; W-VK-018, K8. Freigabe nötig, weil ein bestehender Satz geändert wird.

---

## 5. Vorschläge C: Folgerungen und vorläufiger Stand

### N-13 · §4 Tabelle, Zeile Metropolis, und neuer Absatz · Art E · **übernommen nach `PE-E1022`**

> 📋 **Abgrenzung der 110 km² (bestätigt am 22.10.2026):** Metropolis umfasst 110 km² zusammenhängendes Stadtgebiet einschließlich seiner Vororte, also den urbanen Kern, weitere Stadtviertel und zugehörige Vorortbereiche. Die Fläche bezeichnet nicht ausschließlich die dicht bebaute Innenstadt und nicht die weiter gefasste Metropolregion mit eigenständigen umliegenden Siedlungen. Sie ist Teil der 2.200 km² Festland. Konkrete Stadtteilflächen oder Stadtgrenzen sind nicht festgelegt.

Status 📋 (vorher ❓ mit Folgerung 🧭). Quellen: `PE-E1022` (neue Primärentscheidung, nicht im Master); Herleitung 45#284A, D18-MAP, D19-REQ, 23#910A, 23#911E; W-ST-003, W-ST-037, K1 (geklärt), OF-11 (geklärt). Die ältere Aussage ‚80 km² ist schon krass‘ (35#876E) ist nach dieser Festlegung überholt, soweit sie ihr widerspricht (🕰️). Der Vorschlag erfindet keine Stadtteilflächen.

### N-14 · §5, Abschnitt ‚Karten-Geheimhaltung‘ · Art E · **übernommen nach `PE-E1022`**

> Lage des Hunter-HQ (Dokumentation 📋 freigegeben am 22.10.2026): abseits dichter Siedlung, nicht in oder neben einer Stadt; nach dem Entwurf vom 04.10. südlich bis südwestlich von Metropolis am Übergang zur großen Südlichen Wildnis, etwa 10–15 Straßenkilometer vom Zentrum (🧭). Exakte Position im 2.200-km²-Kandidaten ❓; keine festgelegten Koordinaten. Die Freigabe betrifft nur die geografische Dokumentation, nicht geheime Eingänge, Story-Enthüllungen oder interne Einrichtungen.

Status 📋 (Dokumentation) / 🧭 (Lage) / ❓ (Position). Quellen: `PE-E1022`; 45#7A, 45#13A, 45#279E; W-KA-017, W-ST-035, W-KA-008, K10 (Bunker-Verhältnis weiter ❓), OF-69. Die Spielerkarte beschriftet das HQ weiterhin nicht auffällig.

### N-15 · §10 „Sport“, Punkt Motorsport, Rest · Art K

Bestehend: ‚… festgelegt (🧭, Details ❓).‘ Vorschlag, den Klammerzusatz zu ersetzen:

> (📋 Grundprinzip laut Entwickler; 💡 Ausgestaltung, Namen und Zahlen; Zahl der Standorte ⚠️, K8)

Status 📋/💡. Quellen: 35#562E bis 35#568E, 35#563A bis 35#569A. Begründung: „festgelegt“ mit 🧭 widerspricht sich in der bestehenden Zeile; die Rallye-Namen des Assistenten sind Platzhalter (W-MO-016).

### N-16 · §4 Satz ‚frühere Aussage‘ · Art K · **übernommen (Freigabe vom 22.10.2026, nach `PE-E1022`)**

Ersetzt: ‚die frühere Aussage ‚Metropolis 20–25 km²‘ war ein offenes Testszenario und wurde für diesen Kandidaten durch 110 km² ersetzt‘. Eingefügt:

> Für Metropolis gelten verbindlich 110 km² zusammenhängendes Stadtgebiet einschließlich Vororten (📋). Die früheren 20–25 km² (Zielwert des Assistenten vom 4. Oktober 2026, damals für eine Hülle von 65 × 34 km) sind ein historischer Planungsstand (🕰️), kein gleichberechtigtes aktuelles Testszenario; sie bleiben in der Chronologie erhalten.

Status 🕰️ → 📋. Quellen: `PE-E1022`; 45#3A, 45#6E, D18-MAP; W-ST-003, W-ST-010, W-ST-034, K1. Zusätzlich vermerkt: die Zeile 13.–15. Oktober (Stadtflächen-Hypothesen) und der Chronologiepunkt vom 4. Oktober in §6.1 tragen den Hinweis ‚durch die 110 km² überholt‘. Alle Quellenverweise und Datumszeilen der Chronologie blieben erhalten.

---

## 6. Vorschläge D: offene Fragen und Konflikte

### N-17 · §15, Tabelle (Zeile Hölle) und Aufzählung ‚Bewegung und Persistenz‘, letzter Punkt · Art K · **übernommen nach `PE-E1022`**

Ersetzt die Aussagen ‚mögliche Zeitdehnung (Verhältnis ❓)‘ und ‚Zeitverhältnis der Hölle: von einer bekannten Idee inspiriert … ❓ offen‘:

> 📋 **Zeitverhältnis der Hölle (bestätigt am 22.10.2026):** 40 Jahre in der Hölle entsprechen ungefähr vier Monaten auf der Erde; die Zeit vergeht in der Hölle wesentlich schneller. Der Projektinhaber hat die Zeitrichtung ausdrücklich geklärt. Offen bleiben Synchronisierung, Zeitmessung, Übergänge zwischen Reichen, Online-Behandlung sowie das Zeitverhältnis von Fegefeuer und Himmel.

Status 📋 (vorher 🧭). Quellen: `PE-E1022`; 35#41E, 23#458A, 23#459E; W-RE-005, W-RE-011, OF-66. Die Master-Dokumente M-CA0906 und M-WB89 führen das Verhältnis noch als offen; die Primärentscheidung hat Vorrang. Es werden keine zusätzlichen Zeitreise-Regeln festgelegt.

### N-18 · §15, Zeile „❓ Offen: …“ am Ende · Art K

Bestehend: ‚… Tod und Jenseits-Ablauf.‘ Vorschlag:

> 🧭 **Jenseitsablauf:** Eine Seele löst sich vom Körper; Ziel sind Himmel, Hölle oder (Ausnahme) ein Geist. Das Ziel hängt von Leben und Handlungen ab, Sonderregeln (Vertrag, Fluch, Eingriff) können es überschreiben; kein sichtbarer Karma-Wert. Für NPCs und kanonische Story-Tode gilt das System; normale Gameplay-Tode des Hauptcharakters sind nicht kanonisch.
> ❓ Offen bleibt der **Tod in Hölle, Himmel und Fegefeuer** (Entwickler: „vielleicht aber noch anders“), Zeitverhältnisse von Fegefeuer und Himmel, Biologie und Auftreten der Leviathane.

Status 🧭/❓. Quellen: 23#458A, 23#459E, 23#460A, 23#467A; W-RE-008, W-RE-012, W-RE-014, W-RE-015; OF-65 bis OF-67.

### N-19 · §13 und §16, Hinweis zu Saves · Art E

> ⚠️ **Save-Größenziel:** Der Master nennt langfristig 20–50 MB; ein Assistentenvorschlag vom 11.09. setzte bei ~500.000 NPC-Identitäten 50–150 MB (Extremfall 150–300 MB). Der Entwickler wollte „eher kleiner“. Nicht entschieden (K13).

Status ⚠️. Quellen: 35#279E, 35#280A, M-PD; W-TV-011, K13.

### N-20 · §4 und §5, Gipfelhöhe · Art E

> ⚠️ **Gipfelhöhe:** 4.000–5.000 m (31.08., 01.10.), 2.000–2.500 m befahrbar (31.08.), 2.500–3.000 m (Prototypauftrag), 4.400–4.900 m nur wenige Gipfel (04.10., 🧭); Prototypberichte nennen 4.600 m. Nicht entschieden (K4).

Status ⚠️. Quellen: 23#803E, 35#935E, 23#820E, 30#232E, 45#7A, 54#7A; K4, W-NA-012.

### N-21 · §17 „Offene Punkte“ · Art E

Anhängen:

> Weiter offen: Gipfelhöhe (K4); Anzahl der Flughäfen und Motorsportstandorte (K8); Zeitverhältnis Fegefeuer und Himmel sowie die Technik der Höllenzeit; exakte Position des Hunter-HQ (OF-69); Tod in den anderen Ebenen; Entscheidungen ab dem 20.10.2026, die im Master nicht enthalten sind (K12).

Status ❓. Quellen: [Modul 21](21-Konflikte-und-Widersprueche.md), [Modul 22](22-Offene-Fragen-Katalog.md).

---

## 7. Bestehende Formulierungen mit Korrekturbedarf

| Nr. | Ort in der Datei | Befund | Vorschlag |
|---|---|---|---|
| N-22 | §3 Tabelle, Zeile ‚16.–27. September 2026 · etwa 2.000 km²‘ | Die Aussage ‚2.000 km² weiches Ziel‘ stammt im Chat vom **09.09.** (35#83E bis 35#88A); ob die Datei einen anderen Master-Beleg für 16.–27.09. nutzt, ist hier nicht prüfbar | Datum zu ‚ab 9.9. bis 27.9.‘ präzisieren, nur nach Prüfung der Quelle der Datei (Freigabe) **abgelehnt (22.10.)** |
| N-23 | §4 letzter Satz | ‚offenes Testszenario‘ unterschlug die Zustimmung vom 04.10. | **erledigt durch N-16** (übernommen) |
| N-24 | §10 Motorsport | ‚festgelegt (🧭 …)‘ war widersprüchlich | **abgedeckt durch N-15** (übernommen) |
| N-25 | §15 Tabelle/Aufzählung | ‚Verhältnis ❓‘ stand im Gegensatz zur Chat-Lage | **erledigt durch N-17** (`PE-E1022`) |
| N-26 | §1 Legende | enthält weder ⚠️ (widersprüchlich) noch 💡 (Vorschlag); die Nachträge benutzen beide | zwei Zeilen ergänzen (Art R): ‚⚠️ Widersprüchlich: Quellen widersprechen sich, nicht aufgelöst‘ und ‚💡 Vorschlag: Idee des Assistenten ohne Entscheidung‘ |
| N-27 | §2 Tabelle, Zeile WORLD_700 | nennt nur ca. 690,9 km² Spielfläche | Art E: ‚Hüllfläche ca. 733,8 km² (7 Staaten A–G), gemessen am GeoPackage; Spielfläche 690,9 km²; Hüllfläche und Spielfläche nicht verwechseln‘ (📋, G7-GPKG, [Modul 24](24-WORLD700-GIS-Referenz.md)) |
| N-28 | §4 Einleitung | Stand wird nicht begrenzt | Art E: ‚Der Master enthält keine Entscheidung ab 20.10.2026 (K12); der hier beschriebene Stand gilt bis 19.10.‘ (📋 Befund, M-MS1013) |

Nicht-Widerspruch: Die bestehende Aussage „Meer im Osten“ (§5) passt zu 04.10. (Südosten) und 18./19.10. (Osten); K3 gilt als gelöst durch Datum.

---

## 8. Bewusst nicht vorgeschlagen

| Inhalt | Grund |
|---|---|
| W-GE-034 (Strafsystem, Weltcharakter ‚logisch statt emotional‘) | Tonalität, gehört in Spielvision beziehungsweise Gameplay-Dokumente; du solltest den Zielort wählen |
| W-UR-019 (Darstellung religiöser Bauten, Abweichung vom Assistentenvorschlag) | sensible Weltfiktion; erst nach deiner Entscheidung (OF-68) |
| W-ST-031, W-ST-032, W-WI-007 (Stripclubs, Tattoo-Studios, Wetten/Adult-Branche) | gehören in [Adult-Material](../Adult/14-Entscheidungsregister.md) |
| W-GE-037 Koordinaten im Wortlaut | zu einer früheren Hülle; nur als 💡 erwähnt (N-10) |
| W-TV-010 Simulationsstufen Tier 0–3 | nur Assistentenvorschlag mit Sammelzustimmung („12-35 ja“) |
| W-KA-016 Prototypwerte (Chat 54) | berichtet, nicht überprüft; Mögliche Erwähnung nur in §16 (Art E, optional) |

---

## 9. Freigabe-Übersicht

| Gruppe | Vorschläge | Warum Freigabe |
|---|---|---|
| **1. Rein redaktionell (Art R)** | N-26 | ändert keine Weltaussage; trotzdem Eingriff in eine bestehende Datei |
| **2. Ergänzungen aus bestätigten Quellen (Art E, 📋)** | N-01 bis N-03, N-06 bis N-08, N-27 | neue Aussagen im Überblick; Quellen belegt, aber du bestätigst Aufnahme und Platz (N-06 liegt im Spoiler-Block) |
| **3. Historische Varianten (Art E, 🕰️)** | N-09 bis N-11 | neue Chronik-Zeilen; N-09 enthält den Planungsentwurf vom 09.09., der nur im Chat steht |
| **4. Folgerungen und vorläufiger Stand (🧭/❓)** | N-05, N-10, N-28 (N-13 und N-14 durch `PE-E1022` erledigt und übernommen) | Folgerungen werden sichtbar |
| **5. Änderungen an bestehenden Sätzen (Art K)** | N-12, N-15, N-18, N-22 (N-16 freigegeben und N-17 durch `PE-E1022` übernommen) | ersetzen oder präzisieren Text; **N-22 erfordert Prüfung der Quelle der Datei** |
| **6. Konflikt-/Offen-Hinweise (⚠️/❓)** | N-19 bis N-21 | machen Konflikte K1, K4, K8, K12, K13 im Überblick sichtbar, ohne sie zu lösen |

Reihenfolge-Empfehlung für die Freigabe: erst Gruppe 6 und 2, danach 3 und 4; Gruppe 5 einzeln, vor allem N-22. Alle Vorschläge sind unabhängig voneinander einfügbar.

**Hinweis zur Umsetzung:** Wird einer der Vorschläge angenommen, ändert sich die Zeilenzahl der Datei; der Vorschlag nennt deshalb Abschnitte, keine Zeilennummern. Nach der Einfügung sind [Entscheidungsstatus](../Entscheidungsstatus.md) und [Modul 23 §9](23-Quellenabdeckung-und-Auditbericht.md#9-ehrliche-grenze-der-quellen) anzupassen.

---

## 10. Übernahmestatus

Stand nach der Teilfreigabe. Die Datei `Spielwelt-und-Untergrund.md` hat danach 32 Zeilen mehr (nur Einfügungen, keine gelöschte oder geänderte bestehende Zeile).

| Vorschlag | Status | Ort in der Übersichtsdatei | Abweichung vom Vorschlagstext |
|---|---|---|---|
| N-01 | **übernommen** | §5, Liste ‚Weitere Anforderungen‘ | keine inhaltliche |
| N-02 | **übernommen** | §6.2, Liste | Satz ‚Prüfung: kein Widerspruch‘ entfällt; Zusatz: Namenszuordnung ❓ |
| N-03 | **übernommen** | §7, vor dem Implementierungs-Hinweis | keine inhaltliche |
| N-04 | **nicht übernommen** | – | Der Inhalt steht bereits in §10 (Rallyes, Stadtkurse, ungesicherte Bergstrecke); neu wäre nur die Statusänderung 🧭 → 📋, und die gehört zu N-15 (Motorsport, nicht freigegeben) |
| N-05 | **übernommen** | §13, nach der NPC-Größe | Verweis auf K13 entfällt (N-19 offen) |
| N-06 | **übernommen** | §14, ausschließlich im Spoiler-Block | keine inhaltliche |
| N-07 | **übernommen** | §15, vor der Zeile „Offen“ | Absatz mit Zwischenüberschrift; Zeitverhältnis und Jenseitsablauf nicht berührt |
| N-08 | **übernommen** | §14.3, Absatz nach der Offen-Liste | Anführungszeichen als ‚Paraphrase‘; Link zu World/11 |
| N-09 | **übernommen** (🕰️, nicht als Weltgeografie) | §3, Tabelle (vier neue Zeilen) | Zeile 9.9. um den Hinweis ergänzt, dass die Anzahlen widersprüchlich und nicht aufgelöst sind und die 2.000 km² der Zeile 16.–27.9. entsprechen (Datierung nicht abgeglichen, N-22 offen); Symbol ⚠️ durch Text ersetzt |
| N-10 | **übernommen** (🕰️/🧭, nicht als Weltgeografie) | §5, vor ‚Karten-Geheimhaltung‘ | 💡 durch ‚🧭 (nur Vorschlag)‘ ersetzt; ausdrücklicher Satz, dass die Gipfelhöhe nicht entschieden ist |
| N-11 | **übernommen** (🕰️) | §6.1, Liste „Ältere Planung“ | Schlusszeile ‚ändern die Arbeitsgröße nicht‘ ergänzt; Metropolis-Aussagen der Datei unverändert |
| N-13 | **übernommen nach `PE-E1022`** (neu gefasst) | §4, Tabellenzeile und Absatz ‚Abgrenzung der 110 km²‘; §6.1, Flächenzeile | Text als bestätigte Primärentscheidung statt Folgerung; K1 und OF-11 geklärt |
| N-14 | **übernommen nach `PE-E1022`** (neu gefasst) | §5, Absatz „Lage des Hunter-HQ“ nach ‚Karten-Geheimhaltung‘ | Dokumentation freigegeben; Lage 🧭, Position ❓; Satz zur Spielerkarte ergänzt |
| N-17 | **übernommen nach `PE-E1022`** (neu gefasst) | §15, Tabellenzeile Hölle und Aufzählung ‚Bewegung und Persistenz‘ | Status 📋 statt 🧭; technische Einzelheiten ausdrücklich offen |
| N-16 | **übernommen** (Freigabe vom 22.10.2026) | §4, Absatz „Wichtig“; §3, Zeile 13.–15. Oktober; §6.1, Punkt 4. Oktober | 20–25 km² als historischer Planungsstand; kein gleichberechtigtes Testszenario; Chronologie und Quellenverweise erhalten |
| N-12 | **übernommen (gekürzt, Freigabe 22.10.)** | §11.1, Flughafen-Zeile | nur Dokumentation ‚Anzahl widersprüchlich (⚠️ K8), siehe §3 Zeile 9. September‘; Flughafenzahlen bleiben ❓ offen, keine neuen Zahlen |
| N-15 | **übernommen (Freigabe 22.10.)** | §10, Motorsport-Klammer | 📋 Grundprinzip, 💡 Ausgestaltung, Zahl der Standorte ⚠️/❓; keine zusätzlichen Strecken oder Standorte |
| N-18 | **übernommen, nur offene Fragen (Freigabe 22.10.)** | §15, Absatz nach der Offen-Zeile | kein 🧭-Jenseitsablauf; Zusatz ‚Ein Jenseitsablauf ist damit nicht festgelegt‘ |
| N-20 | **übernommen (Freigabe 22.10.)** | §5, Liste ‚Lage und Relief‘ | ⚠️ Konflikt dokumentiert, keine Höhe festgelegt |
| N-21 | **übernommen (Freigabe 22.10.)** | §17, Schlusszeile | ohne Save-Größenziel, weil N-19 zurückgestellt ist |
| N-26 | **übernommen (Freigabe 22.10.)** | §1, Legende | Zeilen ⚠️ und 💡 |
| N-27 | **übernommen (Freigabe 22.10.)** | §2, Zeile WORLD_700 | Hüllfläche 733,8 km² als ‚historische Referenz‘, nicht Kandidat |
| N-28 | **übernommen (Freigabe 22.10.)** | §4, Absatz ‚Quellenlücke‘ | mit Ausnahme der Primärentscheidungen vom 22.10. |
| N-04 | **abgelehnt** (22.10.) | – | Dublette zu §10; Statusfrage durch N-15 erledigt |
| N-19 | **zurückgestellt** (22.10.) | – | für die spätere technische Save-System-Dokumentation; K13 bleibt offen |
| N-22 | **abgelehnt** (22.10.) | – | bestehende Zeile ist im Master ab 16.09. belegt |
| N-23, N-24, N-25 | **erledigt/abgedeckt** | – | N-23 durch N-16, N-24 durch N-15, N-25 durch N-17 |

**Zusätzliche redaktionelle Einfügung (nicht Teil der 28 Vorschläge):** ein Absatz in §1 erklärt das Format der Quellenverweise (`35#41E`) und verweist auf den Zitierschlüssel und das Register. Er ist nötig, damit die übernommenen Nachträge nachvollziehbar sind; die Legende selbst (N-26) blieb unverändert, die übernommenen Texte verwenden deshalb nur die vorhandenen Zeichen.

**Weitere Änderungen der Übersichtsdatei durch `PE-E1022`** (nicht aus den 28 Vorschlägen): Satz zur Quellenkennung `PE-E1022` in §1; im Anschluss an die Metropolis-Chronik der Satz, das Regionsmodell sei nicht die Bedeutung der 110 km²; Klarstellung in der Karten-Geheimhaltung (gilt für Spielerkarten); Anpassung der Zeile ‚Regeln zur Zeit‘ in der Offen-Liste von §15.

**Frühere Spannung aufgelöst:** Die Aussage in §4, ‚Metropolis 20–25 km²‘ sei ein offenes Testszenario gewesen, stand neben den Zielwerten vom 04.10. Sie wurde mit N-16 durch die Darstellung als historischer Planungsstand ersetzt; die 110 km² gelten verbindlich als Stadtgebiet einschließlich Vororten.

---

## 11. Entscheidungsvorlage

**Entschieden am 22.10.2026:** angenommen N-12 (gekürzt), N-15, N-18 (nur offene Fragen), N-20, N-21, N-26, N-27, N-28; abgelehnt N-04, N-22; zurückgestellt N-19. Die Vorlage unten bleibt als Entscheidungsgrundlage erhalten. Sie nennt bei N-18 und N-21 mehr, als umgesetzt wurde (umgesetzt: nur der ❓-Teil beziehungsweise die Liste ohne Save-Größenziel).

Für die noch offenen Vorschläge. Es wurde nichts davon in die Übersichtsdatei übernommen. „Kanon“ heißt: die Änderung verschiebt den Status einer Weltaussage oder fügt eine neue hinzu; „redaktionell“: Formulierung, Verweis oder Legende ohne neue Weltaussage. Zeilen der Übersichtsdatei werden nach Abschnitt benannt.

**N-04 · §10 Motorsport (Art E)**
1. Inhalt: Entwicklerwunsch zu mehreren Strecken, Wald-Etappen, Stadtkursen mit Absperrungen und Schotterstrecke ‚wie pikespeak‘ ohne Absperrungen als 📋 (35#564E, 35#568E).
2. Betroffen: bestehender Satz in §10, der denselben Inhalt mit 🧭 führt.
3. Art: Kanon (nur Statusanhebung), Inhalt sonst doppelt.
4. Empfehlung: **ablehnen**, weil schon vorhanden; die Statusfrage entscheidet N-15.

**N-12 · §11.1 Flughäfen (Art K)**
1. Inhalt: Der Satz ‚Zahl und Lage ❓ offen‘ erhält die Zählungen: WORLD_700 3, Planung 09.09. mindestens 7 (🕰️), Oktober-Aufträge 3 (🕰️), mit dem Hinweis ⚠️ (K8).
2. Betroffen: ‚ihre Zahl und Lage sind ❓ offen‘.
3. Art: redaktionelle Dokumentation eines Konflikts; **beeinflusst den Kanon nicht**, solange die Zahl ❓ bleibt. Die Zeile 9. September in §3 nennt die Zahlen bereits.
4. Empfehlung: **gekürzt annehmen**: nur ein Verweis ‚Anzahl widersprüchlich, siehe §3 Zeile 9. September (K8)‘, ohne neue Zahlen. Vermeidet Doppelung und lässt die Zahl offen.

**N-15 · §10 Motorsport, Klammer (Art K)**
1. Inhalt: Statt ‚festgelegt (🧭, Details ❓)‘: ‚(📋 Grundprinzip laut Entwickler; 💡 Ausgestaltung, Namen und Zahlen; Zahl der Standorte ⚠️, K8)‘.
2. Betroffen: die Statusklammer der Zeile zu Rallyes, Stadtkursen und Schotter-Bergstrecke. „festgelegt“ zusammen mit 🧭 widerspricht sich.
3. Art: **Kanon** (Statusanhebung des Grundprinzips; die Standortzahl bleibt offen).
4. Empfehlung: **annehmen**: die Entwicklersätze (35#562E, 35#566E, 35#568E) belegen das Prinzip eindeutig; nur ‚⚠️‘ durch ‚❓‘ ersetzen, falls N-26 nicht angenommen wird.

**N-18 · §15 Offen-Liste, ‚Tod und Jenseits-Ablauf‘ (Art K)**
1. Inhalt: 🧭 Ablauf (Seele löst sich, Ziel Himmel/Hölle/Geist; Ziel nach Leben und Handlungen, Sonderregeln für Verträge/Flüche; kein Karma-Wert; NPC- und Story-Tode kanonisch, normale Gameplay-Tode nicht) und ❓ Tod in den anderen Ebenen, Zeitverhältnis von Fegefeuer und Himmel, Leviathan-Biologie.
2. Betroffen: ‚❓ … Tod und Jenseits-Ablauf‘ am Ende von §15.
3. Art: **Kanon** (neue 🧭-Regeln); stützt sich auf Assistentenentwurf mit punktweisen Kurzzustimmungen (23#458A, 23#459E).
4. Empfehlung: **teilweise annehmen**: den ❓-Teil (kein Kanon) ja; den 🧭-Ablauf erst, wenn du ihn ausdrücklich bestätigst, weil er nur aus Kurzantworten stammt.

**N-19 · §13/§16 Save-Größenziel (Art E)**
1. Inhalt: ⚠️ Master 20–50 MB gegenüber Assistentenvorschlag 50–150 MB (Extremfall 150–300 MB) bei ~500.000 NPC-Identitäten; Entwickler ‚eher kleiner‘ (K13).
2. Betroffen: keine bestehende Aussage der Übersichtsdatei; §13 nennt nur ‚mehrere Hunderttausend NPCs‘.
3. Art: Konflikthinweis; **kein Kanon**, aber der Assistentenwert würde sichtbar.
4. Empfehlung: **zurückstellen** (Technikthema, gehört eher in ein Technikdokument als in die Weltübersicht); falls doch, nur die Master-Zahl nennen und K13 verlinken.

**N-20 · §4/§5 Gipfelhöhe (Art E)**
1. Inhalt: ⚠️ Werte 4.000–5.000 m, 2.000–2.500 m befahrbar, 2.500–3.000 m (Auftrag), 4.400–4.900 m (04.10., 🧭); Prototypberichte 4.600 m (K4).
2. Betroffen: keine bestehende Aussage; ergänzt den bereits übernommenen Satz zu N-10 ‚Gipfelhöhe nicht entschieden‘.
3. Art: Konflikthinweis, **kein Kanon**.
4. Empfehlung: **annehmen** (nach N-26 oder mit ❓ statt ⚠️), weil die Übersicht sonst nur einen Wert nennt und der Widerspruch unsichtbar bliebe.

**N-21 · §17 Offene Punkte (Art E)**
1. Inhalt: Zusatz: Gipfelhöhe (K4); Anzahl der Flughäfen und Motorsportstandorte (K8); Save-Größenziel (K13); Zeitverhältnis von Fegefeuer und Himmel sowie Technik der Höllenzeit; exakte HQ-Position (OF-69); Tod in den anderen Ebenen; Entscheidungen ab dem 20.10. (K12).
2. Betroffen: Liste ‚Offene Punkte‘ am Dateiende.
3. Art: **redaktionell**.
4. Empfehlung: **annehmen**, aber erst nach den Entscheidungen zu N-12 bis N-20, damit die Liste nur Gebliebenes nennt.

**N-22 · §3 Zeile ‚16.–27. September 2026‘ (Art K)**
1. Inhalt: Datum auf ‚ab 9.9. bis 27.9.‘ ändern.
2. Betroffen: die Datierung des weichen 2.000-km²-Ziels.
3. Art: Kanon-nah (Chronologie).
4. Empfehlung: **ablehnen**. Prüfung im Master: Die Status-Dokumente des Masters führen ‚~2,000 km² soft target‘ ab 16.09.; die bestehende Zeile ist damit belegt. Der Chat-Beginn am 09.09. steht bereits in der übernommenen Zeile 9. September. Keine Korrektur nötig.

**N-23 bis N-25** sind Dubletten: N-23 durch N-16, N-25 durch N-17 erledigt; N-24 entscheidet sich mit N-15.

**N-26 · §1 Legende (Art R)**
1. Inhalt: zwei Zeilen: ⚠️ ‚Widersprüchlich: Quellen widersprechen sich, nicht aufgelöst‘ und 💡 ‚Vorschlag: Idee des Assistenten ohne Entscheidung‘.
2. Betroffen: die Statuslegende (bisher ✅ 📋 🧭 🔭 ❓ 🕰️).
3. Art: **redaktionell**.
4. Empfehlung: **annehmen**; Voraussetzung, wenn N-12, N-15, N-19 oder N-20 mit ⚠️ übernommen werden sollen.

**N-27 · §2 Tabelle, Zeile WORLD_700 (Art E)**
1. Inhalt: Hüllfläche ca. 733,8 km² (7 Staaten A–G) am GeoPackage gemessen, Spielfläche 690,9 km²; Hüllfläche und Spielfläche nicht verwechseln.
2. Betroffen: ‚ungefähr 690,9 km² Spielfläche laut Planungsstand vom 4. September 2026‘ (bleibt stehen).
3. Art: **redaktionell/Messwert** (G7-GPKG, Modul 24); beeinflusst den Kanon nicht, bestätigt nur den gesperrten Referenzstand.
4. Empfehlung: **annehmen**; verhindert die Verwechslung von Hülle und Spielfläche.

**N-28 · §4 Einleitung (Art E)**
1. Inhalt: Hinweis, dass der Master keine Entscheidung ab 20.10.2026 enthält (K12), mit Ausnahme der drei Primärentscheidungen vom 22.10. (`PE-E1022`).
2. Betroffen: keine bestehende Aussage; §5 nennt ‚Stand 18./19. Oktober 2026‘.
3. Art: **redaktionell** (Quellenlage).
4. Empfehlung: **annehmen**; die Übersicht erwähnt die Primärentscheidungen bereits, der Hinweis macht die Grenze des Stands sichtbar.

**Zusammenfassung der Empfehlungen:** annehmen: N-15, N-20, N-21 (zuletzt), N-26, N-27, N-28; gekürzt annehmen: N-12; teilweise annehmen: N-18; zurückstellen: N-19; ablehnen: N-04, N-22. Reihenfolge: N-26, dann N-15, N-12, N-20, N-27, N-28, zuletzt N-21.
