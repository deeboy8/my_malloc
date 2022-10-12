// #define _CRT_
// #define _DWB_

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#define OVERLOAD (4096 * 4096)
#define SIZE_128 (128) // test request size
#define SIZE (1024 * 1024) // tweak multiple of to 4 or 8 bytes // megabyte
#define META_SIZE sizeof(block_meta)
#define TEST 256


// private functions to interface with malloc family of functions to monitor behavior
bool _my_init_bump(size_t size);
bool _my_init(size_t size);
bool _my_init(size_t size);
size_t _num_free_blocks(); 
size_t _num_alloc_blocks();

bool _is_valid_block(const void* ptr);  // mmap() returned (0) <= ptr <= ptr + sizeof_of_heap - 1
bool _is_in_payload(const void* ptr); // checking to see if ptr is in a block and not a header

// malloc family of functions
void* my_malloc_bump(size_t size_request);
void* my_malloc_implicit(size_t size_request);
void* my_malloc_explicit(size_t size_request);
void my_free(void *ptr);
void* my_realloc(void *ptr, size_t size);
void* my_calloc(size_t nmemb, size_t size);

// will write all test whenever we want to use a specivic malloc we'll just modify the define
// conditional complilation
#define BUMP_ALLOC 
#if defined(BUMP_ALLOC)
    #define INIT(n) _my_init_bump(n)
    #define MALLOC(n) my_malloc_bump(n)
    #define NUM_ALLOC_BUMP() _num_alloc_blocks_bump()
    #define NUM_FREE_BUMP() _num_free_blocks_bump()
#elif defined(IMPLICIT_ALLOC)
    #define INIT(n) _my_init_implicit(n)
    #define MALLOC(n) my_malloc_implicit(n)
#elif defined(EXPLICIT_ALLOC)
    #define MALLOC(n) my_malloc_explicit(n)
#else
#include <stdio.h>
    #pragma message "no allocator defined" // look up pragma; cppref pragma #pragma [compiler specific]; instruction to compiler
#endif  // godbolt: prepocessor -> E/P/EP

// monitor count of free and alloc blocks
typedef struct {
    size_t num_free_blocks;
    size_t num_alloc_blocks;
} block_count_t;

// struct for monitoring block returned from mmap()
typedef struct {
    void* heap_start; // remains at the very beginning of heap
    void* start; // returned from mmap()
    void* end; // end of each allocated block from my_malloc() call
    block_count_t block_count;
} bump_heap_t;

typedef struct {
    size_t current_heap_size;
} heap_size_t;

// // global variables
// block_count_t* global_block_count = NULL; // ptr to struct monitoring count of blocks
// bump_heap_t global_chunk; // establishment of mmap() returned heap on stack

static bump_heap_t bump_heap = {NULL, NULL, NULL, {0, 0}}; // statics initilized by default????? NULL = (void *)0
heap_size_t _heap_size = {0};

// track number of free blocks
size_t _num_free_blocks_bump() {
    return bump_heap.block_count.num_free_blocks;
}

// track number of free blocks
size_t _num_alloc_blocks_bump() {
    return bump_heap.block_count.num_alloc_blocks;
}

void* get_start_of_heap() {
    return bump_heap.heap_start;
}

void* get_start_of_block() {
    return bump_heap.start;
}

// private function to access values in heap struct
// want to avoid "dipping" directly into struct --> corruption
void* get_end_of_heap() {
    return bump_heap.end;
}

// ensure ptr returned is located within initialized heap space
bool _is_valid_ptr(const void* ptr) {     // what my_malloc returned // is this ptr address located INSIDE the heap // this will be any ptr returned from malloc
    return (ptr > bump_heap.heap_start && ptr < (bump_heap.heap_start + SIZE)) ? true : false;
}

// determine if space is availabe to satisfy size request
bool is_size_available(size_t size) {
    bool status = false;
    
    // calculate available space remaining in heap
    size_t available = (bump_heap.start + size) - bump_heap.end; 
    // private --> maintain count of remaining heap size left over for UT
    _heap_size.current_heap_size = available;
    
    // if no allocations occurred or have requested space available
    if (bump_heap.start == bump_heap.end || available > size) {
        status = true;
    }

    return status;
}

void _my_duinit() {
    munmap(bump_heap.heap_start, SIZE); // for UT purposes
}

// initialize heap
bool _my_init_bump(size_t size) {
    bool status = true;
    bump_heap.start = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    
    assert(bump_heap.start != MAP_FAILED); // debugging purposes/ development
    if (bump_heap.start == MAP_FAILED) { // production code
        perror("mmap error");
        status = false;
    } else {
        bump_heap.end = bump_heap.heap_start = bump_heap.start;
        bump_heap.block_count.num_free_blocks = 1;
    }
    
    // unmap heap for each new UT --> UT ptrs were pointing at an old heap from the previous UT
    _my_duinit();

    return status;
}

// return requested bytes to client from bump allocator
void* my_malloc_bump(size_t size_request) {
    if (size_request <= 0 || size_request > SIZE) {
        return NULL;
    }
    bool available = is_size_available(size_request);
    if (!available) {
        assert(!available);
        return NULL;
    }
    
    void* return_ptr = bump_heap.end;
    assert(return_ptr == bump_heap.end);
    bump_heap.end = return_ptr + size_request;
    assert(bump_heap.end = return_ptr + size_request);
    bump_heap.block_count.num_alloc_blocks++;
    assert(bump_heap.block_count.num_alloc_blocks++);

    return return_ptr;
}

// int main(int argc, char *argv[]) {
//     assert(argc);
//     assert(argv);
  
//     bool chunk = my_init(SIZE);
//     assert(chunk);
   

//     void* new_block = my_malloc(10);
//     assert(new_block);


//     // call malloc to allocate size of requested space if available


//     // assert(heap.end);
//     // void *chunk = allocate(100);

//     return 0;
// }