# Vollständigkeitsnachweis Gameplay und Spielvision (GP und SV)

**Stand:** 08.11.2026
**Bearbeiter:** Agent (Claude Haiku 5.5), Auftrag Nachweis Gameplay
**Umfang:** 222 Anforderungen aus `Docs/Planning/Konsolidierung/A-gameplay-spielvision.md`: 137 GP-Anforderungen und 85 SV-Anforderungen. Jede Zeile ist einzeln geführt, ohne Stichprobe.
**Vorgehen:** Jede Anforderung wurde mit ihrer Ausgangszeile gelesen und die angegebene Fundstelle in `Docs/Game/Gameplay-Systeme.md` (GS) bzw. `Docs/Game/Spielvision.md` (SV) geöffnet, einschließlich der Nachtrag-Abschnitte (GS ab Zeile 392, SV ab Zeile 430). Die Zielinhalte wurden inhaltlich verglichen, nicht über Stichworte. Master-Fundstellen wurden nur punktuell geöffnet: `CANONICAL_ADDENDUM_2026-09-05.md` (Zeilen 36–56 vollständig, 39–50 einzeln), `BLOCK_09_STATUS.md:5`, `CANONICAL_ADDENDUM_2026-09-06.md` (Hölle/Purgatorium), `MODDING_ARCHITECTURE_V1_BLOCK_05_CONFIRMED_2026-09-11.md` (Zeilen 7–12, 21–25, 40–42). Alle übrigen Master-Kürzel (RV, MS13, A15, WF, OP, VP, CCA, CAP, AIS, EV, MR9, C-IDs) sind **nicht** geöffnet und in der Spalte mit „–“ gekennzeichnet. Die Konfliktstatus stammen aus `Docs/Planning/CONFLICTS.md` und `W-`-Vorschlägen der Ausgangsliste. Für die Statusvergabe gilt: „bereits vorhanden“ = Inhalt steht im Zieldokument außerhalb des Nachtrags; „vollständig integriert“ = Inhalt steht nachweisbar im Nachtrag; „teilweise“ = nachweisbar nur ein Teil; „blockiert“ = Inhalt vorhanden, aber Entscheidung nach K-ID ausstehend; „noch nicht integriert“ = fehlt im Ziel.

**Wichtige Erkenntnis:** Die Spalte „Lücke“ der Ausgangsliste bezieht sich auf die Master-Quelle, nicht auf das Zieldokument. Im Ziel (GS/SV) ist der Inhalt bei fast allen Anforderungen bereits vorhanden. Die Ausgangsliste hat deshalb mehrere Inhalte fälschlich als „nicht gefunden“ markiert (siehe Abschnitt „Hinweise zur Ausgangsliste“).

**Legende Fundstellen:** `GS:n` = `Docs/Game/Gameplay-Systeme.md` Zeile n; `SV:n` = `Docs/Game/Spielvision.md` Zeile n; `M:` = Master-Stelle (Kürzel wie in der Ausgangsliste, Dateinamen siehe `Docs/Planning/SOURCES.md`); „–“ = Master nicht geöffnet. `Ziel` verwendet die Abschnittsnummern der beiden Dateien; „Nachtrag“ = GS ab Z. 392 bzw. SV ab Z. 430.

## Nachweistabelle: Gameplay-Systeme (GP)

