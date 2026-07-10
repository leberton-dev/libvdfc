#ifndef VDF_ERRORS_H
#define VDF_ERRORS_H

/**
 * Status codes returned by libvdfc functions.
 */
typedef enum e_VDFcode
{
	VDF_OK = 0,     /**< Success. */
	VDF_ERR_OPEN,   /**< Failed to open the input file. */
	VDF_ERR_IO,     /**< Read/seek error while accessing the file. */
	VDF_ERR_MALLOC, /**< Memory allocation failure. */
	VDF_ERR_PARSE   /**< Malformed VDF content. */
} VDFcode;

#endif
