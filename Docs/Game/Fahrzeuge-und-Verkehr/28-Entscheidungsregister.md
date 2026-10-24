# Fahrzeuge und Verkehr 28: Entscheidungsregister

> [!IMPORTANT]
> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 BESCHLOSSEN ist vom Entwickler festgelegt oder im Master ohne Vorbehalt geführt; 🧭, 🧩, 💡, ❓, ⚠️ und 🔍 sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuswörter: [Legende](README.md#statuslegende).
> **Dieses Dokument ist erzeugt** (`Tools/world-audit/render_vehicle_register.py`) aus den Tabellen der Module 02 bis 27. Änderungen gehören in die Module, nicht hierher.

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Konflikte und Fragen: [Modul 29](29-Konflikte-und-offene-Fragen.md).

**Inhalt**

1. [GP: Vision und Grundprinzipien](#gp-vision-und-grundprinzipien)
2. [KL: Fahrzeugklassen und Fahrzeugdaten](#kl-fahrzeugklassen-und-fahrzeugdaten)
3. [PH: Fahrphysik und Fahrverhalten](#ph-fahrphysik-und-fahrverhalten)
4. [ST: Steuerung, Kamera und Anzeige](#st-steuerung-kamera-und-anzeige)
5. [EI: Einsteigen, Aussteigen, Sitze, Kofferraum](#ei-einsteigen-aussteigen-sitze-kofferraum)
6. [ZV: Zustand, Verschleiß, Schaden](#zv-zustand-verschleiß-schaden)
7. [WE: Reparatur, Wartung, Werkstätten](#we-reparatur-wartung-werkstätten)
8. [KE: Kraftstoff, Energie, Tankstellen](#ke-kraftstoff-energie-tankstellen)
9. [SN: Straßennetz und Straßentypen](#sn-straßennetz-und-straßentypen)
10. [KR: Kreuzungen, Signale, Schilder](#kr-kreuzungen-signale-schilder)
11. [SP: Stadtverkehr, Parken, Garagen](#sp-stadtverkehr-parken-garagen)
12. [KI: Verkehrs-KI und Navigation](#ki-verkehrs-ki-und-navigation)
13. [NF: NPC-Fahrer und NPC-Fahrzeuge](#nf-npc-fahrer-und-npc-fahrzeuge)
14. [VD: Dichte, Stau, Baustellen, Sperrungen](#vd-dichte-stau-baustellen-sperrungen)
15. [UP: Unfälle, Pannen, Einsatz- und Sonderverkehr](#up-unfälle-pannen-einsatz--und-sonderverkehr)
16. [VT: Fußgänger, Radfahrer, weitere Teilnehmer](#vt-fußgänger-radfahrer-weitere-teilnehmer)
17. [BZ: Besitz, Zulassung, Diebstahl](#bz-besitz-zulassung-diebstahl)
18. [PV: Polizei und Verkehrsüberwachung](#pv-polizei-und-verkehrsüberwachung)
19. [IT: Interaktionen, Transport, Jobs](#it-interaktionen-transport-jobs)
20. [EJ: Fahrzeuge in Ermittlungen und Jagden](#ej-fahrzeuge-in-ermittlungen-und-jagden)
21. [PE: Persistenz und Lebenszyklen](#pe-persistenz-und-lebenszyklen)
22. [KO: Koop und Netzwerk](#ko-koop-und-netzwerk)
23. [WF: WorldForge-Schnittstelle](#wf-worldforge-schnittstelle)
24. [UE: Unreal-Integration](#ue-unreal-integration)
25. [LS: Simulationsstufen, LOD, Streaming](#ls-simulationsstufen-lod-streaming)
26. [PF: Performance und Skalierung](#pf-performance-und-skalierung)

---

## GP: Vision und Grundprinzipien

Gruppe zu [Modul 02](02-Vision-und-Grundprinzipien.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-GP-001 | Fahren ist ein Kernsystem und eine frühe Ambition für den Vertical Slice; das Fahrzeug ist mobile Ausrüstungsbasis | 📋 BESCHLOSSEN | `W-FZ-001`; Gameplay-Systeme §6.1 | FZ INV | – |
| VEH-DEC-GP-002 | Phasenvertrag: früh sinnvolles Schaltgetriebe mit funktionierender Kupplung, einfacher Zugang, einfache Reparatur; langfristig nahezu vollständige Fahrdynamik, tiefere Interaktion, nahezu vollständige Werkstattsimulation | 📋 BESCHLOSSEN | `W-FP-004`, `W-FZ-011`, `W-FZ-012`; `D15-RV` | FZ | Termine, Motormodell und Wiedergabetreue-Maßstäbe sind nicht beschlossen |
| VEH-DEC-GP-003 | Straßenfahren ist relativ realistisch, Motorsport deutlich simulationslastiger | 📋 BESCHLOSSEN | `W-FP-002`, `W-FP-008` | FZ | Zwei Wiedergabetreue-Stufen derselben Physik: 💡 `VEH-DEC-PH-002` |
| VEH-DEC-GP-004 | Verkehr ist dynamisch, aber nicht absurd; Stoßzeiten, Radiomeldungen bei Stau, seltene NPC-Unfälle (außer der Spieler rammt), Pannen, Baustellen | 📋 BESCHLOSSEN | `W-TV-001` | VKR | Keine Dichte beschlossen |
| VEH-DEC-GP-005 | Die Welt wird nur so tief simuliert, dass kein offensichtlicher Fake auffällt; glaubwürdige Konsistenz geht vor vollständiger Simulation | 📋 BESCHLOSSEN | `N-LT-011` | ALL | Leitlinie für Stufen in Modul 26 |
| VEH-DEC-GP-006 | Verkehr wird beim Betreten eines Gebiets lokal rekonstruiert, die Straße darf weder leer noch unglaubwürdig sein | 📋 BESCHLOSSEN | `W-TV-009`, `N-LE-013`, `N-LE-026` | VKR NPC | Rekonstruktionsverfahren: 💡 [Modul 26](26-Simulationsstufen-LOD-und-Streaming.md) |
| VEH-DEC-GP-007 | Die Welt läuft im Einzelspielerbetrieb nur, während man spielt | 📋 BESCHLOSSEN | `W-TV-004`, `N-LT-020` | VKR | Online-Verhalten später anders |
| VEH-DEC-GP-008 | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI; ein bewusst herbeigeführter Tod ist etwas anderes | 📋 BESCHLOSSEN | `N-LT-033` | NPC VKR | Konflikt mit unabhängiger Lebensentwicklung: `CK-02` |
| VEH-DEC-GP-009 | Verbrenner dominieren das Spiel; E-Ladeinfrastruktur ist zulässig | 📋 BESCHLOSSEN | `W-FZ-019` | FZ | Antriebsanteil nicht festgelegt (`W-FZ-014`) |
| VEH-DEC-GP-010 | Lange Fahrten sind ein Charakterkanal (Gespräche, Radio, Telefon, Schlaf, Streit, Fallbesprechung, spontane Stopps) | 📋 BESCHLOSSEN | Gameplay-Systeme §6.1; `M-PD` | DLG FZ | Dialogsystem: Nachbardokumentation |
| VEH-DEC-GP-011 | Die Weltgröße ist nirgends fest verdrahtet; das Fahrzeug- und Verkehrssystem skaliert mit Netz und Siedlung | 📋 BESCHLOSSEN | `N-LT-007`; `M-PD` ‚Final map size … calibrated‘ | ALL | Kalibrierung der Weltgröße an Fahrzeiten: `W-GE-026` |
| VEH-DEC-GP-012 | Zug und Bus sind für lange Strecken bevorzugt; Radio-Verkehrsmeldungen bei Stau | 📋 BESCHLOSSEN | `W-VK-014` | VKR OPV | – |
| VEH-DEC-GP-013 | Verkehr ist Teil der NPC-Gesellschaft: NPC-Fahrzeuge gehören NPC-Haushalten und Firmen, es gibt keine eigenständige Verkehrspopulation | 🧩 ABGELEITET | `N-ID-005`, `N-ID-007`, `N-LE-006`, `N-LT-008` | NPC VKR | Begründung und Ausnahmen (Statisten) in Modul 14 |
| VEH-DEC-GP-014 | Verhalten wird über austauschbare Datenprofile je Region und Straßenklasse beschrieben, nicht über feste Konstanten | 💡 VORSCHLAG | `CLAUDE.md` §4; `WF-RG` | ALL | Profilformat offen: `CQ-01` |
| VEH-DEC-GP-015 | Besitzer, Zustand, Standort und Rechtslage eines Fahrzeugs haben je genau einen Eigentümer; Änderungen laufen über Ereignisse | 💡 VORSCHLAG | Folge aus `N-LT-008` | FZ PER | Eigentümerliste in Modul 22 |
| VEH-DEC-GP-016 | Wie viele Wiedergabetreue-Stufen und Assistenzoptionen der Straßenverkehr bietet, ist nicht beschlossen | ❓ OFFEN | `W-FP-007` (🧭 Hooks); `W-FP-010` | FZ UI | `CQ-02` |
| VEH-DEC-GP-017 | Schnellreise (Camps, Safehouses) und der Umgang mit langer Fahrzeit sind nicht entschieden | ❓ OFFEN | Welt [OF-50](../World/22-Offene-Fragen-Katalog.md); `35#87E` (Richtung ‚Durchquerung merkbar lang‘) | FZ WLT | `CQ-03` |
| VEH-DEC-GP-018 | Lange Fahrten sind nicht automatisch tote Zeit: Mitfahrende sprechen, reagieren auf das Radio, telefonieren, schlafen, streiten, besprechen den Fall, bitten um Stopps oder schweigen; Beziehungen können sich entwickeln, ohne dass ständige Minispiele nötig sind | 📋 BESCHLOSSEN | `M-DC31` §47 | DLG FZ NPC | Dialogsystem: Nachbardokumentation |
| VEH-DEC-GP-019 | Der Status der Oktober-15-Fahrzeugdetails ist uneinheitlich: `D15-RV` führt den Phasenvertrag und die DualSense-Belegung als ‚user-approved‘ beziehungsweise ‚user specified‘, der Zusatz vom 18.10.2026 führt die Fahrzeugdetails des Oktober-Pakets als ‚review-required specification inputs‘ | ⚠️ KONFLIKT | `D15-RV`; `M-EP1018`; `M-OC1018` | FZ | `CK-07`; betrifft GP-002, ST-001 bis ST-003, WE-001 |

---

## KL: Fahrzeugklassen und Fahrzeugdaten

Gruppe zu [Modul 03](03-Fahrzeugklassen-und-Fahrzeugdaten.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-KL-001 | Breites Fahrzeugspektrum über Jahrzehnte (Autos, Motorräder, ATV/UTV, Schneemobile, Fahrräder, E-Scooter, Lkw, Boote, Kleinflugzeuge, Hubschrauber, Airliner als Passagierverkehr) | 📋 BESCHLOSSEN | `W-FZ-002` | FZ | Keine Modellliste |
| VEH-DEC-KL-002 | Fahrzeugidentität aus Modell, Ausstattung, Motor, Getriebe, Antrieb, Baujahr und Optionen; Modellfamilien können Hand-, Automatik-, Doppelkupplungs-, sequenzielle und Rallye-/Homologationsvarianten haben | 📋 BESCHLOSSEN | `W-FZ-004` | FZ | – |
| VEH-DEC-KL-003 | Fahrzeuge tragen fiktive Namen und Marken | 📋 BESCHLOSSEN | `W-FZ-017` | FZ REC | Welche Autos und Marken es gibt, ist offen (`CQ-06`) |
| VEH-DEC-KL-004 | Gebrauchtwagenmarkt mit Laufleistung, Historie, Verschleiß, versteckten Mängeln und Modifikationen | 📋 BESCHLOSSEN | `W-FZ-003` | FZ WIR | Umsetzung in Modul 07 |
| VEH-DEC-KL-005 | Supersportwagen sind nicht magisch geländetauglich; Geländetauglichkeit unterscheidet sich real | 📋 BESCHLOSSEN | `W-FZ-009` | FZ PHY | Auswirkung auf zugelassene Straßenklassen: 💡 `VEH-DEC-KL-009` |
| VEH-DEC-KL-006 | Manuelles Getriebe ist Standard, andere Versionen sind kaufbar; für jedes Fahrzeug existiert eine manuelle Variante | 📋 BESCHLOSSEN | `W-FZ-005`, `W-FZ-006` | FZ | Verhältnis zu Fahrzeugen ohne Schaltgetriebe: `CU-01` |
| VEH-DEC-KL-007 | Startsortiment, Marken, Modelljahre, Trims, Wartungsökonomie und Vertrautheitsgrenze sind nicht festgelegt | ❓ OFFEN | `W-FZ-014`; `M-OD` ‚Vehicles‘ | FZ | `CQ-06` |
| VEH-DEC-KL-008 | Neben der Fahrzeugklasse wird eine **Verkehrsrolle** geführt (Privat, Lieferung, Taxi, Bus, Einsatz, Wartung, Rennen, Spieler), die Straßenzugang, Sonderrechte und Verhalten bestimmt | 💡 VORSCHLAG | Ableitung aus `W-TV-006`, `W-WI-005`, `N-LE-024` | FZ VKR | Liste und Zuschnitt offen: `CQ-09` |
| VEH-DEC-KL-009 | Der Fahrzeugtyp trägt eine Liste erlaubter Straßenklassen, damit Geländetauglichkeit und Verkehrsrolle im Netz abbildbar sind | 💡 VORSCHLAG | Ableitung aus `W-FZ-009`, `WF-D03` (Klassen) | FZ SN | – |
| VEH-DEC-KL-010 | Typ (statisch, versioniert, austauschbar) und Exemplar (veränderlich, persistent) werden getrennt geführt | 💡 VORSCHLAG | `W-FZ-016`; `M-MOD` | FZ PER | Felder und Format offen |
| VEH-DEC-KL-011 | Der Fahrzeugtyp trägt ein Auffälligkeitsmerkmal, das die Wiedererkennung durch NPCs und Zeugen speist | 💡 VORSCHLAG | `N-GE-018`; `E-SP-011` | NPC ERM | Schwellen offen |
| VEH-DEC-KL-012 | Die Anzahl der Fahrzeugtypen ist nicht beschlossen; das Beispiel 250/120 aus dem Assistentenvorschlag ist Illustration | 💡 VORSCHLAG | `W-FZ-018` | FZ | – |
| VEH-DEC-KL-013 | Antriebsarten jenseits Verbrenner und Elektro (Hybrid, Wasserstoff) sind nicht belegt | 🔍 UNBELEGT | `W-FZ-019` nennt nur Verbrenner und E-Laden | FZ | `CQ-07` |
| VEH-DEC-KL-014 | Spezialfahrzeuge der Hunter sowie Rettungs- und Feuerwehrfahrzeugtypen sind nicht belegt; Polizei- und Taxi/Fleet-Varianten nennt der Master als Trim-Familien | 🔍 UNBELEGT | [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §9; `M-PS31` §40 (Familien) | FZ | Verkehrsrolle bleibt davon unberührt; Trim-Familien: `VEH-DEC-KL-017` |
| VEH-DEC-KL-015 | Luft-, Wasser- und Schienenfahrzeuge sind Besitz- und Persistenzfälle dieses Ordners, ihre Betriebsregeln liegen in der Welt-Dokumentation | 💡 VORSCHLAG | `W-VK-004`, `W-VK-011`, `W-VK-012` | FZ PER | – |
| VEH-DEC-KL-016 | Das Getriebe gehört zur konkreten Fahrzeugvariante: Man kauft nicht ein Fahrzeug und schaltet es beliebig per Menü zwischen Handschaltung, Doppelkupplung und Automatik um; Handschaltung, Wandlerautomatik, Doppelkupplung, sequenzielle und weitere Getriebearten können nebeneinander bestehen | 📋 BESCHLOSSEN | `M-DC31` §30; `M-PS31` §39; `W-FZ-004` | FZ | Ergänzt `VEH-DEC-KL-006` |
| VEH-DEC-KL-017 | Fahrzeugfamilien können Economy, Sport, GT, Track Edition, Rally Homologation, Rally Version, Off-Road, Police, Taxi/Fleet, Utility, Luxury, Performance, Limited und Modelljahrvarianten enthalten; auch Kleinwagen können echte Rallye- und Homologationsversionen mit anderem Antrieb, Bremsen, Fahrwerk, Übersetzung, Verstärkung und Aero haben | 📋 BESCHLOSSEN | `M-DC31` §31; `M-PS31` §40 | FZ RAC | Liste nicht abschließend (‚können‘) |

---

## PH: Fahrphysik und Fahrverhalten

Gruppe zu [Modul 04](04-Fahrphysik-und-Fahrverhalten.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-PH-001 | Manuelles Getriebe simuliert Kupplung, Schalten, Drehzahl, Motorbremse, Schlupf und Abwürgen; freie Gangwahl mit Folgen | 📋 BESCHLOSSEN | `W-FP-001` | FZ | Konstanten offen (`VEH-DEC-PH-013`) |
| VEH-DEC-PH-002 | Straßenfahren und Motorsport benutzen zwei Wiedergabetreue-Stufen derselben Fahrphysik, die über Profile unterschieden werden | 💡 VORSCHLAG | Ableitung aus `W-FP-002`, `W-FP-008` | FZ | Stufenzahl: `CQ-02` |
| VEH-DEC-PH-003 | Die Physik nutzt tatsächlich Antriebsstrang, Reifen, Federung, Lenkung, Differenzial, Bremsen und Sicherheitssysteme, Aerodynamik, Last und Schaden; das Reifenmodell kennt Mischung, Temperatur, Druck, Verschleiß und Nässe | 📋 BESCHLOSSEN | `W-FP-003` | FZ | Tiefe im Straßenverkehr: `CQ-12` |
| VEH-DEC-PH-004 | Fehlschaltungen und Missbrauch (zum Beispiel Herunterschalten bei hoher Geschwindigkeit) können Kupplung, Getriebe, Antriebsstrang und Motor beschädigen und Kontrollverlust verursachen | 📋 BESCHLOSSEN | `23#1025E` Nr. 15; `M-WB17` Block 5 | FZ ZV | Schwellen offen |
| VEH-DEC-PH-005 | Die Kupplung ist deaktivierbar, das Schalten bleibt; eine Kupplungsautomatik darf nur die Kupplung automatisieren | 📋 BESCHLOSSEN | `23#1025E` Nr. 14; `M-WB17` Block 5 | FZ ST | – |
| VEH-DEC-PH-006 | Getriebemodi: Automatik, Handschaltung mit Auto-Kupplung, mit digitaler Kupplung, mit voller Kupplung; Lenkrad, Pedale und H-Schalter als Enthusiastenoption | 🧭 RICHTUNG | `W-FP-006` | FZ ST | Ergänzt durch `W-FP-005` (DualSense) |
| VEH-DEC-PH-007 | Regen, Schnee, Eis, Aquaplaning, Sicht und Reifentemperatur beeinflussen die Fahrzeugphysik, im Rennsport noch stärker | 📋 BESCHLOSSEN | `23#1025E` Nr. 48 | FZ WET | Zusammenspiel: `CQ-12` |
| VEH-DEC-PH-008 | Jahreszeiten beeinflussen Verkehr und Straßen, besonders in der Mountain Region, aber auch in der übrigen Welt | 📋 BESCHLOSSEN | `23#1025E` Nr. 49 | FZ WET VKR | – |
| VEH-DEC-PH-009 | Crash-Energie und Sicherheitsausrüstung verzahnen sich mit dem Verletzungssystem; kein magisches Lenken in der Luft, kein Umdreh-Reset | 📋 BESCHLOSSEN | `W-FZ-015` | FZ MED | Schnittstelle Modul 07, [Kampfsystem 13](../Kampfsystem/13-Verletzungen-Schmerz-und-Kampffolgen.md) |
| VEH-DEC-PH-010 | Ladung verändert die Masse; Kupplung, Motor und Getriebe haben einen thermischen Zustand und Verschleiß | 📋 BESCHLOSSEN | Gameplay-Systeme §6.4 (`M-PD`) | FZ INV | – |
| VEH-DEC-PH-011 | Fahrzeug-Vertrautheit ohne magisches Wachstum von Fahrzeugwerten; Bergen, Abschleppen und Winde sind vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010` | FZ | Grenze der Vertrautheit offen (`W-FZ-014`) |
| VEH-DEC-PH-012 | Der Antriebsstrang (Motorkennlinie, Kupplung, Getriebe, Differenzial) wird als portabler C++20-Kern mit festem Zeitschritt gebaut; Räder, Federung und Kollision liegen im Unreal-Adapter | 💡 VORSCHLAG | `CLAUDE.md` §5 | FZ UE | Hängt an `CQ-10` |
| VEH-DEC-PH-013 | Motor-, Getriebe-, Kupplungs- und Reifenkonstanten sowie der Schleifpunkt-Algorithmus sind nicht beschlossen | ❓ OFFEN | `W-FP-010`; `D15-RV` | FZ | `CQ-13`; erst nach Prototyp |
| VEH-DEC-PH-014 | Welche Unreal-Fahrzeugphysik (Chaos Vehicles, Anpassung, eigene Lösung) verwendet wird, ist nicht entschieden | ❓ OFFEN | `CLAUDE.md` §5 (Unreal-Version offen); `WF-UE` | FZ UE | `CQ-10` |
| VEH-DEC-PH-015 | Jede Straßen- und Geländeoberfläche trägt Haftungs- und Zustandsparameter (trocken, nass, Schnee, Eis, Schlamm), die Wetter und Straßenzustand verändern | 💡 VORSCHLAG | Ableitung aus `W-FP-003`, `23#1025E` Nr. 48 | FZ SN WET | Werte offen |
| VEH-DEC-PH-016 | NPC-Fahrzeuge im Nahbereich nutzen ein vereinfachtes, wegbasiertes Fahrmodell mit denselben sichtbaren Folgen und wechseln bei Berührung auf volle Physik | 💡 VORSCHLAG | Ableitung aus `W-FP-009`, `N-LT-011` | FZ VKR | Zustandsübernahme: `CQ-14` |
| VEH-DEC-PH-017 | Ob NPC-Fahrer dieselbe Reifen-, Schlupf- und Kupplungsphysik wie der Spieler verwenden, ist nicht festgelegt | ❓ OFFEN | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §2; `W-FP-010` | FZ VKR | `CQ-14` |
| VEH-DEC-PH-018 | Wie stark Schaden das Fahrverhalten verändert (Leistungsverlust, Ziehen, Reifenfehler), hängt am unbeantworteten Detailgrad des Schadensmodells | ❓ OFFEN | `W-FZ-013`; `D15-RV` | FZ ZV | `CQ-23` |
| VEH-DEC-PH-019 | Ausbrechen und Übersteuern sind physikalisch möglich, nicht jeder Fehler wird automatisch stabilisiert; realistisch hohe Höchstgeschwindigkeiten statt einer niedrigen globalen Arcade-Grenze | 📋 BESCHLOSSEN | `M-DC31` §32; `M-PS31` §38 | FZ PHY | Kein Wert belegt |
| VEH-DEC-PH-020 | Straßen-, Semislick-, Winter- und Geländereifen verhalten sich tatsächlich verschieden | 📋 BESCHLOSSEN | `M-DC31` §32; `M-PS31` §42 | FZ WET | Reifenwahl und Jahreszeit: Modul 10, 15 |
| VEH-DEC-PH-021 | Motor- und Getriebetemperatur spielen bei Rennen, Volllast, Hitze und Tuning eine größere Rolle als im normalen Fahren; Kupplungsverschleiß und -temperatur akkumulieren realistisch und zerstören die Kupplung nicht nach wenigen Fehlern | 📋 BESCHLOSSEN | `M-DC31` §30, §32; `M-PS31` §39 | FZ ZV | Grenzt `VEH-DEC-PH-004` ein |

---

## ST: Steuerung, Kamera und Anzeige

Gruppe zu [Modul 05](05-Steuerung-Kamera-und-Anzeige.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-ST-001 | DualSense-Belegung: R2 Gas, L2 Bremse, L1 + L2 analoge Kupplung, linker Stick Lenkung, rechter Stick Kamera, Kreuz hoch, Quadrat runter | 📋 BESCHLOSSEN | `W-FP-005`; `D15-RV` | ST FZ | – |
| VEH-DEC-ST-002 | Alle Belegungen sind frei belegbar | 📋 BESCHLOSSEN | `W-FP-005` | ST | Barrierefreiheit: `CQ-18` |
| VEH-DEC-ST-003 | Die Doppelbelegung von L2 (Bremse und Kupplungsgeste) ist im Prototyp zu klären; es wird keine andere Belegung stillschweigend untergeschoben | 📋 BESCHLOSSEN | `W-FP-005`; `D15-RV` | ST FZ | Messgrößen: 💡 `VEH-DEC-ST-012` |
| VEH-DEC-ST-004 | Ältere Controller-Konzepte (Xbox-artig, LB-Modifikator, Rückwärts durch Herunterschalten) bleiben als Richtung bestehen und sind durch die DualSense-Vorgabe präzisiert, nicht aufgehoben | 🧭 RICHTUNG | `W-FP-006`; `M-PD` ‚Vehicles / racing‘ | ST | – |
| VEH-DEC-ST-005 | Handbremse, Rückwärtsgang, Motorstart, Licht, Hupe, Blinker, Aussteigen, Kofferraum und Radio sind in der DualSense-Vorgabe nicht belegt | 🔍 UNBELEGT | `D15-RV` (acht Funktionen) | ST | `CU-03`, `CQ-15` |
| VEH-DEC-ST-006 | Lenkrad, Pedale und H-Schalter sind eine Enthusiastenoption und nicht der Hauptfokus | 📋 BESCHLOSSEN | `W-FZ-006`; Gameplay-Systeme §6.3 | ST | – |
| VEH-DEC-ST-007 | Tastatur- und Mausbelegung für das Fahren sind nicht beschlossen | ❓ OFFEN | `M-OD` ‚Vehicles‘; [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §4 | ST | `CQ-18` |
| VEH-DEC-ST-008 | Assist-Einstellungen, HUD-Realismus und Abwürgoption sind Einstellungs-Hooks; ein Vollassist-Zwang besteht nicht | 🧭 RICHTUNG | `W-FP-007` | ST UI | `CQ-02` |
| VEH-DEC-ST-009 | Eine Fahrzeug-App beziehungsweise ein Menü (Fernsteuerung, Anliefern per Telefon) ist vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010` | FZ UI | Bedienung offen |
| VEH-DEC-ST-010 | Verkehrsmeldungen im Radio und Navigation sowie Verkehrsanzeige im Smartphone sind vorgesehen | 📋 BESCHLOSSEN | `W-VK-014`; `M-CA0905` (Wortlaut: ‚may include‘, also 🧭 für das Smartphone-Detail) | UI VKR | Umfang: `CQ-17` |
| VEH-DEC-ST-011 | Quadrat ist im Fahrzeug Herunterschalten und zu Fuß Kontextinteraktion oder Angriff; Kreuz ist Hochschalten im Fahrzeug und Springen zu Fuß | ⚠️ KONFLIKT | `W-FP-005`; Auftrag Kampfsystem (unbelegte Kampfbelegung, [Kampfsystem 28](../Kampfsystem/28-Konflikte-und-offene-Fragen.md)); Proposal 0009 §3a | ST KMP | `CK-01`; lösbar über getrennte Eingabekontexte |
| VEH-DEC-ST-012 | Der L1-L2-Prototyp misst: Signalweg, Übergang beim Loslassen von L1, Ergonomie, Greifpunkt-Feinheit; Alternativen dienen nur als Vergleichsvariante | 💡 VORSCHLAG | Ableitung aus `D15-RV` (Machbarkeitsnotiz) | ST FZ | Test `CT-ST-…` in Modul 31 |
| VEH-DEC-ST-013 | Controller-Rückmeldung (Trigger-Widerstand, Vibration bei Blockieren, Schlupf) ist in den Quellen nicht erwähnt und wäre ein Vorschlag | 💡 VORSCHLAG | – | ST | nicht priorisiert |
| VEH-DEC-ST-014 | Kameraansichten, Kameraverhalten und Blick zurück sind nicht belegt; beschlossen ist nur der rechte Stick als Kamera | 🔍 UNBELEGT | `D15-RV` | ST | `CQ-16` |
| VEH-DEC-ST-015 | Fahrzeug und Fußgänger haben getrennte Eingabekontexte, zwischen denen beim Einsteigen und Aussteigen gewechselt wird | 💡 VORSCHLAG | Ableitung aus `W-FP-005`, Proposal 0009 §3a | ST | Schießen aus dem Fahrzeug ist im Kampfsystem offen |
| VEH-DEC-ST-016 | Anzeigen für Geschwindigkeit, Drehzahl, Gang, Kraftstoff oder Ladung und Verschleißwarnungen folgen aus den Anforderungen; diegetisch oder HUD ist nicht entschieden | 💡 VORSCHLAG | Ableitung aus `W-FP-001`, `W-FZ-007`, `W-FZ-008` | UI FZ | `CQ-17` |

---

## EI: Einsteigen, Aussteigen, Sitze, Kofferraum

Gruppe zu [Modul 06](06-Einsteigen-Aussteigen-Sitze-und-Kofferraum.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-EI-001 | Früh genügt ein einfacher Zugang mit automatischem Einsteigen und einfachem Start; langfristig sind tiefere Interaktion und erweiterte Startprozeduren vorgesehen; der Sitzwechsel bleibt automatisch | 📋 BESCHLOSSEN | `W-FZ-011`; `D15-RV` | FZ INT | – |
| VEH-DEC-EI-002 | Der Kofferraum ist die mobile Ausrüstungsbasis; das Fahrzeug hat Lager; Fahrzeugzustand und Kofferraum-Inventar sind persistent | 📋 BESCHLOSSEN | `W-FZ-001`; `W-FZ-016` (🧭 für die Persistenz) | FZ INV PER | Schema offen (`CQ-90`) |
| VEH-DEC-EI-003 | Leichen und Tierkörper im Fahrzeug sind vorgesehen | 📋 BESCHLOSSEN | Gameplay-Systeme §6.4; `M-DC31` §48 | FZ INV | Platzierung: `VEH-DEC-EI-015`; Beweiswert, Geruch, Reaktion bei Kontrolle: `CQ-21` |
| VEH-DEC-EI-004 | Bargeld im Fahrzeug oder Safe geht nicht verloren; verloren geht nur das Bargeld in der Tasche | 📋 BESCHLOSSEN | `23#613E` Nr. 17 | INV WIR | Auslöser des Verlusts nicht in der Quelle |
| VEH-DEC-EI-005 | Begleiter können eigene Fahrzeuge fahren, man kann sich ‚dort treffen‘, und die Fahrzeugrolle ist Teil der Einsatzplanung | 📋 BESCHLOSSEN | `N-CO-005`, `N-CO-010` | NPC FZ | – |
| VEH-DEC-EI-006 | Fahrzeug rufen oder liefern lassen per Telefon-App ist vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010` | FZ UI | Bedienung und Kosten offen |
| VEH-DEC-EI-007 | Zugangszustände des Fahrzeugs (abgestellt offen, abgestellt zu, besetzt Motor aus/an, gesperrt, Abtransport) werden über Ereignisse gewechselt | 💡 VORSCHLAG | Ableitung aus `W-FZ-011`, `W-FZ-016`, `E-RE-002` | FZ KRI | Format offen |
| VEH-DEC-EI-008 | Jeder Fahrzeugtyp führt eine Sitzliste mit Rollen; ein Sitz verweist auf die Person, die ihn belegt, ohne ihren Zustand zu kopieren | 💡 VORSCHLAG | `VEH-DEC-GP-015`; `CLAUDE.md` §6 (Koop bis vier Spieler) | FZ KOO | Sitzzahlen nicht belegt |
| VEH-DEC-EI-009 | Das Fahrzeuginventar liefert dem Fahrzeug eine Masse und ist Teil der Spuren- und Beweislage | 🧩 ABGELEITET | `VEH-DEC-PH-010`; `E-RE-002` | INV FZ ERM | – |
| VEH-DEC-EI-010 | Wie die Interaktionsarchitektur für Fahrzeuge aussieht (Türen, Kofferraum, Schlüssel, Motorstart), ist nicht festgelegt | ❓ OFFEN | [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §6; [Proposal 0004](../../Proposals/0004-interaction-and-persistent-door.md) | INT | `CQ-19` |
| VEH-DEC-EI-011 | Ob der Spieler Fahrzeuginhalt durch Diebstahl, Abschleppen oder Beschlagnahme verlieren kann und wie das ausgeglichen wird, ist nicht entschieden | ❓ OFFEN | – | FZ INV | `CQ-20` |
| VEH-DEC-EI-012 | Sitzzahlen, Besatzungsregeln und Fahrerwechsel mehrerer Spieler sind nicht festgelegt | ❓ OFFEN | `CLAUDE.md` §6; `E-KO-010` | KOO FZ | `CQ-22` |
| VEH-DEC-EI-013 | Die Fahrzeug-App beziehungsweise das Schnellmenü kann Schloss, Fenster, Licht, Motor, Kofferraum, Fernstart (wo unterstützt), Parkposition, Kraftstoff oder Ladung und Status bedienen, aber nur Funktionen, die das Fahrzeug wirklich besitzt; Lieferung über eine App ist denkbar | 📋 BESCHLOSSEN | `M-DC31` §34; `M-PS31` §48 | FZ UI | Bedienung beim Fahren: `CU-03` |
| VEH-DEC-EI-014 | Der Fahrzeuglagerraum nimmt schwere Waffen, Outfits, Hunter-Ausrüstung, Werkzeuge, Tiere und Kadaver, gegebenenfalls Verletzte und Körper sowie Geld und Verstecke auf | 📋 BESCHLOSSEN | `M-PS31` §45 | FZ INV | – |
| VEH-DEC-EI-015 | Das Fahrzeug kann normale Passagiere, Verletzte, Bewusstlose, Leichen, Tierkadaver und große Ausrüstung tragen; die Platzierung hängt vom Fahrzeug ab (Rückbank, Kofferraum, Pickup-Ladefläche, Van-Laderaum); bei Polizeidurchsuchungen kann verborgene Fracht entdeckt werden | 📋 BESCHLOSSEN | `M-DC31` §48; `M-PS31` §46 | FZ INV POL | Durchsuchungsregeln offen (`COMBAT-DEC-PO-023`) |

---

## ZV: Zustand, Verschleiß, Schaden

Gruppe zu [Modul 07](07-Zustand-Verschleiss-und-Schaden.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-ZV-001 | Verschleißteile: Motor, Öl, Kühlung, Getriebe, Kupplung, Differenzial, Batterie, Bremsen, Reifen, Federung; Verschleiß wird zumindest teilweise real simuliert | 📋 BESCHLOSSEN | `W-FZ-008`; `23#1025E` Nr. 9 | FZ | Tiefe je Teil offen (`CQ-25`) |
| VEH-DEC-ZV-002 | Mechanische Schäden treten zusätzlich zur visuellen Deformation auf; Leistung kann sich ändern | 📋 BESCHLOSSEN | `23#1025E` Nr. 15 | FZ | Detailgrad: `VEH-DEC-ZV-003` |
| VEH-DEC-ZV-003 | Detailgrad des Schadensmodells (Verformung, Antriebs- und Reifenfehler, Reparaturökonomie, bleibender Verschleiß) ist nicht gewählt | ❓ OFFEN | `W-FZ-013`; `D15-RV` | FZ | `CQ-23` |
| VEH-DEC-ZV-004 | Die Antwort `23#1025E` Nr. 15 (Schäden ja) und der Vermerk ‚nicht gewählt‘ im Phasenvertrag lassen sich als ‚Ob beschlossen, Wie tief offen‘ lesen; ob das gemeint war, ist nicht belegt | ⚠️ KONFLIKT | `23#1025E` Nr. 15; `D15-RV`; `W-FZ-013` | FZ | `CK-06` |
| VEH-DEC-ZV-005 | Gebrauchtfahrzeuge haben Laufleistung, Historie, Verschleiß, versteckte Mängel und Modifikationen | 📋 BESCHLOSSEN | `W-FZ-003` | FZ WIR | Verborgener Zustand: 💡 `VEH-DEC-ZV-009` |
| VEH-DEC-ZV-006 | Fahrzeuge haben unterschiedliche ballistische Strukturen; Glas kann brechen und ablenken | 📋 BESCHLOSSEN | `COMBAT-DEC-FK-017` | FZ KMP | – |
| VEH-DEC-ZV-007 | Schießen aus und auf Fahrzeuge (Reifen, Motor, Fahrer, Innenraum) ist als Kampfmechanik nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-FK-032` | KMP FZ | Frage liegt im Kampfsystem |
| VEH-DEC-ZV-008 | Der Fahrzeugzustand wird in drei Schichten geführt: Verschleiß (kontinuierlich), Defekt (diskret, überlastungs- oder zufallsbedingt), Schaden (ereignisbedingt) | 💡 VORSCHLAG | Ableitung aus `W-FZ-008`, `W-TV-001`, `23#1025E` Nr. 15 | FZ | – |
| VEH-DEC-ZV-009 | Der Zustand hat sichtbare und verborgene Anteile; der Spieler erfährt ihn über Symptome und Prüfung statt über Zahlen | 💡 VORSCHLAG | Ableitung aus `W-FZ-003` | FZ UI | Zufallsanteil: `CQ-24` |
| VEH-DEC-ZV-010 | Verschleiß wird aus Nutzungsereignissen und Zeit über Intervalle fortgeschrieben, nicht pro Bild simuliert | 💡 VORSCHLAG | Ableitung aus `N-LT-011`; Leistung Modul 27 | FZ PF | – |
| VEH-DEC-ZV-011 | Schaden wird je Zone und Bauteil geführt (Front, Heck, Seiten, Dach, Unterboden, Glas, Reifen, Antriebsstrang); Zonenzahl ist nicht beschlossen | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-FK-017` | FZ KMP | Teil von `CQ-23` |
| VEH-DEC-ZV-012 | Optionen für den Schadensdetailgrad: A Stufen, B Zonen mit Teilen, C Verformung, F Freitext; keine ist gewählt | 💡 VORSCHLAG | – | FZ | `CQ-23` |
| VEH-DEC-ZV-013 | Reparaturökonomie und bleibender mechanischer Verschleiß sind nicht gewählt | ❓ OFFEN | `D15-RV`; `W-FZ-014` | FZ WIR | `CQ-26` |
| VEH-DEC-ZV-014 | Totalschaden, Wrack, Verschrottung, Brand und Explosion von Fahrzeugen sind in den Quellen nicht behandelt | 🔍 UNBELEGT | – | FZ PER | `CQ-27`; Wrack-Lebenszyklus: [Modul 22](22-Persistenz-und-Lebenszyklen.md) |
| VEH-DEC-ZV-015 | Wartungsintervalle und Verschleißraten haben keine Quelle und werden hier nicht festgelegt | 🔍 UNBELEGT | – | FZ | `CQ-25` |
| VEH-DEC-ZV-016 | Sicherheits- und Assistenzsysteme (ABS, ESC, Traktionskontrolle, Airbags, Kollisionsvermeidung, Spurhalteassistent, Notbremsassistent) unterscheiden sich nach Fahrzeuggeneration und Ausstattung und sind dort abschaltbar, wo es realistisch ist, besonders im Rennsport; alte und moderne Fahrzeuge verhalten sich unterschiedlich | 📋 BESCHLOSSEN | `M-DC31` §36; `M-PS31` §50 | FZ MED | Wirkung auf Verletzung: `W-FZ-015` |

---

## WE: Reparatur, Wartung, Werkstätten

Gruppe zu [Modul 08](08-Reparatur-Wartung-und-Werkstaetten.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-WE-001 | Reparatur, Tuning und Werkstatt sind früh einfach und langfristig nahezu vollständig simuliert | 📋 BESCHLOSSEN | `W-FZ-012`; `D15-RV` | FZ WRK | Termine nicht beschlossen |
| VEH-DEC-WE-002 | Es gibt spezialisierte Werkstätten (Reparatur, Leistung, Rennen, Gelände, Karosserie); ein schneller Neulack-Service ist nicht die volle Tuning-Werkstatt | 📋 BESCHLOSSEN | `W-FZ-007`; Gameplay-Systeme §6.4 | FZ WRK | – |
| VEH-DEC-WE-003 | Leistungstuning ist wichtiger als kosmetisches Tuning; optisches Tuning existiert, aber weniger umfangreich | 📋 BESCHLOSSEN | `W-FZ-008`; `23#1025E` Nr. 13 | FZ | – |
| VEH-DEC-WE-004 | Abschleppdienst, Seilwinde, Bergefahrzeug sowie Crew oder anderes Fahrzeug als Bergung sind vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010`; Gameplay-Systeme §6.4 | FZ DIE | Bedienung offen |
| VEH-DEC-WE-005 | Abschleppdienst ist ein NPC-System und ein möglicher Spielerjob, aber ‚eher nicht so oft‘; Fahrzeugtransport mit Abschleppwagen, Autotransporter und Anhänger ist physisch nutzbar | 📋 BESCHLOSSEN | `23#1025E` Nr. 41, 42 | DIE FZ | Häufigkeit unscharf (`CU-05`) |
| VEH-DEC-WE-006 | Der Spieler kann ein Haus mit Garage und Werkstatt planen und bauen | 📋 BESCHLOSSEN | `W-UR-003` | IMM FZ | – |
| VEH-DEC-WE-007 | Crewmitglieder reparieren Fahrzeuge im Hauptquartier | 📋 BESCHLOSSEN | `N-CO-015` | NPC FZ | Echter Fortschritt: `N-CO-019` (🧭) |
| VEH-DEC-WE-008 | Ein Crewmitglied repariert Fahrzeuge mit echtem Fortschritt | 🧭 RICHTUNG | `N-CO-019` | NPC FZ | `CQ-30` |
| VEH-DEC-WE-009 | Die Antwort zur Tuning-Tiefe (‚mindestens so wenig‘) ist nicht eindeutig lesbar | 🔍 UNBELEGT | `23#1025E` Nr. 12 | FZ | `CU-04` |
| VEH-DEC-WE-010 | Ein Werkstattauftrag ist ein Weltzustand mit Start, erwartetem Ende, Kosten und Status; das Fahrzeug ist währenddessen nicht fahrbar | 💡 VORSCHLAG | Ableitung aus `W-TV-004`, `W-FZ-012` | FZ WRK PER | Format offen |
| VEH-DEC-WE-011 | Werkstätten sind Arbeitsplätze von NPC-Mechanikern; Öffnungszeiten und Auslastung folgen aus deren Tagesabläufen | 💡 VORSCHLAG | Ableitung aus `W-TV-002` | NPC WRK | – |
| VEH-DEC-WE-012 | Reparaturarten folgen den Zustandsschichten: Wartung (Verschleiß), Austausch (Defekt), Instandsetzung (Schaden) | 💡 VORSCHLAG | `VEH-DEC-ZV-008` | FZ WRK | – |
| VEH-DEC-WE-013 | Wartungsökonomie, Preise, Teilemarkt und Verfügbarkeit von Teilen sind nicht festgelegt | ❓ OFFEN | `W-FZ-014`; `M-OD` ‚Vehicles‘ | WIR FZ | `CQ-29` |
| VEH-DEC-WE-014 | Welche Reparaturschritte der Spieler selbst ausführt (Menü, Dienst mit Wartezeit, praktische Arbeit) ist nicht entschieden | ❓ OFFEN | `W-FZ-012` (nur früh und lang) | FZ UI | `CQ-28` |
| VEH-DEC-WE-015 | Werkstätten können sich spezialisieren: allgemeine Werkstatt, Leistung, Rennen, Gelände, Karosserie und Lack, Markenspezialisten; die Schnellwerkstatt repariert und lackiert grundlegend um, ist aber keine Volltuning-Werkstatt | 📋 BESCHLOSSEN | `M-DC31` §32; `M-PS31` §41 | FZ WRK | Ergänzt `VEH-DEC-WE-002` um Markenspezialisten |
| VEH-DEC-WE-016 | Eine Schnellwerkstatt kann gegebenenfalls die Fahndung nach einem Fahrzeug verringern (Umlackieren) | 🧭 RICHTUNG | `M-PS31` §41 (‚ggf. Fahndungsreduktion‘) | WRK POL | Wirkung auf Wiedererkennung: `CQ-73` |
| VEH-DEC-WE-017 | Optionale Arbeiten im Hauptquartier (Fahrzeugarbeit, Waffenpflege, Munition sortieren, Archiv ordnen) dürfen nicht zu verpflichtenden Fleißaufgaben werden | 📋 BESCHLOSSEN | `M-DC31` §3 | NPC WRK | Verhältnis zu `VEH-DEC-WE-008` |

---

## KE: Kraftstoff, Energie, Tankstellen

Gruppe zu [Modul 09](09-Kraftstoff-Energie-und-Tankstellen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-KE-001 | Fahrzeuge haben einen Kraftstofftank mit realem Verbrauch | 📋 BESCHLOSSEN | `23#1025E` Nr. 10; `W-FZ-007` | FZ | Werte nicht belegt |
| VEH-DEC-KE-002 | Es gibt Benzinsorten, Diesel, E85 (‚109 Oktan‘) und weitere Sorten, mit unterschiedlichen Preisen je Tankstelle | 📋 BESCHLOSSEN | `23#1025E` Nr. 11; `W-FZ-007` | FZ WIR | Preismodell: `CQ-32` |
| VEH-DEC-KE-003 | E85 oder hochoktanige Kraftstoffe können bei geeigneten Motoren und Abstimmungen etwa 50 PS und mehr bringen | 📋 BESCHLOSSEN | `W-FZ-007`; `M-WB17` Block 5 | FZ | Wirkung falscher Sorten nicht beschrieben |
| VEH-DEC-KE-004 | Rennkraftstoffe sind vorgesehen | 📋 BESCHLOSSEN | `W-FZ-007` | FZ RAC | Bezugsort offen (`CQ-34`) |
| VEH-DEC-KE-005 | Ladeinfrastruktur für Elektrofahrzeuge ist zulässig, Verbrenner dominieren das Spiel | 📋 BESCHLOSSEN | `W-FZ-019` | FZ | Antriebsanteil: `CQ-07` |
| VEH-DEC-KE-006 | Ohne Kraftstoff oder Ladung bleibt ein Fahrzeug liegen | 🧩 ABGELEITET | `VEH-DEC-KE-001`; `W-TV-001` (Pannen) | FZ UP | – |
| VEH-DEC-KE-007 | Tankstellen sind wesentlich verbreiteter als Ladepunkte; Schnellladeparks existieren; moderne Stadtteile haben mehr Ladepunkte | 💡 VORSCHLAG | `23#1024A` (Assistent) | WLT FZ | Anteil, Zahlen offen |
| VEH-DEC-KE-008 | Eine Tankstelle ist ein Weltobjekt mit Funktionen (Kraftstoff, Laden, Shop, Parken, Kameras, Arbeitsplatz, Treffpunkt) | 💡 VORSCHLAG | Ableitung aus `W-UR-005`, `W-VK-013`, `E-RE-002`, `W-TV-002` | WLT NPC | Ausstattung offen |
| VEH-DEC-KE-009 | NPC-Fahrzeuge haben einen groben Energiezustand; nahe Fahrzeuge tanken sichtbar, ferne ändern ihn abstrakt über Fahrtzyklen | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `N-LE-013` | NPC VKR | Umfang: `CQ-33` |
| VEH-DEC-KE-010 | Rastplätze und Haltepunkte an Fernstraßen sind nicht belegt; ihr Nutzen ergibt sich aus langen Fahrten mit spontanen Stopps und Trucking | 💡 VORSCHLAG | Gameplay-Systeme §6.1; `W-WI-005` | WLT FZ | `CQ-35` |
| VEH-DEC-KE-011 | Lade- und Tankzeiten sind nicht festgelegt | ❓ OFFEN | Gameplay-Systeme §6.6; `W-FZ-014` | FZ | `CQ-31` |
| VEH-DEC-KE-012 | Preisniveau, Preisschwankung und Verbindung zur Wirtschaft sind nicht belegt | 🔍 UNBELEGT | – | WIR | `CQ-32` |
| VEH-DEC-KE-013 | Zahl, Dichte und Platzierung von Tankstellen, Ladeparks und Rastplätzen sind nicht beschlossen und hängen am Straßennetz | ❓ OFFEN | – | WLT | `CQ-35` |
| VEH-DEC-KE-014 | Der Verbrauch hängt von Fahrzeug, Geschwindigkeit, Fahrstil, Beladung und Temperatur ab; die Reichweiten sind realistisch genug, dass das Tanken keine ständige Fleißarbeit ist (‚kein Tankstellen-Spam‘); Elektrofahrzeuge nutzen Batterie, Ladezustand und Laden | 📋 BESCHLOSSEN | `M-DC31` §34; `M-PS31` §47 | FZ | Grenzt `CQ-31` ein: Reichweiten sind beschlossen, Zeiten nicht |

---

## SN: Straßennetz und Straßentypen

Gruppe zu [Modul 10](10-Strassennetz-und-Strassentypen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-SN-001 | Ländliche Gebiete haben wenig Straßen und Gebäude, Metropolen dichte Netze; Brücken und Tunnel entstehen dort, wo das Gelände sie verlangt | 🧭 RICHTUNG | `W-VK-015` | WLT SN | – |
| VEH-DEC-SN-002 | Das Makronetz ist sparsam, die ländliche Straßendichte soll abnehmen, Bergstraßen folgen Tälern und Pässen | 🧭 RICHTUNG | `W-VK-007` (Arbeitsaufträge an Kartenläufe) | WLT SN | Spannung zu `VEH-DEC-SN-003`: `CK-05` |
| VEH-DEC-SN-003 | Die Berge sollen Straßen mit kleinen Dörfern besitzen | 📋 BESCHLOSSEN | `23#705E` (Antwort auf einen Kartenentwurf) | WLT SN | `CK-05` |
| VEH-DEC-SN-004 | Mit dem Auto kann man auf nicht so hohe Berge fahren; hohe Berge sind nur mit Spezialfahrzeugen oder Gondeln erreichbar | 🧭 RICHTUNG | `23#823E`; [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §1 (`23#821E`) | WLT SN | Höhengrenze nicht belegt |
| VEH-DEC-SN-005 | Straßenmaßstab nordamerikanisch, Fußgänger- und Verkehrslogik europäisch geprägt | 🧭 RICHTUNG | `W-UR-015` | WLT SN | Für die 70-km²-Linie formuliert, nicht erneut bestätigt |
| VEH-DEC-SN-006 | Wildunfälle gehören zur Welt | 📋 BESCHLOSSEN | `M-WB17` Block 6 | WLT VKR | Häufigkeit nicht belegt |
| VEH-DEC-SN-007 | Rettungsgassen sind vorgesehen | 📋 BESCHLOSSEN | `N-LE-024` | VKR | Verhalten: Modul 16 |
| VEH-DEC-SN-008 | Die Arbeitsklassen der Straßen sind Autobahn, Schnellstraße, Haupt-/Bundesstraße, Regional-/Landstraße, Stadtstraße, Wohnstraße, Wirtschafts- und Feldweg | 💡 VORSCHLAG | `WF-D03`; `WF-RG` | SN WF | Welche das Spiel unterscheidet: `CQ-36` |
| VEH-DEC-SN-009 | Verhalten und Darstellung einer Straße folgen aus einem Straßenprofil (Datenobjekt), nicht aus festen Klassenregeln | 💡 VORSCHLAG | `VEH-DEC-GP-014`; `WF-RG` | SN | Felder: Abschnitt 3 |
| VEH-DEC-SN-010 | Die Autobahnklasse hat eigene Merkmale: kein Fuß- und Radverkehr, eigene Anschlussstellen, Standstreifen und Rettungsgasse | 💡 VORSCHLAG | Ableitung aus `WF-RC01`, `N-LE-024` | SN VKR | – |
| VEH-DEC-SN-011 | Brücken, Tunnel und Fähren sind Teil des Fahrzeug- und Navigationsnetzes und brauchen eigene Streaming-, Licht- und Sperrungsregeln | 💡 VORSCHLAG | Ableitung aus `WF-RC01`, `WF-RG` | SN KI LS | – |
| VEH-DEC-SN-012 | Wildwechselzonen sind ein Streckenmerkmal ländlicher und Bergstraßen | 💡 VORSCHLAG | Ableitung aus `M-WB17` Block 6 | SN VKR | – |
| VEH-DEC-SN-013 | Gebirgsstraßen können saisonal schwer befahrbar oder gesperrt sein | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 49, `M-WB17` Block 6 | SN WET | Modul 15 |
| VEH-DEC-SN-014 | Die Fahrtrichtung (Rechts- oder Linksverkehr) ist nirgends festgelegt | 🔍 UNBELEGT | – | SN KI | `CU-06`, `CQ-37` |
| VEH-DEC-SN-015 | Höchstgeschwindigkeiten, Spurzahlen je Klasse und Verkehrsschilder sind nicht belegt | 🔍 UNBELEGT | – | SN KR | `CQ-38` |
| VEH-DEC-SN-016 | Das Gesamtnetz der Hauptwelt (Autobahnen, Pässe, Fähren, Dichte) ist offen | ❓ OFFEN | [Welt 09](../World/09-Strassen-Schiene-und-Verkehrsinfrastruktur.md) §10; `CLAUDE.md` §3 | WLT | `CQ-39` |
| VEH-DEC-SN-017 | Mautstraßen, Kreisverkehre und Einbahnstraßen sind nicht erwähnt | 🔍 UNBELEGT | – | SN KR | `CQ-40` |

---

## KR: Kreuzungen, Signale, Schilder

Gruppe zu [Modul 11](11-Kreuzungen-Signale-und-Schilder.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-KR-001 | Kontrollen von Geschwindigkeit, Rotlicht, gefährlichem Fahren und Kennzeichen sowie Verfolgungen können vorkommen; Warn-Apps im Smartphone zeigen sie an | 🧭 RICHTUNG | `23#1025E` Nr. 18; `E-QA-042` | POL UI | Details: [Modul 19](19-Polizei-und-Verkehrsueberwachung.md) |
| VEH-DEC-KR-002 | Es gibt Verkehrssignale (Rotlicht ist Teil der Verkehrsvergehen) | 🧩 ABGELEITET | `VEH-DEC-KR-001` | SN KR | Verbreitung und Programme offen |
| VEH-DEC-KR-003 | Temporäre Baustellen, Sperren und Umleitungen passend zum Stadtwachstum sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 5; `W-TV-001` | VKR | Modul 15 |
| VEH-DEC-KR-004 | Städte wachsen über Jahre bis Jahrzehnte; Netz und Kreuzungen können sich über Spieljahre ändern | 🧩 ABGELEITET | `W-UR-018`; `W-TV-001` | WLT SN | Versionierung: Modul 22, 24 |
| VEH-DEC-KR-005 | Die Karte liefert Topologie, Geometrie, Abbiegebeschränkungen und Höhenbeziehungen; sie liefert keine Vorfahrt, Signale, Schilder oder Haltelinien | 🧩 ABGELEITET | `WF-RC01`; `WF-RG` | KR WF | Ergänzung durch das Spiel: Modul 24 |
| VEH-DEC-KR-006 | Endgültige Schilder, Markierungen, Laternen und Geländer kommen aus einem Asset-Katalog (Meilenstein M4); M2 exportiert funktionale Proxy-Meshes | 🧭 RICHTUNG | `WF-RG` | WF ASS | Gehört WorldForge, nicht dem Spiel |
| VEH-DEC-KR-007 | Jede Kreuzung erhält Spielseiten-Daten (Prioritäten, Abbiegebeziehungen, Konfliktbereiche, Signalprogramm, Schilder, Haltelinien), die auf die Karten-ID verweisen | 💡 VORSCHLAG | Ableitung aus `WF-RC01` | KR KI | Format offen |
| VEH-DEC-KR-008 | Vorfahrt wird als Priorität je Zufahrt in Daten geführt, das Verhalten liegt in der KI | 💡 VORSCHLAG | – | KR KI | – |
| VEH-DEC-KR-009 | Der Signalzustand ist eine Funktion von Weltzeit und Signalprogramm und wird nicht je Ampel gespeichert | 💡 VORSCHLAG | Ableitung aus `W-TV-004`; Leistung | KR PER PF | Abweichungen als Ereignisse |
| VEH-DEC-KR-010 | Ein Schild ist ein Träger einer Regel; Schild und Regel entstehen und vergehen gemeinsam (Baustellen, Umleitungen, Sperrungen) | 💡 VORSCHLAG | – | KR VKR | – |
| VEH-DEC-KR-011 | Welche Kreuzungsarten vorkommen (Signal, Vorfahrtsstraße, Kreisverkehr, Anschlussstelle) und wie verbreitet sie sind, ist nicht belegt | 🔍 UNBELEGT | – | SN KR | `CQ-41` |
| VEH-DEC-KR-012 | Ausfall und Sonderschaltung von Signalen sowie Signalvorrechte für Einsatzfahrzeuge sind nicht belegt | 🔍 UNBELEGT | – | KR UP | `CQ-42` |
| VEH-DEC-KR-013 | Gestaltung der Schilder (Anlehnung an Vorbilder oder eigenständig) und Regeln an Bahnübergängen sind nicht belegt | 🔍 UNBELEGT | – | KR ASS | `CQ-43` |

---

## SP: Stadtverkehr, Parken, Garagen

Gruppe zu [Modul 12](12-Stadtverkehr-Parken-und-Garagen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-SP-001 | Parkflächen existieren realistisch, der Spieler wird aber nicht gezwungen, lange einen Parkplatz zu suchen | 📋 BESCHLOSSEN | `23#1025E` Nr. 46 | VKR SP | – |
| VEH-DEC-SP-002 | Parken, Park&Ride-Bedarf und Straßensperren bei Veranstaltungen gehören zur Welt | 📋 BESCHLOSSEN | `W-VK-013`; `W-UR-009` | WLT VKR | – |
| VEH-DEC-SP-003 | Parkplätze gehören zum Gewerbe- und Dienstleistungsumfang; der Spieler kann ein Haus mit Garage planen | 📋 BESCHLOSSEN | `W-UR-005`; `W-UR-003` | WLT IMM | – |
| VEH-DEC-SP-004 | Valet-Parken ist Teil der Planungszusammenfassung, der Entwickler antwortete auf die Frage aber ‚was ist das‘; eine bewusste Entscheidung nach der Erklärung ist nicht belegt | 🔍 UNBELEGT | `M-WB17` Block 5; `W-VK-013`; `23#1025E` Nr. 47 | WLT | `CU-02`, `CQ-46` |
| VEH-DEC-SP-005 | NPCs nutzen Wohnort, Arbeitsplätze und Ziele im Alltag; der Stadtverkehr ergibt sich aus ihrem Leben | 📋 BESCHLOSSEN | `W-TV-002` | NPC VKR | – |
| VEH-DEC-SP-006 | Das Fahrzeug jedes NPC hat einen Heimatstellplatz und typische Ziele; der Standort geparkter Fahrzeuge folgt dem Tagesablauf | 🧩 ABGELEITET | `N-ID-005`; `W-TV-002` | NPC VKR | Details: Modul 14 |
| VEH-DEC-SP-007 | Parken wird aus Kapazität und Zustand modelliert; einzelne Parklücken entstehen erst beim Annähern | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-006`, `N-LE-013` | SP LS | Alternative: einzelne Lücken überall (`CQ-45`) |
| VEH-DEC-SP-008 | Die Parkplatzsuche der NPCs ist im Nahbereich sichtbar und in der Ferne abstrakt | 💡 VORSCHLAG | Ableitung aus `N-LE-013` | NPC KI | – |
| VEH-DEC-SP-009 | Orte als Heimat für Fahrzeuge einer Verkehrsrolle (Garage, Firmenhof, Depot, Verwahrstelle, Schrottplatz) sind Weltobjekte mit NPC-Arbeitsplätzen | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `W-TV-006` | WLT NPC | Platzierung: `CQ-46` |
| VEH-DEC-SP-010 | Stadtbezirke tragen ein Verkehrscharakter-Profil (Dichtekurve, typische Rollen, Geschwindigkeitsniveau, Parkdruck, Fußgängeranteil) | 💡 VORSCHLAG | Ableitung aus [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §6 | WLT VKR | Werte nicht belegt: `CQ-47` |
| VEH-DEC-SP-011 | Regeln für Falschparken, Strafzettel und Abschleppen von Spielerfahrzeugen sind nicht belegt | ❓ OFFEN | – | POL VKR | `CQ-44` |
| VEH-DEC-SP-012 | Zahl, Art und Platzierung von Parkhäusern, Garagen, Depots und Verwahrstellen sind nicht beschlossen | ❓ OFFEN | – | WLT | `CQ-46` |
| VEH-DEC-SP-013 | Fußgängerzonen, Verkehrsberuhigung und Umweltzonen sind nicht erwähnt | 🔍 UNBELEGT | – | SN VKR | `CQ-48` |

---

## KI: Verkehrs-KI und Navigation

Gruppe zu [Modul 13](13-Verkehrs-KI-und-Navigation.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-KI-001 | Fahrer-NPCs unterscheiden sich in Können und Persönlichkeit; Anfänger können manuelle Autos abwürgen; das Wetter ändert das Verhalten | 📋 BESCHLOSSEN | `W-FP-009` | NPC KI | – |
| VEH-DEC-KI-002 | Fahrstile: Anfänger, vorsichtig, normal, aggressiv, schlecht; Anpassung an das Wetter; Rettungsgassen | 📋 BESCHLOSSEN | `N-LE-024` | NPC KI | Parameter: 💡 `VEH-DEC-KI-006` |
| VEH-DEC-KI-003 | NPCs können den Spieler und auffällige Fahrzeuge wiedererkennen | 📋 BESCHLOSSEN | `N-GE-018` | NPC ERM | Schwellen offen |
| VEH-DEC-KI-004 | Navigation und Verkehrsanzeige können Teil des Smartphone-Ökosystems sein | 🧭 RICHTUNG | `M-CA0905` (‚may include‘) | UI | `CQ-54` |
| VEH-DEC-KI-005 | Die Verkehrs-KI hat drei Schichten: Strategie (Ziel und Route), Taktik (Manöver), Operation (Gas, Bremse, Lenkung); die Strategie gehört zur NPC-Architektur | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `N-LE-013` | KI NPC | Keine zweite NPC-Architektur |
| VEH-DEC-KI-006 | Ein Fahrstil ist ein Parameterprofil (Abstand, Tempoüberschreitung, Reaktionszeit, Regeltreue, Risiko, Geduld, Spurwechsel, Anfahrkönnen), das sich mit Wetter und Stimmung ändern kann | 💡 VORSCHLAG | Ableitung aus `N-LE-024`, `W-FP-009` | NPC KI | Werte nicht belegt |
| VEH-DEC-KI-007 | NPC-Routenwahl, Spielernavigation und Radiomeldungen lesen denselben Straßengraph und dieselbe Verkehrslage | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-015` | KI UI | – |
| VEH-DEC-KI-008 | Routenkosten sind dynamisch (Stau, Baustelle, Sperrung, Wetter, Klasse, Zugangsrecht); Neuplanung erfolgt bei Ereignissen | 💡 VORSCHLAG | Ableitung aus `W-TV-001` | KI | – |
| VEH-DEC-KI-009 | Hierarchische Routensuche ist wegen der Netzgröße eine zu prüfende Option | 💡 VORSCHLAG | – | KI PF | `CQ-50` |
| VEH-DEC-KI-010 | Zufall der Verkehrs-KI wird deterministisch aus NPC-ID, Welt-Seed und Zeitfenster abgeleitet | 💡 VORSCHLAG | `W-TV-010` (Assistentenvorschlag) | KI PER | `CQ-112` |
| VEH-DEC-KI-011 | Schutzregel für wichtige NPCs: kein tödlicher Simulationsunfall; ob auch Verletzungen verhindert werden, ist offen | 💡 VORSCHLAG | Ableitung aus `N-LT-033` | KI NPC | `CQ-52`, `CK-02` |
| VEH-DEC-KI-012 | Das Verkehrsmodell (Agenten, Fluss oder Mischform) ist nicht beschlossen | ❓ OFFEN | `W-FP-010`; `W-TV-008` | KI | `CQ-49` |
| VEH-DEC-KI-013 | Wie NPC-Fahrer auf Hupen, Drängeln, Blockieren, Verfolgung und Schüsse reagieren, ist nicht beschrieben | ❓ OFFEN | – | KI KMP | `CQ-51` |
| VEH-DEC-KI-014 | Alkohol beeinflusst die Spielfigur in Reaktion, Gleichgewicht, Wahrnehmung und Fahren | 📋 BESCHLOSSEN | `M-CA0905` | KI MED | Rechtsfolgen und NPC-Verhalten: `VEH-DEC-KI-016` |
| VEH-DEC-KI-015 | Zielführung, Straßenschilder und Karte des Spielers sind nicht belegt | 🔍 UNBELEGT | – | UI KI | `CQ-54` |
| VEH-DEC-KI-016 | Rechtsfolgen des Fahrens unter Einfluss, Fahren von NPCs unter Einfluss sowie Müdigkeit und Ablenkung als Fahrfaktor sind nicht belegt | 🔍 UNBELEGT | – | KI POL | `CU-07`, `CQ-53` |

---

## NF: NPC-Fahrer und NPC-Fahrzeuge

Gruppe zu [Modul 14](14-NPC-Fahrer-und-NPC-Fahrzeuge.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-NF-001 | NPCs benutzen wiederholt dasselbe eigene Fahrzeug | 📋 BESCHLOSSEN | `N-ID-005` | NPC FZ | – |
| VEH-DEC-NF-002 | Wichtige NPCs besitzen Fahrzeuge persistent | 📋 BESCHLOSSEN | `N-ID-007`, `N-FA-016` | NPC FZ PER | Grenze ‚wichtig‘ folgt Persistenzstufen |
| VEH-DEC-NF-003 | NPCs verreisen, machen Urlaub und besuchen andere Städte, sonst gäbe es keine Staus | 📋 BESCHLOSSEN | `N-LE-006` | NPC VKR | Ziele auf oder außerhalb der Karte: `CQ-55` |
| VEH-DEC-NF-004 | Ferne NPCs werden als Zustand geführt (`AT_WORK`, `HOME`, `TRAVEL`), nicht als exakte Bewegung | 📋 BESCHLOSSEN | `N-LE-013` | NPC LS | – |
| VEH-DEC-NF-005 | Wichtige NPCs sterben nicht durch banale Simulationsfehler wie Verkehrs-KI | 📋 BESCHLOSSEN | `N-LT-033` | NPC KI | `CK-02`, `CQ-52` |
| VEH-DEC-NF-006 | Zeugen können Polizei und Rettung rufen; die Welt reagiert auf Ereignisse | 📋 BESCHLOSSEN | `N-WA-011`; `N-LE-025` | NPC POL | – |
| VEH-DEC-NF-007 | Fahrzeuge der Persistenzstufen A und B sind volle Fahrzeugdatensätze; Fahrzeuge der Stufe C werden aus NPC-ID und Seed erzeugt und speichern nur Abweichungen | 💡 VORSCHLAG | Ableitung aus `N-SI-008`, `N-SI-013`, `N-LT-016` | NPC FZ PER | Format: `CQ-57` |
| VEH-DEC-NF-008 | Ein berührtes Fahrzeug (beschädigt, gestohlen, durchsucht) wird zu einem vollen Datensatz | 💡 VORSCHLAG | Ableitung aus `N-SI-010` | FZ PER | – |
| VEH-DEC-NF-009 | Eine Fahrt ist ein Zustand TRAVEL (Start, Ziel, Route, erwartete Ankunft, Fahrzeug); in der Ferne ohne Bewegung, in der Nähe als Fahrt auf der Straße | 💡 VORSCHLAG | Ableitung aus `N-LE-013`, `N-SI-011` | NPC LS | – |
| VEH-DEC-NF-010 | Der Pendlerverkehr der Stoßzeiten entsteht aus den Tagesabläufen der NPCs, nicht aus einer eigenen Verkehrsregel | 🧩 ABGELEITET | `N-LE-006`; `W-TV-001`; `W-TV-002` | NPC VKR | Dichte: Modul 15 |
| VEH-DEC-NF-011 | Berufsfahrer (Taxi, Bus, Lieferung, Trucking) sind NPCs mit Arbeitsplatz; ihre Fahrzeuge gehören dem Betrieb | 💡 VORSCHLAG | Ableitung aus `W-WI-005`, `W-TV-002`, `W-TV-006` | NPC WIR | Busfahrpläne nicht belegt |
| VEH-DEC-NF-012 | Der Verkehr setzt sich aus persönlichem Verkehr (NPCs im Zustand TRAVEL) und einer Verkehrsmenge ohne Identität zusammen; Statisten werden bei Beteiligung befördert | 💡 VORSCHLAG | Ableitung aus `N-SI-007`, `N-SI-018`, `N-LE-026` | NPC VKR LS | Verhältnis: `CQ-58` |
| VEH-DEC-NF-013 | Reisen außerhalb der Karte werden als Zustand `ABWESEND(bis t)` geführt | 💡 VORSCHLAG | Ableitung aus `N-LE-006` | NPC | Hängt an `CQ-55` |
| VEH-DEC-NF-014 | NPC-Fahrer reagieren auf Unfälle und Blockaden nach Fahrstil und Lage (Anhalten, Umfahren, Notruf, Weiterfahren); Ferne erfährt von Störungen über Meldungen | 💡 VORSCHLAG | Ableitung aus `N-LE-024`, `N-LT-013`, `W-VK-014` | NPC KI | Verteilung nicht belegt |
| VEH-DEC-NF-015 | Der Anteil der NPCs mit eigenem Fahrzeug, Mehrfahrzeughaushalte und Fahrgemeinschaften sind nicht belegt | 🔍 UNBELEGT | – | NPC FZ | `CQ-56` |
| VEH-DEC-NF-016 | Wie NPCs über Jahre Fahrzeuge kaufen, verkaufen, wechseln und altern, ist nicht belegt | 🔍 UNBELEGT | `N-LT-006` (Umzug, Beruf) ohne Fahrzeugaussage | NPC FZ | `CQ-59` |

---

## VD: Dichte, Stau, Baustellen, Sperrungen

Gruppe zu [Modul 15](15-Verkehrsdichte-Stau-Baustellen-und-Sperrungen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-VD-001 | Die Verkehrsdichte schwankt dynamisch nach Uhrzeit, Wochentag, Stadtteil, Veranstaltungen, Wetter und Ferien, aber nicht zu extrem | 📋 BESCHLOSSEN | `23#1025E` Nr. 1; `W-TV-001` | VKR | Werte: `CQ-60`, `CQ-61` |
| VEH-DEC-VD-002 | Staus zu Stoßzeiten sind möglich und werden im Radio angesagt | 📋 BESCHLOSSEN | `23#1025E` Nr. 2; `W-VK-014`; `W-TV-001` | VKR UI | Der Wortlaut ‚kann man machen‘ ist weicher als ‚ja‘ |
| VEH-DEC-VD-003 | Temporäre Baustellen, Sperren und Umleitungen passend zum Stadtwachstum sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 5; `W-TV-001` | VKR WLT | – |
| VEH-DEC-VD-004 | Veranstaltungen, Stadtrennen und Rallye-Etappen beeinflussen oder sperren den Verkehr | 📋 BESCHLOSSEN | `W-UR-009`; `W-UR-010` | VKR WLT | – |
| VEH-DEC-VD-005 | Dynamische Zwischenfälle verursachen dauerhafte Schäden, NPC-Trupps reparieren über glaubhafte Zeit, der Spieler kann helfen | 📋 BESCHLOSSEN | `W-UR-012`; `N-LE-025` | WLT NPC | – |
| VEH-DEC-VD-006 | Nach größeren Verbrechen gibt es Absperrungen; Tatorte bleiben bis zur Reinigung oder Reparatur | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-016`; `COMBAT-DEC-PO-004` | POL ERM | – |
| VEH-DEC-VD-007 | Die Dichte ist eine Funktion aus Straßenklasse, Bezirk, Tageszeit, Wochentag, Feiertag und Ferien, Ereignissen, Wetter und Nachfrage und wird als Dichtekurven-Profil in Daten geführt | 💡 VORSCHLAG | Ableitung aus `W-TV-001`, `W-TV-002` | VKR | – |
| VEH-DEC-VD-008 | ‚Nicht zu extrem‘ wird als einstellbare Dämpfung oder Obergrenze der Dichte umgesetzt | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 1 | VKR UI | Wert und Abhängigkeit: `CQ-61` |
| VEH-DEC-VD-009 | Staus entstehen aus Nachfrage (Tagesabläufe) und Kapazität (Spuren, Sperrungen); in der Ferne sind sie ein Verzögerungsfaktor je Kante, in der Nähe werden sie als Fahrzeugschlange rekonstruiert | 💡 VORSCHLAG | Ableitung aus `N-LE-006`, `N-LE-013` | VKR LS | `CQ-63` |
| VEH-DEC-VD-010 | Eine Baustelle ist ein Weltobjekt mit Zeitraum, Wirkung, Umleitung und Beschilderung; Quellen sind Stadtentwicklung und Reparatur nach Zwischenfällen | 💡 VORSCHLAG | Ableitung aus `W-UR-018`, `W-UR-012` | VKR WLT | Erzeugung: `CQ-62` |
| VEH-DEC-VD-011 | Eine Sperrung ist ein Ereignis mit Ursache, Geltungsbereich und Dauer; Routenplanung liest sie, Beschilderung zeigt sie | 💡 VORSCHLAG | – | VKR KI | – |
| VEH-DEC-VD-012 | Verkehrsmeldungen werden aus der Verkehrslage abgeleitet, mit Verzögerung und ohne Allwissen | 💡 VORSCHLAG | Ableitung aus `W-VK-014`, `N-LT-013` | UI VKR | Umfang: `CQ-64` |
| VEH-DEC-VD-013 | Nach einem Zeitsprung wird die Verkehrslage neu abgeleitet statt nachsimuliert | 💡 VORSCHLAG | Ableitung aus `N-SI-014` | VKR LS | – |
| VEH-DEC-VD-014 | Konkrete Dichtekurven, Ferienkalender und Staugrenzen sind nicht belegt | 🔍 UNBELEGT | – | VKR | `CQ-60` |
| VEH-DEC-VD-015 | Wie Wetter die Verkehrsdichte und die Unfallrate verändert, ist nur für die Fahrphysik belegt, nicht für die Dichte | 🔍 UNBELEGT | `23#1025E` Nr. 1, 48 | VKR WET | `CQ-60` |
| VEH-DEC-VD-016 | Wie viele Baustellen und Sperrungen es gibt und wie sie erzeugt werden, ist nicht belegt | ❓ OFFEN | – | WLT VKR | `CQ-62` |

---

## UP: Unfälle, Pannen, Einsatz- und Sonderverkehr

Gruppe zu [Modul 16](16-Unfaelle-Pannen-Einsatz-und-Sonderverkehr.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-UP-001 | NPCs können Unfälle verursachen, Polizei, Rettung und Abschleppdienst reagieren; Unfälle sind selten, außer der Spieler rammt die NPCs | 📋 BESCHLOSSEN | `23#1025E` Nr. 3; `W-TV-001` | VKR DIE | Rate nicht belegt: `CQ-65` |
| VEH-DEC-UP-002 | NPC-Fahrzeuge können liegenbleiben (Panne, Reifenpanne, Überhitzung) | 📋 BESCHLOSSEN | `23#1025E` Nr. 4; `W-TV-001` | VKR | Rate: `CQ-65` |
| VEH-DEC-UP-003 | Feuerwehr, Rettung, Polizei, Abschleppdienst, Bergrettung und Wasserrettung werden in geeigneter Tiefe simuliert | 📋 BESCHLOSSEN | `W-TV-006`; `E-EV-001` | DIE | Tiefe: `CQ-66` |
| VEH-DEC-UP-004 | Der Verkehr macht Rettungsfahrzeugen Platz; NPC-Kriminelle können von der Polizei verfolgt werden | 📋 BESCHLOSSEN | `E-PO-009`; `N-LE-024` | VKR POL | – |
| VEH-DEC-UP-005 | Zeugen setzen einen Notruf mit Latenz, Täterbeschreibung (Kleidung, Fahrzeug, Kennzeichen, Richtung) und Kennzeichenabfrage ab | 📋 BESCHLOSSEN | `E-RE-010`; `N-WA-011`; `N-BH-003` | NPC POL | – |
| VEH-DEC-UP-006 | Autotreffen, Straßenrennen (illegal, getrennt vom Profi-Motorsport) und Trackdays (nur auf den richtigen Strecken) sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 43, 44, 45 | WLT RAC | Regeln: `CQ-68` |
| VEH-DEC-UP-007 | Ein Unfall ist ein Ereignis mit Ursache, Beteiligten und Schwere-Eingaben; Folgen laufen an die zuständigen Systeme (Fahrzeugschaden, Verletzung, Kapazität, Zeugen, Einsatz, Strafbarkeit, Wrack) | 💡 VORSCHLAG | Ableitung aus `W-FZ-015`, `E-RE-010`, `W-UR-012` | VKR ALL | – |
| VEH-DEC-UP-008 | Pannen entstehen aus Zustand und Belastung; der Zufall ist deterministisch aus Fahrzeug-ID, Seed und Zeitfenster abgeleitet | 💡 VORSCHLAG | Ableitung aus `W-TV-001`, `VEH-DEC-KI-010` | FZ VKR | – |
| VEH-DEC-UP-009 | Jeder Einsatz folgt der Zustandsfolge Meldung, Alarmierung, Anfahrt, Vor Ort, Räumung, Wiederherstellung; Dienste laufen als parallele Teilfolgen | 💡 VORSCHLAG | Ableitung aus `E-RE-010`, `W-TV-006` | DIE | Dauern: `CQ-65` |
| VEH-DEC-UP-010 | Einsatzfahrzeuge tragen ein Sonderrechte-Merkmal; andere Fahrer reagieren nach Fahrstil (Rettungsgasse, Anhalten) | 💡 VORSCHLAG | Ableitung aus `N-LE-024` | VKR KI | Sonderrechte-Regeln: `CQ-42` |
| VEH-DEC-UP-011 | Eine Unfallstelle bleibt als Weltobjekt (Spuren, Wrack) bis zur Räumung | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-PO-004` | WLT ERM | – |
| VEH-DEC-UP-012 | Fahrerflucht (Entfernen vom Unfallort) ist ein Delikt, das über Zeugen, Kennzeichen und Beschreibung verfolgt wird | 💡 VORSCHLAG | Ableitung aus `E-RE-010`; Rechtslage offen | POL ERM | `CQ-80` |
| VEH-DEC-UP-013 | Der Spieler kann im Unfall Zeuge werden; weitere Rollen (Verursacher, Helfer, Opfer) und ihre Regeln sind nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-PO-006` | SPI POL | `CQ-67` |
| VEH-DEC-UP-014 | Unfall- und Pannenraten, Einsatzzeiten und die Tiefe der Dienste (Wachen, Besatzungen, Hubschrauber) sind nicht belegt | 🔍 UNBELEGT | – | DIE VKR | `CQ-65`, `CQ-66` |
| VEH-DEC-UP-015 | Katastrophenlagen auf Straßen (Hochwasser, Sturm, Waldbrand, Evakuierung) sind als Weltereignisse genannt, ihre Wirkung auf den Verkehr ist nicht beschrieben | 🔍 UNBELEGT | `M-WB17` Block 6 | WET VKR | `CQ-69` |

---

## VT: Fußgänger, Radfahrer, weitere Teilnehmer

Gruppe zu [Modul 17](17-Fussgaenger-Radfahrer-und-weitere-Teilnehmer.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-VT-001 | Fahrräder gibt es; E-Bikes sind ebenfalls vorgesehen, aber seltener | 📋 BESCHLOSSEN | `23#1025E` Nr. 24, 25 | FZ VKR | – |
| VEH-DEC-VT-002 | E-Scooter gibt es als Sharing-Systeme und als private Scooter | 📋 BESCHLOSSEN | `23#1025E` Nr. 26 | FZ WLT | Menge, Standorte offen |
| VEH-DEC-VT-003 | Motorräder sind tiefer simuliert als in typischen Open-World-Spielen, so wie Autos | 📋 BESCHLOSSEN | `23#1025E` Nr. 27 | FZ PHY | Wiedergabetreue: `CQ-11` |
| VEH-DEC-VT-004 | ATVs und UTVs für ländliche, Wald- und Bergregionen sowie Schneemobile für die kalte Bergregion sind vorgesehen | 📋 BESCHLOSSEN | `23#1025E` Nr. 28, 29; `W-FZ-002` | FZ | Geländenavigation offen |
| VEH-DEC-VT-005 | Menschenmengen sind an Stadien, Festivals, Nachtleben, Downtown, Skiorten, Motorsport und Parks wesentlich; Crowd und Verkehr nutzen LOD | 📋 BESCHLOSSEN | `W-UR-011` | NPC LS | – |
| VEH-DEC-VT-006 | Fußgänger- und Verkehrslogik ist europäisch geprägt, der Straßenmaßstab nordamerikanisch | 🧭 RICHTUNG | `W-UR-015` | WLT SN | Für die 70-km²-Linie formuliert |
| VEH-DEC-VT-007 | Der Straßenzugang jedes Teilnehmertyps folgt aus der Verkehrsrolle und dem Straßenprofil (`allowed_roles`) | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-KL-008`, `VEH-DEC-SN-009` | SN VKR | – |
| VEH-DEC-VT-008 | Fußgänger sind NPCs mit Alltagsablauf; Querungen folgen Regeln, die Regeltreue folgt der Persönlichkeit; die Verkehrs-KI kennt Fußgänger als Konfliktpartner | 💡 VORSCHLAG | Ableitung aus `W-TV-002`, `W-UR-011` | NPC KI | – |
| VEH-DEC-VT-009 | E-Scooter-Sharing wird als Bestand je Zone modelliert und in der Ferne abstrahiert | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 26 | WLT LS | – |
| VEH-DEC-VT-010 | Gehweg- und Radwegdaten fehlen in der Kartenschnittstelle nach heutigem Stand | 🧩 ABGELEITET | `WF-RC01` (Felder) | WF SN | Wunsch an die Schnittstelle: `CQ-71` |
| VEH-DEC-VT-011 | Regeln für Kollisionen mit Fußgängern und Tieren (Verletzung, Tod, Recht, Reaktion der Umgebung) sind nicht beschrieben | ❓ OFFEN | `N-BH-003` (nur allgemeine Eskalation) | KMP POL | `CQ-70` |
| VEH-DEC-VT-012 | Darstellung und Verhalten von Kindern und Minderjährigen im Verkehr hängen an einer im Kampfsystem offenen Frage | ❓ OFFEN | [Kampfsystem 28](../Kampfsystem/28-Konflikte-und-offene-Fragen.md); `M-OD` ‚Combat / injury / gore‘ | KMP NPC | `CQ-72` |
| VEH-DEC-VT-013 | Reiter, Tierführer und Kutschen im Straßenverkehr sind nicht erwähnt | 🔍 UNBELEGT | – | NPC | – |

---

## BZ: Besitz, Zulassung, Diebstahl

Gruppe zu [Modul 18](18-Besitz-Zulassung-und-Diebstahl.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-BZ-001 | Versicherungen sind vorgesehen, aber in einer ausgereifteren Fassung | 📋 BESCHLOSSEN | `23#1025E` Nr. 16 | WIR FZ | Umfang und Zeitpunkt: `CQ-74`; Wortlaut ‚fertigere Version‘ unscharf: `CU-08` |
| VEH-DEC-BZ-002 | Fahrzeugkauf soll glaubwürdig funktionieren; Zulassung und Papierkram werden stark abstrahiert | 📋 BESCHLOSSEN | `23#1025E` Nr. 17 | WIR FZ | – |
| VEH-DEC-BZ-003 | Fahrzeugdiebstahl, Einbruch, Raub und illegale Rennen gehören zur unabhängig vom Spieler laufenden Kriminalität | 📋 BESCHLOSSEN | `M-WB89` Block 8 | KRI NPC | Rate offen |
| VEH-DEC-BZ-004 | Fahrzeug und Kennzeichen sind Beweisquellen; die Täterbeschreibung enthält das Fahrzeug; Kennzeichenabfrage | 📋 BESCHLOSSEN | `E-RE-002`; `E-RE-010` | POL ERM | – |
| VEH-DEC-BZ-005 | NPCs können den Spieler und auffällige Fahrzeuge wiedererkennen | 📋 BESCHLOSSEN | `N-GE-018`; `E-SP-011` | NPC ERM | – |
| VEH-DEC-BZ-006 | Intelligente übernatürliche Wesen können unter menschlicher Identität Fahrzeuge besitzen | 📋 BESCHLOSSEN | `M-WB89` Block 9 | KRE NPC | Auswirkung auf Ermittlung: Modul 21 |
| VEH-DEC-BZ-007 | Händler und Gebrauchtmarkt sind NPC-Betriebe mit Fahrzeugbestand | 🧩 ABGELEITET | `W-FZ-003`; `W-UR-005` | WIR NPC | Preisbildung offen |
| VEH-DEC-BZ-008 | Ein Fahrzeug trägt vier getrennte Fakten mit je einem Eigentümer: Eigentümer, Halter, Kennzeichen, Zulassungsstatus; Änderungen laufen über Ereignisse | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-015` | FZ PER | – |
| VEH-DEC-BZ-009 | Ein Kauf überträgt Eigentum und Zulassung in einem Schritt | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 17 | WIR | – |
| VEH-DEC-BZ-010 | Diebstahl ist ein Ereignis mit Täter, Fahrzeug, Zugangsart, Zeugen und Spuren; der Diebstahl des Spielers unterliegt denselben Folgen wie der von NPCs | 💡 VORSCHLAG | Ableitung aus `M-WB89` Block 8, `E-RE-002` | KRI POL | Methoden: `CQ-73` |
| VEH-DEC-BZ-011 | Das Kennzeichen ist ein eigenes Objekt mit Bundesstaat und Nummer; Wiedererkennung läuft über das Kennzeichen und über die Auffälligkeit des Fahrzeugs | 💡 VORSCHLAG | Ableitung aus `E-RE-010`, `N-GE-018` | POL ERM | Format: `CQ-75` |
| VEH-DEC-BZ-012 | Versicherung ist ein Anlass bei Schaden und Diebstahl; Betrug ist ein bereits vorgesehener Falltyp | 💡 VORSCHLAG | Ableitung aus [Ermittlung 07](../Ermittlung/07-Detektivlaufbahn-und-organische-Faelle.md) | WIR ERM | – |
| VEH-DEC-BZ-013 | Methoden des Fahrzeugdiebstahls, Konsequenzen und das Tauschen oder Fälschen von Kennzeichen sind nicht beschrieben | ❓ OFFEN | – | KRI | `CQ-73` |
| VEH-DEC-BZ-014 | Kennzeichenformate und Zulassungsregeln je fiktivem Bundesstaat sind nicht festgelegt | ❓ OFFEN | – | WLT POL | `CQ-75` |
| VEH-DEC-BZ-015 | Eigentum von Gast- und Hostfahrzeugen im Koop folgt dem Host-Speicherstand; Details sind offen | ❓ OFFEN | `CLAUDE.md` §6; `E-KO-010` | KOO PER | `CQ-76` |
| VEH-DEC-BZ-016 | Grenzen des Besitzes (Fahrzeuganzahl, Garagenkapazität, laufende Kosten, Steuern) sind nicht belegt | 🔍 UNBELEGT | – | WIR | `CQ-77` |

---

## PV: Polizei und Verkehrsüberwachung

Gruppe zu [Modul 19](19-Polizei-und-Verkehrsueberwachung.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-PV-001 | Feste und mobile Geschwindigkeitskontrollen sind zulässig, gleich ob Polizeikontrolle oder Blitzer | 📋 BESCHLOSSEN | `23#1025E` Nr. 19 | POL UI | Höchstgeschwindigkeiten fehlen: `CQ-38` |
| VEH-DEC-PV-002 | Verkehrskontrollen sind normale Stops oder Verfolgungen je nach Lage; das Verhalten des Spielers entscheidet über die Eskalation | 📋 BESCHLOSSEN | `N-BH-003`; `E-RE-011` | POL | – |
| VEH-DEC-PV-003 | Das Gesetz folgt tatsächlicher Information und Beweiskonfidenz; die Polizei ist nicht allwissend | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-001`; `COMBAT-DEC-PO-013` | POL ERM | – |
| VEH-DEC-PV-004 | Festnahme ist kein automatisches Game Over; es gibt eine Strafkette von der Tat bis zum Urteil | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-009` | POL | – |
| VEH-DEC-PV-005 | Schwere Fahndung bleibt bis zur glaubwürdigen Auflösung; kein Sternesystem | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-008` | POL | Widerspruch Quiz-Text: `CK-03` |
| VEH-DEC-PV-006 | Es gibt mehrere Polizeitypen und Zuständigkeiten (lokal, Bezirk, Bundesstaat, Bund) | 📋 BESCHLOSSEN | `M-WB89` Block 8; `COMBAT-DEC-PO-001` | POL WLT | – |
| VEH-DEC-PV-007 | NPC-Kriminelle können von der Polizei verfolgt werden; der Verkehr macht Rettungsfahrzeugen Platz | 📋 BESCHLOSSEN | `E-PO-009`; `N-LE-024` | POL VKR | – |
| VEH-DEC-PV-008 | Aufgeben und Flucht sind Wege bei Polizeikontakt | 🧭 RICHTUNG | `COMBAT-DEC-PO-010` | POL | – |
| VEH-DEC-PV-009 | Wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | 📋 BESCHLOSSEN | `COMBAT-DEC-PO-015` | POL NPC | – |
| VEH-DEC-PV-010 | Ein Verstoß wird nur geahndet, wenn ihn ein Wahrnehmer feststellt (Streife, mobile Kontrolle, Kamera, Zeuge); die Feststellung ist eine Beobachtung mit Beweiskonfidenz | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-PO-013`, `E-RE-002` | POL ERM | Dichte: `CQ-82` |
| VEH-DEC-PV-011 | Warn-Apps lesen bekannte Kontrollen aus dem Weltzustand, mit Verzögerung und Unschärfe | 💡 VORSCHLAG | Ableitung aus `E-QA-042`, `N-LT-013` | UI POL | – |
| VEH-DEC-PV-012 | Eine Verkehrskontrolle ist eine Zustandsfolge: Wahrnehmung, Anhaltesignal, Reaktion, Kontrolle, Ergebnis | 💡 VORSCHLAG | Ableitung aus `N-BH-003` | POL | Schwellen offen |
| VEH-DEC-PV-013 | Eine Verfolgung ist ein Eskalationszustand der Kontrolle oder eines Einsatzes mit Verfolger, Verstärkung, möglichen Absperrungen und Beendigung | 💡 VORSCHLAG | Ableitung aus `E-PO-009` | POL KI | Stufen: `CQ-81` |
| VEH-DEC-PV-014 | Verstöße und Folgen liegen als Daten je Jurisdiktion (Katalog) | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-014`, `M-WB89` Block 8 | POL | – |
| VEH-DEC-PV-015 | Der Verstoßkatalog, Bußgelder, Punkte, Fahren unter Einfluss und Haft für Verkehrsdelikte sind offen | ❓ OFFEN | `COMBAT-DEC-PO-023`; `M-OD` ‚Crime / law / evidence‘ | POL | `CQ-78` |
| VEH-DEC-PV-016 | Ein Führerschein des Spielers oder von NPCs ist nicht erwähnt | 🔍 UNBELEGT | – | POL WIR | `CQ-79` |
| VEH-DEC-PV-017 | Behördenwissen muss erfasst, verarbeitet und verteilt werden; die Beweiskonfidenz hat fünf Stufen (Unbekannt, möglicher Treffer, wahrscheinlicher Treffer, identifiziert, bestätigt); die Eskalation ist mehrstufig statt zwingend ein Sternesystem | 📋 BESCHLOSSEN | `M-PS31` §54; `M-DC31` §39 | POL ERM | Widerspruch Quiz-Text: `CK-03` |

---

## IT: Interaktionen, Transport, Jobs

Gruppe zu [Modul 20](20-Interaktionen-Transport-und-Jobs.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-IT-001 | Taxis gibt es; der Spieler kann Taxi und Rideshare als Nebenjob fahren | 📋 BESCHLOSSEN | `23#1025E` Nr. 20, 22; `W-WI-005` | WIR FZ | – |
| VEH-DEC-IT-002 | Mehrere konkurrierende Rideshare-Anbieter mit unterschiedlichen Preisen und Qualitäten sind ok, aber später und nicht priorisiert | 📋 BESCHLOSSEN | `23#1025E` Nr. 21 | WIR | – |
| VEH-DEC-IT-003 | Lieferdienste (Essen, Pakete, Kurier) sind Jobs | 📋 BESCHLOSSEN | `23#1025E` Nr. 23; `W-WI-005` | WIR | – |
| VEH-DEC-IT-004 | Frachtverkehr mit Lkw zwischen Industrie, Hafen, Lagerhäusern und Städten; Trucking ist ein Spielerjob | 📋 BESCHLOSSEN | `23#1025E` Nr. 37, 38; `W-VK-011` | WIR FZ | – |
| VEH-DEC-IT-005 | Güterzüge sind im Weltverkehr sichtbar; Hafenverkehr ist dynamisch | 📋 BESCHLOSSEN | `23#1025E` Nr. 39, 40 | WLT | Bahnbetrieb: `W-VK-004` |
| VEH-DEC-IT-006 | Zug oder Bus sind für lange Strecken bevorzugt | 📋 BESCHLOSSEN | `W-VK-014`; `23#1025E` Nr. 36 | OPV | Erleben der Reisezeit: `CQ-83` |
| VEH-DEC-IT-007 | Fahrzeugtransport mit Anhänger oder Transporter ist physisch nutzbar | 📋 BESCHLOSSEN | `23#1025E` Nr. 42 | FZ | Gespannphysik: `CQ-11` |
| VEH-DEC-IT-008 | Transportaufträge entstehen aus dem Bedarf der Welt (NPC-Fahrtwünsche, Betriebslogistik, Pannen) und sind ein Weltzustand, den NPCs oder der Spieler erfüllen | 💡 VORSCHLAG | Ableitung aus `W-TV-005`, `W-TV-002` | WIR NPC | Format offen |
| VEH-DEC-IT-009 | Ein Fahrgast ist ein echter NPC im Zustand TRAVEL; Gespräche und Beziehungen laufen über das NPC-System | 💡 VORSCHLAG | Ableitung aus `N-LT-002`, `N-LT-019` | NPC DLG | – |
| VEH-DEC-IT-010 | Fracht nutzt das Fahrzeuginventar; ihre Masse wirkt auf die Fahrphysik | 🧩 ABGELEITET | `VEH-DEC-PH-010`; `VEH-DEC-EI-009` | FZ INV | – |
| VEH-DEC-IT-011 | Konkurrierende Fahrdienste sind Betriebe derselben Wirtschaftsstruktur mit Preis- und Qualitätsprofil | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 21 | WIR | Niedrige Priorität |
| VEH-DEC-IT-012 | Bezahlung, Lizenzen für andere Fahrzeuge als Züge und der Jobumfang der frühen Phasen sind nicht belegt | ❓ OFFEN | `W-VK-004` (nur Bahn); – | WIR | `CQ-84` |
| VEH-DEC-IT-013 | Ob Reisezeit im ÖPNV in Echtzeit, im Zeitraffer oder übersprungen erlebt wird, ist offen | ❓ OFFEN | `W-VK-014`; Welt OF-50 | OPV UI | `CQ-83` |
| VEH-DEC-IT-014 | Die Rolle von Taxi und Mitfahrdienst als Fortbewegung des Spielers (neben Eigenfahrzeug und ÖPNV) ist nicht beschrieben | 🔍 UNBELEGT | – | WIR UI | `CQ-85` |
| VEH-DEC-IT-015 | Das Verhältnis der Smartphone-Dienste (Taxi, Mitfahrt, Lieferung) zur Weltwirtschaft ist nicht beschrieben | 🔍 UNBELEGT | `M-CA0905` (‚may include‘) | UI WIR | `CQ-86` |
| VEH-DEC-IT-016 | Fahrzeugkosten (Kauf, Reparatur, Treibstoff, Rennen) zählen zu den Ausgaben, Motorsport und Nebenjobs zu den Einnahmen der Spielwirtschaft | 📋 BESCHLOSSEN | `M-PS31` §53 | WIR FZ | Beträge nicht belegt |

---

## EJ: Fahrzeuge in Ermittlungen und Jagden

Gruppe zu [Modul 21](21-Fahrzeuge-in-Ermittlungen-und-Jagden.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-EJ-001 | Fahrzeug und Kennzeichen sind Beweisquellen; Spieler, Fahrzeug, Kleidung und Maskierung können wiedererkannt werden | 📋 BESCHLOSSEN | `E-RE-002`; `E-ZE-003` | ERM NPC | – |
| VEH-DEC-EJ-002 | Die Fallakte führt Verdachtsgrade und Fahrzeugwechsel | 🧭 RICHTUNG | `E-QA-020` | ERM | – |
| VEH-DEC-EJ-003 | Fahrzeugdurchsuchung nach Regeln ist ein Wunsch | 🧭 RICHTUNG | `E-PO-005` | POL ERM | Regeln offen (`COMBAT-DEC-PO-023`) |
| VEH-DEC-EJ-004 | Zu den Folgen eines Fehlschlags können Ausrüstung und Fahrzeug am Ort zurückgelassen sein | 📋 BESCHLOSSEN | `M-PD` (Fallfolgen) | ERM FZ | – |
| VEH-DEC-EJ-005 | Verfolgen mit Fahrzeugen ist eine Ermittlungsmethode | 🧭 RICHTUNG | [Welt 13](../World/13-Fahrzeuge-und-Fahrzeugklassen.md) §6 (`M-LATE`) | ERM KI | – |
| VEH-DEC-EJ-006 | Die Einsatzplanung enthält die Fahrzeugrolle; Begleiter fahren mit eigenem Fahrzeug und treffen sich ‚dort‘ | 📋 BESCHLOSSEN | `N-CO-010`; `N-CO-005` | NPC FZ | – |
| VEH-DEC-EJ-007 | Intelligente Wesen können unter menschlicher Identität Fahrzeuge besitzen | 📋 BESCHLOSSEN | `K-GS-001` | KRE NPC | – |
| VEH-DEC-EJ-008 | Ebenen können eigene Regeln für Fahrzeuge, Elektronik, Zeit und Raum haben; das Durchqueren von Reichen soll sich deutlich vom normalen Fahren in der materiellen Welt unterscheiden | 🧭 RICHTUNG | [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md); `M-A0906` | KRE FZ | Fahrzeuge in Reichen nicht belegt |
| VEH-DEC-EJ-009 | Das Fahrzeugsystem liefert den Spurenarten der Ermittlung Daten (Kennzeichen, Reifenprofil, Schadenszonen, Innenraum, Standorte, Historie); die Regeln liegen bei der Ermittlung | 💡 VORSCHLAG | Ableitung aus `E-RE-002`, `W-FZ-016` | FZ ERM | Tiefe: `CQ-89` |
| VEH-DEC-EJ-010 | Die Fahrzeughistorie (Besitzer, Halter, Standorte, Unfälle, Reparaturen) speist die Ermittlung | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-KL-010` | FZ ERM | – |
| VEH-DEC-EJ-011 | Das Fahrzeug ist in der Jagd Treffpunkt, Rückzugsweg und Transportmittel (Verletzte, Gefangene, Leichen) | 💡 VORSCHLAG | Ableitung aus `N-CO-010`, `VEH-DEC-EI-003` | FZ NPC | – |
| VEH-DEC-EJ-012 | Das Fahrzeugsystem liest ein Regelprofil der Ebene als Eingabe; die Hauptwelt hat das Standardprofil | 💡 VORSCHLAG | Ableitung aus [Kreaturen 07](../Kreaturen/07-Kosmologie-Seelen-und-Reiche.md) | FZ KRE | – |
| VEH-DEC-EJ-013 | Angriffe, Verfolgung und Behinderung von Fahrzeugen durch Monster und die Wirkung von Fahrzeugen gegen Wesen sind nicht beschrieben | 🔍 UNBELEGT | – | KRE KMP | `CQ-87` |
| VEH-DEC-EJ-014 | Observation aus dem Fahrzeug, Ortung und digitale Fahrtdaten (Telematik) sind nicht erwähnt | 🔍 UNBELEGT | – | ERM | `CQ-88` |

---

## PE: Persistenz und Lebenszyklen

Gruppe zu [Modul 22](22-Persistenz-und-Lebenszyklen.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-PE-001 | Fahrzeuge sind persistent: stabile Identität, Zustand, am Falltatort abgestellt, Kofferraum-Inventar | 🧭 RICHTUNG | `W-FZ-016`; `N-PE-001` | PER FZ | Schema offen (`CQ-90`) |
| VEH-DEC-PE-002 | Gespeichert wird nur, was von der unveränderten Welt abweicht; ein unverändertes Objekt braucht praktisch nur ID und Seed | 📋 BESCHLOSSEN | `N-PE-005`; `N-PE-007` | PER | – |
| VEH-DEC-PE-003 | Spielstände sind segmentiert, inkrementell, absturzsicher, migrierbar und wiederherstellbar; alte Spielstände werden deterministisch migriert | 📋 BESCHLOSSEN | `N-PE-003`; `N-PE-011` | PER | – |
| VEH-DEC-PE-004 | Im Koop ist der Host-Spielstand maßgeblich | 📋 BESCHLOSSEN | `N-PE-019`; `CLAUDE.md` §6 | PER KOO | – |
| VEH-DEC-PE-005 | Mods sind pro Spielstand aktivierbar, deaktivierte Mods und ihr Zustand bleiben erhalten | 📋 BESCHLOSSEN | `N-PE-018` | PER MOD | Verwaiste Fahrzeuge: `CQ-93` |
| VEH-DEC-PE-006 | Fahrzeugzustand läuft nicht durch `WorldFacts`; dort stehen nur diskrete Fakten (zum Beispiel gestohlen, verkauft, verschrottet) | 🧩 ABGELEITET | [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) §6; `N-PE-020` | PER | Decision 0004 ist ein Engineering-Vorschlag, kein Kanon |
| VEH-DEC-PE-007 | Fahrzeuge erhalten eigene Segmente (`vehicle_overrides`, `vehicle_records`, `vehicle_events`, `road_state`, `service_state`) | 💡 VORSCHLAG | Ableitung aus `N-PE-003`, `N-PE-023` | PER | Format: `CQ-90` |
| VEH-DEC-PE-008 | Lebenszyklus eines Fahrzeugs: Standard, Override, Voll, Wrack, Abschleppen, Verwahrung, Schrott, Archiv | 💡 VORSCHLAG | Ableitung aus [NPCs 21](../NPCs/21-Weltpersistenz-und-Speicherentwurf.md) §3 | PER FZ | – |
| VEH-DEC-PE-009 | Verkehrslage, Stau und Signalstand sind abgeleitet und werden nicht gespeichert; gespeichert werden Objekte und Ereignisse mit Zeit | 💡 VORSCHLAG | Ableitung aus `N-PE-005`, `VEH-DEC-KR-009` | PER VKR | – |
| VEH-DEC-PE-010 | Verschrottete Fahrzeuge bleiben als Archivdatensatz mit Kennzeichen und Historie bestehen | 💡 VORSCHLAG | Ableitung aus `N-PE-015` | PER ERM | – |
| VEH-DEC-PE-011 | Besitzerwechsel, Diebstahl und Verkauf sind atomare Ereignisse; ein Fahrzeug hat nie zwei Eigentümer oder Standorte | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-015` | PER FZ | – |
| VEH-DEC-PE-012 | Das Spielerfahrzeug in Fahrt wird mit transientem Zustand gespeichert; der NPC-Verkehr wird nach dem Laden rekonstruiert | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-006`, [Decision 0004](../../Decisions/0004-world-facts-save-format-v1.md) §6 | PER VKR | `CQ-91` |
| VEH-DEC-PE-013 | Wracks und abgestellte Fahrzeuge werden durch eine intelligente Bereinigung begrenzt | 💡 VORSCHLAG | Ableitung aus `COMBAT-DEC-PO-003` | PER WLT | Regel: `CQ-94` |
| VEH-DEC-PE-014 | Die Invariante ‚Laden gleicht Speichern‘ muss für Verkehr als logische, nicht bitgleiche Gleichheit gelesen werden | ❓ OFFEN | `N-PE-022` | PER VKR | `CQ-91` |
| VEH-DEC-PE-015 | Speichergröße und Größenanteil von Fahrzeugen und Verkehr sind nicht festgelegt; das Gesamtziel ist umstritten | ⚠️ KONFLIKT | `N-PE-004`; `W-TV-011` | PER | `CK-04`, `CQ-92` |
| VEH-DEC-PE-016 | Wie Straßenänderungen und Netzversionen zum Re-Import von Kartenregionen passen, ist nicht entschieden | ❓ OFFEN | `WF-C09`; `WF-UE` | PER WF | `CQ-92` |

---

## KO: Koop und Netzwerk

Gruppe zu [Modul 23](23-Koop-und-Netzwerk.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-KO-001 | Koop: 2 bis 4 Spieler, Host-Spielstand maßgeblich, Mods und Cheats erlaubt, getrennt von der offiziellen Online-Progression | 📋 BESCHLOSSEN | `N-KO-001`; `N-KO-003`; `CLAUDE.md` §6 | KOO | – |
| VEH-DEC-KO-002 | Was ein Gast auf seinem eigenen Spielstand hat, bleibt dort; bei einem Freund hat er nur, was auf dessen Spielstand ist | 📋 BESCHLOSSEN | `N-KO-002` | KOO PER | Fahrzeugmitnahme: `CQ-96` |
| VEH-DEC-KO-003 | Ein Welt-Zeitsprung im Koop braucht die Zustimmung aller aktiven Spieler; Verhaftung ist individuell | 📋 BESCHLOSSEN | `N-KO-008` | KOO | – |
| VEH-DEC-KO-004 | Die Simulation läuft nur auf der Autorität; die Replikation des Simulationszustands ist offen | 📋 BESCHLOSSEN | `N-KO-018`; `E-KO-010` | KOO LS | Als offen geführt: `VEH-DEC-KO-013` |
| VEH-DEC-KO-005 | Die Mehrspieler-Synchronisation von Fahrzeugen ist nicht beschlossen | ❓ OFFEN | `W-FP-010`; `D15-RV` | KOO FZ | `CQ-95` |
| VEH-DEC-KO-006 | Fahrzeug-, Sitz- und Besitzdaten verweisen auf Personen per ID statt auf ‚den Spieler‘ | 🧩 ABGELEITET | `CLAUDE.md` §6 | KOO FZ | – |
| VEH-DEC-KO-007 | Optionen für die Autorität der Fahrphysik: A host-autoritativ, B besitzer-autoritativ, C hybrid mit Vorhersage, F Freitext; keine ist gewählt | 💡 VORSCHLAG | – | KOO FZ | `CQ-95` |
| VEH-DEC-KO-008 | Cheats im Koop senken die Anforderung an Betrugsschutz; für das offizielle Online (ohne Mods und Cheats) gilt das nicht | 💡 VORSCHLAG | Ableitung aus `N-KO-003`, `N-KO-004` | KOO | – |
| VEH-DEC-KO-009 | Fahrzeuge in der Nähe eines Spielers werden nach Relevanz repliziert; das Verkehrsbudget wächst nicht linear mit der Spielerzahl | 💡 VORSCHLAG | Ableitung aus [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §10 | KOO LS | – |
| VEH-DEC-KO-010 | Die Verkehrsmenge könnte auf Gästen lokal aus Seed, Straße und Zeit erzeugt und nur bei Interaktion repliziert werden | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-NF-012`, `VEH-DEC-KI-010` | KOO VKR | Setzt plattformübergreifenden Determinismus voraus: `CQ-112` |
| VEH-DEC-KO-011 | Jeder Spieler und jedes Spielerfahrzeug ist eine Streaming-Quelle | 💡 VORSCHLAG | Ableitung aus [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §10 | LS KOO | – |
| VEH-DEC-KO-012 | Fahrzeugtypen aus Mods müssen bei allen Spielern vorhanden sein; ein Spielstand mit kritisch fehlenden Mods lädt standardmäßig nicht | 📋 BESCHLOSSEN | `N-KO-013` | MOD KOO | – |
| VEH-DEC-KO-013 | Netzwerkarchitektur und Replikation sind offen | ❓ OFFEN | `E-KO-010`; `N-KO-018` | KOO | `CQ-95` |
| VEH-DEC-KO-014 | Fahrerwechsel, Beifahrerfähigkeiten, gemeinsame Fahrentscheidungen und Beschlagnahme beim verhafteten Spieler sind nicht beschrieben | ❓ OFFEN | `N-KO-005`; `N-KO-008` | KOO FZ | `CQ-97` |
| VEH-DEC-KO-015 | Fahrzeug-, Renn- und Zeitlogik im offiziellen Online sind nicht beschrieben | 🔍 UNBELEGT | `N-KO-004` | KOO | `CQ-98` |
| VEH-DEC-KO-016 | Im offiziellen Online bleiben Geld, Fahrzeuge, Immobilien und Fortschritt beim Wechsel zwischen öffentlicher, Freunde- und privater Sitzung erhalten; es werden keine absurd dominanten Fahrzeuge zur Leistungssteigerung eingeführt; als langfristiges Ziel gelten 64 bis 100 gleichzeitige Spieler, falls technisch machbar | 📋 BESCHLOSSEN | `M-DC31` §12, §13 | KOO FZ | Spielerzahl nur Zielrichtung; Verkehr im Online: `CQ-98` |

---

## WF: WorldForge-Schnittstelle

Gruppe zu [Modul 24](24-WorldForge-Schnittstelle.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-WF-001 | WorldForge ist ein eigenständiger Rust-Weltgenerator; implementiert ist M0.1 (synthetisches Terrain); der Unreal-Import ist `UNVERIFIED` | 🧩 ABGELEITET | `WF-RD`; `WF-UE` | WF | Stand 24.10.2026; kann sich ändern |
| VEH-DEC-WF-002 | `RC-01` ist ein M2-Vorvertrag mit `SettlementNode`, `RoadNode`, `RoadEdge`, `Crossing`, `Intersection`, nicht als vollständiger Straßengenerator zu lesen | 🧭 RICHTUNG | `WF-RC01` | WF SN | – |
| VEH-DEC-WF-003 | Straßengraph (Topologie) und metrische 3D-Trasse sind getrennt; Straßen verbinden Orte nach Bedarf; Validatoren und Regressionsfälle sind vorgesehen | 🧭 RICHTUNG | `WF-RG`; `WF-D03` | WF SN | – |
| VEH-DEC-WF-004 | Mesh- und Spline-Export referenziert dieselben RoadEdge-, Crossing-IDs und Höhenkoordinaten | 🧭 RICHTUNG | `WF-RC01`; `WF-EX` | WF UE | Import ungeprüft |
| VEH-DEC-WF-005 | Stabile IDs sind deterministisch abgeleitet; der Reimport verändert nur WorldForge-eigene Actors | 🧭 RICHTUNG | `WF-C09`; `WF-UE` | WF UE PER | – |
| VEH-DEC-WF-006 | WorldForge rechnet in Metern; die Achsenkonvention ist nicht festgelegt | 🧭 RICHTUNG | `WF-DC01` | WF UE | `CQ-101` |
| VEH-DEC-WF-007 | `RC-01` enthält keine Fahrspur-Verbindungen, Geschwindigkeitsbegrenzungen, Schilder und Signale mit Regelsemantik, Parkflächen, Tankstellen, Rastplätze oder Depots | 🧩 ABGELEITET | `WF-RC01`; `WF-RG` | WF SN KR | Bedarf: Abschnitt 3 |
| VEH-DEC-WF-008 | Vorhanden oder geplant sind Straßenklasse, Spuren (Feld), Querungsarten, Abbiegebeschränkungen, Steigungs- und Krümmungsprofil, Owner-Region, Nachfrage-Index | 🧩 ABGELEITET | `WF-RC01`; `WF-D03` | WF SN | – |
| VEH-DEC-WF-009 | Kandidatenfelder für das Spiel: lane_semantics, lane_connections, speed_hint, functional_role, ParkingArea, ServicePoint, Depot, RegionMembership | 💡 VORSCHLAG | Ableitung aus Abschnitt 3 | WF SN | Format offen: `CQ-100` |
| VEH-DEC-WF-010 | Drei Wege für die Fahrspur-Ebene: A WorldForge liefert, B Spiel leitet ab, C Überlagerung mit Verweis auf stabile IDs, F Freitext; keiner gewählt | 💡 VORSCHLAG | – | WF SN | `CQ-99` |
| VEH-DEC-WF-011 | Das Spiel führt eine Netzversion im Weltzustand und prüft beim Laden und Neuimport alle Verweise auf Karten-IDs | 💡 VORSCHLAG | Ableitung aus `WF-C09`, `WF-D03` | PER WF | `CQ-105` |
| VEH-DEC-WF-012 | Das Spiel prüft aus der Fahrzeugsicht: Befahrbarkeit, Erreichbarkeit der Dienste, Fahrzeitkalibrierung, Fahrzeugfähigkeit, Overlay-Verweise und die Regressionsfälle der Karte | 💡 VORSCHLAG | Ableitung aus `WF-RC01`, `WF-D03`, `W-GE-026` | WF TST | Tests: Modul 31 |
| VEH-DEC-WF-013 | Für frühe Phasen wird ein kleiner, von Hand erstellter Platzhalter-Straßengraph im Kandidatenformat gepflegt und später durch einen WorldForge-Export ersetzt | 💡 VORSCHLAG | Ableitung aus `CLAUDE.md` §3 | WF TST | – |
| VEH-DEC-WF-014 | Daten der Referenzwelt WORLD_700 sind Referenz; ihre Geometrie wird ohne Genehmigung nicht übertragen | 📋 BESCHLOSSEN | `CLAUDE.md` §2 Punkt 7; [Welt 24](../World/24-WORLD700-GIS-Referenz.md) | WF | Nutzung als Testbasis: `CQ-104` |
| VEH-DEC-WF-015 | Wer die Fahrspur-Ebene, Parkflächen und Dienstpunkte liefert, ist nicht entschieden | ❓ OFFEN | – | WF | `CQ-99`, `CQ-102` |
| VEH-DEC-WF-016 | Austauschformat, Versionierung und Verhältnis zu Geschwindigkeits- und Regelprofilen sind nicht festgelegt | ❓ OFFEN | – | WF | `CQ-100`, `CQ-103` |

---

## UE: Unreal-Integration

Gruppe zu [Modul 25](25-Unreal-Integration.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-UE-001 | Die technologische Richtung ist Unreal Engine + C++20, C++-first; Version, Plugins und Layout sind vor dem Festlegen zu prüfen | 📋 BESCHLOSSEN | `CLAUDE.md` §5 | UE | `CQ-106` |
| VEH-DEC-UE-002 | Engine-unabhängige Logik liegt in portablen C++20-Kernen; Unreal-Spezifisches hinter Unreal-Modulen und Adaptern | 📋 BESCHLOSSEN | `CLAUDE.md` §5 | UE | – |
| VEH-DEC-UE-003 | Die erste Integration benutzt Unreal 5.8.3 mit dem Modul `SpnCore`, das den Kern direkt über UBT kompiliert; der Engine-Stand ist ein Engineering-Vermerk, kein Beschluss | 🧩 ABGELEITET | [Decision 0006](../../Decisions/0006-unreal-core-integration.md) | UE | Version nicht gelockt |
| VEH-DEC-UE-004 | Im Kern gibt es wiederverwendbare Bausteine: `StrongId`, `DeterministicRandom`, `DetailTierTable`, `WorldFacts`, `Interaction`, `Pickup` | 🧩 ABGELEITET | `Core/include/spn/core/` | UE | – |
| VEH-DEC-UE-005 | Das Repository enthält keinen Fahrzeug- oder Verkehrscode | 🧩 ABGELEITET | Suche in `Core/` und `Unreal/SupernaturalProt/Source/`, 24.10.2026 | UE | – |
| VEH-DEC-UE-006 | Kleine deterministische Testkarten für komplexe Bereiche, darunter Fahrzeuge, sind vorgesehen | 📋 BESCHLOSSEN | `M-PD` (Testing) | TST | Karte für Fahrzeuge: 💡 `VEH-DEC-UE-010` |
| VEH-DEC-UE-007 | Aufteilung: Der Kern hält Fahrzeugdaten, Zustand, Antriebsstrang, Straßen- und Regeldaten, Routenplanung, Fahrstilprofile, Stufenzuordnung und Persistenz; der Adapter stellt dar und führt aus | 💡 VORSCHLAG | Ableitung aus `CLAUDE.md` §5 | UE | – |
| VEH-DEC-UE-008 | Für Fahrzeuge und Verkehr entstehen eigene Unreal-Module neben `SpnCore` (Arbeitsnamen `SpnVehicles`, `SpnTraffic`) mit Bridge-Funktionen nach dem Muster von `SpnCore` | 💡 VORSCHLAG | Ableitung aus Decision 0006 | UE | Genehmigung nach `CLAUDE.md` §7 |
| VEH-DEC-UE-009 | Kandidaten zur Prüfung: eingebaute Fahrzeugphysik, Mass Entity und darauf aufbauende Beispiele, StateTree, Smart Objects und Zonen, Navigationsmesh gegenüber eigenem Graph, World Partition; keiner gewählt | 💡 VORSCHLAG | – (nicht aus den Projektquellen) | UE | `CQ-107`, `CQ-108` |
| VEH-DEC-UE-010 | Für Fahrzeuge und Verkehr gibt es eine kleine deterministische Testkarte; Kerne werden standalone getestet, Physik und Verkehr per Unreal-Kommandozeilen-Automation | 💡 VORSCHLAG | Ableitung aus `M-PD` (Testing), `CLAUDE.md` §5 | TST UE | – |
| VEH-DEC-UE-011 | Das Spiel liest aus der Unreal-Darstellung der Straße nur IDs und Geometrieverweise, keine Spielregeln; Fahrphysik und Darstellung beruhen auf derselben Geometrie | 💡 VORSCHLAG | Ableitung aus `WF-EX`, `VEH-DEC-WF-004` | UE WF | Spike nötig |
| VEH-DEC-UE-012 | Unreal-Version und Plugins sind nicht gelockt | ❓ OFFEN | `CLAUDE.md` §5 | UE | `CQ-106` |
| VEH-DEC-UE-013 | Ob der Verkehr auf Mass Entity, einer eigenen Lösung oder einer Mischung beruht, ist offen | ❓ OFFEN | [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §9 | UE LS | `CQ-107` |
| VEH-DEC-UE-014 | Die Wahrnehmung der Verkehrs-KI (Physik-Traces oder Graphnachbarschaft) und die Wahl zwischen Navigationsmesh und eigenem Graph sind offen | ❓ OFFEN | – | UE KI | `CQ-108`, `CQ-109` |
| VEH-DEC-UE-015 | Systemkritische Teile werden möglichst modular und engine-neutral gehalten, damit ein späterer Wechsel auf eine eigene Engine langfristig realistischer wird; die kanonische Basis nennt engine-neutrale, maßgebliche Simulationsgrenzen, wo sie nützlich sind, und eine eigene Weltzustands- und Persistenzarchitektur | 📋 BESCHLOSSEN | `M-PS31` §1; `M-A1015` | UE | Stützt `VEH-DEC-UE-007` |

---

## LS: Simulationsstufen, LOD, Streaming

Gruppe zu [Modul 26](26-Simulationsstufen-LOD-und-Streaming.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-LS-001 | Crowd und Verkehr nutzen LOD und Abstraktion und rekonstruieren eine glaubwürdige lokale Bevölkerung beim Betreten eines Gebiets | 📋 BESCHLOSSEN | `W-UR-011`; `W-TV-009` | LS VKR | – |
| VEH-DEC-LS-002 | Ferne NPCs werden als Zustand geführt; die Hintergrundsimulation arbeitet ereignisbasiert; Reisen kostet Zeit; kein NPC ist an zwei Orten | 📋 BESCHLOSSEN | `N-LE-013`; `N-SI-011` | LS NPC | – |
| VEH-DEC-LS-003 | Nicht jeder NPC läuft als voller Unreal-Actor; die Weltgröße ist nirgends fest verdrahtet | 📋 BESCHLOSSEN | `N-LT-007` | LS | – |
| VEH-DEC-LS-004 | Mehrere Simulationsstufen sind vorgesehen (Arbeitsnamen S0 bis S3, SD) | 🧭 RICHTUNG | `N-SI-008` | LS | – |
| VEH-DEC-LS-005 | Leistungsstufen der Offscreen-Simulation und deterministische Resynchronisation von Zügen, Verkehr und NPCs sind offen | ❓ OFFEN | `D15-OQ` Punkt 9; `M-OQ16` Punkt 9; `W-TV-008` | LS | `CQ-110`, `CQ-112` |
| VEH-DEC-LS-006 | Fahrzeug und Fahrer teilen dieselbe Stufe: S0 Spielerfahrzeug und unmittelbare Interaktion, S1 Nahverkehr, S2 Region, S3 Rest, SD Verkehrsmenge | 💡 VORSCHLAG | Ableitung aus `N-SI-008` | LS FZ NPC | – |
| VEH-DEC-LS-007 | Distanzgrenzen kommen aus konfigurierbaren Tabellen nach dem Muster von `DetailTierTable`, getrennt für Darstellung und Simulation | 💡 VORSCHLAG | `Core/include/spn/core/DetailTiers.h` | LS | Werte: `CQ-110` |
| VEH-DEC-LS-008 | Die Materialisierung setzt zuerst persönlichen Verkehr (TRAVEL), dann geparkte Fahrzeuge, dann deterministisch die Verkehrsmenge und den Kantenzustand | 💡 VORSCHLAG | Ableitung aus `N-LE-026`, `VEH-DEC-NF-012` | LS VKR | – |
| VEH-DEC-LS-009 | Das Rückschreiben speichert Kante und Fortschritt; Identität, Fahrstil und Zustand bleiben | 💡 VORSCHLAG | Ableitung aus `N-SI-010` | LS PER | – |
| VEH-DEC-LS-010 | Fahrzeuge der Stufe S1 existieren nur auf geladenen Zellen; der Verkehr entlang der Route wird vor dem schnellen Spieler materialisiert | 💡 VORSCHLAG | Ableitung aus [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §11 | LS UE | Sicherung: `CQ-111` |
| VEH-DEC-LS-011 | Zeitzustände der Straße (Baustellen, Sperrungen) sind Laufzeitobjekte oder Data Layers | 💡 VORSCHLAG | – | LS SN | `CQ-113` |
| VEH-DEC-LS-012 | Prüfregeln: Erhaltung, Kontinuität, Konsistenz beim Wiederbesuch, nie leer und nie überfüllt, Weltuhr, Determinismus | 💡 VORSCHLAG | Ableitung aus `N-LE-026`, `N-SI-019` | LS TST | Tests: Modul 31 |
| VEH-DEC-LS-013 | Radien, Taktung, Fahrzeugzahlen je Stufe und Vorlaufstrecken sind nicht belegt | 🔍 UNBELEGT | – | LS PF | `CQ-110` |
| VEH-DEC-LS-014 | Wie weit die Szene beim Wiederbesuch gleich sein muss, ist nicht entschieden | ❓ OFFEN | `N-PE-022` | LS PER | `CQ-114` |

---

## PF: Performance und Skalierung

Gruppe zu [Modul 27](27-Performance-und-Skalierung.md).

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|:---:|---|---|---|
| VEH-DEC-PF-001 | Performance-Budgets werden von Anfang an mitgeplant; Optimierung ist ein globales Kernziel | 📋 BESCHLOSSEN | `N-SI-015`; `M-PD` (Rendering) | PF | – |
| VEH-DEC-PF-002 | Hardwarestufen und konkrete Budgets sind offen | ❓ OFFEN | [Welt 18](../World/18-Unreal-Landscape-World-Partition-und-Streaming.md) §1, §8 | PF | `CQ-115`, `CQ-116` |
| VEH-DEC-PF-003 | Die Planungsgröße von rund 500.000 NPC-Identitäten bindet keine Fahrzeugzahl und keine Zahl aktiver Fahrzeuge | 🧩 ABGELEITET | `W-TV-010`; `N-SI-016` | PF NPC | – |
| VEH-DEC-PF-004 | Ressourcen für Verkehr werden mit Detailstufen abstrahiert; Leistungsstufen sind offen | 📋 BESCHLOSSEN | [Welt 16](../World/16-NPC-Verkehr-und-Verkehrssimulation.md) §8; `D15-OQ` | PF LS | `CQ-110` |
| VEH-DEC-PF-005 | Streaming-, Last- und Leistungs-Regressionstests sind verlangt | 📋 BESCHLOSSEN | `M-PD` (Testing) | TST PF | Modul 31 |
| VEH-DEC-PF-006 | Kostentreiber wachsen mit Zahl der S0- und S1-Fahrzeuge, Netzgröße, Bevölkerung, Zeit und Spielerzahl; teure Stufen bleiben klein, die Menge liegt in S2 und S3 | 💡 VORSCHLAG | – | PF | Keine Messung |
| VEH-DEC-PF-007 | Die Obergrenze der Verkehrsdichte ist zugleich ein Leistungsregler | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 1 | PF VKR | `CQ-61` |
| VEH-DEC-PF-008 | Bei Lastspitzen wird in dieser Reihenfolge degradiert: Aktualisierungsraten, Stufenradien, Verkehrsmenge; die Straße bleibt nie leer | 💡 VORSCHLAG | Ableitung aus `N-LE-026`, [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §8 | PF LS | Schwellen offen |
| VEH-DEC-PF-009 | Materialisierung geschieht gestaffelt; Fahrzeugtypen und Verhaltensprofile sind geteilte Daten | 💡 VORSCHLAG | Ableitung aus [NPCs 22](../NPCs/22-Simulationsstufen-Skalierung-und-Performance.md) §4 | PF LS | – |
| VEH-DEC-PF-010 | Ein Messplan mit Szenarien (Innenstadt, Autobahn, Zeitsprung, Koop, Streaming) liefert die Zahlen; sie stehen erst nach der Ausführung in Berichten | 💡 VORSCHLAG | Ableitung aus `CLAUDE.md` §7 | PF TST | `CQ-118` |
| VEH-DEC-PF-011 | Große Städte brauchen kein anderes System, nur andere Profile (Bezirksprofile) und einen strengeren Blick auf Darstellungskosten | 💡 VORSCHLAG | Ableitung aus `W-UR-015`, `VEH-DEC-SP-010` | PF WLT | `CQ-117` |
| VEH-DEC-PF-012 | Für Fahrzeuge und Verkehr liegen weder Leistungsmessungen noch Budgets noch Zielhardware vor | 🔍 UNBELEGT | – | PF | – |
| VEH-DEC-PF-013 | Stadtgrößen sind Szenarien: bei 2.200 km² etwa 20 bis 25 km² für die Metropole und 8 bis 10 km² für andere große Städte; das städtische Flächenwachstum soll mit der Weltgröße unterlinear steigen | 🧭 RICHTUNG | `M-A1015` (PROVISIONAL) | PF WLT | Keine Entscheidung; ältere Kandidaten weichen ab (Welt 03) |

---

## Zählung

| Gruppe | Anzahl |
|---|---:|
| GP | 19 |
| KL | 17 |
| PH | 21 |
| ST | 16 |
| EI | 15 |
| ZV | 16 |
| WE | 17 |
| KE | 14 |
| SN | 17 |
| KR | 13 |
| SP | 13 |
| KI | 16 |
| NF | 16 |
| VD | 16 |
| UP | 15 |
| VT | 13 |
| BZ | 16 |
| PV | 17 |
| IT | 16 |
| EJ | 14 |
| PE | 16 |
| KO | 16 |
| WF | 16 |
| UE | 15 |
| LS | 14 |
| PF | 13 |
| **gesamt** | **407** |
