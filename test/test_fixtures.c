#include <criterion/criterion.h>

#include "vdfc/node.h"
#include "vdfc/vdf.h"

Test(fixtures, libraryfolders)
{
	char    *out;
	size_t   out_size;
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;

	err = vdf_parse(out, &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
	{
		free(out);
		return;
	}
	cr_assert_eq(root->type, VDF_VAL_OBJECT);
	cr_assert_eq(root->child_count, (size_t) 1);
	cr_assert_null(root->key);

	VDFNode *libraryfolders = vdf_get(root, "libraryfolders");
	cr_assert_not_null(libraryfolders);
	cr_assert_eq(libraryfolders->type, VDF_VAL_OBJECT);
	cr_assert_eq(libraryfolders->child_count, (size_t) 1);
	cr_assert_str_eq(libraryfolders->key, "libraryfolders");

	VDFNode *zero = vdf_get(libraryfolders, "0");
	cr_assert_not_null(zero);
	cr_assert_eq(zero->type, VDF_VAL_OBJECT);
	cr_assert_eq(zero->child_count, (size_t) 7);
	cr_assert_str_eq(zero->key, "0");

	cr_assert_str_eq(vdf_get_string(zero, "path", "error"), "/home/leberton/.local/"
	                                                         "share/Steam");
	cr_assert_str_eq(vdf_get_string(zero, "label", "error"), "");
	cr_assert_str_eq(vdf_get_string(zero, "contentid", "error"), "44625321883835991"
	                                                              "59");
	cr_assert_str_eq(vdf_get_string(zero, "totalsize", "error"), "0");
	cr_assert_str_eq(vdf_get_string(zero, "update_clean_bytes_tally", "error"), "14"
	                                                                             "81"
	                                                                             "28"
	                                                                             "79"
	                                                                             "7"
	                                                                             "1");
	cr_assert_str_eq(vdf_get_string(zero, "time_last_update_verified", "error"), "1"
	                                                                              "7"
	                                                                              "6"
	                                                                              "9"
	                                                                              "9"
	                                                                              "6"
	                                                                              "2"
	                                                                              "0"
	                                                                              "0"
	                                                                              "2");
	VDFNode *apps = vdf_get(zero, "apps");
	cr_assert_not_null(apps);
	cr_assert_eq(apps->type, VDF_VAL_OBJECT);
	cr_assert_eq(apps->child_count, (size_t) 3);
	cr_assert_str_eq(apps->key, "apps");

	cr_assert_str_eq(vdf_get_string(apps, "239820", "error"), "0");
	cr_assert_str_eq(vdf_get_string(apps, "1070560", "error"), "13004");
	cr_assert_str_eq(vdf_get_string(apps, "1391110", "error"), "0");

	vdf_free_node(root);
	free(out);
}
