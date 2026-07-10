#include "../include/vdf_token.h"

static int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

VDFToken vdf_next_token(VDFLexer *lexer)
{
	while (is_whitespace(*lexer->cursor))
		lexer->cursor++;
	if (*lexer->cursor == '\0')
		return ((VDFToken) {VDF_TOK_EOF, NULL, 0});
	if (*lexer->cursor == '{')
	{
		lexer->cursor++;
		return ((VDFToken) {VDF_TOK_OPEN_BRACE, NULL, 0});
	}
	if (*lexer->cursor == '}')
	{
		lexer->cursor++;
		return ((VDFToken) {VDF_TOK_CLOSE_BRACE, NULL, 0});
	}
	if (*lexer->cursor == '"')
	{
		const char *start = lexer->cursor + 1;
		size_t      len = 0;

		while (start[len] != '"' && start[len] != '\0')
			len++;
		if (start[len] == '\0')
			return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
		lexer->cursor = start + len + 1;
		return ((VDFToken) {VDF_TOK_STRING, start, len});
	}
	return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
}
