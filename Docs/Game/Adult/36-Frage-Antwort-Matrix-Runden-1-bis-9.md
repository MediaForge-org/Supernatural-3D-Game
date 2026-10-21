# Adult 36: Frage-Antwort-Matrix, Runden 1 bis 9

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

- [Runde 1: Gesamtrichtung (35#186)](#runde-1-gesamtrichtung-35186)
- [Runde 2: Content-Struktur und Rollen (35#188)](#runde-2-content-struktur-und-rollen-35188)
- [Runde 3: Kategorien-Struktur (35#220)](#runde-3-kategorien-struktur-35220)
- [Runde 4: Präferenzprofile (35#226)](#runde-4-präferenzprofile-35226)
- [Runde 5: Beziehung, Attraktion (35#228)](#runde-5-beziehung-attraktion-35228)
- [Runde 6: Initiative, Flirt (35#230)](#runde-6-initiative-flirt-35230)
- [Runde 7: Orte, Öffentlichkeit, Observer (35#232)](#runde-7-orte-öffentlichkeit-observer-35232)
- [Runde 8: Pakete, Modularität (35#246)](#runde-8-pakete-modularität-35246)
- [Runde 9A: Präferenz-DNA (35#260)](#runde-9a-präferenz-dna-35260)
- [Runde 9B.1: Kern/Gedächtnis (35#268)](#runde-9b1-kerngedächtnis-35268)
- [Runde 9B.2: Lernen, Dates, Beziehungen (35#270)](#runde-9b2-lernen-dates-beziehungen-35270)
- [Runde 9B.3: Langzeit, Alter, Tod (35#272)](#runde-9b3-langzeit-alter-tod-35272)
- [Runde 9C: Weltsimulation, Speicher (35#278)](#runde-9c-weltsimulation-speicher-35278)

---


## Runde 1: Gesamtrichtung (35#186)

**Datum:** 2026-09-10 · **Fragen:** 1–12 (12) · **Antwortnachrichten:** 35#187 · **Module (Paketebene):** 02, 03, 05 · **Status:** ❌ 1, 📋 11  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-002, E-IA-003, E-NP-001, E-NP-002, E-NP-003, E-PA-011, E-PA-012, E-SK-005

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll der Adult-Mod nicht nur Player↔NPC, sondern auch NPC↔NPC-Interaktionen in der Welt ermöglichen? | ja genau, man kann auxh zuschauen können oder joinen | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 2 | Wie häufig soll sowas auftreten? | man kann es sich komplett aussuchen, da es eine mod sein wird da steam das nicht freigeben wird, aber wenn man will die ganze … | 35#186 → 35#187 | 📋 Freitext | – |
| 3 | Sollen NPCs solche Interaktionen selbst initiieren können, wenn Persönlichkeit, Beziehung, Ort, Stimmung und Grenzen … | ja sicherlich, manchmal reicht nur das aussehen und paar worte | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 4 | Soll es neben Beziehungen auch Casual/One-off-Kontexte geben, ohne dass vorher eine große Romance aufgebaut werden muss? | ja sicherlich, ine night stands sind wahrscheinlich häufiger | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 5 | Sollen passende Orte wie Clubs, Hotels, Wohnungen, Partys usw. jeweils eigene Regeln und Wahrscheinlichkeiten besitzen? | ja schon, aber sag das noch genauer | 35#186 → 35#187 | 📋 ja | – |
| 6 | Soll Öffentlichkeit wirklich berücksichtigt werden, also Characters unterschiedlich auf privat / halbprivat / … | ja schon, aber manche frauen sind schamlos und machens einfach in der welt wenn andere zuschauen können | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 7 | Soll der Spieler global einstellen können: | nein es ist eine mod an oder aus, wenn man es nicut zu extrem will kann man eine andere package der mod insallieren | 35#186 → 35#187 | ❌ nein | – |
| 8 | Soll Hardcore kein einzelner Schalter, sondern nur ein Preset über viele Einstellungen sein, die man danach noch … | ja schon, hardcore ist wie viel die frauen sich gefallen lassen und diengeschwindigkeit soweit ich weiß | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 9 | Soll jeder wichtige erwachsene NPC eigene individuelle Preferences und Grenzen haben, statt dieselben Standardwerte zu … | ja genau, aber beim richtigen ist das bei den Frauen egal da machen sie alles | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 10 | Sollen sich diese Preferences teilweise durch Beziehungen, Erfahrungen und Character Development verändern können, … | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#186 → 35#187 | 📋 Freitext | – |
| 11 | Soll das System sich relevante frühere Interaktionen merken und später in Dialogen, Beziehung, Verhalten und … | ja die frauen haben die gleichen anzeichen wie in der realen welt | 35#186 → 35#187 | 📋 ja mit Zusatz | – |
| 12 | Sollen Adult-Inhalte in separaten Mod-Packs modular installierbar sein, z. B. Base Framework + Animation Packs + Voice … | ok kann es sein aber auch als pack mit allem Das ist aber noch nicht alles oder, ich habe noch nicht die kategorien und so weiter … | 35#186 → 35#187 | 📋 ja mit Zusatz | – |

## Runde 2: Content-Struktur und Rollen (35#188)

**Datum:** 2026-09-10 · **Fragen:** 13–24 (12) · **Antwortnachrichten:** 35#189 · **Module (Paketebene):** 03, 05 · **Status:** ❓ 3, 📋 9  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-001, E-IA-003, E-IA-004, E-IA-005, E-PA-012, E-SK-006, E-SK-007

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 13 | Soll es grob unterschiedliche Content-Tiers geben, z. B. Romantic / Casual / Explicit / High-Intensity / Hardcore, die … | ja kann es geben | 35#188 → 35#189 | 📋 ja | – |
| 14 | Soll Hardcore tatsächlich mehrere Dinge gleichzeitig beeinflussen, also z. B. Intensität, Tempo, Dauer, körperliche … | ja genau | 35#188 → 35#189 | 📋 ja | – |
| 15 | Soll jedes davon einzeln einstellbar sein, sodass Hardcore Preset nur ein Preset ist und man danach alles verändern … | es kommt auf die person an | 35#188 → 35#189 | 📋 Freitext | – |
| 16 | Soll ein Character unterschiedliche Grenzen je Rolle haben? Also etwas in Rolle A mögen, aber in Rolle B nicht. | verstehe ich nicht | 35#188 → 35#189 | ❓ offen/Rückfrage | E-IA-004 |
| 17 | Soll ein Character je Partner unterschiedliche Preferences haben? Also mit Person A sehr offen, Person B kaum. | ja sicher, so wie in echt | 35#188 → 35#189 | 📋 ja | – |
| 18 | Soll Zuschauen eine eigene Interaction-Rolle sein, statt nur Kamera/Animation? Dann könnte der Zuschauer reagieren, … | ja wenn die frau schon so schamlos ist kann man einfach ihn reinstecken auch | 35#188 → 35#189 | 📋 ja mit Zusatz | E-IA-003 |
| 19 | Soll Joinen dynamisch während einer laufenden Interaction möglich sein, wenn alle beteiligten Character-Regeln passen? | manchmal auch spontan | 35#188 → 35#189 | 📋 Freitext | E-IA-003 |
| 20 | Soll es auch das Gegenteil geben: jemand verlässt die Situation und die anderen machen weiter, sofern die verbleibende … | ja | 35#188 → 35#189 | 📋 ja | E-IA-003 |
| 21 | Soll es für öffentliche/halböffentliche Situationen eigene Character-Eigenschaften geben, etwa sehr schamlos / neutral … | ja schon | 35#188 → 35#189 | 📋 ja | – |
| 22 | Soll Alkohol/Drugs/Mood usw. Preferences beeinflussen können, aber niemals automatisch Zustimmung erzeugen? | ja aber meistens keine wirkliche willenskraft mehr zu reagieren, da reicht dann wenn der npc nickt und das ist consent | 35#188 → 35#189 | 📋 ja mit Zusatz · K1 | – |
| 23 | Soll eine Interaction mehrere Eskalationsstufen besitzen, zwischen denen NPC/Player dynamisch wechseln können? | wie genau? | 35#188 → 35#189 | ❓ offen/Rückfrage | E-IA-001 |
| 24 | Soll das Mod-System praktisch unbegrenzt neue Kategorien hinzufügen können, ohne dass wir dafür den C++-Core ändern … | genauer noxh bitte | 35#188 → 35#189 | ❓ offen/Rückfrage | E-IA-005 |

## Runde 3: Kategorien-Struktur (35#220)

**Datum:** 2026-09-11 · **Fragen:** 1–12 (12) · **Antwortnachrichten:** 35#221, 35#223 · **Module (Paketebene):** 03, 02 · **Status:** 📋 11, 🧭 1  
**Register (Paketebene, Quelle überschneidet sich):** E-PA-014, E-PA-015, E-PA-016

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jede Interaction-Kategorie beliebig viele Unterkategorien haben können, z. B. Category → Subcategory → Variant? | einmal genauer noch (35#221) → ja dann sicher (35#223) | 35#220 → 35#221,35#223 | 📋 ja (nachgereicht/geändert) | – |
| 2 | Soll dieselbe Kategorie völlig unterschiedliche Rollenprofile haben können, sodass ein NPC manche Rollen bevorzugt und … | ja sicher | 35#220 → 35#221 | 📋 ja | – |
| 3 | Soll jede Kategorie eigene Intensity-Dimensionen definieren dürfen, zusätzlich zu gemeinsamen Standardwerten wie Tempo, … | ja aber sag das noch genauer (35#221) → ok (35#223) | 35#220 → 35#221,35#223 | 📋 ja (nachgereicht/geändert) | – |
| 4 | Soll es pro Kategorie mehrere Presets geben können, z. B. Soft / Normal / Intense / Hardcore / Custom, wobei Mods … | ja schon aber bisschen genauer noch (35#221) → ja passt (35#223) | 35#220 → 35#221,35#223 | 📋 ja (nachgereicht/geändert) | – |
| 5 | Soll ein NPC für jede einzelne Kategorie eigene Einstellungen haben können, statt nur einen globalen „wie offen ist … | ja sicher, kann sich von person zu person auch ändern | 35#220 → 35#221 | 📋 ja mit Zusatz | – |
| 6 | Soll zusätzlich gelten: | ja natürlich jeder npc ist unterschiedlich | 35#220 → 35#221 | 📋 ja mit Zusatz | – |
| 7 | Sollen Characters auch spontane Ausnahmen haben können, also normalerweise etwas nicht bevorzugen, aber in einer … | ja genau, kann sich auch von einer person auf die andere ändern | 35#220 → 35#221 | 📋 ja mit Zusatz | – |
| 8 | Soll jede Kategorie definieren können, welche Locations passen: Bett, Couch, Dusche, Fahrzeug, Club, Outdoor, … | ja | 35#220 → 35#221 | 📋 ja | – |
| 9 | Soll jede Location zusätzlich beeinflussen: | kann sein | 35#220 → 35#221 | 🧭 bedingt/unverbindlich | – |
| 10 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Kategorien-Struktur] | ja genau | 35#220 → 35#221 | 📋 ja | – |
| 11 | Soll jede Kategorie beliebig viele Animation-/Audio-/Camera-Packs austauschbar unterstützen, damit Modder denselben … | ja | 35#220 → 35#221 | 📋 ja | – |
| 12 | Soll ein „Alles-in-einem-Pack“ existieren können, aber gleichzeitig dieselben Inhalte auch modular als einzelne Packs … | ja sicher Aber das wird ja sowieso schon eine mod | 35#220 → 35#221 | 📋 ja mit Zusatz | – |

## Runde 4: Präferenzprofile (35#226)

**Datum:** 2026-09-11 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#227 · **Module (Paketebene):** 05 · **Status:** 📋 14, 🧭 1  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-004, E-NP-005

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jeder erwachsene NPC ein eigenes Preference-Profil haben, das z. B. Offenheit, Spontanität, Privatsphäre, … | ja genau, das kann sich auch ändern | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 2 | Soll ein Teil davon fest durch die Persönlichkeit kommen und ein Teil durch Erfahrungen/Beziehungen veränderbar sein? | ja kann sein, machen wir noch genauer | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 3 | Soll es NPCs geben, die von Anfang an sehr offen sind, andere sehr zurückhaltend und wieder andere irgendwo dazwischen? | ja sicher | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 4 | Soll Attraction unabhängig von Beziehung sein? Also jemand kann starke körperliche Anziehung haben, ohne gleich … | ja | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 5 | Soll ein NPC unterschiedliche Werte haben für: | ja hängt von der person ab | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 6 | Soll es situative Preferences geben, z. B. abhängig von Ort, Stimmung, Tageszeit, Öffentlichkeit oder vorherigen Events? | ja wie in echt | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 7 | Soll ein NPC Dinge ausprobieren können, obwohl sie vorher nur Neutral/Situational waren, und danach seine Meinung … | ja, man kann sie auch vielleicht überreden dazu | 35#226 → 35#227 | 📋 ja mit Zusatz | E-NP-004 |
| 8 | Soll ein NPC nach positiven/negativen Erfahrungen dauerhafte Preference-Deltas bekommen können? | kann schon sein | 35#226 → 35#227 | 🧭 bedingt/unverbindlich | E-NP-004 |
| 9 | Soll es auch Dinge geben, die ein NPC fast nie ändert, weil sie Teil seiner Persönlichkeit/Identität sind? | kann bei manchen schon sein | 35#226 → 35#227 | 📋 Freitext | E-NP-004 |
| 10 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Präferenzprofile] | ja kann es schon | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 11 | Soll der Spieler bei wichtigen NPCs diese Preferences nicht einfach als komplette Liste sehen, sondern eher über … | ja | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 12 | Soll es trotzdem im optionalen Mod-/Debug-/Cheat-Modus möglich sein, alle Werte direkt einzusehen und zu verändern, … | ja sicher | 35#226 → 35#227 | 📋 ja | E-NP-004 |
| 13 | Soll es für zufällig generierte NPCs Archetypen + Zufallsvariation geben, damit nicht jeder einzeln von Hand gebaut … | ja das soll es sogar weil wenn wir 500000 npcs haben das von handzubauen ist gottlos | 35#226 → 35#227 | 📋 ja mit Zusatz | E-NP-004 |
| 14 | Soll wichtige Story-/Hero-NPCs dagegen vollständig handkuratierte Preference-Profile bekommen? | ja aber das ist eine mod also ist das erst dort zugänglich also bringt es nicht so viel | 35#226 → 35#227 | 📋 ja mit Zusatz | E-NP-004 |
| 15 | Soll sich ein NPC merken, wie der Spieler mit seinen Grenzen/Preferences umgegangen ist und das später Trust, … | ja | 35#226 → 35#227 | 📋 ja | E-NP-004 |

## Runde 5: Beziehung, Attraktion (35#228)

**Datum:** 2026-09-11 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#229 · **Module (Paketebene):** 05 · **Status:** ❌ 1, 📋 13, 🧭 1  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-007, E-NP-008, E-NP-009, E-NP-010, E-XX-005

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll Attraction, Trust, Affection, Romance, Comfort, Familiarity getrennte Werte sein und sich unabhängig entwickeln? | ja | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 2 | Soll ein One-Night-Stand auch mit niedrigem Trust/Romance, aber hoher Attraction + passendem Charaktertyp möglich sein? | ja sicher | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 3 | Soll es NPCs geben, bei denen Aussehen fast allein reicht, während andere erst Gespräch/Vertrauen brauchen? | ja | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 4 | Soll Attraction direkt von Dingen beeinflusst werden wie: | ja | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 5 | Soll jeder NPC individuelle Attraction Preferences haben, also nicht jeder findet dieselben … | ja sicherlich | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 6 | Soll es instant chemistry geben, also extrem hohe Anfangsanziehung zwischen bestimmten Personen? | ja genau | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 7 | Soll es auch negative Chemistry geben, also zwei Personen mögen sich einfach überhaupt nicht? | ja | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 8 | Soll ein One-Night-Stand später: | kann alles sein | 35#228 → 35#229 | 🧭 bedingt/unverbindlich | E-NP-007 |
| 9 | Soll ein NPC sich merken, ob frühere Begegnungen gut/schlecht waren und das zukünftige Verhalten beeinflussen? | ja kann we | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 10 | Soll Eifersucht ein eigenes System sein, abhängig von Persönlichkeit und Beziehung? | ja | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 11 | Soll offene Beziehung / Poly / Casual ebenfalls als Relationship-Typen möglich sein? | ja kann es sein, man kann auch fremdgehen was die Beziehung verschlechtern kann | 35#228 → 35#229 | 📋 ja mit Zusatz | E-NP-007 |
| 12 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Beziehung, Attraktion] | ja | 35#228 → 35#229 | 📋 ja | E-NP-007 |
| 13 | Soll der Player manche NPCs nur casual, andere nur romantisch und manche beides entwickeln können? | man soll theoretisch mit jedem npc sex haben können | 35#228 → 35#229 | 📋 Freitext | – |
| 14 | Soll das System auch komplette Ablehnung ohne Chance auf Änderung erlauben, wenn zwei Charaktere einfach nicht … | nein, da frauen das oft nur so tun als hätten sie kein interesse und das entwickelt sich dann anders | 35#228 → 35#229 | ❌ nein · K8 | – |
| 15 | Soll es seltene Fälle geben, bei denen eine eigentlich unwahrscheinliche Kombination durch starke gemeinsame Erlebnisse … | ja, aber nicht in der story sondern vielleicht eigenen fällen | 35#228 → 35#229 | 📋 ja mit Zusatz | – |

## Runde 6: Initiative, Flirt (35#230)

**Datum:** 2026-09-11 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#231 · **Module (Paketebene):** 05, 08 · **Status:** 📋 15  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-011

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll ein NPC den Spieler von selbst ansprechen/flirten, wenn Attraction + Context passen? | ja | 35#230 → 35#231 | 📋 ja | – |
| 2 | Soll es Unterschiede geben zwischen: | ja sicher | 35#230 → 35#231 | 📋 ja | – |
| 3 | Soll ein NPC auch nur wegen Aussehen/Outfit/Body sofort Interesse zeigen können? | ja definitiv, wahrscheinlich das erste woran er interesse hat | 35#230 → 35#231 | 📋 ja mit Zusatz | – |
| 4 | Soll Gespräch/Charisma/Humor das Interesse deutlich verändern können? | ja | 35#230 → 35#231 | 📋 ja | – |
| 5 | Soll es spontane Begegnungen geben, die nicht als Quest/Case markiert sind? | natürlich das ist ja das ziel | 35#230 → 35#231 | 📋 ja | – |
| 6 | Soll ein Club/Bar/Party-System die Wahrscheinlichkeit für Casual/One-Night-Stand-Interaktionen stark erhöhen? | ja schon, aber das kann theoretisch überall passieren | 35#230 → 35#231 | 📋 ja mit Zusatz | – |
| 7 | Soll es Orte geben, wo NPCs deutlich offener sind als im normalen Alltag? | ja | 35#230 → 35#231 | 📋 ja | – |
| 8 | Soll ein NPC selbst vorschlagen können, den Ort zu wechseln, z. B. privatere Location/Hotel/Home? | ja kann er | 35#230 → 35#231 | 📋 ja | – |
| 9 | Soll der Player auch sagen können: | ja | 35#230 → 35#231 | 📋 ja | – |
| 10 | Soll es Situationen geben, wo mehrere NPCs gleichzeitig Interesse zeigen und daraus … | ja kann es sein, und es kann sein dass man dann mehrere gleichzeitig hat | 35#230 → 35#231 | 📋 ja mit Zusatz | – |
| 11 | Soll ein NPC nach einer spontanen Begegnung am nächsten Tag anders reagieren können: | ja | 35#230 → 35#231 | 📋 ja | – |
| 12 | Soll der Spieler Kontakte/Nummern von NPCs bekommen können und sie später wieder treffen? | ja | 35#230 → 35#231 | 📋 ja | – |
| 13 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Initiative, Flirt] | ja | 35#230 → 35#231 | 📋 ja | – |
| 14 | Soll es NPCs geben, die fast nur Casual wollen und andere fast nur Beziehungen? | ja kann es geben, aber auch ein mittelding | 35#230 → 35#231 | 📋 ja mit Zusatz | – |
| 15 | Soll Ort + Tageszeit + Crowd + Alkohol + Stimmung + Beziehung + Attraction gemeinsam die Chance auf spontane … | ja | 35#230 → 35#231 | 📋 ja | – |

## Runde 7: Orte, Öffentlichkeit, Observer (35#232)

**Datum:** 2026-09-11 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#233 · **Module (Paketebene):** 08, 03 · **Status:** 📋 15  
**Register (Paketebene, Quelle überschneidet sich):** E-IA-006, E-IA-007, E-IA-008, E-IA-009, E-WE-005

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jede Location intern Tags besitzen wie private, semi_private, public, club, hotel, home, vehicle, outdoor, … | ja soll der spieler nicht sehen | 35#232 → 35#233 | 📋 ja | – |
| 2 | Soll derselbe NPC je nach Location komplett anders reagieren können? Also zuhause sehr offen, öffentlich eher … | ja | 35#232 → 35#233 | 📋 ja | – |
| 3 | Soll es NPCs geben, denen Öffentlichkeit egal oder sogar bevorzugt ist, während andere fast nur privat wollen? | ja | 35#232 → 35#233 | 📋 ja | – |
| 4 | Soll das System berücksichtigen, wie viele andere NPCs tatsächlich in Sicht-/Hörweite sind, statt nur einen simplen … | ja | 35#232 → 35#233 | 📋 ja | – |
| 5 | Soll ein zufälliger Beobachter unterschiedliche Reaktionen haben können: | ja kann sein | 35#232 → 35#233 | 📋 ja | – |
| 6 | Soll ein Observer dauerhaft als richtige Rolle geführt werden, mit eigener AI, Reaktion und Memory? | ja | 35#232 → 35#233 | 📋 ja | – |
| 7 | Soll der Spieler oder NPC einen Observer einladen können, aktiver Teil der Situation zu werden, wenn alle beteiligten … | ok | 35#232 → 35#233 | 📋 ja | – |
| 8 | Soll jemand auch spontan Interesse am Joinen zeigen können, statt immer eingeladen werden zu müssen? | ja | 35#232 → 35#233 | 📋 ja | – |
| 9 | Soll die laufende Situation darauf reagieren können, dass jemand neu dazukommt, z. B. neue … | natürlich | 35#232 → 35#233 | 📋 ja | – |
| 10 | Wenn eine neue Konstellation technisch nicht unterstützt wird: lieber kein Join statt hässlicher/kaputter Animation? | ok aber logisch, dann wehcselt man sich ab oder so was | 35#232 → 35#233 | 📋 ja mit Zusatz | – |
| 11 | Soll ein Character auch sagen können: „mit Person A okay, aber nicht wenn Person B dabei ist“? | ja | 35#232 → 35#233 | 📋 ja | – |
| 12 | Soll Hotels, Clubs, Wohnungen, Safehouses, Fahrzeuge, Outdoor-Spots usw. jeweils eigene … | ja | 35#232 → 35#233 | 📋 ja | – |
| 13 | Soll es spezielle Orte geben, die praktisch als Adult-oriented social hubs funktionieren, während die normale Welt … | ja | 35#232 → 35#233 | 📋 ja | – |
| 14 | Soll Security/Polizei nur dort reagieren, wo es in der Welt logisch wäre, statt überall dieselben Regeln zu haben? | was hat das mit adult zu tun | 35#232 → 35#233 | 📋 Freitext | – |
| 15 | Soll eine Location sich Dinge merken können, z. B. Hausbesitzer, Zugang, Kameras, bekannte NPCs, Beschwerden oder … | ja | 35#232 → 35#233 | 📋 ja | – |

## Runde 8: Pakete, Modularität (35#246)

**Datum:** 2026-09-11 · **Fragen:** 1–15 (15) · **Antwortnachrichten:** 35#247 · **Module (Paketebene):** 02 · **Status:** 📋 15  
**Register (Paketebene, Quelle überschneidet sich):** E-PA-017, E-PA-018, E-PA-019, E-PA-020, E-PA-021, E-PA-022, E-PA-023, E-PA-024

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll es ein Base Framework geben, das nur die technischen Systeme enthält und selbst möglichst wenig Content mitbringt? | sag das genauer | 35#246 → 35#247 | 📋 Rückfrage → pauschal bestätigt (35#249) | – |
| 2 | Soll es dazu getrennte Content-Packs geben, z. B.: | ich weis nicht wie gros die mod wird wenn sie riesig wird dann schon | 35#246 → 35#247 | 📋 Freitext | – |
| 3 | Soll man nur einzelne Packs installieren können, ohne das Komplettpaket zu brauchen? | wenn die mod riesig ist ja, sonst einfach deaktivieren | 35#246 → 35#247 | 📋 Freitext | – |
| 4 | Soll es trotzdem ein All-in-One-Pack geben, das alles enthält bzw. alle Module zusammen installiert? | ja standard wenn es nicht riesig ist | 35#246 → 35#247 | 📋 ja | – |
| 5 | Soll jedes Pack eigene Version, Dependencies und Kompatibilität haben? | ok ja aber wie genau meinst du das | 35#246 → 35#247 | 📋 ja | – |
| 6 | Soll das Base Framework möglichst stabil bleiben, während Content-Packs häufiger Updates bekommen können? | weiß noch nicht, mal schauen | 35#246 → 35#247 | 📋 Rückfrage → pauschal bestätigt (35#249) | – |
| 7 | Soll ein Pack sagen können: | ja | 35#246 → 35#247 | 📋 ja | – |
| 8 | Soll ein Character-/NPC-Pack eigene handgebaute Preferences, Animationen, Audio und Overrides mitbringen können? | ja | 35#246 → 35#247 | 📋 ja | – |
| 9 | Soll ein Animation-Pack für mehrere Kategorien wiederverwendbar sein können? | ja aber nur teilweise, da die charaktere anders reagieren und andere körpermaße haben und sich das damit ändert | 35#246 → 35#247 | 📋 ja mit Zusatz | – |
| 10 | Soll ein Voice-Pack unabhängig von Animation-/Interaction-Packs installierbar sein? | ok, wie genau aber, einfach andere sprachen? | 35#246 → 35#247 | 📋 ja mit Zusatz | – |
| 11 | Soll ein Location-Pack neue: | das soll im hauptspiel einfach drinnen sein ohne mods. Dort hat man halt ohne mod weniger zu tun | 35#246 → 35#247 | 📋 Freitext | – |
| 12 | Soll ein High-Intensity-Pack nur zusätzlichen Content hinzufügen, aber nicht das Base Framework verändern? | weiß nicht erkläre bitte | 35#246 → 35#247 | 📋 Rückfrage → pauschal bestätigt (35#249) | – |
| 13 | Soll der Mod Manager anzeigen: | ja | 35#246 → 35#247 | 📋 ja | – |
| 14 | Soll es Preset-Bundles geben wie: | kann man schauen | 35#246 → 35#247 | 📋 Rückfrage → pauschal bestätigt (35#249) | – |
| 15 | Soll ein Nutzer einzelne Kategorien aus einem installierten Pack trotzdem deaktivieren können, ohne das ganze Pack zu … | ja | 35#246 → 35#247 | 📋 ja | E-PA-018 |

## Runde 9A: Präferenz-DNA (35#260)

**Datum:** 2026-09-11 · **Fragen:** 1–45 (45) · **Antwortnachrichten:** 35#261 · **Module (Paketebene):** 05 · **Status:** 📋 45  
**Register (Paketebene, Quelle überschneidet sich):** E-LE-008, E-NP-012, E-NP-013, E-NP-014, E-NP-015, E-SK-014, E-SK-030, E-XX-017

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jeder erwachsene NPC beim Erzeugen eine feste Preference-DNA bekommen, die grundsätzlich reproduzierbar bleibt? | ja schon die kann sich aber ändern | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 2 | Soll diese DNA aus einem Seed erzeugt werden, damit derselbe NPC nach Save/Load dieselben Grundwerte behält? | ja es wird nur am anfang generiert | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 3 | Soll der Seed an die globale NPC-ID gekoppelt sein und nicht an Spawn-Zeit oder aktuelle Position? | ja genau, das wäre blöd in einer beziehung auf die wäre dann zurückgesetzt weil sich alles geändert hat | 35#260 → 35#261 | 📋 ja mit Zusatz | E-NP-012 |
| 4 | Soll die Preference-DNA getrennt sein von der normalen NPC-DNA für Aussehen, Beruf, Persönlichkeit usw., aber mit ihr … | ja genau wenn ich das richtig verstehe | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 5 | Soll es eine Hierarchie geben wie: | ok | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 6 | Soll jeder NPC eine Mischung aus Archetyp + individuellen Abweichungen bekommen? | wie genau | 35#260 → 35#261 | 📋 Freitext | E-NP-012 |
| 7 | Soll es möglich sein, dass zwei NPCs denselben Archetyp haben, aber sich trotzdem sehr unterschiedlich verhalten? | ja archetyp ist nicht alles oder | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 8 | Soll jeder Archetyp nur als Bias dienen und niemals als starre Schablone? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 9 | Soll das System verhindern, dass alle Werte einfach zufällig gewürfelt werden und dadurch unrealistische Kombinationen … | ja aber was meinst du mit unrealistisch | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 10 | Soll stattdessen ein korreliertes Generierungsmodell verwendet werden, bei dem manche Eigenschaften statistisch … | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 11 | Soll jeder NPC interne Persönlichkeitsdimensionen haben wie Offenheit, Impulsivität, Schüchternheit, Dominanz im … | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 12 | Sollen diese Werte kontinuierlich sein statt nur shy / normal / bold? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 13 | Soll das System daraus trotzdem lesbare interne Archetypen ableiten können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 14 | Soll Persönlichkeit langfristig relativ stabil bleiben? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 15 | Soll starke Story-/Life-Events kleine dauerhafte Änderungen verursachen können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 16 | Soll Mood kurzfristig unabhängig von Persönlichkeit schwanken? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 17 | Soll ein schüchterner NPC trotzdem in bestimmten Kontexten sehr direkt werden können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 18 | Soll ein grundsätzlich offener NPC trotzdem bei bestimmten Personen oder Themen zurückhaltend sein können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 19 | Soll Persönlichkeit Einfluss darauf haben, wer zuerst flirtet/Initiative zeigt? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 20 | Soll Persönlichkeit auch beeinflussen, wie schnell ein NPC von Casual zu emotionaler Bindung wechselt? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 21 | Soll jeder NPC ein eigenes Attraction Profile haben? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 22 | Soll Attraction aus mehreren unabhängigen Komponenten bestehen, z. B. körperlich, sozial, emotional, Status/Prestige, … | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 23 | Soll ein NPC manche dieser Komponenten fast egal finden und andere extrem wichtig? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 24 | Soll körperliche Attraction innerhalb von Sekunden entstehen können, während emotionale Attraction viel länger braucht? | [Antwort enthält explizite Details; nicht wiedergegeben] | 35#260 → 35#261 | 📋 Freitext | E-NP-012 |
| 25 | Soll es eine Initial Attraction geben, die sich beim ersten Treffen bildet? | nur bei dates, bei onenightstands eher weniger | 35#260 → 35#261 | 📋 Freitext | E-NP-012 |
| 26 | Soll Initial Attraction später durch Verhalten deutlich steigen oder fallen können? | nur bei dates, bei onenightstands eher weniger | 35#260 → 35#261 | 📋 Freitext | E-NP-012 |
| 27 | Soll Kleidung/Styling den Eindruck verändern, aber den zugrunde liegenden Körper-/Gesichts-Preference nicht komplett … | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 28 | Soll Hygiene/Grooming einen echten Einfluss haben? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 29 | Soll Stimme/Akzent/Art zu sprechen Attraction beeinflussen können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 30 | Soll Reputation/Status nur bei manchen NPCs wichtig sein? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 31 | Soll jeder NPC zusätzlich zu globalen Preferences partner-spezifische Deltas speichern können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 32 | Soll also dieselbe Person mit Partner A Dinge bevorzugen, die sie mit Partner B nicht will? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 33 | Soll Chemistry ein eigener Faktor sein, der nicht nur aus Attraction + Trust berechnet wird? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 34 | Soll diese Chemistry teilweise sofort entstehen und teilweise durch gemeinsame Erlebnisse wachsen? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 35 | Soll es negative Chemistry geben, die trotz objektiv hoher Attraction alles unpassend macht? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 36 | Soll Chemistry bidirektional unterschiedlich sein können, also A ist stark interessiert, B nur wenig? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 37 | Soll ein NPC nach mehreren guten Begegnungen einen eigenen Partner Memory Profile bekommen? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 38 | Soll dieses Profil Dinge speichern wie Comfort, Trust, Attraction, frühere positive/negative Erfahrungen, bevorzugte … | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 39 | Soll das System veraltete partner-spezifische Daten komprimieren/ausdünnen können, damit 500k NPCs den Save nicht … | ja aber es soll trotzdem gespeichert sein das man die Person kennt und was sie für vorlieben hatte, außer sie ist tod | 35#260 → 35#261 | 📋 ja mit Zusatz | E-NP-012 |
| 40 | Soll nur für wirklich relevante Beziehungen ein detailliertes Profil persistieren und für flüchtige Begegnungen … | ok aber wenn man einen one night stand hat dann muss das genau so sein wie bei einer beziehung von der Genauigkeit | 35#260 → 35#261 | 📋 ja mit Zusatz | E-NP-012 |
| 41 | Soll die Region, in der ein NPC aufgewachsen ist, leichte statistische Einflüsse auf Verhalten/Offenheit/Privatsphäre … | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 42 | Soll das niemals eine harte Regel sein, sondern nur Population-Bias? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 43 | Soll Stadt/Land, Nightlife, Beruf, Alter, soziale Umgebung usw. ebenfalls Population-Biases beeinflussen können? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 44 | Soll der Spieler diese Biases normalerweise nicht direkt sehen, sondern nur durch die Welt wahrnehmen? | ja | 35#260 → 35#261 | 📋 ja | E-NP-012 |
| 45 | Soll jeder Bundesstaat eigene Generation-Profile besitzen dürfen, damit Populationen subtil unterschiedlich wirken, … | die leute aus den verschiedenen gebieten können anders sein ja, also leicht andere stimme, aussehen und so weiter, es soll keine … | 35#260 → 35#261 | 📋 Freitext · K12 | E-NP-012 |

## Runde 9B.1: Kern/Gedächtnis (35#268)

**Datum:** 2026-09-11 · **Fragen:** 1–30 (30) · **Antwortnachrichten:** 35#269 · **Module (Paketebene):** 05 · **Status:** 📋 30  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-016

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jeder NPC eine Core Personality haben, die sich nur sehr langsam oder durch starke Ereignisse verändert? | ja | 35#268 → 35#269 | 📋 ja | – |
| 2 | Soll es daneben fast-changing States geben wie Mood, Stress, Nervosität, Confidence, Attraction und Comfort? | ja | 35#268 → 35#269 | 📋 ja | – |
| 3 | Soll es mid-term States geben, die sich über Tage/Wochen ändern, z. B. Trust, Familiarity, Attachment, Jealousy? | ja | 35#268 → 35#269 | 📋 ja | – |
| 4 | Soll es long-term States geben, die sich über Monate/Jahre entwickeln, z. B. tiefe Bindung, Ressentiment, Gewohnheiten … | ja | 35#268 → 35#269 | 📋 ja | – |
| 5 | Soll ein NPC bestimmte Dinge nie komplett vergessen, auch wenn andere Werte mit der Zeit abklingen? | ja | 35#268 → 35#269 | 📋 ja | – |
| 6 | Soll es sogenannte Anchor Memories geben, also besonders wichtige Erlebnisse, die dauerhaft Gewicht behalten? | ja | 35#268 → 35#269 | 📋 ja | – |
| 7 | Soll normale Erinnerung mit der Zeit schwächer/ungenauer werden können? | ja | 35#268 → 35#269 | 📋 ja | – |
| 8 | Soll ein NPC manche Erinnerungen stärker behalten, wenn sie besonders positiv oder negativ waren? | ja | 35#268 → 35#269 | 📋 ja | – |
| 9 | Soll ein NPC mit sehr guter Erinnerung mehr Details behalten als jemand mit schlechterer Memory-Trait? | ja | 35#268 → 35#269 | 📋 ja | – |
| 10 | Soll Persönlichkeit beeinflussen, wie stark Ereignisse emotional gespeichert werden? | ja | 35#268 → 35#269 | 📋 ja | – |
| 11 | Soll ein NPC nach einem einmaligen Treffen eine Known Person werden, wenn die Begegnung relevant genug war? | ja | 35#268 → 35#269 | 📋 ja | – |
| 12 | Soll ein kurzes belangloses Gespräch eventuell nur eine sehr kleine Erinnerung hinterlassen? | ja | 35#268 → 35#269 | 📋 ja | – |
| 13 | Soll ein Date / One-Night-Stand / wichtiger gemeinsamer Abend automatisch ein Detailed Relationship Record erzeugen? | ja | 35#268 → 35#269 | 📋 ja | – |
| 14 | Soll sich ein NPC beim Wiedersehen erinnern an: | ja aber es kann sein dass er sich nicht mehr genau erinnert | 35#268 → 35#269 | 📋 ja mit Zusatz | – |
| 15 | Soll es Unterschiede geben zwischen: | ja genau | 35#268 → 35#269 | 📋 ja | – |
| 16 | Soll ein NPC nach langer Zeit überrascht sein, dich wiederzusehen? | ja | 35#268 → 35#269 | 📋 ja | – |
| 17 | Soll er dich nach Jahren auch anders bewerten können, wenn du dich stark verändert hast? | ja | 35#268 → 35#269 | 📋 ja | – |
| 18 | Soll Aussehen/Status/Reputation nach langer Zeit beim Wiedersehen erneut Einfluss auf Attraction haben? | ja | 35#268 → 35#269 | 📋 ja | – |
| 19 | Soll ein NPC alte Chemistry teilweise behalten, aber trotzdem neu bewerten? | ja | 35#268 → 35#269 | 📋 ja | – |
| 20 | Soll ein Wiedersehen nach langer Funkstille spezielle Reaktionen triggern können? | ja | 35#268 → 35#269 | 📋 ja | – |
| 21 | Soll sich Trust deutlich schneller verändern können als tiefe Attachment? | ja | 35#268 → 35#269 | 📋 ja | – |
| 22 | Soll Attraction stark schwanken können, ohne dass eine Beziehung sofort zerbricht? | ja | 35#268 → 35#269 | 📋 ja | – |
| 23 | Soll Comfort mit einer Person langfristig wachsen, wenn Interaktionen wiederholt positiv verlaufen? | ja | 35#268 → 35#269 | 📋 ja | – |
| 24 | Soll Familiarity irgendwann dazu führen, dass bestimmte Interaktionen/Verhaltensweisen weniger awkward wirken? | ja | 35#268 → 35#269 | 📋 ja | – |
| 25 | Soll es einen Unterschied geben zwischen: | ja | 35#268 → 35#269 | 📋 ja | – |
| 26 | Soll ein NPC nach vielen positiven Erfahrungen offener werden können, ohne dass seine Persönlichkeit komplett anders … | ja | 35#268 → 35#269 | 📋 ja | – |
| 27 | Soll ein NPC nach mehreren schlechten Erfahrungen dauerhaft vorsichtiger werden können? | ja | 35#268 → 35#269 | 📋 ja | – |
| 28 | Soll Beziehungshistorie auch beeinflussen, wie schnell Missverständnisse verziehen werden? | ja | 35#268 → 35#269 | 📋 ja | – |
| 29 | Soll es möglich sein, dass zwei NPCs nach langer Beziehung auseinanderdriften, obwohl nichts „Großes“ passiert? | ja | 35#268 → 35#269 | 📋 ja | – |
| 30 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Kern/Gedächtnis] | ja | 35#268 → 35#269 | 📋 ja | – |

## Runde 9B.2: Lernen, Dates, Beziehungen (35#270)

**Datum:** 2026-09-11 · **Fragen:** 1–60 (60) · **Antwortnachrichten:** 35#271 · **Module (Paketebene):** 05 · **Status:** ❌ 1, ❓ 1, 📋 57, 🧭 1  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-016, E-NP-017, E-NP-018

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll ein NPC neue Vorlieben durch positive Erfahrungen entwickeln können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 2 | Soll er eine bisher neutrale Sache nach mehreren guten Erfahrungen irgendwann Preferred einstufen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 3 | Soll das Gegenteil ebenfalls passieren: etwas früher Bevorzugtes wird nach schlechten Erfahrungen weniger attraktiv? | ja | 35#270 → 35#271 | 📋 ja | – |
| 4 | Soll eine einzelne sehr starke Erfahrung Preferences sofort deutlich verändern können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 5 | Soll Persönlichkeit bestimmen, wie schnell jemand seine Preferences verändert? | ja | 35#270 → 35#271 | 📋 ja | – |
| 6 | Soll jemand sehr experimentierfreudig sein können, während andere fast immer bei Bekanntem bleiben? | ja | 35#270 → 35#271 | 📋 ja | – |
| 7 | Soll ein NPC auch nach Jahren noch neue Preferences entdecken können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 8 | Soll ein NPC Dinge wieder „entlernen“ bzw. Interesse verlieren können, wenn sie lange keine Rolle spielen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 9 | Soll es bei Preferences einen Unterschied geben zwischen: | ja | 35#270 → 35#271 | 📋 ja | – |
| 10 | Soll der Spieler solche Veränderungen hauptsächlich über Verhalten/Dialog bemerken statt über Werte? | ja | 35#270 → 35#271 | 📋 ja | – |
| 11 | Soll ein Date je nach NPC völlig unterschiedlich aufgebaut sein können, z. B. Bar, Restaurant, Kino, Autofahrt, … | ja | 35#270 → 35#271 | 📋 ja | – |
| 12 | Soll ein Date auch spontan entstehen können, ohne vorher geplanten Termin? | ja | 35#270 → 35#271 | 📋 ja | – |
| 13 | Soll ein Date schlecht laufen können, obwohl Attraction hoch war? | ja | 35#270 → 35#271 | 📋 ja | – |
| 14 | Soll Gesprächskompatibilität genauso wichtig sein können wie Aussehen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 15 | Soll ein NPC ein Date frühzeitig verlassen können, wenn es richtig schlecht läuft? | ja | 35#270 → 35#271 | 📋 ja | – |
| 16 | Soll ein gutes Date nicht automatisch in Adult-Content enden? | kann sein aber oft mit | 35#270 → 35#271 | 🧭 bedingt/unverbindlich | – |
| 17 | Soll ein NPC nach einem sehr guten Date selbst ein zweites Treffen vorschlagen können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 18 | Soll es NPCs geben, die Dates kaum mögen und lieber spontane/casual Begegnungen bevorzugen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 19 | Soll es auch sehr klassische romantische NPCs geben, die Casual fast komplett vermeiden? | ja | 35#270 → 35#271 | 📋 ja | – |
| 20 | Soll ein Date Erinnerungen erzeugen, die später konkrete Dialoge ermöglichen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 21 | Soll ein One-Night-Stand manchmal tatsächlich nur einmalig bleiben und danach kaum Beziehung entstehen? | nein, es kann sein das man daraus eine beziehung macht | 35#270 → 35#271 | ❌ nein | – |
| 22 | Soll dieselbe Begegnung aber manchmal überraschend zu stärkerer Attraction führen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 23 | [Wortlaut explizit; nicht wiedergegeben – Themenfamilie: Lernen, Dates, Beziehungen] | ja | 35#270 → 35#271 | 📋 ja | – |
| 24 | Soll es Fälle geben, wo beide eigentlich nur Casual wollten, aber einer später Gefühle entwickelt? | ja | 35#270 → 35#271 | 📋 ja | – |
| 25 | Soll es auch umgekehrt sein: vorher romantisches Interesse, danach merkt einer, dass es doch nicht passt? | ja | 35#270 → 35#271 | 📋 ja | – |
| 26 | Soll ein One-Night-Stand einen genauso detaillierten Memory-/Preference-State erzeugen wie andere relevante Beziehungen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 27 | Soll der NPC daraus lernen, was mit diesem bestimmten Partner gut/schlecht funktioniert? | ja | 35#270 → 35#271 | 📋 ja | – |
| 28 | Soll wiederholter Casual-Kontakt automatisch in einen eigenen Relationship-State übergehen können, z. B. … | ja | 35#270 → 35#271 | 📋 ja | – |
| 29 | Soll dieser Relationship-State entstehen können, ohne dass einer der beiden romantische Gefühle hat? | ja | 35#270 → 35#271 | 📋 ja | – |
| 30 | Soll sich Casual später jederzeit in Romance entwickeln können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 31 | Soll Fremdgehen nur dann zählen, wenn tatsächlich eine exklusive Beziehung besteht? | ja | 35#270 → 35#271 | 📋 ja | – |
| 32 | Soll offene/poly Beziehungen eigene Regeln haben, sodass dort dieselbe Handlung nicht automatisch als Betrug gilt? | ja | 35#270 → 35#271 | 📋 ja | – |
| 33 | Soll jeder NPC individuelle Eifersuchtsstärke besitzen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 34 | Soll ein sehr eifersüchtiger NPC schon auf Flirten reagieren können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 35 | Soll ein wenig eifersüchtiger NPC vieles ignorieren, solange vereinbarte Grenzen nicht überschritten werden? | ja | 35#270 → 35#271 | 📋 ja | – |
| 36 | Soll ein NPC Betrug auf unterschiedliche Weise herausfinden können: | ja | 35#270 → 35#271 | 📋 ja | – |
| 37 | Soll die Sicherheit der Information eine Rolle spielen, also Gerücht ≠ Beweis? | ja | 35#270 → 35#271 | 📋 ja | – |
| 38 | Soll ein NPC den Spieler zu Unrecht verdächtigen können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 39 | Soll man versuchen können, Missverständnisse aufzuklären? | ja | 35#270 → 35#271 | 📋 ja | – |
| 40 | Soll ein tatsächlicher Betrug Trust stark senken können, ohne automatisch jede Beziehung sofort zu beenden? | ja | 35#270 → 35#271 | 📋 ja | – |
| 41 | Soll ein NPC selbst eine Beziehung beenden können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 42 | Soll der Spieler natürlich ebenfalls Schluss machen können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 43 | Soll Trennung unterschiedliche Gründe speichern, statt einfach nur Relationship = ended? | ja | 35#270 → 35#271 | 📋 ja | – |
| 44 | Soll nach einer Trennung noch Attraction vorhanden sein können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 45 | Soll auch Hass/Ressentiment neben Attraction existieren können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 46 | Soll Ex-Partner später wieder Casual werden können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 47 | Soll eine Beziehung nach langer Zeit erneut entstehen können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 48 | Soll eine zweite Beziehung denselben beiden Personen anders verlaufen, weil beide die erste noch erinnern? | ja | 35#270 → 35#271 | 📋 ja | – |
| 49 | Soll Vergebung von Persönlichkeit + Ereignisschwere + Zeit + späterem Verhalten abhängen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 50 | Soll es Dinge geben, die manche NPCs niemals verzeihen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 51 | Soll ein Relationship-Event Auswirkungen auf gemeinsame Freunde haben können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 52 | Soll Freundeskreiswissen über Beziehungen/Fremdgehen/Trennungen verbreiten können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 53 | Soll Gossip realistisch begrenzt sein, also nicht sofort die ganze 2.000-km²-Welt Bescheid wissen? 😭 | was ist gossip | 35#270 → 35#271 | ❓ offen/Rückfrage | – |
| 54 | Soll Reputation nur in tatsächlich verbundenen sozialen Kreisen beeinflusst werden? | ja | 35#270 → 35#271 | 📋 ja | – |
| 55 | Soll es NPCs geben, die Geheimnisse zuverlässig behalten, und andere, die alles weitererzählen? | ja | 35#270 → 35#271 | 📋 ja | – |
| 56 | Soll öffentliche Bekanntheit des Players die Verbreitung stark erhöhen können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 57 | Soll eine berühmte Person deshalb mehr Schwierigkeiten haben, Dinge privat zu halten? | ja | 35#270 → 35#271 | 📋 ja | – |
| 58 | Soll Social Media / Nachrichten / Fotos je nach Kontext eine Rolle spielen können? | ja | 35#270 → 35#271 | 📋 ja | – |
| 59 | Soll der Spieler versuchen können, Gerüchte zu korrigieren oder zu ignorieren? | ja | 35#270 → 35#271 | 📋 ja | – |
| 60 | Soll ein falsches Gerücht Beziehungen real beschädigen können, bis es aufgeklärt wird? | ja | 35#270 → 35#271 | 📋 ja | – |

## Runde 9B.3: Langzeit, Alter, Tod (35#272)

**Datum:** 2026-09-11 · **Fragen:** 1–50 (50) · **Antwortnachrichten:** 35#273 · **Module (Paketebene):** 05, 06 · **Status:** 📋 50  
**Register (Paketebene, Quelle überschneidet sich):** E-NP-016

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll sich ein NPC über Jahre sichtbar verändern können, nicht nur über Tage/Wochen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 2 | Soll Alterung Aussehen, Stimme, Fitness, Verhalten und Lifestyle langsam verändern? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 3 | Soll Persönlichkeit im Kern stabil bleiben, aber durch starke Lebensereignisse teilweise verschoben werden können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 4 | Soll eine sehr lange Beziehung einen NPC stärker prägen als viele kurze Begegnungen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 5 | Soll ein NPC nach mehreren Jahren ohne Kontakt manche Details vergessen, aber wichtige Erinnerungen behalten? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 6 | Soll die Stärke einer alten Beziehung beeinflussen, wie schnell Erinnerungen verblassen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 7 | Soll ein früherer One-Night-Stand nach 10 Jahren noch erkannt werden können, wenn das Ereignis bedeutend genug war? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 8 | Soll ein NPC jemanden gelegentlich erst nach kurzem Überlegen wiedererkennen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 9 | Soll stark verändertes Aussehen das Wiedererkennen erschweren können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 10 | Soll Stimme/Gang/Name/andere Hinweise beim Wiedererkennen helfen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 11 | Soll ein NPC nach langer Zeit selbst Kontakt aufnehmen können, obwohl jahrelang Funkstille war? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 12 | Soll es Wiedersehensreaktionen geben wie Freude, Überraschung, Distanz, Nostalgie, Ärger oder erneute Attraction? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 13 | Soll alte Chemistry manchmal sofort wieder da sein? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 14 | Soll alte Chemistry manchmal komplett verschwunden sein? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 15 | Soll sich die Beziehung beim Wiedersehen grundsätzlich neu entwickeln können, statt alten Status einfach … | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 16 | Soll ein NPC inzwischen andere Beziehungen aufgebaut haben können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 17 | Soll Heirat/Langzeitpartnerschaft möglich sein? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 18 | Soll ein NPC Kinder/Familie haben können, unabhängig vom Adult-Mod-System? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 19 | Soll Familienstatus Entscheidungen und verfügbare Zeit beeinflussen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 20 | Soll ein NPC eine Beziehung geheim halten können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 21 | Soll ein NPC mehrere parallele Beziehungen führen können, wenn seine Relationship-Regeln das zulassen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 22 | Soll geheime Untreue langfristig unentdeckt bleiben können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 23 | Soll sie auch Jahre später durch neue Informationen auffliegen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 24 | Soll ein NPC nach Trennung eine neue Beziehung beginnen können und trotzdem Erinnerungen an den Ex behalten? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 25 | Soll Ex-Beziehungen spätere Beziehungen indirekt beeinflussen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 26 | Soll vergangene schlechte Behandlung einen NPC vorsichtiger machen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 27 | Soll vergangene gute Behandlung Erwartungen an spätere Beziehungen erhöhen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 28 | Soll ein NPC nach mehreren schlechten Beziehungen seine Bindungsbereitschaft verändern können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 29 | Soll umgekehrt eine sehr gute neue Beziehung alte negative Muster teilweise abbauen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 30 | Soll diese Veränderungen nicht garantiert sein, sondern stark von Persönlichkeit abhängen? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 31 | Soll Tod eines NPCs permanent im World State gespeichert bleiben? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 32 | Soll nach Tod die vollständige relevante Relationship-/History-Datenbasis erhalten bleiben, statt einfach gelöscht zu … | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 33 | Soll nur unnötiger technischer Detailstate eines toten NPCs später komprimiert werden? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 34 | Soll andere NPCs auf einen Tod reagieren und ihn erinnern können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 35 | Soll Tod eines Partners langfristige Auswirkungen auf einen Character haben können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 36 | Soll Trauer je nach Beziehung unterschiedlich stark/lang sein? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 37 | Soll ein Character später trotzdem neue Relationships eingehen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 38 | Soll das Spiel auch nach Jahrzehnten noch wissen, wer früher mit wem verbunden war? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 39 | Soll historische Beziehungen für Dialoge, Fälle, Familiengeschichten und World Lore abfragbar bleiben? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 40 | Soll bei Zeitreisen die Relationship-History sauber zwischen Zeitständen unterschieden werden? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 41 | Soll ein Character in der Vergangenheit natürlich noch nichts von Ereignissen wissen, die erst in seiner Zukunft … | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 42 | Soll Veränderungen an der Vergangenheit potenziell Relationship-History der Gegenwart verändern können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 43 | Soll das bei großen Änderungen über unser allgemeines Temporal World-State-System laufen und nicht über Sondercode der … | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 44 | Soll ein NPC seinen Wohnort wechseln können und trotzdem dieselbe Identität/History behalten? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 45 | Soll ein NPC zwischen den sieben States umziehen können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 46 | Soll Beruf, Einkommen oder Lebenssituation sich langfristig ändern können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 47 | Soll das Auswirkungen auf Dating-/Social-Verhalten und Locations haben können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 48 | Soll Statusverlust/-gewinn Attraction bei manchen Personen verändern, bei anderen praktisch gar nicht? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 49 | Soll ein NPC seine äußerlichen Preferences im Laufe seines Lebens etwas verändern können? | passt alles | 35#272 → 35#273 | 📋 ja | – |
| 50 | Soll das System verhindern, dass Character Development zu schnell chaotisch wird, also langfristige Veränderungen … | passt alles | 35#272 → 35#273 | 📋 ja | – |

## Runde 9C: Weltsimulation, Speicher (35#278)

**Datum:** 2026-09-11 · **Fragen:** 1–40 (40) · **Antwortnachrichten:** 35#279 · **Module (Paketebene):** 05, 02 · **Status:** 📋 40  
**Register (Paketebene, Quelle überschneidet sich):** E-KO-001, E-LE-001, E-LE-002, E-LE-003

| Nr. | Frage | Antwort | Quelle | Status | Register |
|---:|---|---|---|---|---|
| 1 | Soll jeder NPC grundsätzlich eine dauerhafte globale NPC-ID haben, auch wenn er gerade nicht geladen ist? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 2 | Soll es mehrere Simulation-Tiers geben, z. B.: | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 3 | Soll ein NPC automatisch zwischen diesen Tiers wechseln können, ohne seine Identität oder History zu verlieren? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 4 | Soll ein NPC in Tier 3 nicht frameweise simuliert werden, sondern nur über vereinfachte Ereignis-/Zeitschritte? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 5 | Soll Tier 3 trotzdem Dinge wie Jobwechsel, Umzug, Beziehung, Trennung, Tod usw. grob weiterentwickeln können? | ja aber halt nicht der jetzige ort nicht der ganze weg und so weiter | 35#278 → 35#279 | 📋 ja mit Zusatz | E-KO-001 |
| 6 | Soll die Genauigkeit der Simulation steigen, sobald ein NPC für den Spieler wichtiger wird? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 7 | Soll ein zufälliger unbekannter NPC deutlich weniger State brauchen als jemand, den der Spieler kennt? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 8 | Soll ein früherer One-Night-Stand automatisch in einen höheren Persistenz-Tier wechseln, damit die Details erhalten … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 9 | Soll ein NPC mit starker Beziehung/Story-Verbindung dauerhaft in einem hohen Persistenz-Tier bleiben? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 10 | Soll ein NPC später wieder heruntergestuft werden können, wenn er jahrzehntelang irrelevant ist, aber seine History … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 11 | Soll die Welt zwischen Spielsessions nicht real in Echtzeit weiterlaufen, außer wir aktivieren bewusst eine … | die welt läuft nur wenn man gerade spielt, im online wird das anders sein | 35#278 → 35#279 | 📋 Freitext | E-KO-001 |
| 12 | Wenn der Spieler zehn In-Game-Tage vorspult/schläft/reist: Soll Background-Simulation diese Zeit ebenfalls verarbeiten? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 13 | Soll Background-Simulation event-basiert arbeiten statt jede Sekunde durchzurechnen? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 14 | Beispiel: statt 86.400 Sekunden eines NPC-Tages: | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 15 | Soll Zufall dabei immer deterministisch aus NPC-ID + World Seed + Zeitfenster entstehen, damit Save/Load reproduzierbar … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 16 | Soll ein wichtiger NPC genauer simuliert werden als 50.000 unbekannte Personen in einem anderen State? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 17 | Soll ein NPC außerhalb der geladenen Welt trotzdem eine plausible Tagesroutine besitzen? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 18 | Soll ein NPC, wenn der Spieler plötzlich zu ihm reist, einen konsistenten aktuellen Zustand bekommen, der aus der … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 19 | Soll das System verhindern, dass NPCs sich im Hintergrund gleichzeitig an zwei Orten befinden? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 20 | Soll Reisen zwischen Städten/States auch für Background-NPCs Zeit kosten? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 21 | Soll für einen unbekannten NPC nur ein kompaktes generiertes Preference-Profil gespeichert werden, statt hunderte … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 22 | Sollen detaillierte Werte erst materialisiert werden, wenn der NPC wirklich relevant wird? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 23 | Soll der ursprüngliche Seed trotzdem gespeichert bleiben, damit materialisierte Werte exakt reproduzierbar sind? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 24 | Soll bei einer relevanten Begegnung der aktuelle generierte Zustand eingefroren/persistent werden, sodass spätere … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 25 | Soll ein NPC nur detaillierte Relationship-Edges für tatsächlich relevante Personen speichern? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 26 | Für völlig unbekannte NPC↔NPC-Beziehungen: reicht eine vereinfachte Background-Repräsentation? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 27 | Soll eine echte Beziehung zwischen zwei NPCs automatisch detaillierter persistent werden, auch wenn der Player nichts … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 28 | Soll dasselbe für Ehe/Familie/enge Freundschaften gelten? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 29 | Soll kurzfristige Attraction zwischen zwei Hintergrund-NPCs nach einiger Zeit wieder aus dem Detailstate verschwinden … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 30 | Soll wichtige History dagegen niemals einfach gelöscht werden? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 31 | Soll das Save-System Delta-basiert arbeiten: Es speichert hauptsächlich Änderungen gegenüber deterministisch … | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 32 | Soll also ein NPC, bei dem sich nie etwas verändert hat, praktisch nur NPC_ID + Seed brauchen? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 33 | Soll erst bei echten Änderungen zusätzliche Daten gespeichert werden? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 34 | Soll NPC-Memory in verschiedene Klassen aufgeteilt werden: | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 35 | Soll Short-Term-Memory automatisch zusammengefasst werden können, statt jeden einzelnen Event für immer zu speichern? | ja | 35#278 → 35#279 | 📋 ja | E-KO-001 |
| 36 | Soll z. B. aus 50 ähnlichen kleinen Ereignissen später eine kompakte Erinnerung entstehen wie: | ja solange der npc sich an alles einzeln erinnern kann | 35#278 → 35#279 | 📋 ja mit Zusatz | – |
| 37 | Soll wichtige Detail-History für Player-bezogene Beziehungen trotzdem einzeln erhalten bleiben? | ja | 35#278 → 35#279 | 📋 ja | – |
| 38 | Soll Save-Kompression automatisch genutzt werden, solange sie Ladezeiten nicht gottlos macht? | ja | 35#278 → 35#279 | 📋 ja | – |
| 39 | Soll alte Save-Versionen beim Laden deterministisch migriert werden, wenn wir NPC-/Preference-Systeme später ändern? | ja | 35#278 → 35#279 | 📋 ja | – |
| 40 | Soll die Architektur als Ziel haben, dass selbst mit ~500.000 persistenten NPC-Identitäten der normale Save eher im … | ich habe das so gefunden: also ja eher kleiner Typische Größenübersicht nach Genre ¦ GenreDurchschnittliche Größe pro … | 35#278 → 35#279 | 📋 Freitext | – |
