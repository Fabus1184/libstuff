#ifndef DECIMAL_H
#define DECIMAL_H

#include <math.h>
#include <stddef.h>
#include <stdio.h>

const char *decimal_prefixed(double_t value, char *buffer, size_t length) {
    const char *prefix;

    if (value < pow(10, -21)) {
        value *= pow(10, 24);
        prefix = "y";
    } else if (value < pow(10, -18)) {
        value *= pow(10, 21);
        prefix = "z";
    } else if (value < pow(10, -15)) {
        value *= pow(10, 18);
        prefix = "a";
    } else if (value < pow(10, -12)) {
        value *= pow(10, 15);
        prefix = "f";
    } else if (value < pow(10, -9)) {
        value *= pow(10, 12);
        prefix = "p";
    } else if (value < pow(10, -6)) {
        value *= pow(10, 9);
        prefix = "n";
    } else if (value < pow(10, -3)) {
        value *= pow(10, 6);
        prefix = "u";
    } else if (value < pow(10, 0)) {
        value *= pow(10, 3);
        prefix = "m";
    } else if (value < pow(10, 3)) {
        prefix = " ";
    } else if (value < pow(10, 6)) {
        value /= pow(10, 3);
        prefix = "k";
    } else if (value < pow(10, 9)) {
        value /= pow(10, 6);
        prefix = "M";
    } else if (value < pow(10, 12)) {
        value /= pow(10, 9);
        prefix = "G";
    } else if (value < pow(10, 15)) {
        value /= pow(10, 12);
        prefix = "T";
    } else if (value < pow(10, 18)) {
        value /= pow(10, 15);
        prefix = "P";
    } else if (value < pow(10, 21)) {
        value /= pow(10, 18);
        prefix = "E";
    } else if (value < pow(10, 24)) {
        value /= pow(10, 21);
        prefix = "Z";
    } else {
        value /= pow(10, 24);
        prefix = "Y";
    }

    snprintf(buffer, length, "%.3lf %s", value, prefix);

    return buffer;
}

#endif  // DECIMAL_H
