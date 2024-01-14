#!/bin/bash

tools=("valgrind" "clang-tidy" "clang" "clang-tools" "gcc" "cmake" "make")

for tool in "${tools[@]}"; do
  if command -v $tool >/dev/null 2>&1; then
    sudo apt install $tool
  fi
done