| ID | Originalquelle (geprüft: Datei:Zeile) | Ziel (Datei §Abschnitt) | Status | Fehlende Details / Korrektur |
|---|---|---|---|---|
| GP-ERKUNDUNG-001 | GS:49 ✓; SV:118 ✓; M: – | GS §2; SV §5.1 | bereits vorhanden | – |
| GP-ERKUNDUNG-002 | GS:53 ✓; M: – | GS §2 | bereits vorhanden | – |
| GP-ERKUNDUNG-003 | GS:52 ✓; M: – | GS §2 | bereits vorhanden | – |
| GP-KAMERA-001 | GS:50 ✓ („wahrscheinlich“); M: – | GS §2 | bereits vorhanden | Status vorläufig passt. |
| GP-KAMERA-002 | GS:50 ✓; M: – | GS §2 | bereits vorhanden | – |
| GP-SPRUNG-001 | GS:51 ✓; M: – | GS §2 | bereits vorhanden | – |
| GP-INTERAKTION-001 | GS:61, 65 ✓ | GS §3.1, §3.2 | bereits vorhanden | Use und Testtür als ✅ markiert; weitere Verben, Schloss, Hinweis-UI, Controller als ❓ offen markiert (korrekt). |
| GP-INTERAKTION-002 | GS:66 ✓; M: CA5:47 – | GS §3.2 | bereits vorhanden | – |
| GP-INTERAKTION-003 | GS:67 ✓ | GS §3.2 | bereits vorhanden | Modulliste enthält zusätzlich „Ruhe“; Verweis auf Adult-System. |
| GP-VERLETZUNG-001 | GS:74, 78 ✓; M: B9:5 ✓ (Anatomie gleich, Plausibilität) | GS §4 | bereits vorhanden | Todesmodell des Spielers ist K-26 (offen); die Anforderung selbst ist erfüllt. |
| GP-VERLETZUNG-002 | GS:79 ✓; M: – | GS §4 | bereits vorhanden | – |
| GP-VERLETZUNG-003 | GS:81 ✓; M: CA5:50 ✓ | GS §4 | bereits vorhanden | Übernatürliche Modifikatoren stehen in GS:81. Die Ausgangsliste nennt sie fälschlich „nicht gefunden“. |
| GP-VERLETZUNG-004 | GS:82 ✓; M: CA5:50 ✓ | GS §4 | bereits vorhanden | – |
| GP-VERLETZUNG-005 | GS:83 ✓; M: – | GS §4 | bereits vorhanden | – |
| GP-VERLETZUNG-006 | GS:83 ✓; M: CA5:50 ✓ | GS §4 | bereits vorhanden | Ausdauer ist ergänzt. Temporäre Funktionseinschränkung (CA5:50) ist konsistent. |
| GP-VERLETZUNG-007 | GS:84 ✓; M: – | GS §4 | bereits vorhanden | – |
| GP-VERLETZUNG-008 | GS:85 ✓; M: CA5:50 ✓ | GS §4 | bereits vorhanden | – |
| GP-VERLETZUNG-009 | GS:87 ✓; M: – | GS §4 | bereits vorhanden | – |
| GP-MEDIZIN-001 | GS:86 ✓; M: CA5:50 ✓ | GS §4 | bereits vorhanden | – |
| GP-MEDIZIN-002 | GS:86 ✓; M: – | GS §4 | bereits vorhanden | – |
| GP-MEDIZIN-003 | GS:86 ✓; M: CA5:50 ✓ | GS §4; Nachtrag GS:402 | bereits vorhanden | Spannung zu CA5:50 ist im Nachtrag entschärft („keine volle Rehabilitationssimulation“). Die Klärung nach W-10 sollte in GS notiert werden. |
| GP-GORE-001 | GS:91 ✓; M: CA5:50 ✓ („Amputation/Prothesen nur über authored Story“) | GS §4; Nachtrag GS:403 | blockiert (K-22) | Der Verlust von Fingern, Händen und Gliedmaßen steht in GS:91 als plausibles Ergebnis. Das widerspricht CA5:50 und K-22. Entwicklerentscheidung offen (nur Story oder physikalisch ohne Automatismus). |
| GP-GORE-002 | GS:91 ✓; M: – | GS §4 | bereits vorhanden | Endgültige Schwelle als ❓ markiert (korrekt). |
| GP-GORE-003 | GS:92 ✓; M: – | GS §4 | bereits vorhanden | Endgültige Grenzen brauchen Plattform-, Bewertungs- und Rechtsprüfung (in GS:92 markiert). |
| GP-GORE-004 | GS:93 ✓; M: – | GS §4 | bereits vorhanden | – |
| GP-WAFFE-001 | GS:106 ✓; SV:59 ✓; M: B9:5 – | GS §5.1; SV §2.1 | bereits vorhanden | – |
| GP-WAFFE-002 | GS:107 ✓; M: – | GS §5.1 | bereits vorhanden | – |
| GP-WAFFE-003 | GS:109, 112 ✓; M: – | GS §5.1 | bereits vorhanden | – |
| GP-WAFFE-004 | GS:112 ✓; M: – | GS §5.1 | bereits vorhanden | Aktivierung, Dauer und Erholung als ❓ markiert (korrekt). |
| GP-MUNITION-001 | GS:108 ✓; M: CA5:53 – | GS §5.1 | bereits vorhanden | – |
| GP-MUNITION-002 | GS:108 ✓; M: – | GS §5.1 | bereits vorhanden | – |
| GP-MUNITION-003 | GS:110 ✓; M: – | GS §5.1 | bereits vorhanden | – |
| GP-MUNITION-004 | GS:111 ✓; M: – | GS §5.1 | bereits vorhanden | – |
| GP-BALLISTIK-001 | GS:116–118 ✓; M: – | GS §5.2 | bereits vorhanden | – |
| GP-BALLISTIK-002 | GS:119–120 ✓; M: nicht auffindbar (CA5:49 ist Forensik, kein Ballistik-Beleg) | GS §5.2 | bereits vorhanden | Originalquelle nicht im Master belegt; Docs-Stelle ist Quelle. Zitat korrigiert (NIGHT-AUDIT-03). |
| GP-BALLISTIK-003 | GS:121 ✓; M: – | GS §5.2 | bereits vorhanden | Transschall und Spin-Drift angenähert, Coriolis nicht (in GS:121). |
| GP-BALLISTIK-004 | GS:123 ✓; M: – | GS §5.2 | bereits vorhanden | Entfernungsmesser im härtesten Modus als „kann nötig sein“ markiert. |
| GP-BALLISTIK-005 | GS:122, 124 ✓; M: – | GS §5.2 | bereits vorhanden | – |
| GP-NAHKAMPF-001 | GS:130–131, 137 ✓; M: – | GS §5.3 | bereits vorhanden | Blocken physisch in GS:131. |
| GP-NAHKAMPF-002 | GS:132 ✓; M: – | GS §5.3 | bereits vorhanden | – |
| GP-NAHKAMPF-003 | GS:133 ✓; M: – | GS §5.3 | bereits vorhanden | Flasche und Umgebung in GS:133. |
| GP-NAHKAMPF-004 | GS:134 ✓; M: – | GS §5.3 | bereits vorhanden | – |
| GP-NAHKAMPF-005 | GS:135 ✓; M: – | GS §5.3 | bereits vorhanden | – |
| GP-NAHKAMPF-006 | GS:138 ✓; M: – | GS §5.3 | bereits vorhanden | 🧭 vorläufig korrekt. Keine klassische Upgrade-Logik. |
| GP-KUPPLUNG-001 | GS:164 ✓; M: RV:7 – | GS §6.3 | bereits vorhanden | „Freie Gangwahl (auch 5→2)“ und „Abwürgen“ stehen in GS:164. Die Ausgangsliste nennt sie fälschlich „nicht gefunden“. |
| GP-KUPPLUNG-002 | GS:158 ✓; M: RV:11 – | GS §6.2 | bereits vorhanden | – |
| GP-KUPPLUNG-003 | GS:154 ✓; M: RV:7 – | GS §6.2 | bereits vorhanden | Ausgangsliste-Lücke „fehlt“ ist falsch. Phasenvertrag steht in GS:154. |
| GP-KUPPLUNG-004 | GS:165 ✓; M: RV:11 – | GS §6.3 | bereits vorhanden | Auto-Kupplung und digitale Kupplung (progressive Kurve, Abwürgen) stehen in GS:165. |
| GP-KUPPLUNG-005 | GS:165 ✓; M: RV:11 – | GS §6.3 | bereits vorhanden | – |
| GP-STEUERUNG-001 | GS:166 ✓; M: RV:26 – | GS §6.3 | bereits vorhanden | Docs-Status ist 📋 (nicht vorläufig). Die Ausgangsliste ist hier zu korrigieren. Die Gültigkeit neben der DualSense-Belegung ist offen (W-07, keine 🕰️-Kennzeichnung). |
| GP-STEUERUNG-002 | GS:167 ✓; M: RV:15–26 – | GS §6.3 | bereits vorhanden | – |
| GP-STEUERUNG-003 | GS:167 ✓; M: RV:28 – | GS §6.3 | bereits vorhanden | Klärung im Prototyp steht in GS:167 (offen). |
| GP-FAHRZEUG-001 | GS:148 ✓; M: CA5:53 – | GS §6.1 | bereits vorhanden | Charakterkanal (Gespräche, Radio, Telefon, Schweigen) steht in GS:148. Die Ausgangsliste nennt ihn fälschlich „nicht gefunden“. |
| GP-FAHRZEUG-002 | GS:155–157 ✓; M: RV:7–10 – | GS §6.2 | bereits vorhanden | – |
| GP-FAHRZEUG-003 | GS:184 ✓; M: RV:32 – | GS §6.6 | bereits vorhanden | Als ❓ offen markiert (Schadensmodell nicht gewählt, korrekt). |
| GP-FAHRZEUG-004 | GS:171 ✓; M: RV:7 – | GS §6.4 | bereits vorhanden | – |
| GP-FAHRZEUG-005 | GS:172–173 ✓; M: – | GS §6.4 | bereits vorhanden | – |
| GP-FAHRZEUG-006 | GS:173 ✓; M: RV:10 – | GS §6.4 | bereits vorhanden | – |
| GP-FAHRZEUG-007 | GS:173 ✓; M: – | GS §6.4 | bereits vorhanden | – |
| GP-FAHRZEUG-008 | GS:175 ✓; M: – | GS §6.4 | bereits vorhanden | – |
| GP-FAHRZEUG-009 | GS:176 ✓; M: – | GS §6.4 | bereits vorhanden | – |
| GP-FAHRZEUG-010 | GS:180 ✓; M: V-12 (DECISIONS) – | GS §6.5 | bereits vorhanden | Streckenzahl (fünf) als Mindestwert in GS:180, Status „vorläufig“ ist nicht gesetzt. |
| GP-TARNUNG-001 | GS:192 ✓; M: CA5:47 – | GS §7 | bereits vorhanden | – |
| GP-TARNUNG-002 | GS:192 ✓; M: – | GS §7 | bereits vorhanden | – |
| GP-TARNUNG-003 | GS:193 ✓; M: CA5:47 – | GS §7 | bereits vorhanden | Langsames Gehen (kontinuierlich leiser) in GS:193. |
| GP-TARNUNG-004 | GS:194 ✓; M: CA5:47 – | GS §7 | bereits vorhanden | – |
| GP-TARNUNG-005 | GS:193 ✓; M: – | GS §7 | bereits vorhanden | Als Präferenz markiert. |
| GP-TARNUNG-006 | GS:195 ✓; M: B9:5 – | GS §7 | bereits vorhanden | – |
| GP-TARNUNG-007 | GS:196 ✓; M: B9:5 – | GS §7 | bereits vorhanden | – |
| GP-TARNUNG-008 | GS:197 ✓; M: – | GS §7 | bereits vorhanden | – |
| GP-TARNUNG-009 | GS:198, 200 ✓; M: – | GS §7 | bereits vorhanden | Tarn-HUD optional, KI-Inspektor beschrieben. |
| GP-TARNUNG-010 | GS:199 ✓; M: CA5:47 ✓ | GS §7 | bereits vorhanden | „Keine sofortige perfekte Verkleidung“ steht in GS:199. |
| GP-NPC-001 | GS:206 ✓; M: CA5:42 – | GS §8 | bereits vorhanden | StateTree und C++ in GS:206. |
| GP-NPC-002 | GS:207 ✓; M: CA5:38 – | GS §8 | bereits vorhanden | – |
| GP-NPC-003 | GS:207 ✓; SV:220 ✓ | GS §8; SV §7.3 | bereits vorhanden | – |
| GP-NPC-004 | GS:208 ✓; M: – | GS §8 | bereits vorhanden | – |
| GP-NPC-005 | GS:210–217 ✓; M: B9:5 – | GS §8 | bereits vorhanden | Docs-Status: „verbindliche Produktvision, kein M1-Feature“. Die Ausgangsliste nennt „langfristig“, das ist eine Einordnung. Umsetzungsreihenfolge offen (GS:218). Todesursachen wichtiger NPCs: K-27 offen. |
| GP-NPC-006 | GS:218 ✓; M: AIS:22 – | GS §8 | bereits vorhanden | Als ❓ offen markiert (korrekt). |
| GP-BEZIEHUNG-001 | GS:220 ✓; M: CA5:39 ✓ | GS §8 | blockiert (K-23, K-10) | Basisspiel-Sandbox steht in GS:220. Die Abgrenzung zum Adult-Paket widerspricht DECISIONS S-14 (W-02). Entwicklerentscheidung erforderlich. |
| GP-BEZIEHUNG-002 | GS:220 ✓ (nur Sandbox); M: CA5:39 ✓ | GS §8 | blockiert (K-23; Restteil ohne Romantik noch nicht übernommen) | Fehlt in GS:220: Interesse/Zustimmung als Bedingung und die Aussage „nicht auf kleines Roster begrenzt“ (CA5:39). Ergänzen; Abgrenzung abhängig von K-23. |
| GP-KOERPER-001 | GS:234–235 ✓; M: – | GS §9.2 | bereits vorhanden | Formeln als ❓ markiert. |
| GP-KOERPER-002 | GS:234 ✓; M: nicht auffindbar (CA5:51 betrifft Schlaf/Alkohol; Suche „Muskelgedächtnis“ in 01_CURRENT ohne Treffer) | GS §9.2 | bereits vorhanden | Originalquelle nicht im Master belegt; Docs-Stelle ist Quelle. Zitat korrigiert (NIGHT-AUDIT-03). |
| GP-LEBENSSTIL-001 | GS:239–240 ✓; M: CA5:51 – | GS §9.3 | bereits vorhanden | Blackout bei extremer Vergiftung steht in GS:240. |
| GP-LEBENSSTIL-002 | GS:240 ✓; M: – | GS §9.3 | bereits vorhanden | Tiefe der Abhängigkeitsmodelle ❓. |
| GP-LEBENSSTIL-003 | GS:241 ✓; M: CA5:52 – | GS §9.3 | bereits vorhanden | – |
| GP-LEBENSSTIL-004 | GS:242 ✓; M: CA5:53 – | GS §9.3 | bereits vorhanden | – |
| GP-LEBENSSTIL-005 | GS:243–244 ✓; M: – | GS §9.3 | bereits vorhanden | Alterung nur durch vergangene Zeit steht in GS:244. |
| GP-RECHT-001 | GS:254 ✓; M: – | GS §10 | bereits vorhanden | – |
| GP-RECHT-002 | GS:255 ✓; M: – | GS §10 | bereits vorhanden | „Festnahme ist kein automatisches Game Over“ in GS:255. |
| GP-RECHT-003 | GS:255 ✓ (nur Teilliste); SV:219 ✓ (vollständig); M: CA5:49 – | GS §10; SV §7.3 | bereits vorhanden | Vollständige Liste (Obduktion, Tatort, Labore mit Bearbeitungszeit) steht in SV:219. GS:255 nennt nur einen Teil. Querverweis in GS §10 ergänzen. |
| GP-RECHT-004 | GS:256 ✓; M: – | GS §10 | bereits vorhanden | – |
| GP-RECHT-005 | GS:257 ✓; M: – | GS §10 | bereits vorhanden | – |
| GP-RECHT-006 | GS:258 ✓; M: – | GS §10 | bereits vorhanden | – |
| GP-RECHT-007 | GS:259 ✓; M: – | GS §10 | bereits vorhanden | – |
| GP-RECHT-008 | GS:261 ✓; M: – | GS §10 | bereits vorhanden | Als ❓ offen markiert (korrekt). |
| GP-SMART-001 | GS:267 ✓; M: CA5:54 – | GS §11 | bereits vorhanden | Karten, Browser, Jäger-Werkzeuge stehen in GS:267. Die Master-Lücke (L-16) betrifft nur den Master. |
| GP-SMART-002 | GS:267 ✓; M: CA5:54 – | GS §11 | bereits vorhanden | – |
| GP-SMART-003 | GS:269 ✓; SV:301–307 ✓; M: CA5:54 – | GS §11; SV §11.2 | bereits vorhanden | „Kein Zwang“ in GS:269; „Fake-Inhalte sind Pflicht“ in SV:305. |
| GP-SMART-004 | GS:270–271 ✓; M: – | GS §11 | bereits vorhanden | – |
| GP-WIRT-001 | GS:305 ✓; M: CA5:54 – | GS §13 | bereits vorhanden | Schulden als „gegebenenfalls“ markiert. |
| GP-WIRT-002 | GS:307–309 ✓; M: CA5:54 – | GS §13 | bereits vorhanden | 10–20 Coins, 12–18 Buchmacher, „Wert wird nie markiert“ stehen in GS. |
| GP-WIRT-003 | GS:306, 310 ✓; M: CA5:54 – | GS §13 | bereits vorhanden | Firmenjobs optional, nicht Hauptgeschichte (GS:306). |
| GP-GRAFIK-001 | GS:322 ✓; M: – | GS §14.1 | bereits vorhanden | – |
| GP-GRAFIK-002 | GS:323 ✓; M: – | GS §14.1 | bereits vorhanden | 30/60 FPS, kein 40-FPS-Modus, keine 4K+60+RT-Zusage stehen in GS:323. |
| GP-GRAFIK-003 | GS:324 ✓; Nachtrag GS:406 ✓; M: AIS:22 – | GS §14.1; Nachtrag | bereits vorhanden | Konkrete Budgets nicht gewählt (Nachtrag, korrekt). |
| GP-AUDIO-001 | GS:331 ✓; M: – | GS §14.2 | bereits vorhanden | W-06 (OP:7–8, Stimmenpaket „nicht 96/192 kHz“) offen: Geltung der 96-kHz-Regel nur für Produktionsmaster klären. |
| GP-AUDIO-002 | GS:332 ✓; M: – | GS §14.2 | bereits vorhanden | – |
| GP-AUDIO-003 | GS:333 ✓; M: OP:5–10 – | GS §14.2 | bereits vorhanden | – |
| GP-AUDIO-004 | GS:331–332 ✓; M: – | GS §14.2 | bereits vorhanden | Blindtests, ABX und QA-Zweig in GS:332. |
| GP-MODDING-001 | GS:345 ✓; M: MB5:7–12 ✓ | GS §15 | bereits vorhanden | Master bestätigt Provider-Unabhängigkeit. |
| GP-MODDING-002 | GS:346 ✓; M: MB5:15–25 – | GS §15 | bereits vorhanden | – |
| GP-MODDING-003 | GS:347 ✓; M: MB5:27–32 – | GS §15 | bereits vorhanden | – |
| GP-MODDING-004 | GS:348, 355 ✓; M: – | GS §15 | bereits vorhanden | Ebenen vorläufig, Überschreibregeln und Lua als offen markiert (korrekt). |
| GP-MODDING-005 | GS:349 ✓; M: MB5:40–42 ✓ (nur „canonical mod manifest/registry model“, ohne Felder) | GS §15 | bereits vorhanden | Feldliste steht in GS:349, der Master-Nachweis der Felder fehlt (W-08). Manifest-Details offen (GS:355). |
| GP-MODDING-006 | GS:351 ✓; Nachtrag GS:409–410 ✓; M: MR9:7–14 – | GS §15; Nachtrag | vollständig integriert (CONSOLIDATION-02: Backup- und Risikoregel in GS:351 eingearbeitet, rekonstruiert, K-14 bestätigen) | GS:351 hat Ausnahme nach Sicherung. Fehlt: automatisches Backup und Risikohinweis. In-Session-Deaktivierung nur im Nachtrag und als rekonstruiert (K-14, bestätigen lassen). |
| GP-MODDING-007 | GS:350 ✓; SV:353 ✓; M: – | GS §15; SV §13 | bereits vorhanden | Online-Modus ohne Mods/Cheats in SV:353. |
| GP-MODDING-008 | GS:353 ✓; M: – | GS §15 | bereits vorhanden | ✅ laut Docs. Im Code nicht geprüft. |
| GP-PERSIST-001 | GS:361 ✓; M: CC:8 –; CA6:75 – | GS §16 | bereits vorhanden | Stabile IDs stehen in GS:361. |
| GP-PERSIST-002 | GS:361 ✓; M: – | GS §16 | bereits vorhanden | Zielwert 20–50 MB als Ziel markiert, Korrektheit vor Größe (GS:361). |
| GP-PERSIST-003 | GS:361 ✓; M: – | GS §16 | bereits vorhanden | – |
| GP-PERSIST-004 | GS:363 ✓; Nachtrag GS:413–417 ✓; M: WF:7–9 – | GS §16; Nachtrag | bereits vorhanden | ✅ laut Docs, im Code nicht geprüft; WF ist nur Bericht. Offene Punkte stehen in GS:363 und im Nachtrag. |
| GP-TECH-001 | GS:371 ✓; M: CC:8 – | GS §17.1 | bereits vorhanden | C++23-Verbot steht in GS:371. Die Ausgangsliste nennt es fälschlich „nicht gefunden“. |
| GP-TECH-002 | GS:372 ✓; M: CC:8; A15:16 – | GS §17.1 | bereits vorhanden | – |
| GP-TECH-003 | GS:373 ✓; M: – | GS §17.1 | bereits vorhanden | – |
| GP-TECH-004 | GS:374 ✓; M: – | GS §17.1 | bereits vorhanden | 🧭 korrekt. Steht zusätzlich in CLAUDE.md §5. |
| GP-TEST-001 | GS:378 ✓; M: – | GS §17.2 | bereits vorhanden | – |
| GP-TEST-002 | GS:378 ✓; M: – | GS §17.2 | bereits vorhanden | Test-Orchestrator mit Berichten, Logs, Abstürzen, Replays in GS:378. |
| GP-PROD-001 | GS:382 ✓; M: MS13:6–8 – | GS §17.3 | bereits vorhanden | – |
| GP-PROD-002 | GS:337 ✓; M: VP:7–10 – | GS §14.3 | bereits vorhanden | – |
| GP-PROD-003 | SV:402–403 ✓; Nachtrag SV:449 ✓; M: VP:3–6 – | SV §15.2; Nachtrag | vollständig integriert | Einwilligung, kommerzielle Rechte und „Stimmmodelle nicht ausgeliefert“ stehen in SV:402. Referenzaufnahmen, Nutzungsumfang und Löschregeln stehen im SV-Nachtrag (SV:449). Die Lücke der Ausgangsliste ist damit geschlossen. |
| GP-CHAR-001 | GS:280 ✓; M: CCA:5–8 – | GS §12.1 | bereits vorhanden | – |
| GP-CHAR-002 | GS:284 ✓; M: CCA:6 – | GS §12.2 | bereits vorhanden | „Kosmetische Anatomie ist von Gameplay-Stärke getrennt“ steht in GS:284. |
| GP-CHAR-003 | GS:286–287 ✓; M: CAP:7–15 – | GS §12.2 | bereits vorhanden | – |
| GP-CHAR-004 | GS:297 ✓; M: CAP:5–7 – | GS §12.4 | bereits vorhanden | ✅ laut Docs, im Code nicht geprüft. 29 Parameter als Platzhalter markiert. |
| GP-CHAR-005 | GS:292 ✓; M: CCA:19–20 – | GS §12.3 | bereits vorhanden | – |
| GP-CHAR-006 | GS:221 ✓; M: – | GS §8 | bereits vorhanden | 🧭 Planungsgröße korrekt. |
| GP-CHAR-007 | GS:293, 297 ✓; M: CCA:12–14 – | GS §12.3, §12.4 | bereits vorhanden | Pipeline offen (GS:297). |

