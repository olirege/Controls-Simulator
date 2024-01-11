#ifndef SOCKETS_H
#define SOCKETS_H

typedef enum {
    SENSOR,
    APP,
} ConnectionType;

typedef struct{
    char *header;
    char *data;
} ParsedData;

int start_server(int port);
int accept_connection(int server_fd);
ParsedData parse_data(char *buffer);
ConnectionType determine_connection_type(char *buffer_header);
void handle_client(int client_socket);
void handle_app(char *buffer);
void handle_sensor(char *buffer);
#endif