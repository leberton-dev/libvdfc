#ifndef VDF_H
#define VDF_H

#include <stdlib.h>

#include "errors.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

/**
 * Read an entire file into a newly allocated, null-terminated buffer.
 *
 * `path`     Path to the file to read.
 * `out`      Receives the allocated buffer on success. Set to NULL on failure.
 * Caller must free() the buffer. `out_size` is optional. If non-NULL, receives
 * the file size in bytes (excluding the trailing null terminator). returns
 * VDF_OK on success, VDF_ERR_OPEN if the file cannot be opened, VDF_ERR_IO on a
 * read/seek error, VDF_ERR_MALLOC on allocation failure.
 */
VDFcode vdf_read_file(const char *path, char **out, size_t *out_size);

#endif
