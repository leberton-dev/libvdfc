#include "vdfc/node.h"

#include <stdlib.h>
#include <string.h>

VDFNode *vdf_get(const VDFNode *object, const char *key)
{
	size_t i;

	if (!object || object->type != VDF_VAL_OBJECT)
		return (NULL);
	i = 0;
	while (i < object->child_count)
	{
		if (strcmp(object->children[i]->key, key) == 0)
			return (object->children[i]);
		i++;
	}
	return (NULL);
}

const char *vdf_get_string(const VDFNode *object, const char *key, const char *fallback)
{
	VDFNode *node;

	node = vdf_get(object, key);
	if (!node || node->type != VDF_VAL_STRING)
		return (fallback);
	return (node->string);
}

static int node_int_value(const VDFNode *node, int *out)
{
	char *endptr;
	long  value;

	if (!node || node->type != VDF_VAL_STRING || node->string[0] == '\0')
		return (0);
	value = strtol(node->string, &endptr, 10);
	if (*endptr != '\0')
		return (0);
	*out = (int) value;
	return (1);
}

int vdf_get_int(const VDFNode *object, const char *key, int fallback)
{
	int value;

	if (!node_int_value(vdf_get(object, key), &value))
		return (fallback);
	return (value);
}

int vdf_get_bool(const VDFNode *object, const char *key, int fallback)
{
	int value;

	if (!node_int_value(vdf_get(object, key), &value))
		return (fallback);
	return (value != 0);
}
