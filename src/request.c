#include <stdio.h>
#include <stdbool.h>

#include "reader.h"
#include "request.h"
#include "url.h"

static request *newRequest(void) {
	request *req;
	req = malloc(sizeof(request));
	if (req == NULL) return NULL;

	return req;	
}

static void __requestSetError(request *r, int errCode, const char *errStr) {
	size_t len;

	/* clear underlying buffer on erorrs. */
	free(r->method);
	r->method = NULL;

	free(r->proto);
	r->proto = NULL;

	free(r->uri);
	r->uri = NULL;

	free(r->host);
	r->host = NULL;

	/* Set errors */
	r->error = errCode;
	len = strlen(errStr);
	len = len < (sizeof(r->errStr) - 1) ? len : (sizeof(errStr) - 1);
	memcpy(r->errStr, errStr, len);
	r->errStr[len] = '\0';
}

static void requestFree(request *req) {
	if (req == NULL) return;

	if (req->method != NULL) {
		free(req->method);
		req->method = NULL;
	}

	if (req->proto != NULL) {
		free(req->proto);
		req->proto = NULL;
	}

	if (req->uri != NULL) {
		free(req->uri);
		req->uri = NULL;
	}

	if (req->host != NULL) {
		free(req->host);
		req->host = NULL;
	}

	free(req);
}

static int parseRequestLine(char *tokens[static MAX_REQ_TOKENS], request *req) {
	for (int i = 0; i < MAX_REQ_TOKENS; i++) {
		if (tokens[i] == NULL || strlen(tokens[i]) == 0) {
			for (int j = 0; j < i; j++) {
   	        	free(tokens[j]);
    	    }
			return CHTTP_ERR;
		}
	}

	*req = (request) {
		.method = malloc(sizeof(char) * strlen(tokens[0])),
		.uri = malloc(sizeof(char) * strlen(tokens[1])),
		.proto = malloc(sizeof(char) * strlen(tokens[2]))
	};

	if (req->method == NULL || req->uri == NULL || req->proto == NULL) {
		return CHTTP_ERR;
	}

 	strcpy(req->method, tokens[0]);
	strcpy(req->uri, tokens[1]);
	strcpy(req->proto, tokens[2]);

	return CHTTP_OK;
}

static bool validMethod(char *method) {
	if (strlen(method) > 0) {
		/*
			Method         = "OPTIONS"                ; Section 9.2
							| "GET"                    ; Section 9.3
							| "HEAD"                   ; Section 9.4
							| "POST"                   ; Section 9.5
							| "PUT"                    ; Section 9.6
							| "DELETE"                 ; Section 9.7
							| "TRACE"                  ; Section 9.8
							| "CONNECT"                ; Section 9.9
							| extension-method
		*/
		const char *methods[] = {
			"GET",
			"POST",
			"PUT",
			"DELETE",
			"HEAD",
			"OPTIONS",
			"CONNECT",
			"PATCH"
		};

		for (int i = 0; i < (sizeof(methods)/sizeof(methods[1])); i++) 
			if(strcmp(method, methods[i]) == 0) return true;
	}

	return false;
}

/* TODO: refactor to support other http versions [0.9, 1.0, 1.1, 2, 3]*/
static bool parseHTTPVersion(char *version, int *major, int *minor) {
	if (strlen(version) != strlen("HTTP/X.Y")) {
		*major = *minor = 0;
		return false;
	}
	
	if (strcmp(version, "HTTP/1.1") == 0) {
		*major = *minor = 1;
		return true;
	} else if(strcmp(version, "HTTP/1.0") == 0 ) {
		*major = 1;
		*minor = 0;
		return true;
	} else {
		*major = *minor = 0;
		return false;
	}
}

static bool stringContainsCTLByte(char *str) {
	for (size_t i = 0; i < strlen(str); i++) {
		char c = str[i];
		if (c < 0x20 || c == 0x07) return true;
	}

	return false;
}

// TODO: extract into url.c file
int parseURI(char *uri) {
	if (uri == NULL) return CHTTP_ERR;
	if (stringContainsCTLByte(uri)) return invalid_ctl_err;

	// extract the scheme, and the rest
	// parse query string into 

    return 1;
}

int readRequest(bufReader *r, request *req) {
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

			if (parseRequestLine(tokens, req) == CHTTP_ERR) {
				__requestSetError(req, CHTTP_PROTO_ERR, "malformed HTTP request");
				goto cleanup;
			}
			
			if (!validMethod(req->method)) {
				__requestSetError(req, CHTTP_PROTO_ERR, "url: invalid method");
				goto cleanup;
			}

			int major, minor, ok;
			ok = parseHTTPVersion(req->proto, &major, &minor);
			if (!ok) {
				__requestSetError(req, CHTTP_PROTO_ERR, "invalid HTTP version");
				goto cleanup;
			}

			req->proto_major = major;
			req->proto_minor = minor;

			return CHTTP_OK;
		}
	}

	return CHTTP_ERR;

cleanup:
	free(line);
	return CHTTP_ERR;	
}


#if defined(TEST_REQUEST)
#include <unistd.h>
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
	test("readLine() gets the content of the line up to \\r\\n", strcmp(line, "GET / HTTP/1.1") == 0)

	char *bytes;
	char copy[5];
	bytes = readBytes(reader, 5);
	memcpy(copy, bytes, 5);
	test("readBytes() reads n bytes", strcmp(copy, "Host:") == 0)

	bufReaderFree(reader);

	char *httpString =	"GET /hello HTTP/1.1\r\n"
	 					"Host: 127.0.0.1:8080\r\n"
       		 			"User-Agent: curl/8.6.0\r\n"
        				"Accept: */*";	

	reader = newBufReader(httpString, strlen(httpString));
	if (reader == NULL) return -1;

	request *req;
	req = newRequest();

	int result;
	result = readRequest(reader, req);
	test("readRequest() passes without errors", result == 0)
	test("readRequest() parses request method", strcmp(req->method, "GET") == 0)
	test("readRequest() parses uri", strcmp(req->uri, "/hello") == 0)
	test("readRequest() parses protocol", strcmp(req->proto, "HTTP/1.1") == 0)
	test("readRequest() gets proto major and minor", req->proto_major == 1 && req->proto_minor == 1)
	test ("isValidMethod() checks for valid methods", validMethod(req->method))

	{
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
		test("readRequest() fails on malformed requests", req->error != 0)
		test("readRequest() returns correct error string", memcmp(req->errStr, "malformed HTTP request", 10) == 0)	
		
		bufReaderFree(reader);
	}
	
	return 0;	
}
#endif
#ifdef TEST_REQUEST
int main(void) {
	return test_reader();
}
#endif
