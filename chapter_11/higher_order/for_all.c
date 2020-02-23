#include "for_all.h"
#include <limits.h>
#include <stdio.h>

static void for_all(struct list_t* list, for_all_t* func, void** context) {
    struct list_t* it = list;

	while (it != NULL) {
        func(it->value, context);
		it = it->next;
	}
}

static void compute_metrics(const int value, void** context) {
	int* sum_p = (int*) context[0];
    int* max_p = (int*) context[1];
    int* min_p = (int*) context[2];

	*sum_p += value;
	
	if (value > *max_p) {
		*max_p = value;
	}

	if (value < *min_p) {
		*min_p = value;
	}
}

void for_all_test(struct list_t* list) {
    int l_sum = 0;
    int l_max = INT_MIN;
    int l_min = INT_MAX;

	void* context[] = { &l_sum, &l_max, &l_min };

	puts("For all with context test:");
    for_all(list, &compute_metrics, context);
	printf("sum: %d ", l_sum);
    printf("\nmin: %d ", l_min);
    printf("\nmax: %d ", l_max);
}

