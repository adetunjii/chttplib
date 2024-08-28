#ifndef __READER_H
#define __READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 4096 // 4kb
#define MAX_REQ_TOKENS  3

#define CHTTP_ERR   -1
#define CHTTP_OK    0

/* ERRORS */
#define CHTTP_IO_ERR        1
#define CHTTP_EOF_ERR       3
#define CHTTP_PROTO_ERR     4

typedef struct bufReader {
    int error;        // Set to 0 when there are no errors/
    char errStr[128]; // Error string representation.

    char *buf;
    size_t len; // Size of the buffer;
    size_t pos;
} bufReader;

/* size returns the size of the underlying buffer.*/
extern size_t size(bufReader *r);
extern bufReader *newBufReader(char *buf, size_t len);
extern void bufReaderFree(bufReader *r);
extern void bufReaderSetError(bufReader *r, int errCode, const char *errStr);
static char *seekNewLine(char *s, size_t len);
extern char *readBytes(bufReader *r, unsigned int len);
extern char *readLine(bufReader *r, int *_len);

#endif /* __READER_H */
