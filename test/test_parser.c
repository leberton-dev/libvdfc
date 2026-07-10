#include "test_utils.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

static void test_parser_simple_pair(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" \"value\"", &root);
	ASSERT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_EQ(root->type, VDF_VAL_OBJECT);
	ASSERT_EQ(root->child_count, (size_t) 1);
	ASSERT_STREQ(root->children[0]->key, "key");
	ASSERT_EQ(root->children[0]->type, VDF_VAL_STRING);
	ASSERT_STREQ(root->children[0]->string, "value");
	vdf_free_node(root);
}

static void test_parser_multiple_pairs(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"a\" \"1\" \"b\" \"2\"", &root);
	ASSERT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_EQ(root->child_count, (size_t) 2);
	ASSERT_STREQ(root->children[0]->key, "a");
	ASSERT_STREQ(root->children[0]->string, "1");
	ASSERT_STREQ(root->children[1]->key, "b");
	ASSERT_STREQ(root->children[1]->string, "2");
	vdf_free_node(root);
}

static void test_parser_nested_object(void)
{
	VDFNode *root = NULL;
	VDFNode *outer;
	VDFcode  err;

	err = vdf_parse("\"parent\" { \"child\" \"value\" }", &root);
	ASSERT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_EQ(root->child_count, (size_t) 1);
	outer = root->children[0];
	ASSERT_STREQ(outer->key, "parent");
	ASSERT_EQ(outer->type, VDF_VAL_OBJECT);
	ASSERT_EQ(outer->child_count, (size_t) 1);
	ASSERT_STREQ(outer->children[0]->key, "child");
	ASSERT_EQ(outer->children[0]->type, VDF_VAL_STRING);
	ASSERT_STREQ(outer->children[0]->string, "value");
	vdf_free_node(root);
}

static void test_parser_empty_input(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("", &root);
	ASSERT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_EQ(root->child_count, (size_t) 0);
	vdf_free_node(root);
}

static void test_parser_missing_value(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\"", &root);
	ASSERT_EQ(err, VDF_ERR_PARSE);
	ASSERT_EQ(root, NULL);
}

static void test_parser_missing_key(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("{ \"a\" \"1\" }", &root);
	ASSERT_EQ(err, VDF_ERR_PARSE);
	ASSERT_EQ(root, NULL);
}

static void test_parser_unclosed_brace(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"outer\" {", &root);
	ASSERT_EQ(err, VDF_ERR_PARSE);
	ASSERT_EQ(root, NULL);
}

static void test_parser_stray_close_brace(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("}", &root);
	ASSERT_EQ(err, VDF_ERR_PARSE);
	ASSERT_EQ(root, NULL);
}

static void test_parser_error_after_sibling(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"outer\" { \"a\" \"1\" \"b\" }", &root);
	ASSERT_EQ(err, VDF_ERR_PARSE);
	ASSERT_EQ(root, NULL);
}

static void test_parser_unescapes_quotes(void)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" \"a \\\"quoted\\\" word\"", &root);
	ASSERT_EQ(err, VDF_OK);
	if (err != VDF_OK)
		return;
	ASSERT_EQ(root->child_count, (size_t) 1);
	ASSERT_STREQ(root->children[0]->string, "a \"quoted\" word");
	vdf_free_node(root);
}

void test_parser_suite(void)
{
	test_parser_simple_pair();
	test_parser_multiple_pairs();
	test_parser_nested_object();
	test_parser_empty_input();
	test_parser_missing_value();
	test_parser_missing_key();
	test_parser_unclosed_brace();
	test_parser_stray_close_brace();
	test_parser_error_after_sibling();
	test_parser_unescapes_quotes();
}
