# - Try to find the myclass library
# Usage of this module is as follows
#
# == Using any header-only components of myclass: ==
#
#     find_package( myclass )
#     if(myclass_FOUND)
#         include_directories(${myclass_INCLUDE_DIRS})
#         add_executable(foo foo.cc)
#         target_link_libraries(foo ${myclass_LIBRARIES})
#     endif()

# ===========================================================================
# Variables used by this module which can be used to change the default
# behaviour, and hence need to be set before calling find_package:
#
#  myclass_ROOT_DIR      The preferred installation prefix for searching for
#                            myclass. Set this if the module has problems finding
#                            the proper myclass installation.
#
# ============================================================================
# Variables set by this module:
#
#  myclass_FOUND           System has myclass.
#
#  lin_INCLUDE_DIRS    myclass include directories: not cached.
#
#  lin_LIBRARIES       Link to these to use the myclass: not cached.
#
#  work_INCLUDE_DIRS    myclass include directories: not cached.
#
#  work_LIBRARIES       Link to these to use the myclass: not cached.
##============================================================================

SET(myclass_ROOT_DIR "${myclass_ROOT_DIR}" CACHE PATH "prefix of system myclass installation")

#----------------------------------------------------------------------------
# Enable re-search if known myclass_ROOT_DIR changes?
#
IF(NOT "${myclass_ROOT_DIR}" STREQUAL "${myclass_INTERNAL_ROOT_DIR}")
    IF(myclass_INTERNAL_ROOT_DIR AND NOT myclass_FIND_QUIETLY)
        MESSAGE(STATUS "myclass_ROOT_DIR Changed, Rechecking for myclass")
    ENDIF()

    SET(myclass_INTERNAL_ROOT_DIR ${myclass_ROOT_DIR}
        CACHE INTERNAL "Last value supplied for where to locate myclass")
    UNSET(myclass_INCLUDE_DIR CACHE)
    UNSET(myclass_LIBRARY CACHE)
    UNSET(myclass_CONFIG_EXECUTABLE CACHE)
    UNSET(myclass_CONFIG_FILE CACHE)

    SET(myclass_LIBRARIES )
    SET(myclass_INCLUDE_DIRS )
    SET(myclass_FOUND FALSE)
ENDIF()

#----------------------------------------------------------------------------
# - If we already found myclass, be quiet
#
IF(myclass_INCLUDE_DIRS AND myclass_LIBRARIES)
    SET(myclass_FIND_QUIETLY TRUE)
ENDIF()

#----------------------------------------------------------------------------
# Set up HINTS on where to look for myclass
#
SET(_myclass_root_hints ${myclass_ROOT_DIR})

#----------------------------------------------------------------------------
# Find the whole myclass by its configure file
FIND_FILE(myclass_CONFIG_FILE .myclass.auto.cmake
  HINTS ${_myclass_root_hints}
  PATH_SUFFIXES share
  DOC "Path to the myclass configure file"
  )

IF (myclass_CONFIG_FILE)
  INCLUDE(${myclass_CONFIG_FILE})
ENDIF()

#----------------------------------------------------------------------------
# Construct an error message for FPHSA
#
set(myclass_DEFAULT_MSG "Could NOT find myclass:\n")

#----------------------------------------------------------------------------
# Handle the QUIETLY and REQUIRED arguments, setting myclass_FOUND to TRUE if
# all listed variables are TRUE
#
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(myclass
    "${myclass_DEFAULT_MSG}"
    lin_LIBRARY
    lin_LIBRARIES
    lin_INCLUDE_DIRS
    work_LIBRARIES
    work_INCLUDE_DIRS
    )
