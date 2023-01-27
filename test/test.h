#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG(...)                                   \
    {                                              \
        printf("%-25s ", __BASE_FILE__ ": ");      \
        size_t n = snprintf(NULL, 0, __VA_ARGS__); \
        char buffer[n + 1];                        \
        snprintf(buffer, n + 1, __VA_ARGS__);      \
        printf("%-45s", buffer);                   \
    }

#endif  // TEST_H
