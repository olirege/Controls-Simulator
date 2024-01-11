#include "memory.h"
#include "../utils/utils.h"
// hold sensor data
// empty buffer

SensorData sensors[MAX_SENSORS];
int num_sensors = 0; // number of sensors tracked

int get_sensor_index_from_buffer(char *sensor_name) {
    // Get the index of the sensor in the buffer
    for (int i = 0; i < num_sensors; i++) {
        if (strcmp(sensors[i].sensor_name, sensor_name) == 0) {
            return i;
        }
    }
    return -1;
}
int has_buffer_reached_max_sensors() {
    // Check if the buffer has reached the maximum number of sensors
    return num_sensors == MAX_SENSORS;
}
int add_sensor_to_buffer(char *sensor_name) {
    log(__func__, "Verifying if %s is in sensor buffer", sensor_name);
    // Add a sensor to the buffer
    if (has_buffer_reached_max_sensors()) {
        log(__func__, "Buffer is full");
        return -1;
    }
    // Check if the sensor already exists in the buffer
    int sensor_index = -1;
    if ((sensor_index = get_sensor_index_from_buffer(sensor_name)) != -1) {
        log(__func__, "Sensor already exists in buffer");
        return sensor_index;
    }
    // Assign the sensor to the first available index
    if (sensor_index == -1) {
        sensor_index = num_sensors;
        num_sensors++;
        log(__func__, "Assigning sensor to index %d", sensor_index);
    }
    // Add the sensor to the buffer
    log(__func__, "Adding sensor %s to buffer", sensor_name);
    strcpy(sensors[sensor_index].sensor_name, sensor_name);
    // Set the buffer size to 0
    log(__func__, "Setting buffer size to 0");
    sensors[sensor_index].buffer_size = 0;
    return sensor_index;
}

int get_sensor_buffer_size(int sensor_index) {
    // Get the size of the sensor buffer
    return sensors[sensor_index].buffer_size;
}
int is_sensor_buffer_full(int sensor_index) {
    // Check if the sensor buffer is full
    return get_sensor_buffer_size(sensor_index) == MAX_SENSOR_VALUES;
}
int add_sensor_value_to_buffer(int sensor_index, char *sensor_value) {
    // Add a sensor value to the buffer
    if (sensor_index == -1) {
        log(__func__, "Sensor does not exist in buffer");
        return -1;
    }
    if (is_sensor_buffer_full(sensor_index)) {
        log(__func__, "Sensor buffer is full");
        return 1;
    }
    // Get the current date-time
    log(__func__, "Getting current date-time");
    char date_time[TIMESTAMP_SIZE];
    get_current_date_time(date_time, sizeof(date_time));

    // Add the sensor value to the buffer
    log(__func__, "Adding sensor value to buffer");
    strncpy(sensors[sensor_index].data_buffer[sensors[sensor_index].buffer_size], 
            sensor_value, SENSOR_VALUE_SIZE - 1);
    sensors[sensor_index].data_buffer[sensors[sensor_index].buffer_size][SENSOR_VALUE_SIZE - 1] = '\0';

    // Add the timestamp to the buffer
    log(__func__, "Adding timestamp to buffer");
    strncpy(sensors[sensor_index].timestamps[sensors[sensor_index].buffer_size], 
            date_time, TIMESTAMP_SIZE - 1);
    sensors[sensor_index].timestamps[sensors[sensor_index].buffer_size][TIMESTAMP_SIZE - 1] = '\0';

    // Increment the buffer size
    log(__func__, "Incrementing buffer size");
    sensors[sensor_index].buffer_size++;
    return 0;
}
int send_sensor_data_to_db(int sensor_index) {
    // Send the sensor data to the database
    if (sensor_index == -1) {
        log(__func__, "Sensor does not exist in buffer");
        return -1;
    }
    // Assert that the sensor data is not NULL
    char *sensor_name = sensors[sensor_index].sensor_name;
    char *sensor_data = sensors[sensor_index].data_buffer;
    char *sensor_timestamps = sensors[sensor_index].timestamps;
    if (sensor_data == NULL || sensor_timestamps == NULL || sensor_name == NULL) {
        log(__func__, "Sensor data is NULL");
        return -1;
    }
    log(__func__, "Sending %s data to database", sensor_name);
    log(__func__, "Sensor data: %s", sensor_data);
    log(__func__, "Sensor timestamps: %s", sensor_timestamps);
    // Insert the sensor data into the database
    if (insert_data_to_db(sensors[sensor_index]) != 0) {
        log(__func__, "Error inserting sensor data into database");
        return -1;
    }
    return 0;
}
int flush_sensor_buffer(int sensor_index) {
    if (sensor_index == -1) {
        log(__func__, "Sensor does not exist in buffer");
        return -1;
    }
    // Clear the sensor buffer
    memset(sensors[sensor_index].data_buffer, 0, MAX_SENSOR_VALUES);
    memset(sensors[sensor_index].timestamps, 0, MAX_SENSOR_VALUES);
    // Reset the sensor buffer size
    sensors[sensor_index].buffer_size = 0;
    if (sensors[sensor_index].data_buffer == NULL) {
        log(__func__, "Error flushing sensor buffer");
        return -1;
    }
    return 0;
}
int get_buffer_size() {
    // Get the total size of the buffer
    int buffer_size = 0;
    for (int i = 0; i < num_sensors; i++) {
        buffer_size += get_sensor_buffer_size(i);
    }
    return buffer_size;
}
int flush_buffer() {
    // Clear the buffer
    for (int i = 0; i < num_sensors; i++) {
        if (flush_sensor_buffer(i) != 0) {
            log(__func__, "Error flushing sensor buffer");
            return -1;
        }
    }
    return 0;
}