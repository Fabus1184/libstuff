#include <assert.h>
#include <memory.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "header-only/memory.h"

typedef void (*sorting_function)(void *array, size_t length, size_t size, comparator cmp);

/**
 * Check if an array is sorted using a comparator function.
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @param cmp The comparator function.
 * @return True if the array is sorted, false otherwise.
 * @note This function does not modify the array.c
 * @note This function runs in {@code O(length * cmp)}
 * @see memcomparator
 */
bool is_sorted_cmp(void *array_, size_t length, size_t size, comparator cmp) {
    uint8_t *array = array_;
    for (size_t i = 0; i < length - 1; i++) {
        if (cmp(array + (i * size), array + ((i + 1) * size), size) > 0) {
            return false;
        }
    }
    return true;
}

/**
 * Check if an array is sorted using the default comparator (memcmp).
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @return True if the array is sorted, false otherwise.
 * @note This function does not modify the array.
 * @note This function runs in {@code O(length * cmp)}
 */
bool is_sorted(void *array, size_t length, size_t size) { return is_sorted_cmp(array, length, size, memcomparator); }

/**
 * Insertion sort using a comparator function.
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @param cmp The comparator function.
 * @return The sorted array.
 * @note This function modifies the array.
 * @note This function uses {@code size} extra memory.
 * @note This function runs in {@code O(length² * cmp)}
 * @see insertionsort
 */
void insertionsort_cmp(void *array_, size_t length, size_t size, comparator cmp) {
    uint8_t *array = array_;
    for (size_t i = 1; i < length; i++) {
        for (size_t j = i; j > 0 && cmp(array + ((j - 1) * size), array + (j * size), size) > 0; j--) {
            void *temp = malloc(size);
            memcpy(temp, array + (j * size), size);
            memcpy(array + (j * size), array + ((j - 1) * size), size);
            memcpy(array + ((j - 1) * size), temp, size);
            free(temp);
        }
    }
}

/**
 * Insertion sort using the default comparator (memcmp).
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @return The sorted array.
 * @note This function modifies the array.
 * @note This function uses {@code size} extra memory.
 * @note This function runs in {@code O(length² * cmp)}
 * @see insertionsort
 */
void insertionsort(void *array, size_t length, size_t size) { insertionsort_cmp(array, length, size, memcomparator); }

