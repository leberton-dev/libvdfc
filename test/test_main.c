#include <stdio.h>

#include "test_utils.h"

void test_lexer_suite(void);
void test_read_file_suite(void);
void test_parser_suite(void);

int main(void)
{
	test_read_file_suite();
	test_lexer_suite();
	test_parser_suite();

	printf("%d/%d tests passed\n", g_test_run - g_tests_failed, g_test_run);
	return (g_tests_failed != 0);
}
