#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct Vector;

enum VECTOR_ALLOC_STRATEGY {
    VECTOR_ALLOC_STRATEGY_DOUBLE,
    VECTOR_ALLOC_STRATEGY_PLUS_ONE,
};

struct Vector *vector_new(size_t capacity, size_t element_size, enum VECTOR_ALLOC_STRATEGY alloc_strategy);

void vector_free(struct Vector *vector);

void *vector_get(struct Vector *vector, size_t index);

void vector_push_back(struct Vector *vector, void *element);

void vector_pop_back(struct Vector *vector);

size_t vector_size(struct Vector *vector);

void vector_insert(struct Vector *vector, size_t index, void *element);

void vector_remove(struct Vector *vector, size_t index);

void vector_clear(struct Vector *vector);

#endif  // VECTOR_H