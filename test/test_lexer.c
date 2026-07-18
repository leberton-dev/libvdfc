#include "vdfc/lexer.h"
#include "vdfc/token.h"

#include <criterion/criterion.h>

Test(lexer, quoted_string)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"foo\"";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	cr_assert_eq(token.len, 3);
	cr_assert_arr_eq(token.start, "foo", 3);
}

Test(lexer, braces)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "{ }";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, key_value_pair)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"key\" \"value\"";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	cr_assert_eq(token.len, 3);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	cr_assert_eq(token.len, 5);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, nested_object)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\" { \"inner\" \"val\" }";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, unterminated_string)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"unterminated";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_ERR);
}

Test(lexer, empty_input)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, unknown_char)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "@";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_ERR);
}

Test(lexer, escaped_quote)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"a \\\"quoted\\\" word\"";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_STRING);
	cr_assert_eq(token.len, strlen("a \\\"quoted\\\" word"));
	cr_assert_arr_eq(token.start, "a \\\"quoted\\\" word", token.len);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, single_comment_ends_at_eof)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "// This is a commented line";
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_SINGLE_COMMENT);
	cr_assert_eq(token.len, strlen(" This is a commented line"));
	cr_assert_arr_eq(token.start, " This is a commented line", token.len);
	token = vdf_next_token(&lexer);
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, single_comment_before_string_same_line)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\" { \"inner\" // This is a commented line \"val\" "
	               "}";
	token = vdf_next_token(&lexer); // "outer"
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "{"
	cr_assert_eq(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer); // "inner"
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "comment"
	cr_assert_eq(token.type, VDF_TOK_SINGLE_COMMENT);
	cr_assert_eq(token.len, strlen(" This is a commented line"));
	cr_assert_arr_eq(token.start, " This is a commented line", token.len);
	token = vdf_next_token(&lexer); // "val"
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "}"
	cr_assert_eq(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer); // '\0'
	cr_assert_eq(token.type, VDF_TOK_EOF);
}

Test(lexer, single_comment_ends_at_newline)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\"\n{\n \"inner\" // This is a commented line\n "
	               "\"val\"\n }";
	token = vdf_next_token(&lexer); // "outer"
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "{"
	cr_assert_eq(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer); // "inner"
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "comment"
	cr_assert_eq(token.type, VDF_TOK_SINGLE_COMMENT);
	cr_assert_eq(token.len, strlen(" This is a commented line"));
	cr_assert_arr_eq(token.start, " This is a commented line", token.len);
	token = vdf_next_token(&lexer); // "val"
	cr_assert_eq(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "}"
	cr_assert_eq(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer); // '\0'
	cr_assert_eq(token.type, VDF_TOK_EOF);
}
