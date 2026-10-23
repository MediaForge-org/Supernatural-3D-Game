# Kreaturen 12: Jagd, Rituale, Schutz und verfluchte Objekte

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, wie der Spieler Kreaturen jagt, sich vorbereitet, Rituale durchführt, sich schützt und mit Artefakten und verfluchten Objekten umgeht. Wirkungen einzelner Rituale, Materialien und Artefakte sind **nicht festgelegt**. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Hauptquellen: `23#359A`/`23#360E` (Kosmologie, Rituale, Artefakte, Schutz), `23#1074A`/`23#1075E` (Block 9K–M).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Der Jagdprozess](#2-der-jagdprozess)
3. [Rituale](#3-rituale)
4. [Schutzmaßnahmen und Barrieren](#4-schutzmaßnahmen-und-barrieren)
5. [Artefakte](#5-artefakte)
6. [Verfluchte Objekte](#6-verfluchte-objekte)
7. [Wissenschaft und Hunter-Stile](#7-wissenschaft-und-hunter-stile)
8. [Ausrüstung und Bezugsquellen](#8-ausrüstung-und-bezugsquellen)
9. [Entwurf: Ritual- und Artefaktmodell](#9-entwurf-ritual--und-artefaktmodell)
10. [Entwurf: Prüfregeln](#10-entwurf-prüfregeln)
11. [Sonderfälle](#11-sonderfälle)
12. [Was fehlt](#12-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Rituale benötigen echte Zutaten und Vorbereitung; sie dauern Minuten oder länger und können unterbrochen werden; Fehler (Symbol, Zutat, Reihenfolge) haben negative Folgen; manche funktionieren nur an bestimmten Orten; seltene Rituale hängen von Mondphase, Jahreszeit, Uhrzeit ab; Magie kostet Ressourcen, Risiko, Konsequenzen | `23#1074A` Punkte 99–105; „99-105 ja“ in `23#1075E` | 📋 |
| Kein generisches Mana; der Spieler wird kein Fireball-Magier; ein Hunter bleibt ein Hunter | `23#1074A` Punkte 100, 106; „106 ja hunter ist hunter, aber kann kurzzeitig bessessen sein, aber das haben wir schon aufgestellt“ in `23#1075E` | 📋 |
| Rituale selbst vorbereiten: Zutaten und Gegenstände finden, Schutz vorbereiten, richtige Schritte ausführen, nicht nur eine Cutscene | `23#359A` Frage C; „C: ja sicher, und die sachen muss man davor finden, die sind aber nicht leicht zu finden also muss man gut suchen“ in `23#360E` | 📋 |
| Ritualvorbereitung ist Gameplay: Recherche, Material beschaffen, Ort vorbereiten, Schutz, Durchführung; Störungen (Angriff, Stromausfall, verletzter Teilnehmer, beschädigtes Symbol) erzwingen Improvisation | `23#359A` Punkt 17, „17. ja gut“ in `23#360E` | 📋 |
| Rituale sind kein Skyrim-Zaubermenü; keine Quick-Time-Rituale als Standard | `23#359A` Punkte 16, 18; „16. ja gut“, „18. ja passt“ in `23#360E` | 📋 |
| Schutzsymbole, Barrieren, Linien, geweihte oder rituell behandelte Gegenstände, spezielle Fallen, Schutzräume: nicht alles schützt gegen alles; Barrieren sind physisch zerstörbar | `23#359A` Punkte 25–26; „25. ja“, „26. das ist gut“ in `23#360E` | 📋 |
| Artefakte sind persistent und individuell (Ursprung, Geschichte, physische Eigenschaften, Regeln, Aktivierung, Kosten/Risiko, bekannte und unbekannte Eigenschaften) | `23#359A` Punkt 19, „19. klar“ in `23#360E` | 📋 |
| Artefakte können falsch verstanden werden; ihr Preis kann im Archiv fehlen | `23#359A` Punkt 20, „20. gut“ in `23#360E` | 📋 |
| Wissenschaft und Übernatürliches treffen sich: Hunter messen Temperatur, EM-Felder, Audio, chemische Rückstände, Blut, Gewebe; das erklärt nicht alles | `23#359A` Punkt 23, „23. super“ in `23#360E` | 📋 |
| Forschung kann altes Wissen verbessern (Material testen statt Glauben) | `23#359A` Punkt 24, „24. das ist cool“ in `23#360E` | 📋 |
| Moderner Hunter mit Labor und Sensoren neben klassischem Hunter mit alten Büchern: „D: ja kann man einbauen“ | `23#359A` Frage D; `23#360E` | 📋 |
| Verfluchte Gegenstände sind persistente Weltobjekte; sie wechseln Besitzer (verkauft, versteigert, gestohlen, vererbt, gefunden, weggeworfen); der Fluch reist mit und kann einen neuen Fall an anderem Ort erzeugen; Antiquitätenmärkte und Auktionen sind relevant; der Spieler kann unwissentlich ein verfluchtes Objekt kaufen; das eigene Haus oder HQ kann betroffen werden; manche Artefakte werden gelagert statt zerstört; das HQ braucht einen sicheren Artefakt-/Beweisbereich | `23#1074A` Punkte 107–114; „107-114 ja, aber würde ich nicht so häufig machen“ in `23#1075E` | 📋 |
| Mehrere Methoden, Vorbereitung belohnt, Improvisation möglich (Fallen, Schutzkreise, Salz, Spezialmunition, Rituale) | „92 ja“, „93 ja“, „94-98 ja“ in `23#1075E` | 📋 |
| Falsche Lore kann zum Wiederkehren eines ‚getöteten‘ Wesens führen; Überreste müssen bei bestimmten Arten speziell behandelt werden | Punkte 95–96; „94-98 ja“ | 📋 |

---

## 2. Der Jagdprozess

Die Quellen beschreiben das Jagen nicht als eigenen Spielmodus, sondern als **Kette** aus Ermittlung und Vorbereitung:

```text
Vorfall → Ermittlung → Hypothese → Recherche → Beschaffung → Vorbereitung
        → Konfrontation → Nachbereitung (Weltzustand)
```

| Schritt | Quelle |
|---|---|
| Vorfall und Ermittlung | [Ermittlung 09](../Ermittlung/09-Falllebenszyklus-und-Generierung.md); `23#1074A` Punkte 62–83 |
| Hypothese, Identifikation, Fehler | `23#293A` Punkte 8–10, „8. super“, „9. ja genau“, „10. perfekt“ in `23#294E` |
| Recherche (nötige Methode kennen) | „91 korrekt, da muss man davor research machen oder es selbst testen“ in `23#1075E` |
| Beschaffung | Hunter-Shops, normale Läden, Netzwerk (Modul 11) |
| Vorbereitung | Fallen, Schutzkreise, Salz, Spezialmunition, Rituale |
| Konfrontation | Kampf, Stealth, Gefangennahme, Verhandlung ([Modul 13](13-Lebendfang-Fallen-und-Containment.md)) |
| Nachbereitung | Archiv, Weltzustand, Folgen (Modul 15) |

Eine Konfrontation ist nur einer der möglichen Abschlüsse (L8, L9). Ein Fall kann enden mit töten, vertreiben, bannen, fangen, Fluch brechen, Person retten, Artefakt zerstören oder sichern, verhandeln, überleben, evakuieren, austricksen (`M-PD` ‚Story and cases‘).

---

## 3. Rituale

### 3.1 Bestandteile (Quelle `23#359A` Punkt 16, 💡 Zusammenstellung)

Ein Ritual kann benötigen: Ort, Materialien, Symbole, Reihenfolge, Zeit, Teilnehmer, gesprochenen Text, Blut oder biologische Probe, Artefakt, Zielobjekt. Fehler haben unterschiedliche Folgen.

### 3.2 Vorbereitung als Gameplay

`23#359A` Punkt 17: Recherche → Materialien beschaffen → Ort vorbereiten → Schutzmaßnahmen → Ritual durchführen. Währenddessen kann ein Monster angreifen, der Strom ausfallen, ein Teilnehmer verletzt werden, ein Symbol beschädigt werden; dann muss man improvisieren (`23#359A` Punkt 17, „17. ja gut“).

### 3.3 Statt QTE

Nicht ‚Press X, Y, B innerhalb 3 Sekunden‘, sondern Vorbereitung, Positionierung, Wissen, korrekte Gegenstände, Schutz und Zeitmanagement; QTE-artige Eingaben höchstens selten für physische Aktionen (`23#359A` Punkt 18, „18. ja passt“ in `23#360E`).

### 3.4 Kosten und Risiko

Rituale kosten nicht zwingend Lebenspunkte, aber **Ressourcen, Risiko, Konsequenzen** (`23#1074A` Punkt 105). Seltene Ritualkomponenten sind **nicht per Grinding** zu beschaffen: „ja das soll im ganzen spiel nicht so sein“ (`23#466E` Punkt 13, Antwort auf `23#465A` Punkt 13). Stattdessen: wissen, wo sie sind, Zugang bekommen, Gefahr überwinden, verhandeln, Alternative finden (`23#458A` Punkt 11).

### 3.5 Orte und Bedingungen

Rituale funktionieren besser oder nur an bestimmten Orten; bei seltenen Ritualen sind Mondphase, Jahreszeit und Uhrzeit relevant (Punkte 103–104, „99-105 ja“). Das Modell entspricht den Aktivitätsbedingungen der Wesen ([Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §8).

### 3.6 Rituale für Reisen und Verträge

Reisen in andere Ebenen brauchen Rituale oder Artefakte ([Modul 07](07-Kosmologie-Seelen-und-Reiche.md) §5); Verträge sind regelbasierte Abmachungen ([Modul 05](05-Geister-Besessenheit-und-Fluecher.md) §5).

### 3.7 Temporäre Kräfte

Der Hunter kann vorübergehend übernatürliche Möglichkeiten haben (Artefakt, Ritual, Fluch, Besessenheit, Vertrag, Story), mit Konsequenzen, Risiken und Regeln; nicht als dauerhafte Fähigkeiten im Skilltree (`23#359A` Punkt 29, „29. gut“ in `23#360E`).

---

## 4. Schutzmaßnahmen und Barrieren

| Aussage | Quelle | Status |
|---|---|:---:|
| Echte defensive Systeme: Schutzsymbole, Barrieren, Linien, geweihte oder rituell behandelte Gegenstände, spezielle Fallen, Schutzräume | `23#359A` Punkt 25 | 📋 |
| Nicht alles schützt gegen alles | `23#359A` Punkt 25 | 📋 |
| Ein Schutzkreis ist keine abstrakte Gameplay-Wand; ist ein Teil physisch zerstört, ist der Schutz beeinträchtigt | `23#359A` Punkt 26, „26. das ist gut“ in `23#360E` | 📋 |
| Anti-Besessenheit-Schutz ist nie absolut; mächtige Wesen können ihn umgehen oder beschädigen; Fairness über Lore und Ermittlung | `23#361A` Punkt 20 (💡) | 💡 |
| Das Zeichnen von Schutzsymbolen ist **vorgefertigt** (Vorlagen, Größen, Material, Ausrichtung), freihändiges Zeichnen ist keine Pflicht | „Supernatural Fallenbau und Platzierung: theoretisch beliebig aber das wird halt sehr schwer zum zeichnen also würde ich eher das vorgefertigt haben mit verschiedenen größrn und so“ in `48#135E`; Auslegung `48#136A` | 📋 |
| Schutzmaßnahmen sind **persistent** und können durch Wetter, Abnutzung, NPCs, Monster, Zeit verändert werden | „Supernatural Fallen-Persistenz: Realistische Persistenz mit Umwelt- und NPC-Einflüssen“ in `48#133E`; Tabelle `48#134A` | 📋 |

Die vollständige Behandlung von Fallen und Verwahrung steht in [Modul 13](13-Lebendfang-Fallen-und-Containment.md).

---

## 5. Artefakte

### 5.1 Eigenschaften (Quelle `23#359A` Punkt 19)

```text
Artifact
├── Origin, History
├── Physical properties
├── Supernatural rules
├── Activation
├── Cost / Risk
├── Known limitations
└── Unknown properties
```

Ein mächtiges Artefakt kann gefährlicher sein als hilfreich. Artefakte sind persistent und individuell, **keine** Gegenstände wie ‚Magic Sword +12‘ (`23#359A` Punkt 19).

### 5.2 Wissen über Artefakte

Das Archiv kann falsch liegen: ‚Dieses Objekt schützt vor Wesen X.‘ Vielleicht nur unter Bedingungen, oder der Effekt hat einen Preis, den das alte Manuskript verschweigt (`23#359A` Punkt 20). Das epistemische System gilt (Modul 14).

### 5.3 Original-Waffen der Hunter

Das Arsenal enthält gewöhnliche Waffen plus **originale** seltene oder legendäre Jägerwaffen mit IP-sicherer eigener Lore (`M-PD` ‚Gunplay / weapons‘). Serienspezifische Artefakte werden nicht übernommen (`23#376A`, `23#378A`; [Modul 01 §4](01-Quellenlage-und-Methodik.md#4-zwei-regeln-des-entwicklers-zum-umgang-mit-der-serie)).

---

## 6. Verfluchte Objekte

### 6.1 Was die Quellen sagen

`23#1076A` fasst zusammen (💡 Ausformulierung der 📋 Punkte 107–114): Verfluchte Objekte sind **selten genug, dass sie besonders bleiben**. Sie können verkauft, gestohlen, vererbt, versteigert, weggeworfen und gefunden werden. Dadurch wandert ein Fluch durch das **normale Wirtschaftssystem**. Der Spieler kann bei einer Estate Sale oder Auktion unwissentlich so etwas kaufen und ins eigene Haus bringen. Das HQ bekommt später einen stark gesicherten **Artifact-/Evidence-Vault**.

Der Entwickler: „107-114 ja, aber würde ich nicht so häufig machen“ (`23#1075E`).

### 6.2 Verbindungen

| Verbindung | Wirkung |
|---|---|
| Wirtschaft | Auktionen, Antiquitätenmärkte, Flohmärkte und Estate Sales als Fundorte (`M-CA0905` §4: ‚flea/yard/estate sales and auctions‘) |
| Besitzerwechsel | Der Fluch reist und erzeugt einen Fall dort, wo das Objekt landet |
| Haus/HQ | Mitnehmen kann das eigene Haus betreffen |
| Zerstörung | Manche Artefakte sind zu gefährlich oder nicht vernichtbar und werden **gesichert** |
| Archiv | Besitzgeschichte (Provenienz) ist Ermittlung ([Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md)) |

### 6.3 Haunted Properties

Gebäude mit übernatürlicher Geschichte: [Modul 16](16-Uebernatuerliche-Orte-und-Geografie.md).

---

## 7. Wissenschaft und Hunter-Stile

`23#359A` Punkte 23–24 und Frage D (Antwort: „D: ja kann man einbauen“, `23#360E`): Ein sehr moderner Hunter kann Laboranalysen, Kameras, Sensoren verwenden, ein klassischer Hunter alte Bücher und Erfahrung. Das könne unterschiedliche Hunter-Stile ergeben (💡). Das Labor und die Forensik der Kreaturen sind in [Modul 14](14-Forschung-und-Bestiarium.md) und [Ermittlung 11](../Ermittlung/11-Forensik-und-Labor.md) behandelt.

---

## 8. Ausrüstung und Bezugsquellen

Hunter-Material stammt aus Spezialhändlern und normalen Läden; es gibt Hunter-Depots und HQ-Vorräte (Modul 11). Die Frage, ob Silber, Salz, Eisen und andere Materialien im Spiel eine Wirkung haben, hängt an der **Artregel** ([Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §7); sie gelten nicht pauschal.

Bei **Geistern** wurde die Serienlogik von Salz und Eisen als Standard gesetzt ([Modul 05](05-Geister-Besessenheit-und-Fluecher.md) §2); eigene Ausgestaltung statt direkter Übernahme gilt bei Kreaturen und Designs.

---

## 9. Entwurf: Ritual- und Artefaktmodell

> 💡 **Vorschlag.** Er macht die beschlossenen Rituale und Artefakte als Daten prüfbar.

```text
RitualDefinition
├── ritual_id, ip_provenance, purpose
├── requirements    {place_rules, materials[], symbols[], order[], time_conditions,
│                    participants, spoken_text_ref, sample, artifact_ref, target_ref}
├── preparation_steps[]   (jeder Schritt hat Orte, Dauer, Störbarkeit)
├── failure_modes[]       {cause, consequence_ref}
├── cost                  {resources, risk, consequences}
├── knowledge_sources[]   Quellen im Archiv (kann falsch sein)
└── realm_effects         optional (Reise, Bindung, Bannung)

RitualInstance (Weltzustand)
├── started_at, place_id, participants[]
├── progress_state        prepared | in_progress | interrupted | completed | failed
└── environment_damage[]  beschädigte Symbole, Linien

ArtifactState
├── artifact_id, origin, history[]
├── known_properties, unknown_properties
├── owner_chain[]         Besitzerwechsel (Verkauf, Erbe, Diebstahl ...)
└── containment_state     free | stored(vault) | cursed_bound | destroyed
```

**Quellenbezug:** RitualDefinition folgt `23#359A` Punkte 16–18; die Persistenz des Ritualzustands und der Umgebungsschäden folgt der Regel ‚Schutzkreis ist keine abstrakte Wand‘ (Punkt 26); Besitzerkette folgt `23#1074A` Punkt 108.

---

## 10. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Ein Ritual verbraucht die geforderten Materialien und scheitert bei fehlendem Pflichtteil | `23#359A` Punkt 16 |
| Eine Störung im Ritual (zerstörtes Symbol) ändert den Ausgang | `23#359A` Punkte 17, 26 |
| Es gibt keinen Magiebalken in UI und Daten | `23#359A` Punkt 1; `23#1074A` Punkt 100 |
| Seltene Zutaten sind über einen definierten Weg erreichbar (kein Zufallsdrop-Farmen) | `23#466E` Punkt 13; `23#458A` Punkt 11 |
| Ein verfluchtes Objekt hat eine lückenlose Besitzerkette | `23#1074A` Punkt 108 |
| Nach Weitergabe erzeugt der Fluch am neuen Ort einen Vorfall, am alten nicht mehr | Punkt 109 |
| Das Archiv trägt bei einem Artefakt unbekannte Eigenschaften als ‚unbekannt‘ | `23#359A` Punkt 19 |
| Eine Salzlinie schützt nur, solange sie physisch intakt ist | `23#370A` Punkt 4 |

---

## 11. Sonderfälle

| Fall | Behandlung |
|---|---|
| Der Spieler kauft unwissentlich ein verfluchtes Objekt | gewollt (Punkt 111); Ermittlung nach Eintritt der Folgen |
| Ein Artefakt kann nicht zerstört werden | wird im Vault gelagert (Punkt 113) |
| Ein Ritual verlangt Teilnehmer, die der Spieler nicht hat | Vorbereitung ergibt Ermittlung und Beschaffung |
| Ein Ritual wird in einer anderen Ebene durchgeführt | ‚Manche Rituale funktionieren dort nicht‘ (`23#458A` Punkt 27) |
| Mehrere Hunter bereiten gemeinsam ein Ritual vor (Koop) | teilen Zustand (Weltzustand des Hosts) |

---

## 12. Was fehlt

- Konkrete Rituale, Materialien, Symbole (❓, Artendefinition).
- Das Kosten-/Risikomodell im Detail (❓ Q-K17).
- Die Regeln für Exorzismus und Bannung (❓ Q-K06).
- Wie viele verfluchte Objekte es im Spiel gibt („nicht so häufig“, ❓).
- Die Gestaltung des Artifact-/Evidence-Vaults im HQ (HQ-Planung steht aus, `48#131E`).
