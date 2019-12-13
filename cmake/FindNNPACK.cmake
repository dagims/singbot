# Copyright (c) 2008, 2014 OpenCog.org (http://opencog.org)
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# - Try to find NNPACK; Once done this will define
#
# NNPACK_FOUND - system has the NNPACK library
# NNPACK_INCLUDE_DIR - the NNPACK include directory
# NNPACK_LIBRARY - The nnpack library


# Look for the header file
FIND_PATH(NNPACK_INCLUDE_DIR nnpack.h 
	/usr/include/nnpack
	/usr/local/include
)

# Look for the library
FIND_LIBRARY(NNPACK_LIBRARY NAMES nnpack PATHS 
	/usr/lib
	/usr/local/lib
)

# Copy the results to the output variables.
IF(NNPACK_INCLUDE_DIR AND NNPACK_LIBRARY)
	SET(NNPACK_FOUND 1)
ELSE(NNPACK_INCLUDE_DIR AND NNPACK_LIBRARY)
	SET(NNPACK_FOUND 0)
ENDIF(NNPACK_INCLUDE_DIR AND NNPACK_LIBRARY)

IF(NNPACK_FOUND)
	IF(NOT NNPACK_FIND_QUIETLY)
		MESSAGE(STATUS "NNPACK was found.")
	ENDIF(NOT NNPACK_FIND_QUIETLY)
ELSE(NNPACK_FOUND)
	SET(NNPACK_DIR_MESSAGE "NNPACK was not found.")
	IF(NOT NNPACK_FIND_QUIETLY)
		MESSAGE(STATUS "${NNPACK_DIR_MESSAGE}")
	ELSE(NOT NNPACK_FIND_QUIETLY)
		IF(NNPACK_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "${NNPACK_DIR_MESSAGE}")
		ENDIF(NNPACK_FIND_REQUIRED)
	ENDIF(NOT NNPACK_FIND_QUIETLY)
ENDIF(NNPACK_FOUND)
