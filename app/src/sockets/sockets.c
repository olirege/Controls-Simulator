#include "sockets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int establish_connection(const char *server_ip, int server_port) {
    int sock;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char port_str[6]; // Enough to hold all digits of a port number
    snprintf(port_str, sizeof(port_str), "%d", server_port);
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_INET for IPv4
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server_ip, port_str, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            perror("client: connect");
            continue;
        }

        break; // If we get here, we must have connected successfully
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return -1;
    }
    freeaddrinfo(servinfo); // All done with this structure
    return sock;
}

char *create_output_buffer(const char *data) {
    char *header = "APP/";
    size_t total_length = strlen(data) + strlen(header);
    char *buffer = malloc(total_length + 1);
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        return NULL;
    }
    strcpy(buffer, header);
    strcat(buffer, data);
    return buffer;
}
int send_data(const int socket_fd, const char *data) {
    char *buffer = create_output_buffer(data);
    if (buffer == NULL) {
        return -1;
    }
    int bytes_sent = send(socket_fd, buffer, strlen(buffer), 0);
    free(buffer);
    if (bytes_sent < 0) {
        perror("Failed to send data");
        return -1;
    }
    return bytes_sent;
}

int close_connection(const int socket_fd) {
    return close(socket_fd);
}
