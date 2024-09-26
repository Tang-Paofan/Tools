@echo off

set "CoreLib_SourceDir=%UP3D_CORE_LIBRARY%\Bin\Release.x64\"
set "EngineLib_SourceDir=%UP3D_ENGINE_LIBRARY%\Bin\"
set "ScanLib_SourceDir=%UP3D_SCAN_LIBRARY%\Bin\Release.x64\"
set "Introral_SourceDir=%UP3D_ORAL_FRAMEWORK%\BIN1.0\"
set "IntroralPlugin_SourceDir=%UP3D_ORAL_FRAMEWORK%\BIN1.0\plugins\"

cmdkey /add:192.168.1.145 /user:up3d /pass:up3d
net use \\192.168.1.145\UPSymbolServer /user:up3d

echo start CoreLib
symstore add /r /f "%CoreLib_sourceDir%\*.pdb" /s \\192.168.1.145\UPSymbolServer /t "UP3D.Intraoral"

echo start EngineLib
symstore add /r /f "%EngineLib_SourceDir%\*.pdb" /s \\192.168.1.145\UPSymbolServer /t "UP3D.Intraoral"

echo start ScanLib
symstore add /r /f "%ScanLib_SourceDir%\*.pdb" /s \\192.168.1.145\UPSymbolServer /t "UP3D.Intraoral"

echo start Intraoral
symstore add /r /f "%Introral_SourceDir%\*.pdb" /s \\192.168.1.145\UPSymbolServer /t "UP3D.Intraoral"

echo start IntraoralPlugin
symstore add /r /f "%IntroralPlugin_SourceDir%\*.pdb" /s \\192.168.1.145\UPSymbolServer /t "UP3D.Intraoral"

pause