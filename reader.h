#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 4096 // 4kb
#define MAX_REQ_TOKENS 3

/* ERRORS */
#define IO_ERR 1
#define PROTO_ERR 2
#define EOF_ERR 3

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
