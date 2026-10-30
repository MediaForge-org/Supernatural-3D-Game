# Night Build 01 completion: the 20-step journey WITHOUT teleportation (Windows PowerShell 5.1).
#
# Everything the player does is operating-system input into the running game window (SendInput-style keyboard and RELATIVE MOUSE movement): W to walk,
# the mouse to look, E to interact, I / M / Esc / arrows / Enter in the menus. The script never uses the QA teleport hook F5 and does not set any state.
# It only READS: the game (with -SpnQaKeys) writes the player pose to pose.txt about 30 times a second and F9 dumps the game state into the log. Those
# readings close the loop like a person's eyes would: turn until the object is in the middle of the screen, walk until close enough, press E, check what
# happened. This is scripted input, not manual play.
param([string]$OutDir = ('C:\Projekte\Supernatural-3D-Game\Unreal\SupernaturalProt\Saved\NightBuild01\journey-' + (Get-Date -Format 'yyyyMMdd-HHmmss')), [switch]$NoVideo, [string]$StopAfter = '')
. "$PSScriptRoot\nb01_drive.ps1"
Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class Nb01Mouse {
  [DllImport("user32.dll")] public static extern void mouse_event(uint f, int dx, int dy, int data, UIntPtr extra);
  public static void Move(int dx, int dy) { mouse_event(0x0001, dx, dy, 0, UIntPtr.Zero); }
}
'@
$script:PoseFile = $null
$script:KYaw = 0.0; $script:KPitch = 0.0
$script:Walked = 0.0; $script:MaxJump = 0.0

function Get-Pose {
  for ($i = 0; $i -lt 10; $i++) {
    try {
      $t = (Get-Content -Raw -ErrorAction Stop $script:PoseFile).Trim()
      if ($t -and $t -ne 'none') {
        $p = $t -split ' '
        if ($p.Count -ge 10) { return [pscustomobject]@{ X = [double]$p[0]; Y = [double]$p[1]; Z = [double]$p[2]; Yaw = [double]$p[3]; Pitch = [double]$p[4]; EX = [double]$p[5]; EY = [double]$p[6]; EZ = [double]$p[7]; Stack = [int]$p[8]; Paused = [int]$p[9] } }
      }
    } catch { }
    Start-Sleep -Milliseconds 20
  }
  return $null
}
function Wrap([double]$a) { while ($a -gt 180) { $a -= 360 }; while ($a -le -180) { $a += 360 }; return $a }
function Clamp([double]$v, [double]$lim) { return [Math]::Max(-$lim, [Math]::Min($lim, $v)) }

function Calibrate-Mouse($g) {
  # how many degrees does one mouse count turn the camera? Measured, with the sign.
  Focus-Game $g
  $a = Get-Pose; [Nb01Mouse]::Move(120, 0); Start-Sleep -Milliseconds 300; $b = Get-Pose
  if ([Math]::Abs((Wrap ($b.Yaw - $a.Yaw))) -lt 0.01) {
    # the viewport has not captured the mouse yet (a window that was not clicked): one click in the middle of the window gives it the capture, like a player's first click
    Step $g 'mouse look did not respond: clicking once into the window to capture the mouse'
    $o = [Nb01Win]::Origin($g.Process.MainWindowHandle)
    Click-At $g ([int]($o[2] / 2)) ([int]($o[3] / 2)) 'left'
    Start-Sleep -Milliseconds 400
    $a = Get-Pose; [Nb01Mouse]::Move(120, 0); Start-Sleep -Milliseconds 300; $b = Get-Pose
  }
  $script:KYaw = (Wrap ($b.Yaw - $a.Yaw)) / 120.0
  [Nb01Mouse]::Move(0, 120); Start-Sleep -Milliseconds 300; $c = Get-Pose
  $script:KPitch = ($c.Pitch - $b.Pitch) / 120.0
  if ([Math]::Abs($script:KYaw) -lt 0.0001 -or [Math]::Abs($script:KPitch) -lt 0.0001) { throw "mouse look calibration failed (yaw/count=$($script:KYaw), pitch/count=$($script:KPitch)); is the game window focused?" }
  Step $g ("mouse calibration: {0:N4} deg yaw per count, {1:N4} deg pitch per count" -f $script:KYaw, $script:KPitch)
}

