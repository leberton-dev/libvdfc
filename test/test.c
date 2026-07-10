#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "vdfc/vdf.h"

static void test_read_entire_file(void)
{
	char  *out;
	size_t out_size;

	VDFcode err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);

	if (err != VDF_OK)
	{
		printf("Error reading file: %d\n", err);
		return;
	}

	printf("File size: %zu\n", out_size);
	printf("File contents:\n%s\n", out);

	free(out);
}

static char *token_type_to_string(VDFTokenType type)
{
	switch (type)
	{
	case VDF_TOK_NONE:
		return "NONE";
	case VDF_TOK_ERR:
		return "ERR";
	case VDF_TOK_STRING:
		return "STRING";
	case VDF_TOK_OPEN_BRACE:
		return "OPEN_BRACE";
	case VDF_TOK_CLOSE_BRACE:
		return "CLOSE_BRACE";
	case VDF_TOK_EOF:
		return "EOF";
	default:
		return "UNKNOWN";
	}
}

static void test_lexer(void)
{
	VDFToken tok;
	VDFLexer lexer;

	char  *out;
	size_t out_size;

	VDFcode err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);

	if (err != VDF_OK)
	{
		printf("Error reading file: %d\n", err);
		return;
	}

	lexer.cursor = out;

	tok = vdf_next_token(&lexer);
	while (tok.type != VDF_TOK_EOF && tok.type != VDF_TOK_ERR)
	{
		printf("Token type: %s, start: %.*s, len: %zu\n", token_type_to_string(tok.type),
		       (int) tok.len, tok.start, tok.len);
		tok = vdf_next_token(&lexer);
	}
	if (tok.type == VDF_TOK_ERR)
		printf("lexing error\n");
}

int main(void)
{
	printf("=== TESTING READ ENTIRE FILE ===\n");
	test_read_entire_file();
	printf("=== TESTING LEXER ===\n");
	test_lexer();
	return (EXIT_SUCCESS);
}
