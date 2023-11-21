#!/bin/sh
git submodule update --init
git submodule update --recursive --remote
git pull --recurse-submodules
cmake -S . -B build/