function Turn-To($g, [double]$Yaw, [double]$Pitch, [double]$Tol = 1.2, [int]$MaxMs = 4000) {
  $t0 = Get-Date
  while (((Get-Date) - $t0).TotalMilliseconds -lt $MaxMs) {
    $p = Get-Pose
    $ey = Wrap ($Yaw - $p.Yaw); $ep = $Pitch - $p.Pitch
    Add-Content -Path (Join-Path $g.Dir 'turn_trace.txt') -Value ('want yaw {0:N1} pitch {1:N1} | have {2:N1} {3:N1} | err {4:N1} {5:N1}' -f $Yaw, $Pitch, $p.Yaw, $p.Pitch, $ey, $ep)
    if ([Math]::Abs($ey) -lt $Tol -and [Math]::Abs($ep) -lt $Tol) { return $true }
    [Nb01Mouse]::Move([int](Clamp ($ey / $script:KYaw * 0.7) 300), [int](Clamp ($ep / $script:KPitch * 0.7) 300))
    Start-Sleep -Milliseconds 45
  }
  return $false
}

function Aim-At($g, [double]$X, [double]$Y, [double]$Z) {
  $p = Get-Pose
  $d = [Math]::Sqrt([Math]::Pow($X - $p.EX, 2) + [Math]::Pow($Y - $p.EY, 2))
  $yaw = [Math]::Atan2($Y - $p.EY, $X - $p.EX) * 180 / [Math]::PI
  $pitch = [Math]::Atan2($Z - $p.EZ, $d) * 180 / [Math]::PI
  return (Turn-To $g $yaw $pitch)
}

function Walk-To($g, [double]$X, [double]$Y, [double]$Radius = 30, [int]$MaxSeconds = 40, [double]$Coast = 65, [switch]$Pass) {
  # hold W, steer with the mouse; stop turning-in-place logic: W is released while the heading is more than 25 degrees off
  Focus-Game $g
  $vk = [byte]$script:VK['W']; $wDown = $false
  $t0 = Get-Date; $last = Get-Pose; $lastT = Get-Date; $stuckSince = $null; $ok = $false
  try {
    while (((Get-Date) - $t0).TotalSeconds -lt $MaxSeconds) {
      $p = Get-Pose
      $dx = $X - $p.X; $dy = $Y - $p.Y; $dist = [Math]::Sqrt($dx * $dx + $dy * $dy)
      $jump = [Math]::Sqrt([Math]::Pow($p.X - $last.X, 2) + [Math]::Pow($p.Y - $last.Y, 2))
      $script:Walked += $jump; if ($jump -gt $script:MaxJump) { $script:MaxJump = $jump }
      $last = $p
      if ($dist -le ($Radius + $Coast)) { $ok = $true; break }   # W is released early: the character coasts about $Coast cm
      $want = [Math]::Atan2($dy, $dx) * 180 / [Math]::PI
      $err = Wrap ($want - $p.Yaw)
      # keep the camera roughly level while walking
      $pitchErr = (-5.0) - $p.Pitch
      [Nb01Mouse]::Move([int](Clamp ($err / $script:KYaw * 0.6) 220), [int](Clamp ($pitchErr / $script:KPitch * 0.5) 80))
      if ([Math]::Abs($err) -gt 25) { if ($wDown) { [Nb01Win]::Key($vk, $false, $false); $wDown = $false } }
      else { if (-not $wDown) { [Nb01Win]::Key($vk, $true, $false); $wDown = $true } }
      # stuck detection: W held but hardly any progress for 2.5 s
      if ($wDown -and $jump -lt 0.2) { if (-not $stuckSince) { $stuckSince = Get-Date } elseif (((Get-Date) - $stuckSince).TotalSeconds -gt 2.5) { Step $g ("walk to ({0:N0},{1:N0}) STUCK at ({2:N0},{3:N0})" -f $X, $Y, $p.X, $p.Y); break } } else { $stuckSince = $null }
      Start-Sleep -Milliseconds 35
    }
  } finally { if ($wDown -and -not ($Pass -and $ok)) { [Nb01Win]::Key($vk, $false, $false) } }
  if (-not $Pass) { Start-Sleep -Milliseconds 350 }   # let the character come to rest
  if (-not $ok) { $p = Get-Pose; Step $g ("walk to ({0:N0},{1:N0}) ended at ({2:N0},{3:N0}) without reaching it" -f $X, $Y, $p.X, $p.Y) }
  return $ok
}

