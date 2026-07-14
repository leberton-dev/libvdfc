#include "test_utils.h"
#include "vdfc/lexer.h"
#include "vdfc/token.h"

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
