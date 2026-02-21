#include "vector.h"
#include "library.h"
#include <stdlib.h>
#include <string.h>

// funcion que inicializa el vector
int vector_init(tVector *v, size_t elem_size, size_t capacity) {
  v->data = malloc(capacity * elem_size); // reservo la memoria
  if (!v->data)
    return ERR;

  v->size = 0;
  v->capacity = capacity;
  v->elem_size = elem_size; // tam

  return OK;
}

// aca evaluo la capacidad del vector, adaptandolo
int vector_reserve(tVector *v, size_t new_capacity) {
  if (!v || new_capacity <= v->capacity)
    return ERR;

  void *newData = realloc(v->data, new_capacity * v->elem_size);
  if (!newData)
    return ERR;

  v->data = newData;
  v->capacity = new_capacity;
  return OK;
}

// evaluo y agrego elem al fnal del vector
int vector_push_back(tVector *v, const void *elem) {
  if (!v || !elem)
    return ERR;

  if (v->size >= v->capacity) {
    if (!vector_reserve(v, v->capacity * 2))
      return ERR;
  }

  void *fin = (char *)v->data + v->size * v->elem_size;
  memcpy(fin, elem, v->elem_size);

  v->size++;
  return OK;
}

// devuelve el elem en la pos index
void *vector_get(tVector *v, size_t index) {
  if (!v || index >= v->size)
    return NULL;

  return (char *)v->data + index * v->elem_size; // mov calc
}

// devuelve cuantos elem se cargaron
size_t vector_size(const tVector *v) {
  if (!v)
    return 0;

  return v->size;
}

// libera mem
void vector_destroy(tVector *v) {
  free(v->data);

  v->size = 0;
  v->capacity = 0;
  v->elem_size = 0;
}
