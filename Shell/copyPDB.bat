@echo off

set "CoreLib_SourceDir=%UP3D_CORE_LIBRARY%\Bin\Release.x64\"
set "EngineLib_SourceDir=%UP3D_ENGINE_LIBRARY%\Bin\Release.x64\"
set "ScanLib_SourceDir=%UP3D_SCAN_LIBRARY%\Bin\Release.x64\"
set "Introral_SourceDir=%UP3D_ORAL_FRAMEWORK%\BIN1.0\"
set "IntroralPlugin_SourceDir=%UP3D_ORAL_FRAMEWORK%\BIN1.0\plugins\"
set "DestinationDir=%UP3D_ORAL_PACKAGE%\PDB\"

echo start CoreLib
xcopy "%CoreLib_sourceDir%\*.pdb" "%destinationDir%" /y

echo start EngineLib
xcopy "%EngineLib_SourceDir%\*.pdb" "%destinationDir%" /y

echo start ScanLib
xcopy "%ScanLib_SourceDir%\*.pdb" "%destinationDir%" /y

echo start Intraoral
xcopy "%Introral_SourceDir%\*.pdb" "%destinationDir%" /y

echo start IntraoralPlugin
xcopy "%IntroralPlugin_SourceDir%\*.pdb" "%destinationDir%" /y

pause