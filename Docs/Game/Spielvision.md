# Die Spielvision: Geschichte, Fälle, Figuren und Fortschritt

> [!IMPORTANT]
> **Dies ist ein Planungsdokument, keine Beschreibung eines fertigen Spiels.** Spielbar ist heute nur ein technischer Prototyp (eine Testkarte mit Testtüren). Alles hier Beschriebene ist Planung oder Vision. Jede Aussage trägt eine Statuskennzeichnung (siehe [Abschnitt 1](#1-statuskennzeichnung)). Entscheidungsstatus und offene Fragen stehen gebündelt in [Entscheidungsstatus](Entscheidungsstatus.md).

Dieses Dokument ist eigenständig lesbar. Es vertieft die [README](../../README.md) und gehört zu den Spiel-Dokumenten in [`Docs/Game/`](.), neben [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md), [Gameplay-Systeme](Gameplay-Systeme.md), [Adult-System](Adult-System.md) und [Entscheidungsstatus](Entscheidungsstatus.md).

> [!NOTE]
> **Spoiler-Hinweis.** Die Hauptgeschichte enthält Enthüllungen, die der Spieler selbst entdecken soll. Dieses Dokument beschreibt Aufbau, Ton, Struktur und Spielprinzipien und nimmt zentrale Enthüllungen **bewusst nicht vorweg**.

**Inhalt**

1. [Statuskennzeichnung](#1-statuskennzeichnung)
2. [Was für ein Spiel es werden soll](#2-was-für-ein-spiel-es-werden-soll)
3. [Ton und Atmosphäre](#3-ton-und-atmosphäre)
4. [Die verborgene Welt](#4-die-verborgene-welt)
5. [Die Hauptgeschichte](#5-die-hauptgeschichte)
6. [Protagonist, Crew und Hauptquartier](#6-protagonist-crew-und-hauptquartier)
7. [Fälle und Ermittlung](#7-fälle-und-ermittlung)
8. [Das Hunter-Archiv](#8-das-hunter-archiv)
9. [Kreaturen und das Übernatürliche](#9-kreaturen-und-das-übernatürliche)
10. [Jäger, Organisationen und Fraktionen](#10-jäger-organisationen-und-fraktionen)
11. [Nebeninhalte und dynamische Ereignisse](#11-nebeninhalte-und-dynamische-ereignisse)
12. [Fortschritt, Medaillen und Erfolge](#12-fortschritt-medaillen-und-erfolge)
13. [Spielmodi: Einzelspieler, Koop und Online](#13-spielmodi-einzelspieler-koop-und-online)
14. [Schwierigkeit, Scheitern und Tod](#14-schwierigkeit-scheitern-und-tod)
15. [Dialog, Stimmen und Inszenierung](#15-dialog-stimmen-und-inszenierung)
16. [Begleitdienste: Guide-Plattform und Profil](#16-begleitdienste-guide-plattform-und-profil)
17. [Was nicht festgelegt ist](#17-was-nicht-festgelegt-ist)

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

Viele Gameplay-Details stammen aus dem **Planungsstand vom 31. August 2026**. Sie wurden seither nicht widerrufen, aber auch nicht jeweils neu bestätigt. Dokumente mit späterem Datum (insbesondere zu Weltgröße, Städten und Untergrund) haben Vorrang. Wo beide sich widersprechen, steht der Widerspruch ausdrücklich dabei.

---

## 2. Was für ein Spiel es werden soll

📋 **Ein eigenständiges, realistisches 3D-Spiel über Monsterjäger: Open-World-Action-Adventure mit tiefen Rollenspiel-, Ermittlungs-, Immersive-Sim- und Survival-Horror-Elementen.** Die Welt ist das moderne Amerika, in dem das Übernatürliche existiert, für die breite Bevölkerung aber meist verborgen, falsch erklärt oder vertuscht bleibt.

Das Spiel ist eine **originale Marke**. Es orientiert sich an der Genre-Tradition amerikanischer Roadtrip- und Monsterjagd-Geschichten, übernimmt aber keine Namen, Figuren, Waffen, Symbole, Lore, Kosmologie, Geschichten oder Designs bestehender Werke. Eine rechtliche Markenprüfung ist vor einer Veröffentlichung vorgesehen. Der endgültige Spieltitel ist ❓ offen; „Supernatural 3D Game C++“ ist der Arbeitstitel des Projekts.

### 2.1 Gestaltungsprinzipien

- 📋 **Entscheidungen, Beziehungen, Konsequenzen, Ermittlung, Vorbereitung, Ausrüstung, Wissen und organisches Fertigkeitswachstum** tragen die Rollenspieltiefe.
- 📋 **Vermieden werden:** sichtbare Monster- oder Charakterlevel, farbcodierte Beute-Seltenheit, an Stufen gebundene Gegner, automatische Gegnerskalierung und abstrakte Schadenszahlen als Fortschrittsmotor. Die Bedrohung ergibt sich aus den Regeln der Kreatur, ihrem Wissen, der Umgebung, Verletzungen, der Zahl der Gegner und der taktischen Lage.
- 📋 **Realismus und innere Konsistenz** gehören zu den höchsten Prioritäten des ganzen Projekts.
- 📋 **Keine Kugelschwämme:** Die richtige Waffe, Munition und die Schwäche der Kreatur sind das Kernstück der Jagd. Schwierigkeit entsteht aus Wissen, Vorbereitung, Zielen, Positionierung, KI, Kreaturenregeln, Ressourcen und Verletzungen.
- 📋 **Keine unnötigen versteckten Pflichtziele.** Ein Fall darf gelöst werden, sobald genug richtige Hinweise vorliegen.
- 📋 **Inhalte dürfen wieder relevant werden.** Orte werden nicht von einer Quest „verbraucht“.
- 📋 **Nicht „zum Marker laufen und fünf Gegner töten“, hundertmal.** Der Entwickler lehnt diesen Aufbau für Nebeninhalte ausdrücklich ab.

### 2.2 Die Kernschleife eines Falls

Der Vertical Slice (Phase 1) ist als **ein vollständiger übernatürlicher Fall** geplant; Zielwerte sind vorläufig (🧭 etwa 30–60 Minuten):

```text
Ankunft / Fahrt → Dialog → Ermittlung / Beweise → Recherche / Archiv
   → Vorbereitung → Konfrontation / Schwäche → dauerhafte Konsequenzen
```

Der Spieler erreicht einen Schauplatz (oft mit dem Fahrzeug), befragt Beteiligte, sammelt Hinweise, recherchiert die Natur der Bedrohung, bereitet sich vor, stellt die Kreatur unter Ausnutzung ihrer Schwäche und hinterlässt eine dauerhaft veränderte Welt.

---

## 3. Ton und Atmosphäre

📋 **Der Grundton ist düsterer Mystery-Horror**, ausdrücklich nach dem Vorbild der frühen Phase des Genres (nicht als Kopie einer konkreten Bildsprache, sondern als Ton- und Genre-Referenz für die eigene Marke):

- dunklere Nächte und Innenräume, wenig ausgeleuchtetes „Low-Key“-Licht,
- schmutzige Motels, Nebenstraßen, Tankstellen, Wälder, alte Häuser,
- Monster zunächst eher **bedrohlich und unbekannt**,
- **Ermittlung vor der großen Enthüllung**,
- weniger Fantasy-Spektakel im Alltag, Regen, Nebel, nasse Straßen, Neonlicht, praktische Lichtquellen,
- Sounddesign mit Stille, fernen Geräuschen und Spannung,
- Horror darf unangenehm und gruselig sein; die Welt bleibt trotzdem modern und realistisch.

Spätere, größere Lore, mächtigere Gegner und andere Dimensionen sind ausdrücklich vorgesehen. Sie erscheinen aber durch diese **dunklere Horror-Linse**: später großer Umfang, früher Horror-Ton.

Horror soll **systemisch** entstehen, nicht aus Skripten: aus Unsicherheit, Geräuschen, Dunkelheit, Strom und Licht, Türen, Wetter und KI-Verhalten. Kreaturen können Lichter und Stromversorgung manipulieren. Taschenlampen sind Werkzeuge und Risiken zugleich.

---

## 4. Die verborgene Welt

- 📋 Das Übernatürliche ist **öffentlich unbewiesen**. Das Internet ist voll mit gefälschten und mehrdeutigen Beweisen; echte Ereignisse verschwinden im Rauschen.
- 📋 Es gibt **keinen universellen magischen Gedächtnisverlust**. Die Welt erinnert sich, und die öffentliche Meinung kann sich über Jahre verschieben.
- 📋 Das Übernatürliche hat eine **mehrhundertjährige verborgene Geschichte**. Religiöse und kulturelle Gemeinschaften, Jägerfamilien, okkulte Gelehrte, geheime Archive, Programme aus Kriegszeiten und rivalisierende Organisationen können jeweils verschiedene Wissensfragmente besitzen. Künstlich oder rituell geschaffene Wesen und Wächter gehören zum Umfang. Historische Konfliktschichten sind in originaler Form nutzbar.
- 📋 **Geheimes Wissen** kann in der fiktiven Geschichte verborgene Technik umfassen. Solche Ideen sind erfundene Spielwelt und keine Aussage über reale Physik oder Geschichte.
- 📋 **Zeitreise** ist als großes System geplant (siehe [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#7-zeitreise-und-historische-weltzustände)).
- Physische Erscheinungen, Geister, Besessenheit, Flüche, Infektion und Verwandlung kommen selten vor und haben echte Folgen. Rituale brauchen echte Vorbereitung, Zutaten, Zeit und Bedingungen; es gibt **keine allgemeine Mana-Leiste**, der Spieler bleibt im Kern ein Jäger. Verfluchte Gegenstände sind dauerhaft, aber selten, können durch die Wirtschaft wandern und Fälle auslösen. Spukhäuser haben dauerhafte Geschichte mit indirekten Folgen für den Immobilienmarkt. Kulte können alltäglich oder tatsächlich übernatürlich sein, wachsen, spalten sich, zerfallen und lassen sich mit Detektiv- und Sozial-Tarnsystemen infiltrieren.
- 📋 Übernatürliche Geographie nutzt **historische und ursächliche Brennpunkte**. Eine subtile metaphysische Schicht ist möglich, ein sichtbares universelles Linienkarten-Gameplay nicht.

---

## 5. Die Hauptgeschichte

### 5.1 Was feststeht

| Aussage | Status |
|---|---|
| Die Hauptgeschichte ist in **Akte** und **Kapitel** gegliedert. Aktwechsel markieren große Phasen; Kapitelwechsel können Welt, Crew, Dialoge, Hauptquartier und verfügbare Inhalte verändern. | 📋 |
| Ein Akt kann um eine **große Bedrohung** kreisen. Solche Bedrohungen sind **langlaufende Handlungsstränge**, nicht Missionsgegner für eine kurze Mission: Sie wirken über Handlanger, Hinweise, frühe Begegnungen, gescheiterte Versuche und wechselnde Strategien, bis zur Konfrontation. Der Spieler kann mehrere Begegnungen überleben, bevor er versteht, wie die Bedrohung wirklich zu besiegen ist. Eine vermeintliche Hauptbedrohung kann sich später als Teil eines größeren Problems erweisen, wenn die Erzählung es trägt. | 📋 |
| Die Hauptgeschichte hat eine **stabile Handlungsachse („Spine“)**. Große Ereignisse bleiben weitgehend konsistent; Spielerentscheidungen verändern vor allem das **Wie**: Ermittlungswege, Routen, Dialoge, Informationen, optionale Ziele, kleinere NPC-Schicksale, Belohnungen, Beziehungen und Verletzungen. | 📋 |
| Es gibt **keine unkontrollierte Verzweigungs-Explosion**. Bedeutende Todesfälle zentraler Figuren sind bewusst geschriebene Ereignisse und kein Zufallsergebnis. | 📋 |
| **Hauptmissionen sind nie dauerhaft verpassbar.** Nebenmissionen normalerweise auch nicht, ausgenommen seltene logische Ausnahmen (z. B. Tod einer relevanten Person). | 📋 |
| Regionen sind **vor ihrem Handlungsbogen erkundbar**; Missionsverfügbarkeit hängt von Zustand und Abhängigkeiten ab, nicht von einer geografischen Sperre. Echte Sperren gibt es nur mit logischem Weltgrund. | 📋 |
| Geplanter Umfang der Hauptgeschichte: etwa **80–100 Stunden**. Dieser Wert stammt aus dem Stand 31. August 2026, ist ein Planungsziel und keine Zusage. | 📋 / 🧭 |
| Die Hauptgeschichte darf **etwas gerichteter** sein als Nebenfälle und trotzdem sinnvolle Wahlmöglichkeiten bieten. | 📋 |
| Hunter-Hauptquartier und Spielbeginn: siehe 5.2. | 📋 |

### 5.2 Der Anfang

📋 Nach Aussage des Entwicklers beginnt das Spiel **bewusst klein und düster**:

```text
ländliches Gebiet → persönliche / tragische Vergangenheit des Protagonisten
  → erste lokale Fälle → wachsendes Verständnis der übernatürlichen Welt
  → weitere Regionen → größere Zusammenhänge → irgendwann die Entdeckung des Hauptquartiers
```

- Am Anfang gibt es keine riesige Organisation, keinen Supercomputer und kein Artefaktlager, sondern Landstraßen, kleine Orte, Wälder, Motels, Häuser, lokale Legenden und etwas aus der Vergangenheit des Protagonisten, das die Geschichte ins Rollen bringt.
- Der Spieler startet mit **wenig übernatürlichem Wissen**.
- Das große **Hauptquartier** ist nicht die Startbasis. Es wird **erst nach mehreren Stunden Hauptgeschichte entdeckt**, und die Entdeckung soll ein bedeutender Moment sein. Danach wächst es zur Hauptbasis; nicht alle Bereiche müssen sofort zugänglich sein, Teile können jahrzehntelang versiegelt, beschädigt oder vergessen gewesen sein.
- ❓ Das **genaue auslösende Ereignis** ist offen. Die Details der Vergangenheit des Protagonisten und die Reihenfolge der frühen Fälle sind nicht festgelegt.

### 5.3 Was offen ist

❓ Akt- und Kapitelzahl, vollständige Gliederung der Hauptgeschichte, Namen und Eigenschaften der großen Bedrohungen, Figuren-Besetzung und Biografien, Endspiel, vollständige Kosmologie, endgültiges Hauptquartier (Ort, Aufbau, Ausbaufolge, mögliche Umzüge), Feldbasen und ihre Platzierung im Handlungsverlauf.

Konkrete Figurennamen, Handlungsstränge oder Kapitelinhalte gehören deshalb **nicht** in dieses Dokument.

### 5.4 Missionsverfügbarkeit

📋 Missionen folgen einem **Abhängigkeitsgraphen**. Verfügbarkeit hängt ab von Akt/Kapitel, früheren Missionen, Story- und Weltzustand, Beziehungen, Wissen, Ort, Zeit, Figurenzustand und Ergebnissen. Mögliche Zustände einer Mission: gesperrt, zulässig, entdeckbar, entdeckt, verfügbar, aktiv, unterbrochen, abgeschlossen, gescheitert, abgelaufen, ersetzt, Folgen ausstehend, archiviert. Der Graph dient der Freischaltung, Reihenfolge und lokalen Reaktivität, **nicht** einer exponentiell anderen Hauptgeschichte. Wiederholungen („Replay“) verändern den kanonischen Story-Zustand nicht.

---

## 6. Protagonist, Crew und Hauptquartier

### 6.1 Der Protagonist

- 📋 Es gibt **einen frei erstellbaren Hauptcharakter**. Ein fester Protagonist oder ein Wechsel zwischen mehreren spielbaren Hauptfiguren ist nicht das Kernmodell.
- 📋 Er kann männlich oder weiblich sein; beide spielen die ganze Geschichte, alle Fälle, Waffen, Fahrzeuge und Aktivitäten ohne Inhaltssperren. Körperliche Unterschiede dürfen realistische physiologische Grundwerte beeinflussen, aber weder einen einfachen Pauschalnachteil noch Sperren erzeugen. Leistung ergibt sich aus tatsächlichem Körper-, Trainings- und Verletzungszustand.
- Zum Charaktereditor siehe [Gameplay-Systeme](Gameplay-Systeme.md#12-charaktereditor-und-menschen).

### 6.2 Die Hunter-Crew

- 📋 Der Hauptcharakter gehört zu einer festen **Hunter-Crew**, einer familienähnlichen Gruppe. Das **soziale Gefüge** großer Gruppen in bekannten Western-Epen dient als Referenz für Dichte, nicht als Vorlage zum Kopieren.
- 📋 Crew-Mitglieder sind **ausgearbeitete Figuren** mit Persönlichkeit, Vergangenheit, Beziehungen, Konflikten, Tagesabläufen, Fähigkeiten, Fahrzeugen, Zimmern, Ausrüstung, Meinungen, Kontakten und Erinnerungen an wichtige Ereignisse.
- 📋 **Viele große Missionen** finden mit mindestens einem Crew-Mitglied statt; **Solo-Abschnitte** bleiben für Horror, Isolation und persönliche Geschichte erhalten. Begleiter sind teils frei wählbar, Hauptmissionen dürfen bestimmte Figuren verlangen. Abseits solcher Vorgaben ist Solospiel weiterhin gut möglich.
- 📋 **Das Spiel beginnt mit einem Zwei-Jäger-Kern**; später werden weitere Jäger getroffen und gewonnen. Der Spieler kann bewusst darauf verzichten, einen schwächeren oder unerfahrenen Jäger mitzunehmen.
- 📋 **Tod:** Gewöhnliche, anwerbbare Begleiter können dauerhaft sterben. Handlungskritische Figuren des Hauptquartiers sind vor beliebigem Sandbox-Tod geschützt; ausdrücklich geschriebene Todesfälle der Geschichte sind endgültig.
- 📋 **Persönliche Anfragen** der Crew haben mehrere faire Auslöser, sodass sie zuverlässig erscheinen, wenn man genug Zeit mit der Person verbringt. Sie entfallen nur, wenn die Figur stirbt oder dauerhaft geht.
- ❓ Besetzung, Biografien, Spezialisierungen, Todesfälle und Beziehungen sind nicht festgelegt.

### 6.3 Begleiter im Einsatz

📋 Begleiter sind **eigenständige, intelligente Akteure** und keine ferngesteuerten Puppen.

- **Planung vor dem Einsatz:** Rollen, Eingänge, Positionen, laut oder leise, abzudeckende Bereiche, Rückzug, Treffpunkt, Fahrzeugrolle. Einfache Aufträge brauchen keine schwere Planungsoberfläche, komplexe Aufträge bieten mehr Detail.
- **Befehle im Einsatz:** Folgen, warten, dorthin gehen, decken, beobachten, flankieren, Tür oder Ausgang sichern, vorrücken, zurückziehen, leise sein. Wo Handzeichen nicht möglich sind, kommen Stimme, Funk oder Telefon in Frage, mit hörbaren Folgen.
- **Abweichung** vom Plan nur bei unmittelbarer Gefahr oder neuer Information.
- **Persönlichkeit und Können** wirken auf die Leistung; Unerfahrene machen plausible Fehler, sabotieren aber nie zufällig. Begleiter-Tarnverhalten achtet auf Tarnabsicht, Tempo, Licht, Gegner, Schusslinien und Lärm.
- **Eigenfeuer** auf Begleiter ist physikalisch möglich und konfigurierbar (im härtesten Modus standardmäßig voll aktiv).
- **Hunde:** Ein Hund kann Begleiter sein (Rasse, Persönlichkeit, Training, Gehorsam, Rückruf, Suche, Spurenverfolgung, Reaktionen, Verletzung, Altern, Tierarzt). Spurensuche ist umweltbezogen und kein magischer Zielpfeil.

### 6.4 Das Hauptquartier

- 📋 Ein **dauerhaftes Hauptquartier** ist das emotionale und soziale Zentrum. Es **wächst**, statt dass die Gruppe grundlos umzieht. Bereiche können Schlafzimmer, Küche, Wohnzimmer, Garage, Waffenraum, Archiv, Werkstatt, Training, Krankenzimmer, Sicherheitsräume, Lager und Forschung umfassen.
- 📋 Die Crew lebt dort: schlafen, essen, trainieren, recherchieren, Fahrzeuge reparieren, Waffen pflegen, telefonieren, fernsehen, reden, streiten, feiern, gehen und zurückkommen.
- 📋 Versorgung (Lebensmittel, Medikamente, Munition, Waffenpflege, Ritualmaterial, Treibstoff, Werkstattmaterial) soll **organisch** sein, ohne lästigen Verwaltungszwang. Die Crew kann selbst Geld, Vorräte, Ersatzteile und Informationen beisteuern. Optionale Arbeiten (Waffen reinigen, Munition sortieren, Fahrzeug warten, Garage aufräumen, Archiv sortieren, Vorräte verstauen) und spontane Feiern nach Erfolgen oder Geburtstagen sind möglich, aber nie Pflichtmissionen. Große Vorräte werden nachgefüllt, damit Munition kein sinnloses Mikromanagement erzeugt.
- 📋 **Feldbasen** (Motels, Hütten, gemietete Häuser, Unterschlupfe, Bunker) dienen entfernten Handlungsbögen; ein kleines mobiles Wildnislager aus Fahrzeug- und Campingausrüstung ist möglich.
- ❓ Der Standort, Aufbau, die Ausbaufolge und mögliche geschichtsbedingte Umzüge sind offen.
- Eine **öffentliche Spielerkarte** zeigt das Hauptquartier nicht auffällig; geheime Orte bleiben an Entdeckung und Handlung gebunden.

---

## 7. Fälle und Ermittlung

> Vertiefung mit Gesprächsverlauf, Quellen und offenen Fragen: [Ermittlungs-Dokumentation](Ermittlung/README.md) (Module 02, 03, 05, 07).

### 7.1 Fälle

📋 **Zahlreiche hochwertige Nebenfälle** sollen sich wie eigenständige „Monster der Woche“-Episoden anfühlen.

- **Echte Mehrwege-Ermittlungen**, keine linearen versteckten Checklisten. Verschiedene Informationswege können zu einer gültigen Lösung führen. Falsche oder verfrühte Schlüsse können Folgen haben.
- **Stark variierend:** Fallstruktur, Beginn, Hinweismuster, Orte, Opfermuster, Recherchepfad, Monsterverhalten, Wendungen und Auflösung.
- **Auflösungen:** töten, vertreiben, bannen, fangen, Fluch brechen, Opfer oder Wirt retten, Artefakt zerstören oder sichern, verhandeln, überleben, evakuieren, überlisten und mehr.
- **Falsche Alarme und mehrdeutige Fälle** existieren; Fälle können gewöhnliche Kriminalität mit übernatürlichen Ursachen mischen, mehrere Arten oder menschliche Täuschung enthalten. Hinweise können gefälscht sein.
- **Fallbuch (Journal):** Angenommene Fälle erscheinen im Journal. Nach Abschluss hält es den tatsächlichen Ermittlungsweg, Entscheidungen, Vorbereitung und Folgen fest. Optional kann eine alternative Referenz-Lösung gezeigt werden, **nie als einzig richtiger Weg**.
- **Reihenfolge der Arbeit:** Hauptmissionen und große Nebenfälle sind handgeschrieben und hochwertig; viele dynamische Jäger-Aufträge entstehen aus der Weltsimulation.
- **Detektivarbeit ohne Übernatürliches:** Es gibt drei Fallklassen: übernatürliche Jäger-Fälle, gewöhnliche Detektivfälle und mehrdeutige Fälle, deren Ursache zunächst unbekannt ist. Gewöhnliche Fälle: Vermisste, Stalking, Betrug, Diebstahl, Brandstiftung, Entführung, Mord, ungelöste Altfälle, organisiertes Verbrechen, Korruption, Cyberkriminalität. Werkzeuge: Befragungen, Alibis, Observation, Verfolgung, Fotos und Videos, Abhören, Überwachungskameras, Internetrecherche, öffentliche Register, soziale Medien, Zeitleisten, Beweise, Forensik, Informanten, Archive. Fälle können scheitern oder falsch beziehungsweise zu früh abgeschlossen werden, mit bleibenden Folgen. Eine optionale Privatdetektiv-Laufbahn mit Büro und Lizenz ist gewünscht (Verdienstweg).

### 7.2 Ermittlungsmodi

📋 Der **Detektivmodus** ist kompromisslos:

- Name und Status des verfolgten Falls dürfen sichtbar sein, aber **keine automatische Ermittlungshilfe**: keine Questmarker, Suchzonen, hervorgehobenen Objekte, automatischen Schlüsse, Hypothesenvorschläge, „Such hier“-Hinweise, Beweiszähler, automatischen Beweisverknüpfungen oder versteckte Führung.
- Der Spieler markiert Hinweise, Personen und Orte selbst, bildet Hypothesen und setzt persönliche Marker und Ziele.
- **Geführte Modi** können schrittweise Hilfe ergänzen. Die Ermittlungsführung ist **unabhängig** vom Kampf-Schwierigkeitsgrad und anderen Realismussystemen einstellbar.

### 7.3 Beweise, Forensik und Spuren

📋 Beweise sind physisch und vielfältig:

- **Spuren:** Fußspuren, Blut, Reifen, Schleifspuren, Vegetation, Haare/Fell/Federn, Geruch; Wetter baut Spuren ab, Schnee verändert sie, Spuren können manipuliert oder falsch gedeutet werden.
- **Forensik:** Fingerabdrücke, DNA, Ballistik, Schuh- und Reifenabdrücke, Fasern, Toxikologie, Todeszeitfenster, Autopsieberichte, Tatortarbeit, Labore mit Bearbeitungszeit, Mehrdeutigkeit und übernatürliche Anomalien.
- **Zeugen:** Aussagen sind Informationen, keine garantierte Wahrheit; NPC-Überzeugungen können falsch sein. Gerüchte verzerren sich.
- **Recht und Beweise** laufen über das Rechtssystem der Spielwelt (siehe [Gameplay-Systeme](Gameplay-Systeme.md#10-recht-polizei-und-beweise)).

### 7.4 Organische Entstehung von Fällen

📋 Zeit-, wetter-, mond- und jahreszeitabhängige Fälle erscheinen **nicht** wie ein MMO-Quest-Spawn. Weltzustände haben glaubwürdige Vorlaufphasen: leise Zeitungsmeldungen, Vermisstenanzeigen, Polizeifunk, Gerüchte, Gespräche, Tierverhalten, seltsame Verletzungen oder Umweltanomalien, die sich zu einem erkennbaren Muster verdichten. Bei reduzierter Hilfe brauchen neue Fälle keine automatischen Kartenmarker; man findet sie über Informationen, Kontakte, Beobachtung und eigene Markierungen.

Wer ein sich entwickelndes Ereignis ignoriert, riskiert Folgen: mehr Opfer, Polizeieinsatz, Eingreifen eines anderen Jägers, Abwanderung der Kreatur, veränderte Beweise oder einen späteren ungelösten Altfall. Zeitlich begrenzte Bedingungen sind kein billiges Löschen von Inhalten; manche Ereignisse kehren zurück, wenn die Bedingungen wiederkehren.

---

## 8. Das Hunter-Archiv

> Vertiefung: [Ermittlung, Modul 04](Ermittlung/04-Archiv-Journal-und-Hunter-Record.md) (Archiv, Falljournal, Hunter Record, Anzeige von Unbekanntem).

📋 Das Archiv ist ein zentraler **Wissens-, Ermittlungs- und Entdeckungsgraph**. Es verknüpft Orte, Regionen, relevante Straßen, Gebäude, Personen, Monster, Flüche, Rituale, Symbole, Artefakte, Waffen, Fälle, historische Fälle, Dokumente, Fotos, Audio und Video, Zeitungsartikel, Polizeiberichte, Zeugenaussagen, Ereignisse und Anomalien. Querverweise sind wichtig; der Spieler soll Zusammenhänge selbst erschließen.

- **Wissen, nicht Debug-Ansicht.** Es zeigt, was die Figur entdeckt hat, keine internen Daten (KI-Gewichte, Schwellen, Spawnbedingungen, Wahrscheinlichkeiten).
- **Wissensstatus:** Aussagen können bestätigt, vermutet, umstritten, widerlegt oder historisch/unverifiziert sein. Jägerbücher, Familienwissen, Legenden und erfahrene Jäger können falsch, unvollständig oder veraltet sein. Beobachtung, moderne Tests, widersprüchliche Quellen und die Aussagen anderer Jäger können das Archiv berichtigen. Ein erfahrener Begleiter kann sich plausibel irren.
- **Orte sammeln Informationen** über die Zeit. Straßen werden nur Archiv-Einträge, wenn sie Geschichte, Fallbezug oder übernatürliche Relevanz haben.
- **Qualitative Anzeige** des Wissens statt ständiger Prozentzahlen. Unbekannter Inhalt verrät seine Kategorie nicht: Kennt man sieben von zehn Einträgen eines Gebiets, erscheinen die übrigen drei nur als „Unbekannt“. Gebiete haben absichtlich unterschiedliche Kategorie-Mischungen, damit die Oberfläche nicht verrät, was fehlt. Eine optionale exakte Fortschrittsanzeige ist später denkbar.
- **Kartenhierarchie:** Welt → Region/County → Stadt → Bezirk → relevante Straße/Ort → Gebäude/Komplex.
- **Hunter-Aufzeichnung:** Ein eigenes Fortschrittsmenü zeigt Geschichte, Fälle, Scheitern und ungelöste Fälle, Monster, Orte, Sammelobjekte, Erkundung, Artefakte, Waffen, Rituale und Personen. Der Monsterstatus ist reicher als „getötet“: begegnet, identifiziert, erforscht, vertrieben, gebannt, gefangen, getötet, Fluch gebrochen und mehr.
- **Sammelobjekte:** Der ursprüngliche Plan sah etwa 1.200–1.500 physische Sammelobjekte vor, das galt für das frühere, viel kleinere Weltkonzept (🕰️). **Für die heutige Welt sind die Zahlen offen** und müssen je Kategorie neu zugeordnet werden (nicht linear skalieren). Sammelobjekte sind optional. Entdeckungen (Orte, Straßen, Gebäude, Höhlen, Gewässer, Geschichte) sind eine getrennte Ebene und nicht alle physische Funde. Die Gesamtzahl der verfolgten Einträge lag früher bei etwa 5.000, ebenfalls ❓.

---

## 9. Kreaturen und das Übernatürliche

### 9.1 Klassifikation

📋 Kreaturen werden **mehrdimensional** beschrieben, nicht in einer Stufenhierarchie:

- **Existenzform:** körperliche Wesen, verwandelte Menschen, besessene oder kontrollierte Menschen, körperlose oder geistähnliche Wesen, zeitweilig manifestierte Wesen, an Objekte, Orte, Personen, Blutlinien, Rituale oder Ereignisse gebundene Wesen.
- **Herkunft** (getrennt von der Form): natürlich übernatürliche Arten, verfluchte oder verwandelte Menschen, erschaffene Wesen, beschworene Wesen, gebundene Geister, dimensionale Wesen, künstliche übernatürliche Experimente, unbekannte Herkunft.
- **Intelligenz** als mehrere unabhängige Dimensionen: Problemlösen, soziale Intelligenz, Taktik, Gedächtnis, Sprache, Selbstkontrolle, übernatürliches Wissen.
- **Motivation** statt eines „Aggro“-Werts: Hunger, Revierverhalten, Selbstverteidigung, Rache, Schutz, Fortpflanzung, Fluchzwang, fremde Befehle, Sadismus, Neugier, Überleben, rituelle oder religiöse Antriebe.
- **Temperament** wichtiger Individuen als interne Verhaltensfaktoren (vorsichtig, impulsiv, aggressiv, geduldig, neugierig, territorial, manipulativ, ängstlich, selbstsicher, grausam, einfühlsam, beherrscht), nicht als sichtbare Balken.
- **Alter und Erfahrung** sind getrennt. Sehr alte Wesen können durch Wissen und Erfahrung gefährlicher sein, ohne dass Alter einfach Lebenspunkte vervielfacht. Außergewöhnliche, uralte oder „Alpha“-ähnliche Individuen sind keine generischen Elite-Varianten, sondern unterscheiden sich durch Alter, Mutation, Blutlinie, Ritual, Herkunft, Vertrag, Erfahrung, Sonderernährung oder besondere Fähigkeit. Sie können die Standard-Schwäche ihrer Art teilweise überwinden; der Spieler erfährt das durch Beobachtung, Misserfolg und Recherche, nicht durch Beschriftungen wie „ALPHA“ oder „IMMUN“.
- **Aktivitätsbedingungen** (Tag/Nacht, Zeitfenster, Jahreszeit, Wetter, Temperatur, Mondphase, Ortseigenschaften) und **soziale Strukturen** (einzeln, Paar, Familie, Rudel, Kolonie, Hierarchie, kultähnliche Gemeinschaft, Herr/Diener, Schwarm). Eine Gruppe ist nicht automatisch moralisch einheitlich: Eine friedliche Familie kann koexistieren, während ein Mitglied gefährlich wird.
- **Fortpflanzung und Erschaffung** artspezifisch: Geburt, Übertragung durch Verletzung, Fluch, Ritual, Besessenheit, künstliche Erschaffung, Verwandlung, Vererbung oder Tod. Ausgewählte Arten können Jungtiere oder Lebensphasen haben; das ist kein universeller Mechanismus.

### 9.2 Schwächen, Widerstände und Anatomie

- **Schwächen wirken nicht nur als Bonusschaden.** Eine kanonische Schwäche kann tödlich, stark schädigend, regenerationshemmend, abwehrend, lähmend, schutzbrechend, manifestationszwingend, wahre Gestalt enthüllend oder fähigkeitsstörend sein.
- **Widerstände** sind intern abgestuft (immun bis empfindlich), die Oberfläche zeigt aber beobachtete Belege, keine Prozentwerte.
- **Anatomie ist modular und artspezifisch** (Körperregionen, Strukturen, lebenswichtige Organe, Fortbewegung, Sinnesorgane, übernatürliche Strukturen). Wo ein Wesen körperlich ist, gelten funktionale Verletzungen (ein zerstörtes Bein beeinträchtigt die Bewegung, außer es gibt eine Regenerations- oder Ausgleichsregel).
- **Regeneration** wird in Rate, regenerierbare Gewebe, Knochen/Organe/Gliedmaßen, Ressourcen, Umweltbedingungen, Unterdrücker und absolute Grenzen zerlegt.
- **Bleibende Narben und Verletzungen** überdauern Begegnungen, wenn die Art sie nicht regeneriert; wiederkehrende Individuen sind an den Folgen früherer Kämpfe erkennbar.
- **Erinnerung:** Intelligente, dauerhafte Wesen können sich an den Spieler, Begleiter, Verletzungen, Fallen, Waffen sowie Gnade und Feindseligkeit erinnern, aber nur an das, was sie plausibel beobachtet oder erfahren haben.

### 9.3 Lebendige Wesen statt Spawnpunkte

- 📋 **Monster sind keine respawnenden Spawnpunkte.** Stirbt ein Individuum, ist es weg. Starke, intelligente Wesen können dauerhafte Identitäten haben; gewöhnliche Arten sind bevölkerungssimuliert. Territorien, Nester, Rudel, Familien, Kolonien, Wanderung, Fortpflanzung und Wiederbesiedlung sind je nach Art möglich. Zerstörte Nester bleiben zerstört; spätere Wiederbesiedlung erfolgt durch neue Individuen, Gruppen oder Arten. Lebensraum, Beute, Zeit, Wetter, Jahreszeit und Rivalen zählen.
- 📋 **Wichtige Arten sind handgeschrieben.** Prozedurale Variation betrifft Alter, Temperament, Narben, Erfahrung, Größe in plausiblen Grenzen und Tendenzen, ohne lore-widrige Schwäche-/Körper-/Fähigkeitskombinationen zusammenzuwürfeln. Benannte Einzelwesen können eigene Geschichte, Modelldetails, Fähigkeiten, Verhalten, Archivseite, Fälle und langfristige Handlungsrelevanz haben.
- 📋 Das Archiv unterscheidet **Wissen über die Art** und **Wissen über bekannte Einzelwesen**.
- 🧭 **Umfang der Arten:** Der Entwickler bevorzugt grob **20–30 sehr tiefe Kernarten** statt 100 flacher; keine endgültige Festlegung.
- 📋 **Friedliche Wesen:** Menschenähnliche übernatürliche Wesen, die niemandem schaden, können ein normales Leben führen, wenn der Spieler sie verschont: Arbeit, Zuhause, Familie, Freunde, Routinen, spätere Begegnungen. Ein verschontes Wesen wird nicht zwangsläufig später zum Feind. **Andere Jäger und NPCs handeln eigenständig:** Ein anderer Jäger kann ein friedliches Wesen töten, wenn das zu seinem Wissen und seiner Haltung passt. Die Welt unterliegt nicht allein der moralischen Entscheidung des Spielers.
- 📋 Intelligente übernatürliche Wesen können unter menschlichen Identitäten leben (Wohnung, Arbeit, Fahrzeug, Bankkonto, Beziehungen, Geschäfte). Moral variiert: übernatürlich heißt nicht automatisch böse. Der Spieler kann manche verschonen, mit ihnen handeln, ihnen helfen oder dauerhafte Beziehungen pflegen. Seltene Gegner können Schutzorte des Spielers kompromittieren und einen Umzug erzwingen.
- 📋 **Wildtiere** ergänzen die Welt (siehe [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#9-natur-tiere-und-jagd)). Tiere können auf Übernatürliches reagieren, ohne perfekte Detektoren zu sein.

---

## 10. Jäger, Organisationen und Fraktionen

- 📋 **Jäger sind selten, aber nicht unmöglich selten:** Eine normale Person begegnet vielleicht ein- bis zweimal im Leben einem.
- 📋 **Lockeres Jägernetzwerk:** Einzelgänger, Teams, Veteranen, Informanten, regionale Unterschlüpfe und Depots, verlassene Jägerstätten, seltene Spezialgeschäfte neben normalen Läden als Versorgung, versteckte Foren und verschlüsselte Chats, aber auch alte analoge Quellen. Jäger haben Ruf; andere Jäger können sterben, zuerst ermitteln, konkurrieren, Fehler machen oder kooperieren. Der Spieler kann verdächtige Fälle an Hauptquartier oder Netzwerk melden, wenn er beschäftigt ist; NPC-Jäger können sie annehmen.
- 📋 **Mehrere Jäger- und Okkult-Organisationen** können existieren. Eine größere **wissenszentrierte Organisation** soll in ihrer Funktion vorkommen, aber mit vollständig eigener IP, eigenem Namen, eigener Geschichte, Struktur, Mythologie und Optik. Kleine geheime Behörden-, Polizei-, Bundes- oder Militär-Forschungseinheiten kennen Teile der Wahrheit; die meisten Polizisten nicht.
- 📋 Regionale Varianten, Mutationen und übernatürliche Hierarchien können vorkommen.
- ❓ Namen und Eigenschaften konkreter Organisationen und Fraktionen sind nicht festgelegt.

---

## 11. Nebeninhalte und dynamische Ereignisse

### 11.1 Handgeschriebene Nebeninhalte

📋 Nebenfälle (Abschnitt 7), Hunter-Aufträge, Fraktions-Inhalte, Jobs, Aktivitäten, Sammelobjekte, Lore und Archiv, Geheimnisse und versteckte Orte. Der genaue Umfang (Zahl der Nebenfälle) ist ❓ offen.

### 11.2 Dynamische Weltereignisse und soziale Medien

📋 Das Smartphone ist ein umfassendes Spielsystem (siehe [Gameplay-Systeme](Gameplay-Systeme.md#11-smartphone-soziale-medien-und-weltereignisse)). **Soziale Medien erzeugen optionale dynamische Weltereignisse** und **nie** Nebenmissionen:

- Weder die Hauptgeschichte noch handgeschriebene Nebenfälle **setzen soziale Medien voraus**.
- Ein praktisch endloser Feed mit handgeschriebenen und zustandsabhängigen Beiträgen; Beiträge entsprechen manchmal echten aktuellen Ereignissen (ein Streamer ist tatsächlich gerade an einem Ort; man kann freiwillig hinfahren; das Ereignis kann vorbei sein).
- **Fake-Inhalte sind Pflicht**, damit der Feed kein perfekter Monsterdetektor wird. Es gibt Folgebeiträge, Kommentare, Behörden-Erklärungen und Vertuschungsgeschichten.
- NPCs filmen, fotografieren, streamen und posten; das beeinflusst Polizei, öffentliche Wahrnehmung und die Geheimhaltung des Übernatürlichen.
- Ereignisse werden datengetrieben generiert (Vorfallsart, Ort, Zeuge, Beweis, falsche Spur, Ursache, Ergebnis, Archiv- und Belohnungswirkung), **nicht** durch freie Sprachmodell-Erzählung.
- 🧭 Die genaue Architektur, Ereignisfamilien und das Verhältnis handgeschriebener zu generierten Beiträgen sind ❓ offen.

### 11.3 Aktivitäten und Freizeit

📋 Die Welt soll über das Jagen hinaus viel zu bieten haben. Der Entwickler wünscht ausdrücklich, dass der Spieler **viele Systeme und Lebensstile gleichzeitig** verfolgen kann und nicht künstlich in eine Klasse gesperrt wird, nur weil er Jäger ist. Details und Tiefenstufen stehen in [Spielwelt und Untergrund](Spielwelt-und-Untergrund.md#10-aktivitäten-sport-und-freizeit); kurz: Motorsport (sehr hohe Priorität), Sport und Wetten, Glücksspiel, Aktien und Krypto, Immobilien und Hausbau, Jagd, Angeln und Tauchen, Wintersport, Camping und Wandern, Kino und Medien, Nachtleben, Jobs, Detektivarbeit, Metalldetektor, Urbex, Flohmärkte und Auktionen, Fotografie, Fitness und mehr. Die Tiefe wird gestaffelt: Fahrzeuge, Motorsport, Sportwetten, Glücksspiel, Angeln, Jagd, Tauchen, Wintersport und die Kern-Jägererkundung haben höchste Tiefe; andere Aktivitäten gewinnen Tiefe erst im Lauf der Zeit.

---

## 12. Fortschritt, Medaillen und Erfolge

### 12.1 Fortschrittsprinzip

📋 Fertigkeiten wachsen vor allem durch **Benutzung und Training**, nicht durch Punktekauf (wiederholter Schrotflinten-Einsatz verbessert den Umgang mit ihr, Sprinten die Ausdauer, Medizin, Fahren und schwache Hand durch passende Praxis). **Starke abnehmende Erträge** verhindern Ausnutzung („gegen eine Wand sprinten“). Das gesamte Fortschrittsdesign ist ❓ **offen** und wird in einem eigenen Block ausgearbeitet. Das Körpersystem (Gewicht, Fett, regionale Muskeln, Ernährung, Schlaf, Stress, Training) ist ein Teil davon (siehe [Gameplay-Systeme](Gameplay-Systeme.md#9-körper-fortschritt-und-anpassung)).

### 12.2 Missionsmedaillen

📋 Jede Mission wird bewertet:

- **Fünf Stufen:** Bronze, Silber, Gold, Platin, Diamant.
- **Kriterien** vor dem ersten Abschluss **verborgen**. Danach sind sie sichtbar, im Journal und Missionsarchiv nachlesbar und können gezielt wiederholt werden.
- **Anzahl der Kriterien:** kleine Missionen etwa 3–5, normale 5–10, große 10–15, sehr große 15–20 oder mehr.
- **Alternative legitime Lösungswege** werden berücksichtigt (alternative Routen-Logik).
- Im ersten Durchgang können **alle fünf Stufen** erreicht werden.
- **Wiederholung (Replay)** ändert den kanonischen Story-Zustand nicht.
- ❓ Gewichtung und Berechnung der Stufen, Darstellung der Ziele nach dem Abschluss, genaue Zahl und Liste der Erfolge sind offen.

### 12.3 Trophäen und Erfolge

- 📋 Das Platin des Basisspiels ist **vollständig offline** erreichbar, ohne Online-Trophäen im Basisset. Auch Jahrzehnte nach einer Serverabschaltung soll es möglich bleiben.
- 📋 **Eigene Gruppen** für Online, den härtesten Modus („Grounded“), Permadeath und jedes Story-DLC. Grounded und Permadeath sind keine Voraussetzung für das Basis-Platin.
- 📋 Vor der Veröffentlichung gibt es ein Prüfverfahren auf Verpassbares („Missable Audit“) und gute Guide-Tauglichkeit.
- ❓ Die endgültige Trophäenliste und ob eine Mindestschwierigkeit nötig ist, sind offen.

### 12.4 Profil-Plattform

🔭 Langfristig soll eine **eigene Profil- und Statistik-Plattform** entstehen (ähnlich einem Social-Club-Portal, aber tiefer): Geschichtsfortschritt, Akte und Kapitel, Missionen und Medaillen, Nebenfälle und Ergebnisse, Spielzeit, Archiv, Sammelobjekte, Regionen, Figur und Fitness, Beziehungen, Kampf, Monster, Fahrzeuge, Rennzeiten, Aktivitäten, Erfolge, Online-Slots, Rang, Geld, Immobilien, Fahrzeuge und Jobs. Optional: Freundesvergleich, interaktive Karte, Spoiler-Filter.

---

## 13. Spielmodi: Einzelspieler, Koop und Online

| Modus | Planung | Status |
|---|---|---|
| **Einzelspieler** | Komplett offline spielbar; Mods ja; Cheats ja; lokale Spielstände | 📋 |
| **Koop-Kampagne** | Die Einzelspieler-Kampagne mit Freunden, **primär 2–4 Spieler**; keine eigene Koop-Geschichte nötig; Mods ja; Cheats ja; **kein Transfer zur offiziellen Online-Progression**. Fortschritt gehört dem Spielstand des Gastgebers; Gast-Fortschritt ist an den Host-Spielstand gebunden. | 📋 Richtung; ❓ Netzwerkarchitektur und Zeitpunkt |
| **Offizieller Online-Modus** | Öffentliche, Freunde- und private Lobbys teilen **eine** persistente Online-Fortschrittsspur je Slot; **keine Mods, keine Cheats**; serverautoritativ, Anti-Cheat, Wirtschaftssicherheit, Crossplay-Kompatibilität; Sprach-Chat (gegebenenfalls über das Ingame-Handy). Mehr kooperative Jagd, Ermittlung, Jobs, Geschäfte, Erkundung, Motorsport, Freizeit, Crew-Aktivitäten und dynamische Ereignisse; weniger Griefing, bessere Spawn- und Polizeiregeln, keine übermächtigen Griefer-Fahrzeuge. Mögliche dauerhafte Hunter-Crews mit Spezialisierungen ohne harte Klassen, Feindseligkeits-/Verteidigungsmodus, Kopfgelder und Geschäfte. | 🔭 / ❓ |

- 🧭 **Lobbygröße:** Langfristig ist grob **64–100 Spieler pro großer Weltinstanz** ein technisches Ziel, **keine Zusage** für den Start. Die Zahl der Online-Slots pro Konto ist offen.
- Der Singleplayer-Modus soll **ohne Online-Zwang** funktionieren.

### 13.1 Wirtschaft und Monetarisierung (nicht beschlossen)

Folgende Punkte wurden **diskutiert, aber nicht endgültig beschlossen**. Sie sind **keine festgelegten Funktionen**:

- 🧭 Das normale Spiel soll fair und vollständig ohne Echtgeld spielbar sein; keine Lootboxen, keine Energie-/Wartezeit-Mechanik, kein „Pay-to-Revive“, keine käuflichen Kampfwerte oder körperlichen Statuswerte, kein künstlich schlechter Grind, Motorsport durch Klassen und Homologation gegen Pay-to-Win geschützt.
- 🧭 Eine **optionale Echtgeld-Beschleunigung** der gewöhnlichen Online-Währung wurde „im Prinzip“ als zulässig diskutiert, ohne dass Wissen und Fortschritt durch Geld ersetzbar sind. Preise, Pakete und Schutzvorkehrungen sind ❓ offen.
- 🧭 **Kreativmarktplatz mit Umsatzbeteiligung**, Förderfonds für hochwertige kostenlose Inhalte, Creator-Codes und eine großzügige Medienrichtlinie für Videos, Streams, Guides, Lore und Machinima: als Wunschrichtung dokumentiert; Aufteilung, Auszahlungsanbieter, Steuer-/Identitätsprüfung und Betrugsschutz ❓ offen. Kein freies Auszahlen gewöhnlicher Spielgegenstände oder Währung.
- 🧭 Fremde Inhalte kommen in den offiziellen Online-Modus nur durch eine Prüf-, Sicherheits- und Zertifizierungs-Pipeline und gelten dann als offizieller Inhalt.

---

## 14. Schwierigkeit, Scheitern und Tod

### 14.1 Schwierigkeitsgrade

📋 Es gibt viele Voreinstellungen von „Story/Leicht“ bis „Hart/Survivor“ bis zu einer extremen Stufe im Stil von **„Grounded“** sowie „Eigene“. Grounded bedeutet nicht mehr Lebenspunkte, sondern aufmerksamere, aggressivere, schneller zuschlagende und weniger nachsichtige Gegner (laute Nachlade-Geräusche in der Nähe können bemerkt und bestraft werden), schwerere Verletzungen, knappe Ressourcen und hohe Letalität. Darunter liegen dieselben tiefen Systeme; viele Hilfen und Realismus-Optionen sind unabhängig einstellbar. Die Anatomie bleibt in allen Stufen gleich, Parameter, KI und Folgen variieren. Barrierefreiheit ist eine **eigene Schicht** und nicht mit Schwierigkeit zu verwechseln. Grounded schaltet Komfort-HUD standardmäßig ab. Die Munitionsanzeige ist binär (an oder aus); ohne sie zählt man Schüsse und prüft Waffe und Magazin körperlich.

### 14.2 Tod und Scheitern: noch offen

❓ Die **endgültige Schleife bei Tod oder Scheitern** ist nicht entschieden. Festgelegt ist die **Richtung**:

- Ein klassischer Checkpoint-Neustart kann als optionaler Komfortmodus existieren.
- Standardmäßig **kein folgenloser Krankenhaus-Reset**.
- Bevorzugt: kritische/sterbende Zustände, kontextabhängige Rettung, artspezifische Monster-Ausgänge und medizinisch plausibles Todesrisiko.
- Schwere Verletzung ist nicht automatisch Niederlage: Man kann fliehen, einen Freund oder Jäger anrufen, zusammenbrechen, gerettet werden und im Krankenhaus aufwachen. Rettung hängt von Begleiter, Rettungsdiensten, Passanten, Polizei und Ort ab. Monster können gehen, entführen, verfluchen, besitzen oder töten.
- Folgen können Zeitverlust, neue Opfer, Wechsel des Monsterortes, veränderter Fallzustand, zurückgelassene Ausrüstung, ein am Ort zurückgebliebenes Fahrzeug, Krankenhaus und bleibende Verletzungen sein.
- Permadeath wäre nur optional. Ein Soulslike-Wiederherstellungsmodell ist ausdrücklich nicht das Ziel.

---

## 15. Dialog, Stimmen und Inszenierung

### 15.1 Dialog

📋 Dialog ist ein **datengetriebenes Inhaltssystem**: Jede gesprochene Zeile hat eine stabile Dialog-ID und verweist auf Sprecher, Text, Gesprächskontext, Emotionsdaten, Fall-, Wissens- und Beziehungsbedingungen, Wahlmöglichkeiten, Stimm-Asset und Revision, Lippensynchronisation, Untertitel/Lokalisierung und Prüfstatus.

- **Gesprächsarten:** natürliches Gespräch, Ermittlungsdialog über tatsächlich bekannte Themen und Beweise, wichtige Entscheidungsdialoge, dynamischer Kontextdialog beim Fahren, Kämpfen, Schleichen und Erkunden.
- Beziehung und NPC-Gedächtnis wählen Dialogvarianten, ohne verborgene Beziehungszahlen offenzulegen. Gespräche sind unterbrechbar und werden kontextbewusst fortgesetzt. Man kann beim Gehen, Fahren, Ausrüsten, Wundversorgen und Ermitteln sprechen, ohne in statischen Posen einzufrieren. Gespräche im Fahrzeug („Roadtrip-Gespräche“) sind ein wichtiger Kanal der Charakterentwicklung.
- **Tiefe nach Rang:** Hauptfiguren extrem tief, wiederkehrende Nebenfiguren tief, gewöhnliche Einwohner flacher, aber mit Ort, Wetter, Gerüchten, Polizeiaktivität und Weltzustand. Praktisch alle relevanten NPCs sind ansprechbar. NPCs sprechen auch untereinander, sodass Gerüchte organisch wandern und der Spieler Gespräche belauschen kann.
- **Umfang:** Frühere kleine Zeilenannahmen sind ausdrücklich verworfen. Hauptfiguren können insgesamt **Hunderttausende** Dialog- und Textzeilen mit Varianten erreichen, wichtige wiederkehrende Nebenfiguren 10.000–50.000 und mehr, insgesamt potenziell mehrere Millionen Datensätze. Die genaue Zahl ist ❓ offen; Architektur, Werkzeuge, Prüfung, Lokalisierung und Stimmenproduktion müssen darauf ausgelegt sein.

### 15.2 Stimmen

- 📋 Echte menschliche Darbietungen bleiben die bevorzugte Qualitätsquelle für Hauptfiguren und emotionale Schlüsselszenen. Für die Masse an Text ist ein Mix aus hochwertigen synthetischen Platzhalterstimmen, lizenzierter Stimmklon-Sprachsynthese und gezielter Sprache-zu-Sprache-Übertragung die bevorzugte Richtung. Provisorische Stimmen sind klar als nicht final markiert; öffentliche Vorabversionen dürfen offen ausgewiesene synthetische Stimmen nutzen.
- 📋 **Zustimmungs- und Rechtefragen:** Stimmklonen oder Nachahmen benötigt ausdrückliche, informierte Erlaubnis und klare kommerzielle Nutzungsrechte, auch bei Freunden und Familie. Es gibt keine Nachahmung bestehender Schauspieler oder anderer Personen ohne Rechte. Stimmmodelle bleiben gesicherte Produktionsmittel und werden nicht mit dem Spiel ausgeliefert.
- 📋 Stimmen sind austauschbare, versionierte Darstellungsressourcen hinter der Dialog-ID; Schauspieler können Platzhalter später ersetzen, ohne Missionslogik, Untertitel oder Spielstände zu brechen. Beim Austausch wird Lippensynchronisation aus dem Endaudio neu erzeugt.
- ❓ Besetzung, Anbieter und Vertragsstruktur sind offen.
- 📋 **Telefon, Funk, Polizeifunk, Walkie-Talkie, Mailbox und Aufnahmen** nutzen eigene Signalprofile bei erhaltener Verständlichkeit. Räumliche Akustik platziert trockene Dialoge natürlich in der Welt.
- 📋 **Gesicht und Körper:** Mimik umfasst Augen, Blick, Brauen, Wangen, Kopfbewegung, Atmung und emotionale Mikrobewegung. Körpersprache und Beziehungszustand beeinflussen Haltung, Blick, Abstand und Gesten. Große Dialogmengen brauchen hohe Automatisierung; Schlüsselszenen erhalten zusätzliche Handarbeit.

### 15.3 Charakterschreiben

📋 Der individuelle Ausdruck wird bewahrt: Alter, Region und Akzent, Wortschatz, Bildung, Persönlichkeit, Wissen, Humor, Stressreaktion, Tabuthemen. Wichtige Figuren erhalten eine „Voice/Character Bible“. Texte und Logik werden spielgetestet, bevor die endgültige Stimme aufgenommen wird, damit die Stimmproduktion das Missionsdesign nicht vorzeitig festlegt.

---

## 16. Begleitdienste: Guide-Plattform und Profil

🔭 **Offizielle Wissens- und Guide-Plattform** als großes Produktziel: Alle offiziellen Inhalte (Hauptmissionen, Nebenfälle, alternative Lösungen, Sammelobjekte, Orte, Monster, Waffen, Rituale, Fahrzeuge, Unterschlupfe, Mechaniken, Überleben, Fahren, Schwierigkeit, Modding und API, DLC, Versionsunterschiede). **Spoilerstufen:** spoilerfrei → kleiner Hinweis → stärkerer Hinweis → Ermittlungswege → vollständige Lösung. Ein optionaler KI-Assistent nutzt dieselbe Wissensbasis und respektiert die gewünschte Spoilerstufe; mit autorisiertem Spielzustandszugriff kann er auf tatsächlich Entdecktes zugeschnitten antworten, über eine offizielle Schnittstelle und nicht durch das Auslesen des Arbeitsspeichers. Ein optionales physisches oder digitales Lösungsbuch ist denkbar.

🧭 Zugang: Ein einmaliger Freischaltcode aus dem Spielkauf könnte dauerhaften Basis-Guide-Zugang zu einem Webkonto geben; Nicht-Besitzer könnten gegen einen kleinen Einmalpreis Zugang erhalten, der Preis ist ❓ offen. DLC-Guides setzen den DLC-Besitz voraus. Grundphilosophie: legaler Besitz soll die bequemste und bestunterstützte Erfahrung sein; keine feindselige Kopierschutz-Reibung.

🧭 **Vertrieb:** Ein physischer Besitz (Disc) ist ein ausdrückliches Wunsch-Ziel, soweit Plattformregeln es erlauben; das Basisspiel v1.0 soll möglichst ohne Internet oder Plattformkonto installier- und spielbar sein. Die Größe des Basisspiels geht von etwa 100 GB aus. Große optionale Pakete (Ultra-Texturen, verlustfreie Stimmenpakete, zusätzliche Sprachpakete) sollen wählbar sein, ohne die Standardinstallation aufzublähen.

---

## 17. Was nicht festgelegt ist

Zusammengefasst ❓ **offen**: Handlung, Figuren und Fraktionen; Akt- und Kapitelzahl; genaue Bedrohungen und Kosmologie; Zahl und Art der Nebenfälle; Hauptquartier-Details; das Todes-/Scheiterns-System; Fortschrittsdesign; Monsterliste; Trophäenliste; Zahl der Sammelobjekte; Online-Details und Monetarisierung; Stimmenproduktion; Spieltitel. Die vollständige Übersicht mit Quellen steht in [Entscheidungsstatus](Entscheidungsstatus.md).
