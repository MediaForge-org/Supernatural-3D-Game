# Map & navigation tech demo (NightBuild03): runs the scripted level in a real game window and records it.
# Reproducible: needs Windows, Unreal Engine 5.8.4 (UE_ROOT, default C:\Program Files\Unreal Engine\UE_5.8), ffmpeg (FFMPEG, else WinGet's), and the
# level /Game/SpnTest/Lvl_SpnNavDemo (create: Unreal\SupernaturalProt\Tools\create_spn_nav_demo_map.py).
# The game starts, waits for the start flag, ffmpeg starts, the flag is created -> the script runs on a known clock. Nothing is committed or deleted.
#
#   powershell -File Tools\windows\record_nav_demo.ps1 -Mode Record -OutDir Artifacts\MapNavigationDemo
#   powershell -File Tools\windows\record_nav_demo.ps1 -Mode Shots  -OutDir Artifacts\MapNavigationDemo\shots -ShotTimes 3,12,40
#
# Record: ddagrab (desktop duplication) -> h264_nvenc, 2560x1440 @ 60 fps, no audio (no music rights are needed). The game window is borderless on the
# primary monitor at 2560x1440.
param(
    [ValidateSet("Record", "Shots", "Run")][string]$Mode = "Record",
    [string]$OutDir = "Artifacts\MapNavigationDemo",
    [string]$ShotTimes = "3,12,40",
    [int]$Fps = 60,
    [int]$MaxSeconds = 420
)
$ErrorActionPreference = "Stop"
$repo = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$ueRoot = if ($env:UE_ROOT) { $env:UE_ROOT } else { "C:\Program Files\Unreal Engine\UE_5.8" }
$ue = Join-Path $ueRoot "Engine\Binaries\Win64\UnrealEditor.exe"
$ffmpeg = if ($env:FFMPEG) { $env:FFMPEG } else { (Get-ChildItem "$env:LOCALAPPDATA\Microsoft\WinGet\Packages" -Recurse -Filter ffmpeg.exe -ErrorAction SilentlyContinue | Select-Object -First 1).FullName }
if (-not (Test-Path $ue)) { throw "UnrealEditor.exe not found: $ue" }
if ($Mode -ne "Run" -and -not (Test-Path $ffmpeg)) { throw "ffmpeg not found (set FFMPEG)" }
$project = Join-Path $repo "Unreal\SupernaturalProt\SupernaturalProt.uproject"
$out = if ([System.IO.Path]::IsPathRooted($OutDir)) { $OutDir } else { Join-Path $repo $OutDir }
New-Item -ItemType Directory -Force -Path $out | Out-Null
$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$flag = Join-Path $out "start_$stamp.flag"
$log = Join-Path $out "game_$stamp.log"
$report = Join-Path $out "report_$stamp.txt"

$gameArgs = @($project, "/Game/SpnTest/Lvl_SpnNavDemo", "-game", "-windowed", "-ResX=2560", "-ResY=1440", "-WinX=0", "-WinY=0", "-nosplash", "-log",
    "-SpnNavDemoRun", "-SpnNavDemoFlag=$flag", "-SpnNavDemoReport=$report", "-SpnNavDemoQuit", "-abslog=$log", "-ExecCmds=r.setres 2560x1440wf")
Write-Host "starting the game: $ue"
$game = Start-Process $ue -ArgumentList $gameArgs -PassThru
$deadline = (Get-Date).AddSeconds(240)
while (-not ((Test-Path $log) -and (Select-String -Path $log -Pattern "waiting for the start flag" -Quiet -ErrorAction SilentlyContinue))) {
    if ($game.HasExited) { throw "the game exited before it was ready (see $log)" }
    if ((Get-Date) -gt $deadline) { $game.Kill(); throw "timeout waiting for the game (see $log)" }
    Start-Sleep -Milliseconds 500
}
Start-Sleep -Seconds 6   # world ready, shaders warm, HUD built

$video = Join-Path $out "MapNavigationDemo_raw_$stamp.mp4"
$rec = $null
if ($Mode -eq "Record") {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $ffmpeg
    $psi.Arguments = "-y -hide_banner -loglevel warning -f lavfi -i ddagrab=output_idx=0:framerate=${Fps}:draw_mouse=0 -vf hwdownload,format=bgra,format=yuv420p -c:v h264_nvenc -preset p5 -tune hq -rc vbr -cq 17 -b:v 60M -maxrate 90M -g 120 -pix_fmt yuv420p `"$video`""
    $psi.RedirectStandardInput = $true
    $psi.UseShellExecute = $false
    $rec = [System.Diagnostics.Process]::Start($psi)
    Start-Sleep -Seconds 2
}
$t0 = Get-Date
New-Item -ItemType File -Force -Path $flag | Out-Null   # start flag -> the script starts now
Write-Host "demo started at $($t0.ToString('HH:mm:ss.fff'))"

if ($Mode -eq "Shots") {
    foreach ($t in ($ShotTimes -split "[, ]+" | ForEach-Object { [double]$_ })) {
        $wait = $t - ((Get-Date) - $t0).TotalSeconds
        if ($wait -gt 0) { Start-Sleep -Milliseconds ([int]($wait * 1000)) }
        $png = Join-Path $out ("shot_{0:000.0}.png" -f $t)
        & $ffmpeg -y -hide_banner -loglevel error -f gdigrab -framerate 1 -i desktop -frames:v 1 $png
        Write-Host "shot $png"
    }
}
while (-not $game.HasExited) {
    if (((Get-Date) - $t0).TotalSeconds -gt $MaxSeconds) { Write-Warning "time limit reached; closing the game"; $game.Kill(); break }
    Start-Sleep -Milliseconds 500
}
$t1 = Get-Date
if ($rec -ne $null) {
    Start-Sleep -Seconds 1
    $rec.StandardInput.WriteLine("q")
    $rec.WaitForExit(30000) | Out-Null
}
Write-Host ("demo ran {0:N1} s; log: {1}" -f ($t1 - $t0).TotalSeconds, $log)
if (Test-Path $report) { Get-Content $report }
if ($Mode -eq "Record") { Write-Host "raw video: $video" }
