#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "stack.h"
#include "utest.h"

void stack_push(stack_t *stack, int value)
{
	assert(stack != NULL);
    stack_item_t *item = malloc(sizeof(stack_item_t));
	if (item == NULL)
		abort();
	item->value = value;
	item->prev = (!stack->size) ? NULL : stack->head;
	stack->head = item;
	stack->size++;
}

int stack_pop(stack_t *stack, int *data)
{	
	assert(stack != NULL);
	if (!stack->size) {
		return -1;
	} 
	stack->size--;
	*data = stack->head->value;
	stack_item_t *old = stack->head;
	stack->head = old->prev;
	free(old);
	return 0;
}
		
void test_stack()
{
	stack_t *stack = malloc(sizeof(stack_t));
	if (!stack)
		abort();

	EXPECT_EQ(0, stack_size(stack));

	stack_push(stack, 1);
	stack_push(stack, 2);

	int *tmp = malloc(sizeof(int));
	stack_pop(stack, tmp);
	EXPECT_EQ(2, *tmp);
	stack_pop(stack, tmp);
	EXPECT_EQ(1, *tmp);
	EXPECT_EQ(-1, stack_pop(stack, tmp));

	EXPECT_TRUE(stack_is_empty(stack));
}


int main(void)
{
	test_stack();

	return 0;
}
