#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 4096 // 4kb

typedef struct bufReader {
	int error; // Set to 0 when there are no errors/
	char errStr[128]; // Error string representation.  
	
	size_t len; // Size of the buffer;
	size_t pos;
	char buf[];
	
} bufReader;

/* size returns the size of the underlying buffer.*/
size_t size(bufReader* r);
void bufReaderFree(bufReader* r);
