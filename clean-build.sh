#!/bin/bash
rm -rf build
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON
cd ..
cmake --build build
cmake --build build --target check
