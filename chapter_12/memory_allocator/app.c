#include <string.h>
#include <stdint.h>
#include "mem.h"

int main(void) {
    char* str;
    uint8_t* data;
    size_t i;

    heap_init(50);
    
    memalloc_debug_heap(stderr);
    
    str = _malloc(200);

    if (str == NULL) {
        perror("Failed to allocate");
        return 1;
    }

    memalloc_debug_heap(stderr);
 
    strcpy(str, "Hello there");

    memalloc_debug_heap(stderr);
 
    printf("Contents of str: %s\n", str);

    data = _malloc(30);

    if (data == NULL) {
        perror("Failed to allocate");
        return 1;
    }

    memalloc_debug_heap(stderr);

    for (i = 0; i < 30; ++i) {
        data[i] = (uint8_t) i;
    }

    memalloc_debug_heap(stderr);

    _free(str);

    memalloc_debug_heap(stderr);

    _free(data);

    memalloc_debug_heap(stderr);

    return 0;
}