# NPCs 05: Gedächtnis, Erinnerung und Vergessen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, was ein NPC erinnert, wie lange, wie genau und wie sich Erinnerung auf Verhalten auswirkt. Beschlossen sind die Grundsätze (selektiv, nicht perfekt, kein Allwissen, wichtige Ereignisse bleiben, Wiedererkennung); **Datensatz, Verblassen und Verdichtung sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Die Ermittlungssicht (Zeugenaussage, Verhör) steht in [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md); die Monstersicht in [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §8.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Erinnerungsarten und Aufbewahrungsklassen](#2-entwurf-erinnerungsarten-und-aufbewahrungsklassen)
3. [Entwurf: Der Erinnerungseintrag](#3-entwurf-der-erinnerungseintrag)
4. [Entwurf: Wann eine Erinnerung entsteht](#4-entwurf-wann-eine-erinnerung-entsteht)
5. [Entwurf: Verblassen, Verzerren, Verdichten](#5-entwurf-verblassen-verzerren-verdichten)
6. [Wiedererkennung](#6-wiedererkennung)
7. [Entwurf: Wie Erinnerung wirkt](#7-entwurf-wie-erinnerung-wirkt)
8. [Entwurf: Szenarien](#8-entwurf-szenarien)
9. [Erinnerung an Tote, Orte und Zeitstände](#9-erinnerung-an-tote-orte-und-zeitstände)
10. [Entwurf: Gedächtnis von Gruppen und Institutionen](#10-entwurf-gedächtnis-von-gruppen-und-institutionen)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-GE-001 | NPCs erinnern sich an relevante Dinge, die der Spieler getan hat (geholfen, bedroht, gerettet, bestohlen, verletzt, Fahrzeug beschädigt, bei einem Fall belogen) | 📋 | `23#1056A` Frage 23; „ja genau“ in `23#1057E` |
| N-GE-002 | Wichtige NPCs haben Gedächtnis; **wichtige Ereigniserinnerungen bleiben** | 📋 | `M-PD` ‚NPCs and AI‘ |
| N-GE-003 | Das Gedächtnis ist **selektiv**: nicht jede Sekunde wird gespeichert; kurzfristige Informationen verschwinden, wichtige Ereignisse können praktisch dauerhaft bleiben | 📋 Richtung | `23#51A` Abschnitt 9 („Das soll nicht bedeuten, dass jeder NPC jede Sekunde seines Lebens speichert“); bestätigt in `23#52E` Punkt 9 |
| N-GE-004 | Erinnerungen können mit der Zeit **schwächer oder ungenauer** werden | 📋 | `23#1056A` Frage 24; „ja“ in `23#1057E`; `35#268A` Frage 7; „1-13 ja“ in `35#269E` |
| N-GE-005 | Es gibt **Ankererinnerungen**: besonders wichtige Erlebnisse behalten dauerhaft Gewicht; ein NPC vergisst bestimmte Dinge nie komplett, auch wenn andere Werte abklingen | 📋 | `35#268A` Fragen 5–6; „1-13 ja“ in `35#269E` |
| N-GE-006 | Besonders positive oder negative Erlebnisse bleiben stärker haften; ein Memory-Trait bestimmt, wie viele Details ein NPC behält | 📋 | `35#268A` Fragen 8–9; „1-13 ja“ in `35#269E` |
| N-GE-007 | Persönlichkeit bestimmt, wie stark Ereignisse emotional gespeichert werden | 📋 | `35#268A` Frage 10; „1-13 ja“ in `35#269E` |
| N-GE-008 | Ein NPC muss etwas **tatsächlich gesehen, gehört oder erzählt bekommen** haben (oder aus Nachrichten und sozialen Medien erfahren), bevor er davon weiß; kein magisches Allwissen | 📋 | `23#1056A` Frage 25; „ja sicher“ in `23#1057E`; `23#1058A` |
| N-GE-009 | Ein NPC kann eine falsche Überzeugung haben; eine Zeugenaussage ist Information, keine Wahrheit | 📋 | `M-PD` ‚NPCs and AI‘; `23#51A` Abschnitt 9 (‚NPCs können auch falsches Wissen besitzen‘) |
| N-GE-010 | Ein einmaliges, relevantes Treffen macht aus einer Person eine **Known Person**; ein belangloses Gespräch hinterlässt nur eine sehr kleine Erinnerung; ein bedeutsamer Abend erzeugt einen detaillierten Beziehungsdatensatz | 📋 | `35#268A` Fragen 11–13; „1-13 ja“ in `35#269E` |
| N-GE-011 | Beim Wiedersehen erinnert sich ein NPC an Identität, Dauer seit dem letzten Treffen, Verlauf, Stimmung und relevante Absprachen, **aber nicht zwingend perfekt**: „ja aber es kann sein dass er sich nicht mehr genau erinnert“ | 📋 | `35#268A` Frage 14; Antwort in `35#269E`; Präzisierung `35#270A` |
| N-GE-012 | Es gibt Unterschiede zwischen ‚ich kenne dich‘, ‚ich erinnere mich gut‘ und ‚ich kenne dich sehr gut‘ | 📋 | `35#268A` Frage 15; „ja genau“ in `35#269E` |
| N-GE-013 | Ein NPC kann nach langer Zeit überrascht sein, den Spieler wiederzusehen, ihn nach Jahren anders bewerten (wenn der Spieler sich stark verändert hat), und lange Funkstille kann besondere Reaktionen auslösen | 📋 | `35#268A` Fragen 16–20; „16-30 ja“ in `35#269E` |
| N-GE-014 | Nach Jahren ohne Kontakt vergisst ein NPC manche Details, behält aber wichtige Erinnerungen; die Stärke einer alten Beziehung beeinflusst, wie schnell Erinnerungen verblassen | 📋 | `35#272A` Fragen 5–6; „1-50 passt alles“ in `35#273E` |
| N-GE-015 | Ein früherer relevanter Kontakt kann nach 10 Jahren noch erkannt werden; ein NPC erkennt jemanden gelegentlich erst nach kurzem Überlegen; stark verändertes Aussehen erschwert das Wiedererkennen; Stimme, Gang, Name und andere Hinweise helfen | 📋 | `35#272A` Fragen 7–10; „1-50 passt alles“ in `35#273E` |
| N-GE-016 | Ein NPC kann nach langer Funkstille selbst Kontakt aufnehmen; mögliche Wiedersehensreaktionen sind Freude, Überraschung, Distanz, Nostalgie, Ärger oder erneute Anziehung | 📋 | `35#272A` Fragen 11–12; „1-50 passt alles“ in `35#273E` |
| N-GE-017 | Alte Chemie kann sofort wieder da sein oder verschwunden sein; die Beziehung kann sich beim Wiedersehen neu entwickeln, statt den alten Status einfach wiederherzustellen | 📋 | `35#272A` Fragen 13–15; „1-50 passt alles“ in `35#273E` |
| N-GE-018 | NPCs können den Spieler und **auffällige Fahrzeuge** wiedererkennen; Kleidung und Maskierung beeinflussen das | 📋 | `23#1056A` Fragen 28–30; „ja“ beziehungsweise „passt“ in `23#1057E` |
| N-GE-019 | Derselbe Mitarbeiter kann sich später daran erinnern, dass der Spieler schon einmal unter anderer Identität dort war | 📋 | `23#1088A` Frage 67; „61-67 ja“ in `23#1089E` |
| N-GE-020 | Informationen verbreiten sich über Freunde, Familie, soziale Medien und Nachrichten und können dabei falsch, verzerrt, übertrieben oder unvollständig werden | 📋 | `23#1056A` Fragen 26–27; „ja“ in `23#1057E`; `23#1058A` |
| N-GE-021 | Das Gedächtnis wird in Klassen gehalten (permanent, long-term, short-term, disposable); Kurzzeitgedächtnis kann zusammengefasst werden | 📋 Richtung | `35#278A` Fragen 34–35; „12-35 ja“ in `35#279E` |
| N-GE-022 | Aus vielen ähnlichen kleinen Ereignissen entsteht eine kompakte Erinnerung (‚generell positive Geschichte mit X‘), **solange der NPC sich an alles Wichtige einzeln erinnern kann**; Zusammenfassungen ersetzen keine wichtigen Einzelereignisse | 📋 | `35#278A` Frage 36; „ja solange der npc sich an alles einzeln erinnern kann“ in `35#279E` Punkt 36; `35#280A` |
| N-GE-023 | Wichtige Detail-Historie für spielerbezogene Beziehungen bleibt einzeln erhalten | 📋 | `35#278A` Frage 37; „37-39 ja“ in `35#279E` |
| N-GE-024 | Erinnerungsschwund betrifft den NPC, nicht das, was der Spieler im Archiv festhielt | 💡 (Ermittlungs-Auslegung) | [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §4 |
| N-GE-025 | NPCs erinnern sich an verstorbene Personen und Ereignisse; ein Tod kann andere NPCs langfristig beeinflussen; Trauer ist je nach Beziehung unterschiedlich stark und lang | 📋 | `23#1056A` Frage 112; „ja“ in `23#1057E`; `35#272A` Fragen 34–36; „1-50 passt alles“ in `35#273E` |
| N-GE-026 | Orte entwickeln Geschichte (ein Restaurant wird ein bekannter Ort oder schließt) | 📋 | `23#1056A` Frage 113; „ja“ in `23#1057E`; `23#1058A` |
| N-GE-027 | Historische Beziehungen bleiben auch nach Jahrzehnten für Dialoge, Fälle, Familiengeschichten und Weltlore abfragbar | 📋 | `35#272A` Fragen 38–39; „1-50 passt alles“ in `35#273E` |
| N-GE-028 | In der Vergangenheit weiß ein Charakter nichts von Ereignissen seiner Zukunft; die Beziehungshistorie wird zwischen Zeitständen sauber getrennt; Änderungen der Vergangenheit laufen über das allgemeine Temporal-World-State-System | 📋 | `35#272A` Fragen 40–43; „1-50 passt alles“ in `35#273E` |
| N-GE-029 | Die Erinnerung eines NPC wird als Datensatz im Weltzustand geführt; Erinnerungen persistieren wichtige Ergebnisse; ein NPC-Gedächtnis ist von der Zustandslogik getrennt (StateTree: ‚Was mache ich?‘, Gedächtnis: ‚Was weiß ich?‘) | 📋 Richtung | `23#48A` Abschnitt 9; `M-PD` ‚NPCs and AI‘ |
| N-GE-030 | Monster mit individuellem Gedächtnis erinnern sich an Begegnungen, Waffen, Fallen, Verletzungen, Gnade und können Wissen weitergeben | 📋 | `48#139E`; Details in [Kreaturen 10](../Kreaturen/10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) §8 |
| N-GE-031 | Zeugenaussagen und Erinnerungen sind keine Wahrheit: derselbe Spieler kann unter verschiedenen Beobachtern unterschiedlich erinnert werden | 📋 | `M-PD` ‚NPCs and AI‘; `23#603A` Abschnitt 14 (Aussage des Assistenten) |
| N-GE-032 | Wie lange ein NPC etwas behält, welche Genauigkeit gilt und was als ‚relevant genug‘ gilt, ist nicht festgelegt | ❓ | `M-OD`; [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §11 |

---

## 2. Entwurf: Erinnerungsarten und Aufbewahrungsklassen

Zwei Einteilungen der Quellen werden hier zusammengeführt (💡):

**Erinnerungsart** (`23#51A`, Assistent, bestätigt als Richtung N-GE-029):

| Art | Inhalt | Beispiel (Assistent) | Lebensdauer |
|---|---|---|---|
| Kurzzeit (`Short-Term`) | gerade Wahrgenommenes, Spieler vor Sekunden gesehen, Geräusch | ‚Spieler vor 20 Sekunden gesehen‘ | Sekunden bis Stunden; „Kurzfristige Informationen verschwinden.“ |
| Ereignis (`Event Memory`) | konkretes Erlebnis | ‚Spieler hat mich gerettet‘, ‚Familienmitglied wurde getötet‘ | kann sehr lange bleiben: „Wichtige Ereignisse können praktisch dauerhaft bleiben.“ |
| Wissen (`Knowledge`) | Sachwissen und Überzeugung | ‚Ich weiß, dass Monster X existiert‘, ‚Ich kenne Ort Y‘ | solange es nicht widerlegt oder vergessen wird |
| Beziehung (`Relationship Memory`) | Konflikte, Hilfe, Lügen, Vertrauen | ‚Er hat mich belogen‘ | verdichtet in Beziehungszuständen, Einzelereignisse bleiben |

**Aufbewahrungsklasse** (`35#278A` Frage 34):

| Klasse | Sinn | Verdichtung |
|---|---|---|
| permanent | Anker, prägende Ereignisse, Tod naher Personen | nie |
| long-term | relevante Ereignisse, Beziehungen | Details dürfen verblassen; Kern bleibt |
| short-term | frische Beobachtungen | wird zusammengefasst oder verworfen |
| disposable | Beobachtungen ohne Folge | kann sofort verworfen werden |

**Zuordnung (Entwurf):** Jeder Erinnerungseintrag hat *eine Art* und *eine Klasse*. Kurzzeit-Einträge sind meist `short-term`/`disposable`; Anker sind `permanent`. Beim Altern wandern Einträge von `short-term` nach `long-term` (wenn relevant) oder werden verworfen.

---

## 3. Entwurf: Der Erinnerungseintrag

> 💡 **Vorschlag.** Er übernimmt die Felder des Assistenten aus `23#51A` Abschnitt 9 (Ereignis, Ziel, Ort, Zeit, Wichtigkeit, Vertrauen, emotionale Wirkung) und die Beobachtungsfelder aus [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §3. Namen sind unverbindlich.

```text
MemoryEntry
├── id                 stabil im Besitzer-Datensatz
├── owner              NPC-ID
├── kind               short | event | knowledge | relationship
├── retention_class    permanent | long | short | disposable
├── event_ref          Verweis auf Weltereignis im Journal (kann fehlen: Gerücht ohne Ereignis)
├── content            was der Besitzer glaubt (kann von der Wahrheit abweichen)
│   ├── who / what / where / when / how / why  -- je Slot mit eigener Genauigkeit
│   └── identified[]   wen der Besitzer erkannt oder zu erkennen glaubt
├── source             observed(sense) | told(by) | media | inferred | suggested
├── time, location     wann und wo er es erlebte
├── importance         trivial | minor | notable | major | anchor
├── confidence         wie sicher sich der Besitzer ist
├── accuracy           wie nah der Inhalt an der Wahrheit liegt (nur für Tests/Debug; nie sichtbar)
├── valence/emotion    positiv/negativ, Angst, Dankbarkeit, Wut ...
├── distortions[]      verblassen | verschmelzen | auffüllen | beeinflusst | übernatürlich
├── recalled           letzte Abrufzeit, Zahl der Abrufe
├── shared_with[]      wem er es erzählt hat (Modul 07)
└── flags              anchor | secret | traumatic | contested
```

**Entwurfsentscheidungen mit Begründung:**

1. **Inhalt und Wahrheit sind getrennt.** `content` ist der Glaube, `event_ref` die Wahrheit (N-GE-009). Der Test kann prüfen, ob eine Aussage der Wahrheit entspricht; die Oberfläche kann es nicht.
2. **Konfidenz und Genauigkeit sind getrennt.** Ein Zeuge kann sich sehr sicher und trotzdem falsch erinnern (`23#51A`: ‚Fact: Creature = X, NPC Belief: Creature = BlackDog, Confidence = 0.78‘, 💡).
3. **Slots statt Block.** Details verblassen unabhängig voneinander (Farbe vor Anwesenheit, Uhrzeit vor Ereignis). Das macht Verblassen glaubwürdig (N-GE-004).
4. **Quelle.** `told` und `media` verblassen und verzerren anders als `observed` (Abschnitt 5). Wer etwas nur gehört hat, hat keine eigene Sinnesspur.
5. **Nur Spielerrelevantes im Detail.** Für unwichtige Einträge reicht ein kompakter Eintrag (Art, Klasse, Wichtigkeit, Zeit, Ort); die Slots werden erst bei Bedarf materialisiert (vgl. N-ID-014).

---

## 4. Entwurf: Wann eine Erinnerung entsteht

```text
Weltereignis
   │
   ├─ Wer war in Wahrnehmungsreichweite?           -- Modul 06
   ├─ Hat der Beobachter es erkannt / gedeutet?     -- Modul 06
   ├─ Relevanzfilter:  betrifft Ich / Nahestehende / Besitz / Weltbild / Spieler / Gefahr
   │     nein  -> disposable (oder gar nicht)
   │     ja    -> Eintrag mit Wichtigkeit und Emotion
   ├─ Persönlichkeit moduliert Stärke               -- N-GE-007
   └─ Stufe/Tiefe des NPC begrenzt Umfang           -- Modul 03
```

**Regeln (Entwurf):**

1. **Relevanz entscheidet, nicht Dramatik.** Ein unscheinbares Ereignis (der Spieler hielt die Tür auf) kann für einen einsamen NPC relevant sein; ein Autounfall auf der Straße, die er nie nutzt, nicht.
2. **Die Stufe begrenzt den Umfang.** Ein C-NPC speichert weniger Ereignisse mit weniger Slots als ein A-NPC (N-ID-014). Wer in einer *relevanten* Begegnung steht, bekommt dennoch die volle Genauigkeit (N-ID-017).
3. **Wiederholung stärkt.** Ein Ereignis, das erzählt oder wieder erlebt wird, wird stärker; Abruf verlängert die Lebensdauer (Abschnitt 5).
4. **Mehrere Spieler.** Im Koop gelten Erinnerungen **je Spieler als Subjekt**, im Host-Speicherstand (Modul 23). Das ist 💡 und ❓ Q-N15.
5. **Ereignisse ohne Beobachter** erzeugen keine Erinnerung (N-GE-008), nur einen Weltzustand.

---

## 5. Entwurf: Verblassen, Verzerren, Verdichten

### 5.1 Verblassen

| Merkmal | Verhalten (Entwurf) | Quellenlage |
|---|---|---|
| Reihenfolge | Details verblassen vor dem Kern; Zeit und Zahl vor Beteiligten; Beteiligte vor Ereignis; Ereignis vor Gefühl | `35#270A` (‚grob an den Abend, Details verschwimmen, starke Erinnerungen hängen eher‘) |
| Geschwindigkeit | schneller bei trivialer Wichtigkeit, schwachem Gefühl, schlechter Gedächtnisgabe | N-GE-006, N-GE-007 |
| Verlangsamung | stärker bei Anker, starker Emotion, wiederholtem Abruf und enger Beziehung | N-GE-005, N-GE-014 |
| Untergrenze | Anker verblassen im Kern nie; andere Einträge können vollständig verschwinden (disposable) oder in einer Zusammenfassung aufgehen | N-GE-005, N-GE-022 |
| Zeit | verbraucht wird **Weltzeit**, nicht Realzeit; steht die Welt still (Spiel beendet), verblasst nichts | N-LT-020 |

### 5.2 Verzerren

Die Mechanismen aus [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §4 gelten hier als Quelle der Verzerrung: Verblassen, Verschmelzen ähnlicher Ereignisse, Auffüllen von Lücken mit Erwartetem, Beeinflussung durch andere, Stress und Angst, Alkohol und Drogen, Voreingenommenheit, übernatürlicher Einfluss.

**Zusätzliche Entwurfsregeln:**

1. **Verzerrung ist deterministisch.** Dieselbe Erinnerung, derselbe Weltzustand, dieselbe Zeit liefern dieselbe verzerrte Fassung (Seed aus Eintrag-ID und Zeitfenster), damit Fragen an denselben Zeugen konsistent sind ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §4: ‚Keine Wahrheit durch Wiederholung‘).
2. **Verzerrung ist begrenzt.** Pro Zeitfenster höchstens eine Änderung je Slot; eine Erinnerung kippt nicht vom einen ins andere Gegenteil ohne Ursache (N-PS-016).
3. **Gerüchte schieben nach.** Ein NPC, der von anderen hört, übernimmt deren Version ganz oder teilweise (Modul 07).
4. **Erinnerung kann sich verfestigen.** Wer etwas oft erzählt, glaubt zuletzt die eigene Fassung (`confidence` steigt, `accuracy` fällt).

### 5.3 Verdichten

`35#278A` Fragen 35–37 und `35#280A` sagen: Kleinkram wird verdichtet, wichtige Einzelereignisse bleiben einzeln abrufbar, und beides existiert nebeneinander.

```text
Verdichtung (Beispiel des Assistenten, 35#280A):
Summary:  "Mostly positive history with Player"
Details:  - erstes Treffen
          - wichtiger Fall gemeinsam
          - bestimmter Streit
          - Trennung
          - Wiedersehen
```

**Entwurf – Verdichtungsregeln:**

| Regel | Inhalt |
|---|---|
| Kandidaten | Einträge der Klassen `short` und `long` mit Wichtigkeit `trivial` oder `minor` und ähnlichem Inhalt (gleicher Partner, gleiche Art, benachbarte Zeit) |
| Ergebnis | Eine Zusammenfassung mit Tendenz (positiv/negativ/gemischt), Häufigkeit und Zeitraum |
| Ausnahme | Einträge mit Wichtigkeit ab `notable` bleiben einzeln (N-GE-022, N-GE-023) |
| Spielerbezug | Einträge mit dem Spieler als Subjekt werden nur verdichtet, wenn **keiner** als wichtig markiert ist; sonst bleiben alle Einzelereignisse |
| Umkehr | Eine Zusammenfassung kann nicht in Einzelereignisse zerlegt werden; der Speicherstand enthält nur das Ergebnis |
| Begrenzung | Pro NPC gibt es eine **Obergrenze** der Einträge je Klasse; ist sie erreicht, wird zuerst verdichtet, dann verworfen (zuerst disposable, dann trivial); Anker und wichtige Einträge zählen nicht dagegen |

**Die Obergrenze und alle Zahlen sind offen** (Proposal 0010 §4 nennt nur ‚ein begrenztes Gedächtnismodell‘; ❓ Q-N06).

---

## 6. Wiedererkennung

**Quellen:** N-GE-010 bis N-GE-019. Wiedererkennung ist die sichtbarste Wirkung des Gedächtnisses; sie entsteht aus einer **Bekanntheitsstufe**, einer **Wahrnehmung** und einem **Abgleich**.

### 6.1 Bekanntheitsstufen

| Stufe (Quelle N-GE-012) | Bedeutung (Entwurf) | Datentiefe (Modul 03) |
|---|---|---|
| unbekannt | nie gesehen oder nur flüchtig | Minimal oder keiner |
| ‚ich kenne dich‘ | weiß, wer der Spieler ist | Known |
| ‚ich erinnere mich gut‘ | weiß Einzelheiten der Begegnungen | Detailed |
| ‚ich kenne dich sehr gut‘ | kennt Gewohnheiten, Absprachen, Geschichte | Full |

### 6.2 Abgleich (Entwurf)

```text
Wahrnehmung des Gegenübers (jetzt)                      Erinnerung (damals)
- Gesicht (Licht, Entfernung, Winkel)                   - Gesicht, Körperbau
- Stimme, Sprechweise, Gang                             - Stimme, Gang, Namen
- Kleidung, Maske, Frisur, Bart                         - Kleidung, Fahrzeug
- Fahrzeug, Begleiter, Umfeld                           - Umfeld
- Kontext: Hier erwartet?                               - Zeit seit damals
   └────────────── Ähnlichkeit + Kontext + Bekanntheit ───────────────┘
                                 │
                 sicher | zögernd | unsicher | falsch erkannt | nicht erkannt
```

| Einfluss | Wirkung | Quelle |
|---|---|---|
| Bekanntheitsstufe | höhere Stufe erkennt auch bei großer Veränderung | N-GE-012 |
| Zeit | erschwert das Erkennen, verlangsamt ihn (‚erst nach kurzem Überlegen‘) | N-GE-015 |
| Veränderung des Aussehens (Gewicht, Haare, Narben, Tattoos, Alter) | erschwert | N-GE-015, N-LT-022 |
| Maske, Verkleidung, Kleidung | erschwert; bei guter Verkleidung kann es misslingen | N-GE-018 |
| Stimme, Gang, Name, Begleiter | helfen | N-GE-015 |
| Fahrzeug | Wiedererkennung über Fahrzeug unabhängig vom Gesicht | N-GE-018 |
| Kontext | unerwarteter Ort senkt Sicherheit | 💡 |
| Zustand des Beobachters | Rausch, Müdigkeit, Stress, Brille/Sehschwäche | N-PS-011 |
| Einmalige Begegnung mit hohem Gewicht | wird auch nach 10 Jahren erkannt | N-GE-015 |

**Falsche Erkennung (Entwurf).** Ein NPC kann jemanden fälschlich zu erkennen glauben (Ähnlichkeit, Erwartung). Das ist Teil der Ermittlung (Zeugen verwechseln) und der Formwandler ([Ermittlung 13](../Ermittlung/13-Identitaet-Ruf-und-Formwandler.md)). Die Quellen nennen dies nicht ausdrücklich; es folgt aus ‚Beliefs können falsch sein‘ (N-GE-009).

### 6.3 Ergebnis der Wiedererkennung

Die Reaktion beim Wiedersehen kommt aus Erinnerung und Beziehungszustand ([Modul 08](08-Beziehungen-Modell-und-Wirkung.md)): Freude, Überraschung, Distanz, Nostalgie, Ärger, erneute Anziehung (N-GE-016), Misstrauen, Angst (wenn die Erinnerung schlecht ist). Das Ergebnis ist ein Dialogkontext (Modul 20), kein Zahlenwert.

**Sonderfall Verkleidung** (`23#1088A` Fragen 61–67): Wer sich als Techniker ausgibt und zum zweiten Mal kommt, kann von einem Mitarbeiter wiedererkannt werden – auch wenn er sich diesmal anders verkleidet (N-GE-019). Der Mitarbeiter erinnert sich an die *frühere Rolle*, nicht an die echte Identität; das Ergebnis ist Verdacht, nicht Wissen.

---

## 7. Entwurf: Wie Erinnerung wirkt

Erinnerung ist nur wirksam, wenn sie **abgerufen** wird. Ein Dauerscan aller Einträge wäre teuer (Modul 22) und unnötig. Der Entwurf lässt Erinnerung durch **Auslöser** wirken:

| Auslöser | Beispiel | Abruf |
|---|---|---|
| Begegnung | NPC sieht den Spieler | Wiedererkennung (Abschnitt 6) |
| Dialog | Spieler spricht ihn an | Einträge, die zum Gesprächsthema oder zum Beziehungskontext passen (Remembered-Event-Hook, `M-P05` Entwurf 013) |
| Entscheidung | NPC soll helfen, verraten, schweigen | Einträge zu Schuld, Misstrauen, Interessen |
| Ereignis | Neuigkeit betrifft den Spieler oder Nahestehende | Abgleich mit Einträgen |
| Ort | NPC kommt zu einem Ort mit Bedeutung | Eintrag zum Ort |
| Jahrestag, Feiertag | Todestag, Hochzeitstag | persönliche Anker |

**Wirkungskanäle** (Entwurf; Quellenbasis `M-PD` ‚NPCs and AI‘: Beziehungen werden kommuniziert über ‚tone, wording, body language, cooperation, information, access, favors and consequences‘):

1. **Ton und Wortwahl:** warm, kühl, vorsichtig, angespannt.
2. **Zusammenarbeit:** hilft, verweigert, stellt Bedingungen.
3. **Information:** gibt mehr oder weniger preis ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §5, Aussagebereitschaft).
4. **Zugang:** öffnet Türen, Archive, Kontakte (zum Beispiel der Sheriff mit dem Zugang zu Polizeidaten, `23#51A`).
5. **Gefallen und Schuld:** erinnerte Hilfe erzeugt Verpflichtung.
6. **Folgen:** meldet den Spieler, warnt andere, verbreitet Gerüchte (Modul 07).

**Das konkrete Beispiel der Quelle (💡, `23#51A`):** Der Spieler rettet einen Sheriff in Stunde 15. 40 Stunden später braucht er Zugang zu Polizeidaten. Statt eines Schwellenwerts, Zitat: „Nach dem, was damals auf der Farm passiert ist, schulde ich Ihnen ohnehin etwas.“ Der Assistent folgerte: „Das Spiel weiß warum er dir vertraut.“ (Der Entwickler: „ja genau, aber der spieler sieht nicht ob der npc ihn mag es merkt es am ton… das mit dem sheriff können wir so machen“, `23#52E` Punkt 9.)

---

## 8. Entwurf: Szenarien

Die Prompt-Entwürfe der Spur 05 nennen vier Standardszenarien, die das Gedächtnis tragen soll (`M-P05`: ‚saved me‘, ‚threatened me‘, ‚supernatural witness‘, ‚family loss‘). Sie sind nur Titel, keine Spezifikation (Status DRAFT); der Entwurf ergänzt Zustände:

| Szenario | Erinnerung (Entwurf) | Wirkung | Besonderheit |
|---|---|---|---|
| **Gerettet** | Anker, positive Valenz, hohe Wichtigkeit; wer, wo, wann, wie | Dankbarkeit, Zusage von Hilfe, Zugang, Erwähnung gegenüber Dritten | Rettung durch den Spieler bleibt auch, wenn der Spieler später etwas Schlechtes tut |
| **Bedroht** | negative Valenz, Angst oder Wut; Drohung, Waffe, Ort | Meidung, Hass, Anzeige, Rache, Gehorsam | Wirkung hängt vom Kern (Mut, Wertegerüst) ab |
| **Übernatürlicher Zeuge** | Beobachtung mit starker Verzerrung; Deutung nach Weltbild | Glaube, Unglaube, Verdrängung, Gerücht, Angst | `23#51A`: Zeuge ist überzeugt, es sei ein schwarzer Hund gewesen; Deutung folgt Weltbild (Modul 15) |
| **Familienverlust** | Anker; Trauer; Schuldfrage | veränderte Persönlichkeit, Rachemotiv, Hilfebereitschaft | Der Rettende bleibt in Erinnerung, auch wenn die Rettung zu spät kam (`23#51A`: ‚Spieler hat mich gerettet → Familienmitglied wurde getötet‘) |
| **Bestohlen, beschädigt** | negative Valenz, Verlust; Täter erkannt oder nicht | Meldung, Misstrauen, Racheabsicht | Fahrzeug-Wiedererkennung (N-GE-018) |
| **Belogen** | Entdeckung der Lüge, Zeitversatz | Vertrauensverlust, Rückfragen, Verdacht | Erinnerung an die Lüge kann später auffliegen (Ermittlung 12) |
| **Beobachtet bei Einbruch** | Konfidenz hoch, Wichtigkeit mittel | Anzeige, Erpressung, Schweigegeld (`23#1057E` Punkt 48) | `23#51A`: ‚Event: SawPlayerBreakIntoHouse, Importance: Medium, Confidence: 0.92‘ als Beispiel |

---

## 9. Erinnerung an Tote, Orte und Zeitstände

**Tote.** Nach dem Tod eines NPCs bleiben (N-ID-018, N-GE-025, N-GE-027):

- seine Identität und wichtige Historie im Weltzustand,
- die Erinnerungen *anderer* NPCs an ihn (inklusive Trauer, Groll, Schuld),
- Orte, die mit ihm verbunden sind (Grab, Haus, Stammlokal),
- offene Fälle und Beziehungen, die auf ihn verweisen.

Der Detailzustand des Toten (Position, Routine, Tagesstände) darf verdichtet werden (`35#272A` Frage 33). Offen ist, ob personenbezogene Vorlieben nach dem Tod vollständig erhalten bleiben (N-ID-024, ⚠️).

**Trauer (Entwurf).** Trauer ist ein mittlerer bis langer Zustand (Modul 04), der Stärke und Dauer aus Beziehungsnähe, Persönlichkeit und Umständen bezieht (N-GE-025). Der Verlust eines Partners kann langfristige Auswirkungen auf Verhalten und Beziehungsbereitschaft haben (`35#272A` Frage 35).

**Orte.** Orte sammeln Geschichte (N-GE-026): Ereignisse, die dort geschahen, wirken auf Ruf, Besucherzahl und Gerüchte (siehe [Kreaturen 16](../Kreaturen/16-Uebernatuerliche-Orte-und-Geografie.md) für übernatürliche Orte; [Ermittlung 26](../Ermittlung/26-Notfaelle-und-Weltereignisse.md) für Ereignisfolgen). Entwurf: Ein Ort hat ein *Ortsgedächtnis* (öffentlich bekannte Ereignisse mit Sichtbarkeit), das NPCs aus dem Alltag kennen und das Gerüchte speist.

**Zeitstände.** Zeitreise und Reiche unterliegen dem allgemeinen Zeitzustandssystem (N-GE-028; [Welt 26](../World/26-Reiche-Zeitreise-und-historische-Weltzustaende.md)). Konsequenz: Ein NPC-Datensatz, den man in der Vergangenheit trifft, enthält keine Erinnerungen an Ereignisse, die für ihn erst später geschehen (N-EN-044). Änderungen an der Vergangenheit verändern abgeleitete Beziehungsgeschichte nur über das Zeitzustandssystem, nie über eine Sonderlogik des NPC-Systems.

---

## 10. Entwurf: Gedächtnis von Gruppen und Institutionen

Nicht alles Wissen liegt in Personen. Die Quellen kennen Behörden, Archive, Zeitungen und Organisationen als Wissensträger (`CANONICAL_ADDENDUM_2026-09-05.md`: Gemeinschaften, Jägerfamilien, Gelehrte, Archive, Militär und ‚rival organizations‘ halten verschiedene Wissensfragmente; Modul 01 F8; [Ermittlung 04](../Ermittlung/04-Archiv-Journal-und-Hunter-Record.md), [Ermittlung 17](../Ermittlung/17-Geheimhaltung-und-oeffentliches-Wissen.md)).

| Träger | Was er erinnert (Entwurf) | Besonderheit |
|---|---|---|
| Behörde (Polizei, Gericht) | Akten, Fahndung, Vorstrafen | Wissen ist **dokumentiert** und überdauert Personen; Zugriff hängt von Beamten ab |
| Medien | berichtete Ereignisse, Archivartikel | öffentlich, durchsuchbar, aber verzerrt |
| Organisation (Hunter, Gang, Kult) | Mitglieder, Feinde, Taktiken | geteilter Pool mit Rollenrechten (Modul 17) |
| Gemeinschaft (Nachbarschaft, Stadt) | Ortsereignisse, Legenden | informell, schwankend, wandert |
| Familie | Geschichte, Verlust, Schulden | über Generationen (N-LT, Modul 10) |

**Regeln (Entwurf):**

1. **Dokumentiertes Wissen verblasst nicht**, aber es kann **verloren gehen** (Brand, Vernichtung, Aktenvernichtung) oder **unzugänglich** sein.
2. **Ein Individuum weiß nicht, was seine Organisation weiß**, wenn es keinen Zugriff hat (Informationsasymmetrie; Hunter-Organisationen: [Kreaturen 11](../Kreaturen/11-Hunterwelt-Organisationen-und-Kulte.md)).
3. **Kollektives Gedächtnis ist robuster, aber träge.** Ein Gerücht wird erst nach mehreren unabhängigen Wiederholungen zum festen Wissen (Modul 07).

---

## 11. Sonderfälle

1. **Gedächtnislücke durch Besessenheit, Bann oder Rausch.** Der Besitzer hat keinen Eintrag oder einen unklaren (`23#1074A` Fragen 142–143: Besessenheit mit Erinnerungslücken, nach [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §4). Die Lücke ist ein Zustand: Der NPC weiß, *dass* er etwas nicht weiß.
2. **Manipulierte Erinnerung.** Übernatürliche Eingriffe oder Drogen können Einträge ändern oder einfügen (`source = suggested`). Der Eingriff ist im Weltzustand als Ereignis geführt; das Spiel kann ihn aufdecken.
3. **Lüge.** Ein NPC erinnert sich an die Wahrheit und sagt etwas anderes; das ist **Aussage**, nicht **Erinnerung** ([Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §2). Zwei Einträge: Wahrheit und das, was er erzählt hat (`shared_with`).
4. **Zwei NPCs erinnern widersprüchlich.** Normalfall. Es gibt keine Instanz, die den Widerspruch auflöst; der Spieler kann ihn untersuchen.
5. **Der NPC will etwas vergessen.** Verdrängung ist Teil mancher Persönlichkeiten (`23#1058A`: ‚manche verdrängen Erlebnisse‘). Entwurf: Ein Eintrag kann `suppressed` sein; er wirkt dann im Verhalten, aber nicht in der Aussage.
6. **Erinnerung an etwas, das nie geschah.** Ein Gerücht kann zur Erinnerung werden (`event_ref` leer). Prüfregel: Ein Fall darf sich nicht auf ein Ereignis berufen, das nicht geschah (Proposal 0010 §7).
7. **Zeugen mit identischem Gedächtnis.** Verboten. Zwei Zeugen derselben Szene haben verschiedene Einträge (Perspektive, Zustand, Erwartung).
8. **Erinnerungsüberlauf.** Ein Spieler, der tausend Menschen begegnet: Verdichtung und Obergrenze (Abschnitt 5.3); ein Anker pro Mensch genügt nicht, wenn er nichts bedeutet.
9. **Mehrere Spieler.** Siehe Q-N15.
10. **Wiederkehr nach langer Spielpause.** Weltzeit zählt; was real Jahre her ist, aber nur Stunden Weltzeit, ist frisch (N-LT-020).

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-041 | Ein NPC weiß nur Dinge, für die ein Eintrag mit Quelle (gesehen, gehört, erzählt, Medien, abgeleitet) existiert | N-GE-008 |
| N-EN-042 | Der Inhalt eines Eintrags darf von der Wahrheit abweichen, der Verweis auf das Ereignis nicht | N-GE-009 |
| N-EN-043 | Ein Eintrag der Klasse `permanent` wird nie verworfen und verblasst im Kern nie | N-GE-005 |
| N-EN-044 | Ein NPC im Zeitstand T kennt keine Ereignisse nach T | N-GE-028 |
| N-EN-045 | Verdichtung entfernt nie einen Eintrag ab Wichtigkeit `notable` | N-GE-022 |
| N-EN-046 | Dieselbe Frage an denselben Zeugen im selben Weltzustand liefert dieselbe Antwort | Ermittlung 12 §4 |
| N-EN-047 | Verblassen verbraucht Weltzeit, keine Realzeit; ein beendetes Spiel verändert keinen Eintrag | N-LT-020 |
| N-EN-048 | Ein Ereignis ohne Beobachter erzeugt keinen Eintrag bei NPCs | N-GE-008 |
| N-EN-049 | Der Tod eines NPCs entfernt weder die Erinnerungen anderer an ihn noch die Beziehungsgeschichte | N-GE-025, N-ID-018 |
| N-EN-050 | Wiedererkennung ist abhängig von Bekanntheitsstufe, Zeit, Veränderung und Kontext und ist reproduzierbar | N-GE-015 |
| N-EN-051 | Konfidenz und Genauigkeit eines Eintrags sind nie in der Spieler-Oberfläche sichtbar | N-LT-012 |
| N-EN-052 | Ein wichtiger Eintrag mit Spielerbezug bleibt nach Speichern und Laden unverändert einzeln abrufbar | N-GE-023 |
| N-EN-053 | Ein NPC kann nicht über einen Weg erfahren, den es im Weltzustand nicht gibt (Ort, Kontakt, Medium) | N-GE-008, Modul 07 |
| N-EN-054 | Bei Verkleidung erinnert ein NPC die erlebte Rolle, nicht die wahre Identität | N-GE-019 |
| N-EN-055 | Ein Eintrag mit `suggested` als Quelle hat ein Ereignis im Journal, das die Manipulation belegt | Abschnitt 11 |

---

## 13. Was fehlt

- Keine Datenstruktur, keine Obergrenzen, keine Abklingformeln, keine Schwellen für ‚relevant‘ (N-GE-032, Q-N06).
- Offen, ob Erinnerung ein reiner Datensatz oder auch ein Teil des Zustands der Zustandslogik ist (Proposal 0010 §11).
- Offen, wie das Gedächtnis mehrerer Spieler im Koop gespeichert wird (Q-N15).
- Offen, ob NPCs bewusst `suppressed`-Erinnerungen haben dürfen (Verdrängung) und wie das testbar ist.
- Die Quellen enthalten nichts zur Erinnerung **aneinander** (NPC an NPC) außer Beziehungshistorie (`35#278A` Fragen 25–30) und Gerüchtekette; die Verallgemeinerung ist Entwurf.
