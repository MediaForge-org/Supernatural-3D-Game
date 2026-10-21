# Adult 20: Spezifikation Regelkern (Alter, Fähigkeit, Zustimmung, Stopp)

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dies ist eine **Spezifikationsskizze der Dokumentation** (🧭 Vorschlag), abgeleitet aus bestätigten und vorläufigen Quellenaussagen. Sie ist **kein Kanon** und **kein Auftrag zur Implementierung**. Die Herkunft jeder Anforderung steht in der Spalte „Master-Basis“ (Register-IDs aus [Modul 14](14-Entscheidungsregister.md)). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachliche Grundlage: [Modul 01](01-Grundregeln-Alter-Zustimmung.md). Daten: [Modul 21](21-Spezifikation-Datenmodell.md). Tests: [Modul 29](29-QA-Testkatalog.md). Abnahme: [Modul 30](30-Abnahmekriterien.md). Randfälle: [Modul 31](31-Randfaelle-Katalog.md). Konflikte: [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md).

**Inhalt**

1. [Geltungsbereich und Prinzipien](#1-geltungsbereich-und-prinzipien)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Prüfkette (Gate-Reihenfolge)](#3-prüfkette-gate-reihenfolge)
4. [Altersgate](#4-altersgate)
5. [Fähigkeitsgate (Capacity)](#5-fähigkeitsgate-capacity)
6. [Zustimmung als Datensatz](#6-zustimmung-als-datensatz)
7. [Sitzungsrahmen und Live-Signale](#7-sitzungsrahmen-und-live-signale)
8. [Stopp, Unbehagen, Widerruf](#8-stopp-unbehagen-widerruf)
9. [Zustände, Ereignisse, Daten](#9-zustände-ereignisse-daten)
10. [Technische Komponenten](#10-technische-komponenten)
11. [Randfälle](#11-randfälle)
12. [Tests und Abnahmekriterien](#12-tests-und-abnahmekriterien)
13. [Ungeklärtes](#13-ungeklärtes)

---

## 1. Geltungsbereich und Prinzipien

Der Regelkern entscheidet, **ob** eine Interaktion mit Erwachsenenbezug für eine Gruppe von Teilnehmern beginnen, fortgesetzt oder weiterentwickelt werden darf. Er entscheidet **nicht**, was inhaltlich geschieht (das regeln [Modul 22](22-Spezifikation-Interaktionsengine.md) und [Modul 23](23-Spezifikation-Reaktions-und-Koerpersystem.md)).

| Nr. | Prinzip | Master-Basis |
|---|---|---|
| P1 | Fail closed: bei Unwissen, Fehler, fehlenden Daten oder Timeout wird **nicht** gestartet bzw. sicher beendet. | E-SK-010, E-SK-028 |
| P2 | Alter, Fähigkeit und Zustimmung sind getrennte Prüfungen in fester Reihenfolge; keine kann eine andere ersetzen. | E-SK-007, E-SK-029 |
| P3 | Beziehung, Story, KI-Ziele, Admin- oder Party-Rechte, Preise und Verträge ersetzen Zustimmung nie. | E-SK-001, E-SK-003, E-SK-008 |
| P4 | Der Kern ist nicht moddbar; Mods dürfen Inhalte, aber nicht diese Regeln ändern. | E-SK-004, E-PA-034 |
| P5 | Entscheidungen sind deterministisch und inspizierbar (jede Ablehnung hat einen Grund-Code). | E-SK-029, E-SK-016 |
| P6 | Realistisch statt Mikromanagement: keine Dauer-Popups, aber ein klares Nein/Stopp zählt sofort. | E-SK-022, E-SK-023 |
| P7 | Die Prüfungen hängen nie vom **Aussehen** ab; Eingabe sind Datenfelder. | E-SK-010, E-SK-031 |

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Mindestalter 18 Jahre 0 Tage; darunter nicht erlaubt und nicht angezeigt | 📋 | E-SK-009 |
| Unbekanntes Alter schließt aus; Figur kann gefragt werden und einen Nachweis zeigen | 📋 / 🧭 | E-SK-010, E-SK-011 |
| Zustimmung ist kein Bool; Beziehung ersetzt sie nie | 📋 | E-SK-001 |
| Betrunkene, die noch denken und antworten können, können wirksam zustimmen; starke Beeinträchtigung nicht | 📋 | E-SK-015 |
| Bewusstlos/nicht ansprechbar: Interaktion endet nach kurzer Erkennungsverzögerung | 📋 | E-SK-017 |
| Stopp-Signal beendet sofort | 📋 | E-SK-019 |
| Sitzungsrahmen statt Dauerabfrage; Stöhnen allein genügt nicht | 📋 | E-SK-022 |
| Formel `capacity + responsiveness + prior affirmative intent + current behavior` | 🧭 | E-SK-016 |
| Zustimmungszustände (9) und Datensatz mit Scope | 🧭 | E-SK-026, E-SK-027 |
| Gewichtung der Live-Signale, Zeitkonstanten, Schwellen | ❓ | [K1](19-Konflikt-und-Widerspruchsmatrix.md#k1-frühe-zustimmungsvorstellungen-vs-sicherheitskern) |
| Handeln gegen den erkennbaren Willen, bei Schlafenden/Bewusstlosen (auch Admin-Build) | ⛔ | [K2](19-Konflikt-und-Widerspruchsmatrix.md#k2-user-lock-vom-2009) |
| Einvernehmlicher Schlaf-Zustand | ❓ | [K9](19-Konflikt-und-Widerspruchsmatrix.md#k9-einvernehmlicher-schlaf-zustand) |

## 3. Prüfkette (Gate-Reihenfolge)

🧭 Vorschlag. Jede Stufe liefert `Allow`, `Deny(reason)` oder `Defer(reason)`; die erste Nicht-`Allow`-Stufe entscheidet.

| Stufe | Prüfung | Eingaben | Ergebnis bei Fehler |
|---|---|---|---|
| 0 | **Paket aktiv?** Adult-Paket geladen, für diesen Speicherstand aktiviert, Host-Erlaubnis im Mehrspielermodus | Konfiguration | `Deny(PackageInactive)` |
| 1 | **Altersgate** für **jeden** Teilnehmer | Geburtsdatum/Verifikationszustand | `Deny(NotAdultEligible)` |
| 2 | **Fähigkeitsgate** für jeden Teilnehmer | Bewusstsein, Ansprechbarkeit, Beeinträchtigung, Zustand | `Deny(NotCapable)` oder `Defer(Observe)` |
| 3 | **Verfügbarkeit**: nicht in Kampf, nicht gefangen/gefesselt ohne Vereinbarung, nicht kritisch verletzt, nicht in Story-Sperre | Weltzustand | `Deny(Unavailable)` |
| 4 | **Harte Grenzen** der Teilnehmer für die beabsichtigte Handlungsfamilie | Präferenzprofil | `Deny(HardLimit)` |
| 5 | **Zustimmung/Sitzungsrahmen** deckt die Handlung ab | Zustimmungsdatensatz, Live-Signale | `Deny(NoConsent)` / `Defer(AskOrOffer)` |
| 6 | **Ort/Privatheit/Hausregeln** | Venue Policy, Recht | `Deny(VenueRule)` oder Weltfolgen (kein Interaktionsverbot des Kerns) |
| 7 | **Kapazität des Systems** (Animationsset, Platz, Budget) | Solver-Vorprüfung | `Defer(Fallback)` |

Hinweise:
- Stufe 6 und 7 betreffen **Umsetzbarkeit und Weltfolgen**, nicht Sicherheit. Sie dürfen nie die Stufen 1 bis 5 überspringen oder abschwächen (E-IA-007).
- Die Prüfkette läuft **bei jedem Start**, bei **jedem Wechsel der Handlungsfamilie** über den Sitzungsrahmen hinaus (siehe Abschnitt 7), bei **neuem Teilnehmer**, bei **Eskalation der Intensität über den Rahmen**, und nach **längerer Unterbrechung** (E-SK-027).

## 4. Altersgate

| Nr. | Anforderung (🧭) | Master-Basis |
|---|---|---|
| ZU-AL-01 | Das Alter ergibt sich aus dem **Geburtsdatum** und dem Weltdatum; es gibt kein manuell setzbares Feld „Alter = 18“. | E-SK-012 |
| ZU-AL-02 | Eligibility hat drei Werte `Unknown`, `NotEligible`, `AdultEligible`; Standard ist `Unknown`; nur `AdultEligible` besteht Stufe 1. | E-SK-028 |
| ZU-AL-03 | Das Gate liest **keine** Aussehens-, Körper-, Stimm- oder Kleidungsdaten. | E-SK-010 |
| ZU-AL-04 | Verstorbene Figuren haben keinen aktiven Zustand für Interaktionen; ihre Daten bleiben für Gedächtnis/History. | E-SK-014 |
| ZU-AL-05 | Der Zustand `AdultEligible` kann nicht durch Mods, Skripte, Story-Ereignisse oder Cheats gesetzt werden, sondern nur durch die Alterslogik des Spielkerns aus dem Geburtsdatum. | E-SK-004 |
| ZU-AL-06 | Der Nachweis ‚zeigt Ausweis‘ ist ein **Ereignis in der Welt**; gespeichert wird nur das Ergebnis `verifizierter Erwachsener`, kein Ausweisinhalt. | E-SK-011, E-SK-012 |
| ZU-AL-07 | Neu erzeugte Figuren durchlaufen die Altersgrenze der Pipeline: Minderjährige sind für das Adult-Paket **unsichtbar** (kein Eintrag in Auswahllisten, kein Präferenzprofil, keine Anziehungsdaten). | E-SK-032 |
| ZU-AL-08 | Beim Altern wird Eligibility **am Weltdatum** berechnet; Tests prüfen Tag 17 Jahre 364/365 und Tag 18 Jahre 0. | E-SK-009, E-SK-012 |
| ZU-AL-09 | Visuelle Mindestreife sehr junger Erwachsener ist **Asset-Qualität**, kein Laufzeitgate; der Build-Validator prüft Pflichtfelder und Beschreibungen. | E-SK-031 |

## 5. Fähigkeitsgate (Capacity)

### 5.1 Ziel

Die Quellen halten fest: **leicht angetrunken** ist möglich; **stark beeinträchtigt** erzeugt keine Zustimmung; eine Person, die **noch denken und antworten kann**, kann zustimmen; **bewusstlos** beendet. Eine Promillegrenze gibt es nicht (E-SK-015, E-SK-016).

### 5.2 Eingaben (🧭)

| Eingabe | Beschreibung | Herkunft |
|---|---|---|
| `awareness` | wach / benommen / bewusstlos / schlafend | Zustandssystem |
| `responsiveness` | Antwortfähigkeit auf Ansprache und Berührung | Awareness-System (E-SK-018) |
| `cognitive_load` | Beeinträchtigung durch Alkohol, Drogen, Medikamente, Schmerz, Schock, Panik | Körpersystem |
| `orientation` | Weiß die Figur, wer, wo, was passiert? | Gedächtnis/Zustand |
| `prior_affirmative_intent` | Frühere bejahende Absicht in dieser Situation | Sitzungsrahmen |
| `current_behavior` | Aktive Beteiligung, Gegenbewegung, Körperhaltung, Stimme | Live-Signale |

### 5.3 Entscheidungstabelle (🧭 Beispielwerte sind nicht bestätigt)

| awareness | responsiveness | cognitive_load | Ergebnis |
|---|---|---|---|
| bewusstlos / schlafend | – | – | `Deny(NotCapable)` ; laufende Interaktion → Abbruch ([Abschnitt 8](#8-stopp-unbehagen-widerruf)) |
| benommen | niedrig | beliebig | `Deny(NotCapable)` |
| wach | normal | niedrig bis mittel | `Allow` |
| wach | normal | hoch | `Deny(NotCapable)` (z. B. kann nicht mehr klar antworten oder denken) |
| wach | niedrig | beliebig | `Defer(Observe)`, danach `Deny` |

Schwellen für „niedrig/mittel/hoch“ sind **Konfigurationsdaten** und bewusst nicht festgelegt (Entscheidungsbedarf: [Abschnitt 13](#13-ungeklärtes)).

### 5.4 Erkennungsverzögerung

📋 Der Entwickler akzeptierte (35#454), dass eine Bewusstlosigkeit kurz unbemerkt bleiben kann, **aber nicht minutenlang**. Der Assistent (35#453) schlug ein Responsiveness-System vor. Spezifikationsskizze (🧭):

- Alle Teilnehmer liefern kontinuierlich `responsiveness` und `awareness`.
- Ein **Erkennungsfenster** `T_detect` (Konfigurationswert, kein Beschlusswert) beginnt, sobald `responsiveness` unter den Schwellwert fällt. Läuft es ab, wechselt der Zustand auf `Unconscious/Unresponsive` und der Abbruch greift sofort.
- Das System zeigt dem Spieler **keine** Hilfeanzeige ‚Person bewusstlos‘; die Figur zeigt natürliche Signale (Muskelspannung, Atmung, ausbleibende Antworten).
- Kein Pfad darf die Interaktion über das Fenster hinaus weitersimulieren.

## 6. Zustimmung als Datensatz

### 6.1 Zustände

🧭 Aus E-SK-026: `Unknown`, `PotentiallyInterested`, `Offered`, `Considering`, `Accepted`, `Declined`, `Withdrawn`, `Expired`, `TemporarilyUnavailable`.

```
Unknown ──► PotentiallyInterested ──► Offered ──► Considering ──► Accepted ──► (aktiv)
   │                 │                    │            │             │
   │                 └────────────────────┴────────────┴─► Declined   └─► Withdrawn
   └────────────────────────────────────────────────────────────────────────► Expired / TemporarilyUnavailable
```

Regeln (🧭):
- `Accepted` gilt für den **Scope**, nicht pauschal (E-SK-027).
- `Declined` und `Withdrawn` werden **erinnert** (Gedächtnis, [Modul 24](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md)) und verhindern eine unmittelbare erneute Anfrage durch den Spieler im selben Moment.
- `Expired` tritt ein, wenn die Situation sich wesentlich ändert (E-SK-025).
- `TemporarilyUnavailable` ist ein Weltzustand (z. B. Telefonat, Kampf), keine Ablehnung.

### 6.2 Datensatz

| Feld | Bedeutung |
|---|---|
| `request_id` | Kennung der Anfrage/Sitzung |
| `participant_id` | globale NPC- oder Spieler-ID |
| `scope` | Stufe: Familie / Unterart / Variante / Profil |
| `allowed_content` | Familien, Unterarten, Varianten, die abgedeckt sind |
| `allowed_intensity` | Intensitätsachsen mit Grenzwerten (mehrdimensional) |
| `context_id` | Ort, Zeit, Anlass |
| `granted_at` / `validity` | Zeitpunkt, Gültigkeitsbedingung (Situation, Zeit) |
| `source` | `explicit`, `session_envelope`, `learned_preference` (Letzteres ist **nie** allein ausreichend) |
| `revoked_at` / `revoke_reason` | Widerruf |

### 6.3 Neuprüfung

Neuprüfung der Zustimmung ist erforderlich bei (E-SK-027):

1. Wechsel der **Handlungsfamilie** außerhalb des Rahmens,
2. **neuem Teilnehmer**,
3. **Anstieg einer Intensitätsachse** über den erlaubten Bereich,
4. **längerer Unterbrechung** (Konfigurationswert),
5. **Ortswechsel** mit anderer Privatheit, falls Teil des Rahmens.

### 6.4 Was ausdrücklich keine Zustimmung ist

| Signal | Begründung | Basis |
|---|---|---|
| Schweigen allein | Das Gespräch kam 35#576/578 darauf; der Assistent: „Stöhnen allein reicht nicht“ | E-SK-022 |
| Erregungszustand | Arousal ≠ Consent | E-SK-030 |
| Beziehungswert, Vertrauen, Zuneigung | beeinflussen Angebote, nicht Zustimmung | E-SK-008 |
| Preis, Vertrag, Dienstleistungsprofil | Berufsrolle ≠ private Verfügbarkeit | E-WE-009, E-SK-003 |
| Story- oder Quest-Zwang | ausgeschlossen | E-SK-003 |
| Admin-/Party-Leader-Rechte | ausgeschlossen | E-SK-003 |
| frühere Zustimmung in anderer Situation | neue Begegnung = neue Berechnung | E-SK-025 |
| Gelernte Präferenz | Wissen über Vorlieben ersetzt aktuelle Zustimmung nicht | E-NP-006 |

## 7. Sitzungsrahmen und Live-Signale

### 7.1 Sitzungsrahmen (Session Intent Envelope)

📋 Bestätigt (35#579–581): Wenn vorher etabliert wurde, dass die Person führen darf, Überraschungen akzeptiert oder innerhalb ihrer Grenzen frei wechselt, **muss das System nicht ständig nachfragen**. „Ja ok aber nicht zu streng“ (Entwickler 35#580).

🧭 Modell: Der Rahmen ist ein Zustimmungsdatensatz mit `source = session_envelope` und folgenden Feldern:

| Feld | Beschreibung |
|---|---|
| `lead_mode` | wer führt: Person selbst / Partner / wechselnd |
| `surprise_tolerance` | wie viel Abweichung vom Gesagten erlaubt ist (Skala, Konfigurationswert) |
| `family_whitelist` | erlaubte Familien |
| `hard_limit_overlay` | harte Grenzen bleiben aktiv |
| `intensity_corridor` | Korridor pro Achse |
| `stop_phrase` | vereinbartes Stopp-Signal ([Abschnitt 8](#8-stopp-unbehagen-widerruf)) |

### 7.2 Live-Signale

🧭 Liste aus 35#579: Sitzungsrahmen, harte Grenzen, partnerspezifische Präferenzen, **aktive Teilnahme / Gegenbewegung**, Körperhaltung, Stimme, **Schmerz-/Stress- vs. positive Signale**, **explizites Stopp/Nein sofort**.

Regeln:

| Nr. | Regel (🧭) | Basis |
|---|---|---|
| ZU-LS-01 | Ein **explizites Nein/Stopp, Wegdrücken oder Abwehr** wirkt sofort, ohne Auswertung eines Scores. | E-SK-023 |
| ZU-LS-02 | **Ruhig, aber beteiligt** (aktive Teilnahme, Blickkontakt, Gegenbewegung in Richtung der Handlung) unterbricht nicht. | E-SK-023 |
| ZU-LS-03 | **Schmerz-/Stresssignale** (Zucken, Versteifen, Atemanhalten ohne Lustsignale) lösen einen **Check-in** aus, nicht automatisch einen Abbruch; bleibt die Antwort aus, wird abgebrochen. | E-SK-021 |
| ZU-LS-04 | Eine **Handlung außerhalb** des Rahmens (neue Familie, deutliche Eskalation) erfordert eine Neuprüfung: Angebot an die Figur, nicht stillschweigende Ausführung. | E-SK-027, E-SK-034 |
| ZU-LS-05 | **Versehentlicher Kontakt** (z. B. unabsichtlich anderer Körperbereich) ist keine Zustimmung zu einer neuen Handlung; die Figur reagiert, die Handlung wird korrigiert oder beendet. | 35#575/577, E-SK-034 |
| ZU-LS-06 | Die Auswertung liefert pro Entscheidung eine **Begründungsliste** (welche Signale), abrufbar im Developer Inspector. | E-SK-029 |

> **Offen:** Gewichte, Zeitfenster und Schwellen (wie viele Sekunden Passivität führen zum Check-in) sind **nicht** festgelegt und werden im Prototyp ermittelt ([K1](19-Konflikt-und-Widerspruchsmatrix.md)).

## 8. Stopp, Unbehagen, Widerruf

📋 Aus 23#569–571, K§18, 35#187: Ein vereinbartes Stopp-Signal **beendet sofort**; weiche Signale unterscheiden sich vom Rückzug; manche Erwachsene bevorzugen einvernehmlich höhere Intensität.

### 8.1 Signalklassen (🧭)

| Klasse | Beispiele | Wirkung |
|---|---|---|
| `HardStop` | vereinbartes Stopp-Signal; deutliches ‚Nein/Aufhören‘; Wegstoßen; Fluchtreaktion | **Sofort**: logische Beendigung → Sperre weiterer Eskalation → sicherer Ausstieg → neutraler Zustand |
| `Withdraw` | Rücknahme einer Zustimmung („doch nicht“) | wie `HardStop`, Zustimmung → `Withdrawn` |
| `DiscomfortCue` | verzerrte Mimik, Verspannung, Verlangsamen | Check-in, Intensität senken, Repositionierung |
| `PreferenceCue` | „anders“, „langsamer“ | Anpassung innerhalb des Rahmens |
| `IntensityAdjustment` | Wunsch nach mehr oder weniger | Achsen im Korridor anpassen |
| `RepositionRequest` | Wechsel der Haltung | Transition |
| `SafetyInterrupt` | Bewusstlosigkeit, Verletzung, Notfall | Abbruch, medizinischer Ablauf |

### 8.2 Ablauf bei `HardStop` (🧭, aus E-SK-028)

1. **Logisches Ende sofort**: Zustimmung → `Withdrawn`; keine neue Aktion darf beginnen.
2. **Sperre**: weitere Progression gesperrt (Lockout).
3. **Sicherer Übergang**: Kontaktlösung per Solver (Abstand, Entlastung); keine Teleportation (E-SO-021).
4. **Neutraler Ausstieg**: Idle/Reaktion der Figur; Kamera folgt dem Normalzustand.
5. **Aufräumen**: Kleidung/Items/Zustand; Ereignis für Gedächtnis ([Modul 24](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md)).
6. **Fallback**: Fehlen Animationen, greift die Rückfallleiter ([Modul 31](31-Randfaelle-Katalog.md)); es gibt **keinen Softlock**, und ein sicherer Ausstieg ist **immer** erreichbar.

### 8.3 Spieler-Stopp

Der Spieler kann **jederzeit** beenden (Pause, Stopp). Ein Spieler-Stopp wirkt ebenfalls als `HardStop` für die eigene Beteiligung; der NPC erhält ein Ereignis ohne Schuldzuweisung.

## 9. Zustände, Ereignisse, Daten

### 9.1 Ereignisse (🧭)

| Ereignis | Auslöser | Abonnenten |
|---|---|---|
| `EligibilityEvaluated` | Altersgate | Inspector, Tests |
| `CapacityChanged` | Awareness/Beeinträchtigung | Interaktion, Gedächtnis |
| `ConsentGranted` / `ConsentDeclined` / `ConsentWithdrawn` | Zustimmung | Gedächtnis, Beziehung, Statistik |
| `SessionEnvelopeEstablished` | Rahmen | Interaktion |
| `LiveSignalObserved` | Signal | Entscheidung |
| `HardStopRaised` | Stopp | Interaktion, Gedächtnis, Replay |
| `GateDenied(reason)` | Prüfkette | Inspector, UI (Begründung ohne Spoiler) |
| `SafetyInterrupt(reason)` | Notfall | Gesundheit, Welt |

### 9.2 Datenhaltung

- Eligibility und Zustimmungshistorie gehören zum **Adult-Speicherbereich** (E-PA-029/030), verknüpft über die globalen IDs.
- Die Altersdaten selbst (Geburtsdatum) gehören zum **Basis-Charakterdatensatz** (nicht zum Adult-Bereich), damit das Hauptspiel ohne Paket konsistent bleibt (E-PA-028).
- Zustimmung gilt **nur für die Dauer der Situation**; persistiert werden Ergebnisse und Erinnerungen, nicht Frames (28#25; Persistenzgrundsatz E-LE-002).

## 10. Technische Komponenten

🧭 Vorschlag, engine-neutral (kein Unreal-Typ im Kern):

| Komponente | Aufgabe | Schnittstelle (Idee) |
|---|---|---|
| `EligibilityEvaluator` | Altersgate | `evaluate(character_id, world_date) → Eligibility` |
| `CapacityEvaluator` | Fähigkeitsgate | `evaluate(character_state) → CapacityResult` |
| `ConsentLedger` | Zustimmungsdatensätze, Historie | `grant/withdraw/query(participant, scope)` |
| `SessionEnvelope` | Rahmen | `establish(...)`, `covers(action)` |
| `LiveSignalAnalyzer` | Signale → Entscheidung | `observe(signal)`, `decide() → Allow/Check/Stop` |
| `GateChain` | feste Reihenfolge | `run(request) → Decision` |
| `StopCoordinator` | `HardStop`-Ablauf | `raise(reason)` |
| `GateInspector` | Debug-Ausgabe der Begründung | nur in Entwicklungsbuilds |

Rahmen:
- Der Kern enthält **keine** Verweise auf Unreal-Typen; Unreal-Adapter melden Zustände (Awareness, Signale) an den Kern (E-PA-025).
- Der Kern ist über `ctest`/Standalone-Tests ohne Engine prüfbar (CLAUDE.md §5).
- Konfigurationswerte (Schwellen, Zeitfenster) liegen in Daten, nicht im Code.

## 11. Randfälle

Die vollständige Liste steht in [Modul 31](31-Randfaelle-Katalog.md#1-regelkern). Auszug:

- Teilnehmer wechselt während der Interaktion in den Zustand `benommen` (Medikament, Treffer, Panik).
- Mehrere Teilnehmer: einer widerruft, die anderen sind einverstanden → die Sitzung wird für den Widerrufenden beendet; für die übrigen nur, wenn sie ohne ihn weiterlaufen kann und sie zustimmen.
- Verbindungsabbruch im Mehrspielermodus während `HardStop`.
- Speichern/Laden mitten im Stopp-Ablauf.
- Zeitraffer (Schlaf/Warten) während einer Sitzung.
- NPC-zu-NPC-Interaktion mit Spieler als Beobachter: dieselbe Prüfkette gilt für NPC-Teilnehmer.

## 12. Tests und Abnahmekriterien

Tests sind aus bestätigten Aussagen abgeleitet. Vollständig in [Modul 29](29-QA-Testkatalog.md#1-regelkern); Abnahme in [Modul 30](30-Abnahmekriterien.md#1-regelkern).

| Test (Kurz) | Erwartung | Basis |
|---|---|---|
| Alter 17 Jahre 364 Tage / 18 Jahre 0 Tage | `NotEligible` / `AdultEligible` | E-SK-009 |
| Unbekanntes Alter | `Deny(NotAdultEligible)` | E-SK-010 |
| Aussehen ändern, Daten gleich | Entscheidung unverändert | E-SK-010 |
| Bewusstlos | `Deny`, laufende Interaktion endet im Erkennungsfenster | E-SK-017 |
| Leicht beeinträchtigt, ansprechbar, bejahend | `Allow` | E-SK-015 |
| Stark beeinträchtigt, antwortet nicht klar | `Deny` | E-SK-006 |
| Beziehung 100 %, keine Zustimmung | `Deny(NoConsent)` | E-SK-001 |
| `HardStop` | sofort `Withdrawn`, kein neuer Kontakt, sicherer Ausstieg | E-SK-019 |
| Fehlende Ausstiegsanimation | Rückfallleiter, kein Softlock | E-SK-028 |
| Mod versucht, Eligibility zu setzen | abgelehnt, protokolliert | E-SK-004 |
| Gleiche Eingaben, gleicher Seed | gleiche Entscheidung | E-SK-029 |

## 13. Ungeklärtes

| Frage | Bezug |
|---|---|
| Schwellen und Zeitfenster für Capacity, Erkennungsfenster, Check-in | K1 |
| Gewichtung der Signale, wann ‚ruhig und beteiligt‘ in „unsicher“ übergeht | K1 |
| Umgang mit Schlaf-Zuständen (einvernehmliches Schlafspiel) | K9 |
| Alters-/Zustimmungsprüfung bei Gästen im Koop | K21 |
| Darf ein Rahmen über mehrere Tage gelten (z. B. Dauerbeziehung)? Die Quellen sagen: neue Begegnung = neue Berechnung (E-SK-025); Dauerrahmen ist nicht beschlossen. | – |
| Abnahme der Reaktions-Plausibilität (wann wirkt eine Reaktion „überzeugend“)? | Prototyp |
| Spezifikation eines Konsequenz-Testpfads ohne Handlungsmechanik | K2 |
