#ifndef _MEM_H_
#define _MEM_H_

#include <sys/mman.h>
#include <stdbool.h>
#include <stdio.h>

#define HEAP_START  ((void*)0x04040000)
#define BLOCK_MIN_SIZE  4
#define ALLOC_MIN_SIZE  64

struct mem;

#pragma pack(push, 1)
struct mem {
    struct mem* next;
    size_t capacity;
    bool is_free;
};
#pragma pack(pop)

void* _malloc(size_t query);
void _free(void* mem);
void* heap_init(size_t initial_size);

#define DEBUG_FIRST_BYTES 4

void memalloc_debug_heap(FILE* f);

#endif