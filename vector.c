#include "vector.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

int vector_init(tVector* v, size_t elem_size)
{
    if (!v || elem_size == 0)
        return 0;

    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    v->elem_size = elem_size;

    v->data = malloc(v->capacity * elem_size);
    if (!v->data)
        return 0;

    return 1;
}

int vector_reserve(tVector* v, size_t new_capacity)
{
    if (!v || new_capacity <= v->capacity)
        return 1;

    void* newData = realloc(v->data, new_capacity * v->elem_size);
    if (!newData)
        return 0;

    v->data = newData;
    v->capacity = new_capacity;
    return 1;
}

int vector_push_back(tVector* v, const void* elem)
{
    if (!v || !elem)
        return 0;

    if (v->size >= v->capacity)
    {
        if (!vector_reserve(v, v->capacity * 2))
            return 0;
    }

    memcpy((char*)v->data + v->size * v->elem_size,
           elem,
           v->elem_size);

    v->size++;
    return 1;
}

void* vector_get(tVector* v, size_t index)
{
    if (!v || index >= v->size)
        return NULL;

    return (char*)v->data + index * v->elem_size;
}

size_t vector_size(const tVector* v)
{
    if (!v)
        return 0;

    return v->size;
}

void vector_destroy(tVector* v)
{
    if (!v)
        return;

    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}
