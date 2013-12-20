#include <assert.h>
#include <stdlib.h>

#include "list.h"

void list_init(list_t **list, void (*destroy)(list_item_t *item))
{
	*list = malloc(sizeof(list_t));
	(*list)->size = 0;
	(*list)->head = NULL;
	(*list)->tail = NULL;
	(*list)->destroy = destroy;
}


void list_add(list_t *list, void *data)
{
	assert(list != NULL);
	list_item_t *item;
	if ((item = malloc(sizeof(list_item_t))) == NULL) {
		abort();
	}

	if (list->head == NULL) {
		item->next = NULL;
		item->prev = NULL;
		list->head = item;
		list->tail = item;
	} else {
		item->next = NULL;
		item->prev = list->head;
		list->head->next = item;
		list->head = item;
	}

	item->data = data;
	list->size++;
}

int list_remove(list_t *list, void *data, int (*cmp)(const void *, const void *))
{
	assert(list != NULL);
	assert(cmp != NULL);
	list_item_t *p = list->tail;
	if (cmp != NULL) {
		while (p != NULL) {
			if (cmp(p->data, data) == 0) {
				if (p == list->head)
					list->head = p->prev;
				if (p == list->tail)
					list->tail = p->next;

				if (p->next != NULL)
					p->next->prev = p->prev;
				if (p->prev != NULL)
					p->prev->next = p->next;

				if (list->destroy != NULL)
					list->destroy(p);
				p = NULL;

				list->size--;

				return 0;
			}
			p = p->next;
		}
	}
	return -1;
}

#ifndef LIST_H_
#define LIST_H_

#include <stdint.h>

struct _list_item_t;

typedef struct _list_item_t {
	struct _list_item_t *prev;
	struct _list_item_t *next;
	void *data;
} list_item_t;

typedef struct _list_t {
	uint16_t size;
	list_item_t *head;
	list_item_t *tail;
	void (*destroy) (list_item_t *item);
} list_t;

void list_init(list_t **list, void (*destroy)(list_item_t *item));
void list_add(list_t *list, void *data);

void list_print(list_t *list);

#endif /* LIST_H_ */
#include <assert.h>
#include <stdio.h>

#include "list.h"
#include "utest.h"

typedef struct _complex_t {
	int re;
	int im;
} complex_t;

#define RE(c) ((complex_t *) (c))->re
#define IM(c) ((complex_t *) (c))->im
#define int_data(item) *(int *) (item->data)

static void destroy_complex(list_item_t *item)
{
	complex_t *c = (complex_t *) item->data;
	free(c);
	free(item);
}

int int_cmp(const void *v1, const void *v2)
{
	return *(int *) v1 - *(int *) v2;
}

void test_init()
{
	list_t *list;
	complex_t *c1 = malloc(sizeof(complex_t));
	c1->re = 10;
	c1->im = 11;

	complex_t *c2 = malloc(sizeof(complex_t));
	c2->re = 20;
	c2->im = 21;


	list_init(&list, destroy_complex);

	ASSERT_TRUE(list != NULL);
	EXPECT_EQ(0, list->size);
	EXPECT_TRUE(list->head == NULL);


	list_add(list, c1);

	ASSERT_EQ(1, list->size);
	EXPECT_EQ(10, RE(list->head->data));
	EXPECT_EQ(11, IM(list->head->data));
	EXPECT_EQ(10, RE(list->tail->data));
	EXPECT_EQ(11, IM(list->tail->data));

	list_add(list, c2);

	ASSERT_EQ(2, list->size);
	EXPECT_EQ(20, RE(list->head->data));
	EXPECT_EQ(21, IM(list->head->data));
	EXPECT_EQ(10, RE(list->tail->data));
	EXPECT_EQ(11, IM(list->tail->data));
		
	list_item_t *i1 = list->tail;
	list_item_t *i2 = list->head;

	EXPECT_TRUE(i1->prev == NULL);
	EXPECT_TRUE(i1->next == i2);
	EXPECT_TRUE(i2->next == NULL);
	EXPECT_TRUE(i2->prev == i1);
}


