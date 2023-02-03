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

int32_t main(void) {
    LOG("---- Testing vector ----");
    srand(time(NULL));

    size_t sum = 0, x = 0;
    char buffer[100];

    LOG("=> Testing merge sort");

    return EXIT_SUCCESS;
}