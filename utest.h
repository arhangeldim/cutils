#ifndef UTEST_H_
#define UTEST_H_

#include <stdio.h>
#include <string.h>

static int passed_count = 0;
static int failed_count = 0;

#define EPSILON 1e-6

#define ABS(value) ((value < 0) ? -(value) : (value))


#define STR(s) #s
#define ASSERT_OUT stderr

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
		PRINT_FUNCTION \
		if ((condition)) { \
	   		fprintf(ASSERT_OUT, passed_str, __VA_ARGS__); \
			passed_count++; \
		} else { \
			failed_count++; \
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
