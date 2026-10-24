# Kampfsystem 19: Nemesis, Erinnerung und persistente Kampffolgen

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Das Modul bezieht das Kampfsystem auf die verbindliche Produktvision zu Nemesis und Rivalität (`PE-N1022`, Anforderungen A bis H). Die Entscheidungen und Entwürfe stehen in der [NPC-Dokumentation](../NPCs/README.md); hier steht nur, **was der Kampf dafür liefern und beachten muss**. Es gibt keine Rückkehrwahrscheinlichkeiten, Zeitfenster oder Lernregeln. Statuswörter: [Legende](README.md#statuslegende).

Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Das Register steht in [Modul 27](27-Entscheidungsregister.md). Nemesis-Modell: [NPCs 11](../NPCs/11-Nemesis-Modell-und-Datensatz.md); Lebenszyklus: [NPCs 12](../NPCs/12-Nemesis-Lebenszyklus-Rueckkehr-Verletzung-und-Anpassung.md).

**Inhalt**

1. [Befund in Kürze](#1-befund-in-kürze)
2. [Beschlossene und offene Anforderungen](#2-beschlossene-und-offene-anforderungen)
3. [Was ein Kampf an das Gedächtnis meldet](#3-was-ein-kampf-an-das-gedächtnis-meldet)
4. [Rückkehr, Narben, Anpassung](#4-rückkehr-narben-anpassung)
5. [Persistente Folgen für Menschen und Begleiter](#5-persistente-folgen-für-menschen-und-begleiter)
6. [Gegenüberstellung offener Lösungsansätze](#6-gegenüberstellung-offener-lösungsansätze)
7. [Schnittstellen und Fehlerfälle](#7-schnittstellen-und-fehlerfälle)
8. [Offene Punkte dieses Moduls](#8-offene-punkte-dieses-moduls)

---

## 1. Befund in Kürze

1. **Die Vision ist verbindlich:** Überlebende Gegner können zurückkehren, persönliche Feindschaften entwickeln, aus früheren Begegnungen lernen und Taktiken anpassen, auch intelligente übernatürliche Kreaturen (`PE-N1022` C, `N-NE-001`, `K-LW-002`).
2. **Das Gedächtnis ist beobachtungsgebunden:** Wesen erinnern sich an Spieler, Begleiter, Verletzungen, Fallen, Waffen, Gnade und Feindseligkeit, aber nur an Plausibles (`K-KI-013`, `K-KI-014`, Spielvision §9.2).
3. **Verletzungen und Narben bleiben,** wo die Art sie nicht regeneriert; zurückkehrende Individuen zeigen die Folgen (`COMBAT-DEC-KS-006`).
4. **Lernen ist Wissensaufbau, nicht Stärkerwerden:** kein Wert steigt mit jeder Begegnung, keine sichtbaren Stufen (`N-NE-017`).
5. **Offen sind Lernmodell, Rückkehrhäufigkeit und die Frage, wer wichtige Figuren töten darf** (`N-NE-022`, `N-LB-017`, `CK-02`).

---

## 2. Beschlossene und offene Anforderungen

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| COMBAT-DEC-NM-001 | **Überlebende Gegner können zurückkehren, Feindschaften entwickeln, aus Begegnungen lernen und Taktiken anpassen** | 📋 BESCHLOSSEN | `PE-N1022` C; `N-NE-001` | NPC KI | – |
| COMBAT-DEC-NM-002 | **Intelligente Monster haben ein umfassendes individuelles Gedächtnis** (Begegnungen, Kampferfahrungen mit Waffen, Fallen, Strategien, Beziehungen, Rache, Angst, Informationsaustausch) | 📋 BESCHLOSSEN | `N-NE-002`; `K-KI-014` (`48#139E`) | NPC KRE | Ausformulierung `N-NE-003` ist 💡 |
| COMBAT-DEC-NM-003 | **Ein Gegner, der nur vertrieben wurde oder entkam, kann später mit Erinnerung und früheren Verletzungen auftauchen,** muss aber nicht | 📋 BESCHLOSSEN | `N-NL-003`; `23#294E` (‚es kann auftauchen aber es muss nicht‘ in der Quelle) | NPC | Häufigkeit: `N-NE-022` |
| COMBAT-DEC-NM-004 | **Narben und bleibende Verletzungen überdauern,** wo die Art nicht regeneriert | 📋 BESCHLOSSEN | `K-KO-007`; `N-NE-013`, `N-NL-006` | NPC KRE | – |
| COMBAT-DEC-NM-005 | **Gegner kehren verändert zurück** (Narben, besser vorbereitet, mit Verbündeten, wissend, was gewirkt hat) | 🧭 RICHTUNG | `N-NL-010` (📋 Richtung); `PE-N1022` C | NPC KI | – |
| COMBAT-DEC-NM-006 | **Lernen ist Wissensaufbau, nicht Stärkerwerden;** keine sichtbaren Stufen, kein Wert steigt mit jeder Begegnung | 📋 BESCHLOSSEN | `N-NE-017`; `M-PD` ‚Genre and progression identity‘ | KI SCH | – |
| COMBAT-DEC-NM-007 | **Die Anpassung übernatürlicher Wesen ist durch Artregeln und Schwächen begrenzt,** nie freie Erfindung | 💡 VORSCHLAG | `N-NE-020`; `K-LW-005` | KI KRE | Wie ein Nemesis lernt: `N-NE-022` ❓ |
| COMBAT-DEC-NM-008 | **Hunter-NPCs können Rivalen werden;** sie haben Ruf, bearbeiten Fälle vor dem Spieler, behandeln sie falsch, konkurrieren oder kooperieren | 📋 BESCHLOSSEN | `N-NE-015`; `N-BH-013` | NPC | – |
| COMBAT-DEC-NM-009 | **Ein verschontes friedliches Wesen wird nicht automatisch zum Feind;** ein anderer Hunter kann es töten | 📋 BESCHLOSSEN | `N-NE-016`; `K-GS-004` | NPC KRE | – |
| COMBAT-DEC-NM-010 | **Rivalen können im Fight Club entstehen** (Beziehungen, Trainer, Reputation, Hunter-Kontakte) | 💡 VORSCHLAG | `N-NE-021`; `23#164A`, `23#166A` | NPC | `CQ-13` |
| COMBAT-DEC-NM-011 | **Kriminelle Gruppen können wissen, wer der Spieler ist;** wiederholte Probleme mit derselben lokalen Polizei können deren Verhalten beeinflussen | 📋 BESCHLOSSEN | `N-NE-018`; `N-BH-007` | NPC POL | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md) |
| COMBAT-DEC-NM-012 | **Normale NPCs können verletzt werden und genesen;** schwere Verletzungen haben bei wichtigen NPCs dauerhafte Folgen; Krankenhaus und Rettung sind Teil der Welt | 📋 BESCHLOSSEN | `N-LB-012`, `N-NL-007` | NPC MED | – |
| COMBAT-DEC-NM-013 | **Spieler und menschliche NPCs teilen dieselbe Trauma-Regel;** kein Plot Armor | 📋 BESCHLOSSEN | `N-LB-013`; `COMBAT-DEC-GP-007` | NPC MED | Spannung: `CK-02` |
| COMBAT-DEC-NM-014 | **Wichtige NPCs sterben nicht durch banale Simulationsfehler;** Kernfiguren des Hunter-HQ haben keinen zufälligen Sandbox-Tod, ein geschriebener Story-Tod bleibt | 📋 BESCHLOSSEN | `N-LB-017`; `N-CO-007`; `M-CA0905` (Z. 44) | NPC | Wer sie töten darf: NPC-Konflikt C-N1, `CK-02` |
| COMBAT-DEC-NM-015 | **Normale und rekrutierbare Begleiter können dauerhaft sterben** | 📋 BESCHLOSSEN | `N-CO-007`; `M-CA0905` | NPC | – |
| COMBAT-DEC-NM-016 | **Der Tod eines NPCs bleibt dauerhaft im Weltzustand;** Identität und Historie bleiben, nur unnötiger technischer Detailzustand darf verdichtet werden | 📋 BESCHLOSSEN | `N-LB-018`, `N-LB-016` | PER NPC | – |
| COMBAT-DEC-NM-017 | **Wer dem Spieler relevant begegnet (Beziehung, Story, bedeutende Begegnung), steigt in eine höhere Persistenzstufe auf** | 📋 BESCHLOSSEN | `N-ID-015` | PER NPC | Kampf kann Aufstieg auslösen: Abschnitt 3 |
| COMBAT-DEC-NM-018 | **Ein Kampf meldet Ereignisse an das Gedächtnis,** keine Direktschreibzugriffe | 💡 VORSCHLAG | `COMBAT-DEC-ZA-013` | NPC | Abschnitt 3 |
| COMBAT-DEC-NM-019 | **Rückkehrhäufigkeit, Lernmodell (Regeln oder Daten) und was der Spieler von der Feindschaft sieht** sind offen | ❓ OFFEN | `N-NE-022`; `K-LW-009` | NPC KI | `CQ-78` |
| COMBAT-DEC-NM-020 | **Wer wichtige NPCs töten darf (Spieler, Fraktionen, Hunter, Wesen)** ist nicht entschieden | ❓ OFFEN | NPC-Konflikt C-N1; `N-LB-017` | NPC | `CQ-79` |
| COMBAT-DEC-NM-021 | **Ruhm und Bekanntheit durch Kämpfe** (Medien, Social Media) sind selten, aber wirksam; Häufigkeit offen | 🧭 RICHTUNG | NPC-Konflikt C-N8; `23#1057E` P45 | NPC POL | `CQ-80` |

---

## 3. Was ein Kampf an das Gedächtnis meldet

> **💡 VORSCHLAG (`COMBAT-DEC-NM-018`).** Der Kampf schreibt nicht in NPC-Datensätze; er sendet Ereignisse. Die Liste nennt, **was** beobachtbar sein muss.

| Ereignis | Inhalt (nur Beobachtbares) | Empfänger |
|---|---|---|
| `EncounterSeen` | Wer sah den Spieler, ab wann, aus welcher Entfernung | Wissen der Figur |
| `WeaponObserved` | gesehene Waffe, Munition (Geräusch, Wirkung) | Gedächtnis |
| `TrapEncountered` | erkannte oder ausgelöste Falle | Gedächtnis, Taktik |
| `InjuryInflicted` | Wunde an Wesen oder Mensch (Region, Ursache) | Körper, Gedächtnis |
| `MercyShown` / `Fled` | Gnade, Flucht, Aufgabe | Beziehung, Moral |
| `AllyHarmed` | Begleiter oder Gruppe verletzt | Rache, Beziehung |
| `Witnessed` | Zeugen des Kampfes | Ermittlung, Ruf |

**Regel (🧩 ABGELEITET aus `COMBAT-DEC-KI-003`):** Nur Figuren, die ein Ereignis plausibel wahrgenommen haben, erhalten es. Ein Aufstieg der Persistenzstufe (`COMBAT-DEC-NM-017`) ergibt sich aus der Bedeutung der Begegnung, nicht aus der bloßen Zahl der Treffer.

---

## 4. Rückkehr, Narben, Anpassung

```text
Begegnung endet ohne Tod (Flucht, Vertreibung, Rettung, Sieg über andere)
  -> Verletzung/Narbe im Körperdatensatz des Individuums
  -> Gedächtnis: gesehene Waffen, Fallen, Methoden, Gnade
  -> Weltzeit vergeht: Erholung (Regeneration der Art), Verstecken, Verbündete suchen
  -> Rückkehr (muss nicht): mit Narben, Wissen, ggf. neuer Taktik – innerhalb der Artregeln
```

Die Kette folgt `N-NL-001` bis `N-NL-003`, `N-NL-010`, `K-KO-007`. Sie ist als Ablauf Entwurf; Zeiten und Wahrscheinlichkeiten fehlen.

**Prüfbare Eigenschaften (🧩 ABGELEITET):** (1) Eine Narbe, die die Art nicht regeneriert, ist beim Wiedersehen vorhanden. (2) Ein Wesen kennt nur Waffen und Methoden, die es selbst oder durch plausible Weitergabe erfahren hat. (3) Die Anpassung bleibt in der Taktikliste der Art. (4) Der Tod eines Individuums ist endgültig (`BLOCK_09_STATUS`: ‚individual death permanence‘, `K-HW-011` für das Fegefeuer).

---

## 5. Persistente Folgen für Menschen und Begleiter

| Thema | Aussage | Quelle |
|---|---|---|
| Verletzte NPCs | werden behandelt und genesen | `COMBAT-DEC-NM-012` |
| Tote NPCs | bleiben tot; Geschäft, Familie, Orte reagieren | `N-LB-016` |
| Begleiter | können ausfallen oder sterben (außer geschützte Figuren) | `N-CO-006`, `N-CO-007` |
| Friendly Fire | kann Beziehungsfolgen haben (versehentlich ≠ rücksichtslos) | `23#223A` (Entwurf); `N-CO-006` |
| Gegner mit Rachemotiv | Familien, Gruppen, Fraktionen | [Modul 20](20-Fraktionen-Polizei-und-Kampfkonsequenzen.md), `N-FR-001` |
| Weltzustand | große Folgen (Tod, zerstörte Infrastruktur) im Journal | `PE-N1022` E |

---

## 6. Gegenüberstellung offener Lösungsansätze

| Frage | Ansatz | Vorteil | Nachteil |
|---|---|---|---|
| Lernmodell (`CQ-78`) | A: Regelbasiert (benannte Schlüsse wie ‚Waffe X wirkte nicht‘) | testbar, erklärbar | wenig Überraschung |
| | B: Datenbasiert (gewichtete Erinnerung) | flexibel | schwer zu prüfen |
| | C: Hybrid | Balance | Komplexität |
| Tötung wichtiger NPCs (`CQ-79`) | A: Nur der Spieler und die Story | schützt Kernfiguren | Welt wirkt kontrolliert |
| | B: Auch Fraktionen und Wesen | lebendige Welt (D, F der Vision) | widerspricht `N-LB-017` |
| | C: Stufenmodell (Schutzstufen) | Mittelweg | Datenmodell |
| Rückkehr (`CQ-78`) | A: Selten, bedeutsam | Gewicht | wenig Nemesis-Gefühl |
| | B: Häufig, abgestuft | spürbar | Wiederholung |

Die Einträge sind Gegenüberstellungen, keine Empfehlungen.

---

## 7. Schnittstellen und Fehlerfälle

| Schnittstelle | Inhalt |
|---|---|
| zum Kampfzustand ([Modul 03](03-Kampfzustaende-und-Systemarchitektur.md)) | Ereignisse |
| zu NPC-Gedächtnis ([NPCs 05](../NPCs/05-Gedaechtnis-Erinnerung-und-Vergessen.md)) | Empfang und Verdichtung |
| zu Fraktionen ([NPCs 16](../NPCs/16-Fraktionen-Modell-Mitgliedschaft-und-Hierarchie.md)) | Rache, Ruf |
| zur Persistenz ([Modul 24](24-Speicherstaende-und-Weltpersistenz.md)) | Individuen, Narben, Stufen |
| zum Koop ([Modul 23](23-Koop-Synchronisierung-und-Autoritaet.md)) | Gedächtnis gegenüber mehreren Spielern |

| Fehlerfall | Erwartung | Status |
|---|---|---|
| Zeuge stirbt vor der Meldung | Ereignis geht nicht an den Toten | 💡 |
| Zwei Spieler im Koop kämpfen gegen dasselbe Wesen | Gedächtnis je Spieler und gemeinsam (offen) | ❓ |
| Wesen kehrt zurück, aber Art hat Schwäche gewechselt (Mod) | additive Regel, keine Rückwirkung auf Spielstand | 💡 |
| Individuum wird entladen und simuliert | Zustand wird nachgeführt, Rückkehr nach Weltzeit | 🧭 |

---

## 8. Offene Punkte dieses Moduls

| ID | Frage | Betroffene Anforderungen | Modul 28 |
|---|---|---|---|
| `CQ-78` | Lernmodell, Rückkehrhäufigkeit und Sichtbarkeit der Feindschaft | `COMBAT-DEC-NM-005`, `-019` | ja |
| `CQ-79` | Wer darf wichtige NPCs töten; Schutzstufen | `COMBAT-DEC-NM-014`, `-020`, `CK-02` | ja |
| `CQ-80` | Ruhm und Bekanntheit aus Kämpfen: Häufigkeit und Wirkung | `COMBAT-DEC-NM-021` | ja |
