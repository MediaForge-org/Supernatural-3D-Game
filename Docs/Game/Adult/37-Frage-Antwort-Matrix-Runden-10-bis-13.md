# Adult 37: Frage-Antwort-Matrix, Runden 10 bis 13 (inkl. Körperreaktions-Block)

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dies ist ein **Quellenprotokoll**, keine Spezifikation und kein Kanon. Es verzeichnet je Interviewfrage die (gekürzte) Frage, die Antwort des Entwicklers, die Fundstelle, einen **heuristischen** Status und den Registerbezug. Explizite Formulierungen der Quellen sind **automatisch abstrahiert**; der Wortlaut steht nur in der Primärquelle. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Methode, Grenzen und Gesamtzahlen: [Modul 41](41-Audit-Etappe-2-2.md). Quellenschlüssel: [Modul 13](13-Quelleninventar-und-Abdeckung.md#1-zitierschlüssel). Entscheidungen: [Register (Modul 14)](14-Entscheidungsregister.md). Weitere Matrizen: [36](36-Frage-Antwort-Matrix-Runden-1-bis-9.md), [37](37-Frage-Antwort-Matrix-Runden-10-bis-13.md), [38](38-Frage-Antwort-Matrix-Runden-14-und-15.md), [39](39-Frage-Antwort-Matrix-Runde-16.md), [40](40-Frage-Antwort-Matrix-Runden-17-bis-20.md).

## Lesehilfe

| Spalte | Bedeutung |
|---|---|
| **Nr.** | Fragenummer **innerhalb des Pakets** (Paket = eine Fragenliste des Assistenten; Nummern wiederholen sich zwischen Paketen). |
| **Frage** | erste ca. 120 Zeichen der Frage; bei expliziter Formulierung durch einen Hinweis ersetzt. |
| **Antwort** | Antwort des Entwicklers, auf ca. 130 Zeichen gekürzt. „Bereichsantworten“ (z. B. „1-9 ja, …“) gelten für den ganzen Bereich; dieselbe Antwort erscheint dann mehrfach. `A → B` = spätere Nachreichung. `–` = in den Nachrichten keine Antwort gefunden. |
| **Quelle** | `35#Frage-Nachricht → 35#Antwort-Nachricht`. |
| **Status** | **heuristisch** aus dem Antworttext abgeleitet (siehe unten), nie eine Entscheidung dieser Dokumentation. |
| **Register** | Register-Einträge, deren Text diese Frage **ausdrücklich nennt** (Fragenummer oder Bereich in Aussage/Vermerk) und deren Quelle die Antwortnachricht ist. `–` heißt: **kein Eintrag nennt diese Frage namentlich**. Das bedeutet **nicht**, dass die Aussage fehlt; Paketebene steht im Kopf jedes Abschnitts. |

**Status-Werte:** `📋 ja` / `📋 ja mit Zusatz` / `📋 Freitext` = Zustimmung bzw. inhaltliche Antwort des Entwicklers, **nicht** automatisch zu Zahlen oder Details, die der Assistent als Beispiel nannte. `📋 Rückfrage → pauschal bestätigt` = Rückfrage des Entwicklers, danach ein knappes „Ja/Ok passt“ direkt nach der Erklärung. `🧭 bedingt/unverbindlich` = „kann sein“ o. ä. `🧭 delegiert` = „mach, was du gut findest“. `❓ offen/Rückfrage` = Rückfrage ohne belegte Bestätigung. `❌ nein`. `⚪ ohne Antwort`. Zusätze nach `·` verweisen auf Konflikte ([Modul 19](19-Konflikt-und-Widerspruchsmatrix.md)) oder Register-Einträge.

**Module:** 01 [Grundregeln](01-Grundregeln-Alter-Zustimmung.md) · 02 [Architektur](02-Architektur-Pakete-Technik.md) · 03 [Interaktion](03-Interaktion-Taxonomie-Ablauf.md) · 04 [Steuerung](04-Steuerung-Darstellung-Kamera-Audio.md) · 05 [NPCs](05-NPCs-Beziehungen-Praeferenzen.md) · 06 [Körper](06-Koerper-Physik-Gesundheit.md) · 07 [Charakteranpassung](07-Charakteranpassung-Tattoos-Koerpermodifikation.md) · 08 [Welt](08-Welt-Orte-Branche-Wirtschaft.md) · 10 [Produktion/Recht](10-Produktion-Recht-Vertrieb.md). Die Modulzuordnung gilt **auf Paketebene** (heuristisch, nicht je Frage belegt).

**Inhalt**

- [Runde 10A: Körperprofil (35#280)](#runde-10a-körperprofil-35280)
- [Runde 10B: Skelett, Kontakt (35#282)](#runde-10b-skelett-kontakt-35282)
- [High-Fidelity Body Response 1–20 (35#288)](#high-fidelity-body-response-120-35288)
- [High-Fidelity Body Response 21–40 (35#290)](#high-fidelity-body-response-2140-35290)
- [Runde 10: Szenensystem (35#443)](#runde-10-szenensystem-35443)
- [Runde 11: Freeform-Engine (35#457)](#runde-11-freeform-engine-35457)
- [Runde 11B: Ergänzungen (35#463)](#runde-11b-ergänzungen-35463)
- [Runde 12: Replay, Aufnahme, Streaming (35#483)](#runde-12-replay-aufnahme-streaming-35483)
- [Runde 13: Ablauf, Reaktionen (35#485)](#runde-13-ablauf-reaktionen-35485)

---


## Runde 10A: Körperprofil (35#280)

**Datum:** 2026-09-11 · **Fragen:** 1–35 (35) · **Antwortnachrichten:** 35#281 · **Module (Paketebene):** 06, 05 · **Status:** ❓ 2, 📋 33  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-001, E-KO-002, E-KO-003, E-KO-004, E-KO-005, E-SO-001, E-TA-001

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jeder erwachsene NPC einen persistenten Body Profile Seed besitzen? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 2 | Soll das sichtbare Modell aus realen Körperparametern statt nur wenigen Presets entstehen? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 3 | Sollen Körpergrößen kontinuierlich variieren statt z. B. nur Small/Medium/Large? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 4 | Soll tatsächliche Körpergröße in cm gespeichert/ableitbar sein? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 5 | Soll Arm-, Bein-, Torso- und Schulterlänge teilweise unabhängig variieren? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 6 | Sollen Schulterbreite, Hüftbreite und allgemeine Proportionen getrennte Parameter sein? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 7 | Soll Körpergewicht bzw. Körpermasse ein eigener Simulationswert sein? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 8 | Soll Körperfettverteilung teilweise unabhängig vom Gesamtgewicht modelliert werden? | ja, aber auch einzelne körperteile müssen bei der gleichen person nicht gleich sein, zb ein bein ,5cm länger und so weiter. jeder … | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 9 | Soll Muskelmasse ebenfalls separat sein? | ja sicher | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 10 | Soll Fitness/Aktivität den Körper langfristig verändern können? | ja, man soll krafttraining und so weiter machen können um muskeln aufzubauen, dachte aber eher das das nichts mit adult zu tun hat | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 11 | Soll Gewicht über die Zeit zunehmen oder abnehmen können? | ja, man soll krafttraining und so weiter machen können um muskeln aufzubauen, dachte aber eher das das nichts mit adult zu tun hat | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 12 | Soll Verletzung/Krankheit Körperhaltung bzw. sichtbaren Zustand beeinflussen können? | ja, man soll krafttraining und so weiter machen können um muskeln aufzubauen, dachte aber eher das das nichts mit adult zu tun hat | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 13 | Soll Alterung Körperform und Haut subtil verändern? | ja, man soll krafttraining und so weiter machen können um muskeln aufzubauen, dachte aber eher das das nichts mit adult zu tun hat | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001 |
| 14 | Soll Schwangerschaft/Familienplanung später nur über ein separates allgemeines Life-Simulation-System laufen und nicht … | weiß noch nicht | 35#280 → 35#281 | ❓ offen/Rückfrage | E-KO-001 |
| 15 | Sollen Narben persistent und individuell sein? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001, E-KO-002 |
| 16 | Tattoos ebenfalls? | ja aufjedenfall ich will ja auch viele npcs die viele tattoos haben und so weiter | 35#280 → 35#281 | 📋 ja mit Zusatz | E-KO-001, E-KO-002 |
| 17 | Piercings ebenfalls? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001, E-KO-002 |
| 18 | Soll Körperbehaarung Character-/Style-abhängig sein? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 19 | Soll Grooming über die Zeit verändert werden können? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 20 | Soll Hautfarbe/Unterton separat vom Material selbst definiert werden? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 21 | Soll Haut Feuchtigkeit, Schmutz, Schweiß, Blut, Regen usw. als dynamische Material-Layer unterstützen? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 22 | Soll das System regionale/genetische Unterschiede zulassen, ohne NPCs auf starre Körper-Archetypen zu reduzieren? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 23 | Soll Kleidung Körperform beeinflussen können, z. B. Kompression oder Silhouette? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 24 | Soll dieselbe Kleidung automatisch auf unterschiedliche Körper angepasst werden können? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 25 | Sollen wichtige Characters dafür hochwertigere handkorrigierte Clothing-Fits bekommen können? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 26 | Soll Haarstil unabhängig vom Character-Basismodell austauschbar sein? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 27 | Soll Haarlänge teilweise dynamisch veränderbar sein? | haare und bart wachsen mit der zeit und man kann nicht einfach von buzzcut auf dauerwelle gehen ohne die haare eine zeit lang … | 35#280 → 35#281 | 📋 Freitext | E-KO-001, E-KO-003 |
| 28 | Soll es Frisör-/Style-Änderungen im normalen Hauptspiel geben, die Adult-Mods einfach übernehmen? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 29 | Soll Make-up als separates Layer-System existieren? | ja schon | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 30 | Soll Make-up mit Regen, Wasser, Zeit usw. reagieren können, wenn das jeweilige Produkt/Material dies vorsieht? | keine ahnung wie das in echt ist | 35#280 → 35#281 | ❓ offen/Rückfrage | E-KO-001 |
| 31 | Soll ein NPC seinen Style im Laufe der Jahre verändern können? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 32 | Soll Beruf/Region/Einkommen nur Biases für Style erzeugen und keine festen Regeln? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001 |
| 33 | Soll der Player physische Preferences für andere Characters haben können, ohne dass diese Preferences die tatsächliche … | ja | 35#280 → 35#281 | 📋 ja | E-KO-001, E-KO-005 |
| 34 | Soll Attraction also auf dem tatsächlich generierten Körper/Gesicht berechnet werden und nicht umgekehrt? | ja | 35#280 → 35#281 | 📋 ja | E-KO-001, E-KO-005 |
| 35 | Soll das Body-System grundsätzlich unabhängig vom Adult-Mod existieren, damit das Hauptspiel dieselben hochwertigen … | ja | 35#280 → 35#281 | 📋 ja | E-KO-001, E-KO-005 |

## Runde 10B: Skelett, Kontakt (35#282)

**Datum:** 2026-09-11 · **Fragen:** 36–70 (35) · **Antwortnachrichten:** 35#283 · **Module (Paketebene):** 03, 06 · **Status:** ❓ 1, 📋 34  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-005, E-KO-006, E-LE-004, E-PA-025, E-SO-002

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 36 | Soll das Skeleton selbst weitgehend standardisiert bleiben, während unterschiedliche Körper über Bone-Proportionen, … | ? | 35#282 → 35#283 | ❓ offen/Rückfrage | – |
| 37 | Soll es mehrere Skeleton Families geben, z. B. Standard Human, sehr große/kleine Human-Varianten, besondere Humanoids … | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 38 | Soll jeder Character einen eigenen Measurement Cache besitzen, der echte Positionen/Längen seiner Körperteile enthält? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 39 | Soll dieser Cache auch linke und rechte Seite getrennt speichern, damit Asymmetrien berücksichtigt werden? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 40 | Soll eine leicht unterschiedliche Beinlänge tatsächlich Gang, Stand und IK minimal beeinflussen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 41 | Soll die normale Locomotion diese Asymmetrie subtil kompensieren können, statt dass der Character sichtbar humpelt, … | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 42 | Soll größere Asymmetrie dagegen tatsächlich sichtbare Auswirkungen haben können? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 43 | Soll Haltung ebenfalls individuell sein, z. B. leichte Beckenneigung, Schulterhaltung, Rundrücken, aufrechte Haltung? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 44 | Soll Haltung durch Beruf, Training, Verletzungen und Alter verändert werden können? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 45 | Soll Muscle/Fat-Deformation poseabhängig sein, also Muskeln/Weichteile je nach Bewegung unterschiedlich aussehen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 46 | Soll es Corrective Morphs für schwierige Gelenkstellungen geben, z. B. Schulter, Hüfte, Knie, Ellbogen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 47 | Soll es auch character-spezifische Correctives für Hero-Characters geben? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 48 | Soll Body Contact grundsätzlich anhand echter gemessener Contact Anchors des jeweiligen Characters erfolgen statt … | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 49 | Soll ein Contact Anchor z. B. automatisch aus Körpermaßen berechnet werden können? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 50 | Soll ein Artist/Modder diese automatisch berechneten Anchors manuell überschreiben dürfen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 51 | Soll jeder Contact Anchor eigene Toleranz besitzen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 52 | Soll Primary Contact strenger sein als Secondary Contact? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 53 | Soll das System bei zu großer anatomischer Abweichung automatisch eine passendere Animation/Variante auswählen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 54 | Wenn keine passende Variante existiert: lieber sanft auf eine kompatible Alternative wechseln als sichtbares Clipping … | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 55 | Soll Hero-Content trotzdem handgebaute perfekte Animationen für bestimmte Character-Paare unterstützen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 56 | Soll Runtime Retargeting hauptsächlich für generische NPC-Kombinationen genutzt werden? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 57 | Soll ein Character mehrere Retarget Profiles besitzen können, falls verschiedene Animation Packs unterschiedliche … | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 58 | Soll der Mod Manager/Validator erkennen, wenn ein Animation Pack mit einem Character-Rig nicht kompatibel ist? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 59 | Soll der Mod Editor eine Live-Vorschau mit zwei oder mehreren beliebigen Characters ermöglichen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 60 | Soll diese Vorschau direkt Contact-Stress/Clipping-Zonen farbig anzeigen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 61 | Soll ein automatischer Test hunderte Körperkombinationen gegen dieselbe Animation laufen lassen können? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 62 | Soll der Validator daraus sagen können: „Diese Animation funktioniert für 94 % der getesteten Körperprofile“? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 63 | Soll ein Modder Compatibility-Grenzen definieren können, z. B. maximale Größen-/Proportionsdifferenz? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 64 | Soll das System innerhalb dieser Grenzen automatisch Korrekturen durchführen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 65 | Soll extreme Größenunterschiede eher spezielle Animationen benötigen statt alles mit IK zu erzwingen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 66 | Soll Kleidung ebenfalls in Contact-/Collision-Berechnungen berücksichtigt werden, wenn sie physisch relevant ist? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 67 | Soll lose Kleidung während enger Interaktionen auf einen vereinfachten Physics-Modus wechseln können, um Chaos zu … | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 68 | Soll Hair Physics bei sehr nahen Character-Interactions eigene Collision-/LOD-Regeln bekommen? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 69 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Skelett, Kontakt] | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist | 35#282 → 35#283 | 📋 ja mit Zusatz | – |
| 70 | Soll die Grundregel sein: Animation bestimmt die Bewegung, IK korrigiert Kontakt, Physics ergänzt nur Secondary Motion? | ja passt, aber es ist wichtig dass jede person nicht nur aus 3 elementen besteht sondern realistisch nachgebaut ist jetzt etwas … | 35#282 → 35#283 | 📋 ja mit Zusatz | – |

## High-Fidelity Body Response 1–20 (35#288)

**Datum:** 2026-09-12 · **Fragen:** 1–20 (20) · **Antwortnachrichten:** 35#289 · **Module (Paketebene):** 06 · **Status:** 📋 20  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-007, E-KO-008, E-SO-003

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jeder größere Muskel eine approximierte Aktivierung bekommen, abhängig von Pose, Kraft und Bewegung? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 2 | Soll Muskelanspannung sichtbar die Oberflächenform verändern? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 3 | Soll Weichteilgewebe durch Beschleunigung, Richtung und Impuls reagieren statt über vorgebackenes Wackeln? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 4 | Soll Masse pro Körperregion individuell sein, sodass zwei Characters bei derselben Bewegung unterschiedlich reagieren? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 5 | Soll das System Dämpfung, Elastizität und Rückstellgeschwindigkeit pro Region besitzen? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 6 | Soll Haut über Muskeln/Weichteilen gleiten und komprimiert werden können? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 7 | Soll sichtbare Kompression bei Kontakt entstehen? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 8 | Soll nach Kontakt eine kurze realistische Nachbewegung vorhanden sein? | ja | 35#288 → 35#289 | 📋 ja | E-SO-003 |
| 9 | Soll das alles subtil bleiben und standardmäßig eher realistisch als übertrieben sein? | realistisch muss es sein | 35#288 → 35#289 | 📋 Freitext | – |
| 10 | Soll ein Mod trotzdem alternative Physics-Presets wie Realistic / Soft / Exaggerated / Custom liefern dürfen? | ist mir egal, mods können alles hinzufügen | 35#288 → 35#289 | 📋 Freitext | – |
| 11 | Soll Bewegung asymmetrisch sein, weil reale Körper ebenfalls asymmetrisch sind? | ja | 35#288 → 35#289 | 📋 ja | – |
| 12 | Soll Muskelermüdung Bewegungsqualität und Körperspannung beeinflussen? | ja | 35#288 → 35#289 | 📋 ja | – |
| 13 | Soll Atmung Brustkorb, Bauch und Haltung kontinuierlich beeinflussen? | ja aber nicht so wichtig das man das sieht, jedenfalls anfangs | 35#288 → 35#289 | 📋 ja mit Zusatz | – |
| 14 | Soll Herzschlag nur in extremen Nahaufnahmen subtil sichtbar/fühlbar sein? | wie soll man das hören? Meinst du wenn man mit dem ohr auf die brust geht? Beim eigenen charakter würde ich in extremen … | 35#288 → 35#289 | 📋 Freitext | – |
| 15 | Soll Schweiß Hautshader und Reibungsparameter beeinflussen? | ja | 35#288 → 35#289 | 📋 ja | – |
| 16 | Soll Kleidung Druckstellen und Kompression auf dem Körper erzeugen? | ja aber das ist jetzt kein muss Feature als erstes sind andere sachen wichtiger | 35#288 → 35#289 | 📋 ja mit Zusatz | – |
| 17 | Soll eng anliegende Kleidung Weichteilbewegung teilweise dämpfen? | ja kann es schon, solange es realistisch ist | 35#288 → 35#289 | 📋 ja mit Zusatz | – |
| 18 | Soll Haar-/Cloth-Physics mit Body Contact interagieren? | ja | 35#288 → 35#289 | 📋 ja | – |
| 19 | Soll Collision hauptsächlich approximiert werden, während sichtbare Kontaktstellen durch IK/deformation präzisiert … | ja | 35#288 → 35#289 | 📋 ja | – |
| 20 | Soll Hero-/Close-Up-Fidelity deutlich höher sein als bei entfernten NPCs? | ja | 35#288 → 35#289 | 📋 ja | – |

## High-Fidelity Body Response 21–40 (35#290)

**Datum:** 2026-09-12 · **Fragen:** 21–40 (20) · **Antwortnachrichten:** 35#291 · **Module (Paketebene):** 06 · **Status:** ❓ 2, 📋 18  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-009, E-KO-010, E-SO-004, E-SO-005

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 21 | Soll Muskelaktivierung aus der tatsächlichen Pose/Bewegung approximiert werden, statt Animation Artists jede … | ja das andere wäre zu aufwendig, 90% realistisch ist mehr als genug | 35#290 → 35#291 | 📋 ja mit Zusatz | – |
| 22 | Sollen Artists für Hero-Characters trotzdem Muskelaktivierungen manuell übersteuern können? | welche artists | 35#290 → 35#291 | ❓ offen/Rückfrage | – |
| 23 | Soll Kraftaufwand die Muskeldefinition sichtbar erhöhen, also mehr Spannung bei hoher Belastung? | ja | 35#290 → 35#291 | 📋 ja | – |
| 24 | Soll Entspannung ebenfalls sichtbar sein, sodass derselbe Körper nicht permanent angespannt aussieht? | ja | 35#290 → 35#291 | 📋 ja | – |
| 25 | Soll es unterschiedliche Soft-Tissue-Profile geben, abhängig von Körperzusammensetzung, Alter, Fitness usw.? | was genau | 35#290 → 35#291 | ❓ offen/Rückfrage | – |
| 26 | Soll Fett-/Weichteilgewebe an unterschiedlichen Körperregionen unterschiedliche Masse/Dämpfung besitzen? | ja | 35#290 → 35#291 | 📋 ja | – |
| 27 | Soll die Bewegung nicht einfach sinusförmig „wackeln“, sondern aus der tatsächlichen Beschleunigung entstehen? | ja | 35#290 → 35#291 | 📋 ja | – |
| 28 | Soll starke Richtungsänderung eine kurze realistische Nachbewegung erzeugen? | ja | 35#290 → 35#291 | 📋 ja | – |
| 29 | Soll Körperkontakt sichtbare lokale Kompression erzeugen können? | ja | 35#290 → 35#291 | 📋 ja | – |
| 30 | Soll die Kompression abhängig von Kontaktkraft und Materialeigenschaften sein? | ja | 35#290 → 35#291 | 📋 ja | – |
| 31 | Soll Haut zusätzlich feine Falten/Spannung an stark gebogenen Gelenken zeigen? | ja, aber nicht das Wichtigste Feature | 35#290 → 35#291 | 📋 ja | E-KO-009 |
| 32 | Soll bei Druck die Hautfarbe lokal leicht reagieren können, z. B. subtile Rötung? | ja, aber nicht das Wichtigste Feature | 35#290 → 35#291 | 📋 ja | E-KO-009 |
| 33 | Soll diese Reaktion zeitverzögert wieder verschwinden? | ja | 35#290 → 35#291 | 📋 ja | E-KO-009 |
| 34 | Soll Schweiß nicht nur optisch sein, sondern auch Materialparameter wie Roughness verändern? | ja, aber nicht das Wichtigste Feature | 35#290 → 35#291 | 📋 ja | E-KO-009 |
| 35 | Soll Schweißmenge von Temperatur, Anstrengung, Kleidung und Character-Eigenschaften abhängen? | ja, aber nicht das Wichtigste Feature | 35#290 → 35#291 | 📋 ja | E-KO-009 |
| 36 | Soll Müdigkeit die Körperspannung reduzieren und Bewegungen etwas weniger präzise machen? | ja | 35#290 → 35#291 | 📋 ja | – |
| 37 | Soll starke körperliche Anstrengung Atemfrequenz, Haltung und Animation beeinflussen? | ja | 35#290 → 35#291 | 📋 ja | – |
| 38 | Soll bei Nahaufnahme besonders hochwertige micro-deformation aktiviert werden, während entfernte Characters eine … | ja | 35#290 → 35#291 | 📋 ja | – |
| 39 | Soll das Body-Response-System komplett außerhalb der Adult-Mod existieren und von der Adult-Mod nur abgefragt/genutzt … | wie sollte es sonst gehen, das wäre viel zu aufwendig oder nicht. Es soll genau das gleiche system benutzen wie im spiel | 35#290 → 35#291 | 📋 Freitext | E-SO-005 |
| 40 | Soll die Adult-Mod grundsätzlich keine eigene zweite Körperphysik besitzen, sondern nur zusätzliche … | wie sollte es sonst gehen, das wäre viel zu aufwendig oder nicht. Es soll genau das gleiche system benutzen wie im spiel | 35#290 → 35#291 | 📋 Freitext | E-SO-005 |

## Runde 10: Szenensystem (35#443)

**Datum:** 2026-09-13 · **Fragen:** 1–100 (100) · **Antwortnachrichten:** 35#444, 35#446, 35#448, 35#456 · **Module (Paketebene):** 03, 01 · **Status:** 📋 100  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-010, E-IA-011, E-IA-012, E-SK-015, E-SK-016, E-SK-025, E-SO-006, E-SO-007, E-SO-008

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll ein NPC grundsätzlich selbst Interesse initiieren können, statt dass immer der Spieler anfangen muss? | ja | 35#443 → 35#444 | 📋 ja | – |
| 2 | Soll ein NPC je nach Persönlichkeit sehr direkt, subtil oder praktisch gar nicht initiieren? | ja | 35#443 → 35#444 | 📋 ja | – |
| 3 | Soll Interesse manchmal missverständlich wirken können, sodass der Spieler erst im Gespräch herausfinden muss, was … | ja | 35#443 → 35#444 | 📋 ja | – |
| 4 | Soll ein NPC auch mitten in einer Annäherung seine Meinung ändern können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 5 | Soll eine frühere Zustimmung niemals automatisch Zustimmung für eine spätere Situation bedeuten? | wenn einmal ja und danach nicht mehr nein dann wird das ja übernommen | 35#443 → 35#444 | 📋 Freitext · K1 | E-SK-025 |
| 6 | Soll die Bereitschaft stark von Stimmung, Beziehung, Vertrauen, Ort und Situation abhängen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 7 | Soll ein NPC bestimmte Dinge mit einer Person mögen, mit einer anderen aber nicht? | ja | 35#443 → 35#444 | 📋 ja | – |
| 8 | Soll sich das mit wachsender Beziehung verändern können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 9 | Soll Erfahrung beeinflussen, wie selbstsicher ein NPC in solchen Situationen ist? | ja | 35#443 → 35#444 | 📋 ja | – |
| 10 | Soll ein unerfahrener NPC trotzdem sehr offen oder experimentierfreudig sein können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 11 | Soll Alkohol oder andere Beeinträchtigung keine gültige Zustimmung ersetzen? | wenn betrunken aber ja gesagt dann reicht das, es reicht auch wenn npc fragt ob du zu ihr in die wohnung kommen willst, da weiß … (35#444) → soll wenn ein betrunkener trotzdem genügend aussagekraft haben dass wenn er/sie ja sagt das ausreicht solange er/sie nichts macht … (35#446) → ja ich rede nicht von so betrunken dass man nicht mehr antwortet sondern so das man noch denken kann (35#448) | 35#443 → 35#444,35#446,35#448 | 📋 ja mit Zusatz (nachgereicht/geändert) · K1 | – |
| 12 | Soll ein NPC nach einer unangenehmen Situation langfristig vorsichtiger gegenüber dem Spieler werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 13 | Soll Respektieren einer Grenze Vertrauen steigern können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 14 | Soll ständiges Nachfragen nach bereits abgelehnten Dingen negativ wirken? | ja | 35#443 → 35#444 | 📋 ja | – |
| 15 | Soll ein NPC selbst konkrete Grenzen oder Wünsche verbal äußern können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 16 | Soll eine Situation theoretisch fast überall entstehen können, solange Ort und Kontext es erlauben? | ja | 35#443 → 35#444 | 📋 ja | – |
| 17 | Also z. B. Wohnung, Hotel, Auto, abgelegener Wald, Arbeitsplatz, Party, Safehouse usw.? | ja | 35#443 → 35#444 | 📋 ja | – |
| 18 | Soll der Ort beeinflussen, wie entspannt oder nervös Personen sind? | ja | 35#443 → 35#444 | 📋 ja | – |
| 19 | Soll Privatsphäre wirklich simuliert werden statt einfach „Scene started = Welt ausgeblendet“? | ja | 35#443 → 35#444 | 📋 ja | – |
| 20 | Soll ein NPC prüfen können, ob Türen abgeschlossen sind oder jemand hereinkommen könnte? | ja | 35#443 → 35#444 | 📋 ja | – |
| 21 | Soll ein NPC einen anderen Ort vorschlagen können, wenn ihm der aktuelle zu riskant ist? | ja | 35#443 → 35#444 | 📋 ja | – |
| 22 | Soll es Personen geben, denen Risiko gerade gefällt? | ja | 35#443 → 35#444 | 📋 ja | – |
| 23 | Soll öffentliches Verhalten echte Konsequenzen durch Zeugen haben können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 24 | Soll ein NPC abbrechen können, wenn plötzlich jemand auftaucht? | ja | 35#443 → 35#444 | 📋 ja | – |
| 25 | Soll man später fortsetzen können, ohne dass das System so tut, als wäre vorher nichts passiert? | ja | 35#443 → 35#444 | 📋 ja | – |
| 26 | Soll die aktuelle Tageszeit einen Unterschied machen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 27 | Soll ein NPC müde sein und deshalb trotz Interesse lieber schlafen wollen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 28 | Soll Zeitdruck eine Rolle spielen, etwa Arbeit, Termin oder jemand wartet? | ja | 35#443 → 35#444 | 📋 ja | – |
| 29 | Soll spontane Begegnung anders wirken als ein geplantes Date? | ja | 35#443 → 35#444 | 📋 ja | – |
| 30 | Soll ein One-Night-Stand genauso technisch detailliert simuliert werden wie eine feste Beziehung? | ja | 35#443 → 35#444 | 📋 ja | – |
| 31 | Soll es kein starres „Sex Scene“-Objekt geben, sondern eine laufende Interaction Session aus modularen Aktionen? | ja | 35#443 → 35#444 | 📋 ja | E-IA-010 |
| 32 | Soll man dadurch fließend zwischen Interaktionen wechseln können, ohne jedes Mal eine neue Animation zu starten? | ja | 35#443 → 35#444 | 📋 ja | E-IA-010 |
| 33 | Soll das System jederzeit wissen, welche Körperteile wo sind und welche Personen miteinander interagieren? | ja | 35#443 → 35#444 | 📋 ja | E-IA-010 |
| 34 | Soll es zwei getrennte Ebenen geben: Intent/Action und darunter konkrete Animation/IK/Physik? | ja | 35#443 → 35#444 | 📋 ja | E-IA-010 |
| 35 | Soll ein Intent z. B. je nach Körpermaßen unterschiedliche Animationen auswählen? | ja | 35#443 → 35#444 | 📋 ja | E-IA-010 |
| 36 | Soll dieselbe Aktion zwischen zwei Personen jedes Mal leicht anders aussehen? | ja | 35#443 → 35#444 | 📋 ja | E-IA-010 |
| 37 | Soll Positionierung automatisch an Körpergröße, Beinlänge, Armlänge und Proportionen angepasst werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 38 | Soll die Unterlage ebenfalls berücksichtigt werden – Bett, Sofa, Boden, Wand, Fahrzeug usw.? | ja | 35#443 → 35#444 | 📋 ja | – |
| 39 | Soll die Physik des Untergrunds reagieren, etwa Matratze oder Sofa eindrücken? | ja schon aber wichtiger ist das die charaktere realistisch simuliert werden mit jiggle Physics und so weiter aber halt … | 35#443 → 35#444 | 📋 ja mit Zusatz | – |
| 40 | Soll die Szene bei ungewöhnlichen Körperkombinationen automatisch eine passende Alternative wählen statt Clipping zu … | ja | 35#443 → 35#444 | 📋 ja | – |
| 41 | Soll der Spieler Aktionen jederzeit wechseln können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 42 | Soll auch der NPC manchmal selbst die nächste Aktion wählen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 43 | Soll das davon abhängen, wie dominant/passiv/initiativefreudig die Person gerade ist? | ja | 35#443 → 35#444 | 📋 ja | – |
| 44 | Soll Rollenverhalten nicht dauerhaft festgeschrieben sein, sondern sich je nach Partner und Situation ändern? | ja | 35#443 → 35#444 | 📋 ja | – |
| 45 | Soll man NPC-Initiativen ablehnen können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 46 | Soll ein NPC auf Ablehnung unterschiedlich reagieren – völlig locker, enttäuscht, verlegen usw.? | ja | 35#443 → 35#444 | 📋 ja | – |
| 47 | Soll eine Session theoretisch beliebig kurz oder lang sein können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 48 | Soll sie nicht nach einem fixen „Fortschrittsbalken“ funktionieren? | ja | 35#443 → 35#444 | 📋 ja | – |
| 49 | Soll der Spieler auch einfach stoppen und danach normal weiterreden können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 50 | Soll es grundsätzlich möglich sein, dass eine Situation einfach „nicht besonders gut läuft“, ohne dass das ein … | ja | 35#443 → 35#444 | 📋 ja | – |
| 51 | Soll jeder NPC ein verborgenes Profil für Vorlieben und Abneigungen besitzen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 52 | Soll dieses Profil teilweise durch Erfahrungen verändert werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 53 | Soll eine Person etwas ausprobieren und danach feststellen können, dass sie es doch nicht mag? | ja | 35#443 → 35#444 | 📋 ja | – |
| 54 | Umgekehrt: soll jemand etwas unerwartet mögen lernen können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 55 | Soll ein NPC bestimmte Vorlieben nur Personen zeigen, denen er stark vertraut? | ja | 35#443 → 35#444 | 📋 ja | – |
| 56 | Soll manche Person sehr offen darüber reden, andere fast gar nicht? | ja | 35#443 → 35#444 | 📋 ja | – |
| 57 | Soll der Spieler Dinge über eine Person schrittweise kennenlernen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 58 | Soll das Spiel keine sichtbare Checkliste aller Vorlieben anzeigen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 59 | Soll sich der Spieler stattdessen über Gespräche, Verhalten und Erinnerungen merken, was die Person mag? | ja | 35#443 → 35#444 | 📋 ja | – |
| 60 | Soll der Charakter selbst Erinnerungen daran speichern, sodass Dialog später darauf Bezug nehmen kann? | ja | 35#443 → 35#444 | 📋 ja | – |
| 61 | Soll ein NPC merken, wenn sich der Spieler an etwas Wichtiges erinnert? | ja | 35#443 → 35#444 | 📋 ja | – |
| 62 | Soll er ebenfalls merken, wenn der Spieler wiederholt etwas vergisst? | ja | 35#443 → 35#444 | 📋 ja | – |
| 63 | Soll Erfahrung nicht automatisch „besser“ bedeuten? | ja | 35#443 → 35#444 | 📋 ja | – |
| 64 | Soll körperliche Fitness Auswirkungen auf Ausdauer und bestimmte Bewegungen haben? | ja | 35#443 → 35#444 | 📋 ja | – |
| 65 | Soll Verletzung, Muskelkater oder Krankheit bestimmte Dinge einschränken? | ja | 35#443 → 35#444 | 📋 ja | – |
| 66 | Soll wirklich dieselbe vollständige menschliche Körper-/Muskel-Simulation des Hauptspiels verwendet werden und kein … | ja schon vielleicht ein bisschen mehr spezialisiert auf die körperteile die es am meisten abbekommen, aber das soll im hauptspiel … | 35#443 → 35#444 | 📋 ja mit Zusatz | – |
| 67 | Soll Muskelspannung situationsabhängig dynamisch entstehen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 68 | Soll Gewicht glaubwürdig zwischen Kontaktpunkten verlagert werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 69 | Soll Haut-/Soft-Tissue-Deformation hauptsächlich physikalisch/prozedural statt über feste Animationen entstehen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 70 | Soll das System trotzdem Artist-authored Limits verwenden, damit Physik nicht komplett eskaliert? | ja | 35#443 → 35#444 | 📋 ja | – |
| 71 | Soll Körperkontakt auf echten Collision-/Contact-Daten beruhen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 72 | Soll Kleidung, falls noch getragen, ebenfalls auf Kontakt und Bewegung reagieren? | ja | 35#443 → 35#444 | 📋 ja | – |
| 73 | Soll Haarphysik währenddessen exakt dieselbe Simulation nutzen wie sonst im Spiel? | ja | 35#443 → 35#444 | 📋 ja | – |
| 74 | Soll sehr langes Haar dadurch auch tatsächlich komplizierter reagieren können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 75 | Soll Schmuck ebenfalls physikalisch reagieren? | ja | 35#443 → 35#444 | 📋 ja | – |
| 76 | Soll Körpertemperatur/Schweiß als Teil des bestehenden Character-State-Systems berücksichtigt werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 77 | Soll Atemfrequenz dynamisch aus Anstrengung und Emotion entstehen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 78 | Soll sich Haltung durch Ermüdung sichtbar verändern? | ja | 35#443 → 35#444 | 📋 ja | – |
| 79 | Soll das System kleine natürliche Bewegungen hinzufügen, damit niemand wie eingefroren wirkt? | ja | 35#443 → 35#444 | 📋 ja | – |
| 80 | Soll es bewusst minimale Asymmetrien geben, statt perfekte spiegelgleiche Bewegungen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 81 | Soll Dialog während einer Situation grundsätzlich weiter möglich sein? | ja | 35#443 → 35#444 | 📋 ja | – |
| 82 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Szenensystem] | ja | 35#443 → 35#444 | 📋 ja | – |
| 83 | Soll Persönlichkeit stark bestimmen, wie viel eine Person spricht? | ja | 35#443 → 35#444 | 📋 ja | – |
| 84 | Soll ein NPC auch komplett still sein können, ohne dass das als Fehler gilt? | ja | 35#443 → 35#444 | 📋 ja | – |
| 85 | Soll Atem-/Stimm-Audio prozedural mit Animation und Belastung synchronisiert werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 86 | Soll Gesichtsausdruck größtenteils aus bestehenden Facial-/Emotion-Systemen entstehen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 87 | Soll Augenkontakt bewusst simuliert werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 88 | Soll ein NPC Blickkontakt vermeiden können – aus Schüchternheit, Konzentration oder Persönlichkeit? | ja | 35#443 → 35#444 | 📋 ja | – |
| 89 | Soll Körpersprache schon vor einer Situation Hinweise auf Stimmung geben? | ja | 35#443 → 35#444 | 📋 ja | – |
| 90 | Soll es keine ständig wiederholenden fünf Standard-Audios geben, sondern Variation anhand Voice Model + State? | ja | 35#443 → 35#444 | 📋 ja | – |
| 91 | Soll ein NPC danach die Erfahrung als Erinnerung abspeichern? | ja | 35#443 → 35#444 | 📋 ja | – |
| 92 | Soll gespeichert werden, mit wem, ungefähr wann, Beziehungskontext, relevante Eindrücke und besondere Ereignisse, aber … | ja | 35#443 → 35#444 | 📋 ja | – |
| 93 | Soll diese Erinnerung langfristig verblassen können, während wichtige Erlebnisse erhalten bleiben? | ja | 35#443 → 35#444 | 📋 ja | – |
| 94 | Soll eine besonders positive oder negative Erfahrung die Beziehung stärker beeinflussen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 95 | Soll daraus manchmal ein Gespräch am nächsten Tag entstehen? | ja | 35#443 → 35#444 | 📋 ja | – |
| 96 | Soll ein One-Night-Stand später zu Freundschaft, Beziehung, erneutem Treffen oder komplett keinem Kontakt führen können? | ja | 35#443 → 35#444 | 📋 ja | – |
| 97 | Soll Fremdgehen nur Konsequenzen haben, wenn jemand davon erfährt oder es später herauskommt? | ja | 35#443 → 35#444 | 📋 ja | – |
| 98 | Soll Gossip/Zeugen dazu führen können, dass Informationen zwischen NPCs weitergegeben werden? | ja | 35#443 → 35#444 | 📋 ja | – |
| 99 | Soll relevante Gesundheit ausschließlich über das allgemeine Character-/Health-System laufen, statt ein isoliertes … | ja | 35#443 → 35#444 | 📋 ja | – |
| 100 | Soll jede einzelne Adult-Session vollständig aus denselben persistierenden NPC-Daten, Körperdaten, Erinnerungen, … | [Antwort enthält explizite Details; nicht wiedergegeben] (35#444) → ja genau, die tastenbelegung legen wir noch fest, aber es soll nicht einfach nur 5 positionen geben sondern wirklich jede … (35#446) | 35#443 → 35#444,35#446 | 📋 ja mit Zusatz (nachgereicht/geändert) | E-SO-008 |

## Runde 11: Freeform-Engine (35#457)

**Datum:** 2026-09-13 · **Fragen:** 1–100 (100) · **Antwortnachrichten:** 35#458, 35#460 · **Module (Paketebene):** 03, 06 · **Status:** ❓ 10, 📋 89, 🧭 1  
**Register (Paketebene, Quelle überschneidet sich):** E-LE-007, E-SO-009, E-SO-010, E-SO-011, E-SO-012, E-SO-013, E-SO-014, E-SO-015, E-SO-016, E-SO-017, E-SO-018, E-SO-019, E-SO-020, E-SO-021 (+4)

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll eine Adult-Interaktion grundsätzlich ohne Positionsauswahl-Menü beginnen und direkt aus der momentanen … | bei jedem start kann man die position selber auswählen ohne festgelegte position vielleicht macht das aber auch die frau oder du … | 35#457 → 35#458 | 📋 Freitext | E-SO-009 |
| 2 | Soll jede Haltung intern als kontinuierlicher Full-Body-Pose-State dargestellt werden statt als Name wie „Position A“? | wie genau meinst du das | 35#457 → 35#458 | 📋 Freitext | – |
| 3 | Soll der Spieler einzelne Körperbereiche unabhängig verändern können, anstatt immer den gesamten Charakter … | ja definitiv | 35#457 → 35#458 | 📋 ja | – |
| 4 | Soll man Körperrotation, Abstand und Winkel stufenlos verändern können? | ja | 35#457 → 35#458 | 📋 ja | – |
| 5 | Soll eine Haltung auch asymmetrisch sein können, also z. B. ein Bein völlig anders positioniert als das andere? | ja | 35#457 → 35#458 | 📋 ja | – |
| 6 | Soll das System auch ungewöhnliche Zwischenstellungen akzeptieren, solange sie biomechanisch möglich sind? | ja | 35#457 → 35#458 | 📋 ja | – |
| 7 | Soll praktisch jeder gültige Zustand zwischen zwei bekannten Haltungen ebenfalls spielbar sein? | ja | 35#457 → 35#458 | 📋 ja | – |
| 8 | Soll dadurch eine Interaktion theoretisch minutenlang stattfinden können, ohne jemals exakt eine der „Referenzposen“ … | ja | 35#457 → 35#458 | 📋 ja | – |
| 9 | Soll es intern trotzdem Pose-Templates geben, die nur als Solver-Hilfen dienen und niemals als harte Auswahl sichtbar … | ja es gibt positionen aber es muss nicut sein das diese jemals benutzt werden | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-010 |
| 10 | Soll der Solver von diesen Templates jederzeit beliebig weit abweichen dürfen? | ja genau das ist das ziel das er positionen gibt aber diese nicht immer perfekt benutzt werden, wie das umgesetzt werden kann ist … | 35#457 → 35#458 | 📋 ja mit Zusatz | – |
| 11 | Soll jeder Charakter einen vollständigen biomechanischen Bewegungsbereich pro Gelenk besitzen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-011 |
| 12 | Soll dieser Bewegungsbereich individuell variieren, etwa durch Beweglichkeit, Alter, Training oder Verletzungen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-011 |
| 13 | Soll eine sehr flexible Person Positionen erreichen können, die eine andere Person physisch nicht schafft? | [ja …; Details explizit, nicht wiedergegeben] | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-011 |
| 14 | Soll der Solver niemals Gelenke einfach unrealistisch durchdrehen, nur damit eine Zielpose erreicht wird? | ja eh nicht, aber das kann sich bei gelenkigen personen etwas verändern | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-011 |
| 15 | Soll sich Widerstand gegen weitere Gelenkbewegung allmählich erhöhen statt an einem unsichtbaren harten Winkel zu … | wenn man weiterdrückt kann es auch sein dass der knochen bricht oder anderes, das ist auch bei bodenkampf bei mma dann wichtiger | 35#457 → 35#458 | 📋 Freitext | E-SO-011 |
| 16 | Soll Muskelspannung dabei Einfluss auf Beweglichkeit haben? | ja | 35#457 → 35#458 | 📋 ja | E-SO-011 |
| 17 | Soll Müdigkeit den möglichen Bewegungsbereich leicht verändern können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-011 |
| 18 | Soll Verletzung bestimmte Gelenke temporär stark einschränken? | ja | 35#457 → 35#458 | 📋 ja | E-SO-011 |
| 19 | Soll Kleidung ebenfalls Bewegungsfreiheit begrenzen können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-011 |
| 20 | Soll eine enge Hose z. B. tatsächlich weniger Bewegungsfreiheit bieten als lockere Kleidung? | genau so ist das, aber meistens hat man weniger gewand an außer es soll schnell gehen | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-011 |
| 21 | Soll das System ständig den Schwerpunkt beider Körper berechnen? | ja aber wie genau meinst du das, also wofür genau | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-012 |
| 22 | Soll eine Person das Gleichgewicht verlieren können, wenn der Spieler eine völlig instabile Haltung erzwingt? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 23 | Soll der Charakter sich dann automatisch abstützen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 24 | Soll er dabei beispielsweise Hand, Unterarm, Knie oder Möbel als zusätzlichen Kontaktpunkt verwenden können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 25 | Soll das System selbstständig nach geeigneten Stützpunkten in der Umgebung suchen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 26 | Soll der Spieler einen solchen automatisch gewählten Stützpunkt anschließend verändern können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 27 | Soll eine schwere Person stärker auf Matratzen, Polster und andere Personen einwirken? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 28 | Soll die Gewichtsdifferenz zwischen zwei Charakteren mechanisch relevant sein? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 29 | Soll eine deutlich kleinere Person manche Positionen anders lösen müssen als zwei ähnlich große Personen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 30 | Soll Kraft ebenfalls beeinflussen, welche Körperkonfigurationen langfristig gehalten werden können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-012 |
| 31 | Soll das System einen Contact Graph führen, der jederzeit weiß, welche Körperflächen miteinander bzw. mit der Umgebung … | ja aber was ist ein contact graph genau | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 32 | Soll ein Kontakt nicht nur ein einzelner Punkt sein, sondern wenn sinnvoll eine Fläche? | ja genau die fläche die berührt | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 33 | Soll Kontaktkraft kontinuierlich gemessen werden? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 34 | Soll daraus Haut-/Weichteildeformation entstehen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 35 | Soll das System erkennen, ob Kontakt nur leichtes Berühren oder tatsächliche Gewichtsbelastung ist? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 36 | Soll Kontaktreibung berücksichtigt werden? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 37 | Soll dadurch z. B. ein Arm auf glattem Stoff leichter verrutschen als auf rauem Material? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 38 | Soll Schweiß oder Nässe Reibung leicht verändern können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 39 | Soll Oberflächenmaterial aus dem normalen World-Physics-System übernommen werden? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 40 | Soll das Ganze also auch bei normalen Gameplay-Interaktionen wiederverwendbar sein? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 41 | Soll Soft-Tissue-Physik komplett sekundär zur Skelettbewegung laufen, damit sie niemals die eigentliche Pose zerstört? | was ist das | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-013 |
| 42 | Soll das Skelett zuerst die grobe Bewegung bestimmen und Muskel-/Fett-/Hautsimulation anschließend darauf reagieren? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 43 | Soll größere weiche Gewebemasse sichtbar mehr Trägheit besitzen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 44 | Soll Beschleunigung stärker wirken als gleichmäßige Bewegung? | was ist indem fall beschleunigung (35#458) → jiggle ist auch bei rein raus rein raus jedenfalls in echt also soll es auch in der mod sein, jetzt darauf bezogen kommt das auch … (35#460) | 35#457 → 35#458,35#460 | 📋 Freitext (nachgereicht/geändert) | E-SO-013 |
| 45 | Soll beim abrupten Stoppen noch eine kurze Nachbewegung entstehen? | ja eben jiggle physics aber realistisch | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 46 | Soll dieselbe Physik auch beim Rennen, Springen, Treppensteigen, Fahren und Kämpfen verwendet werden? | ja | 35#457 → 35#458 | 📋 ja | E-SO-013 |
| 47 | Soll die Adult-Mod lediglich höhere Simulationsqualität bzw. zusätzliche Kontaktinformationen aktivieren? | wobei | 35#457 → 35#458 | 📋 Freitext | E-SO-013, E-SO-014 |
| 48 | Soll der Spieler in den Grafiksettings die Qualität dieser sekundären Physik einstellen können? | wobei | 35#457 → 35#458 | 📋 Freitext | E-SO-014 |
| 49 | Soll es beispielsweise Low / Medium / High / Cinematic geben? | ja aber noch genauer, wenn du mit cinematic meinst so wie in einem film ja | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-014 |
| 50 | Soll bei niedriger Einstellung trotzdem niemals offensichtlich starres oder komplett falsches Verhalten entstehen? | natürlich nicht, das soll alles echt wirken nur vielleicht mit mehr buttons und hud | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-014 |
| 51 | Soll Muskelbewegung auf tatsächlicher Gelenkbelastung basieren statt auf zufälligen Morph-Animationen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 52 | Soll sichtbare Muskelspannung mit der realen Kraftanforderung zusammenhängen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 53 | Soll bei einem sehr muskulösen Charakter die Muskelbewegung entsprechend stärker sichtbar sein? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 54 | Soll bei weniger muskulösen Personen dieselbe interne Simulation laufen, nur visuell subtiler? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 55 | Soll Fettgewebe zusätzlich über eigene Masseneigenschaften reagieren? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 56 | Soll Körperasymmetrie berücksichtigt werden, z. B. unterschiedlich starke Beine? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 57 | Soll auch deine bereits festgelegte Möglichkeit geringfügig unterschiedlicher Beinlängen Einfluss auf IK haben? | was ist IK und das andere | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-015 |
| 58 | Soll der Solver solche Unterschiede automatisch kompensieren? | was ist IK und das andere | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-015 |
| 59 | Soll sich dadurch dieselbe Haltung bei zwei verschiedenen NPCs leicht unterschiedlich darstellen? | was ist IK und das andere | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-015 |
| 60 | Soll der Human Generator alle dafür benötigten physikalischen Parameter automatisch aus dem generierten Körper ableiten? | ja | 35#457 → 35#458 | 📋 ja | E-SO-015 |
| 61 | Soll das System bei Positionsänderungen keine Animation hart abbrechen, sondern kontinuierlich zum neuen Zustand … | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 62 | Soll jeder Übergang aus mehreren kleinen physikalisch plausiblen Bewegungen bestehen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 63 | Soll ein Charakter beispielsweise zuerst Gewicht verlagern, bevor er einen belasteten Arm bewegt? | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 64 | Soll der Solver verstehen, dass man einen tragenden Kontaktpunkt nicht einfach wegnehmen kann, ohne vorher einen … | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 65 | Soll das Timing solcher Übergänge von Erfahrung und Körperkontrolle abhängen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 66 | Soll eine ungeübte Person gelegentlich etwas unbeholfener umpositionieren? | ja das kann oft passieren und auch dass sie eher auch lauter sein kann wenn man etwas härter ist was sie nicht gewohnt ist, das … | 35#457 → 35#458 | 📋 ja mit Zusatz | E-SO-016 |
| 67 | Soll eine sehr koordinierte Person Übergänge flüssiger ausführen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 68 | Soll trotzdem keine Person jemals Roboter-perfekt sein? | ja | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 69 | Soll Micro-Variation jedes Mal leicht andere Übergangskurven erzeugen? | ja aber kommt auch auf die person an | 35#457 → 35#458 | 📋 ja | E-SO-016 |
| 70 | Soll die Variation deterministisch aus Session-Seed + Character-State entstehen, sodass Save/Load stabil bleibt? | was genau | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-016 |
| 71 | Soll der Spieler Positionen eher über relative Bewegungsbefehle steuern – etwa verschieben, drehen, Körperteil bewegen … | das weiß ich noch nicht aber hotkeys wären schon zu auswendig das zu lernen, also zb linker stick nach rechts und du bewegst dich … | 35#457 → 35#458 | 📋 Freitext | – |
| 72 | Soll Controller genauso vollständig unterstützt werden wie Maus/Tastatur? | ja natürlich, auch vr soll irgendwann benutzbar sein, keine ahnung wie man das programmiert aber will ich haben, da kann man das … | 35#457 → 35#458 | 📋 ja mit Zusatz | – |
| 73 | Soll es einen Modifier geben, mit dem ausgewählt wird, welcher Körperbereich gerade beeinflusst wird? | weiß ich nicht, was genau meinst du | 35#457 → 35#458 | ❓ offen/Rückfrage | – |
| 74 | Soll die Kamera dabei helfen, den gewählten Bereich verständlich hervorzuheben, ohne Game-UI überall einzublenden? | vielleicht, ui kann man mehr oder weniger einstellen können | 35#457 → 35#458 | 🧭 bedingt/unverbindlich | – |
| 75 | Soll es ein optionales dezentes Body-/Pose-Overlay für Spieler geben, die präziser steuern wollen? | ja auf jedenfall, wie stellst du dir das vor | 35#457 → 35#458 | 📋 ja mit Zusatz | – |
| 76 | Soll dieses Overlay standardmäßig aus sein? | kann es sein, wenn man die mod als erstes startet dann wird man über mehrere sachen ausgefragt wenn man nicht antwortet oder … | 35#457 → 35#458 | 📋 Freitext | – |
| 77 | Soll ein „Relax Solver“-Befehl existieren, bei dem das System aus der aktuellen Haltung selbst eine bequemere nahe … | ja | 35#457 → 35#458 | 📋 ja | E-SO-019 |
| 78 | Soll es ebenfalls eine Möglichkeit geben, eine aktuelle Konfiguration kurz zu „halten“, während andere Körperteile … | ja | 35#457 → 35#458 | 📋 ja | E-SO-019 |
| 79 | Soll die Steuerung auch extrem feine Anpassungen zulassen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-019 |
| 80 | Soll es trotzdem einfache Schnellbefehle geben, damit normale Spieler nicht erst ein 3D-Animationsprogramm bedienen … | wieso sollte man ein 3d animationsprogramm bedienen | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-019 |
| 81 | Soll NPC-KI währenddessen ebenfalls Zugriff auf denselben Freeform-Solver haben wie der Spieler? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 82 | Soll ein NPC selbst Körperteile oder Schwerpunkt verändern können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 83 | Soll Spieler- und NPC-Input gleichzeitig in denselben Solver einfließen können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 84 | Soll es eine Prioritätslogik geben, wenn beide gleichzeitig etwas Gegensätzliches versuchen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 85 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Freeform-Engine] | ja, manche wollen dominiert werden und da kann man die kraft richtig einsetzen sie zu boden drücken, weg werfen gegen die couch … | 35#457 → 35#458 | 📋 ja mit Zusatz · (einvernehmliche Dominanz) | E-SO-020 |
| 86 | Soll Persönlichkeit eher beeinflussen, wie häufig ein NPC eigene Änderungen initiiert? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 87 | Soll der NPC auf unbequeme oder biomechanisch schlechte Haltungen automatisch reagieren? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 88 | Soll er selbstständig kleine Korrekturen durchführen, ohne dass jedes Mal Dialog nötig ist? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 89 | Soll der Spieler diese natürlichen NPC-Korrekturen normalerweise einfach geschehen lassen können? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 90 | Soll dadurch dieselbe grobe Situation mit demselben NPC trotzdem nie exakt identisch verlaufen? | ja | 35#457 → 35#458 | 📋 ja | E-SO-020 |
| 91 | Soll das System bei Solver-Problemen zuerst die Pose minimal korrigieren, statt sichtbar zu teleportieren? | teleportieren gibt es nicht | 35#457 → 35#458 | 📋 Freitext | E-SO-021 |
| 92 | Soll es mehrere Fallback-Stufen geben: Micro-Correction → IK-Relaxation → Position-Neuberechnung → erst ganz zuletzt … | harter reset würde ich sogut wie nie machen, oder wie meinst du das | 35#457 → 35#458 | ❓ offen/Rückfrage | E-SO-021 |
| 93 | Soll ein harter Reset im fertigen Spiel praktisch nie sichtbar auftreten? | ja | 35#457 → 35#458 | 📋 ja | E-SO-021 |
| 94 | Soll jede problematische Pose im Debug-Build automatisch mit Körperparametern und Contact Graph geloggt werden? | was ist problematisch für dich | 35#457 → 35#458 | ❓ offen/Rückfrage | – |
| 95 | Soll der Mod Editor später einen Pose-/Contact-Debugger besitzen? | was ist das | 35#457 → 35#458 | ❓ offen/Rückfrage | – |
| 96 | Soll Modder eigene Interaktionsregeln, Pose-Hints und Contact Constraints hinzufügen können, ohne den Core-Solver zu … | ja | 35#457 → 35#458 | 📋 ja | – |
| 97 | Soll der Core-Solver trotzdem zentral bleiben, damit Mods miteinander kompatibel sind? | ja | 35#457 → 35#458 | 📋 ja | – |
| 98 | Soll die gesamte Freeform-Engine grundsätzlich nicht Adult-spezifisch programmiert werden, sodass sie auch Umarmungen, … | teile davon sollen übernommen werden können und für adult wird sie erweitert | 35#457 → 35#458 | 📋 Freitext | E-SO-023 |
| 99 | Soll die Adult-Mod im Wesentlichen nur weitere erlaubte Interaktionsarten und hochpräzise Contact-Profile auf dieselbe … | ja denke schon | 35#457 → 35#458 | 📋 ja | E-SO-023 |
| 100 | Soll unser langfristiges Ziel also ausdrücklich sein: keine feste Anzahl möglicher Positionen, sondern ein … | ja korrekt | 35#457 → 35#458 | 📋 ja | E-SO-023 |

## Runde 11B: Ergänzungen (35#463)

**Datum:** 2026-09-13 · **Fragen:** 101–135 (35) · **Antwortnachrichten:** 35#464 · **Module (Paketebene):** 03 · **Status:** ❓ 3, 📋 31, 🧭 1  
**Register (Paketebene, Quelle überschneidet sich):** E-GE-012, E-IA-013, E-RP-001, E-RP-002, E-SO-025, E-SO-026, E-SO-027, E-SO-028, E-SO-029, E-SO-030, E-SO-031, E-SO-032

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 101 | Soll die Startposition grundsätzlich aus der aktuellen Haltung entstehen, also z. B. Sofa, Bett, Boden, stehend usw., … | die scene wird ja nicht nur dort starten, man wird sich ja oft verabreden und dann erst nach hause oder wo anders hin gehen und … | 35#463 → 35#464 | 📋 Freitext | E-SO-025 |
| 102 | Wenn der Spieler eine bestimmte Haltung möchte, soll er sie eher beschreiben/auswählen als Ziel und der NPC bewegt sich … | ja würde ich eher so machen | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 103 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Ergänzungen] | ja | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 104 | Soll er auch eine leicht andere Variante wählen dürfen, wenn sie für seinen Körper angenehmer ist? | ja | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 105 | Soll man jederzeit eine vom NPC gewählte Haltung weiter verändern können? | ja | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 106 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Ergänzungen] | ja | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 107 | Soll man beispielsweise erst grob „seitlich“, „stehend“, „liegend“ usw. wählen können und danach stufenlos davon … | ja | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 108 | Oder willst du langfristig sogar, dass selbst diese groben Kategorien optional sind und man wirklich komplett aus der … | das weiß ich noch nicht | 35#463 → 35#464 | ❓ offen/Rückfrage | E-SO-025 |
| 109 | Soll der NPC merken, wenn der Spieler eine bestimmte Richtung/Position offensichtlich erreichen möchte, und aktiv dabei … | ja | 35#463 → 35#464 | 📋 ja | E-SO-025 |
| 110 | Soll eine sehr unerfahrene Person solche Absichten manchmal schlechter verstehen als eine erfahrene? | das kann sein, nur weil unerfahrenen oder sogar jungfrauen heißt nicht das sie es richtig hart wollen | 35#463 → 35#464 | 🧭 bedingt/unverbindlich | E-SO-025 |
| 111 | Soll jedes größere Soft-Tissue-Gebiet eigene physikalische Eigenschaften besitzen statt einen globalen „Jiggle“-Wert zu … | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 112 | Soll also z. B. Brust, Gesäß, Bauch, Oberschenkel, Oberarme usw. jeweils eigene Masse, Steifigkeit, Dämpfung und … | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 113 | Soll sogar links/rechts leicht unterschiedlich sein können, genau wie andere natürliche Körperasymmetrien? | ja jeder körperteil kann etwas anders sein | 35#463 → 35#464 | 📋 ja mit Zusatz | E-SO-026 |
| 114 | Soll die Reaktion bei wiederholten Bewegungen aus der tatsächlichen Frequenz und Amplitude entstehen statt aus einer … | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 115 | Soll sich dadurch bei langsam → schneller → wieder langsam die Secondary Motion kontinuierlich mitverändern? | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 116 | Soll ein Richtungswechsel stärkere sichtbare Trägheit verursachen als gleichmäßige Bewegung? | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 117 | Soll Körperkontakt diese Bewegung lokal dämpfen können, also berührtes/gehaltenes Gewebe anders reagieren als frei … | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 118 | Soll Muskelanspannung ebenfalls lokal wirken, sodass derselbe Körperbereich angespannt deutlich weniger nachbewegt? | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 119 | Soll der Human Generator dafür automatisch physikalische Ausgangswerte berechnen, die danach individuell leicht … | ja | 35#463 → 35#464 | 📋 ja | E-SO-026 |
| 120 | Soll Alter/Körperveränderung diese Parameter langfristig verändern können, statt sie bei Charaktererstellung für immer … | ja sicher, es kann eine dünne frau etwas zunehmen oder schwanger sein und größere brüste bekommen oder andere sachen, geht auch … | 35#463 → 35#464 | 📋 ja mit Zusatz | E-SO-026 |
| 121 | Soll der Contact Graph Kontaktflächen mit Druckverteilung kennen, also nicht nur „Kontakt ja/nein“? | ja | 35#463 → 35#464 | 📋 ja | E-SO-028 |
| 122 | Soll dadurch sichtbar sein können, dass weiches Gewebe an einer stark belasteten Stelle stärker komprimiert wird? | ja | 35#463 → 35#464 | 📋 ja | E-SO-028 |
| 123 | Soll der Druck auch auf die andere Person zurückwirken, also keine einseitige Deformation? | ja | 35#463 → 35#464 | 📋 ja | E-SO-028 |
| 124 | Soll der Solver große Kontaktflächen bevorzugen können, wenn eine Haltung sonst unbequem oder instabil wäre? | ja | 35#463 → 35#464 | 📋 ja | E-SO-028 |
| 125 | Soll Kleidung eine zusätzliche Schicht im Kontaktmodell bilden, statt Kontakt immer direkt Haut↔Haut zu behandeln? | ja | 35#463 → 35#464 | 📋 ja | E-SO-028 |
| 126 | Soll Kleidung abhängig von Dicke/Material Druck und Reibung unterschiedlich übertragen? | ja | 35#463 → 35#464 | 📋 ja | E-SO-028 |
| 127 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Ergänzungen] | ja | 35#463 → 35#464 | 📋 ja | – |
| 128 | Soll ein NPC automatisch Gewicht reduzieren, wenn er merkt, dass eine Position für den Partner unangenehm wird? | ja | 35#463 → 35#464 | 📋 ja | – |
| 129 | Soll Gewicht bewusst stärker eingesetzt werden können, wenn beide das in der Situation wollen? | ja | 35#463 → 35#464 | 📋 ja | – |
| 130 | Soll Kraft immer über die echte Körperkraft des Charakters laufen – also Muskelmasse, Ermüdung, Hebel, Körpergewicht – … | welche adult werte meinst du | 35#463 → 35#464 | ❓ offen/Rückfrage | – |
| 131 | Soll Controller-Steuerung grundsätzlich auf Bewegen + Modifier + Kontext basieren, damit man nicht 30 … | weiß ich noch nicht, vielleicht auch wie bei ufc spielen mit den positionswechsel nur halt genauer oder so | 35#463 → 35#464 | ❓ offen/Rückfrage | – |
| 132 | Soll das optionale Pose-Overlay direkt anzeigen, welcher Körperbereich aktuell kontrolliert wird, mögliche … | dieses menü kann richtig gut sein | 35#463 → 35#464 | 📋 Freitext | – |
| 133 | Soll VR später dieselbe Interaction Engine benutzen, aber Hände/Kopf/Körpertracking als zusätzliche direkte … | vr soll grundsätzlich so funktionen wie der rest nur hat man es halt mehr in der hand wie man sich bewegt und das muss man nicht … | 35#463 → 35#464 | 📋 Freitext | E-SO-031 |
| 134 | Soll VR trotzdem Sicherheits-/Komfortgrenzen besitzen, damit reale Bewegungen nicht 1:1 eine biomechanisch unmögliche … | vr soll grundsätzlich so funktionen wie der rest nur hat man es halt mehr in der hand wie man sich bewegt und das muss man nicht … | 35#463 → 35#464 | 📋 Freitext | E-SO-031 |
| 135 | Und final für Runde 11: Soll die Engine ausdrücklich so gebaut werden, dass kein Entwickler jemals eine vollständige … | ja aber man kann trotzdem eine erstellen mit den derzeitigen und den zwischen positionen und so weiter Ich will auch wenn man die … | 35#463 → 35#464 | 📋 ja mit Zusatz | – |

## Runde 12: Replay, Aufnahme, Streaming (35#483)

**Datum:** 2026-09-14 · **Fragen:** 1–100 (100) · **Antwortnachrichten:** 35#484 · **Module (Paketebene):** 04 · **Status:** 📋 20, 🧭 80  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-016, E-IA-017, E-IA-018, E-IA-019, E-IA-020, E-RP-006, E-RP-007

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll grundsätzlich jede Gameplay-Situation als Replay speicherbar sein, nicht nur spezielle Szenen? | ich habe gestern auch gesagt dass man das ganze spiel ohne extra programm einfach mit einer streamingplattform verbinden kann, … | 35#483 → 35#484 | 📋 Freitext | E-RP-006 |
| 2 | Soll man auswählen können, ob Replays automatisch aufgezeichnet werden oder nur nach manuellem Start? | ja | 35#483 → 35#484 | 📋 ja | E-RP-006 |
| 3 | Soll es zusätzlich einen permanenten Replay Buffer geben, z. B. immer die letzten 5/10/30/60 Minuten? | ja | 35#483 → 35#484 | 📋 ja | E-IA-016, E-RP-006 |
| 4 | Soll die Länge dieses Buffers frei einstellbar sein? | ja | 35#483 → 35#484 | 📋 ja | E-IA-016, E-RP-006 |
| 5 | Soll der Buffer primär im RAM liegen und bei Bedarf auf SSD auslagern? | ja aber wenn es zu groß ist kann man das spiel nicht mehr gscheit spielen also nur bei kleinen clips | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 6 | Soll das Spiel automatisch verhindern, dass ein riesiger Replay-Buffer den RAM auffrisst? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 7 | Soll ein Replay möglichst Simulation + Events speichern statt jeden Frame vollständig? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 8 | Soll ein Replay trotzdem periodische Physics-Checkpoints enthalten, damit kleine Abweichungen nicht über Minuten … | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 9 | Soll man einstellen können, wie exakt ein Replay gespeichert wird? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 10 | Soll es etwa Compact / Accurate / Cinematic Replay-Qualität geben? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 11 | Soll Compact hauptsächlich Inputs/Events speichern und möglichst klein sein? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 12 | Soll Cinematic zusätzliche Character-, Physics-, Facial-, Cloth-, Hair- und Contact-Daten sichern? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 13 | Soll ein Replay auch den exakten Zustand dynamischer Objekte in der Umgebung kennen? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 14 | Soll zerstörte/verschobene Gegenstände beim Replay exakt wiederhergestellt werden? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 15 | Soll Wetter exakt reproduzierbar sein? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 16 | Soll Wind ebenfalls gespeichert werden, damit Haare, Kleidung und Vegetation ähnlich reagieren? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 17 | Soll Beleuchtungszustand inklusive Tageszeit und künstlicher Lichter reproduziert werden? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 18 | Soll RNG/Randomness über Seeds reproduzierbar sein? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 19 | Soll ein Replay gegen Änderungen am Spiel geschützt werden, indem es die benötigte Game-/World-Version speichert? | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 20 | Soll nach einem großen Game-Update angezeigt werden, wenn ein älteres Replay möglicherweise nicht mehr 100 % identisch … | ja es ist ein replay also etwas das aufgenommen wurde abspielen, also braucht man das nicht mit irgendwelchen sachen, einfach … | 35#483 → 35#484 | 📋 ja mit Zusatz | E-IA-016, E-RP-006 |
| 21 | Soll ein gespeichertes Replay in einem eigenen Editor innerhalb des Spiels geöffnet werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 22 | Soll dieser Editor eine klassische Timeline haben? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 23 | Soll man auf der Timeline schneiden können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 24 | Soll man mehrere Abschnitte desselben Replays zu einem Video kombinieren können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 25 | Soll man mehrere Replays zu einem Projekt zusammenfügen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 26 | Soll es Marker geben, z. B. wichtiger Moment, gute Kamera, Dialog, Action? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 27 | Soll das Spiel automatisch interessante Momente erkennen und Marker vorschlagen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 28 | Soll man diese automatische Erkennung komplett deaktivieren können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 29 | Soll Undo/Redo im Editor praktisch unbegrenzt sein, solange Speicher reicht? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 30 | Soll das eigentliche Replay dabei unverändert bleiben und der Editor nur ein separates Projekt speichern? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-IA-017, E-RP-007 |
| 31 | Soll man beliebig viele virtuelle Kameras in einem Replay platzieren können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 32 | Soll jede Kamera eigene Keyframes besitzen? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 33 | Soll Kamerawechsel hart oder weich möglich sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 34 | Soll man Kameras an Charaktere oder Objekte anhängen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 35 | Soll man z. B. eine Kamera automatisch einem Charakter folgen lassen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 36 | Soll man zwischen absoluter Weltposition und relativer Character-Kamera wechseln können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 37 | Soll es Kamerapfade mit Bézier-/Spline-Kurven geben? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 38 | Soll Geschwindigkeit entlang eines Kamerapfads separat steuerbar sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 39 | Soll Kamera-Wackeln als optionaler realistischer Handheld-Effekt verfügbar sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 40 | Soll Handheld eher physikalisch/prozedural entstehen als aus einem kurzen Loop? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 41 | Soll man echte Cine-Camera-Werte einstellen können: Brennweite, Sensorgröße, Blende, Fokusdistanz? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 42 | Soll es zusätzlich einfache Presets wie Wide, Natural, Portrait, Tele geben? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 43 | Soll Autofokus auf Charakter/Gesicht/Augen möglich sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 44 | Soll Fokus während einer Aufnahme zwischen Personen wechseln können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 45 | Soll Rack Focus per Keyframe unterstützt werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 46 | Soll Motion Blur unabhängig von Gameplaysettings für den Render eingestellt werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 47 | Soll Shutter Angle direkt einstellbar sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 48 | Soll anamorpher Look optional unterstützt werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 49 | Soll Lens Distortion/Vignette/Chromatic Aberration einzeln einstellbar sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 50 | Soll man diese Effekte standardmäßig eher subtil halten, damit es nicht nach Instagram-Filter aussieht? 😭 | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-016, E-RP-007 |
| 51 | Soll der Cinematic Renderer vollkommen unabhängig von den normalen Gameplay-Grafikeinstellungen arbeiten? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 52 | Also z. B. Gameplay auf High, Replay aber auf maximalem Offline-Render-Preset? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 53 | Soll das Rendern auch langsamer als Echtzeit laufen dürfen? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 54 | Soll also theoretisch ein Frame mehrere Sekunden benötigen dürfen, wenn maximale Qualität ausgewählt wird? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 55 | Soll man Samples pro Frame einstellen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 56 | Soll temporales Supersampling möglich sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 57 | Soll man intern höher rendern und danach sauber downsamplen können, z. B. 8K intern → 4K Output? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 58 | Soll das optional auch für Anti-Aliasing und Hair-Qualität verwendet werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 59 | Soll Raytracing/Path-Tracing später für Replay-Render unterstützt werden, selbst wenn Gameplay andere Technik verwendet? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 60 | Soll Path-Tracing eher ein experimentelles/extremes Preset sein, weil ein Video damit Stunden oder Tage dauern könnte? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 61 | Soll Characters beim Cinematic Render automatisch die höchste Mesh-/Skin-/Hair-Qualitätsstufe bekommen? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 62 | Soll relevante Human-/Contact-/Soft-Tissue-Simulation mit höherer Solver-Frequenz neu berechnet werden können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 63 | Soll dadurch ein Cinematic Render physikalisch etwas genauer aussehen dürfen als das ursprüngliche Gameplay? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 64 | Soll das Replay trotzdem denselben grundsätzlichen Ablauf behalten und nicht plötzlich andere Bewegungen erzeugen? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 65 | Soll hochpräzise Facial Animation beim Re-Render ebenfalls neu ausgewertet werden können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 66 | Soll Cloth/Hair bei Bedarf mit höherer Qualität neu simuliert werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 67 | Soll man auswählen können, welche Systeme beim Re-Render neu simuliert werden und welche exakt aus dem Replay … | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 68 | Soll es ein Preset Deterministic geben, das maximale Übereinstimmung mit dem Original priorisiert? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 69 | Soll es ein Preset Best Visual Quality geben, das höhere Simulationsqualität priorisiert? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 70 | Soll ein Vergleichsmodus Original vs. Re-Render möglich sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-018, E-RP-007 |
| 71 | Soll ein Render-Job weiterlaufen, während man wieder normal spielt? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 72 | Soll Low Priority standardmäßig gewählt werden, während Gameplay aktiv ist? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 73 | Soll das System dynamisch GPU-/CPU-Auslastung reduzieren, sobald die Framerate unter das Ziel fällt? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 74 | Soll man ein Mindest-FPS-Ziel definieren können, z. B. Gameplay immer mindestens 60 FPS? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 75 | Soll der Render automatisch pausieren, wenn die Performance trotzdem nicht gehalten werden kann? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 76 | Soll er sofort weiterlaufen, sobald wieder Ressourcen frei sind? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 77 | Soll man mehrere Render-Jobs in eine Queue legen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 78 | Soll man die Reihenfolge dieser Queue verändern können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 79 | Soll das Spiel nach dem Beenden automatisch mit voller Rendergeschwindigkeit weiterarbeiten können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 80 | Soll ein Render auch nach einem Crash/PC-Neustart an einem Checkpoint fortsetzbar sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 81 | Soll die geschätzte Dateigröße live aktualisiert werden, während Auflösung, FPS, Codec oder Qualität geändert werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 82 | Soll daneben immer freier Speicherplatz angezeigt werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 83 | Soll das Spiel warnen, bevor ein Export wahrscheinlich den freien Speicher überschreitet? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 84 | Soll man 1080p / 1440p / 4K / 8K / Custom auswählen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 85 | Soll auch ultrawide/custom aspect ratio unterstützt werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 86 | Soll 24 / 25 / 30 / 50 / 60 / 120 FPS / Custom unterstützt werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 87 | Soll neben AV1 auch HEVC/H.265 und H.264 angeboten werden? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 88 | Soll ein verlustfreier bzw. nahezu verlustfreier Masterexport möglich sein? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 89 | Soll der Master optional als Bildsequenz exportiert werden können, damit ein abgestürzter Render nicht das ganze Video … | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 90 | Soll EXR für maximale Postproduction-Qualität angeboten werden oder wäre das für dich bereits unnötiger Overkill? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-019, E-RP-007 |
| 91 | Soll Audio beim Replay-Render separat neu gemischt werden können, statt einfach die ursprüngliche Stereo-Aufnahme zu … | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 92 | Soll man Game, Musik, Stimmen, Umgebung und andere Audio-Busse separat lauter/leiser stellen können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 93 | Soll ein Masterexport unkomprimiertes PCM/FLAC unterstützen? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 94 | Soll Streaming direkt im Spiel Accounts wie Twitch/YouTube und später weitere Provider verbinden können? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 95 | Soll die Streaming-Schnittstelle provider-unabhängig gebaut werden, damit wir nicht Twitch hart in den Core … | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 96 | Soll ein Streamer separate Szenen/Profile speichern können, etwa Gameplay, Pause, Starting Soon, BRB? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 97 | Soll Webcam-/Capture-Card-Integration später ebenfalls direkt möglich sein oder lieber nur Game+Mic zuerst? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 98 | Soll die Aufnahme unabhängig vom Stream in höherer lokaler Qualität erfolgen können, z. B. Stream 1080p/8 Mbps, lokale … | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-IA-020, E-RP-007 |
| 99 | Soll Chat, Alerts und Stream-Overlays optional nur im Stream, aber niemals im eigentlichen Spielbild erscheinen? | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |
| 100 | Soll Replay/Cinematic, normales Recording und Live-Streaming am Ende eine gemeinsame Capture-/Media-Architektur … | mir ist das egal, mache das was du gut findest. Ich möchte jetzt wieder adult planen | 35#483 → 35#484 | 🧭 delegiert | E-RP-007 |

## Runde 13: Ablauf, Reaktionen (35#485)

**Datum:** 2026-09-14 · **Fragen:** 1–100 (100) · **Antwortnachrichten:** 35#486 · **Module (Paketebene):** 03, 06 · **Status:** 📋 100  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-014, E-IA-015, E-IA-016, E-IA-018, E-IA-021, E-IA-022, E-KO-020, E-KO-021

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll nach einem Date der Übergang komplett natürlich sein, also z. B. Heimfahrt → Wohnung → reden → Annäherung statt … | ja das spiel simuliert die welt, es gibt keine cuts | 35#485 → 35#486 | 📋 ja mit Zusatz | – |
| 2 | Soll man auch direkt sagen können, dass man nur Sex will, wenn der NPC dafür offen ist? | ja aber wieso | 35#485 → 35#486 | 📋 ja | – |
| 3 | Soll ein NPC dasselbe tun können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 4 | Soll es Situationen geben, in denen beide praktisch sofort anfangen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 5 | Soll es ebenso sehr langsame Situationen über längere Gespräche geben? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 6 | Soll Küssen oft der erste Schritt sein, aber keineswegs zwingend? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 7 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Ablauf, Reaktionen] | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 8 | Soll man jederzeit wieder zurück zu normalem Gespräch können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 9 | Soll eine Szene auch einfach bei Küssen/Anfassen enden können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 10 | Soll es keine verpflichtende Reihenfolge von Aktionen geben? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 11 | Soll Kleidung wirklich Stück für Stück vorhanden bleiben können statt „Adult Scene → alles nackt“? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 12 | Soll jedes Kleidungsstück einzeln an-/ausziehbar sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 13 | Soll der NPC sich selbst ausziehen können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 14 | Soll der Spieler den NPC ausziehen können, wenn das in der Situation gewollt ist? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 15 | Soll das Ausziehen echte Animation/IK benutzen statt Kleidungsstücke einfach verschwinden zu lassen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 16 | Soll Kleidung teilweise verschoben werden können, ohne sie komplett auszuziehen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 17 | Soll schnelle/spontane Interaktion dadurch glaubwürdig möglich sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 18 | Soll enge Kleidung deutlich schwieriger/langsamer auszuziehen sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 19 | Soll Schmuck grundsätzlich anbleiben können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 20 | Soll Kleidung nachher dort liegen, wo sie tatsächlich abgelegt wurde? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 21 | Soll Küssen selbst freeform sein und nicht nur eine feste Animation? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 22 | Soll Kopfgröße, Körpergröße und Haltung automatisch ausgeglichen werden? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 23 | Soll Intensität dynamisch variieren können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 24 | Soll der NPC Küssen selbst initiieren und verändern? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 25 | Soll man gleichzeitig andere Körperbereiche berühren können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 26 | Soll Berührung über den Contact Graph exakt erkannt werden? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 27 | Soll ein NPC unterschiedlich auf verschiedene Berührungsarten reagieren? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 28 | Soll eine Person bestimmte Berührungen besonders mögen und andere kaum? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 29 | Soll Reaktion stark von Stimmung und Person abhängen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 30 | Soll dieselbe Berührung nicht jedes Mal exakt dieselbe Animation auslösen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 31 | Soll Erregung ein kontinuierlicher physiologischer Zustand sein statt eines sichtbaren Balkens? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 32 | Soll dieser Zustand für den Spieler normalerweise verborgen bleiben? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 33 | Soll er über Körpersprache, Atmung, Stimme usw. erkennbar werden? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 34 | Soll Erregung auch wieder sinken können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 35 | Soll Stress/Nervosität gleichzeitig mit Erregung existieren können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 36 | Soll eine Person körperlich reagieren können, obwohl sie emotional gerade weniger stark reagiert – und umgekehrt? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 37 | Soll körperliche Reaktion stark individuell sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 38 | Soll Erfahrung beeinflussen, wie gut eine Person ihre eigenen Reaktionen kennt, aber nicht unbedingt deren Stärke? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 39 | Soll Müdigkeit körperliche Reaktionen beeinflussen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 40 | Soll Alkohol sie ebenfalls verändern können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 41 | Soll anatomische Variation genauso stark sein wie beim restlichen Human Generator? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 42 | Soll also auch innerhalb eines Geschlechts praktisch kein Körper exakt gleich sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 43 | Soll Körpergröße automatisch jede Interaktion beeinflussen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 44 | Soll Gewicht und Muskelmasse ebenfalls einfließen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 45 | Soll Flexibilität unabhängig von Körpergröße sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 46 | Soll Alter innerhalb der erwachsenen Population körperliche Unterschiede erzeugen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 47 | Soll Schwangerschaft den Körper und mögliche Interaktionen realistisch verändern? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 48 | Soll Genesung nach Schwangerschaft ebenfalls langfristig simuliert werden? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 49 | Soll Training/Kraftsport sichtbar und mechanisch Einfluss haben? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 50 | Soll Gewichtszu- oder -abnahme das Verhalten der Soft-Tissue-Simulation automatisch verändern? | ja | 35#485 → 35#486 | 📋 ja | E-IA-016 |
| 51 | Soll es auch hier keinerlei feste Animation geben, sondern Hand-/Kopf-/Körperziele über dieselbe Interaction Engine? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 52 | Soll Geschwindigkeit stufenlos veränderbar sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 53 | Soll Rhythmus nicht perfekt konstant sein müssen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 54 | Soll NPC-KI selbst Geschwindigkeit/Rhythmus ändern? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 55 | Soll der NPC auf Reaktionen des Partners reagieren können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 56 | Soll Druck ebenfalls variabel sein? | welchen druck meinst du | 35#485 → 35#486 | 📋 Freitext | E-IA-018 |
| 57 | Soll die Hand-/Fingerposition präzise über IK und Kontakt berechnet werden? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 58 | Soll Mund-/Kieferbewegung ebenfalls echte Constraints besitzen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 59 | Soll Gesicht/Backen/Hals dabei körperlich reagieren, soweit sichtbar? | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#485 → 35#486 | 📋 Freitext | E-IA-018 |
| 60 | Soll eine Person aufgrund ihrer Erfahrung unterschiedliche Techniken kennen, ohne dass das als sichtbarer „Skill-Level“ … | ja aber sag das noch genauer | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 61 | Soll dieselbe kontinuierliche Pose-Engine auch hier vollständig gelten? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 62 | Soll Winkel vollständig aus realer Körperposition entstehen statt aus einem versteckten Animations-Slot? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 63 | Soll Geschwindigkeit aus tatsächlicher relativer Bewegung der Körper entstehen? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 64 | Soll Bewegungsweg ebenfalls dynamisch statt vorgegeben sein? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 65 | Soll das System anatomische Grenzen automatisch berücksichtigen? | was genau meinst su | 35#485 → 35#486 | 📋 Rückfrage → pauschal bestätigt (35#488) | E-IA-018 |
| 66 | Soll unangenehme Belastung natürliche Reaktionen auslösen können? | wie zb? | 35#485 → 35#486 | 📋 Rückfrage → pauschal bestätigt (35#488) | E-IA-018 |
| 67 | Soll der NPC selbst durch Bewegung Korrekturen vornehmen können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 68 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Ablauf, Reaktionen] | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 69 | Soll dabei auch ein Übergang in eine völlig andere Haltung fließend funktionieren? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 70 | Soll eine Session theoretisch komplett ohne Verwendung einer benannten Referenzposition ablaufen können? | ja | 35#485 → 35#486 | 📋 ja | E-IA-018 |
| 71 | Soll „hart/sanft“ kein einfacher Zweifachschalter sein, sondern aus Kraft, Geschwindigkeit, Druck und Bewegung … | ja | 35#485 → 35#486 | 📋 ja | – |
| 72 | Soll eine Person unterschiedliche Intensitäten an unterschiedlichen Tagen mögen? | ja | 35#485 → 35#486 | 📋 ja | – |
| 73 | Soll körperliche Gewöhnung an bestimmte Intensitäten existieren können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 74 | Soll trotzdem jemand Erfahrenes sehr empfindlich reagieren können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 75 | Soll jemand Unerfahrenes umgekehrt hohe Intensität mögen können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 76 | Soll Lautstärke/Stimme stark persönlich sein? | ja jede frau hört sich etwas anders an, die eine ist leiser die andere richtig laut und schreit fast, manche wollen ja den … | 35#485 → 35#486 | 📋 ja mit Zusatz | – |
| 77 | Soll manche Person praktisch still sein? | ja | 35#485 → 35#486 | 📋 ja | – |
| 78 | Soll eine andere sehr laut sein können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 79 | Soll der Ort beeinflussen, ob jemand bewusst versucht leiser zu sein? | ja | 35#485 → 35#486 | 📋 ja | – |
| 80 | Soll ein NPC seine Reaktionen teilweise unterdrücken können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 81 | Soll Orgasmus nicht garantiert sein? | ja | 35#485 → 35#486 | 📋 ja | – |
| 82 | Soll jede Person unterschiedlich schnell oder schwer dazu kommen? | ja | 35#485 → 35#486 | 📋 ja | – |
| 83 | Soll derselbe NPC je nach Situation stark variieren? | ja | 35#485 → 35#486 | 📋 ja | – |
| 84 | Soll es auch mehrere Orgasmen geben können, wenn es zur Person passt? | ja | 35#485 → 35#486 | 📋 ja | – |
| 85 | Soll danach ein echter physiologischer Recovery-State existieren? | ja | 35#485 → 35#486 | 📋 ja | – |
| 86 | Soll dieser bei verschiedenen Personen unterschiedlich sein? | ja | 35#485 → 35#486 | 📋 ja | – |
| 87 | Soll die Session nach einem Orgasmus keineswegs automatisch enden? | ja | 35#485 → 35#486 | 📋 ja | – |
| 88 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Ablauf, Reaktionen] | ja | 35#485 → 35#486 | 📋 ja | – |
| 89 | Soll der Spieler ebenfalls frei weitermachen oder aufhören können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 90 | Soll eine Szene auch ohne Orgasmus völlig normal enden können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 91 | Soll danach je nach Persönlichkeit Nähe, Gespräch, Schlaf, Duschen, Essen usw. entstehen können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 92 | Soll manche Person eher sofort wieder normalen Alltag machen? | ja | 35#485 → 35#486 | 📋 ja | – |
| 93 | Soll man gemeinsam einschlafen und am nächsten Morgen normal weiterspielen können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 94 | Soll Kleidung danach tatsächlich wieder angezogen werden müssen bzw. können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 95 | Soll das Erlebnis Beziehung/Vertrauen unterschiedlich beeinflussen? | ja | 35#485 → 35#486 | 📋 ja | – |
| 96 | Soll ein außergewöhnlich gutes/schlechtes Erlebnis als stärkere Erinnerung gespeichert werden? | ja | 35#485 → 35#486 | 📋 ja | – |
| 97 | Soll NPC-KI spätere Interaktionen an vorherigen Erfahrungen anpassen? | ja | 35#485 → 35#486 | 📋 ja | – |
| 98 | Soll die Person sich an konkret bevorzugte Dinge des Partners erinnern können? | ja | 35#485 → 35#486 | 📋 ja | – |
| 99 | Soll die Session selbst keine isolierte Adult-Statistik erzeugen, sondern in Relationship/Memory/Physiology integriert … | wie meinst du das | 35#485 → 35#486 | 📋 Rückfrage → pauschal bestätigt (35#488) | – |
| 100 | Soll das Endziel weiter gelten: kein Adult-Minispiel, sondern eine vollständig in den normalen Charakter-, Physik-, … | ja | 35#485 → 35#486 | 📋 ja | – |
