bash install_env.sh

#if 2nd argument is clang then use clang compiler
if [ "$2" = "clang" ]; then
  cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S . -B build # -S: source directory, -B: build directory
else
  # otherwise use gcc compiler
  cmake -S . -B build # -S: source directory, -B: build directory
fi


cd build || exit

make

cd .. || exit

# copy the library file to directory given in first argument fi such directory exists
if [ -d "$1" ]; then
  cp build/libmylloc.a "$1"
else
  cp build/libmylloc.a .
fi