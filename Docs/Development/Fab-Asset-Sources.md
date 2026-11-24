# Fab-/Megascans-Assets: Herkunft, Lizenz, Importstand

Stand: 2026-11-12. Technische Arbeitsliste für den visuellen Ausbau der Platzhalterwelt; keine Planungs- oder Kanon-Aussage.

## Lage

- **Lokal ist nichts vorhanden.** Geprüft am 2026-11-12: kein Megascans-/Fab-Inhalt im Projekt (`Content/`), keine Bridge-Bibliothek,
  die Fab-Bibliothek des Launchers (`C:\ProgramData\Epic\EpicGamesLauncher\VaultCache\FabLibrary\listings_v1.db`) ist leer
  (0 Einträge, 0 Downloads).
- **Kein automatischer Zugriff.** Das Fab-Plugin der Engine (`Engine/Plugins/Fab`) ist ein Browserfenster im Editor mit Epic-Anmeldung;
  es gibt keinen Kommandozeilen- oder Python-Weg. Community-Werkzeuge dafür wurden nicht installiert (keine neue Software ohne Freigabe).
- **Preislage laut Berichten:** Epic hat am 2026-11-05 angekündigt, die Megascans-Bibliothek auf Fab dauerhaft kostenlos zu machen; die
  Einträge sollten bis 2026-11-12 umgestellt sein. Quelle: CG Channel und weitere Berichte (Links unten). **Nicht selbst auf Fab
  geprüft** – Preis und Lizenz stehen verbindlich nur auf der jeweiligen Fab-Seite.
- **Lizenz:** Megascans und Megaplants laufen laut Quixel unter der *Fab Standard License* (kommerzielle Nutzung in Spielen erlaubt).
  Einträge anderer Anbieter können eine andere Lizenz haben (z. B. „UE Marketplace License“); je Eintrag prüfen.
- **Git:** Importierte Assets liegen als `.uasset` unter `Content/` und sind durch `.gitignore` (Zeilen 128/129) ausgeschlossen.
  Quelldateien (FBX, Texturen) nicht in das Repository legen.

## Kandidaten (alle: **nicht importiert**, Preis/Lizenz vor Übernahme auf der Fab-Seite bestätigen)

Die Angaben stammen aus Websuche-Ergebnissen vom 2026-11-12, nicht aus einem Blick in die Fab-Seite mit Anmeldung.

### Priorität 1 – Wald (Qualitätsreferenz Waldabschnitt mit Hütte)

| Zweck | Eintrag | Anbieter | Fab-URL | Angabe aus der Suche |
|---|---|---|---|---|
| Nadelbaum | Megaplants: Norway Spruce | Quixel Megaplants | https://www.fab.com/listings/f87364c9-10a2-4834-8566-a0291c717758 | Preis 0; für PVE in UE 5.8; Nanite-Laub |
| Laubbaum | Megaplants: European Beech | Quixel Megaplants | https://www.fab.com/listings/cefe5722-9c31-4aa2-9ee2-e5426610d5e6 | kostenlos gelistet |
| Jungwuchs | Megaplants: European Beech Saplings | Quixel Megaplants | https://www.fab.com/listings/6867d008-fc3a-404e-9e53-087eccdcd722 | kostenlos gelistet |
| Farn | Lady Fern | Quixel Megascans | https://www.fab.com/listings/3eead001-af7f-4b65-afcc-89c368f089bc | Preis nicht in der Suche sichtbar |
| Waldboden (Nadeln, Wurzeln) | Rocky Forest Floor | Quixel Megascans | https://www.fab.com/listings/58ac3569-1e6d-4d2a-9460-6ce1a0918164 | 2,5 × 2,5 m Oberfläche |
| Waldboden (Moos) | Mossy Forest Floor | Quixel Megascans | https://www.fab.com/listings/266b1938-6675-4b37-90db-9ea2982a4207 | 2 × 2 m, 4096 px/m |
| Waldboden (Laub, Erde) | Forest Floor | Quixel Megascans | https://www.fab.com/listings/0b1ca958-28e9-4a46-b894-3b9766e48774 | 2 × 2 m |
| Totholz | Mossy Wooden Log | Quixel Megascans | https://www.fab.com/listings/427b13b8-d9ae-43c6-a963-91cd05e13b1b | ca. 1,0 × 1,5 m |
| Totholz | Nordic Forest Tree Log Small | Quixel Megascans | https://www.fab.com/listings/eb31e43d-46a8-4c49-9e1f-66bfad35bbe3 | ca. 0,8 × 2,2 m |
| Stumpf | Damaged Tree Stump | Quixel Megascans | https://www.fab.com/listings/91426584-3cd3-4911-87d8-9734f4f66afa | ca. 0,6 m hoch |
| Fels | Mossy Rock | Quixel Megascans | https://www.fab.com/listings/c0f1969a-308f-425c-9f2b-d81e59b8178e | ca. 0,8 m |
| Äste/Kleinholz | Tree Debris | Quixel Megascans | https://www.fab.com/listings/3bf21924-a093-42d8-8ba6-cd9f3ad45c5c | Streugut |
| Geländestück | Forest Terrain | Quixel Megascans | https://www.fab.com/listings/8a8981c4-bb94-4184-84b7-6c70d7c75ef1 | 2,5 × 1,7 m |

