#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>

typedef struct
{
    void* data;
    size_t size;
    size_t capacity;
    size_t elem_size;
} tVector;

int vector_init(tVector* v, size_t elem_size);
int vector_reserve(tVector* v, size_t new_capacity);
int vector_push_back(tVector* v, const void* elem);
void* vector_get(tVector* v, size_t index);
size_t vector_size(const tVector* v);
void vector_destroy(tVector* v);

#endif
