#include "vdfc/node.h"

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
