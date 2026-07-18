#include <sanitizer/lsan_interface.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "test_utils.h"

int main(void)
{
	int   fd[2];
	pid_t pid;
	int   status;
	int   deltas[2];
	int   test_run = 0;
	int   test_failed = 0;
	int   use_color = isatty(STDOUT_FILENO);

	for (TestNode *node = g_test_list_head; node != NULL; node = node->next)
	{
		pipe(fd);
		// printf("TESTING %s\n", node->name);
		fflush(stdout);
		pid = fork();

		if (pid == 0)
		{
			int run_before = g_test_run;
			int failed_before = g_tests_failed;

			close(fd[0]);
			node->fn();
			deltas[0] = g_test_run - run_before;
			deltas[1] = g_tests_failed - failed_before;
			fflush(stdout);
			write(fd[1], deltas, sizeof(deltas));
			close(fd[1]);
			__lsan_do_recoverable_leak_check();
			_exit(0);
		}
		close(fd[1]);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			printf("%sCRASH%s %s (signal %d)\n", use_color ? RED : "", use_color ? RESET : "", node->name, WTERMSIG(status));
			test_failed++;
			test_run++;
		}
		else if (read(fd[0], deltas, sizeof(deltas)) == sizeof(deltas))
		{
			if (deltas[1] > 0)
				printf("%s[KO]%s ", use_color ? RED : "", use_color ? RESET : "");
			else
				printf("%s[OK]%s ", use_color ? GREEN : "", use_color ? RESET : "");
			printf("%s\n", node->name);
			g_test_run += deltas[0];
			g_tests_failed += deltas[1];
			test_run++;
			if (deltas[1] > 0)
				test_failed++;
		}
		close(fd[0]);
	}
	printf("%d/%d assertions passed\n", g_test_run - g_tests_failed, g_test_run);
	printf("%d/%d tests passed\n", test_run - test_failed, test_run);
	return (g_tests_failed != 0);
}
