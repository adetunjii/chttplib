#ifndef __REQUEST_H
#define __REQUEST_H

typedef struct request {
	char *method; // specifies the HTTP method - [GET, POST, PUT, etc.]
	
	char *proto;
    int major;
    int minor;

	char *uri;
	int proto_major;
	int proto_minor;

	char *host;
} request;


#endif /* __REQUEST_H */
