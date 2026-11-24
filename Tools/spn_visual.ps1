<#
.SYNOPSIS
  Visual evidence of the Unreal project: plays a camera run (USpnCineSubsystem, -SpnCine) in the real game and turns it into an MP4,
  into settled still images, or into a performance report.

.EXAMPLE
  .\Tools\spn_visual.ps1 -Spec Unreal\SupernaturalProt\Tools\spn_world\cine\baseline.json -Map /Game/SpnTest/Lvl_SpnMovementTestV3Interiors -Out Artifacts\VisualProgress\00_Baseline.mp4
  .\Tools\spn_visual.ps1 -Spec ...\tour.json -Map /Game/SpnWorld/Lvl_SpnWorld -Mode stills      # PNG stills into Saved\SpnCine\<name>\
  .\Tools\spn_visual.ps1 -Spec ...\tour.json -Map /Game/SpnWorld/Lvl_SpnWorld -Mode perf -Width 2560 -Height 1440

  The spec file is the JSON described in Docs/Development/Visual-Build-Tools.md; out_dir and mode are filled in by this script.
  Needs Unreal Engine 5.8 and, for videos, FFmpeg. Never commits, never stops processes it did not start. Videos stay under -MaxMB.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Spec,
    [Parameter(Mandatory = $true)][string]$Map,
    [ValidateSet('video', 'stills', 'perf')][string]$Mode = 'video',
    [string]$Out = '',
    [string]$Name = '',
    [int]$Width = 1920,
    [int]$Height = 1080,
    [int]$Fps = 30,
    [int]$MaxMB = 480,
    [int]$Crf = 18,
    [int]$TimeoutMinutes = 90,
    [string[]]$Shots = @(),          # only these shots of the spec (by name)
    [string]$Scenario = '',          # force one lighting scenario for every shot
    [string[]]$ExtraGameArgs = @(),
    [switch]$KeepFrames,
    [switch]$AllowEditor,           # run although an editor of this project is open (never for -Mode perf: it distorts the timings)
    [string]$EnginePath = $env:SPN_UE_ROOT,
    [string]$FfmpegPath = $env:SPN_FFMPEG
)

$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$projDir = Join-Path $repo 'Unreal\SupernaturalProt'
$uproject = Join-Path $projDir 'SupernaturalProt.uproject'

