#ifndef VDF_LEXER_H
#define VDF_LEXER_H

#include "token.h"

/**
 * Lexer state: a cursor over a null-terminated input buffer.
 */
typedef struct s_VDFLexer
{
	const char *cursor;
} VDFLexer;

/**
 * Read the next token from the input and advance the cursor.
 * Returns the next token. VDF_TOK_EOF at end of input, VDF_TOK_ERR on a lexing error.
 */
VDFToken vdf_next_token(VDFLexer *lexer);

#endif
