# Kampfsystem 23: Koop, Synchronisierung und Autorität

> [!IMPORTANT]
> **Nichts davon ist implementiert, und die Netzwerkarchitektur ist ausdrücklich nicht entschieden.** Das Modul sammelt die Koop-Entscheidungen, soweit sie den Kampf betreffen (Host-Spielstand, individuelle Folgen, Krankenhaus), und stellt die Autoritäts- und Synchronisierungsfragen als **Entwurf** und **Gegenüberstellung** dar. Es entscheidet kein Netzwerkmodell. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Vertiefung: [Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md), [NPCs 23](../NPCs/23-Koop-Host-Speicherstand-und-Mods.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Wer besitzt was im Koop-Kampf (Entwurf)](#3-wer-besitzt-was-im-koop-kampf-entwurf)
4. [Treffer, Projektile und Zustände (Entwurf)](#4-treffer-projektile-und-zustände-entwurf)
5. [Verletzung, Tod und Rettung im Koop](#5-verletzung-tod-und-rettung-im-koop)
6. [Gegenüberstellung der Autoritätsmodelle](#6-gegenüberstellung-der-autoritätsmodelle)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Koop ist die Einzelspieler-Kampagne für primär 2 bis 4 Spieler,** mit Mods und Cheats, ohne Transfer zur offiziellen Online-Progression (`M-PD` ‚Multiplayer separation‘, Spielvision §13).
2. **Der Host-Spielstand ist maßgeblich;** der Fortschritt von Gästen gehört zum Host-Spielstand, die Welt ist die Welt des Hosts (`N-KO-001`, `E-KO-004`, `CLAUDE.md` §6).
3. **Die Netzwerkarchitektur und ihr Zeitpunkt sind offen** (Spielvision §13, `E-KO-010`, `N-KO-018`). Es gibt keine Entscheidung zu Autorität, Replikation, Latenzausgleich oder Treffervalidierung.
4. **Individuelle Folgen:** Verhaftung ist individuell, die Welt läuft weiter, ein Welt-Zeitsprung braucht die Zustimmung aller aktiven Spieler (`N-KO-008`); das Krankenhaus ist im Koop überspringbar (`E-KO-007`).
5. **Offen für den Kampf:** Treffer-Autorität, Friendly Fire zwischen Spielern, PvP im Online-Modus, Verbindungsabbruch im Kampf, Speichersperre bei mehreren Spielern, Schwierigkeit pro Spieler gegenüber Host (`CQ-96` bis `CQ-101`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-KO-001 | **Koop ist die Kampagne für primär 2 bis 4 Spieler;** keine eigene Koop-Geschichte nötig; Mods ja, Cheats ja; kein Transfer zur offiziellen Online-Progression | 📋 BESCHLOSSEN | `M-PD` ‚Multiplayer separation‘; `N-KO-003`; Spielvision §13 | KOO | – |
| COMBAT-DEC-KO-002 | **Der Host-Spielstand ist maßgeblich;** Gast-Fortschritt ist an den Host-Spielstand gebunden; die Welt ist die Welt des Hosts; was ein Gast auf seinem eigenen Spielstand hat, bleibt dort | 📋 BESCHLOSSEN | `N-KO-001`, `N-KO-002`; `E-KO-004`; `E-WZ-005`; `CLAUDE.md` §6 | KOO PER | – |
| COMBAT-DEC-KO-003 | **Jeder Koop-Spieler erstellt seinen eigenen Hunter** | 📋 BESCHLOSSEN | `E-KO-006` | KOO | Folge: Körperzustand und Ausrüstung je Spieler |
| COMBAT-DEC-KO-004 | **Netzwerkarchitektur, Replikation, Autorität und Zeitpunkt der Umsetzung sind nicht entschieden** | ❓ OFFEN | Spielvision §13; `E-KO-010`; `N-KO-018`; `CLAUDE.md` §6 | KOO | `CQ-97` |
| COMBAT-DEC-KO-005 | **Der offizielle Online-Modus ist serverautoritativ mit Anti-Cheat, ohne Mods und Cheats** und hat eine gemeinsame persistente Online-Progression je Slot; langfristig 64 bis 100 Spieler pro große Instanz als technisches Ziel, keine Zusage | 🧭 RICHTUNG | `M-PD` ‚Multiplayer separation‘ (keine Mods, keine Cheats); Spielvision §13 (🔭) | KOO PRF | Kampfmodus im Online-Modus: `CQ-96` |
| COMBAT-DEC-KO-006 | **Eine Verhaftung ist individuell;** die Welt läuft weiter; ein Welt-Zeitsprung im Koop nur mit Zustimmung aller aktiven Spieler | 📋 BESCHLOSSEN | `N-KO-008`; `E-HA-004` | KOO POL | – |
| COMBAT-DEC-KO-007 | **Im Koop ist das Krankenhaus überspringbar** (Ausfall wenige Minuten bis ca. 15 Minuten); es gibt einen klassischen Modus; ein detailliertes Rettungssystem mit vereinfachter Wiederbelebung als Option | 📋 BESCHLOSSEN | `E-KO-007`, `E-KO-008` | KOO MED | `COMBAT-DEC-BW-025`; `CQ-62` |
| COMBAT-DEC-KO-008 | **Friendly Fire gegen Begleiter ist konfigurierbar;** gegenüber anderen Spielern im Koop ist es nicht beschrieben | ❓ OFFEN | `COMBAT-DEC-GP-023`; Suche am 23.10.2026 (keine Fundstelle) | KOO KI | `CQ-96` |
| COMBAT-DEC-KO-009 | **Mods und Cheats sind im Koop erlaubt;** Mods sind pro Spielstand ein- und ausschaltbar; kritisch fehlende Mods blockieren das Laden | 📋 BESCHLOSSEN | `K-PD-004`, `K-PD-005`; `N-KO-013` | MOD KOO | Mod-Synchronisierung im Koop: `CQ-99` |
| COMBAT-DEC-KO-010 | **Die Simulation läuft nur auf der Autorität;** die Replikation des Simulationszustands ist offen; `WorldFacts` hat noch kein Compare-and-Set | ❓ OFFEN | `N-KO-018`; Gameplay-Systeme §16 (letzte Zeile) | KOO PER | `CQ-97` |
| COMBAT-DEC-KO-011 | **Bewegung bleibt lokal und autoritativ je Spieler** und hat keinen Weltzustands-Einfluss (für Traversal) | 💡 VORSCHLAG | Proposal 0009 §4 | KOO BEW | – |
| COMBAT-DEC-KO-012 | **Körperzustand, Ausrüstung und Wundeinträge gehören dem Spieler,** aber im Host-Spielstand gespeichert | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-KO-002`, `COMBAT-DEC-KO-003`, `COMBAT-DEC-ZA-013` | KOO PER | – |
| COMBAT-DEC-KO-013 | **Fallen, abgelegte Waffen, Tatorte und tote Körper sind Weltzustand des Hosts,** nicht Eigentum eines Spielers | 🧩 ABGELEITET | Prämissen: `COMBAT-DEC-KO-002`, `COMBAT-DEC-FF-004`, `COMBAT-DEC-WA-007` | KOO PER | – |
| COMBAT-DEC-KO-014 | **Autoritätsaufteilung für den Kampf** (Eingabe lokal, Körperzustand autoritativ, Projektile autoritativ mit Vorschau, Animation lokal aus Zustand, KI nur Autorität) | 💡 VORSCHLAG | Abschnitt 3 | KOO | – |
| COMBAT-DEC-KO-015 | **Netzwerkereignisse sind idempotent** (eindeutige Projektil-, Magazin-, Wund-IDs, damit Wiederholung nichts doppelt erzeugt) | 💡 VORSCHLAG | Abschnitt 4; `COMBAT-DEC-ZA-016` | KOO | – |
| COMBAT-DEC-KO-016 | **Treffer-Autorität und Latenzausgleich** (wer entscheidet einen Treffer, wie wird Verzögerung ausgeglichen) | ❓ OFFEN | Suche am 23.10.2026 (keine Fundstelle) | KOO | `CQ-97` |
| COMBAT-DEC-KO-017 | **Gedächtnis von Gegnern gegenüber mehreren Spielern** (wen merken sie sich, wer wird Ziel) | ❓ OFFEN | `COMBAT-DEC-NM-002`; Suche (keine Fundstelle) | KI NPC | `CQ-98` |
| COMBAT-DEC-KO-018 | **Verbindungsabbruch, Beitritt und Verlassen im Kampf** (Körper, Ausrüstung, Zustand des Gastes) | ❓ OFFEN | Suche (keine Fundstelle) | KOO | `CQ-99` |
| COMBAT-DEC-KO-019 | **Speichersperre im aktiven Kampf bei mehreren Spielern** (gilt für alle, wenn einer entdeckt ist) | ❓ OFFEN | `COMBAT-DEC-GP-028`; Suche (keine Fundstelle) | PER KOO | `CQ-100` |
| COMBAT-DEC-KO-020 | **Schwierigkeit und Hilfen im Koop** (pro Spieler oder durch Host; Zielhilfe, Anzeige, Ressourcen) | ❓ OFFEN | `COMBAT-DEC-GP-010`; Suche (keine Fundstelle) | SCH KOO | `CQ-101` |
| COMBAT-DEC-KO-021 | **Kampfmodus im Online-Modus:** Spielergegen-Spieler, ‚Feindseligkeits-/Verteidigungsmodus‘ und Kopfgelder als mögliche Elemente; weniger Griefing | 🧭 RICHTUNG | Spielvision §13 (Online-Zeile, 🔭 / ❓) | KOO | `CQ-96` |

---

## 3. Wer besitzt was im Koop-Kampf (Entwurf)

> **💡 VORSCHLAG (`COMBAT-DEC-KO-014`).** Die Tabelle ist eine Prüffrage an die spätere Architektur, keine Entscheidung. Sie folgt dem Prinzip ‚ein Besitzer je Tatsache‘ (`COMBAT-DEC-ZA-013`) und dem Grundsatz ‚Host-Spielstand maßgeblich‘.

| Tatsache | Autorität (Entwurf) | Replikation (Entwurf) |
|---|---|---|
| Eingaben | lokal | Absichten an die Autorität |
| Bewegung und Pose | lokal vorausgesagt, Autorität bestätigt | Zustand mit Glättung |
| Körperzustand (Wunden, Blut, Schock) | Autorität | Zustand zu allen (Anzeige), Details nur dem Besitzer |
| Waffenzustand und Magazine | Autorität | Zustand zum Besitzer, sichtbare Teile zu allen |
| Projektile | Autorität | Ereignis ‚Schuss‘ plus Ergebnis ‚Treffer‘; Vorschau lokal |
| KI, Wahrnehmung, Gedächtnis | nur Autorität | Verhalten sichtbar, Wissen nicht |
| Weltobjekte (Fallen, Waffen am Boden, Tatorte) | Autorität | Zustandsänderungen |
| Persistente Folgen (Tod, Verletzungen) | Autorität (Host-Spielstand) | Journaleinträge |

**Prüfbare Eigenschaften (🧩 ABGELEITET):** (1) Zwei Spieler sehen denselben Weltzustand nach Abschluss eines Kampfes. (2) Kein Spieler kann den Körperzustand eines anderen direkt schreiben. (3) Ein Gast verliert beim Verlassen weder Weltzustand noch Fortschritt des Hosts. (4) Ein Netzwerk-Ereignis wird bei Wiederholung nicht doppelt angewendet.

---

## 4. Treffer, Projektile und Zustände (Entwurf)

> **💡 VORSCHLAG.** Die Reihenfolge zeigt, welche Fragen offen sind (`CQ-97`).

```text
Client:    Schuss-Absicht (Zeit, Richtung, Waffe)  -> Vorschau lokal (Mündungsfeuer, Geräusch)
Autorität: prüft Waffenzustand (Patronenlager, Störung), erzeugt Projektil mit eindeutiger ID
           -> simuliert Flug und Treffer (Modul 05, 12), wendet Wunde an (Modul 12)
           -> Ereignisse (HitResolved, WoundCreated) an alle; Anzeige korrigiert Vorschau
Persistenz: Folgen (Tod, bleibende Verletzung) im Host-Spielstand
```

Offene Fragen: Wer bestimmt die Position des Ziels zum Schusszeitpunkt (Latenzausgleich)? Wie viel der Projektilbahn wird repliziert (`COMBAT-DEC-FK-014`, keine Maximalreichweite)? Wie wird Schrot (Mehrfachprojektile) repliziert? Alle Fragen stehen in `CQ-97`.

---

## 5. Verletzung, Tod und Rettung im Koop

- Der Körperzustand gehört dem Spieler (Abschnitt 3); Wiederbelebung durch andere Spieler ist als **vereinfachte Option** beschlossen (`COMBAT-DEC-KO-007`).
- Krankenhaus ist im Koop überspringbar; der Ausfall dauert wenige Minuten bis etwa 15 Minuten (`E-KO-007`).
- Begleiter-NPCs im Koop: Gemeinsame Ermittlungsakten, Beweise, Kartenmarkierungen und Teamfortschritt sind beschlossen (`E-MO-019`); Körperzustände von Begleitern folgen der Regel der Autorität.
- Verhaftung ist individuell (`COMBAT-DEC-KO-006`); die anderen spielen weiter.
- Gameplay-Tod im Koop: Wie `COMBAT-DEC-BW-012` mit mehreren Spielern kombiniert wird, ist offen (`CQ-62`).

---

## 6. Gegenüberstellung der Autoritätsmodelle

> Die Quellen entscheiden das Netzwerkmodell nicht. Die Gegenüberstellung hilft, die Fragen vor der Entscheidung zu ordnen.

| Modell | Beschreibung | Vorteil | Nachteil | Bezug zu den Quellen |
|---|---|---|---|---|
| A: Host als autoritativer Teilnehmer (Listen-Server) | Der Host-Rechner führt die Simulation, Gäste senden Absichten | passt zu ‚Host-Spielstand maßgeblich‘; geringe Infrastruktur | Host-Vorteil bei Latenz; Host-Abbruch beendet das Spiel | `COMBAT-DEC-KO-002` |
| B: Dedizierter Server pro Sitzung | ein Prozess hält die Welt | fairer, wiederverwendbar für Online | Kosten, Infrastruktur; passt nicht zu ‚offline-fähig‘ im Koop | `COMBAT-DEC-KO-005` |
| C: Verteilt mit Eigentum je Spieler | jeder Spieler hat Autorität über seine Kämpfer | geringe Latenz im Eigenbereich | Konsistenzprobleme, Cheats (im Koop erlaubt) | – |
| D: Unterschiedlich nach Modus | Koop Modell A, Online Modell B | Anpassung an Modus | zwei Pfade zu testen | `COMBAT-DEC-KO-001`, `-005` |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zur Persistenz ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | Host-Spielstand, Compare-and-Set |
| zur KI ([Modul 18](18-Gegner-KI-und-Kampftaktik.md)) | Autorität, Ziele |
| zu Mods | Gleichheit der Moddaten |
| zu Ermittlung ([Ermittlung 18](../Ermittlung/18-Koop-Ermittlung.md)) | gemeinsame Akten |
| zur Performance ([Modul 25](25-Performance-Skalierung-und-Simulationsstufen.md)) | Netzwerklast |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Gast verlässt im Kampf | Körper und Ausrüstung bleiben im Host-Spielstand; Figur wird gesichert oder abgemeldet (Regel offen) | ❓ |
| Host bricht ab | Spielstand bleibt konsistent (atomares Speichern) | 🧩 (Persistenz, README §26) |
| Zwei Spieler erzeugen gleichzeitig dieselbe Wirkung | Reihenfolge der Autorität entscheidet; keine Doppelung | 💡 |
| Spieler mit anderen Mods | Beitritt abgelehnt oder Mods synchronisiert (offen) | ❓ |
| Zeitsprung ohne Zustimmung aller | nicht möglich (`N-KO-008`) | 📋 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-96` | Friendly Fire zwischen Spielern, Kampf Spieler gegen Spieler im Online-Modus, Feindseligkeitsmodus | `COMBAT-DEC-KO-008`, `-021` | ja |
| `CQ-97` | Netzwerkarchitektur: Autorität, Replikation, Treffervalidierung, Latenzausgleich | `COMBAT-DEC-KO-004`, `-010`, `-016` | ja |
| `CQ-98` | Gedächtnis und Zielwahl der Gegner bei mehreren Spielern | `COMBAT-DEC-KO-017` | ja |
| `CQ-99` | Verbindungsabbruch, Beitritt im Kampf, Mod-Gleichheit | `COMBAT-DEC-KO-009`, `-018` | ja |
| `CQ-100` | Speichersperre im Kampf bei mehreren Spielern | `COMBAT-DEC-KO-019` | ja |
| `CQ-101` | Schwierigkeit und Hilfen im Koop: pro Spieler oder durch Host | `COMBAT-DEC-KO-020` | ja |
