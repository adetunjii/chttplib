#include "reader.h"

bufReader *newBufReader(void) {
	bufReader *r;
	r = malloc(sizeof(bufReader) + sizeof(char[DEFAULT_BUFFER_SIZE]));
	if (r == NULL) return NULL;

	return r;
}	

/* size returns the length of the underlying buffer. */
size_t size(bufReader* r) {
	return r->len;
}

static char *readBytes(bufReader *r, unsigned int len) {
	char* p;
	if((r->len - r->pos) >= len) {
		p = r->buf+r->pos;
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
			if(ret[1] == '\n') break;

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
static char* readLine(bufReader *r, int *_len) {
	char *p, *s;
	int len;

	p = r->buf + r->pos;
	s = seekNewLine(p, r->len-r->pos);
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

	free(r->buf);
	free(r);
}

#if defined(TEST_READER)
#include "testutil.h"

int test_reader(void) {
 const char *str = "GET / HTTP/1.1\r\n"
	 								"Host: localhost:8080\r\n"
        					"User-Agent: curl/8.6.0\r\n"
        					"Accept: */*";

	bufReader *reader = newBufReader();
	if (reader == NULL) return -1;

	memcpy(reader->buf, str, strlen(str));
	reader->len = strlen(str);
	
	test("size() returns the size of buffer", size(reader) == strlen(str))

	int len;
	char *line = readLine(reader, &len);
	test("readLine() reads the correct length of the line", len == 14)
	test("readLine() gets the content of the line up to \\r\\n", memcmp(line, "GET / HTTP/1.1", len) == 0)
	
	int len2;
	char *line2 = readLine(reader, &len2);
	test("readLine() sets r->pos correctly after reading a line", memcmp(line2, "Host: localhost:8080", len2) == 0)

	char* res = readBytes(reader, 5);
	test("readBytes() reads n bytes", strlen(res))
	
	free(reader);

	return 0;	
}
#endif
#ifdef TEST_READER
int main(void) {
	return test_reader();
}
#endif
