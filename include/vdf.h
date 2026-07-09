#ifndef LIB_VDF_C
#define LIB_VDF_C

#include <stdlib.h>

#include "vdf_errors.h"

VDFcode read_entire_file(const char *path, char **out, size_t *out_size);

#endif
