@echo off
chcp65501&cls

echo Now Update UP3D Intraoral Env
cd %UP3D_ENGINE_LIBRARY% & git pull & cd ..
cd %UP3D_CORE_LIBRARY% & git pull & cd ..
cd %UP3D_SCAN_LIBRARY% & git pull & cd ..
cd %UP3D_THIRD_LIBRARY% & git pull & cd ..
cd %UP3D_ORAL_FRAMEWORK% & git pull & cd ..
cd %UP3D_ORAL_PACKAGE% & git pull & cd ..
cd %UP3D_ORAL_BUILDER% & git pull & cd ..