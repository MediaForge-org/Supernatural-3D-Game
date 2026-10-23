# NPCs 17: Fraktionen – Territorien, Konflikte und Machtverschiebung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie Gruppen Gebiete beanspruchen, Konflikte austragen und ihre Macht verschieben, auch ohne den Spieler. Beschlossen sind die Richtung (Territoriumssimulation mit Ressourcen und regionaler Entwicklung bei Monsterorganisationen, Machtkämpfe, regionale Folgen, Persistenz im Host-Speicherstand; Territorien, Einflussbereiche und Rivalitäten bei menschlichen Gruppen) und die Anforderung D (`PE-N1022`); **Zustandsmodell, Konfliktstufen und Zeitschritte sind Entwurf (💡)**. Statuszeichen: [Legende](README.md#statuslegende).

Teil der NPC-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Register: [Modul 26](26-Entscheidungsregister.md). Struktur und Mitglieder der Gruppen: [Modul 16](16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md). Monsterökologie, Lebensraum, Wanderung: [Kreaturen 08](../Kreaturen/08-Oekologie-Populationen-und-Lebenszyklus.md); Monstergesellschaft: [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §4. Kriminalität in der Welt: [Ermittlung 25](../Ermittlung/25-Kriminalitaet-in-der-Welt.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Territorium](#2-entwurf-territorium)
3. [Entwurf: Ressourcen und Bevölkerung der Gruppe](#3-entwurf-ressourcen-und-bevölkerung-der-gruppe)
4. [Entwurf: Strategiezustände](#4-entwurf-strategiezustände)
5. [Entwurf: Konflikte](#5-entwurf-konflikte)
6. [Entwurf: Machtverschiebung](#6-entwurf-machtverschiebung)
7. [Entwurf: Folgen für die Welt](#7-entwurf-folgen-für-die-welt)
8. [Entwurf: Der Spieler im Gefüge](#8-entwurf-der-spieler-im-gefüge)
9. [Entwurf: Zeitschritte und Ferne](#9-entwurf-zeitschritte-und-ferne)
10. [Entwurf: Beispiele](#10-entwurf-beispiele)
11. [Sonderfälle](#11-sonderfälle)
12. [Entwurf: Prüfregeln](#12-entwurf-prüfregeln)
13. [Was fehlt](#13-was-fehlt)

---

## 1. Was beschlossen ist

| ID | Aussage | Status | Quelle |
|---|---|:---:|---|
| N-TE-001 | Monstergruppen beanspruchen Gebiete und verteidigen sie; Machtkämpfe bestehen aus internen Konflikten, Führungswechsel und Abspaltungen; Allianzen: Gruppen können zusammenarbeiten oder sich bekämpfen | 📋 | „Supernatural Monster-Gesellschaft: Organisationen, Hierarchien, Territorien und Machtkämpfe“ in `48#141E`; Tabelle `48#142A` (💡) |
| N-TE-002 | **Umfassende Territoriumssimulation mit Ressourcen und regionaler Entwicklung:** Eroberung, Verteidigung, Expansion, Gebietsverluste; Ressourcen (Nahrung, Verstecke, sichere Rückzugsorte, artspezifische Bedürfnisse); Populationen (Wachstum, Verluste, Wanderungen); strategische KI (Expansion, Rückzug, Konflikte); Machtverhältnisse; regionale Folgen (Angriffe, Vermisstenfälle, NPC-Verhalten, Sicherheitslage); Persistenz im Host-Spielstand | 📋 | „Supernatural dynamische Monsterterritorien: Umfassende Territoriumssimulation mit Ressourcen und regionaler Entwicklung“ in `48#143E`; Tabelle `48#144A` (💡) |
| N-TE-003 | Diese Entwicklungen geschehen auch ohne direkte Beteiligung des Spielers; Rudel A kann Rudel B bedrängen, wenn es mehr Mitglieder und Ressourcen hat; der Ausgang hängt auch von Führung, Verletzungen, Jagderfolg und dem Eingriff des Spielers ab | 💡 (Beispiel) | `48#142A`, `48#144A` |
| N-TE-004 | Entfernte Regionen werden vereinfacht, aktive Gebiete detailliert simuliert | 💡 (Hinweis des Assistenten) | `48#144A` ‚Technisch wichtig‘ |
| N-TE-005 | Der Tod eines Rudelführers lässt das Rudel nicht verschwinden: Nachfolger, Machtkampf oder Übernahme des Territoriums durch ein Nachbarrudel | 💡 (Beispiel) | `48#142A` |
| N-TE-006 | Territorien menschlicher Gruppen: kriminelle Gruppen haben Einflussbereiche; Geschäfte, rivalisierende Gebiete und Einflussbereiche sind für Mitglieder sichtbar; Gebietserweiterung nur für Mitglieder | 📋 | `23#1056A` Fragen 52–54; Antworten in `23#1057E`; `23#1058A` ‚Territorien‘ |
| N-TE-007 | Gangkonflikte gehören zur unabhängig vom Spieler laufenden Kriminalität; Täter können fliehen oder festgenommen werden; die Polizei reagiert auch ohne Spieler | 📋 | `23#1056A` Fragen 46, 49; „ja schon, sehr komplex“ und „ja sicher“ in `23#1057E` |
| N-TE-008 | Viertel unterscheiden sich in Art und Häufigkeit der Kriminalität, nicht als simple Zonen; Faktoren: Armut, Arbeitslosigkeit, organisierte Kriminalität, Drogenmärkte, Infrastruktur, Polizeipräsenz, Stadtentwicklung, soziale Stabilität; Ethnie ist keine mechanische Ursache | 📋 | `23#1056A` Frage 47; `23#1058A`; [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md) §13 |
| N-TE-009 | Kriminalität kann sich langfristig verändern (Stadtentwicklung, Wirtschaft, Polizeipräsenz) | 📋 | `23#1056A` Frage 51; „ja“ in `23#1057E` |
| N-TE-010 | Fraktions-Rivalitäten, Territorialkonflikte, Verhaftungen und Führungskrisen erzeugen Probleme ohne vorgeschriebenen Questmarker | 🧭 | `23#680A` (Vorschlag des Assistenten) |
| N-TE-011 | Gruppen können sich verbünden, verraten, spalten und fusionieren | 🧭 | `23#676A` |
| N-TE-012 | Monster tauschen Wissen über Hunter und Gefahren aus; Monster haben Beziehungen zu Menschen, anderen Monstern und Hunter-Gruppen | 📋 | `48#139E`, `48#141E`; [Kreaturen 09](../Kreaturen/09-Intelligente-Wesen-und-Monstergesellschaft.md) §5 |
| N-TE-013 | Territorium und Wanderung folgen Nahrung, Hunter-Druck, Feuer, Überschwemmung, Rivalen, übernatürlichen Ereignissen; vollständig vernichtete Nester bleiben lange weg; Wiederbesiedlung Jahre später durch andere Individuen, Gruppen oder Arten ist möglich | 📋 | `23#1074A`/`23#1075E` Punkte 33–38; [Kreaturen 08](../Kreaturen/08-Oekologie-Populationen-und-Lebenszyklus.md) §1 |
| N-TE-014 | Eine Fraktion hält kein Territorium, das ihre Mitglieder nicht erreichen können (Konsistenzregel) | 💡 | [Proposal 0010](../../Proposals/0010-living-world-npc-memory-and-nemesis.md) §7 Punkt 3 |
| N-TE-015 | Ein ignoriertes Ereignis kann zu mehr Opfern, Polizeieinsatz, Eingreifen eines anderen Hunters, Abwandern einer Kreatur oder einem späteren historischen Fall führen | 📋 | `M-PD` ‚Organic world-event / case emergence‘ |
| N-TE-016 | Hunter-Organisationen, kriminelle Netzwerke, Behörden und andere Gruppen sind Teil dieser Dynamik (Anforderung D) | 📋 | `PE-N1022` D |
| N-TE-017 | Rivalisierende Organisationen können unterschiedliche Fragmente des Wissens halten | 📋 (Planungslinie) | `CANONICAL_ADDENDUM_2026-09-05.md` (Modul 01 §6 F8) |
| N-TE-018 | Konkrete Karten, Grenzen, Gebiete und Fraktionen sind nicht festgelegt; keine Territorien auf Grundlage einer finalen Karte | ❓ | `CLAUDE.md` §3; [Modul 27](27-Konflikte-und-offene-Fragen.md) |

---

## 2. Entwurf: Territorium

**Begriff.** Ein Territorium ist der Teil der Welt, in dem eine Gruppe Einfluss ausübt (Jagd, Schutz, Geschäft, Wohnen). Die Quellen unterscheiden artspezifische **Lebensräume** der Monster (Kreaturen 08), **Einflussbereiche** menschlicher Gruppen und **Zuständigkeitsgebiete** von Behörden (Jurisdiktionen: `23#1056A` Fragen 57–58). Der Entwurf (💡):

```text
Claim (Anspruch)  =  (Fraktion, Gebiet, Art, Stärke, Sichtbarkeit, Beginn)
  Art:         Jagdgebiet | Nest/Bau | Revier (Geschäft) | Hoheitsgebiet | Schutzzone | Routen
  Stärke:      Kontrolle  (nominell | umkämpft | etabliert | uneingeschränkt)
  Sichtbarkeit: offen | bekannt | verdeckt | unbekannt
```

**Gebiete sind Daten, keine Geometrie.** Das Modell arbeitet mit Orts-IDs und Verbindungen (Straßen, Gelände, Wege); die Dokumentation legt keine Karte fest (`CLAUDE.md` §3). Aufgelöst wird über eine austauschbare Zuordnung von Gebiets-IDs zu Orten.

**Regeln (Entwurf):**

1. **Erreichbarkeit.** Ein Anspruch braucht Zugangswege für die Mitglieder (N-TE-014).
2. **Überlagerung.** Mehrere Ansprüche können sich überlappen; das erzeugt Konflikte.
3. **Stärke ist Zustand, nicht Wert.** Jede Stufe hat Verhalten (Patrouillen, Schutzgeld, Warnschilder, Spuren).
4. **Sichtbarkeit.** Offen sichtbare Reviere (Graffiti, Patrouillen) stehen neben verdeckten (Nester, Verstecke).
5. **Artregeln.** Monsterterritorien folgen Artbedürfnissen (Tageszeit, Wasser, Dunkelheit, Nahrung, Schutzzonen; Kreaturen 08).
6. **Kein Gebiet ohne Grund.** Ansprüche entstehen aus Ressourcen, Geschichte, Bedarf (Nahrung, Schutz, Geschäft).
7. **Orte mit Gewicht.** Bestimmte Orte (Haunted Properties, Kultstätten, Friedhöfe, Hotspots: Kreaturen 16) ziehen Ansprüche an.

**Karte für den Spieler.** Gangreviere sind nicht standardmäßig farbig (N-TE-006); eine spezielle Ansicht gibt es, wenn der Spieler tief in die Struktur einsteigt (Einflussbereiche, Geschäfte, rivalisierende Gebiete: `23#1058A`). Monsterterritorien sind **nie** als Kartenebene sichtbar; der Spieler erkennt sie über Spuren und Wissen.

---

## 3. Entwurf: Ressourcen und Bevölkerung der Gruppe

N-TE-002 nennt Ressourcen (Nahrung, Verstecke, sichere Rückzugsorte, artspezifische Bedürfnisse) und Populationen (Wachstum, Verluste, Wanderungen). Der Entwurf (💡):

| Ressource | Monstergruppen | Menschliche Gruppen |
|---|---|---|
| **Mitglieder** | Anzahl, Alter, Verletzungen | Anzahl, Rang, Loyalität |
| **Nahrung/Einkommen** | Beute, Opfer, Nahrungsquellen | Einnahmen aus Geschäft, Schutzgeld, Arbeit |
| **Unterschlupf** | Bau, Nest, Haus, Höhle, Friedhof | Clubhaus, Safehouse, Lager |
| **Mittel** | Fähigkeiten, Zauber, Wissen | Waffen, Fahrzeuge, Geld, Ausrüstung |
| **Informanten** | Menschen als Werkzeuge, Spione | Spitzel, Beamte, Medien |
| **Wissen** | Taktikvorrat, Hunter-Wissen | Pläne, Namen, Polizeiinfos |
| **Bündnisse** | andere Gruppen, Menschen | Kontakte, Verbündete |
| **Ruf/Abschreckung** | Furcht | Respekt, Furcht, Leumund |

**Bevölkerungsentwicklung (Entwurf).** Gruppen wachsen (Rekrutierung, Fortpflanzung bei Arten, die es kennen: N-TE-013), schrumpfen (Verluste, Austritte, Hunter-Druck), wandern (Nahrungsmangel, Gefahr). Bei Monsterarten folgt das den Artregeln (Kreaturen 08 §6–§7).

**Wechselwirkungen.** Ressourcen begrenzen Handlungen (eine Gruppe ohne Geld kann keine Waffen kaufen; ein Nest ohne Beute wandert). Die Welt ist knapp; knappe Ressourcen erzeugen Konflikt.

---

## 4. Entwurf: Strategiezustände

Proposal 0010 §4 Punkt 5 schlägt ein Fraktionsmodell als **Zustandsmaschine mit grober Zeitschrittweite** vor. Der Entwurf nennt die Zustände (💡):

| Zustand | Bedeutung | Typische Aktionen |
|---|---|---|
| **Konsolidieren** | Stabilisieren, Wiederaufbau | Ressourcen sammeln, Mitglieder halten |
| **Expandieren** | Gebiet oder Einfluss ausdehnen | Rekrutieren, Revier erweitern, Chapter gründen |
| **Verteidigen** | Gebiet halten | Patrouillen, Verstärkung, Bündnisse |
| **Angreifen** | Konflikt aktiv führen | Überfälle, Gefechte, Anschläge |
| **Verhandeln** | Abkommen suchen | Boten, Treffen, Tribut |
| **Verstecken, Untertauchen** | Schlagkraft verbergen | Rückzug, Tarnung |
| **Rückzug, Wandern** | Gebiet aufgeben | Umzug, Flucht |
| **Rache** | gezielt vergelten | Anschläge, Hinterhalt, Intrigen |
| **Zerfall** | Auflösung | Austritte, Spaltung |
| **Wiederaufbau** | Erholung nach Niederlage | Heilung, Anwerben |
| **Beobachten** | Informationsgewinn | Aufklärung |
| **Hegemonie** | dominiert Region | Tribut, Kontrolle |

**Übergänge (Entwurf).** Ein Zustandswechsel hat Auslöser aus Ressourcen, Verlusten, Führung, Gelegenheit und Beziehungen; es gibt Mindestdauern und Hysterese, damit Gruppen nicht flackern. Eine Gruppe führt **mehrere Ziele**, der Zustand ist der aktuell dominante. Die Auswahl folgt Führungs-Persönlichkeit und Gruppenwerten (Modul 04, 16).

**Führungsabhängigkeit.** Der Zustand und die Aktionen hängen von der Führung ab (aggressiv, vorsichtig, rachsüchtig). Führungswechsel ändert oft den Kurs (Modul 16 §7).

**Strategische KI.** `48#144A` nennt „Strategische KI: Monsterorganisationen treffen Entscheidungen über Expansion, Rückzug und Konflikte“. Der Entwurf lässt sie datengetrieben aus Zustand, Zielen und Gelegenheiten entscheiden (Mod-Erweiterbarkeit); wie sie intern arbeitet, ist offen (❓ Q-N08 verwandt).

---

## 5. Entwurf: Konflikte

### 5.1 Anlässe

| Anlass | Beispiel |
|---|---|
| Gebiet | Grenzverletzung, Streit um Revier, Nahrung |
| Geschäft | Konkurrenz, Schutzgeld, Schmuggelroute |
| Rache | Mord an Mitglied, Verrat |
| Ehre/Rang | Beleidigung, Machtkampf |
| Ideologie/Glaube | Konflikt zwischen Weltbildern |
| Ressourcen | Nahrung, Geld, Waffen |
| Gelegenheit | Schwäche des Gegners |
| Eingriff Dritter | Hunter, Polizei, Spieler |
| Spaltung | zwei Hälften einer Gruppe |

### 5.2 Eskalationsstufen (Entwurf)

```text
Spannung ─► Drohung ─► Zwischenfall ─► Vergeltung ─► offener Konflikt ─► Krieg
   ▲                                                                       │
   └──────────── Deeskalation, Verhandlung, Waffenstillstand ◄─────────────┘
```

| Stufe | Merkmale | Sichtbarkeit für den Spieler |
|---|---|---|
| **Spannung** | Beleidigungen, Kontrollen, Drohgesten | Gespräche, Graffiti, Atmosphäre |
| **Drohung** | Warnungen, Einschüchterung | Zeugenberichte |
| **Zwischenfall** | einzelne Gewalttat | Polizeibericht, Zeitung |
| **Vergeltung** | Gegenschlag | Folgeereignis |
| **Offener Konflikt** | regelmäßige Gefechte | Absperrungen, Todesfälle, Meidung (Modul 15) |
| **Krieg** | totale Auseinandersetzung | Veränderung des Viertels |

**Regeln (Entwurf):**

1. **Eskalation braucht Ursachen und Zeit**; eine Stufe wird nicht ohne Ereignis übersprungen (N-LT-023).
2. **Deeskalation ist möglich**, durch Verhandlung, Vermittlung, Verlust, Außendruck.
3. **Kollateral.** Konflikte treffen Unbeteiligte (Zeugen, Familie, Geschäfte).
4. **Polizei und Hunter reagieren** (Modul 18).
5. **Artspezifisch.** Monstergruppen kämpfen nach Art und Intelligenz (Rudel-Rituale, Herausforderungen, Jagd).

### 5.3 Ausgang

| Ausgang | Folge |
|---|---|
| **Sieg** | Gebiet, Ressourcen, Mitglieder wechseln; Verlierer zieht ab, wird integriert oder vernichtet |
| **Niederlage** | Rückzug, Zerfall, Rache |
| **Patt** | Waffenstillstand, Teilung, Dauerkonflikt |
| **Vermittlung** | Abkommen (Tribut, Grenzen) |
| **Außeneingriff** | Polizei, Hunter, Dritte beenden oder verschärfen |
| **Auslöschung** | Gruppe verschwindet; Überlebende als Einzelne (Nemesis-Kandidaten, Modul 11) |

---

## 6. Entwurf: Machtverschiebung

Die Quelle nennt Machtverhältnisse (Allianzen, Rivalitäten, Führungswechsel, Machtkämpfe) und Faktoren (Mitglieder, Ressourcen, Führung, Verletzungen, Jagderfolg, Eingriffe: N-TE-002, N-TE-003). Der Entwurf (💡):

**Relative Macht** ist **ein abgeleiteter Wert**, der aus den Merkmalen der Gruppen berechnet wird, und keine Spielerinformation:

```text
Macht(Gruppe, Gebiet) = f(Mitglieder, Kampfkraft, Ressourcen, Führung, Moral, Verletzungen,
                          Bündnisse, Information, Gelände, Rückhalt)
```

**Eigenschaften:**

1. **Abgeleitet, nicht gespeichert.** Zahlen erscheinen nicht im Spielstand als Hauptwert.
2. **Kontextabhängig.** Eine Gruppe ist im eigenen Gebiet stärker.
3. **Nicht nur Zahlen.** Moral, Führung, Information, Überraschung zählen.
4. **Verschiebung durch Ereignisse.** Verluste, Siege, Bündnisse, Führungswechsel, Spielereingriff.
5. **Trägheit.** Macht verschiebt sich über Wochen bis Monate, nicht sprunghaft (N-PS-016).
6. **Rückkopplung.** Wer gewinnt, wird bekannter, zieht Mitglieder an (positive Spirale); Gegenwehr dämpft (Dritte verbünden sich gegen den Stärksten).

**Machtverschiebung als Erzählung.** Die Welt kann Verläufe haben, die der Spieler als Geschichte versteht: Aufstieg einer Gruppe, Fall einer anderen, Wiederkehr alter Rivalen, Zerfall nach Führerverlust. Das ist die Wirkung der Anforderung D (N-LT-004).

---

## 7. Entwurf: Folgen für die Welt

Die Quellen nennen als regionale Folgen: Veränderungen bei Angriffen, Vermisstenfällen, NPC-Verhalten, Sicherheitslage (N-TE-002). Der Entwurf (💡):

| Folge | Wirkung | Verweis |
|---|---|---|
| **Vorfälle** | Überfälle, Morde, Verschwinden, Brände | Ermittlung 25, 26; Kreaturen 15 |
| **Sicherheitslage** | Polizeipräsenz, Absperrungen, Ausgangszeiten | Modul 15, 18 |
| **NPC-Verhalten** | Meiden, Wegzug, Schutzgeld, Selbstjustiz | Modul 13, 15 |
| **Wirtschaft** | Geschäfte schließen, Preise, Immobilien | Modul 13 §7 |
| **Gerüchte** | Gesprächsthema | Modul 07 |
| **Fälle** | Hinweise, Verbrechen, Spuren | Ermittlung 09 |
| **Fraktionsbeziehungen** | Bündnisse, Feindschaften | Modul 16 §8 |
| **Tourismus** | Rückgang oder Dark-Tourismus | Modul 15 |
| **Behörden** | Sonderkommissionen, Razzien, Korruptionsskandale | Modul 18 |
| **Orte** | Haunted Properties, Hotspots, Verwahrlosung | Kreaturen 16 |

**Sichtbarkeit.** Der Spieler sieht Folgen als **Zustände der Welt** (leere Straßen, neue Graffiti, Polizei an Kreuzungen, Presse), nicht als Konfliktanzeige. Die Verbindung zu Vorlaufphasen (N-IN-018) stellt sicher, dass Konflikte vor ihren Folgen erkennbar sind.

---

## 8. Entwurf: Der Spieler im Gefüge

| Spielerhandlung | Wirkung auf Fraktionen |
|---|---|
| Anführer töten oder fangen | Nachfolge, Machtkampf, Rache, Gebietsübernahme (N-TE-005) |
| Gruppe schwächen (Waffen zerstören, Informanten ausschalten) | Machtverschiebung |
| Gruppe stützen (Hilfe, Waffen, Information) | Machtverschiebung; Schuld; Verpflichtung |
| Vermitteln | Waffenstillstand, Abkommen |
| Informationen weitergeben (Polizei, Presse, Rivalen) | Folgen für Gruppen; Racheketten |
| Eine Gruppe infiltrieren | Zugang zu Wissen (Modul 16 §6) |
| Gerüchte streuen | Misstrauen, Spaltung (Modul 07) |
| Ein Gebiet vom Monster säubern | Lücke; andere Wesen kommen später (N-TE-013) |
| Nichts tun | Die Welt läuft weiter (N-TE-003) |

**Wirkung der Beobachtung.** Der Spieler kann Konflikte über Spuren, Gespräche, Zeitungen und Kontakte verfolgen. Er muss sie nicht beeinflussen.

**Verantwortung und Folgen.** Eingriffe haben Wirkung bei Dritten; kleine Taten haben kleine Folgen, große Taten große (N-LT-023).

---

## 9. Entwurf: Zeitschritte und Ferne

Anforderung G verlangt reduzierte Simulation in der Ferne und ereignisbasiertes Fortschreiben (N-LT-007); der Assistent erwähnte für Territorien ausdrücklich, dass entfernte Regionen vereinfacht, aktive detailliert simuliert werden (N-TE-004). Der Entwurf (💡):

| Stufe | Taktung | Detailtiefe |
|---|---|---|
| **Aktive Region** (Spielernähe) | Echtzeit, Ereignisse einzeln | einzelne Mitglieder, Gefechte, Patrouillen |
| **Nahe Region** | Stunden bis Tage | Gruppenaktionen als Pakete |
| **Ferne Region** | Tage bis Wochen | Zustand und Ereignisse der Gruppe |
| **Dormant** | Wochen bis Monate | Gruppenzustand, grobe Verschiebung |

**Prinzipien (Entwurf):**

1. **Fraktionen sind die kleinste simulierte Einheit in der Ferne.** Sie werden als Gruppenzustand fortgeschrieben, nicht als Mitgliederliste.
2. **Materialisierung.** Beim Betreten der Region werden die Mitglieder aus dem Gruppenzustand erzeugt, mit konsistenten Verlusten, Verletzungen, Positionen (Modul 22 §4).
3. **Deterministisch.** Seed aus Fraktions-ID, Zeitfenster, Weltzustand; gleicher Spielverlauf → gleiches Ergebnis (Modul 22).
4. **Ereignisse bleiben einzeln.** Wichtige Ereignisse (Tod eines Führers, Fall einer Gruppe) sind einzelne Journal-Einträge.
5. **Die Weltuhr verbraucht Zeit nur beim Spielen** (N-LT-020), aber Zeitsprünge (Schlaf, Reise, Haft) werden nachgeholt (N-NL-011).
6. **Kein Überspringen von Schwellen.** Auch grobe Schritte prüfen Eskalationsstufen nacheinander (N-EN-338).

---

## 10. Entwurf: Beispiele

> 💡 **Beispiele**, ohne Namen, Orte oder Arten.

### 10.1 Zwei Rudel (nach dem Beispiel in `48#144A`)

- **Ausgangslage.** Rudel A im Waldgebiet, Rudel B in der Bergregion; eine umkämpfte Grenze.
- **Dynamik.** A wächst, hat bessere Ressourcen; B verliert Mitglieder durch Hunter-Druck. A beginnt zu expandieren.
- **Ereignis.** Der Anführer von B wird vom Spieler getötet (N-TE-005): B hat Nachfolgekonflikt; A nutzt die Lücke.
- **Folge.** A übernimmt Gebiet; Vermisstenfälle verlagern sich; Gerüchte; Hunter-Hinweise; B-Überlebende wandern ab oder suchen Rache (Nemesis-Kandidaten).

### 10.2 Menschliche Gruppen

- **Ausgangslage.** Zwei Gruppen teilen ein Viertel; Schutzgeldrevier.
- **Dynamik.** Eine Gruppe schwächelt (Verhaftungen); die andere expandiert.
- **Ereignis.** Ein Mitglied wird verraten; Polizei durchsucht; Spaltung.
- **Folge.** Gewalt, Presse, Viertel verändert sich; Geschäfte schließen; Zuzug und Wegzug.

### 10.3 Behörde und Hunter

- **Ausgangslage.** Mehrere Vermisste; die Polizei erklärt sie normal; ein Hunter-Netz beobachtet.
- **Dynamik.** Wenn niemand eingreift, steigen Opfer (N-TE-015); Medien berichten; Vertuschung.
- **Folge.** Sonderkommission; Hunter greifen ein; Wesen wandert.

---

## 11. Sonderfälle

1. **Gruppen ohne Gebiet.** Wandernde Gruppen (Banden, Kulte, Wanderrudel); Ansprüche sind Routen.
2. **Verdeckte Gruppen.** Der Anspruch ist unbekannt; Spuren führen dorthin.
3. **Territorien über Bundesstaatsgrenzen.** Offen.
4. **Zwei Fraktionen übernehmen ein verwaistes Gebiet gleichzeitig.** Wettlauf; Konflikt.
5. **Gebiet mit übernatürlichem Hotspot.** Gruppen konkurrieren um Macht (Kreaturen 16).
6. **Hauptstory-Gebiete.** Autorenregeln; Gruppen dort folgen eigenen Plänen (Modul 11 §12).
7. **Nach Katastrophen.** Machtvakuum; Plünderung; Selbsthilfe (Modul 15).
8. **Spieler als Teil einer Fraktion.** Konflikt der Loyalitäten (Modul 16).
9. **Bürgerwehr.** Eine Gemeinschaft organisiert sich selbst (Modul 15 §5).
10. **Gruppe verliert den Zugang** (Straßen gesperrt, Brücke gesprengt): Anspruch fällt (N-EN-336).
11. **Absichtliche Aufgabe des Gebiets.** Gruppen räumen ein Gebiet freiwillig (Falle, Rückzug, Handel).
12. **Wiederkehr nach Jahren** (N-TE-013): andere Individuen, andere Gruppen.
13. **Koop.** Verschiedene Spieler beobachten verschiedene Regionen; der Host führt die Simulation (Modul 23).
14. **Mod-Gruppen.** Additive Registrierung; Gebiets-IDs über Zuordnung (Modul 23).

---

## 12. Entwurf: Prüfregeln

| ID | Regel | Bezug |
|---|---|---|
| N-EN-336 | Eine Fraktion hält nur Gebiete, die ihre Mitglieder erreichen können | N-TE-014 |
| N-EN-337 | Ein Anführer-Tod führt zu Nachfolge, Machtkampf oder Zerfall, nie zum spurlosen Verschwinden der Gruppe | N-TE-005 |
| N-EN-338 | Eskalationsstufen werden auch in groben Zeitschritten nacheinander durchlaufen | Abschnitt 5 |
| N-EN-339 | Ein Strategiezustand wechselt nicht ohne Auslöser und Mindestdauer | Abschnitt 4 |
| N-EN-340 | Relative Macht ist abgeleitet und erscheint nicht in der Spieler-Oberfläche | Abschnitt 6 |
| N-EN-341 | Die Fernsimulation führt Fraktionen als Zustand; beim Betreten der Region sind Mitglieder konsistent zu diesem Zustand | N-TE-004, Modul 22 |
| N-EN-342 | Eine Fraktion handelt nur mit Ressourcen, die sie hat | Abschnitt 3 |
| N-EN-343 | Ein Konflikt hat einen Anlass und eine Ursache im Journal | Abschnitt 5 |
| N-EN-344 | Folgen eines Konflikts (Vorfälle, Meidung, Gerüchte) treten zeitlich nach dem Konflikt auf | Abschnitt 7 |
| N-EN-345 | Ein vollständig vernichtetes Nest erscheint nicht sofort wieder | N-TE-013 |
| N-EN-346 | Monsterterritorien erscheinen nicht als Kartenebene | Abschnitt 2 |
| N-EN-347 | Menschliche Einflussbereiche sind nur für Mitglieder auf der Karte sichtbar | N-TE-006 |
| N-EN-348 | Ethnische Zugehörigkeit ist kein Faktor der Kriminalitäts- oder Machtberechnung | N-TE-008 |
| N-EN-349 | Dieselben Eingaben liefern dieselbe Machtverschiebung | Abschnitt 9 |
| N-EN-350 | Eine Gruppe wird nicht gleichzeitig an zwei weit entfernten Orten stark simuliert, ohne dass Mitglieder dort sind | Abschnitt 9 |

---

## 13. Was fehlt

- Keine Karte, keine Territorien, keine Fraktionen (`CLAUDE.md` §3, N-TE-018).
- Keine Zahlen zu Machtfaktoren, Eskalationsdauer, Zeitschritten.
- Offen: die strategische KI der Gruppen (Regeln vs. Daten, Q-N08 verwandt).
- Offen: Verhältnis zu Mod-Gebieten und Weltgrößenwechsel (die Zuordnung Gebiets-ID → Ort muss austauschbar sein).
- Offen: Konflikte im Untergrund und zwischen Reichen (Q-N25).
