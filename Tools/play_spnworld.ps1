<#
.SYNOPSIS
  Starts the game with the main menu; "New game" opens the placeholder world SpnWorld at the start of the gameplay route (no developer console needed).

.DESCRIPTION
  Uses the existing session options of the game: the main menu level is the game-flow opt-in, and -SpnGameplayMap= tells "New game" which level
  to open (default without it: the test level V3Interiors). Saving and loading work through the pause menu (Esc); a save made in SpnWorld is
  loaded back into SpnWorld (the level of the saved pose decides). Needs the editor build (SupernaturalProtEditor) and the generated world.

  .\Tools\play_spnworld.ps1                 # windowed 1920x1080
  .\Tools\play_spnworld.ps1 -Fullscreen

  Controls (game defaults): WASD move, mouse look, Shift sprint, V gait, Space jump, E use (doors, keys, items, switches), F force,
  I inventory (items and their descriptions), M map, Esc pause menu (save, load, settings).
#>
param(
    [switch]$Fullscreen,
    [int]$Width = 1920,
    [int]$Height = 1080,
    [string]$EnginePath = $env:SPN_UE_ROOT
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$project = Join-Path $repo 'Unreal\SupernaturalProt\SupernaturalProt.uproject'
if (-not $EnginePath) { $EnginePath = 'C:\Program Files\Unreal Engine\UE_5.8' }
$exe = Join-Path $EnginePath 'Engine\Binaries\Win64\UnrealEditor.exe'
if (-not (Test-Path $exe)) { throw "Unreal Editor not found at $exe (set -EnginePath or SPN_UE_ROOT)" }
if (-not (Test-Path (Join-Path $repo 'Unreal\SupernaturalProt\Content\SpnWorld\Lvl_SpnWorld.umap'))) { throw 'Lvl_SpnWorld is missing: build it with Tools\spn_world_build.ps1 -Phase H' }
$mode = if ($Fullscreen) { '-fullscreen' } else { "-windowed -ResX=$Width -ResY=$Height" }
$args = "`"$project`" /Game/SpnTest/Lvl_SpnMainMenu -game -SpnGameplayMap=/Game/SpnWorld/Lvl_SpnWorld $mode"
Write-Host "Starting: main menu -> New game opens /Game/SpnWorld/Lvl_SpnWorld"
Start-Process -FilePath $exe -ArgumentList $args
