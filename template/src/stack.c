#include "../include/stack.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAP 64

#define deref_s (*(struct stack_internal *) s)


struct stack_internal {
    size_t cap;
    size_t len;
    size_t item_size;
    char *data;
};


void stack_init(struct stack *s, size_t item_size)
{
	deref_s.cap = INITIAL_CAP;
	deref_s.len = 0;
	deref_s.item_size = item_size;
	deref_s.data = malloc(item_size * INITIAL_CAP);
}

void stack_destroy(struct stack *s)
{
	free(deref_s.data);
}

void *stack_pop(struct stack *s)
{
	assert(deref_s.len && "stack underflow");

	void *item = stack_at(s, deref_s.len - 1);

	deref_s.len--;

	return item;
}

void stack_push(struct stack *s, void *item)
{
	if (deref_s.len == deref_s.cap) {
		deref_s.cap *= 2;

		deref_s.data = realloc(deref_s.data,
					deref_s.cap * deref_s.item_size);
	}

	memcpy(&deref_s.data[deref_s.len * deref_s.item_size],
	       item, deref_s.item_size);

	deref_s.len++;
}

void *stack_top(struct stack *s)
{
	return stack_at(s, deref_s.len - 1);
}

void *stack_at(struct stack *s, size_t index)
{
	return &deref_s.data[index * deref_s.item_size];
}

size_t stack_len(const struct stack *s)
{
	return deref_s.len;
}
