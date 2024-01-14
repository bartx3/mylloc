//
// Created by bartek on 14.01.24.
//
#include <stdio.h>
#include "mylloc.h"
#include "CUnit/Basic.h"

void test_mylloc() {
    void * ptr = mylloc(10);
    CU_ASSERT_PTR_NOT_NULL(ptr);
    myfree(ptr);
}

void test_mylloc2() {
    void * ptr = mylloc(12);
    void * ptr2 = mylloc(15);
    void * ptr3 = mylloc(20);
    int * x = ptr;
    x[0] = 0x10;
    x[1] = 0x20;
    x[2] = 0x30;
    CU_ASSERT_PTR_NOT_NULL(ptr);
    CU_ASSERT(x[0] == 0x10);
    CU_ASSERT(x[1] == 0x20);
    CU_ASSERT(x[2] == 0x30);
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
}



int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("mylloc", NULL, NULL);
    CU_add_test(suite, "test_mylloc", test_mylloc);
    CU_add_test(suite, "test_mylloc2", test_mylloc2);
    CU_add_test(suite, "test_myfree", test_myfree);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}

