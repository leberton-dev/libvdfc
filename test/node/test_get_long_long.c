#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_long_long, valid)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"size\" \"9000000000000000000\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	long long ll = vdf_get_long_long(appstate, "size", -1);
	cr_assert_eq(ll, 9000000000000000000LL);
	vdf_free_node(out);
}

Test(get_long_long, invalid)
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
	long long ll = vdf_get_long_long(appstate, "size", -1);
	cr_assert_eq(ll, -1);
	vdf_free_node(out);
}
