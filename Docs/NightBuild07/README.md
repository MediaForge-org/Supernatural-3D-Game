# NightBuild07: erster Fall, Night Build A (2026-11-25/26)

Auftrag des Entwicklers vom 2026-11-26: „mache als erstes die erste nachtschicht“. Gemeint ist **Night Build A** aus
`Docs/Proposals/0019-first-case-silent-mill.md` §5:
- Karte des Falls als Platzhalter;
- Kreatur: Patrouille, Hören, Verfolgung, Betäubung durch die Glocke;
- Wurfgegenstände;
- Tod, Todesbildschirm und Neustart.

Arbeitsteilung laut Entwickler: Waffen, Autos und Assets macht der Entwickler später, Claude macht die grobe Form und die Systeme.

**Nichts wurde committet, gepusht oder getaggt.** Der Commit-Plan steht in §8.

## 1. Ergebnis in einem Satz
Der Fall läuft als Graubox: Aus dem Hauptmenü geht es per „Neues Spiel“ in die Karte `Lvl_SpnCaseMill`. Dort gibt es:
- ein Dorf mit Straße und Bücherei, einen Waldrand und die Mühle;
- in der Mühle eine **blinde Kreatur, die nach Gehör jagt**;
- eine **Glocke, die sie betäubt**;
- **Flaschen als Köder**;
- **Autosaves**;
- einen **Todesbildschirm** mit „Letzten Speicherstand laden“. Danach geht es mit der Gesundheit des Schwierigkeitsgrads weiter.

Starten: `.\Tools\play_case_mill.ps1` (optional `-Difficulty test_hardest`).

## 2. Was gebaut wurde

| Teil | Wo | Status |
|---|---|---|
| Kreatur-Regeln (Core, ohne Unreal) | `Core/include/spn/core/Creature.h`, `Core/src/Creature.cpp` | umgesetzt + getestet |
| Schwierigkeitsprofile (beliebig viele, Daten) | `Core/include/spn/core/Difficulty.h`, `Core/src/Difficulty.cpp` | umgesetzt + getestet |
| Unreal-Hülle der Core-Regeln | `Source/SpnCore/Public/SpnCreature.h`, `Private/SpnCreature.cpp` | umgesetzt |
| Kreatur (Platzhalter-Körper) | `Spn/Case/SpnCaseCreature.h/.cpp` (`ASpnCreature`) | umgesetzt + getestet |
| Fall-Rahmen: Gesundheit, Tod, Neustart, Autosave, Speichersperre, Glocke, Würfe | `Spn/Case/SpnCaseSubsystem.h/.cpp` | umgesetzt + getestet |
| Glocke, Flasche, geworfene Flasche, Autosave-Punkt | `Spn/Case/SpnCaseObjects.h/.cpp` | umgesetzt + getestet |
| Werftaste **G** | `SupernaturalProtCharacter.h/.cpp` (`DoThrow`) | umgesetzt (Taste nur über QA-Inventar geprüft, nicht gedrückt) |
| Todesbildschirm, Speichern im Pausemenü gesperrt | `Spn/UI/SpnUiSubsystem.h/.cpp`, `SpnUiTextNightBuild.inc` | umgesetzt + getestet + Screenshot angesehen |
| Fall-Karte (Graubox) | `Tools/create_spn_case_mill_map.py` → `Content/SpnTest/Lvl_SpnCaseMill.umap` | erzeugt + getestet + Screenshots angesehen |
| Startskript | `Tools/play_case_mill.ps1` | umgesetzt (nicht selbst gespielt) |
| Werte (zwei TEST-Profile) | `Config/DefaultGame.ini` `[/Script/SupernaturalProt.SpnCaseSettings]` | Platzhalter |
| Tests | `Tests/Core/CreatureTests.cpp`, `Spn/Tests/SpnCaseAutomation.cpp`, `SpnCaseMapAutomation.cpp`, `SpnCaseFlowAutomation.cpp` | siehe §4 |
| Fab-Wunschliste | `Docs/NightBuild07/FAB_WISHLIST.md` | für dich zum Durchklicken |

## 3. Wie es sich verhält (Platzhalterwerte)

- **Die Kreatur ist blind.** Sie weiß nur, wo ein **Geräusch** war, oder dass sie etwas **berührt** (1,5 m). Wo der Spieler ist, weiß sie
  nicht. So steht es in `Docs/Game/Kreaturen/10` §1: Wahrnehmung mit Unsicherheit.
- **Leise Geräusche** wie Schritte im Gehen oder Sprinten in der Ferne werden **untersucht**. Ein **lautes Geräusch** (Flasche, Glocke,
  später ein Schuss) in bis zu 25 m startet eine **Verfolgung**.
- **Vorsprung:** Vor der Verfolgung **lauscht** sie erst. Wie lange, legt der Schwierigkeitsgrad fest:
  - Testprofil „standard“: 1,5 s;
  - Testprofil „hardest“: 0 s.
