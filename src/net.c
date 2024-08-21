#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <regex.h>
#include <pthread.h>

#include "net.h"

void *handle_client(void *arg) {
    int client_fd = *((int *) arg);

    char *buf = (char *)malloc(BUFFER_SIZE * sizeof(char));

    // recieve request data from client and store into buffer.
    ssize_t bytes_recv = recv(client_fd, buf, BUFFER_SIZE, 0);
    printf("%s\n", buf);
    if (bytes_recv < 0) {
        perror("Couldn't read client request");
        exit(EXIT_FAILURE);
    }

    regex_t regex;
    regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
    regmatch_t matches[2];

    if (regexec(&regex, buf, 2, matches, 0) != 0) {
        perror("Failed to parse request");
        exit(EXIT_FAILURE);
    }
}

int startServer(void) {
    int fd;
    struct sockaddr_in addrinfo;

    /* socket(domain (IPv4 | IPv6), type (SOCK_STREAM for tcp, SOCK_DGRAM for udp), 
     * protocol (set the last field to 0 to choose the default protocol for the sock type) ) */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    addrinfo.sin_family = AF_INET;
    addrinfo.sin_addr.s_addr = INADDR_ANY;
    addrinfo.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr *)&addrinfo, sizeof(addrinfo)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 10) < 0) {
        perror("Failed to listen on port 8080.");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    while (1) {
        // client info
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        if ((*client_fd = accept(fd, 
                                (struct sockaddr *)&client_addr, 
                                &client_addr_len)) < 0) {
            perror("accept failed");
            continue;
        }

        // create a new thread to handle client request
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
        pthread_detach(thread_id);
    }

    close(fd);

    return 0;
}