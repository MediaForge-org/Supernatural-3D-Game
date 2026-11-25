# Entwicklungsbereitschaft (CONSOLIDATION-READINESS, 08.11.2026)

- **Zweck:** Kurzübersicht, welche Aufgaben die nächste Implementierungsphase tragen können. Grundlage: `TASKS.md` (Status und Bemerkung), Nachweise und Quellen. Nur Befunde, keine neuen Entscheidungen.
- **Abgrenzung:** Keine Aussage über Spielgefühl, Visuals oder Performance. Technische Abnahmen sind Entwicklerberichte, nicht von mir erneut ausgeführt.

## 1. Sofort implementierbar (Quelle belegt, kein Designpunkt blockiert)

| Aufgabe | Status | Begründung |
|---|---|---|
| PROTOTYPE-PICKUP | in Arbeit | Automationstests bestanden; Rest ist die PIE-Prüfung (Abschnitt 3). Fortsetzbar. |
| PROTOTYPE-MOVEMENT | in Arbeit | Regressionsbericht 28.10.; offen ist die Abnahme durch den Entwickler. Fortsetzbar. |
| FOUNDATION-INVENTORY | in Arbeit | Core abgeschlossen; UI und Abschluss offen. |
| FOUNDATION-SAVE | in Arbeit | Windows-Haltbarkeit noch unbestätigt (Abschnitt 3). Core-Arbeit fortsetzbar. |
| CASE-DIALOG (Datenmodell) | geplant | Die stabile Dialog-ID je Zeile ist belegt (`SV:392`, 📋). Umfang und Gesprächsarten sind dagegen offen. Das Datenmodell ist sofort möglich. |
| CASE-EVIDENCE (Datenmodell) | geplant | Fakten im WorldFacts-Modell sind belegt (`FOUNDATION-WORLDFACTS`, `Ermittlung/10`). Das Beweisumfang-Modell muss schmal bleiben. |
| MOD-CORE (Kern) | geplant | Block 5 und MR9 sind belegt (Steam nicht Pflicht, `<GameRoot>/mods/`, pro Profil und Spielstand). Das Manifest-Feldmodell ist nicht belegt (Abschnitt 2). |

## 2. Braucht noch eine Designentscheidung

| Aufgabe | Offener Punkt |
|---|---|
| DEC-WORLD-SIZE | Größe der Hauptwelt (N-01). Blockiert alle Karten- und Städtemaße. |
| SLICE-CASE-01 | Akt, Figuren und Fallinhalt (`Nachweis/vertical-slice-kriterien.md`, O1–O6). |
| P0-PLAYABLE-PLACEHOLDER | Reihenfolge laut `README` 27.3 nicht festgelegt. |
| COOP-HOST-SAVE | Netzwerk-Autorität, Synchronisation, Zeitpunkt (K-25 bestätigt nur den Host-Speicherstand). |
| MOD-CORE (Manifest) | Feldliste des Manifests (`Nachweis/gameplay-inhaltspruefung.md`, Korrektur 4: nur Vorschlag). |
| WORLD-REALM-BIBLE-HELL/PURGATORY/HEAVEN | Realm-Bibeln: Zeitverhältnis Hölle, Todesablauf, Kosmologie (Master B9/CA6 offen). |
| DEC-UNREAL-AXES | Achsenzuordnung Unreal (`Coordinates.h`). |
| DEC-P4-DEFINITION | Bedeutung von P4-RELEASE und POST-LAUNCH. |
| DEC-UNDERGROUND-SCOPE | Wortlaut N-02 bestätigen; Geltung U24–U36 durch K-29 offen. |
| CONS-*-BLOCK, CONS-ADULT-OFFEN | Entwicklerentscheidungen K-22, K-23/K-10, K-24, K-26, K-27, K-28, K-31, K-33, K-34. |
| Alle SIM-* (Persistenz, Nemesis, Simulation, Fraktionen) | Abhängig von Ermittlung, Monster-KI und Designklärungen. |
| PROTOTYPE-CASE-01 | Erst nach `P0-ACCEPT-EDITOR` (Aufgabenliste). |

## 3. Braucht zuerst eine Unreal-PIE-Prüfung (Entwickler am Editor)

| Aufgabe | Prüfung |
|---|---|
| P0-ACCEPT-EDITOR | Manuelle Editor-Abnahme: Türen, Pickups, Speichern/Laden, M1-Karte. Grundlage für alles Weitere. |
| PROTOTYPE-DOOR | Manuelle PIE-Abnahme nach `Proposal 0004` §5.2. |
| PROTOTYPE-PICKUP | PIE-Test offen. |
| PROTOTYPE-TESTMAP | Editor-PIE offen (technische Abnahme ist erfolgt, `Proposal 0008` §13). |
| PROTOTYPE-MOVEMENT | Abnahme durch den Entwickler. |
| FOUNDATION-SAVE | Windows-Haltbarkeit der Speicherung. |
| PROTAGONIST-METAHUMAN | Vorschau, nicht freigegeben, nicht integriert. Keine Entwicklungsaufgabe vor Freigabe. |

## 4. Kleinster sinnvoller nächster spielbarer Meilenstein

**Vorschlag (nicht beschlossen):** `PROTOTYPE-CASE-01`, „Ein Raum, ein Zeuge, ein Beweis, ein Abschluss“ (`NAMING_PROPOSAL.md` §4, genehmigt als Test).

Voraussetzungen in dieser Reihenfolge:
1. `P0-ACCEPT-EDITOR` (Entwickler am Editor).
2. Tür und Pickup als abgeschlossen bestätigt (`PROTOTYPE-DOOR`, `PROTOTYPE-PICKUP`).
3. Minimaler Dialog (`CASE-DIALOG`, ein Gespräch) und minimaler Beweis (`CASE-EVIDENCE`, ein Fakt).
4. Ein Zeuge (`NPC-BASIC`, minimal) und ein Abschluss (Journal-Eintrag über WorldFacts).

Blocker heute: 3 und 4 sind `geplant`. Der Meilenstein ist damit der kleinste Schritt, der die sieben Slice-Schritte (K4) in Miniatur durchläuft. Er ist kein Vertical Slice.

## 5. Zusammenfassung

- **Sofort:** Fortsetzung der fünf `in Arbeit`-Aufgaben und die Datenmodelle für Dialog, Beweis und Mod-Kern.
- **Designentscheidung zuerst:** Weltgröße, Slice-Inhalt, Netzwerk, Realm-Bibeln, Achsen, Konfliktpunkte.
- **PIE-Prüfung zuerst:** `P0-ACCEPT-EDITOR` ist der Engpass für alles, was spielbar sein soll.
- **Kleinster Meilenstein:** `PROTOTYPE-CASE-01` nach den vier Voraussetzungen oben.