- **Suche:** Hört sie nichts mehr, sucht sie dort, wo das letzte Geräusch war. Der Suchradius wächst mit der Zeit (Genauigkeit sinkt).
  Die Suche dauert je nach Schwierigkeitsgrad 20 bzw. 45 s. Danach ist sie eine Weile **wachsamer** und hört weiter.
- **Hängt sie an einer geschlossenen Tür fest,** gibt sie nach 12 s ohne neues Geräusch auf und sucht.
- **Glocke:** betäubt jede Kreatur in 15 m, Wände halbieren die Reichweite. Die Betäubung dauert 8 s, danach sucht sie dort, wo sie stand.
  Das ist eine Mechanik, kein Bonusschaden (S-08).
- **Flasche:** E nimmt eine (höchstens 3), G wirft sie. Der Aufprall ist ein lautes Geräusch, also ein Köder.
- **Tod:** Erwischt sie den Spieler, ist er tot, eine Verletzung gibt es noch nicht. Es erscheint der Todesbildschirm mit
  „Letzten Speicherstand laden“ oder „Hauptmenü“; Esc schließt ihn nicht.
- **Neustart:** Es wird der neueste Spielstand geladen, Autosave oder manuell. Die Gesundheit ist dann die des Schwierigkeitsgrads
  (TOD-04): standard 60 %, hardest 10 %. Die Kreatur steht wieder am Anfang ihrer Runde.
- **Autosaves:** bei der Ankunft und vor der Mühle, je einmal pro Sitzung. Während einer Jagd werden sie verweigert.
- **Speichern:** Während die Kreatur jagt, ist „Spiel speichern“ im Pausemenü gesperrt, mit Hinweis (TOD-03).
- **Schwierigkeitsgrade:** eine Liste in der Config mit beliebig vielen Einträgen (DIF-01). Es gibt nur zwei TEST-Profile, die echten
  Stufen und Namen legst du fest.

## 4. Tests

| Test | Ergebnis |
|---|---|
| Core `ctest` (vorher 702) | **721/721** |
| davon neu: `CreatureTests.cpp` (Kreatur + Schwierigkeit) | 19/19 |
| Python (`Unreal/SupernaturalProt/Tools`) | **227/227** |
| QA-Inventar | 0 Probleme (35 Systeme) |
| Unreal headless `SpnGame+SpnCore` (vorher 193) | **199/199** |
| davon neu `SpnGame.Case.*` (6 Tests: Hören, Vorsprung je Schwierigkeit, Glocke nur in Reichweite, Fang → Tod → Neustart mit 60 % / 10 %, Speichersperre, Flasche als Köder) | 6/6 |
| `SpnCaseQA.Mill.ContentNavigationAndShots` (gerendert, `-game` auf der Karte) | Success |
| `SpnCaseQA.Flow.DeathScreenAndRespawnFromTheAutosave` (gerendert, vom Hauptmenü aus) | Success |
| QA `Full` (vorher 360 / 0) | **PASS 360, FAIL 0** (NOT_IMPLEMENTED 2, NOT_TESTED 2, wie vorher; Lauf 20261126-013144) |

**Im Bild angesehen** (`Unreal/SupernaturalProt/Saved/SpnCase/shots/`, nicht im Repo):
- Start im Dorf;
- Straße zur Mühle: wirkt nachts, die Laternen geben warmes Licht, der Himmel ist dunkel;
- Mühlenhalle (sehr dunkel);
- Todesbildschirm;
- Pausemenü mit gesperrtem Speichern.

Das ist eine **technische Sichtprüfung**, keine Bewertung von Grusel oder Spielspaß.

**Beim Testen gefunden und behoben:**
0. **Mein eigener Fehler:** Meine gerenderten Fall-Tests hießen zuerst `SpnQA.*`. Dadurch hat QA Full sie mitgenommen, auf einer
   fremden Karte. Ein ungeschützter Array-Zugriff im Test hat dann das Spiel abstürzen lassen; der Lauf 20261126-011506 hatte
   0 Ergebnisse. Die Tests heißen jetzt `SpnCaseQA.*`, alle Schritte sind abgesichert, und QA Full ist danach wieder 360/0.
1. Die Kreatur stand anfangs **im Mühlstein** und ein Patrouillenpunkt lag im Getriebe-Block. Der Kartentest meldete „1 von 9 Punkten
   erreichbar“.
2. **Die Türöffnungen (110 cm) waren für die Navigation zu schmal,** sobald ein offenes Türblatt darin steht. Die Kreatur blieb in der Tür
   hängen. Die Mühle hat jetzt 160 cm breite Türen. **Das betrifft auch die alten Testkarten:** Dort laufen NPCs nur dank der
   Geradeaus-Notsteuerung durch Türen (NightBuild05). Die alten Karten habe ich nicht geändert.
