# Tagesdeltas 14.–19.10.2026: Übernahmeprüfung

- **Stand:** 08.11.2026 (CONSOLIDATION-ADULT, Übernahme der Tagesdeltas)
- **Quelle:** `99_DAILY_DELTAS/2026-10-14` bis `2026-10-19` im Master-Archiv (`SPN_FULL_CUMULATIVE_MASTER_2026-09-27`), nur gelesen. Gelesen: die sechs `CHANGELOG.md` vollständig; die `docs/`-Dateien nur über die Changelogs und Stichproben. Nicht gelesen: `install.sh`, `tests/`, Manifeste, Assets (nicht ausgeführt, nicht verändert).
- **Status der Deltas:** Vorschläge, berichtete Entwicklerstände und Belege, **nicht Kanon** (`CLAUDE.md` §2, §11; Delta-README: „REVIEW REQUIRED“).
- **Regel dieser Übernahme:** Ein Delta-Punkt gilt nur dann als übernommen, wenn er in den Docs belegt ist. Bei späterer ausdrücklicher Entwicklerentscheidung gilt die spätere (`DECISIONS.md` N-01, N-02, 08.11.2026).

## Übersicht

| Datum | Delta-Punkte | In Docs belegt | Überholt | Offen / Konflikt |
|---|---|---|---|---|
| 14.10 | 4 | 2 | 2 | 0 |
| 15.10 | 17 | 14 | 0 | 3 |
| 16.10 | 2 | 2 | 0 | 0 |
| 17.10 | 3 | 2 | 0 | 1 |
| 18.10 | 6 | 5 | 0 | 1 |
| 19.10 | 9 | 7 | 1 | 1 |

(Die Zählung ist grob: Ein Punkt kann mehrere Teile haben. Die Einzelzeilen sind unten.)

## Einzelpunkte

