//
// Created by bartek on 1/7/24.
//
#include "memblock.h"

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <zlib.h>
#include <string.h>


memblock * new_block(size_t size) {
    memblock * block = sbrk(0); // Get the current location of the program break.
    void * request = sbrk(size + sizeof(memblock)); // Increment the program break by size + sizeof(memblock).
    if (request == (void*) -1) { // If sbrk() returned -1, then an error occurred.
        return NULL;
    } else {
        block->free = false; // Set the block to not free.
        block->file = NULL; // Set the file to NULL.
        block->line = 0; // Set the line to 0.
        block->size = size; // Set the size to the requested size.
        block->prev = NULL; // Set the previous block to NULL.
        block->next = NULL; // Set the next block to NULL.
        validate_block(block); // Validate the block.
        return block; // Return the block.
    }
}

void validate_block(memblock * block) {
    block->hash = generate_hash(block); // Generate a hash for the block.
}

bool is_valid_block(memblock * block) {
    return block->hash == generate_hash(block); // Return true if the block is valid.
}

memblock * find_free_block(size_t size) {
    memblock * block = first_block; // Set the block to the first block.
    memblock * best_block = NULL; // Set the best block to NULL.
    while (block != NULL) { // While the block is not NULL.
        if (block->free && block->size >= size && (best_block == NULL || best_block->size > block->size)) {
            best_block = block; // Set the best block to the block.
        }
        block = block->next; // Set the block to the next block.
    }
    return best_block; // Return the best block.
}

short generate_hash(memblock * block) {
    uLong crc = crc32(0L, Z_NULL, 0);

    crc = crc32(crc, (const Bytef*)&block->free, sizeof(block->free));
    crc = crc32(crc, (const Bytef*)block->file, strlen(block->file) + 1);
    crc = crc32(crc, (const Bytef*)&block->line, sizeof(block->line));
    crc = crc32(crc, (const Bytef*)&block->size, sizeof(block->size));
    crc = crc32(crc, (const Bytef*)&block->prev, (unsigned long int) block->prev>>10);
    crc = crc32(crc, (const Bytef*)&block->next, (unsigned long int) block->next>>10);

    return (short)(crc % SHRT_MAX);
}