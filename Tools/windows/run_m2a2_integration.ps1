# M2-A.2 Windows integration in one pass (see Docs/Development/M2A2-Next-Windows-Integration.md).
# Written on Fedora: the PowerShell syntax was parsed and the -DryRun path executed there; NOTHING of the Unreal/Windows steps has run yet.
#
# Steps: preconditions -> Python tests -> Core tests -> editor build -> V2 map backup -> V2 map regeneration -> generator verdict -> gameplay QA
#        -> V1 map unchanged? -> trace analysis. Each step prints the command it runs. It stops at the first failing step and says what to do.
# Safety: never commits or pushes, never deletes. The V1 map is never passed to the generator (the script path is V2-only: --v2 --force) and its
#         file hash is compared before and after. The V2 map is copied to Saved\M2A2-Backup\<time> before it is regenerated.
#
# Usage:  powershell -File Tools\windows\run_m2a2_integration.ps1 [-DryRun] [-SkipBuild] [-SkipMap] [-Suite Doors|Quick|Movement|Full] [-EnginePath <dir>]
param(
    [switch]$DryRun,
    [switch]$SkipBuild,
    [switch]$SkipMap,
    [ValidateSet('Doors', 'Quick', 'Movement', 'Full')]
    [string]$Suite = 'Doors',
    [string]$EnginePath = $(if ($env:UE_ROOT) { $env:UE_ROOT } else { 'C:\Program Files\Unreal Engine\UE_5.8' })
)
$ErrorActionPreference = 'Stop'

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$projectDir = Join-Path $repo 'Unreal\SupernaturalProt'
$uproject = Join-Path $projectDir 'SupernaturalProt.uproject'
$toolsPy = Join-Path $projectDir 'Tools'
$editorCmd = Join-Path $EnginePath 'Engine\Binaries\Win64\UnrealEditor-Cmd.exe'
$buildBat = Join-Path $EnginePath 'Engine\Build\BatchFiles\Build.bat'
$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$workDir = Join-Path $projectDir "Saved\M2A2-Integration\$stamp"
$v1Map = Join-Path $projectDir 'Content\SpnTest\Lvl_SpnMovementTest.umap'
$v2Map = Join-Path $projectDir 'Content\SpnTest\Lvl_SpnMovementTestV2.umap'
$python = if (Get-Command python -ErrorAction SilentlyContinue) { 'python' } else { 'python3' }
$summary = [System.Collections.Generic.List[string]]::new()

function Note([string]$text) { Write-Host $text; $summary.Add($text) }
function Step([string]$name, [string]$command, [scriptblock]$body) {
    Write-Host ""; Write-Host "=== $name"; Write-Host "    $command"
    if ($DryRun) { $summary.Add("DRYRUN  $name"); return }
    $global:LASTEXITCODE = 0
    & $body
    if ($LASTEXITCODE -ne 0) { $summary.Add("FAILED  $name (exit $LASTEXITCODE)"); throw "step failed: $name (exit $LASTEXITCODE)" }
    $summary.Add("OK      $name")
}
function Hash-Or-Null([string]$path) { if (Test-Path $path) { (Get-FileHash $path -Algorithm SHA256).Hash } else { $null } }

