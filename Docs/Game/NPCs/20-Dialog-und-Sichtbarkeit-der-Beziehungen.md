# NPCs 20: Dialog und Sichtbarkeit der Beziehungen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die Schnittstelle zwischen dem inneren Zustand der NPCs (Gedächtnis, Beziehung, Wissen, Stimmung) und dem, was der Spieler wahrnimmt: Dialog, Barks, Nachrichten, Körpersprache und Handlungen. Beschlossen sind die Grundsätze (keine Zahlen, Dialog ist datengetrieben und liest Beziehungs- und Gedächtniszustand, Dialogtiefe nach Bedeutung der Figur, die Engine entscheidet über Fakten); **Datenschnittstelle, Signalpalette und Inspektionswerkzeuge sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Dialogumfang der Ermittlung: [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §13.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Der Dialogkontext](#2-entwurf-der-dialogkontext)
3. [Entwurf: Bedingungen für Zeilen](#3-entwurf-bedingungen-für-zeilen)
4. [Entwurf: Dialogtiefe nach Stufe](#4-entwurf-dialogtiefe-nach-stufe)
5. [Entwurf: Erinnerungsbezogene Dialoge](#5-entwurf-erinnerungsbezogene-dialoge)
6. [Entwurf: Beziehung ohne Zahlen sichtbar machen](#6-entwurf-beziehung-ohne-zahlen-sichtbar-machen)
7. [Entwurf: Barks und Hintergrundgespräche](#7-entwurf-barks-und-hintergrundgespräche)
8. [Entwurf: Telefon und Nachrichten](#8-entwurf-telefon-und-nachrichten)
9. [Entwurf: Inspektion und Fehlersuche](#9-entwurf-inspektion-und-fehlersuche)
10. [Sonderfälle](#10-sonderfälle)
11. [Entwurf: Prüfregeln](#11-entwurf-prüfregeln)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-DI-001 | Dialog ist ein datengetriebenes Inhaltssystem; jede gesprochene Zeile hat eine stabile Dialog-ID und kann Sprecher, Text, Kontext, Emotion, Teilnehmer, Fall-/Weltzustandsbedingungen, Wissensbedingungen, Beziehungs-/Erinnerungsbedingungen, Antworten, Stimm-Asset, Lip-Sync/Gesicht/Körper, Untertitel, Lokalisierung und Prüfstatus referenzieren | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; Beispiel `23#313A` Abschnitt 1 |
| N-DI-002 | Beziehungs- und NPC-Gedächtniszustand können verschiedene Dialogvarianten auswählen, **ohne** versteckte Beziehungszahlen offenzulegen | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-003 | Das Dialogsystem liest intern Vertrauen, Angst, Respekt, Verdacht, vergangene Entscheidungen, Fallstatus und Erinnerungen und wählt passende Gesprächsvarianten; die Beziehung wird nie als Zahl angezeigt | 📋 Richtung | `23#313A` Abschnitt 4 (Beschreibung des Assistenten); `23#52E` Punkt 9 |
| N-DI-004 | Beispiele der Quelle: Trust niedrig: ‚Das geht Sie nichts an.‘; Trust mittel: ‚Mehr kann ich Ihnen derzeit nicht sagen.‘; Trust hoch: ‚Offiziell habe ich Ihnen das nie gezeigt.‘ | 💡 (Beispiele) | `23#313A` Abschnitt 4 |
| N-DI-005 | Dialogformen: natürliches Gespräch; Investigationsdialog über **tatsächlich bekannte** Themen und Beweise; Entscheidungsdialog bei wichtigen Lagen; dynamischer Dialog beim Fahren, Kämpfen, Erkunden | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; `23#313A` Abschnitt 3 |
| N-DI-006 | Wichtige Figuren brauchen große Mengen kontextabhängiger Barks (Kampf, Stealth, Erkundung, Verletzung, Fahren, Weltereignisse, Begleiterinteraktion); die Variation muss Wiederholung vermeiden | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-007 | Gespräche sind unterbrechbar und setzen kontextgerecht fort, statt lange Zeilen neu zu starten; Dialog findet beim Gehen, Fahren, Ausrüsten, Verbinden, Ermitteln statt | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-008 | Die Schreibweise bewahrt die individuelle Stimme: Alter, Region und Akzent, Wortschatz, Bildung, Persönlichkeit, Wissen, Humor, Stressreaktion, Tabu- und private Themen; wichtige Figuren haben ein Voice/Character Bible | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-009 | **Dialogtiefe nach Bedeutung:** Haupt- und Story-Figuren extrem tief; wiederkehrende Nebenfiguren tief; gewöhnliche Einwohner deutlich flacher, sprechen aber kontextbezogen über Ort, Wetter, Gerüchte, aktuelle Ereignisse, Polizeiaktivität und Weltzustandsfolgen | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-010 | Praktisch alle relevanten NPCs sind ansprechbar und gesprächsfähig, aber nicht jeder braucht gigantische Verzweigungen; mit jedem normalen NPC kann man reden („nicht sehr viel aber schon“) | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; „ja man soll mit jedem npc reden können, nicht sehr viel aber schon“ in `23#1057E` Punkt 31 |
| N-DI-011 | NPCs reden untereinander, damit Gerüchte und lokale Ereignisse sich organisch verbreiten und der Spieler nützliche Informationen belauschen kann | 📋 | `M-PD` ‚Dialogue / conversation architecture‘; `23#1056A` Fragen 34–35 |
| N-DI-012 | Dialogvolumen: Hauptfiguren erreichen am Ende Hunderttausende Textzeilen im Basisspiel samt Kontextvarianten; wichtige wiederkehrende Nebenfiguren 10.000–50.000+; die genauen Endzahlen sind offen; Architektur muss auf sehr große Datenmengen skalieren | 📋 / ❓ | `M-PD` ‚Dialogue volume / scalability‘ |
| N-DI-013 | Der Dialoglaufzeit lädt nur die kontextuell relevanten Teile des Korpus, nicht alles beim Start | 📋 | `M-PD` ‚Dialogue / conversation architecture‘ |
| N-DI-014 | Dialog-IDs und Revisionen machen veraltete Text-, Audio-, Gesichts- und Lokalisierungsassets erkennbar; geänderter Text markiert die frühere Aufnahme als neu aufzunehmen | 📋 | `M-PD` ‚Dialogue volume / scalability‘ |
| N-DI-015 | Dialogsystem zuerst mit klassischen Auswahlmöglichkeiten; später möglicherweise freie KI-Gespräche | 📋 | `48#151E` (Z18 in [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md)) |
| N-DI-016 | Die Spielengine entscheidet über Fakten und Konsequenzen, nicht ein Sprachmodell; ein Faktenpaket aus dem Weltzustand wird in Worte gefasst, eine KI-Formulierung darf nichts hinzuerfinden | 💡 (im Einklang mit `M-PD`) | `48#150A`; [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §13 |
| N-DI-017 | Der Spieler kann anderes erfahren als das, was ein NPC fühlt: er sieht nicht, ob ein NPC ihn mag, er merkt es am Ton | 📋 | „der spieler sieht nicht ob der npc ihn mag es merkt es am ton“ in `23#52E` |
| N-DI-018 | Hidden values brauchen sichtbare Folgen; Debug-Werkzeuge, die erklären, warum ein NPC handelt, sind vom Shipping-UI verborgen | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §4 Punkt 8, §9 |
| N-DI-019 | Eine Werte-Ansicht der Beziehungs- und Präferenzwerte gibt es nur in Mod-, Debug- und Cheat-Modus | 📋 (für Beziehungs-/Präferenzwerte) | [Adult 05](../Adult/05-NPCs-Beziehungen-Praeferenzen.md) §2 |
| N-DI-020 | Prompt-Entwurfstitel der Spur 05 nennen: Beziehung→Dialogkontext-Schnittstelle, Beziehung→Kooperationsschnittstelle, Beziehung→Zugangs-/Gefallen-Schnittstelle, Hook für erinnerte Ereignisse, Regel ‚keine sichtbare Beziehungszahl‘, Debug-Inspektoren | Inventar (kein Beschluss) | `M-P05` (DRAFT ONLY) |
| N-DI-021 | NPCs kommunizieren in verschiedenen Stilen (Telefon, Text, Sprachnachricht, Tempo, Länge, Emojis, Initiative); das Datenmodell ist offen | 📋 / ❓ | `M-PD` ‚Smartphone / social media …‘; `M-OD` |
| N-DI-022 | Die Ausgestaltung der Sprachproduktion (Voice, Facial, Body) und das Verhältnis zu freien KI-Dialogen sind eigene Produktionsbereiche | – | `M-PD` ‚Voice production strategy‘, ‚Voice / facial / body production‘ (hier nicht ausgeführt) |

---

## 2. Entwurf: Der Dialogkontext

Das Dialogsystem soll nicht beliebig im NPC-Datensatz lesen und schreiben. Der Entwurf (💡) definiert eine **schmale Schnittstelle**: Das Dialogsystem bekommt einen **Kontext** (Schnappschuss) und liefert **Wahlen und Ereignisse** zurück.

```text
DialogueContext  (nur lesend, aus NPC-, Welt- und Spielerzustand abgeleitet)
├── speaker            NPC-ID, Rolle, Persönlichkeit (Ausdruck), Stimmung, Zustand (müde, verletzt, betrunken)
├── listener           Spieler (oder anderer NPC); sichtbares Aussehen und Kleidung
├── relation_view      Verhaltensbänder je Dimension (Vertrauen, Angst, Respekt, Verdacht ...)  -- keine Zahlen
├── memory_hooks[]     relevante erinnerte Ereignisse mit Ton, Sicherheit und Alter   (Modul 05)
├── knowledge_view     was der NPC zum Thema weiß, glaubt, vermutet, nicht weiß          (Modul 06)
├── rumors             im Umlauf befindliche Gerüchte, die er kennt                       (Modul 07)
├── situation          Ort, Zeit, Wetter, Anwesende, laufende Ereignisse, Fallstatus
├── affiliation        Fraktion, Rang, Loyalität (Modul 16)
├── topics             erlaubte Themen (was der Spieler kennt, was der NPC preisgibt)    (Ermittlung 12 §5)
└── constraints        Tabus, Geheimnisse, Verbote, rechtliche Rahmen

DialogueOutcome   (Rückgabe an die Welt)
├── events[]           Ereignisse (Information weitergegeben, Gefallen erbeten, Drohung, Zusage)
├── commitments        Zusagen/Termine
└── topics_learned     neues Spielerwissen (über das Archiv, Ermittlung 04)
```

**Entwurfsregeln:**

1. **Das Dialogsystem ändert den Weltzustand nur über Ereignisse** (Modul 24), nie durch direkte Writes in Beziehungs- oder Gedächtnisdaten.
2. **Der Kontext enthält Bänder statt Zahlen.** Die Zuordnung von Dimensionen zu Bändern ist Teil des Beziehungsmodells (Modul 08 §7).
3. **Der Kontext ist ein Schnappschuss.** Er ändert sich während eines Gesprächs nur durch Ereignisse im Gespräch.
4. **Fakten kommen aus dem Weltzustand.** Eine Zeile darf nichts behaupten, was der Kontext nicht stützt (N-DI-016).
5. **Das Archiv ist die Brücke zum Spieler.** Was der Spieler erfährt, landet nach Regeln im Archiv (Ermittlung 04), nicht durch Spiegeln des NPC-Wissens.

---

## 3. Entwurf: Bedingungen für Zeilen

`M-PD` verlangt, dass Zeilen an Fall-/Weltzustand, Wissen und Beziehung/Erinnerung gebunden sein können (N-DI-001). Eine Zusammenstellung der Bedingungsarten (💡):

| Bedingungsart | Beispiel |
|---|---|
| **Beziehungsband** | Vertrauen ab ‚offen‘, Angst ‚hoch‘, Verdacht ‚niedrig‘ |
| **Erinnerung** | Tag `gerettet_von_spieler`, `belogen_worden`, `gemeinsamer_fall`, Alter der Erinnerung, Sicherheit |
| **Wissen** | NPC kennt/ahnt das Thema; Spieler kennt es |
| **Gerücht** | NPC hat von X gehört |
| **Zustand** | verletzt, betrunken, müde, in Eile, in Gefahr |
| **Zeit/Ort** | Tageszeit, Wochentag, Feiertag, Ort (Arbeit, Zuhause, Bar) |
| **Anwesende** | Zuhörer, Familie, Vorgesetzte (beeinflussen Offenheit) |
| **Fallstatus** | Stadium des Falls; Beweislage |
| **Fraktion/Rang** | Mitglied, Rang, Konflikte |
| **Aussehen des Spielers** | Verkleidung, Narben, Veränderung (‚Sie sehen anders aus‘); Fahrzeug |
| **Lebenslage** | Job, Wohnung, Familie, Krankheit |
| **Welt** | Wetter, Katastrophe, Medienlage |
| **Eigenschaften des Spielers** | Kleidung, Verletzungen, Geruch, Bewaffnung |

**Regeln (Entwurf):**

1. **Spezifität gewinnt.** Eine konkrete, auf ein Ereignis passende Zeile hat Vorrang vor einer allgemeinen (der Sheriff-Fall der Quelle: konkrete Erinnerung statt Schwellenwert).
2. **Fallback.** Es gibt immer eine allgemeine Zeile, damit kein Gespräch leer bleibt.
3. **Bedingungen sind prüfbar.** Build-Validierung findet unerfüllbare oder widersprüchliche Bedingungen (`M-PD`: Validierung, Lokalisierung, QA).
4. **Keine versteckte Zahl in den Bedingungen.** Bedingungen auf Bänder und Tags, nicht auf Rohwerte (N-EN-421).

---

## 4. Entwurf: Dialogtiefe nach Stufe

Die Quelle verlangt Dialogtiefe nach Bedeutung (N-DI-009). Gemeinsam mit den Persistenzstufen (Modul 03 §4) ergibt sich:

| Stufe | Dialogart (Entwurf) | Inhalt | Quelle |
|---|---|---|---|
| **A: Haupt-/Story-/enge Figuren** | handgeschrieben, extrem tief; Voice/Character Bible; große Barkmengen; individuelle Beziehungsgeschichte | Story, Beziehung, Alltag, Reaktion auf Ereignisse | N-DI-009, N-DI-012 |
| **A/B: wiederkehrende Nebenfiguren** | tief, modular; Erinnerungs-Hooks; mehrere Varianten je Information | Auskunft nach Beziehung (Sheriff, Gerichtsmediziner, Informant) | N-DI-009; [Ermittlung 12](../Ermittlung/12-Zeugen-Verdaechtige-und-Verhoer.md) §13 |
| **B: Händler, Nachbarn, Arbeitgeber** | Bausteine plus einige geschriebene Kerngespräche | Alltag, Gerüchte, Aufträge | 💡 |
| **C: gewöhnliche Einwohner** | wiederverwendbare Absichten und Aussagebausteine; Kontextthemen (Ort, Wetter, Gerüchte, Polizei, aktuelle Ereignisse) | kurze, kontextbezogene Gespräche | N-DI-009, N-DI-010 |
| **D: Menge** | Barks und Hintergrundgemurmel | Atmosphäre, Hinweise | 💡 |
| **Spontan** | optional KI-gestützte Formulierung innerhalb eines Faktenrahmens | Tiefe ohne Handarbeit | `48#150A` (💡) |

**Hinweis zu Zahlen.** `M-PD` nennt für Hauptfiguren Hunderttausende, für wichtige Nebenfiguren 10.000–50.000+ Zeilen, überhaupt sehr große Datenmengen; die Zahlen sind ausdrücklich offen (N-DI-012). Dieses Modul legt keine Zahlen fest.

**Wie Beziehung die Tiefe verändert.** Je tiefer die Beziehung zu einem NPC, desto mehr Varianten und persönliche Themen werden freigeschaltet: Wer sich vertraut, erzählt Privates (Tabu- und private Themen: N-DI-008). Das steuert der Kontext (Abschnitt 2), nicht ein ‚Freundschaftslevel‘.

---

## 5. Entwurf: Erinnerungsbezogene Dialoge

Die Quelle fordert Dialoge, die konkret Bezug nehmen (Sheriff-Beispiel). Entwurf:

### 5.1 Hooks

Ein **Erinnerungs-Hook** ist eine Dialogbedingung, die an einen Erinnerungstag gebunden ist (Modul 05 §7). Beispiele für Tags (Szenarien aus Modul 05 §8): `gerettet`, `bedroht`, `belogen`, `bestohlen`, `gemeinsamer_fall`, `übernatürlicher_zeuge`, `familienverlust`, `aus_der_not_geholfen`, `im_stich_gelassen`, `verschont`.

### 5.2 Auswahlregeln (Entwurf)

| Regel | Inhalt |
|---|---|
| **Relevanz** | Hooks passen zum Gesprächsthema oder zum Anlass |
| **Gewicht** | prägende Ereignisse (Anker) gehen vor kleinen |
| **Frische** | frische Erinnerungen werden öfter erwähnt als alte |
| **Wiederholung** | ein Hook wird nicht bei jedem Gespräch wiederholt; es gibt Erschöpfung und Abkühlzeit |
| **Gültigkeit** | der Hook darf nur ein Ereignis nennen, das im Weltzustand existiert (N-EN-424) |
| **Unsicherheit** | Sicherheit und Genauigkeit wirken auf die Formulierung (‚Ich glaube, es war …‘, ‚Wenn ich mich richtig erinnere …‘, N-GE-011) |
| **Verzerrung** | Falsche Erinnerungen dürfen sich in falschen Aussagen niederschlagen; der Test prüft Aussage gegen Erinnerung, nicht gegen die Wahrheit |
| **Perspektive** | der NPC spricht aus seiner Sicht; andere sehen es anders |
| **Rolle** | Beruf und Beziehung bestimmen, ob er es erwähnt (Takt, Tabu) |
| **Gerüchte** | Hörensagen wird als solches formuliert (‚Man erzählt sich …‘) |

### 5.3 Wiedersehen

Wiedersehensreaktionen (Freude, Überraschung, Distanz, Nostalgie, Ärger, erneute Anziehung: N-GE-016) sind Dialogvarianten mit Bedingungen **Zeit seit dem letzten Treffen**, **Veränderung des Spielers** und **Erinnerungsqualität**. Sie sind keine Pflichtszene, sondern eine Auswahl aus Varianten (N-GE-017: die Beziehung entwickelt sich neu).

### 5.4 Fehler vermeiden

1. **Keine Zitate von nie Gesagtem.** Ein NPC erwähnt nur Dinge, die geschehen sind und die er wahrgenommen hat.
2. **Keine Allwissenheit.** Er spricht nicht über Handlungen des Spielers, die er nicht kennen kann (N-GE-008).
3. **Keine veralteten Bezüge.** Nach einem Tod, Umzug oder Zeitsprung wird der Bezug aktualisiert (Modul 14).
4. **Keine Widersprüche zwischen NPCs ohne Grund.** Unterschiedliche Aussagen müssen aus unterschiedlichen Perspektiven kommen.

---

## 6. Entwurf: Beziehung ohne Zahlen sichtbar machen

Die Quelle (N-LT-012, N-BE-002) nennt sechs Kanäle: Ton, Wortwahl, Körpersprache, Zusammenarbeit, Informationen, Zugang, Gefallen und Folgen. Eine **Signalpalette** (💡) verbindet Verhaltensbänder (Modul 08 §7) mit Ausdrucksmitteln:

| Signal | Positive Beziehung | Negative Beziehung | Neutral/Unklar |
|---|---|---|---|
| **Stimmton, Tempo** | warm, ruhig, schnell antwortend | kühl, schneidend, gedehnt | sachlich |
| **Wortwahl** | persönlich, Vornamen, Vertraulichkeit | förmlich, spitz, Ausflüchte | höflich |
| **Anrede** | vertraut, Spitznamen | Nachname, ‚Sir/Ma'am‘, schnippisch | üblich |
| **Blick, Haltung** | zugewandt, offen | abgewandt, verschränkte Arme, Abstand | neutral |
| **Nähe** | geringer Abstand, Berührung | Distanz, Barriere, Rückzug | normal |
| **Gesicht** | Lächeln, Entspannung | Anspannung, Misstrauen | Kontrolle |
| **Verfügbarkeit** | nimmt sich Zeit, ruft zurück | wimmelt ab, Zeitnot | Alltag |
| **Information** | gibt freiwillig, warnt, vertraut Geheimes an | verschweigt, lügt, lenkt ab | nur Gefragtes |
| **Zugang** | öffnet Archive, Türen, Kontakte | sperrt, meldet | regelkonform |
| **Gefallen** | hilft ohne Gegenleistung | verlangt Gegenleistung oder verweigert | je nach Fall |
| **Angebote** | Rabatt, Gratis, Einladung | Aufschlag, Verweigerung des Verkaufs | Standard |
| **Schreibstil in Nachrichten** | ausführlich, herzlich, schnell | knapp, kalt, verspätet | normal |
| **Verhalten Dritten gegenüber** | stellt vor, lobt | warnt, spricht schlecht | schweigt |
| **Handlungen** | Hilfe im Notfall, Fürsprache, Aufnahme | Anzeige, Sabotage, Hinterhalt | Zurückhaltung |
| **Folgen** | Dank, Einladung, Treue | Rache, Rufschädigung | – |

**Entwurfsprinzipien:**

1. **Konsistenz über Kanäle.** Eine warme Beziehung zeigt sich in mehreren Signalen gleichzeitig; ein einzelnes widersprüchliches Signal ist ein Zeichen (Täuschung, Stress).
2. **Maskierung ist erlaubt.** Ein NPC kann Gefühle verbergen (Höflichkeit gegenüber Fremden, Täuschung bei Gegnern); der Spieler muss lesen lernen. Die Maskierung ist Persönlichkeit und Lage, kein Zufall.
3. **Der Spieler darf sich irren.** Fehllesen ist Teil der Welt (Ermittlung 12: Wahrheit und Aussage getrennt).
4. **Signale sind ausdrucksstark, aber gebändigt.** Eine kleine Zahl klar unterscheidbarer Bänder pro Kanal; Feinheiten durch Variation.
5. **Kein Abbildungsspiel.** Es gibt kein Menü ‚Beziehung prüfen‘. Wer einen Hinweis will, spricht mit Dritten oder beobachtet.
6. **Barrierefreiheit.** Wichtige Zustände sind nicht nur über Ton oder Farbe erkennbar (Untertitelhinweise wie [flüstert], [zögert]; Animationen); `M-PD` fordert Lokalisierung und Untertitel als Teil der Dialogdaten.

**Lesbarkeit als Risiko.** Proposal 0010 §9 nennt unlesbare Feindschaft als Risiko. Für Nemesis sichert Modul 11 §11 Vorzeichen; für Alltagsbeziehungen sichert diese Palette, dass Veränderungen bemerkbar sind (N-EN-422).

---

## 7. Entwurf: Barks und Hintergrundgespräche

**Barks.** Kurze Gameplay-Zeilen reagieren auf Lage: Kampf, Stealth, Erkundung, Verletzung, Fahren, Weltereignisse, Companion-Interaktion (N-DI-006). Für die NPC-Welt gilt (💡):

| Barkart | Auslöser | Bindung an NPC-Zustand |
|---|---|---|
| **Begrüßung/Verabschiedung** | Begegnung | Beziehungsband, Erinnerung, Tageszeit |
| **Reaktion auf den Spieler** | Verhalten, Waffe, Verletzung, Aussehen | Wahrnehmung, Verdacht |
| **Reaktion auf Ereignisse** | Unfall, Schuss, Feuer, Sturm | Rolle, Persönlichkeit (Modul 15) |
| **Reaktion auf Gerüchte** | Gesprächsthema | Wissen (Modul 07) |
| **Kommentare der Begleiter** | Fahrt, Fund, Entscheidung | Beziehung, Doktrin (Modul 19) |
| **Selbstgespräche, Alltag** | Arbeit, Warten, Wetter | Persönlichkeit, Beruf |

**Wiederholungsvermeidung.** Variation durch Pools, Abkühlzeiten, Zustandsbindung (N-DI-006).

**Hintergrundgespräche zwischen NPCs** (N-DI-011): Sie nutzen die Einträge der Sprecher (Modul 07 §7) und verwenden denselben Dialogkontext, nur mit zwei NPC-Sprechern. Der Spieler hört Fragmente. Atmosphäre-Gespräche ohne Weltbezug sind erlaubt, aber nicht als Information geführt.

**Unterbrechung.** Der Spieler kann ein Gespräch unterbrechen oder betreten; NPCs setzen kontextgerecht fort (N-DI-007).

---

## 8. Entwurf: Telefon und Nachrichten

NPCs kommunizieren mit dem Spieler auch auf Distanz (N-DI-021). Entwurf (💡):

| Aspekt | Beschreibung |
|---|---|
| **Kontaktaufnahme durch NPCs** | sie melden sich von sich aus: Hilfe, Einladung, Warnung, Neuigkeit, Gefallen, Drohung; Häufigkeit nach Initiative und Beziehung |
| **Kanal** | nach Kommunikationsstil (Modul 04 §7): Anruf, Text, Sprachnachricht |
| **Inhalt** | aus Erinnerung, Wissen, Zustand; nicht aus Zufallstexten |
| **Verzögerung** | Antwortgeschwindigkeit ist ein Zeichen (aber nicht automatisch ein Malus) |
| **Nicht erreichbar** | Funkloch, ausgeschaltet, beschäftigt, im Einsatz (Modul 13, 07) |
| **Verknüpfung mit Beziehung** | wer nie anruft, hat nicht notwendig Groll; Persönlichkeit zählt (N-BE-006) |
| **Gruppenchats** | Crew, Familie, Freunde, Fraktionen; Gerüchte laufen dort (Modul 07) |
| **Fotos, Medien** | NPCs schicken Bilder, Links, Beweise; digitale Spuren (N-IN-015) |
| **Falsche Absender** | Täuschung (Identitätsdiebstahl, Formwandler: Ermittlung 13) |

**Regel:** Ein Anruf oder eine Nachricht ist ein **Ereignis**; sie erzeugt Erinnerung und kann Beziehung verändern.

---

## 9. Entwurf: Inspektion und Fehlersuche

Die NPC-Simulation ist nur beherrschbar, wenn man erklären kann, **warum** ein NPC etwas tut. Das Proposal 0010 §4 Punkt 8 nennt dafür ‚Debug and explainability‘; die Prompt-Entwurfstitel der Spur 05 nennen NPC-Memory-Inspektor, Beziehungs-Inspektor ‚hidden from shipping UI‘ und KI-Verhaltens-Inspektor (`M-P05`, DRAFT). Entwurf (💡):

| Werkzeug | Zeigt | Zugang |
|---|---|---|
| **Gedächtnis-Inspektor** | Einträge, Quellen, Wichtigkeit, Sicherheit, Genauigkeit, Verzerrungen, Verdichtungen | Entwicklung, Mod-/Debug-Modus |
| **Beziehungs-Inspektor** | Kanten, Dimensionen, Stufen, Anker, jüngste Ereignisse | wie oben (N-DI-019) |
| **Verhaltens-Inspektor** | aktueller Zustand der Zustandslogik, Ziele, Plan, Begründung der Wahl | wie oben |
| **Zeitplan-Inspektor** | Plan, Abweichungen, Aufenthalt | wie oben |
| **Gerüchte-Inspektor** | Gerüchte, Halter, Verzerrung, Pfad | wie oben |
| **Fraktions-Inspektor** | Zustand, Strategie, Beziehungen, Territorium | wie oben |
| **Nemesis-Inspektor** | Datensatz, Modell, Anpassungen, Begründung | wie oben |
| **Ereignis-Journal-Viewer** | alle Ereignisse mit Ursache, Beteiligten, Ordnung | wie oben |
| **Replay** | gleiche Eingaben, gleiche Ausgaben (Determinismus, Modul 22) | Entwicklung, Tests |
| **Konsistenzprüfer** | prüft Invarianten (Modul 24) | Entwicklung, automatisierte Tests |

**Regeln:**

1. **Nicht im Shipping-UI** (N-DI-018, N-DI-019); in Mod-/Debug-/Cheat-Modus erlaubt (Precedent in Adult 05 §2).
2. **Nur lesend** (außer ausdrückliche Cheat-Befehle).
3. **Quelle der Wahrheit bleibt der Weltzustand.**

---

## 10. Sonderfälle

1. **Der Spieler spricht mit einem NPC, der ihn nicht kennt, aber alles über ihn gehört hat.** Gerüchte-Dialog.
2. **Der Spieler tut so, als sei er jemand anderes.** Dialogvarianten für Verkleidung und Tarnidentität (Ermittlung 13 §10).
3. **Zwei NPCs mit demselben Wissen reden gleichzeitig.** Reihenfolge, Unterbrechung.
4. **Der NPC lügt.** Lügen sind Aussagen, nicht Erinnerung; das System markiert sie (Modul 05 §11).
5. **Der NPC hat ein Tabu.** Das Thema ist nicht ansprechbar oder führt zum Abbruch (N-DI-008).
6. **Der NPC ist unfähig zu sprechen.** Bewusstlos, tot, sprachlos (Ermittlung 12 §5).
7. **Fremdsprachen, Akzente.** Region und Herkunft (N-ID-021); Mehrsprachigkeit offen.
8. **Der Spieler hört nichts** (Funkloch, Lärm, Gehör). Barrierefreiheit durch Untertitel.
9. **NPC-Dialog nach Spielerabwesenheit.** Zeitsprung: Der NPC erwähnt die Zeit, die vergangen ist.
10. **Mod-Dialoge.** Zeilen registrieren Bedingungen additiv (Modul 23).
11. **Bedingungskonflikte.** Zwei Zeilen gleichen Ranges: deterministische Wahl.
12. **Kinder-NPCs.** Dialoge unterliegen den Regeln für Minderjährige (Adult 01; N-FA-019).
13. **Tote NPCs.** Erinnerungen an sie werden von anderen erwähnt (Modul 05 §9).
14. **Satire, Humor, Ironie.** Ton gehört zur Persönlichkeit (N-DI-008).

---

## 11. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-421 | Dialogbedingungen verwenden Bänder und Tags, keine Rohzahlen | N-DI-002 |
| N-EN-422 | Eine Änderung eines Beziehungsbandes ist über mindestens einen Kanal wahrnehmbar | N-DI-017 |
| N-EN-423 | Keine Zeile zeigt eine Beziehungszahl, Prozent oder Balken | N-LT-012 |
| N-EN-424 | Ein Erinnerungs-Hook nennt nur Ereignisse, die im Weltzustand existieren und die der NPC wahrgenommen hat | N-GE-008 |
| N-EN-425 | Ein Gespräch behauptet nichts, was der Dialogkontext nicht stützt | N-DI-016 |
| N-EN-426 | Dieselbe Eingabe liefert dieselbe Zeilenauswahl | Modul 22 |
| N-EN-427 | Es gibt für jedes Thema eine Fallback-Zeile | Abschnitt 3 |
| N-EN-428 | Ein Hook wird nicht bei jedem Gespräch wiederholt | Abschnitt 5 |
| N-EN-429 | Erinnerungsunsicherheit wirkt auf die Formulierung | N-GE-011 |
| N-EN-430 | Eine Zeile mit geändertem Text markiert vorhandene Aufnahmen als veraltet | N-DI-014 |
| N-EN-431 | Die Inspektoren sind im Shipping-Modus nicht erreichbar | N-DI-018, N-DI-019 |
| N-EN-432 | Das Dialogsystem verändert den Weltzustand ausschließlich über Ereignisse | Abschnitt 2 |
| N-EN-433 | NPCs ohne Wissen über ein Thema antworten nicht, als wüssten sie es | N-GE-008 |
| N-EN-434 | Wichtige Zustände sind nicht ausschließlich über Ton oder Farbe vermittelt | Abschnitt 6 |

---

## 12. Was fehlt

- Kein Dialogformat, keine Tag-Liste, keine Bänder, keine Barkmengen (Zahlen offen: N-DI-012).
- Offen: das Kommunikationsstil-Modell (N-DI-021).
- Offen: freie KI-Dialoge und ihre Grenzen (N-DI-015, N-DI-016).
- Offen: Sprachen und Mehrsprachigkeit der Bevölkerung.
- Offen: die Regeln für Kinder-NPCs im Dialog (Rechts- und Politikfrage).
