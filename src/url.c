#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "palloc.h"
#include "url.h"
#include "utils/stringutil.h"

void getScheme(const char *uri, char **scheme, char **path, char **err) {
    if (uri == NULL) return;

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
                return;
            }
        } else if (c == ':') {
            if (i == 0) {
                // missing scheme
                *scheme = NULL;
                *path = NULL;
                *err = strdup("missing protocol scheme");
                return;
            }

            // valid scheme
            *scheme = strndup(uri, i);
            toLowercase(*scheme);
            *path = strdup(uri + i + 1);
            *err = NULL;
            return;
        } else {
            // default case
            *scheme = NULL;
            *path = strdup(uri);
            *err = NULL;
            return;
        }
    }

    // No scheme found, return the whole URL as the path
    *scheme = NULL;
    *path = strdup(uri);
    *err = NULL;
}

bool isValidPort(const char *port) {
    if (port == NULL)
        return true;

    if (port[0] != ':') 
        return false;

    for (int i = 1; i < strlen(port); i++) {
        if (port[i] < '0' || port[i] > '9')
            return false;
    }
    return true;
}

bool parseHost(const char *host_addr, char **host, char **errstr) {
    char *port = strchr(host_addr, ':');
    if (port == NULL) {
        *host = strdup(host_addr);
        return true;
    } 

    if (!isValidPort(port)) {
        int n = snprintf(NULL, 0, "invalid port '%s' after host", port);
        *errstr = (char *) palloc(n);
        sprintf(*errstr, "invalid port '%s' after host", port);
        return false;
    }

    *host = strdup(host_addr);
    return true;
}

bool getAuthority(char *authority, Userinfo *userinfo, 
                  char **host, char **errstr) {
 
    char *lastCharIdx; 
    if ((lastCharIdx = strrchr(authority, '@')) != NULL) {
        if (!parseHost(lastCharIdx + 1, host, errstr)) {
            return false;
        }
        
        size_t len = lastCharIdx - authority;
        authority = strndup(authority, len);

        /* TODO: userinfo requires extra validation */
        char *passwd = strrchr(authority, ':');
        if (passwd != NULL) {
            len = passwd - authority;
            userinfo->username = strndup(authority, len);
            userinfo->password = strdup(passwd + 1);
        } else {
            userinfo->username = strdup(authority);
        }

        return true;
    }

    return parseHost(authority, host, errstr);
}

bool parseRequestURI(const char *rawURL, URL *url, char *errstr) {
    char *path, *err;

    if (stringContainsCTLByte(rawURL)) {
        strcpy(errstr, "url: invalid control characters in url");
        return false;
    };

    getScheme(rawURL, &url->scheme, &path, &err);
    if (err != NULL) {
        strcpy(errstr, err);
        return false;
    }

    cut_str_by_delim(path, '?', &url->path, &url->rawQuery);
    free(path);

    if (has_prefix(url->path, "//")) {
        char *authority, *path_pos;
        Userinfo *userinfo;

        if ((path_pos = strchr(url->path, '/')) != NULL) {
            size_t len = path_pos - url->path;
            authority = strndup(url->path, len);
            url->path = strdup(path_pos);
        }

        userinfo = (Userinfo *) palloc(sizeof(Userinfo));
        getAuthority(authority, userinfo, &url->host, &err);


    }

    return true;
}

#if defined(TEST_URL)
#include "testutil.h"

int test_url(void) {
    URL *url = (URL *) palloc(sizeof(URL));
    char errstr[128];


    if (!parseRequestURI("https://example.com?foo=bar", url, errstr)) {
        return -1;
    };

    char *authority = "username:password@localhost.com";
    char *host;
    char *err = (char *)palloc(1024);

    Userinfo *userinfo = (Userinfo *) palloc(sizeof(Userinfo));
    int res;
    res = getAuthority(authority, userinfo, &host, &err);
    if (strlen(err) != 0) fprintf(stderr, "%s", err);

    fprintf(stderr, "%s %s\n", userinfo->username, userinfo->password);
    fprintf(stderr, "Host:: %s\n", host);

    return 0;
}
#endif
#ifdef TEST_URL
int main(void) {
    return test_url();
}
#endif
