#ifndef VDF_PARSER_H
#define VDF_PARSER_H

#include "errors.h"
#include "node.h"

VDFcode vdf_parse(const char *buffer, VDFNode **out);
void    vdf_free_node(VDFNode *node);

#endif
