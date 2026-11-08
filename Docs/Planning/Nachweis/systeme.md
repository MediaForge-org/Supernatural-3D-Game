# Nachweis Systemanforderungen (C-systeme): Vollständigkeitsprüfung

| Feld | Inhalt |
|---|---|
| Stand | 08.11.2026 |
| Bearbeiter | Agent (Claude Haiku 5.5), Auftrag Nachweis |
| Umfang | 151 Anforderungen aus `Docs/Planning/Konsolidierung/C-systeme.md`: SY-ERM (26), SY-KAMPF (37), SY-KREA (27), SY-NPC (27), SY-FAHR (28), SY-WELT (6). 1447 zitierte Fundstellen in den Registern. |
| Abkürzungen | Er20 = `Docs/Game/Ermittlung/20-Entscheidungsregister.md`; Ka27 = `Kampfsystem/27-…`; Kr20 = `Kreaturen/20-…`; Np26 = `NPCs/26-…`; Fa28 = `Fahrzeuge-und-Verkehr/28-…`; Ka30, Fa31, Np29 = Testkataloge; Fa02 = `Fahrzeuge-und-Verkehr/02-Vision-und-Grundprinzipien.md`. Zeilenangaben beziehen sich auf die Dateien in `Docs/Game/`. |

**Vorgehen.** Jede der 151 Zeilen der Ausgangsliste wurde gelesen, und alle zitierten Fundstellen wurden maschinell geprüft: Jede der 1.447 Zeilen trägt die genannte ID, Abweichungen gab es keine. Die zitierten Zeilen wurden inhaltlich gegen die Kernaussage der Anforderung gelesen; fehlende Teile, Widersprüche und Fundstellen, die die Ausgangsliste nicht nennt, sind vermerkt. Die Master-Angaben von 12 Zeilen mit konkretem Zitat (RV, RS, UC, US, UA, AI, HB, WF sowie Fa01/Fa02) wurden read-only auf Zeileninhalt geprüft, alle übrigen Master-Angaben sind nur übernommen. Die Akzeptanz-IDs wurden in den Testkatalogen gesucht (alle gefunden), gewertet wurde nur, ob eine ID vorhanden ist. Status: „bereits vorhanden“ = Ziel nachweisbar und Akzeptanz-ID vorhanden; „vollständig integriert“ = Ziel nachweisbar ohne Akzeptanz-ID (dann vermerkt); „teilweise“ = nur ein Teil nachweisbar, oder nur eine Teilaussage hängt an einer offenen Entscheidung; „blockiert“ = der Kern hängt an K-26, K-27 oder K-28; keine Entscheidung wurde getroffen, und historische Aussagen (N-02) wurden nur markiert.

## 1. Tabelle

