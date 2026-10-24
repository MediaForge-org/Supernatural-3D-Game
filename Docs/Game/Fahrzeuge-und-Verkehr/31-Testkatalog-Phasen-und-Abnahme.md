# Fahrzeuge und Verkehr 31: Testkatalog, Ausbaustufen und Abnahme

> [!IMPORTANT]
> **Nichts davon wurde ausgeführt, und es gibt keine Ergebnisse.** Der Katalog beschreibt künftige Prüfungen und einen Vorschlag für Ausbaustufen. **Unbeschlossene Mechaniken sind keine verbindlichen Abnahmekriterien:** Jeder Test trägt den Vermerk ‚abnahmefähig‘ (beruht nur auf 📋 oder 🧩) oder ‚bedingt‘ (hängt an einer offenen Frage `CQ-…` oder an einem Vorschlag und wird erst nach der Entscheidung verbindlich). Die Tests prüfen **Eigenschaften und Beziehungen**, keine Zahlenwerte, weil die Quellen keine nennen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Fragen: [Modul 29](29-Konflikte-und-offene-Fragen.md).

**Inhalt**

1. [Grundsätze](#1-grundsätze)
2. [Teststufen und Werkzeuge](#2-teststufen-und-werkzeuge)
3. [Fahrzeugdaten und Zustand (FU)](#3-fahrzeugdaten-und-zustand-fu)
4. [Fahrphysik (PH)](#4-fahrphysik-ph)
5. [Steuerung (ST)](#5-steuerung-st)
6. [Straßennetz, Karte, Kreuzungen (SN)](#6-straßennetz-karte-kreuzungen-sn)
7. [Verkehrs-KI und NPC-Fahrten (KI)](#7-verkehrs-ki-und-npc-fahrten-ki)
8. [Verkehrslage (VD)](#8-verkehrslage-vd)
9. [Unfälle, Pannen, Einsatz (UP)](#9-unfälle-pannen-einsatz-up)
10. [Polizei und Verkehrsüberwachung (PO)](#10-polizei-und-verkehrsüberwachung-po)
11. [Persistenz (PE)](#11-persistenz-pe)
12. [Koop (KO)](#12-koop-ko)
13. [Stufen und Streaming (LS)](#13-stufen-und-streaming-ls)
14. [Leistung (PF)](#14-leistung-pf)
15. [Regression (RG)](#15-regression-rg)
16. [Querschnittsszenarien (SZ)](#16-querschnittsszenarien-sz)
17. [Ausbaustufen und Abnahme (Entwurf)](#17-ausbaustufen-und-abnahme-entwurf)

---

## 1. Grundsätze

1. **Tests folgen Entscheidungen.** Jeder Test nennt die Register-IDs, aus denen er folgt (`CLAUDE.md` §7: Tests aus den bestätigten Anforderungen ableiten).
2. **Eigenschaften statt Zahlen.** Wo die Quellen keine Zahlen nennen, prüft der Test Beziehungen (monoton, erhalten, deterministisch, getrennt), nie einen Wert.
3. **Determinismus.** Tests der Kerne bekommen Zeitschritt und Zufall von außen; gleicher Seed ergibt gleiches Ergebnis (`DeterministicRandom`).
4. **Keine gefälschten Ergebnisse.** Berichte nennen, was tatsächlich lief (`CLAUDE.md` §7). Dieses Dokument enthält keine Ergebnisse.
5. **Manuelle Abnahme bleibt nötig** für Fahrgefühl, Optik, Audio (der Entwickler ist Endabnehmer subjektiver Ergebnisse, `M-PD`).
6. **Offene Fragen sperren Tests nicht, sie machen sie bedingt.** Ein bedingter Test wird mit der Entscheidung zur Abnahme.

---

## 2. Teststufen und Werkzeuge

| Stufe | Inhalt | Voraussetzung |
|---|---|---|
| S (Standalone C++) | Kerne: Fahrzeugdaten, Zustand, Antriebsstrang, Regeln, Signalprogramme, Routenplanung, Persistenz | CMake, Test-Framework des Projekts, Fedora-Laptop |
| U (Unreal-Automation) | Fahrphysik, Kollision, Streaming, Replikation, Kreuzungen | Unreal-Binärdateien (Windows-Rechner), kein Editor-GUI nötig |
| P (Paketiertes Spiel) | Leistung, Ladezeiten, Speichern im Paket | Paketbuild |
| M (manuell) | Fahrgefühl, Optik, Audio, Ergonomie der Steuerung | Entwickler |
| Z (Simulation) | deterministische Szenario- und Soak-Läufe mit Seeds | Test-Orchestrator ([Gameplay-Systeme](../Gameplay-Systeme.md) §17) |

Die Unterscheidung folgt `CLAUDE.md` §5: standalone, Unreal-Kommandozeile, paketiert.

---

## 3. Fahrzeugdaten und Zustand (FU)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-FU-001 | Eine Änderung der Typdefinition ändert keine Exemplardaten; ein Exemplar verweist auf Typ-ID | VEH-DEC-KL-010 | S | bedingt (Vorschlag) |
| CT-FU-002 | Ein Fahrzeug hat zu jedem Zeitpunkt genau einen Eigentümer; Besitzerwechsel ist atomar | VEH-DEC-BZ-008, VEH-DEC-PE-011 | S | bedingt (Vorschlag) |
| CT-FU-003 | Ohne Wartung wächst Verschleiß monoton mit Nutzung | VEH-DEC-ZV-001, VEH-DEC-ZV-008 | S | abnahmefähig |
| CT-FU-004 | Wartung, Austausch und Instandsetzung senken Verschleiß, Defekt und Schaden der jeweiligen Schicht | VEH-DEC-WE-012 | S | bedingt (Vorschlag) |
| CT-FU-005 | Der Energiezustand sinkt mit gefahrener Strecke; bei null bleibt das Fahrzeug liegen | VEH-DEC-KE-001, VEH-DEC-KE-006 | S | abnahmefähig |
| CT-FU-006 | Zugangszustände erlauben nur definierte Übergänge | VEH-DEC-EI-007 | S | bedingt (Vorschlag) |
| CT-FU-007 | Die Inventarmasse addiert sich zur Fahrzeugmasse | VEH-DEC-EI-009, VEH-DEC-PH-010 | S | abnahmefähig |
| CT-FU-008 | Gleicher Seed und gleiches Zeitfenster ergeben dieselbe Panne | VEH-DEC-UP-008, VEH-DEC-KI-010 | S | bedingt (Vorschlag) |
| CT-FU-009 | Ein Kennzeichen ist genau einem Fahrzeug zugeordnet; verschrottete Fahrzeuge bleiben abfragbar | VEH-DEC-BZ-011, VEH-DEC-PE-010 | S | bedingt (Vorschlag) |
| CT-FU-010 | Diebstahl erzeugt ein Ereignis mit Täter und setzt den Status; Wiedererlangung stellt ihn zurück | VEH-DEC-BZ-010 | S | bedingt (Vorschlag) |
| CT-FU-011 | Während eines Werkstattauftrags ist das Fahrzeug nicht fahrbar; der Auftrag endet nach Weltzeit | VEH-DEC-WE-010 | S | bedingt (Vorschlag) |
| CT-FU-012 | Ein Fahrzeug eines fehlenden Mod-Typs bleibt unverändert erhalten und wird nicht gelöscht | VEH-DEC-PE-005 | S | bedingt (`CQ-93`) |
| CT-FU-013 | Das Getriebe ist an die Fahrzeugvariante gebunden; es gibt keine Funktion, die ein Exemplar zwischen Handschaltung, Doppelkupplung und Automatik umschaltet | VEH-DEC-KL-016 | S | abnahmefähig |
| CT-FU-014 | Passagiere, Verletzte, Leichen, Kadaver und große Ausrüstung lassen sich nur auf Plätzen eines Fahrzeugs unterbringen, die der Typ vorsieht | VEH-DEC-EI-015, VEH-DEC-EI-008 | S | bedingt (Sitzlisten offen) |
| CT-FU-015 | Die Fahrzeug-App bietet nur Funktionen, die das Fahrzeug besitzt | VEH-DEC-EI-013 | S | abnahmefähig |

---

## 4. Fahrphysik (PH)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-PH-001 | Bei zu niedriger Drehzahl ohne Kupplung würgt das Fahrzeug ab | VEH-DEC-PH-001 | S | abnahmefähig |
| CT-PH-002 | Eine grobe Fehlschaltung führt zu Verschleiß oder Schaden, der mit der Härte der Fehlschaltung zunimmt | VEH-DEC-PH-004 | S | abnahmefähig (Beziehung), Schwellen bedingt |
| CT-PH-003 | Die Kupplungsautomatik übernimmt nur die Kupplung; die Gangwahl bleibt | VEH-DEC-PH-005 | S | abnahmefähig |
| CT-PH-004 | Die Haftung nimmt mit schlechterem Untergrund ab (trocken, nass, Schnee, Eis) | VEH-DEC-PH-007, VEH-DEC-PH-015 | U | bedingt (`CQ-12`) |
| CT-PH-005 | Größere Masse verlängert den Bremsweg bei sonst gleichen Bedingungen | VEH-DEC-PH-010 | U | abnahmefähig |
| CT-PH-006 | Gleicher Eingabeverlauf mit festem Zeitschritt ergibt denselben Antriebsstrangzustand | VEH-DEC-PH-012 | S | bedingt (Vorschlag) |
| CT-PH-007 | Kein Lenken in der Luft und kein Umdreh-Reset | VEH-DEC-PH-009 | U | abnahmefähig |
| CT-PH-008 | Ein NPC-Anfänger würgt ein manuelles Fahrzeug im Nahbereich sichtbar ab | VEH-DEC-KI-001, VEH-DEC-PH-016 | U | bedingt (`CQ-14`) |
| CT-PH-009 | Straßen-, Winter- und Geländereifen führen bei gleichem Untergrund zu unterschiedlicher Haftung | VEH-DEC-PH-020 | U | abnahmefähig (Beziehung), Werte bedingt |
| CT-PH-010 | Eine einzelne Fehlschaltung zerstört die Kupplung nicht; wiederholte Fehlschaltungen akkumulieren Verschleiß | VEH-DEC-PH-021, VEH-DEC-PH-004 | S | abnahmefähig (Beziehung) |

---

## 5. Steuerung (ST)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-ST-001 | Die Standardbelegung entspricht der Vorgabe (R2, L2, L1 + L2, linker und rechter Stick, Kreuz, Quadrat) | VEH-DEC-ST-001 | U | abnahmefähig |
| CT-ST-002 | Jede der acht Funktionen lässt sich frei umbelegen | VEH-DEC-ST-002 | U | abnahmefähig |
| CT-ST-003 | Ein Messprotokoll zu L1 + L2 (Signalweg, Übergang, Ergonomie, Greifpunkt) liegt vor; es gibt keine stille Ersatzbelegung | VEH-DEC-ST-003, VEH-DEC-ST-012 | M | abnahmefähig (Vorgehen) |
| CT-ST-004 | Zwischen Fußgänger- und Fahrzeugkontext entsteht keine Doppelwirkung von Kreuz und Quadrat | VEH-DEC-ST-011, VEH-DEC-ST-015 | U | bedingt (`CK-01`) |

---

## 6. Straßennetz, Karte, Kreuzungen (SN)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-SN-001 | Jede als befahrbar markierte Kante ist für mindestens eine Verkehrsrolle erreichbar und verlassbar | VEH-DEC-WF-012 | S | bedingt (Vorschlag) |
| CT-SN-002 | Jede Parkfläche, Tankstelle und jedes Depot hat eine erreichbare Anschlusskante | VEH-DEC-WF-012, VEH-DEC-SP-009 | S | bedingt (`CQ-102`) |
| CT-SN-003 | Alle Verweise der Spielseiten-Daten zeigen auf vorhandene Karten-IDs | VEH-DEC-WF-011 | S | bedingt (`CQ-99`) |
| CT-SN-004 | Keine Route führt über Kanten ohne Land (Regressionsfall ‚Strecke ins Meer‘) | VEH-DEC-WF-012 | S | abnahmefähig |
| CT-SN-005 | Die Fahrzeiten zwischen Referenzzielen entsprechen der Absicht der Weltgröße-Kalibrierung | VEH-DEC-GP-011 | Z | bedingt (Sollwerte offen) |
| CT-SN-006 | Steigung, Radius und Querungen sind mit den Fahrzeugklassen vereinbar, die die Straßenklasse nutzen dürfen | VEH-DEC-KL-005, VEH-DEC-KL-009 | S | bedingt (`CQ-36`) |
| CT-SN-007 | Jede Kante hat ein Straßenprofil | VEH-DEC-SN-009 | S | bedingt (Vorschlag) |
| CT-SN-008 | Jede Kreuzung hat Prioritäten ohne Widerspruch | VEH-DEC-KR-007, VEH-DEC-KR-008 | S | bedingt (Vorschlag) |
| CT-SN-009 | Der Signalzustand ist eine reine Funktion von Weltzeit und Programm; es gibt kein Konfliktgrün | VEH-DEC-KR-009 | S | bedingt (Vorschlag) |
| CT-SN-010 | Ungültige Verweise nach einem Karten-Neuimport werden gemeldet und nie still entfernt | VEH-DEC-WF-011 | S | bedingt (`CQ-105`) |

---

## 7. Verkehrs-KI und NPC-Fahrten (KI)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-KI-001 | Fahrstilparameter wirken monoton (höhere Regeltreue ergibt weniger Verstöße) | VEH-DEC-KI-006 | S | bedingt (Vorschlag) |
| CT-KI-002 | Der Verkehr bildet eine Rettungsgasse; das Einsatzfahrzeug erreicht sein Ziel | VEH-DEC-SN-007, VEH-DEC-UP-004 | U | abnahmefähig |
| CT-KI-003 | Gesperrte Kanten werden nicht in Routen benutzt | VEH-DEC-VD-011 | S | bedingt (Vorschlag) |
| CT-KI-004 | NPC-Routenwahl, Spielernavigation und Radiomeldung benutzen dieselbe Sperrung | VEH-DEC-KI-007 | S | bedingt (Vorschlag) |
| CT-KI-005 | Über viele deterministische Läufe stirbt kein wichtiger NPC durch einen Simulationsunfall | VEH-DEC-GP-008, VEH-DEC-NF-005 | Z | abnahmefähig |
| CT-KI-006 | Ein NPC ist nie an zwei Orten; die Ankunft liegt in der erwarteten Zeit | VEH-DEC-NF-009, VEH-DEC-LS-012 | S | bedingt (Vorschlag) |
| CT-KI-007 | Aus den Tagesabläufen entsteht zur Stoßzeit eine höhere Dichte als in der Nacht | VEH-DEC-NF-010 | Z | bedingt (`CQ-60`) |
| CT-KI-008 | Bei einer Blockade wählen Fahrer eine Alternativroute, wenn eine besteht | VEH-DEC-NF-014 | Z | bedingt (Vorschlag) |

---

## 8. Verkehrslage (VD)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-VD-001 | Die Dichte folgt dem Profil (zur Stoßzeit höher als nachts, an Veranstaltungsorten erhöht) | VEH-DEC-VD-001, VEH-DEC-VD-007 | Z | bedingt (`CQ-60`) |
| CT-VD-002 | Die Dichte überschreitet die eingestellte Obergrenze nie | VEH-DEC-VD-008 | Z | bedingt (`CQ-61`) |
| CT-VD-003 | Eine Baustelle hat Start und Ende; danach ist die Kapazität wieder da | VEH-DEC-VD-010 | S | bedingt (Vorschlag) |
| CT-VD-004 | Nach einem Zeitsprung wird die Verkehrslage neu abgeleitet und nicht nachsimuliert | VEH-DEC-VD-013 | Z | bedingt (Vorschlag) |
| CT-VD-005 | Übersteigt die Nachfrage die Kapazität, wächst der Verzögerungsfaktor der Kante | VEH-DEC-VD-009 | S | bedingt (`CQ-63`) |
| CT-VD-006 | Sperrungen bei Veranstaltungen erscheinen zur Ereigniszeit und enden danach | VEH-DEC-VD-004 | Z | abnahmefähig |

---

## 9. Unfälle, Pannen, Einsatz (UP)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-UP-001 | Ein Unfallereignis meldet seine Folgen an alle zuständigen Systeme | VEH-DEC-UP-007 | S | bedingt (Vorschlag) |
| CT-UP-002 | Die Einsatzkette durchläuft die Zustände in Reihenfolge und endet mit Wiederherstellung | VEH-DEC-UP-009 | S | bedingt (Vorschlag) |
| CT-UP-003 | Eine Panne hat eine Ursache im Fahrzeugzustand | VEH-DEC-UP-008 | S | bedingt (Vorschlag) |
| CT-UP-004 | Eine Unfallstelle bleibt bis zur Räumung bestehen | VEH-DEC-UP-011, VEH-DEC-VD-006 | Z | abnahmefähig (Tatorte), sonst bedingt |
| CT-UP-005 | Der Notruf erfolgt mit Latenz; die Täterbeschreibung enthält Fahrzeug und Kennzeichen | VEH-DEC-UP-005 | S | abnahmefähig |
| CT-UP-006 | Die Unfallrate ist einstellbar und ohne Spielereinwirkung selten | VEH-DEC-UP-001 | Z | bedingt (`CQ-65`) |

---

## 10. Polizei und Verkehrsüberwachung (PO)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-PO-001 | Ein Verstoß wird nur geahndet, wenn ein Wahrnehmer ihn feststellt | VEH-DEC-PV-003, VEH-DEC-PV-010 | S | abnahmefähig (Prinzip) |
| CT-PO-002 | Eine Kontrolle durchläuft die Zustandsfolge; das Verhalten des Spielers steuert die Eskalation | VEH-DEC-PV-002, VEH-DEC-PV-012 | Z | bedingt (`CQ-81`) |
| CT-PO-003 | Eine Festnahme führt nie zum Game Over | VEH-DEC-PV-004 | Z | abnahmefähig |
| CT-PO-004 | Die Warn-App zeigt nur bekannte Kontrollen mit Verzögerung | VEH-DEC-PV-011 | Z | bedingt (Vorschlag) |
| CT-PO-005 | Die Fahndung nach einem Fahrzeug folgt dem Informationsstand der Polizei | VEH-DEC-PV-003, VEH-DEC-BZ-004 | Z | abnahmefähig |

---

## 11. Persistenz (PE)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-PE-001 | Eine Rundreise aus Speichern und Laden liefert dieselben Fahrzeugdaten | VEH-DEC-PE-001, VEH-DEC-PE-003 | S | abnahmefähig |
| CT-PE-002 | Ein unverändertes Fahrzeug der Stufe C verbraucht keinen Speicherplatz außer ID und Seed | VEH-DEC-PE-002, VEH-DEC-NF-007 | S | bedingt (Vorschlag) |
| CT-PE-003 | Die Migration alter Spielstände ist deterministisch | VEH-DEC-PE-003 | S | abnahmefähig |
| CT-PE-004 | Ein verschrottetes Fahrzeug bleibt als Archivdatensatz abfragbar | VEH-DEC-PE-010 | S | bedingt (Vorschlag) |
| CT-PE-005 | Nach dem Laden ist kein Fahrzeug doppelt vorhanden oder an zwei Orten | VEH-DEC-PE-011 | S | bedingt (Vorschlag) |
| CT-PE-006 | Das Spielerfahrzeug in Fahrt hat nach dem Laden Position, Geschwindigkeit und Gang wie beim Speichern | VEH-DEC-PE-012 | U | bedingt (`CQ-91`) |
| CT-PE-007 | `WorldFacts` enthält nur diskrete Fahrzeugfakten und keinen Fahrzeugzustand | VEH-DEC-PE-006 | S | abnahmefähig |

---

## 12. Koop (KO)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-KO-001 | Fahrzeuge von Gästen liegen im Host-Spielstand | VEH-DEC-KO-001, VEH-DEC-KO-002 | U | abnahmefähig |
| CT-KO-002 | Ein Zeitsprung im Fahrzeug braucht die Zustimmung aller aktiven Spieler | VEH-DEC-KO-003 | U | abnahmefähig |
| CT-KO-003 | Fahrzeug-, Sitz- und Besitzdaten verweisen auf Personen-IDs | VEH-DEC-KO-006 | S | abnahmefähig |
| CT-KO-004 | Ein Fahrzeug ist auf allen Clients konsistent dargestellt | VEH-DEC-KO-007 | U | bedingt (`CQ-95`) |
| CT-KO-005 | Ein Fahrzeug bleibt beim Wechsel zwischen öffentlicher, Freunde- und privater Online-Sitzung im Besitz | VEH-DEC-KO-016 | P | bedingt (Online offen, `CQ-98`) |

---

## 13. Stufen und Streaming (LS)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-LS-001 | Ein Fahrzeug ist zu jedem Zeitpunkt in genau einer Stufe und an genau einem Ort | VEH-DEC-LS-012 | S | bedingt (Vorschlag) |
| CT-LS-002 | Beim Stufenwechsel bleiben Identität, Besitzer, Zustand und Fahrstil gleich | VEH-DEC-LS-009 | S | bedingt (Vorschlag) |
| CT-LS-003 | Beim Wiederbesuch im selben Zeitfenster ist die Lage gleich, wenn nichts geändert wurde | VEH-DEC-LS-008 | Z | bedingt (`CQ-114`) |
| CT-LS-004 | Beim Betreten eines Gebiets ist die Straße nicht leer und nicht überfüllt | VEH-DEC-LS-001, VEH-DEC-GP-006 | U | abnahmefähig (Prinzip), Werte bedingt |
| CT-LS-005 | Kein Fahrzeug existiert auf ungeladenem Boden | VEH-DEC-LS-010 | U | bedingt (Vorschlag) |
| CT-LS-006 | Gleicher Seed und gleiche Zeit ergeben dieselbe Szene | VEH-DEC-KI-010 | Z | bedingt (`CQ-112`) |
| CT-LS-007 | Bei schneller Fahrt sind die Zellen vor dem Spieler geladen | VEH-DEC-LS-010 | U | bedingt (`CQ-111`) |

---

## 14. Leistung (PF)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-PF-001 | Für jedes Messszenario des Moduls 27 liegt ein Messprotokoll vor | VEH-DEC-PF-010 | P | abnahmefähig (Vorgehen) |
| CT-PF-002 | Unter Last wird zuerst degradiert, die Straße bleibt nie leer | VEH-DEC-PF-008 | P | bedingt (Schwellen offen) |
| CT-PF-003 | Die Materialisierung erfolgt gestaffelt ohne Spitze über dem Budget | VEH-DEC-PF-009 | P | bedingt (`CQ-116`) |
| CT-PF-004 | Speicher je Fahrzeug und Stufe ist gemessen | VEH-DEC-PF-006 | P | abnahmefähig (Vorgehen) |

---

## 15. Regression (RG)

| ID | Prüft | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-RG-001 | Die Regressionsfälle der Karte (gerade Fernstraße, Strecke ins Meer, unerschlossene Region) bleiben in der Navigationsprüfung | VEH-DEC-WF-012 | S | bedingt (Karte offen) |
| CT-RG-002 | Eine eingefrorene Fahrzeug-Speicherdatei lädt unverändert und speichert byte-gleich | VEH-DEC-PE-003 | S | bedingt (`CQ-90`) |
| CT-RG-003 | Eine aufgezeichnete Eingabefolge reproduziert dieselbe Fahrt | VEH-DEC-PH-012 | U | bedingt (Vorschlag) |
| CT-RG-004 | Alte Netzversion wird gegen neue Karte migriert, ohne Verweise zu verlieren | VEH-DEC-WF-011 | S | bedingt (`CQ-105`) |

---

## 16. Querschnittsszenarien (SZ)

> **💡 VORSCHLAG.** Szenarien prüfen das Zusammenspiel. Jedes läuft in der Simulation (Z) mit festen Seeds.

| ID | Szenario | Register | Stufe | Abnahme |
|---|---|---|---|---|
| CT-SZ-001 | **Morgenpendler, Unfall, Stau, Einsatz, Räumung:** Pendler fahren aus Tagesabläufen, ein Unfall verringert die Kapazität, ein Stau entsteht, Dienste rücken aus, Meldung im Radio, Räumung, Wiederherstellung | VEH-DEC-NF-010, VEH-DEC-UP-007, VEH-DEC-VD-009, VEH-DEC-UP-009 | Z | bedingt |
| CT-SZ-002 | **Diebstahl und Fahndung:** Der Spieler stiehlt ein geparktes Fahrzeug, ein Zeuge sieht es, Notruf mit Kennzeichen, Kontrolle auf der Straße, Eskalation nach Verhalten | VEH-DEC-BZ-010, VEH-DEC-UP-005, VEH-DEC-PV-002 | Z | bedingt |
| CT-SZ-003 | **Lange Fahrt mit Begleitern:** Panne, Abschleppen, Werkstatt, Zeitsprung, Weiterfahrt | VEH-DEC-UP-008, VEH-DEC-WE-010, VEH-DEC-EI-005 | Z | bedingt |
| CT-SZ-004 | **Schnelle Fahrt über Zellgrenzen mit Verkehr:** Streaming-Vorlauf, Materialisierung, keine Fahrzeuge auf ungeladenem Boden | VEH-DEC-LS-010, VEH-DEC-LS-008 | U | bedingt |
| CT-SZ-005 | **Koop mit zwei Fahrzeugen:** ein Spieler wird verhaftet, die Welt läuft weiter, Zeitsprung nur mit Zustimmung | VEH-DEC-KO-003, VEH-DEC-KO-014 | U | bedingt |
| CT-SZ-006 | **Speichern in Fahrt, Laden, Wiederbesuch:** Spielerfahrzeug identisch, Verkehr logisch gleich | VEH-DEC-PE-012, VEH-DEC-PE-014 | U | bedingt |

---

## 17. Ausbaustufen und Abnahme (Entwurf)

> **💡 VORSCHLAG.** Die Zuordnung benutzt die Phasen aus `CLAUDE.md` §4 (alle Termine und Flächen indikativ, keine Zusage) und den Phasenvertrag aus `D15-RV` (früh einfach, langfristig tief). Die Stufen sind keine Kürzung des Gesamtumfangs: Phasen bestimmen die Reihenfolge, nicht den Umfang.

| Stufe | Inhalt (Fahrzeug) | Inhalt (Verkehr) | Abnahme (Auswahl der Tests) |
|---|---|---|---|
| **Phase 0** Kerntechnik | ein Fahrzeug auf einer Testkarte; manuelles Getriebe mit funktionierender Kupplung; einfacher Zugang und Start; Kraftstoff; einfacher Verschleiß; Fahrzeugdaten und Speichern im Kern | Platzhalter-Straßengraph; höchstens einzelne Test-KI-Fahrzeuge | CT-FU-003, -005, -007; CT-PH-001, -003, -005; CT-ST-001 bis -003; CT-PE-001, -007 |
| **Vertical Slice** (rund 1 bis 3 km², Richtwert) | Anreise im Fahrzeug, Kofferraum als Ausrüstungsbasis, einfache Reparatur, Fahrzeug am Tatort | minimaler Hintergrundverkehr (je nach `CQ-05`), eine Kreuzungsart, Wetter auf Haftung | CT-FU-006, -011; CT-PH-004; CT-SN-004, -008, -009; CT-LS-004; CT-SZ-006 |
| **Phase 2** (rund 5 bis 10 km²) | Werkstatt, Tuning einfach, Gebrauchtmarkt, Taxi und Lieferjobs, Besitz und Zulassung | mehrere Straßenklassen, Parken, NPC-Fahrzeuge aus Tagesabläufen, Unfälle und Pannen, Einsatzkette, Stufen S0 bis S2, WorldForge-Anbindung | CT-SN-001 bis -003; CT-KI-002, -005, -006; CT-UP-001 bis -005; CT-LS-001, -002; CT-SZ-001, -003 |
| **Phase 3** (rund 25 bis 50 km²) | volle Verschleiß- und Schadensschichten, Versicherung (später), Fahrzeuge in Ermittlungen | Stoßzeiten, Baustellen und Sperrungen, Polizeikontrollen und Verfolgung, Verkehrsmenge (SD), Koop-Replikation, Leistungsbudgets gemessen | CT-VD-001 bis -006; CT-PO-001 bis -005; CT-KO-001 bis -004; CT-PF-001 bis -004; CT-SZ-002, -004, -005 |
| **Phase 4+** | nahezu vollständige Werkstattsimulation und Fahrdynamik, Rennsport tief, Bahn und ÖPNV im Betrieb | Weltmaßstab mit S3, Mass-Optionen, Online | Erweiterung des Katalogs nach den Entscheidungen zu `CQ-49`, `CQ-95`, `CQ-98`, `CQ-107` |

**Abnahmeregeln (Vorschlag):**

1. Eine Stufe ist abgenommen, wenn alle zugeordneten **abnahmefähigen** Tests laufen und bestanden sind, die **bedingten** Tests zu entschiedenen Fragen ebenfalls, und der Entwickler die manuellen Prüfungen (Fahrgefühl, Optik) bestätigt hat.
2. Die Berichte nennen, was tatsächlich lief und mit welchem Ergebnis (`CLAUDE.md` §7). Fehlende Läufe werden als fehlend gemeldet.
3. Eine Stufe verlangt nicht, Funktionen späterer Stufen vorwegzunehmen; die Architektur darf sie aber nicht ausschließen (Platzhalter, Datenprofile, Stufen).
