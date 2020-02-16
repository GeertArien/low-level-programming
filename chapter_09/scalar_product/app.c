#include <stdio.h>

const int array0[] = { 1, 2, 3, 4, 5};
const int array1[] = { 3, 5, 6, 1, 0};

int scalar_product(const int* array0, const int* array1, const size_t count) {
    int i = 0;
    int sum = 0;
    
    if (!array0 || !array1) {
        return sum;
    }

    for (; i < count; ++i) {
        sum += array0[i] * array1[i]; 
    }

    return sum;
}

int main(int argc, char** argv) {
    const size_t arr_len = sizeof(array0) / sizeof(int);

    const int sum = scalar_product(array0, array1, arr_len);

    printf("Scalar produt of array0 and array1: %d\n", sum);

    return 0;
}