source install_env.sh

cmake -S . -B build # -S: source directory, -B: build directory

cd build || exit

make

cd .. || exit

# copy the library file to directory given in first argument fi such directory exists
if [ -d "$1" ]; then
  cp build/libmalloc.a "$1"
fi