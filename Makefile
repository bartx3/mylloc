#Makefile

# Compiler
CC = gcc

make all: libmylloc.a
	./build_lib.sh .

unit_tests: build/mylloc_tests
	./build/mylloc_tests

build/mylloc_tests: build_lib.sh
	./build_lib.sh .

e2e_tests: build/e2e build/rune2etest.py
	cd build && python3 rune2etest.py
	cd ..

build/e2e: build_lib.sh
	./build_lib.sh .

libmylloc.a: build_lib.sh
	./build_lib.sh .

build/rune2etest.py: build_lib.sh
	./build_lib.sh .

clean :
	rm -rf build *.a

valgrind : build/mylloc_tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=1 ./build/mylloc_tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=1 ./build/e2e

clang_tidy: build_lib.sh
	./build_lib.sh .
	clang-tidy -p build/ -header-filter=.* -checks=-*,clang-analyzer-*,cppcoreguidelines-*,google-*,misc-*,modernize-*,performance-*,portability-*,readability-* src/*.c include/*.h

clang_analyzer: build_lib.sh
	./build_lib.sh .
	clang --analyze -Xanalyzer -analyzer-output=text -Xclang -analyzer-checker=core,deadcode,security,unix -Iinclude src/*.c

scan_build: build_lib.sh
	./build_lib.sh .
	scan-build -o build/ --status-bugs --keep-cc --show-description make

test: unit_tests e2e_tests valgrind