<#
.SYNOPSIS
  Narzędzie projektu Zwiadowcy (UE5): przygotowanie, kompilacja, testy, uruchamianie, skrypty Pythona.

.EXAMPLE
  Scripts\zw.cmd setup          # kopiuje manekina i prototypy z szablonu UE, sprawdza narzędzia
  Scripts\zw.cmd build          # kompiluje edytor (ZwiadowcyUEEditor Win64 Development)
  Scripts\zw.cmd build-game     # kompiluje samą grę
  Scripts\zw.cmd py poligon     # uruchamia Scripts\py\poligon.py w edytorze bez okna
  Scripts\zw.cmd test           # testy automatyczne: edytor (bez okna) + gra
  Scripts\zw.cmd game           # uruchamia grę w oknie
  Scripts\zw.cmd editor         # otwiera edytor
  Scripts\zw.cmd paczka         # buduje paczkę gry dla Windows do Saved\Paczka

  Silnik: zmienna ZW_UE albo domyślnie C:\Program Files\Epic Games\UE_5.8
#>
param(
    [Parameter(Position = 0)][string]$Command = "help",
    [Parameter(Position = 1, ValueFromRemainingArguments = $true)][string[]]$Rest
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Project = Join-Path $Root "ZwiadowcyUE.uproject"
$EngineVersion = "5.8"

function Find-Engine {
    if ($env:ZW_UE -and (Test-Path $env:ZW_UE)) { return $env:ZW_UE }
    $reg = Get-ItemProperty "HKLM:\SOFTWARE\EpicGames\Unreal Engine\$EngineVersion" -ErrorAction SilentlyContinue
    if ($reg -and $reg.InstalledDirectory -and (Test-Path $reg.InstalledDirectory)) { return $reg.InstalledDirectory }
    $default = "C:\Program Files\Epic Games\UE_$EngineVersion"
    if (Test-Path $default) { return $default }
    throw "Nie znaleziono Unreal Engine $EngineVersion. Ustaw zmienną ZW_UE na katalog silnika."
}

$UE = Find-Engine
$Editor = Join-Path $UE "Engine\Binaries\Win64\UnrealEditor.exe"
$EditorCmd = Join-Path $UE "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$BuildBat = Join-Path $UE "Engine\Build\BatchFiles\Build.bat"
$RunUAT = Join-Path $UE "Engine\Build\BatchFiles\RunUAT.bat"
$LogDir = Join-Path $Root "Saved\Logs\zw"
New-Item -ItemType Directory -Force $LogDir | Out-Null

function Step($msg) { Write-Host "== $msg" -ForegroundColor Cyan }

function Invoke-Native([string]$Exe, [string[]]$Arguments) {
    # bez przekierowań stderr (PowerShell 5.1 zamienia je na błędy) - sam kod wyjścia
    & $Exe @Arguments | Out-Host
    return $LASTEXITCODE
}

function Copy-TemplatePack([string]$Pack, [string]$Target) {
    $src = Join-Path $UE "Templates\TemplateResources\High\$Pack\Content"
    $dst = Join-Path $Root "Content\$Target"
    if (-not (Test-Path $src)) { throw "Brak paczki szablonu: $src" }
    if (Test-Path $dst) {
        Write-Host "   $Target już jest"
        return
    }
    Copy-Item -Recurse $src $dst
    Write-Host "   skopiowano $Pack -> Content\$Target"
}

function Do-Setup {
    Step "Silnik: $UE"
    $ver = Get-Content (Join-Path $UE "Engine\Build\Build.version") -Raw | ConvertFrom-Json
    Write-Host ("   wersja {0}.{1}.{2}" -f $ver.MajorVersion, $ver.MinorVersion, $ver.PatchVersion)

    Step "Visual Studio"
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $vs = & $vswhere -latest -requires Microsoft.VisualStudio.Workload.NativeGame -property displayName
        if ($vs) { Write-Host "   $vs" } else { Write-Warning "Brak obciążenia 'Game development with C++' w Visual Studio" }
    } else { Write-Warning "Nie znaleziono Visual Studio" }

    Step "Git LFS"
    git -C $Root lfs install --local | Out-Null
    Write-Host "   włączony dla repozytorium"

    Step "Treści z szablonu UE (manekin, prototypy) - poza gitem"
    Copy-TemplatePack "Characters" "Characters"
    Copy-TemplatePack "LevelPrototyping" "LevelPrototyping"
    Write-Host "Gotowe. Dalej: Scripts\zw.cmd build"
}

