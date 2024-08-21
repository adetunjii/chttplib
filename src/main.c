#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256
#define INITIAL_CAP 1000

int main(int argc, char **argv) {

    char *str = "GET / HTTP/1.1 \
        Host: localhost:8080 \
        User-Agent: curl/8.6.0 \
        Accept: */*";

    char *newline_pos, *buf;
    char *start_pos = str;

    size_t cap = INITIAL_CAP;
    size_t size = 0;

    char **lines = malloc(INITIAL_CAP * sizeof(char *));

    while ((newline_pos = strchr(str, '\n')) != NULL) {
        size_t len = newline_pos - start_pos;

        if (len >= BUF_SIZE) {
            len = BUF_SIZE - 1;
        }

        strncpy(buf, str, len);
        buf[len] = '\0';

        if (size >= cap) {
            cap *= 2;
            lines = realloc(lines, cap * sizeof(char *));
            if (lines == NULL) {
                perror("failed to allocate new memory for lines");
                return EXIT_FAILURE;
            }
        }

        lines[size] = strdup(buf);
        if (lines[size] == NULL) {
            perror("Failed to allocate memory for line");
            exit(EXIT_FAILURE);
        }

        start_pos = newline_pos + 1;
        size += 1;
    }

    if (*start_pos != '\0') {   
        if (size >= cap) {
            cap *= 2;
            lines = realloc(lines, cap * sizeof(char *));
            if(lines == NULL) {
                perror("failed to allocate new memory for lines");
                return EXIT_FAILURE;
            }
        }

        strncpy(buf, str, BUF_SIZE-1);
        buf[BUF_SIZE-1] = '\0';

        lines[size] = strdup(buf);
        if (lines[size] == NULL) {
            perror("Failed to allocate memory for line");
            exit(EXIT_FAILURE);
        }
        size++;
    }

      for (size_t i = 0; i < size; i++) {
        printf("Line: %s", lines[i]);
        free(lines[i]);  // Free each line
    }
    free(lines);

    return EXIT_SUCCESS;
}