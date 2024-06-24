//
// Created by bartek on 14.01.24.
//
#include <stdio.h>
#include "mylloc.h"
#include <CUnit/Basic.h>
#include "testfunc.h"
#include "memblock.h"

void test_mylloc() {
    void * ptr = mylloc(10);
    CU_ASSERT_PTR_NOT_NULL(ptr);
    myfree(ptr);
}

void test_mylloc2() {
    void * ptr = mylloc(12);
    CU_ASSERT(mylloc_current_memory_usage == 12);
    void * ptr2 = mylloc(15);
    CU_ASSERT(mylloc_current_memory_usage == 27);
    void * ptr3 = mylloc(20);
    CU_ASSERT(mylloc_current_memory_usage == 47);
    int * x = ptr;
    x[0] = 0x10;
    x[1] = 0x20;
    x[2] = 0x30;
    CU_ASSERT(x[0] == 0x10);
    CU_ASSERT(x[1] == 0x20);
    CU_ASSERT(x[2] == 0x30);
    print_blocks();
    myfree(ptr);
    myfree(ptr2);
    myfree(ptr3);
}

void test_myfree()
{
    void * ptr = mylloc(10);
    void * ptr2 = mylloc(15);
    void * ptr3 = mylloc(20);
    CU_ASSERT(occupied_blocks() == 3);
    myfree(ptr);
    CU_ASSERT(occupied_blocks() == 2);
    myfree(ptr2);
    CU_ASSERT(occupied_blocks() == 1);
    myfree(ptr3);
    CU_ASSERT(occupied_blocks() == 0);
    CU_ASSERT(count_blocks() == 0);
}


void test_mylloc3()
{
    void * ptr = mylloc(10);
    void * ptr2 = mylloc(100);
    void * ptr3 = mylloc(20);
    myfree(ptr2);

    void * ptr4 = mylloc(50);

    CU_ASSERT(occupied_blocks() == 3);
    CU_ASSERT(count_blocks() == 3);

    myfree(ptr);
    if (is_valid_block((memblock *) (ptr4 - sizeof(memblock)))) {
        printf("ptr4 is valid\n");
    }
    myfree(ptr3);
    if (is_valid_block((memblock *) (ptr4 - sizeof(memblock)))) {
        printf("ptr4 is valid\n");
    }
    myfree(ptr4);
}

void test_mylloc_split()
{
    void * ptr = mylloc(10);
    void * ptr2 = mylloc(200);
    void * ptr3 = mylloc(20);
    myfree(ptr2);

    void * ptr4 = mylloc(50);

    CU_ASSERT(occupied_blocks() == 3);
    CU_ASSERT(count_blocks() == 4);

    myfree(ptr);
    if (is_valid_block((memblock *) (ptr4 - sizeof(memblock)))) {
        printf("ptr4 is valid\n");
    }
    myfree(ptr3);
    if (is_valid_block((memblock *) (ptr4 - sizeof(memblock)))) {
        printf("ptr4 is valid\n");
    }
    myfree(ptr4);
}

void test_block_check()
{
    unsigned int * ptr = mylloc(2);
    unsigned int * ptr2 = mylloc(20);
    print_blocks();
    ptr[4] = 0xDEADBEEF;    // overwrite the next block's structure
    print_blocks();

    CU_ASSERT(!is_valid_block((memblock *) (ptr2 - sizeof(memblock))));
}


int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("mylloc", NULL, NULL);
    CU_add_test(suite, "test_mylloc", test_mylloc);
    CU_add_test(suite, "test_mylloc2", test_mylloc2);
    CU_add_test(suite, "test_myfree", test_myfree);
    CU_add_test(suite, "test_mylloc3", test_mylloc3);
    CU_add_test(suite, "test_mylloc_split", test_mylloc_split);
    CU_add_test(suite, "test_block_check", test_block_check); // must be done at the very end
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;   //if test_block_check is performed, it will write about some errors but this is the intended behaviour
}