function Do-Build([string]$Target, [string]$Config = "Development") {
    Step "Kompilacja $Target Win64 $Config"
    $code = Invoke-Native $BuildBat @($Target, "Win64", $Config, "-Project=$Project", "-WaitMutex", "-NoHotReloadFromIDE")
    if ($code -ne 0) { throw "Kompilacja nie powiodła się (kod $code)" }
}

function Do-Python([string]$Name, [string[]]$ScriptArgs) {
    $script = if (Test-Path $Name) { Resolve-Path $Name } else { Join-Path $Root "Scripts\py\$Name.py" }
    if (-not (Test-Path $script)) { throw "Brak skryptu $script" }
    $log = Join-Path $LogDir ("py_" + [IO.Path]::GetFileNameWithoutExtension($script) + ".log")
    Step "Python: $script"
    $argLine = ($ScriptArgs | ForEach-Object { $_ }) -join " "
    $scriptArg = "-script=`"$script $argLine`"".Replace("\", "/")
    $code = Invoke-Native $EditorCmd @($Project, "-run=pythonscript", $scriptArg, "-unattended", "-nosplash", "-nop4", "-NoSound", "-stdout", "-FullStdOutLogOutput", "-abslog=$log")
    $failed = Select-String -Path $log -Pattern "ZW-BLAD|LogPython: Error|Traceback" -SimpleMatch:$false -ErrorAction SilentlyContinue
    if ($failed) {
        $failed | Select-Object -First 15 | ForEach-Object { Write-Host $_.Line -ForegroundColor Red }
        throw "Skrypt Pythona zgłosił błędy (log: $log)"
    }
    if ($code -ne 0) { throw "Edytor zakończył się kodem $code (log: $log)" }
    Write-Host "   OK (log: $log)"
}

function Read-TestReport([string]$Dir, [string]$Label) {
    $index = Join-Path $Dir "index.json"
    if (-not (Test-Path $index)) { Write-Host "   $Label - brak raportu ($index)" -ForegroundColor Red; return 1 }
    $raw = [IO.File]::ReadAllText($index)
    if ($raw.Length -gt 0 -and $raw[0] -eq [char]0xFEFF) { $raw = $raw.Substring(1) }
    $r = $raw | ConvertFrom-Json
    $fails = 0
    foreach ($t in $r.tests) {
        $ok = $t.state -eq "Success"
        if (-not $ok) { $fails++ }
        $color = if ($ok) { "Green" } else { "Red" }
        Write-Host ("   [{0}] {1}" -f $t.state, $t.fullTestPath) -ForegroundColor $color
        if (-not $ok) {
            foreach ($e in $t.entries) {
                if ($e.event.type -eq "Error" -or $e.event.type -eq "Warning") { Write-Host ("        {0}: {1}" -f $e.event.type, $e.event.message) }
            }
        }
    }
    Write-Host ("   {0}: {1} OK, {2} błędów, razem {3}" -f $Label, $r.succeeded, ($r.failed + $r.notRun), $r.tests.Count)
    if ($r.tests.Count -eq 0) { return 1 }
    return $fails
}

function Do-Test([string]$Filter = "Zwiadowcy") {
    $base = Join-Path $Root "Saved\Testy"
    if (Test-Path $base) { Remove-Item -Recurse -Force $base }

    Step "Testy w edytorze (bez okna): $Filter"
    $edDir = Join-Path $base "edytor"
    Invoke-Native $EditorCmd @($Project, "-ExecCmds=Automation RunTests $Filter; Quit", "-unattended", "-nullrhi", "-nosplash", "-nop4", "-NoSound",
        "-ReportExportPath=$edDir", "-abslog=$(Join-Path $LogDir 'test_edytor.log')") | Out-Null
    $f1 = Read-TestReport $edDir "edytor"

    Step "Testy w grze: $Filter"
    $gameDir = Join-Path $base "gra"
    Invoke-Native $Editor @($Project, "-game", "-ExecCmds=Automation RunTests $Filter; Quit", "-unattended", "-windowed", "-ResX=960", "-ResY=540",
        "-nosplash", "-nop4", "-NoSound", "-ReportExportPath=$gameDir", "-abslog=$(Join-Path $LogDir 'test_gra.log')") | Out-Null
    $f2 = Read-TestReport $gameDir "gra"

    if (($f1 + $f2) -gt 0) { throw "Testy nie przeszły" }
    Write-Host "WYNIK: OK" -ForegroundColor Green
}

function Do-Shot([string]$Name = "poligon", [string]$Map = "") {
    $dir = Join-Path $Root "Saved\Zrzuty"
    New-Item -ItemType Directory -Force $dir | Out-Null
    Step "Zrzut i pomiar kl./s: $Name"
    $procArgs = @("`"$Project`"")
    if ($Map) { $procArgs += $Map }
    $procArgs += @("-game", "-zwzrzut=$Name", "-windowed", "-ResX=1920", "-ResY=1080", "-nosplash", "-NoSound", "-abslog=`"$(Join-Path $LogDir "zrzut_$Name.log")`"")
    Start-Process $Editor -ArgumentList $procArgs -Wait
    $png = Join-Path $dir "$Name.png"
    if (-not (Test-Path $png)) { throw "Brak zrzutu $png" }
    Write-Host "   $png"
    Get-Content (Join-Path $dir "raport.txt") -Encoding UTF8 | Select-Object -Last 1 | ForEach-Object { Write-Host "   $_" }
}

function Do-Package {
    $out = Join-Path $Root "Saved\Paczka"
    Step "Paczka Windows -> $out"
    $code = Invoke-Native $RunUAT @("BuildCookRun", "-project=$Project", "-platform=Win64", "-clientconfig=Development", "-build", "-cook", "-stage",
        "-pak", "-iostore", "-archive", "-archivedirectory=$out", "-unattended", "-utf8output", "-nop4")
    if ($code -ne 0) { throw "Budowa paczki nie powiodła się (kod $code)" }
}

switch ($Command.ToLower()) {
    "setup"      { Do-Setup }
    "build"      { Do-Build "ZwiadowcyUEEditor" }
    "build-game" { Do-Build "ZwiadowcyUE" ($(if ($Rest) { $Rest[0] } else { "Development" })) }
    "py"         { if (-not $Rest) { throw "Podaj nazwę skryptu z Scripts\py" }; Do-Python $Rest[0] ($Rest | Select-Object -Skip 1) }
    "test"       { Do-Test ($(if ($Rest) { $Rest[0] } else { "Zwiadowcy" })) }
    "game"       { Start-Process $Editor -ArgumentList @("`"$Project`"", "-game", "-windowed", "-ResX=1600", "-ResY=900") + $Rest }
    "editor"     { Start-Process $Editor -ArgumentList @("`"$Project`"") + $Rest }
    "zrzut"      { Do-Shot ($(if ($Rest) { $Rest[0] } else { "poligon" })) ($(if ($Rest.Count -gt 1) { $Rest[1] } else { "" })) }
    "paczka"     { Do-Package }
    default      { Get-Help $PSCommandPath -Detailed }
}