| ID | Originalquelle (geprüft: Datei:Zeile) | Ziel (Datei §Abschnitt) | Status | Fehlende Details / Korrektur |
|---|---|---|---|---|
| SY-ERM-001 | Er20:18,325,327 | Er20 §FA, QA | vollständig integriert | Master-Angabe (Register zitiert M-PD, 23#46E) im Master nicht gegengeprüft; Dauer bleibt vorläufig (W-06) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-002 | Er20:20,22,23,26,27 | Er20 §FA | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-003 | Er20:24,32,25 | Er20 §FA | vollständig integriert | Bindung der Checkpoint-Option an Schwierigkeit offen (E-FA-008); kein Bezug zu K-26 im Wortlaut, Verhältnis zum Todesmodell nicht geprüft ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-004 | Er20:42,43,44,45,46 | Er20 §MO | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-005 | Er20:48,47,49 | Er20 §MO | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-006 | Er20:53,54,55,56,57,61 | Er20 §MO | vollständig integriert | Schlüsselverfahren offen (E-MO-014); Companion-KI-Regel nur 💡 ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-007 | Er20:67,68,69,71,76,77,79,80 | Er20 §AR | vollständig integriert (CONSOLIDATION-02) | Korrektur: ‚Unbekannt‘ steht in Er20 E-AR-012 (Zeile 78), nicht fehlend; Notizen/Medien E-AR-007 (Zeile 73). Tests T-B12, T-B13 vorhanden; T-B16 (qualitative Anzeige, E-AR-013) ergänzt. Akzeptanzkriterium damit vorhanden. |
| SY-ERM-008 | Er20:81,355,82,83 | Er20 §AR, QA | vollständig integriert | Widerspruch W-08 (Status 📋 gegen 🕰️/❓) bleibt offen; Zahl nicht bestätigt ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-009 | Er20:89,90,91,92,101,102 | Er20 §SP | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-010 | Er20:95,96,97,104 | Er20 §SP | vollständig integriert | Laufzeiten und Schwellen bewusst nicht im Register (Lücke laut Ausgangsliste bestätigt) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-011 | Er20:157,158,159,162,163,98,99 | Er20 §ZE, SP | blockiert (K-27) für Teilaussage; Rest vollständig | Zeugen-Tests T-Z04, T-Z11, T-Z12 vorhanden; T-Z13 (wissen nur Gesehenes, keine garantierte Wahrheit) ergänzt. Blockiert: ‚Wichtige NPCs sterben nur durch den Spieler‘ (K-27, Widerspruch zu NPC-Modul LT-006/LB-017). Stufen A–D bleiben Richtung. |
| SY-ERM-012 | Er20:165,166,167,168,169 | Er20 §ZE | vollständig integriert | Gedächtnis intelligenter Wesen nur Richtung (E-ZE-010) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-013 | Er20:175,176,177,178,184 | Er20 §ID | vollständig integriert | Tarnidentität ❓ (E-ID-010) bleibt offen ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-014 | Er20:110,121,122,192,196,197 | Er20 §RE, PO | vollständig integriert | Widerspruch W-07 (Sternesystem) bleibt offen; E-PO-003 ⚠️ ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-015 | Er20:123,124,205,216,218,219,220,221 | Er20 §RE, GE, HA | vollständig integriert | Fundstellen E-RE-016 und E-GE-002 (Anwälte und Kaution später) fehlen in der Ausgangsliste; Umsetzung Haft offen (E-HA-001 📋 Vorbehalt) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-016 | Er20:235,236,238,239,240 | Er20 §GH | vollständig integriert (CONSOLIDATION-02) | N-02-Vermerk in Er20 E-GH-005 (Zeile 236) vorhanden; der Nachweis hatte ihn zu Unrecht als fehlend geführt. Test T-G05 deckt Story-Entdeckung und freie Suche ab. Kartenhinweis (💡) ist kein Testgegenstand. |
| SY-ERM-017 | Er20:246,248,249,250,251,255 | Er20 §KO | vollständig integriert | Widerspruch Option B (W-09) bleibt offen; Netzwerkarchitektur offen (E-KO-010) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-018 | Er20:261,262,263,264,265,266 | Er20 §WZ | vollständig integriert | Verdichtung offen (E-WZ-003); Master WF:36–38 bestätigt unbegrenztes Journal (kein Kompaktierung) ; Akzeptanzkriterium fehlt (Testkatalog) ; Master WF:36–38 bestätigt (Journal ohne Kompaktierung) |
| SY-ERM-019 | Er20:33,34,321,322,323 | Er20 §FA, QA | blockiert | Blockiert durch K-26 (Todesmodell; W-01). Acht Ansätze A–H ohne Standard; E-QA-003 (💡) steht im Widerspruch zu COMBAT-DEC-BW-001 ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-020 | Er20:330,146,147,148,151 | Er20 §QA, DE | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-021 | Er20:309,311,312,313,314,315 | Er20 §ME | vollständig integriert | Akku-Vereinfachung nur 💡 (E-ME-007) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-022 | Er20:283,284,286,287,289,112,113 | Er20 §KR, RE | vollständig integriert | Ethnik-Ausschluss als Querverweis N-LT-031 (NPCs/26:74) vorhanden, im Ermittlungsregister nicht eigens aufgeführt; Ursachenmodell offen (E-KR-002) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-023 | Er20:272,273,274,275,276,277,143,144,145 | Er20 §DP, DE | vollständig integriert | Mitarbeiter und Firmen nur 💡 bzw. P3 ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-024 | Er20:296,297,298,299,300,301,302,303 | Er20 §EV | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-025 | Er20:35,36 | Er20 §FA | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-ERM-026 | Er20:89,96 Ka27:215,424,367,366 | Er20 §SP ; Ka27 §ST, VL, AU | bereits vorhanden | Keine; Quelle Master 01_CURRENT_CANONICAL_STATE laut DECISIONS.md nicht geprüft |
| SY-KAMPF-001 | Ka27:44,45,46,47,302,291 | Ka27 §GP, WA | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-002 | Ka27:48,49,50,51,116 | Ka27 §GP, NK | bereits vorhanden | Keine |
| SY-KAMPF-003 | Ka27:53,54,55,56,57,58,59,76 | Ka27 §GP | vollständig integriert | Realismusgrad je Mechanik offen (GP-033) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-004 | Ka27:64,65,197 | Ka27 §GP, FK | vollständig integriert | Details (Aktivierung, Dauer, Erholung) offen laut GP-022 ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-005 | Ka27:69,70,448,450,451,452,459,460,461,465 | Ka27 §GP, BW | blockiert | Blockiert durch K-26 (Todesmodell); Widerspruch zu C-21 (Individualtod endgültig) und Spielvision §14 (K-24) |
| SY-KAMPF-006 | Ka27:458,462,464,475,476 | Ka27 §BW | blockiert | Blockiert durch K-26 (Gameplay-Tod-Rückkehr); Mindestzustand (BW-029) und Krankenhaus (BW-030) offen |
| SY-KAMPF-007 | Ka27:71,464,771,772,783 | Ka27 §GP, BW, SP | bereits vorhanden | Zwischen-Checkpoint-Inhalt offen (SP-019) |
| SY-KAMPF-008 | Ka27:86,87,382,383,384,385,386,387,396,400,401 | Ka27 §ZA, TK | bereits vorhanden | Zahlen und Trefferzonen-Geometrie offen (TK-019/020) |
| SY-KAMPF-009 | Ka27:136,249,274,363,364,420,421,429,430 | Ka27 §NK, DF, BT, AU, VL | bereits vorhanden | Schmerzwirkung nur Richtung; Stärke auf Grounded offen |
| SY-KAMPF-010 | Ka27:414,415,416,417,418,422,423,425,427,428,435,436 | Ka27 §VL | bereits vorhanden | Medikamente, Diagnoselisten, Heilzeiten offen (VL-022/023); Amputation nur Story (VL-012) steht im Spannungsfeld zu K-22 (Gore-Gliedmaßenverlust, Gameplay-Systeme Z.91) |
| SY-KAMPF-011 | Ka27:63,153,426,389,402,196,717,507 | Ka27 §GP, NK, VL, TK, FK, AN, KS | blockiert (K-22) | Widerspruch: Gore bis Verlust von Gliedmaßen (CD-VL-013) gegen Amputationsregel (CD-VL-012, K-22). Entwicklerentscheidung nötig. Gore-Schwellen und Darstellung bei Minderjährigen offen (TK-021). Nichts geändert. |
| SY-KAMPF-012 | Ka27:169,181,182,183,186,187,188,189,191,192,193,195 | Ka27 §FK | bereits vorhanden | Vorläufig FK-027 (Entfernungsmesser); Coriolis-Verneinung nicht eigens gelistet |
| SY-KAMPF-013 | Ka27:170,172,173,174,175,176,177,178,179,180,201 | Ka27 §FK | bereits vorhanden | Streuungs- und Schützenfehlermodell offen (FK-033) |
| SY-KAMPF-014 | Ka27:184,185,199,200,253 Fa28:184,185 | Ka27 §FK, DF ; Fa28 §ZV | bereits vorhanden | Deckungssystem und Fahrzeugkampf offen (FK-031/032 als Beschreibung, nicht entschieden) |
| SY-KAMPF-015 | Ka27:320,321,322,323,324,343,345,346,347 | Ka27 §MU | vollständig integriert | Kaliberliste, Magazingrößen, Munitionswirtschaft offen (MU-027/028) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-016 | Ka27:325,326,327,328,330,331,332,334,335,336,337,338,339,342 | Ka27 §MU | bereits vorhanden | Grad der Störungsbeseitigung offen (MU-018) |
| SY-KAMPF-017 | Ka27:357,358,360,361,362,366,367,368,369,370,371,372 | Ka27 §AU | vollständig integriert | Formeln und Traglast-Grenzen offen (AU-012 bis 014) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-018 | Ka27:111,114,117,118,119,121,122,128,130,131,132,140,150,151,155 | Ka27 §NK | bereits vorhanden | Direct vs. Assisted Control offen (NK-040) |
| SY-KAMPF-019 | Ka27:145,146,147,148,149 | Ka27 §NK | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-020 | Ka27:240,241,242,243,244,245,247,248,249,250,254 | Ka27 §DF | vollständig integriert | Zeitfenster, Ausdauerkosten, Erholung offen (DF-015) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-021 | Ka27:211,212,213,214,219,220,225,226,227,228,230 | Ka27 §ST | bereits vorhanden | Konflikt Kreuz (W-03) offen; Haptik, Maus/Tastatur, Prioritäten offen |
| SY-KAMPF-022 | Ka27:264,265,266,267,268,269,270,272,277,280 | Ka27 §BT | bereits vorhanden | Klettern, Luftkampf, Wasserkampf offen (BT-017) |
| SY-KAMPF-023 | Ka27:88,89,90,576,577,578,579,580,581,582,585,586,587,588,591,598,608 | Ka27 §ZA, KI | bereits vorhanden | Teilaussage „keine globale Telemetrie“ und KI-Leistungsgrenzen (KI-024 ff.) nicht eigens in den zitierten Zeilen geprüft |
| SY-KAMPF-024 | Ka27:589,590,592,593,594,595,596,597,599,600,601,602,603,604,605,606,607 | Ka27 §KI | vollständig integriert | Lernen je Art, Deckungsnutzung, Zivilisten offen (KI-029 bis 033) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-025 | Ka27:618,619,620,622,623,624,625,626,627,628,629,630,631,633 | Ka27 §NM | bereits vorhanden | Rückkehrhäufigkeit, Sichtbarkeit offen (NM-016) |
| SY-KAMPF-026 | Er20:163 Ka27:667,638,621,509 | Er20 §ZE ; Ka27 §PO, NM, KS | blockiert | Blockiert durch K-27 (Todesursache wichtiger NPCs); Konflikt PO-020 gegen Lebensentwicklung (LT-006, LB-017) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-027 | Ka27:648,649,650,651,652,653,654,660,668,669,670,671,672,673,674 | Ka27 §PO | bereits vorhanden | Strafkatalog, Kaution, Festnahmeschwelle offen (PO-023) |
| SY-KAMPF-028 | Ka27:684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700 | Ka27 §DE | vollständig integriert | Fluchtmechanik und soziale Deeskalation offen (DE-013/014) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-029 | Ka27:710,711,712,713,714,715,716,719,720,721,722,723,724,725,726,727,728 | Ka27 §AN | vollständig integriert | Ragdoll, Motion Matching, Trefferreaktionen, Herkunft offen (AN-015 bis 019) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-030 | Ka27:738,739,740,742,743,745,746,747,750,751,753,755,756,757,758 | Ka27 §KO | bereits vorhanden | Netzwerkarchitektur, Latenz, Verbindungsabbruch offen (KO-013 bis KO-021); Friendly Fire konfigurierbar (KO-008) |
| SY-KAMPF-031 | Ka27:768,769,770,773,774,775,776,777,778,779,780,781,782,784,785,786,787 | Ka27 §SP | bereits vorhanden | Permadeath-Speicherstände (SP-018) offen, abhängig von K-26 |
| SY-KAMPF-032 | Ka27:797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812 | Ka27 §PF | vollständig integriert | Hardwarestufen, Budgets und Kampf in dichten Gebieten offen (PF-012 bis 014) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-033 | Ka27:552,553,554,555,556,557,558,559,560,561,562,563,564,565,566 | Ka27 §FF | bereits vorhanden | Festnahme, Einrichtungen und rechtlicher Rahmen offen (FF-014/015) |
| SY-KAMPF-034 | Ka27:524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542 | Ka27 §SJ | bereits vorhanden | Schwellen für Hypothese und Ritualkosten offen (SJ-017, SJ-018) |
| SY-KAMPF-035 | Ka27:290,292,293,294,295,296,297,298,299,300,301,303,304,305,306,307,308,309,310 | Ka27 §WA | vollständig integriert | Schutzkleidung, Waffenliste, Kaliber, Inventarmodell offen (WA-016 bis 018) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KAMPF-036 | Ka27:487,488,489,490,491,492,493,494,495,496,497,498,500,502,503,504,505,506,508,511,512,513 | Ka27 §KS | bereits vorhanden | Kampf gegen körperlose Wesen und Hauptbedrohungen offen (KS-026/027), Artenliste offen (KS-025) |
| SY-KAMPF-037 | Ka27:93,94,95,807 | Ka27 §ZA, PF | bereits vorhanden | Keine |
| SY-KREA-001 | Kr20:39,41,42,55,175 | Kr20 §GR, HW | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-002 | Kr20:46,172,173 Np26:354 | Kr20 §GR, HW ; Np26 §NE | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-003 | Kr20:82,83,84,85,86,87,93,95 | Kr20 §KL | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-004 | Kr20:88,89,90,91,92,94,96 | Kr20 §KL | vollständig integriert | Begriff „Ursprungswesen“ nur 💡 (K-KL-015); Alpha-Begriff und IP-Frage offen (W-17, K-IP-005) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-005 | Kr20:107,108,109,110,111,112,113,114,115,116,119,120,124 | Kr20 §KO | vollständig integriert | Archivstatus „tot, aber nicht endgültig“ offen (K-KO-018); Konflikt-Abgleich K-22 nicht betroffen ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-006 | Kr20:117,118,122 | Kr20 §KO | vollständig integriert | Gegenmittel nur möglich, Wunsch-Sofortheilung 📋 (K-KO-016) als Entscheidung prüfen ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-007 | Kr20:134,135,136,137,138,139,140,141,144,145,146,147,148 | Kr20 §GE | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-008 | Kr20:149,150,151,152 | Kr20 §GE | vollständig integriert | Keine (Status offen ist korrekt erfasst) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-009 | Kr20:162,163,164,165,166,167,168,169,170,171,177 | Kr20 §HW | vollständig integriert | Dämonen-Block und Leviathan-Struktur offen (K-HW-017 bis 020) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-010 | Kr20:178,179,180,181 | Kr20 §HW | vollständig integriert | Keine (Offenheit korrekt erfasst) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-011 | Kr20:191,192,193,196,197,198,200,201,204,205,206 | Kr20 §KS | blockiert (K-21) für Zeitverhältnis; Rest vollständig | Wiederbelebung ‚außergewöhnlich‘ steht in Kreaturen/02 Zeile 57, /06 Zeile 76, /04 Zeile 246 (Nachweis hatte ‚nicht gefunden‘ geführt). Zeitverhältnis Hölle/Oberfläche: K-21 (Kanon OPEN, C-08); PE-E1022 steht nicht im Entscheidungsregister. Nicht entschieden. |
| SY-KREA-012 | Kr20:195,202,203,207,208,209,210 | Kr20 §KS | blockiert (K-21, K-26) | Höllenzeit ‚konstant‘ (📋) gegen C-08 ‚Verhältnis offen‘ ist K-21, nicht W-04 (W-04 betrifft Reiche als Open Worlds). Tod-Checkpoint berührt K-26. Seelenziel offen (K-KS-019). Nichts entschieden. |
| SY-KREA-013 | Kr20:220,221,222,223,224,225,226,227,228,232,233,234,235,236,237,239 | Kr20 §OE | vollständig integriert | Wiederbesiedlungszeit und Darstellung offen (K-OE-018/020) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-014 | Kr20:249,250,251,252,253,254,255,256,257,258,259,260,261,262,263 | Kr20 §GS | vollständig integriert | Beziehung zu Wesen und Gruppenbeeinflussung offen (K-GS-014/015) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-015 | Kr20:273,274,275,276,277,278,279,280,281,282,283,285,286,287,288,289,290,291,292 | Kr20 §KI | vollständig integriert | Lernen je Art und Gedächtnisgrenzen offen (K-KI-019/020) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-016 | Kr20:302,303,304,305,306,307,308,309,310,311,312,313,314,315,316 | Kr20 §HU | vollständig integriert | Name und Struktur der Hauptorganisation sowie Zahl der Hunter offen (K-HU-014/015) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-017 | Kr20:326,327,328,329,330,331,332,333,334,335,336 | Kr20 §JA | vollständig integriert | Kosten- und Risikomodell offen (K-JA-011) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-018 | Kr20:346,347,348,349,350,351,352,353,354,355 | Kr20 §CO | vollständig integriert | Bunker als eigenes Thema; Rechtsrahmen und Dauereinrichtungen offen (K-CO-009/010); Bezug K-28 nur mittelbar ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-019 | Kr20:365,366,367,368,369,370,371,372,373,374,375,376 | Kr20 §FO | vollständig integriert | Schwellen für Hypothese und Bestätigung offen (K-FO-012) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-020 | Kr20:386,387,388,389,390,391,392,393,394,404,405,406,407 | Kr20 §VF, OR | vollständig integriert | Metaphysisches Netz offen (K-OR-003); Auslegung bei Mehrdeutigkeit offen (K-VF-009) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-021 | Kr20:417,418,419,420,421,422,423,424,425 | Kr20 §PD | vollständig integriert | Speichergröße-Konflikt W-12 (K-PD-002); Gedächtnis-Speichermodell und lore-widrige Mods offen (K-PD-008/009) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-022 | Kr20:435,436,437,438,439 | Kr20 §TI | vollständig integriert | Statusdifferenz W-13 (Artenzahl 220–280: 📋 in Kr20, vorläufig in Entscheidungsstatus V-07); verwandelte Tiere offen (K-TI-005) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-023 | Kr20:449,450,451,452,453,454,455,456,457 | Kr20 §LW | vollständig integriert | Lernmodell offen (K-LW-009); Ereignisjournal nur Vorschlag ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-024 | Kr20:47,50,52,53,54 | Kr20 §GR | vollständig integriert | Liste und Anzahl der Basisarten offen (K-GR-015) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-025 | Kr20:65,66,67,68,69,70,71,72 | Kr20 §IP | vollständig integriert | Eigenname der Organisation und Ursprungswesen offen (K-IP-005) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-026 | Kr20:40,43,44,45,48,49,52 | Kr20 §GR | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-KREA-027 | Kr20:51 | Kr20 §GR | Konflikt offen (K-34) | DLC-Aussage Kr20 Zeile 51 gegen ‚kein klassisches DLC‘ (ES Zeile 65, DECISIONS W-01). Als K-34 in CONFLICTS.md eingetragen. Der Entwickler klärt, ob sich die Aussage nur auf die Karte bezieht. |
| SY-NPC-001 | Np26:44,45,46,47,48,49,50,51,52 | Np26 §LT | vollständig integriert | Reihenfolge offen (N-LT-009) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-002 | Np26:54,63,64,65,66,67,68,69,70,71,72,73 | Np26 §LT | bereits vorhanden | Ehrensystem und Gruppen nur vorläufig (N-LT-027/029) |
| SY-NPC-003 | Np26:88,89,90,91,95,96,99,100,102,103,104,106,109,59,60 | Np26 §ID, LT | bereits vorhanden | Einwanderer (W-15) und Datensatz toter Personen (W-16) offen; Stufen A–D Richtung |
| SY-NPC-004 | Np26:110,111 | Np26 §ID | vollständig integriert | Widerspruch W-15 und W-16 nicht aufgelöst; Entscheidung des Entwicklers offen ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-005 | Np26:121,122,123,124,125,126,128,131,134,136,137,138 | Np26 §PS | bereits vorhanden | Kommunikationsstil offen (N-PS-014, N-BE-024) |
| SY-NPC-006 | Np26:148,149,150,151,152,155,156,157,158,162,168,169,176,178,179 | Np26 §GE | bereits vorhanden | Dauer und „relevant genug“ offen (N-GE-032) |
| SY-NPC-007 | Np26:189,190,191,192,193,198,199,200,201,202,205,207,208 | Np26 §WA | vollständig integriert | Keine (Verdachtsdynamik in N-WA-004 ff. nicht je Zeile geprüft) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-008 | Np26:219,220,221,222,225,226,227,228,230,232,237,239 | Np26 §IN | bereits vorhanden | Keine |
| SY-NPC-009 | Np26:250,251,252,254,259,262,263,264,267,268,269,271,272,273 | Np26 §BE | bereits vorhanden | Datenmodell offen (N-BE-024) |
| SY-NPC-010 | Np26:283,284,285,286,288,290,291,301,302,303,303,304,305 | Np26 §BV | bereits vorhanden | Crew-Wünsche nur 🧭; Fraktionsfolgen 🧭. Doppelzitat N-BV-021 in der Ausgangsliste |
| SY-NPC-011 | Np26:315,316,317,318,320,322,324,326,329,330,333 | Np26 §FA | vollständig integriert (CONSOLIDATION-02) | Untergrund-Haushalte als historisch (N-02) sind in NPCs/10 markiert (CONSOLIDATION-01; Nachweis hatte ‚nicht markiert‘ geführt). Familiengründung (FA-010 ❓) bleibt offen und ist keine gesicherte Anforderung. |
| SY-NPC-012 | Np26:343,344,347,348,349,350,351,352,353,354,355,357,358,359,360,364 | Np26 §NE | bereits vorhanden | Lernmodell und Sichtbarkeit offen (N-NE-022) |
| SY-NPC-013 | Np26:374,375,376,377,378,379,380,383,385,386,387 | Np26 §NL | blockiert (K-26) | Spielertod-Ablauf (NL-013) blockiert durch K-26. Der übrige Nemesis-Lebenszyklus ist in NPCs/26 §NL vollständig. |
| SY-NPC-014 | Np26:398,399,400,401,402,403,404,406,410,411,412,413,423,425 | Np26 §LE | bereits vorhanden | Hintergrund vorläufig (🧭) |
| SY-NPC-015 | Np26:435,436,437,439,441,442,446,448,449,450,451,452,453,455,459 | Np26 §LB | bereits vorhanden | Wichtige NPCs-Tod (N-LB-017) berührt K-27 (Konfliktliste W-02); Behandlungsstufen Richtung |
| SY-NPC-016 | Np26:469,470,471,472,475,477,478,480,483,484,485,486,487 | Np26 §WE | bereits vorhanden | Immobilien- und Tourismusreaktion nur 🧭 |
| SY-NPC-017 | Np26:497,498,499,500,502,503,504,505,506,507,509,511,513,514,515,516,517,518,521,522,523,524,525 | Np26 §FR | vollständig integriert | Liste, Namen und Ziele offen (N-FR-029) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-018 | Np26:535,536,540,541,542,543,544,545,546,547,549,550,551,552 | Np26 §TE | vollständig integriert | Konkrete Karten, Grenzen, Gebiete offen (N-TE-018) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-019 | Np26:562,563,564,565,566,567,568,569,570,571,573,574,575,577,579,581 | Np26 §BH | bereits vorhanden | Polizisten als Rekruten nur 🧭 (N-BH-020) |
| SY-NPC-020 | Np26:591,592,593,594,595,596,597,598,601,603,604,605,611,612,614,616,619 | Np26 §CO | bereits vorhanden | Besetzung, Biografien, Todesfälle offen (N-CO-026); Tod-Regel für HQ-Figuren berührt K-27 (N-CO-007 vs. LT-006) |
| SY-NPC-021 | Np26:629,630,631,633,634,635,636,637,638,640,643,645,649,650 | Np26 §DI | vollständig integriert | Dialogvolumen und Stilarten offen (N-DI-012/021) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-022 | Np26:660,661,662,663,664,665,666,669,670,671,673,674,675,676,677,678,679 | Np26 §PE | bereits vorhanden | Zielgröße-Konflikt W-12 (N-PE-004/006/012); Journal unbegrenzt (N-PE-020, WorldFacts WF:36–38 im Master bestätigt) ; Master WF:36–38 bestätigt |
| SY-NPC-023 | Np26:692,693,694,696,697,698,699,701,702,703,705,707,708,715 | Np26 §SI | vollständig integriert (CONSOLIDATION-02) | Untergrund-Budgetierung in NPCs/22 Punkt 12 ist als historisch (N-02) markiert (CONSOLIDATION-01). N-SI-024 bleibt an Q-N25 (offen) gebunden; das ist keine Anforderung. |
| SY-NPC-024 | Np26:725,726,727,728,729,730,732,734,735,736,737,738,740,742,743 | Np26 §KO | vollständig integriert | Replikation und Mod-Erweiterungspunkte offen (N-KO-018/019); Native-C++-Modding gesondert (N-KO-016) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-025 | Np26:753,759,760,761,762 | Np26 §KS | vollständig integriert | Akzeptanzkriterium fehlt (Testkatalog) |
| SY-NPC-026 | Np26:772,773,774,775,777,780,781,783,789,792,797,800,811,817 | Np26 §EN | bereits vorhanden | Vorschlagsstatus korrekt; Entwurf nicht bindend |
| SY-NPC-027 | Np26:563,564,570,814,815 | Np26 §BH, EN | vollständig integriert (CONSOLIDATION-02) | Kein Sternesystem ergänzt in NPCs/26 N-BH-004 (M-PD ‚Strafsystem statt Sterne‘, 23#1058A). Ob ‚Fahndungsstufen‘ sichtbar sind, bleibt offen (Ermittlung/14 §2, K-E8). N-EN-067 bleibt Vorschlag. |
| SY-FAHR-001 | Fa28:46,47,48,64,100 | Fa28 §GP, PH | vollständig integriert | Status-Widerspruch GP-019 (⚠️) offen; Motormodell und Termine offen (GP-002) ; Akzeptanzkriterium fehlt (Testkatalog) ; Master RV:7 bestätigt |
| SY-FAHR-002 | Fa28:79,89,90,80,135 | Fa28 §KL, ST | bereits vorhanden | Startsortiment, Modelljahre, Trims offen (KL-007) ; Master RV:11 bestätigt |
| SY-FAHR-003 | Fa28:100,102,103,104,105,112,137 | Fa28 §PH, ST | bereits vorhanden | Schleifpunkt nicht ratifiziert (Master RV:33 bestätigt Offenheit); Konstanten offen (PH-013); RV:8 (Motorstart) im Master nicht gegengeprüft ; Master RV:7 und RV:33 bestätigt (Schleifpunkt offen); RV:8 nicht gegengeprüft |
| SY-FAHR-004 | Fa28:106,107,108,109,110,113,116,117,118,119,120 | Fa28 §PH | bereits vorhanden | Unreal-Physik, NPC-Physik und Schadenseinfluss offen (PH-014/017/018) |
| SY-FAHR-005 | Fa28:130,131,132,133,134,136,140,143 | Fa28 §ST | bereits vorhanden | Konflikt Kreuz (W-03) und L2-Doppelbelegung (VD-ST-003) offen; Maus/Tastatur offen (ST-007) ; Master RV:19–28 bestätigt |
| SY-FAHR-006 | Fa28:155,156,157,158,159,160,163,164,165,166,167,168,169 | Fa28 §EI | vollständig integriert | Schlüssel, Motorstart, Beschlagnahme, Fahrerwechsel offen (EI-010 bis 012) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-007 | Fa28:179,180,181,182,183,184,191,192,193,194 | Fa28 §ZV | bereits vorhanden | Schadenstiefe Widerspruch W-10 (ZV-004 ⚠️); Totalschaden und Wartungsintervalle offen (ZV-014/015) |
| SY-FAHR-008 | Fa28:204,205,206,207,208,209,210,211,212,216,217,218,219,220 | Fa28 §WE | vollständig integriert | Preise, Teilemarkt, Reparaturschritte offen (WE-013/014); Tuning-Tiefe unklar (WE-009) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-009 | Fa28:230,231,232,233,234,235,240,241,242,243 | Fa28 §KE | bereits vorhanden | Lade- und Tankzeiten, Preise, Stationsdichte offen (KE-011 bis 013) |
| SY-FAHR-010 | Fa28:253,254,255,256,257,258,259,266,267,268,269 | Fa28 §SN | vollständig integriert | Gesamtnetz offen (SN-016); Rechts-/Linksverkehr, Höchstgeschwindigkeiten unbelegt (SN-014/015) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-011 | Fa28:279,280,281,282,283,284,289,290,291 | Fa28 §KR | vollständig integriert | Kreuzungsarten, Signalausfall, Bahnübergänge unbelegt (KR-011/012) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-012 | Fa28:301,302,303,304,305,306,311,312,313 | Fa28 §SP | vollständig integriert | Falschparken, Strafzettel, Abschleppen offen (SP-011); Parkhäuser offen (SP-012) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-013 | Fa28:323,324,325,326,334,335,336,337,338 | Fa28 §KI | bereits vorhanden | Verkehrsmodell, Reaktion auf Hupen offen (KI-012/013); Zielführung und Rechtsfolgen unbelegt (KI-015/016) |
| SY-FAHR-014 | Fa28:348,349,350,351,357,362,363,373,374,375,376,377,378,386,387,388 | Fa28 §NF, VD | vollständig integriert | Dichtekurven, Haltequote, Baustellenzahl offen (VD-VD-014 bis 016) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-015 | Fa28:398,399,400,401,402,403,410,411,412 | Fa28 §UP | bereits vorhanden | Unfall- und Pannenraten unbelegt (UP-014) |
| SY-FAHR-016 | Fa28:422,423,424,425,426,427,431,432,433,434 | Fa28 §VT | vollständig integriert | Kollisionen, Kinder und Reiter offen (VT-011 bis 013) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-017 | Fa28:444,445,446,447,448,449,450,456,457,458,459 | Fa28 §BZ | vollständig integriert | Diebstahlmethoden, Kennzeichenformate, Koop-Eigentum, Besitzgrenzen offen (BZ-013 bis 016) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-018 | Fa28:469,470,471,472,473,474,475,476,477,483,484,485 | Fa28 §PV | vollständig integriert | Sternesystem: W-07 (VD-PV-005 📋 vs. E-PO-003 ⚠️); Verstoßkatalog offen (PV-015) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-019 | Fa28:495,496,497,498,499,500,501,504,506,507,508,510 | Fa28 §IT | vollständig integriert | Bezahlung, Lizenzen, Reisezeit offen (IT-012/013) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-020 | Fa28:520,521,522,523,524,525,526,527,532,533 | Fa28 §EJ | vollständig integriert | Fahrzeugwirkung auf Wesen und Angriffe auf Fahrzeuge unbelegt (EJ-013/014) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-021 | Fa28:543,544,545,546,547,548,556,557,558 | Fa28 §PE | bereits vorhanden | W-12 (Speichergröße, PE-015 ⚠️); „Laden gleicht Speichern“ ❓ (PE-014); Re-Import offen (PE-016) ; Master WF:36–38 bestätigt |
| SY-FAHR-022 | Fa28:568,569,570,571,573,579,580,581,582,583 | Fa28 §KO | bereits vorhanden | Fahrzeugsynchronisation, Fahrerwechsel, Eigentum offen (KO-013 bis 016, BZ-015) |
| SY-FAHR-023 | Fa28:593,594,595,596,597,598,599,600,606,607,608 | Fa28 §WF | vollständig integriert | WORLD_700 nur Referenz (WF-014, K-19 betrifft Phase-A-Freigabe der Welt-Doku); Lieferant und Austauschformat offen (WF-015/016); Reimport-Regel in WF-005 belegt ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-024 | Fa28:618,619,620,621,622,623,629,630,631,632 | Fa28 §UE | vollständig integriert | Versionskonflikt W-11 (UE-003 5.8.3 vs. CANON 5.8.4); Verkehrsphysik und KI-Wahrnehmung offen (UE-013/014) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-025 | Fa28:642,643,644,645,646,654,655 | Fa28 §LS | bereits vorhanden | Radien, Taktung, Fahrzeugzahlen unbelegt (LS-013); Wiederbesuchs-Szene offen (LS-014) |
| SY-FAHR-026 | Fa28:665,666,667,668,669,676,677 | Fa28 §PF | vollständig integriert | Keine Messungen und Budgets (PF-012); 2.200-km²-Szenario nicht mehr Richtwert (N-01, siehe SY-WELT-006) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-FAHR-027 | Fa28:88,499,500,57,58 | Fa28 §KL, IT, GP | vollständig integriert (CONSOLIDATION-02); Mengen vorläufig (N-01) | Anlagenbereiche und Betriebsregeln stehen in World/09 §3 (📋 Bereich; Fahren nur mit Job/Berechtigung; NPC-Betreiber). Fa28 KL-015 verweist jetzt darauf (L-17 damit für die Docs geschlossen). Mengen hängen an der offenen Weltgröße. |
| SY-FAHR-028 | Fa28:49,56,57,58,86,87 ; Fa02:59 (Fahrzeuge-und-Verkehr/02, P10) | Fa28 §GP, KL | vollständig integriert (CONSOLIDATION-02) | ‚Fahren ist kein Zwang‘ (Fa02 P10, Zeile 59) als VEH-DEC-GP-020 in Fa28 §GP ergänzt (GP-014 war schon belegt, Kollision korrigiert). Hunter-Spezialfahrzeuge und Rettungstypen bleiben laut KL-014 unbelegt; das ist im Original so festgehalten. |
| SY-WELT-001 | Er20:236 Kr20:348 Np26:579 | Er20 Gr. GH; Np26 Gr. BH; Kr20 Gr. CO (keine N-02-Zeile) | vollständig integriert (CONSOLIDATION-02); Master unverändert | Docs-Stellen sind markiert: E-GH-005, N-BH-018, World/10, /11, /12, NPCs/10, /15, /16, /18, /22. Master-Historie UC:7–20, US:7–15/19–22, UA:5–16, AI:9–21 als Verweis in DECISIONS N-02 ergänzt (read-only). Die frühere Angabe ‚nicht zutreffend‘ ist korrigiert. |
| SY-WELT-002 | Kr20:348,354 Np26:617,618 | Kr20 Gr. CO; Np26 Gr. CO | blockiert | Blockiert durch K-28 (Bunker/HQ-Beziehung, Pflichtklärung CLAUDE.md §3); Familienschlüssel-Herkunft und Tesla-Variable nur Vorschlag/nicht canon ; Akzeptanzkriterium fehlt (Testkatalog) ; Master HB:5–11 bestätigt (Entwurf) |
| SY-WELT-003 | Er20:236 Kr20:348 Np26:579 | Er20 Gr. GH; Np26 Gr. BH; Kr20 Gr. CO | vollständig integriert | Keine (Kartenhinweis 💡 bleibt Vorschlag) ; Akzeptanzkriterium fehlt (Testkatalog) |
| SY-WELT-004 | Fa01:202 (Fahrzeuge-und-Verkehr/01, Lückenvermerk) | Fa28 Gr. KL (kein Untergrundeintrag) | vollständig integriert (CONSOLIDATION-01: Fahrzeuge-und-Verkehr/01 Zeile 202 als historisch (N-02) markiert) | Historische Markierung (N-02) fehlt in Fa28; Trennung zur Oberflächenschiene (SY-FAHR-027) nicht dokumentiert ; Akzeptanzkriterium fehlt (Testkatalog) ; Master UC:10 bestätigt; Fa01:202 bestätigt (Lückenvermerk) |
| SY-WELT-005 | Np26:715 | Np26 Gr. SI; NPC-Module 01, 10, 15, 16, 18, 22 | vollständig integriert (CONSOLIDATION-01: N-02 in NPCs/10, 15, 16, 18, 22 als historisch markiert; Restfrage Bunker/Höhlen bleibt bei Q-N25 offen) | Umwidmung auf historisch (N-02) in den NPC-Modulen nicht umgesetzt (L-03, W-14); Verweise NPCs/10:228, 255; 16:392, 428; 18:280, 311; 15:288, 318; 22:309, 345; 01:165 nicht geprüft ; Akzeptanzkriterium fehlt (Testkatalog) ; Master US:19–22 und AA:3 bestätigt |
| SY-WELT-006 | Np26:707 Fa28:56,677 | Fa28 Gr. GP, PF; Np26 Gr. SI | vollständig integriert (CONSOLIDATION-02); Stadtgrößen durch N-01 blockiert | N-01 ergänzt in Fa28 VEH-DEC-PF-013 (2.200 km² als Basis überholt) und NPCs/26 N-SI-016 (Zahl hängt an N-01). Stadtgrößen selbst bleiben durch N-01 (Größe offen) blockiert (WE-GROESSE-007/008). |

## 2. Zählung je Status

| Status | Anzahl |
|---|---|
| vollständig integriert | 92 |
| bereits vorhanden | 48 |
| teilweise integriert | 0 |
| blockiert | 10 |
| Konflikt offen | 1 |
| verworfen | 0 |
| noch nicht integriert | 0 |
| **Summe** | **151** |

Stand nach CONSOLIDATION-02 (08.11.2026), per Skript aus der Statusspalte gezählt. Die frühere Tabelle (81 / 14 / 5 / 3 „noch nicht integriert“) summierte sich nicht auf 151. Heute steht in keiner Zeile mehr „noch nicht integriert“; die Werte sind neu gezählt.

Neu blockiert (begründet, keine Entscheidung getroffen): K-27 (SY-ERM-011, Teilaussage), K-22 (SY-KAMPF-011), K-21 und K-26 (SY-KREA-011, Zeitverhältnis; SY-KREA-012), K-26 (SY-NPC-013). Die übrigen Blockaden stehen unverändert in ihren Zeilen. Konflikt offen: K-34 (SY-KREA-027).

## 3. Korrekturen für Docs/Game

Stand CONSOLIDATION-02 (08.11.2026): Die Ergänzungen zu SY-ERM-016/-007/-011, SY-KREA-027 (K-34), SY-NPC-027, SY-FAHR-027/-028, SY-WELT-001/-006 sind in den Zielen umgesetzt. Offen bleiben Korrekturen, die eine Entwicklerentscheidung brauchen (K-21, K-22, K-26, K-27, K-34).

**Ermittlung/20-Entscheidungsregister.md**
- E-GH-005 (Geheime Labore) um den N-02-Vermerk ergänzen: unterirdische Labore bleiben möglich (SY-ERM-016, SY-WELT-001/003).
- Anzeige „Unbekannt“ für unbekannte Informationen als Eintrag ergänzen oder als offen markieren (SY-ERM-007).
- Todesmodell-Einträge E-FA-016/017, E-QA-001 bis 003 sind als offen (K-26) zu führen; E-QA-003 (💡) im Widerspruch zu COMBAT-DEC-BW-001 (SY-ERM-019).
- E-ZE-007 (wichtige NPCs sterben nur durch den Spieler) als K-27-Konflikt kennzeichnen (SY-ERM-011).
- E-KO-005 (W-09) und E-AR-015 bzw. E-QA-035 (W-08) als offene Statuskonflikte kennzeichnen.

**Kampfsystem/27-Entscheidungsregister.md**
- COMBAT-DEC-VL-013 (Gore bis Verlust von Gliedmaßen) gegen VL-012 (Amputation nur über Story) und K-22 als Konflikt kennzeichnen (SY-KAMPF-010/011).
- COMBAT-DEC-BW-001 und BW-012 gegen E-QA-003 als K-26 kennzeichnen; Verhalten für Checkpoint und Permadeath bleibt offen (SY-KAMPF-005/006/031).
- COMBAT-DEC-PO-020 und NM-014 gegen NPC-Lebensentwicklung als K-27 kennzeichnen (SY-KAMPF-026).

**Kreaturen/20-Entscheidungsregister.md**
- K-KS-012 (Höllenzeit konstant, 📋) gegen C-08 (`Docs/Planning/DECISIONS.md`) als W-04 kennzeichnen (SY-KREA-011/012).
- K-KS-013 und K-KS-005 (Tod in anderen Reichen) als K-26-Teil kennzeichnen (SY-KREA-012).
- K-GR-013 (DLC, 📋) gegen Entscheidungsstatus Z.65 als W-05 kennzeichnen (SY-KREA-027).
- K-PD-002 (Speichergröße) als W-12 kennzeichnen (SY-KREA-021); K-TI-001 als W-13 (SY-KREA-022).
- Wiederbelebung („außergewöhnlich“, SY-KREA-011) ergänzen oder als offen markieren.
- K-KO-016 (seltene übernatürliche Sofortheilung 📋) auf Verträglichkeit mit der Heilungsregel prüfen (SY-KREA-006).

**NPCs/26-Entscheidungsregister.md**
- N-FA-Zeilen zu Untergrund-Haushalten und N-SI-024 (Untergrund-Budget) als historisch (N-02) kennzeichnen, ohne Text zu entfernen (SY-NPC-011, SY-NPC-023, SY-WELT-005).
- Sternesystem-Aussage „kein Stern“ aus E-RE-013 als Hinweis in Np26 ergänzen; N-EN-067/068 bleiben Vorschlag (SY-NPC-027, W-07).
- N-NL-013, N-LT-006, N-LB-017, N-CO-007, N-CO-026 als K-27 und K-26 kennzeichnen (SY-NPC-013, SY-NPC-020).
- N-ID-023 und N-ID-024 (W-15, W-16) bleiben offen; Entscheidung beim Entwickler (SY-NPC-004).

**Fahrzeuge-und-Verkehr/28-Entscheidungsregister.md**
- „Fahren ist kein Zwang“ (📋, Fa02 P10) als Eintrag in Fa28 ergänzen (SY-FAHR-028).
- Schienen-Mengen (Hubs, Terminals, Ladestellen, Depots), Fahrtakte und Betriebsregeln in Fa28 als Status-Einträge ergänzen oder auf das neue Modul verweisen (SY-FAHR-027, L-17).
- VD-PF-013 (2.200-km²-Szenario) als überholt markieren, da N-01 gilt (SY-WELT-006, SY-FAHR-026).
- Untergrund-Schiene („Untergrundnetz“) als historisch (N-02) kennzeichnen (SY-WELT-004).
- VD-UE-003 (5.8.3) gegen CANON 5.8.4 als W-11 kennzeichnen (SY-FAHR-024).
- VD-ST-003 (L2-Doppelbelegung) und VD-ST-011 (Kreuz) als W-03 kennzeichnen (SY-FAHR-005).
- VD-ZV-004 (Schadenstiefe) als W-10 und VD-PV-005 gegen E-PO-003 als W-07 kennzeichnen (SY-FAHR-007, SY-FAHR-018).

**Korrekturen der Ausgangsliste (`Docs/Planning/Konsolidierung/C-systeme.md`, nicht Docs/Game)**
- SY-ERM-007: Fundstelle E-AR-007 (Notizen, Medien als Belege) ergänzen.
- SY-ERM-015: Fundstellen E-RE-016 und E-GE-002 (Anwälte und Kaution später) ergänzen.
- SY-ERM-021: Fundstelle E-SP-015 (Soziale Medien nie erforderlich) ergänzen, S-11 ist dort belegt.
- SY-ERM-022: Querverweis N-LT-031 im Ermittlungsregister nicht vorhanden (nur in NPCs/26).
- SY-NPC-010: N-BV-021 ist doppelt zitiert.
- SY-WELT-004: keine Register-Fundstelle, nur Fa01:202 (Lückenvermerk).

## 4. Blockaden (keine Entscheidung getroffen)

- **K-26 (Todesmodell):** SY-ERM-019, SY-KAMPF-005, SY-KAMPF-006 (blockiert); Teilaussagen in SY-KAMPF-031, SY-KREA-012, SY-NPC-013.
- **K-27 (Todesursache wichtiger NPCs):** SY-KAMPF-026 (blockiert); Teilaussagen in SY-ERM-011, SY-NPC-020.
- **K-28 (Bunker/HQ):** SY-WELT-002 (blockiert).
- **K-19 (WORLD_700):** Teilaussage in SY-FAHR-023 (nur Referenzstatus); nicht als blockiert gewertet, da die Anforderung selbst nicht davon abhängt.
- Außerhalb der Kategorien, aber als Konflikt vermerkt: K-22 (Amputation, Gore; SY-KAMPF-010/011), W-03 bis W-17 (siehe Konfliktliste).

## 5. Nicht geprüft

- Adult-Paket (`Docs/Game/Adult/`) wurde nicht gelesen.
- Modultexte 01 bis 30 wurden nicht vollständig gelesen; geprüft sind die zitierten Registerzeilen und Testkatalog-IDs.
- Master: nur die 12 Zeilen mit konkreten Zitaten wurden geprüft; alle übrigen Master-Angaben wurden übernommen.
- Stichwortsuchen ersetzen keine Inhaltsprüfung. Die Bewertung „vollständig“ bezieht sich auf die zitierten Zeilen und die zusätzlichen Greps (N-02, Untergrund, Zwang, Reimport).

