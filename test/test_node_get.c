#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_string, valid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"name\" \"division 2\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	const char *str = vdf_get_string(appstate, "name", "unknown");
	cr_assert_str_eq(str, "division 2");
	vdf_free_node(out);
}

Test(get_int, valid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"appid\" \"10\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_int(appstate, "appid", -1);
	cr_assert_eq(i, 10);
	vdf_free_node(out);
}

Test(get_int, invalid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"appid\" \"10abc\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_int(appstate, "appid", -1);
	cr_assert_eq(i, -1);
	vdf_free_node(out);
}

Test(get_bool, valid_true)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"1\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool(appstate, "activated", -1);
	cr_assert_eq(i, 1);
	vdf_free_node(out);
}

Test(get_bool, valid_false)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"0\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool(appstate, "activated", -1);
	cr_assert_eq(i, 0);
	vdf_free_node(out);
}

Test(get_bool, invalid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"abc\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool(appstate, "activated", -1);
	cr_assert_eq(i, -1);
	vdf_free_node(out);
}

Test(get_bool, normalizes_nonzero_to_one)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"activated\" \"5\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool(appstate, "activated", 5);
	cr_assert_eq(i, 1);
	vdf_free_node(out);
}