## Nachweistabelle: Spielvision (SV)

| ID | Originalquelle (geprüft: Datei:Zeile) | Ziel (Datei §Abschnitt) | Status | Fehlende Details / Korrektur |
|---|---|---|---|---|
| SV-VISION-001 | SV:50 ✓; M: – | SV §2 | bereits vorhanden | – |
| SV-VISION-002 | SV:52 ✓; M: CA5:30–35 – | SV §2 | bereits vorhanden | Markenprüfung und Titel offen (SV:52). |
| SV-VISION-003 | SV:57 ✓; M: – | SV §2.1 | bereits vorhanden | – |
| SV-VISION-004 | SV:59–60 ✓; M: – | SV §2.1 | bereits vorhanden | Fall-Lösung bei genug Hinweisen in SV:60. |
| SV-VISION-005 | SV:79–87 ✓; M: – | SV §3 | bereits vorhanden | – |
| SV-VISION-006 | SV:91 ✓; M: CA5:47 – | SV §3 | bereits vorhanden | – |
| SV-VISION-007 | SV:97–98 ✓; M: B9:5 – | SV §4 | bereits vorhanden | – |
| SV-VISION-008 | SV:99–103 ✓; M: B9:5 – | SV §4 | bereits vorhanden | – |
| SV-VISION-009 | SV:66–73 ✓; M: MS13:9–10 – | SV §2.2 | bereits vorhanden | Schleife und 30–60 Minuten (🧭) stehen in SV:66–73. Ausgangsliste-Lücke falsch. Phase-0-Platzhalter nur im GS-Nachtrag (GS:420). |
| SV-STORY-001 | SV:113 ✓; M: – | SV §5.1 | bereits vorhanden | – |
| SV-STORY-002 | SV:114 ✓; M: – | SV §5.1 | bereits vorhanden | – |
| SV-STORY-003 | SV:115–116 ✓; M: CA5:44 – | SV §5.1 | bereits vorhanden | – |
| SV-STORY-004 | SV:117 ✓; M: – | SV §5.1 | bereits vorhanden | – |
| SV-STORY-005 | SV:118 ✓; M: – | SV §5.1 | bereits vorhanden | – |
| SV-STORY-006 | SV:119 ✓; M: – | SV §5.1 | bereits vorhanden | Planungsziel, keine Zusage (SV:119). |
| SV-STORY-007 | SV:146–147 ✓; M: – | SV §5.4 | bereits vorhanden | – |
| SV-STORY-008 | SV:125–136 ✓; M: C-46 – | SV §5.2 | bereits vorhanden | Auslöser offen (SV:136). Der Bunker-Bezug (C-46, K-28) betrifft SV-HQ-002. |
| SV-PROTA-001 | SV:154–155 ✓; M: CA5:56 – | SV §6.1 | bereits vorhanden | – |
| SV-CREW-001 | SV:160–162 ✓; M: CA5:43–45 – | SV §6.2 | bereits vorhanden | – |
| SV-CREW-002 | SV:163 ✓; M: – | SV §6.2 | bereits vorhanden | – |
| SV-CREW-003 | SV:164 ✓; M: CA5:44 ✓ | SV §6.2 | bereits vorhanden | Entspricht CA5:44. Geltung für den Spieler ist K-24 (nur für Crew-Figuren betroffen). |
| SV-CREW-004 | SV:165 ✓; M: – | SV §6.2 | bereits vorhanden | – |
| SV-CREW-005 | SV:170–174 ✓; M: CA5:43 – | SV §6.3 | bereits vorhanden | Befehle stehen in SV:173 (Ausgangsliste-Angabe „nicht gefunden“ falsch). |
| SV-CREW-006 | SV:176 ✓; M: – | SV §6.3 | bereits vorhanden | – |
| SV-CREW-007 | SV:177 ✓; M: CA5:46 ✓ (Hund) | SV §6.3 | bereits vorhanden | Entspricht CA5:46. |
| SV-HQ-001 | SV:181–183 ✓; M: CA5:53 – | SV §6.4 | bereits vorhanden | – |
| SV-HQ-002 | SV:184 ✓; M: MS13:23–27 –; A15:29 – | SV §6.4 | blockiert (K-28) | Bunker als Feldbasis steht in SV:184. Die Beziehung Bunker/HQ ist offen und widerspricht laut W-09 dem Master (K-28, C-46). Entscheidung erforderlich. |
| SV-HQ-003 | SV:185–186 ✓; M: A15:29 – | SV §6.4 | blockiert (K-28) | Standort offen (SV:185) und Geheimhaltung (SV:186) stehen im Ziel. Standort und Bunker-Beziehung hängen an K-28. |
| SV-FALL-001 | SV:196–198 ✓; M: B9:5 – | SV §7.1 | bereits vorhanden | – |
| SV-FALL-002 | SV:200 ✓; M: B9:5 – | SV §7.1 | bereits vorhanden | Alle genannten Auflösungen stehen in SV:200. |
| SV-FALL-003 | SV:201 ✓; M: C-21 – | SV §7.1 | bereits vorhanden | – |
| SV-FALL-004 | SV:202 ✓; M: – | SV §7.1 | bereits vorhanden | – |
| SV-FALL-005 | SV:210–212 ✓; M: – | SV §7.2 | bereits vorhanden | Ausgangsliste-Lücke falsch; Detektivmodus in SV:210 vollständig. |
| SV-FALL-006 | SV:204 ✓; M: – | SV §7.1 | bereits vorhanden | Privatdetektiv mit Büro und Lizenz in SV:204. |
| SV-FALL-007 | SV:225–227 ✓; M: B9:5 – | SV §7.4 | bereits vorhanden | – |
| SV-ARCHIV-001 | SV:235 ✓; M: – | SV §8 | bereits vorhanden | – |
| SV-ARCHIV-002 | SV:238–240 ✓; M: – | SV §8 | bereits vorhanden | Qualitative Anzeige, Unbekanntes ohne Kategorie in SV:240. |
| SV-ARCHIV-003 | SV:239, 241 ✓; M: – | SV §8 | bereits vorhanden | – |
| SV-ARCHIV-004 | SV:242 ✓; M: – | SV §8 | bereits vorhanden | – |
| SV-ARCHIV-005 | SV:243 ✓; M: CA5:78–80 –; CANONICAL_ADDENDUM_2026-09-04.md:20–23 – | SV §8 | bereits vorhanden | Alt-Zahl 1.200–1.500 als 🕰️ markiert. Die Angabe „~200 km²“ der Ausgangsliste steht nicht im Text. Neue Zahl je Kategorie offen (❓). Gesamtzahl ca. 5.000 ❓. |
| SV-MONSTER-001 | SV:251–260 ✓; M: B9:5 – | SV §9.1 | bereits vorhanden | – |
| SV-MONSTER-002 | SV:258 ✓; M: – | SV §9.1 | bereits vorhanden | „Alpha“ ohne Labels in SV:258 (Ausgangsliste-Lücke falsch). |
| SV-MONSTER-003 | SV:264 ✓; M: B9:5 – | SV §9.2; SV-Nachtrag | bereits vorhanden | Alle genannten Schwächenwirkungen stehen in SV:264. Die Formulierung „eine kanonische Schwäche“ wird im SV-Nachtrag (SV:439, L-03) zur Prüfung gestellt. |
| SV-MONSTER-004 | SV:265 ✓; M: – | SV §9.2 | bereits vorhanden | – |
| SV-MONSTER-005 | SV:266–268 ✓; M: B9:5 – | SV §9.2 | bereits vorhanden | Narben und Regenerationszerlegung in SV:267–268. |
| SV-MONSTER-006 | SV:273 ✓; M: B9:5 – | SV §9.3 | bereits vorhanden | Zerstörte Nester bleiben zerstört (SV:273). |
| SV-MONSTER-007 | SV:274 ✓; M: B9:5 – | SV §9.3 | bereits vorhanden | – |
| SV-MONSTER-008 | SV:275 ✓; M: – | SV §9.3 | bereits vorhanden | – |
| SV-MONSTER-009 | SV:276 ✓; M: B9:7 – | SV §9.3 | bereits vorhanden | 🧭 keine endgültige Festlegung (korrekt). |
| SV-MONSTER-010 | SV:277–278 ✓; M: C-21 – | SV §9.3 | bereits vorhanden | – |
| SV-MONSTER-011 | SV:278 ✓; M: – | SV §9.3 | bereits vorhanden | – |
| SV-JAEGER-001 | SV:285–286 ✓; M: C-21 – | SV §10 | bereits vorhanden | – |
| SV-JAEGER-002 | SV:287 ✓; M: C-21, C-22 – | SV §10 | bereits vorhanden | Namen und Eigenschaften ❓ offen (korrekt). |
| SV-JAEGER-003 | SV:288 ✓; M: C-21 – | SV §10 | bereits vorhanden | – |
| SV-NEBEN-001 | SV:312 ✓; M: CA5:56 – | SV §11.3 | bereits vorhanden | – |
| SV-NEBEN-002 | SV:312 ✓; M: CA5:55 – | SV §11.3 | bereits vorhanden | Motorsport „sehr hohe Priorität“ steht in SV:312 (Ausgangsliste falsch). |
| SV-NEBEN-003 | SV:301–308 ✓; M: – | SV §11.2 | bereits vorhanden | – |
| SV-NEBEN-004 | SV:297, 308 ✓; M: – | SV §11.1, §11.2 | bereits vorhanden | Umfang und Architektur als ❓ markiert. |
| SV-FORT-001 | SV:320 ✓; M: – | SV §12.1 | bereits vorhanden | Prinzip verbindlich, Design ❓ (SV:320). |
| SV-FORT-002 | SV:326–328 ✓; M: – | SV §12.2 | bereits vorhanden | – |
| SV-FORT-003 | SV:329–331 ✓; M: – | SV §12.2 | bereits vorhanden | – |
| SV-FORT-004 | SV:332, 339 ✓; M: – | SV §12.2, §12.3 | bereits vorhanden | Gewichtung und Erfolgsliste ❓ offen (korrekt). |
| SV-FORT-005 | SV:336–338 ✓; M: – | SV §12.3 | bereits vorhanden | – |
| SV-FORT-006 | SV:343 ✓; M: – | SV §12.4 | bereits vorhanden | 🔭 langfristig korrekt. |
| SV-MODUS-001 | SV:351, 356 ✓; M: MR9:9–14 – | SV §13 | bereits vorhanden | – |
| SV-MODUS-002 | SV:352 ✓; SV-Nachtrag ✓ (GS:415 Host-Speicherstand); M: C-52 –; A15Q:7 – | SV §13 | bereits vorhanden | Host-Speicherstand ist durch Entwicklerentscheidung K-25 (08.11.2026) bestätigt. Netzwerkarchitektur und Zeitpunkt bleiben ❓. Docs-Kennzeichnung „bestätigt“ ergänzen. |
| SV-MODUS-003 | SV:353 ✓; M: – | SV §13 | bereits vorhanden | 🔭 und ❓ korrekt markiert. |
| SV-MODUS-004 | SV:355 ✓; M: – | SV §13 | bereits vorhanden | 🧭 „keine Zusage“ korrekt. |
| SV-MONET-001 | SV:362 ✓; M: – | SV §13.1 | bereits vorhanden | 🧭 nicht beschlossen, korrekt. |
| SV-MONET-002 | SV:363–365 ✓; M: – | SV §13.1 | bereits vorhanden | – |
| SV-TOD-001 | SV:373 ✓; M: – | SV §14.1 | bereits vorhanden | Inhalt steht in SV:373 (Ausgangsliste nennt 371–373). |
| SV-TOD-002 | SV:373 ✓; M: – | SV §14.1 | bereits vorhanden | – |
| SV-TOD-003 | SV:377–381 ✓; M: CA6:90 –; B9:7 – | SV §14.2 | blockiert (K-26) | Richtung steht in SV:379–381. Die Schleife ist ❓ offen. Todesmodell K-26 (Kanon C-21 gegen BW-001/BW-012) fehlt noch die Entscheidung. |
| SV-TOD-004 | SV:382–383 ✓; M: C-30 –; CA5:50 ✓ | SV §14.2 | bereits vorhanden | Fliehen, Aufwachen im Krankenhaus, Folgen in SV:382–383. Krankenhaus-Abwesenheitsdetails sind nur Nutzeraussage im Nachtrag (SV:453, unbestätigt). |
| SV-TOD-005 | SV:384 ✓; M: BLOCK_09_STATUS:5 ✓; CA5:44 – | SV §14.2 | blockiert (K-24) | Permadeath optional und kein Soulslike-Modell stehen in SV:384. Geltung für den Spieler (individueller Tod endgültig?) ist offen (K-24, W-03). |
| SV-DIALOG-001 | SV:392 ✓; M: – | SV §15.1 | bereits vorhanden | Alle genannten Felder stehen in SV:392. |
| SV-DIALOG-002 | SV:394–396 ✓; M: CA5:42 – | SV §15.1 | bereits vorhanden | NPC-Gespräche untereinander stehen in SV:396 (Ausgangsliste falsch). |
| SV-DIALOG-003 | SV:397 ✓; M: CA5:42, VP:5 – | SV §15.1 | bereits vorhanden | Zahl ❓ offen. Konflikt W-05 (Zuordnung Haupt-/Nebenfiguren) offen. |
| SV-DIALOG-004 | SV:401–403 ✓; M: VP:3–6 – | SV §15.2 | bereits vorhanden | Platzhalter als nicht final markiert (SV:401), Austausch ohne Bruch (SV:403). |
| SV-DIALOG-005 | SV:402 ✓; M: VP:3–6 – | SV §15.2 | bereits vorhanden | Nachahmungsverbot steht in SV:402 (Ausgangsliste falsch). |
| SV-DIALOG-006 | SV:405 ✓; M: – | SV §15.2 | bereits vorhanden | – |
| SV-DIALOG-007 | SV:408–410 ✓; M: – | SV §15.3 | bereits vorhanden | – |
| SV-GUIDE-001 | SV:416 ✓; M: – | SV §16 | bereits vorhanden | 🔭 korrekt; keine Speicherauslesung. |
| SV-GUIDE-002 | SV:418 ✓; M: – | SV §16 | bereits vorhanden | 🧭 korrekt; Preis ❓. |
| SV-GUIDE-003 | SV:420 ✓; M: OP:12–16 – | SV §16 | bereits vorhanden | 100 GB und Offline-Installation stehen in SV:420 (Ausgangsliste falsch). |