function Walk-Route($g, $Points, [double]$Radius = 30) {
  # intermediate points are passed without stopping (W stays down), the last one is approached and the character comes to rest there
  for ($i = 0; $i -lt $Points.Count; $i++) {
    $pt = $Points[$i]; $last = ($i -eq $Points.Count - 1)
    if ($last) { if (-not (Walk-To $g $pt[0] $pt[1] $Radius)) { return $false } }
    else { if (-not (Walk-To $g $pt[0] $pt[1] 45 40 0 -Pass)) { return $false } }
  }
  return $true
}

function Try-Interact($g, [double]$X, [double]$Y, [double]$Z, [scriptblock]$Done, [string]$Label) {
  # aim at the object, press E, look at the game state; if nothing happened step 50 cm closer and try again (a person would do the same)
  for ($try = 1; $try -le 3; $try++) {
    Aim-At $g $X $Y $Z | Out-Null
    if ($try -eq 1) { Save-Shot $g ("prompt_" + $Label) | Out-Null }
    Send-Key $g 'E'; Start-Sleep -Milliseconds 700
    $st = Get-State $g ("{0}: after E (try {1})" -f $Label, $try)
    if (& $Done $st) { return $st }
    $p = Get-Pose
    $d = [Math]::Sqrt([Math]::Pow($X - $p.X, 2) + [Math]::Pow($Y - $p.Y, 2))
    if ($d -gt 70) { $k = ($d - 50) / $d; Walk-To $g ($p.X + ($X - $p.X) * $k) ($p.Y + ($Y - $p.Y) * $k) 15 8 40 | Out-Null }
  }
  return $st
}

