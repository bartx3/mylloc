#include "mylloc.h"
#include "memblock.h"

#include <stdio.h>
#include <stdlib.h>

bool mylloc_initialized = false;

void mylloc_init(void)
{
    printf("mylloc_init()\n");
    pthread_mutex_init(&mylloc_mutex, NULL);
    first_block = NULL;
    last_block = NULL;
    mylloc_total_allocated = 0;
    mylloc_total_allocations = 0;
    mylloc_peak_memory_usage = 0;
    mylloc_current_memory_usage = 0;
    sbrk_calls = 0;
    mylloc_initialized = true;
    atexit(print_final_stats);
}

void print_stats(void)
{
    if (!mylloc_initialized) {
        printf("mylloc not initialized\n");
        return;
    }
    printf("mylloc_total_allocated: %zu\n", mylloc_total_allocated);
    printf("mylloc_total_allocations: %lu\n", mylloc_total_allocations);
    printf("mylloc_peak_memory_usage: %zu\n", mylloc_peak_memory_usage);
    printf("mylloc_current_memory_usage: %zu\n", mylloc_current_memory_usage);
    printf("sbrk_calls: %lu\n", sbrk_calls);
}

void print_final_stats(void)
{
    if (!mylloc_initialized) {
        printf("mylloc not initialized\n");
        return;
    }
    print_stats();
    write_out_leaked_and_invalid();
}


void * mylloc_impl(size_t size, const char * file, unsigned int line) {
    printf("mylloc_impl(%zu, %s, %u)\n", size, file, line);
    if (!mylloc_initialized) {
        printf("mylloc not initialized\n");
        return NULL;
    }
    if (size == 0) { // If the size is 0.
        return NULL; // Return NULL.
    }
    pthread_mutex_lock(&mylloc_mutex);
    memblock *block = find_free_block(size); // Find a free block of memory.
    if (block == NULL) { // If the block is NULL.
        block = new_block(size); // Create a new block of memory.
        sbrk_calls += 2; // Increment the sbrk calls. (one for getting required padding and one for getting the block)
        if (block == NULL) { // If the block is NULL.
            pthread_mutex_unlock(&mylloc_mutex);
            return NULL; // Return NULL.
        }
    } else {
        split_block(block, size); // Split the block.
    }
    block->free = false; // Set the block to not free.
    block->file = (char *) file; // Set the file of the block to the file.
    block->line = line; // Set the line of the block to the line.
    validate_block(block); // Validate the block.
    size_t allocated = block->size; // Set the allocated to the size of the block.
    mylloc_total_allocated += allocated; // Increment the total allocated by the allocated.
    mylloc_current_memory_usage += allocated; // Increment the current memory usage by the allocated.
    mylloc_total_allocations++; // Increment the total allocations.
    if (mylloc_current_memory_usage > mylloc_peak_memory_usage) { // If the total allocated is greater than the peak memory usage.
        mylloc_peak_memory_usage = mylloc_current_memory_usage; // Set the peak memory usage to the total allocated.
    }
    pthread_mutex_unlock(&mylloc_mutex);
    return get_block_data(block); // Return the data of the block.
}

void myfree(void * ptr) {
    printf("myfree(%p)\n", ptr);
    if (!mylloc_initialized) {
        printf("mylloc not initialized\n");
        return;
    }
    pthread_mutex_lock(&mylloc_mutex);
    if (ptr == NULL) { // If the pointer is NULL.
        pthread_mutex_unlock(&mylloc_mutex);
        return; // Return.
    }
    memblock * block = (memblock *) (ptr - sizeof(memblock)); // Set the block to the pointer - size of the block structure.
    if (!is_valid_block(block)) { // If the block is not a valid block.
        printf("freeing invalid block\n");
        pthread_mutex_unlock(&mylloc_mutex);
        return; // Return.
    }
    mylloc_current_memory_usage -= block->size; // Decrement the total allocated by the size of the block.
    free_block(block); // Free the block.
    if (last_block != NULL && last_block->free) { // If the last block is not NULL and is free.
        give_back_hanging_blocks(); // Give back hanging blocks.
    }
    pthread_mutex_unlock(&mylloc_mutex);
}

void give_back_hanging_blocks() {
    printf("Giving back hanging blocks\n");
    memblock *block = last_block; // Set the block to the first block.
    while (block != NULL && block->free) { // While the previous block is not NULL.
        memblock *prev = block->prev; // Set the previous block to the previous block of the block.
        give_block_back_to_os(block); // Give the block back to the OS.
        sbrk_calls++; // Increment the sbrk calls. (for sbrk below 0)
        block = prev; // Set the block to the previous block.
    }
    last_block = block; // Set the last block to the block.
    if (block != NULL) {
        block->next = NULL; // Set the next block of the block to NULL.
        validate_block(block); // Validate the block.
    }
    else
        first_block = NULL; // Set the first block to NULL as we have freed every block
}
