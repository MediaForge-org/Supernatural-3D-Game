# UNTESTED: written on Fedora without Unreal or PowerShell. Read it before running; every step prints the command it runs.
# Runs, in order: Core tests (CMake/CTest), Python tests, editor build, map generation (v2), log verdict, automation tests.
# Needs: UE_ROOT (e.g. C:\Program Files\Epic Games\UE_5.8), python on PATH. Nothing here commits, pushes or deletes files.
# Usage:  powershell -File Tools\windows\run_spn_checks.ps1 [-SkipBuild] [-Instancing] [-Force] [-AdoptLegacy]
param([switch]$SkipBuild, [switch]$Instancing, [switch]$Force, [switch]$AdoptLegacy)
$ErrorActionPreference = "Stop"
if (-not $env:UE_ROOT) { throw "Set UE_ROOT to the Unreal Engine 5.8 install directory." }
$repo = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$project = Join-Path $repo "Unreal\SupernaturalProt\SupernaturalProt.uproject"
$editorCmd = Join-Path $env:UE_ROOT "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$build = Join-Path $env:UE_ROOT "Engine\Build\BatchFiles\Build.bat"
$logDir = Join-Path $repo "Saved-SpnChecks"
New-Item -ItemType Directory -Force -Path $logDir | Out-Null

function Step($name, [scriptblock]$body) {
    Write-Host "=== $name"
    & $body
    if ($LASTEXITCODE -ne 0) { throw "step failed: $name (exit $LASTEXITCODE)" }
}

Step "Core tests (CMake preset from CMakePresets.json)" {
    cmake --preset clang 2>&1 | Out-Host; cmake --build --preset clang 2>&1 | Out-Host; ctest --preset clang 2>&1 | Out-Host
}
Step "Python tests" {
    Push-Location (Join-Path $repo "Unreal\SupernaturalProt")
    python -m unittest discover -s Tools -p "test_spn_*.py" 2>&1 | Out-Host
    Pop-Location
}
if (-not $SkipBuild) {
    Step "Editor build (compiles the new SpnMovementDiagnostics and the changed SpnCore files)" {
        & $build SupernaturalProtEditor Win64 Development "-Project=$project" -WaitMutex 2>&1 | Out-Host
    }
}
$flags = "--v2"
if ($Instancing) { $flags += " --instancing" }
if ($Force) { $flags += " --force" }
if ($AdoptLegacy) { $flags += " --adopt-legacy" }
$genLog = Join-Path $logDir "generator.log"
Step "Map generation: $flags" {
    $script = Join-Path $repo "Unreal\SupernaturalProt\Tools\create_spn_movement_test_map.py"
    & $editorCmd $project -run=pythonscript "-script=`"$script $flags`"" -unattended -nullrhi "-abslog=$genLog" 2>&1 | Out-Host
}
Step "Generator verdict" {
    python (Join-Path $repo "Unreal\SupernaturalProt\Tools\spn_log_tools.py") generator $genLog
}
$autoLog = Join-Path $logDir "automation.log"
Step "Automation tests (SPN.* filter; adjust the filter to the names your automation tests register)" {
    & $editorCmd $project "-ExecCmds=Automation RunTests SPN; Quit" -unattended -nullrhi "-abslog=$autoLog" 2>&1 | Out-Host
}
Write-Host "Logs: $logDir. Jump recorder: in PIE run 'spn.MoveDiag 1', jump at the 30/40/45 cm walls, then:"
Write-Host "  python Unreal\SupernaturalProt\Tools\spn_log_tools.py diag <editor.log> --ledge-cm 30 --distance-cm <measured take-off distance>"
