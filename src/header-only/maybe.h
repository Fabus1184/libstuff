#ifndef MAYBE_H
#define MAYBE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * A Maybe is a type that can either be a value or nothing.
 * It is used to represent the result of a function that may fail.
 * It is similar to the Maybe type in Haskell.
 * @param is_just Whether the Maybe contains a value.
 * @param value The value contained in the Maybe.
 */
struct Maybe {
    bool is_just;
    void *value;
};

/**
 * Returns the value contained in the Maybe if it is a Just, otherwise returns the default value.
 * @param maybe The Maybe to get the value from.
 * @param default_value The default value to return if the Maybe is a Nothing.
 * @return The value contained in the Maybe if it is a Just, otherwise the default value.
 */
void *from_maybe(struct Maybe maybe, void *default_value) {
    if (maybe.is_just) {
        return maybe.value;
    } else {
        return default_value;
    }
}

/**
 * Lifts a value into a Maybe.
 * @param value The value to lift into a Maybe.
 */
struct Maybe just(void *value) { return (struct Maybe){true, value}; }

/**
 * Returns a Maybe containing nothing.
 */
struct Maybe nothing() { return (struct Maybe){false, NULL}; }

#endif  // MAYBE_H