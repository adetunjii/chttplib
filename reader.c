#include <stddef.h>
#include <assert.h>
#include "reader.h"

static void bufReaderSetError(bufReader *r, int errCode, const char* errStr) {
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
	if(r->buf == NULL) goto error; 

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

	if (r->buf != NULL) 
		free(r->buf);

	r->pos = r->len = 0;
	free(r);
}

typedef struct request {
	char *method; // specifies the HTTP method - [GET, POST, PUT, etc.]
	
	char *proto;
	char *uri;
	int proto_major;
	int proto_minor;

	char *host;
} request;

static void requestFree(request *req) {
	if (req == NULL) return;

	if (req->method != NULL) 
		free(req->method);
		req->method = NULL;
	if (req->proto != NULL)
		free(req->proto);
		req->proto = NULL;
	if (req->uri != NULL)
		free(req->uri);
		req->uri = NULL;
	if (req->host != NULL)
		free(req->host);
		req->host = NULL;

	free(req);
}

static request *newRequest() {
	request *req;

	req = malloc(sizeof(request));
	if (req == NULL) return NULL;

	return req;	
}

static int parseRequestLine(char *tokens[static MAX_REQ_TOKENS], request *req) {
	for (int i = 0; i < MAX_REQ_TOKENS; i++) {
		printf("%s\n", tokens[i]);
		if (tokens[i] == NULL || strlen(tokens[i]) == 0) {
			for (int j = 0; j < i; j++) {
   	        	free(tokens[j]);
    	    }
			return -1;
		}
	}

	*req = (request) {
		.method = malloc(sizeof(char) * strlen(tokens[0])),
		.uri = malloc(sizeof(char) * strlen(tokens[1])),
		.proto = malloc(sizeof(char) * strlen(tokens[2]))
	};

	if (req->method == NULL || (req)->uri == NULL || (req)->proto == NULL) {
		requestFree(req);
		return -1;
	}

 	strcpy(req->method, tokens[0]);
	strcpy(req->uri, tokens[1]);
	strcpy(req->proto, tokens[2]);

	return 0;
}


static int readRequest(bufReader *r, request *req) {
	char *p, *line;
	int len;
	
	if ((p = readLine(r, &len)) != NULL) {
		if (len > 0) {
			line = malloc(sizeof(char) * len);
			memcpy(line, p, len);
			line[len] = '\0';

			char *tokens[MAX_REQ_TOKENS];
			int i = 0;
			
			char *tok = strtok(line, " ");
			while (tok != NULL && i < MAX_REQ_TOKENS) {
				tokens[i] = strdup(tok);
				i++;
				tok = strtok(NULL, " ");
			}

			if (parseRequestLine(tokens, req) == -1) {
				goto error;
			}
			
			return 0;
		} else {
			goto error;
		}
	}

	free(line);
	return -1;

error:
	bufReaderSetError(r, PROTO_ERR, "malformed HTTP request");
	free(line);
	return -1;	
}


#if defined(TEST_READER)
#include "testutil.h"

int test_reader(void) {
	char *str = "GET / HTTP/1.1\r\n"
	 			"Host: localhost:8080\r\n"
        		"User-Agent: curl/8.6.0\r\n"
        		"Accept: */*";

	bufReader *reader;
	reader = newBufReader(str, strlen(str));
	if (reader == NULL) return -1;
	
	test("size() returns the size of the buffer", size(reader) == strlen(str))

	int len;
	char *ln, *line;
	ln = readLine(reader, &len); // points to the underlying buffer, we don't want to modify
	memcpy(line, ln, len); 
	line[len] = '\0';
	
	test("readLine() returns the correct length (without null terminator)", len == 14)
	test("readLine() gets the content of the line up to \\r\\n", memcmp(line, "GET / HTTP/1.1", len) == 0)

	char *bytes;
	char copy[5];
	bytes = readBytes(reader, 5);
	memcpy(copy, bytes, 5);
	test("readBytes() reads n bytes", memcmp(copy, "Host:", 5) == 0)

	bufReaderFree(reader);

	char *malformedHttpString = "GET /HTTP/1.1\r\n"
								"Host: localhost:8080\r\n"
        						"User-Agent: curl/8.6.0\r\n"
        						"Accept: */*";
	
	reader = newBufReader(malformedHttpString, strlen(malformedHttpString));
	if (reader == NULL) return -1;

	request* req;
	req = newRequest();
	
	int result;
	result = readRequest(reader, req);
	test("readRequest() fails on malformed requests", result == -1)

	test("readRequest() returns correct error", reader->error == PROTO_ERR)
	test("readRequest() returns correct error string", memcmp(reader->errStr, "malformed HTTP request", 22))

	bufReaderFree(reader);
	
	return 0;	
}
#endif
#ifdef TEST_READER
int main(void) {
	return test_reader();
}
#endif
