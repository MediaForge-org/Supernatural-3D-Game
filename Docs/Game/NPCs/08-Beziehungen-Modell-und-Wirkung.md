# NPCs 08: Beziehungen – Modell und Wirkung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, was eine Beziehung zwischen zwei Akteuren ist, welche inneren Größen sie hat und wie sie nach außen wirkt, ohne dass der Spieler je eine Zahl sieht. Beschlossen sind die verdeckten Dimensionen, die Ausdrucksformen und das Prinzip ‚Beziehung ≠ Gruppenruf‘; **Kantenmodell, Aggregation und Wirkungsabbildung sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Ruf als Überzeugungsmenge: [Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) §4. Romantik, Anziehung und Präferenzen: [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) (hier nicht dupliziert).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Die Beziehungskante](#2-entwurf-die-beziehungskante)
3. [Entwurf: Dimensionen und ihre Bedeutung](#3-entwurf-dimensionen-und-ihre-bedeutung)
4. [Entwurf: Beziehungstypen und Rollen](#4-entwurf-beziehungstypen-und-rollen)
5. [Entwurf: Beziehungsstufen](#5-entwurf-beziehungsstufen)
6. [Entwurf: Aggregation aus Ereignissen](#6-entwurf-aggregation-aus-ereignissen)
7. [Entwurf: Von der Beziehung zum Verhalten](#7-entwurf-von-der-beziehung-zum-verhalten)
8. [Beziehung, Gruppenruf und Ruf](#8-beziehung-gruppenruf-und-ruf)
9. [Beziehungen zwischen NPCs](#9-beziehungen-zwischen-npcs)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-BE-001 | Interne Beziehungswerte wie Vertrauen, Angst, Respekt und Misstrauen **können existieren**, werden aber **nicht als Zahlen oder Balken** angezeigt | 📋 | `M-PD` ‚NPCs and AI‘; `23#1056A` Frage 40; „ja haben wir schon gesagt“ in `23#1057E`; `23#53A`, Zustimmung `23#54E` |
| N-BE-002 | Beziehungen kommen zum Ausdruck über Ton, Wortwahl, Körpersprache, Zusammenarbeit, Informationen, Zugang, Gefallen und Folgen | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-BE-003 | Weitere interne Größen neben den vier genannten: Zuneigung, Anziehung, Vertrautheit, Bindung, Konflikt, jüngste Ereignisse, Beziehungsstufe | 📋 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8; [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §3 |
| N-BE-004 | Anziehung, Vertrauen, Zuneigung, Komfort und Vertrautheit entwickeln sich unabhängig voneinander; emotionale Bindung ist von Anziehung und Vertrauen getrennt | 📋 | [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §3 (Entwickler: ja) |
| N-BE-005 | Beziehung bildet sich durch Qualität der Interaktion, gemeinsame Erlebnisse, Vertrauen, Aufmerksamkeit, Konflikt, Kommunikation und Zeit; gemeinsame Stille, Autofahrt, Fernsehen, Lagerfeuer, Recherche oder Rückfahrt formen Beziehung | 📋 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8; `M-PD` ‚Smartphone …‘ (‚Long vehicle travel can contain dynamic companion conversation/relationship content‘) |
| N-BE-006 | Nicht jede ignorierte Nachricht ist ein Malus; wichtige wiederholte Ignorierung kann je nach Charakter reagieren | 📋 | [Gameplay-Systeme](../Gameplay-Systeme.md) §8 |
| N-BE-007 | Beziehungen verändern sich langfristig (Freundschaften, Beziehungen, Konflikte, Trennungen) | 📋 | `23#1056A` Frage 7; „ja“ in `23#1057E` |
| N-BE-008 | Der Spieler kann mit vielen wiederkehrenden NPCs echte Freundschaften entwickeln; sie entwickeln sich natürlich durch gemeinsame Erlebnisse, Gespräche, Hilfe, Zuverlässigkeit und Streit | 📋 | `23#1088A` Fragen 1–3; „1-3 ja“ in `23#1089E` |
| N-BE-009 | Beziehungen zwischen NPCs persistieren; Haushalte, Besitz, Betreiber und Nachbarschaften verändern sich | 📋 | `23#1056A` Fragen 106–111; „ja“ in `23#1057E` |
| N-BE-010 | Beziehungsstufen: aktive Beziehung, enge Bindung, lockere Verbindung, frühere Beziehung, gelegentlicher Kontakt, praktisch vergessen | 📋 | `35#268A` Frage 30; „16-30 ja“ in `35#269E` |
| N-BE-011 | Vertrauen kann sich deutlich schneller ändern als tiefe Bindung; Anziehung schwankt, ohne dass die Beziehung zerbricht; Komfort wächst durch wiederholt positive Interaktion; Vertrautheit macht Interaktionen weniger ‚awkward‘ | 📋 | `35#268A` Fragen 21–24; „16-30 ja“ in `35#269E` |
| N-BE-012 | Beziehungshistorie beeinflusst, wie schnell Missverständnisse verziehen werden; nach mehreren schlechten Erfahrungen wird ein NPC dauerhaft vorsichtiger, nach guten offener; zwei NPCs können auseinanderdriften | 📋 | `35#268A` Fragen 26–29; „16-30 ja“ in `35#269E` |
| N-BE-013 | Vergebung hängt von Persönlichkeit, Schwere des Ereignisses, Zeit und späterem Verhalten ab; manche Dinge verzeihen manche NPCs nie | 📋 | `35#270A` Fragen 49–50; „22-52 ja“ in `35#271E` |
| N-BE-014 | Hass oder Groll kann neben Anziehung bestehen; Ex-Partner können später wieder Casual werden oder die Beziehung kann erneut entstehen; eine zweite Beziehung verläuft anders, weil beide die erste erinnern | 📋 | `35#270A` Fragen 44–48; „22-52 ja“ in `35#271E` |
| N-BE-015 | Trennungen speichern unterschiedliche Gründe statt ‚Beziehung beendet‘; ein NPC kann selbst Schluss machen | 📋 | `35#270A` Fragen 41–43; „22-52 ja“ in `35#271E` |
| N-BE-016 | Ein Relationship-Event kann gemeinsame Freunde beeinflussen; Freundeskreise können Wissen über Beziehungen verbreiten | 📋 | `35#270A` Fragen 51–52; „22-52 ja“ in `35#271E` |
| N-BE-017 | Beziehungen reagieren auf den Lebensstil (häufiges Verschwinden, Kriminalität, ständige Verletzungen); ein normaler Partner weiß anfangs nichts vom Hunter-Leben; der Spieler entscheidet, ob und wann er die Wahrheit sagt; die Reaktion reicht von Unglauben über Angst bis Akzeptanz | 📋 | `23#1088A` Fragen 11–14; „11-15 genau“ in `23#1089E` |
| N-BE-018 | Begleiter und Hunter erinnern, ob der Spieler sie gerettet, im Stich gelassen, belogen oder einen Fall gut gelöst hat; das beeinflusst, ob sie nachts mitkommen. Dabei hängt es **nicht nur** von einer sichtbaren Beziehungsanzeige ab | 📋 Richtung | `23#117A` Punkt 17; „17 ja genau“ in `23#118E` |
| N-BE-019 | Vertrauen beeinflusst Zusammenarbeit von Begleitern ohne sichtbaren Balken | 📋 | `23#1088A` Frage 20; „16-24 ja“ in `23#1089E` |
| N-BE-020 | **Gruppenruf ≠ individuelle Beziehung**: man kann bei einer Gruppe allgemein unbeliebt sein und ein Mitglied als besten Freund haben, und umgekehrt | 🧭 | `23#678A` (‚Zwei unterschiedliche Beziehungen gleichzeitig‘, Vorschlag des Assistenten); gestützt durch `48#153E` (Wahl D „D plus noch extras“: guter Ruf unter Huntern bei gleichzeitiger Fahndung) |
| N-BE-021 | Jeder wichtige NPC kann eine eigene Meinung über den Spieler entwickeln; es gibt Hunter-Reputation, Polizei, Monster, regionale Bekanntheit | 📋 | `48#153E` (Wahl); Auffächerung `48#154A` (💡) |
| N-BE-022 | Zwischen Fraktionen gelten Beziehungen wie Verbündet, Freundlich, Neutral, Angespannt, Feindlich, Krieg; diese Beziehungen verändern sich | 🧭 | `23#676A` (Vorschlag des Assistenten) |
| N-BE-023 | Ein Companion kann auf Handlungen unterschiedlich reagieren, kommentieren und verweigern (eigene Moral) | 📋 | `23#1088A` Frage 19; „16-24 ja“ in `23#1089E` |
| N-BE-024 | Das Datenmodell für Kommunikationsstil und Beziehungsfolgen ist offen | ❓ | `M-OD` |

---

## 2. Entwurf: Die Beziehungskante

Eine Beziehung ist ein **gerichteter** Datensatz zwischen zwei Akteuren (NPC, Spieler, Gruppe). Gerichtet, weil A etwas anderes für B empfinden kann als B für A: Die Quellen sagen es für Chemie ausdrücklich (`35#260A` Frage 36, ‚bidirektional unterschiedlich‘; „27-38 ja“ in `35#261E`) und für Verdacht, Vertrauen und Ruf allgemein. (💡)

```text
RelationEdge  (von A nach B)
├── from, to          NPC-ID | player | group-ID
├── type              Verwandtschaft | Partner | Freund | Kollege | Nachbar | Geschäft |
│                     Mentor/Schützling | Gläubiger/Schuldner | Rivale | Feind | Informant ...
├── stage             aktiv | eng | locker | früher | gelegentlich | vergessen   (N-BE-010)
├── dims              trust, fear, respect, suspicion, affection, familiarity, attachment,
│                     conflict, resentment, debt, loyalty (+ Adult: attraction, comfort)
├── recent            Puffer der jüngsten Ereignisse (kurze Wirkung)
├── anchors[]         Verweise auf prägende Ereignisse (Modul 05)
├── summary           verdichtete Tendenz ("generell positive Geschichte")
├── knows             was A über B glaubt (Eintrag-Verweise; kann falsch sein)
├── visibility        öffentlich | privat | geheim | unbekannt für Dritte
├── since, last_contact
└── flags             hidden (B weiß nichts von der Kante), asymmetric, frozen (z. B. durch Tod)
```

**Entwurfsregeln:**

1. **Kanten gehören dem Quell-NPC.** Der Datensatz von A enthält seine Sicht auf B; B hat eine eigene Kante (falls er A kennt). Fehlt die Kante, kennt B den A nicht (N-ID-016).
2. **Nur relevante Kanten** werden gespeichert (`35#278A` Frage 25; „12-35 ja“ in `35#279E`): Für Hintergrundbeziehungen genügt eine vereinfachte Darstellung (Frage 26); eine echte Beziehung zwischen zwei NPCs wird automatisch detaillierter (Frage 27); Ehe, Familie und enge Freundschaft ebenso (Frage 28); kurze Anziehung zwischen Hintergrund-NPCs darf wieder verschwinden (Frage 29); wichtige Historie nie (Frage 30).
3. **Eine Kante enthält keine Wahrheit über B**, nur A's Wahrnehmung von B. Die Wahrheit steht im Datensatz von B.
4. **Anker und Summary koexistieren** (N-GE-022): Die Tendenz ersetzt keine prägenden Ereignisse.
5. **Verborgene Kanten.** Eine Kante kann für Dritte unbekannt sein (heimliche Affäre, Informant, Spitzel). Sie wirkt im Verhalten und kann aufgedeckt werden (Ermittlung 13 §9).

---

## 3. Entwurf: Dimensionen und ihre Bedeutung

Die Quellen nennen die Dimensionen, aber nicht ihre Semantik. Der Entwurf legt eine **Arbeitsdefinition** fest, die für Tests und Dialog gebraucht wird (💡):

| Dimension | Frage, die sie beantwortet | Steigt durch | Sinkt durch | Wirkt auf |
|---|---|---|---|---|
| Vertrauen | Kann ich mich darauf verlassen, dass er mir nichts Böses will und Absprachen einhält? | Zuverlässigkeit, Hilfe, Ehrlichkeit | Lüge, Verrat, Unzuverlässigkeit | Informationsweitergabe, Kooperation, Zugang |
| Angst | Fürchte ich ihn? | Gewalt, Drohung, Ruf, Übermacht | Schutz, Zeit ohne Gefahr | Gehorsam, Flucht, Hass |
| Respekt | Achte ich seine Fähigkeiten und Haltung? | Leistung, Mut, Fairness | Schwäche, Charakterfehler | Folgebereitschaft, Ernstnahme |
| Misstrauen/Verdacht | Glaube ich, dass er etwas verbirgt oder Böses vorhat? | Widersprüche, Hinweise, Gerüchte | Bestätigung, Transparenz | Rückfragen, Beobachtung, Meldung |
| Zuneigung | Mag ich ihn? | Wärme, Humor, gemeinsame Erlebnisse | Kälte, Streit | Wohlwollen, Nachsicht |
| Vertrautheit | Kenne ich ihn gut, bin ich ungezwungen? | Zeit, Wiederholung | Funkstille | Natürlichkeit, Offenheit |
| Bindung | Gehört er zu den Menschen, die mir wichtig sind? | geteilte Last, Schuld, Jahre | Trennung, Verrat | Opferbereitschaft, Verlustgefühl |
| Konflikt | Gibt es Streit, der noch offen ist? | Auseinandersetzung | Klärung | Verweigerung, Spannung |
| Groll | Trage ich ihm etwas nach? | Kränkung, Unrecht | Entschuldigung, Zeit, Wiedergutmachung | Rache, Distanz |
| Schuld (Debt) | Schulde ich ihm etwas oder er mir? | Rettung, Gefallen, Geld | Erfüllung | Verpflichtung, Gegenleistung |
| Loyalität | Stehe ich zu ihm, auch wenn es kostet? | Gemeinsamkeit, Eid, Gruppenzugehörigkeit | Verrat | Gruppenverhalten (Modul 16) |
| Anziehung, Komfort | siehe [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) | | | |

**Hinweise (Entwurf):**

1. **Dimensionen sind getrennt, nicht Pole** (N-BE-004): Hoher Respekt bei hoher Angst und niedrigem Vertrauen ist möglich (ein gefürchteter Boss). Hoher Groll bei hoher Anziehung (N-BE-014) ebenso.
2. **Nicht jede Dimension gilt in jeder Kante.** Ein Händler hat kaum Bindung; ein Fremder nur Verdacht, Angst oder Neutralität.
3. **Die Menge der Dimensionen ist offen** (die Quelle sagt „usw.“ in `35#260A`; Mods dürfen weitere registrieren: [Modul 23](23-Koop-Host-Speicherstand-und-Mods.md)).
4. **Keine Dimension ist ein Hauptwert.** Es gibt keine ‚Beziehungsnote‘ (`Gameplay-Systeme` §8: keine Balken; Adult 05 §2: keine versteckte Gesamtnote ‚Kompatibilität = 82‘).
5. **Die vier Grunddimensionen (Vertrauen, Angst, Respekt, Verdacht)** sind die, die Dialog, Kooperation, Zugang und Gefallen zuerst bestimmen (`M-P05`-Titel 008, 010 bis 012; DRAFT).

---

## 4. Entwurf: Beziehungstypen und Rollen

Der Typ legt fest, **welche Dimensionen relevant sind** und **welche Erwartungen** gelten.

| Typ | Beispiele aus den Quellen | Typische Erwartungen (Entwurf) |
|---|---|---|
| Verwandtschaft | Eltern, Kinder, Geschwister (`23#1056A` Frage 5) | Loyalität, Fürsorge, Erbe, Konflikt |
| Partner | Beziehung, Zusammenziehen, Heirat (`23#1088A` Fragen 4–9) | Exklusivität oder vereinbarte Offenheit, Teilen, Vertrauen |
| Freund | Freundschaft | Treue, Unterstützung, Offenheit |
| Kollege, Vorgesetzter | Arbeitsplatz | Pflicht, Hierarchie, Konkurrenz |
| Nachbar | Wohnumgebung | Dauer, Klatsch, Hilfe |
| Geschäft | Händler, Kunde, Auftraggeber | Preis, Wort, Ruf |
| Mentor, Schützling | Hunter, Trainer | Wissensweitergabe, Verantwortung |
| Schuldner, Gläubiger | Gefallen, Geld | Erfüllung, Druck |
| Informant | Informationsquelle | Gegenleistung, Gefahr |
| Rivale | Konkurrenz um Rang, Fall, Partner, Gebiet | Misstrauen, Wetteifer |
| Feind | persönliche Feindschaft | Gegnerschaft ([Modul 11](11-Nemesis-Modell-und-Datensatz.md)) |
| Companion | Hunter-Begleiter, Hund | Kameradschaft, Gefahr teilen |
| Zugehörigkeit | Gruppe–Mitglied | Rang, Pflicht, Regeln ([Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md)) |

**Mehrfachtypen.** Dieselbe Beziehung kann mehrere Typen haben (Kollege und Freund und Schuldner). Die Kante enthält eine Liste von Typen mit unterschiedlicher Gewichtung (💡).

**Typwechsel.** Ein Typ kann wechseln (Feind → Verbündeter, Freund → Rivale). Der Wechsel hat eine Ursache im Ereignisjournal (N-EN-111).

---

## 5. Entwurf: Beziehungsstufen

Die Quellen nennen sechs Stufen (N-BE-010): *aktive Beziehung, enge Bindung, lockere Verbindung, frühere Beziehung, gelegentlicher Kontakt, praktisch vergessen*. Sie sind **Zustände der Kante**, nicht der Person. Ein Übergangsentwurf (💡):

```text
                    Kontakt, Zeit, Ereignis
 (unbekannt) ──► gelegentlicher Kontakt ──► lockere Verbindung ──► aktive Beziehung ──► enge Bindung
                        │                          │                    │                  │
                        ▼                          ▼                    ▼                  ▼
                 praktisch vergessen        frühere Beziehung ◄── Trennung/Streit/Tod ──────┘
                        ▲                          │
                        └── lange Funkstille ◄─────┘          (Wiederannäherung möglich: N-BE-014)
```

| Stufe | Typische Merkmale (Entwurf) | Auslöser des Wechsels |
|---|---|---|
| gelegentlicher Kontakt | kennt, sieht selten | Begegnung → Wiederholung; langer Abstand → vergessen |
| lockere Verbindung | wiederkehrend, unverbindlich | Wiederholung, Gemeinsamkeit |
| aktive Beziehung | regelmäßig, Absprachen | gemeinsamer Alltag, Erlebnisse |
| enge Bindung | Priorität im Leben | geteilte Last, Schuld, Jahre |
| frühere Beziehung | beendet, aber im Gedächtnis | Trennung, Tod, Streit |
| praktisch vergessen | keine aktive Erinnerung | lange Funkstille, geringe Bedeutung |

**Regeln (Entwurf):**

1. **Der Wechsel erfolgt durch Ereignisse und Zeit**, nicht durch einen Schwellenwert alleine; es gibt Hysterese (eine Stufe wird nicht wegen eines einzelnen Datenpunkts verlassen).
2. **Rückwege sind erlaubt:** früher → locker, vergessen → gelegentlicher Kontakt (N-GE-016, N-GE-017).
3. **‚Praktisch vergessen‘ löscht nicht** (N-ID-015): Die Kante wird verdichtet, Identität bleibt.
4. **Stufe und Typ sind verschieden:** ‚Partner‘ kann ‚früher‘ sein.

---

## 6. Entwurf: Aggregation aus Ereignissen

Der Assistent skizzierte in `23#51A` Abschnitt 9, dass aus Einzelerinnerungen Werte aggregiert werden und die Einzelereignisse dennoch erhalten bleiben (Beispiel ‚Trust 72/100, Fear 14/100, Respect 81/100, Suspicion 23/100‘, 💡; „Aber die wichtigen Einzelereignisse bleiben trotzdem erhalten, damit Dialoge konkret darauf Bezug nehmen können“). Der Entwurf formalisiert den Ablauf:

```text
Ereignis e (Journal)
   │  Beteiligte erfahren es? (Modul 06)         -- nein: keine Wirkung
   │  Erinnerung wird angelegt (Modul 05)
   ▼
Bewertung durch A in Bezug auf B:
   Bedeutung des Ereignisses  x  Empfindlichkeit von A (Kern, Zustand)  x  Erwartung von A
   x  Zuschreibung (war B absichtlich schuld?)  x  Kontext (Zeugen, Alter der Kante)
   ▼
Änderung der Dimensionen   (begrenzt je Zeitschritt, mit Trägheit)
   ▼
Anker anlegen, falls prägend;  Stufe prüfen;  Summary nachführen
   ▼
Ausdruck:  Dialogkontext, Kooperationsbereitschaft, Zugang  (Modul 20)
```

**Zuschreibung ist entscheidend.** Wer glaubt, B habe ihm absichtlich geschadet, reagiert anders als bei einem Unfall. Zuschreibung ist Teil der Deutung (Modul 06), die auch falsch sein kann. Beispiel: Ein Formwandler in Gestalt des Spielers verletzt A (`48#154A`, Beispiel): A schreibt dem **Spieler** Schuld zu, obwohl dieser unschuldig ist. Die Beziehungskante A→Spieler verschlechtert sich auf Grundlage eines falschen Glaubens (N-GE-009).

**Gewichtungsregeln (Entwurf):**

| Regel | Inhalt |
|---|---|
| Asymmetrie | Vertrauensbruch wiegt schwerer als Vertrauensaufbau; die Quellen sagen es nicht, es folgt aus ‚Vertrauen ändert sich schneller als Bindung‘ und ‚vergangene schlechte Behandlung macht vorsichtiger‘ (N-BE-011, N-BE-012); Gewichtung ist offen |
| Sättigung | Dimensionen haben Grenzen; hohe Werte wachsen weniger |
| Trägheit | Langzeitzustände ändern sich langsamer als Kurzzeitzustände (Modul 04) |
| Erstes Mal | erste Hilfe oder erster Verrat wirkt stärker als die zehnte |
| Verjährung | Groll nimmt ab, wenn nichts Neues geschieht — manche Dinge nie (N-BE-013) |
| Vergebung | abhängig von Persönlichkeit, Schwere, Zeit, Verhalten danach |
| Gesellschaft | Dritte können mitwirken: Zeugen, Freunde, Gerüchte (N-BE-016) |
| Kontext | Rausch, Stress, Not senken oder erhöhen Auswirkungen |

**Die Formeln, Zahlen und Gewichte sind offen** (❓ Q-N07).

---

## 7. Entwurf: Von der Beziehung zum Verhalten

`M-PD` verlangt, dass Beziehung über sechs sichtbare Kanäle wirkt (N-BE-002). Der Entwurf bildet Dimensionen auf **Verhaltensbänder** ab, nicht auf Zahlen:

| Wirkung | Hohe Werte (Beispiel) | Niedrige Werte (Beispiel) | Hinweis |
|---|---|---|---|
| **Ton und Wortwahl** | Zuneigung, Vertrauen: warm, direkt | Misstrauen, Angst: förmlich, knapp, ausweichend | Persönlichkeit moduliert Ausdruck |
| **Körpersprache** | Nähe, Blickkontakt, Entspannung | Abstand, Anspannung, Fluchtbereitschaft | Animation, Gesten |
| **Kooperation** | Mitkommen, Rückendeckung, Teilen | Verweigern, Bedingungen, Verspätung | N-BE-019 |
| **Information** | gibt Wissen frei, Hinweise, Warnungen | verschweigt, lügt, lenkt ab | Aussagebereitschaft ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §5) |
| **Zugang** | öffnet Archive, Räume, Kontakte | sperrt, meldet | Sheriff-Beispiel |
| **Gefallen** | Hilfe ohne Gegenleistung | verlangt Gegenleistung oder verweigert | Schuld (Debt) |
| **Folgen** | schützt, verteidigt, bürgt | meldet, sabotiert, schädigt | Modul 11, 16 |

**Verhaltensbänder statt Schwellen (Entwurf).** Jedes Band ist ein Satz von Ausprägungen (zum Beispiel: *herzlich, freundlich, neutral, reserviert, kühl, feindselig*), den Dialog, Animation und Gameplay gemeinsam nutzen. Dadurch bleibt die Zahl intern, und die Welt hat eine **gemeinsame Sprache** für Beziehungszustand. Die Zuordnung der Bänder wird bei der Umsetzung getestet (Modul 29).

**Beispiele (💡):**

| Lage | Dimensionen | Ausdruck |
|---|---|---|
| Jemand, dem der Spieler das Leben rettete | hoher Respekt, hohe Schuld, hohes Vertrauen | Zugang zu Wissen, Hilfe, Erwähnung bei Dritten |
| Jemand, den der Spieler bedrohte | hohe Angst, niedriges Vertrauen, hoher Groll | meidet, belügt, meldet, später Rache |
| Ein langjähriger Freund, der belogen wurde | Vertrauen sinkt schnell, Bindung bleibt | Schmerz, Nachfragen, Distanz, mögliche Versöhnung |
| Ein neuer Kollege | wenig, Neugier | neutral, aufmerksam |
| Ein Hunter-Rivale | Respekt hoch, Vertrauen niedrig | Wettbewerb, Information nur gegen Gegenleistung |

---

## 8. Beziehung, Gruppenruf und Ruf

**Quellenlage:** Das Rufsystem ist ‚vollständig dynamisch plus Extras‘ (`48#153E`), also nicht eine Zahl, sondern ein Geflecht individueller, regionaler und gruppenbezogener Sichten (siehe [Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md) §1 und §4). Das Fraktionsmodell der Planung sagt: **Gruppenruf ≠ individuelle Beziehung** (N-BE-020).

**Entwurf – Schichten:**

| Schicht | Wer hält sie | Wodurch entsteht sie |
|---|---|---|
| Individuelle Beziehung | der einzelne NPC (Kante) | direkte Erfahrung (Modul 05) |
| Gruppenhaltung | die Gruppe als Akteur (Modul 16) | Mehrheit der Mitglieder, Führungsentscheidungen, Vorfälle |
| Gerücht, öffentliche Sicht | die Gemeinschaft | Weitergabe (Modul 07) |
| Offizielle Sicht | Behörde, Institution | Akte, Verfahren (Ermittlung 14, 15) |

**Regeln (Entwurf):**

1. **Die Gruppenhaltung ist ein Aggregat der Kanten ihrer Mitglieder, gewichtet nach Rang und Einfluss — aber nicht ersetzbar.** Wer die Führung überzeugt, ändert die Gruppenhaltung stärker als ein Mitglied.
2. **Die Mitglieder halten ihre eigene Meinung** (zum Beispiel der Vizepräsident, der ein Freund ist; `23#678A`).
3. **Individuelle Beziehungen können der Gruppenhaltung widersprechen** und tun es oft (Spitzel, Aussteiger, Freundschaften über Gruppengrenzen).
4. **Ruf ist die Summe der Kanten, die der Spieler nicht kennt.** Der Spieler sieht ihn nie als Zahl (Ermittlung 13 §4).
5. **Fehlinformation** (Formwandler, Gerücht, Manipulation) verändert die Kante des Empfängers, nicht die Wahrheit über den Spieler (Abschnitt 6).

---

## 9. Beziehungen zwischen NPCs

Die Quellen verlangen persistente Beziehungen zwischen NPCs (N-BE-009), ihre Entwicklung auch ohne Spieler (N-LT-006) und ihre Sichtbarkeit (Streit, Freundschaft, Klatsch). Für Hintergrund-NPCs gilt Verdichtung (Abschnitt 2, Regel 2).

| Aspekt | Entwurf |
|---|---|
| **Entstehung** | durch gemeinsame Orte, Arbeit, Familie, Zufallsbegegnungen, Gruppen, Ereignisse |
| **Entwicklung** | im Hintergrund mit gröberem Zeitschritt (Modul 22); wichtige Ereignisse (Heirat, Trennung, Tod) als einzelne Ereignisse |
| **Sichtbarkeit** | Streit, Umarmung, Gespräch, Meidung; im Alltag erkennbar |
| **Wirkung auf den Spieler** | Wer mit dem Spieler befreundet ist, kann über Dritte reden; Konflikte zwischen Freunden des Spielers belasten ihn |
| **Dreiecke** | A mag B, B mag C, C hasst A: eine Kette von Kanten (Konflikt, Eifersucht, Loyalitätsproblem) |
| **Gemeinsame Feinde** | verbinden; gemeinsame Interessen verbinden; Gegensätze trennen |
| **Netzwerk** | Kanten bilden ein soziales Netz, das die Reichweite von Information bestimmt (Modul 07, 10) |

**Grenze:** Das Netz wächst nicht auf die ganze Bevölkerung an; Aufbau und Wartung sind nach Persistenzstufe gestaffelt (Modul 03, 22).

---

## 10. Sonderfälle

1. **Einseitige Beziehung.** A hält sich für einen Freund von B, B kennt A kaum (die Kante A→B ist hoch, B→A fehlt oder niedrig). Wirkt im Dialog (‚Ich dachte, wir sind Freunde‘).
2. **Geheime Beziehung.** Die Kante ist `visibility = geheim`. Wird sie entdeckt (Fremdgehen, Spitzel), wirken Ruf und Dritte (N-BE-016).
3. **Beziehung zu Toten.** Die Kante wird eingefroren (`frozen`); die Dimensionen ändern sich nur noch durch Erinnerung (Trauer, Verklärung) ([Modul 05](05-Gedaechtnis-Erinnerung-und-Vergessen.md) §9).
4. **Beziehung zu verwandelten oder besessenen Personen.** Die Kante richtet sich an die Person, nicht an das Wesen; die Gedächtnisregeln der Besessenheit gelten (Kreaturen 05).
5. **Beziehung zu Wesen mit menschlicher Identität.** Eine Freundschaft kann bestehen, bevor der Spieler die Wahrheit kennt; die Aufdeckung kann das Vertrauen brechen oder festigen ([Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §2.3).
6. **Beziehung unter Zeitreise.** Gilt je Zeitstand (N-GE-028).
7. **Zwei Spieler im Koop.** Jeder Spieler ist ein eigenes Subjekt (Q-N15).
8. **Erpressung.** Eine Kante mit hoher Schuld oder Angst kann missbraucht werden; der Spieler kann Ziel oder Täter sein.
9. **Manipulation durch den Spieler.** Wenn er Vertrauen vortäuscht (Verkleidung, Lüge), baut er eine Kante auf eine falsche Identität. Bei Entdeckung bricht sie (N-GE-019).
10. **Nicht erwiderte Gefühle.** Kein Automatismus: ‚Nein bleibt Nein‘ (Adult 05 §2).
11. **Fehlende Information.** Ein NPC, der den Spieler lange nicht sah, verändert seine Kante auf Basis dessen, was er über ihn hört (Gerüchte).
12. **Beziehung als Spielmechanik.** Es gibt keinen Beziehungsmodus-Wechsel (kein ‚Freundschaftslevel‘, das Fähigkeiten freischaltet); Freischaltungen sind Folgen konkreter Ereignisse (Gefallen, Zugang).

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-111 | Ein Typwechsel einer Beziehung hat ein Ereignis im Journal als Ursache | Abschnitt 4 |
| N-EN-112 | Eine Beziehungskante enthält nur die Wahrnehmung des Quell-NPCs, nie die Wahrheit über das Ziel | Abschnitt 2 |
| N-EN-113 | Dimensionen sind unabhängig; keine Operation darf sie zu einem einzigen Wert zusammenfassen, der Verhalten allein bestimmt | N-BE-004 |
| N-EN-114 | Keine Beziehungsdimension erscheint als Zahl oder Balken in der Spieler-Oberfläche | N-BE-001 |
| N-EN-115 | Eine Beziehungsänderung hat ein Ereignis, das der Beteiligte erfahren hat | N-GE-008 |
| N-EN-116 | Dasselbe Ereignis ändert zwei verschiedene Kanten unterschiedlich, wenn Kern, Erwartung und Zuschreibung unterschiedlich sind | N-LT-027 |
| N-EN-117 | Gruppenhaltung und individuelle Beziehung können widersprechen | N-BE-020 |
| N-EN-118 | Wird eine Kante auf falscher Zuschreibung aufgebaut, ändert sich die Wahrheit im Weltzustand nicht | Abschnitt 6 |
| N-EN-119 | Eine Beziehungsstufe wird nicht durch einen einzelnen Datenpunkt verlassen (Hysterese) | Abschnitt 5 |
| N-EN-120 | Der Tod einer Person friert die Kante ein; die Dimensionen ändern sich danach nur durch Erinnerung | N-GE-025 |
| N-EN-121 | Eine detaillierte Kante zwischen zwei Hintergrund-NPCs entsteht nur durch ein Ereignis, das sie rechtfertigt | `35#278A` Frage 27 |
| N-EN-122 | Eine einseitige Beziehung wird von beiden Seiten verschieden erlebt und gespielt | Abschnitt 10 |
| N-EN-123 | Eine Beziehung zu einem NPC hängt nicht von einem Zufallswurf ab, sondern von Ereignissen und Zustand | N-PS-016 |

---

## 12. Was fehlt

- Keine Zahlen, Gewichte, Skalen, Hysteresewerte (Q-N07).
- Offen: das Kommunikationsstil-Modell und seine Beziehungsfolgen (N-BE-024).
- Keine Aussage der Quellen, ob die Beziehungskante zwischen NPCs und Gruppen auch für Gruppen als Akteure gilt (Entwurf: ja).
- Offen: Orientierung und Präferenzen als allgemeine Beziehungsparameter (Adult 05 §4, Entscheidung dort geführt, nicht hier).
- Offen: ob das Fraktions-Beziehungsraster (Verbündet … Krieg) endgültig sechsstufig ist (N-BE-022).
