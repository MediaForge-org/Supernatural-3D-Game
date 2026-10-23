# NPCs 12: Nemesis – Lebenszyklus, Rückkehr, Verletzung und Anpassung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt den zeitlichen Ablauf einer Feindschaft: von der Begegnung über Folgen und Erholung bis zur Rückkehr, Eskalation, Versöhnung oder zum Tod. Verbindlich ist die Produktanforderung C (`PE-N1022`) und die Regeln der Kreaturenquellen (Flucht, Rückzug, Wiederkehr, Narben). **Der Ablauf, die Auslöser und die Beispiele sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Das Datenmodell steht in [Modul 11](11-Nemesis-Modell-und-Datensatz.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Der Lebenszyklus im Überblick](#2-entwurf-der-lebenszyklus-im-überblick)
3. [Entwurf: Ausgänge einer Begegnung](#3-entwurf-ausgänge-einer-begegnung)
4. [Entwurf: Persistente Verletzungen und ihre Folgen](#4-entwurf-persistente-verletzungen-und-ihre-folgen)
5. [Entwurf: Erholung und Vorbereitung im Hintergrund](#5-entwurf-erholung-und-vorbereitung-im-hintergrund)
6. [Entwurf: Rückkehr](#6-entwurf-rückkehr)
7. [Entwurf: Anpassungsstufen ohne Level](#7-entwurf-anpassungsstufen-ohne-level)
8. [Entwurf: Wendepunkte – Versöhnung, Bündnis, Aufgabe](#8-entwurf-wendepunkte--versöhnung-bündnis-aufgabe)
9. [Entwurf: Tod des Nemesis und seine Folgen](#9-entwurf-tod-des-nemesis-und-seine-folgen)
10. [Entwurf: Beispielabläufe](#10-entwurf-beispielabläufe)
11. [Entwurf: Mehrere Feinde gleichzeitig](#11-entwurf-mehrere-feinde-gleichzeitig)
12. [Sonderfälle](#12-sonderfälle)
13. [Entwurf: Prüfregeln](#13-entwurf-prüfregeln)
14. [Was fehlt](#14-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-NL-001 | Monster müssen nicht bis zum Tod kämpfen; sie fliehen, wechseln den Bau oder das Opfer, verstecken sich, suchen Verstärkung | 📋 | `23#293A` Punkt 13; „13. ja das sowieso“ in `23#294E` |
| N-NL-002 | Verletzte Monster verstecken sich, erholen sich und tauchen später wieder auf; manche beobachten den Spieler und greifen später an | 📋 | `23#1074A` Punkte 58–59; „55-60 ja“ in `23#1075E` |
| N-NL-003 | Ein Wesen, das nur vertrieben wurde oder entkam, kann später mit Erinnerung und früheren Verletzungen auftauchen; es muss nicht | 📋 | `23#294E`: „es kann auftauchen aber es muss nicht“ |
| N-NL-004 | Der Spieler entscheidet, ob er ein Wesen tötet oder freilässt; Ausgänge reichen von töten, vertreiben, bannen, fangen, heilen, Fluch brechen bis verhandeln und retten | 📋 | `23#293A` Punkt 15; `23#294E` Punkt 15; `M-PD` ‚Story and cases‘ |
| N-NL-005 | Gefangene Wesen können verhört und verwahrt werden; Verhandeln und Kooperation sind möglich | 📋 | `48#129E`; [Kreaturen 13](../Kreaturen/13-Lebendfang-Fallen-und-Containment.md) |
| N-NL-006 | Bleibende Verletzungen und Narben von Wesen bleiben; Regeneration ist artabhängig | 📋 | [Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §5–§6 |
| N-NL-007 | Bei Menschen: Verletzte NPCs werden behandelt (EMS, Krankenhaus) und genesen; schwere Verletzungen haben bei wichtigen NPCs dauerhafte Folgen (große Narben, verlorene Hand) | 📋 | `23#1056A` Fragen 71–72, 100–101; „ja sicher, große narben, eine hand verloren, alles soll es geben“ in `23#1057E` |
| N-NL-008 | Menschliche Gegner können fliehen, erstarren oder sich ergeben | 📋 | `M-PD` ‚Multiple-opponent combat …‘ |
| N-NL-009 | Safehouses sind nicht absolut sicher; bestimmte sehr seltene Gegner können Spielerstandorte herausfinden; selten | 📋 | `23#1074A` Punkt 61; Antwort in `23#1075E` |
| N-NL-010 | Gegner kehren verändert zurück (Narben, besser vorbereitet, mit Verbündeten, wissend, was gewirkt hat) | 📋 Richtung | `PE-N1022` C; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §3 (Zielbild, 💡) |
| N-NL-011 | Die Weltzeit läuft nur während des Spielens; Zeitsprünge (Schlaf, Reise, Haft) werden von der Hintergrundsimulation ereignisbasiert nachgeholt | 📋 | `35#279E` Punkt 11 und 12–35; [Ermittlung 19](../Ermittlung/19-Weltzustand-und-Persistenz.md) §8 |
| N-NL-012 | Festnahme des Spielers ist kein Game Over; Folgen sind Verhör, Geldstrafe, Kaution, Gerichtsverfahren, Haft, Zeitsprung | 📋 | `23#1058A` ‚Festnahme ≠ Game Over‘; „68. ja genau, game over soll es nicht geben, das wäre bei so einem spiel zu brutal“ in `23#1057E` |
| N-NL-013 | Der genaue Ablauf des Spielertods bzw. des Scheiterns ist offen | ❓ | `M-PD` ‚Death / failure — OPEN‘ |
| N-NL-014 | Hauptbedrohungen überleben mehrere Begegnungen, entwickeln Strategien und treten über Akte auf | 📋 | `M-PD` ‚Long-term story threats‘ |
| N-NL-015 | Wiederkehrende Rivalen: Kämpfer der Kampfszene, Trainer, Reputation, Kontakte | 💡 | `23#164A`, `23#166A` |

---

## 2. Entwurf: Der Lebenszyklus im Überblick

```text
                      ┌────────────────────────────────────────────────────────┐
                      │                                                        │
 Begegnung ──► Ausgang ──► Folgen ──► Erholung/Planung ──► Gelegenheit ──► Rückkehr
     │            │           │              │                  │              │
     │            │           │              │                  │              ▼
     │            │           │              │                  │         Konfrontation ──► Ausgang ...
     │            │           │              │                  │
     │            │           └─ Verletzungen, Verluste, Wissen, Emotionen
     │            └─ tot | geflohen | verborgen | gefangen | verschont | übergelaufen | unbemerkt
     └─ erste Begegnung mit dem Spieler (oder seinem Umfeld)

 Seitenwege:   Versöhnung ─ Bündnis ─ Aufgabe ─ Verfolgung durch Dritte ─ natürlicher Tod
```

| Phase | Bedeutung | Ort der Simulation |
|---|---|---|
| **Begegnung** | Konfrontation; alles Gesehene wird Beobachtung | voll (Sim 0/1) |
| **Ausgang** | Ergebnis der Konfrontation | Ereignis im Journal |
| **Folgen** | Verletzungen, Verluste, Emotionen, Wissen | Datensatz |
| **Erholung, Planung** | Heilung, Beschaffung, Rekrutierung, Beobachtung | Hintergrund (Sim 2/3) |
| **Gelegenheit** | Ort, Zeit, Voraussetzungen für Rückkehr | Weltzustand |
| **Rückkehr** | Auftritt in Spielernähe: Hinterhalt, Konfrontation, Nachricht, Folge | voll |
| **Konfrontation** | neue Begegnung | voll |

Der Zyklus wiederholt sich, solange der Nemesis lebt und Motiv, Mittel und Gelegenheit hat. **Jede Phase hat Ereignisse im Journal** (N-EN-211).

---

## 3. Entwurf: Ausgänge einer Begegnung

Die Quellen nennen mehrere Ausgänge (N-NL-001 bis N-NL-005, N-NL-008). Der Entwurf ordnet sie nach ihrer Wirkung auf den Nemesis-Datensatz:

| Ausgang | Beschreibung | Folge für den Datensatz | Folge für die Welt |
|---|---|---|---|
| **Getötet** | Gegner stirbt | `dead`; Anker und Erinnerungen anderer bleiben (Modul 05) | Gruppe, Familie, Gebiet reagieren (Modul 10, 17) |
| **Geflohen (sichtbar)** | Gegner entkommt vor Augen | `candidate` → `recovering` | Spuren, Zeugen, Gerüchte |
| **Verborgen (verletzt)** | Gegner versteckt sich | `recovering` | unsichtbar; Spuren im Gelände |
| **Gefangen** | Gegner in Gewahrsam (Spieler, Behörde, Hunter) | `captured` | Verhör, Verwahrung (Kreaturen 13) |
| **Verschont** | Spieler lässt ihn gehen | `candidate`; Schuld oder Verachtung | Beziehungskante verändert sich |
| **Übergelaufen** | Gegner wechselt die Seite | `allied` / `turncoat` | neue Beziehungen, Verrat an der Gruppe |
| **Entkommen (unbemerkt)** | Spieler weiß nicht, dass er überlebte | `candidate` | Gerücht, Hinweis später |
| **Besiegt, geheilt** | Besessener oder Verfluchter wird geheilt | `redeemed` | Rückkehr in normales Leben |
| **Vertrieben** | Wesen wird aus Gebiet getrieben | `recovering` + Gebietsverlust | Territorium verändert sich (Modul 17) |
| **Spieler unterliegt** | Spieler flieht, wird gefangen oder verletzt | Nemesis gewinnt Wissen | Folgen für den Spieler offen (Q-N10) |
| **Dritte greifen ein** | Polizei, Hunter, Rivalen | je nach Ausgang | Weltzustand ändert sich ohne den Spieler |

**Regeln (Entwurf):**

1. **Der Ausgang entscheidet nicht allein.** Entscheidend ist, was der Gegner **erfährt** und **verliert** (Modul 11 §5).
2. **Unbeobachtete Ausgänge sind erlaubt.** Wenn niemand sieht, dass der Gegner überlebte, erfährt der Spieler es später (Hinweise).
3. **Der Spieler wählt nicht immer.** Der Ausgang folgt aus der Lage; die Wahl (töten oder freilassen) gibt es nur, wo sie im Kampf entsteht (Aufgabe, Verwundung, Kapitulation: `M-PD` ‚…surrender‘).
4. **Kapitulation.** Ein Gegner, der sich ergibt, bleibt dem Spieler ausgeliefert. Das ist eine Gelegenheit zur Entscheidung (töten, fesseln, übergeben, laufen lassen), die Folgen für Ruf und Nemesis hat (Modul 09 §5).

---

## 4. Entwurf: Persistente Verletzungen und ihre Folgen

**Quellen:** Verletzungen und Narben bleiben (N-NL-006, N-NL-007). Artabhängige Regeneration ([Kreaturen 04](../Kreaturen/04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §5), Ausfälle funktionaler Körperteile (Kreaturen 04 §3).

**Entwurf – Wirkungen auf einen Nemesis:**

| Verletzungsart | Wirkung (Entwurf) | Sichtbar für Spieler |
|---|---|---|
| **Narbe** | bleibt; Tarn- und Wiedererkennungsmerkmal | Gesicht, Hände, Hals |
| **Verlorenes Glied** | Waffenhand, Gangbild, Taktik ändern sich; Prothese möglich | deutlich |
| **Gehbehinderung** | langsamer, meidet Verfolgung; bevorzugt Fahrzeuge, Hinterhalt | Gangbild |
| **Sinnesverlust** | Auge, Gehör; Taktik verlagert sich | Augenklappe, Hörgerät |
| **Chronischer Schmerz** | Stimmung, Reizbarkeit, Substanzgebrauch | Verhalten |
| **Trauma, Angst** | Furcht vor Waffe, Ort oder Person | Verhalten |
| **Funktionsverlust bei Wesen** | artabhängig: Regeneration, Fähigkeit, Tarnung | artspezifisch |
| **Heilung** | Verletzung heilt, Narbe bleibt | zeitlich |

**Regeln (Entwurf):**

1. **Verletzung ändert das Verhalten, nicht nur die Optik.** Ein Gegner mit zerstörtem Bein jagt anders (Hinterhalt statt Verfolgung).
2. **Verletzung kann Motiv verstärken** (Rache) **oder abbauen** (Angst).
3. **Heilung braucht Zeit und Mittel.** Das Zeitfenster der Erholung (Abschnitt 5) hängt von Schwere, Medizin, Mitteln und Art ab. Offen, wie lange (Q-N19).
4. **Prothesen, Hilfsmittel.** Menschen können Prothesen erhalten; Wesen artabhängig.
5. **Die Verletzung ist eine Spur.** Sie führt zu Fundorten (Krankenhaus, Arzt, Händler), die der Spieler untersuchen kann (Ermittlung 05).
6. **Menschen suchen Ärzte.** Verletzte Menschen tauchen in Notaufnahmen oder bei illegalen Ärzten auf; Polizei und Ermittler registrieren das (Welt 12, Ermittlung 06).

**Zeitreise- und Regenerationsausnahmen** (Wesen, die vollständig regenerieren) folgen den Artregeln, nicht diesem Modul.

---

## 5. Entwurf: Erholung und Vorbereitung im Hintergrund

Während der Spieler anderswo ist, entwickelt sich der Nemesis weiter (Anforderung F, G). Das passiert in der **Hintergrundsimulation** (Sim 2/3, Modul 22), mit groben Zeitschritten:

| Aktivität | Ergebnis (Entwurf) | Dauer (offen) |
|---|---|---|
| **Heilen** | Verletzungen sinken; Narben bleiben | Tage bis Monate |
| **Verstecken, Fliehen** | neuer Ort, neues Gebiet | Tage |
| **Aufklären** | beobachtet den Spieler, sammelt Wissen über Wege, Orte, Nahestehende | Wochen |
| **Beschaffen** | Waffen, Fahrzeuge, Geld, Material | Wochen |
| **Rekrutieren** | Verbündete, Handlanger, Informanten (Modul 16) | Wochen bis Monate |
| **Planen** | wählt Ziel, Ort, Zeit, Taktik | abhängig |
| **Abwarten** | wartet auf Gelegenheit oder auf Heilung | beliebig |
| **Gebiet verlassen** | zieht um | Tage |
| **Machtkampf** | verliert oder gewinnt Rang in der Gruppe | Wochen |

**Prinzipien (Entwurf):**

1. **Deterministisch.** Aus Nemesis-Seed, Zeitfenstern und Weltzustand (Modul 22).
2. **Begrenzt.** Pro Zeitschritt wenige Aktionen; kein Parallel-Alles.
3. **Konsistent mit der Welt.** Er kann nicht ohne Geld Waffen kaufen, nicht ohne Weg reisen, nicht ohne Verbindung Kontakte nutzen.
4. **Spuren.** Hintergrundaktivität erzeugt Spuren (Käufe, Gerüchte, Sichtungen), die der Spieler finden kann (Vorzeichen, Modul 11 §11).
5. **Konflikt mit anderen.** Er kann in Konflikte mit anderen Fraktionen geraten und dabei verlieren (Modul 17).
6. **Rückholbarkeit.** Beim Laden einer Region wird der Nemesis aus dem letzten Zustand bis jetzt fortgeschrieben (Modul 22 §3).

---

## 6. Entwurf: Rückkehr

Die Quellen sagen nur, dass er zurückkehren **kann** („es kann auftauchen aber es muss nicht“). Der Entwurf legt Bedingungen und Formen fest, damit das Verhalten nachvollziehbar ist (💡).

### 6.1 Rückkehrbedingungen

| Bedingung | Beschreibung |
|---|---|
| **Motiv besteht** | Rache, Groll, Auftrag, Interesse ist nicht erloschen |
| **Verletzungen geheilt** oder Kompensation vorhanden | der Nemesis kann handeln |
| **Mittel vorhanden** | Verbündete, Waffen, Verstecke |
| **Wissen vorhanden** | er weiß, wo und wie (Modell, Modul 11 §6) |
| **Gelegenheit** | der Spieler ist erreichbar, Ort und Zeit passen (Weltzustand) |
| **Auftrittsbudget** | Mindestabstand erreicht; nicht zu viele Nemesis aktiv (Q-N09) |
| **Keine Erzählblockade** | Story-Sequenzen und Hauptmissionen werden nicht gestört |
| **Artregeln** | das Wesen darf auftreten (Tageszeit, Mond, Ort) |

### 6.2 Rückkehrformen

| Form | Beschreibung | Beispiel (abstrakt) |
|---|---|---|
| **Direkte Konfrontation** | steht vor dem Spieler | Begegnung in der Stadt |
| **Hinterhalt** | wartet am Ort | Straße, Haus, Fahrzeug |
| **Beobachtung** | verfolgt den Spieler, bevor er zuschlägt | Spuren, Anrufe, Bemerkungen |
| **Indirekt** | trifft Nahestehende, Besitz, Verbündete, Ruf | Haus, Fahrzeug, Begleiter, Bekannte |
| **Botschaft** | Drohung, Warnung, Einladung | Brief, Anruf, Zeichen |
| **Täuschung** | getarnt oder als Verbündeter | Tarnidentität (Ermittlung 13) |
| **Vermittler** | schickt Handlanger | Menschen als Werkzeuge (Kreaturen 09 §6) |
| **Bündnis** | tritt mit unerwarteter Hilfe auf | gemeinsamer Feind |
| **Befreiung** | kehrt aus Haft/Containment zurück | Ausbruch (Ermittlung 16) |
| **Reappearance nach Jahren** | verändert, älter, anderer Name | Zeitsprung |
| **Verfolgung durch Behörde** | nutzt Polizei und Recht | Anzeige, Fahndung |
| **Fraktionsangriff** | Gruppe greift Orte des Spielers an | HQ nicht dauerhaft sicher (N-NL-009) |

### 6.3 Wahrscheinlichkeit und Taktung (Entwurf)

1. **Kein fester Takt.** Die Rückkehr hat Zufall (Seed), Bedingungen und ein Zeitfenster; „es muss nicht“ (N-NL-003).
2. **Gewichtung.** Je größer Motiv, Mittel und Wissen, desto früher und gezielter; je mehr Angst und Verluste, desto später oder nie.
3. **Taktung begrenzt** durch Auftrittsbudget und Mindestabstände (Modul 11 §11).
4. **Kein Schwellenalarm.** Es gibt keinen Countdown und keine Ansage.
5. **Orte.** Die Rückkehr geschieht an Orten, die für den Nemesis zugänglich und für den Spieler plausibel sind.

---

## 7. Entwurf: Anpassungsstufen ohne Level

Die Quellen verbieten ein Hochzählen von Stärke (N-NE-017) und verlangen Anpassung des Verhaltens. Eine **Anpassungsstufe** ist daher keine Zahl, sondern ein Satz von **vorgenommenen Anpassungen**, geordnet nach Art (💡):

| Art der Anpassung | Beispiel (abstrakt) | Voraussetzung im Modell |
|---|---|---|
| **Vorbereitung** | trägt Schutz gegen die bekannte Methode | Methode beobachtet (Modul 11 §6) |
| **Aufklärung** | sammelt Informationen vor dem Angriff | Zeit, Gelegenheit |
| **Gelände** | wählt Ort und Zeit | Wissen über Gewohnheiten |
| **Verstärkung** | kommt mit Verbündeten | Mittel (Modul 16) |
| **Taktikwechsel** | meidet die Stärke des Spielers | Erfahrung |
| **Zielwechsel** | greift Schwächen im Umfeld an | Wissen über Nahestehende |
| **Täuschung** | Scheinrückzug, Köder, falsche Spuren | Intelligenz; Artregeln |
| **Ausnutzen** | nutzt Verletzung, Nachladen, Müdigkeit | Beobachtung |
| **Rückzug** | gibt Boden auf, wenn die Lage schlecht ist | Angst, Verluste |
| **Informationskrieg** | Gerüchte, Anzeige, Rufschädigung | soziale Mittel (Modul 07) |
| **Stellvertreter** | schickt andere | Macht, Geld, Einfluss |
| **Ausweichen** | meidet den Spieler | Angst, Respekt |

**Regeln (Entwurf):**

1. **Jede Anpassung ist Ereignis.** Sie hat eine Ursache und wird im Datensatz geführt (N-EN-183).
2. **Anpassungen sind umkehrbar.** Wer sich auf eine Methode eingestellt hat, kann überrascht werden, wenn der Spieler wechselt.
3. **Anpassungen kosten.** Schutz braucht Material und Geld; Verbündete brauchen Bezahlung oder Loyalität; Aufklärung braucht Zeit.
4. **Es gibt keinen Gesamtwert ‚Anpassung = 5‘.** Auch nicht intern; die Menge der Anpassungen bleibt Daten.
5. **Spieler kann dagegen arbeiten:** Köder, Fehlinformation, Methodenwechsel, Schutz des Umfelds, Fallenbau (Kreaturen 13), Bündnisse.

---

## 8. Entwurf: Wendepunkte – Versöhnung, Bündnis, Aufgabe

Eine Feindschaft kann enden oder sich wandeln (Modul 09 §6). Beim Nemesis gilt zusätzlich:

| Wendepunkt | Auslöser (Entwurf) | Ergebnis |
|---|---|---|
| **Versöhnung** | Wiedergutmachung, Zeit, gemeinsamer Verlust | Kante verändert sich; Rache endet (nicht garantiert; N-BE-013) |
| **Bündnis** | gemeinsamer Feind, gegenseitiger Nutzen | `allied`; Misstrauen bleibt möglich |
| **Aufgabe** | zu viele Niederlagen, Angst, Alter, Krankheit | Rückzug aus dem Leben; Abtauchen |
| **Bekehrung** | Erlösung, Heilung, Glaube | `redeemed` |
| **Flucht ins Ausland** | Verlust der Mittel | verschwindet aus der Region |
| **Verrat der Gruppe** | Gruppe wendet sich gegen ihn | verliert Mittel |
| **Wettbewerb statt Hass** | Rivalität wird sportlich, kollegial | Rivale |
| **Gefangen** | Behörde, Hunter, Gruppe | Verhör, Haft |
| **Tod durch Dritte** | Konkurrenten | `dead` |

**Verschonen als Entscheidung.** Das Verschonen eines Wesens ist eine der Spielerentscheidungen der Quellen (N-NL-004). Das Ergebnis ist **nicht festgelegt**: Der Verschonte kann dankbar sein, den Spieler verachten, es für Schwäche halten, es der Gruppe verheimlichen, abtauchen, sich rächen. Das Ergebnis hängt von Persönlichkeit, Art und Umfeld ab (Modul 04, 08).

**Keine gescriptete Läuterung.** Ein Nemesis ändert sich nur, wenn Ereignisse es rechtfertigen (N-LT-023). ‚Gute Menschen‘ werden nicht automatisch Verbündete.

---

## 9. Entwurf: Tod des Nemesis und seine Folgen

Wenn der Nemesis stirbt, endet seine Geschichte nicht (Modul 05 §9):

| Folge | Beschreibung |
|---|---|
| **Anker bei Beteiligten** | Spieler, Begleiter, Opfer, Verbündete, Angehörige haben Erinnerungen |
| **Gruppe** | Führungswechsel, Machtkampf, Auflösung (Modul 17) |
| **Rache der Nahestehenden** | Familie und Freunde als neue Feinde (Modul 10 §9) |
| **Erbe** | Besitz, Wissen, Schulden gehen über |
| **Nachfolger** | Wesen oder Mensch übernimmt Rolle (`48#142A` Beispiel) |
| **Beweise, Fälle** | Fälle werden geschlossen oder bleiben offen (Ermittlung 09) |
| **Gerüchte** | Der Tod wird bekannt, verzerrt (Modul 07) |
| **Territorium** | Gebiet wird frei; andere Gruppen übernehmen (Modul 17) |
| **Archiv** | Eintrag wird auf den neuen Stand gebracht (Spielerwissen) |
| **Fegefeuer** | artabhängig; ‚tot ist tot‘ mit Ausnahme der Befreiung (N-NE-012) |

**Natürlicher Tod.** Menschliche Nemesis altern und sterben (N-FA-004); Wesen artabhängig.

**Tod durch den Spieler.** Der Spieler kann die Gruppe oder Folgen nicht sauber „beenden“; die Welt reagiert (Anforderung E).

---

## 10. Entwurf: Beispielabläufe

> 💡 **Beispiele**, keine Inhalte: Sie zeigen, wie die Systeme zusammenwirken, ohne Figuren, Orte oder Arten festzulegen.

### 10.1 Menschlicher Straftäter

1. **Begegnung.** Der Spieler verhindert einen Überfall. Der Täter flieht verletzt (Bein).
2. **Ausgang.** Zeugen sehen den Spieler, ein Passant filmt. `Überlebender`.
3. **Folgen.** Polizei sucht; Täter versteckt sich bei einem Bekannten; Groll gegen den Spieler (Gerücht, er habe ihn verraten).
4. **Hintergrund.** Heilt Wochen; wird zu Hause gepflegt; Bande wird informiert (plausible Kommunikation).
5. **Wissen.** Er sah das Fahrzeug. Er erkennt es später (N-GE-018).
6. **Rückkehr.** Er findet das Fahrzeug an einem Ort, wartet auf Gelegenheit; Hinweise: ein Anruf, ein Gerücht, ein bekannter Wagen.
7. **Wendepunkt.** Der Spieler stellt ihn der Polizei, oder verhandelt, oder tötet ihn. Die Folgen laufen über Familie, Bande und Behörde (Modul 17, 18).

### 10.2 Konkurrierender Hunter

1. **Begegnung.** Zwei Hunter arbeiten am selben Fall; der Spieler löst ihn vor ihm. (Hunter-NPCs können Fälle vor dem Spieler bearbeiten und konkurrieren: `M-PD`, Kreaturen 11 §3.1.)
2. **Folgen.** Rivalität; er verbreitet seine Sicht unter Huntern (Ruf, Modul 07).
3. **Entwicklung.** Mal Kooperation (Gefahr), mal Wettbewerb. Respekt hoch, Vertrauen niedrig.
4. **Wendepunkt.** Er rettet den Spieler, oder lässt ihn im Stich, oder wird korrumpiert (Quelle: `19#4E`).
5. **Ausgang.** Rivale bleibt, wird Verbündeter, Feind oder stirbt.

### 10.3 Intelligentes Wesen (nach dem Beispiel des Assistenten, `48#140A`)

1. **Erste Begegnung.** Der Spieler versucht, ein intelligentes Wesen mit einer Falle zu fangen. Es entkommt knapp. (Das Beispiel des Assistenten spricht von einem Dämon und einer Teufelsfalle; die Quelle verlangt die Mythologie als Grenze.)
2. **Zweite Begegnung.** Das Wesen erkennt den Spieler wieder und untersucht verdächtige Räume vorsichtiger (N-NE-004).
3. **Später.** Es könnte anderen Wesen von den bevorzugten Methoden erzählen (N-NE-002).
4. **Langfristig.** Es könnte versuchen, sich zu rächen, den Spieler zu meiden oder gezielt Verbündete anzugreifen (N-NE-019).
5. **Grenzen.** Es lernt nur, was es beobachtet; es bleibt seiner Art gebunden (N-NE-020).

### 10.4 Gangführer mit Gruppe

1. **Begegnung.** Der Spieler tötet einen Gangführer. Die Gruppe verschwindet nicht (Beispiel `48#142A`).
2. **Folgen.** Nachfolger, Machtkampf, Rivalen (Modul 17).
3. **Rache.** Verwandte, Freunde, Lieutenant: unterschiedliche Motive.
4. **Fortlauf.** Die Gruppe sucht Wege, der Spieler beeinflusst sie.

---

## 11. Entwurf: Mehrere Feinde gleichzeitig

Mehrere Nemesis-Figuren können entstehen. Das System braucht Regeln, damit es nicht überfordert (Risiko ‚Scope und Fun‘ in Proposal 0010 §9):

| Regel (Entwurf) | Inhalt |
|---|---|
| **Aktive Obergrenze** | Anzahl gleichzeitig aktiver Nemesis ist begrenzt (Zahl offen, Q-N09) |
| **Ruhezustand** | Überzählige Figuren gehen in `dormant`; sie bleiben im Datensatz |
| **Priorität** | nach Motiv, Nähe, Bedrohung, Plausibilität, Erzählung |
| **Bündnisse** | zwei Nemesis können sich verbünden (Modul 17) |
| **Konflikt untereinander** | Nemesis können einander bekämpfen |
| **Fairness** | kein gleichzeitiger Angriff aus mehreren Richtungen ohne Vorzeichen |
| **Wechsel** | Wenn der Spieler eine Figur besiegt, rückt eine andere nach — nicht automatisch |

---

## 12. Sonderfälle

1. **Der Spieler stirbt oder wird bewusstlos.** Der Nemesis verfolgt sein Ziel; Ausgang ist offen (N-NL-013). Kein Standard-Game-Over (Festnahme ≠ Game Over: N-NL-012 gilt für Festnahme; der Tod ist `M-PD`-OFFEN).
2. **Der Spieler ist im Gefängnis.** Die Welt läuft; Nemesis können handeln (Haft-Zeitsprung: Ermittlung 16). Konsequenzen für Safehouses, Familie, Fahrzeuge sind Folgen der Abwesenheit.
3. **Der Spieler wechselt die Region.** Der Nemesis folgt, wenn er das Wissen hat; sonst verliert er die Spur.
4. **Der Nemesis wird vom Spieler vergessen.** Das ist spielerisch möglich; das System erinnert den Spieler nicht an ihn.
5. **Der Nemesis wird zum Unterstützer eines anderen Spielers im Koop.** Die Beziehung ist je Spieler (Q-N15).
6. **Der Nemesis hat Familie.** Racheketten; Schutz der Familie als Motiv (Modul 10).
7. **Der Nemesis wird gefasst und freigelassen.** Haft endet; er kann zurückkehren (Ermittlung 15, 16).
8. **Der Nemesis wird besessen oder verwandelt.** Der Kern bleibt, Fähigkeiten ändern sich (Kreaturen 05).
9. **Der Nemesis wechselt die Identität.** Tarnidentität; der Datensatz bleibt (Modul 03 §11).
10. **Der Nemesis gerät in Konflikt mit der eigenen Gruppe.** Die Gruppe sucht ihn (Modul 17).
11. **Der Nemesis sucht Hilfe bei Behörden.** Zeugenschutz, Anzeige gegen den Spieler (Modul 18).
12. **Zeitreise.** Ein Nemesis in der Vergangenheit hat nichts erlebt, was erst später kommt (N-GE-028).

---

## 13. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-211 | Jede Phase des Lebenszyklus erzeugt ein Ereignis im Journal | Abschnitt 2 |
| N-EN-212 | Ein Gegner, der die Begegnung überlebt, behält seine Verletzungen | N-NL-006, N-NL-007 |
| N-EN-213 | Eine Rückkehr geschieht nur, wenn Motiv, Mittel, Wissen und Gelegenheit bestehen | Abschnitt 6 |
| N-EN-214 | Eine Rückkehr erfolgt nicht in Story-Sequenzen oder Hauptmissionen, wenn dies die Erzählung stört | Abschnitt 6.1 |
| N-EN-215 | Die Hintergrundentwicklung ist deterministisch und verbraucht nur Weltzeit | N-NL-011 |
| N-EN-216 | Eine Hintergrundaktivität erzeugt eine überprüfbare Spur im Weltzustand | Abschnitt 5 |
| N-EN-217 | Der Nemesis kann Mittel nur aus dem Weltzustand beziehen | Abschnitt 5 |
| N-EN-218 | Ein verletzter Nemesis tritt nicht vor Heilung oder Kompensation in Konfrontationen auf | Abschnitt 4 |
| N-EN-219 | Ein Wendepunkt hat eine Ursache im Journal | Abschnitt 8 |
| N-EN-220 | Der Tod eines Nemesis löscht keine Erinnerungen anderer an ihn | N-GE-025 |
| N-EN-221 | Eine Anpassung kann durch einen Methodenwechsel des Spielers unwirksam werden | Abschnitt 7 |
| N-EN-222 | Gleichzeitig aktive Nemesis-Figuren liegen unter der festgelegten Obergrenze | Abschnitt 11 |
| N-EN-223 | Ein gefangener Nemesis führt keine Außenaktionen aus | Abschnitt 3 |
| N-EN-224 | Ein Nemesis erscheint nicht in einer Region, die er nicht erreichen kann | Abschnitt 6.1 |
| N-EN-225 | Ein Nemesis erkennt den Spieler nur mit Hilfe einer Erinnerung oder neuen Wahrnehmung | N-GE-015 |
| N-EN-226 | Die Verschonung eines Gegners erzeugt eine Beziehungsänderung, deren Richtung von Persönlichkeit und Deutung abhängt | Abschnitt 8 |

---

## 14. Was fehlt

- Keine Zahlen: Erholungsdauer, Mindestabstände, Obergrenze (Q-N09, Q-N19).
- Offen: Wie der Spielertod behandelt wird (N-NL-013, Q-N10).
- Offen: Ob Nemesis-Figuren auch ohne Spielerbegegnung entstehen können (zum Beispiel durch Fraktionskonflikte) — die Anforderung nennt nur ‚überlebende Gegner‘.
- Keine Entscheidung zur Sichtbarkeit im Archiv (welcher Eintrag, welcher Wissensstand).
- Keine konkreten Arten: Bestiarium-Zuordnung der Anpassungsvorräte (Kreaturen 19, Artenvorlage).
