#ifndef _MYDEBUG_H
#define _MYDEBUG_H
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#ifdef DEBUG_TO_FILE
#define DEBUG_FILE "debugmsg"
#define DEBUG_BUFFER_MAX 4096
#define printDebugMsg(moduleName, format, ...) {\
    char buffer[DEBUG_BUFFER_MAX+1]={0};\
    snprintf( buffer, DEBUG_BUFFER_MAX \
              , "[%s]\tFile:%s, Line:%d\t" format "\n", moduleName, __FILE__, __LINE__, ##__VA_ARGS__ );\
    FILE* fd = fopen(DEBUG_FILE, "a");\
    if ( fd != NULL ) {\
      fwrite( buffer, strlen(buffer), 1, fd );\
      fflush( fd );\
      fclose( fd );\
    }\
  }
#else
#define printDebugMsg(moduleName, format, ...) \
  printf( "[%s]\tFile:%s, Line:%d\t" format "\n", moduleName, __FILE__, __LINE__, ##__VA_ARGS__ );
#endif //end for #ifdef DEBUG_TO_FILE
#else
#define printDebugMsg(moduleName, format, ...)
#endif  //end for #ifdef _DEBUG
#endif // for #ifndef _MYDEBUG_H
