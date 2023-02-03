#ifndef TIMING_H
#define TIMING_H

#include <stdio.h>
#include <sys/time.h>

#include "decimal.h"

/**
 * Initializes the timing macros.
 * This has to be called before any of the timing macros.
 */
#define INIT_TIMING()                            \
    double __timing_elapsed;                     \
    struct timeval __timing_start, __timing_end; \
    char __timing_buffer[100]

/**
 * Starts the timing.
 */
#define START_TIMING() gettimeofday(&__timing_start, NULL)

/**
 * Logs the time elapsed since the last call to START_TIMING.
 * The elapsed time is written to the buffer.
 * The format is "took 10µs" or "took 10ms" or "took 10s".
 * Decimal prefixes are used automatically when appropriate.
 * @param buffer The buffer to write the time to.
 * @param size The size of the buffer.
 */
#define END_TIMING(buffer, size) \
    _END_TIMING(buffer, size, __timing_elapsed, __timing_start, __timing_end, __timing_buffer)

char *_END_TIMING(char *buffer, size_t size, double elapsed, struct timeval start, struct timeval end, char *__buffer) {
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (pow(10, -6) * (end.tv_usec - start.tv_usec));
    snprintf(buffer, size, "took %15ss", decimal_prefixed(elapsed, __buffer, 100));
    return buffer;
}

#endif  // TIMING_H