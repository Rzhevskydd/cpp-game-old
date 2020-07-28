#!/bin/sh

BRD_ROOT=$(git rev-parse --show-toplevel); [ $? -eq 0 ] || exit 1
BRD_NPROC_M1=$(nproc --ignore 1); [ $? -eq 0 ] || exit 1

/usr/share/clang/run-clang-tidy.py \
    -p $BRD_ROOT/build \
    -quiet \
    -j $((($BRD_NPROC_M1 + 1) / 2)) || exit 1