try {
    # --- preconditions
    Write-Host "Repo: $repo"; Write-Host "Engine: $EnginePath"; Write-Host "Suite: $Suite   DryRun: $DryRun"
    if (-not $DryRun) {
        foreach ($required in @($uproject, $editorCmd, $buildBat)) { if (-not (Test-Path $required)) { throw "missing: $required (use -EnginePath)" } }
    }
    New-Item -ItemType Directory -Force $workDir | Out-Null
    $v1Before = Hash-Or-Null $v1Map
    if ($null -eq $v1Before) { Note "WARNING V1 map file not found at ${v1Map}: its integrity cannot be compared (the generator is never pointed at it)." }
    $dirty = (git -C $repo status --porcelain) -join "`n"
    if ($dirty) { Note "NOTE working tree has uncommitted changes (expected before the owner commits the Fedora work)." }

    Step 'Python tool tests' "$python -m unittest discover -s Tools -p test_*.py   (in $projectDir)" {
        Push-Location $projectDir
        try { & $python -m unittest discover -s Tools -p 'test_*.py' 2>&1 | Out-Host } finally { Pop-Location }
    }
    Step 'Core tests (expect 280 tests, all passed)' 'cmake --preset clang; cmake --build --preset clang; ctest --preset clang' {
        Push-Location $repo
        try { cmake --preset clang 2>&1 | Out-Host; if ($LASTEXITCODE -eq 0) { cmake --build --preset clang 2>&1 | Out-Host }; if ($LASTEXITCODE -eq 0) { ctest --preset clang 2>&1 | Out-Host } } finally { Pop-Location }
    }
    if (-not $SkipBuild) {
        Step 'Editor build SupernaturalProtEditor Win64 Development' "$buildBat SupernaturalProtEditor Win64 Development -Project=$uproject -WaitMutex" {
            & $buildBat SupernaturalProtEditor Win64 Development "-Project=$uproject" -WaitMutex 2>&1 | Out-Host
        }
    }

    if (-not $SkipMap) {
        $backup = Join-Path $projectDir "Saved\M2A2-Backup\$stamp"
        Step 'Back up the V2 map (copy only)' "Copy-Item $v2Map -> $backup" {
            if (Test-Path $v2Map) { New-Item -ItemType Directory -Force $backup | Out-Null; Copy-Item $v2Map $backup; Note "V2 map backed up to $backup" }
            else { Note "No V2 map yet; nothing to back up."; $global:LASTEXITCODE = 0 }
        }
        $genLog = Join-Path $workDir 'generator.log'
        $script = Join-Path $toolsPy 'create_spn_movement_test_map.py'
        Step 'Regenerate the V2 map only (--v2 --instancing --force)' "$editorCmd $uproject -run=pythonscript -script=`"$script --v2 --instancing --force`" -unattended -nullrhi" {
            & $editorCmd $uproject -run=pythonscript "-script=`"$script --v2 --instancing --force`"" -unattended -nullrhi "-abslog=$genLog" 2>&1 | Out-Host
            $global:LASTEXITCODE = 0   # the verdict below decides, the editor's exit code is not reliable for scripts
        }
        Step 'Generator verdict (exit 0 = saved or unchanged)' "$python spn_log_tools.py generator $genLog" {
            & $python (Join-Path $toolsPy 'spn_log_tools.py') generator $genLog 2>&1 | Out-Host
            if ($LASTEXITCODE -ne 0) { Note "The map was NOT saved correctly. Restore with: Copy-Item `"$backup\Lvl_SpnMovementTestV2.umap`" `"$v2Map`" and read $genLog" }
        }
    }

    $qaArgs = @('-File', (Join-Path $repo 'Tools\run_gameplay_qa.ps1'), '-Suite', $Suite, '-NoBuild')
    $qaExit = 0
    Step "Gameplay QA ($Suite)" "powershell $($qaArgs -join ' ')" {
        & powershell @qaArgs 2>&1 | Out-Host
        $script:qaExit = $LASTEXITCODE
        $global:LASTEXITCODE = 0   # QA exit codes are results (0 ok, 1 failures, 2 incomplete), reported below instead of aborting
    }

    Step 'V1 map unchanged?' "Get-FileHash $v1Map (before == after)" {
        $after = Hash-Or-Null $v1Map
        if ($v1Before -ne $after) { Note "V1 MAP CHANGED (before $v1Before, after $after). Do not commit; restore it from Git."; $global:LASTEXITCODE = 1 } else { Note "V1 map unchanged." }
    }

    $latest = Join-Path $projectDir 'Saved\GameplayQA\latest.txt'
    if (-not $DryRun -and (Test-Path $latest)) {
        $runDir = (Get-Content $latest -Raw).Trim()
        Step 'Trace analysis (sprint-turn mirror symmetry, jump traces)' "$python spn_trace_tools.py $runDir" {
            & $python (Join-Path $toolsPy 'spn_trace_tools.py') $runDir 2>&1 | Out-Host
            $global:LASTEXITCODE = 0   # asymmetry is a finding, not a script failure
        }
        Note "QA report: $(Join-Path $runDir 'report.md')  (exit code of the QA run: $qaExit; 0 ok, 1 failures, 2 incomplete run)"
    }
} catch {
    Write-Host "STOPPED: $($_.Exception.Message)" -ForegroundColor Red
    $summary.Add("STOPPED  $($_.Exception.Message)")
    $failed = $true
} finally {
    Write-Host ""; Write-Host "=== Summary"; $summary | ForEach-Object { Write-Host $_ }
    if (Test-Path $workDir) { $summary | Set-Content (Join-Path $workDir 'summary.txt') -Encoding UTF8 }
}
if ($failed) { exit 1 }
exit $(if ($qaExit) { $qaExit } else { 0 })
