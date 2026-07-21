#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_long, valid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"size\" \"5000000000\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	long l = vdf_get_long(appstate, "size", -1);
	cr_assert_eq(l, 5000000000L);
	vdf_free_node(out);
}

Test(get_long, invalid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"size\" \"10abc\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	long l = vdf_get_long(appstate, "size", -1);
	cr_assert_eq(l, -1);
	vdf_free_node(out);
}
