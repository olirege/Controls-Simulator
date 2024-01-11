#include <stdio.h>
#include "command_functions.h"
#include "commands.h"

SubCommand db_sub_commands[] = {
    {"init", cmd_init_db_connection},
    {"listen", cmd_listen_for_sensor_updates},
    {"stop", cmd_stop_listening_sensor_updates},
    {"close", cmd_close_db_connection},
    {"query", cmd_execute_query},
    {NULL, cmd_unknown} // End of subcommands
};

Command commands[] = {
    {"help", NULL, cmd_help},
    {"db", db_sub_commands, NULL},
    {"ctrl", NULL, cmd_send_ctrl_command},
    {"exit", NULL, cmd_exit_program},
    {NULL, NULL, cmd_unknown} // Default case
};
