

#ifndef MYLLOC_MYLLOC_H
#define MYLLOC_MYLLOC_H
#include <stddef.h>
#include <stdbool.h>
#include <threads.h>

void* mylloc(size_t size, const char * file, unsigned int line);
#define malloc(size) mylloc(size, __FILE__, __LINE__)

mtx_t mylloc_mutex;
size_t mylloc_total_allocated;
unsigned long int mylloc_total_allocations;
size_t mylloc_peak_memory_usage;
unsigned long int sbrk_calls;

void print_stats(void);
void print_final_stats(void);


#ifdef __GNUC__
#define MYLLOC_INIT __attribute__((constructor))
#endif
MYLLOC_INIT void mylloc_init(void);
bool mylloc_initialized = false;



#endif //MYLLOC_MYLLOC_H
