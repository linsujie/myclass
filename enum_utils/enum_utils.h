#ifndef _ENUM_UTILS_H
#define _ENUM_UTILS_H

#include<string.h>

template <class T> T str2enum( const char* );

#define STR2ENUM(TYPE,ARRAY) \
template <> \
TYPE str2enum<TYPE>( const char* str ) \
    { \
    for( unsigned i = 0; i < (sizeof(ARRAY)/sizeof(ARRAY[0])); i++ ) \
        if( !strcmp( ARRAY[i], str ) ) \
            return TYPE(i); \
    return TYPE(1000000); \
    }

#define ENUM2STR(TYPE,ARRAY) \
const char* enum2str( TYPE v ) \
    { \
    return v >= TYPE##_size ? "null" : ARRAY[v]; \
    }

#define ENUMANDSTR(TYPE,ARRAY)\
    STR2ENUM(TYPE,ARRAY) \
    ENUM2STR(TYPE,ARRAY)

#define ENUMDECLEAR(TYPE)\
template <> \
TYPE str2enum<TYPE>( const char* str ); \
const char* enum2str( TYPE v );

#define ENUMNAMECLS(TYPE, CLASS) \
  const char* const CLASS::TYPE##_name[CLASS::TYPE##_size + 1]

#define ENUMNAME(TYPE) \
  const char* const TYPE##_name[TYPE##_size + 1]

#endif // for #ifndef _ENUM_UTILS_H
