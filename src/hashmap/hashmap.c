#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "header-only/maybe.h"
#include "header-only/memory.h"

struct Map {
    struct Maybe *keys;
    void **data;
    size_t key_size;
    size_t data_size;
    size_t capacity;
    comparator compare;
};

/**
 * Create a new map.
 * @param capacity The capacity of the map.
 * @param key_size The size of the key.
 * @param data_size The size of the data.
 * @param cmp The comparator function.
 * @return The map.
 * @note The map is allocated with malloc, and must be freed with map_free.
 */
struct Map *map_create(size_t capacity, size_t key_size, size_t data_size, comparator cmp) {
    struct Map *map = malloc(sizeof(struct Map));
    assert(NULL != map && "Failed to allocate map");

    map->data = calloc(capacity, sizeof(void *));
    assert(NULL != map->data && "Failed to allocate map data");

    map->keys = calloc(capacity, sizeof(struct Maybe));
    assert(NULL != map->keys && "Failed to allocate map keys");
    for (size_t i = 0; i < capacity; ++i) {
        map->keys[i].is_just = false;
    }

    map->compare = cmp;
    map->capacity = capacity;
    map->key_size = key_size;
    map->data_size = data_size;
    return map;
}

/**
 * Free the map.
 * @param map The map.
 */
void map_free(struct Map *map) {
    for (size_t i = 0; i < map->capacity; ++i) {
        if (map->keys[i].is_just == true) {
            free(map->keys[i].value);
            free(map->data[i]);
        }
    }

    free(map->data);
    free(map->keys);
    free(map);
}

/**
 * Hash function 1, used for the initial hash.
 */
size_t hash1(const void *key, size_t key_size, size_t capacity) {
    size_t hash1 = 5381;
    for (size_t i = 0; i < key_size; ++i) {
        hash1 = ((hash1 << 5) + hash1) + *(i + (const uint8_t *) key);
    }
    return hash1 % capacity;
}

/**
 * Returns the index of the key in the map given the iteration.
 */
size_t map_index(const struct Map *map, const void *key, size_t iteration) {
    return (hash1(key, map->key_size, map->capacity) + iteration) % map->capacity;
}

/**
 * Insert a key-value pair into the map.
 * @param map The map.
 * @param key The key.
 * @param value The value.
 * @note The key and value are copied into the map.
 */
void map_insert(struct Map *map, const void *key, const void *value) {
    for (size_t i = 0; i < map->capacity; ++i) {
        size_t index = map_index(map, key, i);

        if (map->keys[index].is_just == false) {
            map->keys[index].value = malloc(map->key_size);
            assert(NULL != map->keys[index].value && "Failed to allocate key");
            memcpy(map->keys[index].value, key, map->key_size);
            map->keys[index].is_just = true;

            map->data[index] = malloc(map->data_size);
            assert(NULL != map->data[index] && "Failed to allocate data");
            memcpy(map->data[index], value, map->data_size);

            return;
        }
    }
    assert(false && "Map overflowed!");
}

/**
 * Delete a key-value pair from the map.
 * @param map The map.
 * @param key The key.
 * @return True if the key was deleted, false otherwise.
 */
bool map_delete(struct Map *map, const void *key) {
    for (size_t i = 0; i < map->capacity; ++i) {
        size_t index = map_index(map, key, i);

        if (map->keys[index].is_just == true) {
            if (map->compare(map->keys[index].value, key, map->key_size) == 0) {
                free(map->keys[index].value);
                free(map->data[index]);
                map->keys[index].is_just = false;
                return true;
            }
        }
    }
    return false;
}

/**
 * Lookup a key in the map.
 * @param map The map.
 * @param key The key.
 * @return The value associated with the key, or NULL if the key is not in the map.
 */
const void *map_lookup(const struct Map *map, const void *key) {
    for (size_t i = 0; i < map->capacity; ++i) {
        size_t index = map_index(map, key, i);

        if (map->keys[index].is_just == true) {
            if (map->compare(map->keys[index].value, key, map->key_size) == 0) {
                return map->data[index];
            }
        }
    }
    return NULL;
}

/**
 * Get the size of the map.
 * @param map The map.
 * @return The size of the map.
 * @note This is O(n), where n is the capacity of the map.
 */
size_t map_size(const struct Map *map) {
    size_t i = 0;
    for (size_t j = 0; j < map->capacity; ++j) {
        if (map->keys[j].is_just == true) {
            ++i;
        }
    }
    return i;
}
