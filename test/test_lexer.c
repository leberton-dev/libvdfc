#include "test_utils.h"
#include "vdfc/lexer.h"
#include "vdfc/token.h"

static void test_lexer_single_string(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"foo\"";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_EQ(token.len, (size_t) 3);
	ASSERT_EQ(strncmp(token.start, "foo", 3), 0);
}

static void test_lexer_braces(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "{ }";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_EOF);
}

static void test_lexer_key_value_pair(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"key\" \"value\"";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_EQ(token.len, (size_t) 3);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_EQ(token.len, (size_t) 5);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_EOF);
}

static void test_lexer_nested_object(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"outer\" { \"inner\" \"val\" }";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_OPEN_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_CLOSE_BRACE);
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_EOF);
}

static void test_lexer_unterminated_string(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"unterminated";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_ERR);
}

static void test_lexer_empty_input(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_EOF);
}

static void test_lexer_unknown_char(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "@";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_ERR);
}

static void test_lexer_escaped_quote(void)
{
	VDFLexer lexer;
	VDFToken token;

	lexer.cursor = "\"a \\\"quoted\\\" word\"";
	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_STRING);
	ASSERT_EQ(token.len, strlen("a \\\"quoted\\\" word"));
	ASSERT_EQ(strncmp(token.start, "a \\\"quoted\\\" word", token.len), 0);

	token = vdf_next_token(&lexer);
	ASSERT_EQ(token.type, VDF_TOK_EOF);
}

void test_lexer_suite(void)
{
	test_lexer_single_string();
	test_lexer_braces();
	test_lexer_key_value_pair();
	test_lexer_nested_object();
	test_lexer_unterminated_string();
	test_lexer_empty_input();
	test_lexer_unknown_char();
	test_lexer_escaped_quote();
}
