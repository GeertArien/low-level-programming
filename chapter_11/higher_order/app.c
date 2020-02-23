#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>
#include "list.h"
#include "algorithm.h"
#include "math.h"
#include "for_all.h"

static int* array_read(size_t* out_count) {
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

static struct list_t* create(const int* array, const size_t count) {
    struct list_t* list;
    size_t i;

    if (count == 0 || array == NULL) {
        return NULL;
    }

    list = list_create(*array);

    for (i = 1; i < count; ++i) {
        list_add_back(array[i], &list);
    }

    return list;
}

static void p_out_space(const int value) {
    printf("%d ", value);
}

static void p_out_newline(const int value) {
    printf("%d\n", value);
}

static int return_value(const int value) {
    return value;
}

static void foreach_test(struct list_t* list) {
    puts("List seperated by space:");
    for_each(list, &p_out_space);
    puts("\nList seperated by newline:");
    for_each(list, &p_out_newline);
}

static void map_test(struct list_t* list) {
    struct list_t* square_l;
    struct list_t* cube_l;

    puts("\nList squared:");
    square_l = map(list, &square);
    for_each(square_l, &p_out_space);
    puts("\nList cube:");
    cube_l = map(list, &cube);
    for_each(cube_l, &p_out_space);

    list_free(cube_l);
    list_free(square_l);
}

static void foldl_test(struct list_t* list) {
    int l_sum;
    int l_max;
    int l_min;

    l_sum = foldl(0, list, &sum);
    printf("\nsum: %d ", l_sum);
    l_min = foldl(INT_MAX, list, &min);
    printf("\nmin: %d ", l_min);
    l_max = foldl(INT_MIN, list, &max);
    printf("\nmax: %d ", l_max);
}

static void map_mut_test(struct list_t* list) {
    struct list_t* l_copy = map(list, &return_value);
    
    puts("\nList modules:");
    map_mut(l_copy, &abs);
    for_each(l_copy, &p_out_space);
    
    list_free(l_copy);
}

static void iterate_test(struct list_t* list) {
    struct list_t* powers;

    puts("\nPowers of two:");
    powers = iterate(1, 10u, &times_two);
    for_each(powers, &p_out_space);

    list_free(powers);
}

static void save_load_test(struct list_t* list) {
    struct list_t* l_read;

    if (!save(list, "list.txt")) {
        puts("Failed to save list as txt file.");
    }
    if (!load(&l_read, "list.txt")) {
        puts("Failed to load list from txt file.");
    }

    puts("\nList loaded from txt file:");
    for_each(l_read, &p_out_newline);

    free(l_read);
}

static void serialize_test(struct list_t* list) {
    struct list_t* l_read;

    if (!serialize(list, "list")) {
        puts("Failed to save list as bin file.");
    }
    if (!deserialize(&l_read, "list")) {
        puts("Failed to load list from bin file.");
    }

    puts("List loaded from bin file:");
    for_each(l_read, &p_out_newline);

    free(l_read);
}

int main(void) {
    int* array;
    size_t count;
    struct list_t* list;

    array = array_read(&count);
    list = create(array, count);

    foreach_test(list);
    map_test(list);
    foldl_test(list);
    map_mut_test(list);
    iterate_test(list);
    save_load_test(list);
    serialize_test(list);

    for_all_test(list);
    
    puts("");
    list_free(list);
    free(array);

    return 0;
}
