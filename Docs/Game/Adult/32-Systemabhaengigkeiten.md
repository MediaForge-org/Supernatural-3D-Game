# Adult 32: Systemabhängigkeiten

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Die Übersicht zeigt, welche Spielsysteme das Adult-Paket **voraussetzt**, welche es **erweitert** und welche es **nicht berühren darf**. Sie ist ein Planungswerkzeug (🧭), kein Auftrag. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Quellen: Assistent 35#605 (Unterbau-Liste für Adult V1), 35#706 (Abhängigkeiten aus Randfall-Audit), 35#473 (Architektur), Spezifikationen [20](20-Spezifikation-Regelkern-Zustimmung.md)–[28](28-Spezifikation-Speicherung-Mods-Builds.md). Eine Abgleichung mit dem tatsächlichen Repository-Stand steht unter [Modul 02, Abschnitt 9](02-Architektur-Pakete-Technik.md#9-einordnung-im-bestehenden-repository) und in `../Gameplay-Systeme.md`.

**Inhalt**

1. [Leseweise](#1-leseweise)
2. [Abhängigkeitsmatrix](#2-abhängigkeitsmatrix)
3. [Voraussetzungen im Detail](#3-voraussetzungen-im-detail)
4. [Was das Paket erweitert](#4-was-das-paket-erweitert)
5. [Was das Paket nicht berühren darf](#5-was-das-paket-nicht-berühren-darf)
6. [Reihenfolge der Abhängigkeiten](#6-reihenfolge-der-abhängigkeiten)
7. [Zusammenhang mit der Entwicklungsphasenplanung](#7-zusammenhang-mit-der-entwicklungsphasenplanung)
8. [Abhängigkeiten zwischen Spezifikationsmodulen](#8-abhängigkeiten-zwischen-spezifikationsmodulen)

---

## 1. Leseweise

- **Braucht (←)**: Das Adult-Paket kann ohne das System nicht arbeiten.
- **Erweitert (→)**: Das Paket ergänzt das System, das System funktioniert ohne das Paket.
- **Verboten (✗)**: Das Paket darf das System nicht verändern.
- Alle Einträge sind **Planung**. Welche Systeme bereits im Repository vorhanden sind, ist nicht Gegenstand dieser Matrix (Phase 0.3 enthält u. a. Weltzustand und Türinteraktion, [Modul 02](02-Architektur-Pakete-Technik.md#9-einordnung-im-bestehenden-repository)).

## 2. Abhängigkeitsmatrix

| System | Beziehung | Begründung | Quelle |
|---|---|---|---|
| Menschensystem (Körper, Generator, Alterung) | ← braucht | ein Körpersystem für alles | E-SO-005, E-KO-011 |
| Charakterkern (Geburtsdatum, Identität, Zustand) | ← braucht | Altersgate | E-SK-012 |
| Persistenzsystem (Delta-Saves, IDs) | ← braucht | getrennter Speicherbereich | E-PA-025, E-PA-029 |
| Weltzustand / Zeit / Kalender | ← braucht | Zeitmaßstab, Zeitpläne | E-LE-011 |
| NPC-Simulation (Tiers, Gedächtnis, Beziehungen) | ← braucht | Präferenzen, Initiative | E-LE-002 |
| Kleidung / Charakter-Layer | ← braucht | Kleidung als Schicht | E-IA-016 |
| Animation, Full-Body-IK, Retargeting | ← braucht | Freeform-Kette | E-SO-008 |
| Physik / Kollision / Weichgewebe | ← braucht | Kontakt, Sekundärbewegung | E-SO-013 |
| Gesundheits- und Verletzungssystem | ← braucht | Unified Risk Pipeline | E-KO-027 |
| Eingabe / UI / Mod-Manager | ← braucht | Steuerung, Bibliothek | E-ST-002 |
| Audio (Stimme, Atmung, Raumakustik) | ← braucht | Reaktionen | E-KO-021 |
| Ortssystem (Möbel, Türen, Privatheit) | ← braucht | Umgebungskontext | E-IA-035 |
| Kriminalitäts-/Justizsystem | ← braucht | Konsequenzebene | E-WE-014 |
| Wirtschaft / Internet / Dienstleistungen | ← braucht | Betriebe, Medien | E-WE-004 |
| Replay / Aufnahme / Streaming | ← braucht | Medien, Videos | E-RP-002 |
| Netzwerk / Mehrspieler | ← braucht | Host-Autorität | E-SP-005 |
| Mod-Schnittstellen und Registries | ← braucht | Paket als Mod | E-PA-034 |
| Lifetime-Statistik | → erweitert | Adult-Statistik optional | E-LE-009 |
| Beziehungs-/Romantik-Sandbox (Basisspiel) | → erweitert | Basis enthält Beziehungen | E-PA-010 |
| Tattoo-/Körpermodifikation (Hauptspiel) | → erweitert | intime Detailoptionen | E-TA-015 |
| Charaktereditor | → erweitert | selber Editor | E-KO-015 |
| Kampf-/Bodenkampf-Engine | → teilt Unterbau | Kontaktgraph, Solver | E-SO-023 |
| Medizin / Tragen / Umarmen | → teilt Unterbau | Interaktionsengine | E-SO-023 |
| Sicherheitskern (Gate) | ✗ nicht änderbar | von Mods nicht änderbar | E-SK-004 |
| Altersdaten | ✗ nicht setzbar | nur abgeleitet | E-SK-012 |
| Basisbuild-Inhalt | ✗ keine Adult-Spuren | Build-Trennung | E-PA-032 |
| Konsolenbuilds | ✗ nicht enthalten | getrennte Pakete | E-PA-004 |
| Offizielles Online | ✗ nicht verfügbar | keine Mods | E-PA-006 |

## 3. Voraussetzungen im Detail

### 3.1 Menschensystem

Siehe [Modul 23](23-Spezifikation-Reaktions-und-Koerpersystem.md). Notwendig: kontinuierliche Parameter, Asymmetrie, Weichgewebe-Profile, Kleidungs-/Kontaktschichten, Alterung, Schwangerschaft. **Ohne Menschensystem keine Adult-V1**. Der Assistent (35#605) nannte als Unterbau: C++-Kern, Menschengenerator V1, Rendering und Kleidung, Animation und Full-Body-IK, Interaktion/Kontakt V1, Weichgewebe V1, NPC-Intent/Präferenzen V1, minimale Beziehung/Gedächtnis, Eingabe/UI, Modlader, Audio/Mimik-Grundlage, Gesundheit/Verletzungs-Unterbau, Testwohnung. Der Umfang ~120.000–250.000 Zeilen für das Gesamtprojekt ist eine **Schätzung** dieses Assistenten.

### 3.2 NPC-Simulation und Gedächtnis

Siehe [Modul 24](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md). Notwendig: Tiers, deterministische Hintergrundsimulation, Gedächtnisstufen, Beziehungsgraph, Zeitpläne (Alltag).

### 3.3 Interaktions- und Animationspipeline

Siehe [Modul 22](22-Spezifikation-Interaktionsengine.md). Notwendig: Rig-Standard, Retargeting, Full-Body-IK, Motion Matching/Mocap-Hybrid, Kollisionsformen, Möbel als Flächen.

### 3.4 Gesundheit

Siehe [Modul 23, Abschnitt 9](23-Spezifikation-Reaktions-und-Koerpersystem.md#9-risiko-verletzung-heilung). Notwendig: Verletzungs-/Heilungsmodell, Notrufpfad, Krankenhaus; das System wird **vom Hauptspiel** getragen.

### 3.5 Welt, Wirtschaft, Internet

Siehe [Modul 26](26-Spezifikation-Weltintegration-Branche-Wirtschaft.md). Notwendig: Betriebssystem, Verträge, Internet-System, Geld, Jurisdiktionen. Die **Wirtschafts-/Internet-/Kriminalitätsplanung** des Gesamtprojekts steht aus (Assistent 35#665–674: Reihenfolge nach Adult World Time, Gebäude, Wirtschaft, Internet, Kriminalität, Fahrzeuge …, Vorschlag).

### 3.6 Persistenz und Replay

Siehe [Modul 28](28-Spezifikation-Speicherung-Mods-Builds.md), [Modul 27](27-Spezifikation-Steuerung-UI-VR-Replay.md). Notwendig: eigenes Persistenzsystem, Replay-Recorder, Cache.

## 4. Was das Paket erweitert

| System | Erweiterung |
|---|---|
| Interaktionsframework | Adult-Familien, Registry-Einträge, Reaktionsinhalte |
| Präferenzmodell | zusätzliche `PreferenceDefinition`s |
| Charaktereditor | intime Detailoptionen, Adult-Einstellungen |
| Orte | Adult-Inhalte der Venues (Nutzung, nicht Bau) |
| Dienstleistungen | Leistungsprofile |
| Statistik | Adult-Zähler (ausblendbar) |
| Reaktionsgraph | Inhalte (Mods), nicht Struktur |

## 5. Was das Paket nicht berühren darf

- Sicherheitskern (Alter, Zustimmung, Blockregeln, Serverautorität) – nicht moddbar.
- Kampf-, Fahrzeug- und Missionslogik – keine Abhängigkeit von Adult.
- Basisbuild und Konsolenbuild – kein Adult-Code.
- Offizielles Online.
- Basis-Speicherstrukturen (keine direkten Schreibzugriffe).
- Hauptspiel-Pflichtfeatures: das Hauptspiel läuft ohne Paket vollständig.

## 6. Reihenfolge der Abhängigkeiten

🧭 Aus Quellen abgeleitet; **keine Termine**.

```
Charakterkern + Persistenz  →  Menschensystem V1  →  Animation/IK/Physik-Basis
        │                              │
        ▼                              ▼
  NPC-Simulation + Gedächtnis    Interaktionskern (Gate + Sitzung)
        │                              │
        ▼                              ▼
  Beziehungen/Präferenzen        Reaktionen + Gesundheit
        │                              │
        └────────────┬─────────────────┘
                     ▼
     Orte/Welt/Wirtschaft/Internet  →  Medien/Replay  →  Tattoos/Modifikation (Hauptspiel)
                     ▼
              Modding / Builds / Recht
```

Der Entwickler legte zuletzt fest (35#715): **Ladebildschirm und ein kleiner Kartenteil kommen zuerst, Adult danach**; zu Beginn nur ein „normaler“ Umfang (35#715).

## 7. Zusammenhang mit der Entwicklungsphasenplanung

Die Phasen in CLAUDE.md §4 (Phase 0 bis 4+) bleiben maßgeblich. Das Adult-Paket ist **kein** Phase-0-Element; es ist ein optionales Paket, dessen Unterbau (Menschensystem, Persistenz, Interaktion, Gesundheit, Mods) über die Phasen wächst. **Es wurde weder ein Phase-Tag noch eine Einordnung in Phase 1–3 beschlossen.** Die Quellen nennen nur: „Adult V1 klein, sehr früh implementiert und dann sehr viel erweitert“ (35#713) und „Adult kommt erst nach Grundlagen“ (35#715). Das ist ein Widerspruch in der Gewichtung ([K19](19-Konflikt-und-Widerspruchsmatrix.md#k19-zeitpunkt-von-adult)) und bleibt dem Entwickler überlassen.

## 8. Abhängigkeiten zwischen Spezifikationsmodulen

| Modul | Hängt ab von |
|---|---|
| [20 Regelkern](20-Spezifikation-Regelkern-Zustimmung.md) | Charakterkern (Alter), Awareness-Zustand, Persistenz |
| [21 Datenmodell](21-Spezifikation-Datenmodell.md) | 20, Registry-Entscheidungen |
| [22 Interaktionsengine](22-Spezifikation-Interaktionsengine.md) | 20, 21, 23 |
| [23 Reaktion/Körper](23-Spezifikation-Reaktions-und-Koerpersystem.md) | Menschensystem, Gesundheit |
| [24 NPC/Gesellschaft](24-Spezifikation-NPC-Gesellschaft-und-Konsequenzen.md) | 20, 21, NPC-Simulation, Justiz |
| [25 Tattoos](25-Spezifikation-Tattoo-und-Koerpermodifikation.md) | Menschensystem, Wirtschaft |
| [26 Welt](26-Spezifikation-Weltintegration-Branche-Wirtschaft.md) | 24, Wirtschaft, Internet |
| [27 Steuerung/UI](27-Spezifikation-Steuerung-UI-VR-Replay.md) | 22, Eingabe, Replay |
| [28 Speicherung/Mods/Builds](28-Spezifikation-Speicherung-Mods-Builds.md) | 21, Mod-Schnittstellen |
