<#
.SYNOPSIS
  Night Build 01 helper: starts the game in a real window, sends REAL keyboard input to it (Windows SendInput, scan codes) and captures
  screenshots of the window. Used for the scripted integration run and the demonstration video; everything it does is operating-system input,
  not a call into game code. It never commits, never kills processes it did not start.

.EXAMPLE
  . .\Tools\windows\nb01_drive.ps1
  $g = Start-Game -Map /Game/SpnTest/Lvl_SpnMainMenu -OutDir $env:TEMP\nb01
  Send-Keys $g 'Down','Return'
  Save-Shot $g 'after_new_game'
  Stop-Game $g
#>
Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
public static class Nb01Win {
  [StructLayout(LayoutKind.Sequential)] public struct RECT { public int Left, Top, Right, Bottom; }
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
  [DllImport("user32.dll")] public static extern bool SetCursorPos(int x, int y);
  [DllImport("user32.dll")] public static extern void mouse_event(uint f, uint dx, uint dy, int data, UIntPtr extra);
  [DllImport("user32.dll")] public static extern uint MapVirtualKey(uint code, uint type);
  [DllImport("user32.dll")] public static extern void keybd_event(byte vk, byte scan, uint flags, UIntPtr extra);
  [StructLayout(LayoutKind.Sequential)] public struct POINT { public int X, Y; }
  public static void Key(byte vk, bool down, bool extended) {
    byte scan = (byte)MapVirtualKey(vk, 0);
    uint flags = (down ? 0u : 2u) | (extended ? 1u : 0u);
    keybd_event(vk, scan, flags, UIntPtr.Zero);
  }
  public static void Mouse(uint flags, int data) { mouse_event(flags, 0, 0, data, UIntPtr.Zero); }
  public static void Shot(IntPtr h, string path) {
    RECT c; GetClientRect(h, out c);
    POINT p = new POINT(); p.X = 0; p.Y = 0; ClientToScreen(h, ref p);
    int w = c.Right - c.Left, ht = c.Bottom - c.Top;
    using (Bitmap bmp = new Bitmap(w, ht)) {
      using (Graphics g = Graphics.FromImage(bmp)) { g.CopyFromScreen(p.X, p.Y, 0, 0, new Size(w, ht)); }
      bmp.Save(path, ImageFormat.Png);
    }
  }
  public static int[] Origin(IntPtr h) { RECT c; GetClientRect(h, out c); POINT p = new POINT(); ClientToScreen(h, ref p); return new int[] { p.X, p.Y, c.Right - c.Left, c.Bottom - c.Top }; }
  public static void ClientPoint(IntPtr h, int x, int y) { POINT p = new POINT(); p.X = x; p.Y = y; ClientToScreen(h, ref p); SetCursorPos(p.X, p.Y); }
}
'@ -ReferencedAssemblies System.Drawing

$script:VK = @{ Escape = 0x1B; Return = 0x0D; Space = 0x20; Tab = 0x09; Back = 0x08; Delete = 0x2E; Left = 0x25; Up = 0x26; Right = 0x27; Down = 0x28
  Shift = 0x10; F5 = 0x74; F9 = 0x78; Home = 0x24; PageUp = 0x21; PageDown = 0x22; Add = 0x6B; Subtract = 0x6D; Oemplus = 0xBB; Oemminus = 0xBD }
foreach ($c in [char[]]'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') { $script:VK["$c"] = [int]$c }
$script:Extended = @('Left', 'Up', 'Right', 'Down', 'Delete', 'Home', 'PageUp', 'PageDown')

function Start-Game {
  param([string]$Map = '/Game/SpnTest/Lvl_SpnMainMenu', [string]$OutDir, [int]$Width = 1920, [int]$Height = 1080, [string[]]$Extra = @(), [int]$WaitSeconds = 90)
  New-Item -ItemType Directory -Force $OutDir | Out-Null
  $exe = 'C:\Program Files\Unreal Engine\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe'
  $proj = 'C:\Projekte\Supernatural-3D-Game\Unreal\SupernaturalProt\SupernaturalProt.uproject'
  $log = Join-Path $OutDir 'game.log'
  $args = @($proj, $Map, '-game', '-windowed', "-ResX=$Width", "-ResY=$Height", '-nosplash', '-log', "-abslog=$log", '-NoVerifyGC', '-FORCELOGFLUSH', '-SpnQaKeys') + $Extra
  $p = Start-Process -FilePath $exe -ArgumentList $args -PassThru
  $deadline = (Get-Date).AddSeconds($WaitSeconds)
  while ((Get-Date) -lt $deadline) {
    $p.Refresh()
    if ($p.HasExited) { throw "game exited early (code $($p.ExitCode)); see $log" }
    if ($p.MainWindowHandle -ne [IntPtr]::Zero -and (Test-Path $log) -and (Select-String -Path $log -Pattern 'Bringing World .* up for play|LogLoad: Took' -Quiet)) { break }
    Start-Sleep -Milliseconds 500
  }
  Start-Sleep -Seconds 4
  $p.Refresh()
  [Nb01Win]::ShowWindow($p.MainWindowHandle, 9) | Out-Null
  [Nb01Win]::SetForegroundWindow($p.MainWindowHandle) | Out-Null
  [pscustomobject]@{ Process = $p; Dir = $OutDir; Log = $log; N = 0 }
}

function Focus-Game($g) { $g.Process.Refresh(); [Nb01Win]::SetForegroundWindow($g.Process.MainWindowHandle) | Out-Null }

