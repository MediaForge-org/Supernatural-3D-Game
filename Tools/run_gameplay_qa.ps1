<#
.SYNOPSIS
  Automated gameplay QA of the Unreal project: builds, plays the SpnQA.* automation tests in a real game window, records the window as
  MP4 (FFmpeg) and writes results.json / report.md under Unreal/SupernaturalProt/Saved/GameplayQA/<run>/.

.EXAMPLE
  .\Tools\run_gameplay_qa.ps1                      # full run with video
  .\Tools\run_gameplay_qa.ps1 -Suite Quick         # fast regression, small window, no video
  .\Tools\run_gameplay_qa.ps1 -Suite Movement      # gaits, turns, jumps
  .\Tools\run_gameplay_qa.ps1 -Suite Doors         # doors, interaction reach, buildings and stairs
  .\Tools\run_gameplay_qa.ps1 -Suite Performance
  .\Tools\run_gameplay_qa.ps1 -Suite Failed        # repeats the test groups that failed in the latest run
  .\Tools\run_gameplay_qa.ps1 -Suite Full -Regression   # additionally the older SpnGame.* regression tests (no video of those)

  Needs: Unreal Engine 5.8 (registry or -EnginePath), FFmpeg for the video (winget install Gyan.FFmpeg). Never commits, never kills
  processes it did not start. An Unreal editor with Live Coding blocks the build: close it first (the script says so).
#>
[CmdletBinding()]
param(
    [ValidateSet('Full', 'Quick', 'Movement', 'Doors', 'Camera', 'Performance', 'Failed')]
    [string]$Suite = 'Full',
    [switch]$NoBuild,
    [switch]$NoVideo,
    [switch]$Regression,
    [switch]$NoClips,
    [int]$Fps = 60,
    [int]$Width = 1920,
    [int]$Height = 1080,
    [int]$TimeoutMinutes = 25,
    [string]$EnginePath = $env:SPN_UE_ROOT,
    [string]$FfmpegPath = $env:SPN_FFMPEG
)

$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$projDir = Join-Path $repo 'Unreal\SupernaturalProt'
$uproject = Join-Path $projDir 'SupernaturalProt.uproject'
$qaRoot = Join-Path $projDir 'Saved\GameplayQA'
$mapPath = '/Game/SpnTest/Lvl_SpnMovementTestV2'
$python = $null

function Find-Engine {
    if ($EnginePath -and (Test-Path (Join-Path $EnginePath 'Engine\Binaries\Win64\UnrealEditor.exe'))) { return $EnginePath }
    $assoc = (Get-Content $uproject -Raw | ConvertFrom-Json).EngineAssociation
    $builds = 'HKCU:\SOFTWARE\Epic Games\Unreal Engine\Builds'
    if ($assoc -and (Test-Path $builds)) {
        $dir = (Get-ItemProperty $builds).$assoc
        if ($dir -and (Test-Path (Join-Path $dir 'Engine\Binaries\Win64\UnrealEditor.exe'))) { return $dir }
    }
    $installed = "HKLM:\SOFTWARE\EpicGames\Unreal Engine\$assoc"
    if ($assoc -and (Test-Path $installed)) {
        $dir = (Get-ItemProperty $installed).InstalledDirectory
        if ($dir -and (Test-Path (Join-Path $dir 'Engine\Binaries\Win64\UnrealEditor.exe'))) { return $dir }
    }
    $candidates = Get-ChildItem 'C:\Program Files\Unreal Engine' -Directory -Filter 'UE_*' -ErrorAction SilentlyContinue | Sort-Object Name -Descending
    foreach ($c in $candidates) { if (Test-Path (Join-Path $c.FullName 'Engine\Binaries\Win64\UnrealEditor.exe')) { return $c.FullName } }
    throw "Unreal Engine not found. Pass -EnginePath or set SPN_UE_ROOT."
}

function Find-Ffmpeg {
    if ($FfmpegPath -and (Test-Path $FfmpegPath)) { return $FfmpegPath }
    $cmd = Get-Command ffmpeg -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $found = Get-ChildItem "$env:LOCALAPPDATA\Microsoft\WinGet\Packages" -Recurse -Filter ffmpeg.exe -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($found) { return $found.FullName }
    return $null
}

$engine = Find-Engine
$editorExe = Join-Path $engine 'Engine\Binaries\Win64\UnrealEditor.exe'
$python = Join-Path $engine 'Engine\Binaries\ThirdParty\Python3\Win64\python.exe'
$toolsDir = Join-Path $projDir 'Tools'

