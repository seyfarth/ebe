#ifndef DICT_H
#define DICT_H

#include "vector.h"

typedef struct DictEntryStruct
{
    Vector *status;
    Vector *keys;
    Vector *data;
} Entry;

typedef struct DictStruct
{
    int item_size;
    int table_size;
    int items_in_use;
    Entry *table;
} Dict;

Dict * dict_create(int *size);
int dict_clear(Dict *v);
int dict_set(Dict *v, char *s, void *d);
int dict_get(Dict *v, char *s, void *d);
int dict_setsize(Dict *v, int *size);

#endif
