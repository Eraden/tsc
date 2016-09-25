#!/usr/bin/env bash

cd build

ctest
[[ -f ./cts_test ]] && ./cts_test
