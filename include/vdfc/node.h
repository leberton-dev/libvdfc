#ifndef VDF_NODE_H
#define VDF_NODE_H

#include <stddef.h>

typedef enum e_VDFValueType
{
	VDF_VAL_STRING = 0,
	VDF_VAL_OBJECT
} VDFValueType;

typedef struct s_VDFNode VDFNode;

struct s_VDFNode
{
	char        *key;
	VDFValueType type;
	char        *string;
	VDFNode    **children;
	size_t       child_count;
};

#endif
