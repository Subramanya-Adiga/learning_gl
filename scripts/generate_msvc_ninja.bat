@echo off
mkdir ..\build\standalone
cd ..\build\standalone
cmake ../.. -G"Ninja Multi-Config" -DVCPKG_TARGET_TRIPLET=x64-windows-static-md -DCMAKE_TOOLCHAIN_FILE=c:/vcpkg/scripts/buildsystems/vcpkg.cmake
