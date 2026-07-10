#ifndef VDF_NODE_H
#define VDF_NODE_H

#include <stddef.h>

/**
 * 'String' or 'Object' value held by a VDFNode.
 */
typedef enum e_VDFValueType
{
	VDF_VAL_STRING = 0, /* node type is a string: `string` is valid */
	VDF_VAL_OBJECT /* node type is a nested object: `children` and `child_count` are valid */
} VDFValueType;

/**
 * A node in the parsed VDF tree.
 *
 * Owns all its memory: `key`, `string`, `children` and every node inside
 * `children` are heap-allocated and freed together by vdf_free_node().
 */
typedef struct s_VDFNode
{
	char        *key; /* key string. NULL only for the root node */
	VDFValueType type;
	char        *string; /* value string, valid when type == VDF_VAL_STRING */
	struct s_VDFNode **children; /* children nodes, valid when type == VDF_VAL_OBJECT */
	size_t child_count;
} VDFNode;

#endif
