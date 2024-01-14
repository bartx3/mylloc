//
// Created by bartek on 14.01.24.
//
#include "mylloc.h"
#include <assert.h>
#include <string.h>

int main() {
    mylloc_init();

    // Test 1: Allocate a small block of memory
    size_t size = 128;
    void *ptr = mylloc(size);
    assert(ptr != NULL);

    // Write and read data
    memset(ptr, 'A', size);
    for (size_t i = 0; i < size; i++) {
        assert(((char*)ptr)[i] == 'A');
    }

    // Free the memory
    myfree(ptr);

    // Test 2: Allocate a large block of memory
    size = 1024 * 1024; // 1 MB
    ptr = mylloc(size);
    assert(ptr != NULL);

    // Write and read data
    memset(ptr, 'B', size);
    for (size_t i = 0; i < size; i++) {
        assert(((char*)ptr)[i] == 'B');
    }

    // Free the memory
    myfree(ptr);

    return 0;
}