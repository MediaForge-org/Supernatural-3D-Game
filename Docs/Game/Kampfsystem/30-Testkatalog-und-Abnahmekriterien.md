# Kampfsystem 30: Testkatalog und Abnahmekriterien

> [!IMPORTANT]
> **Nichts davon wurde ausgeführt.** Der Katalog beschreibt künftige Prüfungen für spätere Implementierungsmeilensteine. **Unbeschlossene Mechaniken sind keine verbindlichen Abnahmekriterien:** Jeder Testfall trägt einen Status. `abnahmefähig` heißt, dass er nur auf 📋 BESCHLOSSEN oder 🧩 ABGELEITET beruht; `bedingt` heißt, dass er an eine offene Frage (`CQ-…`) gebunden ist und erst nach der Entscheidung gilt; `Vorschlag` heißt, dass er aus einem Entwurf stammt. Keine Zahl in einem Test ist eine Balance-Vorgabe; wo Werte nötig sind, steht ‚Konfigurationswert‘. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 27](27-Entscheidungsregister.md). Szenarien: [Modul 26](26-Querschnittsszenarien.md).

**Inhalt**

1. [Grundsätze](#1-grundsätze)
2. [Teststufen und Werkzeuge](#2-teststufen-und-werkzeuge)
3. [Funktionale Tests (FU)](#3-funktionale-tests-fu)
4. [Steuerungs- und Eingabetests (ST)](#4-steuerungs--und-eingabetests-st)
5. [Animations- und Bewegungstests (AB)](#5-animations--und-bewegungstests-ab)
6. [Physiktests (PH)](#6-physiktests-ph)
7. [Verletzungs- und Medizintests (VM)](#7-verletzungs--und-medizintests-vm)
8. [Gegner-KI-Tests (KI)](#8-gegner-ki-tests-ki)
9. [Kreaturen- und Schwächentests (KW)](#9-kreaturen--und-schwächentests-kw)
10. [Persistenztests (PE)](#10-persistenztests-pe)
11. [Koop- und Netzwerktests (KN)](#11-koop--und-netzwerktests-kn)
12. [Performance- und Skalierungstests (PL)](#12-performance--und-skalierungstests-pl)
13. [Regressionstests (RG)](#13-regressionstests-rg)
14. [Fehler- und Wiederherstellungstests (FE)](#14-fehler--und-wiederherstellungstests-fe)
15. [Szenariotests (SZ)](#15-szenariotests-sz)
16. [Abnahmekriterien nach Ausbaustufe (Entwurf)](#16-abnahmekriterien-nach-ausbaustufe-entwurf)

---

## 1. Grundsätze

1. **Tests folgen Entscheidungen, nicht umgekehrt.** Jeder Test nennt die Register-ID, aus der er folgt (`CLAUDE.md` §7: ‚derive tests from the confirmed ones‘).
2. **Eigenschaften statt Zahlen.** Wo die Quellen keine Zahlen nennen, prüft der Test Beziehungen (monoton, erhalten, deterministisch, getrennt), nie einen Wert.
3. **Determinismus.** Tests des portablen Kerns bekommen Zeitschritt und Zufall von außen; derselbe Seed ergibt dasselbe Ergebnis (`COMBAT-DEC-ZA-008`).
4. **Keine Fälschung von Ergebnissen.** Berichte nennen, was tatsächlich lief (`CLAUDE.md` §7). Dieses Dokument enthält keine Ergebnisse.
5. **Manuelle Prüfung bleibt nötig** für Gefühl, Animation, Audio, Bildqualität (`COMBAT-DEC-AN-011`).

---

## 2. Teststufen und Werkzeuge

| Stufe | Inhalt | Voraussetzung |
|---|---|---|
| S (Standalone C++) | portabler Kern (Zustand, Ballistik-Rechnung, Verletzungsmodell, KI-Wissensmodell, Speicherformat) ohne Unreal | CMake, Test-Framework des Projekts (doctest, vorhanden), Fedora |
| U (Unreal-Automation) | Aktoren, Traces, Animation, Eingabe, Kamera, Replikation | Unreal-Binärdateien (Windows-Rechner), kein Editor-GUI nötig |
| P (Paketiertes Spiel) | Verhalten im gebauten Spiel | Paketbuild |
| M (manuell) | Gefühl, Animation, Audio, Horror-Wirkung | Entwickler |
| Z (Simulation) | deterministische Szenario- und Soak-Läufe mit Seeds | Test-Orchestrator (Gameplay-Systeme §17.2) |

Die Stufen S, U, P unterscheiden sich wie in `CLAUDE.md` §5: standalone, Unreal-Kommandozeile, paketiert.

---

## 3. Funktionale Tests (FU)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-FU-001 | Zwei menschliche Kämpfer, gleiche Anatomie | Gleichen Treffer auf Spieler und NPC anwenden | Gleiche Wunde und gleiche Folge | TRF MED | COMBAT-DEC-GP-007, COMBAT-DEC-TK-011 | abnahmefähig |
| CT-FU-002 | Kein Lebenspunktwert als Primärzustand | Viele Treffer geringer Schwere ohne Gefäßtreffer | Tod folgt nur aus Körperzustand, nicht aus Trefferzahl | MED | COMBAT-DEC-TK-006, COMBAT-DEC-BW-005 | abnahmefähig |
| CT-FU-003 | Schusswaffe geladen | Schuss auslösen | Ein Projektil entsteht, Patronenlager und Magazin ändern sich getrennt | WAF MUN | COMBAT-DEC-MU-008, COMBAT-DEC-FK-001 | abnahmefähig |
| CT-FU-004 | Schwache Hand, Schussarm verletzt | Pistole mit schwacher Hand ziehen | Handlung möglich, mit reduzierter Kontrolle und langsamerer Handhabung | WAF MED | COMBAT-DEC-FK-007 | abnahmefähig |
| CT-FU-005 | Mehrere Gegner, enge Tür | Gegner greifen gleichzeitig an, Spieler tritt in die Tür | Nicht alle können gleichzeitig angreifen; kein Stun-Lock | KI BEW | COMBAT-DEC-NK-018, COMBAT-DEC-NK-019 | abnahmefähig |
| CT-FU-006 | Grappling-Eintritt, mehrere Gegner | Spieler versucht Clinch | Eintritt wird nicht gesperrt | STE KI | COMBAT-DEC-NK-020 | abnahmefähig |
| CT-FU-007 | Glasflasche | Hart zuschlagen | Flasche zerbricht nach einem Schlag; Rest kann bleiben | WAF | COMBAT-DEC-NK-022 | abnahmefähig |
| CT-FU-008 | Fight Club, Aufgabe | Aufgabegriff, Gegner klopft ab | Griff wird gelöst; kein schwerer Schaden | NK POL | COMBAT-DEC-NK-013 | bedingt (CQ-13) |
| CT-FU-009 | Würgegriff außerhalb des Fight Clubs | Griff nach Bewusstlosigkeit fortsetzen | Tod ist möglich, abhängig vom Körperzustand | MED | COMBAT-DEC-NK-012 | abnahmefähig |
| CT-FU-010 | Ammo-HUD aus | Magazin zählen und prüfen | Keine Zählanzeige; Waffen- und Magazinprüfung zeigt den Stand | UI WAF | COMBAT-DEC-FK-012, COMBAT-DEC-MU-022 | abnahmefähig |
| CT-FU-011 | Zielhilfe aus | Schlag auf ausgewichenen Gegner | Schlag geht ins Leere, keine Zielnachführung | STE | COMBAT-DEC-NK-006, COMBAT-DEC-DF-006 | abnahmefähig |
| CT-FU-012 | Finisher-Kandidat | Finisher bei beweglichem Gegner anfordern | Abgelehnt mit Grund | STE | COMBAT-DEC-NK-030 | abnahmefähig |
| CT-FU-013 | Schweres Wesen (Testdatensatz) | Blocken mit beiden Armen | Block hat keine Wirkung; Rückmeldung über Animation | KRE | COMBAT-DEC-DF-002 | abnahmefähig |
| CT-FU-014 | Stealth-Takedown an Mensch und an Testwesen ohne Takedown-Regel | Takedown versuchen | Beim Menschen fehlbar, beim Wesen abgelehnt oder artabhängig, kein Sofort-Tod | KI KRE | COMBAT-DEC-NK-041 | abnahmefähig |
| CT-FU-015 | Waffe fällt | Entwaffnung durch Treffer auf den Arm | Waffe liegt physisch am Boden, nicht im Inventar | WAF | COMBAT-DEC-NK-045 | abnahmefähig |

---

## 4. Steuerungs- und Eingabetests (ST)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-ST-001 | DualSense, Gamepad-Layout | Kreuz drücken | Springen wird aktiv ausgelöst | STE BEW | COMBAT-DEC-ST-009, COMBAT-DEC-BT-001 | abnahmefähig |
| CT-ST-002 | Springen ohne Eingabe | Auf ein Hindernis zulaufen | Kein automatisches Springen | BEW | COMBAT-DEC-BT-001 | abnahmefähig |
| CT-ST-003 | Controller ohne Maus | Alle Bereiche (Menü, Inventar, Archiv, Karte, Dialog, Fahrzeug, Kampf, Modbrowser) bedienen | Alles ist ohne Maus bedienbar | STE UI | COMBAT-DEC-ST-001 | abnahmefähig |
| CT-ST-004 | Neubelegung im Menü | Jede relevante Aktion neu belegen | Jede Aktion ist neu belegbar | STE | COMBAT-DEC-ST-002 | abnahmefähig |
| CT-ST-005 | Störung der Waffe | Eine kontextabhängige Eingabe drücken | Die zur Störung passende Behebung wird ausgelöst, keine Kombination nötig | STE WAF | COMBAT-DEC-MU-017, COMBAT-DEC-ST-004 | abnahmefähig |
| CT-ST-006 | Stick wenig ausgelenkt | Gehen und Schleichen | Langsamer und leiser ohne Pflicht-Schleichtaste | STE KI | COMBAT-DEC-ST-005 | abnahmefähig |
| CT-ST-007 | Eine Absicht ist nicht erlaubt (Griff, Bewusstlosigkeit) | Aktion auslösen | Ablehnung mit benanntem Grund, kein stummes Ignorieren | STE | COMBAT-DEC-ST-019 | Vorschlag |
| CT-ST-008 | Zwei Absichten gleichzeitig möglich | Beide Eingaben | Deterministische Vorrangregel; die andere erhält einen Grund | STE | COMBAT-DEC-ST-018, COMBAT-DEC-ST-019 | bedingt (CQ-25) |
| CT-ST-009 | Quadrat in der Nähe von Gegner und Tür | Taste drücken | Verhalten folgt der festgelegten Umschaltregel | STE | COMBAT-DEC-ST-011 | bedingt (CQ-01, CQ-25) |
| CT-ST-010 | Fahrzeug, DualSense | Kreuz und Quadrat im Fahrzeug | Fahrbelegung gilt (Hochschalten, Herunterschalten) | STE | COMBAT-DEC-ST-010 | bedingt (Fahrzeugmeilenstein) |
| CT-ST-011 | Nahkampfbibliothek mit gelernter und unbekannter Technik | Menü öffnen | Nur gelernte Techniken vollständig, Unbekanntes höchstens Name | UI | COMBAT-DEC-NK-038 | bedingt (CQ-02, CQ-12) |
| CT-ST-012 | Grounded-Preset | HUD und Hilfen prüfen | Komfort- und HUD-Hilfen standardmäßig aus; einzeln einschaltbar | UI SCH | COMBAT-DEC-GP-015, COMBAT-DEC-ST-007 | abnahmefähig |
| CT-ST-013 | Barrierefreiheit aktiv in Grounded | Eingabehilfe einschalten | Barrierefreiheit ist nicht durch Schwierigkeit gesperrt | UI SCH | COMBAT-DEC-GP-016 | abnahmefähig |
| CT-ST-014 | Kampf gegen Gegner, Eingabe der Verteidigung | Ausweichen auslösen | Plausible Bewegung (kein Rollen-Spam), Ausdauerkosten nach Konfiguration | STE BEW | COMBAT-DEC-DF-006 | bedingt (CQ-28) |

---

## 5. Animations- und Bewegungstests (AB)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-AB-001 | Hindernis 50 cm | Springen | Kein übertriebener Sprung; Bewegung passt zur Höhe | BEW ANI | COMBAT-DEC-BT-007, COMBAT-DEC-BT-005 | abnahmefähig |
| CT-AB-002 | Sprungwert aus Konfiguration | Scheitelhöhe messen | Sprunghöhe stammt aus der Konfiguration; Ziel „menschlich“ | BEW | COMBAT-DEC-BT-002 | bedingt (CQ-31) |
| CT-AB-003 | Beinverletzung | Laufen und Sprinten | Hinken sichtbar; Sprint eingeschränkt; Animation entspricht der Bewegung | ANI MED | COMBAT-DEC-VL-008, COMBAT-DEC-AN-004 | abnahmefähig |
| CT-AB-004 | Armverletzung | Waffe führen und nachladen | Sichtbar schlechtere Waffenführung; Nachladen langsamer | ANI WAF | COMBAT-DEC-NK-026, COMBAT-DEC-AN-003 | abnahmefähig |
| CT-AB-005 | Zustand mit mehreren Wunden | Verletzungsschichten aktiv | Schichten ergeben eine stimmige Pose, deterministisch bei gleicher Priorität | ANI | COMBAT-DEC-AN-003 | bedingt (CQ-87) |
| CT-AB-006 | Pumpgun, Pistole, Revolver | Nachladen und Bedienen | Mechanisch korrekter Ablauf (Vorderschaft, Schlitten, Trommel) | ANI WAF | COMBAT-DEC-AN-005, COMBAT-DEC-FK-011 | abnahmefähig (M) |
| CT-AB-007 | Kamera an Wand | Rückwärts an die Wand, dann abwenden | Kamera bleibt außerhalb der Geometrie; Arm wächst begrenzt | ANI | COMBAT-DEC-BT-005; Proposal 0008 §12 | abnahmefähig |
| CT-AB-008 | Griff an Wand | Clinch mit Wand | Kamera, Pose und Mesh bleiben konsistent | ANI | COMBAT-DEC-NK-008, COMBAT-DEC-NK-033 | bedingt (CQ-08) |
| CT-AB-009 | Paarzustand mit verschiedenen Körpergrößen | Takedown | Anpassung über Warping; Ablehnung mit Grund außerhalb der Grenzen | ANI | COMBAT-DEC-AN-010 | bedingt (CQ-87) |
| CT-AB-010 | Fehlende Animation | Zustand ohne Clip | Platzhalter und Protokoll, kein Absturz | ANI | COMBAT-DEC-AN-014 | Vorschlag |
| CT-AB-011 | Verletzter Gegner | Beintreffer | Gegner reagiert sichtbar (Arm schützen, humpeln), nicht unverändert | ANI KI | COMBAT-DEC-AN-006, COMBAT-DEC-TK-015 | bedingt (spätere Version) |

---

## 6. Physiktests (PH)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-PH-001 | Projektil mit Anfangszustand | Flug in Zeitschritten | Energie nimmt nie zu; Fall und Luftwiderstand wirken | TRF | COMBAT-DEC-FK-013 | abnahmefähig |
| CT-PH-002 | Sehr große Entfernung | Schuss | Projektil verschwindet nicht an einer künstlichen Grenze | TRF | COMBAT-DEC-FK-014 | abnahmefähig |
| CT-PH-003 | Dünne Holztür, dann massive Wand | Durchschuss | Durchschlag mit Energieverlust, dann Stopp; Restenergie sinkt monoton | TRF | COMBAT-DEC-FK-016 | abnahmefähig |
| CT-PH-004 | Massiver Beton | Beschuss | Projektil stoppt, kosmetische Einschläge | TRF | COMBAT-DEC-FK-016 | abnahmefähig |
| CT-PH-005 | Bewegtes Ziel | Schuss mit Vorhalt | Treffer folgt aus Flugzeit und Zielbewegung | TRF | COMBAT-DEC-FK-020 | abnahmefähig |
| CT-PH-006 | Bergauf und bergab | Schüsse in gleichem Abstand | Winkel wirkt physikalisch korrekt (Richtung, nicht Betrag geprüft) | TRF | COMBAT-DEC-FK-021 | abnahmefähig |
| CT-PH-007 | Standardeinstellung (Grounded aus) und Grounded | Flacher Winkel auf harten Untergrund | Abprallen ist selten; nur Grounded standardmäßig aktiv; einschaltbar | TRF SCH | COMBAT-DEC-FK-018 | abnahmefähig |
| CT-PH-008 | Beton wird getroffen | Splitterbildung | Splitter sind kosmetisch; kein dauerhafter Zusatzschaden außer Sonderfall | TRF | COMBAT-DEC-FK-019 | abnahmefähig |
| CT-PH-009 | Schalldämpfer, Überschallmunition | Schuss | Nicht lautlos; Knall hörbar | AUD KI | COMBAT-DEC-FK-023 | abnahmefähig |
| CT-PH-010 | Trefferweg durch Körper | Seed fest | Gleiche Eingabe liefert gleichen Wundeintrag | TRF MED | COMBAT-DEC-ZA-001, COMBAT-DEC-ZA-008 | abnahmefähig |
| CT-PH-011 | Schrot aus der Nähe und Pistole | Treffer gleicher Region | Verschiedene Wunden und Schwere | TRF | COMBAT-DEC-MU-003, COMBAT-DEC-GP-020 | abnahmefähig |
| CT-PH-012 | Waffe schwer und leicht | Rückstoß | Rückstoß folgt dem Geschoss- und Waffenprofil; Training ändert nicht den Impuls | WAF | COMBAT-DEC-FK-005 | abnahmefähig |
| CT-PH-013 | Sturz aus verschiedenen Höhen | Landen | Höhere Höhe verletzt nicht weniger | TRF MED | COMBAT-DEC-BT-009, COMBAT-DEC-BT-010 | abnahmefähig |
| CT-PH-014 | Treffer wird auf Impuls geprüft | Starker Treffer | Verhalten folgt der Entscheidung zu Impuls | ANI | COMBAT-DEC-TK-022 | bedingt (CQ-48) |

---

## 7. Verletzungs- und Medizintests (VM)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-VM-001 | Großes Gefäß getroffen | Ohne Behandlung warten | Blutvolumen sinkt, Schock steigt, Bewusstsein sinkt, Tod möglich | MED | COMBAT-DEC-TK-004, COMBAT-DEC-VL-011 | abnahmefähig |
| CT-VM-002 | Gleiche Region, drei Pfade | Muskel, Knochen, Gefäß | Drei verschiedene Wundeinträge und Folgen | MED TRF | COMBAT-DEC-TK-002, COMBAT-DEC-TK-006 | abnahmefähig |
| CT-VM-003 | Wunde behandelt | Blutung stoppen, reinigen, verbinden | Verband verhindert Verbluten, heilt nicht sofort | MED | COMBAT-DEC-VL-001, COMBAT-DEC-VL-002 | abnahmefähig |
| CT-VM-004 | Region wird erneut getroffen | In Stufe „heilend“ | Schwere Zustände schneller als in „erholt“ | MED | COMBAT-DEC-VL-005, COMBAT-DEC-ZA-007 | abnahmefähig |
| CT-VM-005 | Schmerzmittel | Einnehmen | Wirkt verzögert; Symptomlinderung; heilt nicht | MED | COMBAT-DEC-VL-004 | abnahmefähig |
| CT-VM-006 | Adrenalin im Kampf | Verletzung im Kampf, dann Ruhe | Wahrnehmung verschiebt sich, Zustand bleibt gleich | MED | COMBAT-DEC-VL-007, COMBAT-DEC-AU-008 | abnahmefähig |
| CT-VM-007 | Verbliebenes Projektil | Behandlung | Entfernung nicht automatisch nötig; hängt von Lage und Zustand ab | MED | COMBAT-DEC-VL-015 | abnahmefähig |
| CT-VM-008 | Innere Verletzung | Direkt nach dem Treffer | Befund unsicher; Symptome später; professionelle Diagnose liefert Details | MED UI | COMBAT-DEC-VL-009, COMBAT-DEC-TK-009 | abnahmefähig |
| CT-VM-009 | Schwere Verletzung (Krankenhausfall) | Feldbehandlung | Feld stabilisiert, ersetzt die Krankenhausbehandlung nicht | MED | COMBAT-DEC-VL-003 | abnahmefähig |
| CT-VM-010 | Spieler und NPC gleicher Anatomie | Gleichen Pumpgun-Schuss aus nächster Nähe | Spieler kann sterben (kein Plot-Armor) | MED | COMBAT-DEC-GP-008, COMBAT-DEC-GP-007 | abnahmefähig |
| CT-VM-011 | Übernatürlicher Modifikator (Heilungshemmung) | Wunde mit Modifikator | Normale Wunde plus Modifikator, kein zweites Gesundheitssystem | MED KRE | COMBAT-DEC-TK-010 | abnahmefähig |
| CT-VM-012 | Schwierigkeit gewechselt | Gleichen Treffer anwenden | Anatomische Wunde gleich; Folgen, Gegnerverhalten und Ressourcen abweichend | SCH MED | COMBAT-DEC-GP-014 | abnahmefähig |
| CT-VM-013 | Gameplay-Tod, Spielstand mit voller Gesundheit | Tod, Checkpoint laden | Kein voller Gesundheitszustand; minimal überlebensfähig | PER MED | COMBAT-DEC-BW-012 | bedingt (CK-01, CQ-07) |
| CT-VM-014 | Kampfunfähigkeit | Zustand erreichen | Nicht automatisch tot; Handlungen eingeschränkt | MED | COMBAT-DEC-BW-006, COMBAT-DEC-GP-027 | bedingt (CQ-60) |
| CT-VM-015 | Anruf an Verbündeten, Zusammenbruch | Rettung durch Begleiter | Spieler wacht im Krankenhaus auf; Zeit vergeht; Welt entwickelt sich | NPC PER | COMBAT-DEC-BW-008, COMBAT-DEC-BW-010 | bedingt (CQ-56) |
| CT-VM-016 | Gliedmaße schwer zerstört (Spieler) | Folge prüfen | Verhalten folgt der Entscheidung zu Gliedmaßenverlust | MED | COMBAT-DEC-VL-012, COMBAT-DEC-VL-013 | bedingt (CK-04, CQ-53) |
| CT-VM-017 | Feuer an Kleidung, früh | Mit Jacke ersticken | Früher Brand lässt sich ersticken, später nicht | TRF MED | COMBAT-DEC-TK-012 | abnahmefähig |
| CT-VM-018 | Heilung über Zeit | Zeit vergeht (Weltzeit) | Stufen schreiten fort; kein Sprung von „akut“ auf „erholt“ | MED | COMBAT-DEC-VL-005 | bedingt (CQ-51) |

---

## 8. Gegner-KI-Tests (KI)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-KI-001 | Geräusch hinter Tür | Nachladen | Gegner erhält Richtung, Entfernung, Unsicherheit; keine Spielerkoordinate | KI AUD | COMBAT-DEC-KI-001, COMBAT-DEC-MU-013 | abnahmefähig |
| CT-KI-002 | Gegner hat Spieler nie gesehen | Spieler versteckt sich im Schrank | Gegner kann nur vermuten, nicht wissen | KI | COMBAT-DEC-KI-002, COMBAT-DEC-KI-003 | abnahmefähig |
| CT-KI-003 | Gegner kennt Spieler aus früheren, ihm unbekannten Begegnungen nicht | Wiederholung eines Verhaltensmusters | Keine Anpassung aus nicht beobachteten Daten | KI | COMBAT-DEC-KI-003 | abnahmefähig |
| CT-KI-004 | Sichtverlust | Spieler verschwindet | Letzte bekannte Position wird mit der Zeit ungenauer; Wissen um die Bedrohung bleibt | KI | COMBAT-DEC-KI-005 | abnahmefähig |
| CT-KI-005 | Suchphasen | Nach Sichtverlust warten | Phasen folgen in der Reihenfolge; Dauer von der Schwierigkeit abhängig | KI SCH | COMBAT-DEC-KI-006 | abnahmefähig |
| CT-KI-006 | Dunkelheit | Spieler in Dunkelheit | Nicht unsichtbar, schlechter erkennbar; im Dunkeln gehört, grob geortet | KI | COMBAT-DEC-KI-009 | abnahmefähig |
| CT-KI-007 | Gleiche Ablenkung dreimal | Wurf | Gegner wird skeptisch, ignoriert, untersucht anders oder vermutet eine Falle | KI | COMBAT-DEC-KI-012 | abnahmefähig |
| CT-KI-008 | Menschliche Gegner, keine Kommunikation | Einen Gegner lautlos ausschalten | Andere erfahren nichts ohne Ruf, Funk oder Sicht | KI | COMBAT-DEC-KI-013 | abnahmefähig |
| CT-KI-009 | Monster mit Taktikvorrat | Gleiche Lage zweimal, gleicher Seed | Gleiche Taktik; Taktik außerhalb des Vorrats nie gewählt | KI | COMBAT-DEC-KI-015, COMBAT-DEC-KI-016 | abnahmefähig |
| CT-KI-010 | Taktik hat mehrfach versagt | Weiterer Versuch | Wechsel der Taktik | KI | COMBAT-DEC-KI-016 | abnahmefähig |
| CT-KI-011 | Schweigen als Taktik | Viele Begegnungen | Schweigen tritt nicht jedes Mal auf | KI | COMBAT-DEC-KI-017 | abnahmefähig |
| CT-KI-012 | Menschlicher Gegner, eingeschüchtert | Drei Angreifer, zwei ausgeschaltet, Drohung | Aufgabe, Flucht oder Erstarren möglich, ohne Furchtbalken | KI | COMBAT-DEC-KI-021, COMBAT-DEC-NK-017 | abnahmefähig (Eigenschaft), Schwellen bedingt (CQ-73) |
| CT-KI-013 | Kampf bricht ab (Sichtlinie weg) | Kampf, dann Verstecken | Kampf wird zu Suche und Tarnung | KI | COMBAT-DEC-KI-007 | abnahmefähig |
| CT-KI-014 | Grounded und Normal | Gleiche Lage | Grounded: Suche länger, aufmerksamer; nie Wandblick | KI SCH | COMBAT-DEC-KI-022, COMBAT-DEC-KI-023 | abnahmefähig |
| CT-KI-015 | KI-Inspektor | Entscheidung auslösen | Wissen, Verdacht, Suchphase, Taktik und Begründung sichtbar | TST | COMBAT-DEC-KI-024 | abnahmefähig |
| CT-KI-016 | Begleiter mit Plan | Gefahr tritt auf | Begleiter weicht ab, wenn der Befehl offensichtlich unsicher ist | NPC KI | COMBAT-DEC-KI-025 | abnahmefähig |
| CT-KI-017 | Friendly Fire an | Begleiter in der Schusslinie | Begleiter wird getroffen; Option ausschaltbar; Grounded standardmäßig an | KI WAF | COMBAT-DEC-GP-023 | abnahmefähig |

---

## 9. Kreaturen- und Schwächentests (KW)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-KW-001 | Testwesen mit Schwäche gegen Material M | Normale Munition, dann Material M | Normale Munition verletzt oder wirkt nicht, M entfaltet die Wirkungsart | KRE MUN | COMBAT-DEC-KS-001, COMBAT-DEC-SJ-013 | abnahmefähig |
| CT-KW-002 | Testwesen verletzbar, nicht tötbar | Viele Schüsse | Wesen wird verletzt, stirbt aber nicht | KRE | COMBAT-DEC-KS-002 | abnahmefähig |
| CT-KW-003 | Testwesen mit Regeneration | Verletzung | Regeneration gemäß Teilregeln; Unterdrücker wirken | KRE MED | COMBAT-DEC-KS-003 | abnahmefähig |
| CT-KW-004 | Individuum mit Narben | Rückkehr nach Weltzeit | Narbe vorhanden, wenn die Art sie nicht regeneriert | NPC KRE | COMBAT-DEC-KS-006, COMBAT-DEC-NM-004 | abnahmefähig |
| CT-KW-005 | Schwäche wirkt nur teilweise (Ausnahme-Individuum) | Methode anwenden | Teilweises Versagen möglich, keine Beschriftung „IMMUN“ | KRE UI | COMBAT-DEC-SJ-004, COMBAT-DEC-KS-018 | abnahmefähig |
| CT-KW-006 | Archiv-Status „Suspected“ | Falsche Methode benutzt | Beobachtung führt zu „Disproven“ oder „Disputed“; Wirkung bleibt aus | ERM KRE | COMBAT-DEC-SJ-005, COMBAT-DEC-SJ-015 | abnahmefähig |
| CT-KW-007 | Resistenz intern gestuft | Archiv öffnen | Es erscheinen Belege, keine Prozentwerte | UI KRE | COMBAT-DEC-SJ-014, COMBAT-DEC-KS-008 | abnahmefähig |
| CT-KW-008 | Wesen ohne Anatomiedatensatz | Spawn | Ablehnung mit Meldung statt Standardmensch | KRE | COMBAT-DEC-KS-024 | Vorschlag |
| CT-KW-009 | Besessener Wirt | Kampf | Wirt und Entität bleiben getrennte Figuren | KRE NPC | COMBAT-DEC-KS-012 | abnahmefähig (Eigenschaft), Regeln bedingt (CQ-63) |
| CT-KW-010 | Wesen wird verletzt, flieht | Weltzeit vergeht | Es versteckt sich, erholt sich und kann zurückkehren | KI NPC | COMBAT-DEC-KS-016, COMBAT-DEC-KS-017 | abnahmefähig |
| CT-KW-011 | Falle intakt, Wesen passt zur Mythologie | Wesen betritt Falle | Falle wirkt; Wesen kann sie zuvor erkennen oder umgehen | KRE KI | COMBAT-DEC-FF-002, COMBAT-DEC-FF-006 | abnahmefähig |
| CT-KW-012 | Wesen in wirksamer Falle gefangen | Zeit vergeht | Es befreit sich nicht durch KI allein | KI | COMBAT-DEC-FF-007 | Vorschlag |
| CT-KW-013 | Falle in entladener Region | Zeit und Regen | Zustand wird nachgeführt, die Falle ist verändert | KRE PER | COMBAT-DEC-FF-004, COMBAT-DEC-SP-010 | abnahmefähig |
| CT-KW-014 | Individuum wird getötet | Spiel läuft weiter | Tod bleibt dauerhaft im Weltzustand | NPC PER | COMBAT-DEC-NM-016 | abnahmefähig |
| CT-KW-015 | Wesen, das niemandem schadet | Spieler verschont es | Es wird nicht automatisch zum Feind; ein anderer Hunter kann es töten | NPC | COMBAT-DEC-NM-009 | abnahmefähig |

---

## 10. Persistenztests (PE)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-PE-001 | Nachladen unterbrochen | Speichern und Laden | Zustand wird fortgesetzt; Patronenzahl insgesamt gleich | PER WAF | COMBAT-DEC-SP-003, COMBAT-DEC-MU-012 | abnahmefähig |
| CT-PE-002 | Gegner hat Spieler entdeckt | Manuell speichern | Gesperrt | PER | COMBAT-DEC-SP-004, COMBAT-DEC-GP-028 | abnahmefähig |
| CT-PE-003 | Gegner misstrauisch, nicht entdeckt | Speichern | Möglich | PER KI | COMBAT-DEC-SP-004 | bedingt (CQ-09) |
| CT-PE-004 | Kampf endet | Bedrohungszustand beendet | Autosave erlaubt | PER | COMBAT-DEC-SP-005, COMBAT-DEC-SP-006 | abnahmefähig |
| CT-PE-005 | Wunden und Behandlungsstand | Speichern, neu starten, laden | Wunden, Behandlungsstand und Genesungsstufe bleiben erhalten | PER MED | COMBAT-DEC-SP-013, COMBAT-DEC-VL-005 | abnahmefähig |
| CT-PE-006 | Falle, Waffe am Boden, Leiche | Speichern und laden | Weltobjekte bleiben, bis sie bereinigt werden | PER | COMBAT-DEC-SP-011 | abnahmefähig |
| CT-PE-007 | Tod mit Permadeath aus | Gameplay-Tod | Checkpoint mit Mindestzustand statt Löschung | PER | COMBAT-DEC-SP-007 | bedingt (CK-01) |
| CT-PE-008 | Kein sicherer Checkpoint | Tod | Softlock-Schutz greift; kein Todes-Loop | PER | COMBAT-DEC-SP-009, COMBAT-DEC-BW-016 | bedingt (CQ-07) |
| CT-PE-009 | Format v1 | Speichern und Laden | Serialisierung ist rundlaufend (Golden-Datei), bestehende Daten unverändert | PER | COMBAT-DEC-SP-001; Decision 0004 | abnahmefähig |
| CT-PE-010 | Unbekanntes Kampfsegment aus Mod | Laden | Spielstand lädt unverändert und speichert wieder, oder kontrollierte Ablehnung | PER MOD | COMBAT-DEC-SP-016 | Vorschlag |
| CT-PE-011 | Wesen und Rivale, persistent | Weltzeit vergeht | Individuum behält Narben, Gedächtnis, Feinde | NPC PER | COMBAT-DEC-SP-012, COMBAT-DEC-NM-002 | abnahmefähig |
| CT-PE-012 | Seed im Spielstand | Replay eines Kampfes | Gleiches Ergebnis bei gleichen Eingaben | TST PER | COMBAT-DEC-SP-015 | bedingt (CQ-105) |
| CT-PE-013 | Autosave in einem langen Kampf | Phasenwechsel | Gespeicherter Zustand ist reproduzierbar | PER | COMBAT-DEC-SP-005, COMBAT-DEC-SP-019 | bedingt (CQ-103) |

---

## 11. Koop- und Netzwerktests (KN)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-KN-001 | Host und Gast | Gast verlässt das Spiel | Fortschritt bleibt im Host-Spielstand; kein Verlust von Weltzustand | KOO PER | COMBAT-DEC-KO-002 | abnahmefähig |
| CT-KN-002 | Zwei Spieler, ein Wesen | Beide schießen gleichzeitig | Jedes Projektil wirkt genau einmal | KOO WAF | COMBAT-DEC-KO-015 | Vorschlag |
| CT-KN-003 | Netzwerk wiederholt ein Ereignis | Doppelte Zustellung | Wunde, Magazin und Projektil entstehen nicht doppelt | KOO | COMBAT-DEC-KO-015 | Vorschlag |
| CT-KN-004 | Zwei Spieler mit verschiedenen Verletzungszuständen | Kampf | Körperzustand je Spieler getrennt; kein direktes Schreiben auf andere | KOO MED | COMBAT-DEC-KO-012 | abnahmefähig |
| CT-KN-005 | Verhaftung eines Spielers | Welt läuft weiter | Nur der Betroffene sitzt ein; Zeitsprung nur mit Zustimmung aller | KOO POL | COMBAT-DEC-KO-006 | abnahmefähig |
| CT-KN-006 | Krankenhaus im Koop | Spieler fällt aus | Krankenhaus überspringbar; Ausfall begrenzt | KOO MED | COMBAT-DEC-KO-007 | abnahmefähig |
| CT-KN-007 | Fallen und Waffen am Boden | Zwei Spieler sehen denselben Ort | Beide sehen denselben Weltzustand | KOO PER | COMBAT-DEC-KO-013 | abnahmefähig |
| CT-KN-008 | Treffer-Autorität | Schuss mit Latenz | Ergebnis folgt der festgelegten Autorität | KOO | COMBAT-DEC-KO-016 | bedingt (CQ-97) |
| CT-KN-009 | Friendly Fire zwischen Spielern | Spieler trifft Spieler | Verhalten folgt der Entscheidung | KOO | COMBAT-DEC-KO-008 | bedingt (CQ-96) |
| CT-KN-010 | Gegner und zwei Spieler | Ziel wählen | Zielwahl nach Wissen, nicht nach Distanz allein | KI KOO | COMBAT-DEC-KO-017 | bedingt (CQ-98) |
| CT-KN-011 | Mods im Koop | Beitritt mit anderen Mods | Ablehnung oder Synchronisierung nach Regel | MOD KOO | COMBAT-DEC-KO-009 | bedingt (CQ-99) |
| CT-KN-012 | Online-Modus | Mods oder Cheats aktiv | Nicht zugelassen | KOO | COMBAT-DEC-KO-005 | bedingt (Online-Meilenstein) |

---

## 12. Performance- und Skalierungstests (PL)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-PL-001 | Testkarte mit Dauerfeuer | Projektilzahl steigern | Messprotokoll der Rechenzeit je Projektil; keine Annahme eines Budgets | PRF | COMBAT-DEC-PF-010 | Vorschlag |
| CT-PL-002 | Szenariomatrix Dichte mal Schwierigkeit | Gegner erhöhen | Rechenzeit der KI protokolliert; Obergrenze wird abgeleitet, nicht vorgegeben | PRF KI | COMBAT-DEC-PF-013 | Vorschlag |
| CT-PL-003 | Gegner im Nachbargebäude | Detailstufe | Stufe wechselt ohne Änderung von Verletzung, Wissen oder Waffenzustand | PRF | COMBAT-DEC-PF-016 | abnahmefähig |
| CT-PL-004 | Ferner Kampf | Fortschreiben | Ergebnis folgt denselben Anatomie- und Beweisregeln | PRF | COMBAT-DEC-PF-016 | abnahmefähig |
| CT-PL-005 | Annäherung an einen fernen Kampf | Zustand materialisiert | Konsistenter Zustand aus dem Ergebnis | PRF PER | COMBAT-DEC-PF-004 | abnahmefähig |
| CT-PL-006 | Projektil verlässt die Region | Flug | Definierte Beendigung; kein stiller Verlust | PRF | COMBAT-DEC-PF-003 | Vorschlag |
| CT-PL-007 | Mikrobenchmark des Verletzungsmodells | n Treffer | Kosten pro Wundauflösung protokolliert | PRF MED | COMBAT-DEC-PF-010 | Vorschlag |
| CT-PL-008 | Gedränge | Kampf mit Menge | Stufenabbau statt Regelabbau | PRF | COMBAT-DEC-PF-014 | bedingt (CQ-107) |
| CT-PL-009 | Paaranimationen | n Paare | Animationskosten protokolliert | PRF ANI | COMBAT-DEC-AN-019 | bedingt (CQ-95) |
| CT-PL-010 | Plattformziele | Profilierung | Aussagen nur aus Messungen | PRF | COMBAT-DEC-PF-012 | bedingt (CQ-108) |

---

## 13. Regressionstests (RG)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-RG-001 | Reload-Suite | Alle Nachladearten mit allen Abbruchpunkten | Patronensumme konstant; definierter Waffenzustand | WAF | COMBAT-DEC-MU-012 | abnahmefähig |
| CT-RG-002 | Penetrationssuite | Materialmatrix | Restenergie monoton; thin/massiv wie festgelegt | TRF | COMBAT-DEC-FK-016 | abnahmefähig |
| CT-RG-003 | Arterienblutung | Verlaufsszenario | Reproduzierbares Ergebnis | MED | COMBAT-DEC-TK-004 | abnahmefähig |
| CT-RG-004 | Erneute Verletzung | Szenario ‚heilende Region‘ | Schnellere Verschlechterung | MED | COMBAT-DEC-VL-005 | abnahmefähig |
| CT-RG-005 | Verzögerte innere Verletzung | Szenario | Unsicherer Befund bis Diagnose | MED | COMBAT-DEC-TK-009 | abnahmefähig |
| CT-RG-006 | Nicht-Plot-Armor-Parität | Spieler und NPC | Gleiches Ergebnis bei gleichen Eingaben | MED | COMBAT-DEC-GP-007 | abnahmefähig |
| CT-RG-007 | Grounded: versteckte Hilfen | Alle Hilfen prüfen | Keine versteckte Hilfe aktiv | SCH | COMBAT-DEC-GP-015 | abnahmefähig |
| CT-RG-008 | Schwierigkeits-Unabhängigkeit | Ermittlungsführung ändern | Kampfschwierigkeit unverändert und umgekehrt | SCH | COMBAT-DEC-GP-011 | abnahmefähig |
| CT-RG-009 | Fight-Club-Regeln | Aufgabe und Bewusstlosigkeit | Griff gelöst | NK | COMBAT-DEC-NK-013 | bedingt (CQ-13) |
| CT-RG-010 | Aufgabegriff mit Verletzung | Fortsetzung | Gelenk- oder Bandverletzung möglich | MED | COMBAT-DEC-NK-011 | abnahmefähig |
| CT-RG-011 | Mehrfachgegner-Grapple | Szenario | Grappling nicht gesperrt, aber gefährlich | KI | COMBAT-DEC-NK-020 | abnahmefähig |
| CT-RG-012 | Fehler mit festem Seed | Reproduktion | Jeder echte Fehler wird zum dauerhaften Test | TST | COMBAT-DEC-ZA-008 | abnahmefähig |

---

## 14. Fehler- und Wiederherstellungstests (FE)

| ID | Voraussetzungen | Ablauf | Erwartetes Ergebnis | Systeme | Bezug | Status |
|---|---|---|---|---|---|---|
| CT-FE-001 | Absturz beim Speichern | Prozess abbrechen | Alter Stand bleibt; Datei nicht beschädigt | PER | COMBAT-DEC-SP-002; README §26.2 | abnahmefähig |
| CT-FE-002 | Beschädigte Datei | Laden | Sicheres Laden lehnt ab; Bestand unverändert | PER | COMBAT-DEC-SP-002 | abnahmefähig |
| CT-FE-003 | Beteiligter stirbt im Griff | Griff | Griff endet; Überlebender frei | NK | COMBAT-DEC-ZA-014 | Vorschlag |
| CT-FE-004 | Unbekanntes Material | Treffer | Standardmaterial mit Protokoll | TRF | COMBAT-DEC-TK-018 | Vorschlag |
| CT-FE-005 | Kreatur ohne Anatomiedatensatz | Spawn | Meldung, kein Absturz | KRE | COMBAT-DEC-KS-024 | Vorschlag |
| CT-FE-006 | Mod-Waffe mit unbekanntem Kaliber | Laden | Ablehnung mit Meldung; Basisspiel unverändert | MOD WAF | COMBAT-DEC-MU-028, COMBAT-DEC-SP-016 | Vorschlag |
| CT-FE-007 | Mod fehlt, Spielstand verweist | Laden | Kritisch fehlende Mods blockieren; Expertenübersteuerung nach Backup | MOD PER | COMBAT-DEC-KO-009 | abnahmefähig |
| CT-FE-008 | Verbindungsverlust im Kampf | Gast fällt aus | Figur und Ausrüstung bleiben konsistent | KOO | COMBAT-DEC-KO-018 | bedingt (CQ-99) |
| CT-FE-009 | Fehlende Animation | Zustand | Platzhalter und Protokoll | ANI | COMBAT-DEC-AN-014 | Vorschlag |
| CT-FE-010 | Softlock nach Tod | Wiederholter Tod am gleichen Checkpoint | Alternative ohne Gratisheilung | PER | COMBAT-DEC-BW-016 | bedingt (CQ-07) |
| CT-FE-011 | Beweis und Leiche häufen sich | Welt voll | Bereinigung folgt der Regel, Beweise nicht sofort gelöscht | PER POL | COMBAT-DEC-PO-003 | bedingt (CQ-83) |
| CT-FE-012 | Doppelte Eingabe in einem Schritt | Zwei Handlungen | Deterministische Reihenfolge; Ablehnung mit Grund | STE | COMBAT-DEC-ST-018 | bedingt (CQ-25) |

---

## 15. Szenariotests (SZ)

Die Szenarien aus [Modul 26](26-Querschnittsszenarien.md) werden als Z-Läufe (deterministische Simulation mit festen Seeds) und, soweit möglich, als U-Tests geplant. Jeder Test prüft die **Kette** Treffer → Zustand → Verhalten → Weltfolge → Persistenz.

| ID | Szenario | Prüfkette | Bezug | Status |
|---|---|---|---|---|
| CT-SZ-001 | CS-01 Unbewaffneter Kampf | Schlagabtausch bis Aufgabe oder Bewusstlosigkeit; Wunden bleiben | COMBAT-DEC-NK-001, COMBAT-DEC-NK-017, COMBAT-DEC-VL-005 | bedingt (CQ-01, CQ-27) |
| CT-SZ-002 | CS-02 Improvisierte Waffe, drei Gegner | Gleichzeitiger Druck, Tür, Flasche zerbricht | COMBAT-DEC-NK-018, COMBAT-DEC-NK-022 | abnahmefähig |
| CT-SZ-003 | CS-03 Schusswechsel in der Gasse | Zugriff nach Trageort, Durchschlag, Nachladegeräusch, Zeugen | COMBAT-DEC-WA-005, COMBAT-DEC-FK-016, COMBAT-DEC-MU-013, COMBAT-DEC-PO-007 | bedingt (CQ-17, CQ-45) |
| CT-SZ-004 | CS-04 Starkes Wesen | Ausweichen, Verletzung, Rückzug, Wiederkehr | COMBAT-DEC-DF-011, COMBAT-DEC-KS-016 | bedingt (CQ-65) |
| CT-SZ-005 | CS-05 Unbekannte Schwäche | Hypothese, Fehlschlag, Archivupdate, zweiter Versuch | COMBAT-DEC-SJ-015, COMBAT-DEC-SJ-005 | abnahmefähig (Eigenschaft) |
| CT-SZ-006 | CS-06 Gefangennahme | Falle, Umgehung, Fang, Verwahrung, Verhör | COMBAT-DEC-FF-001, COMBAT-DEC-FF-006, COMBAT-DEC-FF-008 | bedingt (CQ-70, CQ-72) |
| CT-SZ-007 | CS-07 Flucht | Beinverletzung, Sichtbruch, Suchphasen | COMBAT-DEC-VL-008, COMBAT-DEC-KI-006 | bedingt (CQ-88) |
| CT-SZ-008 | CS-08 Schwer verletzt | Feldversorgung, Krankenhaus, Genesung | COMBAT-DEC-VL-003, COMBAT-DEC-VL-005 | bedingt (CQ-51) |
| CT-SZ-009 | CS-09 Rivale kehrt zurück | Narbe, Gedächtnis, Weltzeit | COMBAT-DEC-NM-004, COMBAT-DEC-NM-002 | abnahmefähig (Eigenschaft), Häufigkeit bedingt (CQ-78) |
| CT-SZ-010 | CS-10 Zeugen und Polizei | Spuren, Notruf, Fahndung nach Beweislage | COMBAT-DEC-PO-002, COMBAT-DEC-PO-008 | bedingt (CQ-83) |
| CT-SZ-011 | CS-11 Koop mit Verletzten | Zustand je Spieler, Host-Weltzustand | COMBAT-DEC-KO-012, COMBAT-DEC-KO-013 | bedingt (CQ-97) |
| CT-SZ-012 | CS-12 Persistente Falle | Zustand nach Zeit, Wesen erkennt Falle | COMBAT-DEC-FF-004, COMBAT-DEC-SP-010 | abnahmefähig |

---

## 16. Abnahmekriterien nach Ausbaustufe (Entwurf)

> **💡 VORSCHLAG.** Das Dokument legt keine Meilensteine fest. Die Stufen folgen der Reihenfolge von `CLAUDE.md` §4 (Phase 0, Vertical Slice) und sind Vorschläge, welche Tests wann gelten könnten.

| Stufe | Möglicher Umfang | Tests, die gelten könnten |
|---|---|---|
| Kern ohne Unreal (S) | Verletzungsmodell, Trefferkette, Ballistik-Rechnung, Wissensmodell, Speicherformat | FU-001 bis -003, PH-001 bis -004, PH-010, VM-001 bis -008, KI-001 bis -004, PE-001, PE-009, FE-001, FE-002, RG-001 bis -006 |
| Erste spielbare Testkarte (U) | eine Schusswaffe, ein menschlicher Gegner, Nahkampf in kleinem Umfang | ST-001, ST-002, FU-005 bis -007, AB-003, AB-007 |
| Vertical-Slice-Fall (U/P) | ein Wesen mit Schwäche, Vorbereitung, Folgen | KW-001, KW-002, KW-006, SZ-005, PE-005, PE-006 |
| Koop-Meilenstein | Host-Spielstand, Zustand je Spieler | KN-001 bis -007 |
| Vollausbau | restliche Tests nach Entscheidung der Fragen | alle `bedingt` nach Klärung der genannten `CQ-…` |

**Pflicht für jede Stufe:** ein Bericht, der nur tatsächlich ausgeführte Tests und deren Ergebnis nennt (`CLAUDE.md` §7).
