#ifndef SORT_H
#define SORT_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define IS_SORTED(T)                                           \
    bool is_sorted_##T(const T *array, size_t size) {          \
        for (size_t i = 1; i < size; ++i) {                    \
            if (array[i - 1] > array[i]) {                     \
                printf("%zu > %zu\n", array[i - 1], array[i]); \
                return false;                                  \
            }                                                  \
        }                                                      \
        return true;                                           \
    }

#define SORT_DEFINE(T) IS_SORTED(T)

#endif  // SORT_H