void test_remove_1()
{
	list_t *list;
	list_init(&list, NULL);
	int a = 1;
	int b = 2;
	list_add(list, &a);
	list_add(list, &b);
	EXPECT_EQ(a, int_data(list->tail));
	EXPECT_EQ(b, int_data(list->head));

	list_remove(list, &a, int_cmp);
	EXPECT_TRUE(list->size == 1);
	EXPECT_EQ(list->head, list->tail);
	EXPECT_EQ(NULL, list->head->next);
	EXPECT_EQ(NULL, list->tail->next);
	EXPECT_EQ(NULL, list->head->prev);
	EXPECT_EQ(NULL, list->tail->prev);

	list_remove(list, &b, int_cmp);
	EXPECT_TRUE(list->size == 0);
	EXPECT_EQ(NULL, list->head);
	EXPECT_EQ(NULL, list->tail);
}

void test_remove()
{
	list_t *list;
	list_init(&list, NULL);
	int a = 1;
	int b = 2;
	int c = 3;
	list_add(list, &a);
	list_add(list, &b);
	list_add(list, &c);

	int r = list_remove(list, &b, int_cmp);

	ASSERT_EQ(0, r);
	EXPECT_EQ(2, list->size);
	list_item_t *head = list->head;
	list_item_t *tail = list->tail;
	EXPECT_EQ(NULL, head->next);
	EXPECT_EQ(NULL, tail->prev);
	EXPECT_EQ(head->prev, tail);
	EXPECT_EQ(tail->next, head);
			
	r = list_remove(list, &b, int_cmp);
	ASSERT_EQ(-1, r);
	EXPECT_EQ(2, list->size);

	r = list_remove(list, &a, int_cmp);
	ASSERT_EQ(0, r);
	EXPECT_EQ(1, list->size);
	EXPECT_EQ(NULL, list->head->next);
	EXPECT_EQ(NULL, list->tail->prev);
	EXPECT_EQ(list->head, list->tail);
}


int main(void)
{
	test_init();	
	test_remove();
	test_remove_1();
	PRINT_SUMMARY;

	return 0;
}
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
#include <stdlib.h>
#include <stdio.h>

#include "ustring.h"

int ustrlen(const char *str)
{
	const char *p = str;
	for (; *p; p++);	
	return p - str;
}

int ustrcmp(const char *s1, const char *s2)
{
	do {
		if (*s1 > *s2)
			return 1;
		if (*s1 < *s2)
			return -1;
		*s1++;
		*s2++;
	} while (*s1 != '\0' && *s2 != '\0'); 
	return 0;
}

char *ustrcat(char *dst, const char *src)
{
	char *p = dst;
	for (; *dst; ++dst);
	while (*dst++ = *src++);
	return p;
}

char *ustrcpy(char *dst, const char *src)
{
	char *p = dst;
	while(*dst++ = *src++);
	return p;
}

char *ustrchr(const char *s, char c)
{
	const char *p = s;
	while (*p) {
		if (*p == c)
			return (char *) p;
		p++;
	}
	return NULL;
}

#ifndef USTRING_H_
#define USTRING_H_

int ustrlen(const char *str);
int ustrcmp(const char *s1, const char *s2);
char *ustrcat(char *dst, const char *src);
char *ustrcpy(char *dst, const char *src);
char *ustrchr(const char *s, char c);

#endif /* USTRING_H_ */

#include "ustring.h"
#include "utest.h"


void test_strchr()
{

	const char *s = "abcde";
	
	EXPECT_STREQ("abcde", ustrchr(s, 'a'));
	EXPECT_STREQ("bcde", ustrchr(s, 'b'));
	EXPECT_STREQ("e", ustrchr(s, 'e'));

	EXPECT_TRUE(NULL == ustrchr(s, 'x'));

}

