# - Try to find the GalpWrapper library
# Usage of this module is as follows
#
# == Using any header-only components of GalpWrapper: ==
#
#     find_package( GalpWrapper )
#     if(GalpWrapper_FOUND)
#         include_directories(${GalpWrapper_INCLUDE_DIRS})
#         add_executable(foo foo.cc)
#         target_link_libraries(foo ${GalpWrapper_LIBRARIES})
#     endif()

# ===========================================================================
# Variables used by this module which can be used to change the default
# behaviour, and hence need to be set before calling find_package:
#
#  GalpWrapper_ROOT_DIR      The preferred installation prefix for searching for
#                            GalpWrapper. Set this if the module has problems finding
#                            the proper GalpWrapper installation.
#
# ============================================================================
# Variables set by this module:
#
#  GalpWrapper_FOUND           System has GalpWrapper.
#
#  GalpWrapper_INCLUDE_DIRS    GalpWrapper include directories: not cached.
#
#  GalpWrapper_LIBRARIES       Link to these to use the GalpWrapper: not cached.
#
#  GalpWrapper_LIBRARY         Link to these to use the GalpWrapper: not cached.
#
#  GalpWrapper_GALPROP_VERSION Link to these to use the GalpWrapper: not cached.
#
#============================================================================

SET(GalpWrapper_ROOT_DIR "${GalpWrapper_ROOT_DIR}" CACHE PATH "prefix of system GalpWrapper installation")

#----------------------------------------------------------------------------
# Enable re-search if known GalpWrapper_ROOT_DIR changes?
#
IF(NOT "${GalpWrapper_ROOT_DIR}" STREQUAL "${GalpWrapper_INTERNAL_ROOT_DIR}")
    IF(GalpWrapper_INTERNAL_ROOT_DIR AND NOT GalpWrapper_FIND_QUIETLY)
        MESSAGE(STATUS "GalpWrapper_ROOT_DIR Changed, Rechecking for GalpWrapper")
    ENDIF()

    SET(GalpWrapper_INTERNAL_ROOT_DIR ${GalpWrapper_ROOT_DIR}
        CACHE INTERNAL "Last value supplied for where to locate GalpWrapper")
    UNSET(GalpWrapper_INCLUDE_DIR CACHE)
    UNSET(GalpWrapper_LIBRARY CACHE)
    UNSET(GalpWrapper_CONFIG_EXECUTABLE CACHE)
    UNSET(GalpWrapper_CONFIG_FILE CACHE)

    SET(GalpWrapper_LIBRARIES )
    SET(GalpWrapper_INCLUDE_DIRS )
    SET(GalpWrapper_FOUND FALSE)
ENDIF()

#----------------------------------------------------------------------------
# - If we already found GalpWrapper, be quiet
#
IF(GalpWrapper_INCLUDE_DIRS AND GalpWrapper_LIBRARIES)
    SET(GalpWrapper_FIND_QUIETLY TRUE)
ENDIF()

#----------------------------------------------------------------------------
# Set up HINTS on where to look for GalpWrapper
#
SET(_GalpWrapper_root_hints ${GalpWrapper_ROOT_DIR})

#----------------------------------------------------------------------------
# Find the whole GalpWrapper by its configure file
FIND_FILE(GalpWrapper_CONFIG_FILE .galpwrapper.auto.cmake
  HINTS ${_GalpWrapper_root_hints}
  PATH_SUFFIXES share
  DOC "Path to the GalpWrapper configure file"
  )

IF (GalpWrapper_CONFIG_FILE)
  INCLUDE(${GalpWrapper_CONFIG_FILE})
ENDIF()

#----------------------------------------------------------------------------
# Construct an error message for FPHSA
#
set(GalpWrapper_DEFAULT_MSG "Could NOT find GalpWrapper:\n")

#----------------------------------------------------------------------------
# Handle the QUIETLY and REQUIRED arguments, setting GalpWrapper_FOUND to TRUE if
# all listed variables are TRUE
#
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GalpWrapper
    "${GalpWrapper_DEFAULT_MSG}"
    GalpWrapper_INCLUDE_DIRS
    GalpWrapper_LIBRARIES
    )
