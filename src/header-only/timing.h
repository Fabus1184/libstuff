#ifndef TIMING_H
#define TIMING_H

#include <stdio.h>
#include <sys/time.h>

#include "decimal.h"

#define INIT_TIMING()   \
    double elapsed;     \
    const char *prefix; \
    struct timeval start, end

#define START_TIMING() gettimeofday(&start, NULL)

#define END_TIMING(print)                                                                      \
    {                                                                                          \
        gettimeofday(&end, NULL);                                                              \
        elapsed = (end.tv_sec - start.tv_sec) + (pow(10, -6) * (end.tv_usec - start.tv_usec)); \
        prefix = decimal_prefixed(&elapsed);                                                   \
        char buffer[1024];                                                                     \
        snprintf(buffer, sizeof(buffer), "took %g %s s\n", elapsed, prefix);                   \
        print(buffer);                                                                         \
    }                                                                                          \
    (void) 0

#endif  // TIMING_H