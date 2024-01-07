//
// Created by bartek on 1/7/24.
//

#ifndef MYLLOC_MEMBLOCK_H
#define MYLLOC_MEMBLOCK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct memblock {   // Our data structure for memory blocks. Takes up 40 bytes of memory.
    short hash;
    bool free;
    char * file;
    unsigned int line;
    size_t size;
    struct memblock *prev;
    struct memblock *next;
} memblock;

memblock * first_block = NULL;  // Pointer to the first block of memory.
memblock * last_block = NULL;   // Pointer to the last block of memory.

memblock  * new_block(size_t size); // Function to create a new block of memory.

memblock * find_free_block(size_t size); // Function to find a free block of memory. returns NULL if no free block is found.

bool is_valid_block(memblock * block); // Function to check if a block is valid.

void validate_block(memblock * block); // Function to validate a block.

short generate_hash(memblock * block); // Function to generate a hash.

void free_block(memblock * block); // Function to free a block of memory.

void merge_block(memblock * block); // Function to merge two blocks of memory.

void split_block(memblock * block, size_t size); // Function to split a block of memory into two blocks.

void give_block_back_to_os(memblock * block); // Function to give a block of memory back to the OS.

void * get_block_data(memblock * block); // Function to get the data of a block of memory.

void write_out_leaked_and_invalid(); // Function to write out stats.

#endif //MYLLOC_MEMBLOCK_H
