#ifndef REQUEST_H
#define REQUEST_H

#define MAX_REQ_TOKENS 3 /* first request line parts */

#include "url.h"

typedef struct Request {
    int error;        /* set to 0 when there are no errors. */
    char errStr[128]; /* error string representation. */

    char *method; /* specifies the HTTP method - [GET, POST, PUT, etc.] */

    char *proto;
    int proto_major;
    int proto_minor;

    /* request_url holds the original request URL and should
     * not be modified. Use the `url` field instead.
     */
    char *request_url;

    URL *url;
    char *host;
} Request;

// static Request *newReqest(void);

#endif /* REQUEST_H */
