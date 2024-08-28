#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "chttplib.h"
#include "url.h"

static bool stringContainsCTLByte(const char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c < 0x20 || c == 0x07) return true;
    }

    return false;
}

void getScheme(const char *uri, char **scheme, char **path, char **err) {
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
            }
        } else if (c == ':') {
            if (i == 0) {
                // missing scheme
                *scheme = NULL;
                *path = NULL;
                *err = strdup("missing protocol scheme");
            }

            // valid scheme
            *scheme = strndup(uri, i);
            toLowercase(*scheme);
            *path = strdup(uri + i + 1);
            *err = NULL;
        } else {
            // default case
            *scheme = NULL;
            *path = strdup(uri);
            *err = NULL;
        }
    }

    // No scheme found, return the whole URL as the path
    *scheme = NULL;
    *path = strdup(uri);
    *err = NULL;
}

bool parseRequestURI(const char *rawURL, URL *url, char *errstr) {
    char *err;

    if (rawURL == NULL) return false;

    if (stringContainsCTLByte(rawURL)) {
        strcpy(errstr, "url: invalid control characters in url");
        return false;
    };

    getScheme(rawURL, &url->scheme, &url->path, &err);
    if (err != NULL) {
        strcpy(errstr, err);
        return false;
    }

    // parse request path

    return true;
}


#if defined(TEST_URL)
#include "testutil.h"

int test_url(void) {
    URL *url = (URL *) palloc(sizeof(URL));
    char errstr[128];

    int res = parseRequestURI("https://example.com", url, errstr);
    if (res == false) {
        fprintf(stderr, "%s\n", errstr);
        return -1;
    }

    fprintf(stderr, "%s\n",url->scheme);
    fprintf(stderr, "%s", url->path);

    return 0;
}
#endif
#ifdef TEST_URL
int main(void) {
    return test_url();
}
#endif
