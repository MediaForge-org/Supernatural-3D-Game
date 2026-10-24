# Fahrzeuge und Verkehr 05: Steuerung, Kamera und Anzeige

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist die DualSense-Belegung vom 15.10.2026 für acht Funktionen. Alles andere (Handbremse, Rückwärtsgang, Zündung, Licht, Hupe, Blinker, Kamera, Anzeige, Tastatur) ist in den Quellen nicht belegt. Dieses Modul stellt fest, was fehlt, und beschreibt, **was ein Prototyp messen muss**, ohne eine Ersatzbelegung zu setzen. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Eingabe in [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §4, Gameplay-Systeme §6.3.

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Was die Belegung nicht abdeckt](#2-was-die-belegung-nicht-abdeckt)
3. [Prüfplan für L1 + L2](#3-prüfplan-für-l1--l2)
4. [Eingabekontexte und Konflikte](#4-eingabekontexte-und-konflikte)
5. [Kamera und Anzeige](#5-kamera-und-anzeige)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Eingabe (DualSense, Entwicklervorgabe 15.10.2026) | Funktion |
|---|---|
| R2 | Gas |
| L2 | Bremse |
| L1 + L2 | analoge Kupplung (gewünschte kombinierte Geste) |
| linker Stick | Lenkung |
| rechter Stick | Kamera |
| Kreuz | Hochschalten |
| Quadrat | Herunterschalten |
| alle | frei belegbar |

Quelle: `W-FP-005`, `D15-RV`. Die Quelle enthält eine **technische Machbarkeitsanmerkung**, keine Kanonänderung: L2 ist gleichzeitig Bremse und Teil der Kupplungsgeste; im Prototyp ist zu klären, wie L1 den analogen L2-Wert modifiziert, wie die Bremse währenddessen unterdrückt oder beibehalten wird und ob die Ergonomie das gewünschte Fahrverhalten trägt. **Keine andere Belegung wird stillschweigend untergeschoben.**

Ältere Konzepte (🧭, durch die DualSense-Vorgabe präzisiert, nicht aufgehoben, `W-FP-006`): Xbox-artig mit Trigger für Gas und Bremse, A hoch, X runter, B Handbremse, Y öffnen, LB als Kupplungsmodifikator, Rückwärtsgang durch fortgesetztes Herunterschalten, Lenkrad/Pedale/H-Schalter als Enthusiastenoption.

---

## 2. Was die Belegung nicht abdeckt

| Funktion | Quelle |
|---|---|
| Handbremse | nur im älteren Xbox-Konzept (B); in der DualSense-Vorgabe nicht belegt |
| Rückwärtsgang | nur im älteren Konzept (fortgesetztes Herunterschalten); nicht in der DualSense-Vorgabe |
| Motor starten und stoppen | ‚einfache Startinteraktionen‘ früh (`W-FZ-011`), keine Taste; die Fahrzeug-App nennt Motor, Schloss, Fenster, Licht, Kofferraum und Fernstart als Funktionen (`M-DC31` §34, [Modul 06](06-Einsteigen-Aussteigen-Sitze-und-Kofferraum.md)) |
| Licht, Hupe, Blinker, Scheibenwischer | Licht ist als App-Funktion genannt; direkte Bedienung beim Fahren sowie Hupe, Blinker, Scheibenwischer nicht belegt |
| Aussteigen, Kofferraum, Fahrzeugmenü | ‚Y öffnen‘ im älteren Konzept; sonst nicht belegt |
| Radio, Telefon, Navigation | nicht belegt |
| Blick zurück, Kameraumschaltung | nicht belegt |
| Tastatur und Maus | offen (`M-OD`, [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §9) |

Das ist **kein Fehler der Quellen**, sondern ein Planungsstand: Der Entwickler hat acht Funktionen vorgegeben und alles frei belegbar genannt. Die Lücke ist als `CU-03` und `CQ-15` geführt.

---

## 3. Prüfplan für L1 + L2

> **💡 VORSCHLAG.** Der Plan nennt Messgrößen. Er legt keine Lösung fest.

Die Quelle verlangt einen Prototyp. Damit der Prototyp ein Ergebnis liefert, das der Entwickler beurteilen kann, sollte er festhalten:

1. **Signalweg:** Wie wird der analoge L2-Wert bei gehaltenem L1 der Kupplung und bei nicht gehaltenem L1 der Bremse zugeführt? Gibt es einen Zustand, in dem beide gleichzeitig wirken?
2. **Übergang:** Was passiert beim Loslassen von L1 mit einem gedrückten L2 (Kupplung wird zur Bremse)? Entsteht ein unbeabsichtigter Bremsimpuls?
3. **Ergonomie:** Lässt sich Anfahren, Schalten und Bremsen bei Kurven in typischen Situationen durchführen? Das Urteil trifft der Entwickler.
4. **Abwürgen:** Lässt sich der Greifpunkt mit der analogen Geste fein genug steuern, damit Abwürgen eine Folge des Fahrens und nicht der Eingabe ist?
5. **Alternativen als Vergleich:** Jede Alternative zur Vorgabe darf nur als Vergleichsvariante in den Prototyp, nicht als Ersatz. Sie bleibt `CQ-15` vorbehalten.

Der Test für diese Messgrößen: [Modul 31](31-Testkatalog-Phasen-und-Abnahme.md), Kategorie ST.

---

## 4. Eingabekontexte und Konflikte

Zu Fuß und im Fahrzeug sind unterschiedliche Funktionen auf denselben Tasten:

| Taste | Zu Fuß | Im Fahrzeug | Quelle |
|---|---|---|---|
| Kreuz | Springen (22.10.2026) | Hochschalten | [Proposal 0009](../../Proposals/0009-traversal-jump-and-vault-direction.md) §3a; `W-FP-005` |
| Quadrat | Kontextinteraktion beziehungsweise Angriff (Auftrag 23.10.2026, 🔍 UNBELEGT) | Herunterschalten | [Kampfsystem 06](../Kampfsystem/06-Steuerung-und-Eingabekontext.md); `W-FP-005` |

Das ist kein Widerspruch, solange Fahrzeug und Fußgänger **eigene Eingabekontexte** haben. Der Konflikt entsteht, wo beide gleichzeitig Eingaben brauchen, zum Beispiel beim **Schießen aus dem Fahrzeug** (offen im [Kampfsystem](../Kampfsystem/28-Konflikte-und-offene-Fragen.md), Frage ‚Schießen aus und auf Fahrzeuge‘). Geführt als `CK-01`.

---

## 5. Kamera und Anzeige

Beschlossen ist nur: rechter Stick = Kamera. Die Quellen sagen **nichts** zu Kameraansichten (Verfolger, Cockpit, Motorhaube), zum Kameraverhalten in Kurven oder zum Blick zurück. Ebenso nichts zu Tacho, Drehzahlmesser oder Ganganzeige. Beschlossen sind nur zwei Hooks: HUD-Realismus und Abwürgoption als Einstellungen (`W-FP-007`, 🧭).

> **💡 VORSCHLAG.** Anzeigen, die aus den Anforderungen folgen: Geschwindigkeit, Drehzahl und Gang (manuelles Getriebe, Abwürgen, Motorbremse), Kraftstoff beziehungsweise Ladung (`W-FZ-007`), Warnanzeigen für Verschleißteile (Modul 07), Navigation und Verkehrsmeldungen über Smartphone und Radio (`M-CA0905`, `W-VK-014`). Ob diegetisch oder als HUD und in welcher Wiedergabetreue, ist zu entscheiden (`CQ-17`).

Mit dem Controller verbundene Rückmeldung (zum Beispiel Trigger-Widerstand oder Vibration beim Blockieren) ist in den Quellen nicht erwähnt; sie wäre ein Vorschlag (`VEH-DEC-ST-013`).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-ST-001 | DualSense-Belegung: R2 Gas, L2 Bremse, L1 + L2 analoge Kupplung, linker Stick Lenkung, rechter Stick Kamera, Kreuz hoch, Quadrat runter | 📋 BESCHLOSSEN | `W-FP-005`; `D15-RV` | ST FZ | – |
| VEH-DEC-ST-002 | Alle Belegungen sind frei belegbar | 📋 BESCHLOSSEN | `W-FP-005` | ST | Barrierefreiheit: `CQ-18` |
| VEH-DEC-ST-003 | Die Doppelbelegung von L2 (Bremse und Kupplungsgeste) ist im Prototyp zu klären; es wird keine andere Belegung stillschweigend untergeschoben | 📋 BESCHLOSSEN | `W-FP-005`; `D15-RV` | ST FZ | Messgrößen: 💡 `VEH-DEC-ST-012` |
| VEH-DEC-ST-004 | Ältere Controller-Konzepte (Xbox-artig, LB-Modifikator, Rückwärts durch Herunterschalten) bleiben als Richtung bestehen und sind durch die DualSense-Vorgabe präzisiert, nicht aufgehoben | 🧭 RICHTUNG | `W-FP-006`; `M-PD` ‚Vehicles / racing‘ | ST | – |
| VEH-DEC-ST-005 | Handbremse, Rückwärtsgang, Motorstart, Licht, Hupe, Blinker, Aussteigen, Kofferraum und Radio sind in der DualSense-Vorgabe nicht belegt | 🔍 UNBELEGT | `D15-RV` (acht Funktionen) | ST | `CU-03`, `CQ-15` |
| VEH-DEC-ST-006 | Lenkrad, Pedale und H-Schalter sind eine Enthusiastenoption und nicht der Hauptfokus | 📋 BESCHLOSSEN | `W-FZ-006`; Gameplay-Systeme §6.3 | ST | – |
| VEH-DEC-ST-007 | Tastatur- und Mausbelegung für das Fahren sind nicht beschlossen | ❓ OFFEN | `M-OD` ‚Vehicles‘; [Welt 14](../World/14-Fahrphysik-und-Fahrzeuginteraktion.md) §4 | ST | `CQ-18` |
| VEH-DEC-ST-008 | Assist-Einstellungen, HUD-Realismus und Abwürgoption sind Einstellungs-Hooks; ein Vollassist-Zwang besteht nicht | 🧭 RICHTUNG | `W-FP-007` | ST UI | `CQ-02` |
| VEH-DEC-ST-009 | Eine Fahrzeug-App beziehungsweise ein Menü (Fernsteuerung, Anliefern per Telefon) ist vorgesehen | 📋 BESCHLOSSEN | `W-FZ-010` | FZ UI | Bedienung offen |
| VEH-DEC-ST-010 | Verkehrsmeldungen im Radio und Navigation sowie Verkehrsanzeige im Smartphone sind vorgesehen | 📋 BESCHLOSSEN | `W-VK-014`; `M-CA0905` (Wortlaut: ‚may include‘, also 🧭 für das Smartphone-Detail) | UI VKR | Umfang: `CQ-17` |
| VEH-DEC-ST-011 | Quadrat ist im Fahrzeug Herunterschalten und zu Fuß Kontextinteraktion oder Angriff; Kreuz ist Hochschalten im Fahrzeug und Springen zu Fuß | ⚠️ KONFLIKT | `W-FP-005`; Auftrag Kampfsystem (unbelegte Kampfbelegung, [Kampfsystem 28](../Kampfsystem/28-Konflikte-und-offene-Fragen.md)); Proposal 0009 §3a | ST KMP | `CK-01`; lösbar über getrennte Eingabekontexte |
| VEH-DEC-ST-012 | Der L1-L2-Prototyp misst: Signalweg, Übergang beim Loslassen von L1, Ergonomie, Greifpunkt-Feinheit; Alternativen dienen nur als Vergleichsvariante | 💡 VORSCHLAG | Ableitung aus `D15-RV` (Machbarkeitsnotiz) | ST FZ | Test `CT-ST-…` in Modul 31 |
| VEH-DEC-ST-013 | Controller-Rückmeldung (Trigger-Widerstand, Vibration bei Blockieren, Schlupf) ist in den Quellen nicht erwähnt und wäre ein Vorschlag | 💡 VORSCHLAG | – | ST | nicht priorisiert |
| VEH-DEC-ST-014 | Kameraansichten, Kameraverhalten und Blick zurück sind nicht belegt; beschlossen ist nur der rechte Stick als Kamera | 🔍 UNBELEGT | `D15-RV` | ST | `CQ-16` |
| VEH-DEC-ST-015 | Fahrzeug und Fußgänger haben getrennte Eingabekontexte, zwischen denen beim Einsteigen und Aussteigen gewechselt wird | 💡 VORSCHLAG | Ableitung aus `W-FP-005`, Proposal 0009 §3a | ST | Schießen aus dem Fahrzeug ist im Kampfsystem offen |
| VEH-DEC-ST-016 | Anzeigen für Geschwindigkeit, Drehzahl, Gang, Kraftstoff oder Ladung und Verschleißwarnungen folgen aus den Anforderungen; diegetisch oder HUD ist nicht entschieden | 💡 VORSCHLAG | Ableitung aus `W-FP-001`, `W-FZ-007`, `W-FZ-008` | UI FZ | `CQ-17` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Belegung der nicht belegten Fahrfunktionen | `CQ-15` |
| Kameraansichten und -verhalten | `CQ-16` |
| Anzeigekonzept (diegetisch/HUD) und Navigation | `CQ-17` |
| Tastatur, Maus, Lenkrad, Barrierefreiheit | `CQ-18` |
| Konflikt Quadrat/Kreuz zwischen Fahren und Kampf | `CK-01` |
