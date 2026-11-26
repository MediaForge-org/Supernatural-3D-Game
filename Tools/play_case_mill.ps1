<#
.SYNOPSIS
  Starts the game with the main menu; "New game" opens the greybox case level Lvl_SpnCaseMill (first case "Die Stille Mühle", night build A).

.DESCRIPTION
  PLACEHOLDER case (Docs/Proposals/0019, Docs/NightBuild07): grey boxes, an engine-shape creature, no sound, no dialogue yet. What it has:
  a blind creature in the mill that hunts by sound, the bell in the mill hall that stuns it, bottles to throw as lures, autosaves at the arrival
  and before the mill, a death screen with "load the last checkpoint".

  .\Tools\play_case_mill.ps1                         # windowed 1920x1080, default difficulty (test_standard)
  .\Tools\play_case_mill.ps1 -Difficulty test_hardest
  .\Tools\play_case_mill.ps1 -Fullscreen

  Controls: WASD move, mouse look, Shift sprint (loud!), V gait (Walk is the quiet one; there is no crouch key yet), E use (doors, bell,
  bottles), G throw a bottle,
  Esc pause menu (save is refused while the creature hunts you).
  The level must exist: build SupernaturalProtEditor, then run Unreal/SupernaturalProt/Tools/create_spn_case_mill_map.py (see its header).
#>
param(
    [switch]$Fullscreen,
    [int]$Width = 1920,
    [int]$Height = 1080,
    [string]$Difficulty = '',
    [string]$EnginePath = $env:SPN_UE_ROOT
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$project = Join-Path $repo 'Unreal\SupernaturalProt\SupernaturalProt.uproject'
if (-not $EnginePath) { $EnginePath = 'C:\Program Files\Unreal Engine\UE_5.8' }
$exe = Join-Path $EnginePath 'Engine\Binaries\Win64\UnrealEditor.exe'
if (-not (Test-Path $exe)) { throw "Unreal Editor not found at $exe (set -EnginePath or SPN_UE_ROOT)" }
if (-not (Test-Path (Join-Path $repo 'Unreal\SupernaturalProt\Content\SpnTest\Lvl_SpnCaseMill.umap'))) { throw 'Lvl_SpnCaseMill is missing: run Unreal/SupernaturalProt/Tools/create_spn_case_mill_map.py (header of the script)' }
$mode = if ($Fullscreen) { '-fullscreen' } else { "-windowed -ResX=$Width -ResY=$Height" }
$diff = if ($Difficulty) { "-SpnDifficulty=$Difficulty" } else { '' }
$args = "`"$project`" /Game/SpnTest/Lvl_SpnMainMenu -game -SpnGameplayMap=/Game/SpnTest/Lvl_SpnCaseMill $diff $mode"
Write-Host "Starting: main menu -> New game opens /Game/SpnTest/Lvl_SpnCaseMill $diff"
Start-Process -FilePath $exe -ArgumentList $args
