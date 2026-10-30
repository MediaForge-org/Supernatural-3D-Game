# Night Build 01 smoke run (Windows PowerShell 5.1): main menu -> new game -> walk -> map -> waypoint, with screenshots. Real OS keyboard input.
param([string]$OutDir = 'C:\Projekte\Supernatural-3D-Game\Unreal\SupernaturalProt\Saved\NightBuild01\smoke')
. "$PSScriptRoot\nb01_drive.ps1"
$g = Start-Game -Map /Game/SpnTest/Lvl_SpnMainMenu -OutDir $OutDir
try {
  Move-Mouse $g 1700 980
  Save-Shot $g 'menu'
  Send-Key $g 'Return'                 # New Game (first enabled entry)
  Start-Sleep -Seconds 14
  Save-Shot $g 'gameplay_start'
  Hold-Key $g 'W' 2500
  Save-Shot $g 'after_walk'
  Send-Key $g 'M'
  Start-Sleep -Milliseconds 800
  Save-Shot $g 'map_open'
  Send-Key $g 'Return'                 # waypoint at the crosshair (keyboard mode)
  Send-Keys $g 'Right','Right','Up'
  Send-Key $g 'Tab'
  Save-Shot $g 'map_selected'
  Send-Key $g 'Return'
  Save-Shot $g 'map_waypoint'
  Send-Key $g 'M'
  Start-Sleep -Milliseconds 800
  Save-Shot $g 'hud_with_waypoint'
} finally {
  Select-String -Path $g.Log -Pattern 'LogSupernaturalProt|LogSpnCore|Error' | Select-Object -Last 40 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(220, $_.Line.Length)) }
  Stop-Game $g
}
