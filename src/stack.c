#include "../include/stack.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAP 64


void stack_init(struct stack *s, size_t item_size)
{
	s->cap = INITIAL_CAP;
	s->len = 0;
	s->item_size = item_size;
	s->data = malloc(item_size * INITIAL_CAP);
}

void stack_destroy(struct stack *s)
{
	free(s->data);
}

void *stack_pop(struct stack *s)
{
	assert(s->len && "stack underflow");

	void *item = stack_at(s, s->len - 1);

	s->len--;

	return item;
}

void stack_push(struct stack *s, void *item)
{
	if (s->len == s->cap) {
		s->cap *= 2;

		s->data = realloc(s->data,
					s->cap * s->item_size);
	}

	memcpy(&s->data[s->len * s->item_size],
	       item, s->item_size);

	s->len++;
}

void *stack_top(struct stack *s)
{
	return stack_at(s, s->len - 1);
}

void *stack_at(struct stack *s, size_t index)
{
	return &s->data[index * s->item_size];
}

size_t stack_len(const struct stack *s)
{
	return s->len;
}
