#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_int_recursive, valid_direct_child)
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
	int i = vdf_get_int_recursive(appstate, "appid", -1);
	cr_assert_eq(i, 10);
	vdf_free_node(out);
}

Test(get_int_recursive, valid_nested_child)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"lib\" {\"deep\" {\"appid\" \"10\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_int_recursive(appstate, "appid", -1);
	cr_assert_eq(i, 10);
	vdf_free_node(out);
}

Test(get_int_recursive, invalid_value_returns_fallback)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"lib\" {\"deep\" {\"appid\" \"10abc\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_int_recursive(appstate, "appid", -1);
	cr_assert_eq(i, -1);
	vdf_free_node(out);
}

Test(get_int_recursive, not_found_returns_fallback)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"lib\" {\"deep\" {\"other\" \"1\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_int_recursive(appstate, "appid", -1);
	cr_assert_eq(i, -1);
	vdf_free_node(out);
}

Test(get_int_recursive, null_object_returns_fallback)
{
	int i = vdf_get_int_recursive(NULL, "appid", -1);
	cr_assert_eq(i, -1);
}
