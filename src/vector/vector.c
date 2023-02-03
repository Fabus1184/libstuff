#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

enum VECTOR_ALLOC_STRATEGY {
    VECTOR_ALLOC_STRATEGY_DOUBLE,
    VECTOR_ALLOC_STRATEGY_PLUS_ONE,
};

struct Vector {
    size_t size;
    size_t capacity;
    void *data;
    size_t element_size;
    enum VECTOR_ALLOC_STRATEGY alloc_strategy;
};

/**
 * Creates a new vector.
 * @param capacity The initial capacity of the vector.
 * @param element_size The size of the elements in the vector.
 * @return The vector.
 */
struct Vector *vector_new(size_t capacity, size_t element_size, enum VECTOR_ALLOC_STRATEGY alloc_strategy) {
    struct Vector *vector = malloc(sizeof(struct Vector));
    assert(vector != NULL && "Could not allocate memory for vector");
    vector->size = 0;
    vector->capacity = capacity;
    vector->data = calloc(capacity, element_size);
    assert(vector->data != NULL && "Could not allocate memory for vector data");
    vector->element_size = element_size;
    vector->alloc_strategy = alloc_strategy;
    return vector;
}

/**
 * Frees the memory allocated for the vector.
 * @param vector The vector to free.
 */
void vector_free(struct Vector *vector) {
    if (vector != NULL) {
        free(vector->data);
        free(vector);
    }
}

void vector_enlarge(struct Vector *vector) {
    switch (vector->alloc_strategy) {
        case VECTOR_ALLOC_STRATEGY_DOUBLE:
            vector->capacity *= 2;
            break;
        case VECTOR_ALLOC_STRATEGY_PLUS_ONE:
            vector->capacity++;
            break;
    }
    vector->data = realloc(vector->data, vector->capacity * vector->element_size);
    assert(vector->data != NULL && "Could not reallocate memory for vector data");
}

void vector_shrink(struct Vector *vector) {
    switch (vector->alloc_strategy) {
        case VECTOR_ALLOC_STRATEGY_DOUBLE:
            if (vector->size < vector->capacity / 2) {
                vector->capacity /= 2;
            } else {
                return;
            }
            break;
        case VECTOR_ALLOC_STRATEGY_PLUS_ONE:
            if (vector->size < vector->capacity - 1) {
                vector->capacity--;
            } else {
                return;
            }
            break;
    }
    vector->data = realloc(vector->data, vector->capacity * vector->element_size);
    assert(vector->data != NULL && "Could not reallocate memory for vector data");
}

/**
 * Gets the element at the given index or NULL if the index is out of bounds.
 * @param vector The vector.
 * @param index The index.
 * @return Pointer to the element or NULL.
 * @note The pointer is only valid until the next operation on the vector.
 */
void *vector_get(struct Vector *vector, size_t index) {
    if (index >= vector->size) {
        return NULL;
    } else {
        return vector->data + index * vector->element_size;
    }
}

/**
 * Pushes an element to the back of the vector.
 * @param vector The vector.
 * @param element The element to push.
 * @return The vector.
 */
void vector_push_back(struct Vector *vector, void *element) {
    if (vector->size == vector->capacity) {
        vector_enlarge(vector);
    }

    void *target = vector->data + vector->size * vector->element_size;
    memcpy(target, element, vector->element_size);

    vector->size++;
}

/**
 * Pops an element from the back of the vector.
 * @param vector The vector.
 */
void vector_pop_back(struct Vector *vector) {
    if (vector->size == 0) {
        return;
    }

    vector->size--;

    vector_shrink(vector);
}

/**
 * Gets the size of the vector.
 * @param vector The vector.
 * @return The size of the vector.
 */
size_t vector_size(struct Vector *vector) { return vector->size; }

/**
 * Inserts an element at the given index.
 * If the index is out of bounds, the element is inserted at the end of the vector.
 * @param vector The vector.
 * @param index The index.
 * @param element The element to insert.
 * @return The vector.
 */
void vector_insert(struct Vector *vector, size_t index, void *element) {
    if (index >= vector->size) {
        return vector_push_back(vector, element);
    }

    if (vector->size == vector->capacity) {
        vector_enlarge(vector);
    }

    void *target = vector->data + index * vector->element_size;
    memmove(target + vector->element_size, target, (vector->size - index) * vector->element_size);
    memcpy(target, element, vector->element_size);

    vector->size++;
}

/**
 * Removes the element at the given index.
 * If the index is out of bounds, nothing happens.
 * @param vector The vector.
 * @param index The index.
 * @return The vector.
 */
void vector_remove(struct Vector *vector, size_t index) {
    if (index >= vector->size) {
        return;
    }

    void *target = vector->data + index * vector->element_size;
    memmove(target, target + vector->element_size, (vector->size - index - 1) * vector->element_size);

    vector->size--;

    vector_shrink(vector);
}

/**
 * Clears the vector.
 * @param vector The vector.
 * @return The vector.
 */
void vector_clear(struct Vector *vector) {
    vector->size = 0;
    vector->capacity = 0;
    vector_shrink(vector);
}
