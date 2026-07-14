#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <string.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

typedef struct s_test_node
{
	const char *name;
	void (*fn)(void);
	struct s_test_node *next;
} TestNode;

extern int       g_test_run;
extern int       g_tests_failed;
extern TestNode *g_test_list_head;

#define TEST(test_name)                                                                     \
	static void test_name(void);                                                            \
	static TestNode test_name##_node = {.name = #test_name, .fn = test_name, .next = NULL}; \
	__attribute__((constructor)) static void test_name##_register(void)                     \
	{                                                                                       \
		test_name##_node.next = g_test_list_head;                                           \
		g_test_list_head = &test_name##_node;                                               \
	}                                                                                       \
	static void test_name(void)

#define ASSERT_EQ(a, b)                                                   \
	do                                                                    \
	{                                                                     \
		g_test_run++;                                                     \
		if ((a) != (b))                                                   \
		{                                                                 \
			g_tests_failed++;                                             \
			printf("FAIL %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b); \
		}                                                                 \
	} while (0)

#define ASSERT_STREQ(a, b)                                                          \
	do                                                                              \
	{                                                                               \
		g_test_run++;                                                               \
		if (strcmp((a), (b)) != 0)                                                  \
		{                                                                           \
			g_tests_failed++;                                                       \
			printf("FAIL %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
		}                                                                           \
	} while (0)

#define ASSERT_INT_EQ(a, b)                                                 \
	do                                                                      \
	{                                                                       \
		g_test_run++;                                                       \
		if ((a) != (b))                                                     \
		{                                                                   \
			g_tests_failed++;                                               \
			printf("FAIL %s:%d: %d != %d\n", __FILE__, __LINE__, (a), (b)); \
		}                                                                   \
	} while (0)

#define ASSERT_SIZE_EQ(a, b)                                                                    \
	do                                                                                          \
	{                                                                                           \
		g_test_run++;                                                                           \
		if ((a) != (b))                                                                         \
		{                                                                                       \
			g_tests_failed++;                                                                   \
			printf("FAIL %s:%d: %zu != %zu\n", __FILE__, __LINE__, (size_t) (a), (size_t) (b)); \
		}                                                                                       \
	} while (0)

#define ASSERT_PTR_EQ(a, b)                                                                   \
	do                                                                                        \
	{                                                                                         \
		g_test_run++;                                                                         \
		if ((a) != (b))                                                                       \
		{                                                                                     \
			g_tests_failed++;                                                                 \
			printf("FAIL %s:%d: %p != %p\n", __FILE__, __LINE__, (void *) (a), (void *) (b)); \
		}                                                                                     \
	} while (0)

#define ASSERT_NULL(a)                                                            \
	do                                                                            \
	{                                                                             \
		g_test_run++;                                                             \
		if ((a) != NULL)                                                          \
		{                                                                         \
			g_tests_failed++;                                                     \
			printf("FAIL %s:%d: %p != NULL\n", __FILE__, __LINE__, (void *) (a)); \
		}                                                                         \
	} while (0)

#define ASSERT_NOT_NULL(a)                                                        \
	do                                                                            \
	{                                                                             \
		g_test_run++;                                                             \
		if ((a) == NULL)                                                          \
		{                                                                         \
			g_tests_failed++;                                                     \
			printf("FAIL %s:%d: %p == NULL\n", __FILE__, __LINE__, (void *) (a)); \
		}                                                                         \
	} while (0)

#endif
