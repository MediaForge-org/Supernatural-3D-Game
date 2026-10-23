# Ermittlung 15: Gerichte und Verfahren

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt das **Verfahren nach der Festnahme**: Anklage, Prozess, Urteil, Strafe, Rechtsmittel und die Rückwirkungen auf den Spieler und die Welt. Der Master legt das **Ergebnis-Spektrum** fest (Haft, Bewährung, Geldstrafe, Freispruch) und dass Anwälte, Kaution und Berufung später ausgebaut werden. Alles Weitere ist **Entwurf** (💡). Das Modul ist das dünnste der Reihe, weil die Quellen hier am wenigsten hergeben. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Vorgänger: [Modul 14](14-Polizei-Behoerden-und-Fahndung.md). Nachfolger: [Modul 16](16-Haft-und-Strafvollzug.md). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Verfahrensphasen](#2-entwurf-verfahrensphasen)
3. [Entwurf: Übergänge](#3-entwurf-übergänge)
4. [Entwurf: Beteiligte](#4-entwurf-beteiligte)
5. [Entwurf: Beweisführung und Zulässigkeit](#5-entwurf-beweisführung-und-zulässigkeit)
6. [Entwurf: Absprache oder Prozess](#6-entwurf-absprache-oder-prozess)
7. [Entwurf: Urteil und Strafen](#7-entwurf-urteil-und-strafen)
8. [Entwurf: Kaution, Anwälte, Berufung](#8-entwurf-kaution-anwälte-berufung)
9. [Entwurf: Vorstrafen und ihre Wirkung](#9-entwurf-vorstrafen-und-ihre-wirkung)
10. [Das Übernatürliche vor Gericht](#10-das-übernatürliche-vor-gericht)
11. [Verfahren gegen NPCs](#11-verfahren-gegen-npcs)
12. [Zeit, Zeitsprung und Koop](#12-zeit-zeitsprung-und-koop)
13. [Sonderfälle](#13-sonderfälle)
14. [Abhängigkeiten](#14-abhängigkeiten)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| G1 | Strafkette: Tat, Schweregrad, Vorsatz/Fahrlässigkeit, Opfer, Zeugen und Beweise, Notwehr/Nothilfe, Vorstrafen, Festnahme, Staatsanwaltschaft, **Prozess oder Absprache**, Urteil, Haft/Bewährung/Geldstrafe/Freispruch | 📋 | `M-PD`; `35#533A` |
| G2 | Festnahme kann zu Verhör, Geldstrafe, Kaution, Gericht, Gefängnis und vergehender Zeit führen; kein Game Over | 📋 | `23#1058A`; `23#1057E` Punkt 68 |
| G3 | Anwälte und Kaution können **später** ausgebaut werden | 📋 | Punkt 69 („ja kann man später machen“) |
| G4 | Die Fahndung endet erst durch Zahlung (Kopfgeld), Rücknahme der Klage oder Freispruch | 📋 | Punkt 65 |
| G5 | Das Strafsystem soll realistisch und logisch sein (zum Beispiel lebenslange Haft für die Tötung der eigenen Kinder) | 📋 | `35#532E`; `35#533A` |
| G6 | Selbstverteidigung/Notwehr wird nach Handlung bewertet, unabhängig vom Geschlecht | 📋 | `35#533A` |
| G7 | Option C und D der Justiz enthalten Anwälte, Gerichtsverfahren, Urteile, Gefängnis, Vorstrafen (C) und zusätzlich regionale Zuständigkeiten, verdeckte Ermittlungen, Identitätsdiebstahl, Beweismanipulation, Korruption und langfristige Konsequenzen (D) | 📋 (Option D gewählt) | `48#154A`, `48#155E` |
| G8 | „Offene Verfahren, Fahndungsakten, Rufschäden und mögliche Rehabilitierung“ als Langzeitfolgen | 💡 des Assistenten | `48#156A` |
| G9 | Ein Spieler kann durch eigene Ermittlungen seine Unschuld beweisen | 💡 des Assistenten (Beispiel) | `48#156A` |
| G10 | Rechtliche Möglichkeiten in der Haft: Anwalt, Beweissuche, Berufung, mögliche Entlastung | 💡 des Assistenten (Baustein) | `48#158A` |
| G11 | Das System soll spielbar bleiben: keine stundenlange Haft wegen einer Kleinigkeit | 💡 des Assistenten | `48#156A` |
| G12 | Besessenheit kann echte Folgen haben, auch Straftaten | 📋 Richtung | Frage 143 `23#1074A`, Antwort ‚142 ja … 143 ja genau‘ (`23#1075E`) |
| G13 | Eine Mission läuft während der Haft nicht verloren; Ermittlungen und Gerichtsverfahren laufen weiter | 📋 | `48#159E`, `48#161E` |

---

## 2. Entwurf: Verfahrensphasen

> [!NOTE]
> **Nur Vorschlag (💡).** Die Phasen lehnen sich an die Strafkette (G1) an und folgen dem Muster eines US-artigen Verfahrens (Welt: fiktive US-Welt, `35#533A`). Namen sind Arbeitsnamen.

```text
[Festnahme] -> [Gewahrsam/Erstvorführung] -> [Kaution/Untersuchungshaft]
                                                    |
                                  [Anklage durch Staatsanwaltschaft]
                                                    |
                          [Vorverfahren: Beweise, Anwalt, Verhandlung über Absprache]
                              /                                      \
                     [Absprache]                                [Hauptverfahren]
                          |                                           |
                     [Urteil nach Absprache]                [Urteil: Schuldspruch / Freispruch]
                                           \                          /
                                            [Strafmaß / Einstellung]
                                                         |
                          [Strafvollzug / Bewährung / Geldstrafe]  -> [Berufung] -> [Rechtskraft]
```

| Phase | Inhalt (Vorschlag) |
|---|---|
| **Festnahme** | Person in Gewahrsam der Polizei (Modul 14) |
| **Gewahrsam/Erstvorführung** | Haftgrund wird geprüft; kurzer Zeitraum; Verhör |
| **Kaution/U-Haft** | Entscheidung über Freilassung gegen Kaution oder Haft bis zum Verfahren (G3: später ausbaubar) |
| **Anklage** | Staatsanwaltschaft erhebt Anklage aufgrund der Beweislage |
| **Vorverfahren** | Beweise, Zeugen, Verteidigung; Verhandlung über Absprache |
| **Absprache** | Schuldeingeständnis gegen mildere Strafe (G1: ‚Prozess oder Absprache‘) |
| **Hauptverfahren** | Verhandlung mit Zeugen, Beweisen, Plädoyers |
| **Urteil** | Schuldspruch, Freispruch, Einstellung |
| **Strafmaß** | Haft, Bewährung, Geldstrafe, Auflagen |
| **Berufung** | Überprüfung des Urteils (G10 nennt sie als Baustein; Details offen) |
| **Rechtskraft** | Verfahren abgeschlossen; Eintrag in Vorstrafen (Abschnitt 9) |

**Zeitverlauf.** Die Dauer jeder Phase ist nicht festgelegt. Weil Verfahren während Haft, Krankenhaus oder Zeitsprung weiterlaufen (G13), müssen sie als **Zeitfortschreibung** in der Hintergrundsimulation laufen (Modul 14 §13).

---

## 3. Entwurf: Übergänge

| Von | Nach | Auslöser | Bedingung |
|---|---|---|---|
| Festnahme | Gewahrsam | Polizei | Person festgenommen |
| Gewahrsam | Freilassung | Behörde | Beweislage zu dünn (Entlastung) oder Verfahren eingestellt |
| Gewahrsam | Kaution/U-Haft | Gericht | Anklage wahrscheinlich; Entscheidung nach Schwere und Fluchtgefahr |
| Kaution/U-Haft | Anklage | Staatsanwaltschaft | Beweislage genügt |
| Anklage | Vorverfahren | Gericht | Verfahren eröffnet |
| Vorverfahren | Absprache | Parteien | Zustimmung des Angeklagten und der Staatsanwaltschaft |
| Vorverfahren | Hauptverfahren | Gericht | keine Absprache |
| Hauptverfahren | Schuldspruch/Freispruch | Gericht | Beweiswürdigung ([Abschnitt 5](#5-entwurf-beweisführung-und-zulässigkeit)) |
| Schuldspruch | Strafmaß | Gericht | Schwere, Vorstrafen, Umstände, Notwehr |
| Strafmaß | Vollzug | Behörde | Haft: [Modul 16](16-Haft-und-Strafvollzug.md) |
| beliebig | Einstellung | Staatsanwaltschaft | Klagerücknahme (G4: „die klage zurückgezogen“) |
| Urteil | Berufung | Verteidigung | neue Beweise, Verfahrensfehler |
| beliebig | Flucht | Angeklagter | Ausbruch oder Nichterscheinen: Fahndung steigt (Modul 14) |

---

## 4. Entwurf: Beteiligte

Die Quellen nennen **Staatsanwaltschaft** (G1), **Anwälte** (G3, G7), **Gericht** (G7). Ein Entwurf der Rollen:

| Rolle | Aufgabe | Spielerbezug |
|---|---|---|
| **Staatsanwalt/-anwältin** | Anklage, Absprachen | NPC mit Zielen (Karriere, Überzeugung, Druck) |
| **Verteidiger** (Pflicht oder privat) | vertritt den Angeklagten | wählbar/bezahlbar; Qualität schwankt; später ausbaubar (G3) |
| **Richter/Richterin** | Verfahrensleitung, Urteil/Strafmaß | NPC mit Persönlichkeit, Vorurteilen (Z10 aus Modul 12) |
| **Geschworene** | ob es sie gibt: nicht beschlossen | ❓ |
| **Zeugen** | Aussagen (Modul 12) | NPC-Zeugen mit Bereitschaft |
| **Sachverständige** | Labor und Gerichtsmedizin | NPC; Befund (Modul 11) |
| **Opfer und Angehörige** | Nebenkläger, Aussagen | NPC; Reaktionen |
| **Medien** | Berichterstattung | beeinflusst Ruf (Modul 13) |

Alle Rollen sind **NPCs** (Tier A bis C) mit Gedächtnis. Wiederkehrende Personen (zum Beispiel ein Richter, der den Spieler schon kennt) folgen den NPC-Regeln (Z1, Z19).

---

## 5. Entwurf: Beweisführung und Zulässigkeit

### 5.1 Grundsatz

Das Gericht urteilt auf der **Behörden- und Prozessüberzeugung** (E4 in [Modul 13](13-Identitaet-Ruf-und-Formwandler.md#3-entwurf-die-vier-ebenen-der-wahrheit)), nicht auf der Wirklichkeit (E1). Ein Unschuldiger kann verurteilt, ein Schuldiger freigesprochen werden. Das setzt ‚Gesetz folgt tatsächlicher Information‘ (P1) im Gericht fort.

### 5.2 Entwurf: Welche Beweise zählen

| Beweisart | Gewicht (Vorschlag, kein Zahlenmodell) |
|---|---|
| Zeugenaussage | abhängig von Zuverlässigkeit und Konsistenz (Modul 10 §6) |
| Kamera/Medium | hoch, wenn echt; angreifbar bei Manipulation (Formwandler, Bearbeitung) |
| Forensik | hoch, wenn Beweiskette sauber (Modul 11, Modul 10 §9) |
| Geständnis | hoch, aber anfechtbar (Verhör unter Druck) |
| Indizien | niedriger; Summe zählt |
| Beweise des Spielers | nur, wenn sie legal erlangt und glaubwürdig sind |

### 5.3 Zulässigkeit

Beweise, die **rechtswidrig** erlangt wurden (Einbruch ins Labor, Leichenschändung, Folter), können unzulässig sein. Das ist nicht in den Quellen, passt aber zu ‚Gesetz folgt tatsächlicher Information‘ und zur Beweiskette (`M-B39`, Themenliste ‚evidence cleanup/chain-of-custody‘). Es erzeugt für Hunter einen **Zielkonflikt**: Illegal beschaffte Hinweise helfen bei der Jagd, aber nicht vor Gericht (❓ Q-E57).

### 5.4 Beweismanipulation

Beweismanipulation ist Teil der Option D (G7). Das Verfahren muss damit rechnen, dass Beweise **gefälscht** sind (Formwandler, Korruption). Mögliche Gegenmaßnahmen (💡): Gegenbeweise, Gutachten, Zeugen, Zweifel, Berufung. Wie oft das System Fälschung **entdeckt**, ist offen.

---

## 6. Entwurf: Absprache oder Prozess

G1 nennt beide Wege. Ein Entwurf der Entscheidung für den Spieler:

| Weg | Vorteil | Nachteil | Auswirkung auf Ruf und Fahndung |
|---|---|---|---|
| **Absprache** | kürzer, geringere Strafe | Eingeständnis (Vorstrafe), Nachteile bei Unschuld | endet die Fahndung; Ruf leidet |
| **Prozess** | Chance auf Freispruch | Risiko höherer Strafe, Dauer, Öffentlichkeit | Medienpräsenz; Ruf hängt vom Ausgang ab |

Für einen **unschuldigen** Spieler (Formwandler, Fälschung) ist die Absprache ein moralisch und spielerisch schwieriger Weg (Eingeständnis einer Tat, die er nicht beging). Das ist als Spielsituation gewünscht (G9: Unschuld beweisen), aber nicht beschlossen.

---

## 7. Entwurf: Urteil und Strafen

### 7.1 Ergebnisse

Beschlossen sind die Ergebnisarten **Haft, Bewährung, Geldstrafe, Freispruch** (G1). Der Strafkatalog fehlt (`M-OD`).

| Ergebnis | Wirkung |
|---|---|
| **Freispruch** | Fahndung endet (G4); Ruf kann sich erholen; Eintrag im Register bleibt als „freigesprochen“ |
| **Geldstrafe** | Zahlung aus Bargeld/Konto; nur Bargeld am Körper ist bei Verlust gefährdet (`M-PD`), die Strafe selbst betrifft das Vermögen (Entwurf) |
| **Bewährung** | Auflagen (Meldepflicht, Verbote), bei Verstoß Haft |
| **Haft** | Strafvollzug ([Modul 16](16-Haft-und-Strafvollzug.md)) |
| **Lebenslange Haft** | für schwerste Delikte (G5: Beispiel Tötung der eigenen Kinder); für den Spieler ist das de facto Spielende der Figur, aber **kein Game Over** (G2); Ausgang ist offen (Q-E58) |
| **Einstellung** | Verfahren endet ohne Urteil (G4) |

### 7.2 Strafmaß (Entwurf)

Faktoren: Schwere, Vorsatz/Fahrlässigkeit, Opfer, Vorstrafen, Notwehr/Umstände, Reue/Absprache, Gericht (Region, Persönlichkeit). Die Quellen nennen diese Faktoren in der Strafkette (G1). Es gibt **keine** Strafwerte.

### 7.3 Spielbarkeit

G11 verlangt, dass kleine Delikte nicht zu stundenlangen Haftzeiten führen. Ein Entwurf: Strafen werden in **Spielzeit** umgerechnet und können, wie die Haft, übersprungen werden ([Modul 16](16-Haft-und-Strafvollzug.md)). Die tatsächlichen Zeiten sind nicht festgelegt.

---

## 8. Entwurf: Kaution, Anwälte, Berufung

G3 sagt „später“, G10 und G8 nennen sie als Bausteine. Ein Entwurf der **Mindestfassung** und der **Ausbaustufen** (`48#158A` ordnet Festnahme mit Konsequenzen `[VS]`, Gewahrsam/Verhöre/rechtliche Entscheidungen `[EARLY]`, spielbare Haftanstalten `[LATER]`, dynamische Gefängnisgesellschaft `[FUTURE]` zu; das ist ein Vorschlag des Assistenten):

| Stufe | Inhalt (Vorschlag) |
|---|---|
| **Mindestfassung** | Festnahme führt zu Verhör, Geldstrafe oder Haft mit Zeitsprung; keine Verfahrensentscheidungen |
| **Erweiterung 1** | Anwalt (Pflicht-/Privatverteidiger), Absprache oder Prozess, Kaution |
| **Erweiterung 2** | Beweisführung des Spielers, Zeugen, Entlastungsermittlung |
| **Erweiterung 3** | Berufung, Gnadenverfahren, Bewährung mit Auflagen |
| **Ausbau** | Prozesse als spielbare Szenen; Anwälte als Begleiter |

Welche Stufe im Basisspiel enthalten ist, ist **nicht entschieden**.

---

## 9. Entwurf: Vorstrafen und ihre Wirkung

| Wirkung | Beispiel | Quelle |
|---|---|---|
| Strafmaß | Vorstrafen erhöhen künftige Strafen | G1 |
| Verdacht | Polizei behandelt Vorbestrafte skeptischer | Modul 14 (Behördenüberzeugung) |
| Ruf | Nachbarn und Arbeitgeber reagieren | Modul 13 |
| Berufe und Lizenzen | Vorstrafen können Lizenzen verhindern (PI-Lizenz, Modul 07; Waffenschein) | 💡; die Quellen nennen die PI-Lizenz als Wunsch, aber keine Voraussetzungen |
| Referenzen | Fingerabdrücke und DNA liegen vor (Modul 11 §10) | 💡 |
| Rehabilitierung | Eintrag kann nach Zeit entfallen oder gelöscht werden | G8 (Assistent: „mögliche Rehabilitierung“) |

Vorstrafen sind **Weltzustand** der Figur und gehören dem Charakter, nicht dem Spielstand allgemein (Koop: jeder Charakter hat eine eigene Akte).

---

## 10. Das Übernatürliche vor Gericht

| Situation | Entwurf |
|---|---|
| **Unmögliche Aussagen** | Zeugen sagen Unmögliches aus; Gericht ordnet sie ein (Wahnvorstellung, Lüge, Drogen); ein Spieler, der die Wahrheit sagt, kann unglaubwürdig wirken (`48#150A` Beispiel: die Polizistin glaubt nicht) |
| **Besessenheit als Umstand** | Straftaten unter Einfluss (G12): Schuldfähigkeit ist eine Frage der Verhandlung; wer die Besessenheit beweisen kann, mildert die Folgen; wer nicht, trägt sie |
| **Gericht mit Wissen** | Ein Richter oder Staatsanwalt kann zur geheimen Einheit gehören (Modul 14 §11) und das Verfahren lenken |
| **Wesen unter menschlicher Identität** | Ein Wesen kann angeklagt sein; der Spieler weiß mehr als das Gericht |
| **Beweise durch Anomalie** | Forensische Anomalien lassen sich vor Gericht nicht normal erklären; Behörden können sie verwerfen (Vertuschung, Modul 17) |

Die Regel ‚Die Polizei erklärt Dinge zunächst normal‘ (Punkt 116) gilt sinngemäß auch für Gerichte (Entwurf).

---

## 11. Verfahren gegen NPCs

NPC-Verfahren sind Teil der Welt (P19, Block 8F: Straftaten unabhängig vom Spieler). Wirkungen:

| Wirkung | Beispiel |
|---|---|
| Weltveränderung | Verurteilter fehlt in der Nachbarschaft; Familie reagiert; Geschäft bekommt neuen Betreiber (Block 8L/8M: `23#1057E` Punkte 103, 109) |
| Zeugenauftritt | Der Spieler kann als Zeuge aussagen oder vorgeladen werden |
| Gelegenheiten | Gerichtstermine als Orte (Gerichtsgebäude) und Ereignisse (Beerdigung, Prozess) |
| Prozess gegen Hunter | NPC-Hunter geraten vor Gericht; Ruf in der Hunter-Gemeinschaft |
| Medien | Berichte über Prozesse beeinflussen das öffentliche Wissen |

Weil Tier C und D nur abstrakt simuliert werden, laufen deren Verfahren **abstrakt** (Ergebniszustände), ohne Detail.

---

## 12. Zeit, Zeitsprung und Koop

| Aspekt | Regel |
|---|---|
| Zeit | Verfahren laufen in Spielzeit; die Dauer jeder Phase ist offen |
| Haft | Der Spieler kann Haft überspringen; die Welt (auch Verfahren) läuft weiter (`48#159E`, `48#161E`) |
| Koop | Nur der Betroffene wird festgenommen; ein Welt-Zeitsprung braucht Zustimmung (`48#163E`) |
| Anwesenheit | Prozesstermine können verlangen, dass der Spieler erscheint; er kann sie versäumen (Folgen: Fahndung, Kautionsverlust) – Entwurf |
| Verhinderung | Krankenhaus, Haft, Ausfall verschieben Termine |

---

## 13. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Der Spieler ist unschuldig, weiß aber den Täter (Wesen) | – | Konflikt zwischen Geheimhaltung und Entlastung (`48#149E`) |
| Beweislage widerspricht sich | – | Zweifel; Freispruch oder Einstellung möglich |
| Zeuge widerruft | – | Aussage ändert sich (Modul 12); Berufung |
| Verteidiger ist korrupt oder ein Wesen | – | Verrat; Option D |
| Kronzeuge | Mitglied einer Gruppe sagt aus | Zeugenschutz (nicht in den Quellen) |
| Mehrere Angeklagte | Komplizen | Absprachen untereinander |
| Mehrere Behörden verfolgen | – | Zuständigkeit klärt Gericht (Modul 14) |
| Auslieferung | Straftat in anderem Staat | fiktive Bundesstaaten (Welt 07); nicht in den Quellen |
| Opfer ist NPC mit Gedächtnis | – | Opfer erinnert sich und handelt (Rache, Anzeige) |
| Tod des Spielers während des Verfahrens | – | Verfahren erlischt oder wird fortgeführt (offen) |
| Verfahren gegen einen Mitspieler (Koop) | – | Nur sein Charakter ist betroffen |

---

## 14. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Polizei und Fahndung ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) | Akte, Haftbefehl, Anklage |
| Haft ([Modul 16](16-Haft-und-Strafvollzug.md)) | Vollzug, Entlassung |
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Beweiswürdigung, Beweiskette |
| Identität ([Modul 13](13-Identitaet-Ruf-und-Formwandler.md)) | E4, Entlastung, Rehabilitierung |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Aussagen vor Gericht |
| Wirtschaft | Geldstrafe, Kaution, Anwaltskosten |
| Lizenzen (PI, Waffen) | Vorstrafen |
| Zeit | Terminplanung, Fortschreibung |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Verfahren, Akten, Vorstrafen als Zustand |

---

## 15. Was fehlt

- **Straftatenkatalog**, Strafen, Kaution, Bewährung (`M-OD`).
- **Jury**: ob es sie gibt (Q-E59).
- **Zulässigkeit** rechtswidrig erlangter Beweise (Q-E57).
- **Lebenslange Haft** für den Spieler: Ausgang (Q-E58).
- **Termine und Anwesenheitspflicht**.
- **Berufung** und Rechtsmittel.
- **Stufen** des Ausbaus im Basisspiel.
- **Prozess als Spielszene**: ob und wie.
