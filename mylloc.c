#include "mylloc.h"

#include <stdio.h>

void mylloc_init(void)
{
  printf("mylloc_init()\n");
}

void * mylloc(size_t size) {
  printf("mylloc(%d)\n", size);
}
