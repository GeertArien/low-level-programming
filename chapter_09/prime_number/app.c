#include <stdio.h>

int is_prime(unsigned long value) {
    unsigned long div;

    if (value < 2) {
        return 0;
    }

    for (div = 2; div < value; ++div) {
        unsigned long t = value % div;

        if (value % div == 0ul) {
            return 0;
        }
    }
    
    return 1;
}

unsigned long read_long() {
    unsigned long value;
    scanf("%lu", &value);

    return value;
}

int main(int argc, char** argv) {
    char* answer;
    const unsigned long value = read_long();

    if (is_prime(value)) {
        answer = "yes";
    }
    else {
        answer = "no";
    }

    printf("%d is prime? %s\n", value, answer);

    return 0;
}