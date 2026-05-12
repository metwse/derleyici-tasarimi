/**
 * @file stack.h
 * @brief Basit stack.
 *
 * Genelde, stack'ler `random access`e izin vermez, stack'teki bir elemana
 * indeksiyle erişilmez. Katı stack implementasyonları, sadece `top` elemana
 * erişim izni (`pop`/`peek`) verir; hatta toplam eleman sayısını da saklar
 * ve yalnızca stack'in boş olup olmadığını belirten bir fonksiyon sunar.
 *
 * Bu stack implementasyonu daha esnektir:
 * - Herhangi bir elemana referans alabilirsiniz.
 * - Toplam eleman sayısını alabilirsiniz.
 *
 * Ancak yine de sadece `top` elemanı çekip, `top`tan eleman ekleyip
 * çıkarabilirsiniz.
 */

#ifndef STACK_H
#define STACK_H


#include <stddef.h>


/** @brief stack. */
struct stack {
	/** @cond */
	char *data;
	size_t cap;
	size_t len;
	size_t item_size;
	/** @endcond */
};


/**
 * @brief Yeni bir stack oluşturur.
 *
 * stack için gerekli hafızayı ayırır. `item_size` parametresi, kullanıcının
 * ekleyeceği elemanların boyutunu belirtir.
 */
void stack_init(struct stack *stack, size_t item_size);

/** @brief stack tarafından ayrılmış hafızayı temizler. */
void stack_destroy(struct stack *stack);

/** @brief En üstteki elemanı (`top`) stack'ten silip döner. */
void *stack_pop(struct stack *stack);

/** @brief stack'in en üstüne elamanı ekler. */
void stack_push(struct stack *stack, void *item);

/** @brief stack'in en üstündeki elemanı döner. */
void *stack_top(struct stack *stack);

/** @brief Baştan `index`inci elemanı döner. */
void *stack_at(struct stack *stack, size_t index);

/** @brief Toplam eleman sayısını döner. */
size_t stack_len(const struct stack *stack);


#endif
