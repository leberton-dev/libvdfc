#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

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
