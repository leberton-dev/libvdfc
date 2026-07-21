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
