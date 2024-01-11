#ifndef SOCKETS_H
#define SOCKETS_H

int establish_connection(const char *server_ip, int serve_port);
int send_data(const int socket_fd, const char *data);
int close_connection(const int socket_fd);

#endif // SOCKETS_H
