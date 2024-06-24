cmake_minimum_required(VERSION 3.10)
project(mylloc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ZLIB REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(mylloc STATIC src/mylloc.c
        src/memblock.c
        "tests/unit tests/test_mylloc.c"
        "tests/unit tests/testfunc.c"
        tests/e2e/e2e.c)

find_library(CUNIT_LIBRARY NAMES cunit CUnit)

add_executable(mylloc_tests "tests/unit tests/test_mylloc.c" "tests/unit tests/testfunc.c")

target_link_libraries(mylloc ${ZLIB_LIBRARIES})
target_link_libraries(mylloc_tests mylloc)
target_link_libraries(mylloc_tests ${CUNIT_LIBRARY})

add_executable(e2e "tests/e2e/e2e.c")

target_link_libraries(e2e mylloc)
target_link_libraries(e2e ${CUNIT_LIBRARY})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/e2e/rune2etest.py ${CMAKE_CURRENT_BINARY_DIR}/rune2etest.py COPYONLY)

add_custom_target(e2e_run
        COMMAND python3 ${CMAKE_CURRENT_BINARY_DIR}/rune2etest.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS e2e
        COMMENT "Running e2e tests")