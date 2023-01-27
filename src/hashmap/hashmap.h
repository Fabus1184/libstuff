#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "header-only/memory.h"

struct Map;

struct Map *map_create(size_t capacity, size_t key_size, size_t data_size, comparator compare);

void map_free(struct Map *map);

void map_insert(struct Map *map, const void *key, const void *value);

bool map_delete(struct Map *map, const void *key);

const void *map_lookup(const struct Map *map, const void *key);

size_t map_size(const struct Map *map);

#endif  // HASHTABLE_H
