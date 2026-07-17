#include "test_utils.h"
#include "vdfc/lexer.h"
#include "vdfc/token.h"
#include <stdio.h>

TEST(lexer_single_string)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"foo\"";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_SIZE_EQ(token.len, 3);
	ASSERT_EQ(strncmp(token.start, "foo", 3), 0);
}

TEST(lexer_braces)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "{ }";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_key_value_pair)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"key\" \"value\"";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_SIZE_EQ(token.len, 3);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_SIZE_EQ(token.len, 5);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_nested_object)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\" { \"inner\" \"val\" }";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_unterminated_string)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"unterminated";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_ERR);
}

TEST(lexer_empty_input)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_unknown_char)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "@";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_ERR);
}

TEST(lexer_escaped_quote)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"a \\\"quoted\\\" word\"";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_SIZE_EQ(token.len, strlen("a \\\"quoted\\\" word"));
	ASSERT_EQ(strncmp(token.start, "a \\\"quoted\\\" word", token.len), 0);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_catches_single_line_comment_eof)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "// This is a commented line";
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_SINGLE_COMMENT);
	ASSERT_SIZE_EQ(token.len, strlen(" This is a commented line"));
	ASSERT_EQ(strncmp(token.start, " This is a commented line", token.len), 0);
	token = vdf_next_token(&lexer);
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_catches_single_line_comment_inside_object)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\" { \"inner\" // This is a commented line \"val\" "
	               "}";
	token = vdf_next_token(&lexer); // "outer"
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "{"
	ASSERT_INT_EQ(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer); // "inner"
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "comment"
	ASSERT_INT_EQ(token.type, VDF_TOK_SINGLE_COMMENT);
	ASSERT_SIZE_EQ(token.len, strlen(" This is a commented line"));
	ASSERT_EQ(strncmp(token.start, " This is a commented line", token.len), 0);
	token = vdf_next_token(&lexer); // "val"
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "}"
	ASSERT_INT_EQ(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer); // '\0'
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}

TEST(lexer_catches_single_line_comment_on_newline)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\"\n{\n \"inner\" // This is a commented line\n "
	               "\"val\"\n }";
	token = vdf_next_token(&lexer); // "outer"
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "{"
	ASSERT_INT_EQ(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer); // "inner"
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "comment"
	ASSERT_INT_EQ(token.type, VDF_TOK_SINGLE_COMMENT);
	ASSERT_SIZE_EQ(token.len, strlen(" This is a commented line"));
	ASSERT_EQ(strncmp(token.start, " This is a commented line", token.len), 0);
	token = vdf_next_token(&lexer); // "val"
	ASSERT_INT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer); // "}"
	ASSERT_INT_EQ(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer); // '\0'
	ASSERT_INT_EQ(token.type, VDF_TOK_EOF);
}
