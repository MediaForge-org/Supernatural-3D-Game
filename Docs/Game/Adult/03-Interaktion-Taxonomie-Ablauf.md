# Adult 03: Interaktions-Framework, Taxonomie und Ablauf

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Dokument beschreibt Planung auf Struktur- und Metadatenebene. Es enthält bewusst **keinen Katalog einzelner expliziter Handlungen**. Der Entwickler hat die Einzelkategorien im Interview auf abstrakter Ebene festgelegt; die vollständige Registry ist ausdrücklich noch nicht eingefroren. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Voraussetzung: [Modul 01](01-Grundregeln-Alter-Zustimmung.md).

**Inhalt**

1. [Allgemeines Interaktions-Framework](#1-allgemeines-interaktions-framework)
2. [Taxonomie](#2-taxonomie)
3. [Positionen: Freeform statt Menü](#3-positionen-freeform-statt-menü)
4. [Zustandsautomat einer Interaktion](#4-zustandsautomat-einer-interaktion)
5. [Eigenschaften des Ablaufs](#5-eigenschaften-des-ablaufs)
6. [Kontext statt Kategorie: Dienstleistung, Rollenspiel](#6-kontext-statt-kategorie-dienstleistung-rollenspiel)
7. [Stand der Registry und der Audits](#7-stand-der-registry-und-der-audits)

---

## 1. Allgemeines Interaktions-Framework

📋 Romantik und Intimität werden **nicht** als isoliertes Spezialsystem gebaut, sondern als Module eines allgemeinen **Multi-Charakter-Interaktions-Frameworks**:

```text
Interaktionskern
├── Sozial            Begrüßung, Verabschiedung, Handschlag, Gespräch
├── Zuneigung         Umarmung, Trost, Kuscheln, Händehalten
├── Hilfe             Helfen, Tragen, medizinische Versorgung
├── Ruhe              Sitzen, Ausruhen, Schlafen
├── Romantik
└── Intimität         (nur im optionalen Paket erweitert)
```

Gemeinsame Infrastruktur: Teilnehmer, Sitzungen, stabile IDs, Anfragen, Berechtigungen, Reservierungen (Möbel, Anker), Annäherung, Ausrichtung, Motion Warping, Ganzkörper-IK, Animationssynchronisation, Übergänge, Unterbrechungen, Kamera, Audio, Dialogfenster, Streaming, Netzwerk, Persistenz, Debugging und Editor-Werkzeuge.

Architekturentscheidungen (📋):

- **Teilnehmer sind ein Array.** Zwei, drei, vier oder mehr Beteiligte sind technisch derselbe Sitzungstyp. Der Entwickler gewichtet Paare zuerst, will Mehrpersonen-Fälle aber nicht vernachlässigen.
- **Keine fest verdrahteten Animationen.** Der Code kennt Kategorien und Eigenschaften, nicht einzelne Animationen. Eine Anfrage durchläuft eine feste Kette: Berechtigung der Teilnehmer → Zustimmung → Präferenzen und Grenzen → Beziehungskontext → Weltkontext → Umgebungs- und Oberflächenanfrage → Animationsanfrage → harter Kompatibilitätsfilter → Bewertung der Kandidaten → Animations-Satz → Zustandsautomat → laufende Sitzung.
- **Stabile Kennungen** und Schema-Version von Anfang an.
- **Kein „Gott-System“**; jedes Modul bleibt austauschbar.
- Die Sitzung wird nachträglich **automatisch klassifiziert**: Aus den tatsächlichen Handlungen entstehen später Erinnerung und Statistik, nicht aus einer vorab gewählten Szene.

---

## 2. Taxonomie

Die Taxonomie ist ein erweiterbares Registry-System mit **orthogonalen Dimensionen**, die frei kombinierbar sind. Kategorie, Intensität, Ort, Beteiligte, Rollen, Körperbereiche, Items, Kleidung, Risiko, Präferenzen und Kontext sind getrennte Achsen. Es sollen **nicht** tausende isolierte „Szenen“ entstehen. 📋

### 2.1 Ebenen

```text
Wurzelkategorie → Unterart → Inhaltsprofil → Varianten-Tags
```

Der Kern kennt Varianten nicht inhaltlich, sondern arbeitet mit Tags, Tag-Abfragen, Zustimmungsumfang, Präferenzregeln, Kompatibilität und Animationsmetadaten. Neue Kategorien lassen sich **ohne Änderung des C++-Kerns** hinzufügen (Entwickler: ja, 10.09.; Begründung: Modder sollen Kategorien ergänzen können, an die niemand gedacht hat).

### 2.2 Familien

| Familie | Beispiele (technische Klassen) |
|---|---|
| **Soziale Interaktion** | Begrüßung, Gespräch |
| **Zuneigung** | Händehalten, Umarmen, Kuscheln, Küssen (kurz, anhaltend, Stirn/Wange, romantisch), gemeinsames Ausruhen, Wiedersehen, Abschied |
| **Romantik** | Flirten, Geständnis, Date, langsamer Tanz, Versöhnung, Jahrestag |
| **Sinnlich** | Massage, Tanz für Erwachsene, intime Berührung, Aus- und Ankleideübergänge, Übergang zu Bett und Ruhe |
| **Erwachsenen-Inhalt** | nicht-penetrativ, manuell, oral, vaginal, anal, solo, gegenseitig, mehrere Beteiligte, gemischt (Wechsel zwischen Kategorien in einer Sitzung), Übergang, Ruhe, sowie eine Sammelkategorie für künftige und modifizierte Kategorien |

Die Sammelkategorie ist ausdrücklich nur für künftige Kategorien, Experimente und Mods gedacht und kein Dauerbehälter. Feinere Unterarten und konkrete Varianten stehen in der Master-Registry der Planung (Interviewrunde 16) und werden hier nicht aufgelistet; sie sind **nicht eingefroren** ([Abschnitt 7](#7-stand-der-registry-und-der-audits)).

### 2.3 Inhaltsprofile

| Profil | Bedeutung |
|---|---|
| Standard | normale Varianten |
| Fortgeschritten | technisch oder körperlich anspruchsvoller: stärkere Kontaktabhängigkeit, komplizierteres Alignment, engere Kompatibilitätsbereiche |
| Hochintensität („Hardcore“) | deutlich intensiver als Standard und viele fortgeschrittene Varianten; **kein bloßes Synonym für ‚Intensität hoch‘**, sondern eine eigene Profilklasse mit zusätzlichen Anforderungen an Kontakt, Ausrichtung, Rollenlayout, Bewegungsprofil, Timing, Kompatibilität, Zustimmung, Präferenzen und Darstellung |
| Höchste unterstützte Stufe | das vom Content-Paket ausdrücklich höchste unterstützte Profil; nicht automatisch aus Hochintensität abgeleitet |

Im Interview wurde „Hardcore“ als **Preset über mehrere Einstellungen** festgelegt (Entwickler: es gehe um das, was Figuren sich gefallen lassen, und um die Geschwindigkeit; danach einzeln änderbar). Für Hochintensitäts-Varianten sind unabhängige technische Variantenfamilien vorgesehen (zum Beispiel erhöhte Kontakttiefe, hohe Kontaktkomplexität, intensives Bewegungsprofil, anhaltend hohe Intensität, eingeschränkte Ausrichtung, spezialisierte Rollenprofile, Mehrfachkontakt). Sie sind **reine Metadaten** für Auswahl, Prüfung und Zustimmung.

### 2.4 Weitere Achsen

| Achse | Stufen oder Inhalt |
|---|---|
| **Intensität** | sanft, niedrig, mittel, hoch, sehr hoch, höchste unterstützte Stufe. Nie ein einzelner Wert, sondern mehrere Achsen (Geschwindigkeit, Kraft, Druck, Bewegungsumfang, Rhythmus, Dauer, psychologische Intensität). Sie kann lokal pro Kontakt gelten und sich laufend ändern (Entwickler: sekundenschnell anpassbar). |
| **Kontakttiefe** | Oberfläche, Standard, erweitert, tief, maximal unterstützt (nur technische Eigenschaft eines Animations-Satzes) |
| **Kontaktkomplexität** | einfach, mittel, fortgeschritten, Mehrpunkt, hochpräzise, spezialisiert (relevant für IK, Kompatibilität, Authoring, Performance) |
| **Bewegungsprofil** | statisch bis hochenergetisch, abwechselnd, variabel, animationsdefiniert |
| **Timing und Tempo** | kurz bis fortlaufend, segmentiert, variabel; sehr langsam bis schnell, steigend oder fallend |
| **Körperbereiche** | hierarchisch (zum Beispiel Oberkörper → Brust → links/rechts), mit getrennten Links-/Rechts-Präferenzen |
| **Items** | echte physische Objekte mit Geometrie, Material, Kontakten und Zustand; mehrere gleichzeitig möglich; sie „spawnen“ nicht aus dem Nichts |
| **Kleidung** | pro Stück ein Zustand (angezogen, verschoben, geöffnet, ausgezogen, am Boden, beschädigt, verschmutzt) |
| **Teilnehmerzahl und Rollen** | ein bis mehrere; Rollen (führend, folgend, abwechselnd, unterstützend, Beobachter) wechseln dynamisch; Rollenpräferenzen sind je Rolle verschieden |
| **Umgebung** | Schlafzimmer, Motelzimmer, Hotel, Wohnung, Haus, Hütte, Unterschlupf, Lager, Fahrzeug, Außenbereich, Arbeitsplatz u. a. |
| **Risiko** | getrennt in Verletzungs-, Entdeckungs-, soziales, rechtliches, gesundheitliches und Beziehungsrisiko |

### 2.5 Ergebnisfamilien (Outcomes)

📋 (Runde 16, Audit-Fragen A9/A10, 25.09.) Körperliche und soziale Folgen sind **getrennte Ergebnisfamilien**:

- **Körperlich:** Lust, Höhepunkt, Schmerz, Ermüdung, Würge-/Brechreize und verwandte Zustände, Atmung, Flüssigkeiten, Haut-/Gewebereaktionen, Verletzung, Erholung.
- **Sozial:** Nähe, Zufriedenheit, Enttäuschung, Vertrauen, Bindung, Scham, Eifersucht, Beziehungsänderung.

Der Entwickler bestätigte die soziale Familie; bei der körperlichen Liste wollte er „noch genauer“ (A9: *nein, noch genauer*). Zustände wie Würgereiz, Übelkeit, Erbrechen, Rückfluss, Husten, Atemnot und Atemunterbrechung sind **eigene Zustände**, die gleichzeitig auftreten dürfen und aus dem gemeinsamen Körper- und Gesundheitssystem kommen ([Modul 06](06-Koerper-Physik-Gesundheit.md)).

### 2.6 Intensität wird nie zur Kategorie

📋 „Sanft“, „rau“, „extrem“ werden **niemals Kategorien**, sondern ergeben sich aus Kraft, Tempo, Druck, Risiko und Präferenzen (A14, bestätigt). Das verhindert doppelte Kategorisierung.

---

## 3. Positionen: Freeform statt Menü

Der Entwickler hat sich **gegen einen Katalog fester Positionen** entschieden (📋):

- Die Kategorie bestimmt nie automatisch die Position. Eine Position entsteht aus Körperhaltungen, relativer Ausrichtung, Unterlage, Höhe, Stützen, Kontaktprofil, Kontakttiefe und Rollenlayout.
- Ziel ist ein **nahezu kontinuierlicher Raum möglicher Körperstellungen**. Referenzpositionen dürfen existieren, müssen aber weder exakt eingehalten noch überhaupt benutzt werden; Zwischenstellungen und fließende Wechsel sollen möglich sein.
- Technische Kette: Pose-Ziele → Kontaktgraph (die Flächen, die sich berühren) → Ganzkörper-IK → Anpassung an Körperproportionen → Kollision und Weichgewebe → Balance und Gewichtsverteilung → Mikrobewegungen.
- Verwaltet wird **nur**, was tatsächlich als Animations-Satz existiert; es gibt keine Kombinationsexplosion.
- Unterstützt ein Mehrpersonen-Aufbau eine Konstellation nicht gemeinsam, fällt das System auf abwechselnde Rollen, Beobachter oder getrennte Varianten zurück, **statt eine fehlerhafte Animation zu erzwingen**.
- Körperliche Grenzen entstehen aus Biomechanik und Gewebezustand ([Modul 06](06-Koerper-Physik-Gesundheit.md)); sehr bewegliche Figuren dürfen mehr, unvorbereitete weniger. Möbel können bei grober Belastung beschädigt werden (Entwickler, 16.09.).

---

## 4. Zustandsautomat einer Interaktion

📋 Von „nicht verfügbar“ über: Kandidat → Berechtigungsprüfung → Kontextbewertung → Einladung → Zustimmungsprüfung → Umgebungssuche → Reservierung (Umgebung und Teilnehmer) → Vorbereitung der Assets → Annäherung → Ausrichtung → anfängliche Zuneigung → Start → **aktive Interaktion** (mit optionaler Pause und optionalem Übergang) → Ausklang → Nach-Interaktion → optionales Gespräch → Ausruhen/Anziehen/Schlafen/Gehen → Aufräumen → abgeschlossen.

Jeder Zustand hat erlaubte Übergänge, Zeitlimit, Fehlerübergang, Autorität, benötigte Assets und Aufräumregeln.

**Fehlerzustände** sind eigene Zustände: abgelehnt, abgebrochen, Zustimmung zurückgezogen, unterbrochen, Teilnehmer nicht verfügbar, Umgebung nicht verfügbar, Animation nicht verfügbar, Asset-, Ausrichtungs-, Streaming-, Netzwerk- oder Autoritätsfehler, Kampf, Bedrohung, Verbindungsabbruch.

**Rückfallleiter:** Fehlt eine Animation oder ein Übergang, greift eine abgestufte Alternative bis zu einem sicheren Normalzustand. Es gibt keinen Softlock.

---

## 5. Eigenschaften des Ablaufs

📋 Bestätigt in den Runden 6 bis 8 und 14 bis 16:

- **Keine Schnitte.** Das Spiel simuliert die Welt weiter. Ein Date kann natürlich zur Annäherung führen (Heimfahrt, Wohnung, Gespräch). Der Entwickler: Verabredungen führen oft zuerst woandershin, dann gibt es einen sinnvollen Übergang, erst danach startet die Szene.
- **Keine Pflichtreihenfolge** und kein Fortschrittsbalken. Eine Situation kann beliebig kurz oder lang sein oder ‚einfach nicht gut laufen‘, ohne dass das ein Fehler wäre.
- **Pausen** (kurz, Ruhe, Gespräch, Übergang, Neupositionierung, umgebungsbedingt, auf Spielerwunsch) erlauben Dialog, Kamerawechsel, Asset-Vorladung, Neupositionierung, neue Anfragen, Zustimmungsprüfung, Stopp und Fortsetzung.
- **Dialog:** Längere Gespräche nur in definierten Phasen (vorher, Pausen, danach). Währenddessen sind kurze, persönlichkeitsabhängige Äußerungen möglich; manche Figuren sind still, ohne dass das ein Fehler wäre. Vollständiger Dialog in der aktiven Phase nur bei Animationen mit ausgewiesenen Dialogfenstern.
- **Danach:** Nähe, Gespräch, Schlafen, Duschen, Essen, Anziehen, gemeinsames Einschlafen, normales Weiterspielen am nächsten Morgen. Duschen ist eine normale Handlung im Tagesablauf, keine Aftercare-Cutscene.
- **Eskalation und Wechsel:** Variantenwechsel, Rollenwechsel und Tempo sind jederzeit möglich. NPCs wählen selbst die nächste Handlung, abhängig von Dominanz und Initiative. Dominanz ist kein binärer Typ und von Initiative getrennt. Rollenspiel ist ein eigenes System ([Abschnitt 6](#6-kontext-statt-kategorie-dienstleistung-rollenspiel)).
- **Unterbrechungen:** durch Spieler- oder Teilnehmerabbruch, Zustimmungsrücknahme, Kampf, Gegner, Zeugen, andere Spieler, Behörden, Quest- oder Welt-Ereignisse, Telefonanrufe, Verletzung, ungültige Umgebung oder technische Fehler; mit Prioritätsstufen. Ein sicherer Ausstieg ist garantiert.
- **Überspringen:** Der Ausgang wird narrativ aufgelöst, danach folgt ein Nachzustand.
- **Zeitmaßstab:** Der Entwickler forderte (15.09.), dass in Szenen die Zeit nicht schnell vergehen darf (30 Minuten Echtzeit dürfen nicht zwölf Spielstunden bedeuten). Als Arbeitsrichtung des Assistenten gilt nahezu 1:1 während Interaktionen; größere Sprünge gehören zu Schlaf, Reise, Warten (🧭). Die Planung von Kalender und Tagesabläufen steht aus.

---

## 6. Kontext statt Kategorie: Dienstleistung, Rollenspiel

- 📋 **Dienstleistung und Verträge** (Begleitung, Bordell, Creator, Performer) sind ein **Geschäfts-/Vertragskontext** (A8, Empfehlung „definitiv ja“, bestätigt), keine eigenen Handlungskategorien. Sie nutzen die vorhandenen Aktionen; Details: [Welt und Branche](08-Welt-Orte-Branche-Wirtschaft.md).
- 📋 **Rollenspiel** zwischen Erwachsenen ist ein eigener Klassifizierungs-Kontext. Das System muss klar speichern, dass es **vereinbartes Rollenspiel zwischen Erwachsenen** ist und nicht dasselbe wie reale Nichteinwilligung. Der Entwickler wünscht ausdrücklich auch gespielte Szenarien extremer Art, etwa als Produktionen für Plattformen in der Spielwelt. Die Umsetzung als Katalog ist **nicht spezifiziert**; die Abgrenzung zu nicht-einvernehmlichen Handlungen bleibt eine harte Grenze ([Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md)). ❓ Inhaltliche Freigabe offen.
- 📋 **Eigene Kontextachsen** (A11 bis A13, bestätigt): Ort (Privatwohnung, Hotel/Motel, Fahrzeug, Außenbereich, Arbeitsplatz, Erwachsenen-Betrieb, Studio, öffentlich/halböffentlich); Zeitkontext (spontan, geplant, kurz, lang, wiederholt, mit Unterbrechungen) vor allem aus der Zeitlinie abgeleitet, nur wenige Präferenz-Tags; Teilnehmerstruktur (Solo, Paar, Gruppe) plus normaler Interaktionsgraph.

---

## 7. Stand der Registry und der Audits

- ❓ Die **Kategorien-Registry ist nicht eingefroren.** Vorgesehen sind (a) ein eigener **Missing-Family-/Taxonomie-Audit** nach dem Ende der Runde 16, (b) ein **externer Gegenprüfungslauf** durch ein anderes Modell als Lückenfinder und (c) eventuell daraus entstehende Folgefragen. Erst danach wird die Registry eingefroren.
- Der Entwickler wollte die externe Prüfung später ausführen („ich dachte, wir machen das erst mit Qwen“); zum Stand 27.09. war sie nicht erfolgt. Ein erster eigener Audit mit 15 Fragen (A1 bis A15) lief am 25.09.; Zu A15 (‚fehlt eine komplette Familie?‘) bat der Entwickler um einen Vorschlag und bestätigte die vorgeschlagenen Punkte danach pauschal („die Sachen passen gut“).
- Der Entwickler bat am 25. September, Runde 16 auf „die wichtigsten Fragen“ zu reduzieren, und hält sehr feine Details der Taxonomie für nachrangig (23.09.: ‚einfach so wie in echt‘).
- Konkrete Inhaltskataloge, Animationsumfang und Kamera-/Stimmenproduktion: ❓ offen ([Offene Fragen](11-Offene-Fragen-und-Konflikte.md)).
