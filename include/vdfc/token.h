#ifndef VDF_TOKEN_H
#define VDF_TOKEN_H

#include <stddef.h>

/**
 * Token type produced by the lexer.
 */
typedef enum e_VDFTokenType
{
	VDF_TOK_NONE = 0,       /* unused / uninitialized token */
	VDF_TOK_ERR,            /* lexing error */
	VDF_TOK_STRING,         /* a quoted string */
	VDF_TOK_OPEN_BRACE,     /* '{' */
	VDF_TOK_CLOSE_BRACE,    /* '}' */
	VDF_TOK_SINGLE_COMMENT, /* '// until eol' */
	VDF_TOK_MULTI_COMMENT,  /* '/\* until *\/' */
	VDF_TOK_EOF             /* end of input */
} VDFTokenType;

/**
 * A single lexical token.
 *
 * For VDF_TOK_STRING, `start` and `len` point into the original input buffer
 * and are NOT null-terminated. The buffer must stay alive as long as the token
 * is used. For every other token type, `start` is NULL and `len` is 0.
 */
typedef struct s_VDFToken
{
	VDFTokenType type;
	const char  *start;
	size_t       len;
} VDFToken;

#endif