## Zählung je Status

| Status | GP (137) | SV (85) | Gesamt (222) |
|---|---|---|---|
| vollständig integriert | 2 | 0 | 2 |
| bereits vorhanden | 132 | 81 | 213 |
| teilweise integriert | 0 | 0 | 0 |
| blockiert | 3 | 4 | 7 |
| verworfen | 0 | 0 | 0 |
| noch nicht integriert | 0 | 0 | 0 |

Stand 08.11.2026 (CONSOLIDATION-FINAL), aus den Zeilen gezählt. Die frühere Tabelle (1 / 2 / 2 / 6) stimmte nicht mit den Zeilen überein.

Blockiert: GP-GORE-001 (K-22), GP-BEZIEHUNG-001 (K-23, K-10), GP-BEZIEHUNG-002 (K-23), SV-HQ-002 (K-28), SV-HQ-003 (K-28), SV-TOD-003 (K-26), SV-TOD-005 (K-24).

Vollständig integriert: GP-MODDING-006 (Backup- und Risikoregel, CONSOLIDATION-02), GP-PROD-003 (Einwilligung und Nutzung, SV:402–449). Die frühere Angabe „teilweise“ für GP-MODDING-006 war durch die Zeile überholt.

## Korrekturen an Docs/Game

Stand CONS-GAMEPLAY (08.11.2026): Die Liste wurde gegen die Dokumente geprüft. Status je Punkt unten. Ohne Entwicklerentscheidung umgesetzt: nur Ergänzungen und Kennzeichnungen, keine Spielentscheidung.

