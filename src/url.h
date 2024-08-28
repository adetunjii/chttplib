#ifndef __URL_H
#define __URL_H

#include <ctype.h>
#include <stdbool.h>

typedef struct userinfo {
    char *username;
    char *password;
    bool passwordSet;
} userinfo;

typedef struct url {
    char *scheme;
    char *opaque;   // encoded opaque parts
    char *host;     // host or host:port
    char *path;     // path (relative paths might not have leading slash)
    char *rawQuery; // encoded query values without the '?'
    userinfo *user;
} URL;


static inline void toLowercase(char *str) {
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) str[i] = tolower(str[i]);
}

extern bool getScheme(const char *uri, char **scheme, char **path, char **err);
extern bool parseRequestURI(const char *rawURL, URL *url, char *errstr);

#endif /* __URL_H*/
