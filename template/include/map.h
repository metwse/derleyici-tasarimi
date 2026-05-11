/**
 * @file map.h
 * @brief Basic hash map implementation.
 *
 * The implementation uses the FNV hash function in open addressing mode. It
 * supports variable-length keys but does not support variable-length values.
 */

#ifndef MAP_H
#define MAP_H


#include <stddef.h>


/** @brief The map implementation. */
struct map {
	/** @cond */
	size_t _[sizeof(size_t) * 3 + sizeof(void *)];
	/** @endcond */
};

/** @brief Map key/value pair iterator. */
struct map_it {
	/** @cond */
	size_t _[sizeof(size_t) + sizeof(void *)];
	/** @endcond */
};

/** @brief Key-value pair stored in the map. */
struct map_entry {
	void *key  /** Key of the entry. */;
	size_t keylen  /** Length of the key. */;
	char value[]  /** Value of the entry. */;
};


/**
 * @brief Initializes a new empty map.
 *
 * Allocates memory required for the map. The `value_size` parameter
 * determines the size of each value to be inserted.
 */
void map_init(struct map *map, size_t value_size);

/** @brief Deallocates memory allocated by the map. */
void map_destroy(struct map *map);

/**
 * @brief Returns the element with the provided null-terminated key, or NULL if no
 * such element exists.
 */
void *map_get(struct map *map, const void *key);

/**
 * @brief Returns the element with the provided key, or NULL if no such element
 * exists.
 */
void *map_get2(struct map *map, const void *key, size_t keylen);

/**
 * @brief Inserts an element into the entry with the provided null-terminated
 * key.
 */
void map_insert(struct map *map, const void *key, const void *value);

/** @brief Inserts an element into the entry with the provided key. */
void map_insert2(struct map *map, const void *key, size_t keylen, const void *value);

/** @brief Same as get, but also deletes the element. */
void *map_delete(struct map *map, const void *key);

/** @brief Same as get, but also deletes the element. */
void *map_delete2(struct map *map, const void *key, size_t keylen);

/** @brief Initializes a iterator handle. */
void map_iter(struct map *map, struct map_it *it);

/** @brief Returns the next entry, NULL if none remain. */
struct map_entry *map_iter_next(struct map_it *it);


#endif
