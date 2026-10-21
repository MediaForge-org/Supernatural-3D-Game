# Adult 06: Körper, Physik und Gesundheit

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Planungsstand zur Körpersimulation und zu Gesundheitsfolgen. Der Abschnitt bleibt bewusst auf der Ebene von Systemen und Zuständen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Der Kern gehört zum **gesamten Spiel**; das Erwachsenen-Paket fügt nur Erweiterungen hinzu.

**Inhalt**

1. [Ein gemeinsamer Human Core](#1-ein-gemeinsamer-human-core)
2. [Körpermodell](#2-körpermodell)
3. [Weichgewebe und Sekundärbewegung](#3-weichgewebe-und-sekundärbewegung)
4. [Biomechanische Grenzen](#4-biomechanische-grenzen)
5. [Zustände: Erregung, Empfindung, Ermüdung, Erholung](#5-zustände-erregung-empfindung-ermüdung-erholung)
6. [Verletzung und Risiko](#6-verletzung-und-risiko)
7. [Flüssigkeiten, Hygiene, Kleidung, Möbel](#7-flüssigkeiten-hygiene-kleidung-möbel)
8. [Schwangerschaft, Verhütung, Infektionen](#8-schwangerschaft-verhütung-infektionen)
9. [Reaktionen und Gesicht](#9-reaktionen-und-gesicht)
10. [Ableitbare Tests](#10-ableitbare-tests)

---

## 1. Ein gemeinsamer Human Core

📋 Der wichtigste Architekturgrundsatz: **Es gibt ein hochwertiges Menschensystem für das ganze Spiel.** Das Erwachsenen-Paket hat keinen eigenen Körper, kein zweites Rig, kein zweites Charaktersystem. Muskelspannung, Fett- und Weichgewebe, Haut, Gewicht, Haar, Kleidung und Schmuck existieren immer und reagieren auf Bewegung und Kontakt; dasselbe gilt für Atmung, Schweiß und Körpertemperatur. Der Entwickler betonte (10.09., 12.09.), dass das System im normalen Spiel (Laufen, Training, Kampf) genauso gilt und dass es ‚nicht aus drei Elementen‘ bestehen darf, sondern realistisch aufgebaut sein muss.

Die Erweiterungen des Pakets: Profile, Bindungen, Content, höhere Solver- und Kontaktpräzision in relevanten Zonen ([Modul 02](02-Architektur-Pakete-Technik.md#1-leitprinzipien)).

---

## 2. Körpermodell

📋 Der Körper ist ein **persistenter, veränderlicher Zustand** mit kontinuierlichen Parametern: Knochenbau, Proportionen, Größe, Becken, Muskelmasse, Fettverteilung, Haltung, **Asymmetrien**. Einzelne Körperteile derselben Person sind nicht exakt gleich (der Entwickler nannte unterschiedlich lange Beine als Beispiel); jeder Mensch ist etwas anders. Es gibt keine kleine Zahl fester Stufen: Maße und Formen sind grenzenlos kontinuierlich, nur plausibel gekoppelt (Gewicht, Training, Schwangerschaft, Alter, kosmetische Eingriffe verändern sie langfristig).

- Weichgewebe wird aus der Anatomie abgeleitet, nicht per Schieberegler festgelegt.
- Krafttraining baut Muskeln auf; Körperzusammensetzung verändert sich über Ernährung, Training, Schlaf und Stress ([Gameplay-Systeme](../Gameplay-Systeme.md)).
- Individuelle Merkmale wie Größen, Formen und Gebrauchsspuren sind einzigartig. Der Entwickler wünscht eine hohe Vielfalt auch bei intimen Merkmalen; Details der Parameter gehören in den Charaktereditor ([Modul 07](07-Charakteranpassung-Tattoos-Koerpermodifikation.md)).
- Alles, was nicht eigens anders festgelegt ist, soll sich **so realistisch und logisch wie in der echten Welt** verhalten (Entwickler, 26.09.; gilt für das ganze Spiel).
- Der Entwickler will ‚90 Prozent Realismus‘ statt Perfektion (12.09.): zu hoher Aufwand für das letzte Zehntel.

---

## 3. Weichgewebe und Sekundärbewegung

📋 Die Weichgewebe-Nachbewegung ergibt sich **pro Körperteil** aus dessen Anatomie (Masse, Steifigkeit, Dämpfung, Muskelzustand, Haltung, Bewegungsrichtung und -frequenz, Kontakt), nicht aus einer pauschalen „Wackel“-Animation. Sie soll **realistisch, nicht übertrieben** sein.

- Dieselbe Simulation gilt im Hauptspiel (zum Beispiel beim Laufen; ein Sport-BH dämpft die Bewegung) und in der Intimität, nur im anderen Kontext. Der Entwickler: ‚Jiggle-Physik, aber realistisch‘.
- Der Entwickler verlangte ausdrücklich, dass sich in intimen Szenen Muskeln, Haut und Gewebe beider Beteiligter erkennbar bewegen. Ob eine Region überhaupt sichtbar sein muss, ist ein Anzeigeprofil ([Modul 04](04-Steuerung-Darstellung-Kamera-Audio.md#2-darstellungsprofile-und-erzählung)), nicht eine Frage der Simulation.
- **Variation:** Wiederholungen sehen nie identisch aus; Mikrobewegungen und leichte Asymmetrien sind gewollt.
- Kleidung, Haar und Schmuck reagieren auf dieselbe Physik.

---

## 4. Biomechanische Grenzen

📋 Grenzen entstehen aus echter Biomechanik: Gelenkwinkel, Reichweite, Proportionen, Gewebekompression, Flexibilität, Haltung, Verletzungen, Belastbarkeit. Unangenehme Belastung (Überdehnung, Krampf, ungünstiger Winkel, zu hohe Kontaktkraft) löst natürliche Reaktionen aus. Dieses System ist **nicht Adult-spezifisch** und gilt auch für Kampfsport, Klettern und Tragen.

- Sehr bewegliche Figuren können Haltungen einnehmen, die andere nicht können; Unvorbereitetes führt eher zu Schmerz und Verletzung. Übung und Gewöhnung verändern Gewebe und Belastbarkeit über Zeit (Entwickler, 18.09.).
- Bei zu starker Belastung können Knochen, Gelenke oder Gewebe brechen (Entwickler: wie im Bodenkampf).
- Die Größe oder Art von Gegenständen begrenzt das Spiel nicht künstlich; das Risiko ergibt sich aus Körperzustand, Anatomie, Erfahrung und Belastung, nicht aus einem einzelnen Größenwert (Entwickler: keine Obergrenzen außer physikalischen).
- ❓ Welche extremen Inhalte im Paket tatsächlich ausgeliefert werden, ist eine **Inhalts- und Rechtsentscheidung**, die noch aussteht ([Offene Fragen](11-Offene-Fragen-und-Konflikte.md)). Die Simulationsebene soll sie nicht durch harte Obergrenzen verhindern, sondern die Folgen korrekt abbilden.

---

## 5. Zustände: Erregung, Empfindung, Ermüdung, Erholung

- 📋 **Erregung** ist ein kontinuierlicher physiologischer Zustand, kein sichtbarer Balken. Sie bleibt normalerweise verborgen und zeigt sich über Körpersprache, Atmung und Stimme. Sie kann auch wieder sinken und gleichzeitig mit Stress oder Nervosität bestehen. Der Entwickler nannte Zustände, die sich bei manchen Figuren sehr schnell, bei anderen langsam einstellen.
- 📋 **Ein Höhepunkt ist nicht garantiert**, nicht für jede Person gleich schnell erreichbar und variiert nach Situation. Danach folgt ein individueller Erholungszustand; die Sitzung endet dadurch nicht automatisch und kann auch ohne enden. Ein Höhepunkt kann auch durch Schmerz oder auf andere Weise entstehen.
- 📋 **Sinneswahrnehmung** (lokale Empfindlichkeitskurven, Schmerz und Lust gleichzeitig möglich, unterschiedliche Schmerztoleranz, Sensibilisierung, Abstumpfung, Überstimulation) und **Ermüdung** (nach Muskelgruppen, Krämpfe, Haltungsermüdung, Muskelkater, Schwellung, Blutergüsse mit zeitlichem Verlauf, oberflächliche Hautschäden) laufen in einer gemeinsamen **Ergebnisschicht** über dem allgemeinen Körpersystem. Auch Kampf, Sport und Medizin greifen darauf zu.
- 📋 **Erholung** addiert sich aus allen aktuellen Belastungen. Stärkere Nachwirkungen beeinflussen später normales Gameplay (Sitzen, Gehen, Rennen, Kämpfen, Schlaf, Arbeit). Kleine Beschwerden können selbst behandelt werden, Eskalation führt zur ärztlichen Behandlung. Ein Entwickler-Inspector kann Erholungszeiten schätzen.
- 📋 **Gedächtnis des Körpers:** Wiederholt belastete Bereiche reagieren später anders; wiederholte Belastung auf eine noch nicht verheilte Verletzung erhöht das Risiko (Runde 15F, bestätigt). Es gibt nie einen „frischen“ Körper pro Sitzung.

---

## 6. Verletzung und Risiko

- 📋 **Adult Health ist nie ein separates Minigame** (Frage 150, bestätigt). Verletzung, Schmerz, Blutung, Heilung, Narben, Medikamente, Erholung und Physiologie kommen vollständig aus dem normalen Charakter- und Gesundheitssystem. Es gibt keinen magischen Sonderzustand für den Erwachsenen-Bereich.
- 📋 **Risiko** ist in sechs Arten getrennt (Verletzung, Entdeckung, sozial, rechtlich, gesundheitlich, Beziehung).
- 📋 Eine Verletzung kann länger anhalten (Wochen) und die Handlungsfähigkeit, das Sitzen, den Schlaf und Folgesitzungen beeinflussen. Ärztliche Versorgung ist möglich; ein Krankenhausaufenthalt ist bei schweren Verletzungen realistisch.
- 📋 Wie viel eine Figur riskieren will, ist eine **Persönlichkeits- und Präferenzfrage** (manche Figuren nehmen Schmerz oder Verletzungsrisiko bewusst in Kauf). Es gilt: Das Spiel setzt keine feste Obergrenze, die **Figur entscheidet**, und alle Folgen laufen über das Gesundheitssystem (Entwickler, 15.09.). Das hebt die Regeln zu Zustimmung und Stopp nicht auf ([Modul 01](01-Grundregeln-Alter-Zustimmung.md)). Eine ‚optimale Ausführungsmechanik‘ für gefährliche Handlungen wird nicht gebaut; es werden Zustand und Folgen simuliert.
- 📋 Bei Bewusstlosigkeit endet die Interaktion ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#5-entscheidungsfähigkeit-alkohol-und-drogen)). Der Entwickler hielt fest, dass der Spieler den Zustand meist selbst bemerkt und aufhört; das System erzwingt den Abbruch nach der Erkennung dennoch.
- 🧭 Heilung, Risikoraten, Zeiträume, Komplikationen: ❓ **Zahlenwerte offen.** Sie werden nicht aus dieser Dokumentation abgeleitet.

---

## 7. Flüssigkeiten, Hygiene, Kleidung, Möbel

- 📋 **Flüssigkeiten haben getrennte Eigenschaften:** Speichel ≠ Schweiß ≠ Tränen ≠ Blut ≠ Erbrochenes ≠ weitere. Menge entsteht aus Körperzustand, Hydration, vorhandenem Inhalt und vorherigen Ereignissen, nicht aus einer festen Effektmenge. Das System nutzt dieselbe Material-/Physik-Infrastruktur des Spiels, keine vorgefertigten Aufkleber, soweit die Leistung es erlaubt. Aufwendige Flüssigkeitsdetails sind **Spät-Features** hinter einem Funktionsschalter (Entwickler, 23.09., zu diesem Themenblock: ‚erst ganz spät implementieren‘).
- 📋 **Hygiene** ist ein allgemeines Charaktersystem (Duschen, Schweiß, Geruch, saubere Kleidung). NPCs reagieren je nach Persönlichkeit unterschiedlich empfindlich.
- 📋 **Kleidung** nimmt Flüssigkeiten auf und bekommt einen Sauberkeitszustand; Bettwäsche, Handtücher und Möbel ebenfalls, ohne daraus ein Haushalts-Minispiel zu machen.
- 📋 Möbel können bei grober Belastung beschädigt werden; das Material zählt.

---

## 8. Schwangerschaft, Verhütung, Infektionen

📋 (Runde 15F, vom Entwickler weitgehend bestätigt; er lehnte einzelne Detailpunkte als unnötig ab: „es ist eh schon genug“, 15.09.)

- **Verhütung** ist Teil der Gesundheits- und Inventarsimulation (Kondome, hormonelle Methoden, weitere) mit unterschiedlicher Wirksamkeit und möglichen Anwendungsfehlern; NPCs haben eine eigene bevorzugte Methode und können mit dem Partner darüber sprechen. Der Entwickler: Hormonelle Verhütung kann die Persönlichkeit verändern.
- **Fruchtbarkeit** variiert individuell; ein **Zyklus** wird abstrahiert simuliert (kein Kalender-Mikromanagement) und beeinflusst Fruchtbarkeit, Körper und Stimmung. Schwangerschaft entsteht nur, wenn Biologie und Situation es ermöglichen.
- **Schwangerschaft** ist ein langfristiger Weltzustand über Monate (Körper, Gewicht, Bewegung, Kleidung, Termine, Arbeit, Beziehungen), mit Ärzten und Kliniken, Tests als echte Gegenstände. Wenn der Spieler es will, kann eine Geburt (Junge oder Mädchen) stattfinden; Familien über Generationen sind niedrigere Priorität.
- **Infektionen (STI):** ⛔ **Nicht beschlossen.** Der Assistent schlug in Runde 15F (Fragen 137 bis 139) ein STI-System vor; der Entwickler lehnte ab (‚nein, würde ich jetzt nicht machen, das brauchen wir nicht, ist eh schon genug‘, 16.09., 35#542). Die Basisversion enthält **kein detailliertes STI-System**. Die Architektur soll eine spätere Erweiterung über das allgemeine Gesundheitssystem nicht ausschließen (35#543). Zu Verhütung, Schwangerschaft und Zyklus gilt weiterhin die obige Planung. Register: [E-GE-004](14-Entscheidungsregister.md#7-ge-gesundheit-verletzung-fortpflanzung).
- ❓ Raten, Zeiträume und Zahlenwerte sind offen.

---

## 9. Reaktionen und Gesicht

- 📋 **Reaktionsgraph:** Die sichtbare Reaktion entsteht aus *Handlung + Körperzustand + Persönlichkeit + Beziehung + Präferenzen + aktueller Absicht + Zustimmungs-/Grenzzustand + Schmerz/Stress + Umgebung*, nicht aus einer fertigen Animation. Mehrere Reaktionen werden zusammengesetzt (Stöhnen, Schreien, Schmerzreaktion, Gegenbewegung, Begeisterung, Überforderung).
- 📋 **Körperliche Reaktionszustände** (Würgereiz, Retching, Übelkeit, Erbrechen, Rückfluss, Husten, Atemnot, Atemunterbrechung) sind eigene Zustände und dürfen gleichzeitig auftreten. Schmerzlautstärke hängt nicht nur davon ab, ob jemand laut oder leise ist, sondern von Ursache, Intensität und Zustand. Hautreaktionen (Rötung, Druck, Reibung, Schwellung, Verletzung) und Muskelkrampf/Erschöpfung laufen über den allgemeinen Körperzustand.
- 📋 Mimik aus den bestehenden Emotionssystemen; Blickkontakt wird simuliert; Atem- und Stimmaudio prozedural ([Modul 04](04-Steuerung-Darstellung-Kamera-Audio.md#7-audio-und-stimmen)).
- 📋 Herzschlag ist beim eigenen Charakter in extremen Stresssituationen als Audio hörbar (Entwickler, 12.09.); Ob und wie man den Herzschlag anderer hören kann, ist nicht festgelegt (❓).

---

## 10. Ableitbare Tests

Ein Vorschlag (🧭), engine-unabhängig:

- Zustands-Übergänge von Erregung/Erholung sind deterministisch bei gleichem Seed und Eingabe.
- Wiederholte Belastung eines Bereichs mit Verletzungszustand erhöht das Risiko in jedem Fall gegenüber dem unverletzten Bereich.
- Verlust des Bewusstseins setzt `Reaktionsfähigkeit` herab und beendet jede aktive Sitzung.
- Flüssigkeiten sind getrennte Material-IDs; keine Umrechnung zwischen ihnen.
- Speichern und Laden stellt den Körpergedächtniszustand identisch wieder her.
