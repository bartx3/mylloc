#!/bin/bash

echo "Installing required tools..."

tools=("valgrind" "clang-tidy" "clang" "clang-tools" "gcc" "cmake" "make" "python3" "zlib1g-dev" "libcunit1-dev" "libcunit1")

blank="(none)"

for tool in "${tools[@]}"; do
  policy=$(apt-cache policy "$tool" | grep "Installed:" | awk -F': ' '{print $2}')
  echo "$tool" : "$policy"
  if [[ "$policy" == "$blank" ]]; then
  echo "Installing $tool..."
    sudo apt-get install "$tool"
  fi
done
