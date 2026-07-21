#include <stdio.h>
#include <vdfc/vdf.h>

int main()
{
	char    *out;
	size_t   out_size;
	VDFcode  err;
	VDFNode *root;
	char    *dump;

	err = vdf_read_file("test/fixtures/libraryfolders.vdf", &out, &out_size);
	if (err != VDF_OK)
	{
		return (1);
	}

	err = vdf_parse(out, &root);
	if (err != VDF_OK)
	{
		return (1);
	}

	err = vdf_dump_node(root, &dump);
	if (err != VDF_OK)
	{
		exit(1);
	}

	printf("%s\n", out);
	return 0;
}
