#include <stdlib.h>

#include "test_utils.h"
#include "vdfc/errors.h"
#include "vdfc/vdf.h"

static void test_read_file(void)
{
	char   *out;
	size_t  out_size;
	VDFcode err;

	err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);
	ASSERT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_EQ(out_size, (size_t) 309);
	ASSERT_EQ(strncmp(out, "\"libraryfolders\"", strlen("\"libraryfolders\"")), 0);
	free(out);
}

static void test_read_file_missing(void)
{
	char   *out;
	size_t  out_size;
	VDFcode err;

	err = vdf_read_file("test/fixtures/does_not_exist.vdf", &out, &out_size);
	ASSERT_EQ(err, VDF_ERR_OPEN);
	ASSERT_EQ(out, NULL);
}

void test_read_file_suite(void)
{
	test_read_file();
	test_read_file_missing();
}
