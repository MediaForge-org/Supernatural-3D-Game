# M2-A.3 Windows integration (Docs/Development/M2A3-Next-Windows-Integration.md). Written on Fedora: the PowerShell syntax was parsed and the -DryRun
# path executed there; NOTHING of the Unreal/Windows steps has run yet.
#
# Stages (each prints its command, is classified and stops the run at the first failure, with what to do next):
#   prerequisites -> python-tests -> core-tests -> editor-build -> map-v2env (back up, regenerate ONLY Lvl_SpnMovementTestV2Env, verdict)
#   -> qa (run_gameplay_qa.ps1) -> integrity (V1 and V2 map files unchanged?) -> traces
# Failure classes in summary.json: PREREQ (tool or path missing), TEST (python/core tests), BUILD (editor build), GENERATE (map generator), RUNTIME (QA run:
# exit 1 failures, exit 2 incomplete), INTEGRITY (a map that must not change did), OK. BUILD and RUNTIME are never mixed up: a stage after a failed build does not run.
# Safety: never commits, pushes or deletes. The plain V2 level and the V1 level are never passed to the generator by this script (the script path is
# `--v2 --enrich --instancing --force`, which writes only the enriched level) and both file hashes are compared before and after.
#
# Usage:  powershell -File Tools\windows\run_m2a3_integration.ps1 [-DryRun] [-SkipBuild] [-SkipMap] [-SkipQa] [-Suite Doors|Quick|Movement|Full] [-EnginePath <dir>] [-Yes]
param(
    [switch]$DryRun,
    [switch]$SkipBuild,
    [switch]$SkipMap,
    [switch]$SkipQa,
    [switch]$Yes,   # confirms regenerating an EXISTING enriched level without asking
    [ValidateSet('Doors', 'Quick', 'Movement', 'Full', 'Access', 'Traversal')]
    [string]$Suite = 'Doors',
    # Which generated level this run rebuilds. V2Env = the original M2A3 target. V3 = /Game/SpnTest/Lvl_SpnMovementTestV3Interiors (the QA then runs on V3).
    # Every other level file must be byte-identical after the run (integrity stage).
    [ValidateSet('V2Env', 'V3')]
    [string]$Level = 'V2Env',
    # Test hook: a different generator script (e.g. one that fails) to prove that a failed commandlet is reported as FAILED, never as OK.
    [string]$ScriptPath = '',
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
$workDir = Join-Path $projectDir "Saved\M2A3-Integration\$stamp"
$mapDir = Join-Path $projectDir 'Content\SpnTest'
$maps = [ordered]@{ V1 = Join-Path $mapDir 'Lvl_SpnMovementTest.umap'; V2 = Join-Path $mapDir 'Lvl_SpnMovementTestV2.umap'; V2Env = Join-Path $mapDir 'Lvl_SpnMovementTestV2Env.umap'; V3 = Join-Path $mapDir 'Lvl_SpnMovementTestV3Interiors.umap' }
$targetMap = $maps[$Level]
$generatorArgs = if ($Level -eq 'V3') { '--v3 --instancing --force' } else { '--v2 --enrich --instancing --force' }
$qaMap = if ($Level -eq 'V3') { '/Game/SpnTest/Lvl_SpnMovementTestV3Interiors' } else { '/Game/SpnTest/Lvl_SpnMovementTestV2' }

# clang++, cmake and ninja ship with Visual Studio but are usually not on the PATH of a plain shell. A configure without clang++ aborts half-way and
# leaves build/clang without CMakeFiles/rules.ninja ("ninja: error: build.ninja:35: loading 'CMakeFiles/rules.ninja'"). Add them for this run when missing.
foreach ($vs in @('C:\Program Files\Microsoft Visual Studio\18\Community', 'C:\Program Files\Microsoft Visual Studio\2022\Community', 'C:\Program Files\Microsoft Visual Studio\2022\Professional', 'C:\Program Files\Microsoft Visual Studio\2022\Enterprise')) {
    foreach ($sub in @('VC\Tools\Llvm\x64\bin', 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin', 'Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja')) {
        $dir = Join-Path $vs $sub
        if ((Test-Path $dir) -and ($env:Path -notlike "*$dir*")) { $env:Path = "$dir;$env:Path" }
    }
}
# A "python" that only prints "Python was not found" (the Microsoft Store shortcut) must not be used; fall back to the Python that ships with the engine.
$python = $null
foreach ($candidate in @('python', 'python3', (Join-Path $EnginePath 'Engine\Binaries\ThirdParty\Python3\Win64\python.exe'))) {
    if (-not (Get-Command $candidate -ErrorAction SilentlyContinue)) { continue }
    try { $probe = & $candidate -c "print(1)" 2>$null } catch { continue }
    if ($LASTEXITCODE -eq 0 -and "$probe".Trim() -eq '1') { $python = $candidate; break }
}
if (-not $python) { $python = 'python' }
$stages = [System.Collections.Generic.List[object]]::new()
$qaExit = $null

function Hash-Or-Null([string]$path) { if (Test-Path $path) { (Get-FileHash $path -Algorithm SHA256).Hash } else { $null } }
function Add-Stage([string]$name, [string]$class, [string]$status, [string]$detail = '') {
    $stages.Add([ordered]@{ stage = $name; class = $class; status = $status; detail = $detail; time = (Get-Date -Format 'o') })
    Write-Host ("[{0}] {1} {2}" -f $status, $name, $detail)
}
function Stage([string]$name, [string]$class, [string]$command, [scriptblock]$body) {
    Write-Host ""; Write-Host "=== $name ($class)"; Write-Host "    $command"
    if ($DryRun) { Add-Stage $name $class 'DRYRUN'; return }
    $global:LASTEXITCODE = 0
    $previousPreference = $ErrorActionPreference; $ErrorActionPreference = 'Continue'   # native tools write progress to stderr; the exit code decides
    try { & $body } finally { $ErrorActionPreference = $previousPreference }
    if ($LASTEXITCODE -ne 0) { Add-Stage $name $class 'FAILED' "exit $LASTEXITCODE"; throw "stage failed: $name (class $class, exit $LASTEXITCODE)" }
    Add-Stage $name $class 'OK'
}

try {
    Write-Host "Repo: $repo"; Write-Host "Engine: $EnginePath"; Write-Host "Suite: $Suite   DryRun: $DryRun"
    if (-not $DryRun) {
        foreach ($required in @($uproject, $editorCmd, $buildBat)) { if (-not (Test-Path $required)) { Add-Stage 'prerequisites' 'PREREQ' 'FAILED' "missing: $required"; throw "missing: $required (use -EnginePath)" } }
        foreach ($tool in @($python, 'cmake', 'git')) { if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) { Add-Stage 'prerequisites' 'PREREQ' 'FAILED' "not on PATH: $tool"; throw "not on PATH: $tool" } }
        if (Get-Process -Name 'UnrealEditor' -ErrorAction SilentlyContinue) { Add-Stage 'prerequisites' 'PREREQ' 'FAILED' 'an Unreal editor is running (Live Coding blocks the build)'; throw 'close the running Unreal editor first' }
        Add-Stage 'prerequisites' 'PREREQ' 'OK'
    }
    New-Item -ItemType Directory -Force $workDir | Out-Null
    $before = [ordered]@{}
    foreach ($k in $maps.Keys) { $before[$k] = Hash-Or-Null $maps[$k] }
    if ($null -eq $before.V1) { Write-Host "NOTE: V1 map file not found; its integrity cannot be compared." }
    if ((git -C $repo status --porcelain) -join "`n") { Write-Host "NOTE: the working tree has uncommitted changes (expected before the owner commits the Fedora work)." }

    Stage 'python-tests' 'TEST' "$python -m unittest discover -s Tools -p test_*.py   (expect 219 tests OK)" {
        Push-Location $projectDir
        try { & $python -m unittest discover -s Tools -p 'test_*.py' 2>&1 | Tee-Object (Join-Path $workDir 'python-tests.log') | Out-Host } finally { Pop-Location }
    }
    Stage 'core-tests' 'TEST' 'cmake --preset clang; cmake --build --preset clang; ctest --preset clang   (expect 354 tests, all passed)' {
        Push-Location $repo
        try {
            cmake --preset clang 2>&1 | Out-Host
            if ($LASTEXITCODE -eq 0) { cmake --build --preset clang 2>&1 | Out-Host }
            if ($LASTEXITCODE -eq 0) { ctest --preset clang 2>&1 | Tee-Object (Join-Path $workDir 'core-tests.log') | Out-Host }
        } finally { Pop-Location }
    }
    if (-not $SkipBuild) {
        Stage 'editor-build' 'BUILD' "$buildBat SupernaturalProtEditor Win64 Development -Project=$uproject -WaitMutex" {
            & $buildBat SupernaturalProtEditor Win64 Development "-Project=$uproject" -WaitMutex 2>&1 | Tee-Object (Join-Path $workDir 'build.log') | Out-Host
        }
    }

    if (-not $SkipMap) {
        $backup = Join-Path $projectDir "Saved\M2A3-Backup\$stamp"
        if ((Test-Path $targetMap) -and -not $Yes -and -not $DryRun) {
            $answer = Read-Host "The level $targetMap exists and will be regenerated (only its own SPN_GEN actors are replaced; a copy goes to $backup). Continue? [y/N]"
            if ($answer -notin @('y', 'Y', 'yes')) { Add-Stage 'map-v2env' 'GENERATE' 'SKIPPED' 'declined by the user'; throw 'declined: enriched level not regenerated' }
        }
        Stage 'map-v2env-backup' 'GENERATE' "Copy-Item (existing $Level) -> $backup   (copy only)" {
            if (Test-Path $targetMap) { New-Item -ItemType Directory -Force $backup | Out-Null; Copy-Item $targetMap $backup; Write-Host "Backup: $backup (SHA256 $(Hash-Or-Null (Join-Path $backup (Split-Path $targetMap -Leaf))))" }
            $global:LASTEXITCODE = 0
        }
        $genLog = Join-Path $workDir 'generator.log'
        $script = if ($ScriptPath) { $ScriptPath } else { Join-Path $toolsPy 'create_spn_movement_test_map.py' }
        $genStart = [DateTimeOffset]::UtcNow.ToUnixTimeSeconds()
        $genExit = $null
        # ONE argument "-script=<path> <script args>": no embedded quotes. Wrapping the value in literal quotes made the commandlet see
        # no script at all ("LogPythonScriptCommandlet: Error: -Script argument not specified", result -1) while this runner reported success.
        $scriptArg = "-script=$script $generatorArgs"
        Stage 'map-v2env-generate' 'GENERATE' "$editorCmd $uproject -run=pythonscript `"$scriptArg`" -unattended -nullrhi" {
            & $editorCmd $uproject '-run=pythonscript' $scriptArg '-unattended' '-nullrhi' "-abslog=$genLog" 2>&1 | Out-Host
            $script:genExit = $LASTEXITCODE
            $global:LASTEXITCODE = 0   # the verdict stage below turns the exit code, the commandlet log and the level file into the result
        }
        Stage 'map-v2env-verdict' 'GENERATE' "$python spn_log_tools.py generator $genLog --exit-code <editor exit> --map <V2Env.umap> --started <run start>   (exit 0 = saved or kept)" {
            if (-not (Test-Path $genLog)) { Write-Host "No generator log was written: $genLog"; $global:LASTEXITCODE = 1; return }
            & $python (Join-Path $toolsPy 'spn_log_tools.py') generator $genLog --exit-code "$($script:genExit)" --map $targetMap --started "$genStart" 2>&1 | Out-Host
            if ($LASTEXITCODE -ne 0) { Write-Host "The enriched level was NOT generated correctly (editor exit $($script:genExit)). Read $genLog. Backup (if any): $backup" }
        }
    }

    if (-not $SkipQa) {
        # The QA runs on the plain V2 level (its tests are written against it); the enriched level is for the visual and gate checks of the checklist.
        Stage "qa-$Suite" 'RUNTIME' "powershell -File Tools\run_gameplay_qa.ps1 -Suite $Suite -NoBuild   (exit 0 ok, 1 failures, 2 incomplete)" {
            & powershell -File (Join-Path $repo 'Tools\run_gameplay_qa.ps1') -Suite $Suite -NoBuild -Map $qaMap 2>&1 | Out-Host
            $script:qaExit = $LASTEXITCODE
            $global:LASTEXITCODE = 0
            if ($script:qaExit -ne 0) { Add-Stage "qa-$Suite-result" 'RUNTIME' 'FINDING' "QA exit code $($script:qaExit)" }
        }
    }

    Stage 'integrity' 'INTEGRITY' 'Get-FileHash of every level file except the rebuilt one (before == after)' {
        $changed = @()
        foreach ($k in @($maps.Keys | Where-Object { $maps[$_] -ne $targetMap })) { if ((Hash-Or-Null $maps[$k]) -ne $before[$k]) { $changed += $k } }
        if ($changed) { Write-Host "MAP FILES CHANGED: $($changed -join ', '). Do not commit them; restore from Git. (This script never writes them: find out who did.)"; $global:LASTEXITCODE = 1 }
        else { Write-Host "All other level files unchanged." }
    }

    $latest = Join-Path $projectDir 'Saved\GameplayQA\latest.txt'
    if (-not $DryRun -and -not $SkipQa -and (Test-Path $latest)) {
        $runDir = (Get-Content $latest -Raw).Trim()
        Stage 'traces' 'RUNTIME' "$python spn_trace_tools.py $runDir" {
            & $python (Join-Path $toolsPy 'spn_trace_tools.py') $runDir 2>&1 | Out-Host
            $global:LASTEXITCODE = 0   # an asymmetry is a finding, not a script failure
        }
        Write-Host "QA report: $(Join-Path $runDir 'report.md')"
    }
} catch {
    Write-Host "STOPPED: $($_.Exception.Message)" -ForegroundColor Red
    $stages.Add([ordered]@{ stage = 'stopped'; class = 'STOP'; status = 'FAILED'; detail = $_.Exception.Message; time = (Get-Date -Format 'o') })
    $failed = $true
} finally {
    Write-Host ""; Write-Host "=== Summary"
    $stages | ForEach-Object { Write-Host ("{0,-9} {1,-10} {2}  {3}" -f $_.status, $_.class, $_.stage, $_.detail) }
    if (Test-Path $workDir) {
        $result = [ordered]@{ run = $stamp; dry_run = [bool]$DryRun; engine = $EnginePath; suite = $Suite; qa_exit_code = $qaExit; failed = [bool]$failed; stages = $stages }
        $result | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $workDir 'summary.json') -Encoding UTF8
        Write-Host "Machine-readable summary: $(Join-Path $workDir 'summary.json')"
    }
}
if ($failed) { exit 1 }
exit $(if ($qaExit) { $qaExit } else { 0 })
