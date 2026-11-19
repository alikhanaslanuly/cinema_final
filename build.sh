#!/usr/bin/env sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j && echo "Run: ./build/bin/cinema3d"
