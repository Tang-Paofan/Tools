@echo off
chcp65501&cls

echo Now Setting UP3D Intraoral Env
echo Setting UP3D_ENGINE_LIBRARY=%~dp0up3d-engine-library
SETX UP3D_ENGINE_LIBRARY "%~dp0up3d-engine-library"
echo Setting UP3D_CORE_LIBRARY=%~dp0up3d-core-library
SETX UP3D_CORE_LIBRARY "%~dp0up3d-core-library"
echo Setting UP3D_SCAN_LIBRARY=%~dp0up3d-scan-library
SETX UP3D_SCAN_LIBRARY "%~dp0up3d-scan-library"
echo Setting UP3D_THIRD_LIBRARY=%~dp0up3d-third-library
SETX UP3D_THIRD_LIBRARY "%~dp0up3d-third-library"
echo Setting UP3D_ENGINE_FRAMEWORK=%~dp0uengine
SETX UP3D_ENGINE_FRAMEWORK "%~dp0uengine"
echo Setting UP3D_ORAL_FRAMEWORK=%~dp0Intraoral
SETX UP3D_ORAL_FRAMEWORK "%~dp0Intraoral"
echo Setting UP3D_ORAL_PACKAGE=%~dp0up3d-intraoral-package
SETX UP3D_ORAL_PACKAGE "%~dp0up3d-intraoral-package"
echo Setting UP3D_ORAL_BUILDER=%~dp0up3d-intraoral-builder
SETX UP3D_ORAL_BUILDER "%~dp0up3d-intraoral-builder"