void test_strcpy()
{
	char str[10] = {0};
	EXPECT_STREQ("", ustrcpy(str, ""));
	EXPECT_STREQ("a", ustrcpy(str, "a"));
	EXPECT_STREQ("ab", ustrcpy(str, "ab"));
}

void test_strcat()
{
	char str[10] = {0};
	EXPECT_STREQ("", ustrcat(str, ""));
	
	strcpy(str,"a");
	EXPECT_STREQ("a", ustrcat(str, ""));

	strcpy(str, "");
	EXPECT_STREQ("b", ustrcat(str, "b"));


	strcpy(str, "a");
	EXPECT_STREQ("ab", ustrcat(str, "b"));
	
	strcpy(str, "a");
	EXPECT_STREQ("abc", ustrcat(str, "bc"));
	
	strcpy(str, "ab");
	EXPECT_STREQ("abc", ustrcat(str, "c"));
}
		
void test_strcmp()
{
	EXPECT_EQ(0, ustrcmp("abc", "abc"));
	EXPECT_EQ(0, ustrcmp("", ""));
	EXPECT_EQ(0, ustrcmp("a", "a"));

	EXPECT_EQ(1, ustrcmp("abc", ""));
	EXPECT_EQ(1, ustrcmp("b", "a"));
	EXPECT_EQ(1, ustrcmp("abc", "abb"));

	EXPECT_EQ(-1, ustrcmp("", "abc"));
	EXPECT_EQ(-1, ustrcmp("a", "b"));
}

void test_strlen()
{
	EXPECT_EQ(0, ustrlen(""));
	EXPECT_EQ(1, ustrlen("1"));
	EXPECT_EQ(3, ustrlen("buy"));
}

int main(void)
{	
	test_strlen();
	test_strcmp();
	test_strcat();
	test_strchr();

	PRINT_SUMMARY;

	return 0;
}
#ifndef UTEST_H_
#define UTEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_CAPACITY 100

static int passed_count = 0;
static int failed_count = 0;

#define EPSILON 1e-6

#define ABS(value) ((value < 0) ? -(value) : (value))

#define STR(s) #s
#define ASSERT_OUT stderr
#define PRINT_PASSED 0

#define FUNCTION_SIG __PRETTY_FUNCTION__

#define PRINT_FUNCTION \
	do { \
		fprintf(ASSERT_OUT, ":: %s():%d -> ", FUNCTION_SIG, __LINE__); \
	} while (0);

#define PRINT_SUMMARY \
	do { \
		fprintf(ASSERT_OUT, "SUMMARY:\npassed: %d\nfailed:%d\n", passed_count, failed_count); \
	} while(0);


#define TEST_COMMON(assert, condition, passed_str, failed_str, ...) \
	do { \
		if ((condition)) { \
	   		if (PRINT_PASSED) fprintf(ASSERT_OUT, passed_str, __VA_ARGS__); \
			passed_count++; \
		} else { \
			failed_count++; \
		    PRINT_FUNCTION \
			fprintf(ASSERT_OUT, failed_str, __VA_ARGS__); \
			if ((assert)) { \
				PRINT_SUMMARY \
				exit(1); \
			}\
		} \
	} while(0)

#define EXPECT_COMMON(condition, passed_str, failed_str, ...) \
	TEST_COMMON(0, condition, passed_str, failed_str, __VA_ARGS__);

#define ASSERT_COMMON(condition, passed_str, failed_str, ...) \
	TEST_COMMON(1, condition, passed_str, failed_str, __VA_ARGS__);

#define ASSERT_TRUE(condition) ASSERT_COMMON(condition, "PASSED\n", \
		"FAILED: Expected: true, Actual: false. Assertion: %s\n", STR((condition)));

#define ASSERT_FALSE(condition) ASSERT_COMMON(!(condition), "PASSED\n", \
		"FAILED: Expected: false, Actual: true. Assertion: %s\n", STR((condition)));