3. Das erste Licht sah nach **Dämmerung** aus, weil der Mond den Himmel wie eine Sonne erhellt hat. Jetzt ist es Nacht.
4. Ein Spieler, der **im** Autosave-Bereich startet, löste kein Autosave aus. Jetzt wird das kurz nach dem Start geprüft.
5. Im Wurftest flog die Flasche über den Spieler hinweg, und die Kreatur lief auf dem Weg zum Aufprall durch ihn durch. Das Verhalten
   ist richtig, der Test war falsch aufgebaut und wirft jetzt seitlich.

## 5. Nicht gemacht und bekannte Grenzen

- **Nicht selbst gespielt.** Ob es gruselig ist, ob die Werte passen und ob man der Kreatur entkommt, musst du beim Spielen sagen.
- **Keine Ducken-Taste:** Das Spiel hat keine. Leise ist die Gangart Walk (V).
- **Kein Ton:** keine Glocke, keine Schritte zu hören, kein „Stille“-Effekt (das kommt in Night Build C).
- **Gesundheit und aufgehobene Flaschen werden nicht gespeichert.** Eine genommene Flasche liegt nach dem Laden wieder da.
- **Die Kreatur öffnet keine Türen.** Deshalb stehen die Türen auf ihrer Runde offen; die Tür Halle-Ost ist zu (Versteck).
- **Der zweite Autosave-Punkt** (vor der Mühle) ist nicht in einem echten Lauf geprüft, nur der erste.
- **Die neuen Tests laufen nicht im `run_gameplay_qa.ps1`-Lauf mit.** Sie laufen über eigene Aufrufe; die Befehle stehen in den
  Dateiköpfen von `SpnCaseMapAutomation.cpp` und `SpnCaseFlowAutomation.cpp`. Eine eigene QA-Suite „Case“ wäre der nächste Schritt.
- **Nicht in Night Build A** (laut 0019 kommt das später):
  - Dialoge, Fallakte, Bücherei-Recherche, Anruf (B);
  - Pistole, Endentscheidung, neues Hauptmenü, Taschenlampe, Stille-Effekt (C);
  - Sammelobjekte.
- **Master-Abgleich:** `Planning/Master` ist hier nicht erreichbar. Abgeglichen habe ich mit der Planung im Repo (`Docs/Planning`,
  `Docs/Game/Kreaturen/10`). Der Fall bleibt ein Vorschlag.

## 6. Deine Entscheidungen
1. **Spielen und Werte sagen:**
   - Ist die Kreatur zu schnell oder zu langsam (Jagd 5,6 m/s, dein Sprint ca. 8 m/s)?
   - Ist der Vorsprung richtig?
   - Ist die Mühle zu dunkel?
2. **Soll es eine Ducken-Taste geben** (leiser als Gehen)? Für ein Schleich-Horrorspiel ist das üblich. Sie würde Kapsel, Kamera und
   Türen berühren.
3. **Türbreite:** Sollen alle Testkarten breitere Türen bekommen, damit NPCs richtig navigieren? Oder bleiben 110 cm, und NPCs bekommen
   eine andere Lösung?
4. **Fab:** Die Wunschliste durchklicken (`FAB_WISHLIST.md`).

## 7. Branch oder main?
Deine Frage vom 26.11. Empfehlung: **ein eigener Branch für jede Nachtschicht**, zum Beispiel `nightbuild/07-case-a`.
- Damit liegt alles Neue zusammen und lässt sich als Ganzes ansehen, testen und notfalls verwerfen, ohne dass `main` betroffen ist.
- Wenn es passt, holst du den Branch mit einem Merge nach `main`, wie gestern.

Die Änderungen liegen ungespeichert im Arbeitsverzeichnis. `git switch -c` nimmt sie in den neuen Branch mit.

## 8. Commit-Plan (zum Kopieren; Claude hat nichts committet)

```powershell
cd C:\Projekte\Supernatural-3D-Game
git switch -c nightbuild/07-case-a

git add -- Core Tests/Core
git commit -m "feat(core): creature brain (blind, hunts by sound, bell stun) and difficulty profiles (NightBuild07)"

git add -- Unreal/SupernaturalProt/Source Unreal/SupernaturalProt/Config
git commit -m "feat(unreal): first case night build A - creature, bell, throwables, death screen and respawn, autosaves, save block (NightBuild07)"

git add -- Unreal/SupernaturalProt/Tools Tools/play_case_mill.ps1 Docs/Development/Gameplay-QA-Inventory.md Unreal/SupernaturalProt/Content/SpnTest/Lvl_SpnCaseMill.umap
git lfs status
git commit -m "chore(case): greybox case level Lvl_SpnCaseMill, its generator and start script, QA inventory (NightBuild07)"

git add -- Docs/NightBuild07 Docs/Proposals/0019-first-case-silent-mill.md
git commit -m "docs: NightBuild07 report and Fab wish list"

git status --short
git push -u origin nightbuild/07-case-a
```

Nach dem dritten `git add` muss `git lfs status` die `.umap` als LFS-Objekt zeigen. Am Ende sollte `git status --short` nur noch
`?? Artifacts/VisualProgress/` zeigen.
