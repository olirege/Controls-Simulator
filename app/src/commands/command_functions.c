#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include <libpq-fe.h>
#include <stdbool.h>
#include "../sockets/sockets.h"
static PGconn *conn = NULL;

// db

void cmd_init_db_connection() {
    conn = PQconnectdb("host=db user=user password=password dbname=sysdb port=5432");
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
    } else {
        printf("Connection to database successful.\n");
        printf("--------------------\n");
        printf("Database info:\n");
        printf("Database name: %s\n", PQdb(conn));
        printf("Database user: %s\n", PQuser(conn));
        printf("Tables in database:\n");
        PGresult *res = PQexec(conn, "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");
        for (int i = 0; i < PQntuples(res); i++) {
            printf("%s\n", PQgetvalue(res, i, 0));
        }
        printf("--------------------\n");
        PQclear(res);
    }
}

void cmd_close_db_connection() {
    if (conn != NULL) {
        PQfinish(conn);
        fprintf(stderr, "Connection to database closed.\n");
    } else {
        fprintf(stderr, "No connection to close.\n");
    }
}

void cmd_execute_query(const char *query) {
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Query failed: %s", PQerrorMessage(conn));
    } else {
        PQclear(res);        
    }
}

bool keep_listening = false;
void cmd_listen_for_sensor_updates() {
    if (conn == NULL) {
        fprintf(stderr, "No connection to database.\n");
        return;
    }
    if (keep_listening) {
        fprintf(stderr, "Already listening for sensor updates.\n");
        return;
    }
    keep_listening = true;
    // Start listening for the notification
    PGresult *res = PQexec(conn, "LISTEN sensor_update");
    PQclear(res);

    while (keep_listening) {
        // Wait for incoming messages
        PQconsumeInput(conn);
        PGnotify *notify;

        while ((notify = PQnotifies(conn)) != NULL) {
            printf("Notification received: %s\n", notify->extra);
            PQfreemem(notify);
            PQconsumeInput(conn);
        }

        // Sleep to prevent busy waiting
        sleep(1);
    }
    
    res = PQexec(conn, "UNLISTEN sensor_update");
    PQclear(res);
}

void cmd_stop_listening_sensor_updates() {
    keep_listening = false;
    printf("Stopped listening for sensor updates.\n");
}

// controller
#define CONTROLLER_IP "CONTROLLER"
#define CONTROLLER_PORT 6543
void cmd_send_ctrl_command(const char *command, const char *data) {
    int socket_fd = establish_connection(CONTROLLER_IP, CONTROLLER_PORT);
    if (socket_fd < 0) {
        fprintf(stderr, "Connection to controller failed.\n");
        return;
    }
    char buffer[256];
    if (data == NULL) {
        sprintf(buffer, "%s", command);
    } else {
        sprintf(buffer, "%s %s", command, data);
    }
    if (send_data(socket_fd, buffer) < 0) {
        fprintf(stderr, "Failed to send command to controller.\n");
        return;
    }
    if(close_connection(socket_fd) < 0) {
        fprintf(stderr, "Failed to close connection to controller.\n");
        return;
    }
}

// Misc
void cmd_exit_program() {
    if (conn != NULL) {
        PQfinish(conn);
    }
    exit(0);
}

void cmd_unknown() {
    fprintf(stderr, "Unknown command.\n");
}

void cmd_help() {
    for(int i = 0; commands[i].command_name != NULL; i++) {
        printf("%s\n", commands[i].command_name);
    }
}