#include <stddef.h>
#include <assert.h>
#include <stdbool.h>

#include "chttplib.h"
#include "reader.h"

void bufReaderSetError(bufReader *r, int errCode, const char *errStr) {
    size_t len;

    /* clear underlying buffer on erorrs. */
    free(r->buf);
    r->buf = NULL;
    r->pos = r->len = 0;

    /* Set errors */
    r->error = errCode;
    len = strlen(errStr);
    len = len < (sizeof(errStr) - 1) ? len : (sizeof(errStr) - 1);
    memcpy(r->errStr, errStr, len);
    r->errStr[len] = '\0';
}

bufReader *newBufReader(char *buf, size_t len) {
    bufReader *r = malloc(sizeof(bufReader));
    if (r == NULL) return NULL;

    r->buf = malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    if (r->buf == NULL) goto error;

    if (buf != NULL) {
        memcpy(r->buf, buf, len);
        r->len = len;
        r->pos = 0;
    }

    return r;

error:
    free(r);
    return NULL;
}

/* size returns the length of the underlying buffer. */
size_t size(bufReader *r) {
    return r->len;
}

char *readBytes(bufReader *r, unsigned int len) {
    char *p;
    if ((r->len - r->pos) >= len) {
        p = r->buf + r->pos;
        r->pos += len;
        return p;
    }
    return NULL;
}

/* seekNewLine finds a pointer to \r\n since HTTP uses carriage return
 * and newline as the standard end-of-line marker.
 */
static char *seekNewLine(char *s, size_t len) {
    char *ret;

    if (len < 2) return NULL;

    while ((ret = memchr(s, '\r', len)) != NULL) {
        if (ret[1] == '\n') break;

        len -= (ret - s) + 1;
        s = ret;
    }
    return ret;
}

/* readLine reads a line from the buffer, up to \r\n sequence,
 * and returns a pointer to the start of the read line.
 * It also stores the length of the line (excluding the
 * \r\n sequence) in _len.
 */
char *readLine(bufReader *r, int *_len) {
    char *p, *s;
    int len;

    p = r->buf + r->pos;
    s = seekNewLine(p, r->len - r->pos);
    if (s != NULL) {
        len = s - (r->buf + r->pos);
        r->pos += (len + 2); // skip \r\n

        if (_len) *_len = len;
        return p;
    }

    return NULL;
}

void bufReaderFree(bufReader *r) {
    if (r == NULL) return;

    if (r->buf != NULL) free(r->buf);

    r->pos = r->len = 0;
    free(r);
}
