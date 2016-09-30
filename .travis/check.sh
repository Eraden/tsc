#!/usr/bin/env bash

cd build

ctest -j 20
[[ -f ./cts_test ]] && ./cts_test
