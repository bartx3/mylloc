//
// Created by bartek on 1/7/24.
//
#include "memblock.h"

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <zlib.h>
#include <string.h>
#include <signal.h>

memblock * new_block(size_t size) {
    void * last_byte = sbrk(0);
    if (last_byte == NULL || last_byte == (void *) -1) {    // If sbrk() returned NULL, then an error occurred.
        return NULL;
    }
    // add enough so thatfirst adress after block structure is aligned to 16
    size_t padding = (16 - ((size_t) last_byte + sizeof(memblock)) % 16) % 16;
    size += padding;
    memblock * block = sbrk(size + sizeof(memblock)); // Increment the program break by size + sizeof(memblock).
    if (block == (void*) -1) { // If sbrk() returned -1, then an error occurred.
        return NULL;
    }

    block->free = false; // Set the block to not free.
    block->padding = padding; // Set the padding to the padding.
    block->file = NULL; // Set the file to NULL.
    block->line = 0; // Set the line to 0.
    block->size = size; // Set the size to the requested size.
    block->prev = last_block; // Set the previous block to NULL.
    block->next = NULL; // Set the next block to NULL.
    validate_block(block); // Validate the block.

    last_block = block; // Set the last block to the block.
    return block; // Return the block.
}

void validate_block(memblock * block) {
    block->hash = generate_hash(block); // Generate a hash for the block.
}

bool is_valid_block(memblock * block) {
    if (block == NULL) { // If the block is NULL.
        return true;
    }
    return block->hash == generate_hash(block); // Return true if the block is valid.
}

bool is_valid_block_or_exit(memblock * block) {
    if (!is_valid_block(block)) { // If the block is not valid.
        raise(SIGSEGV);
        return false;
    }
    return true;
}

memblock * find_free_block(size_t size) {
    memblock * block = first_block; // Set the block to the first block.
    memblock * best_block = NULL; // Set the best block to NULL.
    while (block != NULL) { // While the block is not NULL.
        is_valid_block_or_exit(block); // Check if the block is valid.
        if (block->free && block->size >= size && (best_block == NULL || best_block->size > block->size)) {
            best_block = block; // Set the best block to the block.
        }
        block = block->next; // Set the block to the next block.
    }
    return best_block; // Return the best block.
}

short generate_hash(memblock * block) {
    if (block == NULL) { // If the block is NULL.
        return 0;
    }
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef *) &block->free, sizeof(block->free));
    crc = crc32(crc, (const Bytef *) &block->padding, sizeof(block->padding));
    crc = crc32(crc, (const Bytef *) &block->file, sizeof(block->file));
    crc = crc32(crc, (const Bytef *) &block->line, sizeof(block->line));
    crc = crc32(crc, (const Bytef *) &block->size, sizeof(block->size));
    crc = crc32(crc, (const Bytef *) &block->prev, sizeof(block->prev));
    crc = crc32(crc, (const Bytef *) &block->next, sizeof(block->next));
    return (short)(crc % SHRT_MAX);
}

void free_block(memblock * block) {
    is_valid_block_or_exit(block); // Check if the block is valid.
    block->free = true; // Set the block to free.
    validate_block(block); // Validate the block.
    if (block->next != NULL && is_valid_block_or_exit(block->next)) { // If the next block is not NULL and is free.
        if (block->next->free) { // If the next block is free.
            merge_block(block); // Merge the block.
        }
        block->size += block->next->padding; // Increment the size of the block by the padding of the next block.
        block->next->padding = 0; // Merge the block.
        validate_block(block->next); // Validate the block.
    }
    if (block->prev != NULL && is_valid_block_or_exit(block->prev) && block->prev->free) { // If the previous block is not NULL and is free.
        merge_block(block->prev); // Merge the previous block.
    }
    validate_block(block); // Validate the block.
}

void * get_block_data(memblock * block) {
    return (void *) block + sizeof(memblock); // Return the block + size of clock structure.
}

void merge_block(memblock * block) {
    is_valid_block_or_exit(block); // Check if the block is valid.
    is_valid_block_or_exit(block->next); // Check if the next block is valid.
    if (block->next != NULL && block->next->free) { // If the next block is not NULL and is free.
        block->size += sizeof(memblock) + block->next->size; // Increment the size of the block by the size of the next block.
        block->next = block->next->next; // Set the next block to the next block of the next block.
        if (block->next != NULL) { // If the next block is not NULL.
            block->next->prev = block; // Set the previous block of the next block to the block.
        }
    }
    validate_block(block); // Validate the block.
}

void split_block(memblock * block, size_t size) {
    is_valid_block_or_exit(block); // Check if the block is valid.
    if (block->size > size + sizeof(memblock)) { // If the size of the block is greater than the size + size of the block structure.
        memblock * new = (memblock *) (get_block_data(block) + size); // Create a new block.
        new->free = true; // Set the new block to free.
        new->file = NULL; // Set the file of the new block to NULL.
        new->line = 0; // Set the line of the new block to 0.
        new->size = block->size - size - sizeof(memblock); // Set the size of the new block to the size of the block - size - size of the block structure.
        new->prev = block; // Set the previous block of the new block to the block.
        new->next = block->next; // Set the next block of the new block to the next block of the block.
        if (new->next != NULL) { // If the next block of the new block is not NULL.
            new->next->prev = new; // Set the previous block of the next block of the new block to the new block.
        }
        block->size = size; // Set the size of the block to the size.
        block->next = new; // Set the next block of the block to the new block.
        validate_block(block); // Validate the block.
        validate_block(new); // Validate the new block.
    }
}

void give_block_back_to_os(memblock * block) {
    is_valid_block_or_exit(block); // Check if the block is valid.
    if (block->next == NULL) { // If the next block is NULL.
        if (block->prev == NULL) { // If the previous block is NULL.
            sbrk(-block->size - sizeof(memblock)); // Decrement the program break by the size of the block + size of the block structure.
        } else {
            block->prev->next = NULL; // Set the next block of the previous block to NULL.
            sbrk(-block->size - sizeof(memblock)); // Decrement the program break by the size of the block + size of the block structure.
        }
    }
}

void write_out_leaked_and_invalid() {
    memblock * block = first_block; // Set the block to the first block.
    while (block != NULL) { // While the block is not NULL.
        if (!is_valid_block(block)) { // If the block is not valid.
            printf("Block with allocated pointer at %p allocated at file: %s line: %u is invalid\n", get_block_data(block), block->file, block->line); // Print out that the block is invalid.
            return;
        } else if (!block->free) { // If the block is not free.
            printf("Block with allocated pointer at %p allocated at file: %s line: %u is leaked\n", get_block_data(block), block->file, block->line);
        }
        block = block->next; // Set the block to the next block.
    }
}