#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <string.h>

extern int g_test_run;
extern int g_tests_failed;

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

#endif
