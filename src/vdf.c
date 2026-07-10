#include "vdf_errors.h"

#include <stdio.h>
#include <stdlib.h>

VDFcode read_entire_file(const char *path, char **out, size_t *out_size)
{
	FILE   *f;
	char   *buf = NULL;
	long    size;
	VDFcode err = VDF_OK;

	*out = NULL;

	f = fopen(path, "rb");
	if (!f)
		return (VDF_ERR_OPEN);

	if (fseek(f, 0, SEEK_END) != 0)
	{
		err = VDF_ERR_IO;
		goto cleanup;
	}

	size = ftell(f);
	if (size < 0)
	{
		err = VDF_ERR_IO;
		goto cleanup;
	}

	if (fseek(f, 0, SEEK_SET) != 0)
	{
		err = VDF_ERR_IO;
		goto cleanup;
	}

	buf = malloc((size_t) size + 1);
	if (!buf)
	{
		err = VDF_ERR_MALLOC;
		goto cleanup;
	}

	if (fread(buf, 1, (size_t) size, f) != (size_t) size)
	{
		free(buf);
		buf = NULL;
		err = VDF_ERR_IO;
		goto cleanup;
	}
	buf[size] = '\0';
	*out = buf;
	if (out_size)
		*out_size = (size_t) size;

cleanup:
	fclose(f);
	return (err);
}
