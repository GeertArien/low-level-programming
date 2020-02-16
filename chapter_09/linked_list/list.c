#include "list.h"
#include <malloc.h>

struct list_t* list_create(const int val) {
    struct list_t* list = (struct list_t*) malloc(sizeof(struct list_t));
    list->value = val;
    list->next = NULL;
    return list;
}

struct list_t* list_back(struct list_t* list) {
	struct list_t* it = list;

	while (it->next != NULL) {
		it = it->next;
	}

	return it;
}

void list_add_front(const int value, struct list_t** list) {
	struct list_t* new_item = list_create(value);
	new_item->next = *list;
	*list = new_item;
}

void list_add_back(const int value, struct list_t** list) {
	struct list_t* back_item = list_back(*list);
	struct list_t* new_item = list_create(value);
	back_item->next = new_item;
}

struct list_t* list_node_at(struct list_t* list, const size_t index) {
    struct list_t* it = list;
    size_t i;

    for (i = 0; i < index; ++i) { 
        if (it == NULL) {
            return NULL;
        }
        it = it->next;
    }

    return it;
}

int list_get(struct list_t* list, const size_t index) {
    struct list_t* it = list_node_at(list, index);

    if (it == NULL) {
        return 0;
    }

    return it->value;
}

void list_free(struct list_t* list) {
    while (list != NULL) {
        struct list_t* next = list->next;
        free(list);
        list = next;
    }
}

size_t list_length(struct list_t* list) {
    struct list_t* it = list;
    size_t count = 0;

    while (it != NULL) {
        ++count;
        it = it->next;
    }

    return count;
}

int list_sum(struct list_t* list) {
    struct list_t* it = list;
    int sum = 0;

    while (it != NULL) {
        int val = it->value;
        sum += val;
        it = it->next;
    }

    return sum;
}
