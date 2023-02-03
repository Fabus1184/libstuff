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
    LOG("---- Testing sorting ----");

    srand(time(NULL));

    size_t sum = 0, x = 0;
    char buffer[100];

    LOG("=> Testing merge sort");

    START_TIMING();
    size_t *array = calloc(SIZE, sizeof(size_t));
    assert(array != NULL && "Could not allocate memory for array");
    for (size_t i = 0; i < SIZE; i++) {
        size_t x = rand() % SIZE;
        sum += x;
        array[i] = x;
    }
    LOG_TIMING("Generated %s random values", decimal_prefixed(SIZE, buffer, sizeof(buffer)));

    START_TIMING();
    mergesort(array, SIZE, sizeof(size_t));
    LOG_TIMING("Sorted %s values using merge sort", decimal_prefixed(SIZE, buffer, sizeof(buffer)));

    START_TIMING();
    assert(is_sorted_cmp(array, SIZE, sizeof(size_t), size_t_comparator) == true && "Array not sorted");
    x = 0;
    for (size_t i = 0; i < SIZE; i++) {
        x += array[i];
    }
    assert(x == sum && "Sum of elements is not correct");
    LOG_TIMING("Array is sorted");

    LOG("=> Testing radix sort");

    START_TIMING();
    shuffle(array, SIZE, sizeof(size_t));
    LOG_TIMING("Shuffled %s values", decimal_prefixed(SIZE, buffer, sizeof(buffer)));

    START_TIMING();
    radixsort(array, SIZE, sizeof(size_t), 10, MAX_DECIMAL_LENGTH, alphabetize_size_t);
    LOG_TIMING("Sorted %s values using radix sort", decimal_prefixed(SIZE, buffer, sizeof(buffer)));

    START_TIMING();
    assert(is_sorted_cmp(array, SIZE, sizeof(size_t), size_t_comparator) == true && "Array not sorted");
    x = 0;
    for (size_t i = 0; i < SIZE; i++) {
        x += array[i];
    }
    assert(x == sum && "Sum of elements is not correct");
    LOG_TIMING("Array is sorted");

    LOG("=> Testing multithreaded merge sort");

    START_TIMING();
    shuffle(array, SIZE, sizeof(size_t));
    LOG_TIMING("Shuffled %s values", decimal_prefixed(SIZE, buffer, sizeof(buffer)));

    START_TIMING();
    multithreaded_sort(array, SIZE, sizeof(size_t), 16, mergesort_cmp, size_t_comparator);
    LOG_TIMING("Sorted %s values using multithreaded merge sort", decimal_prefixed(SIZE, buffer, sizeof(buffer)));

    START_TIMING();
    assert(is_sorted_cmp(array, SIZE, sizeof(size_t), size_t_comparator) == true && "Array not sorted");
    x = 0;
    for (size_t i = 0; i < SIZE; i++) {
        x += array[i];
    }
    assert(x == sum && "Sum of elements is not correct");
    LOG_TIMING("Array is sorted");

    free(array);

    LOG("---- PASSED ----");

    return EXIT_SUCCESS;
}