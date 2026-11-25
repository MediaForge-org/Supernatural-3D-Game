# Inhaltliche Gegenprüfung Gameplay (CONS-GAMEPLAY)

- **Stand:** 08.11.2026
- **Vorgehen je Anforderung:** Anforderungstext aus `Konsolidierung/A-gameplay-spielvision.md` (A) mit der Quellzeile in `Docs/Game/Gameplay-Systeme.md` (GS) bzw. `Docs/Game/Spielvision.md` (SV) und der Zielstelle inhaltlich verglichen (Bedingungen, Ausnahmen, Zahlen, Status). Ein Stichwort allein genügt nicht.
- **Originalquelle:** Die Ausgangsliste verweist auf GS/SV, nicht auf den Master. Der Master (CA5, CA6, MR9 usw.) wurde in dieser Prüfung nur dort betrachtet, wo der Nachweis ihn nennt; das ist vermerkt.
- **Ergebnisse:** `übereinstimmend` (Inhalt geprüft), `Abweichung A` (die Ausgangsliste weicht von der Quelle ab; das Dokument ist korrekt), `Korrektur` (Dokument korrigiert), `blockiert` (Entwicklerentscheidung, Widerspruch bleibt markiert).

| ID | Quelle (GS/SV) | Ziel | Ergebnis | Korrektur oder Blockade |
|---|---|---|---|---|
| GP-ERKUNDUNG-001 | GS:49; SV:118 | GS §2; SV §5.1 | übereinstimmend | – |
| GP-ERKUNDUNG-002 | GS:53 | GS §2 | übereinstimmend | – |
| GP-ERKUNDUNG-003 | GS:52 | GS §2 | übereinstimmend | – |
| GP-KAMERA-001 | GS:50 | GS §2 | übereinstimmend | – |
| GP-KAMERA-002 | GS:50 | GS §2 | übereinstimmend | – |
| GP-SPRUNG-001 | GS:51 | GS §2 | übereinstimmend | – |
| GP-INTERAKTION-001 | GS:61 | GS §3.1, §3.2 | übereinstimmend (Use und Testtür als umgesetzt, Rest offen) | – |
| GP-INTERAKTION-002 | GS:66 | GS §3.2 | übereinstimmend | – |
| GP-INTERAKTION-003 | GS:67 | GS §3.2 | Abweichung A: Quellliste nennt „Ruhe“ nicht, GS:67 schon | – (Dokument korrekt) |
| GP-VERLETZUNG-001 | GS:74, 78 | GS §4 | übereinstimmend (gleiche Anatomie, keine Plot-Rüstung, tödliche Treffer) | – (Todesmodell K-26 bleibt offen, nicht Teil dieser Anforderung) |
| GP-VERLETZUNG-002 | GS:79 | GS §4 | übereinstimmend (25+ Regionen) | – |
| GP-VERLETZUNG-003 | GS:81 | GS §4 | übereinstimmend (inkl. übernatürliche Modifikatoren) | – |
| GP-VERLETZUNG-004 | GS:82 | GS §4 | übereinstimmend | – |
| GP-VERLETZUNG-005 | GS:83 | GS §4 | übereinstimmend | – |
| GP-VERLETZUNG-006 | GS:83 | GS §4 | übereinstimmend (Nerven/Sehnen, offene Brüche beim Spieler) | – |
| GP-VERLETZUNG-007 | GS:84 | GS §4 | übereinstimmend | – |
| GP-VERLETZUNG-008 | GS:85 | GS §4 | übereinstimmend | – |
| GP-VERLETZUNG-009 | GS:87 | GS §4 | übereinstimmend | – |
| GP-MEDIZIN-001 | GS:86 | GS §4 | übereinstimmend (drei Ebenen, Krankenhaus-Kriterien) | – |
| GP-MEDIZIN-002 | GS:86 | GS §4 | übereinstimmend (Symptomlinderung) | – |
| GP-MEDIZIN-003 | GS:86; GS:402 | GS §4; Nachtrag | übereinstimmend; Nachtrag GS:402 („keine volle Rehabilitationssimulation“) bestätigt | Hinweis: Klärung nach W-10 noch nicht in GS notiert (Korrekturliste) |
| GP-GORE-001 | GS:91 | GS §4, Nachtrag GS:403 | übereinstimmend im Wortlaut; Widerspruch zu CA5:50 | blockiert (K-22, Entwicklerentscheidung) |
| GP-GORE-002 | GS:91 | GS §4 | übereinstimmend (Schwelle als ❓ markiert) | – |
| GP-GORE-003 | GS:92 | GS §4 | übereinstimmend | – |
| GP-GORE-004 | GS:93 | GS §4 | übereinstimmend | – |
| GP-WAFFE-001 | GS:106; SV:59 | GS §5.1; SV §2.1 | übereinstimmend | – |
| GP-WAFFE-002 | GS:107 | GS §5.1 | übereinstimmend (Trageort-Zugriff) | – |
| GP-WAFFE-003 | GS:109, 112 | GS §5.1 | übereinstimmend (schwache Hand, Training verbessert Kontrolle, nicht Impuls) | – |
| GP-WAFFE-004 | GS:112 | GS §5.1 | übereinstimmend (Aktivierung, Dauer, Erholung ❓) | – |
| GP-MUNITION-001 | GS:108 | GS §5.1 | übereinstimmend | – |
| GP-MUNITION-002 | GS:108 | GS §5.1 | übereinstimmend (Röhrenmagazin, Nachladezustand persistent) | – |
| GP-MUNITION-003 | GS:110 | GS §5.1 | übereinstimmend | – |
| GP-MUNITION-004 | GS:111 | GS §5.1 | übereinstimmend | – |
| GP-BALLISTIK-001 | GS:116–118 | GS §5.2 | übereinstimmend (keine Maximalreichweite) | – |
| GP-BALLISTIK-002 | GS:119–120 | GS §5.2 | übereinstimmend mit Docs; Originalquelle im Master nicht auffindbar | Quellenhinweis: ursprüngliche Masterquelle nicht auffindbar (NIGHT-AUDIT-03) |
| GP-BALLISTIK-003 | GS:121 | GS §5.2 | übereinstimmend (Coriolis nicht) | – |
| GP-BALLISTIK-004 | GS:123 | GS §5.2 | übereinstimmend (Entfernungsmesser „kann nötig sein“) | – |
| GP-BALLISTIK-005 | GS:122, 124 | GS §5.2 | übereinstimmend (Nullpunkt = Optik/Lauf, Schalldämpfer nicht lautlos) | – |
| GP-NAHKAMPF-001 | GS:130–131 | GS §5.3 | übereinstimmend | – |
| GP-NAHKAMPF-002 | GS:132 | GS §5.3 | übereinstimmend | – |
| GP-NAHKAMPF-003 | GS:133 | GS §5.3 | übereinstimmend | – |
| GP-NAHKAMPF-004 | GS:134 | GS §5.3 | übereinstimmend | – |
| GP-NAHKAMPF-005 | GS:135 | GS §5.3 | übereinstimmend | – |
| GP-NAHKAMPF-006 | GS:138 | GS §5.3 | übereinstimmend (🧭 korrekt) | – |
| GP-KUPPLUNG-001 | GS:164 | GS §6.3 | übereinstimmend | – |
| GP-KUPPLUNG-002 | GS:158 | GS §6.2 | übereinstimmend | – |
| GP-KUPPLUNG-003 | GS:154 | GS §6.2 | übereinstimmend | – |
| GP-KUPPLUNG-004 | GS:165 | GS §6.3 | übereinstimmend | – |
| GP-KUPPLUNG-005 | GS:165 | GS §6.3 | übereinstimmend | – |
| GP-STEUERUNG-001 | GS:166 | GS §6.3 | übereinstimmend im Inhalt; Statusangabe der Ausgangsliste (vorläufig) falsch, Dokument 📋 | – (Dokument korrekt; Gültigkeit neben DualSense offen, W-07) |
| GP-STEUERUNG-002 | GS:167 | GS §6.3 | übereinstimmend; Statusangabe der Ausgangsliste (vorläufig, V-13) falsch, Dokument 📋 (Entwicklerangabe 15.10.) | – |
| GP-STEUERUNG-003 | GS:167 | GS §6.3 | übereinstimmend (Klärung im Prototyp offen) | – |
| GP-FAHRZEUG-001 | GS:148 | GS §6.1 | übereinstimmend (Charakterkanal, Kofferraum als Lager) | – |
| GP-FAHRZEUG-002 | GS:155–157 | GS §6.2 | übereinstimmend (Phasenvertrag) | – |
| GP-FAHRZEUG-003 | GS:184 | GS §6.6 | übereinstimmend (Schadensmodell nicht gewählt, ❓) | – |
| GP-FAHRZEUG-004 | GS:171 | GS §6.4 | übereinstimmend | – |
| GP-FAHRZEUG-005 | GS:172–173 | GS §6.4 | übereinstimmend | – |
| GP-FAHRZEUG-006 | GS:173 | GS §6.4 | übereinstimmend | – |
| GP-FAHRZEUG-007 | GS:173 (Zeilenende) | GS §6.4 | übereinstimmend (Kraftstoffe, E85, Laden); Ausgangsliste nennt „fehlt“, falsch | – |
| GP-FAHRZEUG-008 | GS:175 | GS §6.4 | übereinstimmend | – |
| GP-FAHRZEUG-009 | GS:176 | GS §6.4 | übereinstimmend | – |
| GP-FAHRZEUG-010 | GS:180 | GS §6.5 | übereinstimmend; Streckenzahl „mindestens fünf“ steht im 📋-Satz, Ausgangsliste-Status „vorläufig“ für die Zahl nicht bestätigt | – (Hinweis) |
| GP-TARNUNG-001 | GS:192 | GS §7 | übereinstimmend | – |
| GP-TARNUNG-002 | GS:192 | GS §7 | übereinstimmend | – |
| GP-TARNUNG-003 | GS:193 | GS §7 | übereinstimmend (Schuhwerk, Verletzung, langsames Gehen leiser) | – |
| GP-TARNUNG-004 | GS:194 | GS §7 | übereinstimmend | – |
| GP-TARNUNG-005 | GS:193 | GS §7 | übereinstimmend (Raum-/Portalmodell bevorzugt, Hall) | – |
| GP-TARNUNG-006 | GS:195 | GS §7 | übereinstimmend | – |
| GP-TARNUNG-007 | GS:196 | GS §7 | übereinstimmend | – |
| GP-TARNUNG-008 | GS:197 | GS §7 | übereinstimmend | – |
| GP-TARNUNG-009 | GS:198, 200 | GS §7 | übereinstimmend (Tarn-HUD optional, KI-Inspektor) | – |
| GP-TARNUNG-010 | GS:199 | GS §7 | übereinstimmend | – |
| GP-NPC-001 | GS:206 | GS §8 | übereinstimmend | – |
| GP-NPC-002 | GS:207 | GS §8 | übereinstimmend | – |
| GP-NPC-003 | GS:207; SV:220 | GS §8; SV §7.3 | übereinstimmend | – |
| GP-NPC-004 | GS:208 | GS §8 | übereinstimmend | – |
| GP-NPC-005 | GS:210–217 | GS §8 | übereinstimmend (persistente Identität, Gedächtnis, Nemesis, autonome Fraktionen, Konsequenzen, skalierbar); Ausgangsliste-Status „langfristig“ ist Einordnung, Dokument 📋 Produktvision | – (Hinweis) |
| GP-NPC-006 | GS:218 | GS §8 | übereinstimmend (❓ offen) | – |
| GP-BEZIEHUNG-001 | GS:220 | GS §8 | übereinstimmend im Wortlaut der Basis-Sandbox; Widerspruch zu DECISIONS S-14 | blockiert (K-23, K-10) |
| GP-BEZIEHUNG-002 | GS:220 | GS §8 | Teile fehlen (Interesse/Zustimmung als Bedingung, „nicht auf kleines Roster begrenzt“, CA5:39) | blockiert (K-23): Ergänzung erst nach Entscheidung, nicht übernommen |
| GP-KOERPER-001 | GS:234–235 | GS §9.2 | übereinstimmend (Anpassungsmodell, Formeln ❓) | – |
| GP-KOERPER-002 | GS:234 | GS §9.2 | übereinstimmend mit Docs (Muskelgedächtnis); Originalquelle im Master nicht auffindbar | Quellenhinweis: ursprüngliche Masterquelle nicht auffindbar (NIGHT-AUDIT-03) |
| GP-LEBENSSTIL-001 | GS:239–240 | GS §9.3 | übereinstimmend | – |
| GP-LEBENSSTIL-002 | GS:240 | GS §9.3 | übereinstimmend (Rauchen; Abhängigkeitstiefe ❓) | – |
| GP-LEBENSSTIL-003 | GS:241 | GS §9.3 | übereinstimmend | – |
| GP-LEBENSSTIL-004 | GS:242 | GS §9.3 | übereinstimmend | – |
| GP-LEBENSSTIL-005 | GS:243–244 | GS §9.3 | übereinstimmend | – |
| GP-RECHT-001 | GS:254 | GS §10 | übereinstimmend | – |
| GP-RECHT-002 | GS:255 | GS §10 | übereinstimmend | – |
| GP-RECHT-003 | GS:255; SV:219 | GS §10; SV §7.3 | übereinstimmend nach Ergänzung: GS §10 verweist jetzt auf die vollständige Liste in SV:219 | Korrektur: Querverweis in GS §10 ergänzt (Korrekturliste GS:255) |
| GP-RECHT-004 | GS:256 | GS §10 | übereinstimmend | – |
| GP-RECHT-005 | GS:257 | GS §10 | übereinstimmend | – |
| GP-RECHT-006 | GS:258 | GS §10 | übereinstimmend | – |
| GP-RECHT-007 | GS:259 | GS §10 | übereinstimmend | – |
| GP-RECHT-008 | GS:261 | GS §10 | übereinstimmend (❓) | – |
| GP-SMART-001 | GS:267 | GS §11 | übereinstimmend | – |
| GP-SMART-002 | GS:267 | GS §11 | übereinstimmend (Funklöcher; Satellit als Spezialoption) | – |
| GP-SMART-003 | GS:269; SV:301–307 | GS §11; SV §11.2 | übereinstimmend (kein Zwang, optionale Ereignisse; Fake-Inhalte, SV:305) | – |
| GP-SMART-004 | GS:270–271 | GS §11 | übereinstimmend | – |
| GP-WIRT-001 | GS:305 | GS §13 | übereinstimmend | – |
| GP-WIRT-002 | GS:307–309 | GS §13 | übereinstimmend (Aktien nur fiktiv, Krypto 10–20, Buchmacher 12–18, „Wert wird nie markiert“) | – |
| GP-WIRT-003 | GS:306, 310 | GS §13 | übereinstimmend (Glücksspiel, Pferderennen, Firmenjobs optional) | – |
| GP-GRAFIK-001 | GS:322 | GS §14.1 | übereinstimmend (4K Ziel, 8K optional, Raytracing, Path Tracing) | – |
| GP-GRAFIK-002 | GS:323 | GS §14.1 | übereinstimmend (30/60 FPS, kein 40-FPS-Modus, keine 4K+60+RT-Zusage) | – |
| GP-GRAFIK-003 | GS:324 | GS §14.1 | übereinstimmend | – |
| GP-AUDIO-001 | GS:331 | GS §14.2 | übereinstimmend (48/96 kHz, dauerhaft; Horror-Klänge) | – |
| GP-AUDIO-002 | GS:332 | GS §14.2 | übereinstimmend (Raum-/Portal-Ausbreitung, KI-Gehör und Spieler-Audio aus demselben Ereignis) | – |
| GP-AUDIO-003 | GS:333 | GS §14.2 | übereinstimmend (Stimmenpaket, Sprachen getrennt, Ultra/Kino-Texturen) | – |
| GP-AUDIO-004 | GS:331–332 | GS §14.2 | übereinstimmend (Qualitätsstufen, Blindtests, ABX, QA-Zweig) | – |
| GP-MODDING-001 | GS:345 | GS §15 | übereinstimmend | – |
| GP-MODDING-002 | GS:346 | GS §15 | übereinstimmend | – |
| GP-MODDING-003 | GS:347 | GS §15 | übereinstimmend (Installiert ≠ aktiviert, pro Profil und Spielstand) | – |
| GP-MODDING-004 | GS:348, 355 | GS §15 | übereinstimmend (additiv, vorläufige Ebenen; Überschreibregeln und Lua offen) | – |
| GP-MODDING-005 | GS:349 | GS §15 | übereinstimmend (Manifest, Herkunftsklassen, Sicherheitskern) | – |
| GP-MODDING-006 | GS:351; Nachtrag GS:410 | GS §15, Nachtrag | übereinstimmend nach CONSOLIDATION-02 (Backup/Snapshot und Risikoangabe; In-Session-Deaktivierung rekonstruiert, K-14) | – (K-14 zu bestätigen) |
| GP-MODDING-007 | GS:350; SV:353 | GS §15; SV §13 | übereinstimmend | – |
| GP-MODDING-008 | GS:353 | GS §15 | übereinstimmend (Namensraum-IDs, UnknownKind) | – |
| GP-PERSIST-001 | GS:361 | GS §16 | übereinstimmend (zentraler Weltzustand, stabile IDs) | – |
| GP-PERSIST-002 | GS:361 | GS §16 | übereinstimmend (segmentiert, absturzsicher; 20–50 MB als Ziel) | – |
| GP-PERSIST-003 | GS:361 | GS §16 | übereinstimmend (PostgreSQL für Online-Daten) | – |
| GP-PERSIST-004 | GS:363 | GS §16 | übereinstimmend (Kern implementiert, Rest offen) | – |
| GP-TECH-001 | GS:371 | GS §17.1 | übereinstimmend (C++23-Verbot, eigene Systeme) | – |
| GP-TECH-002 | GS:372 | GS §17.1 | übereinstimmend | – |
| GP-TECH-003 | GS:373 | GS §17.1 | übereinstimmend | – |
| GP-TECH-004 | GS:374 | GS §17.1 | übereinstimmend (🧭 korrekt) | – |