# --- guard: do not touch a running editor of this project
$busy = Get-CimInstance Win32_Process -Filter "Name = 'UnrealEditor.exe'" | Where-Object { $_.CommandLine -like '*SupernaturalProt*' }
if ($busy) { throw "An Unreal editor/game of this project is running (PID $($busy.ProcessId -join ', ')). Close it first; this script never stops processes it did not start." }

# --- the coverage inventory must be consistent (new gameplay systems need an entry)
& $python -I (Join-Path $toolsDir 'check_qa_inventory.py') --check
if ($LASTEXITCODE -ne 0) { throw "QA inventory check failed (see above). Add the new system to Tools/qa_inventory.json with at least one test or an explicit gap." }

# --- build
if (-not $NoBuild) {
    Write-Host "Building SupernaturalProtEditor Win64 Development ..."
    $buildLog = Join-Path $qaRoot 'last_build.log'
    New-Item -ItemType Directory -Force $qaRoot | Out-Null
    & (Join-Path $engine 'Engine\Build\BatchFiles\Build.bat') SupernaturalProtEditor Win64 Development "-Project=$uproject" -WaitMutex -NoHotReload *> $buildLog
    if ($LASTEXITCODE -ne 0) { Get-Content $buildLog -Tail 15; throw "Build failed (exit $LASTEXITCODE), log: $buildLog" }
}

# --- which tests
$groups = @{
    Full        = 'SpnQA'
    Quick       = 'SpnQA.Spawn+SpnQA.Doors+SpnQA.DoorReach+SpnQA.SaveLoad'
    Movement    = 'SpnQA.Movement+SpnQA.Turns+SpnQA.Jump'
    Doors       = 'SpnQA.Doors+SpnQA.DoorReach+SpnQA.Buildings'
    Camera      = 'SpnQA.Camera'
    Performance = 'SpnQA.Performance'
}
$expected = @{
    Full        = @('Spawn', 'Doors', 'DoorReach', 'Buildings', 'Movement', 'Turns', 'Jump', 'Camera', 'SaveLoad', 'Performance')
    Quick       = @('Spawn', 'Doors', 'DoorReach', 'SaveLoad')
    Movement    = @('Movement', 'Turns', 'Jump')
    Doors       = @('Doors', 'DoorReach', 'Buildings')
    Camera      = @('Camera')
    Performance = @('Performance')
}
if ($Suite -eq 'Failed') {
    $last = Get-ChildItem $qaRoot -Directory | Where-Object { Test-Path (Join-Path $_.FullName 'results.json') } | Sort-Object Name -Descending | Select-Object -First 1
    if (-not $last) { throw "No earlier run with results.json under $qaRoot." }
    $filter = (& $python -I (Join-Path $toolsDir 'spn_qa_report.py') $last.FullName --failing-suites).Trim()
    if (-not $filter) { Write-Host "The latest run ($($last.Name)) had no failing test groups."; exit 0 }
    $expectedSuites = @($filter.Split('+') | ForEach-Object { $_ -replace '^SpnQA\.', '' })
    Write-Host "Repeating from $($last.Name): $filter"
} else {
    $filter = $groups[$Suite]
    $expectedSuites = $expected[$Suite]
}
if ($Regression -and $Suite -eq 'Full') { $filter = "$filter+SpnGame+SpnCore" }
$headless = $Suite -eq 'Quick'
$video = (-not $NoVideo) -and (-not $headless)

# --- run directory
$runName = Get-Date -Format 'yyyyMMdd-HHmmss'
$runDir = Join-Path $qaRoot $runName
New-Item -ItemType Directory -Force (Join-Path $runDir 'shots'), (Join-Path $runDir 'automation') | Out-Null

function Git-Info {
    Push-Location $repo
    try {
        $commit = (git rev-parse --short HEAD).Trim()
        $dirty = [bool](git status --porcelain)
        return @{ commit = $commit; dirty = $dirty }
    } finally { Pop-Location }
}
$git = Git-Info
$engineVersion = try { $v = (Get-Content (Join-Path $engine 'Engine\Build\Build.version') -Raw | ConvertFrom-Json); "$($v.MajorVersion).$($v.MinorVersion).$($v.PatchVersion) (CL $($v.Changelist))" } catch { 'unknown' }

