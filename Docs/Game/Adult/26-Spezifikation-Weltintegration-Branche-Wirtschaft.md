# Adult 26: Spezifikation Weltintegration, Branche und Wirtschaft

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Spezifikationsskizze (🧭 Vorschlag der Dokumentation) mit Quellenbezug ([Register, Abschnitt WE](14-Entscheidungsregister.md#11-we-welt-orte-branche-wirtschaft)). Kein Kanon. Die finale Karte, Stadtgrößen, Betriebsanzahlen und Preise sind **offene Planungsfragen** (CLAUDE.md §3); Zahlen der Quellen sind Entwicklerwünsche oder Vorschläge. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Fachlich: [Modul 08](08-Welt-Orte-Branche-Wirtschaft.md). NPCs: [Modul 24](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md). Recht/Plattform: [Modul 10](10-Produktion-Recht-Vertrieb.md).

**Inhalt**

1. [Ziel und Abgrenzung](#1-ziel-und-abgrenzung)
2. [Beschlossen, vorgeschlagen, offen](#2-beschlossen-vorgeschlagen-offen)
3. [Orte als Umgebungskontext](#3-orte-als-umgebungskontext)
4. [Betriebe des Nachtlebens](#4-betriebe-des-nachtlebens)
5. [Dienstleistung und Transaktion](#5-dienstleistung-und-transaktion)
6. [Medien, Plattformen, Internet](#6-medien-plattformen-internet)
7. [Gesetze und Jurisdiktionen](#7-gesetze-und-jurisdiktionen)
8. [Sicherheit, Polizei, Hausregeln](#8-sicherheit-polizei-hausregeln)
9. [Wirtschaft und Preise](#9-wirtschaft-und-preise)
10. [Zeit und Alltag](#10-zeit-und-alltag)
11. [Verteilung in der Welt](#11-verteilung-in-der-welt)
12. [Technische Komponenten](#12-technische-komponenten)
13. [Randfälle](#13-randfälle)
14. [Tests und Abnahmekriterien](#14-tests-und-abnahmekriterien)
15. [Ungeklärtes](#15-ungeklärtes)

---

## 1. Ziel und Abgrenzung

Das Adult-Paket integriert sich in die **normale Welt**; es gibt **keine Adult-Welt neben der Welt** (R15B 50; E-WE-007). Die Orte gehören ins Hauptspiel (‚ohne Mod hat man dort weniger zu tun‘, E-PA-017). Das Paket ergänzt Inhalte (Interaktionen, Dienstleistungen, Medienproduktion), nicht Orte.

Prinzip: Weltlogik (‚logisch statt emotional‘, E-WE-015) gilt auch hier.

## 2. Beschlossen, vorgeschlagen, offen

| Aussage | Status | Basis |
|---|---|---|
| Nachtleben ist eine breite erwachsene Weltschicht; Stripclubs „essentiell“ | 📋 | E-WE-004, E-WE-006 |
| Adult-Service-NPCs mit Namen, Zeitplan, Persönlichkeit, Grenzen, Präferenzen, Preisen, Erinnerungen | 📋 | E-WE-001 |
| Beruf ≠ private Verfügbarkeit | 📋 / 🧭 | E-WE-009 |
| Performer als persistente NPCs mit Karriere, Creator-Plattform, Studios, Reputation | 📋 | E-WE-007 |
| Mehrere Adult-Websites im Spiel-Internet | 📋 | E-WE-008 |
| Aufnahme und Veröffentlichung getrennte Zustimmungen | 🧭 | E-WE-011 |
| Video in der Spielwelt: immer exakt dasselbe (feste Replay-ID) | 📋 / 🧭 | E-WE-010 |
| Zahlen: 15–30 Stripclubs in der Metropole, ein riesiger mehrstöckiger Betrieb | 🧭 Entwicklerwunsch | E-WE-006 |
| Bordell-Matching-Website, Langzeitbuchung | 🧭 / ⚠ K6 | E-WE-016 |
| Preise, Marktgrößen, Trends | 🧭 | E-WE-017 |
| Spielzeit im Szenenverlauf | ❓ | [K15](19-Konflikt-und-Widerspruchsmatrix.md#k15-zeitmaßstab) |
| Rechtslage je Jurisdiktion | ❓ ungeprüft | [Modul 10](10-Produktion-Recht-Vertrieb.md) |

## 3. Orte als Umgebungskontext

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| WI-OR-01 | Ein Ort ist **Umgebungskontext**, kein Szenentyp; jeder geeignete Raum kommt in Frage; keine Spezialräume nötig. | E-IA-035 |
| WI-OR-02 | Orts-Tags sind **intern** (Spieler sieht sie nicht). | E-IA-006 |
| WI-OR-03 | Orte haben Hausregeln und Zugang (Besitzer, Schloss, Kameras, Beschwerden); Orte merken sich Ereignisse. | E-IA-009 |
| WI-OR-04 | Ortswahl beeinflusst Reaktion, Privatheit, Risiko (Entdeckung, sozial, rechtlich). | E-IA-025, 35#232 |
| WI-OR-05 | **Adult-Hubs/Venues** (Clubs, Swingerclubs) haben eigene Raumtypen und Hausregeln; Teilnahme freiwillig; Bereiche. | E-WE-020 |
| WI-OR-06 | Studios (Medienproduktion) haben Kamera, Licht, Set, Sound, Crew. | E-WE-020 |
| WI-OR-07 | Arbeitsplatz ist möglich mit höherem Risiko. | E-WE-020 |
| WI-OR-08 | Fahrzeuge, Outdoor, Hotels (Schallschutz), Bad/Dusche, Wohnung: über normale Systeme. | E-IA-035 |
| WI-OR-09 | Wetter, Tageszeit und Beleuchtung bestimmen Sichtbarkeit. | E-IA-035 |
| WI-OR-10 | Gebäudeplanung folgt „nach dem Adult-Block“ (Weltplanung). | E-LE-012 |

## 4. Betriebe des Nachtlebens

### 4.1 Betriebstypen (🧭 Vorschlag des Assistenten, 35#527)

| Typ | Beschreibung |
|---|---|
| Stripclub | Show-/Tanzbetrieb mit Schichten; funktional (nicht nur Dekoration) |
| Nightclub / Adult Club | breiter erwachsener Betrieb mit Bereichen, Swinger-/Adult-Angeboten |
| Brothel / Escort / Companion-Agentur | Dienstleistung mit Vertrag und Profilen |
| Unabhängige | über fiktive Websites/Apps |
| Underground | je Jurisdiktion illegal, riskant |
| Adult Entertainment Complex | ein riesiger mehrstöckiger Betrieb mit Premiumbereichen (Wunsch des Entwicklers, E-WE-006) |

### 4.2 Regeln (🧭)

| Nr. | Anforderung | Basis |
|---|---|---|
| WI-BT-01 | Betriebe haben `VenueType`, `Jurisdiktion`, Hausregeln, Sicherheit, Ruf, Zeitplan. | E-WE-001, E-WE-004 |
| WI-BT-02 | **Art Direction**: glamourös erlaubt, mit visueller Vielfalt. | E-WE-002 |
| WI-BT-03 | **Nicht jeder Mitarbeiter bietet jeden Service.** | E-WE-001 |
| WI-BT-04 | Die Dichte ist weltabhängig: Metropole viele (Entwicklerwunsch: 15–30), kleinere Städte wenige. **Zahlen nicht beschlossen.** | E-WE-006 |
| WI-BT-05 | Stripclubs funktionieren mit Schichten und Rollen (Tanz, Bar, Sicherheit, Management). | 35#526 |
| WI-BT-06 | Betriebe können scheitern, Konkurrenz und Trends. | 35#539 |
| WI-BT-07 | „Jungfrau“ als Preis-/Shop-Kategorie wird **nicht** übernommen; Assistent: eher seltener persönlicher NPC-Zustand (35#527). | E-WE-006 |
| WI-BT-08 | Alle Beteiligten sind verifizierte Erwachsene ([Modul 20](20-Spezifikation-Regelkern-Zustimmung.md)). | E-SK-009 |

## 5. Dienstleistung und Transaktion

### 5.1 Service-Transaktionsrahmen (🧭 aus 23#570)

```
Auswahl/Profil  →  Reservierung  →  Leistungsvereinbarung  →  Durchführung (Regelkern prüft!)  →  Abrechnung/Folgen
```

| Nr. | Anforderung | Basis |
|---|---|---|
| WI-SV-01 | Transaktionen laufen über den **allgemeinen** Wirtschafts- und Serviceframework, nicht über einen Adult-Sonderpfad. | K§19 |
| WI-SV-02 | Die Leistungsvereinbarung beschreibt **Rahmen und Grenzen**; sie ersetzt keine Zustimmung; Abbruch jederzeit möglich. | E-SK-003 |
| WI-SV-03 | „Bezahlt“ ≠ „alles erlaubt“. | 35#539 |
| WI-SV-04 | Tarife unterscheiden sich; **Bewusstlosigkeit** ist nie Teil eines Tarifs (Capacity-Stop). | 35#541–545 |
| WI-SV-05 | Preise marktbasiert, Premium/VIP; Geld ersetzt keine Attraktion. | 35#539 |
| WI-SV-06 | Nichtsexuelle Begleitung ist eine eigene Leistung. | 35#551 (R15H 198) |
| WI-SV-07 | Beziehungen können aus beruflichem Kontakt entstehen. | 35#539 |
| WI-SV-08 | Das Leistungsprofil ist vertraglich; Beruf ≠ private Vorlieben (R15H 194–196). | 35#551 |

### 5.2 Langzeit-Begleitvertrag (⚠ K6)

Vorgeschlagene Eigenschaften (35#543/545/547/549): definierte Leistungen, Vergütung, optional Unterkunft/Verpflegung, Kündigungsrechte **beider** Seiten, kein Eigentumsverhältnis, **Alters- und Identitätsprüfung zwingend**. Die Kündigungsfrage ist ein dokumentierter Kompromiss (NPC kann jederzeit kündigen, Wahrscheinlichkeit sehr niedrig bei passendem freiwilligem Arrangement; Spieler kann ebenfalls beenden). Details: [Modul 24, Abschnitt 13](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md#13-dienstleistung-und-verträge).

## 6. Medien, Plattformen, Internet

### 6.1 Medienproduktion

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| WI-MD-01 | Darsteller und Creator sind persistente NPCs mit Karriere; der Spieler kann Creator sein; gemeinsame Produktion mit Bedingungen; Studios, Crews, Verträge. | E-WE-007 |
| WI-MD-02 | **Aufnahme und Veröffentlichung sind getrennte Zustimmungen**: privat speichern / mit Partner teilen / Creator-Seite / kommerziell; Meinungsänderung vor Veröffentlichung. | E-WE-011 |
| WI-MD-03 | Qualität: Amateur vs. Studio, Kameraqualität, Crew, schnell vs. aufwendig. | 35#528 (R15C) |
| WI-MD-04 | Ruhm hat Vor- und Nachteile; Fans wollen Foto/Chance auf Treffen. | E-WE-012 |
| WI-MD-05 | Medienproduktion startet klein und wächst. | E-WE-019 |
| WI-MD-06 | Die Plattform in der Spielwelt (Pornoseiten, OnlyFans-ähnlich) existiert; Performer-Persönlichkeit ≠ privat verfügbar. | E-WE-009 |
| WI-MD-07 | Gespielte Nichteinwilligung wäre nur als vereinbartes Rollenspiel denkbar (K4, **nicht freigegeben**). | E-IA-027 |

### 6.2 In-Game-Videos

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| WI-VD-01 | Ein in der Welt gezeigtes Video ist **immer dasselbe** (feste Replay-ID). | E-WE-010 |
| WI-VD-02 | Gespeichert werden Metadaten + Replay-Daten (Session-Seed, Zeitlinie, Figurenzustand); der Cache rekonstruiert das Video bei Bedarf. | E-WE-010 |
| WI-VD-03 | Cache-Limit einstellbar (z. B. 5/20/50/100 GB/unbegrenzt – Vorschlag). | 35#527 |
| WI-VD-04 | Dieselben Daten können für News und andere Medien genutzt werden (allgemeiner Replay-/Video-Cache). | 35#527 |

### 6.3 Internet

| Nr. | Anforderung | Basis |
|---|---|---|
| WI-NT-01 | Mehrere Adult-Websites (pro Betrieb, Plattformen, Studios), nutzergenerierte 18+-Seiten. | E-WE-008 |
| WI-NT-02 | Private Konten, Verlauf, Abos, Trinkgeld, Nachrichten; Creator antworten nicht automatisch; parasoziale Beziehungen. | 35#539 |
| WI-NT-03 | Spieler kann Webseiten bauen (stark vereinfachter Website-Builder in C++). | E-TA-009 |
| WI-NT-04 | Bordell-Matching-Website und Buchung: ❓/⚠ K6. | E-WE-016 |
| WI-NT-05 | Das Internet ist ein **funktionales Spielsystem** (eigene Planung steht aus). | 35#500 |

## 7. Gesetze und Jurisdiktionen

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| WI-GS-01 | Jurisdiktionen der fiktiven Welt bestimmen, welche Betriebe legal sind; Gesetze sind Daten. | E-WE-004, E-WE-014 |
| WI-GS-02 | Privat / legale Venues / illegal öffentlich werden rechtlich unterschieden. | 35#527 |
| WI-GS-03 | Kameras und Smartphones sind Quellen für Beweise, Medien, Ruf. | 35#527 |
| WI-GS-04 | Öffentliche Handlungen haben normale Weltreaktionen und Rechtsfolgen. | E-WE-005 |
| WI-GS-05 | Das Gesetz für reale Märkte (Österreich, Deutschland, Steam, Zahlungsdienste) ist ein **separater** Block ([Modul 10](10-Produktion-Recht-Vertrieb.md)), nicht Teil der Spielwelt. | E-RC-004 |

## 8. Sicherheit, Polizei, Hausregeln

📋 E-IA-007: Sicherheit, Polizei und NPC-Reaktionen hängen vom **Ort** ab (World Rules / Venue Policy), nicht vom Adult-System.

- Hausregeln definieren, was in einem Betrieb zugelassen ist (Zutritt, Kameras, Verhalten).
- Verstöße führen zu Hausverbot, Polizei, Ruf.
- Das Adult-System kennt keine Sonderrechte für Orte; der Regelkern gilt überall.
- Sicherheitspersonal reagiert nach Venue Policy.

## 9. Wirtschaft und Preise

| Nr. | Anforderung (🧭) | Basis |
|---|---|---|
| WI-WP-01 | Preise sind **marktbasiert** (Nachfrage, Standort, Qualität, Ruf); Preis ≠ Qualität. | E-WE-017 |
| WI-WP-02 | Bezahlung der Beschäftigten unterschiedlich; Karriere; Konkurrenz; Trends; Nischenmärkte. | 35#539 |
| WI-WP-03 | Wirtschaft ist ein **eigener großer Planungsblock** (Casinos, Börse, Aktien, Internet kommen später). | 35#504–507 |
| WI-WP-04 | Echtgeld-Käufe: nur optional, kein Pay-to-win im normalen Spiel (Entwickler, 23#575). Ein Bezug zum Adult-Paket ist **nicht entschieden**. | 23#573–575 |
| WI-WP-05 | Beispielpreis 250 EUR/Tag für einen Begleitvertrag war ein **möglicher Marktpreis**, kein Beschluss. | 35#543 |
| WI-WP-06 | Globales Statistikmenü (wie viele Frauen, Internetverbrauch, Geld, getötete Dämonen) – unabhängig von Adult. | E-WE-018, E-LE-009 |

## 10. Zeit und Alltag

| Nr. | Anforderung | Basis |
|---|---|---|
| WI-ZT-01 | Der Entwickler: 30 Minuten Realzeit dürfen nicht 12 Stunden Spielzeit sein; **muss festgelegt werden**. | E-LE-011 |
| WI-ZT-02 | Vorschlag des Assistenten: nahe 1:1 im Gameplay und in der Interaktion; größere Zeitsprünge nur bei Schlaf, Reisen, Warten; eigener Block „World Time / Calendar / Schedule“. | E-LE-011 |
| WI-ZT-03 | Zeit ist sauber (Weltzeit), kein Adult-eigener Takt (R15H 188). | 35#551 |
| WI-ZT-04 | Öffnungszeiten, Schichten, Termine hängen von der Weltzeit ab. | E-WE-001 |

Status: ❓ ([K15](19-Konflikt-und-Widerspruchsmatrix.md#k15-zeitmaßstab)).

## 11. Verteilung in der Welt

- Nachtleben, Studios, Kliniken, Tattoo-Studios konzentrieren sich in **Groß- und Nebenstädten** (23#706/730/787/789, E-WE-004).
- Kleinstädte haben weniger, Dörfer kaum.
- Das Platzierungskonzept der Karte ist **offen**; die finale Karte ist nicht entschieden (CLAUDE.md §3). Daten (Dichten, Typen) sollen **austauschbar** sein.
- Die Dokumentation legt keine Orte fest.

## 12. Technische Komponenten

| Komponente | Aufgabe |
|---|---|
| `VenueRegistry` | Betriebstypen, Raumtypen, Regeln |
| `JurisdictionRules` | Gesetze als Daten |
| `ServiceMarket` | Angebote, Preise, Nachfrage |
| `BookingSystem` | Reservierung, Verträge |
| `ContractService` | Vertragslaufzeit, Kündigung |
| `MediaProduction` | Aufnahme, Verträge, Einnahmen |
| `ReplayCache` | feste Replay-IDs, Cache |
| `ReputationLedger` | mehrdimensionaler Ruf |
| `WebsiteBuilder` | Spielerwebseiten (eigene Planung) |
| `NightlifeScheduler` | Öffnungszeiten, Schichten |

## 13. Randfälle

[Modul 31](31-Randfaelle-Katalog.md#7-welt-und-wirtschaft): Betrieb schließt während einer Buchung; Vertrag mit Person, die später stirbt; Gesetzeslage ändert sich; Cache voll beim Anzeigen eines Videos; NPC kündigt während des Spielers Abwesenheit; Interaktion in einem Betrieb, dessen Hausregeln sie untersagen; zwei Medienproduktionen mit demselben Material; Zeitraffer über Vertragsende.

## 14. Tests und Abnahmekriterien

| Test | Erwartung |
|---|---|
| Transaktion ohne Zustimmung | Leistung wird nicht erbracht; Geldfluss gemäß Vertragsregeln |
| Tarif ‚bis zur Ohnmacht‘ | nicht erzeugbar; Capacity-Stop gilt |
| Vertragskündigung beider Seiten | beide Wege führen zum Vertragsende |
| Altersprüfung bei Buchung | `NotAdultEligible` → keine Buchung |
| Video-Replay-Konsistenz | gleiches Video bei jedem Abruf |
| Hausregel ‚keine Kameras‘ | Aufnahme blockiert |
| Jurisdiktionswechsel | Betrieb legal/illegal gemäß Daten |
| Verteilung in synthetischer Karte | Betriebsdichte nach Datenvorgabe |

[Modul 29](29-QA-Testkatalog.md#7-welt-und-wirtschaft), [Modul 30](30-Abnahmekriterien.md#7-welt-und-wirtschaft).

## 15. Ungeklärtes

- Zeitmaßstab ([K15](19-Konflikt-und-Widerspruchsmatrix.md#k15-zeitmaßstab)).
- Zahl, Lage und Größe von Betrieben; Umfang des Spielers als Creator.
- Eigenkündigung und Vertragsdetails ([K6](19-Konflikt-und-Widerspruchsmatrix.md#k6-langzeitvertrag)).
- Jurisdiktionen der Welt und deren Gesetze.
- Wirtschaftsmodell (Preise, Löhne, Steuern).
- Wie weit das Internetsystem Adult-Seiten abbildet (Plattformregeln).
- Bordell-Matching-Website (K6).
