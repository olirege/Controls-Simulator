#include "sockets.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../commands/commands.h"
#include "../commands/command_processor.h"
#include "../utils/utils.h"
#include "../memory/memory.h"
// Server-side functions
int start_server(int port) {
    int server_fd, opt = 1;
    struct sockaddr_in address;
    int addr_len = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int accept_connection(int server_fd) {
    struct sockaddr_in address;
    int addr_len = sizeof(address);

    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addr_len);
    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return new_socket;
}

ParsedData parse_data(char *buffer) {
    ParsedData parsed_data;
    char *header = strtok(buffer, "/");
    char *data = strtok(NULL, "/");
    
    if (header != NULL) {
        parsed_data.header = strdup(header);  // Make a copy
    } else {
        parsed_data.header = NULL;
    }

    if (data != NULL) {
        parsed_data.data = strdup(data);  // Make a copy
    } else {
        parsed_data.data = NULL;
    }

    return parsed_data;
}

#define BUFFER_SIZE 1024
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_recieved;

    // Receive data from the client
    while ((bytes_recieved = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_recieved] = '\0';  // Null-terminate the string
        log(__func__, "Received: %s", buffer);
        // Parse data
        ParsedData parsed_data = parse_data(buffer);
        if (parsed_data.header == NULL || parsed_data.data == NULL) {
            log(__func__, "Failed to parse data.");
            continue;
        }
        // Determine the type of connection
        ConnectionType connection_type = determine_connection_type(parsed_data.header);
        if (connection_type == APP) {
            handle_app(parsed_data.data);
        } else if (connection_type == SENSOR) {
            handle_sensor(parsed_data.data);
        } else {
            log(__func__, "Unknown connection type.");
        }
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // Free the memory
        if(parsed_data.header != NULL) {
            free(parsed_data.header);
        }
        if(parsed_data.data != NULL) {
            free(parsed_data.data);
        }
    }

    if (bytes_recieved < 0) {
        perror("recv failed");
    }
    close(client_socket);
}

ConnectionType determine_connection_type(char *buffer_header) {
    log(__func__,"Determining connection type for header: %s", buffer_header);
    if (strcmp(buffer_header, "APP") == 0) {
        return APP;
    } else if (strcmp(buffer_header, "SENSOR") == 0) {
        return SENSOR;
    } else {
        return -1;
    }
}

void handle_app(char *data) {
    log(__func__, "Received data: %s", data);
    if (process_command(data) != 0) {
        log(__func__, "Error processing command");
    }
}

void handle_sensor(char *data) {
    // Parse the data (sensor_name, sensor_value)
    char *sensor_name = strtok(data, " ");
    char *sensor_value = strtok(NULL, " ");
    // Add the sensor value to the buffer
    int sensor_index = -1; 
    if ((sensor_index = add_sensor_to_buffer(sensor_name)) == -1) {
        log(__func__, "Error adding sensor value");
    }
    int result = add_sensor_value_to_buffer(sensor_index, sensor_value);
    if (result == 0) {
        log(__func__, "Added sensor value to %s buffer", sensor_name);
    } else if (result == 1) {
        log(__func__, "Sensor buffer is full");
        // Send the sensor data to the database
        if (send_sensor_data_to_db(sensor_index) != 0) {
            log(__func__, "Error sending sensor data to database");
        }
        // Clear the sensor buffer
        if (flush_sensor_buffer(sensor_index) != 0) {
            log(__func__, "Error flushing sensor buffer");
        }
    } else {
        log(__func__, "Error adding sensor value to buffer");
    }
}