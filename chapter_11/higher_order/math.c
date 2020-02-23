#include "math.h"

int square(const int value) {
    return value * value;
}

int cube(const int value) {
    return value * value * value;
}

int sum(const int lhs, const int rhs) {
    return lhs + rhs;
}

int min(const int lhs, const int rhs) {
    if (lhs < rhs) {
        return lhs;
    }
    else {
        return rhs;
    }
}

int max(const int lhs, const int rhs) {
    if (lhs > rhs) {
        return lhs;
    }
    else {
        return rhs;
    }
}

int times_two(const int value) {
    return value * 2;
}
