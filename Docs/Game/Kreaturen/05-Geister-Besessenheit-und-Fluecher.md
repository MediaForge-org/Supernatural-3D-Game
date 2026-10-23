# Kreaturen 05: Geister, Besessenheit und Flüche

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt die körperlosen und gebundenen Phänomene: Geister, Bindungen, Besessenheit, Flüche und Verträge. Der Entwickler hat angeordnet, dass bei **nicht festgelegten** übernatürlichen Regeln die Serienlogik als Standard gilt (`23#362E`, `23#371E`). Die Inhalte aus dieser Serienlogik hat der **Assistent** wiedergegeben; sie sind hier als 💡 geführt, die Standardregel selbst als 📋. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md), Umgang mit der Serie: [Modul 01 §4](01-Quellenlage-und-Methodik.md#4-zwei-regeln-des-entwicklers-zum-umgang-mit-der-serie).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Geister](#2-geister)
3. [Besessenheit](#3-besessenheit)
4. [Flüche](#4-flüche)
5. [Verträge](#5-verträge)
6. [Entwurf: Zustandsmodelle](#6-entwurf-zustandsmodelle)
7. [Entwurf: Prüfregeln](#7-entwurf-prüfregeln)
8. [Fallbeispiele (Entwurf)](#8-fallbeispiele-entwurf)
9. [Sonderfälle](#9-sonderfälle)
10. [Was fehlt](#10-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Nicht jedes Übernatürliche folgt denselben Gesetzen; Geister, Flüche, Besessenheit, dämonische Wesen und Artefakte haben **eigene** Regeln, auch überlappend | `23#359A` Punkt 1, „1. gut“ in `23#360E` | 📋 |
| Geisterartige Wesen: mehrere Unterformen; ‚Es sieht nach einem Geist aus‘ ist zunächst eine **Hypothese** | `23#359A` Punkt 7, „7. genau“ in `23#360E` | 📋 |
| Geistzustände wechseln: nicht-körperlich, sichtbare Manifestation, teilweise physische Interaktion, volle erzwungene Manifestation; je Zustand wirken andere Methoden | `23#359A` Punkt 8, „8. ja passt“ in `23#360E` | 📋 |
| Bei **gebundenen** Entitäten ist die Bindung wichtiger als der Schaden; die Lösung kann sein, die Bindung zu zerstören oder zu lösen | `23#359A` Punkt 9, „9. genau“ in `23#360E` | 📋 |
| Flüche sind ein **eigener Systemtyp**, kein ‚Status Effect Curse‘ | `23#359A` Punkt 10, „10. perfekt“ in `23#360E` | 📋 |
| Flüche reagieren auf Bedingungen (Blutlinie, Ort, Handlung, Schuld, Besitz, Ritual, Uhrzeit, Mondphase); der Fall fragt dann: Was aktiviert den Fluch? | `23#359A` Punkt 11, „11. interressant“ in `23#360E` | 📋 |
| Besessenheit trennt **Wirtskörper und Entität** strikt | `23#359A` Punkt 12, „12. perfekt“ in `23#360E`; `M-PD` | 📋 |
| Besessenheit kann graduell sein: Einfluss, Teilkontrolle, zeitweilige Besessenheit, fast vollständige, volle | `23#359A` Punkt 13, „13. gut“ in `23#360E` | 📋 |
| Dämonische Wesen: eigene Kategorie intelligenter Entitäten, die langfristig planen, manipulieren, Verträge anbieten; Orientierung an der Serie | `23#359A` Punkt 14, „14. ja, da werden wir uns an der serie orientieren“ in `23#360E` | 📋 |
| Verträge sind **regelbasiert**, keine Autorenmagie | `23#359A` Punkt 15, „15. super idee“ in `23#360E` | 📋 |
| Die meisten Menschen werden nach dem Tod **keine Geister**; nur besondere Umstände lassen etwas zurück | „Nicht jeder Tote wird zum Geist“ (`23#458A` Punkt 2); `23#361A` Punkt 13 | 📋 / 💡 |
| Besessenheit hat echte Folgen (Erinnerungslücken, Verletzungen, Straftaten, Beziehungen); sie kann auch Spieler und NPCs treffen, „nicht oft, aber schon“ | „142 ja sicher, nicht oft aber schon“, „143 ja genau“ in `23#1075E` | 📋 |
| Flüche auf dem Spieler können länger anhalten, bis eine Lösung gefunden ist; Infektion/Transformation sind bei manchen Arten möglich; kein Game Over, nur Tod | „144-146 genau, aber game over soll es nicht geben, nur den tod“ in `23#1075E` | 📋 |
| Ein Host darf zumindest Fragmente dessen erinnern, was die Entität getan hat, wenn komplett besessen | „C: ja teilweise schon, wenn man komplett besessen ist“ in `23#362E` (zu `23#361A` Frage C) | 📋 |
| Der Spieler kann kurz besessen sein; ein Abschnitt, in dem der Hauptjäger besessen ist und der Begleiter ihn retten muss, ist erwünscht | „28. ja er bleibt ein mensch aber es kann sein dass man in der story oder in missionen besessen von einem dämon wird und man dann als der partner einen retten muss“ in `23#360E` | 📋 |

---

## 2. Geister

### 2.1 Entstehung

Der Entwickler wollte die Geister „deutlich wie in Supernatural“ (Anordnung `23#369A`, bestätigt durch `23#371E`). Die folgende Zusammenstellung gibt wieder, was der Assistent daraus formulierte (`23#370A`) und in `23#361A` Punkte 13–15 ergänzte (💡):

| Frage | Inhalt |
|---|---|
| Normalfall | Mensch stirbt, die Seele geht weiter, kein Geist |
| Warum bleibt jemand? | unerledigte Angelegenheit, Rache, Liebe, Hass, Schuld, Weigerung weiterzugehen, starke Bindung, Bindung an Überreste oder Objekt, übernatürliche Manipulation oder Beschwörung |
| Ist der Geist die ganze Person? | nicht zwingend: Seele, psychische Überreste, wiederholende Fragmente, durch Trauma erzeugte Echo-Strukturen oder eine Entität, die Erinnerungen übernommen hat (`23#361A` Punkt 14) |
| Wer begleitet beim Weitergehen? | Es gibt Wesen oder Mechanismen, die den Verstorbenen begleiten; eine Seele kann sich weigern. Für die eigene IP: eigene Bezeichnung, Erscheinung, Hierarchie, Lore; die Funktion ist brauchbar (`23#370A` Punkt 12) |

Der Entwickler wählte keinen dieser Punkte einzeln; sie stehen unter der Standardregel ‚Serienlogik, solange nichts anderes gesagt ist‘ (`23#372A`).

### 2.2 Anker und Bindung

- Ein Geist kann an **Dinge** gebunden sein: Knochen, Haar, Blut, Kleidung, Schmuck, Mordwaffe, persönlicher Gegenstand (`23#370A` Punkt 3). ‚Grab verbrennen, fertig‘ reicht nicht immer.
- Das Spiel soll daraus **Ermittlung** machen: Identität des Toten, Überreste, Anker (Beispiel: Überreste bereits kremiert, erste Theorie falsch, alter Ehering existiert noch, Ring ist Anker). Der Detective Mode setzt **keinen Marker** auf den Ring (`23#370A` Punkt 14).
- Allgemein: Ist ein Wesen an etwas gebunden, kann man es wiederholt vertreiben; solange das Bindungsobjekt existiert, kehrt es zurück (`23#359A` Punkt 9, 📋).

### 2.3 Mittel und Lösungen

| Mittel/Lösung | Wirkung nach der Wiedergabe in `23#370A` | Status |
|---|---|:---:|
| Salz | Barriere, Schutzkreis, temporärer Schutz; die Linie muss **physisch intakt** sein (Wind, Wasser, Störung unterbrechen sie) | 💡 (Serienlogik) |
| Eisen | stört oder wehrt ab, destabilisiert; tötet nicht endgültig; ‚Zeit kaufen ≠ Fall lösen‘ | 💡 (Serienlogik) |
| Überreste vernichten | klassischer Weg: Identität des Toten feststellen, Grab/Überreste finden, richtige Behandlung | 💡 (Serienlogik) |
| Unerledigtes lösen | Wahrheit erfahren, Täter konfrontieren, abschließen, Person retten, verstehen, dass man tot ist, freiwillig loslassen | 💡 (Serienlogik) |
| Anker zerstören | Bindung lösen | 📋 als Prinzip (`23#359A` Punkt 9), 💡 als Beispiel |

Die Salzlinie als Barriere deckt sich mit der Grundregel, dass Schutzkreise **zerstörbar** sind: eine unterbrochene Linie bedeutet beeinträchtigten Schutz (`23#359A` Punkt 26, „26. das ist gut“ in `23#360E`; Fallen: [Modul 13](13-Lebendfang-Fallen-und-Containment.md)). Ein Schutz schützt **nicht gegen alles** (Punkt 25).

### 2.4 Veränderung über Zeit

Ein Geist, der sehr lange zurückbleibt, kann Erinnerungen verlieren, obsessive Muster entwickeln, aggressiver werden, die Realität schlechter verstehen, Ereignisse wiederholen und irgendwann stark gefährlich werden (`23#370A` Punkt 8, 💡). **Aber** nicht nach einer festen Anzahl Tage: Das bleibt individuell.

### 2.5 Geistzustände und Bewusstsein

Zwei getrennte Skalen:

| Skala | Werte | Quelle |
|---|---|---|
| Körperlichkeit | nicht-körperlich → sichtbare Manifestation → teilweise physische Interaktion → volle erzwungene Manifestation (bzw. gebundene Manifestation) | `23#359A` Punkt 8; `23#307A` Punkt 2 |
| Bewusstsein | Rest-Echo (wiederholt Ereignis, kaum Bewusstsein) → verwirrter Geist → bewusster Geist → feindlicher gebundener Geist | `23#361A` Punkt 15 (💡 interne Klassifikation, keine sichtbaren Kategorien) |

`23#370A` Punkt 9 nennt Typen, die sich ‚organisch aus Geschichte und Zustand‘ ergeben: Echo-artig, bewusst, rachsüchtig, stark degeneriert/gewalttätig, gebunden. Es gibt **kein Arcade-Menü** ‚Ghost Tier 1/2/3‘.

### 2.6 Fähigkeiten

Nicht jeder Geist hat jede Fähigkeit. Mögliche: Temperaturabfall, Elektronik stören, Telekinese, erscheinen und verschwinden, Menschen angreifen, teilweise Besitz ergreifen (`23#370A` Punkt 10, 💡).

### 2.7 Geister-Besessenheit gegen dämonische Besessenheit

Beide können äußerlich ähnlich aussehen, unterscheiden sich in Ursache, Schwächen, Austreibung und Auswirkung; das macht Ermittlung relevant (`23#370A` Punkt 11, 💡).

---

## 3. Besessenheit

### 3.1 Wirtskörper und Entität

`23#359A` Punkt 12 (📋): Verletzungen am Körper bleiben echte Verletzungen. Der Host kann noch leben, die Entität lässt sich eventuell vertreiben, der Host kann Erinnerungen besitzen oder verlieren; Kontrollgrade unterscheiden sich. **Nicht** jeder Fall bedeutet volle Kontrolle.

### 3.2 Grade der Kontrolle

`23#359A` Punkt 13 (📋 Gedanke) und `23#361A` Punkt 16 (💡 Modelle):

```text
Influence → Partial Control → Intermittent Possession → Near Total Control → Full Possession
```

Daneben vier Modelle des Verhältnisses von Wirt und Entität: Unterdrückung (Mensch noch da, ohne Kontrolle), Verdrängung (Bewusstsein/Seele teilweise zurückgedrängt), Teilbesitz (beide beeinflussen den Körper), vollständige Kontrolle. Ein Dämon löscht die Seele **nicht einfach** (`23#361A` Punkt 16).

Mögliche Fälle bei geringem Grad: der Betroffene ist zeitweise normal, sucht Hilfe, sperrt sich selbst ein, hat Erinnerungslücken (`23#359A` Punkt 13).

### 3.3 Der Host kämpft von innen

Ein starker Host kann manchmal kurz Kontrolle zurückgewinnen, Bewegung sabotieren, sprechen, einen Hinweis geben, einen Angriff verhindern; nicht als Minigame, sondern charakterabhängig (`23#361A` Punkt 17, 💡 Vorschlag). Gegenwert der Hunter-Seite: Schutzsymbole, Artefakte, Rituale, Markierungen, Materialien, aber ‚kein Schutz ist zwangsläufig absolut‘ (`23#361A` Punkt 20, 💡).

### 3.4 Erinnerung nach der Besessenheit

Der Entwickler: „C: ja teilweise schon, wenn man komplett besessen ist“ (`23#362E`), also mindestens **Fragmente** bei voller Besessenheit; die Stärke hängt von Dauer und Grad ab (Frage C in `23#361A`: ‚abhängig von Stärke/Dauer der Besessenheit‘).

### 3.5 Besessenheit als Gameplay

- **Spieler besessen:** Hauptjäger A wird besessen, der Spieler übernimmt Jäger B und muss A finden und retten. Der Spieler verliert nicht einfach die Kontrolle für eine Cutscene, er erlebt die Situation aus mehreren Perspektiven; später eventuell kurze Innenperspektive (`23#361A` Punkte 18–19, 💡; Wunsch des Entwicklers in `23#360E` Punkt 28, 📋).
- **Besessene NPCs:** sind Fälle für Ermittlung (Erkennen, Austreibung, Wirtskörper retten) und haben Folgen für Straftaten und Beziehungen (`23#1074A` Punkt 143, „143 ja genau“ in `23#1075E`).
- **Rechtliche Folgen:** Straftaten unter Besessenheit sind ein Thema für Gerichte und Polizei ([Ermittlung 15](../Ermittlung/15-Gerichte-und-Verfahren.md) §Übernatürliches vor Gericht).
- **Häufigkeit:** „nicht oft“ (`23#1075E` Punkt 142).

---

## 4. Flüche

### 4.1 Aufbau eines Fluchs

`23#359A` Punkt 10 (📋 Gedanke) nennt Bestandteile: Ursprung, Ziel, Auslöser, Regeln, Symptome, Eskalation, Übertragung, Fluchtbedingungen, Misserfolgsbedingungen. Das Beispiel der Quelle (💡): Tag 1 Schlafprobleme, Tag 2 Halluzinationen, Tag 4 körperliche Symptome, Tag 7 tödliches Ereignis; ein anderer Fluch funktioniert völlig anders.

### 4.2 Auslöser

Blutlinie, Ort, Handlung, Schuld, Besitz eines Gegenstands, gesprochenes Ritual, bestimmte Uhrzeit, Mondphase (`23#359A` Punkt 11). Der Fall wird dadurch zu der Frage, **was den Fluch aktiviert**.

### 4.3 Flüche auf Menschen und Spieler

- Fluch auf dem Spieler: länger anhaltend, bis eine Lösung gefunden ist (`23#1074A` Punkt 144, „144-146 genau…“ in `23#1075E`).
- Verfluchte Objekte reisen durch die Wirtschaft (Modul 12): verkauft, versteigert, gestohlen, vererbt, gefunden, weggeworfen.
- Behandlung: Der Spieler muss die richtige Behandlung gegebenenfalls selbst herausfinden (`48#124A`, siehe [Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) §10).
- Heilung von Flüchen durch übernatürliche Heiler ist **nicht selbstverständlich** (‚Das kann ich nicht heilen‘, `35#27A`, 💡).

### 4.4 Epistemische Seite

Ein Fluch ist eine Hypothese, bis Symptome, Auslöser und Herkunft belegt sind. Falsche Überlieferungen (Modul 14) sind hier besonders plausibel.

---

## 5. Verträge

`23#359A` Punkt 15 („15. super idee“ in `23#360E`, 📋): Verträge sind **regelbasiert**. Bestandteile (💡): Parteien, Angebot, Preis, Bedingungen, Dauer, Schlupflöcher, Durchsetzung, Beendigung. Der Spieler kann den Wortlaut tatsächlich verstehen versuchen; keine beliebige Autorenmagie.

Kontext: Dämonen können Verträge anbieten (`23#359A` Punkt 14). Dazu `23#458A` Punkt 20 (💡): ein intelligenter Dämon wird nicht sofort angreifen, wenn er etwas davon hat, den Spieler am Leben zu lassen (‚Warum sollte ich dich töten, wenn du mir nützlich sein kannst?‘). Das Verhältnis des Spielers zu Verträgen (darf der Spieler einen schließen?) ist nicht entschieden (❓ Q-K05).

---

## 6. Entwurf: Zustandsmodelle

> 💡 **Vorschlag.** Er macht die beschlossenen Unterscheidungen prüfbar; Namen und Übergänge sind unverbindlich.

### 6.1 Geist

```text
GhostState
├── origin            death_circumstance | summoned | manipulated | unknown
├── anchors[]         {kind: remains|object|place|person|bloodline|ritual, status}
├── corporeality      non_corporeal | visible | partial_physical | forced_manifest
├── awareness         echo | confused | aware | hostile_bound
├── degeneration      Rate pro Individuum (nicht global)
├── abilities[]       Teilmenge
├── unfinished_business{description, resolvable_by[]}
└── status            haunting | dormant | released | destroyed | unknown

Übergänge (Beispiele)
haunting --Anker zerstört--> released oder destroyed (je Art)
haunting --unerledigtes gelöst--> released
haunting --vertrieben ohne Anker zu lösen--> dormant (kehrt zurück)
```

### 6.2 Besessenheit

```text
PossessionState
├── host_id, entity_id
├── control_level     influence | partial | intermittent | near_total | full
├── suppression_model suppress | displace | share | full
├── host_resistance   situativ (charakterabhängig)
├── memory_fragments  [ {event_id, retained: bool} ]
├── protections[]     betroffene Schutzmaßnahmen
└── history           Zeitreihe, damit Folgen (Straftaten, Verletzungen) zuordenbar bleiben
```

### 6.3 Fluch

```text
CurseState
├── origin, target, trigger{kind, conditions}
├── rules[], symptoms[], escalation_schedule[]
├── transmission, escape_conditions[], failure_conditions[]
└── status            dormant | active | escalating | broken | fatal | unknown
```

Alle drei Zustände sind **Weltzustand** (Modul 17) und müssen speichern und laden überstehen, damit eine Eskalation weiterläuft.

---

## 7. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Ein Geist mit intaktem Anker kehrt nach Vertreibung zurück | `23#359A` Punkt 9 |
| Das Zerstören des Ankers beendet die Bindung, aber nicht automatisch jedes Geisterverhalten, wenn weitere Anker bestehen | `23#370A` Punkt 3 |
| Eine unterbrochene Salzlinie schützt nicht mehr | `23#370A` Punkt 4; `23#359A` Punkt 26 |
| Ein Schuss auf einen Besessenen verletzt den Wirt | `23#359A` Punkt 12 |
| Besessenheit erzeugt Fragmente der Erinnerung nur ab dem eingestellten Grad | `23#362E` Antwort C |
| Ein Fluch eskaliert nach Zeitplan, auch wenn der Spieler das Gebiet verlässt; Zustand wird gespeichert | `23#359A` Punkt 10; `M-PD` ‚World state and persistence‘ |
| Ein Hinweis darauf, dass es ein Geist ist, darf eine Hypothese bleiben, bis Belege vorliegen | `23#359A` Punkt 7 |
| Kein Marker auf Anker oder Auslöser im Detective Mode | `23#370A` Punkt 14 |
| Ein Fluch, der über ein Objekt reist, erzeugt am neuen Ort einen neuen Vorfall, nicht den alten | `23#1074A` Punkt 109, „107-114 ja, aber würde ich nicht so häufig machen“ in `23#1075E` |

---

## 8. Fallbeispiele (Entwurf)

Alle Beispiele sind **Skizzen** (💡) zur Prüfung der Zustandsmodelle, keine geplanten Fälle.

**A. Der Ring (Geist, Anker, Ermittlung).** Familie meldet Geräusche → Haus untersuchen (Temperatur, Messung, Zeugenaussagen) → Geist als Hypothese → Friedhof, Überreste kremiert, erste Theorie falsch → Recherche → alter Ehering existiert noch → Anker. Lösungen: Ring zerstören **oder** unerledigte Angelegenheit lösen (Geist geht freiwillig). Quelle des Ablaufs: `23#370A` Punkt 14.

**B. Falsche Identifikation.** Opfer zeigen tiefe Schnittverletzungen; Geist, Wesen, Mensch und Tarnung kommen infrage. Verletzungsmuster, Zeit, Ort und Zeugenaussage grenzen ein; mit unvollständiger Information bereitet sich der Spieler eventuell auf das Falsche vor (`23#293A` Punkte 8–9).

**C. Teilbesessener Helfer (Entwurf, keine Quelle für den Fall).** Ein NPC ist zeitweise er selbst, sperrt sich aus Angst ein und hat Erinnerungslücken (Eigenschaften nach `23#359A` Punkt 13). Lösungen: Wirt retten (Schutz, Ritual), Entität vertreiben, tödliche Lösung. Die Wahl liegt beim Spieler (`23#293A` Punkt 15).

---

## 9. Sonderfälle

| Fall | Behandlung |
|---|---|
| Geist und Dämon verwechselt | Hypothese; unterschiedliche Austreibung (`23#370A` Punkt 11) |
| Ein Geist, der niemandem schadet | darf gehen; kein Töten nötig (`23#370A` Punkt 7; L9) |
| Ein NPC-Jäger zerstört einen Anker, den der Spieler schonen wollte | unabhängige Handlung anderer Jäger ([Modul 11](11-Hunterwelt-Organisationen-und-Kulte.md)) |
| Haunted Property wird renoviert oder abgerissen | Geist/Fluch reagiert abhängig von der Ursache („115-120 ja sicher“, `23#1075E`; [Modul 16](16-Uebernatuerliche-Orte-und-Geografie.md)) |
| Besessene Figur ist der Spielercharakter und stirbt | Gameplay-Tod ist nicht kanonisch; Story-Tod folgt den Jenseitsregeln ([Modul 07](07-Kosmologie-Seelen-und-Reiche.md)) |

---

## 10. Was fehlt

- Eigene Bezeichnungen für Seelenbegleiter und weitere serienspezifische Konzepte (❓, IP).
- Zahl und Art der Geistertypen im Basisspiel (❓).
- Die Regeln für das Austreiben (Exorzismus): Elemente, Ablauf (❓ Q-K06; siehe [Modul 12](12-Jagd-Rituale-und-verfluchte-Objekte.md)).
- Ob der Spieler Verträge mit Wesen schließen darf (❓ Q-K05).
- Das Verhältnis ‚Seele entfernt, Körper lebt‘ (siehe [Modul 07](07-Kosmologie-Seelen-und-Reiche.md) §6; ❓).
