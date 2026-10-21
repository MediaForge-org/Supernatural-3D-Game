# Adult 39: Frage-Antwort-Matrix, Runde 16 (Master-Runde)

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

- [Runde 16A: Master-Taxonomie (35#573)](#runde-16a-master-taxonomie-35573)
- [Runde 16B: Basiskategorien (35#583)](#runde-16b-basiskategorien-35583)
- [Runde 16C: Positionen (35#589)](#runde-16c-positionen-35589)
- [Runde 16D: Steuerung, Intensität (35#609)](#runde-16d-steuerung-intensität-35609)
- [Runde 16E: Reaktionen (35#611)](#runde-16e-reaktionen-35611)
- [Runde 16F: Gewebe, Verletzung (35#618)](#runde-16f-gewebe-verletzung-35618)
- [Runde 16G: NPC-Verhalten (35#624)](#runde-16g-npc-verhalten-35624)
- [Runde 16H: UI, Einstellungen, Technik (35#626)](#runde-16h-ui-einstellungen-technik-35626)
- [Runde 16I: Registry I (35#642)](#runde-16i-registry-i-35642)
- [Runde 16J: Unterkategorien (35#650)](#runde-16j-unterkategorien-35650)
- [Runde 16K: Reaktions-/Outcome-Familien (35#652)](#runde-16k-reaktions-outcome-familien-35652)
- [Runde 16L: Empfindung, Erholung (35#676)](#runde-16l-empfindung-erholung-35676)
- [Runde 16M: Registry II (35#678)](#runde-16m-registry-ii-35678)
- [Runde 16N: Höhepunkt, Kleidung, Solo (35#686)](#runde-16n-höhepunkt-kleidung-solo-35686)
- [Runde 16O: Orte (35#692)](#runde-16o-orte-35692)
- [Runde 16P: Kombinierte Aktionen (35#694)](#runde-16p-kombinierte-aktionen-35694)
- [Runde 16Q: Risiko, Körperzustand (35#706)](#runde-16q-risiko-körperzustand-35706)
- [Runde 16R: Produktions-Locks (35#712)](#runde-16r-produktions-locks-35712)

---


## Runde 16A: Master-Taxonomie (35#573)

**Datum:** 2026-09-16 · **Fragen:** 1–25 (25) · **Antwortnachrichten:** 35#574, 35#576 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-025, E-XX-002, E-XX-010

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Master-Kategorien: Soll jede Adult-Interaktion einer oder mehreren groben Hauptkategorien zugeordnet werden können, … | [ja …; Details explizit, nicht wiedergegeben] | 35#573 → 35#574 | 📋 ja mit Zusatz | E-IA-025 |
| 2 | Unterkategorien: Soll jede Hauptkategorie beliebig viele Unterkategorien besitzen dürfen, damit wir später nicht … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 3 | Tags zusätzlich zur Hierarchie: Soll es neben Kategorien ein freies Tag-System geben, sodass eine Interaktion … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 4 | Intensität als mehrere Achsen: Soll Intensity niemals ein einzelner Wert von 0–100 sein, sondern aus mehreren … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 5 | Individuelle Intensität pro Körperbereich: Soll dieselbe Session gleichzeitig an einem Körperbereich sehr sanft und an … | ja man kann die Intensität sehr oft anpassen, also nicht nur pro bereich sondern von der einen Sekunde zur anderen, zb mit X … | 35#573 → 35#574 | 📋 ja mit Zusatz | E-IA-025 |
| 6 | Dynamische Intensität: Soll Intensität im Verlauf einer Session ständig steigen/fallen können und nicht nur einmal beim … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 7 | Preferred vs. actual intensity: Soll das System getrennt speichern, was eine Person bevorzugt und was gerade … | [ja …; Details explizit, nicht wiedergegeben] (35#574) → ja eh nicht, aber es kann passieren auch vom spieler gewollt, wenn die frau dann nichts sagt dann ist das ok für sie. (35#576) | 35#573 → 35#574,35#576 | 📋 ja mit Zusatz (nachgereicht/geändert) · K1 | E-IA-025 |
| 8 | Location-Mastertypen: Soll jede mögliche Umgebung zuerst einer groben Location-Klasse angehören, z. B. Private Home, … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 9 | Location-Eigenschaften: Soll ein Ort zusätzlich tatsächliche Parameter haben wie Privatsphäre, Sichtbarkeit, … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 10 | Kein Adult-Slot im Raum: Soll ein Bett/Sofa/Tisch/Auto keinen speziellen Adult-Slot brauchen, sondern durch normale … | adult ist überall machbar, wenn du lust hast kannst du die frau einfach ausziehen(solange sie dich liebt und gerne sex hat sollte … | 35#573 → 35#574 | 📋 Freitext · K1 | E-IA-025, E-XX-002 |
| 11 | Participants: Soll die Engine von Anfang an mit einer variablen Anzahl erwachsener Beteiligter arbeiten und nicht … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 12 | Rollen pro Moment: Soll eine Rolle wie führend/folgend/dominant/passiv nicht für die gesamte Session festgeschrieben … | der spieler kann das dynamisxh ändern, wie er will | 35#573 → 35#574 | 📋 Freitext | E-IA-025 |
| 13 | Initiative getrennt: Soll zusätzlich pro Action gespeichert werden, wer sie initiiert hat, wer darauf eingegangen ist … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 14 | Body-Focus-Taxonomie: Soll das System Körperbereiche hierarchisch klassifizieren, z. B. upper body → chest → left/right … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 15 | Links/Rechts getrennt: Soll ein NPC theoretisch sogar unterschiedliche Präferenzen oder Empfindlichkeiten links/rechts … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 16 | Interaction-Type unabhängig vom Körperbereich: Soll eine Action grundsätzlich aus Interaction Type + Target Body Region … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 17 | Items als Modifier/Participant: Soll ein Adult-Item im Interaction Graph wie ein echtes physisches Objekt mit eigener … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 18 | Mehrere Items gleichzeitig: Soll die Engine mehrere Gegenstände gleichzeitig verwalten können, sofern die körperliche … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 19 | Kleidung als kontinuierlicher State: Soll Kleidung pro Kleidungsstück Zustände wie angezogen, teilweise verschoben, … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 20 | Sichtbarkeit/Öffentlichkeit: Soll eine Session getrennte Werte für Privacy Intent und tatsächliche … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 21 | Risiko als mehrere Kategorien: Soll Risiko ebenfalls nicht nur ein Gesamtwert sein, sondern getrennt werden in z. B. … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 22 | Experience-Taxonomie: Soll Erfahrung mindestens auf drei Ebenen gespeichert werden: allgemeine Erfahrung, Erfahrung … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 23 | Preference-State: Soll jede Preference mindestens interest, curiosity, experience, preferredIntensity, riskTolerance, … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 24 | Session Classification nachträglich: Soll das Spiel erst nach oder während der tatsächlichen Interaktion automatisch … | ja | 35#573 → 35#574 | 📋 ja | E-IA-025 |
| 25 | Master-Regel: Soll Runde 16 als technische Grundregel festlegen, dass Kategorie, Intensität, Ort, Beteiligte, Rollen, … | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#573 → 35#574 | 📋 Freitext | E-IA-025 |

## Runde 16B: Basiskategorien (35#583)

**Datum:** 2026-09-16 · **Fragen:** 26–50 (25) · **Antwortnachrichten:** 35#584 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-026, E-IA-027

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 26 | Basiskategorien | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 27 | Kissing als eigene Familie | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 28 | Touch/Body Contact | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 29 | Manual Interactions | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 30 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Basiskategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 31 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Basiskategorien] | ja das ist sehr wichtig und essentiell und muss perfekt sein | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 32 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Basiskategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 33 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Basiskategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 34 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Basiskategorien] | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 35 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Basiskategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 36 | Toys nach Funktion statt Marken | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 37 | Extreme Größen | es soll keine größengrenzen geben, außer du willst ein 1m großes ding seiteärts reinschieben sowas geht nicht aber 10-20cm sollen … | 35#583 → 35#584 | 📋 Freitext | E-IA-026 |
| 38 | Impact | ja, der hintern oder andere körperteile können schnell rot werden und mit der zeit auch aufgekratzt werden und richtig tiefe … | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 39 | Pain als eigene Dimension | ja genau, manche frauen haben eine höhere schmerztoleranz und spüren manche sachen nicht so stark wie andere | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 40 | Restraint | ja sicher | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 41 | Physical Control | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 42 | Sensory | was ist das | 35#583 → 35#584 | 📋 Rückfrage → pauschal bestätigt (35#586) | E-IA-026 |
| 43 | Roleplay | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja · K4 | E-IA-026 |
| 44 | Group Interactions | ja genau, aber als erstes ist mal frau und mann wichtiger als mehrere personen, aber das soll trotzdem wichtig sein und nicht … | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 45 | Parallel Actions | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 46 | Observation / Exhibition | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 47 | Content Production | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 48 | High-Risk Category | [ja …; Details explizit, nicht wiedergegeben] | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |
| 49 | Aftercare / Post-State | ja | 35#583 → 35#584 | 📋 ja | E-IA-026 |
| 50 | Multi-Category Example | ja Die kategorien und so kommen noch oder | 35#583 → 35#584 | 📋 ja mit Zusatz | E-IA-026 |

## Runde 16C: Positionen (35#589)

**Datum:** 2026-09-16 · **Fragen:** 51–75 (25) · **Antwortnachrichten:** 35#590 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-PR-005, E-SO-033, E-TA-013

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 51 | Grundhaltungen als Kategorien, nicht als Presets | ja genau, aber wie gesagt muss eine dieser haltungen nicut perfekt eingenommen werden es kann auch dazwischen sein oder auch ganz … | 35#589 → 35#590 | 📋 ja mit Zusatz | E-SO-033 |
| 52 | Mischformen zwischen Grundhaltungen | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 53 | Auf-/Untereinander getrennt von eigentlicher Pose | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 54 | Orientierung der Körper zueinander | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 55 | Ober- und Unterkörper unabhängig drehen | [ja …; Details explizit, nicht wiedergegeben] | 35#589 → 35#590 | 📋 ja mit Zusatz | E-SO-033 |
| 56 | Beine völlig unabhängig positionierbar | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 57 | Arme/Hände als echte Stütz- und Interaktionspunkte | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 58 | Schwerpunkt bestimmt, was haltbar ist | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 59 | Komfort vs. mögliche Pose | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 60 | Person kann unbequemes trotzdem bewusst halten | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 61 | Körpergröße verändert dieselbe Konfiguration | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 62 | Gewicht und Kraft beeinflussen Positionen | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 63 | Flexibilität als echte Voraussetzung | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 64 | Möbel als Solver-Surfaces | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 65 | Bett/Matratze verformt sich | ja es kann auch passieren wenn es zu hart ist also der sex dass die matraze das bett kaputt geht | 35#589 → 35#590 | 📋 ja mit Zusatz | E-SO-033 |
| 66 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Positionen] | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 67 | Wand / Tür / feste Oberfläche | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 68 | Fahrzeuge ohne spezielle „Auto-Szene“ | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 69 | Sehr enge Räume | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 70 | Boden / Outdoor-Untergrund | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 71 | Freie Positionswechsel ohne Scene Reset | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 72 | Spieler kann Übergang selbst steuern | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 73 | NPC kann eigene Korrektur während des Übergangs machen | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 74 | Positionen als speicherbare Favoriten | ja | 35#589 → 35#590 | 📋 ja | E-SO-033 |
| 75 | Position Registry als offene Bibliothek | ja dieses system soll so sein wie bei ufc wo man jede sache beschrieben bekommt mit der tastenkombination und eine vorschau das … | 35#589 → 35#590 | 📋 ja mit Zusatz | E-SO-033 |

## Runde 16D: Steuerung, Intensität (35#609)

**Datum:** 2026-09-17 · **Fragen:** 76–100 (25) · **Antwortnachrichten:** 35#610 · **Module (Paketebene):** 04, 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-SO-034, E-ST-010

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 76 | Kontinuierliche Intensität statt Stufen: Soll Intensität grundsätzlich stufenlos funktionieren und nicht nur sanft / … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 77 | Geschwindigkeit und Kraft getrennt: Soll eine sehr schnelle Bewegung trotzdem vergleichsweise wenig Kraft haben können … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 78 | Bewegungsumfang als eigene Achse: Soll neben Tempo und Kraft auch die Amplitude bzw. der Bewegungsweg unabhängig … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 79 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung, Intensität] | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 80 | Direkte Controllersteuerung: Soll der Controller primär direkte relative Körperbewegung steuern, also beispielsweise … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 81 | UFC-artiges Kontextmenü: Soll unser bereits angesprochener UFC-artiger Ansatz verwendet werden, bei dem die aktuell … | bei dem ufc meinte ich auch sowas wie die kombos in den spiel mit tastenkombinationen aufgebaut ist kann man das dort auch machen … | 35#609 → 35#610 | 📋 Freitext | E-SO-034 |
| 82 | UI vollständig abschaltbar: Soll ein erfahrener Spieler sämtliche Hilfen ausblenden können und nur über … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 83 | Kein Button-Spam als optimale Strategie: Du hattest als Beispiel X spammen = schneller genannt. Soll Button-Spamming … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 84 | Sofortige vs. geglättete Input-Reaktion: Soll die Figur auf Input zwar unmittelbar reagieren, der Körper aber trotzdem … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 85 | Rhythmus vollständig dynamisch: Soll Rhythmus grundsätzlich aus tatsächlichem Spieler-/NPC-Input entstehen und nicht … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 86 | Rhythmus-Assist: Soll es trotzdem optional möglich sein, eine aktuelle Bewegung kurz automatisch weiterlaufen zu … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 87 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung, Intensität] | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 88 | Gemeinsame Bewegung: Soll Spielerinput und NPC-Bewegung gleichzeitig auf dieselbe Körperkonfiguration wirken können? … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 89 | Widerstand und Mitbewegung: Soll die NPC-KI abhängig von Preference und Situation aktiv mitgehen, sich stärker anpassen … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 90 | Körpergewicht als Intensitätskomponente: Soll Intensität auch daraus entstehen können, wie stark jemand sein … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 91 | Lokaler Druck in Echtzeit: Soll Contact Pressure tatsächlich pro Kontaktfläche laufend berechnet werden, sodass z. B. … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 92 | Mehrere parallele Intensitäten: Wenn gleichzeitig mehrere Actions stattfinden, soll jede Action ihre eigenen … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 93 | Individuelle Toleranzkurven: Soll jeder Charakter für verschiedene Körperregionen eigene Comfort-/Pain-/Injury-Kurven … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 94 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung, Intensität] | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 95 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung, Intensität] | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 96 | Dynamische Entdeckung neuer Preferences: Soll eine ungeplante Veränderung der Intensität oder Action dazu führen … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 97 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Steuerung, Intensität] | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 98 | Performance-LOD für Contact/Physics: Soll die hohe Genauigkeit nur für aktuell relevante Körper/Contacts laufen, … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 99 | Save/Replay-Determinismus: Soll ein Save mitten in einer laufenden Session neben Pose/State auch aktuelle … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |
| 100 | Intensity Event Stream: Soll jede relevante Änderung – Action-Wechsel, Tempo, Kraft, lokale Belastung, … | ja | 35#609 → 35#610 | 📋 ja | E-SO-034 |

## Runde 16E: Reaktionen (35#611)

**Datum:** 2026-09-17 · **Fragen:** 101–125 (25) · **Antwortnachrichten:** 35#612 · **Module (Paketebene):** 06 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-022

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 101 | Atmung als echtes Physiologie-Signal | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 102 | Atemfrequenz und Atemtiefe getrennt | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 103 | Audio direkt mit tatsächlicher Atmung koppeln | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 104 | Stimme pro Person generieren | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 105 | Reaktionslautstärke nicht als fixer Charakterwert | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 106 | Sprache und nonverbale Reaktionen mischen | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 107 | Reaktionslatenz | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 108 | Facial Animation aus mehreren Quellen | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 109 | Individuelle Facial-Muster | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 110 | Blickverhalten | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 111 | Tränen als allgemeines Körpersystem | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 112 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktionen] | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 113 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktionen] | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 114 | Anpassung ohne vollständiges Verschwinden | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 115 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktionen] | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 116 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktionen] | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 117 | Content-Toggle für starke Körperreaktionen | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 118 | Schmerz lokal berechnen | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 119 | Schmerz ≠ Verletzung | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 120 | Sichtbare Hautreaktionen | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 121 | Muskelspannung lokal | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 122 | Zittern / Tremor / Ermüdung | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 123 | Unwillkürliche Körperreaktionen | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 124 | Orgasmus als Multi-System-Ereignis | ja | 35#611 → 35#612 | 📋 ja | E-KO-022 |
| 125 | Unified Reaction Graph | ja Ich gehe jetzt schlafen. Gute Nacht | 35#611 → 35#612 | 📋 ja | E-KO-022 |

## Runde 16F: Gewebe, Verletzung (35#618)

**Datum:** 2026-09-18 · **Fragen:** 126–150 (25) · **Antwortnachrichten:** 35#619 · **Module (Paketebene):** 06 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-GE-009, E-SO-035, E-SP-009, E-ST-012

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 126 | Tissue State: Soll jede relevante Körperregion einen eigenen Gewebezustand haben? Gemeint sind z. B. gesund, gereizt, … | [ja …; Details explizit, nicht wiedergegeben] | 35#618 → 35#619 | 📋 ja mit Zusatz | E-SO-035, E-GE-009 |
| 127 | Unterschiedliche Gewebetypen: Soll Haut, Muskel, Fett, Schleimhaut und Bindegewebe getrennte physikalische … | [ja …; Details explizit, nicht wiedergegeben] | 35#618 → 35#619 | 📋 ja mit Zusatz | E-SO-035, E-GE-009 |
| 128 | Elasticity: Soll Elastizität – also wie weit sich Gewebe verformen und danach zurückbilden kann – individuell und je … | [ja …; Details explizit, nicht wiedergegeben] | 35#618 → 35#619 | 📋 ja mit Zusatz | E-SO-035, E-GE-009 |
| 129 | Compliance: Soll Compliance gespeichert werden? Das beschreibt, wie leicht Gewebe unter Druck nachgibt; hohe Compliance … | [ja …; Details explizit, nicht wiedergegeben] | 35#618 → 35#619 | 📋 ja mit Zusatz | E-SO-035, E-GE-009 |
| 130 | Damping: Soll Dämpfung individuell sein? Damit ist gemeint, wie schnell eine Bewegung oder Verformung nach einem Impuls … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 131 | Temporary Stretch: Soll starke Belastung zu vorübergehender Dehnung führen können, die sich nach Minuten/Stunden/Tagen … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 132 | Long-term Adaptation: Soll häufige, kontrollierte Belastung langfristige Anpassung erzeugen können, ohne dass das … | [ja …; Details explizit, nicht wiedergegeben] | 35#618 → 35#619 | 📋 ja mit Zusatz | E-SO-035 |
| 133 | Keine magische Anpassung: Soll solche Anpassung klare biologische Grenzen haben, sodass extreme Belastung weiterhin … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 134 | Existing Injury: Soll eine bereits verletzte Region bei neuen Interaktionen automatisch empfindlicher und weniger … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 135 | Microtrauma: Soll es Mikroverletzungen geben – sehr kleine Gewebeschäden, die nicht sofort dramatisch aussehen, sich … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 136 | Injury Accumulation: Soll wiederholte Belastung vor vollständiger Heilung aus mehreren kleinen Schäden irgendwann eine … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 137 | Healing Stages: Soll Heilung mehrere Zustände haben, etwa akut → entzündet/empfindlich → Reparatur → weitgehend … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 138 | Scar Tissue: Soll Narbengewebe andere Eigenschaften besitzen als ursprüngliches Gewebe, z. B. andere Elastizität oder … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 139 | Pain Memory: Soll ein NPC sich an starke Schmerzen oder Verletzungen in einer bestimmten Situation erinnern und dieses … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 140 | Pain Tolerance vs. Damage: Soll hohe Schmerztoleranz nur beeinflussen, wie stark jemand Schmerz wahrnimmt – aber … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 141 | Contact Stress: Soll der Contact Graph zusätzlich Kontaktspannung berechnen? Das ist vereinfacht Kraft geteilt durch … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 142 | Shear Force: Soll auch Scherkraft simuliert werden? Das ist seitliches Ziehen/Reiben von Gewebe, nicht nur Druck von … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 143 | Friction Damage: Soll hohe Reibung über längere Zeit Haut/Gewebe reizen oder verletzen können, abhängig von Material, … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 144 | Pressure Duration: Soll nicht nur die Kraft zählen, sondern auch wie lange eine Stelle belastet wird? Niedriger Druck … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 145 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Gewebe, Verletzung] | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 146 | Deformation Recovery: Soll nach starker Verformung eine kurze Recovery-Phase entstehen können, in der Gewebe noch nicht … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 147 | Physics LOD: Soll sehr detaillierte Gewebesimulation nur bei nahen/relevanten Charakteren laufen und bei entfernten … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 148 | Deterministic Save State: Soll Save/Load relevante Gewebezustände, Verletzungen, Heilfortschritt und langfristige … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 149 | Modding API: Soll die Modding-API neue Gewebe-/Verletzungsprofile hinzufügen können, aber über validierte Grenzen, … | ja | 35#618 → 35#619 | 📋 ja | E-SO-035 |
| 150 | Unified Body-State: Soll Anatomie, Belastung, Schmerz, Verletzung, Heilung, Narben, Anpassung und Soft-Tissue-Parameter … | ja Wofür brauche ich dann qwen3.8 uncensored? | 35#618 → 35#619 | 📋 ja mit Zusatz | E-SO-035 |

## Runde 16G: NPC-Verhalten (35#624)

**Datum:** 2026-09-18 · **Fragen:** 151–175 (25) · **Antwortnachrichten:** 35#625 · **Module (Paketebene):** 05 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-029

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 151 | Partner-spezifisches Profil: Soll jeder NPC zusätzlich zum allgemeinen Präferenzprofil ein eigenes Profil pro Partner … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 152 | Attraction getrennt von Preference: Soll Attraction getrennt davon sein, welche Praktiken jemand mag? Eine Person kann … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 153 | Trust: Soll Vertrauen beeinflussen, wie viel jemand ausprobiert, offenbart oder zulässt, ohne automatisch die … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 154 | Emotional Bond: Soll emotionale Bindung separat von Attraction und Trust existieren, damit Casual Sex, Liebe, … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 155 | Current Desire: Soll es einen kurzfristigen aktuellen Wunschzustand geben, der aus Libido, Stimmung, Partner, Situation … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 156 | Initiative Profile: Soll jeder NPC eine Tendenz besitzen, wie oft er selbst Aktionen oder Positionswechsel initiiert, … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 157 | Learning by Observation: Soll ein NPC aus den sichtbaren Reaktionen des Partners lernen, was wahrscheinlich gut oder … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 158 | Learning by Conversation: Soll direkte Kommunikation deutlich zuverlässiger sein als nur Beobachtung, sodass … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 159 | Falsches Lernen: Soll ein NPC etwas falsch interpretieren können und später seine Annahme korrigieren, statt immer … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 160 | Experience Memory: Soll wichtige Erfahrung nicht nur als Zahl gespeichert werden, sondern mit Erinnerungen wie mit … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 161 | Preference Discovery: Soll eine unbekannte Vorliebe erst durch Fantasie, Gespräch, Beobachtung oder tatsächliche … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 162 | Preference Strength Change: Soll eine entdeckte Vorliebe nach mehreren Erfahrungen stärker, schwächer oder unverändert … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 163 | Partner Association: Soll eine Person bestimmte Dinge besonders stark mit einem konkreten Partner verbinden können, … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 164 | Habit Formation: Soll aus häufig wiederholten, positiv erlebten Mustern eine Gewohnheit entstehen können, die der NPC … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 165 | Boredom: Soll zu häufig exakt dasselbe bei manchen Personen irgendwann weniger interessant werden können, während … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 166 | Novelty Seeking: Soll Novelty Seeking – also Interesse an Neuem – eine eigene Persönlichkeitseigenschaft sein und nicht … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 167 | Mood Effects: Soll Stimmung kurzfristig beeinflussen, welche Art von Nähe/Intensität jemand gerade möchte, ohne seine … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 168 | Stress Effects: Soll Stress unterschiedliche Auswirkungen haben können – manche wollen dann gar nichts, andere suchen … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 169 | Relationship Conflict: Soll Streit oder Enttäuschung Attraction/Desire kurzfristig beeinflussen können, ohne dass das … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 170 | Jealousy Memory: Soll Eifersucht an konkrete Personen/Ereignisse gebunden sein, sodass sie nicht einfach als globaler … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 171 | NPC Suggestions: Soll der NPC selbst konkrete neue Dinge, Orte, Items oder Positionsideen vorschlagen können, basierend … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 172 | Context Awareness: Soll er dabei prüfen, was gerade tatsächlich möglich ist – vorhandener Platz, Kleidung, Items, … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 173 | Long-Term Personality Stability: Soll die Kernpersönlichkeit relativ stabil bleiben, während Preferences, Erfahrung, … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 174 | Save/Persistence: Soll all dieses Partnerwissen, Lernen und Preference-Development dauerhaft gespeichert werden, sodass … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |
| 175 | Modding API: Soll Mods neue Preference-Typen, Learning-Regeln oder NPC-Reaktionen registrieren dürfen, während der … | ja | 35#624 → 35#625 | 📋 ja | E-NP-029 |

## Runde 16H: UI, Einstellungen, Technik (35#626)

**Datum:** 2026-09-18 · **Fragen:** 176–200 (25) · **Antwortnachrichten:** 35#627 · **Module (Paketebene):** 04, 02 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-RP-008, E-ST-009, E-XX-011

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 176 | Adult-Mod-Ersteinrichtung: Soll beim ersten Start ein eigener Setup-Assistent erscheinen, der z. B. Detailgrad, … | ja | 35#626 → 35#627 | 📋 ja | – |
| 177 | Settings getrennt nach Bereichen: Soll es statt eines riesigen Menüs klare Kategorien geben wie Gameplay, Controls, … | ja | 35#626 → 35#627 | 📋 ja | – |
| 178 | Presets + Advanced: Soll es einfache Presets für normale Spieler geben, aber darunter weiterhin sehr detaillierte … | ja | 35#626 → 35#627 | 📋 ja | – |
| 179 | UI-Hilfen skalierbar: Soll man die UFC-artigen Action-/Combo-Hinweise auf mehreren Stufen anzeigen können: komplett … | ja | 35#626 → 35#627 | 📋 ja | – |
| 180 | Kontextmenü durchsuchen: Soll die verfügbare Action-/Position-Bibliothek während einer Session nach Kategorie, … | ja | 35#626 → 35#627 | 📋 ja | – |
| 181 | Favoriten: Soll man bestimmte Actions, Position-Hints oder Kombinationen favorisieren und schneller aufrufen können, … | das weiß ich noxh nicht aber ja glaube schon das das gut wäre | 35#626 → 35#627 | 📋 Freitext | – |
| 182 | Eigene Kombinationen speichern: Soll eine frei entstandene Kombination aus Pose, Action und groben Parametern als … | ja | 35#626 → 35#627 | 📋 ja | – |
| 183 | Statistics Events: Soll das globale Lifetime-Statistiksystem relevante Adult-Ereignisse über denselben zentralen … | ja | 35#626 → 35#627 | 📋 ja | – |
| 184 | Statistik-Privatsphäre: Soll Adult-Statistik im normalen Hauptspiel-Menü nur sichtbar sein, wenn die Adult-Mod aktiv … | ja | 35#626 → 35#627 | 📋 ja | – |
| 185 | Detailgrad der Statistik: Soll die Statistik später sowohl allgemeine Werte als auch sehr detaillierte Unterkategorien … | ja | 35#626 → 35#627 | 📋 ja | – |
| 186 | Replay automatisch referenzieren: Soll eine gespeicherte Session auf Wunsch direkt mit einem Replay-/Capture-Eintrag … | ja | 35#626 → 35#627 | 📋 ja | E-RP-008 |
| 187 | Replay ≠ Savegame: Soll ein Replay technisch klar vom normalen Savegame getrennt sein? Savegame hält den aktuellen … | ja | 35#626 → 35#627 | 📋 ja | E-RP-008 |
| 188 | Deterministische Kernspur: Soll das Replay eine feste Kernspur speichern – Inputs, Actions, Pose-/Contact-Zustände, … | ja | 35#626 → 35#627 | 📋 ja | E-RP-008 |
| 189 | Cinematic Re-Render separat: Soll ein Replay später mit höherer Renderqualität neu berechnet werden dürfen, während der … | ja | 35#626 → 35#627 | 📋 ja | E-RP-008 |
| 190 | Background Render: Soll Re-Render weiterhin als Job im Hintergrund laufen können, mit Prioritätsregel Gameplay zuerst, … | ja | 35#626 → 35#627 | 📋 ja | E-RP-008 |
| 191 | VR nutzt denselben Core: Soll VR weiterhin ausschließlich eine zusätzliche Input-/Presentation-Schicht sein und … | ja | 35#626 → 35#627 | 📋 ja | – |
| 192 | Partial Body Tracking: Soll VR bereits mit Headset + Händen funktionieren, aber später optional zusätzliche Tracker für … | ja | 35#626 → 35#627 | 📋 ja | – |
| 193 | VR Intent Prediction: Soll der Solver bei unvollständigem Tracking fehlende Körperbewegungen sinnvoll ableiten? … | ja | 35#626 → 35#627 | 📋 ja | – |
| 194 | Multiplayer optional vorbereiten: Auch wenn Adult zunächst primär Singleplayer ist: Soll die Architektur vermeiden, … | ja | 35#626 → 35#627 | 📋 ja | – |
| 195 | Network Authority: Falls später Multiplayer unterstützt wird, soll der autoritative Server nur relevante … | ja | 35#626 → 35#627 | 📋 ja | – |
| 196 | Privacy im Multiplayer: Soll Adult-Funktionalität in Multiplayer-Servern explizit vom Server/Host aktivierbar sein und … | ja | 35#626 → 35#627 | 📋 ja | – |
| 197 | Savegame-Versionierung: Soll jeder Adult-State eine Schema-/Versionsnummer besitzen, damit spätere Updates alte Saves … | ja | 35#626 → 35#627 | 📋 ja | – |
| 198 | Modding Registry: Soll Mods neue Kategorien, Preferences, Items, Pose-Hints, Reaktionen, Audio-/Visual-Profile und … | ja | 35#626 → 35#627 | 📋 ja | – |
| 199 | Mod-Abhängigkeiten und Konflikte: Soll der Mod Manager erkennen können, wenn zwei Adult-Erweiterungen dieselbe … | ja | 35#626 → 35#627 | 📋 ja | – |
| 200 | Debug/Developer Layer: Soll es für Entwicklung einen umfassenden Inspector geben, der live Pose, Contact Graph, Intent, … | [ja …; Details explizit, nicht wiedergegeben] | 35#626 → 35#627 | 📋 ja mit Zusatz | – |

## Runde 16I: Registry I (35#642)

**Datum:** 2026-09-20 · **Fragen:** 201–225 (25) · **Antwortnachrichten:** 35#643 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-030

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 201 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 202 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 203 | Aktion und Ziel trennen: Soll jede konkrete Aktion grundsätzlich aus Action Type + Target Region + Tool/Body Part + … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 204 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 205 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 206 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 207 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 208 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 209 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 210 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 211 | Size/Load-Tags: Soll jedes Item/Interaction zusätzliche Tags wie normal, large, extreme size, high load, high stretch … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 212 | Impact-Unterteilung: Soll Impact nach hand, flexible implement, rigid implement, surface impact usw. getrennt werden, … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 213 | Schmerz separat: Soll pain weiterhin keine eigene Hauptkategorie, sondern ein Outcome-/Sensation-Tag sein, das bei … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 214 | Restraint-Unterteilung: Soll Restraint mindestens in soft restraint, rigid restraint, body-position restraint, … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 215 | Physical Control: Soll Physical Control als eigene Unterfamilie existieren, wenn eine Person durch Körpergewicht/Kraft … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 216 | Sensory-Unterteilung: Soll Sensory nach vision, sound, temperature, texture/material, smell, pressure/touch pattern … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 217 | Roleplay-Taxonomie: Soll Roleplay eher über Scenario Tags + Rollen + vereinbarte Regeln beschrieben werden, statt … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 218 | Group-Taxonomie: Soll Group nur ein Participant-/Relationship-Layer sein, während die tatsächlichen Actions weiterhin … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 219 | Solo-Kategorie: Soll Solo-Verhalten dieselbe Action Registry benutzen, nur mit einem Teilnehmer und ggf. … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 220 | Observation/Recording: Soll watching, being watched, private recording, creator recording, professional production, … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 221 | Professional/Service Context: Soll ein kommerzieller Kontext nur als zusätzlicher Contract/Service Layer auf normalen … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 222 | Location-Tags in Registry: Soll jede Action nur Kompatibilitätsanforderungen an Locations haben, z. B. needs support … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 223 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry I] | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 224 | Risk-Tags: Soll jeder Eintrag ebenfalls mögliche Risiken deklarieren können, z. B. joint strain, soft-tissue strain, … | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |
| 225 | Registry-Schema: Soll ein Eintrag ungefähr diese Struktur besitzen: | ja genau aber die kategorien waren nur ungefähr oder | 35#642 → 35#643 | 📋 ja mit Zusatz | E-IA-030 |

## Runde 16J: Unterkategorien (35#650)

**Datum:** 2026-09-20 · **Fragen:** 226–250 (25) · **Antwortnachrichten:** 35#651 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-031

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 226 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 227 | Touching: Soll Touch nach Kontaktart getrennt werden, z. B. leichtes Streichen, Greifen, Halten, Drücken, Reiben und … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 228 | Manual external: Soll manuelle äußere Stimulation eine eigene Untergruppe besitzen, getrennt nach Zielregion, … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 229 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 230 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 231 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 232 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 233 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 234 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 235 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 236 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 237 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 238 | Expandable items: Soll es eine eigene Functional Tag-Gruppe für expandierbare Items geben, weil deren Größe während der … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 239 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 240 | Impact by hand: Soll Hand-Impact, z. B. Spanking, als eigener Subtype existieren, weil Kontaktfläche, Hautreaktion und … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 241 | Impact implements: Soll Impact mit Gegenständen nach Flexibilität, Masse, Kontaktfläche und Material klassifiziert … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 242 | Scratching / nails: Soll Kratzen als eigener Contact-Subtype existieren, mit Nagellänge, Schärfe, Druck und Bewegung, … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 243 | Biting: Soll Beißen als eigener Mouth-Contact-Subtype existieren, mit Zielregion, Druck und Dauer und damit getrennten … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 244 | Holding / grabbing: Soll festes Greifen/Halten eine eigene Physical-Control-Unterart sein, die normale Hand-/Armkräfte … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 245 | Body-weight control: Soll zusätzlich Body Weight Control existieren, also Situationen, in denen ein großer Teil des … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 246 | Restraint items: Soll Restraint mit Gegenständen nach soft, rigid, adjustable, fixed-to-environment usw. klassifiziert … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 247 | Sensory vision: Soll Sicht-Einschränkung als eigene Sensory-Unterart existieren, die tatsächlich Perception/Gaze … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 248 | Sensory temperature/material: Soll Temperatur-/Materialstimulation über normale Materialparameter laufen und als Tags … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 249 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Unterkategorien] | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |
| 250 | Unterkategorie-Regel: Soll jede Unterkategorie nur dann separat existieren, wenn sie technisch oder für … | [ja …; Details explizit, nicht wiedergegeben] | 35#650 → 35#651 | 📋 ja mit Zusatz | – |

## Runde 16K: Reaktions-/Outcome-Familien (35#652)

**Datum:** 2026-09-20 · **Fragen:** 251–275 (25) · **Antwortnachrichten:** 35#653 · **Module (Paketebene):** 06, 03 · **Status:** ❓ 1, 📋 24  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-031, E-KO-023, E-XX-015

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 251 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 252 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja aber kann beider gleichzeitig sein | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 253 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 254 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 255 | Vomiting Amount: Soll Menge variieren können, abhängig von Magenzustand, Essen/Trinken und vorherigen Episoden? | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 256 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 257 | Regurgitation: Soll Regurgitation – Hochkommen von Magen-/Speiseröhreninhalt ohne vollständigen Brechvorgang – separat … | was genau ist gemeint | 35#652 → 35#653 | ❓ offen/Rückfrage | E-KO-023 |
| 258 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 259 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja das ist ganz anders, da es meistens mit der hand oder arm die luft abgedrückt wird | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 260 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 261 | Gasps: Soll plötzliches Luftschnappen/Gasping als eigenständige Atemreaktion existieren? | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 262 | Tears: Soll Tränenbildung getrennte Ursachen speichern können, z. B. Reflex, Schmerz, Emotion oder Augenreizung? | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 263 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 264 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 265 | Sweating: Soll Schweiß abhängig von Anstrengung, Raumtemperatur, Stress und Person unterschiedlich stark entstehen? | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 266 | Skin flushing: Soll Hautrötung durch Durchblutung/Anstrengung getrennt von Rötung durch Reibung oder Impact existieren? | ja und das kann durch vieles passieren durch hauen kann die haut anschwellen oder sogar aufreißen und so weiter | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 267 | Pain vocalization: Soll Schmerzlautäußerung separat von Lust-/Anstrengungslauten getrackt werden, auch wenn sie … | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 268 | Pleasure vocalization: Soll positive Lautreaktion wiederum ein eigenes Profil besitzen – Lautstärke, Tonhöhe, … | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 269 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 270 | Moaning: Soll Stöhnen ebenfalls ein allgemeiner Voice-Reaction-Typ sein, der positive, anstrengende oder gemischte … | ja | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 271 | Muscle spasms: Soll es unwillkürliche Muskelkontraktionen/Spasmen als eigenes Reaction-Event geben? | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 272 | Trembling: Soll Zittern getrennt werden in Ermüdung, Adrenalin/Stress und starke körperliche Reaktion, weil die … | ja noch viel genauer | 35#652 → 35#653 | 📋 ja | E-KO-023 |
| 273 | Freeze/Stillness: Soll plötzliches Stillwerden als eigene Reaction-State-Kategorie existieren und nicht automatisch … | ja kann beides sein, durch schmerz oder durch orgasmus oder so | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 274 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | ja sehr abgetrennt das alle eine andere konsistenz haben und unterschiedlich viel im körper existiert | 35#652 → 35#653 | 📋 ja mit Zusatz | E-KO-023 |
| 275 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Reaktions-/Outcome-Familien] | [ja …; Details explizit, nicht wiedergegeben] | 35#652 → 35#653 | 📋 ja mit Zusatz · K2 | E-KO-023 |

## Runde 16L: Empfindung, Erholung (35#676)

**Datum:** 2026-09-20 · **Fragen:** 276–300 (25) · **Antwortnachrichten:** 35#677 · **Module (Paketebene):** 06 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-024

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 276 | Lokale Pleasure-Map: Soll jede relevante Körperregion eine eigene Empfindlichkeits-/Pleasure-Kurve besitzen, statt … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 277 | Pain/Pleasure gleichzeitig: Soll dieselbe Region gleichzeitig Schmerz und positive Empfindung erzeugen können, wobei … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 278 | Sensitization: Soll Sensitivierung existieren – also dass ein Bereich nach längerer Stimulation vorübergehend … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 279 | Desensitization: Soll umgekehrt Desensitivierung möglich sein – nach langer/repetitiver Belastung reagiert ein Bereich … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 280 | Overstimulation: Soll es einen Zustand Überstimulation geben, bei dem eigentlich positive Reize zunehmend unangenehm … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 281 | Mixed-State-Reactions: Soll ein NPC gleichzeitig positive Reaktion, Schmerz, Anstrengung und Stress zeigen können, … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 282 | Orgasm Trigger Model: Soll ein Orgasmus aus mehreren Faktoren entstehen – lokale Stimulation, mentale Erregung, … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 283 | Unterschiedliche Orgasmus-Profile: Soll jede Person unterschiedliche Reaktionsmuster haben können, z. B. Stärke, Dauer, … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 284 | Multiple Orgasms: Soll Mehrfachorgasmus stark individuell sein und von Physiologie, Tagesform und aktueller Belastung … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 285 | Refractory Period: Soll eine Refraktärphase simuliert werden – also die Zeit nach einem Orgasmus, in der erneute starke … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 286 | Afterglow: Soll es einen Afterglow-State geben, also einen vorübergehenden Zustand aus Entspannung, Nähe, Müdigkeit … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 287 | Physical Fatigue: Soll körperliche Ermüdung getrennt nach Muskelgruppen entstehen, sodass z. B. Beine erschöpft sein … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 288 | Cramps: Soll Muskelkrampf ein eigener Outcome sein, abhängig von Belastung, Haltung, Dauer, Flüssigkeitshaushalt und … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 289 | Postural Fatigue: Soll langes Halten einer ungewöhnlichen Pose zu Haltungs-/Gelenkermüdung führen, auch wenn die … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 290 | Soreness: Soll Soreness – also länger anhaltende Druck-/Muskel-/Gewebeempfindlichkeit ohne schwere Verletzung – Stunden … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 291 | Swelling: Soll Schwellung als eigener persistenter Zustand existieren, getrennt von Rötung, Schmerz und Verletzung? | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 292 | Bruising: Soll ein Bluterguss einen echten zeitlichen Verlauf haben – Entstehung, Farbveränderung, Abbau – und über … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 293 | Minor Surface Injury: Soll das Skin-System kleine oberflächliche Schäden wie Reizungen oder Kratzer getrennt von … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 294 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Empfindung, Erholung] | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 295 | Daily-Life Effects: Sollen stärkere Nachwirkungen später normales Gameplay beeinflussen können – Sitzen, Gehen, Rennen, … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 296 | Self-Care: Soll ein Charakter kleinere Beschwerden selbst behandeln können, z. B. Ruhe, Dusche, Kühlung/Wärme oder … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 297 | Medical Escalation: Soll das System erkennen, wann ein Zustand nicht mehr normale Recovery ist und echte medizinische … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 298 | Recovery Prediction: Soll der Developer Inspector eine grobe geschätzte Heil-/Recovery-Zeit anzeigen können, während … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 299 | Persistent History: Soll der Body-State relevante frühere Verletzungen/Heilungsverläufe speichern, damit wiederholt … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |
| 300 | Unified Outcome Layer: Soll Pleasure + Pain + Fatigue + Soreness + Injury + Recovery + Orgasm + After-State als … | ja | 35#676 → 35#677 | 📋 ja | E-KO-024 |

## Runde 16M: Registry II (35#678)

**Datum:** 2026-09-20 · **Fragen:** 301–325 (25) · **Antwortnachrichten:** 35#679 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-032

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 301 | Kissing: Soll Küssen mindestens getrennt werden in lip kiss, deep/french kiss, neck kiss, body kiss und zielbezogene … | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 302 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 303 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja aber auch die tiefe und so weiter | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 304 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 305 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 306 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 307 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 308 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 309 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 310 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 311 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 312 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 313 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 314 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 315 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 316 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 317 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 318 | External toys: Soll äußerlich verwendetes Toy-Gameplay eigene Funktionstypen wie vibration, pressure, movement, … | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 319 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 320 | Body-part contact: Soll sexuelle Nutzung anderer Körperbereiche — z. B. Brust, Oberschenkel, Füße, Hände usw. — über … | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 321 | Foot-focused: Soll Fuß-/Toe-bezogener Content zusätzlich eigene Preferences/Stats bekommen können, obwohl die … | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 322 | Breast-focused: Soll Brust-bezogener Content ebenfalls als eigener Preference-/Target-Bereich klassifiziert werden … | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 323 | Buttocks-focused: Soll Gesäß/Buttocks als eigener Target-Bereich existieren, damit Touch, Spanking, Kissing, … | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 324 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja | 35#678 → 35#679 | 📋 ja | E-IA-032 |
| 325 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Registry II] | ja Ich gehe jetzt schlafen was noch bei runde 16 und wie viele fragen noch für adult | 35#678 → 35#679 | 📋 ja mit Zusatz | E-IA-032 |

## Runde 16N: Höhepunkt, Kleidung, Solo (35#686)

**Datum:** 2026-09-23 · **Fragen:** 326–350 (25) · **Antwortnachrichten:** 35#687 · **Module (Paketebene):** 03, 06 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-033, E-IA-034

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 326 | Climax als Event statt Scene-Ende: Soll ein Orgasmus nur ein Event innerhalb der laufenden Session sein und die Szene … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 327 | Mehrere Climax-Events: Soll dieselbe Person innerhalb einer Session mehrere getrennte Orgasmus-Events haben können, … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 328 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Höhepunkt, Kleidung, Solo] | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 329 | Ejaculate Amount: Soll die Menge individuell variieren und von Physiologie, Zeit seit dem letzten Ereignis, Hydration … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 330 | Target Region: Soll bei Flüssigkeits-/Finish-Events exakt gespeichert werden, welche Körperregion, Kleidung, Oberfläche … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 331 | Internal vs. External: Soll zwischen internem und äußerem Fluid-State unterschieden werden, weil Persistence, Cleanup, … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 332 | Multiple Surfaces: Soll ein einzelnes Fluid-Event mehrere Oberflächen gleichzeitig treffen können, z. B. Haut + … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 333 | Fluid Transfer: Soll Flüssigkeit durch späteren Kontakt von einer Oberfläche auf eine andere übertragen werden können, … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 334 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Höhepunkt, Kleidung, Solo] | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 335 | Drying: Soll Flüssigkeit mit der Zeit trocknen bzw. ihren sichtbaren/materialtechnischen Zustand verändern können? | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 336 | Cleanup: Soll Reinigen über normale World-Actions funktionieren – Dusche, Handtuch, Waschmaschine, Oberflächen reinigen … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 337 | Clothing Layer Interaction: Soll Flüssigkeit nur die tatsächlich exponierte Kleidungsschicht erreichen, sodass mehrere … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 338 | Clothing Displacement: Soll bei Adult-Actions Kleidung weiterhin teilweise verschoben, geöffnet oder hochgeschoben sein … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 339 | Clothing Damage: Soll Kleidung bei hoher mechanischer Belastung beschädigt werden können, wenn Material und Kraft das … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 340 | Material Feel: Soll Kleidungsmaterial zusätzlich Reibung, Wärme, Druckverteilung und Haptik beeinflussen, sodass z. B. … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 341 | Nudity State ohne Scene Mode: Soll Nacktheit einfach ein normaler Clothing/Visibility-State des Charakters sein und … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 342 | Solo Actions: Soll Solo-Verhalten dieselben Action Types wie Partnerinteraktionen verwenden können, soweit … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 343 | Solo Flexibility Limits: Soll bei Solo-Actions dieselbe individuelle Flexibilität/Biomechanik gelten, sodass manche … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 344 | Solo Items: Soll ein NPC/Spieler vorhandene Items auch solo über dieselbe Item-/Contact-Engine verwenden können? | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 345 | Furniture-assisted Solo: Soll Furniture/Environment auch bei Solo-Verhalten als Interaction Surface funktionieren, ohne … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 346 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Höhepunkt, Kleidung, Solo] | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 347 | Media + Solo Context: Soll Adult-Medienkonsum als möglicher Context-Tag einer Solo-Session existieren, ohne anzunehmen, … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 348 | Body-to-body Families: Fehlt uns als eigene Registry-Familie Body-to-body contact, also Aktionen, bei denen größere … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 349 | Hair/Grip/Body Control Tags: Sollen Dinge wie Haare halten, Körperregionen greifen oder Körperposition führen als … | ja | 35#686 → 35#687 | 📋 ja | E-IA-033 |
| 350 | Missing-Family Audit I: Soll nach diesem Block ausdrücklich ein automatischer/manuel­ler Audit prüfen, ob uns noch … | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#686 → 35#687 | 📋 Rückfrage → pauschal bestätigt (35#691) | – |

## Runde 16O: Orte (35#692)

**Datum:** 2026-09-23 · **Fragen:** 351–375 (25) · **Antwortnachrichten:** 35#693 · **Module (Paketebene):** 08, 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** –

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 351 | Location ist kein Scene-Type: Soll Wohnung, Hotel, Auto, Wald usw. nur den Umgebungskontext liefern, während dieselbe … | ja | 35#692 → 35#693 | 📋 ja | – |
| 352 | Private Wohnungen: Soll praktisch jeder geeignete Raum einer Wohnung funktionieren können – Schlafzimmer, Wohnzimmer, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 353 | Schlafzimmer: Soll Bettgröße, Matratzenart, Bettgestell, Wandabstand und verfügbare Fläche tatsächlich beeinflussen, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 354 | Badezimmer/Dusche: Soll Wasser, rutschiger Boden, enge Dusche, Badewanne und Waschbecken über normale … | ja | 35#692 → 35#693 | 📋 ja | – |
| 355 | Hotels/Motels: Sollen Hotels unterschiedliche Zimmergrößen, Bettarten, Schallschutz, Qualität und Privatsphäre … | ja aber das haben wir ja schon gesagt das die assets nicht gleich sein sollen und so weiter | 35#692 → 35#693 | 📋 ja mit Zusatz | – |
| 356 | Fahrzeuge: Soll jedes Fahrzeug anhand seines tatsächlichen Innenraums funktionieren – Sitzabstände, Dachhöhe, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 357 | Unterschiedliche Fahrzeugtypen: Soll ein großer SUV deutlich mehr Möglichkeiten bieten als ein kleiner Sportwagen, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 358 | Outdoor: Soll Outdoor praktisch überall möglich sein, wenn Terrain und Körperkonfiguration es zulassen – Wald, Wiese, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 359 | Untergrund: Soll Gras, Sand, Schlamm, Schnee, Stein, Asphalt usw. über Härte, Reibung, Temperatur, Nässe und Sauberkeit … | ja | 35#692 → 35#693 | 📋 ja | – |
| 360 | Wetter: Soll Regen, Schnee, Hitze, Kälte und Wind die Situation real beeinflussen – Grip, Körpertemperatur, nasse … | ja | 35#692 → 35#693 | 📋 ja | – |
| 361 | Tageszeit: Soll Nacht nicht einfach automatisch „unsichtbar“ bedeuten, sondern tatsächliche Beleuchtung, Mond, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 362 | Privacy Score: Soll jeder Ort dynamisch einen Privacy State haben, der aus Türen, Fenstern, Sichtlinien, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 363 | Sound propagation: Soll Sprache, Geräusche und Schreie über das allgemeine Audio-/World-System durch Türen, Wände und … | ja | 35#692 → 35#693 | 📋 ja | – |
| 364 | Witness Detection: Soll Beobachtung wirklich aus Line of Sight + Hearing + Aufmerksamkeit entstehen und nicht aus einem … | ja | 35#692 → 35#693 | 📋 ja | – |
| 365 | NPC-Schedules: Soll ein eigentlich privater Ort plötzlich weniger privat werden können, weil Mitbewohner, Familie, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 366 | Locks/Doors: Soll abschließen, Tür offen lassen, Vorhänge schließen usw. echte World Actions sein, die Privacy … | ja | 35#692 → 35#693 | 📋 ja | – |
| 367 | Adult Venues: Sollen Stripclubs, Bordelle, Swinger-/Adult-Clubs und Studios jeweils eigene Raumtypen und Hausregeln … | ja | 35#692 → 35#693 | 📋 ja | – |
| 368 | Professional Studios: Soll ein Studio zusätzlich Kamera-, Licht-, Set-, Sound- und Crew-Infrastruktur besitzen, ohne … | ja | 35#692 → 35#693 | 📋 ja | – |
| 369 | Workplace: Soll es grundsätzlich auch an normalen Arbeitsorten möglich sein, sofern die Situation entsteht, aber mit … | ja | 35#692 → 35#693 | 📋 ja | – |
| 370 | Ungewöhnliche Orte: Soll die Engine auch Dinge ermöglichen, die wir nie als Content-Preset vorgesehen haben – z. B. … | ja | 35#692 → 35#693 | 📋 ja | – |
| 371 | Surface Registry: Soll World-Geometry optional Surface Metadata besitzen wie sit, lean, support, soft, grippable, … | ja | 35#692 → 35#693 | 📋 ja | – |
| 372 | Breakable Environment: Soll hohe Belastung auch in Adult-Situationen normale zerstörbare Weltobjekte beschädigen können … | ja | 35#692 → 35#693 | 📋 ja | – |
| 373 | Cleanup/Persistence: Soll zurückgelassene Kleidung, beschädigte Möbel, verschmutzte Oberflächen usw. nach der Session … | ja | 35#692 → 35#693 | 📋 ja | – |
| 374 | Location Memories: Soll ein NPC Erinnerungen mit konkreten Orten verknüpfen können, sodass ein Hotelzimmer, eine … | ja | 35#692 → 35#693 | 📋 ja | – |
| 375 | Location-System-Regel: Soll die finale Regel sein: Adult braucht keine speziell gebauten Adult-Räume; normale … | ja | 35#692 → 35#693 | 📋 ja | – |

## Runde 16P: Kombinierte Aktionen (35#694)

**Datum:** 2026-09-23 · **Fragen:** 376–400 (25) · **Antwortnachrichten:** 35#695 · **Module (Paketebene):** 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-036, E-PR-007, E-SO-036

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 376 | Mehrere Actions gleichzeitig: Soll eine Session mehrere Actions parallel laufen lassen können, solange … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 377 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Kombinierte Aktionen] | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 378 | Action Layers: Soll jede Action auf einer eigenen Layer laufen können, z. B. Hauptbewegung + Handkontakt + Küssen + … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 379 | Wechsel ohne Reset: Soll der Übergang von einer Action zur nächsten ohne Scene-Neustart oder Snap funktionieren? | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 380 | Teilweiser Wechsel: Soll man nur einen Teil der Interaction ändern können, während andere Actions weiterlaufen? | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 381 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Kombinierte Aktionen] | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 382 | NPC übernimmt Teilaktion: Soll ein NPC eine laufende Teilaktion übernehmen können, während der Spieler eine andere … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 383 | Beide initiieren gleichzeitig: Soll es möglich sein, dass Spieler und NPC gleichzeitig unterschiedliche neue Actions … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 384 | Conflicting Intent: Soll der Solver erkennen, wenn zwei Bewegungsabsichten physisch kollidieren, und daraus natürliche … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 385 | Handwechsel: Soll eine Hand ihre Aufgabe verlieren/wechseln können, ohne dass die komplette Pose neu berechnet werden … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 386 | Grip transfer: Soll Grip Transfer möglich sein – also Halten/Abstützen von einer Hand/Fläche auf eine andere übergeben? | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 387 | Item transfer: Soll ein Item während derselben Session zwischen Händen, Personen oder Positionen weitergegeben werden … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 388 | Item drop: Soll ein Item real herunterfallen können, wenn Grip verloren geht, statt automatisch im Inventar zu landen? | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 389 | Environment interruption: Soll die Welt eine Session beeinflussen können, z. B. Klingeln, NPC kommt rein, Stromausfall, … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 390 | Pause ohne Ende: Soll eine Session kurz pausieren können – reden, trinken, Tür schließen, Item holen – und danach … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 391 | Clothing during interaction: Soll Kleidung während laufender Actions weiter verändert werden können, statt nur … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 392 | Injury interruption: Soll eine neue Verletzung laufende Actions dynamisch verändern können, ohne automatisch die ganze … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 393 | Fatigue adaptation: Soll zunehmende Müdigkeit automatisch kleinere Bewegungen, langsamere Übergänge oder andere … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 394 | Cramp edge case: Soll ein Muskelkrampf eine laufende Pose plötzlich ungültig machen können, sodass der Solver eine … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 395 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Kombinierte Aktionen] | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 396 | Space conflict: Soll die Engine erkennen, wenn eine neue Action wegen Wand, Möbel, Fahrzeugdach oder anderem … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 397 | Self-collision: Soll Selbstkollision zwischen Armen, Beinen, Torso usw. ernsthaft berücksichtigt werden, damit extreme … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 398 | Multi-body collision: Soll bei mehreren Personen jeder relevante Körper mit den anderen kollidieren können, aber mit … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 399 | Failure states: Soll eine Action scheitern können, weil Pose, Kraft, Flexibilität, Item-Geometrie oder Umgebung nicht … | ja | 35#694 → 35#695 | 📋 ja | E-IA-036, E-SO-036 |
| 400 | Edge-case principle: Soll generell gelten: Wenn eine ungewöhnliche Kombination technisch nicht explizit geplant wurde, … | ja Aber ich finde diese sachen sind schon zu genau geplant das juckt keinen so richtig. Einfsch so wie in echt und passt schon … | 35#694 → 35#695 | 📋 ja mit Zusatz | E-IA-036, E-SO-036 |

## Runde 16Q: Risiko, Körperzustand (35#706)

**Datum:** 2026-09-24 · **Fragen:** 401–425 (25) · **Antwortnachrichten:** 35#707 · **Module (Paketebene):** 06 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-027, E-SK-035

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 401 | Risk ist kein Verbot | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 402 | Risk pro Action + aktuelle Situation | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 403 | Risk Profile statt Einzelwert | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 404 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Risiko, Körperzustand] | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 405 | Adaptation wirkt lokal | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 406 | Experience ≠ Resistance | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 407 | Pain Tolerance separat | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 408 | Injury Threshold ist dynamisch | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 409 | Minor → Major Escalation | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 410 | Keine Session-Heilung | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 411 | Langfristige Folgen nur wenn plausibel | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 412 | Visual Damage entspricht Body State | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 413 | Health State beeinflusst AI | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 414 | Preference kann Risiko überwiegen | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 415 | Medical Awareness unterschiedlich | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 416 | Severe State → normales Medical System | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 417 | Emergency Detection | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 418 | NPC-Reaktion auf Injury individuell | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 419 | Environment Risk | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 420 | Item Risk Metadata | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 421 | Risk UI standardmäßig subtil | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 422 | Developer Health Inspector | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 423 | Replay speichert Health-Ereignisse | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 424 | Mods erweitern Risiken datengetrieben | ja | 35#706 → 35#707 | 📋 ja | E-KO-027 |
| 425 | Unified Risk Pipeline | ja ich glaube das meiste passt schon, weil adult ist schon sehr langwierig, die fragen sollten reduziert werden glaub ich | 35#706 → 35#707 | 📋 ja mit Zusatz | E-KO-027 |

## Runde 16R: Produktions-Locks (35#712)

**Datum:** 2026-09-25 · **Fragen:** 426–437 (12) · **Antwortnachrichten:** 35#713, 35#715 · **Module (Paketebene):** 10, 02 · **Status:** 📋 12  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-037, E-PA-032, E-PA-033, E-PR-009, E-PR-010, E-PR-011, E-RC-004, E-SK-031, E-SK-032

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 426 | Must-have vs. Late-Stage: Sollen wir jede Adult-Funktion in Core, Important, Late Polish einordnen, damit Claude später … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 427 | Realism Default: Soll endgültig gelten: Alles, was nicht ausdrücklich anders definiert wurde, verhält sich im … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 428 | Category Registry Freeze: Soll die genaue endgültige Kategorien-/Unterkategorienliste erst nach unserem … | ja das stelle ich am sonntag auf oder später, aber wir gehen trotzdem schon davor weiter (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) · OE-82 | E-IA-037 |
| 429 | Data-driven: Soll praktisch jede Kategorie, Preference, Reaction, Item-Art und Risk-Zuordnung datengetrieben … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 430 | Shared Systems: Soll Adult konsequent keine eigenen Kopien von Human, Health, Physics, Clothing, Audio, Memory, World … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 431 | Release Scope: Soll der normale Release zunächst lieber einen kleineren, technisch sehr hochwertigen Adult-Core … | ja sehr früh implementiert und dann sehr viel erweitert (35#713) → [ja …; Details explizit, nicht wiedergegeben] (35#715) | 35#712 → 35#713,35#715 | 📋 ja mit Zusatz (nachgereicht/geändert) · K19 | E-IA-037 |
| 432 | Content Settings: Soll man größere Content-Bereiche modular aktivieren/deaktivieren können, ohne dass Savegames oder … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 433 | Developer Inspector: Soll unser Dev-Build einen zentralen Adult-Debugger bekommen, der Interaction Graph, Body State, … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 434 | Automated QA: Sollen wir automatische Tests erzeugen, die tausende zufällige Körper-, Pose-, Item- und … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 435 | Performance Budget: Soll es feste Budgets für CPU/GPU/Memory pro aktivem High-Fidelity-Charakter geben, sodass die … | ja, aber ich habe erst vor kurzem battlefield von 2009 gesehen und das hat wunderbar ausgeschaut und ist auf 4gb ram gelaufen, … (35#713) → ja genau, man darf nicht erwarten das jeder eine 5090 mit 64gb ram hat, wenn das spiel spruchreif ist also in mehreren jahren (35#715) | 35#712 → 35#713,35#715 | 📋 ja mit Zusatz (nachgereicht/geändert) | E-IA-037 |
| 436 | Persistence: Soll wirklich jeder langfristig relevante Adult-State — Preferences, Experience, Memories, Body State, … | ja (35#713) → ja (35#715) | 35#712 → 35#713,35#715 | 📋 ja (nachgereicht/geändert) | E-IA-037 |
| 437 | Production Gate: Soll Runde 16 als technisch ausreichend abgeschlossen gelten, wenn nach unserem Audit keine große … | ja (35#713) → ja das es battlefield leichter hat ist klar aber trotzdem dürfen wir das nicht vernachlässigen. wie ist das jetzt aber mit dem … (35#715) | 35#712 → 35#713,35#715 | 📋 ja mit Zusatz (nachgereicht/geändert) | E-IA-037 |
