#include "test_utils.h"
#include "vdfc/node.h"
#include "vdfc/vdf.h"

TEST(real_test_libraryfolders)
{
	char    *out;
	size_t   out_size;
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;

	err = vdf_parse(out, &root);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_INT_EQ(root->type, VDF_VAL_OBJECT);
	ASSERT_SIZE_EQ(root->child_count, 1);
	ASSERT_NULL(root->key);

	VDFNode *libraryfolders = vdf_get(root, "libraryfolders");
	ASSERT_NOT_NULL(libraryfolders);
	ASSERT_INT_EQ(libraryfolders->type, VDF_VAL_OBJECT);
	ASSERT_SIZE_EQ(libraryfolders->child_count, 1);
	ASSERT_STREQ(libraryfolders->key, "libraryfolders");

	VDFNode *zero = vdf_get(libraryfolders, "0");
	ASSERT_NOT_NULL(zero);
	ASSERT_INT_EQ(zero->type, VDF_VAL_OBJECT);
	ASSERT_SIZE_EQ(zero->child_count, 7);
	ASSERT_STREQ(zero->key, "0");

	ASSERT_STREQ(vdf_get_string(zero, "path", "error"), "/home/leberton/.local/"
	                                                    "share/Steam");
	ASSERT_STREQ(vdf_get_string(zero, "label", "error"), "");
	ASSERT_STREQ(vdf_get_string(zero, "contentid", "error"), "44625321883835991"
	                                                         "59");
	ASSERT_STREQ(vdf_get_string(zero, "totalsize", "error"), "0");
	ASSERT_STREQ(vdf_get_string(zero, "update_clean_bytes_tally", "error"), "14"
	                                                                        "81"
	                                                                        "28"
	                                                                        "79"
	                                                                        "7"
	                                                                        "1");
	ASSERT_STREQ(vdf_get_string(zero, "time_last_update_verified", "error"), "1"
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
	ASSERT_NOT_NULL(apps);
	ASSERT_INT_EQ(apps->type, VDF_VAL_OBJECT);
	ASSERT_SIZE_EQ(apps->child_count, 3);
	ASSERT_STREQ(apps->key, "apps");

	ASSERT_STREQ(vdf_get_string(apps, "239820", "error"), "0");
	ASSERT_STREQ(vdf_get_string(apps, "1070560", "error"), "13004");
	ASSERT_STREQ(vdf_get_string(apps, "1391110", "error"), "0");

	vdf_free_node(root);
	free(out);
}
