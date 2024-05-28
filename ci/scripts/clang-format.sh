#!/bin/bash

# Format files
find . -type f \( -name '*.hpp' -o -name '*.cpp' \) | xargs -n 1 -P 0 /opt/homebrew/opt/llvm@17/bin/clang-format -i
