# Adult 04: Steuerung, Darstellung, Kamera, Audio und Replay

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Planungsstand zu Eingabe, Oberfläche, Darstellungsprofilen, Kamera, Audio und Aufnahme. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Die Spielerfunktionen für Darstellung und Barrierefreiheit gelten zusätzlich zu den Sicherheitsregeln in [Modul 01](01-Grundregeln-Alter-Zustimmung.md).

**Inhalt**

1. [Spieler-Einstellungen](#1-spieler-einstellungen)
2. [Darstellungsprofile und Erzählung](#2-darstellungsprofile-und-erzählung)
3. [Steuerung](#3-steuerung)
4. [Oberfläche und Aktionsbibliothek](#4-oberfläche-und-aktionsbibliothek)
5. [VR](#5-vr)
6. [Kamera](#6-kamera)
7. [Audio und Stimmen](#7-audio-und-stimmen)
8. [Barrierefreiheit](#8-barrierefreiheit)
9. [Replay, Aufnahme und Streaming](#9-replay-aufnahme-und-streaming)
10. [Statistik](#10-statistik)

---

## 1. Spieler-Einstellungen

🧭 Entwurf aus dem Rahmen vom 28. August, durch spätere Entscheidungen angepasst (kein globaler Intensitätsschalter, siehe [Modul 02](02-Architektur-Pakete-Technik.md#3-paketstruktur)):

| Einstellung | Stufen |
|---|---|
| Erwachsenen-Inhalt | aus / nur Romantik / an |
| Inhaltsprofil | Standard / Fortgeschritten / Hochintensität / alle unterstützten (nur in erweiterten Einstellungen) |
| Nacktheit | aus / reduziert / voll (optional getrennt nach Spielerfigur und NPC, Ober- und Unterkörper) |
| Darstellung | Schwarzblende / verdeckt / reduziert / voll |
| Audio, Dialog | aus / reduziert / normal |
| Kamera | filmisch an/aus; Spielerkamera eingeschränkt oder frei |
| Anfragen (Mehrspieler) | Romantik und Erwachsenen-Inhalt jeweils: alle / Freunde / Vertraute / niemand |

Beim ersten Start des Pakets kann ein **Einführungsdialog** Vorlieben abfragen. Werden Fragen übersprungen, gelten Standardwerte (Entwickler, 13.09.: eine Befragung beim ersten Start, sonst Standards). Das UI-Ausmaß soll einstellbar sein.

Die Werte-Ansicht verborgener Zustände ist nur in Mod-, Debug- und Cheat-Modus vorgesehen ([Modul 02](02-Architektur-Pakete-Technik.md#4-builds)).

---

## 2. Darstellungsprofile und Erzählung

- 📋 **Basisspiel:** einheitlich auf PC, PlayStation und Xbox, ungefähr im Rahmen heutiger großer Rollenspiele. Nacktheit konfigurierbar (voll, reduziert, aus). Intimität wird **angedeutet** durch Blocking, Kamera, Animation, Audio und Reaktionen, ohne sichtbare explizite Anatomie. Assets, Kameras und IK werden so gebaut, dass Kamera- oder Sichtfeldfehler nichts zeigen, was das Profil nicht vorsieht.
- 📋 **Erweiterte Darstellung** ist Sache des optionalen Pakets ([Modul 02](02-Architektur-Pakete-Technik.md#2-bereitstellung-basisspiel-und-optionales-paket)).
- 📋 **Darstellungsmodi:** deaktiviert, nur Romantik, Schwarzblende, angedeutet, filmisch verdeckt, reduziert, voll erlaubt (letzteres nur mit Paket).
- 📋 Die **Erzählung ist unabhängig von der Darstellung.** Ein Spieler mit Schwarzblende erlebt dieselben Folgen wie einer mit voller Darstellung.
- 📋 Intimität besteht aus reaktiven Zuständen, nicht aus festen Animationsschleifen (Haltung, Abstand, Tempo, Pausen, Zustand).
- 🧭 Der Entwickler will, dass es „echt wirkt“ und nicht wie ein Film mit Schnitt; die HUD-Hilfen dürfen dafür nur zurückhaltend sein (siehe [Abschnitt 4](#4-oberfläche-und-aktionsbibliothek)).
- Verletzungs-/Gore-Darstellung im Hauptspiel ist ein getrenntes Thema (Profile Voll/Reduziert/Minimal), nicht Teil dieses Dokuments.

---

## 3. Steuerung

📋 Planungsstand aus Runde 18 (26.09.), vom Entwickler als Richtung bestätigt, Details offen:

- **Hybrid-Steuerung:** *Direkt* (freiere Körpersteuerung) und *Assistiert* (erweiterte Aktionen und Kombinationen, angelehnt an das Prinzip von Kampfspielen mit Kombinations-Menü). Der Wechsel soll ohne Unterbrechung möglich sein. ❓ Ausgestaltung offen. Der Entwickler: Er finde „das UFC-System etwas erweitert extrem stark“, wisse aber noch nicht genau, wie „direkt“ aussieht.
- **Kombinationen sind Abkürzungen.** Freie Bewegung ist nie auf gespeicherte Kombinationen beschränkt.
- **Analoge Steuerung:** Tempo und Richtung über Stick und Trigger; **kein Tasten-Spammen** („Spammen wäre zu GTA“). Ein zusätzlicher Modifikator verändert Stärke oder Tempo einer laufenden Bewegung. Intensität und Tempo sind zu jeder Sekunde änderbar.
- **Schnelle Wahl des gesteuerten Körperbereichs**, falls mehrere Aktionen gleichzeitig laufen, mit **Smart Context** (das Spiel errät die gemeinte Hand/Region, der Spieler kann übersteuern) und einer Anzeige laufender Aktionen.
- **Lernsystem:** Bestimmte Aktionen sind erst nach dem Erlernen freigeschaltet (Entwickler, vor allem für Kampf präzisiert: Grundaktionen frei, anspruchsvolle erst nach Training; sie können beigebracht werden). Ob und wie das für Intimität gilt, ist ❓ offen. Das Lern- und Trainingssystem für Kampf gehört zu einem allgemeinen Kampfblock, nicht zum Adult-System.
- **Freie Tastenbelegung** mit Konfliktwarnung; speicherbare Kontrollprofile (Controller direkt, Controller assistiert, Tastatur, VR). Controller-Haptik entsteht aus echtem Kontakt und ist einstellbar oder abschaltbar.
- **Barrierefreiheits-Hilfen:** Halten statt Drücken, längere Eingabefenster, größere Oberfläche.
- Teleportieren gibt es nicht; harte Zurücksetzungen sollen so gut wie nie nötig sein (Entwickler, 13.09.).

---

## 4. Oberfläche und Aktionsbibliothek

- 📋 **Aktionsbibliothek:** Ein durchsuchbares Menü zeigt alle aktuell möglichen Aktionen mit Beschreibung, Tastenbelegung, Voraussetzungen und Vorschau, gefiltert nach Pose, freien Händen, Items, Umgebung und Körperkonfiguration. Favoriten und Schnellzugriff (vorläufig bestätigt, 20.09.). Außerhalb einer Situation ist die ganze Bibliothek einsehbar.
- 📋 Die **Vorschau** zeigt ungefähr das Ergebnis, aber **kein starres Animationsversprechen**.
- 📋 **HUD:** vollständig modular; Stufen aus / minimal / standard / detailliert / Entwickler. Minimal zeigt nur wichtige Hinweise und soll filmisch wirken.
- 📋 Kamera ist frei und von normalem Gameplay getrennt; filmische Ansicht optional.

---

## 5. VR

📋 Richtung (26.09.): VR nutzt **denselben Kern** wie Controller und Maus/Tastatur: dieselben Aktionen, Physik und Körperzustände, nur andere Eingabe. Handtracking und Fingertracking; Ganzkörper-Tracking **optional**; IK ergänzt fehlende Tracker; die Aktionsbibliothek als räumliches Overlay (ausblendbar); Handschuh-Haptik später und nie Voraussetzung. VR kommt **später**, nachdem der normale Interaktionskern stabil ist. Der Entwickler wünscht, dass es sich in Bewegung und Wirkung „wie echt“ anfühlt.

---

## 6. Kamera

📋 Ein **Kamera-Regisseur** (Gameplay, filmisch, nah, mittel, weit, Umgebung, verdeckt, vom Spieler gesteuert, Blende) mit Beschränkungen: keine Wand-, Möbel- oder Figurenüberschneidung, Respekt vor Darstellungs- und Barrierefreiheits-Einstellung, gültiges Framing. Wechsel zwischen Third Person, näherer Third Person, First Person (stabilisiert gegen Übelkeit) und filmischer Ansicht, soweit die Situation es erlaubt. Der Entwickler lehnt eine „Filmschnitt“-Optik ab; es soll echt wirken, mit mehr Tasten und HUD.

---

## 7. Audio und Stimmen

- 📋 Audio-Schichten: Dialog, kurze Reaktionen, Atmung, Bewegung, Kleidung, Möbel, Umgebung, Raumton, Wetter. Der Raum entscheidet mit, ob jemand bewusst leiser ist (Wände, Nachbarn). Ausdrucksstärke ist stark persönlich (Entwickler: manche leiser, manche laut).
- 📋 Atem- und Stimmaudio entstehen **prozedural** aus Belastung und Persönlichkeit; jede Person klingt anders; es gibt keine fünf Standardsounds. Mimik und Blickkontakt kommen aus den bestehenden Emotionssystemen.
- 📋 Stimm-Pakete sind unabhängig von Animationspaketen; Mehrsprachigkeit soll als eigene Pakete möglich sein.
- ❓ Besetzung und Produktionsweg der Stimmen, Vertragsfragen (insbesondere bei synthetischen Stimmen): offen. Allgemeine Stimmenpolitik: siehe [Spielvision](../Spielvision.md) und [Entscheidungsstatus](../Entscheidungsstatus.md).

---

## 8. Barrierefreiheit

📋 Untertitel, Überspringen, Stopp, Schwarzblende, reduzierte Darstellung, abschaltbare Kamera- und Bewegungsunschärfe-Effekte, Kamerasteuerung, Audioregelung, vereinfachte Eingaben. Das Stopp-Signal ist immer erreichbar ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#6-stopp-unbehagen-und-widerruf)).

---

## 9. Replay, Aufnahme und Streaming

Teile davon sind **allgemein** (alle Spielszenen) und nicht Adult-spezifisch. 📋

- **Replay auf Basis des gespeicherten Szenenzustands**, nicht nur als Video: Kamera, Sichtfeld, Tiefenschärfe und Zeitlupe lassen sich nachträglich wählen; ein eingeblendetes HUD wird im Replay neu gerendert, damit das Ergebnis filmisch aussieht.
- **Export** in lokale Dateien in hohen Auflösungen (bis 4K/8K) und Qualitätsstufen mit sehr hoher Bitrate, auf Wunsch kleiner. Der **Speicherverbrauch** wird angezeigt. Aufnahmen gehören dem Nutzer.
- Das Rendern läuft **im Hintergrund** mit sichtbarem Fortschritt; Weiterspielen ohne große Leistungseinbuße. Für große Aufnahmen kann das Spiel eingeschränkt spielbar sein (Entwickler: nur bei kleinen Clips sinnvoll).
- **Direktes Streaming/Aufnehmen** ohne Zusatzprogramm, unabhängig vom Erwachsenen-Bereich, mit Anbindung an gängige Plattformen. Streamer-Filter und Namen der Nacktheitsstufen sind ❓ offen.
- Replays sind reine Aufnahmen und brauchen keine Spiellogik (Entwickler, 14.09.).

---

## 10. Statistik

Der Entwickler wünscht ein **globales Lifetime-Statistiksystem** (hunderte bis tausende Werte, Kategorien, Zeiträume, Rekorde, Graphen), das **unabhängig vom Erwachsenen-Bereich** arbeitet und über Ereignisse gespeist wird (15.09.). Adult-bezogene Zähler wären dort nur Einträge unter vielen und dürfen nach der Grundregel **nicht das Verhalten von NPCs steuern** ([Modul 05](05-NPCs-Beziehungen-Praeferenzen.md)). Die eigene Planungsrunde dazu steht aus ([Gameplay-Systeme](../Gameplay-Systeme.md)).
