# Adult 31: Randfall-Katalog

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Der Katalog sammelt **Randfälle**, die eine Implementierung behandeln muss, und das **vorgeschlagene Sollverhalten** (🧭). Das Sollverhalten leitet sich aus den Quellenprinzipien ab (fail closed, Sicherheitskern, Realismus-Default); es ist kein Kanon. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Die Spezifikationen verlinken hierher; Tests: [Modul 29](29-QA-Testkatalog.md).

**Inhalt**

1. [Regelkern](#1-regelkern)
2. [Datenmodell](#2-datenmodell)
3. [Interaktionsengine](#3-interaktionsengine)
4. [Körper und Reaktion](#4-körper-und-reaktion)
5. [NPC und Gesellschaft](#5-npc-und-gesellschaft)
6. [Tattoos und Körpermodifikation](#6-tattoos-und-körpermodifikation)
7. [Welt und Wirtschaft](#7-welt-und-wirtschaft)
8. [Steuerung, UI, VR und Replay](#8-steuerung-ui-vr-und-replay)
9. [Speicherung, Mods und Builds](#9-speicherung-mods-und-builds)
10. [Übergreifende Entscheidungsregeln](#10-übergreifende-entscheidungsregeln)

---

**Lesehilfe:** Jede Zeile = ein Randfall. ‚Sollverhalten‘ ist ein Vorschlag; wo die Quellen nichts festlegen, steht **„offen“**.

## 1. Regelkern

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-ZU-01 | Teilnehmer wird während der Sitzung benommen (Treffer, Medikament, Panik) | Capacity neu bewerten; `Defer(Observe)` → bei Bestätigung Abbruch | E-SK-017 |
| RF-ZU-02 | Teilnehmer hat leichten Alkoholeinfluss und wird später stark beeinträchtigt | Capacity zeitabhängig; kein Aufwärtsrechnen auf früheres Ja | E-SK-015/025 |
| RF-ZU-03 | Mehrere Teilnehmer, einer widerruft | Sitzung für ihn beenden; Rest nur mit erneuter Zustimmung | Modul 20 |
| RF-ZU-04 | Widerruf und gleichzeitig Ausstiegsanimation fehlt | Fallback-Leiter; sicherer Ausstieg; kein Softlock | E-SK-028 |
| RF-ZU-05 | Verbindungsabbruch während `HardStop` im Koop | Sitzung serverseitig beenden; Zustand `Withdrawn` erhalten | SM-NW-09 |
| RF-ZU-06 | Speichern/Laden mitten im Stopp-Ablauf | Zustand wiederherstellen; Ablauf fortsetzen oder sauber beenden | – |
| RF-ZU-07 | Zeitraffer (Schlaf/Warten) während der Sitzung | Sitzung endet vorher; kein Überspringen des Gates | E-IA-041 |
| RF-ZU-08 | Geburtstag der Figur während der Sitzung | Neuberechnung nur bei Beginn der nächsten Sitzung | T-ZU-008 |
| RF-ZU-09 | Geburtsdatum fehlt bei importierter Figur | `Unknown` → `Deny` | E-SK-010 |
| RF-ZU-10 | NPC lügt über Alter (‚zeigt Ausweis‘ gefälscht) | Das Spiel kennt Ausweis-Ereignisse als Weltereignisse; die Verifikation läuft über den Figurenzustand, nicht über Spielerglauben; Fälschung ist Weltphänomen (offen) | E-SK-011 |
| RF-ZU-11 | Zwei widersprüchliche Signale gleichzeitig (Wegdrücken + Zustimmungsworte) | Der **restriktivere** Hinweis gewinnt; Check-in | K§18 |
| RF-ZU-12 | Stopp-Signal im Rahmen definiert, aber Figur ist stumm | Alternative Signale (Körper) bleiben gültig | 35#579 |
| RF-ZU-13 | Rahmen deckt Familie A ab, Figur zeigt Unbehagen | Check-in, Intensitätsanpassung, ggf. Ende | E-SK-021 |
| RF-ZU-14 | Spieler bricht ab, NPC wollte weitermachen | Spieler-Stopp wirkt; NPC reagiert nach Persönlichkeit; keine Schuldzuweisung im Kern | Modul 20 |
| RF-ZU-15 | NPC↔NPC-Sitzung, Spieler schaut zu, Spieler greift ein | Eingriff ist Weltaktion (Dazwischengehen, Rufen); die Sitzung wird den Weltregeln unterworfen | E-IA-002 |
| RF-ZU-16 | Konflikt: Mod fordert Ausnahme vom Gate | Ablehnung, Protokoll | E-SK-004 |
| RF-ZU-17 | Zustimmungsdienst nicht erreichbar (Server) | `Deny`, keine Sitzung | P1 |
| RF-ZU-18 | Frühere Zustimmung (Ja am Vorabend), heute neue Situation | Neu berechnen; Erinnerung beeinflusst Angebot, ersetzt Zustimmung nicht | E-SK-025 |
| RF-ZU-19 | Figur ist Parteimitglied/Begleiter und ‚Teil der Gruppe‘ | Party-/Admin-Zustimmung ist nicht gültig | E-SK-003 |
| RF-ZU-20 | Figur ist in Haft oder gefangen | `Unavailable`; abhängig von Weltregeln nicht verfügbar | Modul 20 |

## 2. Datenmodell

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-DM-01 | Figur wird beim Speichern befördert (Tier-Wechsel) | Beförderung transaktional; ID bleibt; kein Neuwürfeln | E-KO-011 |
| RF-DM-02 | Mod entfernt, aber Präferenzdefinition wird referenziert | Verweis wird als verwaist markiert; Zustand bleibt; keine Löschung | E-PA-030 |
| RF-DM-03 | Gleiche Figur in zwei Speicherständen mit unterschiedlicher Mod-Liste | Speicherstände sind getrennt; kein Abgleich | – |
| RF-DM-04 | Zeitreise (Temporal-World-State) | Zustimmung/Gedächtnis folgen dem Zeitstrahl; Konsistenzregeln erforderlich (offen) | E-NP-016 |
| RF-DM-05 | Schema-Migration scheitert | Spielstand bleibt unverändert; Fehler mit Wiederherstellungsoption | E-SP-001 |
| RF-DM-06 | Zwei Mods registrieren dieselbe Kennung | Konflikt erkannt; Laden verweigert oder Namespace-Trennung | E-SP-006 |
| RF-DM-07 | ID-Umbenennung | Alias-Tabelle; alte Referenzen bleiben gültig | E-SP-007 |
| RF-DM-08 | Adult-Bereich beschädigt | Isolierung; Basisspiel unberührt; Reparaturoption | E-PA-030 |
| RF-DM-09 | Zustimmungshistorie wächst unbegrenzt | Archivierung/Verdichtung (Datenschutz, Speicher) – offen | – |
| RF-DM-10 | Figur stirbt mit offener Zustimmung/Vertrag | Zustand `dead`; Verträge enden; Erinnerungen bleiben | E-SK-014 |
| RF-DM-11 | Klonen/Import von Figuren | neue Identität; Geburtsdatum verpflichtend | – |
| RF-DM-12 | Replay referenziert gelöschte Figur | Replay unbrauchbar mit Begründung | Modul 27 |

## 3. Interaktionsengine

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-IE-01 | Teilnehmer mit sehr unterschiedlicher Größe/Gewicht | Alternative Haltung wählen oder Abwechseln; keine Clipping-Akzeptanz | E-IA-008, E-SO-012 |
| RF-IE-02 | Verletzter Teilnehmer (Gipsarm, Rückenverletzung) | Gelenkgrenzen verengen; Pose passt; ggf. Abbruch | E-SO-011 |
| RF-IE-03 | Enger Raum (Fahrzeug, Kabine) | Platzprüfung; Alternative; sauberes Ende | E-IA-035 |
| RF-IE-04 | Zwei Aktionsschichten wollen dieselbe Hand | Priorität; zweite Schicht wartet oder entfällt | Modul 22 |
| RF-IE-05 | Item geht verloren/fällt | Item wird Weltobjekt; Schicht endet | E-IA-036 |
| RF-IE-06 | Möbelbruch mitten in der Sitzung | Neue Haltung; Weltzustand ändert sich; Persistenz | E-SO-033 |
| RF-IE-07 | Latenz im Koop | Autoritativer Server; Client-Interpolation; Sicherheit ohne Desync-Umgehung | E-SP-005 |
| RF-IE-08 | Laden aus einem Savegame, das im `SafeExit` entstand | Ausstieg fortsetzen oder Ende; kein Wiedereintritt | Modul 22 |
| RF-IE-09 | Mehr Teilnehmer als Animationsvarianten | Abwechseln/Beobachter | E-IA-008 |
| RF-IE-10 | Kleidung blockiert gewünschte Haltung | Kleidung ablegen/verschieben oder andere Haltung | E-IA-016 |
| RF-IE-11 | Hindernis taucht auf (NPC tritt ein) | `Interrupted`; Weltpriorität | E-IA-036 |
| RF-IE-12 | Wetterumschwung/Lichtwechsel | Privacy Score neu; Zeugen neu | E-IA-035 |
| RF-IE-13 | Fahrzeug bewegt sich | Sitzung gesperrt oder Weltregeln (fahrend = nicht verfügbar) – offen | Modul 22 |
| RF-IE-14 | Schwerkraft/Hanglage | Balance und Untergrund beeinflussen | E-SO-012 |
| RF-IE-15 | Selbstkollision ohne Lösung | Fallback-Leiter bis Ende | E-SO-036 |
| RF-IE-16 | Zwei Sitzungen im selben Raum | getrennte Teilnehmer; Zeugen untereinander | E-WE-021 |
| RF-IE-17 | Qualitätswechsel während Sitzung | gleiche Entscheidungen, Genauigkeit passt sich an | E-SO-014 |
| RF-IE-18 | NPC-Schedule ruft Teilnehmer weg | `Interrupted` / Ende; Weltpriorität | E-IA-035 |
| RF-IE-19 | Teilnehmer ist größer als jede Variante | Alternative / Ende | – |
| RF-IE-20 | Item mit mehreren Funktionen | Eine Funktion pro Schicht aktiv; Konflikt auflösen | 35#652 |

## 4. Körper und Reaktion

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-KR-01 | Teilnehmer mit Behinderung oder Prothese | Körpermodell mit Prothese als Teil; Gelenkgrenzen angepasst | E-SO-011 |
| RF-KR-02 | Schwangerschaft in späten Stadien | Körper-/Balanceparameter angepasst; Risiken über Gesundheitssystem | E-GE-011 |
| RF-KR-03 | Alkohol und Verletzung zugleich | Capacity-Gate, Verletzungsschwelle | E-GE-010 |
| RF-KR-04 | Medikamente verändern Reaktionen | Reaktionsprofil modifiziert | E-GE-003 |
| RF-KR-05 | Zeitsprung während Sitzung verändert Körper | Sitzung endet vorher | E-LE-011 |
| RF-KR-06 | Content-Toggle wechselt mitten in der Sitzung | Darstellung ändert sich; Physiologie bleibt | E-ST-011 |
| RF-KR-07 | Mod-Reaktion widerspricht Zustimmungszustand | Reaktionsgraph hat Vorrang; Mod-Reaktion wird verworfen | SM-MD-13 |
| RF-KR-08 | Gleichzeitige Reaktionen mit Konflikt (Husten + Lachen) | Reaction Composition löst auf | E-KO-023 |
| RF-KR-09 | Notfall mitten in Sitzung | `SafetyInterrupt`; Medizinsystem | E-GE-010 |
| RF-KR-10 | Ohnmacht durch Überlastung | Bewusstlosigkeit → Abbruch | E-SK-017 |
| RF-KR-11 | Krampf/Ermüdung ändert die Pose | Pose passt an; Pause oder Ende | E-IA-036 |
| RF-KR-12 | Anatomiegrenzen extrem (Mod erweitert) | Grenzbereiche bleiben; Plausibilitätsprüfung | E-KO-015 |
| RF-KR-13 | Verletzung heilt während Wartezeit | Heilung nach Zeitparametern | E-GE-006 |
| RF-KR-14 | Schmerz + Lust gleichzeitig | Mixed State; Check-in bei Schmerzdominanz | E-KO-024 |
| RF-KR-15 | Sehr geringe Erfahrung + ungewohnt Hartes | unbeholfener, lauter; Check-in | 35#458/66 |

## 5. NPC und Gesellschaft

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-NP-01 | Zwei NPCs interessieren sich für denselben Partner | Eifersucht/Konkurrenz nach Persönlichkeit | E-NP-007 |
| RF-NP-02 | NPC mit Gedächtnislücke (Alter, Alkohol) | unscharfe Erinnerung; Konsequenzen entsprechend | NP-GD-03 |
| RF-NP-03 | Partnerprofil falsch gelernt | Konfidenz sinkt, Korrektur | E-NP-029 |
| RF-NP-04 | NPC wechselt Tier während Gespräch | Zustand bleibt konsistent | E-LE-002 |
| RF-NP-05 | Zeuge hört, sieht aber nichts | Gerücht mit niedrigem Wahrheitsgehalt | E-WE-021 |
| RF-NP-06 | Vertrag mit Person, deren Alter nicht verifizierbar ist | kein Vertrag | E-NP-024 |
| RF-NP-07 | Verstorbener Partner mit Vertrag | Vertrag endet; Erinnerung bleibt | E-SK-014 |
| RF-NP-08 | Gerücht in Gebieten mit getrennter Simulationsstufe | Ereigniskette, nicht Echtzeit | E-LE-002 |
| RF-NP-09 | Eifersucht + Zustimmung (offene Beziehung) | Eifersucht möglich; keine Auswirkung auf Zustimmung | E-NP-022 |
| RF-NP-10 | NPC lehnt dauerhaft ab, Spieler fragt wiederholt | Anfragen werden als Belästigung gewertet (Weltreaktion) | E-NP-006 |
| RF-NP-11 | NPC hat keinerlei Präferenz-Daten (Mod-Figur) | Standardprofil aus Archetyp; harte Grenzen Standard „vorsichtig“ (offen) | E-NP-012 |
| RF-NP-12 | Haft während Beziehung | Beziehung pausiert; Ereignisse | E-WE-014 |
| RF-NP-13 | Rufschädigung durch Veröffentlichung | Ruf mehrdimensional; Folgen | E-WE-007 |
| RF-NP-14 | Spieler und NPC gleiche Wohnung, Vertrag endet | Auszug, Konflikt, Recht | E-NP-024 |
| RF-NP-15 | Beziehung zwischen NPCs unterschiedlicher Fraktionen | Fraktionseinfluss | NP-FM-01 |

## 6. Tattoos und Körpermodifikation

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-TA-01 | Tattoo über Tattoo über Narbe | Schichten; Ergebnis hängt von Künstler/Material ab | E-TA-014 |
| RF-TA-02 | Körperumbau nach dem Stechen | Verformung/Verlust | E-TA-014 |
| RF-TA-03 | Importbild mit Transparenz/Alpha | normalisiert oder abgelehnt mit Begründung | TA-ED-04 |
| RF-TA-04 | Studio schließt zwischen Sitzungen | Fortsetzung anderswo; Wartezeit | E-TA-019 |
| RF-TA-05 | Künstler stirbt/wechselt Studio | Portfolio bleibt; Kontakt verändert | E-TA-019 |
| RF-TA-06 | Laser auf großflächigem Blackout | mehrere Sitzungen, Restfarbe | E-TA-006 |
| RF-TA-07 | Piercing in verletzter Haut | Komplikation | E-TA-010 |
| RF-TA-08 | Mod entfernt Pigmentdefinition | Fallback-Farbe; Warnung | – |
| RF-TA-09 | Befördung eines NPC mit vielen Tattoos | Tattoos bleiben identisch | E-KO-011 |
| RF-TA-10 | Tattoo-Motiv sorgt für Rechts-/Plattformproblem (Symbol) | Inhaltsfilter / Zulässigkeitsentscheidung (offen) | K16 |
| RF-TA-11 | Minderjährige Figur ruft Studio auf | Altersgate: keine Leistung | Modul 25 |
| RF-TA-12 | Schwangerschaft während Sitzung | Sitzung ggf. verschoben | E-TA-014 |
| RF-TA-13 | Blackout über Gesicht | erlaubt (Entwickler: alles tätowierbar); Weltreaktionen | E-TA-017 |
| RF-TA-14 | Tattoo mit Partnernamen nach Trennung | Reue → Cover-up/Laser | E-TA-006 |

## 7. Welt und Wirtschaft

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-WE-01 | Betrieb schließt während einer Buchung | Rückerstattung/Folgen nach Vertrag | E-WE-001 |
| RF-WE-02 | Gesetzeslage ändert sich | Daten ändern; Betriebe passen sich an | E-WE-004 |
| RF-WE-03 | Replay-Cache voll beim Anzeigen eines Videos | Cache-Strategie (LRU) / Rekonstruktion | 35#527 |
| RF-WE-04 | NPC kündigt während der Spieler abwesend ist | Ereignis im Hintergrund; Meldung | E-NP-024 |
| RF-WE-05 | Hausregeln verbieten die Interaktion | Weltfolgen (Hausverbot); Regelkern unberührt | E-IA-007 |
| RF-WE-06 | Zwei Medienproduktionen mit gleichem Material | feste Replay-ID; Konflikt in Rechten | E-WE-010 |
| RF-WE-07 | Zeitraffer über Vertragsende | Vertrag endet; Folgen | E-NP-024 |
| RF-WE-08 | Preis unter Mindestlohn der Welt | Markt; Folgen | E-WE-017 |
| RF-WE-09 | Dienstleister lehnt Kunde ab | gültig; keine Erstattungspflicht ohne Vertrag (Daten) | E-WE-009 |
| RF-WE-10 | Spieler als Creator veröffentlicht ohne Zustimmung | Weltfolgen (Recht/Ruf); Systemblock je Regel | E-WE-011 |
| RF-WE-11 | Venue ohne Genehmigung in strenger Jurisdiktion | illegaler Betrieb mit Risiko | E-WE-004 |
| RF-WE-12 | Internetseite eines Betriebs offline | Ereignisse | – |

## 8. Steuerung, UI, VR und Replay

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-ST-01 | Controllerwechsel mitten in der Sitzung | Mapping wechselt, Zustand bleibt | E-ST-005 |
| RF-ST-02 | Remapping-Konflikte | Warnung, Konfliktliste | E-ST-005 |
| RF-ST-03 | VR-Tracker fällt aus | IK ergänzt; sonst Pause | ST-VR-03 |
| RF-ST-04 | Replay eines Savegames mit entferntem Mod | nicht abspielbar; Begründung | Modul 28 |
| RF-ST-05 | Re-Render mit fehlendem Asset | Platzhalter oder Abbruch mit Meldung | – |
| RF-ST-06 | Streamer-Modus während Adult-Szene | Filter greift (falls konfiguriert) | E-RP-005 |
| RF-ST-07 | Content-Toggle-Wechsel während Re-Render | Toggle zum Zeitpunkt des Starts gilt (offen) | – |
| RF-ST-08 | Aufnahmespeicher voll | Warnung, Abbruch der Aufnahme, Spiel läuft weiter | E-RP-001 |
| RF-ST-09 | GPU unterstützt Codec/Bitrate nicht | Fallback-Profil | E-RP-007 |
| RF-ST-10 | Eingabe während Pause | wird ignoriert oder gepuffert (offen) | – |
| RF-ST-11 | HUD-Stufe Developer im Release | nicht verfügbar | E-NP-021 |
| RF-ST-12 | Bibliothek zeigt Aktion, die inzwischen ungültig ist | Begründung statt Ausführung | E-ST-007 |
| RF-ST-13 | Barrierefreiheit: einhändige Bedienung | alternative Steuerprofile | E-ST-005 |

## 9. Speicherung, Mods und Builds

| Nr. | Randfall | Sollverhalten (🧭) | Basis |
|---|---|---|---|
| RF-SM-01 | Speicherstand von Build mit Paket in Build ohne Paket | Basisspiel läuft; Adult-Daten bleiben unberührt | E-PA-028 |
| RF-SM-02 | Mod-Schema ändert sich | Migration oder Ablehnung | E-SP-007 |
| RF-SM-03 | Absturz beim Speichern | Journal/Wiederherstellung | SM-SP-03 |
| RF-SM-04 | Gastspieler ohne Paket tritt bei | definiert (Darstellung/Einschränkung) – offen | K21 |
| RF-SM-05 | Paket wird mitten in der Sitzung deaktiviert | `SafeExit`; keine Datenverluste | E-PA-030 |
| RF-SM-06 | Mod fordert Internetzugriff | verweigert | E-PA-036 |
| RF-SM-07 | Replay-Cache läuft voll | Rekonstruktion/Löschung | SM-SP |
| RF-SM-08 | Reaktivierung nach langer Zeit (veraltetes Schema) | Migration; sonst verwaiste Markierung | E-PA-030 |
| RF-SM-09 | Zwei Mod-Versionen gleichzeitig | Konflikt; Abhängigkeitsprüfung | E-PA-019 |
| RF-SM-10 | Build-Validator findet Adult-Spur im Basisbuild | Build bricht ab | E-PA-032 |
| RF-SM-11 | Provenienz fehlt für ein Asset | Release-Gate schlägt an | E-RC-009 |
| RF-SM-12 | Konsolen-Zertifizierung verlangt Entfernung | Paket nicht in Konsolenbuild | E-PA-004 |
| RF-SM-13 | Host wechselt im Koop | autoritativer Zustand wandert; Zustimmung neu | E-SP-005 |
| RF-SM-14 | Speicherort ohne Schreibrecht | Fehler, kein Datenverlust | – |

## 10. Übergreifende Entscheidungsregeln

Diese Regeln gelten für alle Tabellen oben, wenn eine Zeile nichts anderes sagt.

| Regel | Inhalt | Basis |
|---|---|---|
| R1 | **Im Zweifel nicht starten / sicher beenden.** | E-SK-028 |
| R2 | **Sicherheit vor Immersion**: ein sicherer Ausstieg hat Vorrang vor Animationsqualität. | E-SK-028, E-SO-021 |
| R3 | **Keine stille Umdeutung**: Ein Signal wird nicht zugunsten der Fortsetzung umgedeutet. | K§18 |
| R4 | **Realismus-Default**: Was nicht festgelegt ist, verhält sich wie in der Realität. | E-SK-033 |
| R5 | **Daten statt Sonderfall**: Randfälle werden über Daten/Zustände gelöst, nicht über Sonderlogik. | E-IA-005 |
| R6 | **Protokollieren**: Gate-Ablehnungen und Fallbacks sind im Entwicklungsbuild nachvollziehbar. | E-SK-029 |
