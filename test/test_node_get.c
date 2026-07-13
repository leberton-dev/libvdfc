#include "test_utils.h"

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

void test_node_get_str(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"name\" \"division 2\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
		return;
	const char *str = vdf_get_string(appstate, "name", "unknown");
	ASSERT_STREQ(str, "division 2");
}

void test_node_get_int_valid(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"appid\" \"10\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
		return;
	int i = vdf_get_int(appstate, "appid", -1);
	ASSERT_EQ(i, 10);
}

void test_node_get_int_invalid(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"appid\" \"10abc\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
		return;
	int i = vdf_get_int(appstate, "appid", -1);
	ASSERT_EQ(i, -1);
}

void test_node_get_bool_valid_true(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"1\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
		return;
	int i = vdf_get_bool(appstate, "activated", -1);
	ASSERT_EQ(i, 1);
}

void test_node_get_bool_valid_false(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"0\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
		return;
	int i = vdf_get_bool(appstate, "activated", -1);
	ASSERT_EQ(i, 0);
}

void test_node_get_bool_invalid(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"abc\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
		return;
	int i = vdf_get_bool(appstate, "activated", -1);
	ASSERT_EQ(i, -1);
}

void test_node_get_bool_valid_fallback_same_as_val(void)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"5\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
		return;
	int i = vdf_get_bool(appstate, "activated", 5);
	ASSERT_EQ(i, 1);
}

void test_node_get_suite(void)
{
	test_node_get_str();
	test_node_get_int_valid();
	test_node_get_int_invalid();
	test_node_get_bool_valid_true();
	test_node_get_bool_valid_false();
	test_node_get_bool_invalid();
	test_node_get_bool_valid_fallback_same_as_val();
}
