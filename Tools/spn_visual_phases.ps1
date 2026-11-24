<#
.SYNOPSIS
  Rebuilds the placeholder world phase by phase (C..H, cumulative) and records one video per phase into Artifacts/VisualProgress,
  then the showcase. Long-running (about 80 minutes). Each video shows the real level as built for that phase.
.EXAMPLE
  .\Tools\spn_visual_phases.ps1                 # everything
  .\Tools\spn_visual_phases.ps1 -Phases G,H     # only these phases (the world stays at the last one built)
#>
[CmdletBinding()]
param([string[]]$Phases = @('C', 'D', 'E', 'F', 'G', 'H'), [switch]$NoShowcase, [switch]$AllowEditor)
$ErrorActionPreference = 'Continue'
$repo = Split-Path -Parent $PSScriptRoot
$out = Join-Path $repo 'Artifacts\VisualProgress'
$cine = Join-Path $repo 'Unreal\SupernaturalProt\Tools\spn_world\cine'
$map = '/Game/SpnWorld/Lvl_SpnWorld'
$python = 'C:\Program Files\Unreal Engine\UE_5.8\Engine\Binaries\ThirdParty\Python3\Win64\python.exe'
$walk = 'overview,highway_west,main_street,oak_street,house_close,gas_station'
$plan = @{
    C = @('02_Roads.mp4', 'tour.json', $walk); D = @('03_Buildings.mp4', 'tour.json', $walk); E = @('04_Materials.mp4', 'tour.json', $walk)
    F = @('05_Vegetation.mp4', 'tour.json', 'overview,highway_west,oak_street,house_close,forest_track,cabin_pond,creek')
    G = @('06_Lighting.mp4', 'lighting.json', ''); H = @('07_DetailPass.mp4', 'tour.json', 'main_street,gas_station,motel,church,farm,oak_street,cabin_pond')
}
$extra = @(); if ($AllowEditor) { $extra += '-AllowEditor' }
$Phases = @($Phases | ForEach-Object { $_ -split ',' } | Where-Object { $_ })
foreach ($phase in $Phases) {
    Write-Host "=== phase $phase ==="
    & pwsh -NoProfile -File (Join-Path $PSScriptRoot 'spn_world_build.ps1') -Phase $phase | Select-Object -Last 2
    if ($LASTEXITCODE -ne 0) { Write-Host "phase ${phase}: world build FAILED, no video"; continue }
    & $python -I (Join-Path $repo 'Unreal\SupernaturalProt\Tools\spn_world\make_cine.py') | Out-Null
    $file, $spec, $shots = $plan[$phase]
    $args2 = @('-Spec', (Join-Path $cine $spec), '-Map', $map, '-Name', "phase$phase", '-Out', (Join-Path $out $file), '-TimeoutMinutes', '40') + $extra
    if ($shots) { $args2 += @('-Shots', $shots) }
    & pwsh -NoProfile -File (Join-Path $PSScriptRoot 'spn_visual.ps1') @args2 | Select-Object -Last 1
}
if (-not $NoShowcase -and ($Phases -contains 'H')) {
    Write-Host "=== showcase ==="
    & pwsh -NoProfile -File (Join-Path $PSScriptRoot 'spn_visual.ps1') -Spec (Join-Path $cine 'showcase.json') -Map $map -Name showcase -Out (Join-Path $out '09_Final_Showcase.mp4') -TimeoutMinutes 60 @extra | Select-Object -Last 1
}
