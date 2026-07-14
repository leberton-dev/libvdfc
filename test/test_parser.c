#include "test_utils.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

TEST(parser_simple_pair)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" \"value\"", &root);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_INT_EQ(root->type, VDF_VAL_OBJECT);
	ASSERT_SIZE_EQ(root->child_count, 1);
	ASSERT_STREQ(root->children[0]->key, "key");
	ASSERT_INT_EQ(root->children[0]->type, VDF_VAL_STRING);
	ASSERT_STREQ(root->children[0]->string, "value");
	vdf_free_node(root);
}

TEST(parser_multiple_pairs)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"a\" \"1\" \"b\" \"2\"", &root);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_SIZE_EQ(root->child_count, 2);
	ASSERT_STREQ(root->children[0]->key, "a");
	ASSERT_STREQ(root->children[0]->string, "1");
	ASSERT_STREQ(root->children[1]->key, "b");
	ASSERT_STREQ(root->children[1]->string, "2");
	vdf_free_node(root);
}

TEST(parser_nested_object)
{
	VDFNode *root = NULL;
	VDFNode *outer;
	VDFcode  err;

	err = vdf_parse("\"parent\" { \"child\" \"value\" }", &root);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_SIZE_EQ(root->child_count, 1);
	outer = root->children[0];
	ASSERT_STREQ(outer->key, "parent");
	ASSERT_INT_EQ(outer->type, VDF_VAL_OBJECT);
	ASSERT_SIZE_EQ(outer->child_count, 1);
	ASSERT_STREQ(outer->children[0]->key, "child");
	ASSERT_INT_EQ(outer->children[0]->type, VDF_VAL_STRING);
	ASSERT_STREQ(outer->children[0]->string, "value");
	vdf_free_node(root);
}

TEST(parser_empty_input)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("", &root);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_SIZE_EQ(root->child_count, 0);
	vdf_free_node(root);
}

TEST(parser_missing_value)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\"", &root);
	ASSERT_INT_EQ(err, VDF_ERR_PARSE);
	ASSERT_NULL(root);
}

TEST(parser_missing_key)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("{ \"a\" \"1\" }", &root);
	ASSERT_INT_EQ(err, VDF_ERR_PARSE);
	ASSERT_NULL(root);
}

TEST(parser_unclosed_brace)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"outer\" {", &root);
	ASSERT_INT_EQ(err, VDF_ERR_PARSE);
	ASSERT_NULL(root);
}

TEST(parser_stray_close_brace)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("}", &root);
	ASSERT_INT_EQ(err, VDF_ERR_PARSE);
	ASSERT_NULL(root);
}

TEST(parser_error_after_sibling)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"outer\" { \"a\" \"1\" \"b\" }", &root);
	ASSERT_INT_EQ(err, VDF_ERR_PARSE);
	ASSERT_NULL(root);
}

TEST(parser_unescapes_quotes)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" \"a \\\"quoted\\\" word\"", &root);
	ASSERT_INT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_SIZE_EQ(root->child_count, 1);
	ASSERT_STREQ(root->children[0]->string, "a \"quoted\" word");
	vdf_free_node(root);
}
