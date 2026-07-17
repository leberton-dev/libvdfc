#include "vdfc/lexer.h"
#include "vdfc/token.h"
#include <stdio.h>

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
		{
			if (start[len] == '\\' && start[len + 1] != '\0')
				len++;
			len++;
		}
		if (start[len] == '\0')
			return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
		lexer->cursor = start + len + 1;
		return ((VDFToken) {VDF_TOK_STRING, start, len});
	}
	// lex comments
	if (*lexer->cursor == '/')
	{
		lexer->cursor++;
		if (*lexer->cursor != '/')
			return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
		const char *start = ++lexer->cursor;
		size_t      len = 0;
		while (start[len] != '\n' && start[len] != '\0' && start[len + 1] != '"')
			len++;
		if (start[len] == '\0')
			lexer->cursor = start + len;
		else
			lexer->cursor = start + len + 1;
		return ((VDFToken) {VDF_TOK_SINGLE_COMMENT, start, len});
	}
	return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
}
