#!/bin/bash

set -e
set -u

export PATH="$LLVM_DIR/bin:$PATH"

clang -emit-llvm -c inputs/input1.c -o inputs/input1.bc
opt -load=build/libRAP.so -rap inputs/input1.bc -o inputs/input1.bc
clang inputs/input1.bc -O1 -o input1