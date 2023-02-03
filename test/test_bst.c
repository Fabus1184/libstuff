#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bst/bst.h"
#include "header-only/decimal.h"
#include "header-only/timing.h"
#include "sort/sort.h"
#include "test.h"

INIT_TIMING();

#define SIZE 1000000

void stuff(const void *value, void *arr) {
    static size_t i = 0;
    const size_t *x = (const size_t *) value;
    ((size_t *) arr)[i++] = *x;
}

int32_t main(void) {
    LOG("---- Testing binary search tree ----");

    size_t n = 0;
    struct bst *bst = NULL;

    size_t *values = malloc(SIZE * sizeof(size_t));
    assert(values != NULL && "Could not allocate memory for values");
    char buffer1[100];
    char buffer2[100];

    START_TIMING();

    srand(time(NULL));
    for (size_t i = 0; i < SIZE; i++) {
        values[i] = rand();
        bool x = bst_insert_cmp(&values[i], &bst, sizeof(size_t), size_t_comparator);
        if (x == true) {
            n++;
        }
    }

    LOG_TIMING("Inserted %s / %s values", decimal_prefixed(n, buffer1, sizeof(buffer1)),
               decimal_prefixed(SIZE, buffer2, sizeof(buffer2)));

    START_TIMING();
    for (size_t i = 0; i < SIZE; ++i) {
        assert(bst_search_cmp(&values[i], bst, sizeof(size_t), size_t_comparator) == true && "Value not found");
    }
    LOG_TIMING("All values found!");

    START_TIMING();
    size_t *arr = malloc(n * sizeof(size_t));
    assert(arr != NULL && "Could not allocate memory for array");
    assert(bst_inorder(bst, stuff, arr) == n && "Inorder did not traverse correct number of nodes");
    LOG_TIMING("Inorder traversal of %s nodes", decimal_prefixed(n, buffer1, sizeof(buffer1)));

    START_TIMING();
    assert(is_sorted_cmp(arr, n, sizeof(size_t), size_t_comparator) == true && "Inorder traversal is not sorted");
    LOG_TIMING("Inorder traversal is sorted");

    START_TIMING();
    for (size_t i = 0; i < SIZE; ++i) {
        bst_delete_cmp(&values[i], &bst, sizeof(size_t), size_t_comparator);
    }
    assert(bst == NULL && "Tree is not empty");
    LOG_TIMING("Successfully deleted all values");

    bst_free(bst);

    free(arr);
    free(values);

    LOG("---- PASSED ----");

    return EXIT_SUCCESS;
}