Hinweis zur Zitierprüfung: Während Paket 5 hat sich gezeigt, dass ein versehentlicher Zeileneinschub in `Gameplay-Systeme.md` (Forensik-Verweis, GS:255) die Zeilennummern danach verschoben hatte. Der Einschub wurde zurückgenommen, der Verweis steht nun als Anhang an der bestehenden Zeile 255. Die Zitierungen der Pakete 1–4 beziehen sich auf die Numerierung vor dem Einschub, also auf den tatsächlichen Stand.
| GP-TEST-001 | GS:378 | GS §17.2 | übereinstimmend (Tests als Architektur, Seeds, Replays, Regression) | – |
| GP-TEST-002 | GS:378 | GS §17.2 | übereinstimmend (Testkarten, Test-Orchestrator) | – |
| GP-PROD-001 | GS:382 | GS §17.3 | übereinstimmend (AI-first, Entwickler letzte Instanz) | – |
| GP-PROD-002 | GS:337 | GS §14.3 | übereinstimmend (Grauboxen, Grundriss, Modulset, Zusammenbau) | – |
| GP-PROD-003 | SV:402–403; SV:449 | SV §15.2; Nachtrag | übereinstimmend (Einwilligung, Rechte, Stimmmodelle nicht ausgeliefert, Löschregeln) | – |
| GP-CHAR-001 | GS:280 | GS §12.1 | übereinstimmend (fotorealistisch im Rahmen, modular) | – |
| GP-CHAR-002 | GS:284 | GS §12.2 | übereinstimmend (Editor-Umfang, kosmetisch getrennt von Stärke) | – |
| GP-CHAR-003 | GS:286–287 | GS §12.2 | übereinstimmend (Leistung aus Körperzustand; Regressionstests für Körperkombinationen) | – |
| GP-CHAR-004 | GS:297 | GS §12.4 | übereinstimmend (AppearanceSpec, 29 Platzhalter-Parameter) | – |
| GP-CHAR-005 | GS:292 | GS §12.3 | übereinstimmend | – |
| GP-CHAR-006 | GS:221 | GS §8 | übereinstimmend (🧭) | – |
| GP-CHAR-007 | GS:293, 297 | GS §12.3, §12.4 | übereinstimmend (Pipeline MetaHuman/Blender offen) | – |
| SV-VISION-001 | SV:50 | SV §2 | übereinstimmend | – |
| SV-VISION-002 | SV:52 | SV §2 | übereinstimmend (Markenprüfung, Titel offen) | – |
| SV-VISION-003 | SV:57 | SV §2.1 | übereinstimmend | – |
| SV-VISION-004 | SV:59–60 | SV §2.1 | übereinstimmend | – |
| SV-VISION-005 | SV:79–87 | SV §3 | übereinstimmend | – |
| SV-VISION-006 | SV:91 | SV §3 | übereinstimmend | – |
| SV-VISION-007 | SV:97–98 | SV §4 | übereinstimmend | – |
| SV-VISION-008 | SV:99–103 | SV §4 | übereinstimmend | – |
| SV-VISION-009 | SV:66–73 | SV §2.2 | übereinstimmend (🧭 Zielwerte) | – |
| SV-STORY-001 | SV:113 | SV §5.1 | übereinstimmend | – |
| SV-STORY-002 | SV:114 | SV §5.1 | übereinstimmend | – |
| SV-STORY-003 | SV:115–116 | SV §5.1 | übereinstimmend | – |
| SV-STORY-004 | SV:117 | SV §5.1 | übereinstimmend | – |
| SV-STORY-005 | SV:118 | SV §5.1 | übereinstimmend | – |
| SV-STORY-006 | SV:119 | SV §5.1 | übereinstimmend (Planungsziel, keine Zusage) | – |
| SV-STORY-007 | SV:146–147 | SV §5.4 | übereinstimmend (Abhängigkeitsgraph, Replay und kanonische Story) | – |
| SV-STORY-008 | SV:125–136 | SV §5.2 | übereinstimmend (kleiner Start, Wissen, Stunden); Auslöser offen (SV:136) | – |
| SV-PROTA-001 | SV:154–155 | SV §6.1 | übereinstimmend | – |
| SV-CREW-001 | SV:160–162 | SV §6.2 | übereinstimmend | – |
| SV-CREW-002 | SV:163 | SV §6.2 | übereinstimmend | – |
| SV-CREW-003 | SV:164 | SV §6.2 | übereinstimmend (HQ-kritische Figuren geschützt; Geltung für den Spieler K-24) | – |
| SV-CREW-004 | SV:165 | SV §6.2 | übereinstimmend | – |
| SV-CREW-005 | SV:170–174 | SV §6.3 | übereinstimmend (Befehle SV:173) | – |
| SV-CREW-006 | SV:176 | SV §6.3 | übereinstimmend | – |
| SV-CREW-007 | SV:177 | SV §6.3 | übereinstimmend | – |
| SV-HQ-001 | SV:181–183 | SV §6.4 | übereinstimmend (Zusatzarbeiten nie Pflicht) | – |
| SV-HQ-002 | SV:184 | SV §6.4 | übereinstimmend | blockiert (K-28) |
| SV-HQ-003 | SV:185–186 | SV §6.4 | übereinstimmend | blockiert (K-28) |
| SV-FALL-001 | SV:196–198; SV:60 | SV §7.1; SV §2.1 | übereinstimmend; das „frühe korrekte Lösen“ steht in SV:60, nicht in SV:196–198 | Zitat im Nachweis ergänzt (kein Dokumentfehler) |
| SV-FALL-002 | SV:200 | SV §7.1 | übereinstimmend | – |
| SV-FALL-003 | SV:201 | SV §7.1 | übereinstimmend | – |
| SV-FALL-004 | SV:202 | SV §7.1 | übereinstimmend | – |
| SV-FALL-005 | SV:210–212 | SV §7.2 | übereinstimmend | – |
| SV-FALL-006 | SV:204 | SV §7.1 | übereinstimmend | – |
| SV-FALL-007 | SV:225–227 | SV §7.4 | übereinstimmend | – |
| SV-ARCHIV-001 | SV:235 | SV §8 | übereinstimmend | – |
| SV-ARCHIV-002 | SV:238–240 | SV §8 | übereinstimmend | – |
| SV-ARCHIV-003 | SV:239, 241 | SV §8 | übereinstimmend | – |
| SV-ARCHIV-004 | SV:242 | SV §8 | übereinstimmend (Monsterstatus) | – |
| SV-ARCHIV-005 | SV:243 | SV §8 | übereinstimmend (Alt-Zahl 1.200–1.500 als 🕰️, neue Zahl offen, ca. 5.000 ❓) | – (Hinweis: „~200 km²“ der Ausgangsliste steht nicht in der Quelle) |
| SV-MONSTER-001 | SV:251–260 | SV §9.1 | übereinstimmend | – |
| SV-MONSTER-002 | SV:258 | SV §9.1 | übereinstimmend (Alter und Erfahrung getrennt) | – |
| SV-MONSTER-003 | SV:264 | SV §9.2 | übereinstimmend („wahre Gestalt enthüllend“ entspricht „Wahrheit enthüllend“) | – |
| SV-MONSTER-004 | SV:265 | SV §9.2 | übereinstimmend | – |
| SV-MONSTER-005 | SV:266–268 | SV §9.2 | übereinstimmend | – |
| SV-MONSTER-006 | SV:273 | SV §9.3 | übereinstimmend | – |
| SV-MONSTER-007 | SV:274 | SV §9.3 | übereinstimmend | – |
| SV-MONSTER-008 | SV:275 | SV §9.3 | übereinstimmend | – |
| SV-MONSTER-009 | SV:276 | SV §9.3 | übereinstimmend (🧭) | – |
| SV-MONSTER-010 | SV:277–278 | SV §9.3 | übereinstimmend | – |
| SV-MONSTER-011 | SV:278 | SV §9.3 | übereinstimmend (Umzug durch Gegner) | – |
| SV-JAEGER-001 | SV:285–286 | SV §10 | übereinstimmend | – |
| SV-JAEGER-002 | SV:287 | SV §10 | übereinstimmend (Namen ❓) | – |
| SV-JAEGER-003 | SV:288 | SV §10 | übereinstimmend | – |
| SV-NEBEN-001 | SV:312 | SV §11.3 | übereinstimmend | – |
| SV-NEBEN-002 | SV:312 | SV §11.3 | übereinstimmend (alle Aktivitäten vorhanden, Motorsport hohe Priorität) | – |
| SV-NEBEN-003 | SV:301–308 | SV §11.2 | übereinstimmend | – |
| SV-NEBEN-004 | SV:297, 308 | SV §11.1, §11.2 | übereinstimmend (Umfang ❓) | – |
| SV-FORT-001 | SV:320 | SV §12.1 | übereinstimmend (Prinzip 📋, Design ❓) | – |
| SV-FORT-002 | SV:326–328 | SV §12.2 | übereinstimmend | – |
| SV-FORT-003 | SV:329–331 | SV §12.2 | übereinstimmend | – |
| SV-FORT-004 | SV:332, 339 | SV §12.2, §12.3 | übereinstimmend (❓) | – |
| SV-FORT-005 | SV:336–338 | SV §12.3 | übereinstimmend | – |
| SV-FORT-006 | SV:343 | SV §12.4 | übereinstimmend (🔭 langfristig) | – |
| SV-MODUS-001 | SV:351 | SV §13 | übereinstimmend | – |
| SV-MODUS-002 | SV:352; GS:415 | SV §13; GS §16 | übereinstimmend (Host-Spielstand; Netzwerk ❓) | Korrektur: K-25-Vermerk (bestätigt 08.11.2026) an SV:352 ergänzt |
| SV-MODUS-003 | SV:353 | SV §13 | übereinstimmend (🔭, ❓) | – |
| SV-MODUS-004 | SV:355 | SV §13 | übereinstimmend (🧭 keine Zusage) | – |
| SV-MONET-001 | SV:362 | SV §13.1 | übereinstimmend (🧭) | – |
| SV-MONET-002 | SV:363–365 | SV §13.1 | übereinstimmend (🧭) | – |
| SV-TOD-001 | SV:373 | SV §14.1 | übereinstimmend (Ausgangsliste nennt 371–373; Inhalt in 373) | – |
| SV-TOD-002 | SV:373 | SV §14.1 | übereinstimmend (Barrierefreiheit als eigene Schicht) | – |
| SV-TOD-003 | SV:377–381 | SV §14.2 | übereinstimmend (Richtung, Schleife ❓) | blockiert (K-26) |
| SV-TOD-004 | SV:382–383 | SV §14.2 | übereinstimmend; Krankenhaus-Abwesenheit bleibt Nutzeraussage (SV:453) | – |
| SV-TOD-005 | SV:384 | SV §14.2 | übereinstimmend | blockiert (K-24) |
| SV-DIALOG-001 | SV:392 | SV §15.1 | übereinstimmend | – |
| SV-DIALOG-002 | SV:394–396 | SV §15.1 | übereinstimmend | – |
| SV-DIALOG-003 | SV:397 | SV §15.1 | übereinstimmend (Zahl ❓) | Korrektur: Zeilenzahlen als vorläufig gekennzeichnet, Zuordnung offen (W-05) |
| SV-DIALOG-004 | SV:401–403 | SV §15.2 | übereinstimmend | – |
| SV-DIALOG-005 | SV:402 | SV §15.2 | übereinstimmend | – |
| SV-DIALOG-006 | SV:405 | SV §15.2 | übereinstimmend | – |
| SV-DIALOG-007 | SV:408–410 | SV §15.3 | übereinstimmend | – |
| SV-GUIDE-001 | SV:416 | SV §16 | übereinstimmend (KI-Assistent über Schnittstelle, nicht Speicherauslesen) | – |
| SV-GUIDE-002 | SV:418 | SV §16 | übereinstimmend (🧭, Preis ❓) | – |
| SV-GUIDE-003 | SV:420 | SV §16 | übereinstimmend (🧭, 100 GB, Offline-Installation) | – |

