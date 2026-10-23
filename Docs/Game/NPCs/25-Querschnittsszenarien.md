# NPCs 25: Querschnittsszenarien

> [!IMPORTANT]
> **Nichts davon ist implementiert, und kein Szenario legt Figuren, Orte oder Arten fest.** Dieses Modul spielt zwölf Abläufe über mehrere Module hinweg durch, um zu prüfen, ob die Entwürfe zusammenpassen und wo Lücken bleiben. Die Szenarien sind **Entwurfsbeispiele (💡)**, abgeleitet aus beschlossenen Grundsätzen; jedes nennt die berührten Module, die erwarteten Zustandsänderungen, die für den Spieler sichtbaren Zeichen, die Prüfregeln und die offenen Punkte. Sie sind die Grundlage der Szenariotests in [Modul 29](29-Testkatalog-und-Abnahmekriterien.md). Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Konsistenzregeln: [Modul 24](24-Integration-und-Konsistenzregeln.md).

**Inhalt**

1. [Lesehinweis](#1-lesehinweis)
2. [S1: Zeuge überlebt einen Vorfall](#2-s1-zeuge-überlebt-einen-vorfall)
3. [S2: Der Gefallen nach Jahren](#3-s2-der-gefallen-nach-jahren)
4. [S3: Ein menschlicher Gegner kehrt zurück](#4-s3-ein-menschlicher-gegner-kehrt-zurück)
5. [S4: Ein intelligentes Wesen überlebt](#5-s4-ein-intelligentes-wesen-überlebt)
6. [S5: Vom Clubmitglied zur Crew](#6-s5-vom-clubmitglied-zur-crew)
7. [S6: Haft und Zeitsprung](#7-s6-haft-und-zeitsprung)
8. [S7: Rückkehr in eine lange nicht besuchte Region](#8-s7-rückkehr-in-eine-lange-nicht-besuchte-region)
9. [S8: Koop, ein Zeuge, zwei Spieler](#9-s8-koop-ein-zeuge-zwei-spieler)
10. [S9: Ein Mod wird deaktiviert](#10-s9-ein-mod-wird-deaktiviert)
11. [S10: Ein friedliches Wesen und ein anderer Hunter](#11-s10-ein-friedliches-wesen-und-ein-anderer-hunter)
12. [S11: Ein falsches Gerücht](#12-s11-ein-falsches-gerücht)
13. [S12: Der Tod einer Crew-Figur](#13-s12-der-tod-einer-crew-figur)
14. [Auswertung: was die Szenarien über Lücken sagen](#14-auswertung-was-die-szenarien-über-lücken-sagen)

---

## 1. Lesehinweis

Jedes Szenario hat dieselbe Gliederung:

1. **Ausgangslage** (abstrakt).
2. **Ablauf** als Tabelle: Schritt, System (Modul), Zustandsänderung.
3. **Sichtbar für den Spieler** (nur über Verhalten, Archiv, Welt).
4. **Prüfregeln** (Nummern aus den Modulen).
5. **Offen** (Lücken, die das Szenario aufzeigt).

Die Szenarien benutzen nur Mechaniken, die in den Quellen belegt oder in den Modulen als Entwurf geführt sind. Wo ein Schritt eine Annahme enthält, steht sie in der Spalte „Zustandsänderung“ als Entwurf.

---

## 2. S1: Zeuge überlebt einen Vorfall

**Ausgangslage.** Ein Bewohner (Stufe C) wird nachts im Wald Zeuge eines übernatürlichen Angriffs. Er überlebt verletzt und flieht. Er kennt das Wesen nicht.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Wahrnehmung (Modul 06) | Dunkelheit, Entfernung, Angst: Beobachtung mit schlechter Qualität; er deutet ein „großes Tier“ (Weltbild: Skeptiker) |
| 2 | Gedächtnis (Modul 05) | Erinnerungseintrag der Art `event`: Wichtigkeit hoch (Gefahr), Konfidenz hoch, Genauigkeit niedrig (Verzerrung: Auffüllen); Anker, weil traumatisch |
| 3 | Lebenszustand (Modul 14) | `injured`; Rettungsdienst/Krankenhaus (Ermittlung 26); Genesungsstufen |
| 4 | Persönlichkeit (Modul 04) | Stress, Angst; langfristig Traumafolge möglich |
| 5 | Behörde (Modul 18) | Polizei erklärt: Tierangriff (N-WE-003); Akte |
| 6 | Gerüchte (Modul 07) | Der Zeuge erzählt es Freunden; Gerücht entsteht und verzerrt sich („Wolf“, „Bär“, „Monster“) |
| 7 | Gemeinschaft (Modul 15) | Meidung des Waldes beginnt (N-WE-001) |
| 8 | Persistenz (Modul 03) | Aufstieg: relevante Begegnung für den Spieler? Nur wenn der Spieler ihn trifft; sonst Known/Minimal |
| 9 | Fall (Ermittlung) | Vorlaufphasen: Vermisstenmeldung, Zeitungsnotiz (N-IN-018) |

**Sichtbar für den Spieler.** Zeitungsmeldung „Tierangriff“, Gerüchte beim Einkaufen, ein Wald mit weniger Besuchern, ein Zeuge mit Verband, der widersprüchlich und verängstigt erzählt.

**Prüfregeln.** N-EN-041, N-EN-046, N-EN-061, N-EN-069, N-EN-089, N-EN-291, N-EN-289.

**Offen.** Q-N06 (Obergrenzen des Gedächtnisses), Q-N24 (Gemeinschaftsvariablen).

---

## 3. S2: Der Gefallen nach Jahren

**Ausgangslage.** Der Spieler rettet früh im Spiel einen Beamten (Stufe B) aus einer Notlage. Viele Spielstunden später braucht er Zugang zu Polizeidaten (das Beispiel der Quelle: `23#51A` Abschnitt 9, 💡).

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Ereignis `Rettung` (Modul 09) | Zeugen: der Beamte, vielleicht ein Passant |
| 2 | Gedächtnis des Beamten (Modul 05) | Anker, positive Valenz, hohe Wichtigkeit, Konfidenz hoch |
| 3 | Beziehung (Modul 08) | Kante Beamter→Spieler: Vertrauen↑, Respekt↑, Schuld↑ |
| 4 | Zeit (Modul 21, 22) | Der Beamte lebt weiter: Zeitplan, Beförderung, Umzug (Modul 14); der Anker verblasst nicht im Kern |
| 5 | Wiederbegegnung (Modul 05 §6) | Wiedererkennung: Bekanntheitsstufe „erinnere mich gut“; Veränderung des Spielers beeinflusst nur Zögern |
| 6 | Dialog (Modul 20) | Hook `gerettet`: konkrete Zeile statt Schwellenwert („Nach dem, was damals … ich schulde Ihnen etwas“, als Beispiel der Quelle) |
| 7 | Zugang (Modul 08 §7) | Der Beamte öffnet eine Akte; Ereignis `Zugang gewährt`; der Beamte riskiert etwas (Modul 18) |
| 8 | Archiv (Ermittlung 04) | Spielerwissen: Information aus der Akte |

**Sichtbar.** Ton, Wortwahl und Hilfsbereitschaft des Beamten; keine Zahl.

**Prüfregeln.** N-EN-043, N-EN-052, N-EN-114, N-EN-421, N-EN-424.

**Variante.** Der Beamte ist inzwischen verstorben: Der Hook entfällt; andere NPCs erinnern ihn; Erbe, Trauer, Fall (Modul 14 §8).

**Offen.** Wie lange der Anker wirkt, ob er verjährt (N-GE-032).

---

## 4. S3: Ein menschlicher Gegner kehrt zurück

**Ausgangslage.** Ein Straftäter (Stufe C, wird B) flieht bei einem Überfall verletzt (Bein); der Spieler hat ihn vertrieben, nicht gefasst.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Ausgang (Modul 12 §3) | `candidate`; `injuries`: Bein; Zeugen; Spieler im Gedächtnis des Täters |
| 2 | Polizei (Modul 18) | Fahndung nach Täterbeschreibung (Spieler hat ihn nicht identifiziert) |
| 3 | Hintergrund (Modul 12 §5) | Heilung (Wochen), Versteck, Informationsbeschaffung |
| 4 | Lernen (Modul 11 §6) | Modell: Fahrzeug des Spielers erkannt, Methode (Nahkampf, Waffe) |
| 5 | Ziele (Modul 11 §7) | Rache vs. Überleben: Persönlichkeit entscheidet (vorsichtig → Beobachtung) |
| 6 | Rückkehr (Modul 12 §6) | Gelegenheit: Fahrzeug des Spielers an einem Ort; Bedingungen erfüllt |
| 7 | Vorzeichen (Modul 11 §11) | Spur: bekannter Wagen, Telefonanruf, Gerücht |
| 8 | Konfrontation | Hinterhalt oder Abwarten; Gehbehinderung sichtbar (N-EN-186) |
| 9 | Ausgang | tot, gefangen, verschont, geflohen; Folgen (Gruppe, Familie, Polizei) |

**Sichtbar.** Eine Person mit Humpeln und Narbe, ein bekanntes Auto, Hinweise; im Archiv ein Eintrag, sobald der Spieler Wissen hat.

**Prüfregeln.** N-EN-181, N-EN-183, N-EN-186, N-EN-213, N-EN-218, N-EN-194, N-EN-189.

**Offen.** Q-N08 (Lernmodell), Q-N09 (Anzahl gleichzeitiger Nemesis), Q-N19 (Heilungsdauer).

---

## 5. S4: Ein intelligentes Wesen überlebt

**Ausgangslage.** Ein Spieler versucht, ein intelligentes Wesen in einer vorbereiteten Falle zu fangen. Es entkommt knapp (Beispiel des Assistenten, `48#140A`).

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Taktische KI (Kreaturen 10 §7) | Wesen erkennt/untersucht die Falle, entkommt; Mythologie gilt (kein Entkommen aus fertiger Falle) |
| 2 | Gedächtnis des Wesens | Begegnung, Waffe/Falle, Verletzungen, Gefühle (Angst, Hass, Respekt) |
| 3 | Persistenz | Individuum mit Datensatz (Stufe B/A); Nemesis-Kandidat |
| 4 | Gruppe (Modul 17) | Wesen berichtet innerhalb der Gruppe (plausibler Kanal) |
| 5 | Zweite Begegnung | Wesen erkennt den Spieler, untersucht Räume vorsichtiger (N-NE-004) |
| 6 | Anpassung (Modul 12 §7) | Vorbereitung gegen die bekannte Methode, innerhalb der Artregeln |
| 7 | Langfristig | Rache, Vermeidung oder Angriff auf Verbündete (N-NE-019) |
| 8 | Archiv | Wissen des Spielers wächst (Spuren, Berichte, Gerüchte) |

**Sichtbar.** Spuren, Warnzeichen, ungewöhnlich vorsichtiges Verhalten des Wesens, das die Falle umgeht; Hinweise über andere Hunter und Gerüchte.

**Prüfregeln.** N-EN-181, N-EN-182, N-EN-184, N-EN-187, N-EN-188.

**Offen.** Q-N08; die konkrete Anpassungsmenge je Art.

---

## 6. S5: Vom Clubmitglied zur Crew

**Ausgangslage.** Der Spieler arbeitet für einen optionalen Club (Modul 16). Ein Mitglied (Mechaniker, Stufe B) wird ein Freund.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Mitgliedschaft (Modul 16 §6) | Spieler: Anwärter → Mitglied; Jobs |
| 2 | Beziehung (Modul 09) | Gemeinsame Jobs, Vertrauen, Freundschaft |
| 3 | Ereignis | übernatürlicher Einblick (Spieler und Mechaniker erleben etwas) |
| 4 | Rekrutierungsgelegenheit (Modul 19 §7) | `recruitable`-Flag; Motive; Entscheidung beider |
| 5 | Austritt des Mechanikers aus dem Club (Modul 16 §6.5) | Rang weg; Besitzänderung; Beziehungen im Club ändern sich |
| 6 | HQ-Integration (Modul 19 §8) | Zimmer, Crew-Reaktionen (Misstrauen eines Mitglieds), Konflikte, Vertrauensaufbau |
| 7 | Nutzen | optionale Dialoge/Hilfe in Missionen (nie notwendig) |
| 8 | Club-Folgen | Club reagiert auf Verlust; Gerüchte; mögliche Feindschaft |

**Sichtbar.** Wechsel von Kleidung/Patch, neue Gespräche im HQ, Konflikte am Tisch, Mechaniker repariert Fahrzeuge (Fähigkeiten).

**Prüfregeln.** N-EN-306, N-EN-312, N-EN-319, N-EN-400, N-EN-402, N-EN-403, N-EN-406.

**Offen.** Q-N23 (Zahl der Rekruten, HQ-Kapazität).

---

## 7. S6: Haft und Zeitsprung

**Ausgangslage.** Der Spieler wird festgenommen und kommt in Untersuchungshaft (Ermittlung 16). Er spult die Haft vor (Singleplayer).

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Festnahme (Modul 18) | Fahndung beendet/übergeht in Verfahren; Safehouses und Fahrzeuge bleiben zurück |
| 2 | Welt läuft | Weltzeit-Sprung (Modul 21 §9) |
| 3 | Hintergrund (Modul 22 §3.4) | Fortschreiben in groben Schritten: Fraktionen, Gerüchte, Nemesis, Lebensereignisse |
| 4 | Fälle | ignorierte Ereignisse eskalieren (N-WE-015), Hunter-NPCs lösen Fälle |
| 5 | Nemesis | nutzt die Lage (Safehouse, Familie, Begleiter) |
| 6 | Crew | lebt weiter; Bezüge; Sorge; Hilfe bei Kaution |
| 7 | Entlassung | Rückkehr in die Welt; Materialisierung (Modul 22 §4) |
| 8 | Archiv | Berichte/Medien über die Zeit |

**Sichtbar.** Nachrichten, Briefe, Besuche der Crew, veränderte Stadt; Folgen früherer Ignoranz.

**Prüfregeln.** N-EN-451, N-EN-483, N-EN-478, N-EN-338, N-EN-222.

**Offen.** Q-N10 (wie Verlust/Tod behandelt wird); Koop-Variante: S8.

---

## 8. S7: Rückkehr in eine lange nicht besuchte Region

**Ausgangslage.** Der Spieler reist nach vielen Spielwochen (Weltzeit) in eine Region, die er früher besuchte.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Streaming (Modul 21 §7) | Region wird vorgeladen; Cursor zeigt Zeit der letzten Fortschreibung |
| 2 | Nachholen (Modul 22 §3) | Lebensereignisse der Bewohner, Gruppenkonflikte, Gerüchte, Reparaturen, Wetter, Geschäfte |
| 3 | Eingang (Modul 21 §7) | Ereignisse aus Abwesenheit angewendet |
| 4 | Materialisierung (Modul 22 §4) | Bewohner entsprechend Tageszeit; Straße belebt (N-EN-475) |
| 5 | Wiedererkennung (Modul 05 §6) | Bekannte NPCs erkennen den Spieler (nach Zeit zögernd); Veränderungen sichtbar |
| 6 | Ortswandel | Geschäfte geschlossen/neu, Baustellen, Renovierungen (N-LE-010) |
| 7 | Fälle/Gerüchte | Neuigkeiten; bleibt ein Fall unbearbeitet, hat er sich entwickelt |

**Sichtbar.** Veränderung: neue Frisur/Tattoo bei einem Bekannten, geschlossener Diner, neue Besitzer, Gerüchte über Ereignisse.

**Prüfregeln.** N-EN-247, N-EN-246, N-EN-262, N-EN-263, N-EN-461, N-EN-471.

**Offen.** Q-N27 (Generatorupdate bei nie eingefrorenen NPCs).

---

## 9. S8: Koop, ein Zeuge, zwei Spieler

**Ausgangslage.** Zwei Spieler befragen gemeinsam einen Zeugen. Spieler A ist freundlich, Spieler B droht.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Subjekte (Modul 23 §3) | Zeuge hat Kanten zu A und B getrennt |
| 2 | Dialog (Modul 20) | Kontext je Spieler; Antworten an A offen, an B ausweichend |
| 3 | Gedächtnis | Eintrag mit zwei Subjekten: A freundlich, B bedrohlich; Valenz unterschiedlich |
| 4 | Ruf/Gerüchte | Zeuge erzählt von „zwei Fremden“, einer nett, einer bedrohlich |
| 5 | Teamentscheidung | Beschuldigung eines Verdächtigen per Abstimmung (N-KO-005) |
| 6 | Folgen | Verdächtiger und Gemeinschaft: Zuschreibung folgt der Wahrnehmung |
| 7 | Beitritt/Austritt | Wenn B geht, bleibt seine Kante im Host-Spielstand |
| 8 | Host-Autorität | NPC-Zustand nur auf dem Host fortgeschrieben (N-EN-512) |

**Sichtbar.** Unterschiedlicher Ton gegenüber A und B; Folgen für B bei späteren Begegnungen.

**Prüfregeln.** N-EN-501, N-EN-502, N-EN-503, N-EN-514.

**Offen.** Q-N26 (Teamentscheidungen im Datenmodell, Hostwechsel).

---

## 10. S9: Ein Mod wird deaktiviert

**Ausgangslage.** Ein Mod fügt eine Fraktion mit 30 Mitgliedern und zwei benannte NPCs hinzu; der Spieler hat Beziehungen zu ihnen aufgebaut. Später deaktiviert er den Mod.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Modzustand (Modul 23 §8) | Daten bleiben im Spielstand (`unknown_kinds` / Mod-Segment) |
| 2 | Welt | Mod-NPCs inaktiv (nicht gespawnt, nicht fortgeschrieben); Beziehungen eingefroren |
| 3 | Verweise (Modul 21 §8) | Verweise zeigen auf inaktive Datensätze; keine verwaisten IDs |
| 4 | Fälle | Fälle mit Mod-NPC werden gesperrt oder verlagert (Regel offen) |
| 5 | Gebiet | Anspruch der Mod-Fraktion fällt; andere übernehmen oder Lücke (Modul 17; offen) |
| 6 | Reaktivierung | Zustand wird wieder aufgenommen; die vergangene Weltzeit wird nachgeholt |

**Sichtbar.** Figuren fehlen (Abwesenheit), Orte verändert; beim Reaktivieren Rückkehr.

**Prüfregeln.** N-EN-445, N-EN-447, N-EN-453, N-EN-506, N-EN-507.

**Offen.** Behandlung von Fällen und Gebieten bei Mod-Deaktivierung (Modul 23 §8).

---

## 11. S10: Ein friedliches Wesen und ein anderer Hunter

**Ausgangslage.** Der Spieler verschont ein friedliches Wesen mit menschlicher Identität (`M-PD`). Es lebt weiter: Beruf, Haus, Freunde. Ein NPC-Hunter erfährt von ihm.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Entscheidung | Verschonen; Wesen: Kante zum Spieler (Dankbarkeit/Misstrauen) |
| 2 | Alltag (Modul 13) | Wesen lebt normal; Zeitplan; Beziehungen zu Menschen |
| 3 | Gerüchte/Hinweise (Modul 07) | Auffälligkeiten (Nahrung, Zeiten); ein Hunter erfährt davon |
| 4 | NPC-Hunter (Modul 18 §5) | Eigene Doktrin: Wesen sind gefährlich; er beschließt zu handeln |
| 5 | Ereignis | Angriff auf das Wesen; Wesen wehrt sich oder stirbt |
| 6 | Folgen | Menschen im Umfeld (Trauer, Verdacht), Polizei (Mord?), Gerüchte |
| 7 | Spielerbezug | Der Spieler erfährt davon (Wesen, Freunde, Nachrichten); Nemesis-Möglichkeit gegen den Hunter |
| 8 | Archiv | Spielerwissen aktualisiert |

**Sichtbar.** Verlust einer Figur, Trauer, Konfrontation mit dem Hunter, moralische Fragen; kein Questmarker.

**Prüfregeln.** N-EN-372, N-EN-374, N-EN-379, N-EN-380.

**Offen.** Q-N10 und Q-N09 (Folgen).

---

## 12. S11: Ein falsches Gerücht

**Ausgangslage.** Ein Gerücht behauptet, der Spieler habe jemanden bestohlen. Es stimmt nicht.

| Schritt | System | Zustandsänderung |
|---|---|---|
| 1 | Ursprung (Modul 07 §4) | Falsche Beobachtung/Verwechslung (Formwandler/Ähnlicher); Gerüchteobjekt |
| 2 | Verbreitung | Kreise: Nachbarschaft, Arbeitsplatz; Verzerrung |
| 3 | Beziehungen (Modul 08, 09) | Kanten Dritter → Spieler: Verdacht↑, Vertrauen↓ (falsche Zuschreibung) |
| 4 | Behörde | Anzeige möglich; Ermittlung |
| 5 | Spieler bemerkt | Veränderter Ton, Abweisung, Fragen |
| 6 | Korrektur (Modul 07 §9) | Zeugen, Beweise, Dementi; Gerücht → `refuted` |
| 7 | Nachwirkung | Wer glaubte, muss es erfahren; Groll/Entschuldigung (Modul 09 §6) |
| 8 | Archiv | Spielerwissen: Quelle des Gerüchts |

**Sichtbar.** Kühle Ton, Abweisung, Hinweise von Freunden, Gespräche über den Spieler.

**Prüfregeln.** N-EN-082, N-EN-088, N-EN-090, N-EN-118, N-EN-135.

**Offen.** Q-N16 (Zahl aktiver Gerüchte), Gerüchte aus Formwandlerfällen (Ermittlung 13).

---

## 13. S12: Der Tod einer Crew-Figur

**Ausgangslage A (Sandbox).** Ein rekrutierter Begleiter stirbt in einem Gefecht. **Ausgangslage B (Story).** Eine Kernfigur des HQ stirbt in einer geschriebenen Szene.

| Schritt | A: Sandbox | B: Story |
|---|---|---|
| 1 | Tod folgt aus Gefecht (Eigenfeuer, Verletzung, Monster) | Tod laut Story (geschriebenes Ereignis im Journal) |
| 2 | endgültig im Weltzustand | endgültig (N-CO-007) |
| 3 | Crew-Reaktionen: Trauer, Schuld, Streit | wie A, plus geschriebene Szenen |
| 4 | persönliche Anfragen entfallen | wie A |
| 5 | Beerdigung möglich | geschriebene Trauerfeier |
| 6 | Racheketten gegen Täter (Nemesis) | je Story |
| 7 | Beziehungen verändern sich | wie A |
| 8 | Rekrutenersatz, neue Crew | je Story |

**Schutz.** Kernfiguren sterben nicht durch Sandbox-Zufall (N-CO-007); der Tod in B ist erzählerisch.

**Prüfregeln.** N-EN-395, N-EN-396, N-EN-397, N-EN-398, N-EN-272.

**Offen.** C-N1 (wer außer Spieler und Story töten darf).

---

## 14. Auswertung: was die Szenarien über Lücken sagen

| Befund | Wo sichtbar | Wirkung |
|---|---|---|
| Fast jedes Szenario hängt an der **Obergrenze** und **Verdichtung** des Gedächtnisses | S1, S2, S7 | Q-N03, Q-N04, Q-N06 |
| Die **Zuschreibung** (wer wurde als Täter wahrgenommen) ist die zentrale Variable für Rache und Ruf | S3, S4, S8, S11 | Modul 08 §6 |
| **Gerüchte** tragen viele Szenarien; ihre Mengenbegrenzung ist offen | S1, S11 | Q-N16 |
| **Zeitsprünge** erzeugen viele Folgeereignisse; die Reihenfolge ist entscheidend | S6, S7 | Modul 22 §3, N-EN-472 |
| **Autorenschutz gegenüber Simulation** hat eine offene Grenze | S10, S12 | C-N1 |
| **Mod-Deaktivierung** hinterlässt Lücken in Fällen und Gebieten | S9 | Modul 23 §8 |
| **Koop-Subjekte** brauchen ein klares Datenmodell | S8 | Q-N26 |
| Die **Sichtbarkeit** hängt an Animation, Dialog und Welt, nicht an Menüs | alle | Modul 20 §6 |
| Die **Heilungsdauer** und andere Zeiten sind offen | S3, S6 | Q-N19 |
| Es fehlen **Zahlen** überall | alle | Messplan (Modul 22 §6) |

Das Ergebnis der Szenarien ist **keine Entscheidung**. Es zeigt, dass die Entwürfe zusammenpassen, solange Ereignisse, Besitzer und Zeitordnung (Modul 24) eingehalten werden, und es nennt die Stellen, an denen Entscheidungen des Entwicklers nötig sind ([Modul 27](27-Konflikte-und-offene-Fragen.md)).
