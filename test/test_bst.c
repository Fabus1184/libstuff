#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bst.h"
#include "decimal.h"
#include "sort.h"
#include "timing.h"

SORT_DEFINE(size_t)
INIT_TIMING();

#define SIZE 1000000

void stuff(const void *value, void *arr) {
    static size_t i = 0;
    const size_t *x = (const size_t *) value;
    ((size_t *) arr)[i++] = *x;
}

int32_t size_t_comparator(const void *a, const void *b, __attribute__((unused)) size_t size) {
    const size_t *x = (const size_t *) a;
    const size_t *y = (const size_t *) b;
    return *x - *y;
}

int32_t main(void) {
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
    const char *prefix1 = decimal_prefix(&x);
    const char *prefix2 = decimal_prefix(&y);
    printf(__FILE__ ": Inserted %lg%s / %lg%s values\n", x, prefix1, y, prefix2);

    END_TIMING();

    START_TIMING();
    for (size_t i = 0; i < SIZE; ++i) {
        assert(bst_search_cmp(&values[i], bst, sizeof(size_t), size_t_comparator) == true && "Value not found");
    }
    printf(__FILE__ ": All values found\n");
    END_TIMING();

    START_TIMING();
    size_t *arr = malloc(n * sizeof(size_t));
    assert(arr != NULL && "Could not allocate memory for array");
    assert(bst_inorder(bst, stuff, arr) == n && "Inorder did not traverse correct number of nodes");
    printf(__FILE__ ": Inorder traversal of %zu nodes\n", n);
    END_TIMING();

    START_TIMING();
    assert(is_sorted_size_t(arr, n) == true && "Inorder traversal is not sorted");
    printf(__FILE__ ": Inorder traversal is sorted\n");
    END_TIMING();

    bst_free(bst);

    free(arr);
    free(values);

    printf(__FILE__ ": PASSED\n");
}