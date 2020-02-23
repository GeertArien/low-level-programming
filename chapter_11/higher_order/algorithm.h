#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "list.h"

typedef void (foreach_t) (const int);
typedef int (map_t) (const int);
typedef int (foldl_t) (const int, const int);
typedef int (iter_t) (const int);

void for_each(struct list_t* list, foreach_t* func);

struct list_t* map(struct list_t* list, map_t* func);

void map_mut(struct list_t* list, map_t* func);

int foldl(int start_value, struct list_t* list, foldl_t* func);

struct list_t* iterate(const int value, const size_t length, iter_t* func);

#endif