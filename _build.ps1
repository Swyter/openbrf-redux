mkdir _make
Push-Location _make

# swy: configuring the msvc environment variables
#      note: you can also use:
#             "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build"
Push-Location "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Auxiliary/Build"

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

# swy: don't loop and exit normally when this is used by an automated process like a buildbot: https://stackoverflow.com/a/33855217/674685
$isDotSourced = $MyInvocation.InvocationName -eq '.' -or $MyInvocation.Line -eq ''

while (1) {
  # swy: start the actual build with jom instead of nmake; for speed
  & ..\_qt\jom

  # swy: we're done, do it once and exit
  if ($isDotSourced) {
    break
  }
  
  pause
}

Pop-Location