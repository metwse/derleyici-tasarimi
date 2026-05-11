#include "../include/stack.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>


int main()
{
	srand(time(NULL));

	struct stack s;

	for (int _fuzz = 0; _fuzz < 16; _fuzz++) {
		stack_init(&s, sizeof(int));

		int limit = rand() % 1024;
		for (int i = 0; i < limit; i++) {
			stack_push(&s, &i);
			assert(*(int *) stack_at(&s, i) == i);
			assert(stack_len(&s) == (size_t) i + 1);
		}

		for (int i = limit; i > 0; i--) {
			assert(*(int *) stack_top(&s) == i - 1);
			assert(*(int *) stack_pop(&s) == i - 1);
		}

		stack_destroy(&s);
	}

	return EXIT_SUCCESS;
}
