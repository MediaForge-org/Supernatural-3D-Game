# Adult 08: Welt, Orte, Branche und Wirtschaft

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Planungsstand zur Einbettung in die Spielwelt. Orte, Anzahlen und Preise sind Richtwerte des Entwicklers, keine Kartenfestlegung. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Die Weltgeometrie ist offen (siehe [Spielwelt und Untergrund](../Spielwelt-und-Untergrund.md)); hier steht nur, wie das System sich in eine Welt einfügt.

**Inhalt**

1. [Orte und Privatsphäre](#1-orte-und-privatsphäre)
2. [Beobachter, Beitreten, Zeugen](#2-beobachter-beitreten-zeugen)
3. [Weltregeln statt Sonderregeln](#3-weltregeln-statt-sonderregeln)
4. [Die Erwachsenen-Branche als Weltschicht](#4-die-erwachsenen-branche-als-weltschicht)
5. [Dienstleistende NPCs und Verträge](#5-dienstleistende-npcs-und-verträge)
6. [Spielwelt-Internet und Medien](#6-spielwelt-internet-und-medien)
7. [Gegenstände](#7-gegenstände)
8. [Offene Punkte](#8-offene-punkte)

---

## 1. Orte und Privatsphäre

- 📋 **Fast überall**, solange Ort und Kontext es erlauben: Wohnung, Hotel, Fahrzeug, abgelegene Außenbereiche, Arbeitsplatz, Party, Unterschlupf. Ein Bett oder Sofa braucht **keinen speziellen Anker**; Oberflächen werden aus Geometrie, Kollision und Material erkannt. Die Szene beginnt nicht zwingend am Ort der Verabredung ([Modul 03](03-Interaktion-Taxonomie-Ablauf.md#5-eigenschaften-des-ablaufs)).
- 📋 **Orts-Eigenschaften:** Privatsphäre, Sichtbarkeit, Schalldämmung, Raumgröße, Möbel, Temperatur, Sauberkeit, Zugänglichkeit, Wahrscheinlichkeit, dass jemand auftaucht. Der Spieler sieht sie nicht als Rohwerte.
- 📋 **Privatsphäre wird tatsächlich simuliert** (Türen, Fenster, Sichtlinien, NPCs, Entfernung, Audio-Ausbreitung), nicht als Schalter. Es gibt eine Absicht („Privatsphäre gewünscht“) und eine tatsächliche Entdeckungswahrscheinlichkeit.
- 📋 Figuren verhalten sich **ortsabhängig**: Manche mögen Öffentlichkeit oder Risiko, andere nur Privates; der Entwickler nannte „schamlose“ Figuren, die auch vor Zuschauern handeln würden. Jeder Ort hat eigene Regeln und Wahrscheinlichkeiten (Privatsphäre, Sichtbarkeit, NPC-Dichte, Tageszeit, Zugang, ob Zuschauen oder Beitreten möglich ist, ob andere NPCs reagieren, ob Personal oder Sicherheit eingreift, ob der Ort dafür gedacht ist).
- 📋 **Orte merken sich** Zustände und Vorfälle (Eigentümer, Zugang, Kameras, bekannte NPCs, Beschwerden).

---

## 2. Beobachter, Beitreten, Zeugen

- 📋 **Beobachter** sind eine echte Rolle mit eigener KI und Gedächtnis (zufällig gesehen, absichtlich zugeschaut, eingeladen). Sie können ignorieren, neugierig schauen, bleiben, weggehen, Freunde holen, Sicherheitsdienst oder Polizei informieren oder später darüber reden.
- 📋 **Beitreten:** Ein Beobachter kann eingeladen werden oder spontan Interesse zeigen. Er wechselt nur in eine aktive Rolle, wenn **alle** Beteiligten (Zulassung, Entscheidungsfähigkeit, Präferenzen, Zustimmung) es zulassen ([Modul 01](01-Grundregeln-Alter-Zustimmung.md)). Umgekehrt kann jemand die Situation verlassen, und die anderen machen weiter, sofern die Konstellation unterstützt wird.
- 📋 **Zeugen** entstehen aus Sichtlinie, Entfernung, Räumen, Türen, Wänden, Schallausbreitung, Licht und Sichtbarkeit.
- 📋 Mehrere Interessenten, Gerüchte unter NPCs und Folgen bei Entdeckung gehören dazu.
- 📋 Auch **NPC-zu-NPC**-Situationen in der Welt sind vorgesehen ([Modul 05](05-NPCs-Beziehungen-Praeferenzen.md#5-initiative-und-spontane-begegnungen)).

---

## 3. Weltregeln statt Sonderregeln

- 📋 Wo Polizei, Sicherheitsdienst oder NPCs reagieren, hängt von **Ort und Kontext** ab (privater Club ≠ Park), nicht vom Erwachsenen-System selbst. Dafür sind allgemeine Welt- und Anstandsregeln der Orte zuständig, die das Paket nur abfragt.
- 📋 Öffentlich sichtbares Verhalten kann durch Zeugen echte Folgen haben; sie laufen über die allgemeinen Welt-, Rechts- und Reputationssysteme.
- 📋 Das Rechtssystem der Spielwelt ist **fiktiv** und kann je Zuständigkeitsbereich unterschiedlich sein (zum Beispiel ob eine Branche erlaubt, geduldet oder verboten ist). Wird etwas verboten, kann es illegal weiterbestehen; der Entwickler würde dies nicht selbst bevorzugen, hält es aber für möglich.
- 📋 Das Strafsystem soll **realistisch und logisch** sein ([Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md)).

---

## 4. Die Erwachsenen-Branche als Weltschicht

📋 (im Umfang 🧭) Teil der normalen Wirtschaft und Gesellschaft, keine Parallelwelt.

- **Orte:** Stripclubs, Swinger- und Erwachsenen-Clubs, Bordelle/Studios, Erwachsenen-Unterhaltung und Läden. Der Entwickler: Stripclubs sind „essentiell“, in der Metropole 15 bis 30, in kleineren Städten weniger, dazu mindestens ein sehr großer mehrstöckiger Betrieb mit verschiedenen Preisstufen. Es gibt unterschiedliche Preis-, Qualitäts-, Privatsphäre- und Rufstufen. Läden für Erwachsenen-Bedarf, Nagelstudios, Kosmetik und weitere Geschäfte gehören zur allgemeinen Gebäudeplanung (die Planung der Gebäude steht aus).
- **Karriere und Geschäft:** Erwachsenen-Arbeit als echte Karriere; Studios und Plattformen können wirtschaftlich scheitern, konkurrieren, Trends ändern sich. Es gibt eine **Creator-Ebene** (Abonnements, Inhalte, parasoziale Beziehungen), professionelle Studios und Inhaltsproduktion (auch durch den Spieler). Produktionen von **vereinbarten Rollenspiel-Szenarien** zwischen Erwachsenen sind als Inhalt in der Spielwelt gedacht (siehe [Modul 03](03-Interaktion-Taxonomie-Ablauf.md#6-kontext-statt-kategorie-dienstleistung-rollenspiel)); das ist ❓ inhaltlich nicht freigegeben.
- **Start klein, wachsend** (A7, Entwickler: „anfangs klein und immer wachsend“): Die Medienproduktion beginnt als kleines System und kann zu einem großen Geschäfts-/Creator-System wachsen.
- **Keine „Adult-Welt neben der echten Welt“:** gleiches Gedächtnis, gleiche Beziehungen, gleiche Physiologie, gleicher Weltzustand.
- **Medienkonsum und Solo-Verhalten** sind private NPC-Zustände mit stark individueller Häufigkeit (Browser- und Abonnement-Historie als privater Zustand). Sie dürfen nicht öffentlich ausgelesen werden.

---

## 5. Dienstleistende NPCs und Verträge

Dienstleistung ist ein **Vertragskontext**, keine Handlungskategorie ([Modul 03](03-Interaktion-Taxonomie-Ablauf.md#6-kontext-statt-kategorie-dienstleistung-rollenspiel)).

### 5.1 Grundregeln (📋 aus den konsolidierten Entscheidungen vom 31.08.)

- Dienstleistende NPCs sind individuelle, dauerhafte erwachsene Figuren mit Namen, Persönlichkeit, Tagesablauf, **Grenzen**, Vorlieben, Preisen und Erinnerungen.
- **Nicht jede Person bietet jede Leistung.** Eine Tänzerin ist nicht automatisch Dienstleisterin.
- Preise sind individuell und marktbasiert (Premium-Angebote). **Geld ersetzt weder Anziehung noch individuelle Grenzen noch Zustimmung.**
- Beziehungen können aus beruflichem Kontakt entstehen. Eine Beziehungsfigur behält ihre eigenen harten Grenzen; die Verfügbarkeit anderer einvernehmlich handelnder Erwachsener hebelt sie nicht aus.
- Transaktionen laufen über das **normale Wirtschaftssystem**, nicht über eine Sonderwährung.
- Ein Leistungsprofil wie „sehr weitgehend“ bedeutet **weitreichende freiwillige Grenzen, keine unwiderrufliche Zustimmung**. Risiko bleibt im Gesundheitssystem.
- Ein Spieler kann eine Figur überzeugen, ihre Tätigkeit zu wechseln; **sie entscheidet selbst** (Entwickler: „du kannst ihr sagen, sie hat Potenzial“).
- Alters- und Identitätsprüfung ist bei frisch Volljährigen Pflicht ([Modul 01](01-Grundregeln-Alter-Zustimmung.md#2-altersgrenze-und-prüfung)).

### 5.2 Langzeit-Begleitvertrag

Status: 📋 Richtung des Entwicklers, Details 🧭 (Vorschläge des Assistenten).

Der Entwickler beschrieb (15.09.) eine Website, auf der man Wünsche (Aussehen, Eigenschaften, Sonderwünsche) und eine Telefonnummer angibt; das System sucht eine passende Person, und bei Erfolg wird der Spieler kontaktiert (nicht in Missionen). Die Person kann für eine Zeit oder auch unbefristet **gebucht** werden, gut bezahlt (Beispiel des Entwicklers: etwa 250 Euro pro Tag) und kann zusätzlich Haushalt, Kochen oder Begleitung übernehmen.

Festgehalten wurde, und der Entwickler widersprach nicht:

- Die Person bleibt eine **freie NPC-Person**, die bezahlt wird und **kündigen kann**; es gibt **kein Eigentum** an Personen. Der Entwickler bevorzugt, dass überwiegend der Spieler kündigt; dass die NPC „sehr selten“ kündigt, ist akzeptiert, solange sie gut behandelt wird. Ein pauschal fest verdrahtetes „kündigt nie“ wurde vom Assistenten abgelehnt.
- Modell der Bleibe-Motivation (🧭, Vorschlag des Assistenten): Bezahlung, Beziehung, Lebensqualität, Sicherheit, passende Aufgaben, Vertrauen, persönliche Präferenz, abzüglich Konflikte, schlechte Behandlung und bessere Alternativen. Damit bleibt eine Figur bei guter Behandlung über Jahre, ohne dass es erzwungen ist.
- Kündigung durch den Spieler ist möglich. Was mit der Figur danach geschieht, bestimmt ihr Lebenslauf in der Welt. Aus „kein Eigentum“ folgt (🧭): **kein Verkauf und kein Weiterreichen** als Mechanik. Der Entwickler nannte das Weiterreichen an Dritte als Argument für eine fast ausgeschlossene Kündigung; das wurde nicht übernommen.
- Der Entwickler beschrieb die Person mit einem drastischen Begriff, fügte aber hinzu, dass sie es **aus freiem Willen tut und dafür bezahlt wird**. Der Assistent ordnete eine solche Dynamik als **vereinbartes Rollenspiel zwischen Erwachsenen innerhalb der Beziehung** ein, nicht als Besitzverhältnis ([Modul 03](03-Interaktion-Taxonomie-Ablauf.md#6-kontext-statt-kategorie-dienstleistung-rollenspiel)).
- Ein Vertrag kann zugleich Arbeitsvertrag, Partnerschaft und gemeinsamer Haushalt werden.

❓ Offen: Details von Kündigungsrechten, Vertragsmodell, Altersprüfung im Matching, Auswirkungen auf Reputation und Recht.

---

## 6. Spielwelt-Internet und Medien

- 📋 Das Spiel enthält ein **funktionales Internet** mit verschiedenen Webseiten in der Welt (Läden, Studios, Erwachsenen-Seiten, Buchungsseiten). Spieler können später vereinfachte Webseiten erstellen und damit Geld verdienen; daneben Casinos, Buchmacher, Börse und weitere Wirtschaftssysteme (Entwickler, 14.09.; eigene Planung).
- 📋 **In-Game-Videos sind feste Inhalte:** Ein bestimmtes Video ist **immer exakt dasselbe**, es wird nicht bei jedem Aufruf neu generiert (Entwickler, 15.09., mit Betonung). Die Videos zeigen Figuren der Spielwelt. Die technische Umsetzung ohne riesigen Speicherbedarf ist ❓ offen (der Entwickler befürchtet mehrere Gigabyte pro Video).
- 📋 Das Spiel erzeugt zur Laufzeit **keine** neuen Inhalte mit generativer KI; KI wird in der Produktion eingesetzt, die Laufzeit bleibt deterministisch (Entwickler, 25.09.: „genau so habe ich das im Sinn“). Für Erwachsenen-Inhalte ist das zusätzlich eine Plattformfrage ([Modul 10](10-Produktion-Recht-Vertrieb.md#3-recht-und-plattformen)).
- 📋 Anfragen von NPCs nach Fotos oder Treffen im Zusammenhang mit Plattformen sind möglich (Entwickler, 15.09.).

---

## 7. Gegenstände

- 📋 Erwachsenen-Gegenstände sind **echte Weltobjekte** mit Größe, Material, Qualität, Verschleiß, Reinigung und Lagerung; sie können nicht aus dem Nichts erscheinen („spawnen“). Es gibt verschiedene Läden; NPCs kaufen sie selbst. Das Risiko leitet sich nicht aus einem einzelnen Größenwert ab, sondern aus Körperzustand, Anatomie, Erfahrung und Belastung ([Modul 06](06-Koerper-Physik-Gesundheit.md#4-biomechanische-grenzen)).
- 📋 Mehrere Gegenstände sind gleichzeitig in einer Sitzung möglich.

---

## 8. Offene Punkte

- ❓ Anzahl, Lage und Art aller Betriebe (Kartenplanung steht aus).
- ❓ Rechtslage der Branche in der fiktiven Region; Plattformregeln für entsprechende Inhalte im Spiel.
- ❓ Preismodell, Vertragsdetails ([Abschnitt 5.2](#52-langzeit-begleitvertrag)).
- ❓ Umsetzung fester Videos ohne großen Speicherbedarf.
- ❓ Inhaltliche Freigabe von Rollenspiel-Produktionen als Weltinhalt.

Siehe [Offene Fragen](11-Offene-Fragen-und-Konflikte.md).
