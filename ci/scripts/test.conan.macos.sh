#!/bin/sh

cmake --build --preset conan-clang-17-23-debug --target catch2_unit_test

if [ $? = '0' ]; then
  build/clang-17-23/Debug/tests/unit/catch2_unit_test
fi