$g = Start-Game -Map /Game/SpnTest/Lvl_SpnMainMenu -OutDir $OutDir -Extra @(("-SpnSaveDir=" + (Join-Path $OutDir 'saves')), ("-SpnQaDir=" + $OutDir))
$script:PoseFile = Join-Path $OutDir 'pose.txt'
if (-not $NoVideo) { Start-Recording $g }
$ok = $true
try {
  Move-Mouse $g 1750 1000
  Step $g '1-2 launch, main menu'
  Start-Sleep -Seconds 2; Save-Shot $g 'main_menu'
  Send-Keys $g 'Down','Up'; Save-Shot $g 'main_menu_navigated'
  Step $g '3 new game'
  Send-Key $g 'Return'
  Start-Sleep -Seconds 14
  $s0 = Get-State $g 'after new game'; Save-Shot $g 'new_game_start'
  $startPose = Get-Pose
  Calibrate-Mouse $g
  Step $g '4-6 walk to the yard pickup (no teleport) and take it'
  Walk-To $g -10300 -5440 25 | Out-Null
  $s = Try-Interact $g -10150 -5440 20 { param($t) $t -match 'ration=2' } 'pickup'; Save-Shot $g 'picked_up'
  if ($StopAfter -eq 'pickup') { return }
  Step $g '7-8 approach the cottage door and open it'
  Walk-To $g -9600 -5440 25 | Out-Null
  $s = Try-Interact $g -9400 -5440 110 { param($t) $t -match 'door_cottage1_open=1' } 'door'
  Start-Sleep -Milliseconds 1500
  $s = Get-State $g 'door opened'; Save-Shot $g 'door_open'
  Step $g '9 walk through the doorway into the building'
  Walk-Route $g @(@(-9400, -5440), @(-9320, -5440)) 22 | Out-Null
  $s = Get-State $g 'inside'; Save-Shot $g 'inside'
  if ($StopAfter -eq 'inside') { return }
  Step $g '10b room 2: the bulky crate does not fit'
  Walk-Route $g @(@(-9320, -5280), @(-9100, -5280)) 25 | Out-Null
  Aim-At $g -9150 -5340 20 | Out-Null
  Send-Key $g 'E'; Start-Sleep -Milliseconds 500
  $s = Get-State $g 'crate refused'; Save-Shot $g 'crate_refused'
  Step $g '10 room 2: pick up the flashlight'
  Walk-To $g -8840 -5290 25 | Out-Null
  $s = Try-Interact $g -8840 -5170 20 { param($t) $t -match 'flashlight=1' } 'flashlight'; Save-Shot $g 'flashlight'
  Step $g '11 room 1: open the cupboard (container view)'
  Walk-Route $g @(@(-9100, -5280), @(-9320, -5280), @(-9320, -5440), @(-9320, -5630), @(-9100, -5630), @(-8760, -5630), @(-8700, -5640)) 25 | Out-Null
  $s = Try-Interact $g -8700 -5740 30 { param($t) $t -match 'container_view=1' } 'cupboard'; Save-Shot $g 'container_view'
  if ($StopAfter -eq 'container') { return }
  Step $g '12 transfer: bandages from the cupboard to the player (Right = actions panel, Enter = Move to Carried)'
  Send-Key $g 'Right'; Send-Key $g 'Return'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'container_after_transfer'
  $s = Get-State $g 'after the transfer'
  Step $g '13 close the container view'
  Send-Key $g 'Escape'; Send-Key $g 'Escape'; Start-Sleep -Milliseconds 400
  $s = Get-State $g 'container closed'
  Step $g '14 open the inventory (I) and equip the flashlight'
  Send-Key $g 'I'; Start-Sleep -Milliseconds 500
  Send-Keys $g 'Down','Down','Down','Return'; Start-Sleep -Milliseconds 400
  Save-Shot $g 'inventory_equipped'
  $s = Get-State $g 'flashlight equipped'
  Send-Key $g 'I'; Start-Sleep -Milliseconds 400
  $s = Get-State $g 'inventory closed'
  Step $g '14b leave the building on foot and walk to the street'
  Walk-Route $g @(@(-8760, -5630), @(-9100, -5630), @(-9320, -5630), @(-9320, -5440), @(-9400, -5440), @(-9640, -5440), @(-10200, -5440)) 28 | Out-Null
  $s = Get-State $g 'on the street'; Save-Shot $g 'on_the_street'
  Step $g '15 world map: place a waypoint with the keyboard crosshair'
  Send-Key $g 'M'; Start-Sleep -Milliseconds 800
  Save-Shot $g 'map_open'
  Send-Keys $g 'Down'
  Send-Key $g 'Return'; Start-Sleep -Milliseconds 300
  Save-Shot $g 'map_waypoint'
  Send-Key $g 'M'; Start-Sleep -Milliseconds 600
  $atMap = Get-State $g 'waypoint set, map closed'
  Step $g '16 navigate toward the waypoint on foot, steering by the compass bearing of the navigation reading'
  $dist0 = [double]([regex]::Match($atMap, 'dist=([0-9.]+)').Groups[1].Value)
  $bearing = [double]([regex]::Match($atMap, 'bearing=(-?[0-9.]+)').Groups[1].Value)
  $pose = Get-Pose
  # compass bearing (clockwise from north) -> Unreal yaw in the EastSouthUp frame (east = yaw 0)
  $yawTarget = $bearing - 90.0
  $tx = $pose.X + 1200 * [Math]::Cos($yawTarget * [Math]::PI / 180); $ty = $pose.Y + 1200 * [Math]::Sin($yawTarget * [Math]::PI / 180)
  Walk-To $g $tx $ty 40 25 | Out-Null
  $atNav = Get-State $g 'after walking toward the waypoint'; Save-Shot $g 'navigating'
  $dist1 = [double]([regex]::Match($atNav, 'dist=([0-9.]+)').Groups[1].Value)
  Step $g ("navigation check: distance to the waypoint {0:N1} m -> {1:N1} m ({2})" -f $dist0, $dist1, $(if ($dist1 -lt $dist0 - 3) { 'decreased' } else { 'NOT DECREASED' }))
  Step $g '17 pause menu, save game into a new slot'
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'pause_menu'
  Send-Keys $g 'Down','Return'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'save_slots_empty'
  Send-Key $g 'Return'; Start-Sleep -Milliseconds 700
  Save-Shot $g 'saved'
  $s = Get-State $g 'after save'
  $saved = $s
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 300
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 400
  $s = Get-State $g 'resumed'
  if ($StopAfter -eq 'saved') { return }
  Step $g '18 return to the main menu (pause -> Return to Main Menu -> confirm)'
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 400
  Send-Keys $g 'Down','Down','Down','Down','Return'; Start-Sleep -Milliseconds 400
  Save-Shot $g 'confirm_main_menu'
  Send-Keys $g 'Down','Return'
  Start-Sleep -Seconds 9
  Save-Shot $g 'main_menu_with_continue'
  Step $g '18b a NEW GAME resets the world: proof that the later load really restores'
  Send-Key $g 'Down'; Send-Key $g 'Return'     # Continue -> New Game
  Start-Sleep -Seconds 14
  $fresh = Get-State $g 'new game after the save (state must be empty)'
  Save-Shot $g 'second_new_game'
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 400
  Send-Keys $g 'Down','Down','Down','Down','Return'; Start-Sleep -Milliseconds 400
  Send-Keys $g 'Down','Return'
  Start-Sleep -Seconds 9
  Step $g '19-20 Continue: load the saved game and verify position, inventory, world state and waypoint'
  Save-Shot $g 'menu_continue_ready'
  Send-Key $g 'Return'
  Start-Sleep -Seconds 14
  $loaded = Get-State $g 'after loading the save'
  Save-Shot $g 'loaded_game'
  Send-Key $g 'M'; Start-Sleep -Milliseconds 800
  Save-Shot $g 'map_after_load'
  Send-Key $g 'M'; Start-Sleep -Milliseconds 400
  Send-Key $g 'I'; Start-Sleep -Milliseconds 600
  Save-Shot $g 'inventory_after_load'
  Send-Key $g 'I'
  $keys = 'player\{[^}]*\}', 'cupboard\{[^}]*\}', 'taken\{[^}]*\}', 'door_cottage1_open=\d', 'waypoint="[^"]*"', 'equipped:\S+', 'pos=\([^)]*\)'
  foreach ($k in $keys) {
    $a = [regex]::Match($saved, $k).Value; $f = [regex]::Match($fresh, $k).Value; $b = [regex]::Match($loaded, $k).Value
    $verdict = if ($a -eq $b -and $a -ne $f) { 'RESTORED (and the fresh game was different)' } elseif ($a -eq $b) { 'equal (but the fresh game was equal too: no proof)' } else { 'DIFFERENT' }
    Step $g ("verify {0}: {1} | saved='{2}' fresh='{3}' loaded='{4}'" -f $k, $verdict, $a, $f, $b)
  }
  Step $g ("no-teleport evidence: walked {0:N0} cm in total (sampled), largest position change between two samples {1:N1} cm; the QA teleport key F5 was never pressed" -f $script:Walked, $script:MaxJump)
}
catch { $ok = $false; Write-Host "SCENARIO ERROR: $_"; Add-Content -Path (Join-Path $g.Dir 'steps.txt') -Value "ERROR: $_" }
finally {
  Select-String -Path $g.Log -Pattern 'NB01-QA teleported' -Quiet | ForEach-Object { if ($_) { Add-Content -Path (Join-Path $g.Dir 'steps.txt') -Value 'WARNING: a QA teleport happened in this run' } }
  Select-String -Path $g.Log -Pattern 'LogSupernaturalProt: (Warning|Error)|LogSpnCore: (Warning|Error)|Fatal|Assertion' | Select-Object -First 15 | ForEach-Object { Write-Host $_.Line.Substring(0, [Math]::Min(200, $_.Line.Length)) }
  Stop-Recording $g
  Stop-Game $g
}
