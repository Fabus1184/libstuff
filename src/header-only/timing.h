#ifndef TIMING_H
#define TIMING_H

#include <stdio.h>
#include <sys/time.h>

#include "decimal.h"

#define INIT_TIMING()                            \
    double __timing_elapsed;                     \
    struct timeval __timing_start, __timing_end; \
    char __timing_buffer[100]

#define START_TIMING() gettimeofday(&__timing_start, NULL)

#define END_TIMING(buffer, size) \
    _END_TIMING(buffer, size, __timing_elapsed, __timing_start, __timing_end, __timing_buffer)

char *_END_TIMING(char *buffer, size_t size, double elapsed, struct timeval start, struct timeval end, char *__buffer) {
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (pow(10, -6) * (end.tv_usec - start.tv_usec));
    snprintf(buffer, size, "took %15ss", decimal_prefixed(elapsed, __buffer, 100));
    return buffer;
}

#endif  // TIMING_H