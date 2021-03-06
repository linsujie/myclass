cmake_minimum_required(VERSION 3.0.2)
project(myclass CXX)

# By default CMake will install myclass to ~/ on Linux/Mac
# unless you override the install location manually. You can do this
# either by either running:
#  cmake -DCMAKE_INSTALL_PREFIX=/path/to/dir ..
# or by uncommenting and modifying this variable:
#SET(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

#=============================================================================
# All done editing

#EXECUTE_PROCESS(COMMAND "echo" "$HOME" OUTPUT_VARIABLE HOME)
SET(myclass_VERSION "0.0")

INCLUDE("function.cmake")
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

SET(CMAKE_CXX_FLAGS "-O2 -std=c++11 ${CMAKE_CXX_FLAGS}")
STRING(REPLACE " " ";" CMAKE_CXX_FLAGS_LIST ${CMAKE_CXX_FLAGS})

SET(COMPILE_LOG "${CMAKE_BINARY_DIR}/.myclass.auto.cmake")

IF (NOT LIBRARY_TYPE)
  SET(LIBRARY_TYPE STATIC)
ENDIF()

IF (GalpWrapper_ROOT_DIR)
  GET_FILENAME_COMPONENT(GalpWrapper_ROOT_DIR "${GalpWrapper_ROOT_DIR}" REALPATH)
  IF (EXISTS "${GalpWrapper_ROOT_DIR}")
    FIND_PACKAGE(GalpWrapper)

    IF (GalpWrapper_FOUND)
      ADD_DEFINITIONS(-DGALP_V${GalpWrapper_GALPROP_VERSION})
      LIST(APPEND work_LIBRARIES ${GalpWrapper_LIBRARIES})
      LIST(APPEND work_INCLUDE_DIRS ${GalpWrapper_INCLUDE_DIRS})
    ENDIF()
  ELSE()
    IF(FLAGS)
      STRING(REPLACE " " ";" FLAGS_TMP ${FLAGS})
      FOREACH(flag IN LISTS FLAGS_TMP)
        ADD_DEFINITIONS(-D${flag})
      ENDFOREACH(flag IN LISTS FLAGS_TMP)
    ENDIF(FLAGS)
  ENDIF()
ENDIF()

LIST(FIND FLAGS "NO_ROOT" no_root_index)
IF (("${no_root_index}" STREQUAL "-1") AND NOT (GalpWrapper_LIBRARIES MATCHES "libCore.so"))
  FIND_PACKAGE(Root)

  LIST(APPEND lin_LIBRARIES ${ROOT_LIBRARIES})
  LIST(APPEND lin_INCLUDE_DIRS ${ROOT_INCLUDE_DIR})

  SET(CMAKE_CXX_FLAGS "-O2 -std=c++11 ${ROOT_CPPFLAGS}")
ENDIF()

EXECUTE_PROCESS(COMMAND echo "# This cmake is automatically generated by myclass, it
# include the information of the myclass compilation." OUTPUT_FILE ${COMPILE_LOG})
FOREACH(var flag lin_LIBRARIES lin_INCLUDE_DIRS work_LIBRARIES work_INCLUDE_DIRS)
  WRITE_OPTION(${var} "${${var}}")
ENDFOREACH()
WRITE_OPTION_APPEND(FLAGS "${FLAGS}")
WRITE_END()

MESSAGE("The compiling options set as:")
MESSAGE(STATUS "INSTALL PREFIX::                                        CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "Compiling flag::                                        CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "The class that would be excluded when compiling::       EXCLUDE = ${EXCLUDE}")
MESSAGE(STATUS "The class that you want to open the debug option::      DLIST = ${DLIST}")
MESSAGE(STATUS "The debug option: ['', '-D_DEBUG', '-DDEBUG_TO_FILE']:: DEBUG = ${DEBUG}")
MESSAGE(STATUS "The direction that GalpWrapper installed::              GalpWrapper_ROOT_DIR = ${GalpWrapper_ROOT_DIR}  [FOUND? ${GalpWrapper_FOUND}]")
MESSAGE(STATUS "Supported flag: [NO_ROOT]::                             FLAGS = ${FLAGS}")
MESSAGE(STATUS "================ ${LIBRARY_TYPE} library would be generated ================")

SET(WORKLIST chi2prop mcparas anaspec_zhou)
SET(SUBMODULES antinucleus dm_production interpolator utils)
LIST(APPEND SUBMODULES ".")

FOREACH(module IN LISTS SUBMODULES)
  GET_FILENAME_COMPONENT(mdir ${CMAKE_SOURCE_DIR}/${module} ABSOLUTE)
  ADD_SUBMODULE("${mdir}" HEAD "${HEAD}" CLASS "${CLASS}"
    ENUMDEF "${ENUMDEF}" DATA "${DATA}")
ENDFOREACH(module)

FOREACH(header IN LISTS HEAD)
  GET_FILENAME_COMPONENT(inc ${header} DIRECTORY)
  INCLUDE_DIRECTORIES(${inc})
ENDFOREACH(header)
INCLUDE_DIRECTORIES(${lin_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${work_INCLUDE_DIRS})

IF (EXCLUDE)
  STRING(REPLACE " " ";" EXCLUDE ${EXCLUDE})
  REMOVE_LIST(CLASS "${CLASS}" "${EXCLUDE}")
  REMOVE_LIST(HEAD "${HEAD}" "${EXCLUDE}")
  REMOVE_LIST(DATA "${DATA}" "${EXCLUDE}")
  REMOVE_LIST(ENUMDEF "${ENUMDEF}" "${EXCLUDE}")
ENDIF()

FOREACH(source IN LISTS CLASS)
  GET_FILENAME_COMPONENT(basename ${source} NAME_WE)
  LIST(FIND WORKLIST ${basename} index)
  IF(index EQUAL -1)
    LIST(APPEND SRCLIN ${source})
  ELSE(index EQUAL -1)
    LIST(APPEND SRCWORK ${source})
  ENDIF(index EQUAL -1)
ENDFOREACH(source)

FILE(GLOB sources "${CMAKE_SOURCE_DIR}/quadrature/*cc")
LIST(APPEND SRCLIN ${sources})

ADD_LIBRARY_MYCLASS(lins "${SRCLIN}")
TARGET_LINK_LIBRARIES(lins ${lin_LIBRARIES})
SET_TARGET_PROPERTIES(lins PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
IF (GalpWrapper_FOUND)
  ADD_LIBRARY_MYCLASS(works "${SRCWORK}")

  SET(work_LIBRARIES_temp "")
  FOREACH(lib ${work_LIBRARIES})
    STRING(REGEX MATCH ".so$" mat ${lib})
    IF(mat)
      LIST(APPEND work_LIBRARIES_temp ${lib})
    ENDIF()
  ENDFOREACH()
  TARGET_LINK_LIBRARIES(works ${work_LIBRARIES_temp})
  SET_TARGET_PROPERTIES(works PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF()

INSTALL(FILES ${HEAD} DESTINATION include)
INSTALL(FILES ${ENUMDEF} DESTINATION include/enumdef)
INSTALL(DIRECTORY ${DATA} DESTINATION lib)
INSTALL(FILES ${COMPILE_LOG} DESTINATION share)

INSTALL(TARGETS lins LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
IF (GalpWrapper_FOUND)
  INSTALL(TARGETS works LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
ENDIF()
