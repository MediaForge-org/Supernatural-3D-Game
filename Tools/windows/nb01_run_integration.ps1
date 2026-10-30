# Night Build 01 integrated scenario (Windows PowerShell 5.1). Scripted QA input: real operating-system keyboard/mouse events into the running game window
# (not manual play); only the QA stations F5 (teleport next to an object) and F9 (state dump to the log) are QA hooks, enabled with -SpnQaKeys.
# Every step writes the observed game state to steps.txt; nothing is assumed. Records a 1080p60 video of the window.
param([string]$OutDir = ('C:\Projekte\Supernatural-3D-Game\Unreal\SupernaturalProt\Saved\NightBuild01\integration-' + (Get-Date -Format 'yyyyMMdd-HHmmss')), [switch]$NoVideo, [string]$StopAfter = '')
. "$PSScriptRoot\nb01_drive.ps1"
$g = Start-Game -Map /Game/SpnTest/Lvl_SpnMainMenu -OutDir $OutDir -Extra @(("-SpnSaveDir=" + (Join-Path $OutDir 'saves')))
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
  $s = Get-State $g 'after new game'; Save-Shot $g 'new_game_start'
  Step $g '4 move the character (real keys)'
  Hold-Key $g 'W' 1800; Hold-Key $g 'D' 700
  $s = Get-State $g 'after walking'; Save-Shot $g 'walked'
  Step $g '6 approach the yard pickup'
  Send-Key $g 'F5'; Start-Sleep -Milliseconds 600
  Save-Shot $g 'prompt_pickup'
  Send-Key $g 'E'; Start-Sleep -Milliseconds 500
  $s = Get-State $g 'after taking the rations'; Save-Shot $g 'picked_up'
  Step $g '7-8 open the door of the cottage'
  Send-Key $g 'F5'; Start-Sleep -Milliseconds 600
  Save-Shot $g 'prompt_door'
  Send-Key $g 'E'; Start-Sleep -Milliseconds 1800
  $s = Get-State $g 'door opened'; Save-Shot $g 'door_open'
  Step $g '9 walk into the building'
  Hold-Key $g 'W' 2200
  $s = Get-State $g 'inside'; Save-Shot $g 'inside'
  if ($StopAfter -eq 'inside') { return }
  Step $g '10 pick up the flashlight'
  Send-Key $g 'F5'; Start-Sleep -Milliseconds 600
  Send-Key $g 'E'; Start-Sleep -Milliseconds 500
  $s = Get-State $g 'flashlight taken'; Save-Shot $g 'flashlight'
  Step $g '10b the bulky crate does not fit'
  Send-Key $g 'F5'; Start-Sleep -Milliseconds 600
  Send-Key $g 'E'; Start-Sleep -Milliseconds 400
  $s = Get-State $g 'crate refused'; Save-Shot $g 'crate_refused'
  Step $g '11 open the cupboard (container view)'
  Send-Key $g 'F5'; Start-Sleep -Milliseconds 600
  Save-Shot $g 'prompt_cupboard'
  Send-Key $g 'E'; Start-Sleep -Milliseconds 900
  $s = Get-State $g 'container open'; Save-Shot $g 'container_view'
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
  Step $g '15 world map: place a waypoint 86 m east of the yard with the keyboard crosshair'
  Send-Key $g 'F5'; Start-Sleep -Milliseconds 600
  Send-Key $g 'M'; Start-Sleep -Milliseconds 800
  Save-Shot $g 'map_open'
  $s = Get-State $g 'map open'
  Send-Keys $g 'Right','Right','Right'
  Send-Key $g 'Return'; Start-Sleep -Milliseconds 300
  Save-Shot $g 'map_waypoint'
  Send-Key $g 'M'; Start-Sleep -Milliseconds 600
  $s = Get-State $g 'waypoint set, map closed'
  Step $g '16 navigate toward the waypoint (hold W for 5 s)'
  Hold-Key $g 'W' 5000
  $s = Get-State $g 'after walking toward the waypoint'; Save-Shot $g 'navigating'
  Step $g '17 pause menu, save game into a new slot'
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'pause_menu'
  $s = Get-State $g 'paused'
  Send-Keys $g 'Down','Return'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'save_slots_empty'
  Send-Key $g 'Return'; Start-Sleep -Milliseconds 700
  Save-Shot $g 'saved'
  $s = Get-State $g 'after save'
  $saved = $s
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 300
  Send-Keys $g 'Down','Return'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'load_slots'
  Send-Key $g 'Escape'; Start-Sleep -Milliseconds 300
  Step $g '17b settings from the pause menu'
  Send-Keys $g 'Down','Return'; Start-Sleep -Milliseconds 500
  Save-Shot $g 'settings'
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
  Step $g '19-20 Continue: load the saved game and verify inventory, world state and waypoint'
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
  $keys = 'player\{[^}]*\}', 'cupboard\{[^}]*\}', 'taken\{[^}]*\}', 'door_cottage1_open=\d', 'waypoint="[^"]*"', 'equipped:\S+'
  foreach ($k in $keys) {
    $a = [regex]::Match($saved, $k).Value; $f = [regex]::Match($fresh, $k).Value; $b = [regex]::Match($loaded, $k).Value
    $verdict = if ($a -eq $b -and $a -ne $f) { 'RESTORED (and the fresh game was different)' } elseif ($a -eq $b) { 'equal (but the fresh game was equal too: no proof)' } else { 'DIFFERENT' }
    Step $g ("verify {0}: {1} | saved='{2}' fresh='{3}' loaded='{4}'" -f $k, $verdict, $a, $f, $b)
  }
}
catch { $ok = $false; Write-Host "SCENARIO ERROR: $_"; Add-Content -Path (Join-Path $g.Dir 'steps.txt') -Value "ERROR: $_" }
finally {
  Select-String -Path $g.Log -Pattern 'LogSupernaturalProt: (Warning|Error)|LogSpnCore: (Warning|Error)|Fatal|Assertion' | Select-Object -First 15 | ForEach-Object { Write-Host $_.Line.Substring(0, [Math]::Min(200, $_.Line.Length)) }
  Stop-Recording $g
  Stop-Game $g
}
