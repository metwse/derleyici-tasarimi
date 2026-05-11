#include "../include/map.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAP 64
#define TOMBSTONE SIZE_MAX

#define deref_m (*(struct map_internal *) m)
#define deref_it (*(struct it_internal *) it)
#define entry_at(entries, i) ((struct map_entry *) &((char *) entries) \
			[i * (sizeof(struct map_entry) + deref_m.value_size)])


struct map_internal {
	size_t cap;
	size_t used;
	size_t value_size;
	struct map_entry *entries;
};

struct it_internal {
	struct map *m;
	size_t i;
};


/* https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function */
static uint64_t fnv_hash(const char *, size_t);

/* Normally, insert copies the key. But for rehashing, this function is
 * used. */
static void map_insert2_nocopy(struct map *, char *, size_t, const void *);

/* Rehash the map to a bigger hash bucket. */
static void rehash(struct map *, size_t);

/* Get an entry, key-value pair. */
static struct map_entry *map_get2_entry(struct map *, const char *, size_t);


static uint64_t fnv_hash(const char *key, size_t keylen)
{
	uint64_t hash = 0xcbf29ce484222325;

	for (size_t i = 0; i < keylen; i++) {
		hash *= 0x100000001b3;
		hash ^= (unsigned char) key[i];
	}

	return hash;
}

void map_init(struct map *m, size_t value_size)
{
	deref_m.cap = INITIAL_CAP;
	deref_m.used = 0;
	deref_m.value_size = value_size;
	deref_m.entries = calloc(deref_m.value_size + sizeof(struct map_entry),
			      INITIAL_CAP);
}

void map_destroy(struct map *m)
{
	for (size_t i = 0; i < deref_m.cap; i++) {
		struct map_entry *e = entry_at(deref_m.entries, i);

		if (e->keylen != TOMBSTONE && e->keylen != 0)
			free(e->key);
	}

	free(deref_m.entries);
}

static struct map_entry *map_get2_entry(struct map *m,
					const char *key,
					size_t keylen)
{
	uint64_t hash = fnv_hash(key, keylen);
	size_t start_i = hash % deref_m.cap;
	size_t i = start_i;

	do {
		struct map_entry *e = entry_at(deref_m.entries, i);

		if (e->keylen == 0)
			return NULL;
		else if (e->keylen == keylen && memcmp(e->key, key, keylen) == 0)
			return e;
		else
			i++;

		i %= deref_m.cap;
	} while (i != start_i);

	// Hashmap never fills up to 100%. There alwas a empty slot.
	assert(0);  // GCOVR_EXCL_LINE: unreachable
}

static void rehash(struct map *m, size_t cap)
{
	struct map_entry *old_entries = deref_m.entries;

	deref_m.used = 0;
	deref_m.entries = calloc(deref_m.value_size + sizeof(struct map_entry),
			      deref_m.cap);

	for (size_t i = 0; i < cap; i++) {
		struct map_entry *e = entry_at(old_entries, i);

		if (e->keylen != TOMBSTONE && e->keylen != 0)
			map_insert2_nocopy(m, e->key, e->keylen, e->value);
	}

	free(old_entries);
}

static void map_insert2_nocopy(struct map *m,
			       char *key,
			       size_t keylen,
			       const void *value)
{
	if (deref_m.cap * 3 < deref_m.used * 4) {
		size_t old_cap = deref_m.cap;

		deref_m.cap *= 2;

		rehash(m, old_cap);
	}

	assert(!map_get2(m, key, keylen) && "map contains the element");

	uint64_t hash = fnv_hash(key, keylen);
	size_t start_i = hash % deref_m.cap;
	size_t i = start_i;

	do {
		struct map_entry *e = entry_at(deref_m.entries, i);

		if (e->keylen == 0 || e->keylen == TOMBSTONE) {
			e->keylen = keylen;
			e->key = key;

			if (deref_m.value_size)
				memcpy(e->value, value, deref_m.value_size);

			deref_m.used++;

			return;
		} else {
			i++;
		}

		i %= deref_m.cap;
	} while (i != start_i);

	assert(0);  // GCOVR_EXCL_LINE: unreachable
}

void *map_get(struct map *m, const void *key)
{
	return map_get2(m, key, strlen(key));
}

void *map_get2(struct map *m, const void *key, size_t keylen)
{
	struct map_entry *e = map_get2_entry(m, key, keylen);

	return e ? e->value : NULL;
}

void *map_delete(struct map *m, const void *key)
{
	return map_delete2(m, key, strlen(key));
}

void *map_delete2(struct map *m, const void *key, size_t keylen)
{
	struct map_entry *e = map_get2_entry(m, key, keylen);

	if (e) {
		e->keylen = TOMBSTONE;
		free(e->key);
		deref_m.used--;

		return e->value;
	} else {
		return NULL;
	}
}

void map_insert(struct map *m, const void *key, const void *value)
{
	map_insert2(m, key, strlen(key), value);
}

void map_insert2(struct map *m, const void *key, size_t keylen, const void *value)
{
	char *new_key = malloc(keylen);

	memcpy(new_key, key, keylen);

	map_insert2_nocopy(m, new_key, keylen, value);
}

void map_iter(struct map *map, struct map_it *it)
{
	deref_it.m = map;
	deref_it.i = 0;
}

struct map_entry *map_iter_next(struct map_it *it)
{
	struct map *m = deref_it.m;

	while (deref_it.i != deref_m.cap) {
		struct map_entry *e = entry_at(deref_m.entries, deref_it.i);

		deref_it.i++;

		if (e->keylen != TOMBSTONE && e->keylen != 0)
			return e;
	}

	return NULL;
}
