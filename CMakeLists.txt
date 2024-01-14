cmake_minimum_required(VERSION 3.27)
project(mylloc C)

set(CMAKE_C_STANDARD 11)

find_package(ZLIB REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(mylloc STATIC src/mylloc.c
        src/memblock.c
        "tests/unit tests/test_mylloc.c"
        "tests/unit tests/testfunc.c"
        tests/e2e/e2e.c)

add_executable(mylloc_tests "tests/unit tests/test_mylloc.c" "tests/unit tests/testfunc.c")

target_link_libraries(mylloc ${ZLIB_LIBRARIES})

target_link_libraries(mylloc_tests mylloc)

target_link_libraries(mylloc_tests cunit)

add_executable(e2e "tests/e2e/e2e.c")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/e2e/rune2etest.py ${CMAKE_CURRENT_BINARY_DIR}/rune2etest.py COPYONLY)

target_link_libraries(e2e mylloc)