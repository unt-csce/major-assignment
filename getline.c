#include <stdio.h>

#include "getline.h"
#include "tryrealloc.h"

char* getaline_filehandle(FILE* filehandle){
  char* buff = 0;
  size_t buffsize = 0;
  char c = 0;

  /*read in a string one character at a time dynamically 
   until we hit a newline */
  while ( ( c = getc(filehandle)) != '\n' ){
    buff = try_realloc( buff, buffsize+1 );
    buff[buffsize++] = c;

  }

  /* Null terminate the string we just created */
  buff = try_realloc( buff, buffsize+1 );
  buff[buffsize+1] = 0;

  return buff;

}

char* getaline(){
  return getaline_filehandle( stdin );
}
