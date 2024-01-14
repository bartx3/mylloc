#!/bin/bash

tools=("valgrind" "clang-tidy" "clang" "clang-tools" "gcc" "cmake" "make" "python3" "zlib1g-dev" "libcunit1-dev")

blank=""

for tool in "${tools[@]}"; do
  policy=$(apt-cache policy "$tool" | grep "Installed:")
  if [[ "$policy" == "$blank" ]]; then
    sudo apt-get install "$tool"
  fi
done