#include <stdio.h>
#include <stdlib.h>

#include "tryrealloc.h"

char * try_realloc( char *ptr, size_t size ){

   char *block = realloc( ptr, size );
   if (0 == block ){
      fprintf( stderr, "The system ran out of memory!\n");
      exit(1);
   }

   return block;
}
