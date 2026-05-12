/**
 * @file map.h
 * @brief Basit hashmap.
 *
 * @ref map, FNV hash fonksiyonunu ve open addressing yönetmini kullanır.
 * `Key-value` ikilisi şeklinde verileri tutar, farklı uzunlukta `key`lere izin
 * verirken sabit uzunlukta `value` tutabilir.
 */

#ifndef MAP_H
#define MAP_H


#include <stddef.h>


/** @brief map. */
struct map {
	/** @cond */
	struct map_entry *entries;
	size_t cap;
	size_t used;
	size_t value_size;
	/** @endcond */
};

/** @brief map `entry`lerini tek tek çekebilmek için `iteration handle`. */
struct map_it {
	/** @cond */
	struct map *m;
	size_t i;
	/** @endcond */
};

/** @brief map'te tutulan `key-value` ikilisi, `entry`. */
struct map_entry {
	void *key  /** `Key`, `value`ı işaret eden `unique identifier`. */;
	size_t keylen  /** `Key` içinde tutulan verinin uzunluğu. */;
	char value[]  /** Kullanıcı tarafından `key`e atanmış `value`. */;
};


/**
 * @brief Yeni bir map oluşturur.
 *
 * map için gerekli hafızayı ayırır. `value_size` parametresi, kullanıcının
 * ekleyeceği `value`ların maksimum boyutunu belirtir.
 */
void map_init(struct map *map, size_t value_size);

/** @brief map tarafından ayrılmış hafızayı temizler. */
void map_destroy(struct map *map);

/**
 * @brief `Key` ile eşleşen `value`yı döner, `key` bulunamazsa `NULL` döner.
 *
 * `key` parametresi `null-terminated` olmalıdır.
 */
void *map_get(struct map *map, const void *key);

/** @brief `Key` ile eşleşen `value`yı döner, `key` bulunamazsa `NULL` döner. */
void *map_get2(struct map *map, const void *key, size_t keylen);

/**
 * @brief map'e `key-value` ikilisini ekler.
 *
 * @warning map'te daha önce `key` bulunmuyor olmalıdır.
 *
 * `key` parametresi `null-terminated` olmalıdır.
 */
void map_insert(struct map *map, const void *key, const void *value);


/**
 * @brief map'e `key-value` ikilisini ekler.
 *
 * @warning map'te daha önce `key` bulunmuyor olmalıdır.
 */
void map_insert2(struct map *map, const void *key, size_t keylen, const void *value);

/**
 * @brief `Key-value` ikilisini map'ten siler, `value`yu döner.
 *
 * @attention Dönen `value`, bir sonraki map işlemine kadar hayatta kalır.
 *            Kullanıcı, `value`yu kullanmaya devam etmek istiyorsa onu
 *            kopyalamalıdır.
 *
 * `key` parametresi `null-terminated` olmalıdır.
 */
void *map_delete(struct map *map, const void *key);

/**
 * @brief `Key-value` ikilisini map'ten siler, `value`yu döner.
 *
 * @attention Dönen `value`, bir sonraki map işlemine kadar hayatta kalır.
 *            Kullanıcı, `value`yu kullanmaya devam etmek istiyorsa onu
 *            kopyalamalıdır.
 */
void *map_delete2(struct map *map, const void *key, size_t keylen);

/** @brief map'teki `entry`leri tek tek çekecek `iteration handle` hazırlar. */
void map_iter(struct map *map, struct map_it *it);

/**
 * @brief `Iteration handle`dan sıradaki `entry`yi çeker.
 *
 * Tüm `entry`ler çekilmişse, `NULL` döner.
 */
struct map_entry *map_iter_next(struct map_it *it);


#endif
