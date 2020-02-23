#include "algorithm.h"

void for_each(struct list_t* list, foreach_t* func) {
    struct list_t* it = list;

	while (it != NULL) {
        func(it->value);
		it = it->next;
	}
}

struct list_t* map(struct list_t* list, map_t* func) {
    struct list_t* it = list;
    struct list_t* n_list;

    if (!it) {
        return NULL;
    }

    n_list = list_create(func(it->value));
    it = it->next;

    while (it != NULL) {
        list_add_back(func(it->value), &n_list);
		it = it->next;
	}

    return n_list;
}

void map_mut(struct list_t* list, map_t* func) {
    struct list_t* it = list;

	while (it != NULL) {
        it->value = func(it->value);
		it = it->next;
	}
}

int foldl(const int start_value, struct list_t* list, foldl_t* func) {
    struct list_t* it = list;
    int result = start_value;

    while (it != NULL) {
        result = func(it->value, result);
		it = it->next;
	}

    return result;
}

struct list_t* iterate(const int value, const size_t length, iter_t* func) {
    struct list_t* list;
    struct list_t* it;
    size_t i;

    if (length < 1) {
        return NULL;
    }

    list = list_create(value);
    it = list;

    for (i = 1; i < length; ++i) {
        list_add_back(func(it->value), &it);
		it = it->next;
	}

    return list;
}
