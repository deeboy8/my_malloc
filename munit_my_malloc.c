#include <stdlib.h>
#include "munit.h"
#include "src/my_malloc.c"

#define PAYLOAD_SIZE 16
#define HEADER_SIZE 8

static MunitResult heap_initialized_successfully(const MunitParameter params[], void *user_data) {
    (void)params;
    (void)user_data;
    // arrange
    size_t size = SIZE;
    // act(ion)
    bool success = INIT(size);
    // assert
    munit_assert_true(success);

    return MUNIT_OK;
}

static MunitResult heap_initialized_should_have_1_free_block(const MunitParameter params[], void *user_data)
{
    (void)params;
    (void)user_data;
    // arrange
    size_t size = SIZE;
    bool success = INIT(size);
    if (!success)
    {
        return MUNIT_FAIL;
    }
    // act(ion)
    // SUT = subject under test = num_free_block
    size_t num_free_blocks = _num_free_blocks_bump();
    // assert
    munit_assert_size(num_free_blocks, ==, 1);
    munit_assert_size(num_free_blocks, ==, bump_heap.block_count.num_free_blocks);

    return MUNIT_OK;
}

static MunitResult heap_initialized_should_have_0_alloc_block(const MunitParameter params[], void *user_data)
{
    (void)params;
    (void)user_data;
    // arrange
    size_t size = SIZE;
    bool success = INIT(size);
    if (!success) {
        return MUNIT_FAIL;
    }
    // act(ion)
    // SUT = subject under test
    size_t num_alloc_block = _num_alloc_blocks_bump();
    // assert
    munit_assert_size(num_alloc_block, ==, 0);
    munit_assert_size(num_alloc_block, ==, bump_heap.block_count.num_alloc_blocks);

    return MUNIT_OK;
}

static MunitResult heap_catches_inappropriately_initialized_heap(const MunitParameter params[], void *user_data)
{ // clean up
    (void)params;
    (void)user_data;
    // arrange
    size_t size = SIZE;
    size_t incorrect_size = SIZE_128;
    bool incorrect_success = INIT(SIZE_128); 
    /*if (!success)
    {
        return MUNIT_FAIL;
    }*/
    // act(ion)
    // SUT = subject under test
    bool success = INIT(size);
    // assert
    munit_assert_size(success, !=, incorrect_size);
    

    return MUNIT_OK;
}

static MunitResult successful_allocation_within_designated_heap_space(const MunitParameter params[], void* user_data) {
    (void) params;
    (void) user_data;

    // arrange
    bool success = INIT(SIZE);
    assert(success);
    void* result = NULL;
    void* start = bump_heap.heap_start;
    void* end = bump_heap.heap_start + SIZE_128; 
    // act(ion)
    result = MALLOC(SIZE_128);        
    bool status = (result >= start && result < end);
    munit_assert_true(status);

    return MUNIT_OK;
}

// determine if size returned from my_malloc() is correct
static MunitResult my_malloc_bump_payload_size_accurate(const MunitParameter params[], void* user_data) {
    (void) params;
    (void) user_data;

    // arrange
    void* start = NULL;
    void* end = NULL;
    size_t bump_payload;
    bool success = INIT(SIZE);
    assert(success);
    bool result = MALLOC(SIZE_128);
    assert(result);
    // act(ion)
    start = get_start_of_block();
    end = start + SIZE_128;
    bump_payload = end - start;
    // assert
    munit_assert_size(bump_payload, ==, SIZE_128);

    return MUNIT_OK;
}

static MunitResult size_request_bigger_than_heap_initialized(const MunitParameter params[], void* user_data) {
    (void) params;
    (void) user_data;

    // arrange
    void* result = NULL;
    bool success = INIT(SIZE);
    assert(success);
    // act(ion)
    result = MALLOC(OVERLOAD);
    // assert
    munit_assert_null(result);

    return MUNIT_OK;
}

static MunitResult size_request_is_negative(const MunitParameter params[], void* user_data) {
    (void) params;
    (void) user_data;

    // arrange
    void* result = NULL;
    bool success = INIT(SIZE);
    assert(success);
    // act(ion)
    result = MALLOC(128);
    // assert
    munit_assert_null(result);

    return MUNIT_OK;
}

static MunitResult is_malloc_ptr_valid(const MunitParameter params[], void* user_data) {
    (void) params;
    (void) user_data;

    // arrange
    int size = (-64);
    void* result = NULL;
    bool success = INIT(SIZE);
    assert(success);
    // act(ion)
    result = MALLOC(size);
    assert(result);
    bool valid = _is_valid_ptr(result);
    // assert
    munit_assert_true(valid);

    return MUNIT_OK;
}



/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
    {
        (char *)"heap_initialized_successfully",   // type of test name
        heap_initialized_successfully, // name of fx
        NULL,                          // setup fx
        NULL,                          // teardown fx
        MUNIT_TEST_OPTION_NONE,        // bitmask option
        NULL                           // required terminating NULL
    },
    {
        (char *)"heap_catches_inappropriately_initialized_heap,",   // type of test name
        heap_catches_inappropriately_initialized_heap, // name of fx
        NULL,                          // setup fx
        NULL,                          // teardown fx
        MUNIT_TEST_OPTION_NONE,        // bitmask option
        NULL                           // required terminating NULL
    },
    
    {
        (char *)"heap_initialized_should_have_1_free_block",                    // type of test name
        heap_initialized_should_have_1_free_block, // name of fx
        NULL,                                      // setup fx
        NULL,                                
              // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
     {
        (char *)"heap_initialized_should_have_0_alloc_block",                    // type of test name
        heap_initialized_should_have_0_alloc_block, // name of fx
        NULL,                                      // setup fx
        NULL,                                      // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
    {
        (char *)"successful_allocation_within_designated_heap_space",                    // type of test name
        successful_allocation_within_designated_heap_space, // name of fx
        NULL,                                      // setup fx
        NULL,                                      // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
     {
        (char *)"my_malloc_bump_payload_size_accurate",                    // type of test name
        my_malloc_bump_payload_size_accurate, // name of fx
        NULL,                                      // setup fx
        NULL,                                      // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
    {
        (char *)"size_request_bigger_than_heap_initialized",                    // type of test name
        size_request_bigger_than_heap_initialized, // name of fx
        NULL,                                      // setup fx
        NULL,                                      // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
    {
        (char *)"size_request_is_negative",                    // type of test name
        size_request_bigger_than_heap_initialized, // name of fx
        NULL,                                      // setup fx
        NULL,                                      // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
    {
        (char *)"is_malloc_ptr_valid",                    // type of test name
        size_request_bigger_than_heap_initialized, // name of fx
        NULL,                                      // setup fx
        NULL,                                      // teardown fx
        MUNIT_TEST_OPTION_NONE,                    // bitmask option
        NULL                                       // required terminating NULL
    },
   {NULL}
};

static const MunitSuite test_suite = {
    (char *)"malloc unit tests/",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
    return munit_suite_main(&test_suite, (void *)NULL, argc, argv); // what is "unit" as the default provided?
}