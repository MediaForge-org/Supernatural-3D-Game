# Fab-Wunschliste für den ersten Fall „Die Stille Mühle“ (2026-11-26)

Für den Entwickler zum Durchklicken. **Nur Einträge, bei denen auf der Fab-Seite „Free“ / 0 € steht.** Nichts kaufen, keine Testversion.

Claude hat **keinen Fab-Login** und hat keine Fab-Seite geöffnet. Preise und Lizenzen sind deshalb **nicht geprüft**. Bekannt ist nur aus
`Docs/Development/Fab-Asset-Sources.md` (Stand 2026-11-12), dass Epic die Megascans-Bibliothek nach Berichten dauerhaft kostenlos gemacht
hat. Diese Angabe ist ebenfalls nicht selbst auf Fab geprüft. Deshalb stehen hier **Suchbegriffe**, keine erfundenen Links.

## So geht es
1. Im Editor **Window → Fab**, oder fab.com im Browser mit deinem Epic-Konto.
2. Suchbegriff eingeben und den Filter **Price: Free** setzen. Bei Megascans steht „Quixel“ als Anbieter.
3. Prüfen, dass **Fab Standard License** dasteht. Bei **CC BY** bitte den Urheber notieren, das verlangt die Lizenz.
4. Mit **Add to Project** in `SupernaturalProt` übernehmen. Die Assets landen unter `Content/Fab/…` und sind durch `.gitignore` aus Git
   heraus. Das ist Absicht, denn Fab-Inhalte dürfen nicht öffentlich weitergegeben werden.
5. Mir sagen, was du geholt hast. Ich baue es ein und trage Herkunft und Lizenz ins Asset-Manifest ein
   (`Unreal/SupernaturalProt/Tools/asset_manifest.py`).

## Was der Fall braucht (wichtigste zuerst)

| Prio | Wofür | Suchbegriffe auf Fab | Hinweis |
|---|---|---|---|
| 1 | **Mühle**: alte Holz- oder Backsteinwände, Bretter, Balken | `old wooden planks`, `weathered wood beam`, `old brick wall`, `barn` | Megascans-Oberflächen (Surfaces) reichen schon, die Form bleibt Greybox |
| 1 | **Waldrand bei Nacht**: Nadelbäume, Unterholz | `Norway Spruce` (Megaplants), `forest floor`, `fern`, `dead branches` | Fichte und Farn sind in `Fab-Asset-Sources.md` schon als kostenlos gelistet |
| 1 | **Boden**: Schotterweg, Matsch, Gras | `gravel road`, `muddy ground`, `wild grass` | Surfaces |
| 2 | **Die Glocke** | `old bell`, `church bell`, `iron bell` | Wenn es keine kostenlose gibt, bleibt der Kegel, bis du eine machst |
| 2 | **Requisiten**: Flaschen, Säcke, Kisten, Zahnräder, Mühlstein | `glass bottle`, `burlap sack`, `wooden crate`, `millstone`, `rusty gear` | Flaschen sind die Wurfgegenstände |
| 2 | **Dorfhäuser** (nur Außenwände, Fenster, Türen) | `old house facade`, `wooden door`, `window frame` | Die Häuser sind nicht betretbar, außer der Bücherei |
| 3 | **Bücherei**: Regale, Bücher, Tisch, Lampe | `bookshelf`, `old books`, `desk lamp`, `wooden table` | Brauchen wir erst ab Night Build B (Recherche) |
| 3 | **Sammelobjekte** (0019 §4a): Zeitungsausschnitt, Foto, Kassette, Holzfigur | `newspaper`, `old photograph`, `cassette tape`, `wooden figurine` | Gut genug als Platzhalter, später machst du sie selbst |
| 3 | **Straßenlaterne** | `street lamp`, `old lantern` | |

## Was ich NICHT von Fab nehmen würde
- **Kreatur, Waffen, Autos, Spielfigur:** Die machst du selbst (deine Ansage vom 25.11.).
- **Fertige Horror-Packs mit Monstern oder Story-Inhalten:** Die würden die Optik und Lore festlegen, und die ist offen.
- **Alles mit unklarer Lizenz** oder nur in einer „Personal“-Lizenz.
