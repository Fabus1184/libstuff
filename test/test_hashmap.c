#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hashmap/hashmap.h"
#include "header-only/decimal.h"
#include "header-only/timing.h"
#include "test.h"

INIT_TIMING();

#define SIZE 1000000

int32_t main(void) {
    LOG("---- Testing hashmap ----\n");

    size_t *array = calloc(SIZE, sizeof(size_t));
    struct Map *map = map_create(SIZE, sizeof(size_t), sizeof(size_t), size_t_comparator);

    START_TIMING();
    srand(time(NULL));
    for (size_t i = 0; i < SIZE; ++i) {
        array[i] = rand() % SIZE;
        map_insert(map, &i, &array[i]);
    }
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Inserted %g %s elements,", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    assert(map_size(map) == SIZE && "Map size mismatch");
    {
        double x = map_size(map);
        const char *prefix = decimal_prefixed(&x);
        LOG("Verified map size: %g %s elements,", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    for (size_t i = 0; i < SIZE; ++i) {
        const size_t *value = map_lookup(map, &i);
        assert(value != NULL && "Failed to get value");
        assert(*value == array[i] && "Value mismatch");
    }
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Verified %g %s elements\n", x, prefix);
    }
    END_TIMING(LOG);

    START_TIMING();
    for (size_t i = 0; i < SIZE; ++i) {
        assert(map_delete(map, &i) == true && "Failed to remove value");
    }
    {
        double x = SIZE;
        const char *prefix = decimal_prefixed(&x);
        LOG("Removed %g %s elements\n", x, prefix);
    }
    END_TIMING(LOG);

    map_free(map);
    free(array);

    LOG("---- PASSED ----\n\n");

    return EXIT_SUCCESS;
}