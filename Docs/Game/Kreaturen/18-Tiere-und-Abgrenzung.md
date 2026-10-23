# Kreaturen 18: Tiere und Abgrenzung

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul grenzt **gewöhnliche Tiere** von **Monstern** ab und sagt, welche Systeme beide teilen. Die Tierwelt selbst (220–280 Arten, Studium, Jagd, Angeln, Kadaver) ist in [Spielwelt und Untergrund §9](../Spielwelt-und-Untergrund.md) dokumentiert und wird hier nicht wiederholt. Abschnitte „Entwurf“ sind unverbindlich (💡). Statuszeichen: [Legende](README.md#statuslegende).

Teil der Kreaturen-Dokumentation ([Index](README.md)). Quellen: [Modul 01](01-Quellenlage-und-Methodik.md). Hauptquellen: `M-PD` ‚Wildlife / study / harvesting / monsters‘, `M-B37`, `23#1026A`/`23#1027E` (Block 6, Natur), `M-CA0905` §4 (Hunde).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Abgrenzung](#2-abgrenzung)
3. [Gemeinsame Systeme](#3-gemeinsame-systeme)
4. [Tiere und das Übernatürliche](#4-tiere-und-das-übernatürliche)
5. [Der Hund als Begleiter](#5-der-hund-als-begleiter)
6. [Entwurf: Prüfregeln](#6-entwurf-prüfregeln)
7. [Sonderfälle](#7-sonderfälle)
8. [Was fehlt](#8-was-fehlt)

---

## 1. Was beschlossen ist

| Aussage | Quelle | Status |
|---|---|:---:|
| Auf der Hauptkarte etwa 220–280 Tier-, Vogel- und Fischarten; mehr auf anderen Karten oder DLC | `M-PD`; `M-B37` | 📋 |
| Das Studium einer Art braucht eine bewusste Studier-Aktion; archivpflichtige gewöhnliche Arten sind nicht dauerhaft verpassbar | `M-PD`; `M-B37` | 📋 |
| Einzeltiere variieren (Alter, Geschlecht, Zustand, Gesundheit, Verletzung, Größe); seltene benannte oder legendenartige Individuen ohne Arcade-Beschriftung | `M-PD`; `M-B37` | 📋 |
| Kadaver zerfallen, locken Aasfresser an, haben forensischen Wert | `M-PD`; `M-B37`; „28. ja sixher“ in `23#1027E` | 📋 |
| Haustiere (Hunde, Katzen), Farmtiere, Pferde (auch außerhalb der Rennbahn), Tierverhalten „wie in echt“, **Babytiere**, Roadkill, Jagd in speziellen Gebieten, tiefes Angeln, Naturfotografie, Park-Gebiete, große Höhlen, Unterwasserwelt | `23#1026A` Punkte 22–34; Antworten „22 … 34“ in `23#1027E` | 📋 |
| **Normale Tiere können auf Übernatürliches reagieren**, ohne ein ständiger Monsterdetektor zu sein („man denkt sixh nicht immer was dabei“) | `23#1026A` Punkt 35; „35. ja aber man denkt sixh nicht immer was dabei“ in `23#1027E` | 📋 |
| Der Hund kann Begleiter bei Fällen sein: „sehr wichtig, hund kann begleiter sein bei fällen“ | „22. sehr wichtig, hund kann begleiter sein bei fällen“ in `23#1027E` | 📋 |
| Hundesystem: Rasse, Persönlichkeit, Training, Gehorsam, Rückruf, Suche, Geruchsverfolgung, Reaktionen, Verletzung, Alterung, Tierarzt; Tracking ist **umweltbezogen**, kein magischer Zielpfeil | `M-CA0905` §4 | 📋 |
| Krankheiten und Parasiten sind als Haken vorgesehen | `M-B37` | 🧭 |
| Das Monsterstudium spiegelt das Tierstudium auf tieferer erkenntnistheoretischer Ebene | `M-PD` | 📋 |

---

## 2. Abgrenzung

| Merkmal | Tier | Monster/Kreatur |
|---|---|---|
| Natur | gewöhnliche Biologie | übernatürlich oder übernatürlich verändert |
| Studium | bewusste Studier-Aktion; vollständige Dokumentation möglich | Hypothese, Wahrscheinlichkeit, Bestätigung, unvollständig, möglicherweise falsch ([Modul 14](14-Forschung-und-Bestiarium.md)) |
| Archiv | archivpflichtige gewöhnliche Arten nicht verpassbar | Kreaturen-Wissen kann dauerhaft Lücken haben |
| Zahl | 220–280 | etwa 20–30 tiefe Kernarten bevorzugt, nicht gesperrt ([Modul 02](02-Leitsaetze-und-Abgrenzung.md) §3) |
| Individuen | Variation, seltene benannte Einzeltiere | Individuen mit persistenter Identität, Gedächtnis, Gesellschaft |
| Population | Ökologie wie in der Natur | Ökologie plus Territorien und Machtkämpfe |
| Respawn | Populationsdynamik (Details offen, `M-OD`) | kein Respawn; tot ist tot |
| Intelligenz | tierisch | mehrdimensional |

Die Linie ‚Tier oder Monster‘ ist im Einzelfall **Artdefinition**. Die Quellen kennen ‚tierische Monster‘ (zum Beispiel intelligenzarme Raubwesen) und ‚intelligente Monster‘ (`23#293A`, ‚intelligente vs. tierische Monster‘ in `23#297A`), ohne diese in eine feste Klassifikation zu zwingen.

---

## 3. Gemeinsame Systeme

| System | Tier | Monster | Anmerkung |
|---|---|---|---|
| Anatomie und Verletzung | tiereigene Anatomie, funktionale Verletzung | modulare Kreaturenanatomie | gemeinsames Verletzungsmodell, [Modul 04](04-Anatomie-Verletzung-Schwaechen-und-Regeneration.md) |
| Spuren und Tracking | Fährten, Kot, Haare, Geruch | artspezifische Spuren | [Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md) |
| Wahrnehmung | tierische Sinne | artspezifisch | [Modul 10](10-Monster-KI-Wahrnehmung-und-Gedaechtnis.md) |
| Kadaver und Forensik | Zerfall, Aasfresser, forensischer Wert | Überreste mit Artregeln | `M-PD` |
| Aktivitätsbedingungen | Tageszeit, Jahreszeit, Wetter | zusätzlich Mond, Ort, Sonderzustand | [Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) |
| Krankheit/Parasiten | vorgesehen | Infektion/Verwandlung bei manchen Arten | `M-B37`; „144-146 genau“ in `23#1075E` |
| Archiv | Tierstudium | Bestiarium | [Modul 14](14-Forschung-und-Bestiarium.md) |

---

## 4. Tiere und das Übernatürliche

`23#1026A` Punkt 35 (Beispiele: Hunde bellen, Vögel verstummen, Wildtiere fliehen) mit der Bestätigung „ja aber man denkt sixh nicht immer was dabei“ (`23#1027E`). Daraus (💡):

- Tiere sind **schwache Hinweisgeber** im Vorlauf eines Vorfalls ([Modul 15](15-Vorfaelle-und-lebendige-Welt.md) §3), keine sicheren Anzeiger.
- Dieselbe Tierreaktion kann viele Ursachen haben (ein Raubtier, ein Sturm, ein Mensch). Die Ermittlung wertet sie als **mehrdeutigen Hinweis**.
- Manche Wesen beeinflussen Tiere gezielt (artspezifisch, ❓).
- Der Hund des Spielers reagiert auf Wesen (Bellen, Fluchtverhalten), seine Aussagekraft ist begrenzt ([Ermittlung 05](../Ermittlung/05-Spuren-Tracking-Forensik-und-Zeugen.md)).

---

## 5. Der Hund als Begleiter

Der Master (`M-CA0905` §4) beschreibt das Hundesystem; der Entwickler stuft es als „sehr wichtig“ ein und sieht den Hund als Begleiter bei Fällen (`23#1027E` Punkt 22). `23#1074A` Punkt 81 und „76-83 ja“: Der Hund folgt Gerüchen und Spuren, ist aber **nicht allwissend**. Für Kreaturen gilt:

| Aspekt | Aussage |
|---|---|
| Tracking | folgt umweltbezogenen Spuren, nicht einem Zielpfeil (`M-CA0905` §4) |
| Wesenerkennung | nur plausibel (Geruch, Verhalten); Fehler möglich (💡) |
| Verletzbarkeit | Hunde können verletzt werden, altern, brauchen Tierarzt (`M-CA0905` §4) |
| Gefahr | ein Hund kann von einem Wesen besessen, verflucht oder angegriffen werden (💡, keine Quelle für Einzelfälle) |

---

## 6. Entwurf: Prüfregeln

| Regel | Bezug |
|---|---|
| Ein Tier ist kein Monsterdetektor; seine Reaktion führt nicht zu einer eindeutigen Aussage | `23#1026A` Punkt 35 |
| Der Hund führt nicht zu einem Ziel ohne spurbezogene Grundlage | `M-CA0905` §4 |
| Ein archivpflichtiges Tier bleibt auffindbar | `M-PD` |
| Ein Monster erscheint nicht in der Tierliste und umgekehrt | Abgrenzung (Abschnitt 2) |

---

## 7. Sonderfälle

| Fall | Behandlung |
|---|---|
| Ein verwandeltes Tier | Ursprung ‚verwandelt‘; Art ‚Wesen‘ (❓ Q-K28) |
| Ein Monster ernährt sich von Tieren | Ökologie: Nahrung ([Modul 08](08-Oekologie-Populationen-und-Lebenszyklus.md) §5) |
| Seuchen bei Tieren als Hinweis | möglich (💡) |

---

## 8. Was fehlt

- Ob und wie Tier-Krankheiten und Monster-Infektionen verknüpft sind (❓).
- Tiere mit übernatürlicher Veränderung (❓ Q-K28).
- Hund und Besessenheit/Fluch (❓).
