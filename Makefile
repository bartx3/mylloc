#Makefile

# Compiler
CC = gcc

make all: libmalloc.a
	./install_lib.sh

unit_tests: build/malloc_tests
	./build/malloc_tests

build/malloc_tests: build_lib.sh
	./install_lib.sh

e2e_tests: build/e2e build/rune2etest.py
	cd build
	python3 build/rune2etest.py
	cd ..

build/e2e: build_lib.sh
	./install_lib.sh

libmalloc.a: build/malloc.o
	./install_lib.sh

build/rune2etest.py: build_lib.sh
	./install_lib.sh

clean :
	rm -rf build *.a

valgrind : build/malloc_tests
	valgrind --leak-check=full --show-leak-kinds=all ./build/malloc_tests