#include <stdlib.h>
#include <string.h>
#include "vector.h"

Vector * vector_create(int size)
{
    Vector *v;

    v = malloc(sizeof(Vector));
    if ( v == NULL ) return NULL;
    v->data = malloc(size);
    if ( v->data == NULL ) return NULL;
    v->item_size = size;
    v->size = 0;
    v->allocated = 1;
    return v;
}

int vector_clear(Vector *v)
{
    if ( v == NULL ) return -1;
    free ( v->data );
    v->data = malloc(v->item_size);
    if ( v->data == NULL ) return -1;
    v->size = 0;
    v->allocated = 1;
    return 0;
}

int vector_pushback(Vector *v, void *d)
{
    if ( v == NULL ) return -1;
    while ( v->allocated <= v->size ) {
        v->allocated *= 2;
        v->data = realloc(v->data,v->allocated*v->item_size);
    }
    memcpy ( (void *)(v->data+v->size*v->item_size), d, v->item_size );
    v->size++;
    return 0;
}

int vector_popback(Vector *v, void *d)
{
    if ( v == NULL ) return -1;
    if ( v->size <= 0 ) return -1;
    memcpy ( d, (void *)(v->data+(v->size-1)*v->item_size), v->item_size );
    v->size--;
    return 0;
}

int vector_getback(Vector *v, void *d)
{
    if ( v == NULL ) return -1;
    if ( v->size < 1 ) return -1;
    memcpy ( d, (void *)(v->data+(v->size-1)*v->item_size), v->item_size);
    return 0;
}

int vector_setback(Vector *v, void *d)
{
    if ( v == NULL ) return -1;
    if ( v->size < 1 ) return -1;
    memcpy ( (void *)(v->data+(v->size-1)*v->item_size), d, v->item_size );
    return 0;
}

int vector_set(Vector *v, int i, void *d)
{
    if ( v == NULL ) return -1;
    if ( i < 0 || i >= v->size ) return -1;
    memcpy ( (void *)(v->data+i*v->item_size), d, v->item_size );
    return 0;
}

int vector_get(Vector *v, int i, void *d)
{
    if ( v == NULL ) return -1;
    if ( i < 0 || i >= v->size ) return -1;
    memcpy ( d, (void *)(v->data+i*v->item_size), v->item_size );
    return 0;
}

int vector_reserve(Vector *v, int size)
{
    if ( v == NULL ) return -1;
    if ( size < 1 || size < v->allocated ) return -1;
    v->allocated = size;
    v->data = realloc(v->data,v->allocated*v->item_size);
    return 0;
}