void swap(void *a, void *b, size_t size) {
    uint8_t *temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void *merge_cmp(void *a_, size_t length1, void *b_, size_t length2, size_t size, comparator cmp) {
    uint8_t *a = a_;
    uint8_t *b = b_;
    uint8_t *temp = malloc((length1 + length2) * size);
    size_t i = 0;
    size_t j = 0;
    for (size_t k = 0; k < length1 + length2; k++) {
        if (i >= length1) {
            memcpy(temp + (k * size), b + (j * size), size);
            j++;
        } else if (j >= length2) {
            memcpy(temp + (k * size), a + (i * size), size);
            i++;
        } else if (cmp(a + (i * size), b + (j * size), size) < 0) {
            memcpy(temp + (k * size), a + (i * size), size);
            i++;
        } else {
            memcpy(temp + (k * size), b + (j * size), size);
            j++;
        }
    }
    return temp;
}

/**
 * Merge sort using a comparator function.
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @param cmp The comparator function.
 * @note This function modifies the array.
 * @note This function uses {@code length} extra memory.
 * @note This function runs in {@code O(length * log(length) * cmp)}
 */
void mergesort_cmp(void *array_, size_t length, size_t size, comparator cmp) {
    uint8_t *array = array_;
    if (length <= 1) {
        return;
    }
    size_t length1 = length / 2;
    size_t length2 = length - length1;
    mergesort_cmp(array, length1, size, cmp);
    mergesort_cmp(array + (length1 * size), length2, size, cmp);

    void *merged = merge_cmp(array, length1, array + (length1 * size), length2, size, cmp);
    memcpy(array, merged, length * size);
    free(merged);
}

/**
 * Merge sort using the default comparator (memcmp).
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @note This function modifies the array.
 * @note This function uses {@code length} extra memory.
 * @note This function runs in {@code O(length * log(length) * cmp)}
 */
void mergesort(void *array, size_t length, size_t size) { mergesort_cmp(array, length, size, memcomparator); }

/**
 * Shuffle an array using the Fisher-Yates algorithm.
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @note This function modifies the array.
 * @note This function uses {@code size} extra memory.
 * @note This function runs in {@code O(length)}
 */
void shuffle(void *array_, size_t length, size_t size) {
    uint8_t *array = array_;
    void *temp = malloc(size);
    for (size_t i = length - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        memcpy(temp, array + (j * size), size);
        memcpy(array + (j * size), array + (i * size), size);
        memcpy(array + (i * size), temp, size);
    }
    free(temp);
}

void radixsort_helper(void *array_, size_t length, size_t size, size_t alphabet_size, size_t max_index,
                      size_t (*alphabetize)(void *elem, size_t index), size_t index) {
    if (length <= 1 || index >= max_index) {
        return;
    }

    uint8_t *array = array_;
    void **buckets = calloc(alphabet_size, sizeof(void *));
    size_t *bucket_sizes = calloc(alphabet_size, sizeof(size_t));
    {
        size_t *bucket_allocs = calloc(alphabet_size, sizeof(size_t));

        for (size_t i = 0; i < alphabet_size; ++i) {
            buckets[i] = calloc(length, size);
            bucket_allocs[i] = length;
        }

        for (size_t i = 0; i < length; ++i) {
            size_t bucket = alphabetize(array + (i * size), index);

            if (bucket_sizes[bucket] >= bucket_allocs[bucket]) {
                bucket_allocs[bucket] += length / alphabet_size;
                buckets[bucket] = realloc(buckets[bucket], bucket_allocs[bucket] * size);
            }

            memcpy((uint8_t *) buckets[bucket] + (bucket_sizes[bucket] * size), array + (i * size), size);
            bucket_sizes[bucket]++;
        }

        free(bucket_allocs);
    }

    for (size_t i = 0; i < alphabet_size; ++i) {
        if (bucket_sizes[i] > 1) {
            radixsort_helper(buckets[i], bucket_sizes[i], size, alphabet_size, max_index, alphabetize, index + 1);
        }
    }

    size_t i = 0;
    for (size_t j = 0; j < alphabet_size; ++j) {
        memcpy(array + (i * size), buckets[j], bucket_sizes[j] * size);
        i += bucket_sizes[j];
        free(buckets[j]);
    }

    free(buckets);
    free(bucket_sizes);
}

/**
 * Radix sort using a function to determine the element of the alphabet.
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @param alphabet_size The size of the alphabet.
 * @param max_index The maximum number of alphabet tokens in an element.
 * @param alphabetize The function to determine the element of the alphabet.
 * @note This function modifies the array.
 * @note This function uses {@code length * alphabet_size * max_index} extra memory.
 * @note This function runs in {@code O(length * max_index)} time.
 */
void radixsort(void *array, size_t length, size_t size, size_t alphabet_size, size_t max_index,
               size_t (*alphabetize)(void *elem, size_t index)) {
    radixsort_helper(array, length, size, alphabet_size, max_index, alphabetize, 0);
}

struct sorting_args {
    sorting_function sort;
    void *array;
    size_t length;
    size_t size;
    comparator cmp;
};

void *sorting_thread(void *args_) {
    struct sorting_args *args = args_;
    args->sort(args->array, args->length, args->size, args->cmp);
    return NULL;
}

void *merge_cmp_n(void **arrays, size_t *lengths, size_t n_parts, size_t size, comparator cmp) {
    if (n_parts == 1) {
        void *array = malloc(lengths[0] * size);
        memcpy(array, arrays[0], lengths[0] * size);
        return array;
    }

    void *a = merge_cmp_n(arrays, lengths, n_parts / 2, size, cmp);
    void *b = merge_cmp_n(arrays + (n_parts / 2), lengths + (n_parts / 2), n_parts - (n_parts / 2), size, cmp);

    size_t a_length = 0;
    size_t b_length = 0;
    for (size_t i = 0; i < n_parts; ++i) {
        if (i < n_parts / 2) {
            a_length += lengths[i];
        } else {
            b_length += lengths[i];
        }
    }

    void *merged = merge_cmp(a, a_length, b, b_length, size, cmp);
    free(a);
    free(b);

    return merged;
}

/**
 * Multithreaded sort
 * @param array The array.
 * @param length The length of the array.
 * @param size The size of each element.
 * @param n_threads The number of threads to use.
 * @param sort The sorting function to use.
 * @param cmp The comparator to use.
 * @note This function modifies the array.
 */
void multithreaded_sort(void *array_, size_t length, size_t size, size_t n_threads, sorting_function sort,
                        comparator cmp) {
    uint8_t *array = array_;
    pthread_t threads[n_threads];
    struct sorting_args args[n_threads];

    for (size_t i = 0; i < n_threads; ++i) {
        args[i] = (struct sorting_args){sort, array, length / n_threads, size, cmp};

        if (i == n_threads - 1) {
            args[i].length += length % n_threads;
        }

        pthread_create(&threads[i], NULL, sorting_thread, &args[i]);

        array += (length * size) / n_threads;
    }

    for (size_t j = 0; j < n_threads; ++j) {
        pthread_join(threads[j], NULL);
    }

    void *arrays[n_threads];
    size_t lengths[n_threads];

    for (size_t i = 0; i < n_threads; ++i) {
        arrays[i] = args[i].array;
        lengths[i] = args[i].length;
    }

    void *merged = merge_cmp_n(arrays, lengths, n_threads, size, cmp);
    memcpy(array_, merged, length * size);
    free(merged);
}