Hinweis: Fichte und Buche sind europäische Arten. Für nordamerikanische Arten (Douglasie, Hemlock, Zeder) wurde kein Megaplants-Eintrag
gefunden; in der Fab-Seite des Anbieters nachsehen: https://www.fab.com/sellers/Quixel%20Megaplants

### Priorität 3 – Hütte

| Zweck | Eintrag | Anbieter | Fab-URL | Angabe aus der Suche |
|---|---|---|---|---|
| Hütte mit Innenraum | Modular Rural Cabins | Maarten Hof | https://www.fab.com/listings/508fe84a-4976-4cfe-9a40-c2b9533da601 | Preis 0,00 USD; Lizenz in der Suche nicht sichtbar |

### Priorität 2 – Kleinstadt

Noch nicht recherchiert (Asphalt, Beton, Ziegel, Holzverkleidung als Megascans-Oberflächen; Gebäude-Kits anderer Anbieter).

## Importweg (Schritt des Entwicklers)

1. Unreal-Editor mit `Unreal/SupernaturalProt/SupernaturalProt.uproject` öffnen, Fenster **Fab** öffnen (Menü *Window → Fab* oder
   Schaltfläche *Fab* in der Werkzeugleiste), mit dem Epic-Konto anmelden.
2. Je Eintrag oben: Fab-URL bzw. Titel suchen, prüfen dass der Preis **Free** ist und die Lizenz *Standard License* lautet, dann
   **Add to My Library** und **Add to Project**. Qualität: *High* (Megascans-Oberflächen 4K reichen; *Raw* nicht nötig).
   Megaplants: die Variante für **UE 5.8** wählen.
3. Editor danach **schließen** (sonst blockiert er Build und Messung) und Bescheid geben. Die Assets liegen dann unter
   `Content/Fab/...`; welche Pfade genau entstanden sind, wird danach hier eingetragen.

## Tatsächlich importiert (geprüft am 2026-11-12 im Projekt)

Vom Entwickler über das Fab-Fenster mit „Zum Projekt hinzufügen“ importiert. Lizenz laut Entwickler-Auswahl kostenlos; die Lizenzseite
jedes Eintrags wurde von Claude **nicht** selbst eingesehen.

