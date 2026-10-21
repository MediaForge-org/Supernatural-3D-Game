# Adult 02: Architektur, Pakete und Technik

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Im Repository existiert dafür weder Code noch Content. Dieses Dokument beschreibt Planung und einzelne Vorschläge für die technische Umsetzung. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Die Sicherheitsregeln stehen in [Modul 01](01-Grundregeln-Alter-Zustimmung.md) und gelten über jeder technischen Entscheidung hier.

**Inhalt**

1. [Leitprinzipien](#1-leitprinzipien)
2. [Bereitstellung: Basisspiel und optionales Paket](#2-bereitstellung-basisspiel-und-optionales-paket)
3. [Paketstruktur](#3-paketstruktur)
4. [Builds](#4-builds)
5. [Modul- und Datenarchitektur](#5-modul--und-datenarchitektur)
6. [Leistung, Ausfallsicherheit, Datenschutz, Qualitätssicherung](#6-leistung-ausfallsicherheit-datenschutz-qualitätssicherung)
7. [Modding](#7-modding)
8. [Netzwerk und Mehrspieler](#8-netzwerk-und-mehrspieler)
9. [Einordnung im bestehenden Repository](#9-einordnung-im-bestehenden-repository)

---

## 1. Leitprinzipien

- 📋 **Ein gemeinsamer Human Core.** Das Erwachsenen-Paket besitzt keinen eigenen Körper, kein zweites Rig und kein zweites Charaktersystem. Es registriert nur Erweiterungen auf Mensch, Körper und Interaktion (Profile, Bindungen, Content, höhere Solver- und Kontaktpräzision in relevanten Zonen). Details: [Körper, Physik, Gesundheit](06-Koerper-Physik-Gesundheit.md).
- 📋 **Kein Parallelsystem.** Erinnerungen, Beziehungen, Physiologie, Weltzustand und Wirtschaft sind dieselben wie im Hauptspiel. „Adult“ ist keine zweite Welt neben der echten.
- 📋 **Kein „Gott-System“.** Jedes Teilsystem bleibt austauschbar.
- 📋 **Daten statt Code.** Neue Kategorien, Animationen, Profile entstehen als registrierte Daten, nicht als neuer C++-Code im Kern.
- 📋 **Gleiche öffentliche Schnittstellen.** Das offizielle optionale Paket nutzt dieselben Schnittstellen und dasselbe Paketmodell wie jede Drittanbieter-Mod. Es gibt keine Sonderabfrage der Art ‚ist das Erwachsenen-Paket installiert?‘ im Kern.
- 📋 **Erst kleiner vertikaler Schnitt, dann Skalierung** ([Produktion](10-Produktion-Recht-Vertrieb.md)).

---

## 2. Bereitstellung: Basisspiel und optionales Paket

| Ebene | Inhalt | Status |
|---|---|---|
| **Basisspiel** | Beziehungs- und Romantik-Sandbox, Nachtleben als Weltschicht, nicht explizite Darstellung von Intimität (angedeutet durch Kamera, Blocking, Audio, Reaktionen), konfigurierbare Nacktheit | 📋 |
| **Optionales Erwachsenen-Paket** | erweiterte Interaktionen, Katalog, Animationen, Stimmen, zusätzliche Kategorien | 📋 |

- 📋 Das Basisspiel hängt technisch **nicht** vom Paket ab.
- 📋 Das Paket ist **nicht Teil des Basis-Builds** und **nicht Teil der Konsolen-Builds** (Cooks).
- 📋 Im **offiziellen Online-Modus** (ohne Mods und Cheats) ist das Paket nicht nutzbar.
- 📋 In Einzelspieler und Koop-Kampagne (Mods erlaubt) ist es grundsätzlich möglich, soweit rechtlich und technisch zulässig.
- 📋 Plattformübergreifend einheitliche Basisdarstellung (PC, PlayStation, Xbox), ungefähr im Rahmen heutiger großer Rollenspiele.
- 🧭 Vorschlag aus der Rechtsdiskussion (25.09.): kein „versteckter“, nur per Flag deaktivierter Erwachsenen-Content im Basis-Auslieferungsstand. Begründung: Plattformen verlangen laut Gesprächsstand die Deklaration auch unerreichbarer Inhalte in einem Build. Die Aussagen über Plattformregeln sind **ungeprüft** ([Produktion und Recht](10-Produktion-Recht-Vertrieb.md#3-recht-und-plattformen)).

---

## 3. Paketstruktur

Beschlossene Richtung (📋, Umfang je nach Gesamtgröße):

```text
Erwachsenen-Framework      Technische Grundlage (Interaktions-System, Präferenzen, Beziehungen,
                           Intensität, Beobachten/Beitreten, Synchronisation, Konfiguration,
                           Speicherstand, Mod-Schnittstelle). Wenig eigener Content.
Content-Pakete             Standard-Inhalte, Hochintensitäts-Inhalte, Animations-, Stimmen-,
                           Kamera-/Präsentations- und Figuren-/Körper-Pakete.
Komplett-Paket             Alles zusammen, solange die Gesamtgröße sinnvoll bleibt.
```

- 📋 Pakete haben **eigene Versionen, Abhängigkeiten und Kompatibilitätsangaben** (benötigt, optional, inkompatibel, empfohlen). Der Mod-Manager löst sie auf.
- 📋 Das Framework bleibt **stabil**, Content-Pakete dürfen häufiger aktualisiert werden. Ein Hochintensitäts-Paket fügt nur Content, Presets, Animationen und Optionen hinzu und verändert das Framework nicht.
- 📋 Einzelne Kategorien eines installierten Pakets lassen sich **deaktivieren**, ohne es zu entfernen.
- 📋 Stimm-Pakete sind unabhängig von Animationspaketen (Sprachen, Qualitätsstufen, alternative Reaktions-Sets).
- 📋 Orte wie Clubs, Hotels und Wohnungen gehören **zur normalen Spielwelt**, nicht zu einem Paket. Das Paket nutzt deren Räume, Möbel, Privatsphäre, Besitzer, Öffnungszeiten und Sicherheitsregeln.
- 📋 **Kein einzelner globaler Intensitätsschalter.** Das Paket ist an oder aus; wer weniger Extremes möchte, installiert ein anderes Paket (Entwickler, 10.09.). Innerhalb des Pakets sind Kategorien und Presets einzeln konfigurierbar. Der Entwickler wünscht zusätzlich ein Paket „mit allem“ neben den Teilpaketen.
- ❓ Zuschnitt, Gesamtgröße und Preset-Bündel (zum Beispiel Minimal, Standard, Voll, Hochintensität, Eigene) sind nicht entschieden.
- 🧭 Das Dreier-Schema der dunklen Inhalte (A Framework, B Hochintensität, C ‚Dunkle Kriminalität / Mature World Events‘, unabhängig schaltbar) steht in [Dunkle Themen](09-Dunkle-Themen-und-Nicht-Uebernommenes.md).

---

## 4. Builds

📋 Getrennte Builds, damit interne Testfunktionen nie im falschen Build landen:

| Build | Zweck |
|---|---|
| Basisversion | Hauptspiel ohne Erwachsenen-Paket |
| Interne Entwicklungsversion | Entwicklung ohne Paket |
| Entwicklungsversion mit Erwachsenen-Paket | Entwicklung und Test des Pakets |
| gegebenenfalls später: Distributionsversion mit Paket | öffentliche Verteilung, nach Rechts- und Plattformprüfung |

Die Werte-Ansicht verborgener Zustände ist **nur in Mod-, Debug- und Cheat-Modus** vorgesehen. Die ‚interne Testversion‘ darf keine Funktion enthalten, die der Sicherheitskern ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#1-sicherheitskern)) verbietet.

---

## 5. Modul- und Datenarchitektur

### 5.1 Code-Module (📋 Richtung)

- Erwachsenen-spezifische C++-Module sind **strikt getrennt** von allgemeinen Modulen. Das Hauptspiel muss ohne sie baubar sein.
- Das Paket soll als **optional ladbares Unreal-/Spielmodul** funktionieren, nicht tief im Basisspiel verdrahtet.
- Der Entwickler will im gesamten Projekt weitgehend **C++ statt Blueprints** und ein **eigenes Speichersystem**, weil Unreal bei dieser Größe an Grenzen stoßen könne (12.09.).

### 5.2 Stabile Kennungen und Registries

- Stabile, **versionsfreie IDs** für Teilnehmer, Sitzungen, Anfragen, Animations-Sätze, Positions-Voreinstellungen, Umgebungsanker, Oberflächen; Schema-Version von Anfang an. (Ein interner Entwurf mit versionsabhängigen IDs und Unreal-Typen im engine-unabhängigen Kern wurde als nicht übernehmbar eingestuft, 🕰️.)
- Eine **Präferenz-Registry** statt hunderter fest verdrahteter Wahrheitswerte: Mods können Einträge ergänzen, ohne den NPC-Kern zu ändern (siehe [NPCs](05-NPCs-Beziehungen-Praeferenzen.md)).
- Kategorien-Registry ist **noch nicht eingefroren** (Taxonomie-Audit und externe Gegenprüfung stehen aus, siehe [Interaktion](03-Interaktion-Taxonomie-Ablauf.md)).

### 5.3 Speicherstand (📋)

- Erwachsenen-Zustand liegt **getrennt vom normalen Spielstand** in einem eigenen Datenbereich: Präferenzen, Erfahrung, Erinnerungen, Körper- und Interaktions-Erweiterungen, Items und Verlauf, Verweise auf Replays und Statistik. Entwickler: „die Daten von Adult werden extra gespeichert“.
- Verknüpfung über **dieselben stabilen Kennungen** wie das Hauptspiel (keine doppelte Entitätswelt).
- Getrennt **sicherbar und löschbar**, optional verschlüsselt beziehungsweise nicht direkt menschenlesbar.
- Ein Spielstand muss **ohne das Paket sauber laden**. Bei erneuter Aktivierung bleiben kompatible Daten erhalten. Verwaiste Referenzen werden markiert, nicht gelöscht.
- **Schema-Migrationen** sind Pflicht.
- **Hot Enable/Disable** ohne neuen Spielstand, wo Version und Zustand kompatibel sind.
- Funktionsschalter (*Feature Flags*) für schwere Teilsysteme: Kerninteraktion, erweiterte Physik, Replay, Flüssigkeiten, VR. Späte Politur-Features hängen hinter Schaltern, damit frühe Builds schlank bleiben.

### 5.4 Determinismus

📋 Gameplay-, Gesundheits- und replay-relevante Zustände sind deterministisch genug für Speichern, Laden und Replay. Rein kosmetische Details dürfen nichtdeterministisch sein. Die Welt friert im Einzelspieler beim Beenden ein; es gibt keine heimliche Echtzeitsimulation (Entwickler, 11.09.).

---

## 6. Leistung, Ausfallsicherheit, Datenschutz, Qualitätssicherung

- 📋 **Leistung:** ereignisgesteuert statt pro Frame; Simulations-Detailstufen (nur nahe, relevante Figuren haben volle Qualität); feste Zeit-, RAM- und VRAM-Budgets pro Teilsystem, die *vor* der Implementierung festgelegt werden; bei schwächerer Hardware zuerst Detail reduzieren (Solver-Iterationen, Sekundärbewegung, Mikrodetail, Hautdetail), nicht Kernfunktionen abschalten; Arbeit über Jobs/Threads mit klarer Eigentümerschaft. Der Entwickler betont, dass nicht jeder Spieler Spitzenhardware besitzt; das Spiel soll stark optimiert werden.
- 📋 **Ausfallsicherheit:** Ein Fehler in einem optionalen Teilsystem deaktiviert das Teilsystem und protokolliert Fehlerdaten, ohne den Spielstand zu zerstören. Mods laufen über definierte APIs (Sandboxing), automatische Versions- und Abhängigkeitsprüfung.
- 📋 **Datenschutz:** Telemetrie höchstens für Funktion an/aus, generische Animationsfehler, Abstürze, Asset- und Netzwerkfehler. **Keine** detaillierten individuellen Verläufe.
- 📋 **Qualitätssicherung:** automatisierte Tests mit tausenden generierten Kombinationen (Körper, Größen, Orte, Items, Posen, Spielstände), Golden-Tests, Erkennung von Abstürzen, NaN-Werten, Durchdringungen und ungültigen Zuständen, Kochvalidierung, Debug-Overlays und Inspektoren (inklusive Entscheidungsanzeige für KI-Entscheidungen), Editor- und Batch-Werkzeuge.
- 📋 Jedes große Teilsystem bekommt **vor der Implementierung** eine kurze technische Spezifikation (Eingaben, Ausgaben, Abhängigkeiten, Datenmodell, Ereignisse, Speicherzustand, Tests, Leistungsbudget).

---

## 7. Modding

📋 Modding des Erwachsenen-Bereichs ist als **gleichwertige Erweiterungsschicht** geplant.

| Moddbar | Nicht normal moddbar (Sicherheitskern) |
|---|---|
| Interaktionsdefinitionen, Animations-Sätze, Positions-Voreinstellungen, Varianten-Tags, Inhaltsprofile, Übergangs-Sätze, Kamera-, Audio-, Dialogprofile, Umgebungs- und Oberflächenprofile, Kompatibilitätsprofile, neue Kategorien (mit Rollen, Intensitätsdimensionen, Animationen, Präferenz-Tags, Orten, Regeln) | Erwachsenen-Berechtigung, Zustimmungspflicht, Spieler-Opt-out, Serverautorität, Blockier-Regeln, Anfrage-Berechtigungen, Sitzungssicherheit |

Weitere Festlegungen: kleinere, stabilere öffentliche API als intern; Mods deklarieren benötigte Fähigkeiten (*Capabilities*); Zugriff nur über definierte APIs und Registries; der Mod-Manager prüft vorab Kompatibilität von Paket, Basisspiel, Spielstandschema und abhängigen Mods; Inhaltsbewertung und Altersmarkierung im Manifest.

---

## 8. Netzwerk und Mehrspieler

🧭 Der Netzwerkteil stammt überwiegend aus dem Rahmenentwurf vom 28. August und ist durch die spätere Festlegung teilweise **überholt**, dass der offizielle Online-Modus keine Mods lädt. Relevant bleibt er für die **Koop-Kampagne** (bis zu vier Spieler, Host-Speicherstand maßgeblich, siehe [CLAUDE.md](../../../CLAUDE.md)) und als Architekturgrundsatz.

- **Serverautorität** über Sitzung, Teilnehmerliste, Berechtigung, Zustimmungsprüfung, Umgebungsreservierung und Ergebnis. Clients bestimmen nur Kamera, Darstellung, Nacktheits-Einstellung, Barrierefreiheit und Oberfläche.
- **Jeder Spieler stimmt separat zu.** Keine Mehrheitsentscheidung, keine Gruppen- oder Administrator-Autorität.
- **Ablauf:** Anfrage → Prüfung → Annehmen oder Ablehnen → gegenseitige Zustandsprüfung → Sitzung → Reservierung → Assets vorbereiten → Bereitschaftsprüfung → Start.
- **Anfrage-Berechtigungen** (alle / Freunde / Vertraute / niemand), **Spam-Schutz** (Abkühlzeit, Duplikate, Ratenbegrenzung, Ignorieren, Blockieren, Auto-Ablehnung), serverseitiges Blockieren. Kein automatisches Matchmaking für solche Interaktionen.
- **Clientseitige Darstellung:** Dieselbe Sitzung kann pro Client unterschiedlich dargestellt werden (voll, reduziert, allgemeine Interaktion, Sichtschutz). Ein Client mit ausgeschalteten Erwachsenen-Inhalten empfängt keine Anfragen und lädt die Assets möglichst nicht.
- **Replikation** nur von Sitzungs-ID, Teilnehmern, Szenenzustand, Animationszustand, Übergang und Zeit, nicht von Knochen oder Kamerahistorie.
- **Missbrauchsschutz:** Blockieren von Ankern, Verschieben von Möbeln, Fahrzeug- und Physikmissbrauch, absichtliches Unterbrechen.
- **Robustheit:** Verbindungsabbruch führt zum sicheren Ausstieg; nach einem Serverabsturz ist der Charakter in einem sicheren Normalzustand, nur bestätigte Ergebnisse bleiben.
- ❓ Netzwerkdetails für Koop sind offen.

---

## 9. Einordnung im bestehenden Repository

Dieser Abschnitt ist ein **Vorschlag (🧭)** ohne Änderungsauftrag; es wurde kein Code angelegt.

- Das Repository trennt portable C++20-Logik (`Core/`) von Unreal-Modulen (`Unreal/`). Erwachsenen-Logik, falls sie je umgesetzt wird, gehörte in **eigene, vom Basis-Build trennbare Module** (portabler Regelkern für Alter, Zustimmung, Zustände; Unreal-Adapter für Animation, IK, Kamera, Audio).
- Der Speicherbereich würde sich an das dokumentierte Format der Weltfakten anlehnen ([Entscheidung 0004](../../Decisions/0004-world-facts-save-format-v1.md)): eigene Datei, gleiche stabile IDs, eigene Schema-Version. Das Format selbst wird hier nicht festgelegt.
- Vor jeder Umsetzung gilt das Master-Konsultationsprotokoll aus `CLAUDE.md` §11, und die Frage der Plattform- und Rechtslage ([Produktion und Recht](10-Produktion-Recht-Vertrieb.md)) bleibt vorgeschaltet.
