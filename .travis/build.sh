#!/usr/bin/env bash

export PATH="/usr/local/bin:$PATH"
export PATH="/usr/lib/llvm-3.8/bin:$PATH"

export CMAKE_C_COMPILER=$(which clang)
export CMAKE_CXX_COMPILER=$(which clang++)
export CPP=$(which clang++)
export CC=$(which clang)

echo "clang path: $(which clang)"

cd build
cmake -DCMAKE_C_COMPILER=$(which clang) -DTSC_TEST=1 -DCMAKE_CXX_COMPILER=$(which clang++) -DVALIDATE_MEMORY=FALSE -DVALIDATE_ADDRESS=FALSE -DCMAKE_BUILD_TYPE=Debug ..
make -j20
