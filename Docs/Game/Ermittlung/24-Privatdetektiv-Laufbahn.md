# Ermittlung 24: Privatdetektiv-Laufbahn

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Dieses Modul vertieft [Modul 07](07-Detektivlaufbahn-und-organische-Faelle.md) für den Teil **Beruf**: Lizenz, Büro, Kunden, Aufträge, Verdienst, Ruf und Wechselwirkung mit dem Hunter-Leben. Die Quellen enthalten dazu **wenig** (eine Aufzählung des Assistenten, vom Entwickler mit „ja passt“ bestätigt). Der größte Teil dieses Moduls ist deshalb **Entwurf** (💡); beschlossene Aussagen stehen in Abschnitt 1. Das Modul ist als Gedankenmodell zu lesen. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Ermittlungs-Dokumentation ([Index](README.md)). Register: [Modul 20](20-Entscheidungsregister.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Priorität und Umfang](#2-priorität-und-umfang)
3. [Entwurf: Stufen des Berufs](#3-entwurf-stufen-des-berufs)
4. [Entwurf: Lizenz](#4-entwurf-lizenz)
5. [Entwurf: Aufträge und Auftragsarten](#5-entwurf-aufträge-und-auftragsarten)
6. [Entwurf: Zustandsmaschine eines Kundenauftrags](#6-entwurf-zustandsmaschine-eines-kundenauftrags)
7. [Entwurf: Honorar, Kosten und Verdienst](#7-entwurf-honorar-kosten-und-verdienst)
8. [Entwurf: Rechtliche Grenzen der Ermittlungsarbeit](#8-entwurf-rechtliche-grenzen-der-ermittlungsarbeit)
9. [Entwurf: Büro, Akten, Ausrüstung](#9-entwurf-büro-akten-ausrüstung)
10. [Entwurf: Ruf und Kundschaft](#10-entwurf-ruf-und-kundschaft)
11. [Entwurf: Zusammenspiel mit dem Hunter-Leben](#11-entwurf-zusammenspiel-mit-dem-hunter-leben)
12. [Sonderfälle](#12-sonderfälle)
13. [Abhängigkeiten](#13-abhängigkeiten)
14. [Ergänzungen aus der Quellenauswertung](#14-ergänzungen-aus-der-quellenauswertung)
15. [Was fehlt](#15-was-fehlt)

---

## 1. Was beschlossen ist

| Nr. | Aussage | Status | Quelle |
|---|---|:---:|---|
| D1 | Detektiv ist ein großer optionaler Karriere- und Spielpfad im Supernatural-Spiel, wegen der gewünschten Freiheit | 📋 | `23#1061E`: „detektiv können wir noch in supernatural reingeben, da ich sowieso gesagt habe ich will dass der spiele viel freiheit hat, also ist detektiv gut“ |
| D2 | Der Spieler kann eine PI-Lizenz oder entsprechende fiktive Zulassung erhalten | 📋 Wunsch | `23#1062A`, bestätigt `23#1063E` („ja passt“); Spielvision §7.1 |
| D3 | Eigenes Detective Office oder Arbeit von zu Hause/HQ; Kunden annehmen; Preise verlangen; Ruf aufbauen; bessere Aufträge; Ausrüstung kaufen; Fälle archivieren | 💡 (Aufzählung des Assistenten), vom Entwickler pauschal bestätigt | `23#1062A`; `23#1063E` |
| D4 | Mitarbeiter einstellen: „deutlich später“ | 💡 | `23#1062A` |
| D5 | Eine weitere legale Geldquelle neben Jobs, Aktien, Crypto, Sportwetten, Motorsport und anderen Aktivitäten | 💡 | `23#1062A` |
| D6 | Normale Detektivfälle: vermisste Personen, Beziehungs- und Ehefälle, Stalking, Betrug, Diebstahl, Erpressung, Cold Cases usw.; Werkzeuge: Befragung, Alibis, Observation, Verfolgung, Aufnahmen, Archive | 💡 / 📋 teilweise | `23#1062A`; Spielvision §7.1 |
| D7 | Kleinere prozedural unterstützte Aufträge und hochwertige handgebaute Fälle | 🧭 | `23#1062A` |
| D8 | Fälle können scheitern, falsch oder zu früh abgeschlossen werden; die Welt läuft weiter | 📋 | `23#1062A` |
| D9 | Ein gewöhnlicher Auftrag kann sich organisch als Hunter-Fall erweisen, aber selten | 📋 Richtung | `23#1062A`; `23#1063E` |
| D10 | Der Spieler kann viele Systeme und Lebensstile gleichzeitig verfolgen; keine künstlichen Klassensperren | 📋 | `M-CA0905` Abschnitt 4 |
| D11 | Firmenjobs sind optional und nicht die Hauptgeschichte | 📋 | Gameplay-Systeme §13 |
| D12 | Wirtschaftsdetails (Gesamtbalance, Dienstleistungspreise, Crafting) sind offen | ❓ | `M-OD` ‚Economy / cash / shops‘ |

---

## 2. Priorität und Umfang

Block 10P (`23#1088A` Fragen 182–184; Antwort „182-184 ja“, `23#1089E`) führt „eigene Unternehmen“ unter **P3 / langfristig**; Investigation/Forensics gehört zu **P1 / Kern**. Das ergibt eine natürliche Trennung:

| Teil | Priorität (aus der Zuordnung abgeleitet, Entwurf) |
|---|---|
| **Ermittlungsarbeit selbst** (Fälle, Beweise, Zeugen) | Kern (P1) |
| **Gewöhnliche Detektivfälle als Inhalt** (nicht-übernatürliche Fälle) | Kern oder früh; Quelle sagt nur ‚Fall-Klasse‘, keine Phase |
| **Lizenz, Büro, Kunden, Honorar** | eher spät; „eigene Unternehmen“ ist P3 |
| **Mitarbeiter** | später als das Büro (D4) |

Der Beruf ist damit ein **Aufsatz** auf die Ermittlung, nicht deren Voraussetzung: Die Fallklassen (gewöhnlich, mehrdeutig, übernatürlich) funktionieren auch ohne Lizenz, wenn Aufträge über Kontakte, Zeitungen oder Zufall kommen.

---

## 3. Entwurf: Stufen des Berufs

> [!NOTE]
> **Nur Vorschlag (💡).** Stufen, Namen und Inhalte sind nicht beschlossen.

| Stufe (Arbeitsname) | Inhalt | Voraussetzung (Vorschlag) |
|---|---|---|
| **0 Gelegenheitsermittler** | einzelne Aufträge von Bekannten und aus Zeitungen; keine Lizenz; rechtlich eingeschränkt | – |
| **1 Lizenzierter Ermittler** | Lizenz; legale Auftragsannahme; Zugang zu bestimmten Registern | Lizenzverfahren (Abschnitt 4) |
| **2 Eigenes Büro** | Büro oder Arbeitsplatz; Kundenempfang; Akten | Miete, Ausstattung |
| **3 Etablierte Adresse** | Ruf bringt bessere Aufträge; Empfehlungen; Verträge mit Anwälten, Versicherungen | Ruf (Abschnitt 10) |
| **4 Mit Mitarbeitern** | Mitarbeiter bearbeiten Routineaufträge; „deutlich später“ (D4) | Ausbau; Personalkosten |

Jede Stufe ist **optional**; der Spieler kann auf jeder bleiben oder aufhören (D10). Der Beruf wird nie zur Pflicht (Oberste Regel, E-FA-019).

---

## 4. Entwurf: Lizenz

Die Quelle spricht von „PI-Lizenz bzw. entsprechende fiktive Zulassung“ (`23#1062A`). Die Spielwelt ist eine fiktive US-Welt mit fiktiven Bundesstaaten (`35#533A`; Welt 07). Die Voraussetzungen sind **nicht festgelegt**; ein Entwurf möglicher Bestandteile:

| Bestandteil | Beispiel (erfunden) | Zusammenhang |
|---|---|---|
| **Antrag** | Behörde des Bundesstaates | Zuständigkeit je Staat (Modul 14 §3) |
| **Voraussetzungen** | Mindestalter, Erfahrung, Prüfung | Spielstand-Bedingungen |
| **Hintergrundprüfung** | Vorstrafen, Fahndung | Vorstrafen wirken (Modul 15 §9) |
| **Kosten** | Gebühr, Versicherung | Wirtschaft |
| **Geltung** | gilt im Bundesstaat; andere Staaten brauchen Anerkennung | fiktive Staaten |
| **Entzug** | bei Verstößen, Vorstrafen, Beschwerden | Rechtssystem |
| **Waffenerlaubnis** | getrennt | Waffenrecht (nicht in den Quellen) |

Ob die Lizenz **zwingend** für bestimmte Handlungen ist (zum Beispiel Zugang zu Registern, Kundenverträge) oder nur Vorteile bringt, ist offen (❓ Q-E72). Die **Fahndung** gegen den Spieler (Modul 14) und die Lizenz stehen in einem Spannungsverhältnis: Ein gesuchter Spieler kann keine Lizenz erhalten oder behalten (Entwurf).

---

## 5. Entwurf: Aufträge und Auftragsarten

Die Quelle nennt die Auftragsarten als Liste von Fällen (D6). Ein Entwurf ordnet sie nach **dem, was der Kunde braucht**:

| Auftragsart (Gruppe) | Beispiele aus `23#1062A` | Typischer Ablauf | Wissensquellen (Modul 10) |
|---|---|---|---|
| **Personensuche** | vermisste Personen, vermisste Fahrzeuge, verschwundene Gegenstände | Befragen, Spuren, Register, Observation | Zeugen, Dokumente, Spuren |
| **Beziehungs- und Beobachtungsaufträge** | Ehe-/Beziehungsfälle, Stalking, Mitarbeiterüberwachung | Observation, Verfolgung, Aufnahmen | Beobachtung, Medien |
| **Wirtschaft und Betrug** | Betrug, Versicherungsbetrug, Unternehmensdiebstahl, Identitätsbetrug, Vermögensermittlungen, Background Checks | Register, Finanzdaten, Befragung | Dokumente, digitale Spuren |
| **Straftaten** | Diebstahl, Einbruch, Erpressung, Brandstiftung, Entführung, Mord, Cold Cases | Tatort, Forensik, Zeugen | Spuren, Labor, Archiv |
| **Organisierte Kriminalität und Korruption** | organisierte Kriminalität, Korruption, Cybercrime | Infiltration, Informanten | Zeugen, Dokumente, Informanten |
| **Mehrdeutige Aufträge** | zunächst unklar | Ermittlung entscheidet | alle |

Ein Auftrag trägt (Katalog): Kunde, Ziel, Frist, Honorar, rechtliche Grenzen, Risiken, Vertraulichkeit, Beweisanforderungen. Das **Ziel** des Auftrags und die **Wahrheit** können abweichen: Der Kunde kann lügen (Auftrag ist Täuschung), das Ziel kann unschuldig sein (Folgen für den Ermittler), der Fall kann etwas ganz anderes sein (D9).

---

## 6. Entwurf: Zustandsmaschine eines Kundenauftrags

> [!NOTE]
> **Nur Vorschlag (💡).** Ein Kundenauftrag ist eine Herkunftsart des Falls ([Modul 09 §3](09-Falllebenszyklus-und-Generierung.md#3-entwurf-herkunftstypen-eines-falls)); er folgt der Fall-Zustandsmaschine und fügt Kundenzustände hinzu.

```text
[Anfrage] -> [Angebot] -> [Vertrag] -> [In Arbeit] -> [Bericht] -> [Abgenommen] -> [Honorar]
     |           |            |            |             |               |
 [Abgelehnt] [Abgelehnt] [Gekündigt]   [Eskaliert]   [Beanstandet]  [Streit]
                                          (z. B. wird Hunter-Fall)
```

| Zustand | Bedeutung |
|---|---|
| **Anfrage** | Kunde meldet sich (Anruf, Brief, Besuch) |
| **Angebot** | Spieler nennt Preis und Bedingungen |
| **Vertrag** | Auftrag angenommen; Frist, Honorar, Umfang |
| **In Arbeit** | Ermittlung (Fall-Zustände aus Modul 09) |
| **Bericht** | Ergebnis wird dem Kunden dargestellt (zeigt, was der Spieler herausgefunden hat) |
| **Abgenommen** | Kunde akzeptiert |
| **Honorar** | Bezahlung |
| **Gekündigt** | Kunde bricht ab |
| **Eskaliert** | Auftrag wird Hunter-Fall oder gefährlich |
| **Beanstandet / Streit** | Kunde zweifelt oder verweigert Zahlung |

Der **Bericht** ist eine Verantwortung: Er legt fest, was der Kunde erfährt. Der Spieler kann ehrlich berichten, verschweigen oder lügen (zum Beispiel wenn der Auftrag zu Übernatürlichem führte; Geheimhaltung, [Modul 17](17-Geheimhaltung-und-oeffentliches-Wissen.md)). Die Folgen laufen über Ruf (Abschnitt 10) und Recht (Abschnitt 8).

---

## 7. Entwurf: Honorar, Kosten und Verdienst

Die Quellen nennen **keine** Preise (`M-OD`: ‚Exact overall single-player economy balance‘, ‚service pricing‘). Der Entwurf legt nur **Kategorien** fest:

| Kategorie | Beispiel |
|---|---|
| **Einnahmen** | Honorar je Auftrag, Pauschalen, Erfolgsprämie |
| **Ausgaben** | Büro, Versicherung, Ausrüstung, Reisen, Informanten (Modul 12), Laborgebühren (Modul 11), Lizenzgebühr |
| **Risiko** | Zahlungsausfall, Streit, Klage |
| **Zeitaufwand** | Dauer je Auftrag; Konkurrenz mit Hunter-Aufgaben |

**Grundsatz (Entwurf):** Der Detektivverdienst ist **eine** legale Quelle neben anderen (D5); er soll nicht so lohnend sein, dass er das Hunter-Spiel verdrängt, und nicht so schwach, dass er sinnlos wäre. Konkrete Werte hängen von der Gesamtwirtschaft ab (D12).

---

## 8. Entwurf: Rechtliche Grenzen der Ermittlungsarbeit

Weil ‚Gesetz folgt tatsächlicher Information‘ (P1 in Modul 14), gelten für den Spieler als Ermittler rechtliche Grenzen. Die Quellen nennen als Werkzeuge: Befragungen, Observation, Verfolgung, Fotos und Videos, **Abhören**, Überwachungskameras, Internetrecherche, öffentliche Register, soziale Medien, Informanten, Archive, Drohnen, GPS- und Überwachungstechnik „soweit für das Setting sinnvoll“ (`23#1062A`; Spielvision §7.1). Ein Entwurf, welche **rechtlichen Kategorien** das System unterscheidet:

| Handlung | Rechtlich (Entwurf) | Folge bei Verstoß |
|---|---|---|
| Öffentliche Orte beobachten, fotografieren | meist erlaubt | – |
| Dem Ziel folgen | erlaubt, bis zu Belästigung/Stalking | Anzeige, Lizenzverlust |
| Befragen | erlaubt; Täuschung über die Rolle kann Straftat sein | Anzeige |
| Register einsehen | öffentliche Register erlaubt; geschützte nur mit Lizenz oder Zugang | Zugangsentzug |
| Einbruch, Hausfriedensbruch | verboten | Straftat (Modul 14) |
| Abhören, Wanzen | meist verboten oder nur mit Genehmigung | Straftat, Beweise unzulässig (Modul 15 §5.3) |
| Drohnen, GPS-Tracker | eingeschränkt | Straftat oder Ordnungswidrigkeit |
| Beweise stehlen | verboten | Straftat |
| Zeugen bedrohen oder bestechen | verboten | Straftat (Modul 12 §11) |
| Falsche Identität | eingeschränkt | Straftat bei Betrug |

Die Spalte „Rechtlich“ ist **Entwurf in Anlehnung an reale Muster**, keine Rechtsauskunft und kein Beschluss. Die Quelle verlangt nur, dass der Spieler als Hunter und Ermittler Dinge **darf**, die legal sind, und andere mit Konsequenzen tun **kann** (Kriminalität ist für den Spieler möglich, Punkt 55). Der Spielwert liegt im Dilemma: Illegale Beschaffung hilft dem Fall, schadet aber dem Verfahren und der Lizenz.

---

## 9. Entwurf: Büro, Akten, Ausrüstung

| Element | Entwurf |
|---|---|
| **Büro** | Ort in der Welt (Immobilie, Miete oder Kauf; Immobilien sind beschlossen, Spielwelt-und-Untergrund §12); Empfang, Akten, Schreibtisch |
| **Akten** | archivierte Fälle (D3); Teil des Journals (Modul 04), mit Kundendaten; Vertraulichkeit |
| **Ausrüstung** | Kamera, Aufnahmegerät, Fernglas, Fahrzeug, Laptop, Telefon; Transport begrenzt (Inventar, `M-CA0905`); Fahrzeug als Arbeitsplatz |
| **Sicherheit** | Büro kann Ziel von Einbruch oder Durchsuchung sein (Modul 14 §8) |
| **Verbindung zum HQ** | Arbeit von zu Hause oder HQ möglich (D3); das Büro kann ein Tarnort oder Treffpunkt für Hunter sein |
| **Mitarbeiter** | später (D4) |

Die Akten sind **kein neuer Persistenzbereich**, sondern die Kundenseite der Fallakte; sie gehören zur Archivierung ([Modul 19 §4](19-Weltzustand-und-Persistenz.md#4-entwurf-was-nicht-hineingehört)).

---

## 10. Entwurf: Ruf und Kundschaft

Der Ruf als Ermittler ist ein Ruf einer **Rolle** (Modul 13 §2): eigener Ruf-Datensatz, getrennt vom Hunter-Ruf. Eingaben: Erfolg, Diskretion, Verlässlichkeit, Honorar, Beschwerden, Skandale.

| Wirkung | Beispiel |
|---|---|
| bessere Aufträge | mehr Anfragen, höhere Honorare |
| Zugang | Anwälte, Versicherungen, Polizeikontakte |
| Risiko | Konkurrenten, Gegner der Klienten |
| Verknüpfung | ein schlechter Ruf als Ermittler kann auf die Identität zurückfallen (Rufe sind getrennt, aber durch Wissen verknüpfbar) |

Kunden sind NPCs (Tier A bis C) mit Gedächtnis (Modul 12). Wer einen Kunden verrät, wird erinnert.

---

## 11. Entwurf: Zusammenspiel mit dem Hunter-Leben

| Frage | Vorschlag |
|---|---|
| **Zeit** | Detektivaufträge und Hunter-Fälle konkurrieren um Spielzeit; Fristen können verfallen (Modul 09); Schutzstufen (Modul 09 §9) |
| **Wechsel** | Ein Auftrag kann Hunter-Fall werden (D9): Der Kunde bleibt Auftraggeber; Geheimhaltung gegenüber dem Kunden (Modul 17) |
| **Rolle** | Hunter-Identität und Ermittler-Rolle sind verschiedene Rollen (Modul 13); beide können getrennt oder verbunden genutzt werden |
| **Tarnung** | Die Ermittlerrolle kann ein Vorwand für Hunter-Arbeit sein (Zugang zu Tatorten, Zeugen) |
| **Beweise** | Beweise aus Hunter-Arbeit sind vor Gericht nur verwendbar, wenn legal erlangt und ohne Verrat der Geheimhaltung |
| **Geld** | Honorar als Einkommen; Hunter-Ausrüstung teuer; Kopplung offen |
| **Schwierigkeit** | Der Detektivmodus (Modul 03) gilt für alle Fälle, auch Kundenaufträge |

---

## 12. Sonderfälle

| Sonderfall | Beschreibung | Vorschlag |
|---|---|---|
| Kunde ist der Täter | – | Auftrag ist Täuschung; Wahrheit tritt spät hervor |
| Kunde ist ein Wesen | – | Auftrag ist Falle oder Hilfe; Modul 13 |
| Auftrag widerspricht der Hunter-Geheimhaltung | – | Spielerentscheidung (Bericht) |
| Ziel stirbt während des Auftrags | – | Fall bleibt lösbar oder scheitert (V3) |
| Konkurrenz mit Polizei | Polizei ermittelt im selben Fall | Zusammenarbeit oder Behinderung (Modul 14) |
| Klage des Ziels | – | Verfahren (Modul 15) |
| Lizenz entzogen | – | Aufträge gesperrt; Arbeit als Gelegenheitsermittler weiter möglich |
| Koop | mehrere Hunter führen ein Büro | Büro gehört zum Host-Spielstand; Aufträge als Teamfälle (Modul 18) |
| Spieler wird verhaftet | Kundenaufträge pausieren | Schutzstufe S0 (Modul 09 §9.3), Kunden verlieren Geduld |
| Mods | zusätzliche Auftragsarten | Datenregistrierung (Modul 19 §11) |

---

## 13. Abhängigkeiten

| System | Abhängigkeit |
|---|---|
| Fallmodell ([Modul 09](09-Falllebenszyklus-und-Generierung.md)) | Auftrag als Herkunftsart |
| Beweislogik ([Modul 10](10-Beweislogik-und-Wissensmodell.md)) | Bericht als Beleg-Ausgabe |
| Zeugen ([Modul 12](12-Zeugen-Verdaechtige-und-Verhoer.md)) | Befragung, Informanten |
| Identität ([Modul 13](13-Identitaet-Ruf-und-Formwandler.md)) | Rollen, Ruf |
| Recht ([Module 14](14-Polizei-Behoerden-und-Fahndung.md), [15](15-Gerichte-und-Verfahren.md)) | Lizenz, Grenzen, Vorstrafen |
| Wirtschaft | Honorar, Kosten, Immobilien |
| Hilfen ([Modul 08](08-Ermittlungsfreiheit-und-Einstellungssystem.md)) | Detektivmodus gilt auch hier |
| Weltzustand ([Modul 19](19-Weltzustand-und-Persistenz.md)) | Aufträge, Akten |
| Spielzeit | Fristen |

---

## 14. Ergänzungen aus der Quellenauswertung

### Firmen als Auftraggeber

Der Entwurf des Weltblocks 2 fragte (`23#1010A` Frage 31), ob der Spieler Unternehmen als Auftraggeber erleben soll, „Lieferfahrten, Sicherheit, Ermittlungen, Transport, Reparaturen, spezielle Hunter-Fälle“. Der Entwickler: „ja aber nur als job um geld zu verdienen, die hauptstory würde ich dort nicht machen, auch keine nebenmissionen, da das nichts mit jagen zu tun hat“ (`23#1011E` Punkt 31). Der Assistent hielt fest, normale Firmenjobs dienten „primär dem Geldverdienen“ (`23#1012A`). Status: 📋 für Firmenjobs; für **Ermittlungsaufträge von Firmen** gilt damit: möglich als Geldquelle, nicht als Haupt- oder Nebenmission. Das verträgt sich mit dem Detektivpfad (Abschnitt 1) und schränkt ein, dass Kundenaufträge die Hunter-Story ersetzen.

### Einfluss auf das Internet im Spiel

Der Entwickler wünschte (`23#1011E` Punkt 28), dass es „im hq oder auch wo anders einen laptop/pc“ gibt, an dem man „das komplette internet durchschauen kann, wie in echt auch“. Der Assistent verwarf ein buchstäblich vollständiges Internet zugunsten eines großen fiktiven Netzes (`23#1012A`, siehe Modul 27). Für den Detektivpfad bedeutet das einen **Arbeitsplatz** für Recherche.

---

## 15. Was fehlt
- **Lizenzvoraussetzungen** (Q-E72).
- **Preise, Honorare**, Wirtschaftsbalance (`M-OD`).
- **Rechtliche Grenzen** als Beschluss (Q-E73).
- **Büro und Immobilien**: Ausgestaltung.
- **Mitarbeiter**: später (D4).
- **Fallkatalog** für Kundenaufträge (nichts im Master).
- **Dynamische Erzeugung** von Kundenaufträgen.
