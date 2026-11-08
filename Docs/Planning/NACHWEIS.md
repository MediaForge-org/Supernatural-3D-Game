# Nachweisregister

Stand: 08.11.2026. Jede Anforderung aus den Konsolidierungsberichten bekommt hier eine Zeile. Status-Werte:

- **übernommen**: inhaltlich in der Zieldatei belegt (Quelle und Ziel gelesen und verglichen).
- **vorhanden**: war bereits in der Zieldatei, inhaltlich geprüft.
- **blockiert**: wartet auf eine Entscheidung (Verweis auf K-ID).
- **offen**: Entscheidung oder Klärung nötig, nicht blockierend.
- **verworfen**: bewusst nicht übernommen (Begründung nötig).
- **nicht geprüft**: noch nicht gegen Quelle und Ziel abgeglichen. Das gilt bis zur Prüfung.

Eine Anforderung gilt nur dann als konsolidiert, wenn Quelle und Ziel inhaltlich verglichen wurden. Ein Verweis oder eine Stichwortübereinstimmung reicht nicht.

## Systemintegration (Lücken L-01 bis L-20, Bericht C)

| Lücke | Quelle | Ziel | Status |
|---|---|---|---|
| L-01 Todesmodell | C-21, BW-001/012/028, E-QA-003 | Kampfsystem/27, Ermittlung/20 | blockiert (K-26) |
| L-02 Todesursache wichtiger NPCs | PO-020, E-ZE-007, LT-006, LB-017 | Kampfsystem/27, Ermittlung/20, NPCs/26 | blockiert (K-27) |
| L-03 N-02-Umwidmung in Gesellschaft/Untergrund | SY-WELT-005 | NPCs/, Fahrzeuge/ | nicht geprüft (Einzelstellen fehlen) |
| L-04 Hölle-Zeitverhältnis | W-04, KS-012 | Kreaturen/20 | offen (C-08) |
| L-05 Gehen/Fahren-Kreuz | W-03, ST-009/011 | Fahrzeuge/28, Kampfsystem/27 | offen |
| L-06 L2-Doppelbelegung | VD-ST-003 | Fahrzeuge/28 | offen |
| L-07 Schadenstiefe | W-10, ZV-003/004/013 | Fahrzeuge/28 | offen |
| L-08 Speichergröße | W-12 | NPCs/26, Fahrzeuge/28, Ermittlung/20 | offen |
| L-09 Unreal-Version | W-11, UE-003 | Fahrzeuge/28, CANON | offen (Abgleich) |
| L-10 Fahndungsstufen | W-07 | Ermittlung/20, Fahrzeuge/28 | offen |
| L-11 Hauptgeschichte/Tiere | W-06, W-13 | Ermittlung/20, Kreaturen/20 | übernommen (Status vorläufig) |
| L-12 Sammelobjekte | W-08 | Ermittlung/20 | offen (Widerspruch) |
| L-13 Koop-Missionsfortschritt | W-09, KO-005 | Ermittlung/20 | offen (Host bestätigt, K-25) |
| L-14 DLC-Aussage | W-05, GR-013 | Kreaturen/20 | offen (Klärung) |
| L-15 Einwanderer, tote Personen | W-15/16, ID-023/024 | NPCs/26 | offen |
| L-16 Liste HQ-Figuren | CO-026 | NPCs/26 | nicht ergänzt (Lücke) |
| L-17 Schienen-Betriebsmodul | KL-015 | Fahrzeuge/28 | nicht ergänzt (Lücke) |
| L-18 Weltjournal | WF:36–38, PE-020, PD-002 | NPCs/26, Kreaturen/20, Ermittlung/20 | offen (Technik-Abgleich) |
| L-19 Akzeptanzkriterien fehlen | Testkataloge | Ermittlung/23, Kreaturen/23, NPCs/29, Fahrzeuge/31 | nicht ergänzt (nicht erfunden) |
| L-20 Master-Kanon nicht gelesen | 01_CURRENT_CANONICAL_STATE | — | übernommen (in Paket 2 gelesen) |

## Gameplay und Spielvision (Bericht A)

Die 222 Anforderungen sind im Bericht `Konsolidierung/A-gameplay-spielvision.md` nummeriert (GP-…, SV-…). Dieses Register führt sie zeilenweise nach, sobald der Vollständigkeitsabgleich läuft. Bisher:

- Nachträge in `Gameplay-Systeme.md` und `Spielvision.md` mit Quellen (siehe deren Abschnitt „Nachtrag Master-Abgleich 08.11.2026“): **übernommen** für L-01 bis L-08, L-10, L-12, L-14, L-17 (Bericht A). L-09 (Mod-Manifest) übernommen. L-11 (Vorschlagsdokument) als Proposal-Neuanlage **nicht erfolgt**. L-13 (Bunker-Geheimhaltung) **nicht übernommen** (Spielvision-Spoiler-Regel prüfen). L-15 (Krankenhaus) als Nutzeraussage übernommen. L-16 nur Hinweis.
- Vollständigkeitsabgleich einzelner Anforderungen: **nicht geprüft**.

## Welt (Bericht B)

- Nachträge in World/03, 04, 17, 20, 26 und Spielwelt-und-Untergrund (N-02-Markierung): **übernommen** für L3, L4, L5, L8.
- L1 (N-01 bis N-03 Textfassung): **offen** (Entwickler-Freigabe).
- L2 (PE-E1022): **Docs bereits aktualisiert**, Master-Nachtrag offen.
- L6 (Verrat und Offenlegung): **nicht übernommen** (N-02-Kontext prüfen).
- L7 (Phase-A-Freigabe): **blockiert (K-19)**.
- Vollständigkeitsabgleich der 120 Welt-Anforderungen: **nicht geprüft**.

## Adult, Tagesdeltas

- **Nicht bearbeitet.** Beides folgt in den nächsten Paketen.
