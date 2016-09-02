#!/usr/bin/env bash

cd build
cmake -DCMAKE_C_COMPILER=$(which clang-3.8) -DCMAKE_BUILD_TYPE=Debug ..
make -j4
