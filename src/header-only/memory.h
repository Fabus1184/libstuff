#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

/**
 * Comparator function.
 * @param a The first value.
 * @param b The second value.
 * @param size The size of the values.
 * @return Negative if a < b, positive if a > b, 0 if a == b.
 */
typedef int32_t (*comparator)(const void *a, const void *b, size_t size);

/**
 * Compare two size_t values.
 * @param a The first value.
 * @param b The second value.
 * @param size unused.
 * @return Negative if a < b, positive if a > b, 0 if a == b.
 */
int32_t size_t_comparator(const void *a, const void *b, size_t size) {
    assert(size == sizeof(size_t) && "size_t comparator called with wrong size");
    const size_t *x = (const size_t *) a;
    const size_t *y = (const size_t *) b;
    return *x - *y;
}

/**
 * Compare two byte arrays.
 * @param a The first array.
 * @param b The second array.
 * @param size The size of the arrays.
 * @return Negative if a < b, positive if a > b, 0 if a == b.
 */
int32_t memcomparator(const void *a, const void *b, size_t size) {
    const uint8_t *x = (const uint8_t *) a;
    const uint8_t *y = (const uint8_t *) b;
    for (intmax_t i = size - 1; i >= 0; --i) {
        if (x[i] > y[i]) {
            return 1;
        } else if (x[i] < y[i]) {
            return -1;
        }
    }
    return 0;
}

#endif  // MEMORY_H