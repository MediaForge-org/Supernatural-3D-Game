# Welt 14: Fahrphysik und Fahrzeuginteraktion

> [!IMPORTANT]
> **Nichts davon ist implementiert; keine Konstante, kein Algorithmus und kein Schadensmodell ist beschlossen.** Belegt sind Richtungen, Phasenziele und die Controller-Belegung. Statuszeichen: [Legende](README.md#statuslegende). Register: [Modul 20](20-Entscheidungsregister.md) (FP).

Teil der Welt-Dokumentation ([Index](README.md)). Fahrzeuge: [Modul 13](13-Fahrzeuge-und-Fahrzeugklassen.md).

**Inhalt**

1. [Phasenvertrag](#1-phasenvertrag)
2. [Manuelles Getriebe und Kupplung](#2-manuelles-getriebe-und-kupplung)
3. [Fahrmodell](#3-fahrmodell)
4. [Eingabe und Controller](#4-eingabe-und-controller)
5. [Straße gegenüber Rennsport](#5-straße-gegenüber-rennsport)
6. [Interaktion mit dem Fahrzeug](#6-interaktion-mit-dem-fahrzeug)
7. [Schaden und Verletzung](#7-schaden-und-verletzung)
8. [Technische Hinweise (Vorschläge)](#8-technische-hinweise-vorschläge)
9. [Offene Punkte](#9-offene-punkte)

---

## 1. Phasenvertrag

📋 (D15-RV, „user-approved“):

| System | Früh | Langfristig |
|---|---|---|
| Fahrphysik | sinnvolles Schaltgetriebe mit funktionierender Kupplung | nahezu vollständige Fahrdynamik |
| Motorstart | einfach | erweiterte Startprozeduren |
| Fahrzeuginteraktion | einfacher Zugang, automatisches Einsteigen | tiefere Interaktion; Sitzwahl bleibt automatisch |
| Reparatur/Tuning/Werkstatt | einfach | nahezu vollständige Wartungs-/Werkstattsimulation |
| Getriebe | manuell verfügbar | manuell für jedes Fahrzeug plus Automatiken; optional H-Schaltung |

Keine Termine, kein Motormodell, keine Benchmarks beschlossen.

---

## 2. Manuelles Getriebe und Kupplung

- Simuliert: Kupplung, Schalten, Drehzahl, Motorbremse, Schlupf, Abwürgen (M-PD §Vehicles).
- Freie Gangwahl ist erlaubt, kein künstliches Verbot von 5→2; Folge sind Drehzahl, Motorbremse, Instabilität und je nach Realismus Verschleiß oder Schaden (M-PD). Entwickler: ‚man soll von 5–2 runterschalten können‘ (23#66E) und ein Schaltfehler bei 200 km/h schadet Kupplung und Getriebe (23#1025E Nr. 15).
- Kupplung deaktivierbar, Schalten bleibt (23#1025E Nr. 14).
- Digitale Kupplung: progressive Kurve möglich; schlechtes Gas/Timing kann trotzdem abwürgen (M-PD).
- Modi: Automatik, Manuell + Auto-Kupplung, Manuell + digitale Kupplung, volle Kupplung (M-PD).
- Abwürgen ist einstellbar (Prompt-Entwurf), nicht entschieden.

---

## 3. Fahrmodell

📋 (M-PD, M-B38): Antriebsstrang, Reifen, Federung, Lenkung, Differenzial, Bremsen/Sicherheitssysteme, Aero, Last, Schaden; Reifenzustand; Gewichtstransfer; Ladungsmasse; thermisches Modell von Kupplung/Motor/Getriebe; realistische Höchstgeschwindigkeit; Ausbrechen; kein magisches Lenken in der Luft, kein Reset durch Umdrehen; Bergen und Winde.

Wetter wirkt auf Verhalten (Regen, Schnee; M-WB17 Block 6/8); Straßenzustand und Steigung im Gebirge.

---

## 4. Eingabe und Controller

📋 DualSense (D15-RV): R2 Gas, L2 Bremse, L1 + L2 analoge Kupplung, linker Stick Lenkung, rechter Stick Kamera, Kreuz hoch, Quadrat runter; alles frei belegbar. Hinweis der Quelle: L2 ist doppelt belegt; Prototyp nötig, wie L1 den L2-Wert moduliert und die Bremse unterdrückt; **keine stille Ersatzbelegung**.

🧭 Ältere Konzepte (M-PD): RT Gas, LT Bremse, A hoch, X runter, B Handbremse, Y öffnen; LB hält Kupplungsmodus, LT wird Kupplungsachse; Rückwärts durch weiteres Runterschalten; Lenkrad/Pedale/H-Schalter als Enthusiastenoption, kein Hauptfokus. Der Entwickler fragte in 23#66E, ob LB dauerhaft gehalten werden muss, wenn LT die Kupplung ist – die Antwort darauf wurde in dieser Etappe nicht ausgewertet.

Tastatur-Belegung, Beschleunigungs-Assists und Barrierefreiheit: offen (M-OD).

---

## 5. Straße gegenüber Rennsport

Straßenfahren: relativ realistische Kupplung, nicht zu extrem; Rennsport sehr realistisch (23#1025E Nr. 50). Autorennen: realistische Physik, nicht Arcade, nicht iRacing-perfekt aber deutlich spürbar (23#514E). Rennphysik ist simulationslastiger als Straßenphysik (M-WB17 Block 5). Details: [Modul 15](15-Motorsport-und-Rennaktivitaeten.md).

---

## 6. Interaktion mit dem Fahrzeug

Zugang, Einsteigen automatisch, Starten, Kofferraum als Ausrüstungsbasis, Fahrzeug-App (Telefon), Abschleppen, Reparatur. Die Interaktionsarchitektur ist **nicht** spezifiziert (MasterIndex: keine universelle Interaktions-Spezifikation); Einzelinteraktionen existieren nur als Prompt-Entwürfe (`012_create-engine-restart-interaction`). Repository: `Docs/Proposals/0004-interaction-and-persistent-door.md`.

---

## 7. Schaden und Verletzung

Crash-Energie und Sicherheitsausrüstung kommen mit dem Verletzungssystem zusammen (M-PD). **Der Detailgrad von Verformung, Antriebs-/Reifenfehlern, Reparaturökonomie und bleibendem Verschleiß blieb unbeantwortet** (D15-RV, D15-OQ Nr. 2).

---

## 8. Technische Hinweise (Vorschläge)

💡 Aus den Prompt-Entwürfen (alle `DRAFT_REWRITE_REQUIRED`): Fahrer-Input-Abstraktion, Eingabekontext-Wechsel zu Fuß/Fahrzeug, Streaming-Quellenpolitik für Spieler und Fahrzeuge, Debug-Ansicht des Antriebsstrangs, Wiedergabe-Tests für Fahrzeugeingaben, Reisezeit-Benchmark. Sie sind Themenlisten, keine Anforderungen.

---

## 9. Offene Punkte

Schadensmodell; Motor-/Getriebe-/Kupplungs-Konstanten; Schleifpunkt-Algorithmus; Verkehrsmodell; UI; Assist-Stufen; Mehrspieler-Synchronisation; L1+L2-Ergonomie; Tastaturbelegung.
