Push-Location _make

# swy: configuring the msvc environment variables
Push-Location "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build"

# swy: https://stackoverflow.com/a/41399983/674685
# Invokes a Cmd.exe shell script and updates the environment.
function Invoke-CmdScript {
    param(
    [String] $scriptName
    )
    $cmdLine = """$scriptName"" $args & set"
    & $Env:SystemRoot\system32\cmd.exe /c $cmdLine |
    select-string '^([^=]*)=(.*)$' | foreach-object {
    $varName = $_.Matches[0].Groups[1].Value
    $varValue = $_.Matches[0].Groups[2].Value
    set-item Env:$varName $varValue
    }
}
Invoke-CmdScript vcvarsall.bat x86
Pop-Location

# swy: configuring the Qt environment variables
$env:Path += ";..\_qt\; ..\_qt\5.12.12\msvc2017\bin\"

# swy: run qmake and generate the msvc nmake makefile
& ..\_qt\5.12.12\msvc2017\bin\qmake ..\openBrf.pro

while (1) {
  # swy: start the actual build with jom instead of nmake; for speed
  & ..\_qt\jom

  pause
}