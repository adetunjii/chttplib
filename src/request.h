#ifndef __REQUEST_H
#define __REQUEST_H

#define MAX_REQ_TOKENS 3 /* first request line parts */

typedef struct request {
	int error; /* set to 0 when there are no errors. */
	char errStr[128]; /* error string representation. */  

	char *method; /* specifies the HTTP method - [GET, POST, PUT, etc.] */
	
	char *proto;
    int proto_major;
    int proto_minor;

	char *uri;
	char *host;
} request;

#endif /* __REQUEST_H */
