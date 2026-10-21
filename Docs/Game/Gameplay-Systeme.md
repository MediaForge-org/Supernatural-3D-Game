# Gameplay-Systeme: Planungsstand im Detail

> [!IMPORTANT]
> **Fast alles in diesem Dokument ist geplant und nicht implementiert.** Implementiert und getestet sind nur die Grundlagen, die in der README im Kapitel „Aktueller Implementierungsstand“ stehen: eine kleine universelle Interaktion mit Testtür, der persistente Weltzustand, Speicher-Slots und die Datengrundlage für das Aussehen von Figuren. Jede Aussage trägt eine Statuskennzeichnung (siehe [Abschnitt 1](#1-statuskennzeichnung)). Zahlenwerte für Spielbalance, Fahrverhalten, KI und Simulation sind **nicht** festgelegt.

Dieses Dokument ist eigenständig lesbar. Es vertieft die [README](../../README.md) und gehört zu den Spiel-Dokumenten in [`Docs/Game/`](.), neben [Spielvision](Spielvision.md), [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md), [Adult-System](Adult-System.md) und [Entscheidungsstatus](Entscheidungsstatus.md).

**Inhalt**

1. [Statuskennzeichnung](#1-statuskennzeichnung)
2. [Erkundung, Bewegung zu Fuß und Kamera](#2-erkundung-bewegung-zu-fuß-und-kamera)
3. [Interaktion](#3-interaktion)
4. [Verletzungen und Medizin](#4-verletzungen-und-medizin)
5. [Waffen, Ballistik und Nahkampf](#5-waffen-ballistik-und-nahkampf)
6. [Fahrzeuge und Fortbewegung](#6-fahrzeuge-und-fortbewegung)
7. [Tarnung, Wahrnehmung und Horror-KI](#7-tarnung-wahrnehmung-und-horror-ki)
8. [NPCs, Gedächtnis und Beziehungen](#8-npcs-gedächtnis-und-beziehungen)
9. [Körper, Fortschritt und Anpassung](#9-körper-fortschritt-und-anpassung)
10. [Recht, Polizei und Beweise](#10-recht-polizei-und-beweise)
11. [Smartphone, soziale Medien und Weltereignisse](#11-smartphone-soziale-medien-und-weltereignisse)
12. [Charaktereditor und Menschen](#12-charaktereditor-und-menschen)
13. [Wirtschaft, Besitz und Geld](#13-wirtschaft-besitz-und-geld)
14. [Grafik, Animation, Audio und Plattformen](#14-grafik-animation-audio-und-plattformen)
15. [Modding](#15-modding)
16. [Persistenz und Weltzustand](#16-persistenz-und-weltzustand)
17. [Technische Vision und Arbeitsweise](#17-technische-vision-und-arbeitsweise)
18. [Offene Punkte](#18-offene-punkte)

---

## 1. Statuskennzeichnung

| Kennzeichnung | Bedeutung |
|---|---|
| ✅ | Implementiert und getestet |
| 📋 | **Bestätigte Planung:** vom Entwickler festgelegte Richtung, nicht implementiert |
| 🧭 | **Vorläufig:** Arbeitsstand, Beispiel oder Vorschlag, nicht festgelegt |
| 🔭 | Langfristige Vision ohne Spezifikation |
| ❓ | Offen: bewusst nicht entschieden |
| 🕰️ | Überholt: frühere Planung, die später ersetzt wurde |

Die meisten Systementscheidungen stammen aus dem **Planungsstand vom 31. August 2026**. Sie sind seither nicht widerrufen worden. Neuere Dokumente (Oktober 2026) haben bei Konflikten Vorrang; solche Fälle sind gekennzeichnet.

---

## 2. Erkundung, Bewegung zu Fuß und Kamera

- ✅ **Prototyp:** Third-Person-Bewegung, Kamera und Springen stammen aus der Unreal-Vorlage. Laufen, Klettern oder Schwimmen als eigene Systeme gibt es noch nicht.
- 📋 **Freie Erkundung:** Die Welt soll früh frei bereisbar sein. Regionen sind vor ihrem Handlungsbogen besuchbar; echte Sperren gibt es nur mit logischem Weltgrund.
- 📋 **Kamera:** Third Person ist wahrscheinlich der Standard. **Vollständige First-Person-Unterstützung** ist ernsthaft geplant, besonders für Horror und Ermittlung. Fotomodus mit freier Kamera, Tiefenschärfe, Belichtung, Linse und Sichtfeld, Filtern, Posen und ausblendbarer Oberfläche.
- 📋 **Springen und Wasser:** normaler Sprung, Kopfsprung, weiter Sprung, kontrollierter Sprung mit begrenzter Körperkorrektur. **Keine magische Luftsteuerung**. Höhe, Technik und Wasser beeinflussen das Verletzungsrisiko.
- 📋 **Lose physische Gegenstände:** Hüte, Brillen, Taschenlampen, Smartphones, Taschen, Waffen und Gürtelgegenstände können physisch verloren und aufgehoben werden, mit Bereinigung und Persistenz.
- 📋 **Realismus-Grundregel:** Wenn etwas nicht eigens anders festgelegt ist, soll es sich so realistisch und logisch wie in der echten Welt verhalten.

---

## 3. Interaktion

### 3.1 Was schon existiert (✅)

Eine **kleine, universelle Interaktion** mit einer einzigen Aktion `Use`, einem Objektart-Register (Objektart → Handler), stabilen Objekt-IDs und einer Testtür als Platzhalter (öffnen, schließen, verriegelt → Ablehnung mit Rückmeldungs-ID). E-Taste am Third-Person-Charakter über Enhanced Input, Zielerfassung per Sphere-Trace, Sichtbarkeit des Zustands nach dem Laden. Die Testtür ist **kein Entwurf für Türen im Spiel**. Technische Details stehen in der README und in den Entscheidungsnotizen im Repository.

### 3.2 Geplante Tiefe (📋 / ❓)

- Weitere Verben (Untersuchen, Nehmen, Sprechen und andere), ein Schlossmodell (Schlüssel, Dietrich, Verklemmen, Aufbrechen, Geräusche), Eingabe- und Hinweis-Oberfläche, Controller-Belegung: alle ❓ offen und von späteren Systemen zu bestimmen.
- 📋 Türen haben **akustische Ergebnisse** je nach Bedienung und Zustand (alte Türen knarren, vorsichtiges Öffnen kann dennoch hörbar bleiben). Türen, Dunkelheit, Strom und KI tragen zum Horror bei.
- 📋 **Allgemeines Interaktions-Framework** (Kern plus Module für Soziales, Zuneigung, Hilfe, Tragen, Medizin, Ruhe, Romantik, Intimität; gemeinsame Infrastruktur: Teilnehmer, Sitzungen, stabile IDs, Anfragen, Reservierungen, Annäherung, Ausrichtung, Motion Warping, IK, Synchronisation, Übergänge, Netzwerk, Persistenz). Kein Monolith-System, kein „Gott-System“. Siehe [Adult-System](Adult/03-Interaktion-Taxonomie-Ablauf.md#1-allgemeines-interaktions-framework).
- 📋 Ein großer Teil weiterer Interaktion folgt aus Fahrzeugen, Medizin, Tragen, Ermittlung und Reparatur; sie sollen sich sinnvoll in dasselbe Interaktionsmodell einfügen.

---

## 4. Verletzungen und Medizin

📋 Ein tiefes, aber spielrelevantes Verletzungssystem. Es ist ein **Kernpfeiler** und nicht optional.

**Grundsätze**

- Kein Energy-Drink- oder Sofortheilungs-Modell. **Spieler und menschliche NPCs teilen dieselbe grundlegende Anatomie und dieselben Traumaregeln; keine Plot-Rüstung.** Plausibel tödliche Treffer können den Spieler töten.
- Die Anatomie ist detailliert, aber spielrelevant: etwa **25+ Trefferregionen**, Weichgewebe, relevante Knochen, Gelenke, kritische Organe, große Arterien und kritische Gefäßzonen, vereinfachte Nerven und Sehnen.
- **Projektile** folgen einem Weg durch Kleidung, Materialien, Gewebe, Knochen, Organe und Gefäße; sie können austreten oder stecken bleiben (ein steckengebliebenes Projektil muss nicht zwingend entfernt werden).
- **Traumaarten:** Schuss/Splitter, Schnitt, Stich, stumpf/Quetschung, Bruch und offener Bruch, innere Verletzung, Verbrennung/Kälte/Elektrizität, Bisse/Krallen/Reißen, Explosion/Sturz/Fahrzeug/Trümmer sowie **übernatürliche Modifikatoren** (die das normale System erweitern, etwa Heilungshemmung oder Fluch, statt ein getrenntes Gesundheitssystem zu bilden).
- **Systemzustand:** Blutvolumen, Blutungsrate, Schmerz, Schock, Bewusstsein, Atmung, Adrenalin, Beweglichkeit, Gliedmaßenfunktion und Infektionsrisiko. Schwere Arterienverletzung kann ohne Behandlung schnell zum Verbluten führen. Verletzungen **summieren sich** und können Bewegung und Kampf unmöglich machen oder töten.
- **Wahrnehmung der Verletzung:** Blutverlust, Schmerz, Schock und Adrenalin müssen durch Animation, Audio, Atmung, Sicht, Zittern, Bewegung, Zielen, Stimme, Koordination und Handlungsfähigkeit spürbar sein. **Schmerz ist vom physischen Schaden getrennt.** Adrenalin kann wahrgenommenen Schmerz unterdrücken; Symptome werden später deutlicher. Nerven- und Sehnenschäden beeinflussen Kontrolle und Griff. Verletzte Gliedmaßen wirken sich auf Hinken, Sprinten, Waffeneinsatz, Nachladen, Gleichgewicht, Atmung und Ausdauer aus. Offene Brüche mit sichtbarem Knochen sind möglich, auch beim Spieler.
- **Feuer** kann sich ausbreiten; ein früher kleiner Kleidungsbrand lässt sich ersticken.
- **Verzögerte oder innere Verletzungen** können bis zur ärztlichen Diagnose unsicher bleiben.
- **Behandlung auf drei Ebenen:** Feldbehandlung für leichte Verletzungen und Stabilisierung; ein medizinisch geschulter Jäger oder Arzt für manche mittelschweren Fälle; schwere innere Verletzungen, komplizierte Schüsse, schwere offene Brüche, schwere Verbrennungen, kritische Infektionen und massiven Blutverlust brauchen Krankenhaus. Schmerzmittel wirken verzögert und sind Symptomlinderung, keine magische Heilung. Erholungsstufen: akut → stabilisiert → heilend → erholend → erholt. Erneute Verletzung einer noch nicht ausgeheilten Region erreicht schwere Zustände schneller.
- **Animation:** Komplexe Verletzungsanimation (Schichten, IK, prozedurale Anpassung) ist vor der Veröffentlichung nötig, auch wenn frühe Builds einfacher sind.

**Darstellung**

- 📋 **Gore** ist gewollt und ursachenabhängig (Pistole ≠ Schrotflinte aus der Nähe ≠ Messer ≠ Axt ≠ Kralle). Bei **erwachsenen Menschen** liegt die Zielschwere mindestens auf dem Niveau eines bekannten Western-Spiels, aber enger an Verletzungs- und Medizinzustand, Kleidungsschaden und tatsächliche Schadensbedingungen gebunden; plausible Ergebnisse umfassen Ein- und Austrittswunden, Schnitte, Verbrennungen, Brüche, schweren Gewebeschaden, Verlust von Fingern, Händen und Gliedmaßen, Enthauptung unter sehr extremen und physikalisch passenden Bedingungen und Explosionstrauma. Ein späterer Planungsblock nennt für erwachsene Menschen und Monster teils eine Schwere „deutlich darüber“. Die endgültige Schwelle ist ❓ von Plattform-, Alters- und Rechtsprüfung abhängig.
- 📋 **Minderjährige** können in der Welt existieren und haben keinen magischen Unverwundbarkeits-Schutz, aber die offizielle Darstellung schließt extreme Verstümmelung, Zerlegung oder Enthauptung aus, und es gibt weder Belohnungen noch Herausforderungen, die auf ihre Verletzung zielen. Endgültige Grenzen brauchen Plattform-, Bewertungs- und Rechtsprüfung.
- 📋 Nutzer-Einstellungen für Gore: voll / reduziert / minimal. Einstellbare **Kill-Cams** für seltene, hochwertige Tötungen (filmische Projektilverfolgung, optional eine anatomische Röntgen-ähnliche Ansicht mit eigener visueller Sprache, die an die tatsächliche Verletzungssimulation gebunden ist; Häufigkeit konfigurierbar, standardmäßig selten).
- Siehe [Spielvision](Spielvision.md#14-schwierigkeit-scheitern-und-tod) für Tod und Scheitern (❓ offen).

---

## 5. Waffen, Ballistik und Nahkampf

### 5.1 Waffen

📋

- **Kein Kugelschwamm.** Die richtige Waffe, Munition und Kreaturschwäche sind das Kernstück. Das Arsenal enthält gewöhnliche Waffen und originale, seltene oder legendäre Jägerwaffen mit IP-sicherem Hintergrund.
- **Ausrüstung:** Der Spieler trägt mehrere Waffen, aber nicht alles; **die Vorbereitung vor der Jagd zählt**. Kofferraum und Unterschlupf sind größere Lager. Der Zugriff hängt vom Trageort ab (Holster oder Gürtel schnell, Rucksack langsamer, Kofferraum nicht im Kampf).
- **Munition:** Magazine behalten ihre tatsächliche Schusszahl; **kein globaler Munitionspool**. Patronenlager und Magazin sind getrennte Zustände. Taktisches Nachladen behält ein angebrochenes Magazin; Notnachladen vor allem bei leerem Magazin und wirft das leere Magazin ab. Röhrenmagazine werden Patrone für Patrone geladen und das Nachladen ist an jeder Patrone unterbrechbar. Der Nachladezustand ist persistent und unterbrechbar.
- **Handhabung:** Masse, Schwerpunkt, Länge, Rückstoß, tiefe Bereitschaftsstellung, Punktschießen, Visieren, Raumbeschränkungen, Ermüdung, Atmung, Verletzungseinfluss, schwache Hand. Lange Haltezeiten ermüden moderat. Mit verletztem Schussarm kann eine Pistole mit schwacher Hand geführt werden (reduzierte Kontrolle, langsamere Handhabung). Viele Körperhaltungen werden unterstützt.
- **Störungen** sind selten, aber wahrscheinlicher bei schlechter Wartung, Verschmutzung, Schaden oder problematischer Munition; die Beseitigung bleibt bedienungsfreundlich (**eine** kontextabhängige Eingabe löst die richtige realistische Handlung aus). Die Wartung ist moderat und realistisch, nicht ständig.
- **Anzeige:** Die Munitionsanzeige ist binär (an oder aus); ohne sie zählt man Schüsse und prüft Waffe und Magazin körperlich. Physische Waffen- und Magazinprüfung existiert; normale Modi dürfen auch eine Anzeige nutzen.
- **Rückstoß** ist physikalisch; Training verbessert die Kontrolle, nicht den Impuls der Waffe. **Combat Focus** ist die bevorzugte Alternative zu einer Zeitlupen-Fähigkeit: keine Zeitverlangsamung, kein Wandblick, nur trainierte Reaktion und Kontrolle mit fokussierter Präsentation (Aktivierung, Dauer, Erholung ❓).

### 5.2 Ballistik

📋

- Physikalische Prinzipien mit leistungsbewusster Näherung; bei Jagd- und Präzisionsgewehren werden Flugzeit, Fall, Luftwiderstand, Geschwindigkeitsverlust und Restenergie aus plausiblen Formeln berechnet. **Keine künstliche Maximalreichweite**, bei der Geschosse verschwinden.
- Die **Verletzung** beruht auf dem Restzustand des Projektils, der Durchdringung und dem anatomischen Weg, **nicht** auf einer einfachen linearen Abstands-Schadenskurve.
- Projektildefinition mit Mündungsgeschwindigkeit, Masse, ballistischem Koeffizienten bzw. Luftwiderstandsverhalten und Aufbau. Durchdringung hängt von Projektil, Energie, Aufbau, Winkel, Material und Dicke ab. Dünne Deckung kann durchschlagen werden, massive Deckung stoppt Geschosse und zeigt kosmetische Einschläge. Mehrere Materialschichten verringern den Zustand schrittweise. Fahrzeuge haben unterschiedliche ballistische Strukturen (Türblech ≠ Motorblock); Glas kann brechen und ablenken. Gefährliche Abpraller sind selten; im härtesten Modus standardmäßig aktiv.
- Wind wirkt mit wachsender Flugzeit; bewegte Ziele brauchen echtes Vorhalten; Höhe und Luftdichte können modelliert werden; Bergauf-/Bergab-Winkel, Visierhöhe über der Laufachse und Neigung („Cant“) bei Fernschüssen; Transschall und Spin-Drift können angenähert werden, Coriolis nicht.
- Anbauteile verändern relevante Eigenschaften physikalisch (Optik, Schalldämpfer, Lauflänge, Zweibein, Magazinmasse). Der Schalldämpfer macht Überschallschüsse nicht lautlos; Unterschall-Munition hat eigene Kompromisse.
- Typische Kämpfe finden meist unter etwa 100 m statt, aber seltene Schüsse von 500–1000 m und mehr müssen funktionieren. Schießstände, Unterschlupf-Training und Trainingsumgebungen sind vorgesehen, ebenso optionale Schießwettbewerbe. Im härtesten Modus kann ein tatsächliches Entfernungsmessgerät nötig sein.
- Nullpunkteinstellung ist echte Optik-/Lauf-Geometrie, nicht veränderte Geschossphysik.

### 5.3 Nahkampf und Ringen

📋

- Nahkampf ist **tief und häufig nützlich**, kein flacher Notfallknopf und kein UFC-Klon. Hybridstil aus Schlagen, Treten, Fußarbeit, Distanzmanagement, Finten, Block/Parade/Ausweichen, Clinch, Ringen, Takedowns, Griffkampf, bodennahe Kontrolle, Scrambles und Aufgabegriffen.
- Leichte und schwere Angriffe; **keine unterbrechungslosen Kombo-Ketten** (Fehlschläge, Blocks, Konter verändern den Zustand). **Keine harte Zielaufschaltung** standardmäßig (optional weiche Hilfe). Blocken ist physisch, Parade nur wenn plausibel, kein Soulslike-Rollspam.
- Bodenkampf mit stabilen Positionen und umkämpften Übergängen; Aufgabegriffe können Abklopfen, Bewusstlosigkeit oder echte Verletzung auslösen. Im tödlichen Straßenkampf kann anhaltendes Würgen nach der Bewusstlosigkeit töten. Mehrere Gegner machen langes Ringen gefährlich.
- **Mehrere Gegner** greifen nicht höflich einzeln an (gleichzeitiger Druck, Flankieren, Greifen, Ausgänge abschneiden), unter Beachtung von Körperkollision, Raumgeometrie und gegenseitiger Behinderung, ohne permanenten Betäubungs-Lock. Improvisierte Gegenstände und die Umgebung können genutzt werden (Flasche werfen, Messer ziehen, Wände, Möbel, Türen); eine Glasflasche zerbricht nach einem harten Schlag und der Rest kann bleiben. Waffen-, Nahkampf-, Messer- und Improvisationswechsel sollen fließend sein; extrem enge Haltung der Pistole ist möglich; Gegner können die Waffe greifen, und die Mündungsrichtung bleibt im Handgemenge physikalisch bedeutsam.
- **Menschliche Gegner** können fliehen, erstarren oder sich ergeben, wenn sie genug eingeschüchtert sind; Furcht und Moral bleiben intern und zeigen sich im Verhalten. Finisher nur bei tatsächlicher Handlungsunfähigkeit und passendem Kontext.
- **Fight Club:** gleiches physisches Kampfsystem; ein Ehrenkodex (kein absichtliches Töten oder vermeidbare schwere Verletzung; Aufgabegriffe bei Abklopfen oder Bewusstlosigkeit lösen) mit ernsten sozialen, rechtlichen und Spielfolgen bei Verstoß.
- **Individuelle Stile:** Jeder Kämpfer und Jäger hat einen eigenen Stil und eigene Vorlieben statt starrer Klassen.
- **Steuerung:** kontextabhängig und lernbar. Die Simulation darf komplex sein, der Spieler soll keine riesigen Kombotabellen auswendig lernen. Position, Distanz, Griff, Richtung und Kontext bestimmen die sinnvollen Aktionen.
- Ein **Lernsystem für Kampfbewegungen** (einfache Bewegungen anfangs, anspruchsvolle erst durch Training und Lehrer in Studios; kein klassisches Upgrade-System) wurde im Planungsgespräch gewünscht und gehört zu einem späteren allgemeinen Kampf-Block (🧭).

---

## 6. Fahrzeuge und Fortbewegung

### 6.1 Rolle

📋 Fahren ist ein **Kernsystem** und eine frühe Ambition für den Vertical Slice. Das Fahrzeug ist eine mobile Ausrüstungsbasis (Kofferraum als Lager). Lange Fahrten sind ein Charakterkanal: Gespräche, Radio, Telefon, Schlaf, Streit, Fallbesprechung, spontane Stopps, Schweigen.

### 6.2 Phasenvertrag (Oktober 2026)

| System | Frühe Version | Langzeitziel |
|---|---|---|
| Fahrphysik | sinnvolles Schaltgetriebe mit funktionierender Kupplung | nahezu vollständige Fahrdynamik-Simulation |
| Motorstart | einfache Startinteraktionen | fortgeschrittene Startvorgänge |
| Allgemeine Fahrzeuginteraktion | einfacher Zugang und Benutzung, automatisches Sitzen | tiefere Interaktion, Sitzen bleibt automatisch |
| Reparatur, Tuning, Werkstätten | einfache Reparatur und Abstimmung | nahezu vollständige Instandhaltungs- und Werkstattsimulation |
| Getriebewahl | Handschaltung verfügbar | **Handschalt-Variante für jedes Fahrzeug** plus passende Automatik; optionale H-Schaltung als Hardware |

Die Trennung zwischen früh und lang ist vom Entwickler bestätigt. Keine Veröffentlichungstermine, Motormodelle oder Wiedergabetreue-Maßstäbe sind beschlossen.

### 6.3 Schaltung und Kupplung

- 📋 **Manuelle Fahrzeuge** simulieren Kupplung, Schaltvorgang, Drehzahl, Motorbremse, Kupplungsschlupf und Abwürgen. **Freie Gangwahl** ist erlaubt (auch 5→2); Folgen sind Drehzahl, Motorbremse, Instabilität und je nach Realismus-Einstellung Verschleiß und Schaden. Fehlschaltungen und Missbrauch können Kupplung, Getriebe, Antriebsstrang und Motor beschädigen.
- 📋 **Automatik, Handschaltung mit Auto-Kupplung und Handschaltung mit digitaler Kupplung** sind einfachere Modi. Die digitale Kupplung kann eine progressive simulierte Kurve nutzen; schlechtes Gas oder Timing kann dennoch abwürgen. Lenkrad, Pedale und H-Schalter sind eine Enthusiasten-Option und nicht der Hauptfokus.
- 📋 **Controller:** Das frühere Konzept (Xbox-artig): Gas rechter Trigger, Bremse linker Trigger, A Hochschalten, X Herunterschalten, B Handbremse, Rückwärtsgang durch fortgesetztes Herunterschalten, Y öffnen. Fortgeschrittener manueller Modus: LB als Kupplungs-Modifikator, wobei der linke Trigger bei gehaltenem LB zur analogen Kupplungsachse wird. Alles frei belegbar.
- 📋 **Vom Entwickler am 15. Oktober 2026 angegebene DualSense-Belegung:** R2 Gas, L2 Bremse, **L1 + L2 analoge Kupplung**, linker Stick Lenkung, rechter Stick Kamera, Kreuz/X Hochschalten, Quadrat Herunterschalten; alles frei belegbar. **Technische Anmerkung (keine Kanon-Änderung):** L2 ist gleichzeitig Bremse und Teil der Kupplungsgeste. Im Prototyp ist zu klären, wie L1 den analogen L2-Wert modifiziert, wie die Bremse während der Kupplungsnutzung unterdrückt oder beibehalten wird und ob die Ergonomie das gewünschte Fahrverhalten trägt. Es wird keine andere Belegung stillschweigend untergeschoben.

### 6.4 Physik, Varianten und Pflege

- 📋 **Fahrzeugphysik** nutzt tatsächlich Antriebsstrang, Reifen, Federung, Lenkung, Differenzial, Bremsen und Sicherheitssysteme, Aerodynamik, Last und Schaden. Reifenmischung, -temperatur, -druck, -verschleiß und -nässe können simuliert werden. Supersportwagen sind nicht magisch geländetauglich; echte Gelände-Unterschiede. Realistisches Ausrutschen und realistische Höchstgeschwindigkeit; Ladung verändert die Masse; thermischer Zustand und Verschleiß von Kupplung, Motor und Getriebe.
- 📋 **Varianten:** Das Getriebe gehört zum gekauften Fahrzeug bzw. Ausstattungsniveau. Modellfamilien können Hand-, Automatik-, Doppelkupplungs-, sequenzielle, Rallye- und Homologationsvarianten haben (Modell, Ausstattung, Motor, Getriebe, Antrieb, Baujahr, Optionen).
- 📋 **Werkstätten:** Spezialisierte Reparatur-, Leistungs-, Renn-, Offroad- und Karosseriewerkstätten; ein schneller Neulack-Service ist nicht die volle Tuning-Werkstatt. Tuning mit Priorität auf Leistung statt Kosmetik. Mechanischer Verschleiß: Motor, Öl, Kühlung, Getriebe, Kupplung, Differenzial, Batterie, Bremsen, Reifen, Federung. Gebrauchtwagenmarkt mit Laufleistung, Geschichte, versteckten Mängeln und Umbauten. Realer Kraftstoffverbrauch; Benzinsorten, Diesel, E85 und Rennkraftstoffe (E85 kann bei geeigneten Motoren spürbar Leistung bringen). Elektrofahrzeuge und Laden.
- 📋 **Familiarität:** Fahrzeugvertrautheit ohne magisches Wachstum der Fahrzeugwerte.
- 📋 **Komfort und Dienste:** Fahrzeug-App und Menü (Fernsteuerung, Anliefern), Abschleppdienst, Seilwinde, Bergefahrzeug, Crew oder anderes Fahrzeug als Bergung. Kein Eingreifen in der Luft und keine Taste zum Wiederaufrichten. Unfallenergie und Sicherheitsausstattung greifen in das Verletzungssystem. Leichen oder Tierkörper im Fahrzeug, Fahrzeuglager und Fahrzeugpersistenz sind geplant.
- 📋 **Räder und mehr:** Fahrräder, E-Scooter, Motorräder, Quads, Schneemobile, Boote (an Marinas), kleine Flugzeuge (spielerfliegbar), Hubschrauber (besitzbar, meist pilotiert), Verkehrsflugzeuge zum Reisen. Straßenfahren relativ realistisch, Motorsport deutlich simulationsintensiver.

### 6.5 Bahn und Rennen

Siehe [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#112-eisenbahn) für Bahnnetz und Bahnbetrieb. 📋 **Rennen:** Motorsport über Klassen, Homologation und Balance, damit ein teures Auto nicht automatisch Rennsiege kauft; kein Pay-to-Win; mindestens fünf Rennstrecken.

### 6.6 Offen

❓ Realistisches Schadensmodell (Karosserieverformung, Antriebs- und Reifenfehler, Reparaturkosten, dauerhafter mechanischer Verschleiß – die Wahl wurde nicht getroffen); Motordaten, Übersetzungen, Algorithmus für den Kupplungs-Greifpunkt, Verkehrsmodell, Oberfläche, Fahrhilfen und Mehrspieler-Synchronisation; Starter-Fahrzeugliste, Modelljahre, Marken (fiktive Marken sind das Ziel), Tuning- und Verschleißtiefe, Kraftstoff-/Ladezeiten, Familiaritätsgrenzen, Rennklassen, Abschlepp-Bedienung.

---

## 7. Tarnung, Wahrnehmung und Horror-KI

📋

- **Wahrnehmung mit Unsicherheit** statt magischer Spielerkoordinaten. Das Wissen der KI unterscheidet bestätigtes Wissen, starken Verdacht, schwachen Verdacht und Unbekanntes. Adaptive Taktiken nutzen nur beobachtete oder plausibel gefolgerte Fakten; **keine globale Anti-Spieler-Telemetrie**. Zustände: ahnungslos, misstrauisch, untersuchend, alarmiert, bestätigtes Ziel, suchend. Die Sicherheit der letzten bekannten Position **zerfällt** mit der Zeit; Gegner verlieren Genauigkeit, aber nicht das Wissen, dass eine bestätigte Bedrohung existiert. Suchverhalten reicht von sofortiger Verfolgung über lokale und erweiterte Suche und Einkesselung bis zu vorsichtiger Wachsamkeit. Die Suchdauer und -intelligenz hängt von der Schwierigkeit ab; auch im härtesten Modus sind die Gegner **niemals allwissend**.
- **Gehör:** Richtung, Entfernung und Unsicherheit aus tatsächlicher Schallausbreitung. Berücksichtigt werden Lautstärke, Dauer und Frequenz, Entfernung, Raumakustik, Wände, Türen, Materialien, Hintergrundlärm, Wetter, Böden und Hörvermögen. Unterschiedliche Geräuschkategorien werden unterschiedlich gedeutet (Schritte, Nachladen, Magazinabwurf, Tür, Glasbruch, Stimme, Schuss, Motor, geworfener Gegenstand). Schrittgeräusche hängen von Tempo, Haltung, Untergrund, Schuhwerk und Verletzung ab. Wer den Stick nur wenig auslenkt, bewegt sich kontinuierlich langsamer und leiser (kein separater Pflicht-Schleichknopf). Für Innenräume wird ein Raum-/Portalmodell bevorzugt. Hall kann die Ortung erschweren.
- **Sicht:** Richtung, Reichweite, tatsächliches Licht, Bewegung, Deckung, Silhouette, Haltung und Sichtbarkeit/Wetter. Dunkelheit verringert die Sicht, ohne Unsichtbarkeit zu gewähren. Ein im Dunkeln gehörter Spieler kann ohne visuelle Identifikation grob geortet werden. Taschenlampen sind Werkzeuge und Risiken; kreaturspezifische KI kann Licht und Strom manipulieren.
- **Intelligente Gegner** kombinieren Spuren (Geräusch, Blut, Türen, Fenster, letzte Position) zu Hypothesen, prüfen Verstecke auch ohne Gewissheit (Verstecke sind nie garantiert sicher) und erkennen wiederholte Ablenkungstricks. Menschliche Gegner teilen Informationen nur über plausible Kommunikation (Stimme, Funk, Sichtzeichen), **kein Schwarmbewusstsein**. Kreaturen kommunizieren artspezifisch. Blutspuren können von Menschen visuell verfolgt werden; Geruchsverfolgung ist kreaturspezifisch.
- **Taktikvorrat** intelligenter Monster: beobachten, auflauern, flankieren, Strom abschalten, Licht manipulieren, falsche Geräusche und Stimmen erzeugen, absichtlich schweigen, Ausgänge blockieren, Jäger isolieren, verletzte Ziele verfolgen, Begleiter angreifen, zurückziehen und neu positionieren, Nachladen und Verletzungen ausnutzen. Die Taktikwahl ist kontextabhängig und adaptiv, nicht zufällig rotierend. Absichtliches Schweigen darf nicht so häufig sein, dass es vorhersehbar wird. Falsche oder psychologische übernatürliche Geräusche unterscheiden zwischen körperlichen Weltgeräuschen und Wahrnehmung nur des Ziels.
- **Zusammenspiel:** Kampf kann zu Suche und Tarnung zurückkehren, wenn die Sichtlinie bricht und die genaue Position verloren geht. Verletzung erschwert Tarnung (Hinken, schweres Atmen, Schmerzlaute, Blutspuren).
- **Anzeigen:** Tarn-HUD ist optional (im härtesten Modus standardmäßig aus).
- **Sozial und Tarnen:** Soziale Tarnung (Licht, Schatten, Sicht, Geräusch, Untergrund, Wettermaskierung, Ablenkung, Verstecken, Kameras, Alarme, Schlösser, Fenster, Hausfriedensbruch, Verkleidung, Ausweise, Verhalten, Wiedererkennung). **Keine sofortige perfekte Verkleidung.**
- **Entwicklerwerkzeuge:** Ein KI-Inspektor zeigt Wissen, Verdacht, letzte Position, Hörschätzung, Suchphase, aktuelle Taktik und Begründung.

---

## 8. NPCs, Gedächtnis und Beziehungen

- 📋 **Nicht jeder NPC braucht die volle Simulation.** Wichtige Figuren sind vollständige Akteure mit StateTree-Verhalten und Gedächtnis; Hintergrund- und Fernbevölkerung ist leichter beziehungsweise abstrakt. Monster-KI variiert stark je Art; StateTree organisiert Zustände, eigener C++-Code übernimmt Wahrnehmung, Gedächtnis, Kampf und Sonderregeln.
- 📋 **Wichtige NPCs haben Gedächtnis.** Interne Beziehungswerte (Vertrauen, Angst, Respekt, Misstrauen; weiter Zuneigung, Anziehung, Vertrautheit, Bindung, Konflikt, jüngste Ereignisse, Beziehungsstufe) sind **nicht als Zahlen oder Balken sichtbar**. Beziehungen zeigen sich über Ton, Wortwahl, Körpersprache, Zusammenarbeit, Informationen, Zugang, Gefallen und Folgen. Wichtige Ereigniserinnerungen bleiben. Überzeugungen von NPCs können falsch sein, Zeugenaussagen sind Information und keine garantierte Wahrheit.
- 📋 **Beziehungsbildung:** Qualität der Interaktion, gemeinsame Erlebnisse, Vertrauen, Aufmerksamkeit, Konflikt, Kommunikation und Zeit. Gemeinsame Stille, Autofahrt, Fernsehen, Lagerfeuer, Recherche oder Rückfahrt formen Beziehung. Nicht jede ignorierte Nachricht ist ein Malus; wichtige wiederholte Ignorierung kann je nach Charakter reagieren. Kommunikationsstile unterscheiden sich (Schreiben, Anrufen, Sprachnachrichten, Antworttempo, Länge, Emojis, Initiative); eine Figur kann Schreiben hassen und lieber anrufen.
- 📋 **Persistenzstufen A–D** und weitere Weltsimulation siehe [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#13-gesellschaft-und-dynamische-welt).
- 📋 **Begleiter, Hunde, Crew:** siehe [Spielvision](Spielvision.md#63-begleiter-im-einsatz).
- 📋 **Romantische und sexuelle Beziehungen:** Beziehungs-Sandbox im Basisspiel; erweiterter Bereich als optionales Paket: [Adult-System](Adult-System.md).
- 🧭 Planungsgröße: mehrere Hunderttausend NPCs; für nahezu jeden individuelles Aussehen aus einem Generator (siehe Abschnitt 12).
- ✅ Nur die Aussehensdaten für NPCs existieren (Generator, Rezepte); NPC-Verhalten nicht.

---

## 9. Körper, Fortschritt und Anpassung

### 9.1 Fortschritt

📋 Fertigkeiten wachsen vor allem durch Benutzung und Training (Details in [Spielvision](Spielvision.md#12-fortschritt-medaillen-und-erfolge)). Das gesamte Fortschrittsdesign ist ❓ offen.

### 9.2 Körpermodell

📋 Statt einfacher Gewichts- und Muskelregler ein interner **Anpassungsmodell**: Körpergewicht, Fett, Magermasse, regionale Muskelmasse, Hydration, Energie, Fitness, Kraft, Ausdauer, Beweglichkeit, Ermüdung und Erholung. Muskelgruppen (Brust, Schultern, oberer/unterer Rücken, Arme, Unterarme, Rumpf, Gesäß, Quadrizeps, Beinbeuger, Waden und weitere) passen sich unabhängig an, abhängig von Trainingsreiz, Erholung, Ernährung, Fitness und individueller Körperanlage. **Kraft ist nicht identisch mit sichtbarer Muskelgröße**; Technik und neuronale Effizienz können vor großer sichtbarer Hypertrophie zunehmen. Ernährung kann intern Kalorien, Eiweiß, Kohlenhydrate, Fett, Qualität, Flüssigkeit und Sättigung modellieren, ohne dass der Spieler Makros zählen muss. Kontinuierliche Morph-Ziele statt diskreter Körpertypen. Weitere Einflüsse: Schlaf, Stress, Alkohol, Rauchen, Müdigkeit, Verletzung. Training, Klettern und Alltag beeinflussen den Körper passiv (Muskelgedächtnis). Die Oberfläche kommuniziert den Zustand, ohne das Spiel zur Tabellenkalkulation zu machen.
- ❓ Formeln und Grenzen für Körperzusammensetzung, Muskelzuwachs und -verlust, Kraft, Kardio, Müdigkeit, Erholung, Flüssigkeit, Schlaf, Stress und Ernährung sind offen.

### 9.3 Lebensstil

- 📋 **Schlaf** hat Folgen, ohne zum ständigen Anzeigen-Management zu werden. **Essen** ist optionale Lebensstil-Aktivität statt Pflicht-Hungerschleife.
- 📋 **Alkohol** beeinflusst Reaktion, Gleichgewicht, Wahrnehmung, Dialog, Fahrfähigkeit, Schlaf und Erholung; extreme Vergiftung kann selten Blackout, Zeitverlust oder einen anderen Ort verursachen. **Drogen** haben unterschiedliche Wirkungen und Risiken; extremer Konsum kann zu Abhängigkeit führen. **Rauchen:** Zigarette anzünden, länger rauchen (teils im Gehen, Sitzen, Reden), kürzer werden, ausdrücken; kein magischer Fokus-Heal; kurzfristig mögliche subjektive Stress-/Aufmerksamkeitswirkung, langfristig Kardio, Erholung und Abhängigkeit. Tiefe der Abhängigkeits- und Entzugsmodelle ❓.
- 📋 **Kleidung** hat Wetter-, Schutz-, Stauraum- und soziale Funktion und kann nass, schmutzig, blutig oder beschädigt werden; Pflege ist vorhanden, aber nicht mühsam.
- 📋 **Inventar:** Der Körper hat begrenzten Stauraum; Fahrzeuge bieten Lager; das Hauptquartier kann sehr große, nachgefüllte Vorräte halten.
- 📋 **Gemeinsame Sekundärphysik:** eine gemeinsame Schicht für Rückstoß, Laufen, Kampf, Fahrzeugbeschleunigung und -unfälle, Stürze, Tragen, Interaktion, Haar, Kleidung, Zubehör und weitere Körperdynamik; realistische Dämpfung und Masse, kein übertriebenes Wackeln.
- 📋 **Alterung:** nur durch tatsächlich vergangene Story- oder Weltzeit oder Zeitsprünge.

---

## 10. Recht, Polizei und Beweise

📋

- **Gesetz folgt tatsächlicher Information** und Beweislage über Zuständigkeiten hinweg (lokal, Bezirk, Bundesstaat, Bund), nicht omniszienter Fahndungslogik. Beweisquellen: Zeugen, Überwachungskameras, Kleidung, Gesicht, Fahrzeug und Kennzeichen, Waffen und Ballistik, Fingerabdrücke und DNA wo passend, digitale Beweise, Komplizen. Meldelatenz beim Notruf; Verarbeitungs- und Verteilungsverzögerungen; Vertrauensmodell der Verdächtigung.
- **Strafsystem statt „Sterne“:** Tat → Schweregrad → Vorsatz oder Fahrlässigkeit → Opfer → Zeugen und Beweise → Notwehr/Nothilfe → Vorstrafen → Festnahme → Staatsanwaltschaft → Prozess oder Absprache → Urteil → Haft, Bewährung, Geldstrafe oder Freispruch. **Festnahme ist kein automatisches Game Over:** Verhör, Geldstrafe, Kaution, Gericht, Gefängnis und vergehende Zeit können auftreten. Das Gefängnis ist eine eigene soziale Simulation. Schwerer Fahndungsstatus bleibt bis zu glaubwürdiger Auflösung bestehen, nicht nur für eine kurze Zeit. Man kann **landesweit gesucht** werden. Aufgeben und Festnahme sind echte Alternativen zur tödlichen polizeilichen Lösung.
- **Polizei-Details:** Spürhunde und ein wiederverwendbares Geruchs-/Windmodell; wichtige abgelegte Waffen oder Gegenstände bleiben physisch und können Beweise werden (mit intelligenter Bereinigung gegen unbegrenzten Müll); Durchsuchung, Beschlagnahme und Fahrzeugdurchsuchung nach Regeln; Beweiskette und Aufräumen; Hintergründe der Polizeikarte je Schwierigkeit.
- **Selbstverteidigung** ist unabhängig vom Geschlecht. Schwere Straftaten sind Teil der Kriminalitäts- und Justizsimulation und werden ausschließlich über Konsequenzsysteme behandelt (Zeugen, Spuren, Ermittlung, Festnahme, Prozess, Haft, Ruf); es gibt dafür keine eigene Spielmechanik (siehe [Adult-System](Adult/09-Dunkle-Themen-und-Nicht-Uebernommenes.md)).
- **NPC-Reaktion:** NPCs, die ein schweres Verbrechen beobachten, können Polizei rufen, eingreifen, festhalten, fliehen, angreifen oder (bei besonders gewalttätigen Gruppen) massiv eskalieren, abhängig von Persönlichkeit, Fraktion und Moral.
- **Bargeld:** Nur Bargeld, das der Spieler tatsächlich bei sich trägt, ist bei üblichen Bargeldverlusten gefährdet; Geld im Auto, Safe, Hauptquartier oder auf der Bank geht nicht automatisch verloren.
- **Pfand-, Second-Hand- und Hehlerläden** mit wechselndem Bestand führen Alltägliches und gelegentlich Jäger-Relevantes.
- ❓ Details der Straftatenliste, Festnahmeschwellen, Strafen, Kaution, Zuständigkeitskarte, Überwachungsdichte, Beweiswertung, Spürhund-Modell, Durchsuchungsregeln und Polizeikarte.

---

## 11. Smartphone, soziale Medien und Weltereignisse

📋 Das Smartphone ist ein **diegetisches Breitensystem**: Nachrichten, Anrufe, Kontakte, Kamera, Galerie, Karten, Browser, soziale Medien, Nachrichten, Wetter, Bank, E-Mail, Musik, Taxi/Mitfahrt, Unternehmen und Jäger-Werkzeuge; ferner Navigation, Verkehr, Märkte, Sport und Wetten, Messenger, Lieferdienste, Läden, Immobilien, Fahrzeuge, Marktplatz und Jobs. Abdeckung und Funklöcher zählen; Satellitenkommunikation ist eine Spezialoption.

- **Soziale Medien** sind ein praktisch endloser Feed (handgeschriebene Beiträge, NPC-Beiträge, Weltzustand, Nachrichten, Werbung, Memes, Sport, Autos, Essen, Influencer, Fotos, Gerüchte, Fake-Inhalte, Kommentare, Reposts), regional gewichtet. **Kein Zwang** für Hauptgeschichte oder normale Nebenfälle. Sie erzeugen nur **optionale dynamische Weltereignisse** (siehe [Spielvision](Spielvision.md#112-dynamische-weltereignisse-und-soziale-medien)).
- **Digitale Spuren:** NPCs können filmen, fotografieren, streamen und posten. Das kann Polizei, soziale Medien und die Geheimhaltung beeinflussen. Digitale Spuren bei schweren Ermittlungen sind möglich, aber **kein magischer Vollzugriff**.
- **Internet im Spiel:** Geschäfte, Studios und Dienste haben Auftritte (Portfolio, Bewertungen, Termine, Preise, Online-Buchung); später kann der Spieler einfache Webseiten erstellen. Geschwindigkeit hat Spielwirkung.
- ❓ Name und Oberfläche des Spielnetzwerks, Feed-Ranking, App-Liste, Offline-Verfügbarkeit, Generator-Architektur, Verhältnis handgeschrieben zu generiert, Folgen von Livestreams und digitalen Beweisen.

---

## 12. Charaktereditor und Menschen

### 12.1 Ziel

📋 Menschliche Figuren sollen **so fotorealistisch wie im Rahmen der Produktion vernünftig erreichbar** sein, und der Charaktereditor soll **tief und modular** sein, für Spielerfigur **und** NPCs. Die Umsetzung erfolgt schrittweise: nicht jede spätere Anpassungsfunktion ist für Phase 0 nötig.

### 12.2 Umfang des Editors (Planung)

📋 Körpergröße und -bau, Schultern, Hüfte, Proportionen, Muskelmasse, Körperfett, Fettverteilung, Brust-, Gesäß- und Beinform, Hände und Füße, Hautdetails, Narben, Muttermale, Sommersprossen, Tattoos, Piercings, detailliertes Gesicht, Haare, Bart, Augen, Körperbehaarung, Make-up, Nägel, Stimme, gegebenenfalls dominante Hand, Startkleidung. Anatomische Merkmale Erwachsener werden dort einbezogen, wo das Nacktheits- und Körpersystem sie nutzt. **Kosmetische Anatomie ist von Gameplay-Stärke getrennt.**

- 📋 Körperliche Leistung ergibt sich aus dem Körperzustand, nicht aus einem pauschalen Geschlechtsmultiplikator.
- 📋 Kontinuierliche Morph-Kompatibilität für Körperzusammensetzung; Kleidung, Animation und IK müssen über den unterstützten Körperbereich funktionieren; Speicher- und Versionsregeln für Editor-Parameter; Regressionstests für extreme gültige Körperkombinationen; Kompatibilität mit Zwischensequenzen und Retargeting, Online-Slots, Barrierefreiheit, Voreinstellungen und Zufallsgenerator, Foto- und Porträterzeugung.
- 📋 Tattoos, Piercings, Körpermodifikation und kosmetische Eingriffe sind als Erweiterung desselben Editors geplant (siehe [Adult: Charakteranpassung](Adult/07-Charakteranpassung-Tattoos-Koerpermodifikation.md)).

### 12.3 NPC-Menschen

- 📋 Individuell aussehende NPCs aus einem **Generator**; Detail- und Simulationsstufen abhängig von der Entfernung, getrennt für Darstellung und Simulation; wichtige Figuren erhalten ausgearbeitete Tiefe, generische NPCs systemische Inhalte. Prozedurale Variation wird aus Archetypen und Zufallsvariation erzeugt, damit nicht jede Figur von Hand entsteht.
- 📋 Assets müssen für Detailstufen/Nanite geeignet sein; Drittanbieter-Inhalte bleiben getrennt von eigenen.

### 12.4 Was bereits existiert (✅)

Die **engine-unabhängige Datengrundlage für das Aussehen** im Core: Datenmodell (`AppearanceSpec`), Katalog und Validierung; deterministischer Generator; „Rezepte“ (Seed plus gezielte Abweichungen) für kleine Speicherstände; Editor-Grundlage mit zehn Bearbeitungsarten, atomar, mit Rückgängig/Wiederholen, **ohne Oberfläche**; distanzabhängige Detailstufen-Tabelle **ohne Zahlenwerte**. Die 29 vorhandenen Parameter sind **Platzhalter**. Darstellung in Unreal (Meshes, Materialien) und Editor-Oberfläche gibt es **nicht**. ❓ Echte Parameterliste; Menschen-Pipeline (MetaHuman, Blender/modular oder Mischung); Alters- und Geschlechtsvariationen als Parameter.

---

## 13. Wirtschaft, Besitz und Geld

📋 (Planungsstand 31. August bis 4. September 2026, Details ❓)

- **Single-Player-Wirtschaft:** Bargeld, Bank, Einkommen, Ausgaben, Vermögen, Immobilien, Fahrzeuge, Ausrüstung, Dienste, gegebenenfalls Schulden. Ausgaben: Waffen, Munition, Jäger-Material, Kleidung, Essen, Hotels, Fahrzeuge, Reparatur, Treibstoff, Rennen, Unterschlüpfe, Medizin, Freizeit, Clubs und Bars, Dienste. Gesamtbalance, Crafting- und Beutewirtschaft, Dienstleistungspreise ❓.
- **Firmen und Arbeit:** Dynamische Firmenwirtschaft (siehe [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#113-technisch-wirtschaftliche-weltschichten)). Taxi-, Fahrdienst-, Liefer-, Trucking-Jobs und andere; **Firmenjobs sind optional** und nicht die Hauptgeschichte.
- **Aktien:** Markt-, Limit- und Stop-Orders, Stop-Loss, Take-Profit, Leerverkauf, Gewinn und Verlust, Charts, Nachrichten, Firmendaten, Dividenden. Nur fiktive börsennotierte Firmen, anfangs eine Hauptbörse.
- **Krypto:** etwa 10–20 relevante Coins, hohe Volatilität, Bullen- und Bärenzyklen, Betrug und Pump-and-Dump; anfangs eine Hauptbörse.
- **Sportwetten:** 12–18 Buchmacher (scharf, halbscharf, weich) mit unterschiedlichen Quoten, Margen, Limits, Spielen/Ligen, Marktzahl, Livequalität, Boni, Cashout, Liniengeschwindigkeit und Kundenlimitierung. **Wert wird dem Spieler nie markiert.** Wettbörse mit Back/Lay, Liquidität und Provision. Geschickte Spieler können wirklich reich werden; die Systeme bleiben optional.
- **Glücksspiel:** Blackjack, Roulette, Poker, Craps, Automaten, Lotterie und kleinere ortsspezifische Spiele; Pferderennen.
- **Immobilien und Hausbau:** siehe [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#12-gebäude-innenräume-und-immobilien).
- **Weiteres:** Pfandleihen und Hehlerläden; Kauf und Verkauf von Ausrüstung; Creator-/Marktplatz-Konzepte sind nicht beschlossen (siehe [Spielvision](Spielvision.md#131-wirtschaft-und-monetarisierung-nicht-beschlossen)).

---

## 14. Grafik, Animation, Audio und Plattformen

### 14.1 Grafik

📋 (Planungsstand 31. August 2026)

- Sehr hohe visuelle Qualität bei guter Optimierung; **4K ist primäres Ziel**, 8K optional (Enthusiasten, Screenshots). PC unterstützt natives Rendering und hochwertige Hochskalierung (optional). Umfangreiche Grafikeinstellungen statt Niedrig/Mittel/Hoch. **Raytracing** gehört zum visuellen Ziel (Funktionen und Stufen werden später profiliert); **Path Tracing** nur als optionaler Hochleistungs-, Foto- und Kinopfad, nicht als Spielbasis.
- **Konsolen:** PS5 mit zwei Hauptmodi: Grafik (30 FPS, höchste machbare Qualität) und Leistung (60 FPS, 4K-Ausgabe mit reduzierter interner Auflösung); **kein 40-FPS-Modus geplant**. PS5 Pro versucht hohe Qualität bei 60 FPS, soweit die Profilierung es erlaubt. Xbox Series X folgt derselben 30/60-Philosophie, Series S erhält angepasste Rendering-, VRAM- und Streaming-Einstellungen unter Beibehaltung der Spielsysteme. Natives 4K plus 60 FPS plus maximales Raytracing auf der Basis-PS5 wird **nicht versprochen**, wenn die Profilierung es nicht trägt. Mehr Anzeigeoptionen als üblich (HDR/Kalibrierung, Bewegungsunschärfe, Filmkorn, chromatische Aberration, Kameraeffekte, Sichtfeld, Gore-Dauer), aber nicht jede PC-Konsolenvariable.
- **Optimierung ist Architekturanforderung von Anfang an**, nicht nur Politur: CPU, GPU, VRAM, RAM, Streaming, KI, Physik, Animation, Audio und Simulation werden gemeinsam budgetiert.
- ❓ Mindest-/Empfohlen-/Ultra-Hardware, native Auflösungen, Bildraten, Raytracing-Funktionssatz, Path-Tracing-Verfügbarkeit, 8K-Verhalten, PS5-Pro- und Series-S-Profile, Hochskalierungs- und Frame-Generation-Matrix.

### 14.2 Audio

📋

- Audio ist ein **Gameplay-System**, besonders für Horror und Tarnung. Verlustfreie Produktions-Master (Standard 48 kHz / 24 Bit, 96 kHz / 24 Bit für stark bearbeitetes Material) werden dauerhaft behalten. Lokalisierte 3D-Punktquellen werden aus passenden Mono-Quellen aufgebaut; Stereo/Mehrkanal für Musik und breite Ambiente. Komprimiertes Audio darf im normalen Gebrauch nicht hörbar „komprimiert“ klingen; Qualitätsstufen nach Wichtigkeit und Artefaktempfindlichkeit, endgültige Einstellungen nach Blindtests. Leise Horror-Klänge (Atem, Flüstern, Kratzen, ferne Schritte, Regen) verdienen höhere Qualität.
- Raum-/Portal-Ausbreitung durch verbundene Räume und Türen statt einfacher Direktradius; Spieler-Audio und KI-Gehör stammen aus demselben kanonischen Geräuschereignis, mit unterschiedlicher Hörerphysiologie. Geräuschereignisse tragen semantische Kategorien. Streaming, Caching und Priming halten eine große hochwertige Audiobibliothek praktikabel. Ein eigener QA-Zweig mit Blind- und ABX-Vergleichen, Plattform-Prüfung, Lokalisierungs- und Ausbreitungstests.
- **Distribution:** Standardinstallation mit hochwertig komprimiertem Ton; ein großes optionales verlustfreies Stimmenpaket (Studioqualität um 48 kHz, kein sinnloses „Hi-Res“); zusätzliche Sprachen getrennt wählbar; Ultra-/Kino-Texturen optional.

### 14.3 Animation und Produktion

📋 Komplexe Verletzungsanimation, Ganzkörper-IK, Motion Warping, prozedurale Anpassung, Gesicht (Augen, Blick, Brauen, Wangen, Kopf, Atmung, Mikroausdruck). Die Produktion von Umgebungen und Figuren folgt einem festgelegten Ablauf: Grundriss und Messvorgabe → Grauboxen → modulares Set → Zusammenbau → einzigartige Schicht → Materialien → Beleuchtung → Dressing → Gameplay und Audio → Optimierung und Prüfung; ein Asset ist nie „fertig“, nur weil eine Blender-Datei existiert. Bei der Kunstproduktion sind Messen („wie groß“) und Aussehen („wie sieht es aus“) getrennte Quellen der Wahrheit.

---

## 15. Modding

📋 **Architektur v1 (Block 5, bestätigt am 11. September 2026):**

- **Anbieterunabhängig:** Der Mod-Kern darf nicht von einem Steam Workshop oder einem einzelnen Anbieter abhängen. Online-Hosting ist optional und nur eine Quelle für Entdeckung, Download und Updates, nicht die technische Wahrheit für installierte Mods; die lokale Modmenge bleibt ohne Online-Anbieter nutzbar.
- **Standard-Ort:** `<GameRoot>/mods/`. Ein Mod-Manager durchsucht und registriert portable **`.spnmod`**-Pakete und/oder entpackte Verzeichnisse. Zusätzliche Bibliotheken auf anderen Laufwerken sind eine optionale Zusatzfunktion, die Standardinstallation hängt von keinem Launcher ab.
- **Aktivierung:** Installiert ≠ aktiviert. Aktivierung pro Profil und **pro Spielstand**; Deaktivieren löscht weder Paket noch Zustand.
- **Additiv:** Mods erweitern standardmäßig über APIs, Register und Hooks; Überschreiben nur mit ausdrücklichen Ebenen- und Ladereihenfolge-Regeln. Vorläufige Ebenen: Daten/Inhalt, Unreal/Blueprint, native C++-Mods (vertrauter nativer Code; öffentliche Schnittstellen mit geringstem Privileg). Lua nur, wenn es eine echte Lücke schließt.
- **Manifest:** ID, Version, benötigte Spielversion, Abhängigkeiten, Autor, Lizenz, Plattform-/Netzwerkkompatibilität; Herkunftsklassen (Basisspiel, offizielles DLC, Entwickler-Mod, Community-Mod); Entwickler-Mods erhalten keine Sonderrechte. Sicherheitskern nicht moddbar (Altersberechtigung, Zustimmung, Blockregeln, Serverautorität, Kontoberechtigungen, Spamschutz, Onlinesicherheit).
- **Modi:** Einzelspieler und Koop-Kampagne erlauben Mods; der offizielle Online-Modus **nicht**. Ein offizieller Mod-Browser mit komfortabler Installation und Abhängigkeitsauflösung ist gewünscht, mit automatischem Scan, Kompatibilitätstest und, wo möglich, Prüfung und Signatur für native Mods. Eine Sicherheitsprüfung kann beliebigen nativen Code nicht mathematisch garantieren.
- **Kritische fehlende Mods** blockieren das Laden standardmäßig, mit Experten-Ausnahme nach Sicherung.

✅ **Vorbereitung im Core:** Inhalts-IDs mit Namensraum (`namespace:pfad`); unbekannte Objektarten sind Daten und kein Fehler (ein Spielstand mit Fakten einer nicht registrierten Mod-Objektart lädt und speichert unverändert; nur das *Benutzen* meldet `UnknownKind`); Aussehen mit fehlenden Mod-Inhalten wird nicht zerstört.

❓ `.spnmod`-Manifest im Detail, Laden von Mods, Überschreibregeln, Registrierung neuer Objektarten, Hosting-Anbieter, Lua oder andere Skriptebene, Signierrichtlinie für native Mods.

---

## 16. Persistenz und Weltzustand

📋 Ein zentraler **persistenter Weltzustand** ist „das Gedächtnis der Welt“: Fallergebnisse, NPC lebt/tot, Beziehungen, veränderte oder zerstörte Orte, entdecktes Wissen, Monsteraktivität, Ruf, Fahrzeuge, Unterschlüpfe, Weltereignisse, Zeit und weiterer dauerhafter Zustand. Wichtige persistente Entitäten haben stabile IDs; die Weltpartitionierung lädt Laufzeitinhalt, der Weltzustand bestimmt die richtige Variante. Spielstände sollen segmentiert, inkrementell, absturzsicher, migrierbar, wiederherstellbar und komprimiert sein; ein Zielwert von etwa 20–50 MB bei langer Laufzeit, wo vernünftig erreichbar; Korrektheit, Zuverlässigkeit, Ladezeit und Wiederherstellung gehen vor Größe. PostgreSQL ist für strukturierte Backend- und Online-Daten bevorzugt, nicht als Pflicht-Datenbank für lokale Einzelspieler-Speicherstände.

✅ Der Kern (Fakten, Journal, Format v1, historische Abfrage, atomare Speicher-Slots) ist implementiert; siehe das README-Kapitel „Persistenz“ für Einzelheiten. ❓ Segmentiertes Speichersystem, Journal-Kompaktierung, Metadaten, Reichstrennung, Compare-and-Set für Koop.

---

## 17. Technische Vision und Arbeitsweise

### 17.1 Technik

- 📋 **Unreal Engine und C++20.** Es wird keine eigene vollständige Engine gebaut. Unreal liefert Rendering, Weltpartitionierung und Streaming, Animation, Audio, Oberfläche, Editor und Asset-Pipeline; **eigene C++-Systeme** entstehen dort, wo das Projekt maßgeschneidertes Verhalten braucht: Ermittlung, KI, Weltzustand, Persistenz, Verletzung, Kampf, Fahrzeuge, Modding und Werkzeuge. Keine stillschweigende Migration auf C++23.
- 📋 **Engine-neutrale Grenzen, wo sinnvoll.** Rein logische Systeme (Regeln, Zustandsübergänge, Beweise, Inventar, Speicherformate, Teile der Antriebslogik) liegen in portablem C++20; engine-abhängige Systeme werden nicht künstlich engine-unabhängig gemacht. Ein frühes Argument für Modularität war, einen späteren Engine-Wechsel realistischer zu machen; das ist eine Option und keine Absicht.
- 📋 **Kein Wiederholen ohne Grund:** wiederverwendbarer Offener-Welt-Simulationskern (Welt, NPCs, Wirtschaft, Fahrzeuge, Verkehr, Wetter, Immobilien, Innenräume, Bau, Sport, Polizei, Kriminalität, Internet/Medien, Ermittlung, Persistenz), über dem Jäger-, Monster-, Lore- und Ritualsysteme liegen. Ziel: spätere Spiele (Krimi, Detektiv, Auto, Lebenssimulation) können das Fundament nutzen, ohne dass mehrere Spiele gleichzeitig entwickelt werden.
- 🧭 **Hardware-Rollen:** Der Fedora-Laptop ist für Code und schnelle Tests ohne Engine, der Windows-Desktop für Unreal-Integration und schwere Builds.

### 17.2 Tests und Qualität

📋 Tests sind Teil der Architektur, nicht eine Schlussphase: Einheits-, Integrations-, Gameplay-, Regressions- und Randfalltests; für Risikosysteme (KI, Tarnung, Kampf, Verletzung, Fahrzeuge, Persistenz, Streaming, Modding, Plattform) zusätzlich deterministische Simulationen, generative Szenariomatrizen, Replay-Tests, Dauer- und Belastungstests, Fuzzing und Leistungsregression. Zufall ist über Seeds und Replays reproduzierbar. Echte Fehler werden zu dauerhaften Regressionstests. Manuelle QA bleibt für Gefühl, Kunst, Horror-Wirkung, Animation, Audio und Bildqualität wichtig. Kleine deterministische Testkarten für Waffen/Ballistik, Nahkampf, Verletzung, KI/Tarnung, Audio/Akustik, Fahrzeuge und Streaming. Inspektoren erklären KI-Entscheidungen. Ein eigener **Test-Orchestrator** soll Builds starten, Szenarien ausführen und Berichte, Logs, Abstürze, Screenshots, Replays, Leistungsspuren und Zustandsabzüge sammeln.

### 17.3 Entwicklung mit KI

📋 Das Projekt ist **KI-gestützt** („AI-first“). Technische Prüfung (Tests, Selbstprüfung) ersetzt keine kreative Freigabe: Der Entwickler ist der endgültige Entscheider für alles Subjektive (Modelle, Figuren, Monster, Waffen, Fahrzeuge, Umgebungen, Materialien, Animation, Gore, Oberfläche, Musik, Sounddesign, Stimme, Kinematik, Licht, Effekte, Story, Dialog, Fälle). Ein technisch validiertes Asset kann abgelehnt werden. Dazu gehören eine große, schrittweise geplante Prompt-/Spezifikationsbibliothek mit Qualitätstor, ein interner Spezifikations-Explorer und ein Scrum-ähnlicher Ablauf; diese Teile sind Werkzeuge der Vorproduktion und kein Spielinhalt.

---

## 18. Offene Punkte

❓ Zahlenwerte für Spielbalance, Fahrverhalten, KI und Simulation; Schadensmodell der Fahrzeuge; Fortschrittssystem; Todes-/Scheiterns-System; Schlossmodell und Interaktionsverben; Eingabe- und Hinweis-Oberfläche; Combat-Focus-Details; Gore-Schwellen; Straftatenkatalog und Strafmodell; Wirtschaftsbalance; Spielnetzwerk und App-Liste; Menschen-Pipeline und Parameterliste; Hardware-Anforderungen und Bildraten; `.spnmod`-Details. Die vollständige Übersicht steht in [Entscheidungsstatus](Entscheidungsstatus.md).
