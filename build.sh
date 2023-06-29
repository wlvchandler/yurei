#!/bin/bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
mkdir -p bin

#ln -srf build/bin/oni bin/oni
#ln -srf build/bin/yokai bin/yokai
ln -srf build/bin/yurei bin/yurei
