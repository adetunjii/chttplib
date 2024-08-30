#ifndef __URL_H
#define __URL_H

#include <ctype.h>
#include <stdbool.h>

typedef struct Userinfo {
    char *username;
    char *password;
    bool passwordSet;
} Userinfo;

typedef struct url {
    char *scheme;
    char *opaque;   // encoded opaque parts
    char *host;     // host or host:port
    char *path;     // path (relative paths might not have leading slash)
    char *rawQuery; // encoded query values without the '?'
    Userinfo *user;
} URL;

enum encoding {
    UNUSED = 0,
    ENCODE_HOST = 1
};

extern void getScheme(const char *uri, char **scheme, char **path, char **err);
extern bool parseRequestURI(const char *rawURL, URL *url, char *errstr);
extern bool isValidPort(const char *port);
extern bool parseHost(const char *host_addr, char **host, char **errstr);
extern bool getAuthority(char *authority, Userinfo *userinfo, 
                         char **host, char **errstr);

#endif /* __URL_H*/
