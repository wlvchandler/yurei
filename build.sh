#!/bin/bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release --build build --config Release
mkdir -p bin
ln -sr build/bin/jaskASM bin/j16
