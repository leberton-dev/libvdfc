#ifndef VDF_H
#define VDF_H

#include <stdlib.h>

#include "errors.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

VDFcode vdf_read_file(const char *path, char **out, size_t *out_size);

#endif