## Abschlussbericht CONS-GAMEPLAY (08.11.2026)

- **Inhaltlich geprüft:** 222 von 222 Anforderungen (137 GP, 85 SV). Je ID: Anforderungstext aus `Konsolidierung/A-gameplay-spielvision.md` gegen die zitierte Quellzeile in `Gameplay-Systeme.md` bzw. `Spielvision.md` und die Zielstelle verglichen (Bedingungen, Ausnahmen, Zahlen, Status). Verschiedene Zeilen wurden im Volltext oder per Stichwort am Volltext bestätigt.
- **Übereinstimmend:** 215 (210 ohne Änderung, 5 mit Hinweis oder Korrektur). **Blockiert** mit Entwicklerentscheidung: 7 (GP-GORE-001 K-22; GP-BEZIEHUNG-001 und -002 K-23/K-10; SV-HQ-002 und -003 K-28; SV-TOD-003 K-26; SV-TOD-005 K-24).
- **Ausgangsliste falsch oder ungenau (Dokument korrekt):** 7 Fälle, vor allem falsche Statusangaben (vorläufig statt 📋) und Zeilenangaben (z. B. Zitat „fehlt“ bei vorhandenem Inhalt, „Alpha“-Regel in SV:258, „Befehle“ in SV:173). Die Dokumente wurden dafür nicht geändert.
- **Korrigierte Übertragungsfehler:** 1 Zitierfehler im Register (GP-BALLISTIK-001, Zeile 116–118). 1 fehlender Querverweis (Forensik, GS:255 → SV:219). 1 falsche Wortwahl (SV:264 „eine kanonische Schwäche“ → Master-Wortlaut, `BLOCK_09_STATUS.md:5`). 1 Zitat ergänzt (SV-FALL-001 → SV:60 für „frühes Lösen“). 1 Zählung korrigiert (213/7/2).
- **15 Korrekturen (Liste im Gameplay-Nachweis):** 9 erledigt (Nr. 3, 4, 6, 7, 8, 9, 10, 11, 13, 14, 15), 3 Entwicklerentscheidungen (Nr. 1, 2, 5, 12), 0 ohne Lösung. Nr. 12 (Todesmodell) ist nur als Richtung dokumentiert und bleibt K-24/K-26.
- **Nicht zugängliche bzw. nicht geöffnete Originalquellen:** Der Master (CA5, CA6, MR9, MB5, VP, BW, A15, MS13) wurde für die Gegenprüfung nur punktuell geöffnet (`BLOCK_09_STATUS.md:5`). Die Ausgangsliste verweist für die meisten Anforderungen auf GS/SV selbst. Die Master-Stellen, die der Nachweis zitiert, sind daher nicht erneut inhaltlich geprüft.
- **Nächster Einstiegspunkt:** Entwicklerentscheidungen K-22, K-23/K-10, K-24, K-26, K-28; danach Master-Gegenprüfung der zitierten Master-Stellen (CA5:39, CA5:44, CA5:50, MR9:7–14) als eigener Schritt.
- **Keine Spielentscheidungen** getroffen, keine Spielcodeänderungen, keine Commits oder Pushes.
