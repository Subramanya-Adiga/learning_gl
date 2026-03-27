#!/bin/bash

cd ../build/ && mkdir standalone && cd standalone
cmake ../.. -G"Ninja Multi-Config" -DCMAKE_TOOLCHAIN_FILE="~/vcpkg/scripts/buildsystems/vcpkg.cmake"
