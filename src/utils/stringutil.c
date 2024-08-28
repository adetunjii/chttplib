#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool stringContainsCTLByte(const char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c < 0x20 || c == 0x07) return true;
    }

    return false;
}

void toLowercase(char *str) {
    size_t len = strlen(str);

    if (len > 0) {
        for (size_t i = 0; i < len; i++) str[i] = tolower(str[i]);
    }
}

void toUppercase(char *str) {
    size_t len = strlen(str);

    if (len > 0) {
        for (size_t i = 0; i < len; i++) str[i] = toupper(str[i]);
    }
}

void cut_str_by_delim(const char *str, char delim, char **a, 
                      char **b) { 
    char *pos = strchr(str, delim);

    if (pos != NULL) {
        size_t len = pos - str;
        *a = strndup(str, len);
        *b = strdup(pos + 1);
    } else {
        *a = strdup(str);
        *b = NULL;
    }
}

bool has_prefix(const char *str, const char *prefix) {
    size_t len_s = strlen(str);
    size_t len_p = strlen(prefix);

    return len_s > len_p && strncmp(str, prefix, len_p) == 0;
}
