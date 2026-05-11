/**
 * @file stack.h
 * @brief Basic stack implementation.
 *
 * Generally, stacks do not allow "random access" to their contents. In strict
 * stack implementations, only the top element can be accessed. And usually,
 * the total number of elements is hidden, with only an interface to check
 * whether or not the stack is empty.
 *
 * This stack is less strict:
 * - You can get a reference to any element.
 * - You can get the number of elements.
 *
 * But still, you can only push elements to the top of the stack, and pop the
 * top one.
 */

#ifndef STACK_H
#define STACK_H


#include <stddef.h>


/** @brief The stack implementation. */
struct stack {
	/** @cond */
	char _[sizeof(size_t) * 3 + sizeof(void *)];
	/** @endcond */
};


/**
 * @brief Initializes a new empty stack.
 *
 * Allocates memory required for the stack. The `item_size` parameter
 * determines the size of each element to be pushed.
 */
void stack_init(struct stack *stack, size_t item_size);

/** @brief Deallocates memory allocated by the stack. */
void stack_destroy(struct stack *stack);

/** @brief Pops the top element of the stack. */
void *stack_pop(struct stack *stack);

/** @brief Pushes an element onto the stack. */
void stack_push(struct stack *stack, void *item);

/** @brief Returns the top element of the stack. */
void *stack_top(struct stack *stack);

/** @brief Returns a reference to the item at `index`. */
void *stack_at(struct stack *stack, size_t index);

/** @brief Returns the length of the stack. */
size_t stack_len(const struct stack *stack);


#endif
