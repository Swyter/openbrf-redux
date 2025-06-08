if (!([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) { Start-Process powershell.exe "-NoProfile -ExecutionPolicy -Scope CurrentUser RemoteSigned -File `"$PSCommandPath`"" -Verb RunAs; exit } 

# --
Write-Output '[-] swy: install chocolatey, to make it easy to install other software unattended'
# --
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

choco install 7zip curl                  -y --wait --no-progress
choco install visualstudio2022buildtools -y --wait --no-progress --package-parameters "--add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.Component.VC.Runtime.UCRTSDK --add Microsoft.VisualStudio.Component.Windows10SDK --add Microsoft.VisualStudio.Component.Windows10SDK.17763" --force

# --
Write-Output '[-] swy: downloading precompiled versions of qtbase, qttools (for windeployqt) and jom (for a more parallel nmake)'
# --
mkdir _qt
mkdir _qt_download
Push-Location _qt_download
curl.exe -LJ --no-clobber -o qt-base.7z  'https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt6_680/qt6_680/qt.qt6.680.win64_msvc2022_64/6.8.0-0-202410030750qtbase-Windows-Windows_11_23H2-MSVC2022-Windows-Windows_11_23H2-X86_64.7z'
curl.exe -LJ --no-clobber -o qt-tools.7z 'https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt6_680/qt6_680/qt.qt6.680.win64_msvc2022_64/6.8.0-0-202410030750qttools-Windows-Windows_11_23H2-MSVC2022-Windows-Windows_11_23H2-X86_64.7z'
curl.exe -LJ --no-clobber -o qt-trans.7z 'https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt6_680/qt6_680/qt.qt6.680.win64_msvc2022_64/6.8.0-0-202410030750qttranslations-Windows-Windows_11_23H2-MSVC2022-Windows-Windows_11_23H2-X86_64.7z'
curl.exe -LJ --no-clobber -o qt-decla.7z 'https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt6_680/qt6_680/qt.qt6.680.win64_msvc2022_64/6.8.0-0-202410030750qtdeclarative-Windows-Windows_11_23H2-MSVC2022-Windows-Windows_11_23H2-X86_64.7z' # swy: needed only for Qt6Qml.dll to be able to run lupdate.exe to refresh translations from source code, go figure >:(
curl.exe -LJ --no-clobber -o qt-jom.zip  'https://download.qt.io/official_releases/jom/jom.zip'

# --
Write-Output '[-] swy: extracting the downloaded qt binaries'

# --
7z x qt-base.7z  '-o../_qt' -y
7z x qt-tools.7z '-o../_qt' -y
7z x qt-trans.7z '-o../_qt' -y
7z x qt-decla.7z '-o../_qt' '6.8.0\msvc2022_64\bin\Qt6Qml*.dll' -y # swy: only extract the needed DLLs for lupdate.exe to run, instead of almost a gigabyte of crap, the quotes are needed for it to work under PowerShell
7z x qt-jom.zip  '-o../_qt' -y

Write-Output '         feel free to delete the qt-something.7z files in the _qt_download subfolder'
Write-Output '         once this is finished, as they really take a ton of space!'

# --
Write-Output '[-] swy: turn the qt install from enterprise to foss; remove the licensing checks'
# --
${qconfig-pri-folder} = '..\_qt\6.8.0\msvc2022_64\mkspecs\qconfig.pri'
(Get-Content ${qconfig-pri-folder}).replace('QT_EDITION = Enterprise', 'QT_EDITION = OpenSource') | Set-Content ${qconfig-pri-folder}
(Get-Content ${qconfig-pri-folder}).replace('QT_LICHECK = licheck.exe', '')                       | Set-Content ${qconfig-pri-folder}

# swy: make it find the qt sdk install folder instead of using the default: c:/Users/qt/work/install
#      keep in mind that Qt won't read UTF-16-encoded text files (the default for Out-File in PowerShell)
#      plus, even if you choose utf8 it writes a binary BOM/byte-order-mark which makes it not read it
#      -encoding UTF8NoBOM is only supported on 6+, so use plain ascii, absolutely braindead:
#      https://stackoverflow.com/a/55077592/674685
"[Paths]"+ "`r`n" +"Prefix = .." | Out-File -encoding ASCII '..\_qt\6.8.0\msvc2022_64\bin\qt.conf'
Pop-Location

# --
Write-Output '[-] swy: done'
# --