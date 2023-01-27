#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "header-only/timing.h"
#include "sort/sort.h"
#include "test.h"

INIT_TIMING();

const size_t SIZE = 1000000;
const size_t MAX_DECIMAL_LENGTH = 20;

size_t pow10(size_t n) {
    size_t result = 1;
    for (size_t i = 0; i < n; i++) {
        result *= 10;
    }
    return result;
}

size_t alphabetize_size_t(void *elem, size_t index) {
    if (index >= MAX_DECIMAL_LENGTH) {
        return 0;
    } else {
        return (*(size_t *) elem / pow10(MAX_DECIMAL_LENGTH - index - 1)) % 10;
    }
}

int32_t main(void) {
    LOG("---- Testing sorting ----\n");

    srand(time(NULL));

    START_TIMING();
    size_t *array = malloc(SIZE * sizeof(size_t));
    assert(array != NULL && "Could not allocate memory for array");
    for (size_t i = 0; i < SIZE; i++) {
        array[i] = rand() % SIZE;
    }
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Generated %g %s random values\n", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    mergesort(array, SIZE, sizeof(size_t));
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Sorted %g %s values using merge sort\n", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    assert(is_sorted_cmp(array, SIZE, sizeof(size_t), size_t_comparator) == true && "Array not sorted");
    LOG("Array is sorted\n");
    END_TIMING(LOG);

    START_TIMING();
    shuffle(array, SIZE, sizeof(size_t));
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Shuffled %g %s values\n", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    radixsort(array, SIZE, sizeof(size_t), 10, MAX_DECIMAL_LENGTH, alphabetize_size_t);
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Sorted %g %s values using radix sort\n", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    assert(is_sorted_cmp(array, SIZE, sizeof(size_t), size_t_comparator) == true && "Array not sorted");
    LOG("Array is sorted\n");
    END_TIMING(LOG);

    free(array);

    LOG("---- PASSED ----\n\n");

    return EXIT_SUCCESS;
}