| Nr. | Status | Umsetzung |
|---|---|---|
| 1 | Entwicklerentscheidung (K-22) | nicht geändert |
| 2 | Entwicklerentscheidung (K-23) | nicht geändert |
| 3 | erledigt (in GS:351 vorhanden) | Sicherung und Risikohinweis stehen bereits; K-14 zu bestätigen |
| 4 | erledigt | GS:349: Feldliste als 💡 Vorschlag gekennzeichnet (W-08) |
| 5 | Entwicklerentscheidung (W-07) | nicht geändert |
| 6 | erledigt | GS:331 begrenzt 96 kHz auf Produktionsmaster; GS:333 führt die Distribution getrennt |
| 7 | erledigt | GS:255 verweist auf die vollständige Forensikliste SV:219 |
| 8 | erledigt (Hinweis) | GS:86: Widerspruch W-10 vermerkt, Klärung ausstehend |
| 9 | erledigt | SV:184: Bunker als Feldbasis als ❓ offen (K-28) |
| 10 | erledigt | SV:264: „eine kanonische Schwäche“ auf Master-Wortlaut (Vorbereitung, mehrere Tötungswege, `BLOCK_09_STATUS.md:5`) gebracht |
| 11 | erledigt | SV:352: K-25 (Host-Speicherstand bestätigt, 08.11.2026) vermerkt |
| 12 | Entwicklerentscheidung (K-24, K-26) | Richtung und Festlegung sind in SV:377–381 bereits getrennt |
| 13 | erledigt | SV:397: Zeilenzahlen vorläufig, Zuordnung offen (W-05) |
| 14 | erledigt | SV:435: Reichsgrößen als Planungsbereiche, Hauptwelt-Größe offen (N-01) |
| 15 | erledigt | SV:453: Nutzeraussage bereits als unbestätigt markiert |

