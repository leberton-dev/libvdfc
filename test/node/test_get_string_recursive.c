#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_string_recursive, valid_direct_child)
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
	const char *str = vdf_get_string_recursive(appstate, "name", "unknown");
	cr_assert_str_eq(str, "division 2");
	vdf_free_node(out);
}

Test(get_string_recursive, valid_nested_child)
{
	VDFNode *out;
	VDFcode  err;

	err =
	    vdf_parse("\"AppState\" {\"lib\" {\"deep\" {\"name\" \"division 2\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	const char *str = vdf_get_string_recursive(appstate, "name", "unknown");
	cr_assert_str_eq(str, "division 2");
	vdf_free_node(out);
}

Test(get_string_recursive, not_found_returns_fallback)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"lib\" {\"deep\" {\"other\" \"x\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	const char *str = vdf_get_string_recursive(appstate, "name", "unknown");
	cr_assert_str_eq(str, "unknown");
	vdf_free_node(out);
}

Test(get_string_recursive, null_object_returns_fallback)
{
	const char *str = vdf_get_string_recursive(NULL, "name", "unknown");
	cr_assert_str_eq(str, "unknown");
}
