#ifndef SOCKETS_H
#define SOCKETS_H
int send_data(const int socket_fd, const char *data);
int establish_connection(const char *server_ip, const int server_port);
#endif