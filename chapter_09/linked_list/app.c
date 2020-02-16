#include <stdio.h>
#include <malloc.h>
#include "list.h"

int* array_read(size_t* out_count) {
    int* array;
    size_t count;
    size_t i;

    puts("Enter length:");
    if (scanf("%zu", &count) != 1) {
        puts("Invalid input.");
        count = 0;
    }
    
    array = (int*) malloc(count * sizeof(int));

    puts("Enter numbers:");
    for (i = 0; i < count; ++i) {
        scanf("%d", &array[i]);
    }

    *out_count = count;
    return array;
}

struct list_t* create(const int* array, const size_t count) {
    struct list_t* list;
    size_t i;

    if (count == 0 || array == NULL) {
        return NULL;
    }

    list = list_create(*array);

    for (i = 1; i < count; ++i) {
        list_add_front(array[i], &list);
    }

    return list;
}

int main(void) {
    int* array;
    size_t count;
    struct list_t* list;
    int sum;
    size_t index;

    array = array_read(&count);
    list = create(array, count);

    sum = list_sum(list);

    printf("Sum is %d\n", sum);
    printf("Lenght is: %zu\n", list_length(list));

    puts("Show content of node:");
    scanf("%zu", &index);

    if (index < list_length(list)) {
        printf("Node at %zu is %d\n", index, list_get(list, index));
    }
    else {
        puts("Index out of bounds.");
    }

    list_free(list);
    free(array);

    return 0;
}
