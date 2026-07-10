#ifndef VDF_PARSER_H
#define VDF_PARSER_H

#include "errors.h"
#include "node.h"

/**
 * Parse a VDF buffer into a node tree.
 *
 * `buffer` is a null-terminated string containing the VDF content to parse.
 * `out` receives the root node on success. Set to NULL on failure.
 * the tree must be released with vdf_free_node().
 * returns `VDF_OK` on success, `VDF_ERR_PARSE` on malformed input, `VDF_ERR_MALLOC` on allocation failure.
 */
VDFcode vdf_parse(const char *buffer, VDFNode **out);

/**
 * Recursively free a node and all its descendants.
 */
void vdf_free_node(VDFNode *node);

#endif
