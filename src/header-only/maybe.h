#ifndef MAYBE_H
#define MAYBE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct Maybe {
    bool is_just;
    void *value;
};

#endif  // MAYBE_H