function Send-Key($g, [string]$Name, [int]$HoldMs = 60) {
  Focus-Game $g
  $vk = [byte]$script:VK[$Name]
  $ext = $script:Extended -contains $Name
  [Nb01Win]::Key($vk, $true, $ext)
  Start-Sleep -Milliseconds $HoldMs
  [Nb01Win]::Key($vk, $false, $ext)
  Start-Sleep -Milliseconds 140
}

function Send-Keys($g, [string[]]$Names, [int]$Hold = 60) { foreach ($n in $Names) { Send-Key $g $n $Hold } }

function Hold-Key($g, [string]$Name, [int]$Ms) {
  Focus-Game $g
  $vk = [byte]$script:VK[$Name]
  $ext = $script:Extended -contains $Name
  [Nb01Win]::Key($vk, $true, $ext); Start-Sleep -Milliseconds $Ms; [Nb01Win]::Key($vk, $false, $ext); Start-Sleep -Milliseconds 120
}

function Hold-Keys($g, [string[]]$Names, [int]$Ms) {
  Focus-Game $g
  foreach ($n in $Names) { [Nb01Win]::Key([byte]$script:VK[$n], $true, ($script:Extended -contains $n)) }
  Start-Sleep -Milliseconds $Ms
  foreach ($n in $Names) { [Nb01Win]::Key([byte]$script:VK[$n], $false, ($script:Extended -contains $n)) }
  Start-Sleep -Milliseconds 120
}

function Click-At($g, [int]$X, [int]$Y, [string]$Button = 'left') {
  Focus-Game $g
  [Nb01Win]::ClientPoint($g.Process.MainWindowHandle, $X, $Y); Start-Sleep -Milliseconds 120
  if ($Button -eq 'left') { [Nb01Win]::Mouse(0x2, 0); Start-Sleep -Milliseconds 50; [Nb01Win]::Mouse(0x4, 0) } else { [Nb01Win]::Mouse(0x8, 0); Start-Sleep -Milliseconds 50; [Nb01Win]::Mouse(0x10, 0) }
  Start-Sleep -Milliseconds 200
}

function Move-Mouse($g, [int]$X, [int]$Y) { Focus-Game $g; [Nb01Win]::ClientPoint($g.Process.MainWindowHandle, $X, $Y); Start-Sleep -Milliseconds 120 }
function Wheel($g, [int]$Clicks) { Focus-Game $g; [Nb01Win]::Mouse(0x800, 120 * $Clicks); Start-Sleep -Milliseconds 200 }

function Save-Shot($g, [string]$Name) {
  $g.N++
  $path = Join-Path $g.Dir ('{0:D2}_{1}.png' -f $g.N, $Name)
  [Nb01Win]::Shot($g.Process.MainWindowHandle, $path)
  $path
}

function Stop-Game($g) {
  if ($g.Process -and -not $g.Process.HasExited) { $g.Process.CloseMainWindow() | Out-Null; Start-Sleep -Seconds 3; if (-not $g.Process.HasExited) { $g.Process.Kill() } }
}


function Get-State($g, [string]$Label) {
  $before = if (Test-Path $g.Log) { (Select-String -Path $g.Log -Pattern 'NB01-STATE' | Measure-Object).Count } else { 0 }
  Send-Key $g 'F9'
  $line = $null
  for ($i = 0; $i -lt 30; $i++) {
    Start-Sleep -Milliseconds 200
    $m = Select-String -Path $g.Log -Pattern 'NB01-STATE'
    if (($m | Measure-Object).Count -gt $before) { $line = ($m | Select-Object -Last 1).Line; break }
  }
  $text = if ($line) { $line.Substring($line.IndexOf('NB01-STATE') + 11) } else { 'NO STATE LINE' }
  $out = "[{0}] {1}" -f $Label, $text
  Add-Content -Path (Join-Path $g.Dir 'steps.txt') -Value $out
  Write-Host $out
  $text
}

function Step($g, [string]$Text) { Add-Content -Path (Join-Path $g.Dir 'steps.txt') -Value ("--- " + $Text); Write-Host ("--- " + $Text) }

function Start-Recording($g, [string]$Name = 'run.mp4', [int]$Fps = 60) {
  # DXGI desktop duplication (ddagrab) + NVENC: 1920x1080 at 60 fps of the game window's client area. Needs ffmpeg with ddagrab (Gyan full build).
  $p = $g.Process; $p.Refresh()
  $o = [Nb01Win]::Origin($p.MainWindowHandle)
  $out = Join-Path $g.Dir $Name
  $psi = New-Object System.Diagnostics.ProcessStartInfo
  $psi.FileName = 'cmd.exe'
  $psi.Arguments = "/c ffmpeg -y -f lavfi -i ddagrab=output_idx=0:framerate=$Fps`:draw_mouse=1:video_size=$($o[2])x$($o[3]):offset_x=$($o[0]):offset_y=$($o[1]) -c:v h264_nvenc -preset p5 -cq 21 `"$out`" 2> `"$out.log`""
  $psi.RedirectStandardInput = $true; $psi.UseShellExecute = $false; $psi.CreateNoWindow = $true
  $ff = [System.Diagnostics.Process]::Start($psi)
  # drain stderr so ffmpeg never blocks on a full pipe


  $g | Add-Member -NotePropertyName Recorder -NotePropertyValue $ff -Force
  $g | Add-Member -NotePropertyName Video -NotePropertyValue $out -Force
}
function Stop-Recording($g) {
  if ($g.Recorder -and -not $g.Recorder.HasExited) {
    $g.Recorder.StandardInput.Write('q'); $g.Recorder.StandardInput.Flush()   # a clean stop finalizes the MP4 (moov atom)
    if (-not $g.Recorder.WaitForExit(20000)) { $g.Recorder.Kill() }
  }
}
