#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "url.h"

static bool stringContainsCTLByte(const char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c < 0x20 || c == 0x07) return true;
    }

    return false;
}

bool getScheme(const char *uri, char **scheme, char **path, char **err) {
    if (uri == NULL) return false;

    for (size_t i = 0; i < strlen(uri); i++) {
        char c = uri[i];

        if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z') {
            // do nothing
        } else if ('0' <= c && c <= 9 || c == '+' || c == '-' || c == '.') {
            if (i == 0) {
                // scheme cannot start with these characters
                *scheme = NULL;
                *path = strdup(uri);
                *err = NULL;
                return true;
            }
        } else if (c == ':') {
            if (i == 0) {
                // missing scheme
                *scheme = NULL;
                *path = NULL;
                *err = strdup("missing protocol scheme");
                return false;
            }

            // valid scheme
            *scheme = strndup(uri, i);
            *path = strdup(uri + i + 1);
            *err = NULL;
            return true;
        } else {
            // default case
            *scheme = NULL;
            *path = strdup(uri);
            *err = NULL;
            return true;
        }
    }

    // No scheme found, return the whole URL as the path
    *scheme = NULL;
    *path = strdup(uri);
    *err = NULL;
    return 0;
}

bool parseRequestURI(const char *rawURL, URL *url, char *errstr) {
    if (rawURL == NULL) return false;

    if (stringContainsCTLByte(rawURL)) {
        strcpy(errstr, "url: invalid control characters in url");
        return false;
    };

    char *scheme, *path, *err;

    if(!getScheme(rawURL, &scheme, &path, &err)) {
        if(err != NULL) strcpy(errstr, err);
        return false;
    }

    // TODO: null checks
    // if (scheme != NULL) toLowercase(scheme);

    // url->scheme = strdup(scheme);
    // url->path = strdup(path);

    return true;
}


#if defined(TEST_URL)
#include "testutil.h"

int test_url(void) {
    URL *url = malloc(sizeof(URL));
    char errstr[128];

    int res = parseRequestURI("//example.com", url, errstr);
    if (res == false) {
        fprintf(stderr, "%s\n", errstr);
        return -1;
    }

    // fprintf(stderr, "%s",url->scheme);
    fprintf(stderr, "%s", url->path);

    return 0;
}
#endif
#ifdef TEST_URL
int main(void) {
    return test_url();
}
#endif