Nichts davon hat eine Spielentscheidung getroffen. Die Entwicklerentscheidungen bleiben in `CONFLICTS.md` offen.

1. **GS:91**, Gliedmaßenverlust: Nach K-22 präzisieren (nur Story-gebunden oder physikalisches Ergebnis ohne Automatismus). Master CA5:50 lässt nur die Story-Variante zu.
2. **GS:220**, Beziehungen: Zustimmung/Interesse und „nicht auf kleines Roster begrenzt“ (CA5:39) ergänzen. Basis/Adult-Abgrenzung nach K-23 festlegen.
3. **GS:351**, kritische fehlende Mods: Automatische Sicherung und Risikohinweis ergänzen (MR9:7–14). In-Session-Regel bleibt rekonstruiert (K-14).
4. **GS:349**, Mod-Manifest: Feldliste ist im Master nicht nachweisbar (MB5:40–42 nennt nur das Modell). Als Vorschlag oder offen kennzeichnen (W-08), bis ein Nachweis vorliegt.
5. **GS:166**, Xbox-Controller-Konzept: Kennzeichnung entscheiden lassen (W-07). Derzeit 📋 neben der DualSense-Belegung in GS:167.
6. **GS:331**, Audioformat: Geltung von 96 kHz/24 Bit auf Produktionsmaster begrenzen und Distribution getrennt führen (W-06, OP:7–8).
7. **GS:255**, Forensik: Verweis auf die vollständige Beweisliste in SV:219 ergänzen.
8. **GS:86 und Nachtrag GS:402**, Erholung vs. Rehabilitation: Klärung nach W-10 in GS notieren.
9. **SV:184**, Bunker als Feldbasis: Als „möglich, nicht festgelegt“ führen (W-09, K-28).
10. **SV:264**, „eine kanonische Schwäche“: Formulierung nach SV-Nachtrag SV:439 (L-03) prüfen.
11. **SV:352**, Host-Speicherstand: Als durch K-25 bestätigt kennzeichnen (08.11.2026). Netzwerkarchitektur bleibt offen.
12. **SV:377–384**, Todesmodell: Nach K-24 und K-26 entscheiden. Richtung und Festlegung getrennt kennzeichnen.
13. **SV:397**, Dialogzahlen: Als vorläufig kennzeichnen und Zuordnung Haupt-/Nebenfiguren klären (W-05).
14. **SV-Nachtrag SV:435**, Reichsgrößen (Hölle 350–500 km², Purgatorium 500–750 km², Himmel 250–400 km²): Keine Festlegung. Gegen N-01 (Hauptwelt-Größe offen) abgrenzen und als Planungsbereich kennzeichnen. In GS/SV findet sich keine 2.200-km²-Angabe mehr (K-20 bestätigt, überholt).
15. **SV-Nachtrag SV:453**, Krankenhaus-Abwesenheit: Als Nutzeraussage markiert lassen, bis bestätigt (L-15).

