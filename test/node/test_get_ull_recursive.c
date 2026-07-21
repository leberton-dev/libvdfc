#include <criterion/criterion.h>

#include "vdfc/errors.h"
#include "vdfc/node.h"
#include "vdfc/parser.h"

Test(get_ull_recursive, valid_direct_child)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"size\" \"18446744073709551615\"}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	unsigned long long ull = vdf_get_ull_recursive(appstate, "size", 0);
	cr_assert_eq(ull, 18446744073709551615ULL);
	vdf_free_node(out);
}

Test(get_ull_recursive, valid_nested_child)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse(
		"\"AppState\" {\"lib\" {\"deep\" {\"size\" \"18446744073709551615\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	unsigned long long ull = vdf_get_ull_recursive(appstate, "size", 0);
	cr_assert_eq(ull, 18446744073709551615ULL);
	vdf_free_node(out);
}

Test(get_ull_recursive, invalid_value_returns_fallback)
{
	VDFNode *out;
	VDFcode  err;

	err = vdf_parse("\"AppState\" {\"lib\" {\"deep\" {\"size\" \"10abc\"}}}", &out);
	if (err != VDF_OK)
		return;
	VDFNode *appstate = vdf_get(out, "AppState");
	if (!appstate)
	{
		vdf_free_node(out);
		return;
	}
	unsigned long long ull = vdf_get_ull_recursive(appstate, "size", 42);
	cr_assert_eq(ull, 42ULL);
	vdf_free_node(out);
}

Test(get_ull_recursive, not_found_returns_fallback)
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
	unsigned long long ull = vdf_get_ull_recursive(appstate, "size", 42);
	cr_assert_eq(ull, 42ULL);
	vdf_free_node(out);
}

Test(get_ull_recursive, null_object_returns_fallback)
{
	unsigned long long ull = vdf_get_ull_recursive(NULL, "size", 42);
	cr_assert_eq(ull, 42ULL);
}
