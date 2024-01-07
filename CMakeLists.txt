cmake_minimum_required(VERSION 3.27)
project(mylloc C)

set(CMAKE_C_STANDARD 90)

add_library(mylloc STATIC mylloc.c
        memblock.h
        memblock.c)
