#ifndef UTILS_H
#define UTILS_H

#include <libpq-fe.h>
#include <stdbool.h>
#include "../memory/memory.h"

typedef enum {
    DOCKER_OK = 201,
    DOCKER_OK_NO_CONTENT = 204,
    DOCKER_BAD_REQUEST = 400,
    DOCKER_NOT_FOUND = 404,
    DOCKER_INTERNAL_SERVER_ERROR = 500
} DockerStatusCode;
int use_request(char *url, char *header, char *data, char *socket_path, char *method);
void sanitize_sensor_name(char *dest, const char *src);
int handle_pg_result(PGresult *res, PGconn *conn, const char *operation);
int insert_data_to_db(SensorData sensor_data);
int create_sensor_table(char *sensor_name);
int add_sensor_to_sensors_table(char *sensor_name);
int remove_sensor_from_sensors_table(char *sensor_name);
int set_sensor_status(char *sensor_name, bool status);
PGconn *db_connect();
int db_execute_params(PGconn *conn, const char *query, int n_params, const char *param_values[], ExecStatusType expected_status);
void db_finish(PGconn *conn);
void get_current_date_time(char *buffer, size_t buffer_size);
void log(const char *func, const char *format, ...);
#endif // UTILS_H