$runInfo = [ordered]@{
    run_name = $runName; suite = $Suite; filter = $filter; map = $mapPath; engine_version = $engineVersion
    git_commit = $git.commit; git_dirty = $git.dirty; date = (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
    expected_suites = $expectedSuites; video = $null; video_note = ''; video_start_epoch = $null; exit_code = $null; timed_out = $false; clips = @()
}

# --- win32 helpers for the client rectangle of the game window
Add-Type -TypeDefinition @'
using System; using System.Runtime.InteropServices;
public static class SpnWin {
  [StructLayout(LayoutKind.Sequential)] public struct RECT { public int Left, Top, Right, Bottom; }
  [StructLayout(LayoutKind.Sequential)] public struct POINT { public int X, Y; }
  [DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] public static extern bool SetWindowPos(IntPtr h, IntPtr after, int x, int y, int cx, int cy, uint flags);
}
'@

# --- start the game
$gameLog = Join-Path $runDir 'game.log'
$execCmds = "Automation RunTests $filter"
$gameArgs = @("`"$uproject`"", $mapPath, '-game', "-ExecCmds=`"$execCmds`"", '-TestExit="Automation Test Queue Empty"', "-SpnQaOut=`"$runDir`"",
    "-ReportExportPath=`"$(Join-Path $runDir 'automation')`"", "-abslog=`"$gameLog`"", '-unattended', '-nosplash', '-NoVerifyGC',
    "-ini:Engine:[SystemSettings]:t.MaxFPS=$Fps", '-ini:Engine:[SystemSettings]:r.VSync=0')
# Quick: a small window without video. -nullrhi was tried and drops key events unreliably (doors/walking flaked), so every suite renders.
if ($headless) { $gameArgs += @('-windowed', '-ResX=960', '-ResY=540', '-WinX=0', '-WinY=0', '-nosound') } else { $gameArgs += @('-windowed', "-ResX=$Width", "-ResY=$Height", '-WinX=0', '-WinY=0') }

Write-Host "Run $runName ($Suite): $filter"
$game = Start-Process -FilePath $editorExe -ArgumentList $gameArgs -PassThru
$deadline = (Get-Date).AddMinutes($TimeoutMinutes)

# --- start the video once the window exists
$ff = $null; $ffProc = $null; $videoFile = Join-Path $runDir 'gameplay.mp4'
if ($video) {
    $ffmpeg = Find-Ffmpeg
    if (-not $ffmpeg) { $runInfo.video_note = 'FFmpeg not found (winget install Gyan.FFmpeg)'; Write-Warning $runInfo.video_note }
    else {
        $hwnd = [IntPtr]::Zero
        while ((Get-Date) -lt $deadline -and -not $game.HasExited) {
            $game.Refresh()
            if ($game.MainWindowHandle -ne [IntPtr]::Zero) { $hwnd = $game.MainWindowHandle; break }
            Start-Sleep -Milliseconds 300
        }
        if ($hwnd -eq [IntPtr]::Zero) { $runInfo.video_note = 'game window never appeared' }
        else {
            Start-Sleep -Seconds 2
            [void][SpnWin]::SetWindowPos($hwnd, [IntPtr](-1), 0, 0, 0, 0, 0x0001 -bor 0x0002)  # topmost, no move, no resize
            [void][SpnWin]::SetForegroundWindow($hwnd)
            $rect = New-Object SpnWin+RECT; [void][SpnWin]::GetClientRect($hwnd, [ref]$rect)
            $pt = New-Object SpnWin+POINT; [void][SpnWin]::ClientToScreen($hwnd, [ref]$pt)
            $w = [int]([Math]::Floor(($rect.Right - $rect.Left) / 2) * 2); $h = [int]([Math]::Floor(($rect.Bottom - $rect.Top) / 2) * 2)
            Write-Host "Recording client area ${w}x${h} at ($($pt.X), $($pt.Y)) -> $videoFile"
            $encoders = @(
                @('-c:v', 'h264_nvenc', '-preset', 'p4', '-rc', 'vbr', '-cq', '27', '-b:v', '0', '-maxrate', '8M', '-bufsize', '16M', '-pix_fmt', 'yuv420p'),
                @('-c:v', 'libx264', '-preset', 'veryfast', '-crf', '23', '-pix_fmt', 'yuv420p'))
            foreach ($enc in $encoders) {
                $ffArgs = @('-y', '-hide_banner', '-loglevel', 'warning', '-f', 'gdigrab', '-framerate', "$Fps", '-offset_x', "$($pt.X)", '-offset_y', "$($pt.Y)",
                    '-video_size', "${w}x${h}", '-draw_mouse', '0', '-i', 'desktop') + $enc + @('-movflags', '+faststart', $videoFile)
                $psi = New-Object System.Diagnostics.ProcessStartInfo
                $psi.FileName = $ffmpeg; $psi.Arguments = ($ffArgs | ForEach-Object { if ($_ -match '\s') { "`"$_`"" } else { $_ } }) -join ' '
                $psi.RedirectStandardInput = $true; $psi.RedirectStandardError = $true; $psi.UseShellExecute = $false; $psi.CreateNoWindow = $true
                $ffProc = [System.Diagnostics.Process]::Start($psi)
                Start-Sleep -Seconds 2
                if (-not $ffProc.HasExited) {
                    $runInfo.video_start_epoch = [DateTimeOffset]::new($ffProc.StartTime.ToUniversalTime()).ToUnixTimeMilliseconds() / 1000.0 + 0.5
                    $runInfo.video = $videoFile
                    $runInfo.video_note = "$($enc[1]) ${w}x${h} @ $Fps fps"
                    break
                }
                $runInfo.video_note = "encoder $($enc[1]) failed: " + $ffProc.StandardError.ReadToEnd()
                $ffProc = $null
            }
        }
    }
}

# --- wait for the game, with a time limit; only our own process is stopped
if (-not $game.WaitForExit([Math]::Max(1000, [int]($deadline - (Get-Date)).TotalMilliseconds))) {
    $runInfo.timed_out = $true
    Write-Warning "Timeout after $TimeoutMinutes minutes: stopping the game process we started (PID $($game.Id))."
    Stop-Process -Id $game.Id -Force
    $game.WaitForExit(15000) | Out-Null
}
$runInfo.exit_code = $game.ExitCode

if ($ffProc -and -not $ffProc.HasExited) {
    Start-Sleep -Seconds 1
    try { $ffProc.StandardInput.WriteLine('q'); $ffProc.StandardInput.Flush() } catch { }
    if (-not $ffProc.WaitForExit(30000)) { Stop-Process -Id $ffProc.Id -Force }
}
if ($runInfo.video -and -not (Test-Path $videoFile)) { $runInfo.video = $null; $runInfo.video_note = 'ffmpeg produced no file' }

# screenshots Unreal wrote into Saved/Screenshots are already in the run folder (QaDir/shots); the automation report is in .\automation
$runInfo | ConvertTo-Json -Depth 5 | Set-Content (Join-Path $runDir 'run_info.json') -Encoding UTF8
& $python -I (Join-Path $toolsDir 'spn_qa_report.py') $runDir
$reportExit = $LASTEXITCODE

# --- clips of failing checks (10 s around the failure), from the results
if ($runInfo.video -and -not $NoClips -and $ffmpeg) {
    $results = Get-Content (Join-Path $runDir 'results.json') -Raw | ConvertFrom-Json
    $clipDir = Join-Path $runDir 'clips'; $n = 0
    foreach ($c in ($results.checks | Where-Object { $_.status -in 'FAIL', 'BLOCKED' -and $null -ne $_.video_offset_s })) {
        if ($n -ge 12) { break }
        New-Item -ItemType Directory -Force $clipDir | Out-Null
        $name = ("{0}_{1}.mp4" -f $c.suite, $c.id) -replace '[^A-Za-z0-9_.-]', '_'
        $start = [Math]::Max(0, [double]$c.video_offset_s - 6)
        & $ffmpeg -y -hide_banner -loglevel error -ss $start -t 10 -i $videoFile -c:v libx264 -preset veryfast -crf 24 -pix_fmt yuv420p (Join-Path $clipDir $name)
        if (Test-Path (Join-Path $clipDir $name)) { $runInfo.clips += "clips/$name"; $n++ }
    }
    if ($n -gt 0) {
        $runInfo | ConvertTo-Json -Depth 5 | Set-Content (Join-Path $runDir 'run_info.json') -Encoding UTF8
        & $python -I (Join-Path $toolsDir 'spn_qa_report.py') $runDir | Out-Null
    }
}

Write-Host ""
Write-Host "Report : $(Join-Path $runDir 'report.md')"
Write-Host "Video  : $(if ($runInfo.video) { $runInfo.video } else { 'none - ' + $runInfo.video_note })"
Set-Content (Join-Path $qaRoot 'latest.txt') $runDir -Encoding UTF8
exit $reportExit
