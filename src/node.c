#include "vdfc/node.h"
#include "vdfc/errors.h"

#include <stddef.h>
#include <stdio.h>
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

const char *vdf_get_string_recursive(const VDFNode *object, const char *key, const char *fallback)
{
	const char *found;
	VDFNode    *node;

	if (!object || object->type != VDF_VAL_OBJECT)
	{
		return (fallback);
	}

	node = vdf_get(object, key);
	if (node && node->type == VDF_VAL_STRING)
	{
		return (node->string);
	}
	else
	{
		for (size_t i = 0; i < object->child_count; i++)
		{
			found = vdf_get_string_recursive(object->children[i], key, fallback);
		}
	}
	return (found);
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

static VDFcode append_text_to_buffer(char **buf, size_t *len, const char *text)
{
	size_t text_len = strlen(text);
	size_t new_len = *len + text_len + 1;
	char  *tmp;

	tmp = (char *) realloc(*buf, new_len);
	if (!tmp)
		return (VDF_ERR_MALLOC);
	tmp[*len] = '\0';
	strlcat(tmp, text, new_len);
	*len += text_len;
	*buf = tmp;
	return (VDF_OK);
}

static VDFcode dump_node_recursive(VDFNode *node, char **buf, size_t *len, int depth)
{
	char   *key;
	char   *value;
	char   *str;
	size_t  size;
	VDFcode err;

	key = node->key ? node->key : "(null)";
	value = node->type == VDF_VAL_STRING ? "string" : "object";
	size = strlen(key) + strlen(value) + 4;
	str = (char *) malloc(size);
	if (!str)
		return (VDF_ERR_MALLOC);
	for (int i = 0; i < depth; i++)
	{
		err = append_text_to_buffer(buf, len, "  ");
		if (err != VDF_OK)
		{
			free(str);
			return (err);
		}
	}
	strlcpy(str, key, size);
	strlcat(str, ": ", size);
	strlcat(str, value, size);
	strlcat(str, "\n", size);
	err = append_text_to_buffer(buf, len, str);
	if (err != VDF_OK)
	{
		free(str);
		return (err);
	}
	if (node->type == VDF_VAL_OBJECT)
	{
		for (size_t i = 0; i < node->child_count; i++)
		{
			err = dump_node_recursive(node->children[i], buf, len, depth + 1);
			if (err != VDF_OK)
			{
				free(str);
				return (err);
			}
		}
	}
	free(str);
	return (VDF_OK);
}

VDFcode vdf_dump_node(VDFNode *root, char **dump)
{
	char   *buf = NULL;
	size_t  size = 0;
	VDFcode err;
	err = dump_node_recursive(root, &buf, &size, 0);
	if (err != VDF_OK)
	{
		if (buf)
			free(buf);
		return err;
	}
	*dump = buf;
	return (VDF_OK);
}
