#!/bin/bash

set -x
set -e
set -u

wget https://apt.llvm.org/llvm.sh
chmod +x ./llvm.sh
sudo ./llvm.sh 12
rm ./llvm.sh