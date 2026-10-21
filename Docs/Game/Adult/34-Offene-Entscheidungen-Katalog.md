# Adult 34: Katalog offener Entscheidungen

> [!IMPORTANT]
> **Nichts davon ist implementiert und nichts davon ist entschieden.** Der Katalog sammelt alle offenen Entscheidungen des Adult-Systems an **einer** Stelle, mit Optionen, Abhängigkeiten und Wirkung. Er enthält **keine Empfehlungen als Beschluss**; die Entscheidung liegt beim Entwickler (CLAUDE.md §1). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Konflikte im Detail: [Modul 19](19-Konflikt-und-Widerspruchsmatrix.md); kürzere Liste: [Modul 11](11-Offene-Fragen-und-Konflikte.md); Register: [Modul 14](14-Entscheidungsregister.md).

**Inhalt**

1. [Aufbau und Priorität](#1-aufbau-und-priorität)
2. [Sicherheit und Zustimmung](#2-sicherheit-und-zustimmung)
3. [Welt, Setting, Gesellschaft](#3-welt-setting-gesellschaft)
4. [Recht, Plattform, Vertrieb, KI-Offenlegung](#4-recht-plattform-vertrieb-ki-offenlegung)
5. [Technik und Produktion](#5-technik-und-produktion)
6. [Inhalt und Darstellung](#6-inhalt-und-darstellung)
7. [Zahlen, die festzulegen sind](#7-zahlen-die-festzulegen-sind)
8. [Audits und externe Läufe](#8-audits-und-externe-läufe)
9. [Abhängigkeitsübersicht](#9-abhängigkeitsübersicht)

---

## 1. Aufbau und Priorität

Jede Entscheidung hat eine Nummer `OE-nn`, eine Frage, die Optionen **ohne Wertung**, die Auswirkung und den Bezug.

**Dringlichkeit (🧭 Vorschlag der Dokumentation):**
- **A** = blockiert die Spezifikation oder Daten (vor Implementierung nötig),
- **B** = blockiert Release oder Produktion,
- **C** = kann während der Entwicklung entschieden werden.

## 2. Sicherheit und Zustimmung

| Nr. | Dringl. | Frage | Optionen (ohne Wertung) | Auswirkung | Bezug |
|---|---|---|---|---|---|
| OE-01 | A | Wie wird der Wunsch vom 20.09. (Handeln gegen den Willen, bei Schlafenden) behandelt? | (a) Wunsch zurückziehen; (b) reiner **Konsequenz-Testpfad** (Zustände, Beweise, Polizei, Gericht) ohne sexuelle Handlungsmechanik definieren lassen; (c) aufrechterhalten (dann außerhalb dieser Dokumentation) | Sicherheitskern bleibt unverändert; Testkatalog; externe Planung | [K2](19-Konflikt-und-Widerspruchsmatrix.md#k2-user-lock-vom-2009) |
| OE-02 | A | Wie gewichten und takten wir die Live-Signale des Sitzungsrahmens („nicht zu streng“)? | im Prototyp mit Spielern ermitteln; Kandidaten für Zeitfenster und Schwellen | Spielgefühl, Sicherheit | [K1](19-Konflikt-und-Widerspruchsmatrix.md#k1-frühe-zustimmungsvorstellungen-vs-sicherheitskern) |
| OE-03 | A | Gibt es einen einvernehmlichen **Schlaf-Zustand**? | (a) nein (strengere Regel bleibt); (b) vereinbartes Schlafspiel mit Zustand/Atmung/Aufwachen | Capacity-Gate, Datenmodell | [K9](19-Konflikt-und-Widerspruchsmatrix.md#k9-einvernehmlicher-schlaf-zustand) |
| OE-04 | A | Schwellen für Capacity (leicht/stark beeinträchtigt) und Erkennungsfenster | Konfigurationswerte per Prototyp | Sicherheit, UX | Modul 20 |
| OE-05 | B | Gelten Zustimmungen über mehrere Tage (Dauerbeziehung) als Dauerrahmen? | (a) nein (neue Begegnung = neue Berechnung); (b) ja mit Ablauf; (c) pro Beziehungsform | Datenmodell | E-SK-025 |
| OE-06 | B | Wie werden Alter und Zustimmung für **Gäste im privaten Koop** geprüft? | Host-Verantwortung; Gast-Opt-in; Gast-Altersnachweis; Paket nur bei allen Teilnehmern aktiv | Netzwerk, Datenschutz | [K21](19-Konflikt-und-Widerspruchsmatrix.md#k21-mehrspieler) |
| OE-07 | B | Regel für gefälschte Altersnachweise in der Welt | (a) Weltphänomen ohne Kernauswirkung; (b) Figurenzustand bleibt maßgeblich | Gate | Modul 31 RF-ZU-10 |
| OE-08 | B | Aufbewahrungsdauer und Umfang der Zustimmungshistorie | Verdichtung/Archivierung; Datenschutz | Speicher | Modul 21 |

## 3. Welt, Setting, Gesellschaft

| Nr. | Dringl. | Frage | Optionen | Auswirkung | Bezug |
|---|---|---|---|---|---|
| OE-10 | A | Orientierung im Datenmodell (Setting-Wunsch vs. Spektrum) | (a) Modell nur mit vorgesehener Demografie, Mods dürfen mehr; (b) neutrales Modell, Demografie über Daten; (c) weitere Vorgabe | Beziehungsdatenmodell, Mods, Plattformangabe | [K5](19-Konflikt-und-Widerspruchsmatrix.md#k5-orientierung) |
| OE-11 | A | Welche Spielerfigur-Geschlechter und Konstellationen gibt es? | entscheidet der Entwickler | Datenmodell | Modul 11 |
| OE-12 | B | Zuwanderung/Touristen in der Bevölkerung | (a) keine; (b) einzelne; (c) datengesteuert | Generator, Namen, Stimmen | [K12](19-Konflikt-und-Widerspruchsmatrix.md#k12-zuwanderung) |
| OE-13 | C | Beschneidung und religiöse Zusammensetzung | Setting-Daten | Anatomie-/Religionsdaten | [K14](19-Konflikt-und-Widerspruchsmatrix.md#k14-beschneidung-und-religion) |
| OE-14 | B | Zeitmaßstab der Weltzeit und Sitzungszeit | nahe 1:1 (Vorschlag), anderer Faktor, variabel | Zeitpläne, Heilung, Schwangerschaft | [K15](19-Konflikt-und-Widerspruchsmatrix.md#k15-zeitmaßstab) |
| OE-15 | B | Behandlung von **Sexualstraftaten durch Spielerfiguren** als Weltereignis | (a) gar nicht; (b) nur Konsequenzebene ohne Handlung; (c) anderes | Justiz, Rating | [K3](19-Konflikt-und-Widerspruchsmatrix.md#k3-spielerbegangene-sexualstraftaten) |
| OE-16 | B | Gespielte Nichteinwilligung in Produktionen | (a) nicht aufnehmen; (b) als vereinbartes Rollenspiel mit Spezifikation und Rechtsprüfung | Taxonomie, Recht | [K4](19-Konflikt-und-Widerspruchsmatrix.md#k4-gespielte-nichteinwilligung) |
| OE-17 | B | Langzeit-Begleitvertrag: Kündigungsmodell | Kompromiss (NPC kann kündigen, sehr selten) / anderes; Altersnachweisverfahren; Darstellung | Wirtschaft, NPC | [K6](19-Konflikt-und-Widerspruchsmatrix.md#k6-langzeitvertrag) |
| OE-18 | C | Anzahl, Lage und Größe der Betriebe; Preismodell | Datenvorgaben der Welt | Wirtschaft | Modul 26 |
| OE-19 | C | Jurisdiktionen der fiktiven Welt und ihre Gesetze | Daten | Betriebe | E-WE-004 |
| OE-20 | C | Familiensystem/Generationen | niedrige Priorität/offen | Gesundheit/NPC | CA05 |
| OE-21 | C | Mehr als eine Beziehung gleichzeitig: Balance | Spielbalance | NPC | Modul 24 |
| OE-22 | B | Extremistische Symbole auf Tattoos | (a) nicht aufnehmen; (b) an Fraktionen, fiktive Symbole; (c) Prüfung | Recht, Plattform | [K16](19-Konflikt-und-Widerspruchsmatrix.md#k16-extremistische-symbolik) |

## 4. Recht, Plattform, Vertrieb, KI-Offenlegung

| Nr. | Dringl. | Frage | Optionen | Auswirkung | Bezug |
|---|---|---|---|---|---|
| OE-30 | B | Veröffentlichungsziel (privat/öffentlich) und Zeitpunkt | öffentlich als Ziel (zuletzt gewünscht), privat als Rückfall | Recht, Builds | [K18](19-Konflikt-und-Widerspruchsmatrix.md#k18-veröffentlichungsziel) |
| OE-31 | B | Vertriebsweg des Pakets (Steam, Workshop, extern, nur SDK) | Optionen im Gespräch 23#541–548 | Recht, Support | E-PA-008 |
| OE-32 | B | Rechtsberatung (Österreich/Deutschland, Plattformen, Zahlungsdienste) | Fachberatung | alles | [Modul 10](10-Produktion-Recht-Vertrieb.md) |
| OE-33 | B | Offenlegung der KI-Nutzung | wahrheitsgemäße Angaben (Quellen: Assistent), Form offen | Release | [K7](19-Konflikt-und-Widerspruchsmatrix.md#k7-ki-geheimhaltung) |
| OE-34 | B | Grenze „Adult Only“ und Nacktheits-Deskriptoren | Plattformprüfung | Einstufung | [K17](19-Konflikt-und-Widerspruchsmatrix.md#k17-erst-bei-penetration-adult-only) |
| OE-35 | B | Streamer-Filter und Namen der Nacktheitsstufen | Plattformregeln | Streaming | E-ST-012 |
| OE-36 | C | Fiktive Marken, Projektname, IP | Entscheidung zum Release | Marketing | E-RC-005 |
| OE-37 | C | Provenienzregister: Detailtiefe | pro Asset | Release | E-RC-009 |
| OE-38 | B | Konsolen-Zertifizierung und Pakettrennung | Plattformprüfung | Builds | E-PA-004 |

## 5. Technik und Produktion

| Nr. | Dringl. | Frage | Optionen | Auswirkung | Bezug |
|---|---|---|---|---|---|
| OE-40 | A | Unreal-Version, Plugin-Layout, Repository-Struktur für den Kern | Validierung nötig | alles | CLAUDE.md §5 |
| OE-41 | A | Testframework für Standalone-C++-Tests | z. B. Catch2/GoogleTest/doctest (nicht entschieden) | Tests | CLAUDE.md §5 |
| OE-42 | A | Speicherformat und Backend des Adult-Bereichs | Binär/JSON/Datenbank | Daten | Modul 21 |
| OE-43 | B | Verschlüsselung des Adult-Bereichs | optional; Umfang | Datenschutz | E-PA-030 |
| OE-44 | B | Anfangsposition: Spieler wählt oder NPC positioniert sich | Optionen | Engine | E-SO-009 |
| OE-45 | B | Steuerungsdetails (Belegung, Modifier, Menü) | Prototyp | UI | E-SO-030 |
| OE-46 | B | Verhältnis Mocap/Motion Matching/Prozedural | Prototyp | Animation | E-PA-026 |
| OE-47 | B | Weichgewebe-Solver und Budgets | Prototyp | Physik | Modul 23 |
| OE-48 | B | Wann wird Adult in der Entwicklungsreihenfolge umgesetzt? | „sehr früh“ vs. „nach Ladebildschirm/Karte“ | Planung | [K19](19-Konflikt-und-Widerspruchsmatrix.md#k19-zeitpunkt-von-adult) |
| OE-49 | C | Preset-Bündel (Minimal/Standard/Full) | später | Paket | E-PA-022 |
| OE-50 | C | Paketgrößen und -aufteilung | nach Größenmessung | Distribution | E-PA-018 |
| OE-51 | C | Verhältnis Tattoo-Rendering (Decals/Virtual Texture/prozedural) | Prototyp | Rendering | Modul 25 |
| OE-52 | C | Barrierefreiheitsziele | Standards wählen | UI | E-ST-005 |
| OE-53 | C | VR-Einführung | später | UI | E-ST-006 |

## 6. Inhalt und Darstellung

| Nr. | Dringl. | Frage | Optionen | Auswirkung | Bezug |
|---|---|---|---|---|---|
| OE-60 | B | Detailtiefe körperlicher Grenzreaktionen und deren Darstellung | Konfigurierbarer Inhalt, Plattformabstimmung | Recht, Content-Toggle | [Modul 23, Abschnitt 8](23-Spezifikation-Reaktions-und-Koerpersystem.md#8-körperliche-grenzreaktionen-und-inhaltsschalter) |
| OE-61 | B | Darstellungsprofile (Konsole/PC/Mod) und Kamera-Regeln | Profile aus 23#538 als Ausgangspunkt | Darstellung | K§17 |
| OE-62 | C | Umfang der Hochrisiko-Handlungen im ersten Release | Prioritätsklassen | Scope | E-PR-012 |
| OE-63 | C | Fluid-System Zeitpunkt und Umfang | spät | Scope | E-IA-033 |
| OE-64 | C | Reihenfolge der Kategorien (Core/Important/Late) | Entscheider | Scope | E-IA-037 |

## 7. Zahlen, die festzulegen sind

| Nr. | Größe | Quelle der Zahl | Stand |
|---|---|---|---|
| OE-70 | Tattoo-Verteilung (Prozente) | Assistent 35#776; Entwickler „passt“ | vorläufig |
| OE-71 | Studiodichte (Metropole 20–30 oder mehr) | Entwickler | Wunsch |
| OE-72 | Stripclubs (Metropole 15–30) | Entwickler | Wunsch |
| OE-73 | Speichergröße (50–150 MB) | Assistent | Orientierung |
| OE-74 | Wartezeit beim Tattoo (1–2 Spielwochen) | Entwickler | Wunsch |
| OE-75 | Tagespreis Begleitvertrag (250 EUR/Tag) | Entwickler als Beispiel | Beispiel |
| OE-76 | Performance-Budgets je Subsystem | – | fehlt |
| OE-77 | Erkennungsfenster/Check-in-Zeiten | – | fehlt |
| OE-78 | Entity-Zahlen (500k/5k/100–300) | Assistent | Hypothese |
| OE-79 | Toleranzen für Tests | – | fehlt |

## 8. Audits und externe Läufe

| Nr. | Frage | Stand |
|---|---|---|
| OE-80 | **Missing-Family-/Taxonomie-Audit** (Ende R16) | offen; Pre-Audit A1–A15 liegt vor ([Modul 17](17-Interviewprotokoll-Runden-17-bis-20-und-Rahmenchats.md#23-pre-audit-742748)) |
| OE-81 | **Qwen-Audit** (Lückenfinder) | offen; kein Qwen-Ergebnis im Archiv; Entwickler „jetzt nicht“ (35#747) |
| OE-82 | **Registry-Freeze** | nach OE-80/81 (E-PR-009) |
| OE-83 | Prototyping- und Rechtserkenntnisse | offen |
| OE-84 | Konsolidierte Master-Dokumente für R1–R20 | im Master nicht auffindbar ([Modul 13](13-Quelleninventar-und-Abdeckung.md#2-befund-wo-die-adult-planung-tatsächlich-liegt)) |
| OE-85 | Datei `USER_REQUESTED_BUT_NOT_CANONICALIZED.md` | im Archiv nicht vorhanden |

## 9. Abhängigkeitsübersicht

```
OE-01 (K2) ─┐
OE-02/03/04 ┼─► Spezifikation Regelkern (Modul 20) ─► Standalone-Tests
OE-10/11 ───┴─► Datenmodell Beziehungen (Modul 21/24)
OE-40/41/42 ──► Prototyp Regelkern
OE-80/81 ─► OE-82 (Registry-Freeze) ─► Taxonomie-Daten
OE-30/31/32/33/34/35/38 ─► Release/Builds/Recht
OE-14 (Zeitmaßstab) ─► Heilung, Schwangerschaft, Zeitpläne
OE-48 (Zeitpunkt) ─► Phasenplanung (CLAUDE.md §4)
```

Der Katalog ist **nicht** abgeschlossen: Mit weiterer Planung entstehen neue Fragen. Neue Einträge sind mit Datum und Quelle zu ergänzen.
