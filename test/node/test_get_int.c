#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

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
