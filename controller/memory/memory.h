#ifndef MEMORY_H
#define MEMORY_H

#define MAX_SENSOR_VALUES 10 // Maximum number of sensor values
#define SENSOR_VALUE_SIZE 256 // Maximum size of each sensor value
#define TIMESTAMP_SIZE 20     // Size for timestamp strings
#define MAX_SENSORS 3        // Maximum number of sensors
typedef struct {
    char sensor_name[50];
    char data_buffer[MAX_SENSOR_VALUES][SENSOR_VALUE_SIZE]; // 2D array: Array of strings
    char timestamps[MAX_SENSOR_VALUES][TIMESTAMP_SIZE];     // 2D array: Array of timestamps
    int buffer_size;
} SensorData;
int get_sensor_index_from_buffer(char *sensor_name);
int has_buffer_reached_max_sensors();
int add_sensor_to_buffer(char *sensor_name);
int add_sensor_value_to_buffer(int sensor_index, char *sensor_value);
int get_sensor_buffer_size(int sensor_index);
int is_sensor_buffer_full(int sensor_index);
int send_sensor_data_to_db(int sensor_index);
int flush_sensor_buffer(int sensor_index);
int get_buffer_size();
int flush_buffer();
#endif