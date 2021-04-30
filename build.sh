#!/bin/bash

set -x
set -e
set -u

export PATH="$LLVM_DIR/bin:$PATH"

rm -r build || true
mkdir -p build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../RAP
make
cd ..

clang -O1 -emit-llvm -c -o RAP/runtime/rap.bc RAP/runtime/rap.c