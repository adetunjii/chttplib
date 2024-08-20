#ifndef __READER_H
#define __READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 4096 // 4kb
#define MAX_REQ_TOKENS 3

#define HTTP_LIB_OK 0
#define HTTP_LIB_ERR -1

/* ERRORS */
#define HTTP_LIB_IO_ERR 1
#define HTTP_LIB_PROTO_ERR 2
#define HTTP_LIB_EOF_ERR 3

typedef struct bufReader {
	int error; // Set to 0 when there are no errors/
	char errStr[128]; // Error string representation.  
	
	char *buf;	
	size_t len; // Size of the buffer;
	size_t pos;
} bufReader;

/* size returns the size of the underlying buffer.*/
size_t size(bufReader* r);
void bufReaderFree(bufReader* r);

#endif /* __READER_H */
