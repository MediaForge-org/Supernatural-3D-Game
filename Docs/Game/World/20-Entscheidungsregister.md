# Welt 20: Entscheidungsregister

> [!IMPORTANT]
> **Nichts davon ist implementiert, und keine Karte ist freigegeben.** Dieses Register hält fest, **was wo mit welchem Status in den Planungsquellen steht**. Es erfindet keine Entscheidung. Statuszeichen: [Legende](README.md#statuslegende). Quellenkürzel: [Modul 01](01-Quellenlage-und-Methodik.md#2-zitierschlüssel).

Teil der Welt-Dokumentation ([Index](README.md)). Konflikte stehen in [Modul 21](21-Konflikte-und-Widersprueche.md), offene Fragen in [Modul 22](22-Offene-Fragen-Katalog.md). Die ausführliche Größenchronik steht in [Modul 03](03-Massstab-Flaechen-Proportionen.md).

**Inhalt**

1. [Lesehilfe](#1-lesehilfe)
2. [Weltgröße und Grundstruktur (GE)](#2-weltgröße-und-grundstruktur-ge)
3. [Karte und Abnahme (KA)](#3-karte-und-abnahme-ka)
4. [Städte und Siedlungen (ST)](#4-städte-und-siedlungen-st)
5. [Urbane Gestaltung, Gebäude, Orte (UR)](#5-urbane-gestaltung-gebäude-orte-ur)
6. [Natur, Berge, Wald (NA)](#6-natur-berge-wald-na)
7. [Gewässer (WA)](#7-gewässer-wa)
8. [Straßen, Schiene, Verkehrsinfrastruktur (VK)](#8-straßen-schiene-verkehrsinfrastruktur-vk)
9. [Untergrund: Lore und Raum (UG)](#9-untergrund-lore-und-raum-ug)
10. [Untergrund: Gesellschaft, Technik, KI (GS)](#10-untergrund-gesellschaft-technik-ki-gs)
11. [Wirtschaft an der Oberfläche (WI)](#11-wirtschaft-an-der-oberfläche-wi)
12. [Fahrzeuge (FZ)](#12-fahrzeuge-fz)
13. [Fahrphysik und Steuerung (FP)](#13-fahrphysik-und-steuerung-fp)
14. [Motorsport (MO)](#14-motorsport-mo)
15. [NPC-Verkehr und Betriebssimulation (TV)](#15-npc-verkehr-und-betriebssimulation-tv)
16. [Technik und Weltgenerierung (TE)](#16-technik-und-weltgenerierung-te)
17. [Reiche, Zeit, Aktivitäten (RE, AK)](#17-reiche-zeit-aktivitäten-re-ak)
18. [Kennzahlen des Registers](#18-kennzahlen-des-registers)

---

## 1. Lesehilfe

- **Aussage:** knappe, sinngemäße Wiedergabe (‚…‘ = Paraphrase). Wörtliche Zitate in „…“ sind maschinell gegen die Quelle geprüft ([Modul 23](23-Quellenabdeckung-und-Auditbericht.md)).
- **Quelle:** Kürzel aus Modul 01. `23#881E` heißt: Chat 23, Nachricht 881, Rolle Entwickler (`E`); `A` = Assistent. Eine Entwicklerrolle im Transkript kann **eingefügte Fremdtexte** enthalten (zum Beispiel Statusberichte aus anderen Chats); solche Stellen sind in der Anmerkung als „eingefügt“ markiert und gelten als **berichtet, nicht überprüft**.
- **Status 📋** erhält nur, was die Quelle selbst als bestätigt, verbindlich oder vom Entwickler gewählt ausweist. Berichtete Ergebnisse von Kartenläufen sind nie 📋.
- **Primärentscheidungen vom 22.10.2026 (`PE-E1022`)** sind neue, verbindliche Festlegungen des Projektinhabers und nicht im Master enthalten; sie betreffen W-ST-002, W-ST-003 (Metropolis 110 km²), W-RE-005, W-RE-011 (Höllenzeit) und W-KA-017 (Dokumentation der HQ-Lage). Es wurden keine doppelten Entscheidungen angelegt, ausgenommen W-KA-017 für die bisher nicht erfasste Dokumentationsfreigabe.
- Vorschläge des Assistenten tragen 💡, auch wenn der Entwickler mit „ja“ oder „passt“ antwortete, ohne die Zahl zu wiederholen (Begründung: [Modul 01 §3](01-Quellenlage-und-Methodik.md#3-methodik-und-statusregeln)).

---

## 2. Weltgröße und Grundstruktur (GE)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-GE-001 | Die größere Hauptwelt (Arbeitsname WORLD_1500) ist die **Standardwelt des Basisspiels**; WORLD_700 ist nicht die Standardkarte. | 📋 | M-CA0906; M-CU0908; 23#1101E | Der Namensteil „1500“ ist historisch; die Größe hat sich seither mehrfach geändert (W-GE-009 bis 013). |
| W-GE-002 | WORLD_700 ist geografisch und optisch abgeschlossen (`WORLD_MASTER_LOCK_1`); Neues nur additiv, außer der Entwickler öffnet die Geografie ausdrücklich. | 📋 | M-W700; M-AD1015; 23#1019E | 23#1019E: ‚die 700-km²-Karte gilt als abgeschlossen bis auf Ergänzungen‘. |
| W-GE-003 | WORLD_700 ist kein klassisches DLC; mögliche Rollen: Referenzwelt, Benchmark, Quelle bewährter Muster. Keine Rolle ist beschlossen. | 📋 / ❓ | M-CA0906; 23#1101E | Verwendung ❓. |
| W-GE-004 | Es werden **keine Geometrien** zwischen WORLD_700 und der größeren Welt übertragen; der 2.200-km²-Kandidat öffnet WORLD_700 nicht. | 📋 | D18-MAP; D18-CMP; M-AD1015 | Entspricht CLAUDE.md §2 Nr. 7. |
| W-GE-005 | Die finale Weltgenerierung beginnt erst, wenn Anforderungen und räumliche Zuordnung (Spatial Allocation) abgeschlossen sind. | 📋 | M-W1500; M-CA0906; M-AD1015 | Als „CANON POLICY“ bzw. „BLOCKED“ geführt (M-DR1015 D-002). |
| W-GE-006 | 🕰️ Hauptkarte ca. 200–300 km² als frühe Richtung; Endgröße sollte aus Reisezeiten, Straßendichte und Inhaltsdichte kalibriert werden. | 🕰️ | M-PD §World; M-OD | Abgelöst durch W-GE-009 ff. |
| W-GE-007 | 🕰️ 31.08.: Der Entwickler hält 300 km² für zu klein, wenn die Hauptstadt 15–20 % ausmacht; der Assistent arbeitet mit ~620 km² weiter. | 🕰️ | 23#697E; 23#788E | 23#788E ist eingefügte Assistentenantwort. Führte zur WORLD_700-Linie. |
| W-GE-008 | 🕰️ WORLD_700: ca. 690,9 km² Spielfläche in 733,8 km² Hüllfläche (7 Staaten A–G). | 🕰️ / 📋 | G7-CLOSE; G7-GPKG; M-W700 | Als abgeschlossene Referenz gesperrt; Zahlen siehe [Modul 24](24-WORLD700-GIS-Referenz.md). |
| W-GE-009 | 🕰️ 01.09.: „1500km² spielbare fläche“ für die größere Karte; Metropolis 70 km², die nächstgrößten 20–25 km². | 🕰️ | 23#881E | Beginn der WORLD_1500-Linie. |
| W-GE-010 | 🕰️ 06.–10.09.: Standardbereich 1.500–1.800 km²; ~1.650 km² nur Favorit; kein Lock. | 🕰️ | M-CA0906; M-CU0908; M-W1500 | Abgelöst. |
| W-GE-011 | 🕰️ 09.09.: ca. 2.000 km² für die Hauptstory; DLC und die anderen Gebiete (Reiche) kommen zusätzlich. | 🕰️ | 35#87E | Frage 10 der Antwort; ‚Weltgröße 2.000 km²‘ = weiches Ziel. |
| W-GE-012 | 🧭 13.–15.10.: ca. 2.000–2.200 km² als offener, nicht gesperrter Planungsrahmen. | 🧭 | M-MS1013; M-AD1015; M-AD1018 | „not a geometry lock“. |
| W-GE-013 | 🧭 18.–19.10.: Kandidat mit **2.200 km² zusammenhängendem trockenem Festland** (ohne Wasser) plus bewohnter Ostinsel. | 🧭 | D18-MAP; D19-REQ | Aktueller Arbeitsstand; Fläche, Terrain und Geometrie ausdrücklich nicht freigegeben. |
| W-GE-014 | 🧭 Ostinsel: 80–100 km² zusätzliches Land, 90 km² als vorläufige Testgeometrie; bewohnt, geografisch getrennt, auffällig; Name und Form nicht festgelegt. | 🧭 | D19-REQ; D18-MAP | Form nicht beiläufig umzeichnen (D19-REQ). |
| W-GE-015 | Die Welt wird **größenunabhängig** gebaut: etwa 25 Kartengrößen-Varianten; die 2.200 km² dürfen nirgends fest einkodiert werden; Stadtflächen, Detail und Siedlungszahl werden je Variante geplant, nicht linear skaliert. | 📋 | D19-REQ; D19-CMP; 45#22E; 45#24E; M-MS1013 | 45#22E/24E: Test mehrerer Größen bis „1mio km²“. Die 25er-Tabelle selbst ist ein Assistentenvorschlag (45#60A). |
| W-GE-016 | Kein Insel- oder Fortnite-Eindruck; die Welt ist **Festland**, ihre Ränder sind Berge oder anderes Land. | 📋 | 35#876E; 23#779E; D18-MAP; D19-REQ | Eine zusätzliche Insel im Meer ist ausdrücklich erwünscht (23#779E). |
| W-GE-017 | Das Meer liegt im **Osten** (vor allem Ost/Südost); Norden und Süden werden nicht von offenem Meer umschlossen. | 📋 | D18-MAP; 45#279E | 45#279E: „ich will das meer im osten nicht im westen wie bei hfw“. |
| W-GE-018 | 🕰️ Prototyp-Auftrag vom 29.09.: Küste nur im äußersten Südosten (X 56–66 km, Y 0–10 km). | 🕰️ | 30#232E | Eingefügter Auftragstext; abgelöst durch die Ostküste (W-GE-017). |
| W-GE-019 | Das **Hauptgebirge liegt im Norden** und ist ein einzelnes, klares Makromerkmal; sonst Hügel, Wälder, Felder, Täler, Wildnis. | 📋 | D18-MAP; D19-REQ; 35#879E | 35#879E: Berge ‚nur an einer Stelle‘, ‚kann auch durch die Map gehen‘. |
| W-GE-020 | Die **Gipfelhöhe** ist widersprüchlich: 4.000–5.000 m (31.08. und 01.10.), 2.000–2.500 m „befahrbar“ (31.08.), 2.500–3.000 m (Auftrag 29.09.). | ⚠️ | 23#803E; 23#820E; 35#935E; 30#232E | Offen, siehe [Konflikt K4](21-Konflikte-und-Widersprueche.md#k4-gipfelhöhe). |
| W-GE-021 | Die Spielwelt hat eine **natürliche, unregelmäßige Außengrenze**; das Bildrechteck ist nicht die Spielwelt. | 📋 | 35#905E; 35#907E; 35#909E; 30#255E; 30#259E | Mehrere Entwicklerkorrekturen am 01.10. |
| W-GE-022 | 🕰️ Planungshülle 66 × 34 km (2.244 km²) mit Ursprung Südwest (X 0/Y 0), 1-km-Raster, Seitenverhältnis ~1,94:1. | 🕰️ | 30#232E | Auftrag vom 29.09. (eingefügt); durch W-GE-021 relativiert. |
| W-GE-023 | Zwischen den Siedlungen liegen echte Leerräume (5–10 km überwiegend ländlich/Wildnis); die Welt soll nicht leer **wirken**, sondern ‚abgelegen wie im echten Leben‘. | 📋 | 35#87E; D19-REQ; 30#232E | 30#232E §30 ist Auftragstext. |
| W-GE-024 | NPC-Dichte: sehr viele NPCs in der Metropole, wenige auf dem Land, ganz wenige im Gebirge. | 📋 | 23#717E | |
| W-GE-025 | Orte dürfen sich nicht doppeln; die Welt muss einzigartig sein. | 📋 | 23#46E; M-PD §World | |
| W-GE-026 | Die endgültige Größe soll mit aus echten Fahrzeiten, Straßendichte und Fahrzeuggeschwindigkeit abgeleitet werden. | 📋 | M-PD §World; M-PD §Vehicles | Noch nicht geschehen: kein Fahrprototyp mit Karte. |
| W-GE-027 | Die Welt ist fiktiv („nicht wie die USA“); modernes USA-Gefühl; nie nur eine große Stadt pro Bundesstaat. | 📋 | 23#723E; M-PD §World | Ob die 2.200-km²-Karte Bundesstaaten hat, ist ❓ (W-ST-020). |
| W-GE-028 | 09.09.: **Soft Target ~2.000 km²**, kein Hard Lock; finaler Lock erst nach Spatial Allocation (2.000 ist ‚eine schönere Zahl als 1.800‘); die 2.000 km² gehören komplett zum Basisspiel, DLC-Gebiete und Reiche kommen zusätzlich. | 🕰️ | 35#83E; 35#87E; 35#88A | Die Master-Dokumente vom 06.–10.09. nennen weiter 1.500–1.800 km² (M-CA0906); die Sicht vom 09.09. steht nur im Chat. |
| W-GE-029 | 09.09.: Die zusätzliche Fläche gegenüber 1.800 km² soll **regionale Tiefe** liefern (Übergangszonen, Wildnis, Täler, Seen, Straßen, Hotspots), nicht mehr Großstädte; ein spürbarer Teil ist für echte Wildnis reserviert; eine Weltdurchquerung ist merkbar lang, normale Fälle sind regional. | 📋 | 35#86A (Fragen 1–6); 35#87E | Entwickler: ‚Frage 1) ja genau … Frage 5) ja genau‘. |
| W-GE-030 | 28.09.: Die vergrößerte Karte (2.000+ km²) ist **keine Kopie** der 700-km²-Karte, sondern neu angepasst und eher **länglich**. | 📋 | 35#873E; 35#878E | |
| W-GE-031 | 01.10.: Die 105 × 60 km große Anordnung (6.300 km²) „passt gut“, soll aber auf etwa ein Drittel skaliert werden; Ergebnis: ~62 × 35,5 km (Assistent, ×0,591). | 🕰️ | 35#923E; 35#926A | Maßstabs-Rechnung des Assistenten; spätere Karte widerspricht der Form (W-GE-021). |
| W-GE-032 | 09.09.: Der Entwickler beurteilt die erste grobe Prototypkarte als falsch skaliert („wäre die map 500-800km2 nicht 2000km2“) und verschiebt die Korrektur („egal machen wir wann anders“). | 🕰️ | 35#100E; 35#101E | Nur ein Urteil über einen Prototyp des Assistenten; keine Kartenfreigabe, Datei nicht Teil des ausgewerteten Materials. |
| W-GE-033 | Reihenfolge: Die genaue Karte wird später geplant, „wenn viele systeme schon stehen“; zum Anfang zählen laut Entwickler „der loading screen und ein kleiner teil der map“. Vorschlag des Assistenten: Karte parallel zum Code, von grob zu präzise, Makroplanung der Karte etwa im Dezember. | 📋 / 💡 | 35#598E; 35#715E; 35#603A; 35#670A; 35#700A | 📋 nur die Aussagen des Entwicklers; die Zeitplanung ist 💡. Vereinbar mit Phase 0 aus CLAUDE.md §4, aber keine Freigabe einer Karte. |
| W-GE-034 | Weltcharakter: Die Welt soll „hauptsächlich logisch denken und nicht emotional“; das Strafsystem soll „realistisch sein und logisch“ sein (Verbrechen ohne Einschränkung möglich, Folgen im Spiel). | 📋 | 35#532E; 35#536E; 35#530E | Entwicklerwunsch zu Tonalität und Rechtssystem. Weitere weltanschauliche Aussagen dieser Nachrichten gehören zum Adult-Material und stehen dort ([Adult-Register](../Adult/14-Entscheidungsregister.md)); hier nicht erneut bewertet. |
| W-GE-035 | 04.10.: Der Entwickler verlangt, dass „95-97% vom rechteck“ Spielfläche sind („weil sonst ist das nicht gut“). Folge im Assistentenvorschlag: ~2.200 km² in einem Hüllrechteck von ~67 × 34 km (2.278 km², ~96,5 % belegt); der vorherige Vorschlag 75 × 40 km (73 % Belegung) ist damit verworfen. | 📋 / 💡 | 45#18E; 45#19A; 45#17A | 📋 die Belegungsvorgabe; die Maße 67 × 34 sind Rechenergebnis des Assistenten. Spätere Planungshülle der Oktoberaufträge siehe [Modul 03](03-Massstab-Flaechen-Proportionen.md). |
| W-GE-036 | 04.10. „Geografische Masterstruktur v1“ (Assistent): 7 Makroregionen ohne harte Grenzen (nördliches Hochgebirge, Alder-Becken/Nordwest, Western Gorge, zentrales Meridian-Becken, Southwest Frontier, Southern Wilderness, östliche Tiefebene/Greywater-Küste); das Meridian-Wassersystem als „Designgesetz“; Meer nur im Südosten. Der Entwickler: „der rest klingt gut“ (nur der See wird geändert). | 🧭 | 45#7A; 45#8E; 45#6E | Zustimmung zu einer Liste ohne Wiederholung der Einzelzahlen, deshalb 🧭. Teile später überholt (Städtegrößen, K1). Koordinaten und Umrisse siehe W-GE-037. |
| W-GE-037 | 04.10. „Master Coordinates v1“ (Assistent, Koordinatenraum SW = (0,0), NE = (65,34)): Metropolis (34,17), Major City A (15,19), Major City B (51,20), Lake Alder (25,25), Lake Meridian (34,22), Hunter HQ (31,11), Greywater (47,10), Coastal Town (55,8), Harbor Isle (59,5). Die exakten Stadtkoordinaten sollen noch nicht gelockt werden; Reihenfolge: Grenze, Berge, Wasser, Täler, Küste, Städte, Straßen/Bahn, Flughäfen, Motorsport, Siedlungen. | 💡 | 45#13A; 45#12E; 45#14E | Entwickler: „ja genau machen wir das“ und „ok“, keine Einzelwerte. Koordinaten gehören zu einer **früheren** Planungshülle (65 × 34) und dürfen nicht mit WORLD_700-Koordinaten oder dem Kandidaten vom 18.10. vermischt werden. |
| W-GE-038 | 04.10.: Der Assistent schlägt 2.500 km² (~72 × 36 km, ~96 % belegt) als Favorit vor; der Entwickler fragt, ob die Karte größer sein könnte, und beschließt stattdessen Größentests (W-GE-015, 45#22E). | 🕰️ / 💡 | 45#21A; 45#20E; 45#22E | Kein Lock auf 2.500 km². |
| W-GE-039 | 09.10. (Chat 48): Auf die Städtevision hin schlägt der Assistent 10.000–15.000 km² mit 12.000 km² als vorläufigem Favoriten vor (Natur 70 %, Landwirtschaft 20 %, Städte 10 %); der Entwickler: „wir machen erstmal nur die map testen mit extrem vielen verschiedenen größen“. | 🕰️ / 💡 | 48#44A; 48#43E; 48#45E | Kein Lock; durch die Klarstellung 45#61E (Städte sollen sich nur so anfühlen) und die Aufträge vom 18./19.10. (2.200 km²) überholt. |

---

## 3. Karte und Abnahme (KA)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-KA-001 | Es gibt **eine geografisch maßgebliche Karte**; Regional- und Detailansichten leiten sich aus ihren Koordinaten ab; keine unabhängig erzeugten Küsten, Flüsse, Seen oder Stadtstandorte. | 📋 | D18-MAP; D19-REQ; D18-ACC | |
| W-KA-002 | Die visuelle Hauptkarte ist ein **flacher, senkrechter 2D-Atlas**; Perspektive, Isometrie oder 3D-Relief erfüllen die Anforderung nicht. | 📋 | D19-REQ; D19-CHK | Gegensatz zu den früheren isometrischen Aufträgen (30#1E). |
| W-KA-003 | **Keine Karte ist akzeptiert.** Status `VISUAL_NOT_ACCEPTED`; V6 ist nur Kandidat; V5 wurde abgelehnt. | 📋 | D18-MAP; D18-REV; D19-REQ | Stand 18./19.10.; spätere Abnahmen sind im Master nicht belegt. |
| W-KA-004 | V7 bleibt **Stilreferenz**; für die geografische Anordnung hat die Papierskizze Vorrang. Beides begründet keinen Raster- oder GIS-Master. | 📋 | D19-REQ | Die Skizze ist im Master nicht enthalten. |
| W-KA-005 | Gewünschte Lieferung: ein PNG ≥ etwa 7680×4320 (wo möglich), 4K-Vorschau derselben Geografie, Detailausschnitte nur aus dem akzeptierten Master, ZIP mit README, CHANGELOG, Manifest und echten SHA-256. | 📋 | D18-ACC | „user-specified acceptance requirements“. |
| W-KA-006 | Kein Hochskalieren (Lanczos o. ä.) von ~1766×891 oder ~2048×1033 als ‚echtes 8K‘; native Größe, Skalierungsfaktor und Herkunft müssen offengelegt werden. | 📋 | D18-ACC; D19-CHK; D18-REV | |
| W-KA-007 | Stil: hochwertige satelliten-/atlasähnliche Kartografie; vermeiden: Schematik als Endkunst, Spielzeugfarben, wolkige Texturen, ovale/kreisrunde Stadtflächen, Raster-/Voronoi-Bezirke, zu viele Kanäle, Felsen außerhalb des Nordens. | 📋 | D18-ACC | |
| W-KA-008 | Ein **geheimes Hunter-Hauptquartier wird auf einer öffentlichen Spielerkarte nicht auffällig beschriftet**; geheime Orte liegen in getrennten, geschützten Ebenen. | 📋 | D18-MAP; D19-REQ; D19-CMP | D19-CMP: einige Karten vom 19.10. zeigten „Hunter HQ“ trotzdem (Qualitätsbefund). Betrifft die Spielerkarte; die Dokumentation der HQ-Lage regelt W-KA-017. |
| W-KA-009 | Reihenfolge der Abnahme: erst visuelle Gesamtkomposition, dann Details; keine weitere lange Hydrologie-Forschung als Ersatz für eine überzeugende Gesamtkarte. | 📋 / 💡 | D18-ACC | Teil „Deliberate non-goals“ als Anforderung, Gates als Methodenvorschlag. |
| W-KA-010 | Die Statusbegriffe `CANDIDATE`, `VISUAL_NOT_ACCEPTED`, `ACCEPTED_MASTER`, `TECHNICALLY_UNVALIDATED` sind zu unterscheiden; bestandene Prüfsummen bedeuten keine visuelle Abnahme. | 💡 | D18-ACC | Vorgeschlagenes QA-Verfahren, nicht freigegeben. |
| W-KA-011 | Messbare Abnahmeprüfungen (Flächenbilanz mit Toleranz, Flussgraph, Lagekonsistenz, Geheimnisebene, Siedlungshierarchie, Ableitung aller Exporte aus einer Geometrie). | 💡 | D19-CHK | „engineering test proposal“. Details in [Modul 17](17-Weltgenerierung-und-GIS-Daten.md). |
| W-KA-012 | Entwicklerskizze (13.10., Papier, „extrem grob“): Gebirge im Norden, Metropolis zentral, zwei weitere Großstädte (A, B) westlich und östlich, Meer rechts mit größerer Insel, HQ abseits. Maßstab nicht aussagekräftig. | 📋 | 45#277E; 45#279E; 45#278A | 45#279E: A und B sind die Major Cities, Maßstab „nicht gut“, HQ „auf jeden fall nicht in der stadt oder daneben“. Skizze selbst nicht im Master. |
| W-KA-013 | 🕰️ Kartenkandidaten V3–V6 (Oktober), V3.1–V3.1t (Terrain-Forschung) und V1.0–V1.3e.3 (Welt-Blueprint) sind **weder akzeptiert noch gelieferte Master**. | 🕰️ | D18-REV; 45#80E bis 45#274E; 54#104E | Berichte eingefügt; Rohdateien nicht im Master. [Modul 19](19-Kartenreferenzen-und-Versionsgeschichte.md). |
| W-KA-014 | Die Karte braucht Größeneinheit, Legende und Maßstab, damit ihre Größe lesbar ist. | 📋 | 35#905E | |
| W-KA-015 | Das Lieferpaket der Karte ist ein einzelnes ZIP je Iteration (nicht viele Einzeldateien). | 📋 | 30#104E | Arbeitsregel des Kartenlaufs, 01.09. |
| W-KA-016 | Berichte aus Chat 54 (09.–13.10., Assistent, nicht überprüft): „Variante 1/25 – 2.200 km²“ mit Kartenrechteck 67,4 × 34 km, 96,00 % Rechteckabdeckung, Lake Alder 25, Lake Meridian 5, Südreservoir 7, Metropolis 22,5, Major City A/B je 9 km², Raster 50 m; V2 mit höchstem Gipfel 4.600 m, 8,42 % Hochalpin, 4.177 Gebäude-Proxys, 21 Hauptverbindungen und 128 von 131 bestandenen Tests (drei bekannte Fehler: Alder entwässert nicht durch Meridian, Gefälleprobleme, zwölf Trassenabschnitte). | 🕰️ | 54#3A; 54#7A | Berichtete Prototypwerte, keine Kartenabnahme (W-KA-003, W-KA-013). Metropolis 22,5 km² stammt aus dem Testlauf, nicht aus der 110-km²-Vorgabe. |
| W-KA-017 | Die genaue geografische Lage des Hunter HQ darf in der allgemeinen, öffentlich zugänglichen Weltdokumentation stehen. Die Freigabe betrifft die geografische Dokumentation, **nicht** geheime Eingänge, Story-Enthüllungen oder interne Einrichtungen. Die Spielerkarte beschriftet das HQ weiterhin nicht auffällig (W-KA-008). | 📋 | PE-E1022 | Neue Primärentscheidung des Projektinhabers vom 22.10.2026. Ob die exakte Position festgelegt ist, ist ❓ (OF-69); belegt ist nur die Lagebeschreibung W-ST-035. |

---

## 4. Städte und Siedlungen (ST)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-ST-001 | Metropolis (Arbeitsname) ist die größte Stadt und das zentrale urbane Gebiet; sie liegt nicht am Kartenrand. | 📋 | D18-MAP; 45#278A | |
| W-ST-002 | **110 km²** für Metropolis (zusammenhängendes Stadtgebiet einschließlich Vororten, W-ST-003) für den Kandidaten mit 2.200 km² trockenem Festland; kein dauerhafter Weltmaßstab, da andere Kartengrößen andere Werte erhalten. | 📋 | PE-E1022; D18-MAP; D19-REQ; D18-CMP | Bestätigt am 22.10.2026 (neue Primärentscheidung). Zuvor 🧭 Arbeitsgröße. |
| W-ST-003 | Die 110 km² sind **zusammenhängendes Stadtgebiet einschließlich der Vororte** (urbaner Kern, weitere Stadtviertel, zugehörige Vorortbereiche). Sie bezeichnen **nicht** nur die dicht bebaute Innenstadt und **nicht** die weiter gefasste Metropolregion mit eigenständigen umliegenden Siedlungen. Die 110 km² sind Teil der 2.200 km² Festland und kommen nicht zusätzlich hinzu. | 📋 | PE-E1022; 45#284A; D19-REQ | Neue Primärentscheidung des Projektinhabers vom 22.10.2026; klärt [K1](21-Konflikte-und-Widersprueche.md#k1-metropolis-fläche) (Flächenabgrenzung) und OF-11. Die frühere Folgerung war 🧭. Es gibt keine festgelegten Stadtteilflächen oder Stadtgrenzen über die in W-ST-004/005 genannten hinaus. |
| W-ST-004 | Downtown ca. 20 km²; der Hochhauskern ca. 12 km² liegt **innerhalb** der Downtown. | 🧭 | D18-MAP | |
| W-ST-005 | Metropolis hat **20 unterscheidbare Bezirke**; Namen offen. | 🧭 | D18-MAP | |
| W-ST-006 | 3–5 Landmarken-Wolkenkratzer im Bereich ca. 700–1.000 m als Gestaltungsziel; Machbarkeit offen. | 🧭 | D18-MAP; D18-OQ | Keine vorhandenen Modelle. |
| W-ST-007 | Ein dominanter Fluss führt durch bzw. entlang eines **äußeren** Bezirks, nicht durch Downtown/Hochhauskern; Inseln, Arme und Brücken begrenzt. | 📋 | D18-MAP; D19-REQ | Der Assistentenvorschlag ‚breiter Fluss mitten durch die Innenstadt‘ (45#62A) ist damit nicht übernommen. |
| W-ST-008 | Organische, nicht kreisförmige Form; Wohn-, Gewerbe- und Industrienachbarschaften, Parks, Straßen- und Bahnhierarchie, Brücken, glaubhafte Dichtegefälle. | 📋 | D18-MAP; D18-ACC | |
| W-ST-009 | New York, Chicago, San Francisco, Boston sind **Referenzen für Gefühl, Vielfalt und Dichte**, keine Flächenvorgabe; die aus Hilfsrechnung stammenden 570 km² (350+100+120) werden als Projektentscheidung abgelehnt. | 📋 | M-MS1013; M-AD1015; 45#61E | 45#61E: es solle sich nur ‚so anfühlen‘; ‚in Spider-Man ist Manhattan 5 km² groß‘. |
| W-ST-010 | 🕰️ Städteflächen-Test für 2.200 km²: Metropolis 20–25 km², Major Cities 8–10 km²; Wachstum mit der Weltgröße sublinear. | 🕰️ | M-MS1013; M-AD1015; 45#62A | „Open hypotheses“; für den Kandidaten vom 18.10. durch 110 ersetzt (D18-MAP). |
| W-ST-011 | 🕰️ Bisherige GIS-Testwelt (2.200 km²): Metropolis 22,5 km², Glenhaven 9 km², Ashford 9 km² als bestehende Stadtkerne. | 🕰️ | 45#216E; 45#282A | Berichtet (eingefügt); nicht akzeptierte Testwelt. |
| W-ST-012 | 🕰️ 09.10.: Assistenten-Vorschlag mit 58 Siedlungen (Metropolis 70 km², Glenhaven/Ashford je 30 km², weitere Stufen bis 0,1 km²; gesamt ~193,5 km² = 8,8 %). | 💡 | 45#282A; 45#284A | Entwickler: ‚ist für mich ok … ich will die auflistung noch genauer‘ (45#283E). Nicht als Entscheidung geführt. |
| W-ST-013 | Siedlungshierarchie: Metropolis > mehrere Großstädte > weitere Städte > Kleinstädte > Dörfer/Weiler; größere Städte dürfen nicht winzige Punkte sein, zugleich keine Überfüllung mit Kleinstsiedlungen. | 📋 | D19-REQ; D19-OQ | Zahlen offen. |
| W-ST-014 | **Glenhaven und Ashford** liegen binnenländisch, abseits großer Seen und Flüsse; nicht um Flüsse, Seen oder Meeresfront gebaut. | 📋 | D18-MAP; D19-REQ | |
| W-ST-015 | **Greywater** ist die einzige klar maritime Hafenstadt; glaubwürdige Küste und Hafen, **nicht venezianisch**, nicht von Kanälen durchzogen, nicht im überfluteten Sumpf. | 📋 | D18-MAP; D19-REQ | |
| W-ST-016 | ⚠️ Namenszuordnung: In WORLD_700 heißen die Großstädte „Major City A/B“; im 2.200-km²-Kandidaten Glenhaven/Ashford/Greywater. Dass A/B = Glenhaven/Ashford ist, nennt kein Entwicklertext ausdrücklich. | ⚠️ | 45#278A; 45#279E; 45#282A; D18-MAP | Siehe [Konflikt K6](21-Konflikte-und-Widersprueche.md#k6-namen-der-großstädte). |
| W-ST-017 | Stadtcharaktere (September): eine Großstadt mit Industrie-/Automobil-/Bahnlogistik-Identität (Altfabriken, Leerstand, Umnutzung); eine mit Ingenieurwesen/Forschung/Hightech und Berg-/Wintertourismus. | 🧭 | M-WB17 Block 1; M-W15R | Zuordnung zu Namen nicht dokumentiert. |
| W-ST-018 | Verschiedene Stadttypen: Metropole, Küstenstadt, klassische Kleinstadt (Main Street, Sheriff, Diner, Tankstelle), Berg-/Dorfsiedlung; auch Kleinstädte mit Clubs, Tattoo-Studios usw. | 🧭 | 45#284A; 23#705E | Assistentenbild, Entwickler ergänzte Gewerbe (23#705E). |
| W-ST-019 | Bergsiedlungen sind klein: ‚auf einem Berg keine 20-km²-Stadt, höchstens ein 1–2 km² großes Dorf‘; Berge brauchen Straßen mit kleinen Dörfern. | 📋 | 35#89E; 23#705E | |
| W-ST-020 | **Sieben Bundesstaaten** (Untergebiete je Staat) sind in der früheren Planung beschlossen: ‚machen wir es mal mit 7 Bundesstaaten‘ (31.08.); 5–10 Staaten mit Unterregionen, damit Sammelobjekte und Inhalte auf der Spielkarte besser sichtbar werden; Gesetze unterscheiden sich je Staat. Für die Oktober-Linie (2.200 km²) nennt kein Dokument Staaten. | 📋 (bis 09.09.) / ❓ (Oktober) | 23#719E; 23#721E; 23#729E; 35#96A; 35#97E; M-W15R; D18-MAP | Siehe [Konflikt K7](21-Konflikte-und-Widersprueche.md#k7-staatenstruktur). |
| W-ST-021 | Bevölkerung konzentriert sich in den Städten; Land und Wildnis bleiben dünn besiedelt; mehr Fläche der größeren Welt dient Wildnis, Wasser, Gebirge, Abstand, nicht proportional mehr Stadt. | 📋 | M-WB17 Block 1; M-LATE; 23#1009E | 23#1009E Nr. 23/24: beide Welten ähnliches Gefühl, die größere „mehr Wildnis“. |
| W-ST-022 | Eine große Stadt soll nicht größer als ihr Anteil am Land plausibel erlaubt; Obergrenzen für sehr große Welten (Städte wachsen nicht mit). | 📋 | 45#61E; M-MS1013 | 45#61E Nr. 3: bei 100.000 km² muss die Stadt nicht 2.000 km² groß sein. |
| W-ST-023 | 🕰️ Stadtanteil an der Weltfläche: ‚höchstens 12–18 %‘ (09.09.); Nachfrage nach dem US-Wert ~3 % (04.09.). Beides ist keine aktuelle Vorgabe; Siedlungsanteil bleibt offen. | 🕰️ / ❓ | 35#87E; 23#1069E; M-LATE | |
| W-ST-024 | 🕰️ Megacity 70–73 km², zwei Major Cities je 20–25 km² (WORLD_1500-Anforderungen vom 04.09.). | 🕰️ | M-W15R; M-WB17; 23#881E | Vor dem Oktober-Kandidaten. |
| W-ST-025 | Städte verändern sich über lange Zeiträume durch Sanierung, Aufwertung und Verfall; Neubau, Abriss und Verkehrsprojekte laufen über Jahre. | 📋 | M-WB17 Block 1/4 | |
| W-ST-026 | Ein Flughafen liegt **außerhalb des Kerns** (Vorbild: Wien-Schwechat). | 📋 | 23#907E; M-W15R | Antwort 8 der Liste. |
| W-ST-027 | Mindestens ein Flughafen pro Bundesland und einige Flugplätze (zum Beispiel am Berg) — Stand der Sieben-Staaten-Idee. | 🕰️ | 35#93E | Hängt an W-ST-020. |
| W-ST-028 | 09.09. **WORLD_2000-Arbeitsrichtung** (7 Staaten): 1 Metropolis 70–80 km²; 6 State Cities 20–35 km²; 3–5 mittlere Städte 10–18; 5–8 kleinere 5–10; 10–20 Towns 1,5–5; viele Dörfer 0,2–1,5; Stadtfläche insgesamt 12–18 % (~240–360 km²). Entwickler: „Ja das ist besser“. | 🕰️ | 35#96A; 35#97E; 35#98A | Vor dem Oktober-Kandidaten; Zahlen von 110 km² Metropolis überholt. |
| W-ST-029 | 09.09. Flächenbudget für ~2.000 km² (Assistent): Städte/Siedlungen 250–270 km² (~13 %); Wälder ~450, Gebirge ~320, Farmland ~330, Seen/Flüsse/Wetlands ~180, Grasland ~160, Küste ~70, Industrie ~55, Straßen/Bahn ~65, Flughäfen ~25, Motorsport ~15, POI/Park/Reserve ~70 km². | 💡 | 35#92A | Ausdrücklich ‚noch kein Lock‘; ‚Flächenbudget‘, Überlappungen erlaubt. |
| W-ST-030 | Jeder Bundesstaat soll sich wie ein eigener Staat anfühlen (nicht ‚Metropolis plus zwei Städte plus Landschaft‘) und eine eigene Mischung aus Motorsport, Industrie, Bergen, Landwirtschaft, Pferdesport, Küste erhalten, ohne ‚Biome-Staaten‘; Staaten haben unterschiedliche Gesetze. | 🧭 | 35#95A; 35#96A; 35#97E; 23#719E | Hängt an W-ST-020. |
| W-ST-031 | Stripclubs „essentiell“: in Metropolis „alle 15-30“, in kleineren Städten weniger, dazu ein riesiger mehrstöckiger Club mit Auswahl nach Budget; die Zuordnung zu Nachtclub oder Bordell ist offen. | 📋 / ❓ | 35#526E | Quellenzitat des Entwicklers, Details in [Adult/26](../Adult/26-Spezifikation-Weltintegration-Branche-Wirtschaft.md). Der Assistent nennt als Beispiel „30 Stripclubs“ und sieben Krankenhäuser für Metropolis (35#603A, 💡). |
| W-ST-032 | Gewerbe-Gefälle Stadt/Land: Tattoo-Studios in Metropolis teils sehr groß und mehrstöckig, dörfliche eher klein; Tattoos sind in der Stadt häufiger als auf dem Land; die Preise liegen „in der downtown“ deutlich über denen auf dem Land. | 📋 | 35#775E; 35#749E; 35#761E; 35#767E | Dichte pro Stadt in Prozent wurde nicht festgelegt („stellen wir gleich auf“, 35#775E). Siehe [Adult/25](../Adult/25-Spezifikation-Tattoo-und-Koerpermodifikation.md). |
| W-ST-033 | Planungsreihenfolge für Städte (Assistent): Makro-Zuordnung, Siedlungsplanung, Gebäudetypen-Liste, Bedarf je Ort, Stadtteile, Hero-Gebäude, KI-Varianten, erst danach Unreal. Der Entwickler fragte „Wann planen wir die gebäude…“. | 💡 | 35#531A; 35#530E; 35#670A | Antwort des Entwicklers auf den Ablauf liegt in Chat 35 nicht vor. |
| W-ST-034 | 04.10.: Städtegrößen „20–25 km² / 8–10 km² / 8–10 km²“ (Metropolis, Major City A, B; Downtown ~4–5 km², Coastal Town 1,5–2 km²) als bereits festgelegte Zielwerte für 2.200 km²; sie lösten Werte von 40–50 bzw. 15–22 km² ab. Der Entwickler bestätigt den Lockvorschlag mit „ok passt ja“ und „ja mache das“. | 🕰️ | 45#3A; 45#5A; 45#4E; 45#6E | Der Assistent beruft sich auf Dateien der Planung vom ≤ 04.10. Vor der Städtevision vom 09.10. (45#61E) und der 110-km²-Linie (D18-MAP); seit 22.10.2026 gelten 110 km² verbindlich (W-ST-003), die 20–25 km² sind in der Übersichtsdatei als historischer Planungsstand geführt und nicht mehr gleichberechtigtes Testszenario. Siehe [Chronik](03-Massstab-Flaechen-Proportionen.md#4-chronik-der-metropolis-fläche) und K1. |
| W-ST-035 | 04.10.: Hunter HQ nicht neben der Stadt, sondern südlich/südwestlich von Metropolis am Übergang vom Meridian-Becken zur Southern Wilderness, etwa 10–15 Straßenkilometer vom Zentrum, versteckt und waldnah. | 🧭 | 45#7A; 45#13A; 45#8E; 45#279E | Entwickler 13.10.: HQ „aufjedenfall nicht in der stadt oder daneben“ (W-KA-012). Verhältnis zum Untergrund-Bunker offen (K10). Die Lagebeschreibung darf nach W-KA-017 in der allgemeinen Weltdokumentation stehen; die exakte Position im 2.200-km²-Kandidaten ist ❓ (OF-69); die Koordinaten des Entwurfs vom 04.10. sind nur Vorschläge einer früheren Hülle (W-GE-037). |
| W-ST-036 | 31.08.: Eine riesige Metropole und „2 größere städte“; die Abstände zwischen größeren Siedlungsräumen werden „eher 5-10km“ als 20 km sein; kleinere Dörfer bleiben, wo sie hinpassen. Der Entwickler hatte zuvor mehrere kleinere Städte mit mehr Leerraum erwartet und ließ beide Ansätze zeichnen. | 📋 | 23#707E; 23#709E; 23#711E; 23#699E | Bestätigt die Struktur Metropolis plus zwei Major Cities (W-ST-001/016). |
| W-ST-037 | 01.09. (WORLD_1500-Linie): Downtown/Kern wird von 4–5 auf ca. 7–8 km² gesetzt, die Skyline hat Hochhäuser von 350–500+ m; das Stadtgebiet (City Proper) hat 70 km², **außerhalb** liegt ein Metropolitan Belt (Satellitenstädte, Suburbs, Flughafen, Gewerbe), die zusammenhängende Metropolregion soll „deutlich über 100 km²“ umfassen. Stark community-geprägte Viertel eher in äußeren Bezirken. Entwickler: „ja passt, aber die wolkenkratzer sind riesig“; „ja genau“. | 🕰️ | 23#908A; 23#909E; 23#910A; 23#911E | Wortlaut „riesig“ ist Entwicklerzusatz. Bedeutung für W-ST-003: Das Belt-/Regionsmodell vom 01.09. ist nicht gemeint; PE-E1022 stellt klar, dass die 110 km² nicht die Metropolregion mit eigenständigen umliegenden Siedlungen bezeichnen. |
| W-ST-038 | 09.10. (Chat 48): Der Entwickler will „richtig krasse städte“ wie San Francisco, New York, Boston und Chicago und „eine gigantische Hauptmetropole und mehrere unterschiedlich große Städte“. Assistentenvorschläge: Metropolis 150–300+ km², Major City A 60–120, B 40–90, ausdrücklich „erste Diskussionswerte“ und mit der Warnung, Stadtgebiet, Verwaltungsgebiet und Metropolregion zu unterscheiden. | 📋 / 🕰️ | 48#39E; 48#41E; 48#40A; 48#42A | 📋 die Vision; die Flächen sind 💡 und wurden durch 45#61E (nur das Gefühl, Maßstab an die Weltgröße angepasst) eingeschränkt. Ursprung der Städtevision von W-ST-009. |

---

## 5. Urbane Gestaltung, Gebäude, Orte (UR)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-UR-001 | Mindestens 90 % der normalen Gebäude sind in irgendeiner Form betretbar; Innenraumstufen Hero / Full Gameplay / Procedural / Limited. | 📋 | M-WB17 Block 4; M-PD §World | Reine Fassaden sind eine Minderheit. |
| W-UR-002 | Hero-Gebäude sind einmalig, Fall-/Story-Gebäude stark authored; Modulassets dürfen wiederverwendet werden, die Orte müssen trotzdem stark variieren. | 📋 | M-PD §World | |
| W-UR-003 | Der Spieler kann Land kaufen und ein Haus frei planen und bauen (Garage, Werkstatt, Hunter-Räume, Archiv, Serverraum, Gym, Pool, Garten, Sicherheit, Notstrom, schnelles Internet); Größe begrenzt durch Land, Geld, Gelände, Regeln, Technik. | 📋 | M-WB17 Block 4; 23#1025E | Kaufen/Mieten/Weitervermieten (35#41E). |
| W-UR-004 | Immobilienwerte reagieren auf Lage, Zustand, Aussicht, Verkehr, Kriminalität, Wirtschaft, Entwicklung. | 📋 | M-WB17 Block 4 | |
| W-UR-005 | Gewerbe- und Dienstleistungsvielfalt (Einzelhandel, Gastronomie, Nachtleben, Casinos, Banken, Hotels, Krankenhäuser, Colleges, Museen, Zoo, Arenen, Händler, Tuning, Parkplätze, Logistik, Friedhöfe, Notdienste, Rechenzentren, Marinas, Bergresorts, Campingplätze, Hunter-Fachgeschäfte). | 📋 | M-WB17 Block 4 | |
| W-UR-006 | Wasserpark (mindestens einer, benutzbar) und Freizeit-/Achterbahnpark (mindestens einer, fahrbar); kleinere Parks später bei der räumlichen Zuordnung; selbst gestaltete Achterbahn = niedrige Priorität. | 📋 | 23#1039E; M-WB17 Block 7; M-LATE | Kein Startumfang. |
| W-UR-007 | Nachtleben ist ein wesentlicher Weltlayer (Clubs, Bars, Stripclubs, optionale Erwachsenenbetriebe nach fiktiver Rechtslage); optional, ohne unverhältnismäßigen Entwicklungsanteil. | 📋 | M-WB17 Block 7; 23#1035E | Details im [Adult-System](../Adult-System.md). |
| W-UR-008 | Nationale Ligen werden simuliert, ohne dass jedes Stadion auf der Karte existiert; nur Heimstadien der Region sind physisch; Mehrzweckarenen für Basketball, Eishockey, Kampf, Konzerte. | 📋 | M-WB17 Block 7 (+Zusatz); 23#1035E | 23#1035E Nr. 14: eine Liga komplett simuliert, Stadionbesuch möglich. |
| W-UR-009 | Die Welt plant Veranstaltungen (Autotreffen, Sport, Clubnächte, Konzerte, Märkte, Kämpfe, Motorsport, Festivals, Pferderennen), NPCs besuchen sie und beeinflussen Verkehr, ÖPNV, Parken, Gastronomie, Hotels, Wettmärkte. | 📋 | M-WB17 Block 7 | |
| W-UR-010 | Stadt-Formel-1-Straßenkurse und Rallyes in der Stadt mit gesperrten Straßen an Ereigniszeiten; Rallyes sind mehrere getrennte Veranstaltungen mit 10–20 Etappen. | 📋 | 35#562E; 35#566E | Antwort des Assistenten dazu nicht ausgewertet. |
| W-UR-011 | Menschenmengen sind an Stadien, Festivals, Nachtleben, Downtown, Skiorten, Motorsport und Parks wesentlich; Crowd/Verkehr nutzen LOD und rekonstruieren lokal glaubwürdige Bevölkerung. | 📋 | M-WB89 Block 8 | |
| W-UR-012 | Dynamische Zwischenfälle verursachen dauerhafte Schäden; NPC-Trupps reparieren Straßen, Gebäude und Versorgung über glaubhafte Zeit. | 📋 | M-WB89 Block 8 | |
| W-UR-013 | Die erste Stadtkartografie für Städte soll mit QGIS beginnen („nur ein ungefähres Bild für später“). | 📋 | 23#717E | Arbeitsmethode der Preproduction. |
| W-UR-014 | Gestaltungsabsicht: Hügel, Vegetation, Parks, Flüsse, organisch gewachsene Straßen und Verkehrsnetze; Topografie und Hydrologie zuerst, daraus Stadtentwicklung. | 📋 | M-MS1013; 45#61E; 45#62A | Letzteres Assistentenaussage; Entwickler: Fluss, Vegetation, Steigung gewünscht (45#61E Nr. 4). |
| W-UR-015 | Metropolis: starke Hochhausdichte, aber nicht jeder Bezirk; historisch gewachsener Charakter; europäisch geprägte Fußgänger-/Verkehrslogik mit nordamerikanischem Straßenmaßstab; Viertel wie „Little Italy, Bronx“. | 🧭 | M-W15R; M-WB17; 23#907E | Für die 70-km²-Linie formuliert, nicht gegen 110 km² erneut bestätigt. |
| W-UR-016 | Wirtschaft der Hauptstadt: Finanzen (nicht übergroß), Tech-/Forschungscluster höchstens ~1–2 km², Medien, Gesundheit/Pharma, Tourismus. | 🧭 | M-WB17 Block 2 | |
| W-UR-017 | Immobilien sind in der Downtown deutlich teurer, am Stadtrand wieder billiger („wie in echt“). | 📋 | 35#41E (Frage 5 und 6) | Antwort auf Paket 11A. |
| W-UR-018 | Städte wachsen über Jahre bis Jahrzehnte („deshalb kann über jahre bis jahrzehnte die städte wachsen“); große Rechenzentren in eigenen Industrie- oder Tech-Gebieten „so wie siliconvally“, nicht in der Downtown. | 📋 | 23#1023E | Antworten auf Block-4-Fragen 35 und 43 (Assistent 23#1022A). Vereinbar mit W-TV-001 (Baustellen). |
| W-UR-019 | Religiöse Bauten: Kirchen ja; andere Glaubensbauten „vielleicht vereinzelt in einem eigenen disktrikt“, mit der Fiktion, dass sie nicht gern gesehen werden. Der Assistent hatte normale Präsenz mehrerer Glaubensrichtungen vorgeschlagen. | 📋 / ❓ | 23#1023E; 23#1022A | Abweichung vom Assistentenvorschlag; Darstellung, Plattform- und Sensibilitätsfragen sind ungeklärt (OF-68). Weiteres zu Glaubensanteilen steht im [Adult-Material](../Adult/14-Entscheidungsregister.md). |

---

## 6. Natur, Berge, Wald (NA)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-NA-001 | Große zusammenhängende Wälder, Wildnis, Täler, Seen, Weiden und Ackerland gehören zur Welt; breite Räume machen lange Fahrten glaubwürdig und vermeiden den ‚Spielzeugwelt‘-Eindruck. | 📋 | D18-MAP; D19-REQ; M-W15R | |
| W-NA-002 | Gebirge, Wald und Täler wirken natürlich, nicht wie einheitliche Klippenränder oder eine komplette Felslandschaft. | 📋 | D18-MAP; D19-REQ; 35#909E | |
| W-NA-003 | Das Gebirge soll verwinkelt sein und viele Geheimnisse („Mysteries“) bergen, nicht alles sichtbar. | 📋 | 23#780E | 31.08. |
| W-NA-004 | Im Gebirge: Straßen bis auf eine gewisse Höhe; Wintersport (Ski, Snowboard, Langlauf, Skitouren, Schneemobil, Lawinenrisiko); Gondeln; hohe Berge nur mit Spezialfahrzeugen oder Gondeln befahrbar. | 📋 | 35#947E; 23#821E; 23#823E; M-WB17 Block 7 | |
| W-NA-005 | Hochgebirge kühl ganzjährig (Sommer ~10–15 °C, Winter −10 °C und kälter); Schneeansammlung und Spuren; vier Jahreszeiten; Gewitter, Überschwemmung, Sturm, Nebel, seltene Tornados und Waldbrand. | 📋 | M-WB17 Block 6 | Tag/Nacht-Takt ‚ca. 2 Echtstunden je Spieltag‘ = nicht gesperrt. |
| W-NA-006 | Tierwelt, Jagd, Angeln, Fotografie, Höhlen und Höhlentauchen; Tiere reagieren auf Übernatürliches ohne perfekte Detektoren. | 📋 | M-WB17 Block 6/7 | Weitere Tiersysteme: [Gameplay-Systeme](../Gameplay-Systeme.md). |
| W-NA-007 | Sehr ausgeprägte Unterwasserwelt mit vielen Tieren in allen Gewässern (Tauchen, Schnorcheln, Wracks, Höhlen). | 📋 | 23#1035E Nr. 72; M-WB17 Block 6/7 | Begründung für mehr Küste/größere Seen als Option (M-W15R). |
| W-NA-008 | 🕰️ Landnutzungsmix im Auftrag vom 29.09. (Stadt 4–5 %, Acker ~25 %, Wald ~30 %, Gebirge 15–18 %, Feuchtgebiet ~4 %, Gewässer ~5 %). | 🕰️ | 30#232E | Planungsziele eines Prototypauftrags; nicht übernommen. |
| W-NA-009 | Weniger Gebirge/mehr Vielfalt: ‚Berge nicht überall, sondern auch anderes; Inspiration bei großen AAA-Karten‘. | 📋 | 35#909E; 35#911E | |
| W-NA-010 | Südwesten trockener und offener (Ranch, Trockengras, Buschland, kleine Canyons) — Auftragsdetail. | 🕰️ | 30#232E | Nur Prototypauftrag. |
| W-NA-011 | Fauna-/Vegetations-/Ökologie-Sukzession im Zeitverlauf (saisonale Vegetation, langfristige Entwicklung). | 📋 | M-WB17 Block 6 | |
| W-NA-012 | 04.10.: Nordgebirge aus **drei verbundenen Massiven** (Alder Highlands, Meridian Crown, Eastern Range), nur ~8–12 % der Welt hochalpin, höchste Gipfel ~4.400–4.900 m (nur wenige), ein Ausläufer („Ski-Arm“) zieht 6–10 km nach Süden; Hauptstraße plausibel bis ~2.500–3.200 m, Gipfel unerschlossen. | 🧭 | 45#7A; 45#13A; 45#3A; 45#8E | Entwickler „der rest klingt gut“. Fließt in den Auftragstext vom 09.10. ein (45#64A, Auftragstext). Berührt [K4](21-Konflikte-und-Widersprueche.md#k4-gipfelhöhe). Berichtete Kartenwerte 4.600 m (V2/V3.1: 54#7A, 54#25A, 45#89A) sind Prototypberichte. |
| W-NA-013 | 04.10.: Westen als Fluss-Schlucht (Western Gorge) mit bewaldetem Plateau statt zweiter Gebirgswand; Südwesten als offene Weite mit Grasland, Ranches, Mesas und kleinen Canyons (ausdrücklich keine Wüste); Süd-Zentrum als große Wald-/Hochlandwildnis von ~150–250 km². | 🧭 | 45#7A; 45#13A; 45#8E | Deckt sich mit W-NA-010 (Auftragsdetail). |

---

## 7. Gewässer (WA)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-WA-001 | Benannte Gewässer des Kandidaten: **Lake Alder** (mit Alder Isle, ~25 km²), **Lake Meridian** (~5 km²), **South Reservoir** (~7 km²), **Greywater Bay**. Genaue Ufer und relative Lagen sind Sache des akzeptierten Masters. | 🧭 | D18-MAP; D19-REQ | Seeflächen sind nicht im Festland-Trockenland enthalten. |
| W-WA-002 | Lake Alder soll größer sein, damit Tauchen, Surfen, Angeln und mehr möglich sind: ‚auf 25 km²‘. | 📋 | 45#8E | Entwickler-Anweisung 04.10. |
| W-WA-003 | Flüsse, Kanäle, Sumpfarme und zersplittertes Wasser werden **deutlich reduziert**; wenige zusammenhängende Entwässerungssysteme, glaubhafte Seeabflüsse; Wasser dominiert das bewohnbare Land nicht. | 📋 | D18-MAP; D19-REQ | |
| W-WA-004 | Der **Hauptfluss** verläuft vom westlichen/nördlichen Seensystem an den nördlichen äußeren Bezirken von Metropolis vorbei zur Ostküste; die Strecke Lake Alder – Metropolis darf nicht abrupt enden; er führt nicht durch Glenhaven oder die Innenstadt. | 📋 | D19-REQ; D19-CHK | |
| W-WA-005 | Die Ostküste ist ruhig und organisch regelmäßig, kein Labyrinth aus Mündungen und Inseln; eine auffällige große Insel im Osten/Südosten. | 📋 | D18-MAP; D19-REQ | |
| W-WA-006 | South Reservoir liegt weiter im Südosten als in kritisierten Entwürfen. | 📋 | D19-REQ | |
| W-WA-007 | Natürliche Seeabläufe und zusammenhängende Flusstopologie; keine freien Flussenden, ungeklärte Gabelungen oder unplausible Kreuzungen (Prüfanforderung). | 📋 / 💡 | D19-REQ; D19-CHK | Anforderung 📋, Prüfmethode 💡. |
| W-WA-008 | 🕰️ Prototyp-Hydrologie (29.09.): Seen/Flusskette Berge → Alder → Meridian → Metropolis → Greywater-Auen → Südostküste; Greywater-Feuchtgebiet 50–80 km², Lake Alder 15–22 km², Meridian 5–8 km². | 🕰️ | 30#232E | Auftragstext; teils durch W-WA-001/003 ersetzt (Feuchtgebiete reduziert). |
| W-WA-009 | WORLD_700-Gewässer (gesperrt): Lake Alder 6,3 km², Lake Meridian 4,9 km², Greywater-Feuchtgebiet 8,7 km², Eagle Lake, Summit Lake, Pine Hill Lake, Südwest-Talreservoir mit Damm. | 🕰️ | G7-GPKG | Eigene Linie; Zahlen berechnet aus der Geometrie ([Modul 24](24-WORLD700-GIS-Referenz.md)). |
| W-WA-010 | Seen und Küsten sind Aktivitätsräume: Tauchen, Surfen, Angeln, Segeln, Wassersport; Marinas mit Service. | 📋 | 45#8E; M-WB17 Block 7 | |
| W-WA-011 | Der Entwickler wünscht mehrere große und kleine Seen für verschiedene Aktivitäten und nur einen kleinen Küstenanteil statt „150 km Küste“. | 🕰️ | 23#779E | Frühe Fassung; die Ostküste von 2026-10 ist neu bewertet (W-GE-017). |
| W-WA-012 | Wasser zählt nicht zur Festlandfläche (Meer, Seen und Reservoir werden von den 2.200 km² getrennt geführt); Sumpf ist Landnutzungsklasse, nicht Wasserfläche. | 📋 | D19-REQ; D19-CHK; G7-CLOSE | Letzteres Methode in WORLD_700. |

---

## 8. Straßen, Schiene, Verkehrsinfrastruktur (VK)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-VK-001 | Alle Züge bleiben innerhalb der Spielkarte; kein Durchlauf in die Außenwelt. | 📋 | D15-RW | |
| W-VK-002 | Bahnanlagen (Planungsbereiche, keine Bestandslisten): Güterknoten 3–4, regionale Güterterminals 8–10, lokale Ladestellen 18–25, große Depots 4–6, regionale Betriebsdepots 9–13, Hauptwerkstätten 3–4, kleine Abstellanlagen 18–26. | 📋 | D15-RW | „User-approved range“. Bezug auf die Kartengröße 2.200 km² ist nicht dokumentiert. |
| W-VK-003 | Elektrifizierung insgesamt hoch: Haupt- und S-Bahn-Strecken fast vollständig elektrisch, Regionalstrecken überwiegend; abgelegene Zweige Diesel/Batterie. Kein Prozentwert, keine Spannung, kein System. | 📋 | D15-RW | |
| W-VK-004 | Langfristiges Simulationsziel Bahn: nahezu vollständiger Betrieb (Fahrpläne, Signale, Weichen, Stellwerke, Blöcke, Zugsicherung, Bremskurven, Verspätungsfortpflanzung, Rangieren, Stromversorgung, Instandhaltung, Umläufe, Personal, Offscreen-Betrieb); Fahren erfordert einen passenden Job/Berechtigung; NPC-Betreiber halten das Netz unabhängig vom Spieler am Laufen. | 📋 / 🔭 | D15-RW | Entwickler bestätigt als langfristiges Ziel, nichts davon umgesetzt. |
| W-VK-005 | Bedienungsdichten aus früherer Entscheidung: S-Bahn Spitzenintervall 3–5 min, RE 10–15 min, RB 10–20 min, Zweiglinien 15–30 min. Kein Fahrplan, keine Linienanzahl. | 🧭 | D15-RW | „Carried-forward“; Maximalziele. |
| W-VK-006 | Metropolis-ÖPNV (Linie der 70-km²-Planung): 8–10 U-Bahn-Linien, 7–9 S-Bahn-Linien, Straßenbahn und Bus ‚noch mehr‘, stark vernetzter Regional-/Fernverkehr „wie in Europa“; für Reisen nutzbar. | 🧭 | 23#907E; M-W15R | Gegen den 110-km²-Kandidaten nicht neu bestätigt. |
| W-VK-007 | Straßen der Welt: sparsames Makronetz; ländliche Straßendichte soll abnehmen (Verfeinerung: ländliche Straßen um ca. 30–40 % reduzieren, größere Wildnislücken); Bergstraßen folgen Tälern und Pässen. | 🕰️ | 30#232E; 30#241E | Anweisungen an einzelne Kartenläufe (29.09.). |
| W-VK-008 | Strategische Korridore im Prototyp (Autobahn A1 West–Ost über Major City A – Metropolis – Major City B; zweiter Korridor Nord (Pass) – Metropolis – Süd/Südost; Küstenkorridor zur Küstenstadt); Hauptbahn A – Metropolis – B, Güterzweig zur Küste. | 🕰️ | 30#232E | Auftragstext 29.09.; WORLD_700-Netz siehe [Modul 24](24-WORLD700-GIS-Referenz.md). |
| W-VK-009 | Der Zustand von 23 Verkehrskorridoren der 2.200-km²-Testwelt wurde berichtet als 2 PASS / 19 NEEDS_ENGINEERING / 2 FAIL; die beiden nicht freigegebenen Bergzugänge wurden nicht still korrigiert. | 🕰️ | 45#216E | Eingefügter Bericht, nicht überprüft. |
| W-VK-010 | Drei Flughäfen im Prototyp (Metropolis International, West-/Ostregionalflughafen); Flächenrichtwerte 5–8 km² (international), kleiner für regional. | 🕰️ | 30#232E; G7-GPKG | WORLD_700-Flughäfen: 6,4 / 2,4 / 2,7 km² (Hüllgeometrie). |
| W-VK-011 | Fernverkehr: Flughäfen für Passagierflüge (Airliner), Kleinflugzeuge spielerflugbar, Hubschrauber besitzbar, aber meist von Piloten geflogen; Güterverkehr per LKW, Bahn, Hafen. | 📋 | M-WB17 Block 5; 23#1025E Nr. 34 | |
| W-VK-012 | Häfen und Marinas: Boote gehören dem Spieler und liegen an Marinas; Tauchen/Erkunden per Boot. | 📋 | M-WB17 Block 5 | |
| W-VK-013 | Parken, Valet-Service an passenden Orten; Parkplatz- und Park&Ride-Bedarf; Straßensperrungen bei Veranstaltungen. | 📋 | M-WB17 Block 5; 35#562E | |
| W-VK-014 | Reisen im Spiel: Zug und Bus sind für lange Strecken bevorzugt, weil schneller; Radio-Verkehrsmeldungen bei Stau. | 📋 | 23#1025E Nr. 2/36 | |
| W-VK-015 | Straßennetz-Ziel: ländliche Gebiete haben wenig Straßen/Gebäude, Metropolen dichte Netze; Brücken und Tunnel entstehen dort, wo Gelände es verlangt. | 🧭 | D18-MAP; D18-ACC | |
| W-VK-016 | Ob die oberirdische Bahn mit dem eigenständigen unterirdischen Fernnetz verbunden ist, ist offen. | ❓ | D15-OQ | Frage 8. |
| W-VK-017 | Streckenverläufe, Spurweite, Strom- und Signalstandard, Blocklängen, Zugsicherung, Fahrzeugflotten, Bahnsteig- und Stationsinventar sind nicht festgelegt. | ❓ | D15-OQ; D15-RW | |
| W-VK-018 | 09.09. **Flughäfen WORLD_2000**: mindestens 7 (einer pro Staat), davon 1 großer Metropolis International (~12–16 km²), 2 größere, 4 kleinere; evtl. 8–10; dazu 8–15 Flugplätze/Airstrips, Mountain Airfield, Heliports. | 🕰️ | 35#93E; 35#96A; 35#97E; 35#92A | Entwickler (35#93E): ‚Flughafen einer mindestens pro Bundesland, vielleicht auch noch paar Flugplätze zb auch auf dem Berg‘. |
| W-VK-019 | Die Regelung ‚drei Flughäfen‘ des Oktober-Prototyps (Metropolis International, West-/Ost-Regional) ist ein Auftragsrahmen, keine Entscheidung gegen die ≥ 7 Flughäfen von 09.09.; die Quellen lösen den Widerspruch nicht auf. | ⚠️ | 30#265E; 45#62A; 35#96A | [K8](21-Konflikte-und-Widersprueche.md#k8-flughäfen-und-motorsportstandorte). |

---

## 9. Untergrund: Lore und Raum (UG)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-UG-001 | Eine **eigenständige Untergrundzivilisation** existiert, kein bloßer Ausbau der Hunter-Basis; sie lebt in mehreren verborgenen Städten. | 📋 | D14-UC; D15-U; M-AD1018 | |
| W-UG-002 | Die Städte wurden überwiegend vor Jahrtausenden von einer außergewöhnlich fortgeschrittenen Urzivilisation gebaut und über Epochen verändert; die meisten heutigen Bewohner sind Nachfahren. | 📋 | D14-UC; M-AD1015 | |
| W-UG-003 | Einige wenige ursprüngliche Erbauer leben noch, unsterblich, aber schwer bewegungseingeschränkt; Kognition und Anzahl offen. „Unsterblich“ heißt nicht ewig jung oder gesund. | 📋 | D14-UC; M-AD1015 | |
| W-UG-004 | Mehrere Städte sind durch ein unterirdisches Schienen- oder ähnliches Transportsystem verbunden; ob es ein durchgehendes physisches System ist, ist offen. | 📋 / ❓ | D14-UC; D14-OQ | |
| W-UG-005 | Die Städte sind stark gegen die Oberfläche abgeschottet; viele Bewohner wissen wenig oder nichts über die Oberfläche; Abgestufte Wissensgruppen (Bewohner, Verbündete, Forscher, Elite). | 📋 | D14-UC | Zugangsstufen offen. |
| W-UG-006 | Die Bewohner sind gegenüber bekannten Oberflächenbewohnern feindselig/zurückhaltend und lehnen besonders die Elite ab; Zugehörigkeit entsteht individuell über Vertrauen (U24). | 📋 | D14-UC; D15-U | |
| W-UG-007 | Die oberirdische Elite weiß von den Städten, nicht von deren sensibelster Technik, und versucht einzudringen. Die Elite ist ein fiktives Machtnetz einzelner Personen, keine reale ethnische oder religiöse Gruppe. | 📋 | D14-UC | |
| W-UG-008 | Die **größte Untergrundstadt**: Fläche (horizontal) **tentativ 50–100 km²**; ausdrücklich nicht final, keine Geometrie-Sperre; sagt nichts über Ebenen, Bevölkerung oder Höhlenlayout. | 🧭 | M-AD1015; M-AD1018; M-DR1015 D-012 | Am 14.10. noch unbeantwortet (D14-OQ), am 15.10. vom Entwickler bekräftigt. Siehe [Konflikt K9](21-Konflikte-und-Widersprueche.md#k9-größe-der-untergrundstadt). |
| W-UG-009 | Gesamtbevölkerung der Untergrundstadt/Zivilisation: grob mehrere hunderttausend bis ~1 Mio.; nicht bestätigt, Freitext des Entwicklers hat Vorrang vor der Voreinstellung. | 🧭 | D14-CL; M-AD1015; M-AD1018 | Das 30-km²/600.000-Beispiel ist **nicht** angenommen (D14-PS). |
| W-UG-010 | Flächenmaße sind zu trennen: horizontaler Footprint, summierte Nutzfläche über Ebenen, mittlere Dichte je Footprint-km². Beispieltabellen (20–100 km²) sind reine Rechenbeispiele. | 💡 | D14-PS | |
| W-UG-011 | Naturräume erwünscht: unterirdischer Fluss, Seen, Höhlenökosysteme, offene Naturzonen; Herkunft, Entwässerung, Licht, Belüftung und Ökologie ungeklärt; kein bewiesenes perpetuelles Ökosystem erfinden. | 📋 / ❓ | D14-UC; D14-PS | |
| W-UG-012 | Weitere unabhängige oder verbundene Untergrundanlagen anderswo sind geplant; Art und Ort offen. | 📋 / ❓ | D14-UC; D14-OQ | |
| W-UG-013 | Fiktive „Freie Energie“ und Automatisierung tragen die Gesellschaft; ein Mechanismus ist offen. Kein Anspruch auf reale Physik. | 🔭 | D14-UC; M-CA0905 §2 | „Pre-1775 free energy“ wird nicht als reale Geschichte übernommen. |
| W-UG-014 | Der **Hunter-Bunker** ist ein hochgesicherter, autarker Bereich innerhalb oder neben dem größeren Komplex, nicht identisch mit der ganzen Stadt. Hunter und Untergrund verbindet ein altes Bündnis zum gegenseitigen Schutz; Details des historischen Abkommens sind nicht geschrieben. | 📋 | D14-HB; M-AD1015 | |
| W-UG-015 | Nikola Tesla lebt und forscht (überwiegend) im Bunker; Bunkerbewohner wissen es weitgehend und sind zur Verschwiegenheit verpflichtet; Außenstehende nur Spekulation; sehr kleiner geprüfter Zugangskreis. Zugangs- und Sicherheitsdesign offen. | 📋 | M-MS1013; D14-HB; 48#209E; 48#211E; 48#213E | Tesla-Rolle: 48#183E (‚ermordet … Zeitreise … gerettet‘) = Entwicklerwunsch; Bunker mit großem Garten, gegen alles geschützt. |
| W-UG-016 | Der **Familienschlüssel** führt zur Bunkerentdeckung; seine Herkunft, erster Besitzer, Zweck und Verbindung zu den Erbauern sind **für einen eigenen Story-Strang reserviert**; frühere Assistentenvorschläge sind kein Kanon. | 📋 | D14-HB; D14-CL; M-AD1015 | |
| W-UG-017 | Ob der Untergrund-Bunker und das **oberirdische Hunter-HQ** (früher: südliche Waldregion) identisch, getrennt oder verbunden sind, ist offen. | ❓ | M-AD1015 D-014; M-AD1018; D14-OQ | Siehe [Konflikt K10](21-Konflikte-und-Widersprueche.md#k10-bunker-und-hq). |
| W-UG-018 | Die Frage nach Footprint der größten Stadt war am 14.10. die ‚nächste konkrete Planungsfrage‘, Freitext hat Vorrang vor Optionen. | 📋 | D14-OQ | Planungsprozess: eine Frage auf einmal, Freitext immer erlaubt. |
| W-UG-019 | Die Bunkergröße der Serienvorlage (Men-of-Letters-Bunker) wurde als **Orientierung** besprochen (3 Hauptebenen plus Garage; kein offizieller Grundriss); für den Spielbunker gilt das nicht als Entscheidung, und die Namen müssen originaler IP folgen. | 💡 | 43#2A; 43#5A; M-WB89 Block 9 | Eigenes IP-Gebot: nur der Funktion nach analog. |
| W-UG-020 | Im Spiel soll der Bunker nicht fehlen und als echter zusammenhängender Hub gebaut werden (Entwicklerwunsch). | 📋 | 43#4E | |
| W-UG-021 | Vorgeschlagene Zustandsvariablen der Bunker-Story (`key_provenance`, `bunker_discovery_stage`, `hunter_city_treaty_status`, `tesla_identity_exposure`) sind Skizzen, keine Zustandsautomaten. | 💡 | D14-HB | |
| W-UG-022 | Zugang/Geheimhaltung als getrennte Systeme: Stadtgeheimnis, Transportberechtigung, Bezirkszugang, Hunter-Enklave, Tesla-Identität. | 💡 | D14-NS | „proposed, not yet approved“. |
| W-UG-023 | Das Hunter-HQ (oberirdisch) liegt abseits dichter Siedlung, natürlich abgeschirmt, unauffällig, mit kontrollierbarer Anbindung und Platz für unterirdische Einrichtungen; nicht in oder neben einer Stadt. | 📋 / 🧭 | 45#279E; 45#280A | Entwickler: nicht in der Stadt; Rest Assistentenformulierung. Lagebeschreibung dokumentierbar nach W-KA-017. |
| W-UG-024 | 🕰️ Frühere Lage des Hunter-HQ: WORLD_700 Punkt in State E bei (13.500 / 9.509); Prototypauftrag 29.09.: X 28–31, Y 11–14 km im Wald nahe Wasser. | 🕰️ | G7-GPKG; 30#232E | Nicht auf den 2.200-km²-Kandidaten übertragbar. |
| W-UG-025 | Geheime Forschungskomplexe: Wer sie in der Story freigeschaltet hat, weiß, dass es mehrere gibt, und kann sie suchen; sie reichen „von sehr klein bis riesig“, die kleineren eher freundlich, die großen der Elite feindselig. | 📋 | 48#173E; 48#175E | Berührt OF-34 (weitere geheime Anlagen). Zahl, Lage und Größe offen. |
| W-UG-026 | Der Hunter-Bunker „muss noch einzeln geplant werden“, weil er „so super sein soll“; außerhalb davon kann man an beliebigen Orten Fallen (zum Beispiel eine Teufelsfalle) bauen. | 📋 | 48#131E | Bestätigt, dass der Bunker eine eigene Planung erhält (K10, W-UG-017). |

---

## 10. Untergrund: Gesellschaft, Technik, KI (GS)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-GS-001 | Die Gesellschaft funktioniert **ohne konventionelle Regierung und ohne Steuern**; Selbstorganisation; Grundbedürfnisse sind am Nutzungspunkt kostenlos gedacht. | 📋 | D15-U; D16-AI; M-AD1018 | „carried forward“ (vor dem 15.10. entschieden, genaues Datum nicht belegt). |
| W-GS-002 | ❓ Handel, unabhängige Unternehmen, besondere Dienstleistungen und Luxusgüter im Untergrund: **im Master nicht belegt** (kein Treffer in den Untergrund-Dokumenten 14.–19.10.); die Aufgabenstellung von Etappe 3 nennt sie als Orientierung. | ❓ | D14-UC; D15-U; D16-AI; D17-U | Kein Kanon. Als offene Frage geführt ([Modul 22](22-Offene-Fragen-Katalog.md)). |
| W-GS-003 | U24: Zugehörigkeit entsteht durch persönliches Vertrauen; keine automatische globale Aufnahme. | 📋 | D15-U | |
| W-GS-004 | U25: natürliche Mischung aller Familien-/Haushaltsformen. | 📋 | D15-U | |
| W-GS-005 | U26: Lernen nach persönlichem Interesse, starker Praxisbezug, fortgeschrittene Lerntechnik, individuelles Lernen/Mentoring; **kein verpflichtender Fächerkatalog**. | 📋 | D15-U | Freitext des Entwicklers geht vor der Option. |
| W-GS-006 | U27: alltags- und projektbezogenes Lernen mit Spitzentechnik, ohne konventionellen Schulzwang. | 📋 | D15-U | |
| W-GS-007 | U28: starke, aber begrenzte Heilung, Regeneration und Verjüngung; antikes Wissen kann verloren sein. | 📋 | D15-U | |
| W-GS-008 | U29: Lebensdauer variiert stark, von Jahrhunderten bis möglicherweise Jahrtausenden. | 📋 | D15-U | |
| W-GS-009 | U30: verlangsamtes Altern, Regeneration und Verjüngung, individuell verschieden genutzt. | 📋 | D15-U | |
| W-GS-010 | U31: freiwillige Genetik, Organersatz und Verbesserungen; die meisten bleiben relativ natürlich. | 📋 | D15-U | |
| W-GS-011 | U32: Infrastruktur-KI, allgegenwärtige Aufgabenroboter und einzelne hochautonome Systeme koexistieren. | 📋 | D15-U | |
| W-GS-012 | U33: KI hat unterschiedliche Entwicklungsstufen; Bewusstsein ist nicht automatisch belegt. | 📋 | D16-AI | |
| W-GS-013 | U34: individuelle Anerkennung statt einheitlicher KI-Rechte. | 📋 | D16-AI | Autonomie ≠ Bewusstsein ≠ Rechtspersönlichkeit ≠ Rechte. |
| W-GS-014 | U35: unterschiedliche Regeln je nach KI-Art und Aufgabe; keine universelle menschliche Übersteuerung. | 📋 | D17-U | Notaus, Protokolle, Genehmigung offen. |
| W-GS-015 | U36: Kommunikation kombiniert Geräte, holografische/räumliche Systeme, intelligente Umgebungen und **optionale, freiwillige** neurale Schnittstellen; Privatsphäre bleibt zentral. | 📋 | D17-U | |
| W-GS-016 | U37 (interne Netze und Oberflächen-Internet): **gestellt, nicht beantwortet** (OFFEN). Assistentenempfehlungen sind keine Entscheidung. | ❓ | D17-OQ; D17-PRI | Kein Gateway vorab implementieren. |
| W-GS-017 | Privatsphäre: Infrastruktur-KI bedeutet keine flächendeckende Überwachung der Bewohner. | 📋 | D15-U; D16-AI; D17-U | |
| W-GS-018 | Offene Gesellschaftsfragen: Ressourcenverteilung, Konfliktlösung, Finanzierung der Infrastruktur, Rechts- und Streitregeln ohne Staat, Alterspolitik, Medizinzugang, Bildungssicherung. | ❓ | D15-OQ; D14-OQ; D16-REC | Der Master nennt Konfliktlösung und Infrastrukturfinanzierung nicht als beantwortet. |
| W-GS-019 | Engineering-Vorschläge zu getrennten Zustandsgrößen (chronologisches/biologisches Alter, Behandlungen, Interessen, Haushalt, KI-Autonomieklasse) und Fähigkeits-Berechtigungsmodell für KI sind **keine** Entscheidung. | 💡 | D15-U; D17-AUT | |
| W-GS-020 | Ein global synchronisierter Vertrauenswert wird als Anti-Pattern empfohlen, nicht als Regel gesetzt. | 💡 | D15-U | |

---

## 11. Wirtschaft an der Oberfläche (WI)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-WI-001 | Dynamische Wirtschaft: Unternehmen florieren, schrumpfen, schließen, öffnen, werden übernommen; ca. 20–40 große fiktive Konzerne plus lokale Firmen. | 📋 | M-WB17 Block 2 | |
| W-WI-002 | Wirtschaftszweige: Landwirtschaft, Forst, Steinbruch/Bergbau, Lebensmittelverarbeitung, Häfen, Fischerei, Energie, Telekom; mehrere ISPs (Adressen bis ~50 Gbit/s), Internetgeschwindigkeit hat Spielwirkung. | 📋 | M-WB17 Block 2 | |
| W-WI-003 | Firmenjobs sind optionales Geldverdienen, nicht die Hunter-Hauptstory. | 📋 | M-WB17 Block 2 | |
| W-WI-004 | Börse (nur fiktive börsennotierte Firmen), Krypto (10–20 Münzen), Sportwetten (12–18 Buchmacher sharp/soft), Wettbörse, Glücksspiel, Pferderennen: optionale Systeme. | 📋 | M-WB17 Block 3; 23#986E; 23#1013E | |
| W-WI-005 | Jobs: Taxi, Rideshare, Lieferung, Trucking als Geldquellen. | 📋 | M-WB17 Block 5 | |
| W-WI-006 | Bargeld: nur das Geld am Körper ist gefährdet; Fahrzeug, Safe, HQ, Bank nicht automatisch. | 📋 | 23#613E; M-PD §Cash | |
| W-WI-007 | Geld verdienen mit der Adult-Branche soll möglich sein; daneben „casinos, Buchmacher börse aktien“. | 📋 | 35#504E | Bestätigt W-WI-004; Branchenregeln in [Adult/26](../Adult/26-Spezifikation-Weltintegration-Branche-Wirtschaft.md). |

---

## 12. Fahrzeuge (FZ)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-FZ-001 | Fahrzeuge sind ein Kernsystem; der Kofferraum ist die mobile Ausrüstungsbasis des Hunters. | 📋 | M-PD §Vehicles | |
| W-FZ-002 | Breites Fahrzeugspektrum über Jahrzehnte (Autos, Motorräder, ATV/UTV, Schneemobile, Fahrräder, E-Scooter, Lkw, Boote, Kleinflugzeuge, Hubschrauber, Airliner als Passagierverkehr). | 📋 | M-WB17 Block 5 | Keine Modellliste. |
| W-FZ-003 | Gebrauchtwagenmarkt mit Laufleistung, Historie, Verschleiß, versteckten Mängeln und Modifikationen. | 📋 | M-WB17 Block 5 | |
| W-FZ-004 | Das Getriebe gehört zur gekauften Ausstattungsvariante (Modell/Trim/Motor/Getriebe/Antrieb/Baujahr/Optionen); Modellfamilien können manuelle, automatische, DCT-, sequenzielle und Rallye-/Homologationsvarianten haben. | 📋 | M-PD §Vehicles/racing; M-B38 | |
| W-FZ-005 | Standard ist manuell mit Kupplung (Schleifpunkt, Abwürgen); andere Autoversionen kaufbar; automatisch separat erhältlich. | 📋 | 23#613E; 23#1025E Nr. 14 | |
| W-FZ-006 | **Für jedes Fahrzeug** gibt es eine manuelle Variante plus passende Automatiken; optional H-Schaltung als Hardware. | 📋 | D15-RV | „Manual variant for every vehicle“. |
| W-FZ-007 | Reale Kraftstoffe (Benzinsorten, Diesel, E85 mit +50 PS wo Motor/Tune es erlauben, Rennkraftstoffe); E-Auto-Laden; spezielle Werkstätten (Reparatur, Leistung, Rennen, Gelände, Karosserie). | 📋 | M-WB17 Block 5; M-PD §Vehicles/racing; 23#1025E Nr. 11 | |
| W-FZ-008 | Verschleiß: Motor, Öl, Kühlung, Getriebe, Kupplung, Differenzial, Batterie, Bremsen, Reifen, Federung; Leistungstuning wichtiger als kosmetisches Tuning. | 📋 | M-WB17 Block 5; 23#1025E Nr. 13 | „nicht so viel wie echtes Performance-Tuning“. |
| W-FZ-009 | Supersportwagen sind nicht „magisch“ geländetauglich; Geländetauglichkeit unterscheidet sich real. | 📋 | M-PD §Vehicles/racing; M-B38 | |
| W-FZ-010 | Fahrzeug-Vertrautheit ohne magisches Wachstum von Werten; Bergen/Abschleppen/Winde; Fahrzeug-App/Anlieferung per Telefon. | 📋 | M-PD §Vehicles/racing | Vertrautheitsgrenze ❓ (M-OD). |
| W-FZ-011 | Fahrzeugstart/-Interaktion: früh einfache Start-/Zugangsinteraktion mit automatischem Einsteigen; langfristig erweiterte Startprozeduren und tiefere Interaktion. | 📋 | D15-RV | Sitzwechsel bleibt automatisch. |
| W-FZ-012 | Reparatur/Tuning/Werkstatt früh einfach, langfristig nahezu vollständige Wartungs-/Werkstattsimulation. | 📋 | D15-RV | |
| W-FZ-013 | ❓ Realistisches Schadensmodell (Verformung, Antriebs-/Reifenfehler, Reparaturökonomie, bleibender Verschleiß): die Frage nach dem Detailgrad blieb **unbeantwortet**. | ❓ | D15-RV; D15-OQ | |
| W-FZ-014 | ❓ Startsortiment, Marken (fiktive), Modelljahre und Trims; Wartungsökonomie; Obergrenze der Vertrautheit. | ❓ | M-OD §Vehicles | |
| W-FZ-015 | Crash-Energie und Sicherheitsausrüstung verzahnen sich mit dem Verletzungssystem; kein magisches Lenken in der Luft, kein Umdreh-Reset. | 📋 | M-PD §Vehicles/racing | |
| W-FZ-016 | Fahrzeuge sind persistent (stabile Identität, Zustand, am Falltatort abgestellt, Kofferraum-Inventar). | 🧭 | M-PD §World state; M-B38 | Im Prompt-Katalog nur als Entwurfsthemen. |
| W-FZ-017 | Fahrzeuge tragen fiktive Namen: „autos würde ich nicht so nennen wie in echt“ (rechtliche Absicherung); offen bleibt, welche Autos und Marken es gibt („alle autos und marken festlegen“). | 📋 / ❓ | 35#717E; 35#699E | Deckt sich mit fiktiven Marken im Master (W-FZ-014). |
| W-FZ-018 | Vorgehen bei Fahrzeugen (Assistent): Taxonomie, Markenstrategie, Klassen, Epochen, Regionen, Stückzahl, erst dann Modellroster; Beispielzahlen (250 Fahrzeugtypen Metropolis, 120 ländlich) sind Illustration. | 💡 | 35#700A | Keine Zahl ist Entscheidung. |
| W-FZ-019 | Ladeinfrastruktur für Elektrofahrzeuge ist in der Welt zulässig, aber „verbrenner ist die macht im spiel“. | 📋 | 23#1023E (Antwort auf Block-4-Frage 29) | Kein Antriebsanteil festgelegt; siehe W-FZ-014. |

---

## 13. Fahrphysik und Steuerung (FP)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-FP-001 | Manuelles Getriebe simuliert Kupplung, Schalten, Drehzahl, Motorbremse, Kupplungsschlupf und Abwürgen; freie Gangwahl (auch 5→2) mit Folgen für Drehzahl, Motorbremse, Instabilität und Verschleiß je nach Realismuseinstellung. | 📋 | M-PD §Vehicles; M-WB17 Block 5; 23#1025E Nr. 15 | Kupplung deaktivierbar, Schalten bleibt. |
| W-FP-002 | Straßenfahren: **relativ realistische** Kupplung, nicht zu extrem; im Rennsport sehr realistisch. Rennsportphysik realistisch statt Arcade, nicht pay-to-win; iRacing-Perfektion nicht nötig, aber deutlich spürbar. | 📋 | 23#1025E Nr. 50; 23#514E; M-PD | |
| W-FP-003 | Physik nutzt wirklich Antriebsstrang, Reifen, Federung, Lenkung, Differenzial, Bremsen/Sicherheitssysteme, Aero, Last und Schaden; Reifenmodell mit Mischung, Temperatur, Druck, Verschleiß, Nässe. | 📋 | M-PD §Vehicles/racing | |
| W-FP-004 | Früh: sinnvolles manuelles Getriebe mit funktionierender Kupplung; langfristig nahezu vollständige Fahrdynamik. | 📋 | D15-RV | |
| W-FP-005 | DualSense (Entwicklervorgabe): R2 Gas, L2 Bremse, L1+L2 analoge Kupplung, linker Stick Lenkung, rechter Stick Kamera, Kreuz hoch, Quadrat runter, alles frei belegbar. | 📋 | D15-RV | Machbarkeit (L2 doppelt belegt) zu prototypisieren, keine stille Ersatzbelegung. |
| W-FP-006 | Ältere Controller-Konzepte (RT/LT, A/X, B Handbremse, Y; LB-Modifikator mit LT als analoge Kupplung) sowie Modi Automatik, Manuell+Auto-Kupplung, Manuell+digitale Kupplung; Lenkrad/Pedale/H-Schalter als Enthusiasten-Option. | 🧭 | M-PD §Vehicles | Durch D15-RV (DualSense) präzisiert, nicht aufgehoben. |
| W-FP-007 | Kein Vollassist-Zwang; Assist-Einstellungen, HUD-Realismus und Abwürg-Option als Einstellungs-Hooks. | 🧭 | M-PD; Prompt-Entwürfe | |
| W-FP-008 | Motorsport-Physik ist deutlich simulationslastiger als Straßenfahren. | 📋 | M-WB17 Block 5; 23#1025E Nr. 50 | |
| W-FP-009 | Fahrer-NPCs unterscheiden sich in Können und Persönlichkeit; Anfänger können manuelle Autos abwürgen; Wetter ändert Verhalten. | 📋 | M-WB89 Block 8 | |
| W-FP-010 | Motor-, Getriebe-, Kupplungs- und Reifenkonstanten, Schleifpunkt-Algorithmus, Verkehrsmodell, UI und Multiplayer-Synchronisation sind nicht beschlossen. | ❓ | D15-RV; M-OD | |

---

## 14. Motorsport (MO)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-MO-001 | Motorsport hat **sehr hohe Priorität**; Tiefe mindestens wie Fußball/Eishockey, eher mehr (Teams, Hersteller, Fahrer, Qualifying, Reifen, Wetter, Setups, Schaden, Boxenstopps, Entwicklung, Zuverlässigkeit, Strafen, Rekorde, Wetten). | 📋 | M-WB17 Block 7; 23#1035E Nr. 32 | |
| W-MO-002 | Autoserien/-disziplinen (Aufzählung der Anforderungen): Formel-/Top-Rundkurs, GT, Tourenwagen, Sportwagen, Rallye, Bergrennen, Drag, Oval/Stock Car, Dirt Oval, Offroad, Rallye Raid, Kart, Drift, Autocross, Time Attack, Track Days. | 📋 | M-WB17 Block 7 | Anforderungsliste, keine Serienliste mit Namen. |
| W-MO-003 | Motorrad-Rennsport: MotoGP-artige GP-Meisterschaft, Prototyp-Zubringerklassen, Superbike/Supersport, Straßenrennen, Motocross/Supercross, Enduro/Hard Enduro, Rallye Raid, Bergrennen, Drag Bikes, Flat/Dirt Track, Supermoto, Club-/Amateurrennen und Track Days; Spieler kann später teilnehmen; Tiefe vergleichbar mit Auto-Motorsport. | 📋 | M-WB17 Block 7 (+Zusatz) | |
| W-MO-004 | Mindestens **fünf Rennstrecken** und realistische Rennklassen/Homologation; zusätzlich eine F1-Strecke und ein Bergrallye-Anstieg „wie Pikes Peak“. | 📋 | M-PD §Vehicles/racing; 23#511E; 23#822E; 23#823E | 23#823E: ‚eine F1-Strecke und 5 andere Rennstrecken‘; Rennstreckenroster darüber hinaus ❓ (M-OD). |
| W-MO-005 | WORLD_700 verankert sechs Motorsportstandorte (F1-Kurs, Road Circuit, Rally Park, Mountain Hillclimb, Drag Strip, Motorsport Test Park) als Makropositionen. | 🕰️ | G7-GPKG | Gesperrte Referenzwelt; Lage in [Modul 24](24-WORLD700-GIS-Referenz.md). |
| W-MO-006 | Hillclimb-Technik in WORLD_700: 18,9 km, 1.430 m Höhengewinn, ~7,6 % Mittelgefälle, 17 Kehren, Mindestradius 330 m; zwei Gondelabschnitte; Bergzufahrt mit kurzem Tunnel. | 🕰️ | G7-CLOSE (Gondel-/Hillclimb-JSON) | Auf die größere Welt nicht übertragen. |
| W-MO-007 | 2.200-km²-Prototypauftrag verortet Motorsport grob: F1/Straßenkurs im West-Zentrum, Rallye im Gebirge/Wald, Bergrennen im Hauptgebirge, Drag im Süden/Südosten, Testpark im Osten; winzig auf Makromaßstab. | 🕰️ | 30#232E | Auftragstext. Drei Flughäfen und ‚sechs Motorsportsysteme‘ als Anker: 30#265E. |
| W-MO-008 | Ereignisse sperren Straßen (Stadtkurse) und lösen NPC-Besucher, Verkehr, Parken, Wettmärkte aus. | 📋 | 35#562E; M-WB17 Block 7 | |
| W-MO-009 | Autorennen sind „sehr wichtig“; Straßenrennen, Autotreffen, Track Days wichtig. | 📋 | 23#511E; M-WB17 Block 5 | |
| W-MO-010 | Rennwetter (Regen, Gewitter) ist für Motorsport wichtig. | 📋 | M-WB17 Block 6 | |
| W-MO-011 | 09.09. WORLD_2000: ca. **16–22 physische Motorsportstandorte** in Hierarchie (1 GP-Circuit, 2 große + 2 kleine Road/Club-Circuits, Ovale, 2 Drag Strips, 2 Rally Parks, Offroad-Komplex, Hillclimb(s), Motocross/Supercross/Enduro/Flat Track/Supermoto, Drift, Autocross, Time Attack, Test Park, Kartstrecken). Viele Anlagen tragen mehrere Disziplinen. | 🕰️ | 35#96A; 35#97E | Entwickler: „Ja das ist besser“. |
| W-MO-012 | Flächen der sechs Motorsportanlagen (Assistent 09.09.): F1/GP 3–4 km², Road Circuit 2–3, Rally Park 3–4, Hillclimb-Anlage 1–2, Drag Strip 0,5–1, Test Park 2–3; zusammen 12–17 km². | 💡 | 35#92A | Hillclimb nutzt bestehende Bergstraße. |
| W-MO-013 | Oktober: ‚sechs Motorsportsysteme‘ als Ankerelemente der Prototypaufträge; Konflikt mit W-MO-011 ungelöst. | ⚠️ | 30#265E; 45#62A | [K8](21-Konflikte-und-Widersprueche.md#k8-flughäfen-und-motorsportstandorte). |
| W-MO-014 | Hillclimb-Wunsch: eine Schotterstrecke „wie pikespeak“ ohne Absperrungen; wer zu schnell ist, riskiert das Leben („war es das mit dem leben“). Der Assistent skizziert Point-to-Point-Bergstraße ohne Leitplanken, Gefahrenstufen und Nutzung des normalen Schaden-/Verletzungssystems. | 📋 / 💡 | 35#568E; 35#570E; 35#569A | 📋 Wunsch des Entwicklers; Ausgestaltung 💡. Nicht geklärt, ob dies die Pikes-Peak-Bergrallye aus W-MO-004 ist oder eine zweite Strecke. |
| W-MO-015 | Mehrere Rennstrecken, teils „durch den wald mit mehreren etappen“; Stadtkurs mit Absperrungen passt. Assistent: temporäres Event-System (Aufbau über Tage, Umleitungen, Preise und Öffnungszeiten ringsum) statt permanenter Strecke. | 📋 / 💡 | 35#564E; 35#563A; 35#565A | Ergänzt W-UR-010. Eventsystem-Details sind 💡. |
| W-MO-016 | Benannte Rallyes („Northwoods“, „Mountain Crown“ u. a.) und ein „neuer Lock“ zu mehreren Rallyes mit 10–20 Etappen stammen vom Assistenten; der Entwickler bestätigt nur das Prinzip (35#566E, 35#568E). | 💡 | 35#567A; 35#566E | Namen sind Platzhalter, keine Kanonnamen. |
| W-MO-017 | Motorsport gilt am 24.09. als laufender Planungsbereich mit einem „neuesten bestätigten Motorsport-Stand“; dessen Inhalt liegt in Chat 35 nicht vor. | ❓ | 35#705E | Quellenlücke ([Modul 22](22-Offene-Fragen-Katalog.md#6-quellenlücken)). |

---

## 15. NPC-Verkehr und Betriebssimulation (TV)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-TV-001 | Verkehr ist dynamisch, aber nicht absurd: Stoßzeiten, Verkehrsmeldungen im Radio, seltene NPC-Unfälle (außer der Spieler rammt), Pannen, Baustellen. | 📋 | M-WB17 Block 5; 23#1025E Nr. 2/3 | |
| W-TV-002 | NPCs haben Alltags-/Wochenroutinen und nutzen Wohnort, Arbeitsplätze, Geschäfte, Bars, Clubs, Parks, Casinos, Skiorte, Sportstätten und Verkehr. | 📋 | M-WB89 Block 8 | |
| W-TV-003 | Persistenz-Stufen A (Story) bis D (Menge, stark abstrahiert); Wichtige NPCs bleiben dieselben. | 📋 | M-WB89 Block 8 | |
| W-TV-004 | Die Welt läuft nur, wenn man spielt (Offline); im Online-Modus anders. | 📋 | 35#279E | Antwort zu Frage 11. |
| W-TV-005 | NPC-Betreiber halten Bahn- und Verkehrsnetz unabhängig vom Spieler am Laufen (Offscreen-Betrieb). | 📋 | D15-RW | |
| W-TV-006 | Dienste (Feuerwehr, Rettung, Polizei, Abschleppdienst, Bergrettung, Wasserrettung) werden in geeigneter Tiefe simuliert. | 📋 | M-WB89 Block 8 | |
| W-TV-007 | Fahrzeug-Streaming und Offscreen-Auflösung (persistente Zugidentitäten, Fahrplanfortschritt, sichere Zusammenführung an Ladegrenzen) sind ein **technischer Vorschlag**, keine Entscheidung. | 💡 | D15-RW | Zerlegung in sechs Teilgraphen als Engineering-Notiz. |
| W-TV-008 | Performance-Stufen der Offscreen-Simulation und deterministische Resynchronisation von Zügen/Verkehr/NPCs sind offen. | ❓ | D15-OQ | |
| W-TV-009 | Crowd- und Verkehrs-LOD: Rekonstruktion einer glaubwürdigen lokalen Bevölkerung beim Betreten. | 📋 | M-WB89 Block 8 | |
| W-TV-010 | Größenordnung ~500.000 persistente NPC-Identitäten als Planungsannahme („wenn wir 500000 npcs haben“); der Assistent schlägt Simulationsebenen (Tier 0 bis 3), ereignisbasierte Hintergrundsimulation und deterministische Zufälle aus NPC-ID, Welt-Seed und Zeitfenster vor. | 🧭 / 💡 | 35#227E; 35#278A; 35#279E | Auf die Pauschalantwort „12-35 ja“ (35#279E) wird keine Einzelentscheidung gestützt. |
| W-TV-011 | Save-Größe: Der Entwickler hält kleinere Saves für sinnvoll („ja eher kleiner“); der Assistent setzt 50–150 MB, im Extremfall 150–300 MB als Ziel. Im Master gilt langfristig 20–50 MB. | 💡 / ⚠️ | 35#279E; 35#280A; M-PD | [K13](21-Konflikte-und-Widersprueche.md#k13-save-größenziel). |

---

## 16. Technik und Weltgenerierung (TE)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-TE-001 | Unreal Engine + C++20; Unreal nutzt Rendering, World Partition/Streaming, Animation, Audio, UI, Editor, Asset-Pipeline; eigene C++-Systeme für Ermittlung, KI, Weltzustand, Persistenz, Verletzung, Kampf, Fahrzeug, Modding, Tools. | 📋 | M-PD §Technology; M-AD1015 | |
| W-TE-002 | World Partition lädt Laufzeitinhalt; der Weltzustand bestimmt die richtige persistente Variante. | 📋 | M-PD §World state | |
| W-TE-003 | PCG für Vegetation, Steine, Straßenbild und kontrollierte Variation, **nicht** als Ersatz für authored wichtige Orte. | 📋 | M-PD §World | |
| W-TE-004 | Stabile IDs für wichtige persistente Entitäten; Saves segmentiert, inkrementell, absturzsicher, migrierbar, komprimiert; Ziel ~20–50 MB langfristig, Korrektheit vor Größe. | 📋 | M-PD §World state | |
| W-TE-005 | Performance-Budgets (CPU, GPU, VRAM, RAM, Streaming, KI, Physik, Animation, Audio, Simulation) werden gemeinsam und von Anfang an geplant; genaue Hardwarestufen und Frame-Budgets sind offen. | 📋 / ❓ | M-PD §Rendering | |
| W-TE-006 | Lokales Koordinatensystem **OWP-LCS-1** (fiktiv, kartesisch, Meter; Ursprung Südwest; X Ost, Y Nord) existiert für WORLD_700; Pixel-Weltumrechnung mit 2,7 m/px; GCPs aus der gesperrten Karte. | 🕰️ | G7-LCS; G7-GPKG | Nur WORLD_700. Für die größere Welt gibt es **kein** festgelegtes CRS (D19-OQ Nr. 6). |
| W-TE-007 | Es existiert ein GeoPackage (Schema 1.3, 495 Features, stabile IDs) mit Verwaltung, Stadt, Verkehr, Hydrologie, Gelände, POI für WORLD_700; ein 10,8-m-Planungs-DEM und D8-Wassereinzugsgebiete. | 🕰️ | G7-CLOSE; G7-GPKG | Vorhandene GIS-Daten **der gesperrten Referenzwelt**. |
| W-TE-008 | WORLD_700-Kartenproduktion: 11 Regionalkarten (Phase A.2) mit festgelegten Auflösungsklassen, vom Entwickler als `APPROVED_LOCKED` bestätigt. | 🕰️ | 30#194E; G7-PA | Zahlen: 24K/20K, ~0,45–0,50 m/px effektiv. |
| W-TE-009 | Die 2.200-km²-Testwelt besaß ein 50-m-Höhenraster (1.348 × 680 px), Landnutzung, Stadtpolygone, Seen, Flüsse, Straßen-/Bahnkorridore (berichtet); Seespiegel-Konflikt Lake Alder 460 m vs. 459 m. | 🕰️ | 45#210E; 45#253E | Eingefügte Berichte; nicht akzeptiert. |
| W-TE-010 | Eine reale Terrain-Forschungsreihe (USGS-Referenzraster, D8, Priority-Flood) lieferte Prüfstände, aber keinen akzeptierten Terrain-Kandidaten; „NEITHER“ bei den zwei verglichenen Architekturen. | 🕰️ | 45#136E bis 45#253E | Berichtet. |
| W-TE-011 | Vorgeschlagene Architektur: handkuratierter, editierbarer GIS-Kontrollgraph (Kämme, Täler, Seen, Höhen, Abflüsse) statt reiner Terrain-Synthese. | 💡 | 45#253E; 45#257E | Berichtet/vorgeschlagen; V1.3e als Blueprint mit 18 Landschaftsregionen. |
| W-TE-012 | Keine vollständige Neugenerierung aller 2.200 km² in 16K auf einmal; kontrollierte Verbesserung der bestehenden Welt und reproduzierbare Pipeline für Master- und Regionalkarten. | 📋 | M-MS1013; 54#104E | |
| W-TE-013 | Open-World-Simulationskern (Welt, NPCs, Wirtschaft, Fahrzeuge, Verkehr, Wetter, Immobilien, Innenräume, Bau, Sport, Polizei, Internet, Persistenz) soll IP-neutral wiederverwendbar bleiben. | 📋 | M-LATE | Unterstützt Architekturentscheidungen der Engine-Schicht. |
| W-TE-014 | Phase-Ziele: Phase 0 Kerntechnik, Phase 1 Vertical Slice ca. 1–3 km² und 30–60 min; später 5–10 km², 25–50 km². | 🧭 | M-MS1013; M-AD1015 | Zielwerte, keine Sperren. |

---

## 17. Reiche, Zeit, Aktivitäten (RE, AK)

| ID | Aussage | Status | Quelle | Anmerkung |
|---|---|:---:|---|---|
| W-RE-001 | Mindestens drei Reiche im Basisspiel: Hölle, Fegefeuer, Himmel; jeweils eigene offene Welt mit eigener Weltbibel, Ökologie, Architektur, Fortbewegung, Gefahren, Zeitregeln, Persistenz und späterem Weltmaster. | 📋 | M-CA0906; M-CU0908; M-WB89 Block 9 | |
| W-RE-002 | Größenrahmen (nicht gesperrt): Hölle ~350–500 km² (Favorit ~420), Fegefeuer ~500–750 km² (~650), Himmel ~250–400 km² (~320) plus Erinnerungs-/Taschendomänen. | 🧭 | M-CA0906; M-CU0908 | Planungshypothesen. |
| W-RE-003 | Zutritt zu Reichen selten (Story, Ritual, Portal, Tod, Sonderereignisse), nicht über normales Schnellreisen; Änderungen persistieren. | 📋 | M-WB89 Block 9; M-CA0906 | |
| W-RE-004 | Zeitreise: jahrgenau ab etwa 1600 bis heute als Planungsanker; früher möglich, spärlicher; historische Weltzustände aus zeitversionierten Schichten, nicht aus einer Vollkarte je Jahr. | 📋 | M-CA0905 §2 | Zukunftsreisen und verzweigte Zeitlinien offen/spät. |
| W-RE-005 | Hölle-Zeitverhältnis: **40 Jahre in der Hölle entsprechen ungefähr vier Monaten auf der Erde**; die Zeit vergeht in der Hölle wesentlich schneller als auf der Erde. Das Fegefeuer-Zeitverhältnis bleibt offen. | 📋 / ❓ | PE-E1022; 35#41E (Fragen 137–146); 23#458A | Der Projektinhaber hat am 22.10.2026 die Zeitrichtung ausdrücklich geklärt (früher: Satz in 35#41E ohne Richtung, 🧭). Das Master-Dokument M-CA0906 und M-WB89 führen das Verhältnis noch als offen; die Primärentscheidung hat Vorrang. Offen bleiben Synchronisierung, Zeitmessung und Übergänge zwischen Reichen (siehe W-RE-011). |
| W-RE-006 | Zeitreise ins Jahr 1600: gleich bleiben nur Seen, Berge und Flüsse; die Städte sind ‚wahrscheinlich komplett weg‘. Geografie ist die stabile Schicht, Siedlungen sind zeitversioniert. | 📋 | 35#41E (Frage 125) | Ergänzt M-CA0905 §2; hat Folgen für Geografie-Konsistenz ([Modul 17](17-Weltgenerierung-und-GIS-Daten.md)). |
| W-RE-007 | Handlungen in der Vergangenheit können die Gegenwart verändern: ‚theoretisch ja, aber ich würde es nicht übertreiben‘. | 📋 | 35#41E (Frage 128) | |
| W-RE-008 | 26.08.: Tod als übernatürlicher Ablauf: Seele trennt sich, Ziel Himmel, Hölle oder Geist; der Normalfall ist, dass die Seele weitergeht, Geister entstehen nur unter besonderen Umständen. Für NPCs und kanonische Story-Tode gilt das System; normale Gameplay-Tode des Hauptcharakters sind nicht kanonisch (Checkpoint). Das Ziel hängt von Leben und Handlungen ab, Sonderregeln (Vertrag, Fluch, Eingriff) können es überschreiben; kein sichtbarer Karma-Wert. | 🧭 | 23#458A; 23#459E; 23#460A; 23#461E | Entwickler antwortet punktweise „ja/genau“ und stellt klar, dass Punkt 1 NPCs betrifft („für npcs passt es“). Der Assistent führt das als „fest“; wegen der Kurzantworten 🧭. |
| W-RE-009 | Himmel: „in der serie hat jeder mensch seine eigenen himmel“; der Himmel soll individuell und teilweise subjektiv sein, mit Erinnerungsräumen, nicht eine Wolkenstadt; eigene Interpretation statt Kopie der Serie. Hölle: stärker physisch/feindlich, mit eigener Gesellschaft (Hierarchien, Deals, neutrale Figuren), nicht nur Feuer und Lava. | 📋 / 💡 | 23#459E; 23#458A; 23#460A | 📋 nur der Entwicklersatz zu persönlichen Himmeln; Ausgestaltung 💡. Ergänzt [Modul 26](26-Reiche-Zeitreise-und-historische-Weltzustaende.md). |
| W-RE-010 | Hölle ist eine echte, kleinere Open-World-Ebene, frei erkundbar: „es ist openworld und hat auch collectibles und anderes“. Erste Reise storygebunden, kleiner Zugangsbereich, freie Reisen erst nach Entdeckung von Ritual, Ort und seltenen Zutaten (ohne Grinding); Hin- und Rückreise können verschiedene Regeln haben; Begleiter wählbar („ja genau“). Reisen kostet Erdzeit, aber „viel weniger“ als im Verhältnis der Hölle. | 📋 / 🧭 | 23#459E; 23#458A | Details von 23#458A (Punkte 8 bis 14, 22, 23, A, B) sind Assistentenentwurf, die Antworten Kurzzustimmungen. Der Satz „halt viel weniger“ bezieht sich auf Punkt 22 (Zeitkosten). |
| W-RE-011 | Höllenzeit: ungefähr **40 Jahre Hölle ≈ 4 Monate Erde** (Rechnung ≈ 120:1, nur als Arithmetik der Aussage); ein verstorbener Körper altert dabei nicht: „die person ist tot gewesen also kann sie nicht altern“. | 📋 | PE-E1022; 23#458A; 23#459E; 35#41E | Bestätigt am 22.10.2026 durch den Projektinhaber (Zeitrichtung ausdrücklich geklärt); die Zeitrichtung ist nicht mehr 🧭. Die Relation ist ungefähr; **offene technische Einzelheiten**: Synchronisierung, Zeitmessung, Übergänge zwischen den Reichen, Online-Behandlung. Keine zusätzlichen Zeitreise-Regeln festgelegt. |
| W-RE-012 | Tod in anderen Ebenen: Der Assistent schlägt vor, dass normaler Gameplay-Tod auch dort zu einem Checkpoint führt; der Entwickler: „ok, vielleicht aber noch anders“. Der Assistent notiert das ausdrücklich als nicht entschieden. | ❓ | 23#458A; 23#459E; 23#460A | Offene Frage OF-65. |
| W-RE-013 | DLC können Ebenen erweitern (neue Regionen, neue Ebenen, neue Wesen, neue Reisemethoden): „ja das ist fix“; Online-Ebenenreisen nur für die Party instanziert, „ja passt“. | 📋 / 💡 | 23#458A; 23#459E | 📋 nur die DLC-Aussage; Instanzierung 💡. |
| W-RE-014 | Fegefeuer: eigene, deutlich kleinere Open World, Welt der Monsterseelen (Vampire, Werwölfe, Gestaltwandler u. a.), extrem gefährlich, aber nicht „alle 20 Meter ein Gegner“; Wiederbegegnung mit früher getöteten Monstern über das Creature Memory System; seltene Materialien „finden und verstehen“ statt farmen („ja das soll im ganzen spiel nicht so sein“); Zeitverhältnis nicht festgelegt, vermutlich weniger extrem als die Hölle. | 🧭 / 💡 | 23#465A; 23#466E; 23#467A | Der Assistent nennt den Fegefeuerblock „im Wesentlichen bestätigt“ (23#467A); Entwickler antwortet 30 Punkte mit Kurzzustimmungen. Größenrahmen siehe W-RE-002. |
| W-RE-015 | Leviathane: uralt, intelligent, eng mit dem Fegefeuer verbunden, an die Serienvorlage angelehnt (Entwickler); gewöhnliche Waffen verletzen sie, töten aber nicht („man kann sie mit gewöhnlichen waffen auch nicht töten“); sie sollen „nicht von anfang an auf der welt sind“; Dämonen werden später besprochen. | 📋 | 23#466E; 23#468E; 23#467A | 📋 durch Entwicklersätze; Biologie, Optik, Hierarchie sind offen (Assistentenentwurf 23#467A). |
| W-AK-001 | Aktivitätstiefe in Stufen A (Fahrzeuge, Motorsport, Sportwetten, Glücksspiel, Angeln, Jagd, Tauchen, Wintersport, Hunter-Erkundung), B und C; Tiefe darf über die Zeit wachsen. | 📋 | M-WB17 Block 7 | |
| W-AK-002 | Räumliche Zuordnung (Spatial Allocation) der Aktivitäten, Kleinparks, Stadien, Strecken, Resorts steht aus und ist Voraussetzung der finalen Karte. | ❓ | M-W15R; M-CA0906 | Siehe [Modul 08](08-Raeumliche-Allokation-Orte-und-Aktivitaeten.md). |
| W-AK-003 | Sammelobjekte/Entdeckungen werden nicht linear mit der Fläche skaliert, sondern je Kategorie nach der Zuordnung festgelegt; 1.200–1.500 physische Sammelobjekte gehörten zur alten ~200-km²-Idee (gesamt ~5.000 Entdeckungsobjekte). | 📋 | M-LATE; M-CA0906 | |
| W-AK-004 | 09.09. Inhaltsmengen WORLD_2000 (Arbeitsrichtung): pro Staat mindestens ein Sports Hub; 2–4 Ski-/Snowboardgebiete; 5–10 größere Marinas; 1 sehr großer Themenpark + 1 regionaler; 1 großer Wasserpark + 2–4 kleine; mehrere Golfplätze; Mega-POIs ×1,5–2, Städte/Sport/Freizeit ×2–3 gegenüber WORLD_700, Alltags-POIs stärker. | 🕰️ | 35#96A; 35#97E | Entwickler: „Ja das ist besser“. |
| W-AK-005 | Nicht urban ≠ leer: Wildnisflächen enthalten Straßen, Forstwege, Hütten, Tankstellen, Motels, Minen, Camps, Ranger-Stationen, Höhlen, Lost Places, Wildlife, Monsterterritorien, Fälle. | 📋 | 35#87E (Frage 4); 35#88A; 35#96A | Entwickler: ‚soll auch nicht leer wirken, sondern nur abgelegen wie in real life‘. |
| W-AK-006 | Collectibles werden bei mehr Welt vermehrt (Straßen, besondere Gebäude als eigene Entdeckungsebenen). | 📋 | 23#1077E; M-LATE | 23#1077E: ‚da die map so groß geworden ist, sollten es noch mehr collectibles geben‘. |

---

## 18. Kennzahlen des Registers

Die Zahlen werden per Skript aus diesem Modul gezählt ([`Tools/world-audit/check_docs.py`](../../../Tools/world-audit/check_docs.py)); die gemessenen Werte stehen in [Modul 23](23-Quellenabdeckung-und-Auditbericht.md#7-kennzahlen).
