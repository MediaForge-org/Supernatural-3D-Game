# Produktions-Referenzen und Produktionsprompts

> [!IMPORTANT]
> **Stand 08.11.2026. Das Produktionsprompt-System ist nicht abgeschlossen.** Die Master-Quellen legen die Richtung fest, nicht die konkreten Tutorials, Prompt-Vorlagen oder Arbeitsschritte. Alles, was hier nicht als 📋 markiert ist, ist offen.

Statuszeichen: siehe [Spielwelt und Untergrund §1](../Game/Spielwelt-und-Untergrund.md#1-statuskennzeichnung).

## 1. Quellen und Status

| Master-Quelle | Inhalt | Status |
|---|---|---|
| `01_CURRENT_CANONICAL_STATE/CURRENT_CANON_2026-09-16.md` | „Final Blender/environment/asset prompts should incorporate high-quality practical external tutorials/references and concrete visual-reference/scene-setup guidance.“ | 📋 Kanon (laut Master) |
| `01_CURRENT_CANONICAL_STATE/VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10.md` | Externe Referenzen für Blender-Szenen, Umgebungsbau und Asset-Erstellung; praktische Sichtreferenz- und Szenenaufbau-Hinweise | 📋 Kanon (laut Master) |
| `01_CURRENT_CANONICAL_STATE/PLANNING_BLOCK_STATUS_2026-09-16.md` | „Complete production-prompt system: unfinished.“ | Offen (laut Master) |
| `02_EMBEDDED_BASE_ARCHIVES/…` (Prompt-Bibliothek, 3.673 Dateien) | Entwurfsmaterial. Laut `Docs/Planning/SOURCES.md`: „Entwurf, Neuschreibung nötig“. | Nicht Anforderung |

**Regel:** Die Prompt-Bibliothek des Masters ist ein Entwurf (`DRAFT_REWRITE_REQUIRED`). Ihre Inhalte gelten nicht als Anforderung, bevor sie neu geschrieben und geprüft sind.

## 2. Gesicherte Anforderungen (📋)

1. **Externe Referenzen.** Produktionsprompts für Blender, Umgebungen und Assets sollen hochwertige, praxisnahe externe Tutorials und Referenzen einbeziehen, wo sie nützlich sind (Quelle: CURRENT_CANON_2026-09-16).
2. **Konkrete Szenenvorgaben.** Prompts sollen Sichtreferenzen und Hinweise zum Szenenaufbau enthalten, damit sie nicht nur textuell bleiben (Quelle: CURRENT_CANON_2026-09-16 und VOICE_PRODUCTION_AND_REFERENCE_WORKFLOW_2026-09-10).
3. **Entwickler-Abnahme.** Wahre Formen und Anatomien brauchen Konzept, Anatomiebogen, Blender-Prototyp und Abnahme durch den Entwickler, bevor eine Darstellung freigegeben wird (`Docs/Game/Kreaturen/20-Entscheidungsregister.md`, K-HW-015; Konzept in `Docs/Game/Kreaturen/19-Artennennungen-und-Artenvorlage.md`).
4. **Stimmproduktion** läuft nach einem eigenen Referenz-Workflow: freiwillige Freunde oder Familie liefern saubere Referenzaufnahmen; Einwilligung, kommerzielle Rechte, Nutzungsumfang, Aufbewahrung und Löschung sind Pflicht (`Docs/Game/Spielvision.md` §15.2 und Ergänzung „Stimmen“; `Docs/Planning/DECISIONS.md` C-40).

## 3. Offen

- Auswahl der konkreten externen Tutorials und Referenzquellen. Dies ist eine Entwicklerentscheidung; die Quellen nennen keine.
- Gliederung und Pflichtfelder einer Produktionsprompt-Vorlage (Asset, Szene, Referenz, Stil, Abnahme).
- Zuständigkeit: wer schreibt, prüft und pflegt die Prompts; Versionierung.
- Verhältnis zwischen Prompt-Vorlagen und den Asset-Pipelines (Blender, Unreal-Import).
- Neuschreibung der Prompt-Bibliothek des Masters: nicht begonnen. Sie ist Entwurf und keine Quelle für Anforderungen.

## 4. Abgrenzung

- Dieses Dokument ist keine Asset-Spezifikation. Konkrete Gestalt, Anatomie und Figuren stehen in `Docs/Game/Kreaturen/`.
- Keine Produktionsprompts werden hier festgeschrieben. Das System ist nicht freigegeben.

## 5. Zugehörige Dokumente

- [Spielvision](../Game/Spielvision.md): Stimmen und Referenzaufnahmen.
- [Gameplay-Systeme](../Game/Gameplay-Systeme.md): Produktionsanforderungen für Animation und Verletzung.
- [Kreaturen 19](../Game/Kreaturen/19-Artennennungen-und-Artenvorlage.md) und [Kreaturen 20](../Game/Kreaturen/20-Entscheidungsregister.md): Blender-Prototyp und Abnahme.
- `Docs/Planning/Nachweis/master-zuordnung-01-current.md`: Zuordnung der Master-Quellen.
