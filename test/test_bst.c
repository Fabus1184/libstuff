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
    LOG("---- Testing binary search tree ----\n");

    size_t n = 0;
    struct bst *bst = NULL;

    size_t *values = malloc(SIZE * sizeof(size_t));
    assert(values != NULL && "Could not allocate memory for values");

    START_TIMING();

    srand(time(NULL));
    for (size_t i = 0; i < SIZE; i++) {
        values[i] = rand();
        bool x = bst_insert_cmp(&values[i], &bst, sizeof(size_t), size_t_comparator);
        if (x == true) {
            n++;
        }
    }

    double_t x = n, y = SIZE;
    const char *prefix1 = decimal_prefixed(&x);
    const char *prefix2 = decimal_prefixed(&y);
    LOG("Inserted %lg%s / %lg%s values, ", x, prefix1, y, prefix2);
    END_TIMING(printf);

    START_TIMING();
    for (size_t i = 0; i < SIZE; ++i) {
        assert(bst_search_cmp(&values[i], bst, sizeof(size_t), size_t_comparator) == true && "Value not found");
    }
    LOG("All values found, ");
    END_TIMING(printf);

    START_TIMING();
    size_t *arr = malloc(n * sizeof(size_t));
    assert(arr != NULL && "Could not allocate memory for array");
    assert(bst_inorder(bst, stuff, arr) == n && "Inorder did not traverse correct number of nodes");
    {
        double_t x = n;
        const char *prefix = decimal_prefixed(&x);
        LOG("Inorder traversal of %g %s nodes, ", x, prefix);
    }
    END_TIMING(printf);

    START_TIMING();
    assert(is_sorted_cmp(arr, n, sizeof(size_t), size_t_comparator) == true && "Inorder traversal is not sorted");
    LOG("Inorder traversal is sorted, ");
    END_TIMING(printf);

    bst_free(bst);

    free(arr);
    free(values);

    LOG("---- PASSED ----\n\n");

    return EXIT_SUCCESS;
}