| Nr. | Datum | Delta-Inhalt (Kurzfassung) | Status im Delta | Stand in Docs | Bewertung und Aktion |
|---|---|---|---|---|---|
| D14-1 | 14.10 | Ursprung des Familienschlüssels zurückgestellt (eigene Story-Linie) | Entscheidung „aufgeschoben“ | `World/10` §5, Zeile 4 („Schlüssel-Ursprung … bewusst reserviert“); CLAUDE §3 | **In Docs belegt.** Kein Ursprungsvorschlag übernommen. |
| D14-2 | 14.10 | Bevölkerung der größten Untergrundstadt „mehrere hunderttausend bis ~1 Mio.“, vorläufig | bestätigt als ungelöst | `World/10` §7 und `Entscheidungsstatus` §6.4 (historisch N-02); W-UG-002 | **Überholt durch N-02 (08.11.2026).** Zahl nicht übernommen. |
| D14-3 | 14.10 | Mehrere verbundene Untergrundstädte; die Hauptstadt mehrere km²; Bunker als Teilbereich | Übernahme früherer Entscheidungen | `World/10` §1 und §7 (historisch); W-UG-001 bis 003 | **Überholt durch N-02.** Bunker, Höhlen, Labore bleiben möglich. |
| D14-4 | 14.10 | Unabhängige Untergrundgesellschaft; Erbauer; Freie-Energie-Konzept; geheime Elite kennt die Städte; Misstrauen; Hunter-Schutzbund; Tesla im Bunker, Bürger nicht automatisch informiert | Übertragung, nicht neu bestätigt | `World/10` §1–5; `Ermittlung/17` Zeile 47 (Tesla geheim) | **Teilweise überholt.** Gesellschaft und Städte: historisch (N-02). Bunker-Aussagen: Geltung offen (K-28). |
| D14-5 | 14.10 | Freitext hat Vorrang vor vorgegebenen Auswahlen (Verfahrensregel des Entwicklers) | bestätigt (Verfahren) | In `Docs/Planning` nicht verankert (Suche ohne Treffer) | **Lücke.** Als Frage an den Entwickler in `OPEN_QUESTIONS.md` (Q-DELTA-1). Keine Regel eigenmächtig ergänzt. |
| D14-6 | 14.10 | 30 km² / 600.000 als Beispiel **nicht** als Kanon | nicht übernommen | Keine Festlegung in Docs | **Korrekt nicht übernommen.** |
| D15-1 | 15.10 | U24: Zugehörigkeit durch persönliches Vertrauen, kein Integrationsverfahren | bestätigt | `World/12` §1; W-GS-003 | **In Docs belegt.** Geltung nach N-02: K-12. |
| D15-2 | 15.10 | U25: natürlich gemischte Haushaltsformen | bestätigt | `World/12` §1 (U25) | **In Docs belegt.** Geltung nach N-02: K-12. |
| D15-3 | 15.10 | U26: Lernen nach Interesse, Praxis, Technik; kein Schulzwang | bestätigt (Freitext vorrangig) | `World/12` §2 | **In Docs belegt.** Freitext-Regel: D14-5. |
| D15-4 | 15.10 | U27: Grundfertigkeiten ohne Schulzwang | bestätigt | `World/12` §2 (U27) | **In Docs belegt.** |
| D15-5 | 15.10 | U28–U31: Medizin, Lebensdauer, Verjüngung, Körpermodifikation mit Grenzen, freiwillig | bestätigt; Zahlen offen | `World/12` §2 (U28, U31) | **In Docs belegt.** Zahlen bleiben offen. |
| D15-6 | 15.10 | U32: Infrastruktur-KI, Aufgabenroboter, autonome Systeme nebeneinander | bestätigt | `World/12` §3; W-GS-014 | **In Docs belegt.** |
| D15-7 | 15.10 | Schiene: 3–4 Güterknoten, 8–10 Terminals, 18–25 Ladestellen, 4–6 Depots, 9–13 Betriebsdepots, 3–4 Werkstätten, 18–26 Abstellgleise | bestätigt (User) | `World/09` §3 (📋 D15-RW); Tabelle | **In Docs belegt.** Mengen hängen an N-01 (Weltgröße offen). |
| D15-8 | 15.10 | Schiene: vollständige Betriebssimulation als Langziel; Fahren nur mit Job oder Berechtigung; NPC-Betreiber | bestätigt | `World/09` §3 | **In Docs belegt.** |
| D15-9 | 15.10 | Straße: Gangschaltung, echte Kupplung früh; Ausbau später | bestätigt | `Gameplay-Systeme` §Controller (📋 15.10) | **In Docs belegt.** |
| D15-10 | 15.10 | DualSense-Belegung: R2 Gas, L2 Bremse, L1+L2 Kupplung, Stick Lenkung, rechter Stick Kamera, X/Quadrat Schalten; alles umbelegbar | bestätigt (Machbarkeit offen) | `Gameplay-Systeme` Zeile 167; `Entscheidungsstatus` Zeile 115 | **In Docs belegt.** Machbarkeit der Kombi-Eingabe offen (Prototyp). |
| D15-11 | 15.10 | Start/Motor: einfach früh, später fortgeschritten; Handschaltung für jedes Fahrzeug; H-Schaltgate optional | bestätigt | `Fahrzeuge-und-Verkehr/03` Zeile 25 (manuelle Variante für jedes Fahrzeug); `04` VEH-DEC-PH-006 | **Teilweise belegt.** Die Stufen „einfach früh, später fortgeschritten“ und der H-Schaltgate-Punkt sind nicht gesondert geprüft. |
| D15-12 | 15.10 | Fahrzeug-Interaktionen einfach zuerst, später tiefer; Sitzwechsel automatisch | bestätigt | `Fahrzeuge-und-Verkehr/06` Zeilen 4, 24, 57 (`W-FZ-011`) | **In Docs belegt** (Sitzwechsel automatisch). |
| D15-13 | 15.10 | Tuning/Werkstätten: einfach zuerst, langfristig nahezu vollständig | bestätigt | `Fahrzeuge-und-Verkehr/08` Zeilen 4, 23 (`W-FZ-012`) | **In Docs belegt.** |
| D15-14 | 15.10 | U33 (KI-Bewusstsein) offen | offen | `World/12` §3 | **Konsistent.** |
| D15-15 | 15.10 | V06 Schadensmodell: Frage ohne Antwort | offen | `Fahrzeuge-und-Verkehr/07` Zeile 4 und 31 | **Konsistent.** |
| D15-16 | 15.10 | Untergrund-Einwohnerzahl, Fläche, Ausbau, Lebensdauer, Verjüngungsgrenzen | offen | N-02; `World/10` | **Konsistent** (historisch bzw. offen). |
| D15-17 | 15.10 | Zugnetz-Layout, Standorte, Meilensteine, Performance-Budgets | offen | `World/09`, `Fahrzeuge-und-Verkehr/27` | **Konsistent.** |
| D16-1 | 16.10 | U33: unterschiedliche Entwicklungsstufen; Bewusstsein nicht bewiesen; keine Zusatzelemente | bestätigt („D“, ohne Zusatz) | `Entscheidungsstatus` Zeile 123; W-GS-012 | **In Docs belegt.** Nicht übernommen: Assistenten-Stufenmodell (kein Gesetz). |
| D16-2 | 16.10 | U34: individuelle Anerkennung, weder Sachstatus noch volle Menschenrechte pauschal | bestätigt („D“) | `Entscheidungsstatus` Zeile 123; `World/12` §3 | **In Docs belegt.** Kriterien offen. |
| D17-1 | 17.10 | U35: unterschiedliche Regeln je nach KI und Aufgabe; keine pauschale Übersteuerung | bestätigt („D“) | W-GS-014 (📋); `World/11` Zeile 83 | **In Docs belegt.** Nicht übernommen: universelle Notaus-/Veto-Regeln, Regulierungsbehörde. |
| D17-2 | 17.10 | U36: Kombination mehrerer Kommunikationstechnologien; Implantate optional | bestätigt („E“) | `Entscheidungsstatus` Zeile 123; `World/11` §2 | **In Docs belegt.** |
| D17-3 | 17.10 | U37: Netz und Oberflächen-Internet, Antwort offen | offen | `World/11` Zeile 85; `World/12` §9 | **Konsistent** (kein Gateway vorab). |
| D18-1 | 18.10 | Weltkarte: 2.200 km² als *Arbeitsvorgabe*; Ost-Meer, Nord-Berge; Ostinsel; Glenhaven und Ashford binnen | Arbeitsstand | `Spielwelt` §4, §4-Tabelle; `World/02`, `World/04` §5 | **Teilweise überholt.** 2.200 km² ist als Richtwert durch N-01 und K-20 überholt; die Karten-Beschreibung bleibt als Kandidat dokumentiert. |
| D18-2 | 18.10 | Metropolis 110 km², 20 Bezirke, Downtown 20 km², Hochhauskern 12 km², 3–5 Supertalls; Fluss nicht durch Downtown | bestätigt (Arbeitsmaß) | `Entscheidungsstatus` §3; `World/06` §2 | **In Docs belegt** (PE-E1022-Bestätigung 22.10. laut `World/06`). |
| D18-3 | 18.10 | Kartenregeln: eine maßgebliche Geografie; native Auflösung; keine HQ-Beschriftung auf öffentlichen Karten | bestätigt (Regel) | W-KA-017; `Spielwelt` §4 (Karten-Geheimhaltung) | **In Docs belegt.** |
| D18-4 | 18.10 | Iterationsstand V3/V4 Richtung; V5 verworfen; V6 Kandidat ohne Abnahme | berichtet | Keine Abnahme in Docs | **Konsistent.** Keine Kartenabnahme ableitbar. |
| D18-5 | 18.10 | Phase 0.1 StrongId und Phase 0.2 WorldFacts implementiert (C++20, Tests) | berichtet (User) | `TASKS.md` FOUNDATION-CORE, FOUNDATION-WORLDFACTS: abgeschlossen | **Konsistent.** Testzahlen nicht unabhängig geprüft. Commit `3d8a5e3` ist eine Entwickler-Handlung (keine Aktion hier). |
| D18-6 | 18.10 | Projektpolitik: Phasen ohne Einfrieren; Assistent darf nicht committen, pushen, taggen | bestätigt | CLAUDE §10 | **In Docs belegt.** |
| D19-1 | 19.10 | Weltgröße: ~25 Varianten; 2.200 km² nur ein Arbeitspreset | bestätigt (Richtung) | `Spielwelt` §4 („größenunabhängig“); `World/03` Zeile 53, 179 | **In Docs belegt.** Variantenliste ist Assistenten-Vorschlag. |
| D19-2 | 19.10 | Maßangaben Kandidat: 2.200 km² trockenes Festland; Metropolis 110 (5 %); 2.090 außerhalb; Ostinsel 80–100 (90 Test); Wasser zählt nicht | berichtet als Messung | `World/03` §2; K-20 | **Teilweise überholt** durch N-01 (08.11.2026): Messung als Kandidat gilt, Richtwert nicht. |
| D19-3 | 19.10 | 2D-Atlas: senkrecht, keine Isometrie oder 3D-Relief | bestätigt | W-KA-002; `Spielwelt` Zeile 127 | **In Docs belegt.** |
| D19-4 | 19.10 | Hydrographie: Lake-Alder-Fluss nicht abrupt; nördliche äußere Bezirke; Greywater Hafen; Meer nur Osten | bestätigt | W-WA-004; `Spielwelt` §4 | **In Docs belegt.** |
| D19-5 | 19.10 | Siedlungshierarchie: wenige große Städte, weitere Städte und Kleinstädte, wenige Dörfer; keine Vervielfachung kleiner Dörfer | bestätigt | `World/07` §1, §4 | **In Docs belegt.** |
| D19-6 | 19.10 | Ostinsel: bevorzugte frühere Form beibehalten; keine finale Form | Entwicklerwunsch; keine Form genehmigt | `World/04` §5; OF-04 | **Teilweise.** Form offen (OF-04, OF-23). |
| D19-7 | 19.10 | Phase 0.3 WorldFacts: StateAt, Golden v1, 17 Tests, „62 Tests, 4/4 Presets grün“ | berichtet (User) | `TASKS.md` FOUNDATION-WORLDFACTS abgeschlossen | **Konsistent.** Die Testzahl ist nicht unabhängig geprüft. |
| D19-8 | 19.10 | Git: privates Remote, Commits `3d8a5e3`, `24c9b3b`, `b52d129` | berichtet (User) | CLAUDE §10 | **Keine Aktion** (Entwickler-Kontrolle). |
| D19-9 | 19.10 | Recherche: Tripo Rigging v3.0 (Unreal-Skelett) | Recherche, keine Entscheidung | nicht in Docs | **Nicht übernommen** (Werkzeug-Recherche, kein Spielentscheid). |
| D19-10 | 19.10 | Vorschau-Bilder zeigen teils „Hunter HQ“ | Befund | W-KA-017 (keine HQ-Beschriftung öffentlich) | **Hinweis** an den Entwickler (Q-DELTA-3): Vorschauen dürfen nicht öffentlich sein. |

