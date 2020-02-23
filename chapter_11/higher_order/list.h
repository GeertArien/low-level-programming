#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdbool.h>

struct list_t {
    int value;
    struct list_t* next;
};

struct list_t* list_create(const int val);

struct list_t* list_back(struct list_t* list);

void list_add_front(const int value, struct list_t** list);
void list_add_back(const int value, struct list_t** list);

struct list_t* list_node_at(struct list_t* list, const size_t index);
int list_get(struct list_t* list, const size_t index);

void list_free(struct list_t* list);

size_t list_length(struct list_t* list);
int list_sum(struct list_t* list);

bool save(struct list_t* lst, const char* filename);
bool load(struct list_t** lst, const char* filename);

bool serialize(struct list_t* lst, const char* filename);
bool deserialize(struct list_t** lst, const char* filename);

#endif