#!/bin/sh
cmake --build --preset conan-clang-17-23-relwithdebinfo -j6

if [ $? = '0' ]; then
  rootdir=`pwd`
  $rootdir/build/clang-17-23/RelWithDebInfo/app/Ray_tracing sphere
fi

