<#
.SYNOPSIS
  Builds, cooks and packages a Windows DEVELOPMENT build of the prototype (NightBuild06, Phase I). Not a release, not a Steam build.

.DESCRIPTION
  Runs the engine's BuildCookRun for SupernaturalProt (Win64, Development, pak) and cooks only the maps the game flow opens:
    /Game/SpnTest/Lvl_SpnMainMenu        main menu (game-flow entry)
    /Game/SpnWorld/Lvl_SpnWorld           placeholder world ("New game" with -SpnGameplayMap)
    /Game/SpnTest/Lvl_SpnMovementTestV3Interiors   default gameplay map of "New game" without -SpnGameplayMap
    /Game/ThirdPerson/Lvl_ThirdPerson     the project's GameDefaultMap (unchanged; a launch without arguments opens it)
  The archive goes to a NEW folder (it refuses an existing one: nothing is overwritten or deleted). Next to the .exe it writes
  Start_SpnPrototype.bat, which starts the main menu with "New game" -> SpnWorld, like Tools/play_spnworld.ps1 does in the editor build.
  The QA test maps are not removed or changed; they are simply not cooked.

  Needs: the local content (SpnWorld, Fab, Megaplant; see Docs/NightBuild06/ASSET_MANIFEST.md). The editor must be closed (UBT).

  .\Tools\package_dev_build.ps1                       # -> Saved\Packages\Dev_<timestamp>
  .\Tools\package_dev_build.ps1 -ArchiveDir D:\SpnDev
#>
param(
    [string]$ArchiveDir = '',
    [string]$EnginePath = $env:SPN_UE_ROOT
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$project = Join-Path $repo 'Unreal\SupernaturalProt\SupernaturalProt.uproject'
if (-not $EnginePath) { $EnginePath = 'C:\Program Files\Unreal Engine\UE_5.8' }
$uat = Join-Path $EnginePath 'Engine\Build\BatchFiles\RunUAT.bat'
if (-not (Test-Path $uat)) { throw "RunUAT.bat not found at $uat (set -EnginePath or SPN_UE_ROOT)" }
if (-not $ArchiveDir) { $ArchiveDir = Join-Path $repo ('Unreal\SupernaturalProt\Saved\Packages\Dev_' + (Get-Date -Format 'yyyyMMdd-HHmmss')) }
if (Test-Path $ArchiveDir) { throw "Archive folder $ArchiveDir exists: choose a new one (this script never overwrites a build)" }
if (Get-Process UnrealEditor -ErrorAction SilentlyContinue) { throw 'The Unreal editor is running: close it first (UBT refuses to build while Live Coding is active)' }

$maps = '/Game/SpnTest/Lvl_SpnMainMenu+/Game/SpnWorld/Lvl_SpnWorld+/Game/SpnTest/Lvl_SpnMovementTestV3Interiors+/Game/ThirdPerson/Lvl_ThirdPerson'
$log = Join-Path $repo ('Unreal\SupernaturalProt\Saved\Logs\package_dev_' + (Get-Date -Format 'yyyyMMdd-HHmmss') + '.log')
$uatArgs = @('BuildCookRun', "-project=$project", '-noP4', '-platform=Win64', '-clientconfig=Development', '-build', '-cook', "-map=$maps",
    '-stage', '-pak', '-archive', "-archivedirectory=$ArchiveDir", '-unattended', '-utf8output', '-nocompileeditor')
Write-Host "BuildCookRun -> $ArchiveDir (log $log)"
$start = Get-Date
& $uat @uatArgs 2>&1 | Tee-Object -FilePath $log | Select-String -Pattern 'BUILD SUCCESSFUL|BUILD FAILED|AutomationTool exiting|Error:|error ' | Select-Object -Last 25 | ForEach-Object { $_.Line }
$code = $LASTEXITCODE
Write-Host ("BuildCookRun exit code $code after {0:N1} min" -f ((Get-Date) - $start).TotalMinutes)
if ($code -ne 0) { exit $code }

# the launcher stub at the top of the staged folder (it forwards its arguments to Binaries\Win64\SupernaturalProt.exe)
$exe = Get-ChildItem $ArchiveDir -Recurse -Filter 'SupernaturalProt.exe' | Sort-Object { $_.FullName.Length } | Select-Object -First 1
if ($null -eq $exe) { throw "no SupernaturalProt.exe under $ArchiveDir" }
$bat = Join-Path $exe.DirectoryName 'Start_SpnPrototype.bat'
@(
    '@echo off',
    'rem Development build of the SPN prototype (placeholder world, not a release). Main menu; "New game" opens SpnWorld.',
    'start "" "%~dp0SupernaturalProt.exe" /Game/SpnTest/Lvl_SpnMainMenu -SpnGameplayMap=/Game/SpnWorld/Lvl_SpnWorld -windowed -ResX=1920 -ResY=1080 %*'
) | Set-Content -Encoding ascii $bat
$size = (Get-ChildItem $ArchiveDir -Recurse -File | Measure-Object Length -Sum).Sum
Write-Host ("packaged: {0} ({1:N2} GB), starter {2}" -f $exe.FullName, ($size / 1GB), $bat)
