#ifndef VDF_ERRORS_H
#define VDF_ERRORS_H

typedef enum e_VDFcode
{
	VDF_OK = 0,
	VDF_ERR_OPEN,
	VDF_ERR_IO,
	VDF_ERR_MALLOC
} VDFcode;

#endif
