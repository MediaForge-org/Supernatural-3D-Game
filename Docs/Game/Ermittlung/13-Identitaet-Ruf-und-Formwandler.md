# Ermittlung 13: Identität, Ruf und Formwandler

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, **wer jemand ist, wofür er gehalten wird und was andere über ihn glauben**: das Ruf- und Identitätssystem, Formwandler und die Wirkung falscher Beweise auf den Spieler. Beschlossen ist das System (vollständig dynamischer Ruf mit Gerüchten, Falschinformationen und geheimen Identitäten). Alles Weitere ist **Entwurf** (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Voraussetzungen: [Modul 10](10-Beweislogik-und-Wissensmodell.md), [Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Begriffe von Identität](#2-entwurf-begriffe-von-identität)
3. [Entwurf: Die vier Ebenen der Wahrheit](#3-entwurf-die-vier-ebenen-der-wahrheit)
4. [Entwurf: Ruf als Menge von Überzeugungen](#4-entwurf-ruf-als-menge-von-überzeugungen)
5. [Entwurf: Gruppen, Regionen und Bekanntheit](#5-entwurf-gruppen-regionen-und-bekanntheit)
6. [Entwurf: Formwandler und Identitätsmissbrauch](#6-entwurf-formwandler-und-identitätsmissbrauch)
7. [Entwurf: Entlastung und Rehabilitierung](#7-entwurf-entlastung-und-rehabilitierung)
8. [Entwurf: Erkennen von Verwandelten](#8-entwurf-erkennen-von-verwandelten)
9. [Zugehörigkeiten, Infiltration und Spitzel](#9-zugehörigkeiten-infiltration-und-spitzel)
10. [Tarnidentität und Geheimhaltung](#10-tarnidentität-und-geheimhaltung)
11. [Sonderfälle](#11-sonderfälle)
12. [Abhängigkeiten](#12-abhängigkeiten)
13. [Ergänzung: Tattoos als Erkennungsmerkmal](#13-ergänzung-tattoos-als-erkennungsmerkmal)
14. [Was fehlt](#14-was-fehlt)

---

## 1. Was beschlossen ist

### 1.1 Quiz zum Rufsystem

Frage `48#152A`: „Welches Ruf- und Reputationssystem möchtest du?“ Optionen: A (einfacher Ruf), B (Gruppen: Polizei, Hunter, Monster, Zivilisten mit eigenen Bewertungen), C (individuelle und regionale Reputation), D (vollständig dynamischer Ruf: Gerüchte, Falschinformationen, geheime Identitäten, widersprüchliche Meinungen, gesellschaftliche Folgen).

Antwort `48#153E`: „man kann auf jedenfall auch gesucht werden, wenn ein formwandler oder andere wesen dir irgendwas in die schuhe schieben, aber man kann auch unter jägern einen guten ruf haben und so weiter, also sehr komplex, also D plus noch extras“.

| Aussage | Status |
|---|:---:|
| Das Rufsystem entspricht **D** (vollständig dynamisch) **plus Extras** | 📋 |
| Man kann gesucht werden, wenn **Formwandler oder andere Wesen** dem Spieler etwas „in die Schuhe schieben“ | 📋 |
| Man kann gleichzeitig unter **Huntern** einen guten Ruf haben | 📋 |
| Das System ist „sehr komplex“ | 📋 |

### 1.2 Ältere Entscheidungen zu Reputation

| Aussage | Status | Quelle |
|---|:---:|---|
| Kein sichtbarer „Reputation +10“-Balken; Beziehungen bleiben überwiegend intern | 📋 | Frage 40 `23#1056A`, „ja“ (`23#1057E`) |
| Eine Stadt oder Community kann den Spieler anders wahrnehmen, wenn er dort bekannt ist (lokale Reputation) | 📋 | Frage 41 |
| Andere Hunter kennen eventuell seinen Ruf | 📋 | Frage 42 |
| Wiederholte Probleme mit derselben Polizei können deren Verhalten ändern | 📋 | Frage 43 |
| Bestimmte kriminelle Gruppen können wissen, wer der Spieler ist | 📋 | Frage 44 |
| Medienbekanntheit durch große Ereignisse ist möglich, aber **selten** | 📋 | Frage 45, Antwort „ja, kann passieren aber nicht oft“ |
| Der Spieler sieht nicht, ob ein NPC ihn mag; er merkt es am Ton | 📋 | `23#52E` Punkt 9; `23#53A` |
| Werte wie Vertrauen, Angst, Respekt dürfen intern existieren, der Spieler sieht sie nicht | 📋 | `23#53A` (Ausformulierung), Zustimmung `23#54E` („das mit den npcs habe ich genau so gemeint“) |
| NPCs erkennen Spieler, Fahrzeuge und Verkleidung wieder | 📋 | Fragen 28–30 |

### 1.3 Beschlossene Ergänzungen im Quiz

| Aussage | Status | Quelle |
|---|:---:|---|
| Polizei und Strafverfolgung **vollständig dynamisch**, mit Zuständigkeiten, verdeckten Ermittlungen, **Identitätsdiebstahl**, **Beweismanipulation**, Korruption und Langzeitfolgen | 📋 (Option D) | `48#154A`, `48#155E` |
| Die Polizei soll nicht allwissend sein; falsche Schlussfolgerungen der Behörden sind möglich | 💡 des Assistenten, im Einklang mit `M-PD` | `48#156A` |
| Öffentliches Wissen über das Übernatürliche ist regional unterschiedlich und kann sich ändern | 📋 | `48#147E` (Option D der Frage `48#146A`) |

Die Tabelle der Ausformulierung `48#154A` (Individueller Ruf, Hunter-Reputation, Polizei und Fahndung, Monster-Reputation, Regionale Bekanntheit, Identitätsdiebstahl, Gerüchte und Manipulation, Langzeitfolgen) ist ein Vorschlag des Assistenten (💡), der die gewählte Option D inhaltlich auffächert.

---

## 2. Entwurf: Begriffe von Identität

> [!NOTE]
> **Nur Vorschlag (💡).** Die Quellen benutzen „Identität“ für mehrere Dinge. Die Begriffe trennen sie.

| Begriff | Bedeutung | Beispiel |
|---|---|---|
| **Körper** | die physische Person | der Spieler-Hunter |
| **Erscheinung** | wie jemand aussieht (Gesicht, Statur, Kleidung, Fahrzeug) | was Kameras und Zeugen sehen |
| **Name und Papiere** | amtliche Identität (Ausweis, Konten, Fahrzeugregistrierung) | was Behörden zuordnen |
| **Rolle** | gesellschaftliche Rolle (Hunter, Kunde, Bürger, Gangmitglied) | was andere in ihm sehen |
| **Ruf** | was andere über die Person glauben (Abschnitt 4) | ‚gilt als zuverlässig‘ |
| **Tarnidentität** | bewusst erzeugte falsche Identität (`48#148A` Option C: „Tarnidentitäten“) | falsche Papiere und Rolle |
| **Wesen unter der Erscheinung** | Formwandler, Besessene, Verwandelte (`23#1076A`) | Person sieht aus wie X, ist Y |

Die Quellen nennen alle sieben, trennen sie aber nicht. Jede der Ebenen kann **unabhängig** manipuliert werden (Papiere gefälscht, Erscheinung imitiert, Ruf zerstört).

---

## 3. Entwurf: Die vier Ebenen der Wahrheit

Der Assistent unterschied beim Beispiel eines Formwandlers (`48#154A`, 💡): **Was tatsächlich passiert ist; was Zeugen und Kameras beobachtet haben; wen die Beteiligten für verantwortlich halten; was Behörden, Organisationen und NPCs darüber glauben.** Zweck: Ein Formwandler soll glaubwürdig belasten können, „ohne dass die Spielengine selbst die Wahrheit verliert“.

| Ebene | Inhalt | Wer „besitzt“ sie | Wo gespeichert (Entwurf) |
|---|---|---|---|
| **E1 Wirklichkeit** | Was wirklich geschah (wer, was, wann, wo) | die Welt | Fallwahrheit ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) |
| **E2 Beobachtung** | Was Zeugen, Kameras, Sensoren aufgezeichnet haben | Beobachter (Beobachtungseinträge, [Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | pro Beobachter |
| **E3 Zuschreibung** | Wen Beteiligte für verantwortlich halten | jeder Beteiligte | pro NPC/Gruppe |
| **E4 Behördenüberzeugung** | Was Polizei, Gericht, Organisationen als Stand führen | Institution | pro Behörde |

**Beispielablauf (Formwandler begeht ein Verbrechen in der Gestalt des Spielers, nach `48#154A`):**

| Schritt | E1 | E2 | E3 | E4 |
|---|---|---|---|---|
| Tat | Formwandler W tötet Person P | Kameras und Zeugen sehen „den Spieler“ | niemand | niemand |
| Meldung | – | Zeugen berichten | Zeugen schreiben es dem Spieler zu | Polizei nimmt Anzeige auf |
| Ermittlung | – | Kameras werden ausgewertet | Polizei hält den Spieler für verdächtig | Fahndung gegen den Spieler (E4) |
| Medien | – | Bild des Spielers in den Nachrichten | Bevölkerung hält ihn für den Täter (Gerüchte) | Behörde bestätigt den Verdacht |
| Hunter | – | – | Erfahrene Hunter erwägen einen Formwandler | Hunter-Gemeinschaft kann dem Spieler vertrauen oder nicht |
| Entlastung | – | Neue Beweise (zweite Kamera, Alibi, Anomalie) | Verdacht verschiebt sich | Behörde revidiert oder bleibt (Korruption/Fehler) |

**Grundregel (Entwurf):** *E1 ändert sich nie durch Glauben.* E2 bis E4 ändern sich durch Ereignisse und Informationen. Das Spiel darf also wahr und falsch zugleich ‚im Umlauf‘ haben, ohne die Wirklichkeit zu verlieren.

---

## 4. Entwurf: Ruf als Menge von Überzeugungen

### 4.1 Idee

Statt eines Balkens (verworfen: Frage 40, kein sichtbarer Balken) ist der Ruf eine **Menge von Überzeugungen einzelner Beobachter** (NPCs, Gruppen, Behörden). Ein „Ruf“ ist eine Zusammenfassung davon, die das Spiel für Entscheidungen verwendet (Wie reagiert NPC X? Wie wirkt der Spieler in Stadt Y?).

### 4.2 Katalog möglicher Felder (Beobachter → Spieler)

Die Prompt-Sammlung und der Assistent nennen für Beziehungen **Vertrauen, Furcht, Verdacht** und Wissensmarken (`48#150A`, Codeskizze des Assistenten mit `trust`, `fear`, `suspicion`, `knowsSupernaturalExists`, `knowsPlayerIsHunter`; `23#53A`: Vertrauen, Angst, Respekt). Ein Entwurf:

| Feld | Inhalt |
|---|---|
| Vertrauen | wie sehr der Beobachter dem Spieler vertraut |
| Furcht | wie sehr der Beobachter ihn fürchtet |
| Respekt | wie sehr er ihn achtet |
| Verdacht | wie sehr er ihn verdächtigt (konkret: wofür) |
| Sympathie | persönliche Zuneigung |
| Wissensmarken | weiß er, dass der Spieler Hunter ist? weiß er, dass es Übernatürliches gibt? |
| Belege | worauf stützt er sich (eigene Erfahrung, Gerücht, Medien, Behörde) |
| Aktualität | wann zuletzt bestätigt (für Verblassen) |

Die Zahlen existieren intern, der Spieler sieht sie nie (`23#53A`). Eine **Skala** ist nicht festgelegt.

### 4.3 Aggregation

| Ebene | Entsteht aus | Nutzung |
|---|---|---|
| **Individuell** | einzelner NPC (Tier A/B, [Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Dialog, Hilfe, Verrat |
| **Gruppe** | Mitglieder und Führung einer Gruppe (Polizeirevier, Hunter-Netz, Gang, Clique) | Verhalten der Gruppe |
| **Regional** | Stimmung einer Stadt oder Region | Gerüchte, Hilfsbereitschaft, Fahndungseifer |
| **Öffentlich** | Medien und allgemeines Wissen | Bekanntheit, Bild in den Nachrichten |

Die **Aggregation** ist eine Rechenregel, die die Quellen nicht nennen. Sie muss vermeiden, dass ein einzelner Balken wieder entsteht. Beispiel (💡): Regionale Reputation ist eine **Verteilung** (viele halten ihn für X, einige für Y), nicht ein Mittelwert; so kann sie widersprüchlich sein (`48#152A` Option D: „widersprüchliche Meinungen“).

### 4.4 Wie Ruf sich ändert

| Auslöser | Wirkung |
|---|---|
| Beobachtete Handlung | Beobachter bildet Überzeugung (Z3 in Modul 12) |
| Gerücht | Überzeugung ohne eigene Erfahrung (Z4) |
| Medien | breite Wirkung |
| Behördenaussage | wirkt auf Behörden und Folgende |
| Zeit | Überzeugungen verblassen oder verfestigen sich |
| Gegenbeweis | Überzeugung wird revidiert (oder nicht, bei Voreingenommenheit) |
| Wiedergutmachung | Beziehungen reparieren |

Die Dynamik der **Langzeitfolgen** ist beschlossen (Option D), die Formeln nicht.

---

## 5. Entwurf: Gruppen, Regionen und Bekanntheit

| Gruppe/Ebene | Ruf-Beispiele (erfunden) | Quelle der Anforderung |
|---|---|---|
| **Polizei** (Revier, Sheriff, State Police) | als hilfsbereit, als Problem, als verdächtig bekannt | Frage 43; `23#53A` (Beispiel „Sheriff“: nach Jahren erinnert sich der Sheriff an eine Rettungsaktion nicht mit „+20 Reputation“, sondern mit einem Satz) |
| **Hunter** | als zuverlässig, als Rivale, als Verräter | Frage 42; `48#153E` |
| **Monster** | gefürchtet, respektiert, gehasst, gejagt | `48#154A` (💡), `48#139E` (Monster-Gedächtnis) |
| **Kriminelle Gruppen** | Spieler ist bekannt | Frage 44 |
| **Zivilisten** | Nachbarn, Händler, Kunden | Fragen 40–41 |
| **Medien** | selten berühmt | Frage 45 |

**Regionale Bekanntheit.** Der Spieler kann in einer Stadt als Held gelten und in einer anderen als gesuchter Verbrecher (`48#154A`, 💡; stimmt mit `48#153E` überein). Das bedeutet: Fahndung und Reputation sind **ortsgebunden**, mit Ausbreitung über Zuständigkeiten (Modul 14) und Medien.

**Hunter-Ruf getrennt von Polizei-Ruf.** Beschlossen (`48#153E`): Man kann unter Huntern einen guten Ruf haben, obwohl die Polizei einen sucht.

---

## 6. Entwurf: Formwandler und Identitätsmissbrauch

### 6.1 Beschlossen

Formwandler und andere Wesen können dem Spieler etwas in die Schuhe schieben (`48#153E`). Option D der Justiz enthält ausdrücklich **Identitätsdiebstahl** und **Beweismanipulation** (`48#154A`, gewählt in `48#155E`). Intelligente Wesen leben unter menschlichen Identitäten (`23#1076A`, Zusammenfassung des Entwicklerstands).

### 6.2 Offen: Was ein Formwandler kann

Die **Fähigkeiten** der Art (wie genau die Kopie ist, ob Stimme, Fingerabdrücke, DNA, Erinnerung kopiert werden, wie lange die Gestalt hält, wie sie gebrochen wird) sind **nicht festgelegt**. Sie hängen an der Monster-Mythologie, die erst entschieden werden muss (`48#124A`: „legen wir anhand unserer endgültigen Monster-Mythologie fest“). Der Entwurf behandelt deshalb **Fähigkeitsstufen** statt Einzelheiten:

| Stufe (Arbeitsname) | Imitation | Folgen für Beweise |
|---|---|---|
| **Optisch** | Aussehen und Stimme, aber keine Biologie | Kameras und Zeugen täuschen; Forensik (DNA, Fingerabdruck) widerspricht |
| **Biologisch** | Aussehen plus Spuren (DNA/Fingerabdrücke) | nur Anomalien oder Verhalten verraten |
| **Sozial** | zusätzlich Wissen und Gewohnheiten der Zielperson | Befragung deckt Lücken nur über tiefes Wissen auf |
| **Vollständig** | zusätzlich Erinnerungen | nahezu unerkennbar; nur Mythologie-Tests |

Welche Stufe zu welcher Art gehört, ist offen (Q-E52).

### 6.3 Wege, den Spieler zu belasten (Entwurf)

| Mittel | Beispiel (erfunden) | Wirkt auf |
|---|---|---|
| Tat in Gestalt des Spielers | Mord vor Kamera | E2, E3, E4 |
| Gefälschte Dokumente | Quittung am Tatort | E4 |
| Platzierte Gegenstände | Waffe im Fahrzeug des Spielers | E4 |
| Gerüchte | Nachbarn „wissen“ es | E3 |
| Täuschung von Hunter-Kontakten | falsche Nachricht im Namen des Spielers | E3 |
| Korrupter Beamter | Akte „verschwindet“ oder wird „ergänzt“ | E4 |

### 6.4 Wege der Täuschung durch Menschen

Der Formwandler ist ein Beispiel. Dieselben Mittel stehen **Menschen** offen (Option D: Beweismanipulation, Korruption). Der Fall G aus `23#55A` (menschlicher Täter nutzt das Übernatürliche als Tarnung) ist die Umkehrung.

### 6.5 Spielerhandlungen bei falscher Beschuldigung (Aufzählung des Assistenten, `48#154A`)

Untertauchen; Beweise sammeln; den Formwandler finden; die Unschuld beweisen; versuchen, mit der Fahndung zu leben. Weitere Hunter erwägen einen Formwandler und können helfen. Mit Haft siehe [Modul 16](16-Haft-und-Strafvollzug.md): Das Beispiel ‚unschuldig durch Formwandler verhaftet‘ stammt von `48#158A`.

---

## 7. Entwurf: Entlastung und Rehabilitierung

Die Aufzählung in `48#156A` nennt „Langzeitfolgen: Offene Verfahren, Fahndungsakten, Rufschäden und mögliche Rehabilitierung“ (💡 des Assistenten; im Einklang mit Option D).

| Schritt | Entwurf |
|---|---|
| **Gegenbeweis** | Neue Belege auf E2 oder E4 (Kamera, Alibi, Forensikbefund, Anomalie, Zeugenaussage) |
| **Verdachtsverschiebung** | Behörde ändert E4 (nicht automatisch; Korruption und Voreingenommenheit können sie verhindern) |
| **Verfahren** | Verfahren wird eingestellt oder freigesprochen ([Modul 15](15-Gerichte-und-Verfahren.md)) |
| **Fahndung beendet** | Die Fahndung bleibt bis zur glaubwürdigen Auflösung (Modul 14: `23#1057E` Punkt 65) |
| **Ruf** | Ruf erholt sich langsam und unvollständig; Gerüchte bleiben (Option D) |
| **Akte** | Eintrag kann bleiben (Vorstrafen, Verdachtsakten) |

**Wichtig:** Die Entlastung ist eine **eigene Ermittlung** des Spielers (`48#154A`: „durch eigene Ermittlungen deine Unschuld beweist“, in `48#156A`). Damit gehört sie zum Fallmodell (Modul 09): Ein „Fall“, bei dem der Spieler selbst das Opfer des Verdachts ist. Es ist Fall F aus `23#55A`.

---

## 8. Entwurf: Erkennen von Verwandelten

Die Mythologie jeder Art bestimmt Erkennungsmerkmale; der Spieler soll sie durch Forschung herausfinden (`48#125E`, `48#127E`). Der Entwurf nennt nur **Kategorien**:

| Kategorie | Beispiel (erfunden) | Voraussetzung |
|---|---|---|
| **Wissenslücken** | die imitierte Person kennt etwas nicht, das sie kennen müsste | Befragung mit Detailkenntnis ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) |
| **Verhalten** | untypische Gewohnheiten | Beobachtung über Zeit |
| **Forensik** | Spuren stimmen nicht | Labor ([Modul 11](11-Forensik-und-Labor.md)) |
| **Anomalie** | physikalisch Unmögliches | Modul 11 §9 |
| **Test nach Mythologie** | Substanz, Gegenstand, Ritual zeigt die Natur | Forschung (`48#127E`) |
| **Dritte Hinweise** | Tiere reagieren; Hunter erkennen | `M-PD` (‚Tiere können auf Übernatürliches reagieren‘) |

Wichtig (Entwurf): Es gibt **keinen universellen Detektor**. Jede Art hat eigene Erkennungswege, und der Spieler darf sie erst kennen, wenn er sie erforscht oder erfahren hat (Archiv, Modul 04). Ein falscher Test (zum Beispiel ein Test, der bei dieser Art nichts zeigt) ist ein **falsches Negativ**: Der Spieler glaubt, jemand sei Mensch, obwohl er es nicht ist. Das ist gewollt (falsche Lore ist beschlossen, `M-PD`).

---

## 9. Zugehörigkeiten, Infiltration und Spitzel

Dieser Abschnitt gehört zur Identität, weil der Spieler dort **Rollen** einnimmt. Die Quellen stammen aus Chat 23.

### 9.1 Beschlossen

| Aussage | Status | Quelle |
|---|:---:|---|
| Neben der Hunter-Crew (HQ) kann der Spieler Mitglied in einer Gruppe sein (Motorradclub, Sekte, Extremisten, Clubs) und dort Missionen erfüllen; hierarchischer Aufstieg bis zum Anführer | 📋 | `23#675E`, `23#679E` |
| Diese Gruppen sind **optional**, die Hauptstory braucht sie nicht; es ändern sich allenfalls Dialoge | 📋 | `23#677E` |
| Man kann theoretisch ein **Spitzel** sein | 📋 | `23#677E` |
| Sehr komplex, aber nicht mit der Hauptstory verbunden; nach der Hauptstory kann man dort weitermachen | 📋 | `23#677E` |
| Clubs, Sekten, Extremisten können rekrutiert werden und im Hunter-HQ landen; das kann etwas zur Story beitragen, wenn der Charakter bei einer Mission zusätzlich zur Verfügung steht | 📋 | `23#677E` |
| Unbegrenzt zusätzlicher Inhalt bei Hunter-Gruppen und optionalen Clubs, auch nach der Story; Aufstieg zum Leader möglich | 📋 | `23#679E` |
| Der Spieler beurteilt Gruppen selbst, es muss nicht immer schlecht sein; Personen können eigentlich gute Leute sein | 📋 | `23#675E` |

Die Ausformulierung (drei Wege: ehrliche Mitgliedschaft, Infiltration/Spitzel, lose Verbindung; Hierarchien; Mitglieder haben untereinander Beziehungen; kein reiner Faction-Balken) stammt vom Assistenten (`23#676A`, `23#678A`, 💡). Der Assistent riet ausdrücklich davon ab, daraus einen realistischen Anwerbe-Simulator für reale extremistische Gruppen zu bauen (`23#678A`); Mitgliedschaft, Personen, Konflikte und Konsequenzen reichen. Der Entwickler widersprach dem nicht (`23#679E`).

### 9.2 Entwurf: Infiltration als Ermittlungsform

Infiltration ist **Ermittlung durch Identität**: Der Spieler (oder ein Auftraggeber) bringt eine Rolle in eine Gruppe und sammelt Informationen.

```text
Auftrag/Motiv -> Zugang -> Vertrauen aufbauen -> Informationen sammeln
   -> entscheiden, was weitergegeben wird -> Enttarnung / Ausstieg / Verrat
```

| Element | Entwurf |
|---|---|
| **Auftraggeber** | Polizei, Bundesbehörde, andere Organisation oder eigene Motivation (`23#678A`) |
| **Rolle** | Tarnidentität (Abschnitt 10) oder echte Person mit verdecktem Auftrag |
| **Vertrauen** | wächst über Jobs; Mitglieder haben Beziehungen (Abschnitt 4) |
| **Wissen** | Der Spieler erfährt Interna; was er davon wem weitergibt, ist seine Entscheidung |
| **Konflikt** | Verbundenheit mit Mitgliedern gegen den Auftrag (`23#678A`, Beispiel) |
| **Enttarnung** | Gruppe erkennt den Spitzel; Folgen reichen von Vertrauensverlust bis Lebensgefahr (Frage 48 zeigt, dass Zeugen bedroht werden können) |
| **Beweiswert** | Informationen aus Infiltration sind Belege mit Provenienz (Modul 10); Gericht verwertet sie nur in bestimmten Fällen (offen, Modul 15) |

Die Verknüpfung mit der Polizei ist auch durch die Justizoption D gedeckt: „verdeckte Ermittlungen“ gehören zum System (`48#154A`, gewählt `48#155E`; Aufzählung in `48#156A`: Undercover-Beamte, Informanten, längerfristige Operationen). Wie der Spieler selbst als Undercover-Informant der Polizei arbeitet, ist nicht beschlossen (❓ Q-E53).

---

## 10. Tarnidentität und Geheimhaltung

### 10.1 Beschlossen

Die Frage `48#148A`: „Wie stark soll der Spieler selbst die Geheimhaltung beeinflussen können?“ Optionen: A automatisch; B einfach (Zeugen beruhigen, Beweise beseitigen, unauffällig handeln); C umfangreich (Tarnidentitäten, falsche Erklärungen, Kontakte zu Behörden, Zeugenmanagement, Manipulation von Spuren); D systemische Freiheit (alles Genannte, aber auch bewusstes Offenlegen mit dynamischen gesellschaftlichen Konsequenzen).

Antwort `48#149E`: „soll er selbst entscheiden können, aber die frage ist wie man das mit dem dialogen machen kann weil sonst werden das wirklich 100000 seiten pro hauptcharakter“.

| Aussage | Status |
|---|:---:|
| Der Spieler entscheidet **selbst**, wie er Geheimhaltung handhabt | 📋 |
| Welche der Optionen B bis D genau gilt, ist **nicht benannt** | ❓ (die Antwort wählt keinen Buchstaben; die Formulierung „selbst entscheiden“ entspricht dem Geist von D) |
| Das Dialogproblem ist erkannt; Lösung: Hybridsystem (Vorschlag des Assistenten `48#150A`) | 💡 |

### 10.2 Entwurf: Tarnidentität

| Frage | Vorschlag |
|---|---|
| Was ist eine Tarnidentität? | falscher Name, Papiere, Rolle und Hintergrund, die der Spieler beschafft oder aufbaut |
| Woher? | Kontakte, Fälscher, Behörden (legal, z. B. Zeugenschutz oder Undercover), eigene Erfindung (Spiel regelt Machbarkeit) |
| Wie wird sie geprüft? | von Behörden, Gruppen, NPCs mit Wissen (Papiere, Gesicht, Verhalten); das Ergebnis hängt von Qualität und Beziehungen ab |
| Wie fliegt sie auf? | durch Wiedererkennung (Z6), widersprüchliche Belege, Gerüchte, Verrat |
| Wie wirkt sie auf den Ruf? | Ruf hängt an der **Identität**: Eine Tarnidentität hat einen eigenen Ruf; sie trennt Rufe voneinander (`48#153E`: guter Ruf bei Huntern trotz Fahndung) |
| Koop? | Jeder Hunter hat eine eigene Identität (`48#109E`: jeder erstellt seinen Hunter) |

Alle Tarnidentitäten sind als **Option C/D** angedacht; ob sie im Basisspiel vorkommen, ist offen (❓ Q-E54).

---

## 11. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Zwei Formwandler imitieren dieselbe Person | – | Eigenes Fallthema; E1 bleibt unverändert |
| Der Spieler wird vom Wesen **als Spieler** gejagt (Monster-Reputation) | Monster merken sich den Hunter, Waffen, Fallen (`48#139E`) | Reputation bei Monstern ist Gedächtnis, nicht Zahl |
| Doppelte Identität | Spieler hat Tarnidentität und echte Identität | Zwei Ruf-Datensätze; Verknüpfung nur durch Wissen, das jemand besitzt |
| Verwechslung | Zeugen halten den Spieler für jemand anderen (Z6) | Fahndung auf die falsche Person; derselbe Mechanismus wie Formwandler |
| Opfer erkennt den Spieler nicht | – | Aussage der Opfer; Wiedererkennung unsicher |
| Spieler stirbt im Koop und wird ersetzt | – | Ruf bleibt beim Charakter, nicht beim Spieler (Charaktere gehören dem Spielstand, `48#109E`) |
| Ruf in Hölle/Fegefeuer/Himmel | – | offen |
| Mods | Mod fügt Reputationsgruppen hinzu | Gruppen müssen in der Aggregation (4.3) erscheinen können |
| Barriere durch Sprache | Fremdsprachige Region | offen |
| Kindliche Figuren | – | nicht in den Quellen |

---

## 12. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| NPC-Gedächtnis und Beziehungen | Quelle aller Überzeugungen |
| Polizei und Fahndung ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) | E4 und Fahndungsakten |
| Gerichte ([Modul 15](15-Gerichte-und-Verfahren.md)) | Beweiswürdigung, Rehabilitierung |
| Haft ([Modul 16](16-Haft-und-Strafvollzug.md)) | Folgen unschuldiger Verhaftung |
| Geheimhaltung ([Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md)) | öffentliches Wissen, Tarnidentität |
| Forensik ([Modul 11](11-Forensik-und-Labor.md)) | Spuren des Formwandlers; Anomalien |
| Monster-Mythologie | Fähigkeiten und Erkennbarkeit |
| Charaktererstellung | Aussehen, Wiedererkennbarkeit (Phase 0.4, [Decision 0005](../../Decisions/0005-character-appearance-foundation.md)) |
| Fraktionen und Mitgliedschaften | Rollen, Ränge, Infiltration |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Überzeugungen als Zustand, Größe, Aktualisierung |

---

## 13. Ergänzung: Tattoos als Erkennungsmerkmal

Aus dem Tattoo-System (Chat 35, 26.09.2026) stammen Aussagen, die für Identifizierung relevant sind; sie gehören zum Adult-/Körpersystem und sind dort nicht Ermittlungsregeln:

| Aussage | Quelle | Status |
|---|---|---|
| Tattoos können erneuert, gelasert, „geblackouted“ und überdeckt („coverup“) werden | `35#749E` Punkt 1 | 📋 |
| Der Spieler kann dem Tätowierer beliebige Motive an beliebigen Stellen angeben | `35#749E` Punkt 2 | 📋 |
| Ein Tattoo am Bauch verformt sich bei Schwangerschaft; das Motiv ist danach nicht mehr proportional | `35#749E` Punkt 3 | 📋 |
| Verteilung in der Bevölkerung: auf dem Land selten, bei Älteren „die nicht im gefängnis waren“ ebenfalls selten; bei Jungen modern und häufiger, in der Stadt häufiger; wenige NPCs mit Fullbody-Tattoos | `35#749E` Punkt 18 (Frage `35#748A` Punkt 18) | 📋 |

**Folgerungen für die Ermittlung (💡):** Tattoos sind ein **dauerhaftes, aber veränderbares Erkennungsmerkmal** (vier Wahrheitsebenen: [Abschnitt 3](#3-entwurf-die-vier-ebenen-der-wahrheit); die Welt kennt das Tattoo, die Zeugenbeschreibung nicht unbedingt). Überdecken oder Lasern kann eine Personenbeschreibung entwerten; ein Tattoo kann als Haftindikator gelten, da der Entwickler Tattoos bei älteren Menschen mit Gefängnisaufenthalt in Verbindung bringt. Ob das Spiel diese Verbindung als Indiz für Vorstrafen nutzt, ist ❓ und nicht beschlossen. Ein Formwandler könnte Tattoos übernehmen oder nicht ([Abschnitt 6](#6-entwurf-formwandler-und-identitätsmissbrauch), ❓).

---

## 14. Was fehlt
- Die **Fähigkeiten** der Formwandler und ihre Erkennbarkeit (Q-E52, abhängig von der Monster-Mythologie).
- **Skalen und Formeln** der Überzeugungen und ihrer Aggregation.
- Die **Rolle des Spielers als Undercover-Informant** (Q-E53).
- **Tarnidentitäten**: Basisspiel oder später (Q-E54).
- Die **Verknüpfung** von Gesicht und Aussehen mit der Charaktererstellung (Wie sehr unterscheidet das System Gesichter?).
- Die **Zahl** der Reputationsgruppen.
