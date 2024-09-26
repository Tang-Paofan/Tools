@echo off

set CoreLib_SourceDir=%UP3D_CORE_LIBRARY%\Bin\Release.x64
set EngineLib_SourceDir=%UP3D_ENGINE_LIBRARY%\Bin\Release.x64
set ScanLib_SourceDir=%UP3D_SCAN_LIBRARY%\Bin\Release.x64
set Introral_SourceDir=%UP3D_ORAL_FRAMEWORK%\BIN1.0
set IntroralPlugin_SourceDir=%UP3D_ORAL_FRAMEWORK%\BIN1.0\plugins
set DestinationDir=%UP3D_ORAL_PACKAGE%\BIN1.0

echo start CoreLib_SourceDir
robocopy "%CoreLib_SourceDir%" "%DestinationDir%" *.dll /XC /XL
echo start EngineLib_SourceDir
robocopy "%EngineLib_SourceDir%" "%DestinationDir%" *.dll /XC /XL
echo start ScanLib_SourceDir
robocopy "%ScanLib_SourceDir%" "%DestinationDir%" *.dll /XC /XL
echo start Introral_SourceDir
robocopy "%Introral_SourceDir%" "%DestinationDir%" *.dll /XC /XL
robocopy "%Introral_SourceDir%" "%DestinationDir%" *.exe /XC /XL
echo start IntroralPlugin_SourceDir
robocopy "%IntroralPlugin_SourceDir%" "%DestinationDir%\plugins" *.dll /XC /XL
REM robocopy拷贝返回值不同, 要特殊处理
@echo robocopy exit code: %ERRORLEVEL%
@if %ERRORLEVEL% GTR 3 ( echo robocopy ERROR )
@if %ERRORLEVEL% GTR 3 ( exit %ERRORLEVEL% )
@set ERRORLEVEL=0
REM at the end:
exit /b 0