#define ASSERT_EQ(expected, actual) ASSERT_COMMON((expected) == (actual), "PASSED\n", \
		"FAILED: Must be equal. Expected: %d, Actual: %d\n", (expected), (actual));

#define ASSERT_NE(condition) ASSERT_COMMON((expected) != (actual), "PASSED\n", \
		"FAILED: Must be not equal. Expected: %d, Actual: %d\n", (expected), (actual));

#define EXPECT_TRUE(condition) EXPECT_COMMON((condition), "PASSED\n", \
		"FAILED: Expected: true, Actual: false. Assertion: %s\n", STR((condition)));

#define EXPECT_FALSE(condition) EXPECT_COMMON((condition), "PASSED\n", \
		"FAILED: Expected: false, Actual: true. Assertion: %s\n", STR((condition)));

#define EXPECT_EQ(expected, actual) EXPECT_COMMON((expected) == (actual), "PASSED\n", \
		"FAILED: Must be equal. Expected: %d, Actual: %d\n", (expected), (actual));

#define EXPECT_NE(expected, actual) EXPECT_COMMON((expected) != (actual), "PASSED\n", \
		"FAILED: Must be not equal. Expected: %d, Actual: %d\n", (expected), (actual));

/* String comparsion */

#define EXPECT_STREQ(expected, actual) EXPECT_COMMON(strcmp((expected), (actual)) == 0, "PASSED\n", \
		"FAILED: Must be equal. Expected: %s, Actual: %s\n", (expected), (actual));

#define EXPECT_STRNE(expected, actual) EXPECT_COMMON(strcmp((expected), (actual)) != 0, "PASSED\n", \
		"FAILED: Must be not equal. Expected: %s, Actual: %s\n", (expected), (actual));

#define ASSERT_STREQ(expected, actual) ASSERT_COMMON(strcmp((expected), (actual)) == 0, "PASSED\n", \
		"FAILED: Must be equal. Expected: %s, Actual: %s\n", (expected), (actual));

#define ASSERT_STRNE(expected, actual) ASSERT_COMMON(strcmp((expected), (actual)) != 0, "PASSED\n", \
		"FAILED: Must be not equal. Expected: %s, Actual: %s\n", (expected), (actual));

/* Floating point comparsion */

#define EXPECT_FP_EQ(expected, actual) EXPECT_COMMON(ABS((expected) - (actual)) <= EPSILON, "PASSED\n", \
		"FAILED: Must be equal. Expected: %f, Actual: %f, Epsilon: %f\n", (expected), (actual), EPSILON);

#define EXPECT_FP_NE(expected, actual) EXPECT_COMMON(ABS((expected) - (actual)) > EPSILON, "PASSED\n", \
		"FAILED: Must be not equal. Expected: %f, Actual: %f, Epsilon: %f\n", (expected), (actual), EPSILON);

#define EXPECT_FP_NEAR(expected, actual, epsilon) EXPECT_COMMON(ABS((expected) - (actual)) <= (epsilon), "PASSED\n", \
		"FAILED: Must be near. Expected: %f, Actual: %f, Epsilon: %f\n", (expected), (actual), (epsilon));

#define ASSERT_FP_EQ(expected, actual) ASSERT_COMMON(ABS((expected) - (actual)) <= EPSILON, "PASSED\n", \
		"FAILED: Must be equal. Expected: %f, Actual: %f, Epsilon: %f\n", (expected), (actual), EPSILON);

#define ASSERT_FP_NE(expected, actual) ASSERT_COMMON(ABS((expected) - (actual)) > EPSILON, "PASSED\n", \
		"FAILED: Must be not equal. Expected: %f, Actual: %f, Epsilon: %f\n", (expected), (actual), EPSILON);

#define ASSERT_FP_NEAR(expected, actual, epsilon) ASSERT_COMMON(ABS((expected) - (actual)) <= (epsilon), "PASSED\n", \
		"FAILED: Must be near. Expected: %f, Actual: %f, Epsilon: %f\n", (expected), (actual), (epsilon));


#endif /* UTEST_H_ */
