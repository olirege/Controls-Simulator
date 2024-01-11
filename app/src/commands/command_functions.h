#ifndef COMMAND_FUNCIONS_H
#define COMMAND_FUNCIONS_H

// db
void cmd_listen_for_sensor_updates();
void cmd_stop_listening_sensor_updates();
void cmd_init_db_connection();
void cmd_close_db_connection();
void cmd_execute_query(const char *query);

// controller
void cmd_send_ctrl_command(const char *command, const char *data);

// misc
void cmd_exit_program();
void cmd_unknown();
void cmd_help();
#endif // COMMAND_FUNCIONS_H