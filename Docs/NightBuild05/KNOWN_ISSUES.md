# Known issues and limits – NightBuild 05

Stand: wird mit den Testergebnissen abgeglichen (`TEST_RESULTS.md`).

1. **Alles Platzhalter-Gameplay, kein Canon.** Master-Konsultation war nicht möglich (`Planning/Master` fehlt auf diesem Rechner). Hörweiten, Wandfaktor, Zeiten, Wachsamkeitsgewichte, Alarmfenster, Aufbrechdauer und Geräuschwerte sind Engineering-Platzhalter (`IMPLEMENTATION_REPORT.md`, „Vorläufige Regeln“). Kein Regelwerk wurde als verbindlich beschlossen.
2. **NPC = Engine-Formen.** Zylinder + Kugel mit Zustandsfarbe und schwebendem Debug-Label; keine Animation, keine Gesichter, kein Dialog. Das Label ist eine Entwicklungsvisualisierung, keine Spiel-UI.
3. **Keine Positionen im Save.** NPC-Positionen und -Routinen sind flüchtig; nach dem Laden beginnt jeder NPC an seinem Level-Startpunkt (nur Erinnerungen und Weltfakten werden wiederhergestellt). Das ist beabsichtigt (Unterscheidung dauerhaft/flüchtig), aber für ein fertiges Spiel nicht ausreichend.
4. **Alarm nach dem Laden ist eine Ableitung.** Wer innerhalb von 120 Spielsekunden vor dem Speichern Alarm hatte, beginnt mit Investigate (zum letzten Vorfallsort), nicht mit Alert/Flee. Der echte Alarmzustand wird nicht rekonstruiert.
5. **Kein Fraktions-/Nemesis-/Schedule-System.** Bewusst nicht gebaut. Der NPC hat nur eine Patrouille zwischen Punkten.
6. **Spieler-Schritte erzeugen kein Geräusch.** `Footstep` existiert als Art und ist getestet, aber die Spielfigur sendet noch keine Schritt-Geräusche (Sprinten/Schleichen wären der nächste Schritt).
7. **Geräusch-Ort ist exakt.** Gehörte Geräusche merken sich die genaue Position; eine Ortsunschärfe nach Genauigkeit/Entfernung ist noch nicht modelliert (die Genauigkeit skaliert nur den Einfluss der Erinnerung).
8. **Flucht ist einfach.** Fluchtziel = der von der Bedrohung am weitesten entfernte Punkt aus Routinepunkten und zufälligen erreichbaren NavMesh-Punkten; eingekesselt zu sein ist nicht behandelt (Flee endet nur nach Mindestzeit und Abstand/Sichtverlust).
9. **Türen:** NPCs öffnen keine Türen. Ein Weg über eine geschlossene Tür existiert für sie nicht (im Testlevel ist die Lagertür deshalb offen).
10. **NavMesh wird zur Laufzeit gebaut.** Es gibt kein gebackenes Mesh; nach dem Levelstart braucht es kurz (die QA wartet 2,5 s). Ohne Mesh fallen NPCs auf Geradeaus-Steuerung zurück.
11. **Gamepad/DualSense:** Dreieck hält die Aufbrechaktion (Events `Started`/`Completed` der Enhanced-Input-Aktion); mit echter Hardware nicht getestet (nur Tastatur-Events im QA-Lauf).
12. **Vorbestehend (unverändert):** `SpnQA.Jump.late_jump_ledge_40_run/45_jog` (bekannte Sprungfehler), nicht implementierte/ungetestete Einträge der V2-Level-QA, Warnung `EKeys::Virtual_Accept`, „Condition failed“-Zeilen beim Editorstart.
13. **`SpnQA.HouseJourney` lädt das Level nicht wirklich neu** (Night-Build-04-Test, siehe README „Bekannte Korrektur“). Der Test prüft Zustandswiederherstellung, nicht frische Actors. `SpnQA.NpcReaction` tut es richtig. Die Korrektur von `HouseJourney` ist nicht Teil dieses Meilensteins.
14. **Kein Mensch hat gespielt.** Alles Gerenderte lief automatisiert mit echten Tastenereignissen; es gab keinen manuellen Playtest und keinen Test mit Hardware-Gamepad.
