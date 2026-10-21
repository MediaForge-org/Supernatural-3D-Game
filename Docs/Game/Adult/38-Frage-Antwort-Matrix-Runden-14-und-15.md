# Adult 38: Frage-Antwort-Matrix, Runden 14 und 15

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

- [Runde 14: Körper, Tattoos (35#499)](#runde-14-körper-tattoos-35499)
- [Runde 14B: Piercings, Körpermodifikation (35#511)](#runde-14b-piercings-körpermodifikation-35511)
- [Runde 15A: Präferenzmodell (35#523)](#runde-15a-präferenzmodell-35523)
- [Runde 15B: Gruppen, Branche, Medien (35#525)](#runde-15b-gruppen-branche-medien-35525)
- [Runde 15C: Orte, Öffentlichkeit, Produktion (35#527)](#runde-15c-orte-öffentlichkeit-produktion-35527)
- [Runde 15D: Praktiken-Präferenzen, Fesseln, Rollenspiel (35#529)](#runde-15d-praktiken-präferenzen-fesseln-rollenspiel-35529)
- [Runde 15E: Solo, Medien, Dienstleistung (35#539)](#runde-15e-solo-medien-dienstleistung-35539)
- [Runde 15F: Gesundheit, Hygiene (35#541)](#runde-15f-gesundheit-hygiene-35541)
- [Runde 15G: Orientierung, Dynamik (35#549)](#runde-15g-orientierung-dynamik-35549)
- [Runde 15H: Kompatibilität, Statistik (35#551)](#runde-15h-kompatibilität-statistik-35551)

---


## Runde 14: Körper, Tattoos (35#499)

**Datum:** 2026-09-14 · **Fragen:** 1–100 (100) · **Antwortnachrichten:** 35#500, 35#502, 35#504 · **Module (Paketebene):** 06, 07 · **Status:** 📋 100  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-013, E-KO-014, E-KO-015, E-KO-016, E-KO-017, E-KO-018, E-KO-019, E-TA-004, E-TA-005, E-TA-006, E-TA-007, E-TA-008, E-TA-009, E-TA-027

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll der Human Generator grundsätzlich jeden Körper als individuell parametrisierten Menschen erzeugen statt aus … | ja | 35#499 → 35#500 | 📋 ja | – |
| 2 | Soll Körpergröße wirklich kontinuierlich sein, also nicht nur Small / Medium / Tall? | ja | 35#499 → 35#500 | 📋 ja | – |
| 3 | Soll Schulterbreite unabhängig von Körpergröße variieren können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 4 | Soll Beckenbreite ebenfalls unabhängig variieren? | manche Frauen haben ein breiteres becken, manche haben auch hip dips oder so was anderes | 35#499 → 35#500 | 📋 Freitext | – |
| 5 | Soll Torso-Länge unabhängig von Beinlänge sein? | ja | 35#499 → 35#500 | 📋 ja | – |
| 6 | Soll Armspannweite von Körpergröße abweichen können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 7 | Soll jedes einzelne Bein minimal unterschiedlich lang sein können, wie wir schon angesprochen haben? | ja | 35#499 → 35#500 | 📋 ja | – |
| 8 | Dasselbe auch bei Armen? | ja | 35#499 → 35#500 | 📋 ja | – |
| 9 | Soll Fußgröße links/rechts leicht unterschiedlich sein können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 10 | Soll auch Handgröße links/rechts minimal abweichen können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 11 | Soll Muskelmasse pro Körperregion unterschiedlich verteilt sein? | ja | 35#499 → 35#500 | 📋 ja | – |
| 12 | Also z. B. starke Beine, aber relativ schwacher Oberkörper? | ja | 35#499 → 35#500 | 📋 ja | – |
| 13 | Soll Fettverteilung ebenfalls individuell sein? | ja | 35#499 → 35#500 | 📋 ja | – |
| 14 | Soll der Körperfettanteil nicht automatisch bestimmen, wo Fett sitzt? | ja | 35#499 → 35#500 | 📋 ja | – |
| 15 | Soll genetisch bedingte Verteilung wichtig sein, z. B. stärker Hüfte/Beine vs. Bauch? | ja | 35#499 → 35#500 | 📋 ja | – |
| 16 | Soll Knochenbau separat von Muskel/Fett simuliert werden? | ja | 35#499 → 35#500 | 📋 ja | – |
| 17 | Soll jemand schmalen Knochenbau + viel Muskelmasse haben können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 18 | Soll Haltung den sichtbaren Körper stark beeinflussen? | ja | 35#499 → 35#500 | 📋 ja | – |
| 19 | Soll leichte Skoliose, Rundrücken oder andere natürliche Abweichungen vorkommen können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 20 | Soll der Generator extreme, aber real mögliche Körperformen zulassen statt sie künstlich auf „attraktiv … | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 21 | Soll Gesichtsasymmetrie ausdrücklich erwünscht sein? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 22 | Soll z. B. ein Auge minimal höher sitzen können als das andere? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 23 | Soll Nasenform sehr fein parametrisiert werden? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 24 | Soll Lippenform links/rechts leicht asymmetrisch sein können? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 25 | Soll Zahnstellung variieren? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 26 | Soll es Zahnlücken, leichte Fehlstellungen, Veneers, Kronen usw. geben können? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 27 | Soll Haut echte Variation wie Poren, Sommersprossen, Muttermale und Pigmentunterschiede haben? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 28 | Soll Narbengewebe wirklich andere Shader-/Gewebeeigenschaften haben als normale Haut? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 29 | Soll Akne bzw. Hautunreinheiten dynamisch auftreten und wieder verschwinden können? | ja | 35#499 → 35#500 | 📋 ja | E-TA-005 |
| 30 | Soll Haut durch Alter, Sonne, Lebensstil und Verletzungen langfristig sichtbar verändert werden? | sonne ist gut für den körper dadurch wird nichts schlechtes auftreten aber andere sachen können schon sein | 35#499 → 35#500 | 📋 Freitext | E-TA-005 |
| 31 | Soll Brustgröße komplett kontinuierlich statt in Cup-Presets generiert werden? | genau, es sollen auch gigantische brüste geben aber nicht nur 10 verschiedene größen, die größe kann sich auch mit dem gewicht … | 35#499 → 35#500 | 📋 ja mit Zusatz | E-KO-014 |
| 32 | Soll linke/rechte Brust unterschiedlich groß oder geformt sein können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 33 | Soll Form unabhängig von Volumen sein? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 34 | Soll Position/Höhe individuell variieren? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 35 | Soll Gewebefestigkeit unabhängig von Größe sein? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 36 | Soll Schwangerschaft/Stillzeit Größe und Gewebestruktur temporär oder langfristig verändern können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 37 | Soll Altersveränderung berücksichtigt werden? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 38 | Soll Gewichtsverlust/-zunahme dort sichtbar Einfluss haben? | ja | 35#499 → 35#500 | 📋 ja | E-KO-014 |
| 39 | Soll Krafttraining Brustmuskel verändern, aber natürlich nicht einfach Brustgewebe ersetzen? | ja aber eher beim mann, bei der frau will ich eher große brüste, es können natürlich auch kleinere sein. Auch umdurch training … | 35#499 → 35#500 | 📋 ja mit Zusatz | E-KO-014 |
| 40 | Soll Kleidung wie BHs die reale Physik mechanisch beeinflussen? | ja mit sportbh wackeln die busen weniger zum beispiel | 35#499 → 35#500 | 📋 ja mit Zusatz | E-KO-014 |
| 41 | Soll Gesäßgröße ebenfalls aus Muskel + Fett + Knochenbau entstehen statt einem einzigen Slider? | ja sicher, aber der hintern kann auch wachsen durch krafttraining und weiteres noch, es soll auch bbl und andere kosmetische … | 35#499 → 35#500 | 📋 ja mit Zusatz | E-KO-014 |
| 42 | Soll linke/rechte Seite leicht unterschiedlich sein können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 43 | Soll Form durch Training deutlich veränderbar sein? | ja | 35#499 → 35#500 | 📋 ja | – |
| 44 | Soll Fettzu-/abnahme zusätzlich Einfluss haben? | ja | 35#499 → 35#500 | 📋 ja | – |
| 45 | Soll die Jiggle-/Deformationsphysik automatisch aus dieser Anatomie entstehen? | ja | 35#499 → 35#500 | 📋 ja | – |
| 46 | Soll Oberschenkelkontakt beim Gehen abhängig von Anatomie wirklich physisch vorkommen? | ja | 35#499 → 35#500 | 📋 ja | – |
| 47 | Soll Bauchgewebe ebenfalls individuell auf Bewegung reagieren? | ja | 35#499 → 35#500 | 📋 ja | – |
| 48 | Soll Oberarm-/Oberschenkel-Weichteilgewebe dieselbe Physikfamilie verwenden? | ja | 35#499 → 35#500 | 📋 ja | – |
| 49 | Soll jede Region eigene Dämpfungs-/Steifigkeitsparameter besitzen? | ja | 35#499 → 35#500 | 📋 ja | – |
| 50 | Soll der Spieler in einem erweiterten Character Creator diese Werte zumindest teilweise beeinflussen können? | ja | 35#499 → 35#500 | 📋 ja | – |
| 51 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Körper, Tattoos] | [ja …; Details explizit, nicht wiedergegeben] (35#500) → [ja …; Details explizit, nicht wiedergegeben] (35#502) | 35#499 → 35#500,35#502 | 📋 ja mit Zusatz (nachgereicht/geändert) | E-KO-015 |
| 52 | Soll es dabei keine Handvoll fertiger Presets, sondern kontinuierliche Parameter geben? | ja natürlich soll es das sehr viele parameter geben | 35#499 → 35#500 | 📋 ja mit Zusatz | E-KO-015 |
| 53 | Soll natürliche Asymmetrie ebenfalls vorkommen? | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 54 | Soll Hautfarbe/Pigmentierung lokal variieren können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 55 | Soll Beschaffenheit/Geometrie bei jeder Person etwas anders sein? | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 56 | Soll Anatomie im Character Creator sehr detailliert einstellbar sein, wenn Adult Content aktiviert ist? | ja genau, wie genau planst du das, gibt es ein anderes menü wenn man die adult mod installiert hat | 35#499 → 35#500 | 📋 ja mit Zusatz | E-KO-015 |
| 57 | Soll der Generator trotzdem realistische Grenzbereiche besitzen, damit zufällige NPCs nicht ständig anatomisch absurde … | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 58 | Soll Mods diese Grenzbereiche erweitern können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 59 | Soll männliche Anatomie genauso detailliert parametrisiert werden? | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 60 | Soll Größe/Form im Ruhezustand und bei körperlicher Reaktion physikalisch zusammenhängen statt zwei komplett getrennte … | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 61 | Soll auch dort leichte Asymmetrie möglich sein? | ja | 35#499 → 35#500 | 📋 ja | E-KO-015 |
| 62 | Soll Beschneidung bzw. unterschiedliche natürliche Anatomie vorkommen können? | eher nicht, nur wenn es gesundheitlich ist, nicht wegen religion, die welt soll zu 99.9% christlich sein die anderen können das … | 35#499 → 35#500 | 📋 Freitext · K14 | – |
| 63 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Körper, Tattoos] | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 64 | Soll Alter innerhalb 18+ sichtbare anatomische Unterschiede verursachen können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 65 | Soll Körperveränderung durch Gewicht, Hormone, Schwangerschaft etc. auch diese Bereiche beeinflussen, wo realistisch? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 66 | Soll Körperbehaarung kontinuierlich wachsen, genau wie Kopfhaar/Bart? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 67 | Soll Achsel-, Bein-, Arm-, Brust-, Bauch- und Schambehaarung jeweils getrennte Dichte-/Wachstumsparameter besitzen? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 68 | Soll Haarfarbe pro Region leicht variieren können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 69 | Soll Textur ebenfalls variieren? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 70 | Soll eine Person genetisch fast keine Körperbehaarung haben können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 71 | Soll Rasieren wirklich einen Zustand erzeugen, aus dem wieder Stoppeln und dann längere Haare wachsen? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 72 | Soll Waxing/Epilation länger anhaltende glatte Zustände erzeugen? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 73 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Körper, Tattoos] | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 74 | Soll der Spieler/NPC bestimmte Grooming-Gewohnheiten haben? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 75 | Soll sich diese Gewohnheit im Laufe des Lebens ändern können? | ja | 35#499 → 35#500 | 📋 ja | E-KO-018 |
| 76 | Soll Tattoos praktisch jede Körperfläche bedecken können, solange die Geometrie sinnvoll unterstützt wird? | [ja …; Details explizit, nicht wiedergegeben] (35#500) → augen kann man auch tattowiert haben, je mehr tattoos desto seltener findet man die person wenn man nicht bei tsttoostudios sucht … (35#502) | 35#499 → 35#500,35#502 | 📋 Freitext (nachgereicht/geändert) | E-TA-004 |
| 77 | Soll ein einzelner NPC von null Tattoos bis nahezu kompletter Körperbedeckung alles haben können? | der gleiche npc? Wenn nicht dann ja, soll es alles in der Welt geben können | 35#499 → 35#500 | 📋 Freitext | E-TA-004 |
| 78 | Soll ein Sleeve wirklich aus vielen einzelnen Motiven entstehen können, die zusammengewachsen wirken? | teilweise schon, teilweise aber schon zusammen als sleeve geplant | 35#499 → 35#500 | 📋 Freitext | E-TA-004 |
| 79 | Soll es auch als ein zusammenhängendes großes Artist-Design erzeugt werden können? | ja natürlich, wenn es dazwischen freie flächen gibt kann man diese nocu auffüllen lassen, mit farbe oder motiven oder anderen … | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-004 |
| 80 | Soll Rücken, Brust, Hals, Hände, Finger, Gesicht, Beine, Füße usw. alle unterstützt werden? | auf jedenfall, theoretiscu möglich sind 100% bis auf den letzten mm^2 | 35#499 → 35#500 | 📋 Freitext | E-TA-004 |
| 81 | Soll Tattoo-Stil als echtes Artist-/Designmerkmal gespeichert werden, z. B. Realism, Traditional, Blackwork, Fine Line, … | ja | 35#499 → 35#500 | 📋 ja | E-TA-004 |
| 82 | Soll ein Mensch Tattoos verschiedener Artists/Stile gemischt haben können? | natürlich, wir werden noch festlegen wie viele studios es geben wird aber alleine in metropolis sicherlich 20-30 oder noch mehr | 35#499 → 35#500 | 📋 ja mit Zusatz | – |
| 83 | Soll Qualität stark variieren, von extrem professionell bis schlecht gestochen? | ja kann sie, es kanj zb auch im häfn tattoos gestochen worden sein und diese sind eher nicht so gut | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 84 | Soll ein Tattoo-Artist selbst ein individuelles Skill-/Style-Profil besitzen? | ja, aber wie genau meinst du das | 35#499 → 35#500 | 📋 ja | E-TA-006 |
| 85 | Soll ein bestimmter Artist an seinem Stil wiedererkennbar sein? | ja kann er schon | 35#499 → 35#500 | 📋 ja | E-TA-006 |
| 86 | Soll Tattoo-Farbe mit der Zeit realistisch ausbleichen? | ja, aber nicht schon nach 2 wochen sondern erst nach mehreren jahren, dann kann man vielleicht noch extra nachbessern lassen … | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 87 | Soll UV/Sonne das beschleunigen können? | ja schon, aber sonne ist gut für den körper aber bei zu viel kann es etwas schneller ausbleichen | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 88 | Soll Alter der Haut Tattoos ebenfalls verändern? | ja es kann bisschen verrunzelt sein oder anderes | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 89 | Soll Muskel- oder Gewichtszunahme ein Tattoo glaubwürdig deformieren? | ja oder auch wenn man sich tättowieren lässt und dann abnimmt kann es auch scheiße ausschauen | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 90 | Soll Schwangerschaft Tattoos am Bauch mitdehnen können? | ja natürlich | 35#499 → 35#500 | 📋 ja | E-TA-006 |
| 91 | Soll ein späterer Gewichtsverlust die Form wiederum verändern? | ja | 35#499 → 35#500 | 📋 ja | E-TA-006 |
| 92 | Soll Narbenbildung durch oder unter Tattoos berücksichtigt werden? | ja, man kann narben über tattowieren damit man diese nicht mehr sieht | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 93 | Soll man Cover-ups machen können, wobei das alte Tattoo teilweise darunter bleibt? | ja, es soll aber auch weggelasert werden können | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 94 | Soll Laserentfernung mehrere Sitzungen benötigen und möglicherweise leichte Reste hinterlassen? | ja | 35#499 → 35#500 | 📋 ja | E-TA-006 |
| 95 | Soll ein NPC spontan oder geplant ein neues Tattoo bekommen können, auch wenn er weit entfernt simuliert wird? | ja kann er aber meistens mit termin, jedenfalls bei den besseren | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 96 | Soll Tattoos persönliche Bedeutung haben können – Erinnerung, Partner, Familie, Gruppe, Lebensereignis usw.? | ja sicherlich, manche wollen aber einfach nur viele haben | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 97 | Soll ein NPC ein Tattoo später bereuen können? | ja kann er er hat dann die möglichkeit cover up oder lasern | 35#499 → 35#500 | 📋 ja mit Zusatz | E-TA-006 |
| 98 | Soll Beziehungen dazu führen können, dass jemand Partnernamen/-symbole tätowiert und später ggf. covern lässt? 😭 | ja | 35#499 → 35#500 | 📋 ja | E-TA-006 |
| 99 | Soll Tattoo-Shops echte Termine, Artists, Preise, Wartezeiten und verschiedene Qualitätsstufen besitzen? | ja genau, das ist auch online buchbar, es soll wie schon mal angesprochen ein funktionales internet geben, und dieses soll … (35#500) → ja das passt, der Spieler kann auch webseiten programmieren, natürlich nicht so wie in echt aber stark vereinfacht mit vielleicht … (35#502) → man kann dann auch geld verdienen damit im spiel und kann damit reich werden, dann hat man ja noch casinos, Buchmacher börse … (35#504) | 35#499 → 35#500,35#502,35#504 | 📋 Freitext (nachgereicht/geändert) | E-TA-006 |
| 100 | Soll unsere langfristige Regel sein: Körperdetails sind persistenter, veränderlicher Lebens-State – Tattoos, Piercings, … | ja aber piercings haben wir gar nicht erwähnt oder | 35#499 → 35#500 | 📋 ja mit Zusatz | – |

## Runde 14B: Piercings, Körpermodifikation (35#511)

**Datum:** 2026-09-14 · **Fragen:** 101–140 (40) · **Antwortnachrichten:** 35#512 · **Module (Paketebene):** 07 · **Status:** 📋 40  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-025, E-TA-010, E-TA-011

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 101 | Soll praktisch jede anatomisch plausible Piercing-Stelle unterstützt werden und nicht nur Ohr/Nase/Bauchnabel? | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#511 → 35#512 | 📋 Freitext | E-TA-010 |
| 102 | Soll es also auch Lippen-, Zungen-, Augenbrauen-, Brustwarzen- und Intimpiercings geben? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 103 | Soll ein NPC mehrere Piercings derselben Art haben können, z. B. viele Ohrpiercings gleichzeitig? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 104 | Soll ein Mensch von gar keinem Schmuck bis zu extrem vielen Piercings alles haben können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 105 | Soll jedes Piercing persistent zur konkreten Person gehören und nicht bei jedem Laden neu generiert werden? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 106 | Soll Schmuck selbst austauschbar sein, also Ring, Stecker, Barbell, andere Formen und Materialien? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 107 | Soll Größe/Dicke des Schmucks variieren können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 108 | Soll Gold, Silber, Titan, Stahl, Kunststoff usw. tatsächlich unterschiedliche Materialeigenschaften/Optik besitzen? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 109 | Soll billiger Schmuck eher Probleme verursachen können als hochwertiger? | nur weil er billig ist heißt es nicht das er schlechter ist, kann bei teurem auch schnell passieren | 35#511 → 35#512 | 📋 Freitext | E-TA-010 |
| 110 | Soll es individuelle Materialunverträglichkeiten geben können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 111 | Soll ein frisch gestochenes Piercing einen echten Heilungszustand besitzen? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 112 | Soll man während dieser Zeit leichte Rötung, Schwellung oder andere normale Heilungsreaktionen sehen können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 113 | Soll schlechte Pflege zu Komplikationen führen können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 114 | Soll ein Piercing bei Problemen entfernt werden können und eventuell eine kleine Narbe zurücklassen? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 115 | Soll ein lange bestehendes Piercing nach Entfernung teilweise sichtbar bleiben können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 116 | Soll Schmuck physikalisch reagieren, also sich beim Laufen, Kleidungkontakt oder anderen Bewegungen leicht bewegen? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 117 | Soll sehr kleiner Schmuck aus Performancegründen vereinfacht simuliert werden, während größere Teile echte Secondary … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 118 | Soll Kleidung an Piercings physisch vorbeilaufen statt einfach hindurchzuclippen? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 119 | Soll es selten auch Situationen geben können, in denen Schmuck hängenbleibt und dadurch eine Verletzung entsteht? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 120 | Soll der Spieler deshalb bestimmte Schmuckstücke vor Sport, Kampf oder anderen Situationen freiwillig entfernen können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 121 | Soll es gedehnte Ohrläppchen / Plugs / Tunnel geben? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 122 | Soll Dehnung ein persistenter Körperzustand sein, der sich nur langsam verändert? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 123 | Soll sehr starke Dehnung sichtbare langfristige Veränderungen hinterlassen können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 124 | Soll es weitere Body Mods wie gespaltene Zunge, Scarification oder subdermale Implantate geben können? | ja und noch vieles mehr | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 125 | Soll solche extreme Body Modification deutlich seltener als normale Tattoos/Piercings sein? | ja etwas schon, aber manche sehr tattowierten haben das auch | 35#511 → 35#512 | 📋 ja mit Zusatz | E-TA-010 |
| 126 | Soll die Wahrscheinlichkeit solcher Personen stark von passenden Orten/Subkulturen abhängen, genauso wie wir es bei … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 127 | Soll ein spezialisierter Body-Mod-Artist andere Fähigkeiten brauchen als ein normaler Tattoo-Artist? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 128 | Soll Tattoo-, Piercing- und Body-Mod-Studios teilweise alles anbieten, teilweise aber klar spezialisiert sein? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 129 | Soll Qualität/Hygiene/Reputation eines Studios persistent sein und sich durch Bewertungen oder Vorfälle verändern … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 130 | Soll man Studios über das In-Game-Internet finden, Portfolios ansehen und Termine buchen können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 131 | Soll auch kosmetische Chirurgie über echte Kliniken/Ärzte in der Welt laufen statt einfach über einen … | ja genau | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 132 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Piercings, Körpermodifikation] | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 133 | Soll ein Eingriff tatsächlich die zugrunde liegenden Human-Generator-Parameter verändern, sodass Physik, Kleidung und … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 134 | Soll es eine Heilungs-/Recovery-Phase nach größeren Eingriffen geben, statt dass man am nächsten Morgen komplett fertig … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 135 | Soll Qualität des Arztes Einfluss auf Ergebnis, Symmetrie, Narbenbildung und Komplikationsrisiko haben? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 136 | Soll eine Person mit späteren Eingriffen frühere Eingriffe verändern oder korrigieren können? | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 137 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Piercings, Körpermodifikation] | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 138 | Soll dadurch ein NPC über viele In-Game-Jahre äußerlich massiv anders aussehen können, aber weiterhin eindeutig … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 139 | Soll der Adult-Mod im Character Creator für Erwachsene zusätzliche Detailoptionen für intime Piercings/Body Mods … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |
| 140 | Soll die Gesamtregel lauten: Tattoos, Piercings, Schmuck, Body Mods, kosmetische Eingriffe, Narben, Haare, Muskeln und … | ja | 35#511 → 35#512 | 📋 ja | E-TA-010 |

## Runde 15A: Präferenzmodell (35#523)

**Datum:** 2026-09-15 · **Fragen:** 1–25 (25) · **Antwortnachrichten:** 35#524 · **Module (Paketebene):** 05 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-023, E-KO-026, E-NP-019, E-NP-020, E-NP-021, E-WE-009, E-XX-009, E-XX-018

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Individuelles Präferenzprofil statt Kategorien | ja | 35#523 → 35#524 | 📋 ja | – |
| 2 | Stärke einer Vorliebe | ja | 35#523 → 35#524 | 📋 ja | – |
| 3 | Partnerabhängige Vorlieben | [ja …; Details explizit, nicht wiedergegeben] | 35#523 → 35#524 | 📋 ja mit Zusatz | – |
| 4 | Situationsabhängige Vorlieben | [ja …; Details explizit, nicht wiedergegeben] | 35#523 → 35#524 | 📋 ja mit Zusatz | – |
| 5 | Neugier getrennt von Erfahrung | ja | 35#523 → 35#524 | 📋 ja | – |
| 6 | Erfahrung nach einzelnen Bereichen | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#523 → 35#524 | 📋 Freitext | – |
| 7 | Vorlieben können sich verändern | ja | 35#523 → 35#524 | 📋 ja | – |
| 8 | Stabile Kernvorlieben vs. flexible Vorlieben | ja | 35#523 → 35#524 | 📋 ja | – |
| 9 | Seltene Interessen tatsächlich selten halten | ja genau so, aber natürlich bei stripclubs ist die change höher. Es soll auch pornodarstellerinnen geben und die sind halt … | 35#523 → 35#524 | 📋 ja mit Zusatz | – |
| 10 | Keine sichtbare vollständige Fetischliste | normale spieler nicht aber ich will das schon sehen, während der entwicklung und vielleicht nachher zum einblenden auch aber nur … | 35#523 → 35#524 | 📋 Freitext | – |
| 11 | Bekanntheitsgrad einzelner Präferenzen | ja | 35#523 → 35#524 | 📋 ja | – |
| 12 | NPCs können falsche Annahmen haben | ja | 35#523 → 35#524 | 📋 ja | – |
| 13 | Kommunikation über Vorlieben | ja | 35#523 → 35#524 | 📋 ja | – |
| 14 | Grenzen ähnlich individuell behandeln | ja | 35#523 → 35#524 | 📋 ja | – |
| 15 | Intensität nicht mit Praktik vermischen | ja | 35#523 → 35#524 | 📋 ja | – |
| 16 | Dominanz/Submissivität nicht als binärer Typ | ja | 35#523 → 35#524 | 📋 ja | – |
| 17 | Initiative getrennt von Dominanz | ja | 35#523 → 35#524 | 📋 ja | – |
| 18 | Roleplay als eigenes System | [ja …; Details explizit, nicht wiedergegeben] | 35#523 → 35#524 | 📋 ja mit Zusatz · K4 | – |
| 19 | Outfit-/Kleidungspräferenzen | ja | 35#523 → 35#524 | 📋 ja | – |
| 20 | Materialien und sensorische Vorlieben | ja | 35#523 → 35#524 | 📋 ja | – |
| 21 | Adult-Items als echte Weltobjekte | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#523 → 35#524 | 📋 Freitext | E-KO-026 |
| 22 | Shops + In-Game-Internet | ja | 35#523 → 35#524 | 📋 ja | E-KO-026 |
| 23 | Qualität und Materialeigenschaften von Items | ja | 35#523 → 35#524 | 📋 ja | E-KO-026 |
| 24 | Mehrere erwachsene Beteiligte | ja | 35#523 → 35#524 | 📋 ja | – |
| 25 | Keine künstliche Inhaltsliste als technische Grenze | ja | 35#523 → 35#524 | 📋 ja | – |

## Runde 15B: Gruppen, Branche, Medien (35#525)

**Datum:** 2026-09-15 · **Fragen:** 26–50 (25) · **Antwortnachrichten:** 35#526 · **Module (Paketebene):** 05, 08 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-022, E-WE-006, E-WE-007, E-WE-008, E-WE-009

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 26 | Individuelle Beziehungen innerhalb einer Gruppe | kann sein aber bei gruppensex machen die leute das wahrscheinlich nur zum spaße. Bei 2 Frauen und ein mann kann es sein das die … | 35#525 → 35#526 | 📋 Freitext | E-NP-022, E-WE-006 |
| 27 | Unterschiedliche Beteiligungsgrade | ja | 35#525 → 35#526 | 📋 ja | E-NP-022, E-WE-006 |
| 28 | NPC-Initiative bei mehreren Personen | ja aber du kannst auch sagen sie soll herkommen | 35#525 → 35#526 | 📋 ja mit Zusatz | E-NP-022, E-WE-006 |
| 29 | Eifersucht trotz grundsätzlicher Zustimmung | ja | 35#525 → 35#526 | 📋 ja | E-NP-022, E-WE-006 |
| 30 | Offene Beziehungen / Polyamorie | ja | 35#525 → 35#526 | 📋 ja | E-NP-022, E-WE-006 |
| 31 | Swinger-/Adult-Clubs als echte Orte | ja | 35#525 → 35#526 | 📋 ja | E-NP-022 |
| 32 | Stripclubs als mehr als Kulisse | stripclubs sind essentiell, es soll in metropolis alle 15-30 geben und und in kleineren städten halt weniger, es gibt auch einen … | 35#525 → 35#526 | 📋 Freitext | – |
| 33 | Adult-Performer als normale persistente NPCs | ja natürlich, nur das diese professionelle sexarbeiter sind | 35#525 → 35#526 | 📋 ja mit Zusatz | E-WE-007 |
| 34 | Performer-Karriere | [ja …; Details explizit, nicht wiedergegeben] | 35#525 → 35#526 | 📋 ja mit Zusatz | E-WE-007 |
| 35 | Adult-Creator-Plattform | ja | 35#525 → 35#526 | 📋 ja | E-WE-007 |
| 36 | Spieler als Adult-Creator | ja | 35#525 → 35#526 | 📋 ja | E-WE-007 |
| 37 | Gemeinsame Content-Produktion | ja | 35#525 → 35#526 | 📋 ja | E-WE-007 |
| 38 | Professionelle Studios | ja | 35#525 → 35#526 | 📋 ja | E-WE-007 |
| 39 | Adult-Content als Teil der Economy | ja | 35#525 → 35#526 | 📋 ja | E-WE-007 |
| 40 | Reputation nach Zielgruppen | ja | 35#525 → 35#526 | 📋 ja | E-WE-007 |
| 41 | Adult-Websites im funktionalen Internet | ja genau, diese haben jeweils andere videos die halt von charakteren in der welt aufgenommen wurden, ich weiß nur nicht wie das … | 35#525 → 35#526 | 📋 ja mit Zusatz | – |
| 42 | User-generated Websites | ja | 35#525 → 35#526 | 📋 ja | – |
| 43 | Unterschiedliche Adult-Shops | ja | 35#525 → 35#526 | 📋 ja | – |
| 44 | Physische Item-Größen | [ja …; Details explizit, nicht wiedergegeben] | 35#525 → 35#526 | 📋 ja mit Zusatz | – |
| 45 | Materialphysik | das soll so sein wie in echt | 35#525 → 35#526 | 📋 Freitext | – |
| 46 | Zustand / Verschleiß | ja | 35#525 → 35#526 | 📋 ja | – |
| 47 | Reinigung und Hygiene | ja | 35#525 → 35#526 | 📋 ja | – |
| 48 | Lagerung | ja | 35#525 → 35#526 | 📋 ja | – |
| 49 | NPC-Käufe | ja | 35#525 → 35#526 | 📋 ja | – |
| 50 | Keine „Adult-Welt neben der echten Welt“ | ja | 35#525 → 35#526 | 📋 ja | – |

## Runde 15C: Orte, Öffentlichkeit, Produktion (35#527)

**Datum:** 2026-09-15 · **Fragen:** 51–75 (25) · **Antwortnachrichten:** 35#528 · **Module (Paketebene):** 08, 04 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-GE-001, E-TA-012, E-WE-010, E-WE-012

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 51 | Soll ein NPC eine eigene Präferenz dafür haben, wie privat oder riskant ein Ort sein darf – von „nur hinter … | ja | 35#527 → 35#528 | 📋 ja | – |
| 52 | Soll dieselbe Person je nach Partner unterschiedlich risikofreudig sein, also mit einem langjährigen Partner Dinge … | ja | 35#527 → 35#528 | 📋 ja | – |
| 53 | Soll das Spiel wirklich erkennen, wer eine Situation sehen oder hören könnte, statt Öffentlichkeit nur mit einem … | ja | 35#527 → 35#528 | 📋 ja | – |
| 54 | Soll dabei Sichtlinie, Entfernung, Fenster, Türen, Beleuchtung und Geräuschpegel berücksichtigt werden, sodass ein NPC … | ja | 35#527 → 35#528 | 📋 ja | – |
| 55 | Soll es NPCs geben, die bewusst beobachtet werden möchten, während andere allein die Möglichkeit extrem unangenehm … | ja kann sein | 35#527 → 35#528 | 📋 ja | – |
| 56 | Soll Voyeurismus ebenfalls als individuelle Präferenz vorkommen können – also Erwachsene, die freiwillig andere … | ja | 35#527 → 35#528 | 📋 ja | – |
| 57 | Soll ein Adult-Club dafür spezielle Bereiche besitzen können, etwa private Räume, öffentliche Showbereiche und Bereiche … | ja | 35#527 → 35#528 | 📋 ja | – |
| 58 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Orte, Öffentlichkeit, Produktion] | ja | 35#527 → 35#528 | 📋 ja | – |
| 59 | Soll Exhibitionismus auch außerhalb spezieller Clubs möglich sein, wobei öffentliche Orte dann normale World-Reaktionen … | ja | 35#527 → 35#528 | 📋 ja | – |
| 60 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Orte, Öffentlichkeit, Produktion] | ja | 35#527 → 35#528 | 📋 ja | – |
| 61 | Soll es für Content-Produktion reale Kameras/Smartphones in der Welt geben, sodass ein aufgenommenes Video logisch eine … | ja | 35#527 → 35#528 | 📋 ja | – |
| 62 | Soll eine Person zustimmen können, dass eine Situation aufgenommen, aber nicht veröffentlicht wird – also Aufnahme und … | ja | 35#527 → 35#528 | 📋 ja | – |
| 63 | Soll wiederum eine separate Zustimmung existieren können für privat speichern, mit Partner teilen, auf Creator-Seite … | ja | 35#527 → 35#528 | 📋 ja | – |
| 64 | Soll ein NPC später seine Meinung über eine zukünftige Veröffentlichung ändern können, solange sie noch nicht … | ja | 35#527 → 35#528 | 📋 ja | – |
| 65 | Soll professioneller Content mit Verträgen arbeiten können, sodass Bezahlung, Veröffentlichungsrechte und Beteiligung … | ja | 35#527 → 35#528 | 📋 ja | – |
| 66 | Soll amateurhafter Content dagegen oft nur mit Smartphone/Kamera entstehen und entsprechend andere Bild-/Tonqualität … | ja | 35#527 → 35#528 | 📋 ja | – |
| 67 | Soll Kameraqualität tatsächlich von Gerät, Objektiv, Licht und Aufnahmebedingungen abhängen, sodass nicht jedes … | ja | 35#527 → 35#528 | 📋 ja | – |
| 68 | Soll ein professionelles Studio echte Crew-Rollen besitzen können – Kamera, Licht, Ton, Produktion usw. – obwohl wir … | ja | 35#527 → 35#528 | 📋 ja | – |
| 69 | Soll ein Spieler bei eigener Content-Produktion wählen können zwischen schnell und improvisiert oder aufwendig … | ja | 35#527 → 35#528 | 📋 ja | – |
| 70 | Soll der Markt auf unterschiedliche Content-Nischen reagieren können, sodass bestimmte Kategorien größere oder kleinere … | ja | 35#527 → 35#528 | 📋 ja | – |
| 71 | Soll die Popularität eines Performers nicht nur aus Adult-Fähigkeiten entstehen, sondern auch aus Aussehen, … | ja | 35#527 → 35#528 | 📋 ja | – |
| 72 | Soll ein „normaler“ NPC plötzlich viral gehen können, wenn ein Content-Stück extrem populär wird, und dadurch Karriere, … | ja | 35#527 → 35#528 | 📋 ja | – |
| 73 | Soll Berühmtheit auch Nachteile haben können – Wiedererkennung auf der Straße, Presse/Online-Kommentare, … | ja sicher kann das sein, manche leute wollen dann vielleicht ein foto oder sie wollen eine chance zum sex oder anderes | 35#527 → 35#528 | 📋 ja mit Zusatz | – |
| 74 | Soll Adult-Content auf In-Game-Websites nicht als riesige Menge fertiger Videodateien installiert werden, sondern … | ja aber es darf nicht immer ein anderes video aus dem gleichen generiert werden es muss immer das gleiche video sein wenn man auf … | 35#527 → 35#528 | 📋 ja mit Zusatz | – |
| 75 | Soll dieses generative Content-System so allgemein gebaut werden, dass später auch normale In-Game-Videos – Newsclips, … | ja Noch extra frauen haben die möglichkeit in nagelstudios sich die nägel zu machen hand und füße, manche leute mögen sowas. Da … | 35#527 → 35#528 | 📋 ja mit Zusatz | – |

## Runde 15D: Praktiken-Präferenzen, Fesseln, Rollenspiel (35#529)

**Datum:** 2026-09-15 · **Fragen:** 76–100 (25) · **Antwortnachrichten:** 35#530 · **Module (Paketebene):** 05, 03 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-LE-012, E-WE-013, E-XX-008

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 76 | Soll eine Person Vorlieben für bestimmte Körperbereiche besitzen können, beispielsweise Hände, Füße, Nägel, Haare, … | ja | 35#529 → 35#530 | 📋 ja | – |
| 77 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Praktiken-Präferenzen, Fesseln, Rollenspiel] | ja | 35#529 → 35#530 | 📋 ja | – |
| 78 | Soll Grooming dadurch tatsächlich sozial relevant werden können, sodass beispielsweise ein NPC, der stark auf gepflegte … | ja | 35#529 → 35#530 | 📋 ja | – |
| 79 | Soll es Präferenzen für bestimmte Kleidungsstücke wie Dessous, Strümpfe, High Heels, bestimmte Uniform-/Outfit-Arten … | ja | 35#529 → 35#530 | 📋 ja | – |
| 80 | Soll dasselbe Kleidungsstück je nach Person vollkommen unterschiedlich wirken können – eine Person findet es besonders … | ja | 35#529 → 35#530 | 📋 ja | – |
| 81 | Soll es Präferenzen für bestimmte Materialien wie Leder, Latex, Seide oder andere Stoffe geben, wobei Materialoptik, … | ja | 35#529 → 35#530 | 📋 ja | – |
| 82 | Soll Führen/Unterwerfen sehr fein abgestuft sein, sodass manche Personen lediglich gern Entscheidungen abgeben, andere … | ja | 35#529 → 35#530 | 📋 ja | – |
| 83 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Praktiken-Präferenzen, Fesseln, Rollenspiel] | ja | 35#529 → 35#530 | 📋 ja | – |
| 84 | Soll consensuales Restraint/Fesseln als eigenes Interaction-System unterstützt werden, wobei Gegenstände physisch … | ja | 35#529 → 35#530 | 📋 ja | – |
| 85 | Soll es dabei verschiedene Materialien und Gegenstände mit unterschiedlichen Eigenschaften geben, statt dass alles … | ja | 35#529 → 35#530 | 📋 ja | – |
| 86 | Soll Blindfolding bzw. freiwillige Einschränkung einzelner Sinne möglich sein und tatsächlich Perception/AI … | ja | 35#529 → 35#530 | 📋 ja | – |
| 87 | Soll sensorische Variation wie Temperatur, unterschiedliche Berührungsstärken oder Materialien Teil desselben … | ja | 35#529 → 35#530 | 📋 ja | – |
| 88 | Soll Roleplay praktisch beliebige fiktive Erwachsenenrollen und vereinbarte Szenarien unterstützen können, wobei der … | ja | 35#529 → 35#530 | 📋 ja | – |
| 89 | Soll das Spiel innerhalb einer vereinbarten Roleplay-Szene trotzdem wissen, welche Person gerade „in Rolle“ spricht und … | ja | 35#529 → 35#530 | 📋 ja | – |
| 90 | Soll es NPCs geben, die Roleplay lieben, andere die es gelegentlich mögen und wieder andere, die es vollkommen albern … | ja | 35#529 → 35#530 | 📋 ja | – |
| 91 | Soll Zuschauen, beobachtet werden und Content-Produktion getrennte Präferenzen sein? Eine Person könnte gern privat … | ja | 35#529 → 35#530 | 📋 ja | – |
| 92 | Soll auch das persönliche Verhältnis zu Fotos/Videos partnerabhängig sein, sodass jemand mit einem langjährigen Partner … | ja | 35#529 → 35#530 | 📋 ja | – |
| 93 | Soll eine Person sehr spezifische Vorlieben für Orte oder Umgebungen entwickeln können – beispielsweise Bett, Dusche, … | ja | 35#529 → 35#530 | 📋 ja | – |
| 94 | Soll die Umgebung dabei echte praktische Auswirkungen haben: verfügbarer Platz, Stabilität, Oberflächenmaterial, … | ja | 35#529 → 35#530 | 📋 ja | – |
| 95 | Soll Adult-Items miteinander und mit Freeform-Positioning frei kombinierbar sein, statt dass jedes Item nur für eine … | ja | 35#529 → 35#530 | 📋 ja | – |
| 96 | Soll die KI bei extremen Größen/Formen von Items anhand von Anatomie, Erfahrung, aktuellem Körperzustand und … | [ja …; Details explizit, nicht wiedergegeben] | 35#529 → 35#530 | 📋 ja mit Zusatz | – |
| 97 | Soll eine Person Risiken bewusst akzeptieren können, während eine andere bei derselben Situation sofort ablehnt – und … | ja genau | 35#529 → 35#530 | 📋 ja | – |
| 98 | Soll es keine künstliche Obergrenze für die Anzahl gleichzeitig zutreffender Vorlieben geben? Eine Person kann also … | ja eine frau kann theoretisch alles wollen was der mann man | 35#529 → 35#530 | 📋 ja mit Zusatz | – |
| 99 | Soll der Population Generator außerdem verhindern, dass „extrem“ automatisch bedeutet, dass ein NPC alles Extreme mag? … | ja | 35#529 → 35#530 | 📋 ja | – |
| 100 | Soll unser finales Präferenzmodell deshalb ungefähr aus Interesse + Stärke + Erfahrung + Neugier + gewünschter … | ja | 35#529 → 35#530 | 📋 ja | – |

## Runde 15E: Solo, Medien, Dienstleistung (35#539)

**Datum:** 2026-09-15 · **Fragen:** 101–125 (25) · **Antwortnachrichten:** 35#540 · **Module (Paketebene):** 05, 08 · **Status:** ❓ 1, 📋 24  
**Register (Paketebene, Quelle überschneidet sich):** E-LE-011, E-WE-016, E-WE-017

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 101 | Solo-Verhalten als echter NPC-State | ja | 35#539 → 35#540 | 📋 ja | – |
| 102 | Häufigkeit stark individuell | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#539 → 35#540 | 📋 Freitext · K15 | – |
| 103 | Beziehung verändert Solo-Verhalten nicht automatisch | ja | 35#539 → 35#540 | 📋 ja | – |
| 104 | Adult-Medienkonsum als separate Präferenz | ja | 35#539 → 35#540 | 📋 ja | – |
| 105 | Medienpräferenzen nicht 1:1 Realität | ja | 35#539 → 35#540 | 📋 ja | – |
| 106 | Browser-/Account-Historie als privater State | ja aber eher später, also nicht priorisieren | 35#539 → 35#540 | 📋 ja mit Zusatz | – |
| 107 | Eigene Content-Abonnements | ja | 35#539 → 35#540 | 📋 ja | – |
| 108 | Creator antworten nicht automatisch | ja | 35#539 → 35#540 | 📋 ja | – |
| 109 | Parasoziale Beziehungen simulieren | ja | 35#539 → 35#540 | 📋 ja | – |
| 110 | Escort-/Sex-Work-Services differenzieren | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#539 → 35#540 | 📋 Freitext | E-WE-017 |
| 111 | Sexarbeiter bleiben individuelle NPCs | [ja, …; Details explizit, nicht wiedergegeben] | 35#539 → 35#540 | 📋 ja mit Zusatz · K1/Bewusstlosigkeit | E-WE-017 |
| 112 | Preise individuell und marktbasiert | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 113 | Premium-/VIP-Angebote | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 114 | Geld ersetzt persönliche Attraction nicht | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 115 | Beziehungen aus beruflichem Kontakt möglich | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 116 | Adult-Arbeit als richtige Karriere | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 117 | Studios können wirtschaftlich scheitern | kann passieren aber bei größeren eher nicht oft aber möglich wenn es zb verboten wird in dem staat, würde ich nicht machen aber … | 35#539 → 35#540 | 📋 Freitext | E-WE-017 |
| 118 | Konkurrenz zwischen Businesses | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 119 | Adult-Trends verändern sich | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 120 | Vorlieben für Altersgruppe innerhalb 18+ | ja | 35#539 → 35#540 | 📋 ja | E-WE-017 |
| 121 | Körpermerkmale als Attraction-Faktoren | ja genau, manche leute mögen viele tattoos, manche lange fake nägel, manche großen hintern und so weiter. Bei zb bordellen kann … | 35#539 → 35#540 | 📋 ja mit Zusatz · K6 | – |
| 122 | Persönlichkeit stärker als Optik möglich | ja | 35#539 → 35#540 | 📋 ja | – |
| 123 | „Type“ als unscharfe Tendenz | ja | 35#539 → 35#540 | 📋 ja | – |
| 124 | Präferenzen können miteinander interagieren | ja | 35#539 → 35#540 | 📋 ja | – |
| 125 | Präferenzsystem ohne festes Ende | weiß nicht was eine fetisch db ist | 35#539 → 35#540 | ❓ offen/Rückfrage | – |

## Runde 15F: Gesundheit, Hygiene (35#541)

**Datum:** 2026-09-15 · **Fragen:** 126–150 (25) · **Antwortnachrichten:** 35#542 · **Module (Paketebene):** 06 · **Status:** ❌ 3, 📋 22  
**Register (Paketebene, Quelle überschneidet sich):** E-GE-002, E-GE-003, E-GE-004, E-GE-005, E-GE-006, E-GE-007, E-GE-008, E-NP-023

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 126 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Gesundheit, Hygiene] | ja genau, wenn man will kann eine frau auch schwanger werden und man bekommt dann ein kind, junge oder mädchen | 35#541 → 35#542 | 📋 ja mit Zusatz | E-GE-002 |
| 127 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Gesundheit, Hygiene] | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 128 | Soll ein erwachsener NPC selbstständig eine bevorzugte Verhütungsmethode besitzen und diese Entscheidung unabhängig vom … | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 129 | Soll ein NPC mit dem Partner darüber reden können, welche Methode verwendet wird, besonders wenn Schwangerschaft oder … | ja genau, die pille kann die fanze persönlichkeit verändern der frau | 35#541 → 35#542 | 📋 ja mit Zusatz | E-GE-002 |
| 130 | Soll Fruchtbarkeit individuell variieren, sodass nicht jede Person dieselbe Wahrscheinlichkeit für eine Schwangerschaft … | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 131 | Soll ein Menstruations-/Ovulationszyklus grundsätzlich simuliert werden, aber so abstrahiert, dass er nicht zu nervigem … | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 132 | Soll dieser Zyklus trotzdem Einfluss auf Fruchtbarkeit, Körperzustand, Stimmung und eventuell bestimmte körperliche … | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 133 | Soll Schwangerschaft nur entstehen können, wenn die zugrunde liegende Biologie und Situation das tatsächlich … | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 134 | Soll Schwangerschaft als langfristiger World-State über Monate laufen und Körper, Gewicht, Bewegung, Kleidung, Termine, … | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 135 | Soll ein Schwangerschaftstest als echtes kaufbares/benutzbares Item bzw. medizinische Untersuchung existieren können? | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 136 | Soll medizinische Versorgung rund um Schwangerschaft über Ärzte/Kliniken laufen, genau wie andere Health-Systeme? | ja | 35#541 → 35#542 | 📋 ja | E-GE-002 |
| 137 | Soll das Spiel STI/sexuell übertragbare Infektionen grundsätzlich simulieren können, wobei Häufigkeit und Detailgrad … | nein würde ich jetzt mal nicut machen, das brauchen wir nicht es ist eh schon genug | 35#541 → 35#542 | ❌ nein · E-GE-004 | E-GE-004 |
| 138 | Soll ein NPC eine Infektion haben können, ohne sofort Symptome zu zeigen, sodass Tests medizinisch sinnvoll sein können? | nein würde ich jetzt mal nicut machen, das brauchen wir nicht es ist eh schon genug | 35#541 → 35#542 | ❌ nein · E-GE-004 | E-GE-004 |
| 139 | Soll Behandlung über echte Medikamente/Ärzte laufen und nicht über einen simplen „Cure“-Button? | nein würde ich jetzt mal nicut machen, das brauchen wir nicht es ist eh schon genug | 35#541 → 35#542 | ❌ nein · E-GE-004 | E-GE-004 |
| 140 | Soll ein NPC Wissen über seinen eigenen Gesundheitsstatus besitzen können und dieses Wissen ehrlich mitteilen, … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 141 | Soll Hygiene ein allgemeines Character-System sein — Duschen, Schweiß, Körpergeruch, saubere Kleidung usw. — und … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 142 | Soll ein NPC abhängig von Persönlichkeit unterschiedlich empfindlich auf schlechte Hygiene reagieren? | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 143 | Soll Duschen nach oder vor einer Situation einfach eine normale Handlung im laufenden Tagesablauf sein, statt einer … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 144 | Soll Flüssigkeitssimulation grundsätzlich dieselbe Material-/Physics-Infrastruktur des Spiels verwenden und nicht aus … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 145 | Soll dabei Menge, Viskosität, Oberfläche, Kleidung und Schwerkraft beeinflussen, wie sich Flüssigkeiten bewegen oder … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 146 | Soll Kleidung Flüssigkeiten aufnehmen bzw. sichtbar verschmutzt werden können und danach einen entsprechenden … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 147 | Soll Bettwäsche, Handtücher, Möbel usw. ebenfalls Zustände wie sauber/verschmutzt bekommen können, ohne dass wir daraus … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 148 | Soll ein NPC nach körperlich sehr intensiven Situationen automatisch auf Schmerzen, Verletzungen oder Erschöpfung … | ja | 35#541 → 35#542 | 📋 ja | E-GE-005 |
| 149 | Soll wiederholte starke Belastung auf eine noch nicht verheilte Verletzung das Risiko realistisch erhöhen, statt dass … | [ja …; Details explizit, nicht wiedergegeben] | 35#541 → 35#542 | 📋 ja mit Zusatz | – |
| 150 | Soll die Grundregel lauten, dass Adult Health niemals ein separates Minigame ist, sondern Schwangerschaft, … | ja | 35#541 → 35#542 | 📋 ja | – |

## Runde 15G: Orientierung, Dynamik (35#549)

**Datum:** 2026-09-15 · **Fragen:** 151–175 (25) · **Antwortnachrichten:** 35#550 · **Module (Paketebene):** 05 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-025, E-NP-027

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 151 | Sexuelle und romantische Orientierung getrennt | ja | 35#549 → 35#550 | 📋 ja | – |
| 152 | Orientierung nicht als drei harte Schalter | [abwertende Äußerung zur Orientierung; Wortlaut in der Quelle; siehe K5] | 35#549 → 35#550 | 📋 Freitext · K5 | – |
| 153 | Körpermerkmale wichtiger als Label möglich | ja | 35#549 → 35#550 | 📋 ja · K5 | E-NP-027 |
| 154 | Romantische Liebe und sexuelle Lust unabhängig | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 155 | Sexuelle Häufigkeit als Paar-Dynamik | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 156 | Unterschiedliche Libido zwischen Partnern | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 157 | Spontanität als eigene Präferenz | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 158 | Dauer als individuelle Präferenz | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 159 | Pausen innerhalb derselben Session | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 160 | Rhythmus und Abwechslung | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 161 | Orgasmus als individuelles Reaktionsprofil | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 162 | Orgasmus nicht nur aus „Progress“ | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 163 | Edging / bewusstes Verzögern | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 164 | Orgasmuskontrolle innerhalb freiwilliger Dominanz-Dynamiken | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 165 | Mehrfache Orgasmen individuell | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 166 | Körpergeruch und persönlicher Geruch | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 167 | Parfüm / Deo / Pflegeprodukte | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 168 | Stimme als Adult-/Attraction-Präferenz | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 169 | Haare als eigenständiger Attraction-Faktor | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 170 | Kosmetische Details wie Nägel | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 171 | Sehr extreme kosmetische Looks | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 172 | Besitz bestimmter Adult-Items als persönlicher State | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 173 | Solo- und Partnerpräferenzen getrennt | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 174 | Fantasien, die nie umgesetzt werden sollen | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |
| 175 | Partner-spezifische „komplette Offenheit“ | ja | 35#549 → 35#550 | 📋 ja | E-NP-027 |

## Runde 15H: Kompatibilität, Statistik (35#551)

**Datum:** 2026-09-15 · **Fragen:** 176–200 (25) · **Antwortnachrichten:** 35#552 · **Module (Paketebene):** 05 · **Status:** 📋 25  
**Register (Paketebene, Quelle überschneidet sich):** E-LE-009, E-NP-025, E-NP-028, E-WE-018

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 176 | Soll die sexuelle Kompatibilität zweier Erwachsener aus vielen einzelnen Faktoren entstehen, statt dass es einen … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 177 | Soll ein Paar mit sehr unterschiedlichen Vorlieben trotzdem eine langfristig gute Beziehung führen können, wenn beide … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 178 | Soll ein NPC mit der Zeit lernen, welche Reaktionen sein Partner auf bestimmte Berührungen, Intensitäten, … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 179 | Soll dieses Partnerwissen fehlerhaft sein können? Also ein NPC glaubt, sein Partner mag etwas besonders, obwohl es nur … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 180 | Soll körperliche Attraction im Laufe einer Beziehung steigen oder fallen können, wenn sich Aussehen, Gewicht, Tattoos, … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 181 | Soll gleichzeitig emotionale Bindung eine gewisse Veränderung der körperlichen Attraction ausgleichen können, ohne sie … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 182 | Soll ein NPC merken können, dass sein Partner sich körperlich stark verändert hat, und dazu je nach Persönlichkeit … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 183 | Soll sich ein erwachsener NPC aktiv verändern können, weil er weiß, dass sein Partner etwas besonders attraktiv findet … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 184 | Soll ein NPC umgekehrt auch bewusst sagen können: „Das mache ich nicht nur für dich“, wenn eine gewünschte körperliche … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 185 | Soll es Präferenzen für sehr konkrete optische Kombinationen geben können, beispielsweise bestimmte Frisur + … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 186 | Soll Vorfreude eine Rolle spielen? Eine länger geplante Begegnung könnte psychologisch anders wirken als spontane … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 187 | Soll längere sexuelle Abstinenz bei manchen Personen Lust/Initiative erhöhen können, bei anderen kaum Einfluss haben … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 188 | Soll die Weltzeit dafür sauber funktionieren, sodass Bedürfnisse und Beziehungen tatsächlich auf Stunden/Tage/Wochen … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 189 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Kompatibilität, Statistik] | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 190 | Soll eine Person theoretisch mehrere Begegnungen an einem Tag wollen können, während eine andere nach einer einzigen … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 191 | Soll die Häufigkeit vergangener Begegnungen langfristig in Erinnerungen/Beziehungsdynamik einfließen können, aber nicht … | ja, ich will aber auch unabhängig vom adult system etwas haben das alle möglichen sachen nicht nur trackt sondern auch in einem … | 35#551 → 35#552 | 📋 ja mit Zusatz | E-NP-028 |
| 192 | Soll ein NPC Vorlieben auch bewusst verbergen können, obwohl er dem Spieler vertraut, beispielsweise aus Scham, … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 193 | Soll eine Person später freiwillig etwas offenbaren können, das sie über Monate oder Jahre nicht erwähnt hat, wenn … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 194 | Soll Adult-Arbeit oder öffentlich verfügbarer Content nicht automatisch bedeuten, dass die betreffende Person privat … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 195 | Soll ein Performer bestimmte Inhalte ausschließlich beruflich machen können und privat sogar ausdrücklich kein … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 196 | Soll umgekehrt jemand beruflich relativ gewöhnlichen Content produzieren, privat aber wesentlich ungewöhnlichere … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 197 | Soll bei gekauften/gebuchten Adult-Services das konkrete vereinbarte Leistungsprofil Teil des Vertrages sein, sodass … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 198 | Soll ein langfristiger Companion-Service zusätzliche nichtsexuelle Leistungen wie Kochen, Haushalt, Begleitung, Reisen … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 199 | Soll solche langfristige Nähe wiederum echte Beziehungen erzeugen können – Freundschaft, starke emotionale Bindung oder … | ja | 35#551 → 35#552 | 📋 ja | E-NP-028 |
| 200 | Soll Runde 15 insgesamt mit der Regel schließen, dass es keinen einzelnen „Adult-NPC-Typ“ gibt: Beruf, Erfahrung, … | ja Ich will jetzt schlafen | 35#551 → 35#552 | 📋 ja | E-NP-028 |
