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

int getScheme(const char *uri, char **scheme, char **path, char **err) {
    if (uri == NULL) return -1;

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
                return 0;
            }
        } else if (c == ':') {
            if (i == 0) {
                // missing scheme
                *scheme = NULL;
                *path = NULL;
                *err = strdup("missing protocol scheme");
                return -1;
            }

            // valid scheme
            *scheme = strndup(uri, i);
            *path = strdup(uri + i + 1);
            *err = NULL;
            return 0;
        } else {
            // default case
            *scheme = NULL;
            *path = strdup(uri);
            *err = NULL;
            return 0;
        }
    }

    // No scheme found, return the whole URL as the path
    *scheme = NULL;
    *path = strdup(uri);
    *err = NULL;
    return 0;
}

int parseRequestURI(const char *rawURL, URL *url, char *errstr) {
    if (rawURL == NULL) return -1;

    if (stringContainsCTLByte(rawURL)) {
        strcpy(errstr, "url: invalid control characters in url");
        return -1;
    };

    char *scheme, *path, *err;
    int res;
    res = getScheme(rawURL, &scheme, &path, &err);
    if (res == -1) {
        strcpy(errstr, err);
        return -1;
    }

    toLowercase(scheme);
    url->scheme = strdup(scheme);
    url->path = strdup(path);

    return 0;
}


#if defined(TEST_URL)
#include "testutil.h"

int test_url(void) {
    URL *url = malloc(sizeof(URL));
    char errstr[128];

    int res = parseRequestURI("://example.com", url, errstr);
    if (res == -1) {
        fprintf(stderr, "%s\n", errstr);
        return -1;
    }

    fprintf(stderr, url->scheme);
    fprintf(stderr, url->path);

    return 0;
}
#endif
#ifdef TEST_URL
int main(void) {
    return test_url();
}
#endif