#include "mem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static struct mem* mem_start = NULL;

static void* to_region_start(void* addr) {
    return addr + sizeof(struct mem);
}

static struct mem* map_memory(size_t size, void* start) {
    const int prot = PROT_READ | PROT_WRITE;
    struct mem* memory;
    int fd = open("/dev/zero", O_RDWR);

    if (size < ALLOC_MIN_SIZE) {
        size = ALLOC_MIN_SIZE;
    }
    
    memory = mmap(start, size, prot, MAP_PRIVATE | MAP_FIXED, fd, 0);

    if (memory == MAP_FAILED) {
        memory = mmap(NULL, size, prot, MAP_PRIVATE, fd, 0);
    }

    close(fd);

    if (memory == MAP_FAILED) {
        return NULL;
    }

    memory->next = NULL;
    memory->capacity = size;
    memory->is_free = true;

    return memory;
}

void* _malloc(size_t query) {
    struct mem* memory;
    struct mem* last = NULL;
    struct mem* next;
    size_t required_length;
    
    if (query < BLOCK_MIN_SIZE) {
        query = BLOCK_MIN_SIZE;
    }

    required_length = query + sizeof(struct mem) + BLOCK_MIN_SIZE;

    for(memory = mem_start; memory != NULL; memory = memory->next) {
        if (required_length <= memory->capacity) {
            break;
        }

        last = memory;
    }

    if (memory == NULL) {
        memory = map_memory(required_length, (void*)last + sizeof(struct mem) + last->capacity);
        last->next = memory;
    }

    if (memory == NULL) {
        return NULL;
    }
    
    //split in two, return first
    next = memory->next;
    memory->next = to_region_start(memory) + query;
    memory->next->next = next;
    memory->next->capacity = memory->capacity - query - sizeof(struct mem);
    memory->next->is_free = true;

    memory->capacity = query;
    memory->is_free = false;

    return to_region_start(memory);
}

void _free(void* mem) {
    struct mem* memory = mem - sizeof(struct mem);

    memory->is_free = true;

    if (memory->next != NULL && 
        memory->next->is_free && 
        memory->next == (void*)memory + sizeof(struct mem) + memory->capacity) 
    {
        memory->capacity += sizeof(struct mem) + memory->next->capacity;
        memory->next = memory->next->next;
    }
}


void* heap_init(size_t initial_size) {
    mem_start = map_memory(initial_size, HEAP_START);

    if (mem_start == NULL) {
        return NULL;
    }

    return to_region_start(mem_start);
}

static void memalloc_debug_struct_info(FILE* f, struct mem const* const address) {
    size_t i;

    fprintf(f, "start: %p\nsize: %lu\nis_free: %d\n",
            (void*) address, 
            address->capacity,
            address->is_free);

    for(i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i) {
        fprintf(f, "%hhX", ((char*) address)[sizeof(struct mem) + i]);
    }

    putc('\n', f);
}

void memalloc_debug_heap(FILE* f) {
    const struct mem* ptr = mem_start;

    fputs("---------------\nDebugging heap\n---------------\n", f);

    for(; ptr; ptr = ptr->next) {
        memalloc_debug_struct_info(f, ptr);
    }
}
