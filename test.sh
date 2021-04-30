#!/bin/bash

set -e
set -u
set -x

export PATH="$LLVM_DIR/bin:$PATH"

clang -O1 -emit-llvm -c inputs/input1.c -o inputs/input1.bc
llvm-link -o inputs/input1.linked.bc inputs/input1.bc RAP/runtime/rap.bc
opt -load=build/libRAP.so -rap inputs/input1.linked.bc -o inputs/input1.rap.bc
clang inputs/input1.rap.bc -o input1