#ifndef FORALL_H_
#define FORALL_H_

#include "list.h"

typedef void (for_all_t) (const int, void**);

void for_all_test(struct list_t* list);

#endif