function Find-Engine {
    if ($EnginePath -and (Test-Path (Join-Path $EnginePath 'Engine\Binaries\Win64\UnrealEditor.exe'))) { return $EnginePath }
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
$specPath = (Resolve-Path $Spec).Path
if (-not $Name) { $Name = [IO.Path]::GetFileNameWithoutExtension($specPath) + '_' + $Mode }
$runDir = Join-Path $projDir "Saved\SpnCine\$Name"
$frameDir = Join-Path $runDir 'frames'
if (Test-Path $frameDir) { Remove-Item $frameDir -Recurse -Force -Confirm:$false }
New-Item -ItemType Directory -Force $frameDir | Out-Null
Remove-Item (Join-Path $runDir 'cine_done.json'), (Join-Path $runDir 'perf.json') -ErrorAction SilentlyContinue

# --- the spec of this run: out_dir, mode and fps filled in, optional shot filter
$specObj = Get-Content $specPath -Raw | ConvertFrom-Json
$outDir = if ($Mode -eq 'video') { $frameDir } else { $runDir }
$specObj | Add-Member -NotePropertyName out_dir -NotePropertyValue ($outDir -replace '\\', '/') -Force
$specObj | Add-Member -NotePropertyName mode -NotePropertyValue $Mode -Force
$specObj | Add-Member -NotePropertyName fps -NotePropertyValue $Fps -Force
$Shots = @($Shots | ForEach-Object { $_ -split ',' } | Where-Object { $_ })
if ($Shots.Count -gt 0) { $specObj.shots = @($specObj.shots | Where-Object { $Shots -contains $_.name }) }
if ($Scenario) { foreach ($s in $specObj.shots) { $s | Add-Member -NotePropertyName scenario -NotePropertyValue $Scenario -Force } }
if (@($specObj.shots).Count -eq 0) { throw "No shots left in $specPath." }
# Game binaries built before the capture-timing fix (EPhase::Hold in SpnCineSubsystem.cpp) move the camera in the tick that requests an image,
# so the image requested right before a camera jump is smeared. Stills: every pose is requested twice; the files with an even index
# (<shot>_0, _2, _4 ...) are the valid ones. Video: the last frame of each shot is replaced below. Harmless with fixed binaries.
if ($Mode -eq 'stills') {
    foreach ($s in $specObj.shots) {
        $list = if ($s.PSObject.Properties['stills']) { @($s.stills) } else { @(0.0, 0.5, 1.0) }
        $s | Add-Member -NotePropertyName stills -NotePropertyValue @($list | ForEach-Object { $_; $_ }) -Force
    }
}
$runSpec = Join-Path $runDir 'spec.json'
$specObj | ConvertTo-Json -Depth 12 | Set-Content $runSpec -Encoding UTF8

$busy = Get-CimInstance Win32_Process -Filter "Name = 'UnrealEditor.exe'" | Where-Object { $_.CommandLine -like '*SupernaturalProt*' }
if ($busy -and -not $AllowEditor) { throw "An Unreal editor/game of this project is running (PID $($busy.ProcessId -join ', ')). Close it first." }

$gameLog = Join-Path $runDir 'game.log'
$gameArgs = @("`"$uproject`"", $Map, '-game', "-SpnCine=`"$runSpec`"", "-abslog=`"$gameLog`"", '-unattended', '-nosplash', '-nosound', '-NoVerifyGC',
    '-windowed', "-ResX=$Width", "-ResY=$Height", '-WinX=0', '-WinY=0', '-ini:Engine:[SystemSettings]:r.VSync=0')
if ($Mode -eq 'perf') { $gameArgs += '-ini:Engine:[SystemSettings]:t.MaxFPS=0' }
$gameArgs += $ExtraGameArgs
$started = Get-Date
Write-Host "SpnCine '$Name' ($Mode, ${Width}x${Height}) on $Map ..."
$game = Start-Process -FilePath $editorExe -ArgumentList $gameArgs -PassThru
if (-not $game.WaitForExit($TimeoutMinutes * 60000)) {
    Write-Warning "Timeout after $TimeoutMinutes minutes: stopping the game process this script started (PID $($game.Id))."
    Stop-Process -Id $game.Id -Force
    $game.WaitForExit(15000) | Out-Null
}
$seconds = [int]((Get-Date) - $started).TotalSeconds
$doneFile = Join-Path $outDir ($(if ($Mode -eq 'perf') { 'perf.json' } else { 'cine_done.json' }))
if (-not (Test-Path $doneFile)) {
    Get-Content $gameLog -Tail 25 -ErrorAction SilentlyContinue
    throw "The camera run did not finish (no $doneFile, exit $($game.ExitCode), ${seconds}s). Log: $gameLog"
}
$done = Get-Content $doneFile -Raw | ConvertFrom-Json
Write-Host "Run finished in ${seconds}s: $($done.frames) frames."

if ($Mode -ne 'video') {
    if ($Mode -eq 'perf') {
        $done.shots | ForEach-Object { '{0,-26} {1,-9} {2,6:N1} fps  p95 {3,5:N1} ms  game {4,5:N1}  render {5,5:N1}  gpu {6,5:N1}  draws {7,6:N0}' -f $_.name, $_.scenario, $_.avg_fps, $_.p95_ms, $_.game_ms, $_.render_ms, $_.gpu_ms, $_.draw_calls }
    }
    Write-Host "Output: $runDir"
    exit 0
}

# --- encode
$ffmpeg = Find-Ffmpeg
if (-not $ffmpeg) { throw "FFmpeg not found (winget install Gyan.FFmpeg); the frames are in $frameDir" }
if (-not $Out) { $Out = Join-Path $runDir "$Name.mp4" }
if (-not [IO.Path]::IsPathRooted($Out)) { $Out = Join-Path $repo $Out }
New-Item -ItemType Directory -Force (Split-Path -Parent $Out) | Out-Null
foreach ($s in $done.shots) {   # see the note on capture timing above: the last frame of a shot repeats the one before it
    $last = [int]$s.first_frame + [int]$s.frames - 1
    $src = Join-Path $frameDir ('frame_{0:D6}.png' -f ($last - 1)); $dst = Join-Path $frameDir ('frame_{0:D6}.png' -f $last)
    if ([int]$s.frames -gt 1 -and (Test-Path $src) -and (Test-Path $dst)) { Copy-Item $src $dst -Force }
}
$frames = @(Get-ChildItem $frameDir -Filter 'frame_*.png').Count
if ($frames -ne [int]$done.frames) { Write-Warning "Expected $($done.frames) frames, found $frames." }
$duration = [Math]::Max(1.0, $frames / [double]$Fps)
$maxKbps = [int](($MaxMB * 8192.0 / $duration) * 0.93)       # 7 % reserve for the container and rate overshoot
$maxKbps = [Math]::Min($maxKbps, 40000)
$encode = {
    param($crf)
    & $ffmpeg -y -hide_banner -loglevel error -framerate $Fps -i (Join-Path $frameDir 'frame_%06d.png') -c:v libx264 -preset slow -crf $crf `
        -maxrate "${maxKbps}k" -bufsize "$($maxKbps * 2)k" -pix_fmt yuv420p -an -movflags +faststart $Out
    if ($LASTEXITCODE -ne 0) { throw "ffmpeg failed (exit $LASTEXITCODE)" }
}
& $encode $Crf
$sizeMB = (Get-Item $Out).Length / 1MB
if ($sizeMB -gt $MaxMB) {
    Write-Warning ("{0:N0} MB is over the limit of $MaxMB MB: encoding again with a lower quality." -f $sizeMB)
    $maxKbps = [int]($maxKbps * 0.8)
    & $encode ($Crf + 5)
    $sizeMB = (Get-Item $Out).Length / 1MB
}
# playback check: decode the whole file
$decodeErrors = & $ffmpeg -v error -i $Out -f null - 2>&1
$ffprobe = Join-Path (Split-Path -Parent $ffmpeg) 'ffprobe.exe'
$probe = if (Test-Path $ffprobe) { & $ffprobe -v error -select_streams v:0 -show_entries stream=codec_name,width,height,r_frame_rate,nb_frames:format=duration -of json $Out | ConvertFrom-Json } else { $null }
$info = [ordered]@{
    file = $Out; size_mb = [Math]::Round($sizeMB, 1); frames = $frames; fps = $Fps; duration_s = [Math]::Round($duration, 1)
    codec = if ($probe) { $probe.streams[0].codec_name } else { 'h264' }; width = if ($probe) { $probe.streams[0].width } else { $Width }
    height = if ($probe) { $probe.streams[0].height } else { $Height }; decode_ok = (-not $decodeErrors); under_limit = ($sizeMB -le $MaxMB)
    map = $Map; spec = $specPath; shots = @($done.shots | ForEach-Object { $_.name }); date = (Get-Date -Format 'yyyy-MM-dd HH:mm')
}
$info | ConvertTo-Json -Depth 5 | Set-Content ([IO.Path]::ChangeExtension($Out, '.json')) -Encoding UTF8
if (-not $KeepFrames) { Remove-Item $frameDir -Recurse -Force -Confirm:$false }
Write-Host ("Video: $Out  {0:N1} MB, {1:N1} s, {2}x{3} {4}, decode {5}" -f $sizeMB, $duration, $info.width, $info.height, $info.codec, $(if ($info.decode_ok) { 'ok' } else { 'ERRORS' }))
if (-not $info.decode_ok -or -not $info.under_limit) { exit 1 }
exit 0
