#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_bool_recursive, valid_true)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"lib\" {\"deep\" {\"activated\" \"1\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool_recursive(appstate, "activated", -1);
	cr_assert_eq(i, 1);
	vdf_free_node(out);
}

Test(get_bool_recursive, valid_false)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"lib\" {\"deep\" {\"activated\" \"0\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool_recursive(appstate, "activated", -1);
	cr_assert_eq(i, 0);
	vdf_free_node(out);
}

Test(get_bool_recursive, invalid_value_returns_fallback)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"lib\" {\"deep\" {\"activated\" \"abc\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool_recursive(appstate, "activated", -1);
	cr_assert_eq(i, -1);
	vdf_free_node(out);
}

Test(get_bool_recursive, normalizes_nonzero_to_one)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"lib\" {\"deep\" {\"activated\" \"5\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool_recursive(appstate, "activated", 5);
	cr_assert_eq(i, 1);
	vdf_free_node(out);
}

Test(get_bool_recursive, not_found_returns_fallback)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"appstate\" {\"lib\" {\"deep\" {\"other\" \"1\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "appstate");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	int i = vdf_get_bool_recursive(appstate, "activated", -1);
	cr_assert_eq(i, -1);
	vdf_free_node(out);
}

Test(get_bool_recursive, null_object_returns_fallback)
{
	int i = vdf_get_bool_recursive(NULL, "activated", -1);
	cr_assert_eq(i, -1);
}
