<#
.SYNOPSIS
  Builds the placeholder test world /Game/SpnWorld/Lvl_SpnWorld: terrain tool, textures, then the Unreal generator script.
.EXAMPLE
  .\Tools\spn_world_build.ps1              # full world (phase H)
  .\Tools\spn_world_build.ps1 -Phase C     # only terrain and roads (phases are cumulative: B C D E F G H)
#>
[CmdletBinding()]
param(
    [ValidatePattern('^[B-H]$')][string]$Phase = 'H',
    [string]$Map = '/Game/SpnWorld/Lvl_SpnWorld',
    [switch]$Textures,          # regenerate and re-import the textures
    [string]$EnginePath = $(if ($env:SPN_UE_ROOT) { $env:SPN_UE_ROOT } else { 'C:\Program Files\Unreal Engine\UE_5.8' })
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$proj = Join-Path $repo 'Unreal\SupernaturalProt'
$tool = Join-Path $repo 'build\worldtool\Release\spn_worldtool.exe'
if (-not (Test-Path $tool)) { throw "Build the tool first: cmake -S Tools/SpnWorldTool -B build/worldtool; cmake --build build/worldtool --config Release" }
$gen = Join-Path $proj 'Saved\SpnWorldGen'
$texDir = Join-Path $gen 'textures'
if ($Textures -or -not (Test-Path (Join-Path $texDir 'T_Asphalt_C.tga'))) {
    New-Item -ItemType Directory -Force $texDir | Out-Null
    & $tool textures $texDir 2048 | Out-Null
    $env:SPN_FORCE_TEX = '1'
} else { $env:SPN_FORCE_TEX = '0' }
$env:SPN_PHASE = $Phase; $env:SPN_MAP = $Map
if (-not $env:SPN_WPO_SCALE) { $env:SPN_WPO_SCALE = '0' }      # wind animation cut-off of the scanned vegetation (woodland.py), for A/B runs
$log = Join-Path $gen "build_$Phase.log"
$script = Join-Path $proj 'Tools\spn_world\build_world.py'
$started = Get-Date
& (Join-Path $EnginePath 'Engine\Binaries\Win64\UnrealEditor-Cmd.exe') (Join-Path $proj 'SupernaturalProt.uproject') -run=pythonscript "-script=$script" -unattended -nosplash `
    -AllowCommandletRendering '-EnablePlugins=GeometryScripting,ProceduralVegetationEditor' "-abslog=$log" *> (Join-Path $gen "build_$Phase.out")
$code = $LASTEXITCODE
Select-String -Path $log -Pattern 'SpnWorld: |Traceback|Error: |LogPython: Error' | Select-Object -Last 30 | ForEach-Object { $_.Line -replace '^\[[^\]]*\]\[[^\]]*\]', '' }
Write-Host ("exit $code, {0:N0} s, log $log" -f ((Get-Date) - $started).TotalSeconds)
if (-not (Select-String -Path $log -Pattern 'SpnWorld: created ' -Quiet)) { exit 1 }
exit 0
