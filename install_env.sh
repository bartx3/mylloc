#!/bin/bash

echo "Installing required tools..."

tools=("valgrind" "clang-tidy" "clang" "clang-tools" "gcc" "cmake" "make" "python3" "zlib1g-dev" "libcunit1-dev" "libcunit1")

blank=""

for tool in "${tools[@]}"; do
  policy=$(apt-cache policy "$tool" | grep "Installed:")
  echo "$tool" : "$policy"
  if [[ "$policy" == "$blank" ]]; then
  echo "Installing $tool..."
    sudo apt-get install "$tool" -y
  fi
done
