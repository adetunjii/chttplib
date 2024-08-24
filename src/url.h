#ifndef __URL_H
#define __URL_H

#include <stdbool.h>

typedef enum { empty_err = 1, invalid_ctl_err = 2, scheme_err = 3, segment_err = 4 } urlErr;

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


void toLowercase(char *str) {
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) str[i] = tolower(str[i]);
}

#endif /* __URL_H*/
