# Ermittlung 27: Medien, soziale Medien und digitale Spuren

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul beschreibt, welche **Rolle Medien und das Smartphone** in der Ermittlung spielen: als Quelle, als Werkzeug, als Fehlerquelle und als Gefahr für die Geheimhaltung. Beschlossen sind die Grundsätze (Smartphone als diegetisches System, Feed nie erforderlich, echte und falsche Aufnahmen, kein magischer Vollzugriff). Die Mechanik ist **offen** und hier als **Entwurf** (💡) beschrieben. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Verwandt: [Modul 05 §10](05-Spuren-Tracking-Forensik-und-Zeugen.md#10-digitale-spuren), [Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md), [Modul 26](26-Notfaelle-und-Weltereignisse.md). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Entwurf: Medienarten und ihre Rolle als Quelle](#2-entwurf-medienarten-und-ihre-rolle-als-quelle)
3. [Entwurf: Zuverlässigkeit und Verzögerung](#3-entwurf-zuverlässigkeit-und-verzögerung)
4. [Entwurf: Das Smartphone als Ermittlungswerkzeug](#4-entwurf-das-smartphone-als-ermittlungswerkzeug)
5. [Entwurf: Aufnahmen von NPCs und ihre Folgen](#5-entwurf-aufnahmen-von-npcs-und-ihre-folgen)
6. [Entwurf: Echte und falsche Aufnahmen](#6-entwurf-echte-und-falsche-aufnahmen)
7. [Entwurf: Weltereignisse und Vermisstenmeldungen im Feed](#7-entwurf-weltereignisse-und-vermisstenmeldungen-im-feed)
8. [Entwurf: Suche, Register, verborgene Bereiche](#8-entwurf-suche-register-verborgene-bereiche)
9. [Entwurf: Spuren des Spielers im Netz](#9-entwurf-spuren-des-spielers-im-netz)
10. [Entwurf: Behörden und digitale Beweise](#10-entwurf-behörden-und-digitale-beweise)
11. [Sonderfälle](#11-sonderfälle)
12. [Abhängigkeiten](#12-abhängigkeiten)
13. [Ergänzungen aus der Quellenauswertung](#13-ergänzungen-aus-der-quellenauswertung)
14. [Was fehlt](#14-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| M1 | Das Smartphone ist ein breites diegetisches System (Nachrichten, Anrufe, Kamera, Karten, Browser, soziale Medien, Nachrichten, Wetter, Bank usw.) | 📋 | `M-PD`; Gameplay-Systeme §11 |
| M2 | Apps unter anderem für Navigation, Verkehr, Radarwarnungen, Wetter, Banking, Aktien, Crypto, Sportwetten, Sport, Nachrichten, soziale Medien, Messaging, Kamera, Rideshare, Lieferdienste, Shops, Immobilien, Fahrzeuge, Marketplace, Jobs | 📋 (Fragen 145–158 „ja sehr gut“, Zusammenfassung „vollständig bestätigt“) | `23#1089E`; `23#1090A` |
| M3 | Mehrere App-Anbieter; Mobilfunkabdeckung; Funklöcher in Bergen, Wäldern, Höhlen; Satellitenkommunikation als teure Outdoor-/Hunter-Lösung | 📋 | `23#1090A` |
| M4 | Der Feed ist praktisch endlos, regional gewichtet; er ist für die Hauptgeschichte und normale handgeschriebene Nebenfälle **nie erforderlich** | 📋 | `M-PD`; `M-B40` |
| M5 | Er zeigt echte World-Event-Posts, Vermisstenmeldungen, **echte und falsche** Monsteraufnahmen; Fake- und irrelevante Inhalte sind nötig, damit der Feed kein perfekter Detektor ist | 📋 | `23#1090A`; `M-PD` |
| M6 | NPC-Aufnahmen und Livestreams können digitale Beweise werden und die Geheimhaltung erschweren | 📋 | `M-PD` |
| M7 | Digitale Spuren bei schweren Ermittlungen sind möglich, aber **kein magischer Vollzugriff** | 📋 | Gameplay-Systeme §11 |
| M8 | Hidden-Web-artige Bereiche für Crime/Hunter-Recherche, aber nicht als Hackerfantasie | 📋 Richtung | Frage 157, „145-158 ja sehr gut“ |
| M9 | Eine Suchmaschine über die Weltbegriffe (Namen, Orte, Begriffe) | 📋 Richtung | Frage 158 |
| M10 | Scams im Marketplace möglich | 📋 | Frage 156 |
| M11 | Der Akku wird vereinfacht („damit man nicht permanent USB-C-Simulator spielt“) | 💡 des Assistenten, vom Entwickler mit der Gruppe bestätigt | `23#1090A` |
| M12 | Dynamische Weltereignisse aus sozialen Medien sind keine Nebenmissionen | 📋 | `M-B40` |
| M13 | Die Companion-App zeigt nur, was auch im Spiel sichtbar wäre | 📋 | `48#71E` |
| M14 | Ein virales Video bedeutet nicht automatisch, dass eine ganze Stadt an das Übernatürliche glaubt; Gerücht, Glaube, belegtes Wissen und Anerkennung sind verschieden | 💡 | `48#148A` |
| M15 | Alte analoge Quellen (Bücher, Tagebücher, Kirchenarchive, Zeitungen, Bibliotheken) bleiben wichtig | 📋 | `23#1074A` Frage 10; „10 ja genau“ |

Offen laut `M-OD`: Name, Oberfläche und Feed-Ranking des Spielnetzwerks; Architektur für generierte Beiträge; Verhältnis handgeschrieben zu generiert; Ereignisgenerator-Schema, Familien, Persistenz, Abkühlzeiten; Folgen von digitalen Beweisen und Livestreams; App-Liste und Offline-Verfügbarkeit; Datenmodell der NPC-Kommunikationsstile.

---

## 2. Entwurf: Medienarten und ihre Rolle als Quelle

> [!NOTE]
> **Nur Vorschlag (💡).** Die Medien werden hier nach ihrer **Funktion in der Ermittlung** geordnet, nicht nach Technik.

| Medium | Funktion | Zuverlässigkeit (Tendenz) | Verzögerung |
|---|---|---|---|
| **Zeitung (gedruckt, online)** | Berichte, Vermisstenanzeigen, Todesanzeigen, Archiv | mittel bis hoch; Interessen und Fehler möglich | Stunden bis Tage; Archiv dauerhaft |
| **Radio und TV** | aktuelle Meldungen, Fahndungsaufrufe, Verkehr | mittel; Eile erzeugt Fehler | kurz |
| **Websites, Foren** | Hintergründe, Hunter-Foren, Gerüchte | niedrig bis hoch, je Quelle | kurz |
| **Soziale Medien** | Gerüchte, Aufnahmen, Livestreams, Meinungen | niedrig, mit Ausnahmen | sehr kurz |
| **Messenger** | Gespräche mit NPCs; Verabredungen | direkt | sofort |
| **Behördliche Mitteilungen** | Pressemitteilungen, Fahndungsaufrufe | hoch, aber gefiltert (Vertuschung) | mittel |
| **Archive, Bibliotheken** | alte Zeitungen, Register, Bücher | hoch bei Echtheit; Lücken | Zeit zum Suchen |
| **Hunter-Netzwerk, Foren** | Hinweise anderer Hunter, Aufträge übergeben | je Person | kurz |
| **Radar-/Verkehrs-Apps** | Straßenlage | hoch | kurz |

Für Ermittlungen sind Medien zugleich **Quelle** (Informationen), **Signal** (Fälle, Modul 09) und **Gegner** (Desinformation, Vertuschung, Modul 17). Der Spieler muss mit dieser Doppelrolle umgehen. Die Rolle der Medien im Detektivmodus ist dieselbe wie in anderen Modi: Sie zeigen, **was in der Welt veröffentlicht ist**, nicht, was relevant ist (Modul 08 R3).

---

## 3. Entwurf: Zuverlässigkeit und Verzögerung

| Eigenschaft | Entwurf |
|---|---|
| **Zuverlässigkeit** | Jedes Medium und jede Quelle hat eine Zuverlässigkeitsart (Modul 10 §6): Irrtum, Lücke, Veraltung, Übertreibung, Verzerrung, Lüge, Fälschung, Interessenlage |
| **Verzögerung** | Berichte folgen Ereignissen mit Latenz (Modul 14 §4); soziale Medien schneller als Zeitungen |
| **Verzerrung durch Wiederholung** | Eine Meldung wird von Medium zu Medium übernommen und verändert (Gerüchteketten, Modul 12 §12) |
| **Korrekturen** | Berichtigungen erscheinen später; der Spieler kann sie übersehen |
| **Belegform** | Der Spieler sichert Berichte als Dokument (Zeitung, Screenshot, Aufnahme) mit Herkunft und Zeitpunkt (Modul 10 §3) |
| **Zeitpunkt** | Das Datum der Meldung zählt als Beweis (Alibi, Zeitfenster) |
| **Archivwert** | Alte Berichte zeigen Muster über Jahrzehnte (`23#1074A` Frage 79) |

---

## 4. Entwurf: Das Smartphone als Ermittlungswerkzeug

| Funktion | Nutzung für die Ermittlung | Grenzen (Entwurf) |
|---|---|---|
| **Kamera** | Spuren, Tatorte, Personen dokumentieren; Beweis mit Zeit und Ort | Qualität; Akku vereinfacht (M11); Funklöcher beeinflussen nur Übertragung |
| **Aufnahme** | Tonaufnahmen von Zeugen und Geräuschen (Beispiel `23#59A`: Stimme auf Aufnahme) | Recht (Aufzeichnung ohne Einwilligung kann Straftat sein; Modul 24 §8) |
| **Karten und Navigation** | Wege, Orte, eigene Marker | Detektivmodus: nur eigene Marker (Modul 03) |
| **Suche** | Namen, Orte, Begriffe (M9) | Welt-Inhalte; keine Lösungen |
| **Nachrichten und Anrufe** | Kontakte, Informanten, Zeugen | Beziehungen; NPC-Kommunikationsstil (Anrufe, Nachrichten, Sprachnachrichten, `23#1058A`: manche NPCs bevorzugen Anrufe) |
| **Soziale Medien** | Gerüchte, Aufnahmen, Posts | niedrige Zuverlässigkeit |
| **Marketplace** | Gebrauchtes, Sammlerstücke, Spuren zu Diebesgut | Scams (M10) |
| **Notizen** | persönliche Notizen (Modul 04, 08) | Sprachnotizen als Option |
| **Funkverbindung** | Kontakt zum HQ, zu Mitspielern | Funklöcher (M3), Satelliten |
| **Radar-/Verkehrs-App** | Straßenlage, Kontrollen | Hinweis auf Polizei (die Quelle nennt Radarwarnungen, `23#1090A`) |
| **Companion-App** | Karte, Archiv, Hilfen | außerhalb des Spiels (`48#81E`) |

### 4.1 Empfang als Spielelement

Funklöcher in Bergen, Wäldern, Höhlen (M3) machen Ermittlungen **ohne Netz** zu einer eigenen Situation: keine Suche, kein Anruf, keine Navigation; der Spieler arbeitet mit seinem Wissensstand, Karten und Notizen. Das passt zum Grundsatz, dass Freiheit und Realismus wichtiger sind als Bequemlichkeit (`23#64E`). Satellitenkommunikation ist eine teure Hunter-Lösung (M3).

---

## 5. Entwurf: Aufnahmen von NPCs und ihre Folgen

Beschlossen (M6): NPCs können filmen, fotografieren und streamen; das kann Beweise erzeugen und die Geheimhaltung erschweren. Der Entwurf behandelt Aufnahmen wie Beobachtungen ([Modul 12 §3](12-Zeugen-Verdaechtige-und-Verhoer.md#3-entwurf-beobachtungseintrag)) mit zusätzlichen Eigenschaften:

| Eigenschaft | Bedeutung |
|---|---|
| **Aufnehmer** | NPC, Kamera, Spieler |
| **Inhalt** | was aufgezeichnet wurde (mit Qualität) |
| **Zeit und Ort** | Metadaten (können fehlen oder falsch sein) |
| **Verbreitung** | wohin die Aufnahme gelangt (Plattform, Medien, Behörde) |
| **Echtheit** | echt, bearbeitet, inszeniert (Abschnitt 6) |
| **Löschung** | Aufnahmen können gelöscht werden (Besitzer, Plattform, Täter, Behörde) |
| **Reichweite** | wie viele sehen sie (Wissen der Region, Modul 17) |

Daraus entstehen Folgen: Polizei sieht ein Video (Meldekette, Modul 14), der Spieler wird erkannt (Wiedererkennung, Modul 12), Gerüchte entstehen, eine Gruppe wird aufmerksam, das Übernatürliche wird sichtbar. Ein einzelnes Video ist kein Beweis für die Existenz des Übernatürlichen (M14).

### 5.1 Der Spieler als Quelle

Der Spieler kann selbst aufnehmen, posten, löschen lassen oder Aufnahmen entfernen (Beweis zerstören, Straftat in manchen Fällen, Modul 14/15). Die Geheimhaltungsentscheidungen des Spielers (Modul 17 §5) laufen zum Teil über Aufnahmen.

---

## 6. Entwurf: Echte und falsche Aufnahmen

M5 verlangt echte **und falsche** Monsteraufnahmen sowie Fake-Inhalte. Zweck: Der Feed ist kein perfekter Detektor. Ein Entwurf der Arten:

| Art | Beispiel (erfunden) | Folge |
|---|---|---|
| **Echte Aufnahme** | Wesen auf Video | Spur; Gerücht; Behördenreaktion |
| **Echte Aufnahme, falsch gedeutet** | Tier, Mensch, Lichtreflex | Missverständnis |
| **Inszenierung** | Streich, Kostüm (Halloween: „Nicht jeder Typ im Monsterkostüm ist tatsächlich ein Monster“, `23#1090A`) | Fehlalarm |
| **Bearbeitung** | Verfälschung von Bild oder Ton | Täuschung |
| **Absichtliche Fälschung durch Wesen oder Kult** | Ablenkung oder Tarnung | Täuschung des Spielers |
| **Absichtliche Fälschung durch Behörde** | Desinformation, Vertuschung | Modul 17 |
| **Marketing/Unterhaltung** | Gruselvideos, Film | Rauschen |
| **Unklar** | nicht überprüfbar | bleibt umstritten |

Die Echtheit ist **Teil der Fallwahrheit** und für den Spieler verborgen, bis Prüfung (Forensik, Quelle, Zeugen, Archiv) sie klärt (Modul 10 §3). Ob der Spieler Bild-/Tonanalyse als Werkzeug hat, ist offen (Q-E76).

---

## 7. Entwurf: Weltereignisse und Vermisstenmeldungen im Feed

Beschlossen: Der Feed zeigt Weltereignisse (M5) und Vermisstenmeldungen (Frage 153); NPCs posten Weltereignisse (Frage 152). Dynamische Weltereignisse sind keine Nebenmissionen (M12).

| Aspekt (Entwurf) | Beschreibung |
|---|---|
| **Herkunft der Beiträge** | handgeschrieben, von NPCs aus Zuständen der Welt erzeugt, aus Medien, Werbung, Memes, Sport, Autos, Essen, Influencern (`M-PD`) |
| **Zusammenhang mit Ereignissen** | Beiträge folgen echten Weltereignissen (Modul 26); Livestreams können gerade an einem tatsächlichen Ereignis stattfinden (`23#674A` Beispiel Car-Influencer) |
| **Vermisstenmeldungen** | Signale für Fälle (Modul 09 §3–7); haben Zeitstempel, Quelle, Verlauf (gefunden, nicht gefunden) |
| **Rauschen** | viele irrelevante und falsche Beiträge (M5) |
| **Regional** | gewichtet nach Region (`M-PD`) |
| **Pflicht** | nie erforderlich (M4) |
| **Zeit** | Beiträge erscheinen und verschwinden; Archivierung durch den Spieler möglich |

Der **Generator** ist offen (`M-OD`). Der Entwurf folgt den Anforderungen der Fallgenerierung (Modul 09 §8): deterministisch, wiederholungsarm, lösbar ohne Feed.

---

## 8. Entwurf: Suche, Register, verborgene Bereiche

| Funktion | Entwurf |
|---|---|
| **Suchmaschine über die Spielwelt** (M9) | findet Namen, Orte, Unternehmen, Ereignisse, die in der Welt existieren; Ergebnis hängt vom Wissen der Welt ab (Daten, nicht Zufall) |
| **Öffentliche Register** | Grundbuch, Firmenregister, Fahrzeughalter nur soweit öffentlich (Modul 24 §8) |
| **Archive** | Zeitungsarchive, Bibliotheken; Suche kostet Spielzeit |
| **Verborgene Bereiche** (M8) | Foren und Märkte für Crime/Hunter-Recherche; Zugang über Beziehungen und Wissen; keine Hacker-Mechanik |
| **Hunter-Netzwerk** | Foren, verschlüsselte Chats (`M-PD`/Spielvision §10), Fälle melden und übergeben (Modul 07 §7) |
| **Ergebnisqualität** | Suchergebnisse haben Zuverlässigkeit (Modul 10 §6) |

**Grenzen (Entwurf):** Die Suche liefert nie die **Lösung**; sie liefert **Weltinhalte**, die der Spieler deuten muss. Der Detektivmodus verbietet automatische Schlüsse (`M-PD`), also keine „Vorschläge“ in der Suche.

---

## 9. Entwurf: Spuren des Spielers im Netz

Weil die Behörde „nicht allwissend“ ist und digitale Spuren ‚kein magischer Vollzugriff‘ sind (M7), muss entschieden werden, **welche Spuren** der Spieler hinterlässt. Katalog:

| Spur | Entsteht durch | Wer kann sie nutzen |
|---|---|---|
| Anrufe, Nachrichten | Kommunikation | Behörde (mit Rechtsgrundlage), Gegner mit Zugang |
| Ortungsdaten des Handys | Mitführen | Behörde, Gegner |
| Käufe, Konten | Bank, Marketplace | Behörde |
| Suchanfragen | Suche | offen |
| Aufnahmen, Beiträge | eigene Veröffentlichung | alle |
| Kennzeichenlesung, Kameras | Fahrzeug (Modul 14) | Behörde |
| Aufnahmen anderer | NPCs, Livestreams (M6) | alle |

**Entwurfsregel:** Nutzung digitaler Spuren durch Behörden braucht eine **Rechtsgrundlage** und **Zeit** (Verfahren, Modul 14 §8). Der Spieler kann sie mindern (Handy aus, Bargeld, Verleih, Wegwerftelefon), aber nicht beliebig. Ob Handyortung gespielt wird, ist offen (Q-E77). Der Schutz der Spielerdaten im Sinne der Datenverarbeitung außerhalb des Spiels ist ein anderes Thema und gehört nicht hierher.

---

## 10. Entwurf: Behörden und digitale Beweise

`M-OD` führt ‚digital-evidence rules‘ und ‚CCTV density/ownership/retention‘ als offen. Der Entwurf legt **Kategorien** fest:

| Beweis | Kategorien |
|---|---|
| **Kameraaufnahmen** | Eigentümer (Behörde, Unternehmen, privat), Aufbewahrungsdauer, Qualität, Zugriff |
| **Aufnahmen von Zivilisten** | freiwillig übergeben, öffentlich, erzwungen |
| **Kommunikationsdaten** | Anbieter, Anfrage, Frist |
| **Standortdaten** | Handy, Fahrzeug |
| **Finanzdaten** | Bank, Marketplace |
| **Soziale Medien** | öffentlich/privat |

Die Gewichtung vor Gericht folgt Modul 15 §5; Fälschungen sind möglich (Formwandler, Bearbeitung, Modul 13).

---

## 11. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Handy verloren/gestohlen | – | Zugang zu Kontakten, Nachrichten; Wiederbeschaffung |
| Kein Empfang im Fall | – | Wissensstand genügt; Funklöcher als Spielraum (Abschnitt 4.1) |
| Beitrag über den Spieler | NPC postet | Ruf (Modul 13); Fahndung (Modul 14) |
| Viral-Fälschung | – | Gerücht; Behörde reagiert |
| Livestream eines Verbrechens | – | Zeugen weltweit; schnelle Meldung; Folgen |
| Tarnidentität im Netz | – | falsche Konten (Modul 13 §10) |
| Hunter-Forum-Eintrag | Spieler gibt Fall weiter | Fall wird ‚Fremd bearbeitet‘ (Modul 09) |
| Koop | mehrere Handys | jeder Charakter hat eigenes; Teamakte teilt (Modul 18) |
| Mods | Apps, Plattformen | Daten/Mod-API |
| Schutz Minderjähriger | – | nicht in den Quellen; Adult 09 für Darstellungen |
| Offline-Spiel | – | alle Inhalte der Welt sind lokal vorhanden; Online-Funktionen offen |

---

## 12. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Medien als Belege, Zuverlässigkeit |
| Geheimhaltung ([Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md)) | öffentliches Wissen, Vertuschung |
| Ereignisse ([Modul 26](26-Notfaelle-und-Weltereignisse.md)) | Inhalte des Feeds |
| Fallmodell ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) | Signale |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Aufnahmen als Beobachtungen, Gerüchte |
| Identität ([Modul 13](13-Identitaet-Ruf-und-Formwandler.md)) | Wiedererkennung, Fälschungen |
| Polizei ([Modul 14](14-Polizei-Behoerden-und-Fahndung.md)) | digitale Beweise |
| Detektivpfad ([Modul 24](24-Privatdetektiv-Laufbahn.md)) | Werkzeuge, rechtliche Grenzen |
| Companion-App | Anzeige außerhalb des Spiels |
| Wirtschaft | Marketplace, Scams |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Beiträge, Aufnahmen als Zustand (Größe!) |
| Hilfen ([Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md)) | keine Lösungen im Feed oder in der Suche |

---

## 13. Ergänzungen aus der Quellenauswertung

### Der Feed als Weltinformationssystem

Aus dem Vorschlag des Assistenten (`23#622A`, 30.08.2026; Entwickler-Vorbehalt `23#623E`: kein Social-Media-Zwang für die Story, nur für optionale Weltereignisse, die keine Nebenmissionen sind):

| Element | Inhalt (💡) |
|---|---|
| Quellen des Feeds | handgeschriebene wichtige Beiträge; NPC-Beiträge aus Tagesabläufen und Weltzuständen; lokale Nachrichten; Werbung, Memes, Sport, Autos, Restaurants, Influencer, Beschwerden, Fotos; Beiträge über dynamische Unfälle, Verbrechen, Wetter, Tierereignisse; Gerüchte und Falschmeldungen; prozedural erzeugte verdächtige Beobachtungen; Reaktionen, Kommentare, Reposts, Folgeposts |
| Regionalität | Der Feed gewichtet nach aktueller Region, Heimatregion, Freunden, gefolgten Konten, letzten Orten, Trends, Weltereignissen |
| NPC-Konten | wichtige und viele mittlere NPCs besitzen Konten mit Beitragsstil, Interessen, Freunden, Privatsphäre, Aktivität; verknüpft mit den Kommunikationsstilen der NPCs |
| Fake-Inhalte | „Pflicht“, sonst wäre der Feed ein Monster-Radar |
| Live-Beiträge | Livestreams sind tatsächlich live; der Spieler kann hinfahren; das Ereignis kann vorbei sein; keine Nebenmission (`23#628A` Punkt 8, 💡) |
| Rückwirkung | Folgeposts nach der Lösung („Update: Sie haben ihn gefunden“); Videos bei öffentlicher Tötung eines Monsters; Löschungen; Behördenerklärungen; Verschwörungstheorien |

Der Assistent bewertete zusätzlich in `23#618A`: Punkt 26 „Social Media als echte Open-World-Informationsquelle“ (Beispielpost mit verwackeltem Video; der Spieler muss unterscheiden), Punkt 27 ‚NPCs können eigene Fotos und Videos vom Spieler machen‘ (kann der Polizei helfen, online auftauchen, das Verbergen erschweren), Punkt 28 unterschiedliche Kommunikationsstile. Die Antwort des Entwicklers (`23#619E`): „26 ja aber nicht in missionen verwickeln sondern einfach nur wenn man ohne irgendwas helfen will“; zu 28 stellte er eine Rückfrage zur Formulierung („hasst telefonieren und ruft lieber an“, gemeint wohl „hasst Schreiben“); „Die anderen finde ixh gut“. Status: 📋 für Punkt 26 (Hilfe nur freiwillig, nicht als Missionsverstrickung), 📋 Richtung für 27 und 28.

### Das Spiel-Internet

Der Entwickler (`23#1011E` Punkt 28): Es soll im HQ oder anderswo einen Laptop oder PC geben, an dem man „das komplette internet durchschauen kann, wie in echt auch“ (mit Anbietern „bis zu 50gbit“ an bestimmten Adressen). Der Assistent (`23#1012A`): Zugriff an HQ-PC, Laptop, eigenen Immobilien, Bibliotheken, Büros, eventuell Internetcafés, später teilweise Smartphone; Inhalt: Suchmaschine, Nachrichtenseiten, Firmenwebsites, Banken, Broker, Crypto, Wettanbieter, Shops, Gebrauchtwagenportale, Immobilien, Jobportale, Foren, Social Media, lokale Community-Seiten, Behördeninformationen, Wetter, Verkehr, Sport, Streaming, „Hunter-relevante Archive, alte Foren und obscure Websites“; „kein buchstäblich vollständiges echtes Internet“, sondern ein großes fiktives Netz; „Gerade für Ermittlungen ist das extrem wertvoll.“ Zusätzlich (`23#1024A`): große Daten- und Recherche-Downloads (Hunter-Datenbanken, CCTV-Dumps, Polizeiscans, historische Archive, Karten- und Satellitendaten) als Bedarf an Telekom und Internetverträgen. Der Entwickler sagte, der Tarif sei „nicht nur zur deko, er bringt wirklich was, was genau müssen wir noch aufstellen, weil das nur was mit viel download was bringt“ (`23#1023E` Punkt 42). Status: Wunsch 📋 (Arbeitsplatz im HQ), Ausformulierung 💡; Produktionsklasse laut `23#1012A` P1 („In-Game-Internet“), „hunderte interaktive Websites mit jeweils eigenen Systemen“ P3.

### Karte in der Begleit-App

Die interaktive Begleit-Karte zeigt nur, was der Spieler in seinem Spielstand hat (`23#691E`, siehe [Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md#113-die-interaktive-karte-der-begleit-app)).

---

## 14. Was fehlt
- **Spielnetzwerk**: Name, Oberfläche, Ranking, Moderationsregeln (`M-OD`).
- **Generator** für Beiträge und Ereignisse; Anteil handgeschrieben (`M-OD`).
- **Bild- und Tonanalyse** als Werkzeug (Q-E76).
- **Handyortung** und Spuren des Spielers (Q-E77).
- **App-Liste** und Offline-Verfügbarkeit (`M-OD`).
- **Datenmenge**: Beiträge und Aufnahmen dürfen den Weltzustand nicht sprengen ([Modul 19 §4](19-Weltzustand-und-Persistenz.md#4-entwurf-was-nicht-hineingehört)).

## Ergänzung aus dem Entscheidungsabgleich 31.08. (Stand 09.11.2026)

- 📋 **NPC-Aufnahmen (Entscheidung 46):** NPCs können außergewöhnliches Verhalten fotografieren, filmen, live streamen oder posten. Das beeinflusst Polizeibeweise, soziale Medien und die Geheimhaltung des Übernatürlichen.
- 📋 **Keine universelle Telefonzugriff der Behörden (Entscheidung 46):** Digitale Beweise sind in ernsthaften Ermittlungen unter plausiblen rechtlichen und technischen Bedingungen verfügbar. Behörden haben keinen magischen Zugriff auf jedes Telefon.
