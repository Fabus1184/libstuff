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

#define END_TIMING()                                                                       \
    gettimeofday(&end, NULL);                                                              \
    elapsed = (end.tv_sec - start.tv_sec) + (pow(10, -6) * (end.tv_usec - start.tv_usec)); \
    prefix = decimal_prefix(&elapsed);                                                     \
    printf("Elapsed time: %7g %ss\n", elapsed, prefix);

#endif  // TIMING_H