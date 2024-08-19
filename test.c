#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char str[] = "www.windows.com";
    char *token;

    // First call to strtok with the string and delimiters (space and comma)
    token = strtok(str, ".");

    // Continue to extract all tokens
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ".");
    }

    return 0;
}
