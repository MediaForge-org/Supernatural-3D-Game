# Fahrzeuge und Verkehr 18: Besitz, Zulassung und Diebstahl

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Beschlossen ist, dass Versicherungen vorgesehen sind (in einer späteren, ausgereifteren Fassung), dass Zulassung und Papierkram stark abstrahiert werden, dass Fahrzeugdiebstahl unabhängig vom Spieler geschieht und dass Fahrzeuge und Kennzeichen Beweisquellen sind. Methoden, Rechtsfolgen und Zulassungsregeln sind nicht beschrieben. Statuswörter: [Legende](README.md#statuslegende).

Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Register: [Modul 28](28-Entscheidungsregister.md). Rechtsfolgen und Beweise gehören den Ordnern [Ermittlung](../Ermittlung/README.md) und [Kampfsystem 20](../Kampfsystem/20-Fraktionen-Polizei-und-Kampfkonsequenzen.md).

**Inhalt**

1. [Was beschlossen ist](#1-was-beschlossen-ist)
2. [Eigentum und Zulassung](#2-eigentum-und-zulassung)
3. [Kauf, Verkauf, Versicherung](#3-kauf-verkauf-versicherung)
4. [Diebstahl](#4-diebstahl)
5. [Kennzeichen und Wiedererkennung](#5-kennzeichen-und-wiedererkennung)
6. [Register](#6-register)
7. [Offene Punkte](#7-offene-punkte)

---

## 1. Was beschlossen ist

| Gegenstand | Aussage | Quelle |
|---|---|---|
| Versicherung | Haftpflicht-/Kasko-artige Versicherungen: ja, aber in einer ausgereifteren Fassung (‚eher in einer fertigeren version‘) | `23#1025E` Nr. 16 |
| Zulassung | Fahrzeugkauf soll glaubwürdig funktionieren, Papierkram wird stark abstrahiert | `23#1025E` Nr. 17 (Antwort ‚ja‘) |
| Markt | Gebrauchtmarkt mit Laufleistung, Historie, Verschleiß, versteckten Mängeln; Händler gehören zum Gewerbe | `W-FZ-003`; `W-UR-005` |
| Kriminalität | Fahrzeugdiebstahl, Einbruch, Raub, Bandenkriminalität und illegale Rennen laufen unabhängig vom Spieler | `M-WB89` Block 8 |
| Spieler | Der Spieler kann tiefes kriminelles Gameplay betreiben und landesweit gesucht werden | `M-WB89` Block 8; `COMBAT-DEC-PO-008` |
| Beweise | Fahrzeug und Kennzeichen sind Beweisquellen; Kennzeichenabfrage; Täterbeschreibung enthält das Fahrzeug | `E-RE-002`; `E-RE-010` |
| Wiedererkennung | NPCs können den Spieler und auffällige Fahrzeuge wiedererkennen | `N-GE-018`; `E-SP-011` |
| Identitäten | Intelligente übernatürliche Wesen können unter menschlicher Identität mit Wohnung, Beruf, Fahrzeugen und Konten leben | `M-WB89` Block 9 |
| Persistenz | Fahrzeuge sind persistent mit stabiler Identität | `W-FZ-016` |
| Falltypen | Versicherungsbetrug und vermisste Fahrzeuge kommen als organische Fälle vor | [Ermittlung 07](../Ermittlung/07-Detektivlaufbahn-und-organische-Faelle.md) |

Jurisdiktionen: Es gibt mehrere Polizeitypen und Zuständigkeiten in einer fiktiven US-artigen Welt mit fiktiven Bundesstaaten (`M-WB89` Block 8; [Welt 07](../World/07-Weitere-Staedte-und-Siedlungen.md)). Daraus folgt für Kennzeichen und Zulassung ein Bezug zum Bundesstaat; die Quellen nennen ihn nicht.

---

## 2. Eigentum und Zulassung

> **💡 VORSCHLAG.** Ein Fahrzeug trägt vier getrennte Fakten, die je einen Eigentümer haben (`VEH-DEC-GP-015`):

| Fakt | Inhalt | Wer ändert es |
|---|---|---|
| Eigentümer | Person, Haushalt, Firma, Behörde, Spieler | Kauf, Erbe, Verkauf, Diebstahl-Rechtslage |
| Halter oder Fahrer | wer es nutzt | Nutzungsereignisse |
| Kennzeichen | Objekt mit Bundesstaat und Nummer | Zulassung |
| Zulassungsstatus | zugelassen, abgemeldet, als gestohlen gemeldet, beschlagnahmt | Zulassung, Polizei |

Alle Änderungen laufen über Ereignisse (Kauf, Verkauf, Anmelden, Abmelden, Diebstahl, Beschlagnahme, Wiedererlangung). Der Fahrzeugdatensatz ([Modul 03](03-Fahrzeugklassen-und-Fahrzeugdaten.md)) verweist nur auf diese Fakten und kopiert sie nicht.

---

## 3. Kauf, Verkauf, Versicherung

> **💡 VORSCHLAG.**
>
> - **Kauf in einem Schritt:** Ein Kauf überträgt Eigentum und Zulassung in einem Schritt, ohne Wartezeiten oder Formulare. Das setzt `23#1025E` Nr. 17 (Abstraktion) um und lässt Raum für eine tiefere Fassung.
> - **Verkauf und Handel:** Händler und Gebrauchtmarkt (`W-FZ-003`, `W-UR-005`) sind NPC-Betriebe mit Fahrzeugbestand als Weltzustand. Preisbildung ist nicht belegt.
> - **Versicherung:** In einer späteren Phase. Ein Schadensfall oder Diebstahl ist der Anlass; Betrugsfälle sind ein bereits vorgesehener Falltyp der Ermittlung. Umfang, Tarife und Zeitpunkt sind offen (`CQ-74`).

---

## 4. Diebstahl

Fahrzeugdiebstahl durch NPCs ist Teil der unabhängig laufenden Kriminalität (`M-WB89` Block 8). Der Spieler kann kriminelles Gameplay betreiben (ebd.). Die Quellen beschreiben weder Methoden noch Folgen für Fahrzeugdiebstahl.

> **💡 VORSCHLAG.** Diebstahl ist ein **Ereignis** mit Täter, Fahrzeug, Zugangsart, Zeugen und Spuren:
>
> ```text
> VehicleTheft
>   vehicle_id, perpetrator_ref, world_time, location_ref
>   access_method     (offen, aufgebrochen, Schlüssel, ...)  -> Modul 06 Zugangszustand
>   witnesses[], evidence_refs[]
> ```
>
> Folgen laufen an die zuständigen Systeme: Eigentümerwechsel in den Zustand ‚gestohlen‘ (Rechtslage), Fahndung nach Kennzeichen und Fahrzeug (Modul 19), Zeugen und Spuren (Ermittlung 05), Wiedererlangung (Fahrzeug taucht auf, wird gefunden, beschlagnahmt). Welche Zugangsmethoden das Spiel kennt und wie sie das Spiel bedienen, ist offen (`CQ-73`). Der Diebstahl **durch den Spieler** unterliegt denselben Folgen wie durch NPCs; eine Sonderregel wäre eine Designentscheidung.

---

## 5. Kennzeichen und Wiedererkennung

Das Kennzeichen ist ein Beweis (`E-RE-002`) und Teil der Täterbeschreibung (`E-RE-010`). Auffällige Fahrzeuge können wiedererkannt werden (`N-GE-018`).

> **💡 VORSCHLAG.** Das Kennzeichen ist ein eigenes Objekt (Bundesstaat, Format, Nummer), das einem Fahrzeug zugeordnet wird. Wiedererkennung läuft über **zwei Wege**: das Kennzeichen (Datenabfrage, Polizei, Zeugen mit Notiz) und die **Auffälligkeit** des Fahrzeugs (Modul 03, Feld `visual_conspicuousness`). Ob Kennzeichen getauscht oder gefälscht werden können (und welche Folgen das hat), ist nicht belegt (`CQ-73`).

Intelligente Wesen mit menschlicher Identität können Fahrzeuge besitzen (`M-WB89` Block 9). Ihr Fahrzeug ist ein Fahrzeug wie jedes andere und steht in Ermittlungen als Spur (siehe [Modul 21](21-Fahrzeuge-in-Ermittlungen-und-Jagden.md)).

---

## 6. Register

| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |
|---|---|---|---|---|---|
| VEH-DEC-BZ-001 | Versicherungen sind vorgesehen, aber in einer ausgereifteren Fassung | 📋 BESCHLOSSEN | `23#1025E` Nr. 16 | WIR FZ | Umfang und Zeitpunkt: `CQ-74`; Wortlaut ‚fertigere Version‘ unscharf: `CU-08` |
| VEH-DEC-BZ-002 | Fahrzeugkauf soll glaubwürdig funktionieren; Zulassung und Papierkram werden stark abstrahiert | 📋 BESCHLOSSEN | `23#1025E` Nr. 17 | WIR FZ | – |
| VEH-DEC-BZ-003 | Fahrzeugdiebstahl, Einbruch, Raub und illegale Rennen gehören zur unabhängig vom Spieler laufenden Kriminalität | 📋 BESCHLOSSEN | `M-WB89` Block 8 | KRI NPC | Rate offen |
| VEH-DEC-BZ-004 | Fahrzeug und Kennzeichen sind Beweisquellen; die Täterbeschreibung enthält das Fahrzeug; Kennzeichenabfrage | 📋 BESCHLOSSEN | `E-RE-002`; `E-RE-010` | POL ERM | – |
| VEH-DEC-BZ-005 | NPCs können den Spieler und auffällige Fahrzeuge wiedererkennen | 📋 BESCHLOSSEN | `N-GE-018`; `E-SP-011` | NPC ERM | – |
| VEH-DEC-BZ-006 | Intelligente übernatürliche Wesen können unter menschlicher Identität Fahrzeuge besitzen | 📋 BESCHLOSSEN | `M-WB89` Block 9 | KRE NPC | Auswirkung auf Ermittlung: Modul 21 |
| VEH-DEC-BZ-007 | Händler und Gebrauchtmarkt sind NPC-Betriebe mit Fahrzeugbestand | 🧩 ABGELEITET | `W-FZ-003`; `W-UR-005` | WIR NPC | Preisbildung offen |
| VEH-DEC-BZ-008 | Ein Fahrzeug trägt vier getrennte Fakten mit je einem Eigentümer: Eigentümer, Halter, Kennzeichen, Zulassungsstatus; Änderungen laufen über Ereignisse | 💡 VORSCHLAG | Ableitung aus `VEH-DEC-GP-015` | FZ PER | – |
| VEH-DEC-BZ-009 | Ein Kauf überträgt Eigentum und Zulassung in einem Schritt | 💡 VORSCHLAG | Ableitung aus `23#1025E` Nr. 17 | WIR | – |
| VEH-DEC-BZ-010 | Diebstahl ist ein Ereignis mit Täter, Fahrzeug, Zugangsart, Zeugen und Spuren; der Diebstahl des Spielers unterliegt denselben Folgen wie der von NPCs | 💡 VORSCHLAG | Ableitung aus `M-WB89` Block 8, `E-RE-002` | KRI POL | Methoden: `CQ-73` |
| VEH-DEC-BZ-011 | Das Kennzeichen ist ein eigenes Objekt mit Bundesstaat und Nummer; Wiedererkennung läuft über das Kennzeichen und über die Auffälligkeit des Fahrzeugs | 💡 VORSCHLAG | Ableitung aus `E-RE-010`, `N-GE-018` | POL ERM | Format: `CQ-75` |
| VEH-DEC-BZ-012 | Versicherung ist ein Anlass bei Schaden und Diebstahl; Betrug ist ein bereits vorgesehener Falltyp | 💡 VORSCHLAG | Ableitung aus [Ermittlung 07](../Ermittlung/07-Detektivlaufbahn-und-organische-Faelle.md) | WIR ERM | – |
| VEH-DEC-BZ-013 | Methoden des Fahrzeugdiebstahls, Konsequenzen und das Tauschen oder Fälschen von Kennzeichen sind nicht beschrieben | ❓ OFFEN | – | KRI | `CQ-73` |
| VEH-DEC-BZ-014 | Kennzeichenformate und Zulassungsregeln je fiktivem Bundesstaat sind nicht festgelegt | ❓ OFFEN | – | WLT POL | `CQ-75` |
| VEH-DEC-BZ-015 | Eigentum von Gast- und Hostfahrzeugen im Koop folgt dem Host-Speicherstand; Details sind offen | ❓ OFFEN | `CLAUDE.md` §6; `E-KO-010` | KOO PER | `CQ-76` |
| VEH-DEC-BZ-016 | Grenzen des Besitzes (Fahrzeuganzahl, Garagenkapazität, laufende Kosten, Steuern) sind nicht belegt | 🔍 UNBELEGT | – | WIR | `CQ-77` |

---

## 7. Offene Punkte

| Frage | Verweis |
|---|---|
| Methoden, Folgen und Kennzeichenmanipulation bei Diebstahl | `CQ-73` |
| Versicherungsumfang und -zeitpunkt | `CQ-74` |
| Kennzeichenformate und Zulassungsregeln | `CQ-75` |
| Eigentum im Koop | `CQ-76` |
| Besitzgrenzen und laufende Kosten | `CQ-77` |
