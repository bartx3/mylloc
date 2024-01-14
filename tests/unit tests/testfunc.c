//
// Created by bartek on 14.01.24.
//

#include <stdio.h>
#include "mylloc.h"
#include "memblock.h"
#include "testfunc.h"

int occupied_blocks() {
    int count = 0; // Set the count to 0.
    memblock * block = first_block; // Set the block to the first block.
    while (block != NULL) { // While the block is not NULL.
        if (!block->free) { // If the block is not free.
            count++; // Increment the count.
        }
        block = block->next; // Set the block to the next block.
    }

    return count; // Return the count.
}

int count_blocks() {
    int count = 0; // Set the count to 0.
    memblock * block = first_block; // Set the block to the first block.
    while (block != NULL) { // While the block is not NULL.
        count++; // Increment the count.
        block = block->next; // Set the block to the next block.
    }

    return count; // Return the count.
}