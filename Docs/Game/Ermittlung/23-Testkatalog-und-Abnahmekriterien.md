# Ermittlung 23: Testkatalog und Abnahmekriterien

> [!IMPORTANT]
> **Nichts davon ist implementiert, und kein Test existiert.** Dieses Modul leitet aus den **beschlossenen Anforderungen** und den **Entwurfsregeln** der Module 02 bis 19 überprüfbare Aussagen ab. Es soll später Tests und Abnahmen anleiten, ohne dass jemand die Quellen erneut lesen muss. Die Prüfebenen richten sich nach den Testarten des Projekts (`CLAUDE.md` §5): eigenständige C++-Tests (Core), Unreal-Automation, Handtest. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Aufbau und Ebenen](#1-aufbau-und-ebenen)
2. [Tests der Hilfen und des Detektivmodus](#2-tests-der-hilfen-und-des-detektivmodus)
3. [Tests des Fallmodells](#3-tests-des-fallmodells)
4. [Tests der Beweislogik und des Wissens](#4-tests-der-beweislogik-und-des-wissens)
5. [Tests der Forensik](#5-tests-der-forensik)
6. [Tests der Zeugen und Verhöre](#6-tests-der-zeugen-und-verhöre)
7. [Tests der Identität und des Rufs](#7-tests-der-identität-und-des-rufs)
8. [Tests von Polizei, Gericht und Haft](#8-tests-von-polizei-gericht-und-haft)
9. [Tests von Geheimhaltung und öffentlichem Wissen](#9-tests-von-geheimhaltung-und-öffentlichem-wissen)
10. [Tests des Koops](#10-tests-des-koops)
11. [Tests der Persistenz](#11-tests-der-persistenz)
12. [Inhaltsprüfungen für Autoren](#12-inhaltsprüfungen-für-autoren)
13. [Querschnittstests](#13-querschnittstests)
14. [Abnahmekriterien nach Ausbaustufe](#14-abnahmekriterien-nach-ausbaustufe)
15. [Tests aus der Quellenauswertung (Etappe 4)](#15-tests-aus-der-quellenauswertung-etappe-4)
16. [Was fehlt](#16-was-fehlt)

---

## 1. Aufbau und Ebenen

| Ebene | Bedeutung | Wann möglich |
|---|---|---|
| **C** (Core) | eigenständiger C++-Test ohne Unreal; setzt voraus, dass die Logik im portablen Kern liegt | sobald die Logik gebaut ist |
| **U** (Unreal-Automation) | Test im Spiel (Editor oder `-game`, `-nullrhi`) | sobald die Spielobjekte gebaut sind |
| **M** (Manuell) | Handtest durch den Entwickler (PIE oder Paket) | immer; für Gefühl, Lesbarkeit, Wirkung |
| **I** (Inhalt) | Prüfwerkzeug für Autoren (Validator im Editor oder Build) | sobald Falldaten existieren |

Die Spalte „Herkunft“ nennt die **Anforderung oder Regel**, aus der der Test folgt. Ein Test, dessen Herkunft ein Entwurf (💡) ist, ist ein **Vorschlag**; er wird erst bindend, wenn der Entwurf entschieden ist.

Kein Test hier ist „bestanden“. Es gibt keinen Code.

---

## 2. Tests der Hilfen und des Detektivmodus

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-H01 | Im Detektivmodus zeigt das Spiel keinen Questmarker, keine Suchzone, kein hervorgehobenes Objekt, keinen Beweiszähler, keine automatische Verknüpfung, keine ‚Such hier‘-Meldung | U, M | `M-PD`; [Modul 03 §3](03-Ermittlungsmodi-und-Hilfen.md#3-der-detektivmodus) |
| T-H02 | Im Detektivmodus ist nur der Name und Status des verfolgten Falls sichtbar | U | `M-PD`; `23#54E` |
| T-H03 | Ein vom Spieler gesetzter Kartenmarker erscheint im Detektivmodus; ein vom Spiel gesetzter nicht | U | `23#55A` |
| T-H04 | Jeder Fall ist im Detektivmodus ohne Hilfe lösbar (Pfadprüfung der Falldaten) | I | `23#52E`; V5 |
| T-H05 | Hilfen lesen nur den Wissensstand, nie die Fallwahrheit (Datenzugriffsprüfung der Hilfe-Anbieter) | C, U | R3 |
| T-H06 | Hilfe-Einstellung wechseln ändert keine Fallwahrheit, keinen Fallzustand und kein gespeichertes Wissen | C | R4; S7 |
| T-H07 | Jede Einstellungsachse lässt sich unabhängig von der Kampfschwierigkeit setzen | U | `M-PD`; `48#63E` |
| T-H08 | Hilfen mit Lösungswirkung sind in der Standardeinstellung aus | U | R2 |
| T-H09 | Auf „Detektiv“ wechseln lässt Journal-Einträge bestehen | C, U | Modul 08 §6 |
| T-H10 | Der Referenzweg nach Abschluss ist optional, wird nie als „richtiger Weg“ bezeichnet | U, M | `M-PD`; `23#64E` |
| T-H11 | Der Journaltext zeigt nur, was der Spieler erfahren hat | U | `23#55A` |
| T-H12 | Eine Einstellung, die von einer anderen abhängt (A6 braucht A7), wird erklärt, nicht verborgen | U | R1 |
| T-H13 | Controller und Tastatur bedienen Notizen, Karte und Journal gleichwertig | M | `48#62A`; `23#60E` (100 % Controller) |

---

## 3. Tests des Fallmodells

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-F01 | Ein Fall mit mehreren Auflösungen lässt sich über jede der Auflösungen abschließen | C, U | `23#59A`; V1 |
| T-F02 | Ein Fall wird nach wenigen Hinweisen abgeschlossen, wenn Handeln und Wahrheit übereinstimmen; es werden keine weiteren Schritte verlangt | C | `23#56E`; Modul 10 §4.3 |
| T-F03 | Ein Handeln gegen die Wahrheit führt zum Zustand „Falsch gelöst“, nicht zum Abschluss | C | `23#1062A`; Modul 09 §5.2 |
| T-F04 | „Falsch gelöst“ stellt die Eskalationsuhr des Vorfalls wieder her | C | Modul 09 §6 |
| T-F05 | Jeder erlaubte Zustandsübergang der Tabelle ist möglich, jeder andere abgelehnt | C | Modul 09 §6 |
| T-F06 | Ein Lead, den niemand annimmt, wird nach Ablauf der Frist „Verlaufen“ | C | Modul 09 §6 |
| T-F07 | Ein abgebrochener Fall lässt den Vorfall weiterlaufen; Wiederaufnahme erhält das Wissen | C | Modul 09 §10 |
| T-F08 | Ein Zeitsprung ändert nicht den Fortbestand eines geschützten Falls; der Eintrag bleibt im Journal | C | `48#161E`; Modul 09 §9 |
| T-F09 | Ein dynamischer Auftrag kann beim Zeitsprung verfallen, ein geschützter nicht | C | Modul 09 §9.3 |
| T-F10 | Eskalationsuhr erreicht nach gleicher Zeitspanne bei einem Sprung und bei Einzelschritten denselben Zustand | C | W9; Modul 19 §8 |
| T-F11 | Ein Fall mit Zeugen, der vor der Befragung stirbt, bleibt lösbar oder wechselt bewusst zu „Gescheitert“ | C, I | V3 |
| T-F12 | Ein Vorfall, den mehrere Aufträge betreffen, existiert einmal | C | Modul 09 §12 |
| T-F13 | Ein Fall, der vor der Annahme gelöst wird, erzeugt einen Journal-Eintrag mit dem Erfahrenen | C | Modul 09 §6.1 |
| T-F14 | Die Generator-Pipeline verwirft einen Fall, der V1 bis V12 verletzt | C, I | Modul 09 §8, §13 |
| T-F15 | Der Generator erzeugt gleiche Ergebnisse bei gleichem Zustand und gleicher Saat | C | W9 |
| T-F16 | Die Wiederholungsmarken zeigen, wenn zu viele ähnliche Fälle hintereinander erzeugt werden | I | `23#49E` Punkt 11 |
| T-F17 | Ein Fall, der Hilfen zum Lösen braucht, wird vom Prüfwerkzeug gemeldet | I | V5 |
| T-F18 | Der Wechsel von „gewöhnlich“ zu „Hunter-Fall“ geschieht nur aus definierten Auslösern | C | Modul 07 §4 |

---

## 4. Tests der Beweislogik und des Wissens

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-B01 | Ein Beleg gleicher Herkunft zweimal gefunden erzeugt einen Eintrag | C | Modul 10 §13 |
| T-B02 | Das Wissen des Spielers wächst nur durch Handlung, nie durch den Fall | C | Modul 09 §4.4 |
| T-B03 | Zwei Belege, die dieselbe Behauptung stützen, erhöhen den Status gemäß Regel; ein widersprechender senkt ihn | C | Modul 10 §5.2 |
| T-B04 | Eine als unzuverlässig erkannte Quelle senkt die Stufe ihrer Behauptungen, soweit nicht anderweitig gestützt | C | Modul 10 §5.2 |
| T-B05 | Ein gefälschter Beleg bleibt dem Spieler verborgen, bis eine Prüfung (Forensik, Herkunft) ihn entlarvt | C | Modul 10 §3 |
| T-B06 | Das Spiel weist den Status nicht automatisch zu, wenn die Hilfe aus ist (Spielerstatus) | U | Modul 10 §5.3 (V-A/V-B) |
| T-B07 | Eine Hypothese wird nicht vom Spiel bewertet | U | `23#55A` |
| T-B08 | Die Wahrheitsprüfung erfolgt erst bei einer Handlung (Waffe einsetzen, Ritual, Konfrontation) | C | Modul 10 §8.3 |
| T-B09 | Ein Beleg verfällt in der Welt (Regen), das Foto im Wissensstand nicht | C | Modul 10 §10 |
| T-B10 | Ein vom Spieler bewegter Beleg trägt eine Beweiskette | C | Modul 10 §9 |
| T-B11 | Zwei Behauptungen, die sich widersprechen, werden nebeneinander geführt; der Detektivmodus hebt den Widerspruch nicht hervor | U | Modul 10 §7 |
| T-B12 | Der Wissensstatus ‚historisch/unverifiziert‘ wird für Legenden gesetzt und nicht automatisch bestätigt | C | `M-PD` |
| T-B13 | Das Archiv zeigt eine unbekannte Kategorie nicht; es zeigt „Unbekannt“ für unbekannte Einträge | U | `23#56E`, `23#60E` |
| T-B14 | Zwei Gebiete mit gleich vielen Einträgen zeigen nicht dieselbe Kategorienmischung | I | `23#60E` Punkt 4 |
| T-B15 | Die Hauptgeschichte hängt nie von einem Sammelobjekt ab | I | V2; `M-PD` |

---

## 5. Tests der Forensik

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-X01 | Ein Laborergebnis nennt nie einen Namen ohne Referenz | C | Modul 11 §5.1 |
| T-X02 | Ein Monsterbefund nennt nie eine Art ohne Referenz im Archiv; sonst „Anomalie“ | C | Modul 11 §5.2 |
| T-X03 | Die Todeszeit wird als Bereich, nie als Zeitpunkt ausgegeben | C | `M-CA0905` |
| T-X04 | Eine Probe geht beim Transport durch die Zeit in den Zustand ‚Verdorben‘, wenn die Art verderblich ist | C | Modul 11 §4 |
| T-X05 | Eine kontaminierte Probe liefert ein Ergebnis, das ohne Hinweis falsch sein kann (Detektivmodus) | C | Modul 11 §4 |
| T-X06 | Die Analyse dauert Spielzeit; sie läuft bei Zeitsprung weiter | C | `23#1088A` Frage 90; Modul 11 §8 |
| T-X07 | Dieselbe Probe, zweimal eingereicht, liefert dasselbe Ergebnis | C | Modul 11 §12 |
| T-X08 | Ein Spielerspuren-Treffer entsteht nur, wenn die Behörde eine Referenz besitzt | C | Modul 11 §10 |
| T-X09 | Ein Ergebnis ist eindeutig, wahrscheinlich, möglich, unbestimmt oder widersprüchlich; Prozentzahlen erscheinen nicht | U | Modul 11 §6 |
| T-X10 | Fingerabdrücke sind nicht auf jedem Weltobjekt simuliert | I | `23#1088A` Frage 80 |
| T-X11 | Eine Anomalie nennt nie die Art | C | Modul 11 §9.3 |
| T-X12 | Tatortarbeit in der falschen Reihenfolge verändert die Beweislage | C | Modul 11 §11.2 |

---

## 6. Tests der Zeugen und Verhöre

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-Z01 | Ein NPC kennt ein Ereignis nur, wenn er es wahrgenommen oder erzählt bekommen hat | C | `23#1057E` Punkt 25 |
| T-Z02 | Eine Gerüchtekette verändert die Aussage in jedem Schritt innerhalb festgelegter Grenzen | C | Z5; Modul 12 §12 |
| T-Z03 | Dieselbe Frage, zweimal gestellt ohne Änderung der Welt, ergibt dieselbe Antwort | C | Modul 12 §4 |
| T-Z04 | Die Erinnerung verblasst mit der Zeit nach festgelegter Regel | C | Z2 |
| T-Z05 | Die Aussagebereitschaft ändert sich bei Bedrohung, Bestechung, Vertrauen | C | Modul 12 §5 |
| T-Z06 | Ein bedrohter Zeuge ändert seine Aussage; beide Aussagen bleiben im Journal mit Zeitstempel | C | Z13; Modul 12 §13 |
| T-Z07 | Eine Alibi-Prüfung für einen Tier-C-NPC liefert einen konsistenten Tagesablauf, der bekannten Tatsachen nicht widerspricht | C | Modul 12 §8 |
| T-Z08 | Ein Verhör des Spielers prüft die Aussage gegen die Beweislage der Behörde, nicht gegen eine verborgene Wahrheit | C | Modul 12 §9.2 |
| T-Z09 | Ein verhörtes Wesen gibt nur Wissen preis, das es erworben hat | C | Modul 12 §10 |
| T-Z10 | Ein Zeuge, der die Polizei ruft, löst die Meldekette mit Latenz aus | C | P7 |
| T-Z11 | Das Wiedererkennen des Spielers hängt von Sicht, Kleidung, Maskierung ab | C | `23#1057E` Punkte 28–30 |
| T-Z12 | Ein NPC kann ohne Zeugenstatus gesprochen werden (Alltagsgespräch) | U | `23#1057E` Punkt 31 |

---

## 7. Tests der Identität und des Rufs

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-I01 | Die Wirklichkeit (E1) ändert sich nie durch Überzeugungen (E2 bis E4) | C | Modul 13 §3 |
| T-I02 | Ein Formwandler erzeugt Beobachtungen, die auf den Spieler zeigen; die Behörde führt eine Akte gegen ihn | C | `48#154A` Beispiel |
| T-I03 | Die Entlastung durch einen Gegenbeweis ändert E4 nur, wenn die Behörde ihn annimmt | C | Modul 13 §7 |
| T-I04 | Der Hunter-Ruf ist unabhängig vom Polizeiruf | C | `48#153E` |
| T-I05 | Ein sichtbarer Reputationsbalken existiert nicht | U | Frage 40 |
| T-I06 | Eine regionale Reputation ist eine Verteilung; zwei Regionen können widersprechen | C | Modul 13 §4.3 |
| T-I07 | Eine Tarnidentität hat einen eigenen Ruf | C | Modul 13 §10.2 |
| T-I08 | Medienbekanntheit ist selten | C | Frage 45 |
| T-I09 | Eine Infiltration erzeugt Beziehungen im Zielgruppenzustand; Enttarnung hat Folgen | C | Modul 13 §9.2 |
| T-I10 | Die Hauptgeschichte ist ohne Mitgliedschaft in einer Gruppe abschließbar | I, M | `23#677E` |

---

## 8. Tests von Polizei, Gericht und Haft

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-P01 | Eine unbeobachtete Tat führt nicht zu einer Fahndung gegen den Spieler | C | P1; P12 |
| T-P02 | Jeder Schritt der Meldekette kann scheitern | C | Modul 14 §4 |
| T-P03 | Die Aktenzustände durchlaufen nur zulässige Übergänge | C | Modul 14 §5 |
| T-P04 | Eine schwere Fahndung bleibt bestehen, bis eine Auflösung erfolgt (Verfahren, Freispruch, Zahlung, Rücknahme) | C | P9; `23#1057E` Punkt 65 |
| T-P05 | Die Fahndung wirkt ortsgebunden; Informationen erreichen andere Behörden mit Verzögerung | C | Modul 14 §3.1 |
| T-P06 | Aufgeben ist bei Festnahme eine mögliche Antwort, die nicht in Tötung mündet, sofern der Spieler nicht eskaliert | U | P3 |
| T-P07 | Eine Verkehrskontrolle erkennt ein gesuchtes Kennzeichen | U | Punkt 63; P7 |
| T-P08 | Korruption verhindert oder erzwingt einen Übergang der Akte | C | Modul 14 §10 |
| T-P09 | Festnahme führt nicht zum Spielende | U, M | `23#1057E` Punkt 68 |
| T-P10 | Ein Haft-Zeitsprung lässt die Welt weiterlaufen (Monster, NPCs, Verfahren) | C | `48#159E` |
| T-P11 | Eine verfügbare Nebenmission verschwindet nicht allein durch den Haft-Zeitsprung | C | `48#161E` |
| T-P12 | Ein Verfahren läuft während der Haft weiter | C | `48#159E` |
| T-P13 | Die Strafe richtet sich nach den Faktoren der Strafkette | C | G1 |
| T-P14 | Selbstverteidigung wird nach Handlung bewertet, nicht nach Geschlecht | C | G6 |
| T-P15 | Ein Ausbruch erhöht die Fahndung und verschärft die Sicherheit | C | `48#168A` |
| T-P16 | Gefängnissicherheit passt sich an bekannte Vorfälle an; ohne Wissen bleibt sie gewöhnlich | C | `48#170A`; Modul 16 §11 |
| T-P17 | Der Admin-/Entwicklerpfad für Straftat-Konsequenzen ist in der Veröffentlichung nicht erreichbar | M | `35#636A` |
| T-P18 | Polizei erklärt Vorfälle ohne Wissen zunächst normal | C | Punkt 116 |

---

## 9. Tests von Geheimhaltung und öffentlichem Wissen

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-G01 | Ein virales Video erhöht Gerücht (K1), nicht automatisch Glaube oder Anerkennung | C | `48#148A` |
| T-G02 | Das öffentliche Wissen ändert sich langsam; ein Ereignis verschiebt eine Region nicht vollständig | C | Modul 17 §3.1 |
| T-G03 | Vertuschung hinterlässt mindestens eine auffindbare Spur | C, I | Modul 17 §4.3 |
| T-G04 | Der Spieler kann Zeugen beruhigen, Spuren beseitigen, offenlegen; jede Handlung wirkt auf die Wissensmarken des NPC und die Region | C | `48#149E` |
| T-G05 | Geheime Einrichtungen sind nach der Story-Erstentdeckung suchbar; nach der Enthüllung erscheinen nicht alle auf der Karte | U | `48#173E`; `48#174A` |
| T-G06 | Kleine Einrichtungen reagieren freundlich, große feindselig, ohne dass jeder Mitarbeiter so ist | I | `48#175E`; `48#176A` |
| T-G07 | Die Hauptgeschichte ist abschließbar, wenn das Übernatürliche nie öffentlich wird | I | `48#150A` |

---

## 10. Tests des Koops

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-K01 | Der Fortschritt eines Gastes im Host-Spiel verändert den Gast-Spielstand nicht | C | `48#107E`; `48#108A` |
| T-K02 | Ein Host-Zeitsprung erfordert Zustimmung aller aktiven Spieler; bei Ablehnung läuft die Weltzeit normal | U | `48#163E` |
| T-K03 | Nur der verhaftete Charakter wird eingesperrt | U | `48#161E` |
| T-K04 | Persönliche Entscheidungen trifft der Spieler; Teamentscheidungen werden abgestimmt | U | `48#97E` |
| T-K05 | Bei Gleichstand greift die Regel der Entscheidungsart | U | `48#99E` |
| T-K06 | Das Teamjournal löst Meinungsverschiedenheiten nicht automatisch auf | U | `48#98A` |
| T-K07 | Hilfen sind pro Person | U | Modul 18 §11 |
| T-K08 | Der Krankenhausausfall kann übersprungen werden | U | `48#119E` |
| T-K09 | Fahndung, Akte und Vorstrafen sind pro Charakter | C | Modul 18 §8 |

---

## 11. Tests der Persistenz

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-S01 | Speichern und Laden in jedem Fallzustand ist byte-identisch bei erneutem Speichern | C | S1 |
| T-S02 | Ein abgeschnittener oder beschädigter Spielstand lässt den Zustand unverändert | C | S2 |
| T-S03 | Ein Fallübergang ist atomar; ein Ausfall nach dem Übergang führt nicht zu einem halben Zustand | C | S3 |
| T-S04 | Eine Zeitsprung-Fortschreibung vor und nach dem Speichern ergibt denselben Endzustand | C | S4, S5 |
| T-S05 | Ein Spielstand mit unbekanntem Mod-Fall lädt; Basisfälle bleiben spielbar | C | S6 |
| T-S06 | Die Größe eines Beispielspielstands bleibt unter 50 MB | C | S8; `23#55A` |
| T-S07 | Der Companion erhält nie die Fallwahrheit | U | S9 |
| T-S08 | `StateAt(n)` liefert einen Fall in einem früheren Zustand | C | Modul 19 §9 |
| T-S09 | Zustandscodes bleiben nach Versionswechsel stabil | C | Modul 19 §5.3 |
| T-S10 | Mehrere Fälle gleichzeitig aktiv beeinflussen sich nicht über gleiche Schlüssel | C | Modul 19 §5 |

---

## 12. Inhaltsprüfungen für Autoren

Aus den Prüfregeln V1 bis V12 (Modul 09 §13) und den Einstellungsregeln:

| Nr. | Prüfung | Quelle |
|---|---|---|
| I-01 | Mindestens eine Auflösung ohne Pflichtziel | V1 |
| I-02 | Kein Einzelpunkt-Ausfall (Zeuge, Gegenstand, Sammelobjekt) | V2 |
| I-03 | Nach dem Ausfall eines Wegs bleibt eine Auflösung erreichbar | V3 |
| I-04 | Keine Abhängigkeit von sozialen Medien | V4 |
| I-05 | Lösbar im Detektivmodus | V5 |
| I-06 | Hilfen nur auf Wissensstand | V6 |
| I-07 | Fallorte einmalig und vorhanden | V7, V8 |
| I-08 | Pausierbar | V9 |
| I-09 | Speichern und Laden unverändert | V10 |
| I-10 | Mod-Fall überschreibt nichts | V11 |
| I-11 | Wiederholungsmarken gesetzt | V12 |
| I-12 | Jeder Hinweis nennt seine Herkunft und seine Zuverlässigkeitsarten | Modul 10 §6 |
| I-13 | Falsche Fährten sind als solche markiert (für den Autor, nicht für den Spieler) | `23#1074A` Frage 82 |
| I-14 | Jede Entscheidung im Koop ist als persönlich oder gemeinsam markiert | Modul 18 §4.2 |
| I-15 | Jeder Fall nennt seine Schutzstufe | Modul 09 §9.3 |
| I-16 | Wesen in Tarnung haben erkennbare, erforschbare Merkmale | Modul 13 §8 |

---

## 13. Querschnittstests

| Nr. | Test | Ebene |
|---|---|:---:|
| T-Q01 | Ein vollständiger Fall von Signal bis Lösung, mit Zeugen, Spur, Labor, Polizei, Journal, in der Testumgebung durchgespielt (Vertical-Slice-Fall) | U, M |
| T-Q02 | Derselbe Fall im Detektivmodus und im geführten Modus: gleiche Wahrheit, gleiche Auflösung | U |
| T-Q03 | Ein Fall, in dem der Spieler zu Unrecht verdächtigt wird, wird durch eigene Ermittlung entlastet | U, M |
| T-Q04 | Ein Hunter wird verhaftet, überspringt die Haft, kehrt zurück, die Welt hat sich verändert, der Fall besteht fort | U, M |
| T-Q05 | Ein Koop-Fall mit zwei Spielern, ein Spieler verhaftet, der andere sammelt Beweise | U, M |
| T-Q06 | Ein Fall mit Vertuschung wird durch Widersprüche aufgedeckt | U, M |
| T-Q07 | Ein Speichern in jedem Schritt von T-Q01 und Laden führt zum selben Verlauf | C, U |

---

## 14. Abnahmekriterien nach Ausbaustufe

### 14.1 Was die Quellen zur Priorität sagen

Der Block 10P in `23#1088A` (Fragen 182–185) ordnet Systeme in Prioritäten; der Entwickler bestätigte die Reihenfolge mit „182-184 ja“ (`23#1089E`):

| Priorität | Inhalt laut Frage 182 bis 184 | Status |
|---|---|:---:|
| **P1 / Kern** | Beziehungen, Companions, Hund, Stealth, Tracking, **Investigation/Forensics**, Gesundheit/Verletzung, vernünftiges Inventar, Smartphone/Internet | 📋 Richtung |
| **P2 / danach** | tiefere Romanzen, Social Stealth, Tourismus, Holidays, Urbex, Marketplace/Auktionen, Kleidungstiefe | 📋 Richtung |
| **P3 / langfristig** | Generationensimulation, Familiengründung, extreme Wirtschafts-/Lebenssimulation, sehr tiefe Versicherungen, eigene Unternehmen, komplexe Prothesen-/Langzeitrehabilitation | 📋 Richtung |

Frage 185: Alle diese Systeme sollen das Spiel realistischer und freier machen, aber „niemals die Hunter-/Investigation-Hauptidentität unter einer Million Pflichtmechaniken begraben“; Antwort des Entwicklers: „genau, aber wenn man will kann man vieles gleichzeitig machen und soll das auch machen können“ (`23#1089E` Punkt 185). Daraus folgt: Die Ermittlung gehört zum **Kern (P1)**; Gefängnis, Gerichte, Geheimhaltungssimulation und Tarnidentitäten sind damit **nicht** automatisch Kern, sondern hängen an den Phasenzuordnungen der Quiz-Antworten (`[VS]`/`[EARLY]`/`[LATER]`/`[FUTURE]`, Vorschläge des Assistenten).

### 14.2 Entwurf der Stufen

> [!NOTE]
> **Nur Vorschlag (💡).** Die Stufen übernehmen die Produktionskennzeichen aus den Quellen (`[VS]`, `[EARLY]`, `[LATER]`, `[FUTURE]`) nur dort, wo der Assistent sie in den Quizantworten vergeben hat (Gefängnis `48#158A`, Dialogsystem `48#152A`); der Master legt sie für die übrigen Bereiche nicht fest. Die Prioritäten P1 bis P3 aus Abschnitt 14.1 gelten zusätzlich.

| Stufe | Inhalt (Vorschlag) | Abnahme |
|---|---|---|
| **Kern (VS)** | ein Fall mit Zeugen, Spur, einer Probe, Journal; Detektivmodus; Speichern und Laden; einfache Festnahme mit Konsequenzen | T-Q01, T-Q02, T-Q07, T-H01 bis T-H06, T-S01 bis T-S04, T-P09 |
| **Früh** | mehrere Fälle, Eskalationsuhr, dynamische Aufträge, Polizeiakten, Verhör, Zeitsprung | T-F, T-B, T-X, T-Z, T-P01 bis T-P05 |
| **Später** | Ruf und Identität, Formwandler, Gerichte, spielbare Haft, Ausbrüche, Geheimhaltung, Koop | T-I, T-P06 bis T-P18, T-G, T-K |
| **Zukunft** | tiefe Justizsimulation, Gefängnisgesellschaft, KI-gestützte Dialoge | offen |

---

## 15. Tests aus der Quellenauswertung (Etappe 4)

Ergänzung zu den Abschnitten 2 bis 13. Herkunft sind die in dieser Etappe nachgetragenen Quellenfunde.

| Nr. | Test | Ebene | Herkunft |
|---|---|:---:|---|
| T-F19 | Nicht beobachtetes Monsterverhalten wird dem Spieler nicht magisch bekannt; beobachtetes kann gelernt werden | C, U | `23#293A` Punkt 24 |
| T-F20 | Das Archiv zeigt nichts Unbekanntes | C, U | `23#293A` Punkt 24; `23#307A` Punkt 24 |
| T-F21 | Ein Fallergebnis aktualisiert den Weltzustand; Save/Load erhält den Monsterzustand | C | `23#293A` Punkt 24 |
| T-F22 | Ein Mod-Monster lässt sich registrieren und überschreibt keine Basisdaten | C | `23#293A` Punkt 24 |
| T-F23 | Ein Fall, dessen Zeitfenster verpasst wurde, ist im nächsten Zeitfenster wieder möglich oder besteht mit geänderter Lage | C | `23#307A` Punkt 22; `23#308E` |
| T-F24 | Ein Fall entsteht erst, wenn der Weltzustand die Voraussetzungen erfüllt; vorher erscheinen Vorlauf-Signale, kein Marker | C, U | `23#307A` Punkt 21; `23#308E` |
| T-F25 | Eine Hauptmission und eine Nebenmission sind nie dauerhaft verpassbar; ein Fall mit veränderter Welt wird zur Fallvariante, nicht gelöscht | C, I | `23#582E`; `23#583A` |
| T-F26 | Freischaltung eines Kapitelfalls geschieht über den Story-Zustand, nicht über geografische Sperren | C | `23#579A`; `23#580E` |
| T-F27 | Ein Beitrag im Feed löst keine Mission aus; er ist Quelle; ohne Feed bleibt jeder Pflichtfall lösbar | C, I | `23#622A`; `23#623E` |
| T-M01 | Die Ziele einer Mission werden vor dem ersten Abschluss nicht angezeigt, danach schon | U | `23#584E` |
| T-M02 | Die Bewertungsstufe kann schon im ersten Durchgang erreicht werden; ein Replay verändert den Story-Zustand nicht | C, U | `23#584E`; `23#585A` |
| T-M03 | Bei Missionen mit mehreren legitimen Lösungswegen bestraft die Bewertung keinen vorgesehenen Weg | I | `23#587A` |
| T-P19 | Das Spiel zeigt für Polizeifahndung keine Sterne; interne Stufen wirken über Welt (Streifen, Sperren, Medien) | U, M | `23#612A` Punkt 16; offen K-E8 |
| T-P20 | Der Verdachtsgrad der Behörde durchläuft Unknown, Possible, Likely, Identified, Confirmed nur in dieser Ordnung | C | `23#618A` Punkt 23 |
| T-P21 | Ein Fahrzeugwechsel entfernt das Kennzeichen als Hinweis, nicht Gesicht oder Kleidung | C | `23#618A` Punkt 24 |
| T-P22 | Auf Hardcore/immersiv erscheinen keine Polizeipositionen auf der Minimap | U | `23#612A` Punkt 15 (Wunsch; Richtung) |
| T-P23 | Eine fallen gelassene Tatwaffe bleibt in der Welt, kann gesichert werden und wird beim Aufräumen nicht gelöscht, solange sie Beweis ist | C | `23#628A` Punkte 6–7 |
| T-P24 | Die Weltreaktionen urteilen nach Tat, Kontext und Beweis, nicht nach Gruppenzugehörigkeit (gleiche Tat, zwei Gruppen, gleiches Urteil bei gleicher Beweislage) | C | `35#537A`; `35#538E` |
| T-G08 | Ein einzelner Beitrag ohne Beleg erhöht nur das Gerücht, nicht Glauben, belegtes Wissen oder Anerkennung | C | `48#148A`; `23#622A` |
| T-K10 | Beweis- und Fallzustand eines Online-Spielers ändert sich nicht, wenn ein anderer Spieler auf demselben Server denselben Fallanlass untersucht (personenbezogener Zustand) | C | `23#444E` |
| T-K11 | Bei einem Gruppenjob gilt der Fall für alle Teilnehmer als abgeschlossen; Nicht-Teilnehmer bleiben unberührt | C | `23#446E` |
| T-K12 | Koop-Fortschritt und Cheats wirken nie auf den Online-Raum und umgekehrt | C | `23#440E`; `23#553E` |
| T-S12 | Manuelles Speichern und Quicksave sind gesperrt, solange ein Gegner den Spieler entdeckt hat; ein nur misstrauischer Gegner sperrt nicht | C | `23#368E`; `23#370A` (Auslegung) |
| T-S13 | Nach einem Tod führt das Laden nicht zu voller Gesundheit | C | `23#365A`; `23#366E` |
| T-Q08 | Ein Hund als Begleiter folgt dem Spieler in den Fall und beeinflusst den Fall-Beweiszustand nicht ohne Spieleraktion | C, U | `23#1027E` Punkt 22 (Mechanik ❓) |
| T-Q09 | Ein Freizeit-Hinweis erzeugt einen Auftrag, aber keine auf der Karte markierte Nebenmission | C, I | `23#1035E` Punkt 125 |
| T-I11 | Ein überdecktes oder gelasertes Tattoo entwertet eine auf dem Tattoo beruhende Personenbeschreibung | C | `35#749E` Punkt 1 |

---

## 16. Was fehlt
- **Tests für die Module 24 bis 27** (Detektivpfad, Kriminalität, Ereignisse, Medien) sind **noch nicht abgeleitet**; der Katalog deckt die Module 02 bis 19 ab.
- **Zahlen** für alle Toleranzen (Dauer, Schwelle, Wahrscheinlichkeit): ohne Entscheidung kein prüfbarer Wert.
- **Testdaten** (Fall, Zeuge, Probe) für die Kernstufe: bewusst nicht erfunden.
- **Messung der Leistung** (Zeitsprung, Hintergrundsimulation).
- **Barrierefreiheits- und Lokalisierungstests**.
- **Sicherheitstests** für Mods.
- Eine **Abnahme** der Darstellung von Gewalt im Gefängnis und bei Verhören (Altersfreigabe, nicht in den Quellen).
