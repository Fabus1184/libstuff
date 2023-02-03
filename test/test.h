#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _LOG_PREFIX printf("%-25s ", __BASE_FILE__ ": ");

#define LOG(...)             \
    {                        \
        _LOG_PREFIX;         \
        printf(__VA_ARGS__); \
        printf("\n");        \
    }

#define LOG_TIMING(...)                                                                                               \
    {                                                                                                                 \
        _LOG_PREFIX;                                                                                                  \
        char __log_timing_buffer[1024];                                                                               \
        char __log_timing_buffer2[1024];                                                                              \
        snprintf(__log_timing_buffer, sizeof(__log_timing_buffer), __VA_ARGS__);                                      \
        printf("%-55s => %s\n", __log_timing_buffer, END_TIMING(__log_timing_buffer2, sizeof(__log_timing_buffer2))); \
    }

#endif  // TEST_H
