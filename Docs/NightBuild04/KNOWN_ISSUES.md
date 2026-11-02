# Known issues and limits — NightBuild 04

1. **Platzhalter-Regeln.** Schlüssel, Aufbrechen, „blockiert" sind Engineering-Platzhalter (`placeholder.*`-Fakten), kein Master-Canon (Master-Archiv war auf diesem Rechner nicht erreichbar, siehe README). Aufbrechen hat keinen Aufwand, kein Geräusch, keinen Lärm-/Werkzeug-/Fertigkeitsbezug, kein Scheitern.
2. **Schlüssel = Fakt, kein Inventarobjekt.** `placeholder.key.<id>.held`; ein Inventar-Schlüssel kann den Fakt später setzen. Der Schlüssel wird nicht verbraucht.
3. **Aufgehende Türen können den Spieler schieben.** Das Blatt schwenkt per Transform (kein Sweep). Steht der Spieler im Schwenkbereich einer *aufgehenden* Tür, wird er von der Kollision weggedrückt (im QA-Lauf an der Innentür beobachtet; der Lauf steht deshalb weiter weg). Nur das *Schließen* ist gegen Pawns gesichert.
4. **Prompt-Sprache.** Türtexte folgen `FSpnUiText` (EN/DE). Der Lichtschalter-Prompt und „… nehmen" des Schlüssels sind noch fest deutsch.
5. **Gamepad.** Quadrat/Dreieck sind per Key-Namen gemappt und von einem Test geprüft; **keine Hardware-DualSense** wurde getestet. Der Pad-Name im Prompt erscheint, sobald irgendein Gamepad angeschlossen ist (nicht erst nach der ersten Pad-Eingabe).
6. **Kein Angriff.** Die Spielfigur hat keine Angriffsaktion; „Trennung von normalem Angriff" ist daher nur als disjunkte Tastenbelegung (Test) und getrennte Aktionen umgesetzt, nicht gegen ein echtes Kampfsystem geprüft.
7. **Gebäude** aus Engine-Würfeln mit Platzhaltermaterialien, Decke geschlossen, keine Fenster (zweiter Zugang = Hintertür/Lagertür aufbrechen). Beleuchtung: Punktlichter, nicht final abgestimmt.
8. **Persistenz** nutzt das bestehende WorldFacts-v1-Format ohne Änderung; die Lage der Spielfigur wird vom QA-Test nicht mitgespeichert (dafür gibt es den Game-Flow-Pfad `player.pose`, hier nicht aktiv).
9. **Vorbestehend (unverändert):** `SpnQA.Jump.late_jump_ledge_40_run/45_jog` FAIL (bekannt, M2A2-Bericht §5); nicht implementierte/ungetestete SaveLoad-/Spawn-Einträge des V2-Levels; Warnung `EKeys::Virtual_Accept`; 17 × „Condition failed" beim Editorstart.
