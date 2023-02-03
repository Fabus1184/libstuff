#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "header-only/memory.h"

typedef void (*sorting_function)(void *array, size_t length, size_t size, comparator cmp);

bool is_sorted(void *array, size_t length, size_t size);

bool is_sorted_cmp(void *array, size_t length, size_t size, comparator cmp);

void shuffle(void *array_, size_t length, size_t size);

void insertionsort_cmp(void *array, size_t length, size_t size, comparator cmp);

void insertionsort(void *array, size_t length, size_t size);

void mergesort_cmp(void *array, size_t length, size_t size, comparator cmp);

void mergesort(void *array, size_t length, size_t size);

void radixsort(void *array, size_t length, size_t size, size_t alphabet_size, size_t max_index,
               size_t (*alphabetize)(void *elem, size_t index));

void multithreaded_sort(void *array, size_t length, size_t size, size_t threads, sorting_function sort, comparator cmp);

#endif  // SORT_H