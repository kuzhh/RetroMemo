#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "library.h"
#include <stddef.h>

typedef struct {
  void *data;       // Puntero al bloque de memoria
  size_t capacity;  // Capacidad maxima actual del vector
  size_t size;      // Cantidad de elementos cargados
  size_t elem_size; // Tam en bytes de cada elemento
} tVector;

int vector_init(tVector *v, size_t elem_size, size_t capacity);
int vector_reserve(tVector *v, size_t new_capacity);
int vector_push_back(tVector *v, const void *elem);
void *vector_get(tVector *v, size_t index);
size_t vector_size(const tVector *v);
void vector_destroy(tVector *v);

#endif
