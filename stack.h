#ifndef STACK_H_
#define STACK_H_

#define stack_size(stack) (stack)->size
#define stack_is_empty(stack) !((stack)->size)

struct stack_item;

typedef struct stack_item {
	int value;
	struct stack_item *prev;
} stack_item_t;

typedef struct stack {
	uint16_t size;
	stack_item_t *head;
} stack_t;


void stack_push(stack_t *stack, int value);
int stack_pop(stack_t *stack, int *data);

#endif /* STACK_H_ */
