#include <stdio.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../memory/memory.h"

int cmd_add_sensor(char *sensor_name) {
    log(__func__,"Adding Sensor %s", sensor_name);
    // Emulate Sensor
    char data[256];
    char *network_name = getenv("CONTAINER_NETWORK");
    if (network_name == NULL) {
        log(__func__,"Error getting network name");
        return -1;
    }
    snprintf(data, sizeof(data), "{\"Image\":\"sensor-template\",\"Env\":[\"SENSOR_NAME=%s\"],\"Name\":\"%s\", \"HostConfig\":{\"NetworkMode\":\"%s\"}}", sensor_name, sensor_name, network_name);
    char url[256];
    snprintf(url, sizeof(url), "http://v1.43/containers/create?name=%s", sensor_name);
    if ( use_request(
        url,
        "Content-Type: application/json",
        data,
        "/var/run/docker.sock",
        "POST"
    ) != DOCKER_OK) {
        log(__func__,"Error creating sensor");
        return -1;
    }
    log(__func__,"Sensor created");
    // Add sensor to sensor Repo
    if(add_sensor_to_sensors_table(sensor_name) != 0) {
        log(__func__,"Error adding sensor to sensors table");
        return -1;
    }
    // Create sensor table
    if(create_sensor_table(sensor_name) != 0) {
        log(__func__,"Error adding sensor to sensors table");
        return -1;
    }
    return 0;
}
int cmd_remove_sensor(char *sensor_name) {
    log(__func__,"Removing Sensor %s", sensor_name);
    char url[256];
    snprintf(url, sizeof(url), "http://v1.43/containers/%s?force=true", sensor_name);
    // Remove sensor emulator
    if(use_request(
        url,
        "Content-Type: application/json",
        NULL, // No data needed for a DELETE request
        "/var/run/docker.sock",
        "DELETE"
    ) != DOCKER_OK_NO_CONTENT) {
        log(__func__,"Error removing sensor");
        return -1;
    } 

    log(__func__,"Sensor removed");
    // Remove sensor from Sensor Repo
    if(remove_sensor_from_sensors_table(sensor_name) != 0) {
        log(__func__,"Error removing sensor from sensors table");
        return -1;
    }
    return 0;
}

int cmd_stop_sensor(char *sensor_name) {
    log(__func__,"Stopping Sensor %s", sensor_name);
    char url[256];
    snprintf(url, sizeof(url), "http://v1.43/containers/%s/stop", sensor_name);

    if(use_request(
        url,
        "Content-Type: application/json",
        NULL, // No data needed for a STOP request
        "/var/run/docker.sock",
        "POST"
    ) != DOCKER_OK_NO_CONTENT) {
        log(__func__,"Error Stopping sensor");
        return -1;
    }
    if(set_sensor_status(sensor_name, false) != 0) {
        log(__func__,"Error setting sensor status");
        return -1;
    }
    // Flush the sensor buffer
    int sensor_index = get_sensor_index_from_buffer(sensor_name);
    if (sensor_index == -1) {
        log(__func__, "Sensor does not exist in buffer");
        return -1;
    }
    if (flush_sensor_buffer(sensor_index) != 0) {
        log(__func__, "Error flushing sensor buffer");
        return -1;
    }
    log(__func__,"Sensor stopped");
    return 0;
}
int cmd_start_sensor(char *sensor_name) {
    log(__func__,"Starting Sensor %s", sensor_name);
    char url[256];
    snprintf(url, sizeof(url), "http://v1.43/containers/%s/start", sensor_name);
    int response = use_request(
        url,
        "Content-Type: application/json",
        NULL, // No data needed for a START request
        "/var/run/docker.sock",
        "POST"
    );
    log(__func__,"response: %d", response);
    if(response != DOCKER_OK_NO_CONTENT) {
        log(__func__,"Error starting sensor");
        return -1;
    }
    log(__func__,"Changing sensor status");
    int status_set = set_sensor_status(sensor_name, true);
    log(__func__,"status_set: %d", status_set);
    if(status_set != 0) {
        log(__func__,"Error setting sensor status");
        return -1;
    }
    log(__func__,"Sensor started");
    return 0;
}

int cmd_all_sensors(char *command) {
    // TODO Get all sensors from DB table "sensors" to execute command (start, stop, remove)
    log(__func__,"Executing command %s", command);
    return 0;
}
void cmd_unknown() {
    log(__func__,"Unknown command executed");
    return 0;
}