#!/bin/sh
cmake --build --preset conan-clang-17-23-debug -j6 -Dgsl_CONFIG_CONTRACT_VIOLATION_ASSERTS

rootdir=`pwd`
$rootdir/build/clang-17-23/Debug/app/Ray_tracing sphere
