FUNCTION(GET_CLSNAME FILELIST ROOTDIR NAMELIST)
  FOREACH(element IN LISTS FILELIST)
    GET_FILENAME_COMPONENT(elename ${element} NAME_WE)
    GET_FILENAME_COMPONENT(elepath ${element} DIRECTORY)
    GET_FILENAME_COMPONENT(elebasepath ${elepath} NAME_WE)
    IF("${elename}" STREQUAL "${elebasepath}")
      LIST(APPEND CLSNAME ${ROOTDIR}/${elename})
    ENDIF("${elename}" STREQUAL "${elebasepath}")
  ENDFOREACH(element)

  SET(${NAMELIST} ${CLSNAME} PARENT_SCOPE)
ENDFUNCTION(GET_CLSNAME)

FUNCTION(FIND_CLASS ROOTDIR POSTFIX CLASS)
  FILE(GLOB SOURCE ${ROOTDIR}/*/*.${POSTFIX})
  GET_CLSNAME("${SOURCE}" "${ROOTDIR}" CLS)
  SET(${CLASS} ${CLS} PARENT_SCOPE)
ENDFUNCTION(FIND_CLASS)

FUNCTION(ADD_SUBMODULE DIR HEAD HEADWHOLE CLASS CLASSWHOLE)
  FIND_CLASS(${DIR} "h" HEADCOMP)
  FIND_CLASS(${DIR} "cc" CLASSCOMP)

  LIST(APPEND HEADWHOLE ${HEADCOMP})
  LIST(APPEND CLASSWHOLE ${CLASSCOMP})

  SET(${HEAD} ${HEADWHOLE} PARENT_SCOPE)
  SET(${CLASS} ${CLASSWHOLE} PARENT_SCOPE)
ENDFUNCTION(ADD_SUBMODULE)

FUNCTION(GET_CXX_FLAGS class CXX_FLAGS DLIST DEBUG RESULT)
  GET_FILENAME_COMPONENT(${class} basename NAME)

  IF(DEBUG)
    LIST(FIND DLIST ${basename} index)
    IF(NOT index EQUAL -1)
      STRING(REPLACE " " ";" DEBUG ${DEBUG})
      LIST(APPEND CXX_FLAGS ${DEBUG})
    ENDIF(NOT index EQUAL -1)
  ENDIF(DEBUG)

  IF(EXISTS ${class}/${basename}_data)
    LIST(APPEND CXX_FLAGS
      "-DDATDIR=\\\"${CMAKE_INSTALL_PREFIX}/lib/${basename}_data\\\"")
  ENDIF(EXISTS ${class}/${basename}_data)

  SET(${RESULT} ${CXX_FLAGS} PARENT_SCOPE)
ENDFUNCTION(GET_CXX_FLAGS)

FUNCTION(WRITE_OPTION VNAME VAR)
  EXEC_PROGRAM(echo ARGS "\"SET(${VNAME} \\\"${VAR}\\\")\" >> ${COMPILE_LOG}"  OUTPUT_VARIABLE VAR)
ENDFUNCTION(WRITE_OPTION VNAME VAR)

FUNCTION(ADOPT_FLAGS FLAGS)
  IF(FLAGS)
    STRING(REPLACE " " ";" FLAGS ${FLAGS})
    FOREACH(flag IN LISTS FLAGS)
      ADD_DEFINITIONS(-D${flag})
    ENDFOREACH(flag IN LISTS FLAGS)
  ENDIF(FLAGS)
ENDFUNCTION(ADOPT_FLAGS FLAGS)
