#include <criterion/criterion.h>

#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(parser, simple_pair)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" \"value\"", &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(root->type, VDF_VAL_OBJECT);
	cr_assert_eq(root->child_count, (size_t) 1);
	cr_assert_str_eq(root->children[0]->key, "key");
	cr_assert_eq(root->children[0]->type, VDF_VAL_STRING);
	cr_assert_str_eq(root->children[0]->string, "value");
	vdf_free_node(root);
}

Test(parser, multiple_pairs)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"a\" \"1\" \"b\" \"2\"", &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(root->child_count, (size_t) 2);
	cr_assert_str_eq(root->children[0]->key, "a");
	cr_assert_str_eq(root->children[0]->string, "1");
	cr_assert_str_eq(root->children[1]->key, "b");
	cr_assert_str_eq(root->children[1]->string, "2");
	vdf_free_node(root);
}

Test(parser, nested_object)
{
	VDFNode *root = NULL;
	VDFNode *outer;
	VDFcode  err;

	err = vdf_parse("\"parent\" { \"child\" \"value\" }", &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(root->child_count, (size_t) 1);
	outer = root->children[0];
	cr_assert_str_eq(outer->key, "parent");
	cr_assert_eq(outer->type, VDF_VAL_OBJECT);
	cr_assert_eq(outer->child_count, (size_t) 1);
	cr_assert_str_eq(outer->children[0]->key, "child");
	cr_assert_eq(outer->children[0]->type, VDF_VAL_STRING);
	cr_assert_str_eq(outer->children[0]->string, "value");
	vdf_free_node(root);
}

Test(parser, empty_input)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("", &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(root->child_count, (size_t) 0);
	vdf_free_node(root);
}

Test(parser, missing_value)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\"", &root);
	cr_assert_eq(err, VDF_ERR_PARSE);
	cr_assert_null(root);
}

Test(parser, missing_key)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("{ \"a\" \"1\" }", &root);
	cr_assert_eq(err, VDF_ERR_PARSE);
	cr_assert_null(root);
}

Test(parser, unclosed_brace)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"outer\" {", &root);
	cr_assert_eq(err, VDF_ERR_PARSE);
	cr_assert_null(root);
}

Test(parser, stray_close_brace)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("}", &root);
	cr_assert_eq(err, VDF_ERR_PARSE);
	cr_assert_null(root);
}

Test(parser, error_after_sibling)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"outer\" { \"a\" \"1\" \"b\" }", &root);
	cr_assert_eq(err, VDF_ERR_PARSE);
	cr_assert_null(root);
}

Test(parser, unescapes_quotes)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" \"a \\\"quoted\\\" word\"", &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(root->child_count, (size_t) 1);
	cr_assert_str_eq(root->children[0]->string, "a \"quoted\" word");
	vdf_free_node(root);
}

Test(parser, skips_comments)
{
	VDFNode *root = NULL;
	VDFcode  err;

	err = vdf_parse("\"key\" // comment \"value\"", &root);
	cr_assert_eq(err, VDF_OK);
	if (err != VDF_OK)
		return;
	cr_assert_eq(root->child_count, (size_t) 1);
	cr_assert_str_eq(root->children[0]->string, "value");
	vdf_free_node(root);
}
