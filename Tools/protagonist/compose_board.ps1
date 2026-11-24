<#
.SYNOPSIS
  Protagonist V1 feasibility phase, task 1: lays out the normalised head tiles of reference_board.py into two boards (colour and grey)
  with labels and the conflict / gap marks of the report. Reads only the tiles; the original reference sheets are never touched.
.EXAMPLE
  .\Tools\protagonist\compose_board.ps1 -BoardDir Artifacts\VisualProgress\ProtagonistV1_Feasibility\board
#>
param([string]$BoardDir = (Join-Path $PSScriptRoot '..\..\Artifacts\VisualProgress\ProtagonistV1_Feasibility\board'))
Add-Type -AssemblyName System.Drawing
$BoardDir = (Resolve-Path $BoardDir).Path   # System.Drawing resolves relative paths against the process directory, not PowerShell's
$meta = (Get-Content (Join-Path $BoardDir 'landmarks.json') -Raw | ConvertFrom-Json).views
# marks per view: what the report found (see Docs/Proposals/0016 section 2)
$marks = @{
    'key_34'           = 'MASTER identity (decision). Single view: no depth.'
    'fs_front'         = 'Older look: deep forehead lines; heavier beard.'
    'fb_front'         = 'Small (80 px eye-chin): +-5 %.'
    'fb_front_neutral' = 'Best front for shape; lips fuller than key art.'
    'fs_34_left'       = 'Usable 3/4.'
    'fb_34_left'       = 'Small; consistent with fs_34_left.'
    'fs_34_right'      = 'Head raised: not for proportions.'
    'fb_34_right'      = 'Usable 3/4 (small).'
    'fs_profile_l'     = 'ONLY profile of his LEFT side.'
    'fs_profile_r'     = 'Best profile (right side).'
    'fb_profile_a'     = 'CONFLICT: labelled LEFT, shows RIGHT side.'
    'fb_profile_b'     = 'Same side as fb_profile_a; nose longer.'
}
$tileW = 280; $tileH = 373; $cols = 6; $pad = 10; $top = 70; $labelH = 58
foreach ($kind in 'color', 'gray') {
    $rows = [math]::Ceiling($meta.Count / $cols)
    $W = $cols * ($tileW + $pad) + $pad; $H = $top + $rows * ($tileH + $labelH + $pad) + 40
    $bmp = New-Object System.Drawing.Bitmap $W, $H
    $g = [System.Drawing.Graphics]::FromImage($bmp); $g.Clear([System.Drawing.Color]::FromArgb(20, 20, 24)); $g.InterpolationMode = 'HighQualityBicubic'; $g.TextRenderingHint = 'AntiAlias'
    $fTitle = New-Object System.Drawing.Font('Segoe UI', 18, [System.Drawing.FontStyle]::Bold)
    $fId = New-Object System.Drawing.Font('Segoe UI', 10, [System.Drawing.FontStyle]::Bold)
    $fTxt = New-Object System.Drawing.Font('Segoe UI', 9)
    $g.DrawString("Protagonist V1 - head reference board ($kind) - normalised: eye line and chin at the same height, grey-world balance, luminance stretch", $fTitle, [System.Drawing.Brushes]::White, $pad, 10)
    $g.DrawString("Originals untouched (References/Characters/Protagonist). Green lines: eye line / chin; yellow: ~nose base / ~mouth at fixed fractions (not measured). Landmarks +-3..5 px on the 1536x1024 sheets.", $fTxt, [System.Drawing.Brushes]::Silver, $pad, 42)
    for ($i = 0; $i -lt $meta.Count; $i++) {
        $m = $meta[$i]; $x = $pad + ($i % $cols) * ($tileW + $pad); $y = $top + [math]::Floor($i / $cols) * ($tileH + $labelH + $pad)
        $img = [System.Drawing.Image]::FromFile((Join-Path $BoardDir "tile_$($m.id)_$kind.png"))
        $g.DrawImage($img, $x, $y, $tileW, $tileH); $img.Dispose()
        $mark = $marks[$m.id]
        $brush = if ($mark -match 'CONFLICT|not for|ONLY') { [System.Drawing.Brushes]::OrangeRed } elseif ($mark -match 'MASTER|Best') { [System.Drawing.Brushes]::LightGreen } else { [System.Drawing.Brushes]::Gainsboro }
        $g.DrawString("$($m.id)  ($($m.view))", $fId, [System.Drawing.Brushes]::White, (New-Object System.Drawing.RectangleF $x, ($y + $tileH + 2), $tileW, 18))
        $g.DrawString("eye-chin $($m.eye_to_chin_px_on_sheet) px on sheet, x$($m.scale). $mark", $fTxt, $brush, (New-Object System.Drawing.RectangleF $x, ($y + $tileH + 20), $tileW, 36))
    }
    $g.DrawString("Missing in every view: hairline / forehead / ears (hair), neutral flat light, scale (now 193 cm by decision), teeth, back of the head without hair, orthographic views.", $fTxt, [System.Drawing.Brushes]::Orange, $pad, $H - 30)
    $out = Join-Path (Split-Path $BoardDir -Parent) "reference_board_$kind.png"
    $bmp.Save($out); $g.Dispose(); $bmp.Dispose()
    Write-Host "wrote $out"
}
