#ifndef VDF_LEXER_H
#define VDF_LEXER_H

#include "token.h"

typedef struct s_VDFLexer
{
	const char *cursor;
} VDFLexer;

VDFToken vdf_next_token(VDFLexer *lexer);

#endif
