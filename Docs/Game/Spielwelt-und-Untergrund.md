# Die Spielwelt: Karte, Metropolis, Natur, Untergrund und die anderen Reiche

> [!IMPORTANT]
> **Die Karte ist nicht freigegeben.** Alle Größen, Namen und Lagen in diesem Dokument sind ein **Arbeitsstand der Planung** und kein fertiges Kartenmaster. Keiner der bisher erzeugten Kartenkandidaten wurde vom Entwickler als endgültig akzeptiert. Die Testkarte des Prototyps ist ein Platzhalter und kein Kartenkandidat. Jede Aussage trägt eine Statuskennzeichnung (siehe [Abschnitt 1](#1-statuskennzeichnung)).

Dieses Dokument ist eigenständig lesbar. Es vertieft die [README](../../README.md) und gehört zu den Spiel-Dokumenten in [`Docs/Game/`](.), neben [Spielvision](Spielvision.md), [Gameplay-Systeme](Gameplay-Systeme.md), [Adult-System](Adult-System.md) und [Entscheidungsstatus](Entscheidungsstatus.md).

**Inhalt**

1. [Statuskennzeichnung](#1-statuskennzeichnung)
2. [Zwei getrennte Linien der Weltplanung](#2-zwei-getrennte-linien-der-weltplanung)
3. [Wie sich die Größenplanung entwickelt hat](#3-wie-sich-die-größenplanung-entwickelt-hat)
4. [Der aktuelle Arbeitsstand der Hauptwelt](#4-der-aktuelle-arbeitsstand-der-hauptwelt)
5. [Geografie](#5-geografie)
6. [Metropolis und die weiteren Städte](#6-metropolis-und-die-weiteren-städte)
7. [Zeitreise und historische Weltzustände](#7-zeitreise-und-historische-weltzustände)
8. [Wetter, Jahreszeiten und Zeit](#8-wetter-jahreszeiten-und-zeit)
9. [Natur, Tiere und Jagd](#9-natur-tiere-und-jagd)
10. [Aktivitäten, Sport und Freizeit](#10-aktivitäten-sport-und-freizeit)
11. [Verkehr und Infrastruktur](#11-verkehr-und-infrastruktur)
12. [Gebäude, Innenräume und Immobilien](#12-gebäude-innenräume-und-immobilien)
13. [Gesellschaft und dynamische Welt](#13-gesellschaft-und-dynamische-welt)
14. [Die Untergrundzivilisation](#14-die-untergrundzivilisation)
15. [Himmel, Hölle und Fegefeuer](#15-himmel-hölle-und-fegefeuer)
16. [Wie Karten abgenommen werden](#16-wie-karten-abgenommen-werden)
17. [Offene Punkte](#17-offene-punkte)

---

## 1. Statuskennzeichnung

| Kennzeichnung | Bedeutung |
|---|---|
| ✅ | Implementiert und getestet |
| 📋 | **Bestätigte Planung:** vom Entwickler festgelegte Richtung, nicht implementiert |
| 🧭 | **Vorläufig:** Arbeitsstand, Testszenario oder Vorschlag, nicht festgelegt |
| 🔭 | Langfristige Vision ohne Spezifikation |
| ❓ | Offen: bewusst nicht entschieden |
| 🕰️ | Überholt: frühere Planung, die später ersetzt wurde |

Die Weltplanung ist mehrfach überarbeitet worden. Es gelten die Regeln: **spätere ausdrückliche Entscheidungen des Entwicklers gehen vor; ältere Planung bleibt als Geschichte erhalten, wird aber nicht als aktuell dargestellt; erfundene Details werden nicht ergänzt**. Namen von Orten, Gewässern und Städten sind **Arbeitsbezeichnungen der Planung** und nicht endgültig.

---

## 2. Zwei getrennte Linien der Weltplanung

Es gibt zwei Linien, die nicht verwechselt werden dürfen:

| Linie | Stand | Rolle |
|---|---|---|
| **Die ältere, abgeschlossene Welt** („WORLD_700“) | 📋 geografisch und optisch abgeschlossen; Makrogeografie gesperrt (ungefähr 690,9 km² Spielfläche laut Planungsstand vom 4. September 2026) | Historischer Referenz- und Technik-Master. Neue Inhalte dürfen **nur additiv** eingebracht werden. Sie ist **nicht** als klassisches DLC geplant, weil die größere Hauptwelt ähnlichen Charakter haben soll. Mögliche Verwendungen: Referenz- oder Vorproduktionswelt, technischer Benchmark, Quelle bewährter Gestaltungsmuster. Keine ist beschlossen. |
| **Die größere Hauptwelt** („WORLD_1500“, Arbeitsname der Planungslinie) | 🧭 in Planung; **keine Geometrie festgelegt** | Die geplante Standardwelt des Basisspiels. Erbt Gestaltungsprinzipien der älteren Welt, ist aber kein Ausbau davon. |

**Regeln:** Keine Geometrie wird zwischen beiden Linien übertragen. Die größere Hauptwelt darf **nicht erzeugt oder festgelegt werden**, bevor Anforderungen und räumliche Zuordnung („Spatial Allocation“) vollständig abgeschlossen sind. Die ältere Welt wird weder gestreckt noch als endgültige Geometrie der größeren behandelt.

---

## 3. Wie sich die Größenplanung entwickelt hat

Die Zahlen unterscheiden sich in verschiedenen Planungsdokumenten, weil die Planung sich mehrfach geändert hat. **Keine davon ist eine Geometrie-Festlegung.**

| Zeitraum | Planungsstand zur Größe der Hauptwelt | Status |
|---|---|---|
| bis Ende August 2026 | etwa 200–300 km² als Richtung für die Hauptkarte; die endgültige Größe sollte aus Reisezeiten, Straßendichte, Fahrzeuggeschwindigkeit und Inhaltsdichte abgeleitet werden | 🕰️ überholt |
| 4.–8. September 2026 | größere Standardwelt, **1.500–1.800 km²** (Vergleich 1.500 / ca. 1.650 / 1.800); ca. 1.650 nur als Favorit | 🕰️ überholt |
| 16.–27. September 2026 | etwa **2.000 km²** als weiches Ziel („Soft Target“), ausdrücklich keine Geometrie-Sperre | 🕰️ abgelöst |
| 13.–15. Oktober 2026 | **ca. 2.000–2.200 km²** als offener, nicht gesperrter Planungsrahmen; Stadtflächen als Hypothesen | 🧭 |
| 18.–19. Oktober 2026 | Kandidat von **ca. 2.200 km² trockenem, zusammenhängendem Festland** plus **bewohnter Ostinsel**, mit festgelegten Anforderungen an Geografie und Städte | 🧭 aktueller Arbeitsstand |

Die Hauptwelt soll **größenunabhängig** gebaut werden: Der Entwickler plant etwa **25 Karten-Größenvarianten**, und kein System darf den 2.200-km²-Kandidaten fest einkodieren. Stadtflächen, Geländedetail und Siedlungszahl müssen je Variante geplant werden; das Wachstum der Stadtflächen soll **sublinear** sein, nicht linear.

---

## 4. Der aktuelle Arbeitsstand der Hauptwelt

🧭 **Provisorische Voreinstellung** („Preset“) von 2.200 km². Das ist ein Arbeitsziel für die nächste Kartenversion und kein abgeschlossenes Kartenmaster.

| Bereich | Wert | Bedeutung |
|---|---:|---|
| Zusammenhängendes trockenes Festland | **2.200 km²** | aktuell angestrebte Fläche, ohne Wasser |
| davon Metropolis | 110 km² | rund 5 % des Festlands |
| Festland außerhalb von Metropolis | 2.090 km² | Differenz |
| Bewohnte Ostinsel | 80–100 km² | zusätzliches Land; 90 km² als vorläufige Testgeometrie |
| Lake Alder | ca. 25 km² | Seefläche (mit Alder Isle) |
| Lake Meridian | ca. 5 km² | Seefläche |
| South Reservoir | ca. 7 km² | Seefläche |

Wichtig: Die 2.200 km² beziehen sich auf **trockenes Festland**, nicht auf die Gesamtkarte, und sind **keine universelle Weltgröße**. Die Zahlen können sich von Entwurf zu Entwurf ändern; die frühere Aussage „Metropolis 20–25 km²“ war ein offenes Testszenario und wurde für diesen Kandidaten durch 110 km² ersetzt.

---

## 5. Geografie

🧭 Aktive Anforderungen des Entwicklers an die nächste Kartenversion (Stand 18./19. Oktober 2026). Sie gelten für den 2.200-km²-Kandidaten und **nicht** für die ältere Welt.

**Lage und Relief**

- Das **Meer liegt im Osten** (vor allem Ost/Südost). Norden und Süden werden nicht von offenem Meer umschlossen; das Festland setzt sich nach Westen und Süden über den sichtbaren Kartenrand hinaus fort.
- Das **Hauptgebirge liegt im Norden** und ist ein einzelnes, klares Makromerkmal. Sonst gibt es plausible Hügel, Wälder, Felder, Täler und Wildnis. Die ganze Welt soll **nicht** wie eine Felslandschaft aussehen; Gebirge, Wald und Täler wirken natürlich und nicht wie einheitliche Klippenränder.
- Breite Räume zwischen Siedlungen, Wälder, Weiden und Ackerland sollen lange Fahrten glaubwürdig machen und die „Spielzeugwelt“-Wirkung vermeiden.

**Wasser**

- Flüsse, Kanäle, Sumpfarme und zersplittertes Wasser werden **deutlich reduziert**; es bleiben wenige zusammenhängende natürliche Entwässerungssysteme und glaubhafte Seeabflüsse. Wasser soll das bewohnbare Land nicht dominieren.
- Benannte Gewässer: **Lake Alder** (mit Alder Isle), **Lake Meridian**, **South Reservoir**, **Greywater Bay**. Genaue Ufer und relative Lagen bleiben dem akzeptierten Kartenmaster überlassen.
- Die **Ostküste** soll ruhiger und organisch regelmäßiger sein, kein Labyrinth aus Mündungen und Inseln. Dazu kommt **eine auffällige große Insel** im Osten/Südosten (80–100 km², bewohnt, geografisch getrennt). Name und Form sind nicht festgelegt, und die Form soll nicht beiläufig umgezeichnet werden.
- Der **Hauptfluss** verläuft vom westlichen/nördlichen Seensystem an den nördlichen äußeren Bezirken von Metropolis vorbei zur Ostküste. Die Strecke zwischen Lake Alder und Metropolis darf nicht abrupt enden. Der Fluss geht **nicht** durch Glenhaven und **nicht** durch die Innenstadt von Metropolis.
- Glenhaven und Ashford liegen **abseits** großer Seen und Flüsse; South Reservoir liegt weiter im Südosten als in kritisierten Entwürfen.

**Weitere Anforderungen**

- Es gibt **eine geografisch maßgebliche Karte**. Regionale und Detailansichten leiten sich aus ihren Koordinaten ab; keine unabhängig erzeugten Küsten, Flüsse, Seen oder Stadtstandorte.
- Die visuelle Hauptkarte muss ein **flacher, senkrechter Atlas** sein; eine perspektivische oder isometrische Darstellung und ein 3D-Relief erfüllen die Anforderung nicht.
- Der Anspruch an Gelände und Hydrologie: natürliche Seeabläufe und eine zusammenhängende Flusstopologie.

**Karten-Geheimhaltung:** Eine allgemeine, für Spieler sichtbare Hauptkarte darf das **geheime Jäger-Hauptquartier nicht auffällig beschriften**. Geheime Orte bleiben in den Kartenebenen hinter Entdeckung und Handlung verborgen.

---

## 6. Metropolis und die weiteren Städte

### 6.1 Metropolis

„Metropolis“ ist der **Arbeitsname** der größten Stadt. Sie soll nach der aktuellen Kartenanforderung folgende Eigenschaften haben (🧭):

| Merkmal | Arbeitsziel |
|---|---|
| Fläche | ca. **110 km²** als Arbeitsgröße dieser Kartenversion, kein dauerhafter Weltmaßstab |
| Stadtteile | **20 unterscheidbare Bezirke** (die Bezirksnamen sind nicht festgelegt) |
| Innenstadt | ca. 20 km² |
| Hochhauskern | ca. 12 km², innerhalb der Innenstadt (keine zusätzlichen 12 km²) |
| Skyline | **3–5 Landmarken-Wolkenkratzer** im Bereich von etwa 700–1.000 m; Gestaltungsziele, die noch auf Machbarkeit zu prüfen sind, keine vorhandenen Modelle |
| Fluss | ein dominanter Fluss durch einen **äußeren** Bezirk, nicht durch Innenstadt oder Hochhauskern; Inseln, Arme und Brücken begrenzt und glaubwürdig |
| Form | organisch, nicht kreisförmig; abwechslungsreiche Wohn-, Gewerbe- und Industrienachbarschaften, Parks, Vegetation, Straßen- und Bahnhierarchie, Brücken, glaubhafte Dichtegefälle |

**Gestaltungsabsicht (📋):** Die Bezirke sollen Charakter und Vielfalt von New York, Chicago, San Francisco und Boston **evozieren**. Diese Städte sind Referenzen für **Atmosphäre, Vielfalt und Dichte**, aber **keine Vorgabe, reale Stadtflächen nachzubauen**. Die früher aus Hilfsrechnungen abgeleitete Gesamtsiedlungsfläche von 570 km² wurde ausdrücklich als Projektentscheidung abgelehnt. Gewünscht sind plausible Nachbarschaften, Hügel, Vegetation, Parks, Flüsse und organisch gewachsene Straßen- und Verkehrsnetze.

**Ältere Planung zu Metropolis (September 2026, damals für eine kleinere Weltgröße)** – als Ideen weiterhin lesbar, aber nicht neu gegen die 110-km²-Variante bestätigt (🧭):

- Ein dominanter Wolkenkratzerkern im Manhattan-Stil, aber nicht jeder Bezirk in dieser Dichte; historisch gewachsener Charakter mit älterem Stadtgefüge.
- Mischung aus **europäisch geprägter Verkehrs- und Fußgängerlogik** mit **nordamerikanischem Straßen- und Infrastrukturmaßstab**.
- Ein internationaler **Flughafen außerhalb des Kerns**.
- Ein dichtes Nahverkehrsnetz: grob 8–10 U-Bahn-Linien, 7–9 S-Bahn-Linien, ausgedehnte Straßenbahnen und Busse, starker Regional- und Fernverkehr.
- Viele Bezirkstypen: wohlhabende Wasser- und Hanglagen, Mittelstandsvororte, Arbeiter- und Industrieviertel, wirtschaftlich schwächere Gebiete, historisch verwurzelte Gemeinschaftsviertel.
- Wirtschaft: Finanzwesen (nicht übergroß), ein Technik- und Forschungscluster von höchstens etwa 1–2 km², Medien, Gesundheit und Pharma, Tourismus.
- Städte entwickeln sich über lange Zeiträume durch Sanierung, Aufwertung und Verfall.

### 6.2 Weitere Städte

| Name (Arbeitsbezeichnung) | Eigenschaften nach aktueller Anforderung |
|---|---|
| **Glenhaven** | Binnenstadt, optisch trocken, nicht um Flüsse, Seen oder Meeresfront gebaut |
| **Ashford** | wie Glenhaven eine Binnenstadt, ohne große Kanäle |
| **Greywater** | die **einzige klar maritime Hafenstadt**; Küste und Hafen glaubwürdig, **nicht venezianisch**, nicht von Kanälen durchzogen oder in überflutetes Sumpfland gebettet |

- 📋 **Siedlungshierarchie:** Metropolis > mehrere Großstädte > weitere Städte > Kleinstädte > Dörfer und Weiler. Größere Städte dürfen nicht als winzige Punkte erscheinen; gleichzeitig soll die Karte nicht mit zu vielen Kleinstsiedlungen überladen sein. Die Zahl der Siedlungen ist ❓ offen.
- 🧭 Frühere (September-)Planung wies zwei „Großstädten“ Charakterrollen zu: eine mit **älterer Industrie-, Automobil-, Fertigungs- und Bahnlogistik-Identität** (sichtbare Altfabriken, Leerstand, Umnutzung) und eine mit **Ingenieurwesen, Forschung und Hochtechnologie** sowie einer **Berg- und Wintertourismus-Wirtschaft**. Wie diese Rollen den Namen Glenhaven, Ashford und Greywater zuzuordnen sind, ist nicht dokumentiert.
- 📋 Die Bevölkerung konzentriert sich stark in den Städten, ländliche Gebiete und Wildnis bleiben dünn besiedelt.
- 🧭 Die zusätzliche Fläche der größeren Welt soll vor allem Wildnis, Küste und Wasser, Gebirge, Seen, Ökologie und Abstände aufnehmen, **nicht proportional mehr Stadt**.

---

## 7. Zeitreise und historische Weltzustände

📋 **Zeitreise ist als großes, geplantes System festgelegt**, nicht nur als einige feste „Zeitalter-Karten“.

- Im voll unterstützten Zeitraum kann der Spieler **jedes einzelne Jahr** anwählen. Der aktuelle Plan-Anker für jahrgenaue Unterstützung ist etwa **1600 bis zur Gegenwart**.
- Es gibt **keine harte kosmologische Untergrenze** für Reisen in die Vergangenheit. Frühere Zeiten bleiben erreichbar, anfangs mit geringerem Detail und später ausbaubar. Falls 1600 zu teuer wird, kann der früheste voll ausgearbeitete Zeitpunkt später verschoben werden ❓.
- Monate und Tage brauchen in der ersten Umsetzung keine eigene Zielwahl; eine praktische Standardeinstellung behält Monat, Tag und Uhrzeit bei einem Jahreswechsel bei.
- Der historische Zustand wird aus **zeitversionierten Schichten** (Geografie, Infrastruktur, Gebäude, Bevölkerung, Technik) und **Änderungsereignissen** rekonstruiert, nicht als vollständige Karte je Jahr gespeichert. Das motiviert das Journal-Konzept des Weltzustands (siehe README, Kapitel Persistenz).
- **Reisen in die Zukunft** bleiben ein offenes, strengeres Gestaltungsproblem, weil Spielerhandlungen den ursächlichen Zustand verändern können. Vollständig verzweigbare alternative Zeitlinien sind ein späteres, hochkomplexes Merkmal und keine Voraussetzung für den Start.
- ✅ Ein winziger Teil ist bereits implementiert: das Journal des Weltzustands kann frühere Zustände rekonstruieren (`StateAt`). **Zeitreise selbst ist nicht implementiert und für die jetzigen Systeme nicht vorausgesetzt.**

---

## 8. Wetter, Jahreszeiten und Zeit

📋 (Planungsstand vom 4. September 2026)

- Es gibt einen **vollständigen Kalender**: jeder Tag, jede Stunde, jede Minute existiert. Die Zeit läuft beschleunigt; eine Idee ist etwa **2 Echtstunden pro Spieltag**, nicht festgelegt ❓. Es gibt vier Jahreszeiten, Feiertage, Tourismus und saisonale Ereignisse.
- Hohe Gebirgslagen bleiben ganzjährig kühl: im Hochsommer etwa 10–15 °C, im Winter unter −10 °C. Dynamische **Schneeansammlung, Schneeschmelze und Spuren**.
- Regen und Gewitter sind wichtig, besonders für den Motorsport. Hochwasser, Stürme, Nebel, seltene Tornados, seltene Waldbrände bei anhaltender Hitze und Trockenheit.
- Saisonale Vegetation und langfristige ökologische Sukzession.
- Alterung geschieht nur durch tatsächlich vergangene Story- oder Weltzeit oder durch Zeitsprünge, nicht durch Echtspielstunden.
- ❓ Wie viel Echtzeit im Spielverlauf vergeht, und die genaue Kalender-/Jahreszeitenlogik sind offen.

---

## 9. Natur, Tiere und Jagd

### 9.1 Landschaft

- 📋 Große Wälder, Seen, Berge, Feuchtgebiete, Ackerland, Täler und abgelegene Wildnis sind wesentlich. Das Spiel soll mindestens so wild wirken wie die ältere Welt, eher wilder.
- 📋 Große **Höhlen** und eine **stark ausgebaute Unterwasserwelt** (Tierschwärme, Pflanzen, Felsen, Wracks, Höhlen, Tiefenzonen, versunkene oder ruinierte Strukturen, wo es passt) gehören zur Planung. Die Unterwasserwelt ist ein bedeutender Spielbereich; deshalb ist mehr Küste oder mehr Seefläche eine offene Gestaltungsoption.
- 📋 **PCG** (prozedurale Generierung) ist für Vegetation, Felsen, Straßenschmuck und kontrollierte Umgebungsvariation geeignet, **nicht** als Ersatz für handgebaute wichtige Orte. Hero-Gebäude sind einzigartig, Fall- und Story-Gebäude stark ausgearbeitet.

### 9.2 Tierwelt

📋 Ziel: Auf der Hauptkarte etwa **220–280 Tier-, Vogel- und Fischarten** (auf späteren Karten oder DLC mehr).

- Große Tiere, Raubtiere, kleine Tiere, Vögel, Wassertiere, Haustiere, Nutztiere, Pferde, Fortpflanzung und Nachwuchs, Kadaver und Aasfresser, Verkehrsopfer.
- Das **Studium** einer Art braucht eine **bewusste Studier-Aktion**; bloßes Sehen genügt nicht für vollständige Dokumentation. Pro Art kann es Fotos, Spuren, Lebensraum, Verhalten, Nahrungsaufnahme, Anatomie und weitere artspezifische Beobachtungen geben. Archivpflichtige gewöhnliche Arten sind nicht dauerhaft verpassbar.
- **Einzeltiere** variieren in Alter, Geschlecht, Zustand, Gesundheit, Verletzung und Größe; seltene benannte oder legendenartige Tiere ohne Arcade-Beschriftung.
- Materialqualität beim Häuten und Verarbeiten ergibt sich aus tatsächlichem Zustand und Schaden, nicht aus einem Sternesystem; bessere Felle werden teurer verkauft. Kadaver zerfallen, locken Aasfresser an und haben forensischen Wert; Proben und Fleisch verderben mäßig. Krankheiten und Parasiten sind vorgesehen.
- Jagd in rechtlich oder besonders zugelassenen Gebieten, tiefes Angeln, Tierbeobachtung und Fotografie.
- **Monster-Studium** spiegelt das Tier-Studium, auf tieferer erkenntnistheoretischer Ebene: Hypothese, Wahrscheinlichkeit, Bestätigung, Opferanalyse, Proben, Fähigkeiten und Schwächen. Intelligente Monster können sich an das Gejagtwerden anpassen.
- Wildlife-Details sind teils ❓ offen: genaue Artenliste, Abdeckung der Klassen, Studienkriterien, Preise, Verderbniszeiten, Simulationstiefe.

---

## 10. Aktivitäten, Sport und Freizeit

📋 Die Welt soll über das Jagen hinaus viel bieten, **ohne künstliche Feature-Überladung**; Aktivitäten sollen in die Welt passen. Der Umfang wächst gestaffelt (Planungsstand 4. September 2026):

| Stufe | Beispiele |
|---|---|
| **A (höchste Tiefe)** | Fahrzeuge, Motorsport, Sportwetten, Glücksspiel, Angeln, Jagd, Tauchen, Wintersport, zentrale Jäger-Erkundung |
| **B** | Basketball, Eishockey, Tennis, Golf, Bowling, Billard, Darts, Camping, Kajak und Rafting, Mountainbike, Minigolf |
| **C** | Sonnenbaden, Streicheln und Füttern, kleine Spielautomaten, Fernsehen, kleine Barbeschäftigungen |

Die Tiefe kann im Lauf der Zeit wachsen; nicht alles muss beim Start maximal tief sein.

**Sport**

- Nationale Ligen werden **vollständig simuliert**, ohne dass jedes Team physisch auf der Karte existiert; gebaut werden nur die lokalen Heimstätten. Auswärts- und Außenspiele werden über TV, Streaming, Radio, Zeitung, Sportbars und Wettanbieter verfolgt. Saisons, Karrieren, Alterung, Verletzungen, Transfers, Statistiken, Nachrichten und Wetten. Der Spieler kann Heimspiele besuchen. Hauptsportarten: American Football, Basketball, Baseball, Eishockey, Fußball, College-, Nachwuchs- und Highschool-Sport als Weltschicht. Mehrzweck-Arenen können mehrere Sportarten, Kämpfe und Konzerte ausrichten.
- **Motorsport** (sehr hohe Priorität): Formel-/Spitzenrennserien, GT, Tourenwagen, Sportwagen, Rallye, Bergrennen, Drag, Oval/Stock Car, Dirt Oval, Offroad, Rallye Raid, Kart, Drift, Autocross, Time Attack, Trackdays. Motorräder in vergleichbarer Tiefe: GP-Serien, Prototypen-Nachwuchs, Superbike, Supersport, Straßenrennen, Motocross, Supercross, Enduro und Hard Enduro, Rallye Raid, Bergrennen, Drag Bikes, Flat/Dirt Track, Supermoto und Club-Rennen. Motorsport kann so tief sein wie Fußball oder Eishockey, mit Teams, Herstellern, Fahrern, Qualifikation, Reifen, Wetter, Setups, Schäden, Boxenstopps, Entwicklung, Zuverlässigkeit, Strafen, Rekorden und Wetten. Mindestens **fünf Rennstrecken**. Im Planungsgespräch wurden zusätzlich mehrere getrennte **Rallyes mit je 10–20 Etappen**, **Stadtkurse** auf normalen Straßen mit realem Auf- und Abbau und ein **extrem gefährliches, überwiegend geschottertes Berg-Rennen** ohne Leitplanken auf großen Teilen festgelegt (🧭, Details ❓).
- **Kampfsport:** Boxen, MMA, Wrestling (auch Untergrund und Hinterhof), Pferderennen und -besitz, Tennis, Golf, Darts, Billard.
- **Spieleraktivitäten:** Basketball, Fußball, Tennis, Golf, Baseball (Schlagkäfige), Bowling (**auf jeden Fall spielbar**), Billard, Darts, Minigolf, Volleyball, Schwimmen, Fitnessstudio, Boxen und MMA-Training, Joggen, Skateboard, BMX.
- **Draußen:** Wandern, Trekking, Camping und Backcountry, Klettern, Bouldern, Abseilen, Mountainbike, Rennrad, Kajak und Kanu, Rafting, Angeln und Turniere, Jagd, Fotografie, Höhlenforschung, Höhlentauchen.
- **Wasser:** Scuba, Schnorcheln, Jetski, Schnellboote, Segeln, Yachten, Wasserski, Wakeboard, Surfen, Klippen- und Brückenspringen, volle Marina-Dienste.
- **Winter:** Ski alpin, Snowboard, Langlauf, Schneeschuh, Skitouren, Freeride, Schneemobil, Eislaufen, Eishockey, Winterveranstaltungen, Lawinenrisiko.
- **Unterhaltung:** Kino mit sehenswerten fiktiven Inhalten, TV, Streaming, viele Radiosender, Podcasts, Konzerte, Festivals, Bars, Spielhallen, Zoo, Aquarium, Museen. **Nachtleben ist wesentlich** (Clubs, Erwachsenen-Unterhaltung; siehe [Adult-System](Adult-System.md)).
- **Wasserpark** mit nutzbaren Becken und Rutschen und **Freizeitpark** mit fahrbaren Achterbahnen: je mindestens einer; kleinere nach der räumlichen Zuordnung. Ein selbst gebautes Fahrgeschäft ist eine **niedrig priorisierte** Zukunftsidee (Gelände, Budget, Stützen, Kapazität, Sicherheit, Wartung), kein Startziel.
- **Alltag:** echtes Einkaufen, Essengehen, Grillen, Haustiere, Gartenarbeit, Möblieren, PC-Spiele, Lesen, Fotografie, Drohnen, Lauschen. Kochen ist nicht erforderlich.
- **Dynamik:** Die Welt plant Treffen, Spiele, Clubabende, Konzerte, Märkte, Kampfveranstaltungen, Motorsport, Festivals und Pferderennen. NPCs besuchen sie wirklich und beeinflussen Verkehr, Nahverkehr, Parken, Bars, Hotels und Wettmärkte. Gewöhnliche Aktivitäten können gelegentlich Gerüchte, Hinweise oder kleine Aufträge erzeugen, bleiben aber meist normal und werden keine markierten Nebenmissionen.

Weiter, gleicher Planungsstand: Metalldetektor, Geocaching, Urbex, Flohmärkte, Hofverkäufe, Nachlassverkäufe und Auktionen sind akzeptierte Sandbox-Schichten.

---

## 11. Verkehr und Infrastruktur

### 11.1 Straße, Luft und Wasser

- 📋 Dynamischer, aber nicht absurder Verkehr: Stoßzeiten, Verkehrsmeldungen, seltene Unfälle und Pannen, Baustellen. NPC-Fahrer unterscheiden sich in Können und Persönlichkeit; Anfänger können mit manuellen Wagen abwürgen; Wetter ändert ihr Verhalten.
- 📋 Weitere Verkehrsmittel: Fahrräder, E-Scooter, Motorräder, Quads, Schneemobile, Boote an Marinas, kleine Flugzeuge **vom Spieler fliegbar**, Hubschrauber besitzbar (meist pilotiert), Verkehrsflugzeuge zum Reisen, Güter-Lkw, Schiene und Häfen. Taxi-, Fahrdienst-, Liefer- und Trucking-Arbeit. Wertparkservice an geeigneten Orten.
- 🧭 Ein internationaler Flughafen außerhalb des Stadtkerns und regionale Flughäfen sind in früheren Planungsständen vorgesehen; ihre Zahl und Lage sind ❓ offen.
- Details zu Fahrzeugen und Fahrphysik: [Gameplay-Systeme](Gameplay-Systeme.md#6-fahrzeuge-und-fortbewegung).

### 11.2 Eisenbahn

Planungsstand vom 15. Oktober 2026 (📋 bestätigte Bereiche, nicht exakte Bestandszahlen):

- **Alle Züge bleiben innerhalb der Spielkarte.** Kein Durchgangsverkehr nach außerhalb.

| Kategorie | Bestätigter Bereich |
|---|---:|
| Große Güterknoten | 3–4 |
| Regionale Güterterminals | 8–10 |
| Lokale Ladeanlagen | 18–25 |
| Große Zugdepots | 4–6 |
| Regionale Betriebsdepots | 9–13 |
| Hauptwerkstätten | 3–4 |
| Kleinere Abstellanlagen | 18–26 |

- **Elektrifizierung:** insgesamt hoch; Haupt- und S-Bahn-Strecken fast vollständig elektrisch, die meisten Regionalstrecken elektrisch, abgelegene Zweigstrecken können mit Diesel oder Batterie fahren. Prozentwert, Spannung, Oberleitung/Stromschiene und Unterwerke sind nicht festgelegt.
- **Langfristige Simulationstiefe:** nahezu vollständiger Bahnbetrieb mit realistischen Fahrplänen, Signalen, Weichen, Stellwerken, Blöcken, Fahrstraßen, Zugsicherung, Bremskurven, Verspätungsfortpflanzung, Rangieren, Stromversorgung, Instandhaltung, Fahrzeugumläufen, Personal und Betrieb außerhalb des Sichtbereichs. Vollständig interaktiv, aber **Fahren erfordert einen passenden Job beziehungsweise eine Berechtigung**. NPC-Betreiber halten das Netz unabhängig vom Spieler am Laufen. Das ist ein **Langzeitziel**; kein Teil ist implementiert.
- **Frühere Dichteziele** (kein Fahrplan): S-Bahn zur Stoßzeit 3–5 Minuten, Regionalexpress 10–15, Regionalzug 10–20, Zweigstrecken 15–30 Minuten.
- 🧭 Eine vorgeschlagene technische Zerlegung (Infrastrukturgraph, Betriebsgraph, Angebotsgraph, Güterverkehrsgraph, Streaming- und Offscreen-Simulation, interaktive Rollen) liegt als **Ingenieursvorschlag** vor und ist nicht beschlossen. Spurweite, Signalstandard, Hersteller, Streckenlängen und Dateiformat sind nicht gewählt.

### 11.3 Technisch-wirtschaftliche Weltschichten

📋 Dynamische Wirtschaft (Firmen können wachsen, schrumpfen, schließen, wieder öffnen, übernommen werden); viele fiktive Marken (grob 20–40 große Konzerngruppen plus lokale Firmen); Landwirtschaft, Forst, Steinbruch und Bergbau, Lebensmittelverarbeitung, Häfen, Fischerei, Energie, Telekommunikation. Mehrere Internetanbieter, ausgewählte Adressen bis etwa 50 Gbit/s; die Internetgeschwindigkeit hat echte Spielwirkung (große Downloads, Überwachungsvideo-Übertragung, Cloud-Sicherung, Fernüberwachung, Serverdaten). Firmenjobs sind optionaler Gelderwerb und nicht die Hauptgeschichte der Jäger.

---

## 12. Gebäude, Innenräume und Immobilien

📋 (Planungsstand 4. September 2026, Details ❓)

- **Mindestens 90 % der normalen Gebäude** sollen in irgendeiner Form betretbar sein; reine Fassaden sind eine Minderheit. Orte sind abwechslungsreich: Modulare Elemente dürfen wiederverwendet werden, aber wichtige Orte und Fallorte dürfen nicht erkennbar kopiert wirken.
- **Innenraum-Stufen:** Hero (einzigartig), voller Spielinhalt, prozedural, begrenzt.
- **Kaufen und mieten:** mehrere Immobilien; der Wert reagiert auf Lage, Zustand, Aussicht, Verkehr, Kriminalität, Wirtschaft und Entwicklung.
- **Grundstückskauf und Hausbau:** Der Spieler kann Land kaufen und ein Haus selbst entwerfen und bauen, einschließlich Garage, Werkstatt, Jägerräumen, Archiv, Serverraum, Fitnessraum, Pool, Garten, Nebengebäuden, Sicherheit, Notstrom und schnellem Internet. Die Hausgröße begrenzen vor allem Land, Geld, Gelände, Vorschriften und Technik, keine willkürliche Kleinstgrenze.
- **Stadtwachstum über Jahre und Jahrzehnte:** Bau, Abriss, Sanierung, Geschäftswechsel und Verkehrsprojekte.
- **Dichtes Wirtschafts- und Dienstleistungsgefüge:** Einzelhandel, Restaurants, Bars, Nachtleben, Casinos, Wettbüros, Banken, Hotels, Krankenhäuser, Colleges, Bibliotheken, Museen, Zoo und Aquarium, Arenen, Fitnessstudios, Autohäuser, Tuning-Werkstätten, Parkplätze, Logistik, Industrie, Friedhöfe, Rettungsdienste, Telekommunikations- und Rechenzentren, Marinas, Bergresorts, Campingplätze und seltene Jäger-Fachgeschäfte.
- Häuser, die Spukgeschichte haben, wirken sich auf den Immobilienmarkt aus (siehe [Spielvision](Spielvision.md#4-die-verborgene-welt)).

---

## 13. Gesellschaft und dynamische Welt

📋 Ein **zentraler Weltzustand** ist das Gedächtnis der Welt (siehe README, Kapitel Persistenz).

- **NPC-Leben:** Echte Tages- und Wochenabläufe, beeinflusst von Wochenenden, Arbeit, Wetter, Krankheit, Feiertagen und Ereignissen. Die meisten NPCs haben plausible Wohnungen, manche sind obdachlos. Familien, Freunde, Kollegen, Nachbarn und Bekannte; Beziehungen ändern sich. NPCs können umziehen, den Job wechseln, gefeuert oder befördert werden, ihren wirtschaftlichen Status ändern. Alterung ist vorgesehen; Generationenwechsel ist spät priorisiert.
- **Vier Persistenzstufen:** A: Story-, Jäger-, Schlüsselfall- und enge Beziehungsfiguren (maximale Persistenz); B: Händler, Nachbarn, Arbeitgeber, wiederkehrende Kontakte; C: normale Einwohner (leichtere Identität, vereinfachte Abläufe und Erinnerung); D: Menschenmengen (stark abstrahiert in der Ferne).
- **Gedächtnis:** NPC-Erinnerung erfordert Beobachtung oder Kommunikation; keine magische Allwissenheit. Gerüchte können falsch oder verzerrt sein. Es gibt keine einfachen sichtbaren Ruf-Balken: Lokaler, Jäger-, Polizei- und Kriminellen-Ruf ist überwiegend intern.
- **Gesprächsfähigkeit:** Praktisch jeder NPC ist ansprechbar; gewöhnliche NPCs haben kürzeren, kontextabhängigen Dialog.
- **Kriminalität** läuft unabhängig vom Spieler (Diebstahl, Einbruch, Raub, Fahrzeugdiebstahl, Körperverletzung, Vandalismus, Drogenhandel, organisiertes Verbrechen, Banden, Schmuggel, Untergrund-Glücksspiel, -Kämpfe und -Rennen), variiert mit sozioökonomischen Bedingungen, Netzwerken, Polizeiarbeit, Infrastruktur und Geschichte; ethnische Zugehörigkeit dient nicht als mechanische Ursache.
- **Notdienste** (Feuerwehr, Rettungsdienst, Polizei, Abschleppdienst, Bergrettung, Wasserrettung, Suchaktionen) sind in angemessener Tiefe simuliert. Vorfälle können dauerhaften Schaden erzeugen; NPC-Teams reparieren Straßen, Gebäude und Versorgungsnetze in glaubwürdigen Zeiträumen, der Spieler kann helfen.
- **Menschenmengen** sind wichtig (Stadien, Festivals, Nachtleben, Innenstadt, Skiresorts, Motorsport, Freizeitparks, Wasserparks) und nutzen Detailstufen und Abstraktion; beim Betreten eines Gebiets wird eine glaubwürdige lokale Bevölkerung rekonstruiert.
- 🧭 Die Planungsgröße der Bevölkerung liegt bei mehreren Hunderttausend NPCs; Einzelheiten und Simulationsbudgets sind ❓ offen.
- **Dynamische Welt:** Hausbau, Straßensanierung, Geschäfte öffnen und schließen, Baustellen, Brand und Reparatur, Rodung, Unfallräumung, Story- und Weltzustandsänderungen. Lose physische Gegenstände (Hüte, Brillen, Taschenlampen, Smartphones, Taschen, Waffen) können verloren und aufgehoben werden, mit Bereinigung und Persistenz.

---

## 14. Die Untergrundzivilisation

> [!NOTE]
> **Spoiler-Hinweis.** Die Untergrundzivilisation ist Teil der Hauptgeschichte. Dieser Abschnitt beschreibt Gesellschaftsprinzipien und Maßstab. Zentrale Enthüllungen über Zugang, Geschichte und Beteiligte werden **nicht** wiedergegeben. Der Hintergrund steht in einem eingeklappten Block.

### 14.1 Was feststeht

- 📋 Eine **verborgene, unabhängige Zivilisation** lebt in mehreren **abgeschlossenen Städten unter der Erde**, verbunden durch ein unterirdisches **Schienen- oder ähnliches Verkehrsnetz**. Sie ist keine bloße Erweiterung der Jägerbasis.
- 📋 Die Städte sind stark gegen die Oberfläche abgeschottet; viele Einwohner wissen wenig oder nichts über die Oberfläche. Die Einwohner sind misstrauisch gegenüber Fremden und Oberflächenbewohnern. Die Zugangs- und Wissensstufen sind ❓ offen.
- 📋 **Wünschenswerte Naturräume:** ein unterirdischer Fluss, Seen, Höhlen-Ökosysteme und offene Naturzonen. Herkunft, Entwässerung, Beleuchtung, Belüftung und Wasserchemie sind nicht festgelegt. Ein bewiesener, ewig geschlossener Realwelt-Ökokreislauf wird nicht behauptet.
- 🧭 **Maßstab:** Größere Städte umfassen mehrere Quadratkilometer oder mehr. Für die größte Stadt gilt eine **vorläufige horizontale Ausdehnung von etwa 50–100 km²**; das ist **keine Geometrie-Festlegung** und sagt nichts über Höhenstufen, Bevölkerung oder Kavernenlayout. Eine Gesamtbevölkerung im Bereich von mehreren Hunderttausend bis etwa einer Million ist nur ein grobes Szenario. Weitere unabhängige oder verbundene unterirdische Anlagen sind in der Planung.
- 🧭 Das Bevölkerungsmodell der Planung unterscheidet *horizontale Grundfläche*, *gesamte nutzbare Geschossfläche* und *durchschnittliche Dichte*; die Simulationsstufen der NPCs sind unabhängig von der Lore-Bevölkerung (die gesamte Bevölkerung muss nicht gleichzeitig individuell aktiv sein).

### 14.2 Gesellschaft

Bestätigte Entscheidungen zur Gesellschaft (Oktober 2026):

| Thema | Festlegung |
|---|---|
| **Regierung und Wirtschaft** | Weitgehend selbstorganisierend, ohne herkömmliche Regierung und ohne Steuern; Grundbedürfnisse sind am Ort der Nutzung kostenlos, ermöglicht durch fiktive fortgeschrittene Energie, Automatisierung und Ressourcenverteilung. Persönliche Freiheit und Privatsphäre bleiben zentral; Infrastruktur-KI bedeutet keine automatische allgemeine Überwachung. |
| **Zugehörigkeit** | Entsteht individuell durch zwischenmenschliches Vertrauen, nicht durch automatische Aufnahme oder feste Fristen. |
| **Haushalte** | Natürliche Vielfalt aller Familien- und Wohnformen. |
| **Lernen** | Man lernt, was einen persönlich interessiert; starker Praxisbezug, fortgeschrittene Lerntechnik, individuelles Lernen und freiwillige Begleitung. **Keine verpflichtende Liste akademischer Fächer** und kein herkömmlicher Schulzwang. |
| **Medizin** | Fortgeschrittene Heilung, Regeneration und Verjüngung, aber begrenzt; altes Wissen kann verloren sein. Genetik, Ersatzorgane und freiwillige körperliche Verbesserungen sind möglich; die meisten bleiben relativ natürlich. Keine Pflicht zu Verbesserungen. |
| **Lebensdauer** | Stark unterschiedlich, von Jahrhunderten bis möglicherweise Jahrtausenden. Keine feste Lebenserwartung und keine allgemeine Unsterblichkeit. |
| **KI und Roboter** | Infrastruktur-KI, allgegenwärtige Aufgabenautomatisierung und teils hochautonome Systeme koexistieren. KI hat unterschiedliche **Entwicklungsstufen**; Anerkennung und Behandlung sind **individuell**, nicht einheitlich für alle. Autonomie, Bewusstsein, Rechtspersönlichkeit und Rechte sind **verschiedene Dinge**; es ist nicht festgelegt, welche KI bewusst ist. |
| **KI-Aufsicht** | Regeln hängen von KI und Aufgabe ab (Routine-Serviceroboter mit aufgabenbezogenen Grenzen, Infrastruktur-KI mit zusätzlichen Sicherungen, Forschungs-KI mit weitreichender Unabhängigkeit und verhältnismäßigen Grenzen, hochautonome Systeme kontextabhängig). Eine allgemeine menschliche Übersteuerung wurde nicht gewählt. |
| **Kommunikation** | Geräte, holografische und räumliche Kommunikation, intelligente Umgebungen und **freiwillige** neuronale Schnittstellen koexistieren. Kein Zwangsimplantat, keine vorgeschriebene Modalität. Privatsphäre bleibt zentral. |

Alle Beispiele und technischen Taxonomien, die in Planungspaketen als Illustration standen, sind **keine** vom Entwickler bestätigten Weltfakten.

### 14.3 Offen

❓ Endgröße, Höhenstufen, Lage und Raumzuordnung der größten Stadt; Bevölkerung, Dichte, Kavernen, technische Machbarkeit; Zugangsregeln, Schienen- und Schleusenrechte; ein getrenntes Untergrund-Netz und seine Verbindung zur Oberfläche; AI-Rechte und Notfallbefugnisse; Sicherheit, Identität und Ausfallmodi neuronaler Schnittstellen; die Beziehung zwischen den unterirdischen Städten und weiteren Jägereinrichtungen.

<details>
<summary>Hintergrund der Untergrundzivilisation (Spoiler)</summary>

- Die verborgenen Städte wurden **vor Jahrtausenden** von einer außergewöhnlich fortgeschrittenen Frühzivilisation gegründet und über die Zeitalter verändert. Die meisten heutigen Einwohner sind Nachfahren. Einige wenige ursprüngliche Gründer leben noch, sind aber **schwer körperlich eingeschränkt**; die Unsterblichkeit bedeutet nicht ewige Jugend oder Gesundheit. Ihr geistiger Zustand ist offen.
- Die Städte besitzen außergewöhnliche **fiktive** Infrastruktur, darunter ein erfundenes Energiesystem. Es handelt sich um spekulative Fiktion und nicht um eine Behauptung über reale Physik.
- Auch die Oberflächen-Elite kennt ihre Existenz, nicht aber die empfindlichsten technischen Geheimnisse, und versucht sie zu erlangen. Wissen um die Existenz einer Stadt ist weder Zutrittsrecht noch Kenntnis ihrer Technik. Die Elite ist ein erfundenes Machtnetzwerk aus Einzelpersonen unterschiedlicher Herkunft; keine reale ethnische oder religiöse Gruppe trägt eine kollektive Verantwortung.
- Zwischen den Einwohnern und den Jägern gibt es eine **alte Allianz zum gegenseitigen Schutz** gegen Bedrohungen; ein pauschaler, uneingeschränkter Zugang für Jäger ist nicht bestätigt, und nicht alle Bewohner akzeptieren die Jäger. Die Einzelheiten der historischen Vereinbarung sind nicht geschrieben.
- Eine gesicherte Jäger-Einrichtung innerhalb des Untergrund-Komplexes ist ein eigener geschützter Bereich und nicht gleichbedeutend mit einer ganzen Stadt. Ob sie mit dem Hauptquartier an der Oberfläche identisch, getrennt oder verbunden ist, ist ❓ offen.
- Weitere zentrale Enthüllungen der Hauptgeschichte um diesen Strang sind **bewusst reserviert** und werden nicht festgelegt oder offengelegt.

</details>

---

## 15. Himmel, Hölle und Fegefeuer

📋 Das Basisspiel soll **mindestens drei übernatürliche Open-World-Reiche** enthalten: **Hölle, Fegefeuer und Himmel**. Es sind keine gewöhnlichen Level und keine umgefärbten Karten der materiellen Welt. Jedes Reich braucht eine eigene **Weltbibel**, räumliche Zuordnung, wichtige Orte, Bewegungsregeln, Persistenz- und Weltzustandsregeln, gegebenenfalls Ökologie und Gesellschaft sowie ein eigenes Karten-Master. Die **Größe eines Reichs sagt nichts über den Entwicklungsaufwand**.

| Reich | Planungsbereich (keine Festlegung) | Arbeitsfavorit | Charakter |
|---|---:|---:|---|
| **Hölle** | ca. 350–500 km² | ca. 420 km² | dicht, vertikal, labyrinthisch und innenraumreich statt weit und flach; Macht-Strukturen, Gefängnisse, Ruinen; mögliche **Zeitdehnung** (Verhältnis ❓) |
| **Fegefeuer** | ca. 500–750 km² | ca. 650 km² | Wildnis, Überleben, Monsterökologie; der stärkste Kandidat für Territorium, Jagd, Wanderung und Nahrungsketten |
| **Himmel** (gemeinsame Welt) | ca. 250–400 km² | ca. 320 km² | **kein** Wolken-und-weiße-Gebäude-Klischee; vertrautes und fremdes, eingeschränktes Gelände mischen sich; zusätzlich variable **Gedächtnis- und Taschendomänen**, die den Erlebnisraum größer machen als die gemeinsame Fläche |

**Kandidaten für wichtige Orte** (Arbeitsbezeichnungen, nicht endgültig):

- *Hölle:* Ankunfts-/Abstiegszone, zentrale höllische Stadt oder Zitadelle, Gefängnisviertel, Ruinenfelder, Aschewüsten, ein dunkler Fluss oder Wasserweg, altes höllisches Viertel, Tore und Portalkomplexe, verlassene Festung, Tiefe Hölle.
- *Fegefeuer:* endloser Wald, Knochen- bzw. Schlachttal, graue Berge und Höhlen, totes Flussbecken mit Feuchtgebieten, uralte Ruinen, verlorene Jägerlager und Gräber, Raubtier-Reviere, seltene sichere Lichtungen, Portalnarbe als unstabiler Übergang, tiefe Wildnis.
- *Himmel:* zentrale himmlische Stadt als Verwaltungszentrum, Gartenregion, Archiv der Leben, Hallen des Übergangs, persönliche Erinnerungsdomänen, abgesperrter Bezirk, vergessener älterer Himmel, Tor-Netzwerk, Observatorium oder zeitliches Archiv, Grenzregion.

**Bewegung und Persistenz**

- Die Bewegung im Reich soll sich **spürbar von der gewöhnlichen Autofahrt** unterscheiden: In der Hölle können reichsspezifische Passagen oder Tore und Transportmittel dienen, das Fegefeuer ist vor allem Wildnis-Fortbewegung, der Himmel nutzt besondere Tor- und Übergangsregeln.
- Zugang ist **selten**: über Story, Ritual, Portal, Tod oder besondere Ereignisse, **nicht** über normales Schnellreisen.
- Große Reichs-Aktionen sind **dauerhaft** (geöffnete Tore, entfernte Artefakte, zerstörte oder veränderte Orte, befreite oder getötete Wesen, Machtverschiebungen).
- Eigene Sammel- und Entdeckungsplanung für jedes Reich.
- Zeitverhältnis der Hölle: von einer bekannten Idee inspiriert, das Verhältnis für die eigene IP ❓ offen. Das Zeitverhältnis des Fegefeuers ebenfalls ❓.

❓ Offen: genaue Reichsgrößen und -grenzen, Namen, Kosmologie und politische Strukturen, die Architektur der Gedächtnisdomänen, Regeln zur Zeit, Schnellreise und Transport, Tod und Jenseits-Ablauf.

---

## 16. Wie Karten abgenommen werden

- 📋 Das Kartenmaster entsteht in **Qualitätsstufen**: Eine vorgeschlagene Karte, die technisch alle Prüfungen besteht, kann trotzdem **visuell abgelehnt** werden. Der Entwickler entscheidet; ein technisch bestandener Kandidat ist keine akzeptierte Geografie.
- 📋 Verbesserungen erfolgen **inkrementell** (bestehende Grundlagen bewahren), nicht durch erneutes Erzeugen der ganzen Welt in 16K.
- 📋 Eine Abnahme-Spezifikation und Prüfregeln sind vorgesehen (Koordinatensystem, Toleranzen, Rasterauflösung, Georeferenz, Ableitungsprüfung von Master zu Detail, Flächenmessung, Fluss-Zusammenhang). Der **aktuell zuletzt erzeugte Kartenstand** wurde bisher nicht durch gemessene Flächen bestätigt. Vorschaubilder sind visuelle Referenzen, keine messbaren GIS-Belege.
- 📋 Geheime Orte werden aus öffentlichen Spielerkarten ausgeschlossen, bleiben aber in geschützten Autorenebenen erhalten.

---

## 17. Offene Punkte

❓ Endgültige Weltfläche und Geometrie; Anzahl der Siedlungen, Stadtgrößen, Bezirke (inklusive Namen), Straßentopologie und Regionen; das akzeptierte Karten-Master; die Namen der Städte und Gewässer; Grenzen der Ostinsel; die Anzahl und Lage von Flughäfen, Sportstätten, Wasser- und Freizeitparks, Casinos und Nachtleben-Orten; Bevölkerungszahlen; Zeitskala; Zeitreise-Details (Zukunft, Verzweigung); Details zu Reichen und Untergrund; Zahl und Verteilung von Sammelobjekten und Entdeckungen. Die gesamte Übersicht steht in [Entscheidungsstatus](Entscheidungsstatus.md).
