/* Authors: Edmund, Chris, Peter & Loc	Project: Major Assigment
 * Class:CSCE3600.021	Instructor: Mark Thompson
 * Details: Unix Terminal shell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "getline.h"
#include "tryrealloc.h"

/*define a way of identifying tokens*/
typedef enum _tokentype {
  COMMAND,
  ARG,
  SEMICOLON,
  UNKNOWN,
  QUIT
} tokentype;

/*define a data structure for our 'list' of tokens*/
typedef struct _token {
  struct _token *next;
  tokentype type;
  char *data;
} token;

/*take a string and append it as a token to a list
  of tokens*/
void append_token_list( char* token_string , token** list ) {
  token **current = list;

  while ( *current != 0 ) {
    current = &( (*current)->next );
  }

  *current = (token *) try_realloc( 0, sizeof(token) );
  (*current)->data = token_string;
  (*current)->type = UNKNOWN;
  (*current)->next = 0;
}

/*print out a list of assembled tokens*/
void print_token_list ( token* list ) {
  token *current = list;
  while ( 0 != current ) {
    switch ( current->type ) {
    case COMMAND:
      printf( "(%s, COMMAND)->", current->data );
      break;
    case ARG:
      printf( "(%s, ARG)->", current->data );
      break;
    case SEMICOLON:
      printf( "(%s, SEMICOLON)->", current->data );
      break;
    default:
      printf( "(%s, UNKNOWN)->", current->data );
    }
    current = current->next;
  }
  printf( "TERMINUS\n" );
}

/*a function to clean up the token list*/
void free_token_list ( token* list ) {
  token *current = list;
  token *next= 0;
  while ( current != 0 ) {
    next = current->next;
    free( current );
    current = next;
  }
}

/* seting the functions of token to the token list*/ 
void Paringlop ( token* list){
  list->type = COMMAND;
  token *current= list->next;
  while ( current != 0 ){
    current->type = ARG; 
    current = current->next;
  }
  current = list;
  while ( current != 0 ){
      if ( strcmp ( current->data, ";")==0)
         current->type = SEMICOLON;
      current = current->next;
  }
}

void runtokenlist ( token* list){

  int count = 0;
  token* current = list;
  while (current != 0 ){
    count++;
    current = current->next;  
  }
 
  char** argarray = (char**) try_realloc ( 0 ,(count+1)*sizeof ( char* ));
  current = list;
  int i;
  for ( i = 0 ; i < count && current !=0 ; ++i ){
     argarray[i] = current->data;
     current = current->next;
  }

  argarray[i] = 0;
 
  pid_t pid = fork();
  if (pid){
    int status;
    waitpid ( pid, &status, 0);
  }
  else {
    execvp( list->data, argarray );
  }
  /*in the future let make the fork and wait, so we will need to clean up*/
  free( argarray );   
}
  

/*a function to turn an input string into a list of tokens*/
void tokenize( char* buff ) {
  token* tokenlist = 0;
  char* token_string = 0;

  /*repeatedly use strtok to seperate out
    the tokens*/
  token_string = strtok(buff," ");
    
  append_token_list( token_string, &tokenlist );
  while( token_string != 0 ) {
    token_string = strtok(0," ");
    if ( token_string != 0 ) {
      append_token_list( token_string, &tokenlist );
    }
  }

  /* tokenlist code to classify tokens!*/
  Paringlop( tokenlist );
 
 /*run the tokenlists*/
  runtokenlist ( tokenlist );
  /*print the results*/
  print_token_list( tokenlist );
  
  /*cleanup the token list*/
  free_token_list( tokenlist );
 }

void run_tokenizer( FILE* handle )
{
  char* buff = 0;
  
  do {
    /*we clean up at the top because we need the string
      to compare in the while loop at the bottom*/
    if ( buff != 0 ) {
      free(buff);
      buff = 0;
    }

    /*print out the prompt and get a line*/
    printf( "prompt> ");
    buff = getaline_filehandle(handle);

    /*tokenize the line*/
    tokenize( buff );    
  } while ( strcmp( buff , "quit" ) != 0 );

  /*ensuring that buff is freed when we exit*/
  free( buff );
  
}


int main( int argc , char **argv) {

  if ( argc > 1 ) {
    printf( "Batch mode! File name is %s\n" , argv[1] );

    /*doing batch processing here*/
    FILE* f = fopen( argv[1] , "r" );
    run_tokenizer( f );
    fclose( f );
    exit(0);
  }
  else {
    run_tokenizer( stdin );
  }
  return 0;
}


