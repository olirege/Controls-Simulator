#include "utils.h"
#include <stdlib.h>
PGconn *db_connect()
{
    log(__func__, "Connecting to database");
    char *DB_HOST = getenv("DB_HOST");
    char *DB_USER = getenv("DB_USER");
    char *DB_PASS = getenv("DB_PASS");
    char *DB_NAME = getenv("DB_NAME");
    char *DB_PORT = getenv("DB_PORT");

    if (!DB_HOST || !DB_USER || !DB_PASS || !DB_NAME || !DB_PORT)
    {
        log(__func__, "Database environment variables are not set");
        return NULL;
    }

    char conninfo[256];
    snprintf(conninfo, sizeof(conninfo), "host=%s user=%s password=%s dbname=%s port=%s", DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT);

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    return conn;
}
int db_execute_params(PGconn *conn, const char *query, int n_params, const char *param_values[], ExecStatusType expected_status)
{
    log(__func__, "executing query: %s", query);
    PGresult *res = PQexecParams(conn, query, n_params, NULL, param_values, NULL, NULL, 0);

    if (PQresultStatus(res) != expected_status)
    {
        fprintf(stderr, "Query failed: %s", PQerrorMessage(conn));
        return -1;
    }
    PQclear(res);
    return 0;
}
void db_finish(PGconn *conn)
{
    if (conn == NULL)
    {
        return;
    }
    log(__func__, "Finishing database connection");
    PQfinish(conn);
}
// int insert_data_to_db(char *sensor_name, char *data)
// {
//     log(__func__, "Inserting data to table %s", sensor_name);
//     // Connect to the database
//     const PGconn *conn = db_connect();
//     if (conn == NULL)
//     {
//         return -1;
//     }
//     // Prepare a query with a placeholder
//     char query[1024];
//     snprintf(query, sizeof(query), "INSERT INTO $1 (value, timestamp) VALUES");
//     // Bind the parameter
//     const char *param_values[2] = {sensor_name, data};

//     // Execute the query
//     if (db_execute_params(conn, query, 2, param_values, PGRES_COMMAND_OK) != 0)
//     {
//         fprintf(stderr, "Failed to execute query: %s", PQerrorMessage(conn));
//         return -1;
//     }

//     // Finish the connection
//     db_finish(conn);
//     return 0;
// }
void sanitize_sensor_name(char *dest, const char *src) {
    // Sanitize the sensor name to avoid SQL injection
    int i = 0;
    while (src[i] != '\0') {
        if (src[i] == '\'' || src[i] == '\"') {
            dest[i] = '_';
        } else {
            dest[i] = src[i];
        }
        i++;
    }
    dest[i] = '\0';
}
int handle_pg_result(PGresult *res, PGconn *conn, const char *operation) {
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Failed to %s: %s", operation, PQerrorMessage(conn));
        PQclear(res);
        db_finish(conn);
        return -1;
    }
    PQclear(res);
    return 0;
}
int insert_data_to_db(SensorData sensor_data) {
    log(__func__, "Inserting data to table %s", sensor_data.sensor_name);
    PGconn *conn = db_connect();
    if (conn == NULL) {
        return -1;
    }

    // Sanitize the sensor name to avoid SQL injection
    char sanitized_sensor_name[50];
    sanitize_sensor_name(sanitized_sensor_name, sensor_data.sensor_name);
    log(__func__, "Sanitized sensor name: %s", sanitized_sensor_name);
    // Begin a transaction
    PGresult *res = PQexec(conn, "BEGIN");
    if (handle_pg_result(res, conn, "BEGIN") != 0) {
        return -1;
    }

    // Construct the query with the dynamic table name
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO %s (value, timestamp) VALUES ($1, $2)", sanitized_sensor_name);
    log(__func__, "Query: %s", query);
    // Prepare the INSERT statement
    const char *stmtName = "insert_sensor_data";
    res = PQprepare(conn, stmtName, query, 2, NULL);
    handle_pg_result(res, conn, "PQprepare");

    // Execute the prepared statement for each row
    log(__func__, "Executing prepared statement");
    for (int i = 0; i < sensor_data.buffer_size; i++) {
        if (sensor_data.data_buffer[i] == NULL || sensor_data.timestamps[i] == NULL) {
            log(__func__, "Sensor data is NULL");
            return -1;
        }
        const char *paramValues[2] = {
            sensor_data.data_buffer[i],
            sensor_data.timestamps[i]
        };

        res = PQexecPrepared(conn, stmtName, 2, paramValues, NULL, NULL, 0);
        if(handle_pg_result(res, conn, "PQexecPrepared") != 0) {
            return -1;
        }
    }

    // Commit the transaction
    log(__func__, "Committing transaction");
    res = PQexec(conn, "COMMIT");
    handle_pg_result(res, conn, "COMMIT");

    db_finish(conn);
    return 0;
}

