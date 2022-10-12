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

#define SIZE (1024 * 1024)

struct {
  void *bump_start;
  void *start;
  void *end;
}  heap = {0};

void* my_malloc(size_t size);
void my_free(void *ptr);
void* my_calloc(size_t nmemb, size_t size);
void* my_realloc(void *ptr, size_t size);
bool my_init(size_t size);
bool my_release();

void* my_get_start() { return heap.start; } // just returning a value


bool my_init(size_t size) {
  bool status = true;
  heap.bump_start = mmap(NULL, size, PROT_NONE | MAP_ANONYMOUS, -1, 0, 0);
  assert(heap.bump_start != MAP_FAILED);
  if (heap.bump_start == MAP_FAILED) {
     status = false;
  }
  heap.start = heap.bump_start;
  heap.end = heap.start + size;

  return status;
}



void init(size_t size) {
  heap.bump_start = mmap(NULL, size, PROT_NONE | MAP_ANONYMOUS, -1, 0, 0);
  assert(heap.bump_start != MAP_FAILED);
  if (heap.bump_start == MAP_FAILED) {
    printf("failure to allocate memory"); // avoid I/O with calc/function/computations
    exit(EXIT_FAILURE); // entire process exits
  }
  heap.start = heap.bump_start;
  heap.end = heap.start + size;
}
// trick question...it does nothing
void de_allocate(void* p){
  
}

void* allocate(size_t size) {
  void *newly_alloc = heap.bump_start;
  if (heap.bump_start + size > heap.end) {
    printf("cannot allocate-- size not available");
    return NULL;
  } else {
    newly_alloc = heap.bump_start + size;
  }

  return newly_alloc;
}
// void* re_allocate(void* p, size_t size) {
//   void* p1 = allocate(size);
//   memcpy(p,p1, sizeof(block-p);
// }

int main(int argc, char *argv[]) {
  // arrange
  my_release();
  size_t size = SIZE;
  // act
  bool b = my_init(size);
  // assert
  assert(b);
  
  // arrange
  my_release();
  size = SIZE * 2;
  // act
  my_init(size);
  // assert
  assert(my_get_start());  // TODO: private f()
  
  // assert(heap.end);
  // void *chunk = allocate(100);

  return 0;
}