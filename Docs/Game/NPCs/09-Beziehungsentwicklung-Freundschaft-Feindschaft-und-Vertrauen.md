# NPCs 09: Beziehungsentwicklung – Freundschaft, Feindschaft und Vertrauen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie sich Beziehungen über die Zeit entwickeln und wie NPCs auf Handlungen des Spielers reagieren. Beschlossen sind die Handlungsarten, die Gedächtnisbindung und die Grundverläufe (Freundschaft, Vertrauensverlust, Trennung, Versöhnung); **Verlaufsmodelle, Reaktionstabellen und Eskalationsstufen sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Das Beziehungsmodell steht in [Modul 08](08-Beziehungen-Modell-und-Wirkung.md), die Feindschaft zu überlebenden Gegnern in [Modul 11](11-Nemesis-Modell-und-Datensatz.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Ereigniskatalog der Spielerhandlungen](#2-entwurf-ereigniskatalog-der-spielerhandlungen)
3. [Entwurf: Reaktionsbilder](#3-entwurf-reaktionsbilder)
4. [Entwurf: Verlauf einer Freundschaft](#4-entwurf-verlauf-einer-freundschaft)
5. [Entwurf: Verlauf von Vertrauensverlust und Feindschaft](#5-entwurf-verlauf-von-vertrauensverlust-und-feindschaft)
6. [Entwurf: Versöhnung, Vergebung und Wiederannäherung](#6-entwurf-versöhnung-vergebung-und-wiederannäherung)
7. [Entwurf: Gefallen, Schuld und Gegenseitigkeit](#7-entwurf-gefallen-schuld-und-gegenseitigkeit)
8. [Entwurf: Langzeitentwicklung ohne den Spieler](#8-entwurf-langzeitentwicklung-ohne-den-spieler)
9. [Entwurf: Gemeinsame Zeit als Beziehungsquelle](#9-entwurf-gemeinsame-zeit-als-beziehungsquelle)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-BV-001 | Beziehungen entstehen durch gemeinsame Erlebnisse, Gespräche, Hilfe, Zuverlässigkeit, Streit und Zeit | 📋 | `23#1088A` Frage 3; „1-3 ja“ in `23#1089E`; [Gameplay-Systeme](../Gameplay-Systeme.md) §8 |
| N-BV-002 | Hunter-NPCs: andere Hunter treffen → zusammenarbeiten → Vertrauen entsteht → Freundschaft oder Rivalität → später möglicherweise als Begleiter verfügbar | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-BV-003 | Hunter-NPCs können nicht nur helfen, sondern auch Rivalen werden, korrumpiert oder besessen werden oder dauerhaft sterben | 📋 | „Jäger-NPCs: können nicht nur helfen, sondern auch Rivalen werden, korrumpiert/besessen werden oder dauerhaft sterben“ in `19#4E` |
| N-BV-004 | Beziehung zu Begleitern beeinflusst, ob sie nachts mitkommen; sie erinnern sich an Rettung, Im-Stich-Lassen, Lüge, gemeinsame Fälle und Verletzungen wegen Entscheidungen | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-BV-005 | Wer als Hunter-Rekrut ins HQ kommt, wird nicht automatisch von allen akzeptiert; es entstehen Reaktionen der Crew, Konflikte und Vertrauensentwicklung; manche akzeptieren ihn nie ganz | 🧭 | `23#678A` (Abschnitt ‚Nicht jeder Recruit passt automatisch ins HQ‘, Vorschlag des Assistenten) |
| N-BV-006 | Vertrauensverlust, Untreue, Eifersucht, Streit, unterschiedliche Lebensziele und das Hunter-Leben können Beziehungen scheitern lassen | 📋 | `23#1088A` Frage 6; „6-9 ja“ in `23#1089E` |
| N-BV-007 | Partner reagieren auf Lebensstil: Verschwinden, Kriminalität, ständige Verletzungen | 📋 | `23#1088A` Frage 11; „11-15 genau“ in `23#1089E` |
| N-BV-008 | Der Spieler kann Mehrfachbeziehungen führen; ob sie geheim, offen oder akzeptiert sind, beeinflusst die Konsequenzen; Eifersucht, Streit, Vertrauensverlust und Trennung können entstehen | 📋 | `23#1090A` ‚Beziehungen & Sexualität‘; Details [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §3 |
| N-BV-009 | Zusammenziehen, Heirat und gemeinsame Immobilien sind möglich | 📋 | `23#1088A` Fragen 7–9; „6-9 ja“ in `23#1089E` |
| N-BV-010 | Missverständnisse können geklärt werden; falsche Anschuldigungen können Beziehungen beschädigen; ein tatsächlicher Betrug senkt Vertrauen stark, beendet aber nicht automatisch jede Beziehung | 📋 | `35#270A` Fragen 36–40, 59–60; „22-52 ja“ und „54-60 ja“ in `35#271E` |
| N-BV-011 | Eifersucht ist individuell; ein sehr eifersüchtiger NPC reagiert schon auf Flirten, ein wenig eifersüchtiger ignoriert vieles, solange vereinbarte Grenzen nicht überschritten werden | 📋 | `35#270A` Fragen 33–35; „22-52 ja“ in `35#271E` |
| N-BV-012 | Ein NPC kann Betrug auf verschiedene Weise herausfinden (selbst sehen, Freunde, Nachrichten, Gerüchte, Geständnis); die Sicherheit der Information spielt eine Rolle (Gerücht ≠ Beweis) | 📋 | `35#270A` Fragen 36–37; „22-52 ja“ in `35#271E` |
| N-BV-013 | Ein NPC kann den Spieler zu Unrecht verdächtigen; der Spieler kann versuchen, Missverständnisse aufzuklären | 📋 | `35#270A` Fragen 38–39; „22-52 ja“ in `35#271E` |
| N-BV-014 | Geheime Untreue kann langfristig unentdeckt bleiben oder Jahre später durch neue Informationen auffliegen | 📋 | `35#272A` Fragen 22–23; „1-50 passt alles“ in `35#273E` |
| N-BV-015 | Ein NPC kann nach Trennung eine neue Beziehung beginnen und behält Erinnerungen an den Ex; Ex-Beziehungen beeinflussen spätere Beziehungen indirekt; schlechte Behandlung macht vorsichtiger, gute erhöht Erwartungen; eine gute neue Beziehung kann alte negative Muster teilweise abbauen; das ist nicht garantiert | 📋 | `35#272A` Fragen 24–30; „1-50 passt alles“ in `35#273E` |
| N-BV-016 | Der Tod eines Partners kann langfristige Auswirkungen haben; Trauer ist unterschiedlich stark und lang; ein Charakter kann später neue Beziehungen eingehen | 📋 | `35#272A` Fragen 35–37; „1-50 passt alles“ in `35#273E` |
| N-BV-017 | Eine sehr lange Beziehung prägt stärker als viele kurze Begegnungen | 📋 | `35#272A` Frage 4; „1-50 passt alles“ in `35#273E` |
| N-BV-018 | NPCs bauen auch ohne den Spieler neue Beziehungen und ein neues Leben auf | 📋 | `35#272A` Frage 16; `35#274A`; „1-50 passt alles“ in `35#273E`; [Modul 02](02-Leitsaetze-und-Abgrenzung.md) N-LT-006 |
| N-BV-019 | Persönliche Item- und Gefallen-Wünsche von Crewmitgliedern (Buch, Werkzeug, Schallplatte, Ersatzteil, Waffe, Foto) führen nach Erfüllung zu persönlichem Dialog und Erinnerung; sie sind nicht wegen absurder Zeitfenster verpassbar | 🧭 | `23#606A` Abschnitt 5 (Vorschlag des Assistenten); `M-PD` ‚Acts, chapters and controlled story reactivity‘ (Crew personal requests) |
| N-BV-020 | Crew-Aktivitäten (gemeinsam trainieren, essen, Billard, Bar, Rennen, Schießstand, Angeln, Jagd, Kino, Roadtrip, Auto reparieren, Recherche, kleinere Jagd, einkaufen, Sport) formen Beziehung; sie sind nicht unnötig verpassbar | 🧭 | `23#603A` Abschnitt 3 (Vorschlag des Assistenten) |
| N-BV-021 | Road-Trip-Gespräche im Fahrzeug sind ein wichtiger Kanal der Charakterentwicklung und der Beziehung | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; ‚Smartphone / social media …‘ |
| N-BV-022 | Konsequenzen bei Fraktionen: Mehrfachmitgliedschaft ist möglich, aber nicht folgenlos (zum Beispiel zwei rivalisierende Clubs oder Polizei und Gang); große Folgen brauchen große Ursachen | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-BV-023 | Aufhören mit einer Gruppe: freiwillig, Rauswurf, Suspendierung, Verrat, Zerfall der Gruppe; die Vergangenheit hat sichtbare Folgen (zum Beispiel bleibt ein Tattoo) | 🧭 | `23#676A` (Vorschlag des Assistenten) |

---

## 2. Entwurf: Ereigniskatalog der Spielerhandlungen

Die Quellen nennen Beispiele von Spielerhandlungen, die NPCs erinnern (N-GE-001): *geholfen, bedroht, gerettet, bestohlen, verletzt, Fahrzeug beschädigt, bei einem Fall belogen* (`23#1056A` Frage 23). Der Katalog unten ordnet sie und ergänzt Kategorien (💡). Jede Kategorie ist ein **Ereignistyp** im Journal; die Wirkung hängt von Zeugen, Betroffenen und Deutung ab (Modul 06, 08).

| Kategorie | Beispiele | Hauptdimensionen | Betroffene |
|---|---|---|---|
| **Hilfe** | helfen, reparieren, retten, heilen, schützen, bezahlen | Vertrauen↑, Respekt↑, Schuld↑, Zuneigung↑ | Geholfener, Zeugen, Angehörige |
| **Rettung** | aus Gefahr holen, Leben retten, verletzten Begleiter bergen | Schuld↑↑, Bindung↑, Vertrauen↑ | Gerettete, Angehörige |
| **Bedrohung, Gewalt** | drohen, schlagen, erschießen, foltern, einschüchtern | Angst↑, Groll↑, Vertrauen↓ | Opfer, Zeugen, Familie |
| **Diebstahl, Sachschaden** | stehlen, Fahrzeug beschädigen, einbrechen | Vertrauen↓, Groll↑, Verdacht↑ | Geschädigter, Zeugen |
| **Täuschung** | lügen, fälschen, verkleiden, Identität verbergen | Vertrauen↓ bei Entdeckung, Verdacht↑ | Getäuschter, Dritte |
| **Ehrlichkeit** | Wahrheit sagen, Geheimnis anvertrauen, Fehler zugeben | Vertrauen↑, Vertrautheit↑ | Gesprächspartner |
| **Verrat, Bruch** | Absprache brechen, im Stich lassen, verraten | Vertrauen↓↓, Groll↑, Bindung↓ | Betroffener, Gruppe |
| **Treue, Zuverlässigkeit** | Wort halten, zurückkommen, beschützen | Vertrauen↑, Loyalität↑ | Betroffener |
| **Tötung** | jemanden töten (Gegner, Nahestehender, Zeuge) | Angst↑, Groll↑↑ bei Angehörigen, Rache | Angehörige, Gruppe, Behörde |
| **Verschonen** | Gegner leben lassen, Wesen verschonen | Respekt↑ oder Verachtung (je Persönlichkeit), Schuld↑ | Verschonter, Angehörige |
| **Anzeigen, Melden** | Polizei rufen, Zeugenaussage, Hinweise geben | Vertrauen↑ (Behörde), Groll↑ (Beschuldigter) | Betroffene |
| **Schweigen** | nicht melden, decken | Vertrauen↑ (Begünstigter), Verdacht↑ (Ermittler) | Betroffene |
| **Gerüchte** | Gerüchte streuen, widerlegen | Ruf, Verdacht | Kreise (Modul 07) |
| **Gemeinsames Erleben** | gemeinsame Fahrt, Mahlzeit, Recherche, Training | Vertrautheit↑, Zuneigung↑ | Beteiligte |
| **Nähe** | Zuwendung, Zeit, Geschenk | Zuneigung↑, Bindung↑ | Beteiligte |
| **Vernachlässigung** | lange nicht melden, wiederholt ignorieren | Vertrautheit↓, Groll (abhängig von Charakter) | Betroffene |
| **Verletzung zufügen durch Entscheidung** | jemand wird wegen der Wahl des Spielers verletzt | Groll oder Schuld je Deutung | Verletzter, Angehörige |
| **Fehlentscheidung im Fall** | falsche Anschuldigung, falscher Verdacht | Groll↑, Vertrauen↓ | Beschuldigter, Gemeinschaft |
| **Öffentliches Auftreten** | Heldentat, Skandal, Rettung vor Publikum | Respekt↑ oder Misstrauen, Bekanntheit | Gemeinschaft |
| **Amtliche Folgen** | Festnahme, Haft, Strafe | Vertrauen (Behörde), Ruf | Behörden, Gemeinschaft |

**Regeln (Entwurf):**

1. **Die Spalte ‚Dimensionen‘ ist eine Richtung, keine Wirkung.** Die tatsächliche Wirkung folgt aus Persönlichkeit, Zuschreibung und Kontext (Modul 08 §6).
2. **Kein Ereignistyp ist ‚gut‘ oder ‚schlecht‘.** Verschonen kann für einen Rache Suchenden Verrat sein; Gewalt kann für ein Opfer Schutz sein (N-LT-027).
3. **Ereignisse haben Gewicht:** Schwere, Nähe, Absicht, Öffentlichkeit, Reversibilität.
4. **Ereignisse können Folgeereignisse auslösen:** Wer bedroht wurde, meldet; wer verraten wurde, rächt; wer gerettet wurde, erzählt es weiter.

---

## 3. Entwurf: Reaktionsbilder

Die Quellen verlangen, dass verschiedene Persönlichkeiten unterschiedlich reagieren (N-PS-010). Die Reaktionsbilder zeigen **Verhaltensmuster**, nicht Berechnungen.

| Ereignis | Mutiger, loyaler NPC | Ängstlicher NPC | Misstrauischer NPC | Pragmatischer NPC |
|---|---|---|---|---|
| Wurde bedroht | stellt zur Rede, wehrt sich | meidet, zahlt, flieht, schweigt | meldet, beobachtet | verhandelt, rächt später |
| Wurde belogen | konfrontiert | zieht sich zurück | glaubt sofort nichts mehr | bewertet Nutzen |
| Wurde gerettet | dankbar, bietet Hilfe | dankbar, aber vorsichtig | prüft Motive | erwartet Gegenleistung |
| Wurde bestohlen | verfolgt | trauert, meidet | misstraut allen | rechnet Schaden |
| Sieht Gewalt | greift ein oder ruft Hilfe | verbirgt sich, schweigt | prüft, wer schuld ist | meidet Ärger |
| Sieht Verbrechen | meldet, hilft Opfern | schweigt, flieht | prüft Interessen | handelt nach Vorteil |
| Wurde verschont (Gegner) | Respekt oder Scham | Dankbarkeit, Angst | Verdacht auf Falle | Gelegenheit nutzen |
| Hört Gerücht | fragt nach | glaubt, ist verängstigt | überprüft | nutzt es |

**Zusätzliche Dimensionen der Reaktion (Entwurf):** Beruf (Polizist meldet; Barkeeper schweigt; Arzt behandelt), Alter, Familie (Verantwortung für Kinder macht vorsichtiger), Sucht, Ideologie, Zugehörigkeit (Gang hat Regeln), Lage (verletzt, in Not).

**Darstellung:** Das Bild entsteht aus **Verhalten** (Animation, Dialog, Ort, Handlung) und nicht aus einer Nachricht (‚NPC ist wütend‘).

---

## 4. Entwurf: Verlauf einer Freundschaft

Aus N-BV-001 und N-BV-002 folgt ein typischer Verlauf. Er ist kein Skript, sondern ein **Zustandspfad**, den Ereignisse tragen (💡).

```text
Begegnung ──► Bekanntschaft ──► regelmäßiger Kontakt ──► gemeinsames Erlebnis ──► Vertrauen
                                                                     │
                                         Prüfung (Konflikt, Gefahr, Schuld, Lüge)
                                                    │
                      ┌─────────────────────────────┴───────────────────────────┐
                      ▼                                                         ▼
                 Vertiefung ──► Freundschaft ──► enge Bindung            Bruch ──► Distanz ──► früher
```

| Station | Merkmal | Typische Ereignisse |
|---|---|---|
| Bekanntschaft | kennt Namen, erkennt wieder | Gespräche, Gemeinsamkeiten |
| regelmäßiger Kontakt | wiederholt Treffen | Stammlokal, Arbeitsplatz, Nachbarschaft |
| gemeinsames Erlebnis | prägender Moment | Fall, Gefahr, Fahrt, Fest |
| Vertrauen | Offenheit, Geheimnisse | Anvertrauen, Hilfe |
| Prüfung | Beziehung wird belastet | Entscheidung, die Kosten verursacht |
| Vertiefung | geteilte Last | gemeinsame Gefahr, Schuld |
| Freundschaft/enge Bindung | Priorität | Zusagen, Opfer |
| Bruch | Verrat, Streit | Lüge, Verlassen |

**Wichtig:** Es gibt keine ‚Freundschaftsstufe‘ als Spielmechanik (kein Freischalten durch Level). Konsequenzen sind konkrete Ereignisse (Zugang, Hilfe, Begleitung).

**Zeit:** Freundschaft braucht Weltzeit und Kontakt. Wer nur Quests abarbeitet, baut keine auf; wer Zeit teilt, schon (N-BE-005).

**Wiederkehrende Kontakte:** Der Spieler kann Menschen nach Jahren wiedertreffen; die Beziehung hat sich verändert (N-GE-013, N-GE-017).

---

## 5. Entwurf: Verlauf von Vertrauensverlust und Feindschaft

```text
Anlass (Verrat, Gewalt, Lüge, Schaden)
   │
   ▼
Erkenntnis:  Weiß der Betroffene davon? (Modul 06)
   │ ja
   ▼
Deutung:  Absicht oder Zufall?  Wer ist schuld?
   │
   ├─ Zufall / entschuldbar ──► Enttäuschung ──► Klärung ──► Reparatur
   │
   └─ Absicht / schwer ──► Groll ──► Distanz ──► Meidung ──► Gegnerschaft ──► Feindschaft
                                         │
                                         └─► Rache (Handlungsabsicht, Modul 11)
```

**Feindschaft als Zustand (Entwurf).** Eine Feindschaft ist eine Kante mit hohem Groll, niedrigem Vertrauen und einer **Absicht** (meiden, schädigen, vernichten, bloßstellen). Die Absicht gehört zu den Zielen des NPC (Modul 13). Die Quellen nennen Feindschaft als Beziehungsform (`35#272A` Fragen 44–45; `PE-N1022` C: ‚persönliche Feindschaften‘); sie legen nicht fest, wann sie zu aktiver Gegnerschaft wird.

| Stufe der Gegnerschaft (Entwurf) | Merkmal | Beispiel |
|---|---|---|
| Abneigung | meidet, redet schlecht | Nachbar, Kollege |
| Behinderung | verweigert Hilfe, verbreitet Gerüchte | Rivale |
| Schädigung | stört, schadet, meldet | Rache durch Anzeige |
| Verfolgung | sucht den Betroffenen | Vergeltung, Fahndung |
| Vernichtung | will töten oder ruinieren | Nemesis (Modul 11, 12) |

**Eskalation und Deeskalation.** Beides geschieht durch Ereignisse. Der Entwurf lässt die Eskalation von drei Dingen abhängen: Schwere des Anlasses, Persönlichkeit (Rachsucht, Mut, Mittel), Mittel (Geld, Waffen, Freunde, Fraktion). Eine Stufe wird nicht ohne neue Ereignisse höher (N-LT-023).

**Feindschaft ist nicht automatisch Gewalt.** Der Großteil der Feindschaften verläuft sozial: Gerüchte, Anzeige, Boykott, Intrigen (Modul 07). Gewalt ist eine Option für Menschen mit entsprechenden Mitteln und Persönlichkeit.

---

## 6. Entwurf: Versöhnung, Vergebung und Wiederannäherung

Die Quellen sagen: Vergebung hängt von Persönlichkeit, Schwere, Zeit und späterem Verhalten ab; manche Dinge verzeiht ein NPC nie; Beziehungen können erneut entstehen und verlaufen anders (N-BE-013, N-BE-014).

| Faktor | Wirkung (Entwurf) |
|---|---|
| Schwere | je schwerer, desto langsamer und unwahrscheinlicher |
| Absicht | unabsichtlich: leichter; absichtlich: schwerer |
| Zeit | verdünnt Groll; heilt nicht automatisch |
| Verhalten danach | Reue, Wiedergutmachung, Konsequenz |
| Persönlichkeit | Nachtragen ↔ Großzügigkeit |
| Geschichte | lange gute Erfahrung erleichtert; frühere Verfehlungen erschweren |
| Dritte | Vermittler, Gruppendruck |
| ‚Niemals‘ | Dinge, die ein NPC nie verzeiht (zum Beispiel Mord an Angehörigen), sind Teil seines Kerns |

**Mögliche Wege:** Entschuldigung, Wiedergutmachung, Zeit, Vermittlung, gemeinsames Ereignis, äußere Not. Der Spieler kann auch **nicht** versöhnen wollen.

**Wiederannäherung.** Eine frühere Beziehung kann sich neu entwickeln (N-BE-014). Sie startet nicht beim alten Stand, sondern aus der aktuellen Lage beider (N-GE-017).

---

## 7. Entwurf: Gefallen, Schuld und Gegenseitigkeit

Gefallen sind in den Quellen eine Wirkung von Beziehung (‚favors‘, N-BE-002) und eine Dialogfolge (Sheriff-Beispiel, `23#51A`). Ein einfaches Modell (💡):

| Begriff | Bedeutung | Zustand |
|---|---|---|
| **Gefallen** | konkrete Handlung mit Wert für den anderen | Ereignis im Journal |
| **Schuld (Debt)** | A fühlt sich B verpflichtet | Dimension der Kante |
| **Einlösung** | A tut etwas für B | Ereignis; Schuld sinkt |
| **Zinsen, Druck** | Schuld wird eingefordert | Dialog, Drohung |
| **Verjährung** | Schuld wird weniger wichtig | Zeit, Persönlichkeit |

**Regeln (Entwurf):**

1. **Schuld wirkt unterschiedlich:** Ein loyaler NPC löst ein; ein pragmatischer fragt nach dem Wert; ein stolzer will schnell quitt sein.
2. **Schuld kann verkauft oder vererbt werden** (Familie, Gruppe, Organisation) – Option für Fraktionen (Modul 16).
3. **Der Spieler kann Schuld bewusst aufbauen** (Gefallen tun, um Zugang zu bekommen). NPCs durchschauen Berechnung eher bei misstrauischem Kern.

---

## 8. Entwurf: Langzeitentwicklung ohne den Spieler

Anforderung F (N-LT-006) und `35#272A` Frage 16 (‚inzwischen andere Beziehungen aufgebaut‘): NPC-Beziehungen verändern sich im Hintergrund. Das System braucht dafür **Ereignisquellen**, die nicht vom Spieler stammen:

| Quelle | Beispiele |
|---|---|
| Alltag | neue Kollegen, Nachbarn, Vereine |
| Lebensereignisse | Heirat, Geburt, Umzug, Jobwechsel, Krankheit, Tod (Modul 14) |
| Konflikte | Streit, Betrug, Eifersucht, Schulden |
| Weltereignisse | Katastrophen, Kriminalität, Stadtentwicklung (Modul 15) |
| Fraktionen | Beitritt, Austritt, Konflikte (Modul 16, 17) |
| Zufall (deterministisch) | Begegnungen, Missgeschicke |

**Prinzipien (Entwurf):**

1. **Wahrscheinlichkeitsmodell, kein Drehbuch.** Je Zeitschritt wird für jeden relevanten NPC bestimmt, ob ein Ereignis eintritt; Eintrittswahrscheinlichkeit folgt Persönlichkeit, Lage und Umgebung. Der Zeitschritt ist grob (Tage bis Wochen) für Ferne.
2. **Sparsamkeit.** Nur Ereignisse mit Wirkung auf Zustand werden gespeichert (Verdichtung, Modul 05).
3. **Nachvollziehbarkeit.** Jede Änderung hat eine Ursache im Journal (N-PS-016, N-EN-021).
4. **Schutz.** Geschützte NPCs unterliegen Regeln gegen triviale Unfälle (Modul 03 §8).
5. **Konsistenz mit Schauplätzen.** Wer in der Ferne umzieht, ist an anderem Ort; Beziehungen zu Zurückbleibenden werden zu Fernbeziehungen.
6. **Spielerbezug bleibt:** Veränderungen des Lebens eines NPCs, der dem Spieler wichtig ist, sollen für den Spieler auffindbar sein (Telefon, Besuche, Nachrichten), nicht heimlich verschwinden.

---

## 9. Entwurf: Gemeinsame Zeit als Beziehungsquelle

Die Quellen betonen, dass nicht nur dramatische Ereignisse, sondern **gemeinsame Zeit** Beziehung formt (Stille, Autofahrt, Fernsehen, Lagerfeuer, Recherche, Rückfahrt; N-BE-005). Das ist ein Kernbaustein für Companions und HQ-Leben. Der Entwurf:

| Kontext | Beitrag |
|---|---|
| Fahrt (Road Trip) | Gespräche, Musik, Schweigen; langfristige Charakterentwicklung (N-BV-021) |
| HQ-Leben | Zusammen essen, trainieren, feiern; Crew handelt eigenständig (`23#603A` Abschnitt 2, 💡) |
| Aktivität | Billard, Bar, Angeln, Jagd, Schießstand, Sport, Kino (N-BV-020) |
| Recherche | gemeinsames Lesen, Diskussion, Erfolg |
| Not | gemeinsame Gefahr, Verletzung, Rettung |
| Alltag | Telefonate, Nachrichten, Besuche |

**Qualität zählt:** Zufälliges Beisammensein reicht nicht; Verhalten, Gespräch und Aufmerksamkeit bestimmen (N-BE-005).

**Nicht verpassbar:** Aktivitäten verschwinden nur aus Storygründen, nicht wegen Zeitfenstern (N-BV-020, N-LT-035).

**Bewusste Pausen.** Wer lange nicht präsent war, hat weniger Vertrautheit (N-BE-012); die Beziehung kann sich lockern, aber nicht automatisch zerbrechen (Modul 08).

---

## 10. Sonderfälle

1. **Tod eines Freundes durch Entscheidung des Spielers.** Reaktion hängt von Zuschreibung (Absicht? Notwehr? Zwang?), Beziehung und Persönlichkeit ab; Rache möglich.
2. **Mehrere Beziehungen kollidieren.** Wer mit zwei verfeindeten Gruppen verbunden ist, steht in Konflikt (N-BV-022).
3. **Freund eines Feindes.** Der Spieler tötet einen Feind; dessen Freund wird zum Feind (Racheketten, Modul 11, 17).
4. **Falscher Verdacht.** Ein NPC hält den Spieler zu Unrecht für Täter; die Kante bricht trotz Unschuld (N-BV-013).
5. **Erpressung durch Schuld.** Missbrauch von Schuld/Angst (Abschnitt 7).
6. **Freundschaft zu einem Wesen.** Ein verschontes Wesen führt ein normales Leben; die Beziehung zur Umwelt kann scheitern, wenn die Wahrheit herauskommt ([Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §3).
7. **Selbstzerstörerische NPCs.** Sucht, Verschuldung, Gewalt: Beziehungen leiden ohne Zutun des Spielers (Modul 04).
8. **Wiedersehen nach Jahren.** Die Beziehungslage ist nicht eingefroren (N-GE-017).
9. **Beziehungen zu Eltern, Kindern, Geschwistern des Spielercharakters.** Die Quellen führen die Herkunft des Spielercharakters nicht; offen (Q-N17).
10. **Beziehungen im Koop.** Je Spieler eigene Kanten (Q-N15).
11. **Mod-Beziehungstypen.** Erweiterbar, ohne den Kern zu ändern (Modul 23).

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-131 | Jede Änderung einer Beziehungsdimension hat ein Ereignis, das der Beteiligte kennt | N-BE-001, N-GE-008 |
| N-EN-132 | Vergebung ist nicht allein von der Zeit abhängig | N-BE-013 |
| N-EN-133 | Ein NPC verzeiht, was in seinem Kern ‚nie verziehen‘ ist, nicht | N-BE-013 |
| N-EN-134 | Eine Feindschaft eskaliert nur durch ein Ereignis | N-LT-023 |
| N-EN-135 | Eine falsche Anschuldigung kann eine Beziehung beschädigen, ohne dass die Wahrheit sich ändert | N-BV-010, N-BV-013 |
| N-EN-136 | NPC-NPC-Beziehungen verändern sich im Hintergrund mit einer Ursache im Journal | N-BV-018 |
| N-EN-137 | Die Welt zählt Beziehungsereignisse nicht als ‚Punkte‘; es gibt kein Freischalten durch Beziehungsstufe | Abschnitt 4 |
| N-EN-138 | Ein verpasstes Zeitfenster entfernt keine Beziehungsaktivität | N-BV-020, N-LT-035 |
| N-EN-139 | Ein Ereignis wirkt auf Betroffene, Zeugen und Dritte getrennt und deterministisch | Abschnitt 2 |
| N-EN-140 | Ein Verhaltensbild (Reaktionstabelle) hängt von Persönlichkeit, Beruf und Lage ab, nicht von einem Zufallswurf | Abschnitt 3 |
| N-EN-141 | Schuld sinkt durch Einlösung und kann nicht ohne Ereignis verschwinden | Abschnitt 7 |
| N-EN-142 | Eine Fernbeziehung bleibt eine Beziehung; ihre Pflege geschieht durch Kontakt | Abschnitt 8 |

---

## 12. Was fehlt

- Keine Verlaufskurven, Schwellen oder Wahrscheinlichkeiten.
- Offen, wie Rekruten und Crew ihre Beziehung zueinander entwickeln, außer als Vorschlag (N-BV-005).
- Offen, was mit der Herkunft des Spielercharakters (Familie, Freunde) ist (Q-N17).
- Offen, wie die Beziehung zu Mod-NPCs gesichert wird (Modul 23).
- Keine Quelle zu ‚Mord an Angehörigen‘ als Kernkonstante; es ist ein Entwurfsbeispiel.