int create_sensor_table(char *sensor_name)
{
    log(__func__, "Creating table for sensor %s", sensor_name);
    const PGconn *conn = db_connect();
    if (conn == NULL)
    {
        return -1;
    }
    // Prepare a query with a placeholder
    char sanitized_sensor_name[50];
    sanitize_sensor_name(sanitized_sensor_name, sensor_name);
    
    char query[256];
    snprintf(query, sizeof(query), "CREATE TABLE IF NOT EXISTS %s "
                                   "(id SERIAL PRIMARY KEY, "
                                   "timestamp TIMESTAMP, "
                                   "value VARCHAR(256))", sanitized_sensor_name);
    // Execute the query
    if (db_execute_params(conn, query, 0, NULL, PGRES_COMMAND_OK) != 0)
    {
        fprintf(stderr, "Failed to execute query: %s", PQerrorMessage(conn));
        return -1;
    }

    // Finish the connection
    db_finish(conn);
    return 0;
}

int add_sensor_to_sensors_table(char *sensor_name)
{
    log(__func__, "Adding sensor %s to sensors table", sensor_name);
    const PGconn *conn = db_connect();
    if (conn == NULL)
    {
        return -1;
    }
    // Prepare a query with a placeholder
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO sensors (name) VALUES ($1)");
    // Bind the parameter
    const char *param_values[1] = {sensor_name};

    // Execute the query
    if (db_execute_params(conn, query, 1, param_values, PGRES_COMMAND_OK) != 0)
    {
        fprintf(stderr, "Failed to execute query: %s", PQerrorMessage(conn));
        return -1;
    }

    // Finish the connection
    db_finish(conn);
    return 0;
}
int remove_sensor_from_sensors_table(char *sensor_name)
{
    log(__func__, "Removing sensor %s from sensors table", sensor_name);
    const PGconn *conn = db_connect();
    if (conn == NULL)
    {
        return -1;
    }
    // Prepare a query with a placeholder
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM sensors WHERE name = $1");
    // Bind the parameter
    const char *param_values[1] = {sensor_name};

    // Execute the query
    if (db_execute_params(conn, query, 1, param_values, PGRES_COMMAND_OK) != 0)
    {
        fprintf(stderr, "Failed to execute query: %s", PQerrorMessage(conn));
        return -1;
    }
    // Finish the connection
    db_finish(conn);
    return 0;
}

int set_sensor_status(char *sensor_name, bool status)
{
    const char *status_str = status ? "true" : "false";
    
    log(__func__, "Setting sensor status to %s", sensor_name, status_str);
    const PGconn *conn = db_connect();
    if (conn == NULL)
    {
        return -1;
    }
    // Prepare a query with a placeholder
    char query[256];
    snprintf(query, sizeof(query), "UPDATE sensors SET status = $1 WHERE name = $2");
    const char *param_values[2] = {status_str, sensor_name};
    // Execute the query
    if (db_execute_params(conn, query, 2, param_values, PGRES_COMMAND_OK) != 0)
    {
        fprintf(stderr, "Failed to execute query: %s", PQerrorMessage(conn));
        return -1;
    }
    // Finish the connection
    db_finish(conn);
    return 0;
}