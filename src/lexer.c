#include "vdfc/lexer.h"
#include "vdfc/token.h"
#include <stdio.h>

static int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static VDFToken lex_quoted_string(VDFLexer *lexer)
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

static VDFToken lex_single_comment(VDFLexer *lexer)
{
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

static VDFToken lex_multi_comment(VDFLexer *lexer)
{
	const char *start = ++lexer->cursor;
	size_t      len = 0;

	while (start[len] != '\0' && !(start[len] == '*' && start[len + 1] == '/'))
		len++;
	if (start[len] == '\0')
		return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
	lexer->cursor = start + len + 2;
	return ((VDFToken) {VDF_TOK_MULTI_COMMENT, start, len});
}

static VDFToken lex_comment(VDFLexer *lexer)
{
	lexer->cursor++;
	if (*lexer->cursor == '/')
		return (lex_single_comment(lexer));
	else if (*lexer->cursor == '*')
		return (lex_multi_comment(lexer));
	else
		return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
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
		return (lex_quoted_string(lexer));
	if (*lexer->cursor == '/')
		return (lex_comment(lexer));
	return ((VDFToken) {VDF_TOK_ERR, NULL, 0});
}
