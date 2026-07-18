#include <stdlib.h>
#include <string.h>

#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/vdf.h"

Test(read_file, existing_file)
{
	char   *out;
	size_t  out_size;
	VDFcode err;

	err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(out_size, (size_t) 330);
	cr_assert_arr_eq(out, "\"libraryfolders\"", strlen("\"libraryfolders\""));
	free(out);
}

Test(read_file, missing_file)
{
	char   *out;
	size_t  out_size;
	VDFcode err;

	err = vdf_read_file("test/fixtures/does_not_exist.vdf", &out, &out_size);
	cr_assert_eq(err, VDF_ERR_OPEN);
	cr_assert_null(out);
}
