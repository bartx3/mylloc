

#ifndef MYLLOC_MYLLOC_H
#define MYLLOC_MYLLOC_H
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

/**
 * Allocates a block of memory of size bytes.
 * @param size The size of the block of memory to allocate.
 * @param file The file where the allocation was made.
 * @param line The line where the allocation was made.
 * @return A pointer to the allocated block of memory.
 * @note If size is 0, then it returns NULL.
 * @note If the block of memory cannot be allocated, then it returns NULL.
 * @note If the block of memory cannot be allocated, then it prints out an error message.
 *
 * @note This function is thread-safe.
 *
 * @example
 * void * ptr = mylloc_impl(10, __FILE__, __LINE__);
 */
void* mylloc_impl(size_t size, const char * file, unsigned int line);

/**
 * Allocates a block of memory of size bytes.
 * @param size The size of the block of memory to allocate.
 * @return A pointer to the allocated block of memory.
 * @note If size is 0, then it returns NULL.
 * @note If the block of memory cannot be allocated, then it returns NULL.
 * @note If the block of memory cannot be allocated, then it prints out an error message.
 *
 * @note This function is thread-safe.
 *
 * @note This function is a wrapper for mylloc_impl(size, __FILE__, __LINE__).
 * @see mylloc_impl(size, __FILE__, __LINE__)
 *
 * @example
 * void * ptr = mylloc(10);
 */
#define mylloc(a) mylloc_impl(a, __FILE__, __LINE__)

/**
 * Frees the block of memory pointed to by ptr.
 * @param ptr A pointer to the block of memory to free.
 * @note If ptr is NULL, then it does nothing.
 * @note If ptr is not a pointer to a block of memory allocated by mylloc, then it does nothing.
 *
 * @note This function is thread-safe.
 *
 * @example
 * void * ptr = mylloc(10);
 * myfree(ptr);
 */
void myfree(void * ptr);

/**
 * Gets the total amount of memory allocated by mylloc through the entire runtime.
 * @return
 */
size_t get_mylloc_total_allocated(void);

/**
 * Gets the total count of allocations made by mylloc through the entire runtime.
 * @return
 */
unsigned long int get_mylloc_total_allocations(void);

/**
 * Gets the size of currently reserved memory by mylloc.
 * @return
 */
size_t get_mylloc_current_memory_usage(void);

/**
 * Gets the peak memory usage of mylloc.
 * @return
 */
size_t get_mylloc_peak_memory_usage(void);

/**
 * Gets the total number of sbrk calls made by mylloc.
 * @return
 */
unsigned long int get_mylloc_sbrk_calls(void);

/**
 * Prints out the statistics of mylloc.
 * @note If mylloc is not initialized, then it prints out an error message.
 * @note This function is thread-safe.
 *
 * @example
 * print_stats();
 */
void print_stats(void);

/**
 * Prints out the statistics of mylloc and exits the program.
 * @note If mylloc is not initialized, then it prints out an error message.
 *
 * @note This function is thread-safe.
 */
void print_final_stats(void);

/**
 * Prints out the blocks of memory.
 * @note If mylloc is not initialized, then it prints out an error message.
 *
 * @note This function is thread-safe.
 */
void print_blocks();

#ifdef __GNUC__
#define MYLLOC_INIT __attribute__((constructor))
/**
 * Initializes mylloc.
 * @note This function is not thread-safe.
 * @note This function is called automatically if you use GNU C.
 */
MYLLOC_INIT void mylloc_init(void);
#endif


#endif //MYLLOC_MYLLOC_H
