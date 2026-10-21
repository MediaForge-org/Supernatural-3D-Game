# Adult 19: Konflikt- und Widerspruchsmatrix

> [!IMPORTANT]
> **Nichts davon ist implementiert.** Hier stehen Konflikte **zwischen Quellen** und **zwischen Entwickleraussagen und Sicherheitskern**. Die Matrix löst nichts auf; Entscheidungen trifft der Entwickler. Statuszeichen: [Legende](README.md#statuslegende).

Teil der Adult-Dokumentation ([Einstieg](../Adult-System.md), [Index](README.md)). Die Konflikte K1 bis K9 mit Kurzfassung stehen in [Modul 11](11-Offene-Fragen-und-Konflikte.md); dieses Modul ergänzt sie um **Quellenbelege, Gegenpositionen, Auswirkungen und Entscheidungsoptionen** und führt weitere Konflikte (K10 ff.) und reine Zählabweichungen.

**Inhalt**

1. [Einordnung und Legende](#1-einordnung-und-legende)
2. [Übersicht](#2-übersicht)
3. [Sicherheitsrelevante Konflikte K1 bis K4, K9](#3-sicherheitsrelevante-konflikte)
4. [Setting-, Vertrags- und Kommunikationskonflikte K5 bis K8](#4-setting--vertrags--und-kommunikationskonflikte)
5. [Weitere Konflikte K10 bis K23](#5-weitere-konflikte)
6. [Zählabweichungen](#6-zählabweichungen)
7. [Auflösungsreihenfolge (Vorschlag)](#7-auflösungsreihenfolge-vorschlag)

---

## 1. Einordnung und Legende

| Kategorie | Bedeutung |
|---|---|
| S | sicherheitsrelevant (berührt den [Sicherheitskern](01-Grundregeln-Alter-Zustimmung.md)) |
| P | Planungsstand/Quellenabweichung |
| R | rechtlich/plattformbezogen (ungeprüft) |
| W | Welt-/Setting-Entscheidung |
| T | technischer Vorschlag ohne Entscheidung |

Spalte „Stand“: **aufgelöst** (Quellen selbst haben es entschieden), **vorläufig** (Kompromiss ohne endgültige Entscheidung), **offen**, **nicht freigegeben** (⛔, wird nicht entwickelt).

## 2. Übersicht

| Nr. | Kurzbeschreibung | Kat. | Stand |
|---|---|---|---|
| K1 | Frühe Zustimmungsvorstellungen vs. Sicherheitskern | S | aufgelöst, Gewichtung offen |
| K2 | „USER-LOCK“, Handeln gegen den Willen im Admin-Build | S | ⛔ nicht freigegeben |
| K3 | Spielerbegangene Sexualstraftaten als Weltkonsequenz | S | vorläufig |
| K4 | Gespielte Nichteinwilligung im Rollenspiel/in Produktionen | S | vorläufig, nicht freigegeben |
| K5 | Orientierung: Setting-Wunsch vs. datengetriebenes Modell | W | offen |
| K6 | Langzeitvertrag und Kündigung | W/S | vorläufig |
| K7 | KI-Nutzung geheimhalten | R | offen |
| K8 | ‚Frauen sind bereit‘ vs. harte Grenzen | S | aufgelöst |
| K9 | Einvernehmlicher Schlaf-Zustand | S | offen |
| K10 | Fragenzahl Runde 15 (175 vs. 200) | P | offen (Zählung) |
| K11 | Zählung Runde 16P | P | offen (Zählung) |
| K12 | Zuwanderung in der Bevölkerung | W | offen |
| K13 | Entwickler-Mod als Fan-Mod ausgeben | R/S | ⛔ abgelehnt |
| K14 | Beschneidung und religiöse Zusammensetzung | W | vorläufig |
| K15 | Zeitmaßstab bei Interaktionen | T | offen |
| K16 | Extremistische Symbole als Tattoos | R/W | offen |
| K17 | ‚Erst bei Penetration Adult Only‘ | R | vorläufig (Assistent widersprach) |
| K18 | Veröffentlichungsziel (privat vs. öffentlich) | R | vorläufig |
| K19 | Zeitpunkt von Adult in der Entwicklungsreihenfolge | P | vorläufig |
| K20 | Tattoo-Prozentverteilung | T | vorläufig |
| K21 | Mehrspieler: Mods im Online vs. Host-Aktivierung | T/R | offen |
| K22 | „Adult fertig“ vs. offene Audits | P | aufgelöst (Audit offen) |
| K23 | Reihenfolge der Abschlussschritte (Audit, Qwen, Runde 17) | P | aufgelöst durch tatsächlichen Verlauf; zweiter Audit offen |

## 3. Sicherheitsrelevante Konflikte

### K1 Frühe Zustimmungsvorstellungen vs. Sicherheitskern

| Feld | Inhalt |
|---|---|
| Positionen des Entwicklers | 35#191/193 (einmal „Ja“ reiche auch bei Nichtwissen, Nicken); 35#195/199 (Ausziehen ok, „System blockiert nicht, wenn zu betrunken“); 35#444 Frage 5 (früheres Ja werde übernommen, wenn kein späteres Nein) und Frage 11 (Betrunkener mit Ja genügt); 35#446 (Betrunkene sollen genug Aussagekraft haben, solange sie nichts abwehren); 35#450/452 (Weggetretene bemerke der Spieler; ein paar Minuten unbemerkt möglich); 35#578 (Schweigen oder Stöhnen = weitermachen). |
| Gegenpositionen | 35#190–202, 35#445, 35#447, 35#451, 35#453, 35#577, 35#579. |
| Spätere Bestätigungen | 35#448 (Entwickler: ‚betrunken nicht so, dass man nicht mehr antwortet‘), 35#454 („Ja ok dann passt das“), 35#580 („Ja ok aber nicht zu streng“). |
| Auflösung | Capacity Gate + Ansprechbarkeit; Zustimmung gilt in der laufenden Situation; Sitzungsrahmen; Awareness-Verzögerung kurz; Bewusstlosigkeit beendet die Interaktion. |
| Offen | **Gewichtung der Live-Signale** (wie schnell reagiert das System auf Passivität, wann ist ein Check-in nötig) – Zahlenwerte gibt es nicht. |
| Auswirkung | [Modul 01](01-Grundregeln-Alter-Zustimmung.md), Tests im [QA-Katalog](29-QA-Testkatalog.md) |

### K2 „USER-LOCK“ vom 20.09.

| Feld | Inhalt |
|---|---|
| Positionen | 35#627 (Brutalität gegen den Willen ‚mit Konsequenzen‘), 35#633 („markiere das als von mir festgelegt“), 35#635 (nur im Admin-Build), 35#637/639 (gleicher Funktionsumfang), 35#653 (Weitermachen bei schlafender Partnerin, nur Admin), 35#655 („mach das so“). |
| Gegenposition | 35#628 (gehört ins Crime-System), 35#630/632, 35#634/636 (formulierte „USER-LOCK“ als **interne Konsequenz-Testversion**, nicht Spielinhalt), 35#638/640 (kein gleich großer Katalog), 35#654/656 (abgelehnt, auch für Admin). |
| Ergebnis im Gespräch | 35#657: „Ja ok dann plane ich das dann halt mit Qwen.“ Das ist keine Zustimmung zum Ersatzvorschlag. |
| Dokumentationsentscheidung | **⛔ nicht freigegeben.** Keine Spezifikation, keine Tests, keine Abnahmekriterien. Der Sicherheitskern (Regeln 2 und 3) bleibt unverändert. |
| Möglichkeit für den Entwickler | (a) Forderung zurückziehen; (b) einen reinen **Konsequenz-Testpfad** (Zustände, Beweise, Polizei, Gericht) **ohne** sexuelle Handlungsmechanik definieren lassen; (c) die Forderung aufrechterhalten – dann ist sie außerhalb dieser Dokumentation zu entscheiden. Die Dokumentation wählt nicht. |
| Querverweis | [Modul 09](09-Dunkle-Themen-und-Nicht-Uebernommenes.md), Register [E-XX-011 bis E-XX-016](14-Entscheidungsregister.md#17-xx-nicht-freigegeben-historisch-überholt) |

### K3 Spielerbegangene Sexualstraftaten

| Feld | Inhalt |
|---|---|
| Entwickler | 35#530 (keine Einschränkung bei Verbrechen, nicht erklärt), 35#532 (nicht empfohlen, realistisches Strafsystem). |
| Assistent | 35#531/533: Crime-System ohne künstliche Liste; **nur Konsequenzebene** (Schreie, Zeugen, Spuren, Erinnerungen, Polizei, Verfahren, Haft, Ruf); keine Ausführungsmechanik, nicht erotisiert, kein Menü/Tutorial. |
| Stand | vorläufig. Etappe 2.2: Die projekteigene Konsolidierung (`VD15`, `CRIME_JUSTICE_LOGIC_DIRECTION`) führt dies als **DIRECTION**: „Planung bleibt auf Crime-/Consequence-Ebene, nicht als tutorialisierte Ausführungsmechanik“. |
| Offen | Ob solche Straftaten für Spielerfiguren überhaupt als Weltereignis modelliert werden (Rating, Plattform, [Modul 10](10-Produktion-Recht-Vertrieb.md)) und wie Zeugen/Beweise ohne Handlungsdarstellung abstrahiert werden. |
| Abgrenzung | Kein Adult-Interaktionsmodus; Verhalten läuft über die Systeme Kriminalität/Justiz. |

### K4 Gespielte Nichteinwilligung

| Feld | Inhalt |
|---|---|
| Entwickler | 35#524/35#526 (Rollenspiel ‚wie bei Produktionsfirmen‘, hochgeladen auf Seiten der Spielwelt); 35#584 Frage 43. |
| Assistent | 35#525, 35#585: als **vereinbartes Rollenspiel zwischen Erwachsenen** speicherbar; tatsächliche Gewalt bleibt Crime-System. |
| Stand | vorläufig, **inhaltlich nicht freigegeben**. |
| Offen | Messbare Abgrenzung (Rollenspielvereinbarung, Stoppwort, getrennter Weltzustand) – braucht eine eigene, vom Entwickler beauftragte Spezifikation und Rechts-/Plattformprüfung. |

### K9 Einvernehmlicher Schlaf-Zustand

| Feld | Inhalt |
|---|---|
| Vorschlag | 35#654/656: vorher vereinbartes Schlafspiel (Schlafzustand, Atmung, Aufwachen). |
| Gegenregel | Sicherheitskern: Schlafende nehmen nicht aktiv teil; Entwurf 28#25 Abschnitt 0. |
| Stand | offen; bis zur Entscheidung gilt die strengere Regel. |

## 4. Setting-, Vertrags- und Kommunikationskonflikte

### K5 Orientierung

| Feld | Inhalt |
|---|---|
| Entwickler | 35#550: Frage 152 mit abwertendem Zusatz; 35#552: „das war ein Spaß, ich will LGBTQ nicht im Spiel haben“. |
| Assistent | 35#551: datengetriebener Zustand, stark heterosexuell geprägte Demografie möglich, Gewalt wegen Orientierung keine Weltregel; 35#553: ‚Setting-Entscheidung klar: keine entsprechenden Charakter-/Beziehungsinhalte‘ (Formulierung des Assistenten). |
| Widerspruch | Das Präferenzmodell (R15G, Fragen 151–153) beschreibt Orientierung als Spektrum. Die Setting-Entscheidung schließt Inhalte aus. |
| Auswirkung | Datenmodell der Beziehungen, Spielerfigur-Konstellationen, Mod-Grenzen, Plattform-Beschreibung. |
| Weitere Belege (Etappe 2.2) | `VD15`: R15-Lock („Aktuelle Nutzer-Setting-Vorgabe: keine LGBTQ-Relationship-/Character-Inhalte im geplanten Spielcontent. Relationship-/Human-Systeme bleiben technisch datengetrieben.“) und `CONTENT_SETTING_SOCIAL_DIRECTION`. Die Setting-Vorgabe ist damit auch in der projekteigenen Konsolidierung festgehalten; offen bleibt die Frage nach dem Datenmodell. |
| Entscheidungsbedarf | (a) Das Modell bildet nur die vorgesehene Demografie ab, Mods dürfen mehr; (b) das Modell bleibt neutral, die Demografie wird über Daten gesteuert; (c) weitere Vorgabe. Die Dokumentation folgt (b) **nicht stillschweigend**. |
| Hinweis | Unabhängig davon gilt die Regel der Quellen: Gewalt wegen Orientierung ist kein gewünschter Welt-Mechanismus (35#551). |

### K6 Langzeitvertrag

Siehe Kurzfassung in [Modul 11](11-Offene-Fragen-und-Konflikte.md) und die Chronologie in [Modul 16, Abschnitt 8.5/8.6](16-Interviewprotokoll-Runden-11-bis-16.md#85-r15e-539540-101125). Wunsch des Entwicklers (35#544/546/548): Eigenkündigung ‚so gut wie nie‘, bevorzugt Kündigung durch den Spieler. Assistent (35#545/547/549): Kündigungsrecht bleibt; Eigenkündigung nur aus Präferenz-/Beziehungszustand. Offen: Begründung, Schwellen, Altersnachweis-Verfahren, Darstellung der Vertragsarten.

### K7 KI-Geheimhaltung

Entwickler 35#733: „ich will das eher geheimhalten“. Assistent 35#734: nicht verstecken oder falsch kleinreden. Aus Dokumentationssicht gilt: Plattformangaben müssen **wahrheitsgemäß** sein; Details zu Umfang und Form der Offenlegung sind eine Release-Entscheidung ([Modul 10](10-Produktion-Recht-Vertrieb.md)).

### K8 ‚Frauen sind bereit‘ (aufgelöst)

35#229 (Entwickler: ein Nein werde oft nur gespielt) und 35#229–231 (Assistent: aktuelles Nein bleibt Nein). Sehr weite Präferenzprofile sind erlaubt, aber partner- und situationsabhängig; jede Figur hat harte Grenzen (K§19).

## 5. Weitere Konflikte

### K10 Fragenzahl Runde 15

Entwickler-Kontextprompt 35#608 und 35#705: „175/175“. Die Konsolidierung vom 15.09. (`VD15`, R15-Lock) führt „Runde 15 (1–200)“. Assistentenzählung in den Paketen R15A bis R15H: A–D je 25 = 100, E–H je 25 = 100, zusammen 200. Die Pakete R15A–H sind in den Quellen belegt ([Modul 16](16-Interviewprotokoll-Runden-11-bis-16.md#8-runde-15-praktiken-präferenzen-gesellschaft)). Die Dokumentation nennt **beide Zahlen** und bevorzugt keine. Auswirkung: nur die Statistik der Abdeckung.

### K11 Zählung R16P

35#695: „376–400 ja“ (beantwortet). 35#705 (Entwickler-Kontextprompt vom 24.09.): R16P ‚vorbereitet, nicht vollständig beantwortet‘. 35#706 (Assistent): bereits beantwortet. Vermutlich ein veralteter Stand im Kontextprompt. Auswirkung: keine inhaltliche.

### K12 Zuwanderung

35#261: ‚keine Immigranten, nur Leute von dort oder aus den USA‘. 35#262 (Assistent): einzelne Zugewanderte und Touristen seien glaubwürdig. 35#263: „Ok“ ohne klare Aussage. **Offen.** Auswirkung: Demografie-Generator, Namen, Stimmen. Die Dokumentation fixiert nichts.

### K13 Entwickler-Mod als Fan-Mod

23#545/546: Täuschung abgelehnt. Es gibt keine Planung dazu. Dokumentierter Hinweis: eigene Mods sind als solche zu kennzeichnen ([E-RC-003](14-Entscheidungsregister.md#16-rc-recht-plattform-ki-vertrieb)).

### K14 Beschneidung und Religion

35#500 Frage 62: Entwickler ‚eher nicht, nur gesundheitlich, nicht wegen Religion; Welt zu 99,9 % christlich, andere vereinzelt‘. Assistent: Realismus anders. **Vorläufig**, Daten- und Setting-Entscheidung; Auswirkung auf Anatomievariation und Religionszusammensetzung der Welt (nicht Adult-spezifisch, [Modul 08](08-Welt-Orte-Branche-Wirtschaft.md)).

### K15 Zeitmaßstab

35#540: ‚30 Minuten Realzeit dürfen nicht 12 Stunden Spielzeit sein‘; 35#541: Vorschlag nahe 1:1, größere Sprünge nur bei Schlaf/Reise/Warten; eigener World-Time-Block. Der Entwickler hat die Zahl nicht bestätigt. Die Konsolidierung `VD15` hält im R15-Lock „Adult-Situationen zeitlich nahe 1:1; größere Time Skips separat planen“ fest und führt „World Time Scale“ zugleich unter *Deferred*. **Offen.**

### K16 Extremistische Symbolik

35#775 Frage 103 (Hasssymbole auf Tattoos von Gruppen). Rechtliche Zulässigkeit je Markt ungeprüft. Ein Lösungsansatz der Quellen (35#776 zusammen mit `PD`): Symbole nur an konkrete fiktive Fraktionen binden, keine Verherrlichung. **Offen.**

### K17 ‚Erst bei Penetration Adult Only‘

35#749 Frage 20 (Entwickler) vs. 35#750 (Assistent): Grenze zu eng. Beide Aussagen ungeprüft. **Vorläufig**; Plattform-Deskriptoren sind korrekt anzugeben ([Modul 10](10-Produktion-Recht-Vertrieb.md)).

### K18 Veröffentlichungsziel

35#717 (nur privat, falls ungeeignet), 35#810 (Frage 10: kein Pflichtziel), 35#811 (will Veröffentlichung), 35#812 (Ziel öffentlich, privat = Rückfall). Spätere Aussage gilt, Rechtslage ungeprüft.

### K19 Zeitpunkt von Adult

35#604 (‚schnell verfügbar, vor manchen anderen Systemen‘), 35#713 („sehr früh implementiert und dann sehr viel erweitert“), 35#715 (Ladebildschirm und kleiner Kartenteil zuerst). Die späteste Aussage hat Vorrang; die [Roadmap-Reihenfolge](../Entscheidungsstatus.md) ist nicht festgelegt.

### K20 Tattoo-Prozentverteilung

35#776: Zahlen des Assistenten; Entwickler 35#777 „passt“. Nicht als Zahlen bestätigt. Siehe [E-TA](14-Entscheidungsregister.md#10-ta-tattoos-piercings-körpermodifikation).

### K21 Mehrspieler

23#551 (offizielles Online ohne Mods; Koop mit Freunden in privater Lobby), 35#626 Frage 196 (Adult nur vom Host aktivierbar, autoritativer Server). Vereinbar, aber nicht abgestimmt: Ob Adult in privatem Koop mit Gästen überhaupt zulässig ist, wie Gäste zustimmen und welche Alters-/Zustimmungsprüfung gilt, ist offen. Siehe [Modul 02](02-Architektur-Pakete-Technik.md).

### K22 „Adult fertig“

37#83 („Bro ich habe Adult fertig“), 35#812 (‚im Wesentlichen abgeschlossen; offen: Audits, Qwen, Prototyping, Recht‘). Zusätzlich 37#84/85 (Tagesplan-Assistent): „Adult-Gesamtplanung: ABGESCHLOSSEN“, jedoch mit der Einschränkung „soweit du mit ‚Adult fertig‘ den Gesamtblock meinst“. Aufgelöst: Planung „im Wesentlichen“ abgeschlossen, **Taxonomie-Audit, Qwen-Audit, Prototyping und Rechtsprüfung offen**.

### K23 Reihenfolge der Abschlussschritte

| Feld | Inhalt |
|---|---|
| Positionen | **Tagesplan-Assistent** (37#77, 37#80–82, 24.–28.09.): „MEGA 16 → Missing-Family/Taxonomy-Audit → Ergänzungsfragen → später Qwen-Audit → danach erst Adult-Runde 17“. **Entwickler** (35#747, 25.09.): „ich sagte dir, dass ich Qwen jetzt nicht habe, also jetzt Runde 17“. |
| Tatsächlicher Verlauf (35) | Pre-Audit A1–A15 (35#742–746, 25.09.); Assistent 35#748: „Runde 16 normale Planung abgeschlossen, der finale Qwen-/Taxonomy-Audit bleibt als späterer Nachcheck offen“; Runde 17 25.–26.09.; Runden 18–20 26.–27.09. |
| Befund | Der Tagesplan (37) spiegelt einen älteren Stand; der Chat 35 ist maßgeblich. Der in 35#742 angekündigte **zweite, kurze Audit über die Unterkategorien** wurde **nicht** durchgeführt. |
| Stand | aufgelöst (Verlauf); Audit-Lücke bleibt offen ([OE-80, OE-86](34-Offene-Entscheidungen-Katalog.md#8-audits-und-externe-läufe)). |

## 6. Zählabweichungen

| Punkt | Wert A | Wert B | Behandlung |
|---|---|---|---|
| Rundenzahl | 18 (35#447) | 19/20 (35#491, 35#591) | Endstand 20 inkl. Replay-Zusatzrunde 12 |
| Runde 15 | 175 (Entwickler) | 200 (Assistent) | siehe K10 |
| R16P | beantwortet (35#695) | offen (35#705) | siehe K11 |
| Pre-Audit | 15 Punkte (35#742) | Verweise auf „A1–A14“ | Zählung 15 inkl. A15 (Kandidaten) |
| „Adult fertig“ | 37#83 | 35#812 | siehe K22 |
| Runde 17 | ‚kompakt, 60–90 Fragen‘ (35#748) | 141 Fragen (17A–17I) | Umfang wuchs; Entwickler hatte „nur die wichtigsten“ gewünscht (35#709) |
| Fassungen des Rahmenentwurfs | mehrere (28#2/4/6/9/11/25) | Endfassung 28#25 | nur 28#25 vollständig ausgewertet |

## 7. Auflösungsreihenfolge (Vorschlag)

1. **K2** klären (Rücknahme oder Konsequenz-Testpfad ohne Handlungsmechanik).
2. **K5** und Spielerfigur-Konstellationen klären (bestimmt das Datenmodell der Beziehungen).
3. **K9, K4, K3**: Abgrenzungen messbar formulieren, bevor ein Testkatalog über die Sicherheitsfälle hinausgeht.
4. **K1** Gewichtung der Live-Signale im Prototyp erproben.
5. **K7, K16, K17, K18, K21**: mit Rechts-/Plattformberatung vor jeder Veröffentlichung.
6. **K12, K14, K15, K20**: Setting- und Datenentscheidungen im Weltaufbau.
7. K10, K11, K22, K23: nur Zählung bzw. Ablauf.

Dieser Vorschlag ist eine **Reihenfolge der Klärung**, keine Rangordnung von Wünschen.