## Konflikte und Entscheidungsbedarf

- **Q-DELTA-1:** Gilt die Freitext-Regel (D14-5, D15-3) als Planungsregel? Sie fehlt in `Docs/Planning`.
- **Q-DELTA-2:** Die 2.200-km²-Messung (D19-2) bleibt Grundlage für Kandidatenkarten, während N-01 die Größe offen lässt. Bitte bestätigen, dass die Messung nur für den Kandidaten gilt (K-20).
- **Q-DELTA-3:** HQ-Beschriftung auf Vorschaubildern (D19-10). Keine Entscheidung nötig, nur Prüfung der Bilder.
- **K-12 / K-28:** Geltung der U24–U36-Gesellschaftsentscheidungen nach N-02 und Bunker-Geltung (bestehende Konflikte, unverändert).

## Ergebnis

- **In Docs belegt:** die meisten Punkte (vollständig konsistent mit den Docs).
- **Überholt durch N-01 oder N-02:** Untergrundstadt-Bevölkerung und -Gesellschaft als Stadtkonzept (D14-2, D14-3), 2.200 km² als Richtwert (D18-1, D19-2 teilweise).
- **Nicht übernommen:** Assistenten-Vorschläge (Stufenmodell, Variantenliste im Vorschlagsstatus, Tripo-Recherche, 30 km²-Beispiel).
- **Offen:** U37, V06, Untergrund-Zahlen, Zugnetz-Layout, Kartenabnahme, Freitext-Regel (Q-DELTA-1).
