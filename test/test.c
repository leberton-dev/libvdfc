#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/vdf.h"

int main(void)
{
	char  *out;
	size_t out_size;

	VDFcode err = read_entire_file("test/fixtures/libraryfolders.vdf", &out, &out_size);

	printf("%d\n", err);
	printf("%s", out);

	free(out);

	return (EXIT_SUCCESS);
}
