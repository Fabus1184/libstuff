#ifndef DECIMAL_H
#define DECIMAL_H

#include <math.h>
#include <stddef.h>

const char *decimal_prefixed(double_t value, const char **buffer, size_t *length) {
    const char *prefix;
    if (value < pow(10, -9)) {
        prefix = "n";
        value *= pow(10, 9);
    } else if (value < pow(10, -6)) {
        prefix = "µ";
        value *= pow(10, 6);
    } else if (value < pow(10, -3)) {
        prefix = "m";
        value *= pow(10, 3);
    } else if (value < pow(10, 3)) {
        prefix = NULL;
    } else if (value < pow(10, 6)) {
        prefix = "k";
        value /= pow(10, 3);
    } else if (value < pow(10, 9)) {
        prefix = "M";
        value /= pow(10, 6);
    } else if (value < pow(10, 12)) {
        prefix = "G";
        value /= pow(10, 9);
    } else if (value < pow(10, 15)) {
        prefix = "T";
        value /= pow(10, 12);
    } else if (value < pow(10, 18)) {
        prefix = "P";
        value /= pow(10, 15);
    } else if (value < pow(10, 21)) {
        prefix = "E";
        value /= pow(10, 18);
    } else if (value < pow(10, 24)) {
        prefix = "Z";
        value /= pow(10, 21);
    } else if (value < pow(10, 27)) {
        prefix = "Y";
        value /= pow(10, 24);
    } else {
        prefix = "Y";
        value /= pow(10, 24);
    }
    if (prefix == NULL) {
        snprintf(*buffer, *length, "%g", value);
    } else {
        snprintf(*buffer, *length, "%g %s", value, prefix);
    }
    return buffer;
}

#endif  // DECIMAL_H
