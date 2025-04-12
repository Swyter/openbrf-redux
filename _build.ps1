Push-Location _make

# swy: configuring the msvc environment variables
Push-Location "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build"

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
Invoke-CmdScript vcvarsall.bat x64
Pop-Location

# swy: configuring the Qt environment variables
$env:Path += ";..\_qt\; ..\_qt\6.8.0\msvc2022_64\bin\"

# swy: run qmake and generate the msvc nmake makefile
& ..\_qt\6.8.0\msvc2022_64\bin\qmake ..\openBrf.pro

# swy: refresh the .ts files from the source code with lupdate; lrelease compiles .qm files from .ts
& ..\_qt\6.8.0\msvc2022_64\bin\lupdate ..\openBrf.pro
& ..\_qt\6.8.0\msvc2022_64\bin\lrelease ..\openBrf.pro

while (1) {
  # swy: start the actual build with jom instead of nmake; for speed
  & ..\_qt\jom

  pause
}