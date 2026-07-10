#ifndef VDF_TOKEN_H
#define VDF_TOKEN_H

#include <stddef.h>

typedef enum e_VDFTokenType
{
	VDF_TOK_NONE = 0,
	VDF_TOK_ERR,
	VDF_TOK_STRING,
	VDF_TOK_OPEN_BRACE,
	VDF_TOK_CLOSE_BRACE,
	VDF_TOK_EOF
} VDFTokenType;

typedef struct s_VDFToken
{
	VDFTokenType type;
	const char  *start;
	size_t       len;
} VDFToken;

#endif
