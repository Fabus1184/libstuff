#ifndef DECIMAL_H
#define DECIMAL_H

#include <math.h>
#include <stddef.h>

const char *decimal_prefix(double_t *value) {
    if (*value < pow(10, -9)) {
        *value *= pow(10, 12);
        return "p";
    } else if (*value < pow(10, -6)) {
        *value *= pow(10, 9);
        return "n";
    } else if (*value < pow(10, -3)) {
        *value *= pow(10, 6);
        return "µ";
    } else if (*value < pow(10, 0)) {
        *value *= pow(10, 3);
        return "m";
    } else if (*value < pow(10, 3)) {
        return "";
    } else if (*value < pow(10, 6)) {
        *value /= pow(10, 3);
        return "k";
    } else if (*value < pow(10, 9)) {
        *value /= pow(10, 6);
        return "M";
    } else if (*value < pow(10, 12)) {
        *value /= pow(10, 9);
        return "G";
    } else if (*value < pow(10, 15)) {
        *value /= pow(10, 12);
        return "T";
    } else if (*value < pow(10, 18)) {
        *value /= pow(10, 15);
        return "P";
    } else if (*value < pow(10, 21)) {
        *value /= pow(10, 18);
        return "E";
    } else if (*value < pow(10, 24)) {
        *value /= pow(10, 21);
        return "Z";
    } else if (*value < pow(10, 27)) {
        *value /= pow(10, 24);
        return "Y";
    } else {
        return "?";
    }
}

#endif  // DECIMAL_H
