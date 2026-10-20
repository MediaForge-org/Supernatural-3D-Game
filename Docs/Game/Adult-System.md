# Das Adult-System: Planungsstand

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Dokument beschreibt den **Planungsstand** eines optionalen Erwachsenen-Bereichs. Im Repository existiert dafür weder Code noch Content. Es ist eine sachliche, technisch orientierte Rekonstruktion der dokumentierten Planung und keine Zusage, wann oder in welchem Umfang etwas umgesetzt wird. Alle Aussagen tragen eine Statuskennzeichnung (siehe [Abschnitt 1.2](#12-statuskennzeichnung)).

Das Dokument ist eigenständig lesbar. Es ergänzt die README ([Kapitel 21](../../README.md#21-das-adult-system-optional-und-modular)) und gehört zu den Spiel-Dokumenten in [`Docs/Game/`](.), neben [Spielvision](Spielvision.md), [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md), [Gameplay-Systeme](Gameplay-Systeme.md) und [Entscheidungsstatus](Entscheidungsstatus.md).

**Inhalt**

1. [Einordnung und Quellenlage](#1-einordnung-und-quellenlage)
2. [Harte Grundregeln](#2-harte-grundregeln)
3. [Bereitstellung und Modularität](#3-bereitstellung-und-modularität)
4. [Was das Basisspiel enthält](#4-was-das-basisspiel-enthält)
5. [Das allgemeine Interaktions-Framework](#5-das-allgemeine-interaktions-framework)
6. [Content-Taxonomie](#6-content-taxonomie)
7. [Zustimmung, Grenzen und Entscheidungsfähigkeit](#7-zustimmung-grenzen-und-entscheidungsfähigkeit)
8. [NPCs: Präferenzen, Anziehung und Beziehungen](#8-npcs-präferenzen-anziehung-und-beziehungen)
9. [Ablauf einer Interaktion](#9-ablauf-einer-interaktion)
10. [Körper, Physik und Reaktionen](#10-körper-physik-und-reaktionen)
11. [Darstellung, Kamera, Audio und Einstellungen](#11-darstellung-kamera-audio-und-einstellungen)
12. [Steuerung, Oberfläche und VR](#12-steuerung-oberfläche-und-vr)
13. [Orte, Öffentlichkeit und Weltreaktion](#13-orte-öffentlichkeit-und-weltreaktion)
14. [Die Erwachsenen-Branche als Weltschicht](#14-die-erwachsenen-branche-als-weltschicht)
15. [Charakteranpassung: Tattoos, Piercings, Körpermodifikation](#15-charakteranpassung-tattoos-piercings-körpermodifikation)
16. [Gesundheit und Folgen](#16-gesundheit-und-folgen)
17. [Dunkle Kriminalität als getrenntes Modul](#17-dunkle-kriminalität-als-getrenntes-modul)
18. [Technische Integration](#18-technische-integration)
19. [Modding und Erweiterbarkeit](#19-modding-und-erweiterbarkeit)
20. [Netzwerk und Mehrspieler](#20-netzwerk-und-mehrspieler)
21. [Produktionsplan und Qualitätstore](#21-produktionsplan-und-qualitätstore)
22. [Offene Punkte, Widersprüche und überholte Planung](#22-offene-punkte-widersprüche-und-überholte-planung)
23. [Ausdrücklich nicht Teil der Planung](#23-ausdrücklich-nicht-teil-der-planung)

---

## 1. Einordnung und Quellenlage

### 1.1 Bedeutung im Gesamtprojekt

Der Erwachsenen-Bereich ist **kein Randthema der Planung**. Er wurde zwischen Ende August und Ende September 2026 in einem eigenen, sehr ausführlichen Planungsprozess mit dem Entwickler bearbeitet: zuerst als Rahmenwerk (28. August), danach in einer Serie von **20 nummerierten Planungsrunden** (10. bis 27. September), in denen der Entwickler zu mehreren Hundert Einzelfragen Entscheidungen traf. Der Bereich ist damit einer der am tiefsten durchgeplanten Teile des gesamten Projekts.

Zugleich gilt: **Er ist ein optionales Subsystem eines größeren Spiels.** Er darf die Hauptarchitektur nicht dominieren, das Basisspiel darf nicht von ihm abhängen, und er nutzt dieselben Kernsysteme wie das restliche Spiel (Körper, Physik, Gesundheit, Gedächtnis, Beziehungen, Weltzustand). Ein paralleles Zweitsystem ist ausdrücklich nicht vorgesehen.

### 1.2 Statuskennzeichnung

| Kennzeichnung | Bedeutung |
|---|---|
| ✅ | Implementiert und getestet. Für dieses Dokument trifft das **auf nichts** zu. |
| 📋 | **Bestätigte Planung:** Der Entwickler hat die Richtung ausdrücklich bestätigt. Nicht implementiert. |
| 🧭 | **Vorläufig:** Arbeitsstand, Vorschlag oder Entwurf; nicht verbindlich festgelegt. |
| 🔭 | Langfristige Vision ohne Spezifikation. |
| ❓ | Offen: bewusst nicht entschieden. |
| 🕰️ | Überholt: frühere Planung, die später ersetzt wurde. |

### 1.3 Wie belastbar ist die Planung?

Die Planung hat zwei Schichten:

- **Konsolidierte Entscheidungen (Stand 31. August 2026).** Sie stehen in den Entscheidungsdokumenten des Planungsmasters und betreffen vor allem die Rahmenbedingungen: Trennung vom Basisspiel, Modularität, Zustimmungsprinzipien, Darstellungsgrenzen des Basisspiels, Nachtleben, Beziehungs-Sandbox.
- **Detailplanung aus dem Entwicklerinterview (Ende August bis 27. September 2026).** Sie ist der weitaus größere Teil. Die Antworten des Entwicklers liegen in den Gesprächsprotokollen vor und wurden dort von Runde zu Runde als „gelockt“ festgehalten. **Diese Detailplanung wurde bisher nicht in die kanonischen Statusdokumente übernommen** und ist dort als ungeprüfte Quelle markiert. Dieses Dokument behandelt sie deshalb als **📋 bestätigte Planung des Entwicklers, die noch nicht in den Kanon konsolidiert wurde**.

Zwei Prüfungen stehen ausdrücklich noch aus: ein **Prüflauf auf fehlende Inhaltsfamilien („Missing-Family-/Taxonomy-Audit“)** und ein **externer Gegenprüfungslauf** der Taxonomie. Bis dahin ist die Kategorien-Registry **nicht eingefroren**.

Der Planungsprozess selbst hat mehrere Vorschläge des Entwicklers korrigiert, wo sie mit den hier genannten Grundregeln (Abschnitt 2) nicht vereinbar waren. Maßgeblich ist jeweils der **endgültige** Stand nach dieser Korrektur. Wo ein früherer Zwischenstand abweicht, steht er in [Abschnitt 22](#22-offene-punkte-widersprüche-und-überholte-planung).

---

## 2. Harte Grundregeln

Diese Regeln gelten für jede Erwachsenen-Interaktion und sind als **Kernregeln** geplant, die weder Mods noch Datenpakete deaktivieren dürfen. 📋

1. **Nur erwachsene Figuren.** Alle Beteiligten sind eindeutig erwachsen. Minderjährige existieren in diesem System nicht.
2. **Zustimmung ist zwingend.** Zustimmung ist Pflicht und kann jederzeit zurückgenommen werden.
3. **Entscheidungsfähigkeit.** Alle Beteiligten müssen bei Bewusstsein, handlungsfähig und entscheidungsfähig sein.
4. **Beziehung ersetzt keine Zustimmung.** Beziehungswerte, Anziehung, Story-Ereignisse oder KI-Ziele ersetzen die Zustimmung niemals. Ein Story- oder Quest-Zwang ist ausgeschlossen.
5. **Kein stellvertretendes Einverständnis.** Weder ein Gruppen- noch ein Server-Administrator kann für andere Spieler zustimmen.
6. **Altersprüfung vor allem anderen.** Die Prüfung der Erwachsenen-Berechtigung erfolgt **vor** Präferenzen, Anziehung, Beziehung und allen weiteren Regeln. Sie arbeitet nach dem Prinzip „im Zweifel gesperrt“ (*fail closed*): Eine Figur ist nur zugelassen, wenn sie eindeutig als erwachsen definiert ist.

### 2.1 Altersgrenze und Prüfung

- Maßgeblich ist das **Geburtsdatum** der Figur und die Spielzeit, nicht ein fester „Alter“-Wert. Eine Figur ist ab **exakt 18 Jahren und 0 Tagen** zugelassen.
- **Unter 18** ist vollständig ausgeschlossen. Solche Figuren erscheinen weder in Auswahlmenüs noch in Flirt- oder Interaktionskatalogen.
- Das Alter wird **nicht aus Aussehen, Körperbau oder Stimme abgeleitet**, sondern ausschließlich aus eindeutigen Figurendaten.
- Ist das Alter einer Figur **unbekannt**, bleibt der Erwachsenen-Bereich gesperrt. Es gibt eine **Altersbestätigung in der Spielwelt**: Die Figur kann das Alter nennen oder ein Dokument zeigen (z. B. einen Ausweis). Danach wird nur noch der Status „verifiziert“ gespeichert, nicht das Dokument. Unter 18 oder keine Bestätigung bedeutet: gesperrt.
- Alter und Vorlieben sind **vollständig getrennt**. Das Alter entscheidet nur, ob eine Figur überhaupt im System vorkommen darf, nicht, welche Inhalte oder Intensitäten sie mag.

📋 Geplant sind außerdem Figuren-Kategorien in den Stammdaten, die nicht frei durch Mods verändert werden dürfen (die Berechtigung ist **kein** frei modifizierbares Tag).

---

## 3. Bereitstellung und Modularität

### 3.1 Basisspiel und optionales Paket

| Ebene | Inhalt | Status |
|---|---|---|
| **Basisspiel** | Beziehungs- und Romantik-Sandbox, Nachtleben als Weltschicht, **nicht explizite** Darstellung von Intimität (angedeutet durch Kamera, Blocking, Audio, Reaktionen), konfigurierbare Nacktheit | 📋 |
| **Optionales Erwachsenen-Paket** | Erweiterte Interaktionen, Katalog, Animationen, Stimmen, zusätzliche Kategorien | 📋 |

- Das Basisspiel darf technisch **nicht** von Erwachsenen-Inhalten abhängen und enthält **keine** Spezialabfrage der Art „ist das Erwachsenen-Paket installiert?“.
- Das optionale Paket nutzt **dieselben öffentlichen Schnittstellen und dasselbe Paketmodell wie jede Drittanbieter-Mod**. Es gibt keinen Sonderzugriff auf den Kern.
- Der Erwachsenen-Anteil ist **nicht Teil des Basis-Builds** und **nicht Teil der Konsolen-Builds** (Cooks).
- Im **offiziellen Online-Modus** (ohne Mods und ohne Cheats) ist das optionale Paket **nicht nutzbar**.
- In Einzelspieler und Koop-Kampagne (Mods erlaubt) ist das Paket grundsätzlich möglich, soweit rechtlich und technisch zulässig.

### 3.2 Paketstruktur

Beschlossene Richtung (📋, Umfang je nach Gesamtgröße):

```text
Erwachsenen-Framework      Technische Grundlage (Interaktions-System, Präferenzen, Beziehungen,
                           Intensität, Beobachten/Beitreten, Synchronisation, Konfiguration,
                           Speicherstand, Mod-Schnittstelle). Wenig eigener Content.
Content-Pakete             Standard-Inhalte, Hochintensitäts-Inhalte, Animations-, Stimmen-,
                           Kamera-/Präsentations- und Figuren-/Körper-Pakete.
Komplett-Paket             Alles zusammen, solange die Gesamtgröße sinnvoll bleibt.
```

- Pakete haben **eigene Versionen, Abhängigkeiten und Kompatibilitätsangaben** (benötigt, optional, inkompatibel, empfohlen). Der Mod-Manager löst das automatisch auf.
- Das Framework soll **stabil** bleiben, Content-Pakete dürfen häufiger aktualisiert werden. Ein Hochintensitäts-Paket fügt nur Content, Presets, Animationen und Optionen hinzu und verändert **nicht** das Framework.
- Einzelne Kategorien eines installierten Pakets lassen sich **deaktivieren**, ohne das Paket zu entfernen.
- Voice-Pakete sind unabhängig von Animationspaketen (Sprachen, Qualitätsstufen, alternative Reaktions-Sets).
- Orte wie Clubs, Hotels und Wohnungen gehören **zur normalen Spielwelt** und nicht zu einem Paket; das Erwachsenen-Paket nutzt ihre Räume, Möbel, Privatsphäre, Besitzer, Öffnungszeiten und Sicherheitsregeln.
- Preset-Bündel (z. B. Minimal, Standard, Voll, Hochintensität, Eigene) sind erwünscht, ihre Ausgestaltung ❓.

### 3.3 Veröffentlichung

- 📋 **Ziel ist eine öffentliche Veröffentlichung** des optionalen Pakets. Eine rein private Nutzung ist ausdrücklich nur der **Rückfallplan**, falls Recht oder Plattformregeln eine Veröffentlichung unpraktikabel machen. Das Basisspiel darf davon in keinem Fall abhängen.
- ❓ Vertriebsweg, Plattform- und Altersfreigaberegeln sind **nicht entschieden**. Die Planung geht davon aus, dass gängige Spiele-Plattformen ein solches Paket nicht im Hauptspiel akzeptieren würden und dass es deshalb als getrenntes Paket zu behandeln ist. Eine Prüfung der Regeln erfolgt erst zum Veröffentlichungszeitpunkt.
- 📋 Es sind getrennte Builds vorgesehen: Basisversion, interne Entwicklungsversion, Entwicklungsversion mit Erwachsenen-Paket und gegebenenfalls später eine Distributionsversion mit Paket. So landen interne Testfunktionen nicht im falschen Build.

---

## 4. Was das Basisspiel enthält

Dieser Abschnitt beschreibt den Teil, der **nicht** vom optionalen Paket abhängt. 📋

- **Beziehungs-Sandbox.** Dynamische Freundschaft, Vertrauen, Respekt, Angst, Sympathie und verwandte interne Beziehungszustände. Romantische und sexuelle Beziehungen sind mit erwachsenen NPCs breit möglich, wenn gegenseitiges Interesse und die Logik der Figur es zulassen; sie sind nicht auf eine kleine festgelegte Romanze-Auswahl beschränkt. Mehrere gleichzeitige Beziehungen sind möglich; Eifersucht, Konflikte, Heimlichkeit, offene Arrangements, Trennungen und Folgen können entstehen. Zusammenleben, Heirat und gemeinsamer Besitz gehören dazu; Familien über Generationen sind niedrigere Priorität.
- **Tiefe nach Rolle.** Wichtige Figuren erhalten ausgearbeitete Tiefe, generische NPCs nutzen systemische Beziehungsinhalte.
- **Keine sichtbaren Beziehungsbalken.** Zustände zeigen sich durch Ton, Verhalten, Zusammenarbeit und Zugang.
- **Nachtleben als Weltschicht.** Clubs, Bars, Stripclubs und weitere Unterhaltungsorte gehören zur Spielwelt. Details siehe [Abschnitt 14](#14-die-erwachsenen-branche-als-weltschicht).
- **Darstellung im Basisspiel.** Einheitlich auf PC, PlayStation und Xbox, ungefähr in dem Rahmen heutiger großer Rollenspiele: Nacktheit konfigurierbar (voll, reduziert, aus), Intimität klar angedeutet durch Blocking, Kamera, Animation, Audio und Reaktionen, **ohne** sichtbare explizite Anatomie. Assets, Kameras und IK werden so gebaut, dass Kamera- oder Sichtfeldfehler nichts offenlegen, was das Profil nicht vorsieht.
- **Eindeutiges Stopp-Signal.** Wird das vereinbarte Stopp-Signal gegeben, endet die Interaktion sofort über einen sicheren Ausstieg (Abschnitt 7.4).
- **Keine starren Schleifen.** Intime Interaktionen sind keine festen Animationsschleifen, sondern reagieren auf Haltung, Abstand, Tempo, Pausen und Zustand (Abschnitt 9).

---

## 5. Das allgemeine Interaktions-Framework

📋 Romantik und Intimität werden **nicht** als isoliertes Spezialsystem gebaut. Vorgesehen ist ein allgemeines **Multi-Charakter-Interaktions-Framework** mit einem gemeinsamen Kern und fachlichen Modulen:

```text
Interaktionskern
├── Sozial            Begrüßung, Verabschiedung, Handschlag, Gespräch
├── Zuneigung         Umarmung, Trost, Kuscheln, Händehalten
├── Hilfe             Helfen, Tragen, medizinische Versorgung
├── Ruhe              Sitzen, Ausruhen, Schlafen
├── Romantik
└── Intimität         (nur im optionalen Paket erweitert)
```

Gemeinsame Infrastruktur dieser Module: Teilnehmer, Sitzungen, stabile IDs, Anfragen, Berechtigungen, Reservierungen (Möbel, Anker), Annäherung, Ausrichtung, Motion Warping, Ganzkörper-IK, Animationssynchronisation, Übergänge, Unterbrechungen, Kamera, Audio, Dialogfenster, Streaming, Netzwerk, Persistenz, Debugging und Editor-Werkzeuge.

Wichtige Architekturentscheidungen (📋):

- **Teilnehmer sind ein Array**, nicht „Figur A und Figur B“. Zwei, drei, vier oder mehr Beteiligte sind technisch derselbe Sitzungstyp.
- **Keine fest verdrahteten Animationen.** Der Code kennt Kategorien und Eigenschaften, nicht einzelne Animationen. Eine Anfrage wird in einer festen Kette bewertet: Berechtigung der Teilnehmer, Zustimmung, Präferenzen und Grenzen, Beziehungskontext, Weltkontext, Umgebungs- und Oberflächenanfrage, Animationsanfrage, harter Kompatibilitätsfilter, Bewertung der Kandidaten, Animations-Satz, Zustandsautomat, laufende Sitzung.
- **Stabile Kennungen** und Schema-Version von Anfang an (Teilnehmer, Sitzungen, Anfragen, Animations-Sätze, Positions-Voreinstellungen, Umgebungsanker, Oberflächen).
- **Kein „Gott-System“.** Jedes Modul bleibt austauschbar.

---

## 6. Content-Taxonomie

Die Taxonomie ist ein erweiterbares Registry-System mit **orthogonalen Dimensionen**, die frei kombinierbar sind. Kategorie, Intensität, Ort, Beteiligte, Rollen, Körperbereiche, Items, Kleidung, Risiko, Präferenzen und Kontext sind getrennte Achsen. Es sollen **nicht** tausende isolierte „Szenen“ entstehen. 📋

### 6.1 Ebenen

Mindestens vier Ebenen werden unterschieden:

```text
Wurzelkategorie → Unterart → Inhaltsprofil → Varianten-Tags
```

Beispiel in abstrakter Schreibweise: Wurzelkategorie „oral“, Unterart „fortgeschritten“, Profil „Hochintensität“, Varianten-Tags wie „positionsspezifisch“ oder „verlängert“. Der Kern kennt die einzelnen Varianten nicht inhaltlich, sondern arbeitet mit Tags, Tag-Abfragen, Zustimmungsumfang, Präferenzregeln, Kompatibilität und Animationsmetadaten. Neue Kategorien lassen sich **ohne Änderung des C++-Kerns** hinzufügen.

### 6.2 Wurzelkategorien

Die Inhalte sind in Familien gegliedert (jeweils mit Unterarten und Varianten):

| Familie | Beispiele (technische Klassen) |
|---|---|
| **Soziale Interaktion** | Begrüßung, Gespräch |
| **Zuneigung** | Händehalten, Umarmen, Kuscheln, Küssen (kurz, anhaltend, auf Stirn oder Wange, romantisch), gemeinsames Ausruhen, Wiedersehen, Abschied |
| **Romantik** | Flirten, Geständnis, Date, langsamer Tanz, Versöhnung, Jahrestag |
| **Sinnlich** | Massage, Tanz für Erwachsene, intime Berührung, Aus- und Ankleideübergänge, Übergang zu Bett und Ruhe |
| **Erwachsenen-Inhalt** | nicht-penetrativ, manuell, oral, vaginal, anal, solo, gegenseitig, mehrere Beteiligte, gemischt (Wechsel zwischen Kategorien in einer Sitzung), Übergang, Ruhe, sowie eine Sammelkategorie für künftige und modifizierte Kategorien |

Die Sammelkategorie ist ausdrücklich nur für künftige Kategorien, experimentelle Inhalte und Mods gedacht und kein Dauerbehälter.

### 6.3 Inhaltsprofile

Zusätzlich zur Wurzelkategorie gibt es eine eigene Profilebene:

| Profil | Bedeutung |
|---|---|
| Standard | normale Varianten |
| Fortgeschritten | technisch oder körperlich anspruchsvoller: stärkere Kontaktabhängigkeit, komplizierteres Alignment, engere Kompatibilitätsbereiche |
| Hochintensität („Hardcore“) | deutlich intensiver als Standard und viele fortgeschrittene Varianten; **kein bloßes Synonym für „Intensität hoch“**, sondern eine eigene Profilklasse mit zusätzlichen Anforderungen an Kontakt, Ausrichtung, Rollenlayout, Bewegungsprofil, Timing, Kompatibilität, Zustimmung, Präferenzen und Darstellung |
| Höchste unterstützte Stufe | das jeweils vom Content-Paket ausdrücklich höchste unterstützte Profil; wird nicht automatisch aus Hochintensität abgeleitet |

Im Entwicklerinterview wurde „Hardcore“ als **Preset über mehrere Einstellungen** festgelegt: Es beeinflusst Intensität, Tempo, Dauer, körperliche Härte, Reaktionsstärke und Grenzen gleichzeitig und ist danach einzeln änderbar. Ein einzelner globaler Intensitätsschalter ist nicht vorgesehen: Das Paket ist an oder aus, wer weniger Extremes möchte, kann ein anderes Paket installieren, und innerhalb des Pakets sind Kategorien und Presets einzeln konfigurierbar.

Für Hochintensitäts-Varianten sind unabhängige technische Variantenfamilien vorgesehen, zum Beispiel erhöhte Kontakttiefe, hohe Kontaktkomplexität, anspruchsvolle manuelle oder orale Kontakte, intensives Bewegungsprofil, anhaltende hohe Intensität, eingeschränkte Ausrichtung, spezialisierte Rollen- oder Positionsprofile, Mehrfachkontakt und Kombinationen. Sie sind **reine Metadaten** für Auswahl, Prüfung und Zustimmung.

### 6.4 Weitere Achsen

| Achse | Stufen oder Inhalt |
|---|---|
| **Intensität** | sanft, niedrig, mittel, hoch, sehr hoch, höchste unterstützte Stufe. Nie ein einzelner Wert, sondern mehrere Achsen (Geschwindigkeit, Kraft, Druck, Bewegungsumfang, Rhythmus, Dauer, psychologische Intensität). Intensität kann lokal pro Kontakt gelten und sich laufend ändern. |
| **Kontakttiefe** | Oberfläche, Standard, erweitert, tief, maximal unterstützt (nur technische Eigenschaft eines Animations-Satzes) |
| **Kontaktkomplexität** | einfach, mittel, fortgeschritten, Mehrpunkt, hochpräzise, spezialisiert (relevant für IK, Kompatibilität, Authoring, Performance) |
| **Bewegungsprofil** | statisch bis hochenergetisch, abwechselnd, variabel, animationsdefiniert |
| **Timing und Tempo** | kurz bis fortlaufend, segmentiert, variabel; Tempo von sehr langsam bis schnell, steigend oder fallend |
| **Körperbereiche** | hierarchisch (z. B. Oberkörper → Brust → links/rechts), mit getrennten Links-/Rechts-Präferenzen |
| **Items** | als echte physische Objekte mit Geometrie, Material, Kontakten und Zustand; mehrere gleichzeitig möglich |
| **Kleidung** | pro Kleidungsstück ein Zustand (angezogen, verschoben, geöffnet, ausgezogen, am Boden, beschädigt, verschmutzt), nicht nur an/aus |
| **Teilnehmerzahl und Rollen** | ein bis mehrere; Rollen (führend, folgend, abwechselnd, unterstützend, Beobachter) wechseln dynamisch |
| **Umgebung** | Schlafzimmer, Motelzimmer, Hotel, Wohnung, Haus, Hütte, Unterschlupf, Lager, Fahrzeug, Außenbereich, Arbeitsplatz und andere |
| **Risiko** | getrennt in Verletzungs-, Entdeckungs-, soziales, rechtliches, gesundheitliches und Beziehungsrisiko |

### 6.5 Positionen: Freeform statt Menü

Der Entwickler hat sich **gegen einen Katalog fester Positionen** entschieden (📋):

- „Kategorie“ bestimmt nie automatisch die Position. Eine Position entsteht aus Körperhaltungen der Beteiligten, relativer Ausrichtung, Unterlage, Höhe, Stützen, Kontaktprofil, Kontakttiefe und Rollenlayout.
- Das Ziel ist ein **nahezu kontinuierlicher Raum möglicher Körperstellungen**. Referenzpositionen dürfen existieren, müssen aber weder exakt eingehalten noch überhaupt benutzt werden. Auch Zwischenstellungen und fließende Wechsel sollen möglich sein.
- Technische Kette: Pose-Ziele, Kontaktgraph, Ganzkörper-IK, Anpassung an Körperproportionen, Kollision und Weichgewebe, Balance und Gewichtsverteilung, Mikrobewegungen.
- Es wird **nur** verwaltet, was tatsächlich als Animations-Satz erstellt wurde; nicht alle theoretischen Kombinationen müssen existieren („keine Kombinationsexplosion“).
- Unterstützt ein Mehrpersonen-Aufbau eine Konstellation nicht gemeinsam, fällt das System auf abwechselnde Rollen, Beobachter oder getrennte Varianten zurück, **statt eine fehlerhafte Animation zu erzwingen**.
- Eine Sitzung kann nachträglich **automatisch klassifiziert** werden: Aus den tatsächlichen Handlungen entstehen später Erinnerung und Statistik.

---

## 7. Zustimmung, Grenzen und Entscheidungsfähigkeit

### 7.1 Modell

Zustimmung ist ausdrücklich **kein boolescher Wert** (📋). Ein Zustimmungs-Datensatz enthält:

- die Anfrage und den Teilnehmer,
- erlaubte Inhalte, Varianten, Profile und Intensitäten,
- den Kontext,
- Zeitpunkt und Gültigkeit.

Zustandsfolge: unbekannt → möglicherweise interessiert → angeboten → wird erwogen → akzeptiert, abgelehnt, zurückgezogen, abgelaufen oder vorübergehend nicht verfügbar.

**Geltungsbereich** (Scope): Zuneigung, Romantik, Sinnlich, Erwachsenen-Kategorie, Unterart, Profil, Variante, Intensität, Positionsprofil, Sitzung. Eine Erlaubnis für eine Wurzelkategorie ist **nicht automatisch** eine Erlaubnis für alle Unterarten oder Profile.

**Neu geprüft** wird nur an echten Grenzen: Wechsel der Wurzelkategorie, von Romantik zu Erwachsenen-Inhalt, auf eine nicht freigegebene Unterart oder Variante, von Standard/Fortgeschritten zu Hochintensität, bei neuen Teilnehmern, deutlicher Intensitätsänderung, relevanter Positionsänderung, längerer Unterbrechung oder wesentlicher Zustandsänderung. **Nicht** bei jeder kleinen Variation; ein Popup alle paar Sekunden wäre ausdrücklich unerwünscht. Eine einmal gegebene Zustimmung bleibt innerhalb einer laufenden Situation bestehen, bis sie widerrufen wird oder sich die Situation wesentlich ändert. Am nächsten Tag oder in einer neuen Begegnung zählt sie nicht automatisch.

**Zustimmungspfad:** Entscheidungsfähigkeit → Zustimmung → Eskalation. Aus fehlender Gegenwehr wird **nie** Zustimmung abgeleitet. Deutlich positive Signale sind Mitmachen, Initiieren, ein klares Ja oder ein eindeutiges Nicken. Rückzug, Abwehr, Weggehen oder Ablehnung stoppen.

### 7.2 Voraussetzungen für eine Sitzung

Alle Beteiligten müssen: als Erwachsene verifiziert, bei Bewusstsein, entscheidungsfähig, zustimmend, verfügbar, nicht im Kampf, nicht gefesselt, nicht kritisch verletzt, nicht story-gesperrt und mit der Umgebung und der Interaktion kompatibel sein.

### 7.3 Entscheidungsfähigkeit und Alkohol

Dies war einer der am intensivsten diskutierten Punkte. **Endgültiger Stand:**

- Leichte bis mittlere Beeinträchtigung (nüchtern, leicht angetrunken, betrunken aber klar ansprechbar und beteiligt): Die Person kann entscheiden, initiieren und zustimmen. Alkohol kann Hemmungen, Tempo, Dialog und Initiative verändern.
- Alkohol und Stimmung können Vorlieben beeinflussen, **ersetzen aber nie die Zustimmung**.
- Ist eine Person bewusstlos, nicht mehr ansprechbar oder offensichtlich nicht mehr entscheidungsfähig, wird die Interaktion beendet beziehungsweise jede weitere sexuelle Handlung blockiert. Es folgt normales Gameplay (ansprechen, Zustand prüfen, hinlegen, Hilfe holen).
- Wahrnehmung ist nicht augenblicklich: Das System darf etwas Zeit brauchen, um den Zustandswechsel zu erkennen (aus fehlender Eigenbewegung, Muskelspannung, Atemmuster, ausbleibenden Antworten), dann aber **sofort** abbrechen. Ein längeres bewusstes Weitersimulieren ist ausgeschlossen.
- Dasselbe `Reaktionsfähigkeit`-System dient auch bei Verletzung, Ohnmacht und Schlaf.

### 7.4 Stopp, Unbehagen und Widerruf

- **Hartes Stopp-Signal:** Wird das vereinbarte Signal gegeben, endet die Interaktion sofort über einen sicheren Ausstieg. Es gibt keine mehrdeutige Neuinterpretation. Ein Stopp überschreibt alles.
- **Weiche Signale** (Unbehagen, Nachfragen, Position ändern, Intensität anpassen) sind etwas anderes: Sie führen zu Anpassungen, nicht zwangsläufig zum Abbruch. Manche Erwachsene bevorzugen einvernehmlich höhere Intensität bis hin zu Schmerz; das ist keine Zustimmungsrücknahme.
- **Widerruf** jederzeit: kritische Unterbrechung → sicherer Übergang → Ausstieg → Aufräumen.
- Ein früherer Widerruf wirkt in der Erinnerung nach (Abschnitt 8).

---

## 8. NPCs: Präferenzen, Anziehung und Beziehungen

### 8.1 Präferenzprofil

Jede erwachsene Figur besitzt ein eigenes, verborgenes Präferenzprofil. 📋

- **Stufen** (intern): neugierig, neutral, bevorzugt, stark bevorzugt, situativ, nicht gemocht, weiche Grenze, harte Grenze. Eine harte Grenze („nie“) ist wirklich hart.
- **Anwendbar auf**: Wurzelkategorie, Unterart, Profil, Variante, Intensität, Position, Beziehungsstufe, Umgebung, Privatsphäre, Zweck und Teilnehmerkonfiguration.
- **Abhängig von** Rolle (dieselbe Handlung kann in einer Rolle gemocht und in einer anderen abgelehnt werden), Partner (mit Person A offen, mit Person B kaum), Beziehungstyp (fremd, bekannt, Freund, feste Beziehung), Ort, Stimmung, Tageszeit und vorherigen Ereignissen.
- **Veränderlichkeit:** Persönlichkeit ändert sich langsam, Erfahrung mittel- bis langfristig, Stimmung kurzfristig. Ein **aktuelles** Nein bleibt in der laufenden Situation ein Nein. Über längere Zeit können sich Grenzen verschieben (harte Grenze → weiche Grenze → situativ → erlaubt), wenn das Figurendesign es vorsieht. Überreden, Flirten oder Verhandeln können eine neutrale oder situative Haltung kippen lassen, aber keine harte Grenze durch bloßes Wiederholen auflösen. Ständiges Nachfragen nach bereits Abgelehntem wirkt sich negativ aus.
- **Erfahrung ist keine Zahl.** Es gibt keinen „Fertigkeitswert“. Erfahrung besteht aus mehreren unabhängigen Bereichen (Erfahrung mit bestimmten Interaktionen, Körperkontrolle, Kenntnis des eigenen Körpers, Lesen des Partners, Selbstsicherheit, Gewohnheiten, Erinnerungen) und ist von Vorlieben getrennt. Neugier ist von Erfahrung getrennt.
- **Seltene Interessen bleiben selten.** Es gibt keine sichtbare vollständige Liste von Vorlieben; der Spieler lernt über Gespräche, Verhalten und gemeinsame Erlebnisse. NPCs können falsche Annahmen über den Partner haben.
- **Die Werte-Ansicht** gibt es nur in Mod-, Debug- und Cheat-Modus.

### 8.2 Generierung bei sehr vielen NPCs

- Wichtige, unterstützte Figuren erhalten **handkuratierte** Profile. Der Rest wird aus **Archetypen plus Zufallsvariation** erzeugt (Seeds, regionale und persönlichkeitsbasierte Variation).
- Die Planungsgröße für die Bevölkerung liegt bei mehreren Hunderttausend NPCs (🧭, Richtwert des Entwicklers). Deshalb ist Handarbeit pro NPC ausgeschlossen.
- Es gibt Simulationsstufen: Nur nahe, relevante Figuren erhalten volle Interaktions- und Physikqualität.

### 8.3 Anziehung und Beziehung

- **Getrennte Werte:** Anziehung, Vertrauen, Zuneigung, Romantik, Komfort und Vertrautheit entwickeln sich unabhängig. Hinzu kommen Bindung, Konflikt, jüngste Ereignisse und Beziehungsstufe. Anziehung kann ohne Romantik bestehen und umgekehrt.
- **Anziehungsfaktoren:** Aussehen, Kleidung, Körper, Stimme, Geruch und Hygiene, Status, Humor, Verhalten, bisherige Erlebnisse; jede Figur hat eigene Anziehungspräferenzen. Bei manchen Figuren reicht anfangs das Aussehen, bei anderen braucht es Gespräch und Vertrauen. Es gibt sofortige Chemie und auch negative Chemie. Persönlichkeit kann stärker wirken als Optik.
- **Orientierung:** Sexuelle und romantische Orientierung sind getrennt und keine harten Schalter; Körpermerkmale können wichtiger sein als ein Label. Libido und Spontanität sind individuell. Dauer, Pausen, Rhythmus und Abwechslung sind persönliche Präferenzen.
- **Beziehungstypen:** einmalig, Wiederholung, Freundschaft mit Vorteilen, feste Beziehung, offene Beziehung, Polyamorie, Casual-only, Beziehungs-only, Mischtypen. Fremdgehen ist möglich und verschlechtert Vertrauen, Zuneigung, Ruf und Beziehungsstand, wenn es herauskommt. Eifersucht ist ein eigenes persönlichkeitsabhängiges System.
- **Nach der Begegnung:** Am nächsten Tag kann sich eine Person locker, verlegen, romantischer, distanziert oder wiederholungswillig verhalten. Kontakte und Nummern, spätere Nachrichten und Einladungen durch die NPC-KI sind möglich.
- **Erinnerung statt Statistik:** Gespeichert wird, mit wem, ungefähr wann, in welchem Kontext, mit welchen Eindrücken und besonderen Ereignissen, nicht jede Physikposition. Erinnerungen verblassen langfristig, wichtige bleiben. Eine intern geführte Zähler-Statistik darf nicht das Verhalten steuern („kein Sex-Level“).
- **Jede Figur kann theoretisch Teil des Systems sein.** Der Entwickler wünscht, dass man „theoretisch mit jedem“ erwachsenen, mod-kompatiblen NPC eine Beziehung entwickeln kann. Die Planung hält fest: Das heißt **nicht**, dass jeder NPC garantiert verfügbar ist; Orientierung, Präferenzen und Grenzen bleiben bedeutsam. Ein Nein ist nicht „nur eine Phase“.
- **Initiative:** NPCs können selbst ansprechen, flirten, Orte vorschlagen und Kontakt aufnehmen; Stile reichen von subtil bis offensiv, schüchtern bis dominant. Mehrere Interessenten gleichzeitig führen zu Konkurrenz oder Eifersucht. Es gibt NPCs, die fast nur unverbindlich wollen, andere nur Beziehungen, und Mischformen.
- **Spontane Begegnungen** sind ein Kernziel, ohne Quest-Markierung. Ort, Tageszeit, Menge, Alkohol, Stimmung, Beziehung und Anziehung beeinflussen die Wahrscheinlichkeit gemeinsam.

---

## 9. Ablauf einer Interaktion

### 9.1 Zustandsautomat

Von „nicht verfügbar“ über Kandidat, Berechtigungsprüfung, Kontextbewertung, Einladung, Zustimmungsprüfung, Umgebungssuche, Reservierung (Umgebung und Teilnehmer), Vorbereitung der Assets, Annäherung, Ausrichtung, anfängliche Zuneigung, Start, **aktive Interaktion** (mit optionaler Pause und optionalem Übergang), Ausklang, Nach-Interaktion, optionales Gespräch, Ausruhen/Anziehen/Schlafen/Gehen, Aufräumen bis „abgeschlossen“. Jeder Zustand hat erlaubte Übergänge, Zeitlimit, Fehlerübergang, Autorität, benötigte Assets und Aufräumregeln.

Fehlerzustände sind eigene Zustände (abgelehnt, abgebrochen, Zustimmung zurückgezogen, unterbrochen, Teilnehmer nicht verfügbar, Umgebung nicht verfügbar, Animation nicht verfügbar, Asset-, Ausrichtungs-, Streaming-, Netzwerk- oder Autoritätsfehler, Kampf, Bedrohung, Verbindungsabbruch).

### 9.2 Eigenschaften des Ablaufs (📋)

- **Keine Schnitte:** Das Spiel simuliert die Welt weiter. Ein Date kann natürlich zur Annäherung führen (Heimfahrt, Wohnung, Gespräch, Annäherung); manche beginnen sofort, andere sehr langsam; ein Gespräch ist jederzeit wieder möglich; eine Situation kann bei Küssen oder Berührung enden.
- **Keine Pflichtreihenfolge** und kein Fortschrittsbalken. Eine Situation kann beliebig kurz oder lang sein oder „einfach nicht gut laufen“, ohne dass das ein Fehler wäre.
- **Pausen** (kurz, Ruhe, Gespräch, Übergang, Neupositionierung, umgebungsbedingt, auf Spielerwunsch) erlauben Dialog, Kamerawechsel, Asset-Vorladung, Neupositionierung, neue Anfragen, Zustimmungsprüfung, Stopp und Fortsetzung.
- **Dialog:** Längere Gespräche nur in definierten Phasen (vorher, Pausen, danach). Währenddessen sind kurze, persönlichkeitsabhängige Äußerungen möglich; manche Figuren sind komplett still, ohne dass das ein Fehler ist. Vollständiger Dialog in der aktiven Phase nur bei Animationen mit ausgewiesenen Dialogfenstern.
- **Danach:** Nähe, Gespräch, Schlafen, Duschen, Essen, Kleidung wieder anziehen, gemeinsames Einschlafen und normales Weiterspielen am nächsten Morgen.
- **Eskalation und Wechsel:** Variantenwechsel, Rollenwechsel und Tempo sind jederzeit möglich; NPCs wählen selbst die nächste Handlung, abhängig von Dominanz und Initiativfreude. Dominanz ist kein binärer Typ und von Initiative getrennt. Rollenspiel ist ein eigenes System.
- **Unterbrechungen:** durch Spieler- oder Teilnehmerabbruch, Zustimmungsrücknahme, Kampf, Gegner, Zeugen, andere Spieler, Behörden, Quest- oder Welt-Ereignisse, Telefonanrufe, Verletzung, ungültige Umgebung oder technische Fehler; mit Prioritätsstufen. Ein sicherer Ausstieg ist garantiert.
- **Kein Softlock:** Fehlt eine Animation oder ein Übergang, greift eine Rückfallleiter; ein sicherer Normalzustand ist immer erreichbar.
- **Überspringen:** Der Ausgang wird narrativ aufgelöst, danach folgt ein Nachzustand.

---

## 10. Körper, Physik und Reaktionen

### 10.1 Ein gemeinsamer Human Core

Der wichtigste Architekturgrundsatz (📋): **Es gibt ein hochwertiges Menschensystem für das ganze Spiel.** Das Erwachsenen-Paket besitzt keinen eigenen Körper, kein zweites Rig und kein zweites Charaktersystem. Es registriert nur Erweiterungen (Profile, Bindungen, Content, höhere Solver- und Kontaktpräzision in relevanten Zonen). Muskelspannung, Fett- und Weichgewebe, Haut, Gewicht, Haar, Kleidung und Schmuck existieren immer und reagieren auf Bewegung und Kontakt. Das gilt auch für Atmung, Schweiß und Körpertemperatur.

- **Weichgewebe-Nachbewegung** ergibt sich pro Körperteil aus dessen individueller Anatomie (Masse, Steifigkeit, Dämpfung, Muskelzustand, Haltung, Bewegungsrichtung und -frequenz, Kontakt), nicht aus einer pauschalen „Wackel“-Animation. Sie soll **realistisch, nicht übertrieben** sein. Dieselbe Simulation gilt im Hauptspiel (z. B. beim Laufen) und in der Intimität, nur in anderem Kontext.
- **Anatomische Grenzen** entstehen aus echter Biomechanik: Gelenkwinkel, Reichweite, Proportionen, Gewebekompression, Flexibilität, Haltung, Verletzungen, Belastbarkeit. Unangenehme Belastung (Überdehnung, Krampf, ungünstiger Winkel, zu hohe Kontaktkraft) löst natürliche Reaktionen aus. Dieses System ist **nicht Adult-spezifisch** und gilt auch für Kampfsport, Klettern und Tragen.
- **Variation:** Wiederholungen sehen nie identisch aus; Mikrobewegungen und leichte Asymmetrien sind gewollt.
- **Gesicht und Stimme:** Mimik aus den bestehenden Emotionssystemen, Blickkontakt wird simuliert; Atem- und Stimmaudio entstehen prozedural aus Belastung und Persönlichkeit, jede Person klingt anders. Es gibt keine fünf Standardsounds.
- **Deterministik:** Gameplay-, Gesundheits- und replay-relevante Zustände sind deterministisch genug für Speichern, Laden und Replay; rein kosmetische Details dürfen nichtdeterministisch sein.

### 10.2 Zustände und Folgen

- **Erregung** ist ein kontinuierlicher physiologischer Zustand, kein sichtbarer Balken. Er bleibt normalerweise verborgen und zeigt sich über Körpersprache, Atmung und Stimme. Er kann auch wieder sinken und gleichzeitig mit Stress oder Nervosität existieren.
- **Ein Höhepunkt ist nicht garantiert**, nicht für jede Person gleich schnell erreichbar und variiert je nach Situation. Danach folgt ein individueller Erholungszustand; die Sitzung endet dadurch nicht automatisch und kann auch ohne enden.
- **Sinneswahrnehmung** (lokale Empfindlichkeitskurven, Schmerz und Lust gleichzeitig möglich, Sensibilisierung, Abstumpfung, Überstimulation) und **Ermüdung** (nach Muskelgruppen, Krämpfe, Haltungsermüdung, Muskelkater, Schwellung, Blutergüsse mit zeitlichem Verlauf, oberflächliche Hautschäden) laufen in einer gemeinsamen **Ergebnisschicht** über dem allgemeinen Körpersystem. Auch Kampf, Sport und Medizin greifen auf dieselben Grunddaten zu.
- **Erholung** addiert sich aus allen aktuellen Belastungen. Stärkere Nachwirkungen beeinflussen später normales Gameplay (Sitzen, Gehen, Rennen, Kämpfen, Schlaf, Arbeit). Kleine Beschwerden können selbst behandelt werden, Eskalation führt zu medizinischer Behandlung; ein Entwickler-Inspector kann Erholungszeiten schätzen.
- **Gedächtnis:** Der Körperzustand speichert relevante frühere Belastungen, damit wiederholt belastete Bereiche später anders reagieren.

---

## 11. Darstellung, Kamera, Audio und Einstellungen

### 11.1 Einstellungen für Spieler (🧭 Entwurf)

| Einstellung | Stufen |
|---|---|
| Erwachsenen-Inhalt | aus / nur Romantik / an |
| Inhaltsprofil | Standard / Fortgeschritten / Hochintensität / alle unterstützten (nur in den erweiterten Einstellungen) |
| Nacktheit | aus / reduziert / voll (optional getrennt nach Spielerfigur und NPC, Ober- und Unterkörper) |
| Darstellung | Schwarzblende / verdeckt / reduziert / voll |
| Audio, Dialog | aus / reduziert / normal |
| Kamera | filmisch an/aus; Spielerkamera eingeschränkt oder frei |
| Anfragen (Mehrspieler) | Romantik und Erwachsenen-Inhalt jeweils: alle / Freunde / Vertraute / niemand |

Die Erzählung bleibt **unabhängig von der Darstellung**: Ein Spieler mit Schwarzblende erlebt dieselben Folgen wie einer mit voller Darstellung. Die Darstellungsmodi sind: deaktiviert, nur Romantik, Schwarzblende, angedeutet, filmisch verdeckt, reduziert, voll erlaubt.

### 11.2 Kamera

Ein **Kamera-Regisseur** (Gameplay, filmisch, nah, mittel, weit, Umgebung, verdeckt, vom Spieler gesteuert, Blende) mit Beschränkungen (keine Wand-, Möbel- oder Figurenüberschneidung, Respekt vor Darstellungs- und Barrierefreiheits-Einstellung, gültiges Framing). Die Kamera kann frei zwischen Third Person, näherer Third Person, First Person (stabilisiert gegen Übelkeit) und filmischer Ansicht wechseln, soweit die Situation es erlaubt.

### 11.3 Audio

Schichten: Dialog, kurze Reaktionen, Atmung, Bewegung, Kleidung, Möbel, Umgebung, Raumton, Wetter. Der Raum entscheidet mit, ob jemand bewusst leiser ist (Wände, Nachbarn). Ausdrucksstärke ist stark persönlich.

### 11.4 Barrierefreiheit

Untertitel, Überspringen, Stopp, Schwarzblende, reduzierte Darstellung, abschaltbare Kamera- und Bewegungsunschärfe-Effekte, Kamerasteuerung, Audioregelung, vereinfachte Eingaben.

### 11.5 Replay und Aufzeichnung

📋 Vorgesehen ist ein Replay-System auf Basis des gespeicherten Szenenzustands (nicht nur Video): Kamera, Sichtfeld, Tiefenschärfe und Zeitlupe lassen sich **nachträglich** wählen, danach folgt der Export (lokale Dateien, hohe Auflösungen, Qualitätsstufen, Schätzung des Speicherbedarfs). Die Aufnahmen gehören dem Nutzer. Ein Teil dieser Planung ist allgemein (Replay/Aufnahme/Streaming) und nicht Adult-spezifisch.

---

## 12. Steuerung, Oberfläche und VR

📋 Planungsstand aus den Runden 18 und 19 (teilweise noch zu präzisieren):

- **Zwei Steuermodi:** *Direkt* (freiere Körpersteuerung) und *Assistiert* (erweiterte Aktionen und Kombinationen). Der Wechsel soll jederzeit ohne Unterbrechung möglich sein ❓ (Ausgestaltung offen).
- Kombinationen sind **Abkürzungen**; freie Bewegung ist nie auf gespeicherte Kombinationen beschränkt.
- **Analoge Steuerung:** Tempo und Richtung über Stick und Trigger; kein Taster-Spammen. Ein zusätzlicher Modifikator verändert Stärke/Tempo einer laufenden Bewegung. Eine schnelle Wahl des gesteuerten Körperbereichs (bei mehreren gleichzeitigen Aktionen), mit **Smart Context** (das Spiel errät die gemeinte Hand/Region, der Spieler kann übersteuern) und eine Anzeige laufender Aktionen.
- **Aktionsbibliothek:** Ein Menü zeigt alle aktuell möglichen Aktionen mit Beschreibung, Tastenbelegung, Voraussetzungen und Vorschau, gefiltert nach Pose, freien Händen, Items, Umgebung und Körperkonfiguration. Außerhalb einer Situation ist die gesamte Bibliothek einsehbar. Bestimmte Aktionen sind erst nach dem Erlernen freigeschaltet.
- **HUD-Stufen:** aus / minimal / standard / detailliert / Entwickler. Minimal zeigt nur wichtige Hinweise und soll filmisch wirken.
- Alle Tasten sind **frei belegbar**, mit Konfliktwarnung; Kontrollprofile (Controller direkt, Controller assistiert, Tastatur, VR) sind speicherbar. Controller-Haptik entsteht aus echtem Kontakt und ist einstellbar oder abschaltbar. Barrierefreiheits-Hilfen (Halten statt Drücken, längere Eingabefenster, größere Oberfläche).
- **VR:** Dieselben Aktionen, Physik und Körperzustände wie bei Controllern, nur andere Eingabe. Handtracking, optionale Ganzkörper-Verfolgung (nicht Pflicht), IK ergänzt fehlende Körperbewegung, die Aktionsbibliothek als räumliches Overlay (ausblendbar), Handschuh-Haptik später und nie Voraussetzung.

---

## 13. Orte, Öffentlichkeit und Weltreaktion

- **Fast überall**, solange Ort und Kontext es erlauben (Wohnung, Hotel, Fahrzeug, abgelegene Außenbereiche, Arbeitsplatz, Party, Unterschlupf). Ein Bett oder Sofa braucht **keinen speziellen Anker**: Oberflächen werden aus Geometrie, Kollision und Material erkannt.
- **Orts-Eigenschaften:** Privatsphäre, Sichtbarkeit, Schalldämmung, Raumgröße, Möbel, Temperatur, Sauberkeit, Zugänglichkeit und Wahrscheinlichkeit, dass jemand auftaucht. Privatsphäre wird **tatsächlich simuliert** (Türen, Fenster, Sichtlinien, NPCs, Entfernung, Audio-Ausbreitung), nicht als Schalter. Es gibt eine Absicht („Privatsphäre gewünscht“) und eine tatsächliche Entdeckungswahrscheinlichkeit. Orte haben interne Tags, die der Spieler nicht als Rohwerte sieht. Figuren verhalten sich ortsabhängig; manche mögen Öffentlichkeit oder Risiko, andere nur privat.
- **Beobachter** sind eine echte Rolle mit eigener KI und Gedächtnis (zufällig gesehen, absichtlich zugeschaut, eingeladen). Sie können ignorieren, neugierig schauen, bleiben, weggehen, Freunde holen, Sicherheitsdienst oder Polizei informieren oder später darüber reden. Beobachter können eingeladen werden oder spontan Interesse am **Beitreten** zeigen; umgekehrt kann jemand die Situation verlassen und die anderen machen weiter, sofern die Konstellation unterstützt wird.
- **Zeugen** entstehen aus Sichtlinie, Entfernung, Räumen, Türen, Wänden, Schallausbreitung, Licht und Sichtbarkeit.
- **Weltregeln statt Sonderregeln:** Wo Polizei, Sicherheitsdienst oder NPCs reagieren, hängt von Ort und Kontext ab (privater Club ≠ Park), nicht vom Erwachsenen-System selbst. Dafür sind allgemeine Welt- und Anstandsregeln der Orte zuständig, die das Paket abfragt.
- **Orte merken sich** Zustände und Vorfälle (Eigentümer, Zugang, Kameras, bekannte NPCs, Beschwerden).
- Mehrere Interessenten, Gerüchte unter NPCs und Folgen bei Entdeckung gehören dazu. Öffentlich sichtbares Verhalten kann durch Zeugen echte Folgen haben; diese laufen über die allgemeinen Welt-, Rechts- und Reputationssysteme.

---

## 14. Die Erwachsenen-Branche als Weltschicht

Diese Weltschicht ist als Teil der normalen Wirtschaft und Gesellschaft geplant, nicht als Parallelwelt (📋, im Umfang 🧭):

- **Orte:** Stripclubs, Swinger- und Erwachsenen-Clubs, Erwachsenen-Unterhaltung und Läden. Stripclubs sind mehr als Kulisse. Es gibt unterschiedliche Preis-, Qualitäts-, Privatsphäre- und Rufstufen sowie unterschiedliche Rechtslagen je nach fiktivem Recht und Zuständigkeitsbereich.
- **Dienstleistende NPCs:** Individuelle, dauerhafte erwachsene Figuren mit Namen, Persönlichkeit, Tagesablauf, Grenzen, Vorlieben, Preisen und Erinnerungen. **Nicht jede Person bietet jede Leistung.** Preise sind individuell und marktbasiert, es gibt Premium-Angebote. Geld ersetzt weder persönliche Anziehung noch individuelle Grenzen. Beziehungen können aus beruflichem Kontakt entstehen. Eine Beziehungsfigur behält ihre eigenen harten Grenzen; die Verfügbarkeit anderer einvernehmlich handelnder Erwachsener hebelt sie nicht aus. Transaktionen laufen über das **normale Wirtschaftssystem**, nicht über eine Sonderwährung.
- **Karriere und Geschäft:** Erwachsenen-Arbeit als echte Karriere; Studios und Plattformen können wirtschaftlich scheitern, konkurrieren, Trends ändern sich. Es gibt eine Creator-Ebene (Abonnements, Inhalte, parasoziale Beziehungen), professionelle Studios, Inhaltsproduktion (auch durch den Spieler) und ein funktionales Spielwelt-Internet mit Erwachsenen-Seiten und Läden. Der Inhalt von In-Game-Videos soll ohne riesigen Speicherbedarf auskommen (🧭, technischer Ansatz offen).
- **Items:** Erwachsenen-Gegenstände sind echte Weltobjekte mit Größe, Material, Qualität, Verschleiß, Reinigung und Lagerung; es gibt unterschiedliche Läden, NPCs kaufen sie selbst. Das Risiko einer Handlung leitet sich nicht aus einem einzelnen Größenwert ab, sondern aus Körperzustand, Anatomie, Erfahrung und Belastung.
- **Medienkonsum und Solo-Verhalten** sind als individuelle, private NPC-Zustände geplant (Häufigkeit stark individuell, Browser- und Abonnement-Historie als privater Zustand).
- **Keine „Adult-Welt neben der echten Welt“:** Alles hängt am selben Gedächtnis, an denselben Beziehungen, derselben Physiologie und demselben Weltzustand.

---

## 15. Charakteranpassung: Tattoos, Piercings, Körpermodifikation

Die Planung (Runden 14 und 17) erweitert den **allgemeinen** Charaktereditor. Es gibt **keinen zweiten Charaktergenerator**; das Erwachsenen-Paket ergänzt nur zusätzliche anatomische Detailoptionen für Erwachsene im selben System. 📋

- **Körper** als persistenter, veränderlicher Zustand mit kontinuierlichen Parametern: Knochenbau, Proportionen, Größe, Becken, Muskelmasse, Fettverteilung, Haltung, Asymmetrien. Weichgewebe wird aus der Anatomie abgeleitet; Gewicht, Training, Schwangerschaft, Alter und kosmetische Eingriffe verändern es langfristig.
- **Tattoos:** von keinem bis nahezu vollständiger Körperbedeckung (einzelne Motive, Sleeves, Rückenstücke, zusammenhängende Ganzkörperkonzepte, nachträgliches Auffüllen, Mischung von Künstlern und Stilen, Gesicht und Hände, sehr selten Extremformen). Je extremer, desto seltener begegnet man solchen Figuren zufällig, außer in Studios, bei Messen oder in Szene-Orten. Tattoo-Künstler haben mehrdimensionale Fähigkeiten (Linienführung, Schattierung, Realismus, Blackwork, Cover-ups, Narbenabdeckung). Lebenszyklus: frisch → Heilung → stabil → Alterung/Verblassen → Auffrischung/Cover-up/Laser. Körperveränderungen verändern sie sichtbar; NPCs bekommen, bereuen, entfernen oder überarbeiten sie im Lauf ihres Lebens. Tätowierungsraten ändern sich über Jahre und Trends.
- **Piercings und Körpermodifikation:** von keinem bis sehr vielen; an vielen plausiblen Körperstellen. Schmuck ist getrennt vom Piercing (Form, Größe, Material, Gewicht, Zustand). Heilungszustände, Reizungen, Verletzungen und Narben. Seltenere Modifikationen (z. B. Narbengestaltung, gespaltene Zunge, subdermale Implantate) kommen vor, meist seltener.
- **Kosmetische Eingriffe** als echte Weltmechanik (Brustvergrößerung/-verkleinerung, Fettabsaugung, Gesichtschirurgie, Nasenkorrekturen und weitere): Kliniken, Ärzte, Preise, Ruf, Termine, Heilungszeit, mögliche Komplikationen. Ein Eingriff verändert wirklich Maße, Kleidung, Weichgewebe-Physik, Schwerpunkt, Animation/IK und Aussehen. NPCs können selbst solche Entscheidungen treffen.
- **Studios** sind echte Betriebe mit Internetauftritt (Portfolio, Künstler, Bewertungen, Termine, Preise). Spieler können später vereinfachte Webseiten erstellen (Teil des Wirtschaftssystems).
- **Technik:** Tattoo-Darstellung mit hoher Nahqualität, aber aggressiv gebündelt und Detailstufen-optimiert; gemeinsamer Produktionsablauf für Spieler, NPCs, Mods und Werkzeuge (Entwurf/Import, Vorschau, Interpretation durch den Künstler, Platzierung, Sitzungen, Heilung, Langzeitzustand).
- Defaultregel für alles nicht eigens Festgelegte: so realistisch und logisch wie in der echten Welt.

---

## 16. Gesundheit und Folgen

Alles läuft über das **allgemeine Gesundheitssystem**, nicht über ein isoliertes Erwachsenen-Gesundheitssystem. 📋 (Detailtiefe 🧭)

- **Schwangerschaft:** verändert Körper und mögliche Interaktionen realistisch; die Erholung danach wird langfristig simuliert.
- **Verhütung:** unter anderem hormonelle Verhütung als Teil der Gesundheitssimulation.
- **Sexuell übertragbare Infektionen:** Teil der Gesundheits- und Gedächtnissimulation.
- **Hygiene, körperliche Zustände, kumulative Belastungen** und Selbstversorgung, bis hin zu ärztlicher Behandlung.
- **Drogen, Alkohol und Stimmung** beeinflussen Verhalten, aber nicht die Zustimmung (Abschnitt 7.3).

Genauere Zahlen, Raten und Zeiträume sind ❓ offen.

---

## 17. Dunkle Kriminalität als getrenntes Modul

Sexuelle Gewalt kann als **ernstes Welt- und Fallthema** in einer gefährlichen Welt vorkommen, ist aber **nicht Teil des Erwachsenen-Systems** und wird nicht erotisiert. 📋

- Trennung der Pakete: *A – Erwachsenen-Framework*, *B – Hochintensitäts-Inhalte*, *C – Dunkle Kriminalität / Mature World Events*. Spieler können sie unabhängig voneinander ein- und ausschalten.
- Behandlung ausschließlich auf **System- und Konsequenzebene** des Kriminalitätssystems: Straftat, Zeugen, Beweise (z. B. DNA und Spuren), Opferreaktion und Traumafolgen, Erinnerungen, Fahndung, Ermittlung, Festnahme, Prozess, Haft, Reputation. Das Spiel bewirbt solche Taten nicht als Funktion und bietet keine Anleitung, kein Menü und keine detaillierte Ausführungsmechanik. Die Darstellung ist ausdrücklich nicht pornografisch.
- NPCs können auf schwere Täter reagieren (Polizei rufen, eingreifen, festhalten, fliehen, angreifen) abhängig von Persönlichkeit, Fraktion und Moral. Eingreifen wird rechtlich nach Notwehr/Nothilfe, Verhältnismäßigkeit und Beweislage bewertet.
- Eine „Belohnung“ der Rettung durch Sex ist ausgeschlossen. Entwickelt sich später freiwillig und unabhängig etwas zwischen erwachsenen Figuren, ist das ein separater Beziehungspfad.

---

## 18. Technische Integration

📋 Planungsstand (Runde 19 und 20):

- **Gemeinsamer Human Core:** Das Erwachsenen-Paket registriert nur Erweiterungen auf Mensch, Körper und Interaktion und besitzt nie einen zweiten Character-Core. Es wird als optional ladbares Spiel- beziehungsweise Unreal-Modul gebaut; das Hauptspiel muss ohne es baubar sein. Erwachsenen-spezifische C++-Module sind strikt getrennt.
- **Datenhaltung:** Erwachsenen-Zustand wird **getrennt vom normalen Spielstand** in einem eigenen Datenbereich gespeichert (Präferenzen, Erfahrung, Erinnerungen, Körper- und Interaktions-Erweiterungen, Items und Verlauf, Replay- und Statistik-Verweise), verknüpft über **dieselben stabilen Kennungen** wie das Hauptspiel (keine doppelte Entitätswelt). Er lässt sich getrennt sichern oder löschen und optional verschlüsseln beziehungsweise nicht direkt menschenlesbar ablegen. Ein Spielstand muss ohne das Paket sauber laden; bei erneuter Aktivierung bleiben kompatible Daten erhalten. Verwaiste Referenzen werden markiert, nicht gelöscht. Schema-Migrationen sind Pflicht.
- **Aktivierung:** Wo Version und Zustand kompatibel sind, ohne neuen Spielstand ein- und ausschaltbar. Größere Teilsysteme (Kerninteraktion, erweiterte Physik, Replay, Flüssigkeiten, VR) haben eigene Funktionsschalter; schwere Polish-Systeme hängen hinter Schaltern, damit frühe Builds schlank bleiben.
- **Ausfallsicherheit:** Ein Fehler in einem optionalen Teilsystem soll das Teilsystem deaktivieren und Fehlerdaten protokollieren, nicht den Spielstand zerstören.
- **Leistung:** Ereignisgesteuert statt pro Frame; Simulations-Detailstufen (nur der nahe Charakter hat volle Qualität); feste Zeit-, RAM- und VRAM-Budgets pro Teilsystem; stufenweise Qualitätsreduktion (Solver-Iterationen, Sekundärbewegung, Mikrodetail) statt Abschalten von Kernfunktionen; Arbeit über Jobs/Threads mit klarer Eigentümerschaft.
- **Daten statt Code:** Inhalte werden vor allem über registrierte Daten-Assets geladen, sodass neue Inhalte oft ohne neuen C++-Code entstehen. Primär-Asset-IDs, Soft-References und asynchrones Laden, passend zur Weltpartitionierung.
- **Datenschutz:** Telemetrie höchstens für Funktion an/aus, generische Animationsfehler, Abstürze, Asset- und Netzwerkfehler; **keine** detaillierten individuellen Verläufe.
- **Qualitätssicherung:** Automatisierte Tests mit tausenden generierten Kombinationen (Körper, Größen, Orte, Items, Posen, Spielstände), Golden-Tests, Erkennung von Abstürzen, NaN-Werten, Durchdringungen und ungültigen Zuständen, Kochvalidierung, Debug-Overlays und Inspektoren (inklusive Entscheidungsanzeigen für KI-Entscheidungen), Editor- und Batch-Werkzeuge.

---

## 19. Modding und Erweiterbarkeit

📋 Modding des Erwachsenen-Bereichs ist als **gleichwertige Erweiterungsschicht** geplant, mit stabilen Registries und APIs.

**Moddbar:** Interaktionsdefinitionen, Animations-Sätze, Positions-Voreinstellungen, Varianten-Tags, Inhaltsprofile, Übergangs-Sätze, Kameraprofile, Audioprofile, Dialogpakete, Umgebungs- und Oberflächenprofile, Kompatibilitätsprofile, neue Kategorien (mit Rollen, Intensitätsdimensionen, Animationen, Präferenz-Tags, Orten, Regeln).

**Nicht normal moddbar** (Sicherheitskern): Erwachsenen-Berechtigung, Zustimmungspflicht, Spieler-Opt-out, Serverautorität, Blockier-Regeln, Anfrage-Berechtigungen, Sitzungssicherheit.

Weitere Festlegungen: kleinere, stabilere öffentliche API als intern; Mods deklarieren benötigte Fähigkeiten („Capabilities“); Zugriff nur über definierte APIs und Registries; Mod-Manager prüft vorab Kompatibilität von Paket, Basisspiel, Spielstandschema und abhängigen Mods. Inhaltsbewertungen und Altersmarkierungen gehören ins Manifest.

---

## 20. Netzwerk und Mehrspieler

🧭 Der Netzwerkteil stammt überwiegend aus dem Rahmenentwurf vom 28. August. Er ist durch die spätere Festlegung teilweise **überholt**, dass der offizielle Online-Modus keine Mods lädt und das Paket dort nicht nutzbar ist. Er bleibt relevant für die **Koop-Kampagne** und als Architekturgrundsatz.

- **Serverautorität** über Sitzung, Teilnehmerliste, Berechtigung, Zustimmungsprüfung, Umgebungsreservierung und Spielergebnis; Clients bestimmen nur Kamera, Darstellung, Nacktheits-Einstellung, Barrierefreiheit und Oberfläche.
- **Jeder Spieler stimmt separat zu.** Keine Mehrheitsentscheidung, keine Gruppen- oder Administrator-Autorität.
- **Ablauf:** Anfrage → Prüfung → Annehmen oder Ablehnen → gegenseitige Zustandsprüfung → Sitzung → Reservierung → Assets vorbereiten → Bereitschaftsprüfung → Start. Anfrage-Berechtigungen (alle / Freunde / Vertraute / niemand), Spam-Schutz (Abkühlzeit, Duplikate, Ratenbegrenzung, Ignorieren, Blockieren, Auto-Ablehnung), serverseitiges Blockieren.
- **Clientseitige Darstellung:** Dieselbe Sitzung kann auf verschiedenen Clients unterschiedlich dargestellt werden (voll, reduziert, allgemeine Interaktion, Sichtschutz); ein Client mit ausgeschalteten Erwachsenen-Inhalten empfängt keine Anfragen und lädt die Assets möglichst nicht. Es gibt kein automatisches Matchmaking für solche Interaktionen.
- **Replikation** nur von Sitzungs-ID, Teilnehmern, Szenenzustand, Animationszustand, Übergang und Zeit, nicht von Knochen oder Kamerahistorie.
- **Schutz vor Missbrauch:** Blockieren von Ankern, Verschieben von Möbeln, Fahrzeug- und Physikmissbrauch, absichtliches Unterbrechen.
- **Robustheit:** Verbindungsabbruch → sicherer Ausstieg; nach einem Serverabsturz ist der Charakter in einem sicheren Normalzustand, nur bereits bestätigte Ergebnisse bleiben.

---

## 21. Produktionsplan und Qualitätstore

📋 (Runde 20, 27. September 2026) Planung ist für den Erwachsenen-Bereich „im Wesentlichen abgeschlossen“, vorbehaltlich echter Erkenntnisse aus Prototyping, Rechtslage und den ausstehenden Audits.

- **Version 1 startet klein:** Grundinteraktionen, wenige Positionen und Übergänge, grundlegende Reaktionen, einfache Präferenzen, eine kleine Testumgebung. Erst danach wächst der Umfang schrittweise.
- **Prioritätsklassen:** Kern → Wichtig → Späte Politur.
- **Kerntor:** Version 1 gilt erst als funktionierend, wenn IK, Physik, Körperzustand, Reaktionsgraph, Speichern/Laden und NPC-Absicht stabil zusammenspielen. Lieber weniger Funktionen, aber sauber, statt viele halbfertige Aktionen.
- **Tore:** Leistungstor (Zeit-/RAM-/VRAM-Budgets vor dem Zusammenführen), QA-Tor (automatisierte Solver-, Kollisions-, Speicher- und Replay-Tests), visuelles Qualitätstor (Animation/IK, Kontakt, Weichgewebe, Audio und Reaktionen müssen zusammen glaubwürdig wirken).
- **Registry-Einfrieren** erst nach dem Taxonomie-Audit und der externen Gegenprüfung.
- **Dokumentation:** Für jedes große Teilsystem vor der Implementierung eine kurze technische Spezifikation (Eingaben, Ausgaben, Abhängigkeiten, Datenmodell, Ereignisse, Speicherzustand, Tests, Leistungsbudget).
- **Erst ein kleiner vertikaler Schnitt, dann Skalierung.** Die allgemeinen Systeme (Mensch, Physik, Gesundheit, Gedächtnis, Welt, Replay, Statistik, Modding) werden zuerst und für das ganze Spiel gebaut. Die zeitliche Einordnung zum restlichen Projekt ist ❓ offen und in der [Roadmap der README](../../README.md#27-entwicklungsphasen-roadmap-und-meilensteine) nicht enthalten.

---

## 22. Offene Punkte, Widersprüche und überholte Planung

### 22.1 Offen (❓)

- Finaler Vertriebsweg, Plattform- und Altersfreigabe; rechtliche Prüfung des Pakets.
- Finaler Zuschnitt der Pakete (Gesamtgröße, Aufteilung, Preset-Bündel).
- Abschluss des **Missing-Family-/Taxonomie-Audits** und der externen Gegenprüfung; Einfrieren der Registry.
- Konkrete Inhaltskataloge, Animationsumfang, Kameraprofile und Stimmenproduktion (Besetzung und Produktionsweg).
- Zeitliche Einordnung der Umsetzung im Projektverlauf.
- Netzwerkdetails für Koop; Vertragsfragen zur Stimmenproduktion.
- Ausgestaltung der Steuermodi „Direkt“/„Assistiert“, Tastenbelegung, Lernsystem für Aktionen (das Lern- und Trainingssystem für Kampf gehört zu einem allgemeinen Kampf-Block).
- Konkrete Raten, Zeiträume und Zahlenwerte der Gesundheitssimulation.
- Detailregeln zur Zustimmung bei weiteren Beeinträchtigungen (Drogen, Medikamente).
- Altersmarkierung von Mod-Inhalten und Verifizierungsablauf in der Praxis.
- Mehrstufige Mehrpersonen-Konstellationen, die nicht gemeinsam animiert werden können (Rückfallverhalten ist festgelegt, die Abdeckung nicht).

### 22.2 Überholt oder aufgelöst (🕰️)

| Frühere Planung | Nachfolgender Stand |
|---|---|
| Rahmenentwurf (28. August): Erwachsenen-Darstellungsmodi (inklusive „voll erlaubt“) als Teil des Spielkerns | Seit der Festlegung vom 31. August und den September-Runden: Der erweiterte Teil ist ein **getrenntes optionales Paket**; das Basisspiel zeigt nicht explizit. |
| Rahmenentwurf: Anfrage-Fluss und Server-Architektur auch für großen Online-Betrieb | Der offizielle Online-Modus lädt keine Mods; das Paket ist dort nicht nutzbar. Der Netzwerkteil bleibt nur für Koop und als Architekturgrundsatz. |
| Frühe Zwischenstände zur Zustimmung bei starker Trunkenheit | Endgültiger Stand siehe Abschnitt 7.3 (Entscheidungsfähigkeit ist Voraussetzung; Passivität ist keine Zustimmung). |
| Standardwert „Alter bestätigt = wahr“ in einem externen Entwurf | Ersetzt durch „im Zweifel gesperrt“ mit explizitem Status. |
| Interner Entwurf mit „versionsabhängigen IDs“ und Unreal-Typen im engine-unabhängigen Kern | Als nicht übernehmbar eingestuft (stabile, versionsfreie IDs; Kern bleibt engine-unabhängig). |

### 22.3 Hinweis zur Quellenlage

Die Planung des Erwachsenen-Bereichs liegt überwiegend als Gesprächsprotokoll vor. Dort enthaltene Formulierungen von KI-Assistenten (Entwürfe, Datenstrukturen, Listen) sind **Vorschläge**, keine Entscheidungen. Hier wurden nur die vom Entwickler bestätigten Festlegungen übernommen, und in einzelnen Fällen sind frühere Entwicklerwünsche im Planungsprozess nach Korrektur ersetzt worden.

---

## 23. Ausdrücklich nicht Teil der Planung

Diese Punkte sind **nicht** vorgesehen und werden in diesem Projekt nicht spezifiziert:

- Minderjährige in jeder Form im Erwachsenen-System.
- Nicht einvernehmliche sexuelle Handlungen als Spielmechanik, in welcher Variante auch immer. Zu einer solchen Mechanik existiert keine Spezifikation; sexuelle Gewalt kommt nur als Kriminalitätsthema auf Konsequenzebene vor (Abschnitt 17).
- Sexuelle Handlungen mit bewusstlosen oder schlafenden Figuren als aktiv Teilnehmende.
- Story- oder Quest-Zwang zu intimen Handlungen.
- Zustimmung aus Beziehungswerten, KI-Zielen, Gruppen- oder Administratorentscheidungen.
- Sichtbare „Sex-Level“-Zähler oder eine Arcade-Statistik als Verhaltenssteuerung.
- Der Erwachsenen-Bereich im offiziellen Online-Modus.
- Explizite Darstellung im Basisspiel und in Konsolen-Builds.
