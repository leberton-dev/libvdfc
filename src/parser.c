#include "vdfc/parser.h"
#include "vdfc/errors.h"
#include "vdfc/lexer.h"
#include "vdfc/node.h"
#include "vdfc/token.h"

#include <stdio.h>
#include <stdlib.h>

static char *vdf_strdup(const char *start, size_t len)
{
	char  *s;
	size_t i;
	size_t j;

	s = (char *) malloc(len + 1);
	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (start[i] == '\\' && i + 1 < len && (start[i + 1] == '"' || start[i + 1] == '\\'))
			i++;
		s[j] = start[i];
		i++;
		j++;
	}
	s[j] = '\0';
	return (s);
}

void vdf_free_node(VDFNode *node)
{
	size_t i;

	if (!node)
		return;
	free(node->key);
	if (node->type == VDF_VAL_STRING)
		free(node->string);
	else
	{
		i = 0;
		while (i < node->child_count)
		{
			vdf_free_node(node->children[i]);
			i++;
		}
		free(node->children);
	}
	free(node);
}

static void free_child_array(VDFNode **children, size_t count)
{
	size_t i;

	i = 0;
	while (i < count)
	{
		vdf_free_node(children[i]);
		i++;
	}
	free(children);
}

static VDFcode append_child(VDFNode *node, VDFNode *child)
{
	VDFNode **tmp;

	tmp = realloc(node->children, (node->child_count + 1) * sizeof(VDFNode *));
	if (!tmp)
		return (VDF_ERR_MALLOC);
	node->children = tmp;
	node->children[node->child_count] = child;
	node->child_count++;
	return (VDF_OK);
}

static VDFToken next_significant_token(VDFLexer *lexer)
{
	VDFToken tok;

	tok = vdf_next_token(lexer);
	while (tok.type == VDF_TOK_SINGLE_COMMENT || tok.type == VDF_TOK_MULTI_COMMENT)
	{
		tok = vdf_next_token(lexer);
	}
	return (tok);
}

static VDFcode parse_object(VDFLexer *lexer, VDFNode *node, int is_root)
{
	VDFToken key_tok;
	VDFToken value_tok;
	VDFNode *child;
	VDFcode  err;

	node->children = NULL;
	node->child_count = 0;
	while (1)
	{
		key_tok = next_significant_token(lexer);
		if (key_tok.type == VDF_TOK_EOF)
			return (is_root ? VDF_OK : VDF_ERR_PARSE);
		if (key_tok.type == VDF_TOK_CLOSE_BRACE)
			return (is_root ? VDF_ERR_PARSE : VDF_OK);
		if (key_tok.type != VDF_TOK_STRING)
			return (VDF_ERR_PARSE);

		value_tok = next_significant_token(lexer);
		child = malloc(sizeof(VDFNode));
		if (!child)
			return (VDF_ERR_MALLOC);
		child->string = NULL;
		child->children = NULL;
		child->child_count = 0;
		child->key = vdf_strdup(key_tok.start, key_tok.len);
		if (!child->key)
		{
			free(child);
			return (VDF_ERR_MALLOC);
		}

		if (value_tok.type == VDF_TOK_STRING)
		{
			child->type = VDF_VAL_STRING;
			child->string = vdf_strdup(value_tok.start, value_tok.len);
			err = child->string ? VDF_OK : VDF_ERR_MALLOC;
		}
		else if (value_tok.type == VDF_TOK_OPEN_BRACE)
		{
			child->type = VDF_VAL_OBJECT;
			err = parse_object(lexer, child, 0);
		}
		else
			err = VDF_ERR_PARSE;

		if (err == VDF_OK)
			err = append_child(node, child);
		if (err != VDF_OK)
		{
			vdf_free_node(child);
			free_child_array(node->children, node->child_count);
			node->children = NULL;
			node->child_count = 0;
			return (err);
		}
	}
}

VDFcode vdf_parse(const char *buffer, VDFNode **out)
{
	VDFLexer lexer;
	VDFNode *root;
	VDFcode  err;

	*out = NULL;
	root = malloc(sizeof(VDFNode));
	if (!root)
		return (VDF_ERR_MALLOC);
	root->key = NULL;
	root->string = NULL;
	root->type = VDF_VAL_OBJECT;

	lexer.cursor = buffer;
	err = parse_object(&lexer, root, 1);
	if (err != VDF_OK)
	{
		free(root);
		return (err);
	}
	*out = root;
	return (VDF_OK);
}
