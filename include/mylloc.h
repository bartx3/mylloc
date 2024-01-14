

#ifndef MYLLOC_MYLLOC_H
#define MYLLOC_MYLLOC_H
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

void* mylloc_impl(size_t size, const char * file, unsigned int line);
#define mylloc(a) mylloc_impl(a, __FILE__, __LINE__)

void myfree(void * ptr);
pthread_mutex_t mylloc_mutex;
size_t mylloc_total_allocated;
unsigned long int mylloc_total_allocations;
size_t mylloc_current_memory_usage;
size_t mylloc_peak_memory_usage;
unsigned long int sbrk_calls;
extern bool mylloc_initialized;

void print_stats(void);
void print_final_stats(void);

#ifdef __GNUC__
#define MYLLOC_INIT __attribute__((constructor))
MYLLOC_INIT void mylloc_init(void);
#endif


#endif //MYLLOC_MYLLOC_H
