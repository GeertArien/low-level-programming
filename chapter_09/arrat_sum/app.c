#include <stdio.h>

const int array[] = { 1, 2, 3, 4, 5};

int array_sum(const int* array, const size_t count) {
    int i = 0;
    int sum = 0;
    
    if (!array) {
        return sum;
    }

    for (; i < count; ++i) {
        sum += array[i]; 
    }

    return sum;
}

int main(int argc, char** argv) {
    const size_t arr_len = sizeof(array) / sizeof(int);

    const int sum = array_sum(array, arr_len);

    printf("Sum of array: %d\n", sum);

    return 0;
}