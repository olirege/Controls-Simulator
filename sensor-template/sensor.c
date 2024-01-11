// sensor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function
#include <stdbool.h>
#include "./sockets/sockets.h"
double data = 0.0;
bool is_increasing = true;
char *sensor_name;
#define CONTROLLER_IP "CONTROLLER"
#define CONTROLLER_PORT 6543

void changeData(double *data) {
    if (is_increasing) {
        *data += 0.1;
    } else {
        *data -= 0.1;
    }
    if (*data >= 1.0) {
        is_increasing = false;
    } else if (*data <= -1.0) {
        is_increasing = true;
    }
}
int main() {
    sensor_name = getenv("SENSOR_NAME");
    if (sensor_name == NULL) {
        fprintf(stderr, "SENSOR_NAME environment variable not set.\n");
        return 1;
    }
    int socket_fd = establish_connection(CONTROLLER_IP, CONTROLLER_PORT);
    if (socket_fd < 0) {
        fprintf(stderr, "Connection to controller failed.\n");
        return 1;
    }
    sleep(5);
    while (true) {
        changeData(&data);
        char buffer[256];
        sprintf(buffer, "%s %f", sensor_name, data);
        if (send_data(socket_fd, buffer) < 0) {
            fprintf(stderr, "Failed to send data.\n");
            return 1;
        }
        sleep(1);
    }
    close(socket_fd);
    return 0;
}