| Asset | Megascans-ID | Unreal-Pfad | Fab-URL | Genutzt für |
|---|---|---|---|---|
| Megaplants: Norway Spruce (Varianten A–D) | – | `/Game/Megaplant_Library/Tree_Norway_Spruce/Tree_Norway_Spruce_01/Tree_Norway_Spruce_01_A…D` | https://www.fab.com/listings/f87364c9-10a2-4834-8566-a0291c717758 | Bäume im Referenzabschnitt |
| Megaplants: European Beech (Varianten A–D) | – | `/Game/Megaplant_Library/Tree_European_Beech/Tree_European_Beech_01/Tree_European_Beech_01_A…D` | https://www.fab.com/listings/cefe5722-9c31-4aa2-9ee2-e5426610d5e6 | Bäume, Jungwuchs |
| Lady Fern (9 Varianten) | wdvlditia | `/Game/Fab/Megascans/Plants/Lady_Fern_wdvlditia/High/wdvlditia_tier_1/StaticMeshes/SM_wdvlditia_VarA…I` | https://www.fab.com/listings/3eead001-af7f-4b65-afcc-89c368f089bc | Farnhorste, Landscape-Gras „Forest“ |
| Mossy Forest Floor | wjvpahk | `/Game/Fab/Megascans/Surfaces/Mossy_Forest_Floor_wjvpahk/High/wjvpahk_tier_1/` | https://www.fab.com/listings/266b1938-6675-4b37-90db-9ea2982a4207 | Landscape-Schicht „Forest“ |
| Rocky Forest Floor | vl0lecrlw | `/Game/Fab/Megascans/Surfaces/Rocky_Forest_Floor_vl0lecrlw/High/vl0lecrlw_tier_1/` | https://www.fab.com/listings/58ac3569-1e6d-4d2a-9460-6ce1a0918164 | Landscape-Schicht „Dirt“ |
| Mossy Wooden Log | rhfdj | `/Game/Fab/Megascans/3D/Mossy_Wooden_Log_rhfdj/High/rhfdj_tier_1/StaticMeshes/rhfdj_tier_1` | https://www.fab.com/listings/427b13b8-d9ae-43c6-a963-91cd05e13b1b | liegende Stämme, Sitzstämme |
| Damaged Tree Stump | rcrwm | `/Game/Fab/Megascans/3D/Damaged_Tree_Stump_rcrwm/High/rcrwm_tier_1/StaticMeshes/rcrwm_tier_1` | https://www.fab.com/listings/91426584-3cd3-4911-87d8-9734f4f66afa | Stümpfe, Hackklotz |
| Mossy Rock Cluster | regsH | `/Game/Fab/Megascans/3D/Mossy_Rock_Cluster_regsH/High/regsH_tier_1/StaticMeshes/regsH_tier_1` | nicht bekannt (anderer Eintrag als „Mossy Rock“ der Liste) | Felsen, Steine der Feuerstelle |

Mit importiert: Fab-Mastermaterialien und -Funktionen unter `/Game/Fab/Materials`, `/Game/Fab/MaterialFunctions`, `/Game/Fab/Textures`,
`/Game/Fab/MaterialParameterCollection` sowie `BP_GlobalFoliageActor` (zweimal: `/Game/` und `/Game/Fab/Megascans/Plants/`).

**Aus der Liste nicht importiert:** European Beech Saplings, Nordic Forest Tree Log Small, Mossy Rock (c0f1969a…), Modular Rural Cabins.

### Änderungen an importierten Assets

- `M_MS_Foliage`, `M_MS_Billboard` und `M_MS_Base_Fuzz` (`/Game/Fab/Materials/Standard/`): Material-Nutzungsflags *Nanite* und *Instanced Static Meshes*
  gesetzt (`fab.ensure_usage`). Ohne sie erscheinen die Assets in einem `-game`-Lauf mit dem Standardmaterial; der Editor setzt dieselben
  Flags beim ersten Platzieren von Hand. Sonst wird nichts unter `/Game/Fab` oder `/Game/Megaplant_Library` verändert, erzeugt oder gelöscht.
- Eigene Ableitungen: Nanite-Kopien der Farne unter `/Game/SpnWorld/FabDerived/SM_Fern_A…I` (die Originale haben LODs und Billboard).
- Projekteinstellung: `r.Nanite.Foliage=True` in `Config/DefaultEngine.ini` (für Megaplants nötig, experimentell).

### Reproduzierbarkeit

Der Weltgenerator (`Tools/spn_world/fab.py`) lädt die Assets über die Pfade oben. Fehlt eines, protokolliert er `fab: missing …` und
verwendet den bisherigen prozeduralen Platzhalter – die Welt baut also auch ohne Fab-Inhalt (z. B. auf dem Fedora-Rechner).
Der Generator löscht nur in `/Game/SpnWorld`; `/Game/Fab` und `/Game/Megaplant_Library` überstehen jeden Neuaufbau.
Alle `.uasset` sind per `.gitignore` ausgeschlossen; `git status` zeigt `Content/Fab` und `Content/Megaplant_Library` nicht.

## Quellen der Preis-/Lizenzangaben

- https://www.cgchannel.com/2026/11/epic-has-made-megascans-free-again-for-good-this-time/
- https://quixel.com/news/discover-the-latest-quixel-megascans-and-free-megaplants
- https://www.3dart.it/en/megascans-free-fab-2026/
