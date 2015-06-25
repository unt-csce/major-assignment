#ifndef GETLINE_H
#define GETLINE_H

/*this get a line from the console. It is kind of a compromise between
 *using the readlne library and not using the limited getline function
 */

char* getaline();

/*read a line from a arbitrary file handle */
char* getaline_filehandle( FILE* filehandle );

#endif /*GETLINE_H*/