Bereits erledigt, keine Änderung nötig: `Spielwelt-und-Untergrund.md` Z. 346–349 ist mit „historisch (N-02, 08.11.2026)“ markiert (K-02).

## Hinweise zur Ausgangsliste (A-gameplay-spielvision.md)

Diese Punkte sind in der Ausgangsliste falsch oder ungenau. Sie wurden nicht in der Ausgangsliste geändert.

- Mehrere Lücken-Angaben „fehlt/nicht gefunden“ sind falsch, weil der Inhalt im Ziel steht: GP-KUPPLUNG-001, -003, -004; GP-VERLETZUNG-003; GP-FAHRZEUG-001; GP-TECH-001; GP-CHAR-002; GP-RECHT-003 (teilweise); SV-VISION-009; SV-CREW-005; SV-MONSTER-002; SV-FALL-005; SV-DIALOG-002, -004, -005; SV-GUIDE-003; SV-NEBEN-002.
- GP-STEUERUNG-001: Docs-Status ist 📋, nicht „vorläufig“.
- GP-NPC-005: Docs-Status ist „verbindliche Produktvision“, nicht „langfristig“.
- SV-ARCHIV-005: „~200 km²“ steht nicht im Text.
- GP-RECHT-003: GS:255 enthält nur einen Teil der Forensik-Liste, der vollständige Text steht in SV:219.
- Die Master-Stellen mit „–“ wurden nicht geöffnet. Eine vollständige Master-Gegenprüfung steht noch aus.
