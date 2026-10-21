# Adult 29: QA-Testkatalog (Entwurf)

> [!IMPORTANT]
> **Nichts davon ist implementiert, und es wurde kein einziger dieser Tests ausgeführt.** Dies ist ein **Entwurf von Testfällen**, abgeleitet aus bestätigten oder vorläufigen Quellenaussagen und den Spezifikationsskizzen [20](20-Spezifikation-Regelkern-Zustimmung.md) bis [28](28-Spezifikation-Speicherung-Mods-Builds.md). Er ist kein Auftrag. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Abnahmekriterien: [Modul 30](30-Abnahmekriterien.md). Randfälle: [Modul 31](31-Randfaelle-Katalog.md). CLAUDE.md §7: Tests für deterministische Regeln, Persistenz und Regressionen; nur tatsächlich gelaufene Tests melden.

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
10. [Querschnitt: Regression, Fuzzing, Last](#10-querschnitt-regression-fuzzing-last)
11. [Testarten und Ausführungsumgebung](#11-testarten-und-ausführungsumgebung)
12. [Nicht testbar / nicht freigegeben](#12-nicht-testbar--nicht-freigegeben)

---

**Legende der Spalte „Art“:** **S** = Standalone C++ (ohne Unreal, auf dem Fedora-Laptop möglich); **U** = Unreal-Automation (Unreal-Binaries nötig); **M** = manueller Playtest; **P** = Performance-/Lasttest; **R** = Review/Prozessprüfung; **V** = Build-Validator.
**Priorität:** **K** = Kern (Sicherheit), **W** = wichtig, **S** = später.

## 1. Regelkern

Spezifikation: [Modul 20](20-Spezifikation-Regelkern-Zustimmung.md). Alle Tests der Priorität K sind Release-Bedingung für das Paket.

| ID | Prio | Art | Testziel | Vorbedingung / Schritte | Erwartung | Basis |
|---|---|---|---|---|---|---|
| T-ZU-001 | K | S | Altersgrenze exakt | Geburtsdatum so, dass Alter 17 Jahre 364 Tage / 18 Jahre 0 Tage am Weltdatum | `NotEligible` / `AdultEligible`; Gate an 0:00 des Geburtstags | E-SK-009 |
| T-ZU-002 | K | S | Schaltjahr-Geburtstag (29.02.) | Figur am 28.02./01.03. in Nicht-Schaltjahren | definierte, dokumentierte Regel; kein Off-by-one zugunsten Eligibility | E-SK-012 |
| T-ZU-003 | K | S | Unbekanntes Alter | `Unknown` | `Deny(NotAdultEligible)` | E-SK-010 |
| T-ZU-004 | K | S | Aussehensunabhängigkeit | gleiche Daten, extrem unterschiedliche Körper-/Stimm-/Kleidungsparameter | Entscheidung identisch | E-SK-010 |
| T-ZU-005 | K | S | Mod setzt Eligibility | Mod-API-Aufruf zum Setzen | abgelehnt, Protokolleintrag | E-SK-004 |
| T-ZU-006 | K | S | Cheat/Konsole setzt Alter | Cheatversuch | abgelehnt (Alter nur aus Geburtsdatum) | E-SK-012 |
| T-ZU-007 | K | S | Minderjährige im Adult-Pfad | NPC < 18 | kein Eintrag in Auswahl, kein Präferenzprofil, kein Anfragepfad | E-SK-032 |
| T-ZU-008 | K | S | Alterung während Sitzung | Weltzeit überschreitet Geburtstag | Eligibility ändert sich nicht rückwirkend in einer laufenden Sitzung; Neuberechnung am Beginn | E-SK-009 |
| T-ZU-009 | K | S | Ausweis-Ereignis | Figur zeigt Nachweis | nur Status „verifiziert“ gespeichert, keine Ausweisdaten | E-SK-012 |
| T-ZU-010 | K | S | Capacity: bewusstlos | `awareness=unconscious` | `Deny(NotCapable)` | E-SK-017 |
| T-ZU-011 | K | S | Capacity: schlafend | `awareness=sleeping` | `Deny(NotCapable)` | E-SK-003 |
| T-ZU-012 | K | S | Capacity: stark beeinträchtigt | `cognitive_load=hoch` | `Deny(NotCapable)` | E-SK-006 |
| T-ZU-013 | K | S | Capacity: leicht beeinträchtigt, ansprechbar, bejahend | niedrige Last | `Allow` (wenn Consent) | E-SK-015 |
| T-ZU-014 | K | S | Erkennungsfenster | `responsiveness` fällt in laufender Sitzung | nach `T_detect` Abbruch; kein Weiterlaufen darüber hinaus | E-SK-017 |
| T-ZU-015 | K | S | Keine Hilfsanzeige „bewusstlos“ | Bewusstlosigkeit | kein HUD-Hinweis; natürliche Signale | E-SK-018 |
| T-ZU-016 | K | S | Beziehung ersetzt Zustimmung nicht | Beziehungswerte maximal, keine Zustimmung | `Deny(NoConsent)` | E-SK-001 |
| T-ZU-017 | K | S | Story/Quest/Admin ersetzt Zustimmung nicht | Quest verlangt Interaktion | `Deny(NoConsent)` | E-SK-003 |
| T-ZU-018 | K | S | Preis/Vertrag ersetzt Zustimmung nicht | Bezahlte Buchung ohne Zustimmung | `Deny(NoConsent)` | E-SK-003 |
| T-ZU-019 | K | S | Arousal ersetzt Zustimmung nicht | hohes Arousal, keine Zustimmung | `Deny(NoConsent)` | E-SK-030 |
| T-ZU-020 | K | S | Gelernte Präferenz ersetzt Zustimmung nicht | Präferenz „interessiert“, keine aktuelle Zustimmung | `Deny(NoConsent)` | E-NP-006 |
| T-ZU-021 | K | S | Hard Limit | Familie im Hard Limit | `Deny(HardLimit)` unabhängig von Überredung | E-SK-028 |
| T-ZU-022 | K | S | Überreden verschiebt nur Neutral/Situativ | 1.000 Überredungsversuche | Hard Limit unverändert; Neutral kann sich ändern | E-NP-006 |
| T-ZU-023 | K | S | Hard-Limit-Änderung nur durch Figurenvorgang | Zeit/Erlebnis vs. Mod/Wiederholung | nur Vorgang der Figur ändert (selten); Mod nicht | E-SK-005 |
| T-ZU-024 | K | S | Prüfreihenfolge | Teilnehmer mit mehreren Verstößen | Reihenfolge Alter → Capacity → Verfügbarkeit → Hard Limit → Consent; erste Stufe nennt Grund | E-SK-007 |
| T-ZU-025 | K | S | Gate ohne Paket | Paket nicht aktiv | `Deny(PackageInactive)` | Modul 20 |
| T-ZU-026 | K | S | Zustimmungszustände | Übergänge aller 9 Zustände | gültige Übergänge nur gemäß Tabelle | E-SK-026 |
| T-ZU-027 | K | S | Scope | Zustimmung für Familie A, Handlung Familie B | Neuprüfung/Angebot | E-SK-027 |
| T-ZU-028 | K | S | Neuer Teilnehmer | dritte Person tritt bei | Neuprüfung für alle | E-SK-027 |
| T-ZU-029 | K | S | Intensitätsanstieg über Korridor | Achse über Grenze | Check-in oder `Deny`, nie stillschweigend | E-SK-027 |
| T-ZU-030 | K | S | Längere Unterbrechung | Pause > Schwellwert | Neuprüfung | E-SK-027 |
| T-ZU-031 | K | S | Neue Begegnung | gleicher Tag/Ort, neue Situation | neue Berechnung | E-SK-025 |
| T-ZU-032 | K | S | Schweigen allein | keine Signale | keine Zustimmung | E-SK-022 |
| T-ZU-033 | K | S | „Ruhig, aber beteiligt“ | aktive Teilnahme | keine Unterbrechung | E-SK-023 |
| T-ZU-034 | K | S | Stöhnen allein | nur Laut | nicht ausreichend für Rahmenerweiterung | E-SK-022 |
| T-ZU-035 | K | S | Explizites Nein | Signal `HardStop` | sofortige Wirkung, kein Score | E-SK-023 |
| T-ZU-036 | K | S | Wegdrücken/Abwehr | Signal | `HardStop` | E-SK-023 |
| T-ZU-037 | K | S | Schmerz/Stress ohne positive Signale | Signalmuster | Check-in, bei ausbleibender Antwort Abbruch | E-SK-021 |
| T-ZU-038 | K | S | Versehentlicher Kontakt | Kontakt außerhalb Rahmen | keine Zustimmung zur neuen Handlung; Korrektur/Ende | 35#575/577 |
| T-ZU-039 | K | S | Handlung außerhalb Rahmen | neue Familie | Angebot/Neuprüfung | E-SK-034 |
| T-ZU-040 | K | S | `HardStop` Ablauf | aus jedem Sitzungszustand | `Withdrawn` → Lockout → sicherer Übergang → neutraler Ausstieg | E-SK-028 |
| T-ZU-041 | K | S/U | Kein Teleport beim Stopp | fehlende Ausstiegsanimation | Fallback-Leiter, kein Teleport, kein Softlock | E-SO-021 |
| T-ZU-042 | K | S | Spieler-Stopp | Pause/Stopp | wirkt wie `HardStop` | Modul 20 |
| T-ZU-043 | K | S | Dialog-Popups | Standardkonfiguration | keine Dauer-Popups | E-SK-023 |
| T-ZU-044 | K | S | Determinismus | gleiche Eingaben, gleicher Seed | gleiche Entscheidung und Begründung | E-SK-029 |
| T-ZU-045 | K | S | Begründung | jede `Deny`-Antwort | Grund-Code vorhanden | E-SK-029 |
| T-ZU-046 | K | S | Mod-Konflikt mit Kernregel | Mod-Profil „keine Zustimmung nötig“ | wird abgelehnt/ignoriert, Protokoll | E-SK-004 |
| T-ZU-047 | K | S | Verfügbarkeit: Kampf | NPC im Kampf | `Deny(Unavailable)` | Modul 20 |
| T-ZU-048 | K | S | Verfügbarkeit: gefesselt ohne Vereinbarung | Zustand | `Deny(Unavailable)` | Modul 20 |
| T-ZU-049 | K | S | Mehrere Teilnehmer: Widerruf eines | einer widerruft | Sitzung für ihn beendet; Rest nur mit Zustimmung | Modul 20 |
| T-ZU-050 | K | S | NPC↔NPC | Spieler beobachtet | dieselbe Prüfkette für NPCs | E-IA-002 |
| T-ZU-051 | K | S | Fail closed | Fehler im Zustimmungsdienst | Sitzung wird nicht gestartet / sicher beendet | E-SK-028 |
| T-ZU-052 | K | S | Timeout | Zustimmungsdienst antwortet nicht | `Deny` | P1 |
| T-ZU-053 | K | S | Verstorbene Figur | `life_state=dead` | keine Interaktion; Daten erhalten | E-SK-014 |
| T-ZU-054 | W | S | Zustimmungsdatensatz-Round-Trip | speichern/laden | identisch | Modul 20 |
| T-ZU-055 | W | S | Erinnerung nach Widerruf | `Withdrawn` | Gedächtniseintrag, Wirkung auf Vertrauen | NP-GD-04 |

## 2. Datenmodell

Spezifikation: [Modul 21](21-Spezifikation-Datenmodell.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-DM-001 | K | S | Basisspeicherstand ohne Adult-Bereich laden | Basisspiel intakt | E-PA-028 |
| T-DM-002 | K | S | Adult-Bereich getrennt löschbar | Basisdaten unverändert | E-PA-030 |
| T-DM-003 | K | S | IDs ohne Version | `schemaVersion` separat; Umbenennung via Alias | E-SP-007 |
| T-DM-004 | K | S | Stabile ID über Tier-Wechsel | gleiche ID | E-LE-002 |
| T-DM-005 | W | S | Nur Ergebnisse persistiert | keine Frames/Kamera/IK im Save | D5 |
| T-DM-006 | W | S | Delta gegenüber Ausgangszustand | unveränderte NPCs speichern nur ID+Seed | E-LE-002 |
| T-DM-007 | W | S | Migration v(n)→v(n+1) | Golden-File-Vergleich | E-SP-001 |
| T-DM-008 | W | S | Hot Disable/Enable | Daten erhalten, verwaiste Referenzen markiert | E-PA-030 |
| T-DM-009 | W | S | Präferenzstufen | Skala neugierig…Hard Limit | E-NP-004 |
| T-DM-010 | W | S | Partnerspezifische Modifikatoren | gleiche Figur, zwei Partner, unterschiedliche Werte | E-NP-020 |
| T-DM-011 | W | S | Wissensstufen | unbekannt/vermutet/erzählt/erlebt | E-NP-019 |
| T-DM-012 | W | S | Falsches Lernen | Partnerprofil mit Konfidenz | E-NP-029 |
| T-DM-013 | W | S | Gedächtnisstufen | Minimal→Full-Konsolidierung | E-LE-008 |
| T-DM-014 | W | S | Zusammenfassung ersetzt Einzelerinnerung nicht | Einzelabruf möglich | E-LE-002 |
| T-DM-015 | W | S | Registry-Duplikat-ID | Mod mit gleicher ID | Konflikt erkannt | E-SP-006 |
| T-DM-016 | W | S | Registry-Granularität | Eintrag ohne technische/physiologische/Präferenz-/Statistik-/Gesundheitsrelevanz | Validator warnt | E-IA-032 |
| T-DM-017 | W | S | Intensität als Kategorie verboten | Registry-Eintrag mit Kategorie „Intensität“ | Validator lehnt ab | E-IA-038 |
| T-DM-018 | W | S | Konfigurationsschichten | Global→Mod→Charakter→Save→NPC | Override-Reihenfolge, nicht destruktiv | E-SP-008 |
| T-DM-019 | W | S | Größenbudget | 10.000 simulierte Begegnungen | Größe unter Budget (Budget festlegen) | E-LE-003 |
| T-DM-020 | W | S | Keine STI-Felder | Schema-Scan | keine Felder | E-GE-004 |
| T-DM-021 | W | S | Kein `usageCount` | Schema-Scan | nicht vorhanden | E-KO-016 |
| T-DM-022 | W | S | Kein Ausweisdatenfeld | Schema-Scan | nicht vorhanden | E-SK-012 |
| T-DM-023 | S | S | Verschlüsselungsoption | Adult-Bereich verschlüsselt | Laden mit/ohne Schlüssel definiert | E-PA-030 |

## 3. Interaktionsengine

Spezifikation: [Modul 22](22-Spezifikation-Interaktionsengine.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-IE-001 | K | S | Abbruch aus jedem Zustand | `SafeExit` erreichbar | E-SK-028 |
| T-IE-002 | K | S/U | Fehlende Übergangsanimation | Fallback ohne Teleport, kein Softlock | E-SO-021 |
| T-IE-003 | K | U | Solver ohne Lösung | Leiter: Mikrokorrektur→IK-Lockerung→Neuberechnung→Ende | E-SO-021 |
| T-IE-004 | W | S/U | Determinismus | gleicher `session_seed` → gleiche Mikrovariation | E-SO-016 |
| T-IE-005 | W | U | Save/Load mitten in Sitzung | gleiche Mikrovariation danach | E-SO-016 |
| T-IE-006 | W | U | 1.000 Zufallskonstellationen (Größe, Gewicht, Proportionen) | keine Penetration über Toleranz, kein Softlock | E-SO-011 |
| T-IE-007 | W | U | Gelenkgrenzen | Überschreitung | Widerstand steigt, Unbehagen/Verletzung | E-SO-011 |
| T-IE-008 | W | U | Gewichtsunterschied | Konfiguration möglich/unmöglich korrekt | E-SO-012 |
| T-IE-009 | W | U | Kleidung begrenzt Beweglichkeit | enge Kleidung | eingeschränkter Bereich | E-SO-011 |
| T-IE-010 | W | U | Kleidung verschwindet nicht | Ausziehen | Kleidung liegt am Ort, Schmuck bleibt | E-IA-016 |
| T-IE-011 | W | U | Asymmetrie | Bein 0,5 cm länger | IK/Kontakt berücksichtigt | E-SO-001 |
| T-IE-012 | W | S | Layer-Konflikt | zwei Aktionen, eine Hand | Prioritätsauflösung | E-SO-037 |
| T-IE-013 | W | S | Scheduler-Prioritäten | Sicherheit > Balance > Kontakt > Aktion | Reihenfolge | Modul 22 |
| T-IE-014 | W | U | Zwischenpositionen | kontinuierlicher Übergang | gültig | E-SO-033 |
| T-IE-015 | W | U | Pose-Templates optional | Sitzung ohne Template | funktioniert | E-SO-010 |
| T-IE-016 | W | U | NPC nutzt denselben Solver | NPC-Führung | gleiche Constraints | E-SO-020 |
| T-IE-017 | W | S | Stärke entscheidet nie allein | Kraftunterschied | Zustimmung/Intents entscheiden | E-SO-020 |
| T-IE-018 | W | U | Kollisions-LOD | viele Teilnehmer | akzeptable Kollisionsqualität, keine Selbstdurchdringung > Toleranz | E-SO-036 |
| T-IE-019 | W | U | Möbelbruch | Belastung | Zustandsänderung, Persistenz | E-SO-033 |
| T-IE-020 | W | U | Platzmangel | enge Umgebung | Alternative oder sauberes Ende | Modul 22 |
| T-IE-021 | W | U | Umweltunterbrechung | Klingel/Tür/Alarm/Angriff | `Interrupted`, Weltpriorität | E-IA-036 |
| T-IE-022 | W | S | Nicht unterstützte Konstellation | 4 Teilnehmer ohne Variante | Abwechseln/Beobachter | E-IA-008 |
| T-IE-023 | W | S | Kein Fortschrittsbalken / Minispiel | UI-Scan | nicht vorhanden | E-IA-010 |
| T-IE-024 | W | S | Orgasmus ≠ Ende | Ereignis | Sitzung läuft weiter | E-IA-019 |
| T-IE-025 | W | S | Ende ohne Orgasmus | Sitzungsende | `Completed` | E-IA-019 |
| T-IE-026 | W | M | Aftercare-Abläufe | Duschen/Schlaf/Gespräch | verfügbar | E-IA-020 |
| T-IE-027 | W | U | Qualitätsstufen | Low vs. Cinematic | gleiche Entscheidungen | E-SO-014 |
| T-IE-028 | W | M | „Low sieht nicht fake aus“ | Playtest | Prüfliste (visuell) | E-SO-014 |
| T-IE-029 | S | U | Privacy Score | Fenster/Licht/Geräusch | plausible Werte | E-IA-035 |
| T-IE-030 | S | U | Zeugenerkennung | Sichtlinie+Gehör | Zeugen konsistent | E-WE-021 |
| T-IE-031 | S | U | Fahrzeug-Innenraum | reale Innenmaße | Möglichkeiten korrekt | E-IA-035 |
| T-IE-032 | S | U | Wetter/Tageszeit | Sichtbarkeit | verändert Zeugen/Privatheit | E-IA-035 |
| T-IE-033 | W | S | Anfangsposition (Entscheidung offen) | – | Test nach Entscheidung | E-SO-009 |
| T-IE-034 | W | P | Solver-Zeit pro Frame | Budget | eingehalten (Budget festlegen) | E-SP-001 |
| T-IE-035 | W | U | Mehrspieler: autoritativer Zustand | Latenz | Konsistenz, kein Desync-Safety-Bypass | E-SP-005 |

## 4. Körper und Reaktion

Spezifikation: [Modul 23](23-Spezifikation-Reaktions-und-Koerpersystem.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-KR-001 | W | S | Körperparameter-Fuzz (1.000 Körper) | plausible Kopplungen, Asymmetrie vorhanden | E-KO-001 |
| T-KR-002 | W | S | Keine festen Stufen | stetige Parameter | E-KO-014 |
| T-KR-003 | W | S | Gewichtsänderung | Körper/Weichgewebe verändern sich | E-SO-027 |
| T-KR-004 | W | S | Schwangerschaft über 9 Monate | Parameter ändern sich plausibel | E-GE-011 |
| T-KR-005 | W | U | Sport-BH dämpft | Parameter ändern Mechanik | E-KO-014 |
| T-KR-006 | W | U | Weichgewebe pro Zone | links/rechts verschieden | E-SO-026 |
| T-KR-007 | W | U | Sekundärbewegung nicht pauschal nach Größe | gleiche Größe, andere Anatomie → andere Bewegung | E-SO-024 |
| T-KR-008 | W | S | Reaktions-Determinismus | gleicher Seed → gleiche Zusammensetzung | E-SO-016 |
| T-KR-009 | K | S | `HardStop` in jedem Reaktionszustand | konsistent; Abbruch | E-SK-019 |
| T-KR-010 | K | S | Positive Reaktion macht Grenzübertritt nicht rückwirkend ok | Zustand bleibt `NoConsent` | 35#632 |
| T-KR-011 | W | S | Erstarren | kein automatisches Pos/Neg; Check-in | 35#652 |
| T-KR-012 | W | S | Reaktionsprofile individuell | zwei Figuren, gleiche Handlung | verschieden | E-KO-021 |
| T-KR-013 | W | S | Content-Toggle | Darstellung ändert sich, Physiologie gleich | E-ST-011 |
| T-KR-014 | W | S | Verletzung → Folgesitzung | höheres Risiko, längere Heilung | E-GE-006 |
| T-KR-015 | W | S | Keine Heilung während Sitzung | Verletzungsstand bleibt | E-GE-010 |
| T-KR-016 | W | S | Risikoklassen monoton | Last ↑ → Risiko ↑ | E-GE-010 |
| T-KR-017 | W | S | Erfahrung ≠ Widerstand | Erfahrung erhöht Schwelle nicht automatisch | E-GE-010 |
| T-KR-018 | W | S | Notfall → Medizinsystem | schwere Verletzung | normales Medizin-/Notrufsystem, kein Adult-Sonderpfad | E-GE-010 |
| T-KR-019 | K | S | Bewusstlosigkeit beendet | alle Pfade | Interaktion endet | E-SK-017 |
| T-KR-020 | W | S | Hormonelle Verhütung | Stimmung/Libido beeinflusst | E-GE-003 |
| T-KR-021 | W | S | Verhütungs-Anwendungsfehler | Wirksamkeit sinkt | E-GE-002 |
| T-KR-022 | W | S | Schwangerschaft nur bei passender Biologie | Biologie nicht passend → nie | E-GE-002 |
| T-KR-023 | W | S | Kein usageCount | wiederholte Nutzung ändert Anatomie nicht | E-KO-016 |
| T-KR-024 | W | S | Keine STI-Zustände | – | E-GE-004 |
| T-KR-025 | W | S | Erregung kein Balken | UI-Scan | nicht sichtbar | E-IA-021 |
| T-KR-026 | W | S | Kein SexSkill | Schema-Scan | nicht vorhanden | E-IA-021 |
| T-KR-027 | W | S | Orgasmus nicht garantiert | Statistik über viele Läufe | variabel | E-IA-019 |
| T-KR-028 | W | S | Libido nicht konstant | Zeitreihe | schwankt | E-NP-023 |
| T-KR-029 | W | S | Schmerz ≠ Schaden | Toleranz hoch | Schaden unverändert | E-GE-010 |
| T-KR-030 | W | S | Keine „weil gewollt, passiert nichts“-Regel | Praxis mit hoher Last | Verletzungssystem greift | E-KO-019 |
| T-KR-031 | S | U | Flüssigkeitssystem (Spät) | – | später; hinter Feature Flag | E-IA-033 |
| T-KR-032 | W | S | Mod registriert Reaktion | Graph unverändert | E-SP-006 |
| T-KR-033 | W | U | Atemsynchronisierung | Brustbewegung/Audio | synchron | E-KO-022 |
| T-KR-034 | W | S | Stimme: Lautstärke situativ | Ort/Nachbarn | angepasst | E-IA-035 |

## 5. NPC und Gesellschaft

Spezifikation: [Modul 24](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-NP-001 | W | S | NPC-Generierung 100.000 | Verteilungen plausibel, deterministisch | E-NP-005 |
| T-NP-002 | W | S | Plausibilitätscheck | Archetyp-Bias, keine absurden Kombinationen | 35#260 |
| T-NP-003 | W | S | Präferenz-DNA nur bei Erzeugung | später unverändert (nur Erlebnisse ändern) | E-NP-012 |
| T-NP-004 | K | S | Hard Limit unter Überredung | bleibt | E-NP-006 |
| T-NP-005 | W | S | Beziehung ersetzt Zustimmung nicht | – | E-SK-001 |
| T-NP-006 | W | S | Chemie bidirektional | A→B ≠ B→A möglich | E-NP-007 |
| T-NP-007 | W | S | Einmalbegegnung kann Beziehung werden | Übergang möglich | E-NP-018 |
| T-NP-008 | W | S | Gutes Date ≠ zwingend Adult | Statistik über viele Läufe | E-NP-017 |
| T-NP-009 | W | S | Initiative: NPC flirtet/kontaktiert | Ereignisse | E-NP-011 |
| T-NP-010 | W | S | Gedächtnis nach Widerruf | Beziehung/Vertrauen verändert sich | NP-GD-04 |
| T-NP-011 | W | S | Anker-Erinnerungen | bleiben bei Zusammenfassung | E-NP-016 |
| T-NP-012 | W | S | Tod | Daten bleiben, Interaktion gesperrt | E-SK-014 |
| T-NP-013 | W | S | Zeugen-Sichtlinie | konsistent mit Geometrie/Audio | E-WE-021 |
| T-NP-014 | W | S | Gerüchte ≠ Fakten | Wahrheitsgehalt-Feld | E-WE-015 |
| T-NP-015 | W | S | Mehrdimensionaler Ruf | Updates getrennt | E-WE-007 |
| T-NP-016 | W | S | Vertrag: Kündigungsrecht beider Seiten | beide Wege führen zum Ende | E-NP-024 |
| T-NP-017 | K | S | Vertrag: Alters-/Identitätsprüfung | `NotAdultEligible` → kein Vertrag | E-NP-024 |
| T-NP-018 | K | S | Vertrag ersetzt Zustimmung nicht | – | E-SK-003 |
| T-NP-019 | W | S | Eigenkündigung aus Zustand | kein Zwangsflag; Auslösung aus Präferenz-/Beziehungsdaten | 35#549 |
| T-NP-020 | W | S | Rechtskette | Reihenfolge unverletzt | E-WE-014 |
| T-NP-021 | K | R | Keine Handlungsmechanik im Justizpfad | Datenmodell-Review | NP-RK-03 |
| T-NP-022 | W | S | Population 10 Spieljahre | stabil, Speicher im Budget | E-LE-005 |
| T-NP-023 | W | S | Simulation-LOD | Tier 3 nur Ereignisschritte | E-LE-002 |
| T-NP-024 | W | S | Materialisierung friert Zustand ein | – | E-LE-002 |
| T-NP-025 | W | S | NPC↔NPC-Interaktion | gleiche Prüfkette | E-IA-002 |
| T-NP-026 | W | S | Beobachter tritt bei | Neuprüfung für alle | E-IA-003 |
| T-NP-027 | W | S | Orientierung nach Datenmodell | Entscheidung K5 offen | Test nach Entscheidung | K5 |
| T-NP-028 | W | S | Keine „Jungfrau“-Shop-Kategorie | Registry-Scan | nicht vorhanden | E-WE-006 |
| T-NP-029 | W | S | Fantasie vs. Aktivität | getrennt gespeichert | E-IA-038 |
| T-NP-030 | W | S | Partnerprofil lernt fehlerhaft | Konfidenz | E-NP-029 |

## 6. Tattoos und Körpermodifikation

Spezifikation: [Modul 25](25-Spezifikation-Tattoo-und-Koerpermodifikation.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-TA-001 | K | S | Altersgate im Studio | `NotAdultEligible` → keine Leistung | Modul 25 |
| T-TA-002 | W | U | Abdeckung 0/50/100 % | Rendering ohne Brüche | E-TA-004 |
| T-TA-003 | W | U | Gewichtsänderung ±30 % | Tattoos verformen sich plausibel | E-TA-014 |
| T-TA-004 | W | U | Schwangerschaft + Bauchtattoo | Dehnung/Rückbildung | E-TA-014 |
| T-TA-005 | W | S | Fading 10 Jahre | gemäß Parametern | E-TA-006 |
| T-TA-006 | W | S | UV beschleunigt Fading | – | E-TA-006 |
| T-TA-007 | W | S | Cover-up abhängig von Skill | – | E-TA-006 |
| T-TA-008 | W | S | Laser mehrere Sitzungen | Restfarbe/Vernarbung | E-TA-006 |
| T-TA-009 | W | S | Save/Load mit 200 Tattoos | identisch | Modul 25 |
| T-TA-010 | W | U | Import 4K-Design | akzeptiert, LOD | E-TA-024 |
| T-TA-011 | W | S | Importprüfung | Metadaten/Lizenz erforderlich | TA-ED-04 |
| T-TA-012 | W | S | Verteilung 100.000 NPCs | Histogramm nahe Baseline (Toleranz festlegen) | E-TA-023 |
| T-TA-013 | W | S | Standortfaktor beim Preis | Innenstadt > Land | E-TA-019 |
| T-TA-014 | W | S | Wartezeit | 1–2 Spielwochen | E-TA-019 |
| T-TA-015 | W | M | Zeitraffer statt Echtzeit | Sitzung | E-TA-020 |
| T-TA-016 | W | S | Künstlerprofil mehrdimensional | gute Künstler nicht in jedem Stil gut | E-TA-008 |
| T-TA-017 | W | S | Gefängnis-Tattoo-Qualität | schlechter | E-TA-006 |
| T-TA-018 | W | U | LOD-Erkennbarkeit | aus 10–20 m erkennbar | E-TA-024 |
| T-TA-019 | W | P | Draw Calls bei Vollkörper | Bake reduziert | E-TA-024 |
| T-TA-020 | W | S | Piercing-Heilung/Unverträglichkeit | – | E-TA-010 |
| T-TA-021 | S | S | Intim-Optionen nur mit Paket | Editor ohne Paket | nicht sichtbar | Modul 07 |
| T-TA-022 | W | M | Editor/Nacktheit im Hauptspiel | ohne Paket | verfügbar | E-TA-015 |
| T-TA-023 | W | S | Klinik-Eingriff ändert Parameter | – | E-TA-027 |
| T-TA-024 | W | S | Mod entfernt Pigment | Fallback | – |
| T-TA-025 | S | R | Symbolik-Prüfung | Rechtsprüfung vor Entscheidung | K16 |

## 7. Welt und Wirtschaft

Spezifikation: [Modul 26](26-Spezifikation-Weltintegration-Branche-Wirtschaft.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-WE-001 | K | S | Transaktion ohne Zustimmung | Leistung nicht erbracht | E-SK-003 |
| T-WE-002 | K | S | Tarif „bis zur Ohnmacht“ | nicht erzeugbar | 35#545 |
| T-WE-003 | W | S | Vertragskündigung beider Seiten | Vertragsende | E-NP-024 |
| T-WE-004 | K | S | Altersprüfung bei Buchung | keine Buchung bei `NotAdultEligible` | E-NP-024 |
| T-WE-005 | W | S | Video-Replay-Konsistenz | gleiches Video | E-WE-010 |
| T-WE-006 | W | S | Cache-Limit | Obergrenze | 35#527 |
| T-WE-007 | W | S | Hausregel „keine Kameras“ | Aufnahme blockiert | E-IA-007 |
| T-WE-008 | W | S | Jurisdiktion | legal/illegal nach Daten | E-WE-004 |
| T-WE-009 | W | S | Aufnahme ≠ Veröffentlichung | getrennte Zustimmungen | E-WE-011 |
| T-WE-010 | W | S | Meinungsänderung vor Veröffentlichung | Veröffentlichung blockiert | E-WE-011 |
| T-WE-011 | W | S | Beruf ≠ private Verfügbarkeit | Dienstleister lehnt privat ab | E-WE-009 |
| T-WE-012 | W | S | Nicht jeder bietet jeden Service | Profile | E-WE-001 |
| T-WE-013 | W | S | Betriebsdichte nach Daten | synthetische Karte | E-WE-006 |
| T-WE-014 | W | S | Preis ≠ Qualität | Korrelation nicht 1 | E-WE-017 |
| T-WE-015 | W | S | Zeitmaßstab (nach Entscheidung) | Test nach K15 | K15 |
| T-WE-016 | S | S | Medienproduktion startet klein | Wachstum | E-WE-019 |
| T-WE-017 | W | S | Kein Echtgeld-Adult (offen) | – | WI-WP-04 |

## 8. Steuerung, UI, VR und Replay

Spezifikation: [Modul 27](27-Spezifikation-Steuerung-UI-VR-Replay.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-ST-001 | K | S | Eingabe → Absicht | kein Eingabepfad umgeht Gate | ST-IR-06 |
| T-ST-002 | W | S | Remapping-Konflikte | erkannt, Warnung | E-ST-005 |
| T-ST-003 | W | S | Hybrid: gleiche Absicht über Analog und Bibliothek | gleiche Sitzung | E-ST-002 |
| T-ST-004 | W | U | Spamming nicht optimal | Hämmern nicht schneller als Analog | E-ST-001 |
| T-ST-005 | W | S | Aktionsbibliothek erst nach Freischaltung | Sichtbarkeit | E-ST-004 |
| T-ST-006 | W | S | Begründung bei ungültiger Aktion | Text vorhanden | E-ST-007 |
| T-ST-007 | W | S | HUD-Stufen | alle 5 | E-ST-005 |
| T-ST-008 | W | S | Pose-Overlay Standard aus | – | E-SO-018 |
| T-ST-009 | W | S | Setup-Assistent | Standardwerte bei Skip | E-ST-008 |
| T-ST-010 | W | S | Replay-Determinismus | Re-Render identisch (Toleranz) | E-RP-008 |
| T-ST-011 | W | S | HUD aus im Re-Render | kein HUD | E-RP-002 |
| T-ST-012 | W | P | Re-Render im Hintergrund | Gameplay-Leistung bleibt | E-RP-003 |
| T-ST-013 | W | S | Content-Toggle | Darstellung ändert, Physiologie gleich | E-ST-011 |
| T-ST-014 | W | U | VR vs. Flat | gleiche Ergebnisse | E-SO-031 |
| T-ST-015 | W | S | Streaming-Filter | markierte Szenen ausgeblendet | E-RP-005 |
| T-ST-016 | W | S | Replay ≠ Savegame | getrennt | E-RP-008 |
| T-ST-017 | S | M | Barrierefreiheit | alle Funktionen ohne feine Motorik erreichbar (Kriterien festlegen) | E-ST-005 |
| T-ST-018 | W | S | Streaming ohne Zusatzprogramm | Ein-Klick | E-RP-004 |
| T-ST-019 | W | S | Speicherschätzung des Exports | angezeigt | E-RP-001 |
| T-ST-020 | W | S | Replay mit fehlendem Mod | Begründung statt Absturz | Modul 28 |
| T-ST-021 | W | S | Statistik nur bei aktivem Paket sichtbar | – | E-SP-004 |

## 9. Speicherung, Mods und Builds

Spezifikation: [Modul 28](28-Spezifikation-Speicherung-Mods-Builds.md).

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-SM-001 | K | V | Basisbuild-Scan | keine Adult-Assets/Symbole/Verweise | E-PA-032 |
| T-SM-002 | K | V | Basisbuild baut ohne Adult-Module | Build erfolgreich | E-PA-028 |
| T-SM-003 | K | S | Kein `if AdultModInstalled` im Kern | Quellscan | E-PA-004 |
| T-SM-004 | K | S | Mod ändert Eligibility/Zustimmung/Block | abgelehnt | E-SK-004 |
| T-SM-005 | W | S | Mod fordert Netzwerk | verweigert | E-PA-036 |
| T-SM-006 | W | S | Hot Disable/Enable | Daten erhalten | E-PA-030 |
| T-SM-007 | W | S | Absturz beim Speichern | konsistenter Stand (Journal) | SM-SP-03 |
| T-SM-008 | W | S | Migration v(n)→v(n+1) | Golden Files | E-SP-001 |
| T-SM-009 | K | S | Offizielles Online | Mods deaktiviert, Paket nicht verfügbar | E-PA-006 |
| T-SM-010 | W | S | Koop-Host aktiviert Paket | Gast definiert | E-SP-005 |
| T-SM-011 | W | R | Provenienz-Vollständigkeit | Release-Gate bei Lücken | E-RC-009 |
| T-SM-012 | W | P | Performance-Budgets je Subsystem | erfüllt (Budgets festlegen) | E-PR-012 |
| T-SM-013 | W | S | Doppelte Mod-ID | Konflikt erkannt | E-SP-006 |
| T-SM-014 | W | S | Mod-Abhängigkeiten requires/incompatible | korrekt | E-PA-019 |
| T-SM-015 | W | S | Mod-Absturz | Isolation | E-PA-030 |
| T-SM-016 | W | S | Paket im Build ohne Paket geladen | verwaiste Daten markiert, Spiel läuft | E-PA-030 |
| T-SM-017 | W | S | Gast ohne Paket tritt bei | definiertes Verhalten | K21 |
| T-SM-018 | W | S | Keine Lizenz-/Veröffentlichungsartefakte im Repo | Repo-Scan | CLAUDE.md §9 |
| T-SM-019 | S | S | Verschlüsselter Adult-Bereich | Lesbarkeit je Schlüssel | E-PA-030 |
| T-SM-020 | W | S | Schemaversion im Adult-Bereich | vorhanden | E-SP-004 |

## 10. Querschnitt: Regression, Fuzzing, Last

| ID | Prio | Art | Testziel | Erwartung | Basis |
|---|---|---|---|---|---|
| T-QS-001 | W | S | Massentest „tausende Kombinationen“ | kein Absturz, Gates halten | E-SP-001 |
| T-QS-002 | W | S | Golden Tests für Entscheidungen | stabil über Versionen | E-SP-001 |
| T-QS-003 | W | S | Property-Test: Gate ist deterministisch | gleiche Eingabe → gleiche Ausgabe | E-SK-029 |
| T-QS-004 | K | S | Fuzzing der Zustimmungsübergänge | keine verbotenen Übergänge | E-SK-026 |
| T-QS-005 | W | P | Stresstest 500 Sitzungen (Server) | Budget (keine Behauptung ohne Messung) | 28#25 |
| T-QS-006 | W | M | Playtest-Protokoll | Checkliste je Kategorie | E-PR-012 |
| T-QS-007 | W | S | Regression nach Registry-Änderung | alle Gate-Tests grün | E-PR-009 |

## 11. Testarten und Ausführungsumgebung

Nach CLAUDE.md §5 sind drei Arten klar zu trennen:

| Art | Voraussetzung | Wo |
|---|---|---|
| Standalone-C++-Tests (S) | CMake + Testframework, **kein** Unreal | Fedora-Laptop möglich |
| Unreal-Kommandozeilenautomation (U) | Unreal-Binaries, Editor-GUI nicht zwingend | Windows-Desktop |
| Paketierte Builds / Playtests (M, P) | Unreal-Laufzeit | Windows-Desktop |

Die Wahl des Testframeworks ist **noch offen** (CLAUDE.md §5: nach Review der Architektur).

## 12. Nicht testbar / nicht freigegeben

Für folgende Themen **existieren absichtlich keine Testfälle**, weil sie nicht freigegeben sind: Fortsetzen von Handlungen gegen erkennbaren Willen oder bei Schlafenden/Bewusstlosen (K2); Nicht-Einwilligungs-Kataloge; Handlungsmechanik für Sexualstraftaten (K3); Tarnung von Mods als Fan-Mods (K13). Tests dürfen nur die **Verweigerung** dieser Pfade (Gate lehnt ab, Datenmodell kennt sie nicht) prüfen.
