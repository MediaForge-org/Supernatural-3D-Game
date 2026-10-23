# NPCs 24: Integration und Konsistenzregeln

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie die Systeme der NPC-Welt zusammenarbeiten, ohne einander zu widersprechen (Anforderung H, `PE-N1022`): Besitzer je Tatsache, Ereignisse statt Direktschreibzugriffe, Invarianten als Tests, Beobachtungskonsistenz. Die Anforderung H ist **beschlossen**, die Regeln des Proposals 0010 §7 sind **Ingenieurvorschlag**; die Ausarbeitung hier (Besitzertabelle, Ereignisordnung, Invariantenkatalog) ist **Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Testkatalog: [Modul 29](29-Testkatalog-und-Abnahmekriterien.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Besitzer je Tatsache](#2-entwurf-besitzer-je-tatsache)
3. [Entwurf: Ereignisse und Ordnung](#3-entwurf-ereignisse-und-ordnung)
4. [Entwurf: Invariantenkatalog](#4-entwurf-invariantenkatalog)
5. [Entwurf: Querwirkungen an Beispielen](#5-entwurf-querwirkungen-an-beispielen)
6. [Entwurf: Beobachtungskonsistenz](#6-entwurf-beobachtungskonsistenz)
7. [Entwurf: Autorenschutz und Systemeingriffe](#7-entwurf-autorenschutz-und-systemeingriffe)
8. [Entwurf: Abhängigkeiten und Reihenfolge der Umsetzung](#8-entwurf-abhängigkeiten-und-reihenfolge-der-umsetzung)
9. [Sonderfälle](#9-sonderfälle)
10. [Entwurf: Prüfregeln](#10-entwurf-prüfregeln)
11. [Was fehlt](#11-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-KS-001 | **Integration (H):** Weltzustand (WorldFacts), Ermittlung/Caseboard, Monster-KI und Bestiarium, Verletzungen und Medizin, Ruf, Polizei und Justiz, Organisationen und Territorien, Dialog und Beziehungen, Speichern/Laden, Koop und Host-Speicherstand; die Simulation darf keine widersprüchlichen Zustände zwischen diesen Systemen erzeugen | 📋 | `PE-N1022` H |
| N-KS-002 | **Ein Besitzer je Tatsache:** Jede Art von Zustand hat genau einen maßgeblichen Speicher; andere halten Verweise über stabile IDs und lesen, kopieren nicht | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 1 |
| N-KS-003 | **Ereignisse statt Direktschreibzugriffe:** Querwirkungen (Tod → Beziehungen, Ruf, Fall, Territorium) werden als Ereignisse in definierter Reihenfolge verarbeitet, sodass Wiederabspielen dasselbe Ergebnis liefert | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 2 |
| N-KS-004 | **Invarianten als Tests:** ein toter Einzelner handelt nicht; eine Verletzung ist in jedem System sichtbar, das die Person zeigt; ein Zeuge weiß nur, was er beobachtet oder gehört hat; ein Fall zitiert nur Geschehenes; eine Fraktion hält kein Gebiet, das ihre Mitglieder nicht erreichen; der Zustand nach Laden gleicht dem vor dem Speichern | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 3 |
| N-KS-005 | **Beobachtungskonsistenz:** Was der Spieler nach dem Laden einer Region sieht, entspricht dem persistenten Datensatz, fortgeschrieben bis jetzt; es gibt keine Rücksetzer wichtiger Ergebnisse | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 4 |
| N-KS-006 | **Eine Weltuhr;** Fortschreiben läuft nie vor ihr; Geschichte und Zeitreise sind getrennte offene Themen | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 5 |
| N-KS-007 | Die Welt, die Monsterwelt und die Gesellschaft sind **dieselbe Simulation:** Monsterfälle beeinflussen normale Systeme (Immobilien, Tourismus, Polizei, Medien, Verkehr, Firmen, Krankenhäuser, Tiere, lokales Wetter, Gerüchte) | 🧭 | `23#1066A` Punkt 30 (Vorschlag des Assistenten); `23#1058A` ‚Supernatural-Reaktion‘ |
| N-KS-008 | Das Hauptstory-Gerüst bleibt kontrolliert; Spielerentscheidungen ändern Wege, Informationen, lokale Ausgänge und Beziehungen stärker als das Überleben vieler kritischer Hauptfiguren | 📋 | `M-PD` ‚Acts, chapters and controlled story reactivity‘ |
| N-KS-009 | Ein Wesen lernt aus dem Beobachteten; Behörden handeln aus tatsächlicher Information; Menschen teilen Informationen nur über plausible Kommunikation | 📋 | `M-PD` ‚Stealth, perception and horror AI‘ |
| N-KS-010 | Der Weltzustand bestimmt beim Laden die richtige persistente Variante (zerstörte Kirche, toter NPC wird nicht gespawnt) | 📋 | `23#48A` Abschnitt 12; `M-PD` ‚World state and persistence‘ |
| N-KS-011 | Die Welt wird von Systemen getragen, die miteinander sprechen („Nicht 500 isolierte Features bauen, sondern dieselben Systeme ständig miteinander sprechen lassen.“) | 💡 | `23#603A` (Schlussbemerkung des Assistenten) |
| N-KS-012 | Das System darf nicht begonnen werden, bevor seine Mindest-Eingaben existieren | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §8 |

---

## 2. Entwurf: Besitzer je Tatsache

Jede Art von Tatsache hat einen **Besitzer** (maßgeblicher Speicher) und **Leser**. Das verhindert doppelte Wahrheit (Proposal 0010 §7 Punkt 1). Tabelle (💡):

| Tatsache | Besitzer | Leser | Verweis |
|---|---|---|---|
| NPC lebt, tot, vermisst, gefangen | NPC-Datensatz (`life_state`) | Beziehungen, Haushalt, Fraktion, Fall, Dialog, Ort | Modul 03 §7 |
| Verletzung, Narbe, Behinderung | NPC-Datensatz (`injuries`) | Aussehen, Dialog, Zeitplan, Kampf, Ermittlung | Modul 14 |
| Aussehen (Rezept und Überschreibungen) | Aussehenssystem | Wiedererkennung, Dialog, Ermittlung | Decision 0005 |
| Persönlichkeit und Zustände | NPC-Datensatz | Dialog, Entscheidung, Beziehung | Modul 04 |
| Erinnerung und Glaube | NPC-Datensatz (Gedächtnis) | Dialog, Aussage, Entscheidung | Modul 05, 06 |
| Beziehungskante A→B | Datensatz des Quell-NPC | Dialog, Fraktion, Spieler-Ruf | Modul 08 |
| Haushalt (Mitglieder, Wohnort) | Haushaltsdatensatz | NPC-Datensatz (Verweis), Zeitplan, Ort | Modul 10 |
| Zugehörigkeit zu einer Gruppe | Fraktionsdatensatz (Mitgliederliste) und NPC-Datensatz (Rang) — **abgestimmt** | Dialog, Beziehung, Karte | Modul 16 |
| Territorium (Anspruch) | Fraktionsdatensatz | Welt, Karte (für Mitglieder), Vorfälle | Modul 17 |
| Gerücht (Fassung, Halter) | Gerüchteobjekt | NPC-Erinnerung (Verweis), Medien | Modul 07 |
| Nemesis-Zustand, Modell | Nemesis-Teil des Datensatzes | Weltzustand, Archiv (Sicht), Dialog | Modul 11 |
| Fall, Beweis, Verdächtige | Ermittlungssystem (`WorldFacts` + Ermittlungsspeicher) | NPC-Aussage, Behörden | Ermittlung 19 |
| Behördenakte, Fahndung | Behördendatensatz | Dialog, Fall | Modul 18; Ermittlung 14 |
| Ruf (Überzeugungen von Beobachtern) | Beziehungskanten + Gerüchte + Behördenakten (Summe) | Anzeige nirgends als Zahl | Ermittlung 13 |
| Ortszustand (zerstört, geschlossen) | Orts-/Weltzustand (`WorldFacts`, `locations_history`) | NPC, Ereignis, Karte | Modul 05 §9 |
| Fahrzeug | Fahrzeugdatensatz | Haushalt (Verweis), Zeitplan, Wiedererkennung | Welt 13 |
| Zeit | Weltuhr | alle | Modul 21 §9 |
| Spielerwissen | Archiv/Journal | Dialog (Themenfreigabe) | Ermittlung 04 |
| Modzustand | Mod-Segment | Kern über Registries | Modul 23 |

**Regeln (Entwurf):**

1. **Kein Datum hat zwei Besitzer.** Wo zwei Seiten dieselbe Wahrheit brauchen (Mitgliedschaft in beiden Listen), regelt ein **atomarer Übergang** beide Seiten (Ermittlung 19 §7).
2. **Leser kopieren nicht,** sie lesen über ID.
3. **Abgeleitete Werte** (Ruf, Macht, Stimmungsbänder) werden aus Quellen berechnet und nicht gespeichert, sofern sie nicht als Cache markiert sind.
4. **Jede Tatsache hat einen Schreibweg:** das Ereignis des Besitzers.

---

## 3. Entwurf: Ereignisse und Ordnung

Systemübergreifende Wirkungen laufen über **Ereignisse** (N-KS-003). Ein Ereignis hat (💡):

```text
Event
├── id, kind            Art (Tod, Verletzung, Begegnung, Gespräch, Umzug, Beitritt, Verhaftung, Gerücht ...)
├── time                Weltzeit
├── place               Ort / Region
├── actors[]            Handelnde (NPC-IDs, Spieler, Gruppen)
├── affected[]          Betroffene
├── witnesses[]         Wahrnehmende (Kanal und Qualität, Modul 06)
├── cause               Ursache (Vorereignis-ID, Regel, Entscheidung, Zufall mit Seed)
├── payload             Einzelheiten (Schwere, Ort der Verletzung, Inhalt der Aussage ...)
└── visibility          öffentlich | bekannt | verdeckt
```

### 3.1 Verarbeitung

```text
Ereignis entsteht  ──►  Besitzer-System prüft/ändert seinen Zustand (atomar)
                          │
                          ├─► Folgeereignisse (neue Ereignisse mit Ursache = dieses Ereignis)
                          └─► Benachrichtigung der interessierten Systeme (Abonnenten)
                                   │
                                   ├─ Erinnerung bei Zeugen (Modul 05)
                                   ├─ Beziehungsänderung (Modul 08)
                                   ├─ Gerüchte (Modul 07)
                                   ├─ Fall/Beweis (Ermittlung)
                                   ├─ Behörde (Modul 18)
                                   ├─ Fraktion (Modul 16, 17)
                                   └─ Dialog/Archiv (Modul 20)
```

### 3.2 Ordnung

1. **Totale Ordnung:** Weltzeit, dann Ereignis-ID, dann Sequenz (Modul 22 §7).
2. **Ursache vor Wirkung:** Ein Folgeereignis hat nie einen früheren Zeitstempel als seine Ursache.
3. **Verarbeitungsreihenfolge je System ist festgelegt** (zum Beispiel: Lebenszustand zuerst, dann Beziehungen, dann Fraktionen, dann Gerüchte), damit Wiederabspielen reproduzierbar ist (N-KS-003).
4. **Idempotenz:** Ein Ereignis wird einmal angewendet; Wiederholung bei Wiederabspielen ändert nichts.
5. **Keine Systemgrenzüberschreitung durch Direktzugriff:** Fraktion ändert nicht die Beziehungskante direkt, sondern erzeugt ein Ereignis.
6. **Atomare Gruppen:** zusammengehörige Änderungen (Tod: Lebenszustand, Haushalt, Fall) gelten alle oder keine.

### 3.3 Journal

Das Ereignisjournal ist die Quelle der Wahrheit für **Warum** (Ursache), nicht für jeden Zustand (Modul 21 §4). Abgeleitete Zustände können neu berechnet werden, soweit das Journal reicht.

---

## 4. Entwurf: Invariantenkatalog

Invarianten werden als **Tests** laufen (N-KS-004). Katalog (💡), gruppiert; die Nummern sind Prüfregeln (Abschnitt 10):

### 4.1 Existenz und Identität

| Invariante | Prüfregel |
|---|---|
| Ein toter NPC handelt nicht und wird nicht als lebende Figur materialisiert | N-EN-006 |
| NPC-IDs sind eindeutig und werden nie wiederverwendet | N-EN-001 |
| Jeder Verweis zeigt auf einen existierenden Datensatz oder einen Tombstone | N-EN-447 |
| Ein NPC ist zu jeder Weltzeit an genau einem Ort | N-EN-476 |

### 4.2 Wissen

| Invariante | Prüfregel |
|---|---|
| Ein Zeuge weiß nur, was er beobachtet, gehört oder erzählt bekommen hat | N-EN-041 |
| Ein Fall zitiert nur Geschehenes | N-EN-531 |
| Eine Behörde handelt nur aus tatsächlicher Information | N-EN-366 |
| Ein Nemesis kennt vom Spieler nur Beobachtetes | N-EN-181 |
| Ein Gerücht erreicht nur über existierende Kanäle | N-EN-081 |

### 4.3 Körper und Zustand

| Invariante | Prüfregel |
|---|---|
| Eine Verletzung ist in jedem System sichtbar, das die Person zeigt (Aussehen, Dialog, Zeitplan, Kampf) | N-EN-532 |
| Eine Narbe verschwindet nicht ohne Ereignis | N-EN-265 |
| Genesung verläuft in Weltzeit | N-EN-268 |
| Ein Gefangener nimmt nicht an Außenaktionen teil | N-EN-223 |

### 4.4 Gruppen und Raum

| Invariante | Prüfregel |
|---|---|
| Eine Fraktion hält kein Gebiet, das ihre Mitglieder nicht erreichen | N-EN-336 |
| Eine Fraktion hat eine gültige Führung oder dokumentierten Ausfall | N-EN-311 |
| Mitgliedschaft ist in Gruppen- und NPC-Datensatz übereinstimmend | N-EN-533 |
| Ein Haushalt hat keine Mitglieder, die tot oder ausgezogen sind, ohne Ereignis | N-EN-162 |

### 4.5 Zeit und Speicher

| Invariante | Prüfregel |
|---|---|
| Fortschreiben läuft nie der Weltuhr voraus | N-EN-478 |
| Zustand nach Laden gleicht dem Zustand vor dem Speichern | N-EN-443 |
| Dasselbe Ereignisjournal liefert denselben abgeleiteten Zustand | N-EN-449 |
| Zeitsprünge verbrauchen Weltzeit und holen Ereignisse nach | N-EN-451 |

### 4.6 Dialog und Oberfläche

| Invariante | Prüfregel |
|---|---|
| Keine Beziehungszahl ist sichtbar | N-EN-423 |
| Ein Dialogbezug nennt nur Existierendes | N-EN-424 |
| Ein Marker für Nemesis oder Fraktion erscheint nicht | N-EN-192 |

### 4.7 Spielregeln

| Invariante | Prüfregel |
|---|---|
| Kein Hauptstorypfad hängt von Fraktionen oder Rekruten ab | N-EN-306, N-EN-406 |
| Kinder sind vom Adult-System ausgeschlossen | N-EN-166 |
| Schutzstufen werden eingehalten | N-EN-009, N-EN-272 |

---

## 5. Entwurf: Querwirkungen an Beispielen

Beispiele zeigen, welche Systeme ein Ereignis berührt und in welcher Reihenfolge (💡). Die Beispiele sind abstrakt; sie legen keine Figuren fest.

### 5.1 Tod eines wichtigen NPCs (Stufe A) durch Gewalt

| Schritt | System | Wirkung |
|---|---|---|
| 1 | NPC-Datensatz | `life_state = dead`; `cause` verweist auf das Gewaltereignis |
| 2 | Haushalt | Mitglied entfernt; Einkommen und Zeitplan ändern sich |
| 3 | Zeugen | Erinnerungseinträge (wer sah was) |
| 4 | Beziehungen | Kanten zu Angehörigen/Freunden `frozen`; Trauer (Zustand) |
| 5 | Fraktion | Rang frei; Nachfolge (Modul 16 §7) |
| 6 | Behörde | Akte, Ermittlung, Fahndung (Modul 18) |
| 7 | Fall | Fallzustand ändert sich (Ermittlung) |
| 8 | Gerüchte | Gerücht entsteht, verzerrt sich (Modul 07) |
| 9 | Ort | Ortsgedächtnis erhält Ereignis (Modul 05 §9) |
| 10 | Nemesis | Racheketten, falls Täter bekannt (Modul 11) |
| 11 | Dialog | Hooks und Reaktionen anderer NPCs (Modul 20) |
| 12 | Archiv | Eintrag erst, wenn der Spieler es erfährt (Ermittlung 04) |

### 5.2 Verletzung einer Nebenfigur (Stufe B) durch Unfall

NPC `injuries` → Zeitplan (Krankenhaus, Pause) → Haushalt (Pflege) → Betrieb (Ausfall) → Gerücht (klein) → Dialog (Hinweis). Kein Tod, kein Fall.

### 5.3 Spieler rettet einen NPC

Ereignis `Rettung` → Zeugen-Erinnerungen → Kante NPC→Spieler (Schuld, Vertrauen) → Gerücht (‚der Fremde, der …‘) → Dialog (Hooks) → eventuell Medien → Archiv-Hinweis. Beziehungen Dritter (Familie) ändern sich.

### 5.4 Ein Nemesis kehrt zurück und verletzt einen Begleiter

Ereignis `Angriff` → Begleiter `injuries` → Crew (Zeitplan, HQ) → Beziehung Begleiter→Spieler (je nach Zuschreibung) → Nemesis-Modell (Lernen: Begleiter verletzbar) → Gerücht in der Hunter-Szene → Behörden (falls öffentlich) → Fall.

### 5.5 Fraktionskonflikt trifft Zivilisten

Ereignis `Gefecht` → Mitglieder `injuries`/`dead` → Gruppenzustand (Moral, Macht) → Territorium (Anspruch ändert sich) → Zivilisten (Verletzte, Tote, Zeugen) → Polizei → Medien → Gemeinschaftsvariablen (Angst) → Meidung (Modul 15).

### 5.6 Wesen mit menschlicher Identität wird entdeckt

Ereignis `Enttarnung` → Zeugen → Haushalt/Familie (Reaktion) → Betrieb → Behörde/Hunter (Interesse) → Beziehungskanten (Vertrauen bricht oder hält) → Gerüchte → Archiv (Spieler) → Wesen reagiert (Flucht, Verteidigung) → Nemesis-Möglichkeit.

---

## 6. Entwurf: Beobachtungskonsistenz

Die Anforderung (N-KS-005): Der Spieler soll nie Dinge sehen, die dem persistenten Zustand widersprechen.

| Regel | Beispiel |
|---|---|
| **Kein Reset** | Ein verletzter NPC hat bei jedem Laden die Verletzung |
| **Kein Pop** | Ein NPC taucht nicht plötzlich am falschen Ort auf (Modul 22 §4) |
| **Kein Zeitwiderspruch** | Ein NPC kann nicht ‚gestern gestorben‘ und ‚heute im Laden‘ sein |
| **Kein Wissensbruch** | Ein NPC kennt kein Detail, das er nicht erfahren hat |
| **Kein Aussehenwechsel** | Ein NPC sieht ohne Ereignis nicht anders aus |
| **Kein Dialogwiderspruch** | Zwei Zeilen desselben NPCs widersprechen sich nicht ohne Grund (Lüge, Unsicherheit) |
| **Kein Zahlenleck** | UI zeigt keine inneren Werte |
| **Kein Orts-Widerspruch** | Ein zerstörtes Haus bleibt zerstört, bis es repariert wird |
| **Gleichheit über Sitzungen** | Wer in Sitzung 1 sah, dass X Y tat, sieht in Sitzung 2 dasselbe Ergebnis |
| **Gleichheit im Koop** | Alle Spieler sehen dieselbe Welt (Modul 23) |

**Konsistenz heißt nicht Unveränderlichkeit.** Die Welt darf sich ändern; jede Änderung hat eine Ursache und wird nachvollziehbar (N-LT-011).

---

## 7. Entwurf: Autorenschutz und Systemeingriffe

Systemische Simulation darf handgeschriebene Inhalte nicht zerstören (Proposal 0010 §9 ‚Incoherence‘: ‚protected/authored entities‘).

| Schutz | Inhalt |
|---|---|
| **Geschützte Figuren** | `sim_safe`, `story_safe`, `narrative_only` (Modul 03 §8) |
| **Autorenereignisse** | geschriebene Ereignisse laufen im Journal wie systemische, haben aber Vorrang |
| **Hauptstory-Ort** | Orte der Hauptstory sind vor systemischer Zerstörung geschützt, soweit die Story es braucht |
| **Fälle** | handgeschriebene Fälle sind Daten mit Zuständen; systemische Entwicklung darf sie nicht beenden (Hunter-NPC löst einen ignorierten Fall nur in dem Rahmen, den der Fall erlaubt) |
| **Rekruten** | nie notwendig |
| **Fraktionen der Hauptstory** | Ziele geschützt (Modul 11 §12) |
| **Transparenz** | Wenn Simulation und Autorenschutz kollidieren, gewinnt der Schutz; das Ereignis wird als `blocked` im Journal vermerkt |

**Offene Frage.** Wer außer dem Spieler einen wichtigen NPC töten darf (C-N1), ist die Grenze zwischen Autorenschutz und Simulation.

---

## 8. Entwurf: Abhängigkeiten und Reihenfolge der Umsetzung

Das Proposal 0010 §10 skizziert Phasen L0–L5 (unverbindlich). Der Abhängigkeitsgraph der NPC-Dokumentation zeigt, was wovon abhängt (💡):

```text
Kern (portabel):  IDs & Seeds ─► Ereignis ─► Gedächtnis (begrenzt) ─► Beziehung ─► Dialogkontext
                                      │
                                      ├─► Zeitplan/Fortschreiben ─► Lebensereignisse
                                      ├─► Gerüchte ─► Medien
                                      ├─► Fraktion ─► Territorium/Konflikt
                                      └─► Nemesis (braucht Gedächtnis, Beziehung, Artregeln, Fraktion optional)

Unreal-Seite:     Materialisierung ─► Wahrnehmung ─► StateTree ─► Dialog-Laufzeit ─► Streaming
Speicher:         Segmente ─► Migration ─► Verdichtung
```

| Stufe | Inhalt | Voraussetzung |
|---|---|---|
| **L1** | Datenmodell für IDs, Ereignisse und ein begrenztes Gedächtnis als portable Kernbibliothek mit Tests; Entscheidung der Grenze zu `WorldFacts` | Beschlüsse der Q-N03, Q-N04 |
| **L2** | ein benannter NPC mit Gedächtnis im Vertical Slice (Zeuge mit Folgen im Dialog und Fall) | Dialog-/Ermittlungsgrundlagen |
| **L3** | ein wiederkehrender Gegner für eine Kreaturenart | Artregeln, Gedächtnis, Beziehung |
| **L4** | gestufte Simulation über eine kleine Testregion, Messung der Budgets | L1–L3, Messplan |
| **L5** | Fraktionen und Territorien, Skalierung, Koop-Entscheidung | L4 |

Diese Reihenfolge ist **Entwurf** und kürzt den Umfang nicht (`CLAUDE.md` §1, §4).

---

## 9. Sonderfälle

1. **Zwei Ereignisse gleichzeitig mit gegenseitiger Wirkung.** Totale Ordnung (Abschnitt 3.2) entscheidet.
2. **Zyklische Abhängigkeit** (A hängt von B ab, B von A): Zeitordnung löst; Wirkung nur auf spätere Zeit.
3. **Ereignis ohne Besitzer.** Nicht erlaubt; jedes Ereignis hat einen Besitzer.
4. **Besitzer fehlt (Mod entfernt).** Ereignis wird gestoppt; Zustand eingefroren (Modul 23 §8).
5. **Verspätete Ereignisse.** Eingang für nicht geladene Empfänger (Modul 21 §7).
6. **Widerspruch zwischen Autorenereignis und Simulationsereignis.** Autor gewinnt, `blocked`-Vermerk.
7. **Zeitreise ändert die Vergangenheit.** Temporal-World-State-System (N-GE-028).
8. **Replay mit anderer Modmenge.** Ergebnis anders; bewusst (Mod ist Teil des Weltzustands).
9. **Fehler im Besitzer.** Reparatur über Journal (Modul 21 §8).
10. **Massenereignis (Katastrophe).** Viele Ereignisse; Verdichtung nur für Hintergrund (Modul 22 §8).

---

## 10. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-521 | Jede Tatsache hat genau einen Besitzer-Speicher | N-KS-002 |
| N-EN-522 | Leser kopieren keine Tatsachen; sie verweisen per ID | Abschnitt 2 |
| N-EN-523 | Systemübergreifende Wirkungen entstehen nur als Ereignisse, nicht als Direktschreibzugriff | N-KS-003 |
| N-EN-524 | Die Verarbeitung der Ereignisse ist idempotent und in definierter Reihenfolge | Abschnitt 3.2 |
| N-EN-525 | Ein Folgeereignis hat nie einen früheren Zeitstempel als seine Ursache | Abschnitt 3.2 |
| N-EN-526 | Zusammengehörige Änderungen (Tod: Lebenszustand, Haushalt, Fall) gelten alle oder keine | Abschnitt 3.2 |
| N-EN-527 | Der Invariantenkatalog (Abschnitt 4) läuft als automatisierter Test | N-KS-004 |
| N-EN-528 | Ein Autorenschutz verhindert systemische Ereignisse, die ihm widersprechen, und vermerkt sie als blockiert | Abschnitt 7 |
| N-EN-529 | Kein sichtbarer Widerspruch zwischen Systemen nach dem Laden einer Region | N-KS-005 |
| N-EN-530 | Die Umsetzungsreihenfolge beginnt erst, wenn die Mindest-Eingaben existieren | N-KS-012 |
| N-EN-531 | Ein Fall zitiert nur Ereignisse, die im Journal stehen | N-KS-004 |
| N-EN-532 | Eine Verletzung ist in jedem System sichtbar, das die Person darstellt | N-KS-004 |
| N-EN-533 | Mitgliedschaft in Fraktions- und NPC-Datensatz stimmt überein | Abschnitt 2 |
| N-EN-534 | Ein Ereignis ohne Besitzer wird abgelehnt | Abschnitt 9 |
| N-EN-535 | Dieselbe Ereignisfolge ergibt auf jeder Plattform dasselbe Ergebnis | N-SI-012 |

---

## 11. Was fehlt

- Keine Ereignisschemata, keine Abonnentenmodelle, keine Verarbeitungsreihenfolge (nur Prinzipien).
- Offen: wer außer dem Spieler wichtige NPCs töten darf (C-N1).
- Offen: der Umgang mit Zeitreise (Welt 26; Q-N29).
- Offen: das Ereignismodell der Ermittlung in Bezug auf `WorldFacts`.
- Kein Entwurf für die Implementierung von Tests (Modul 29 beschreibt nur die Anforderungen).
