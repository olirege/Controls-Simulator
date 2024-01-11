#include <stdio.h>
#include "command_functions.h"
#include "commands.h"

SubCommand db_sub_commands[] = {
    {NULL, cmd_unknown} // End of subcommands
};

Command commands[] = {
    {"add", NULL, cmd_add_sensor},
    {"remove", NULL, cmd_remove_sensor},
    {"start", NULL, cmd_start_sensor},
    {"stop", NULL, cmd_stop_sensor},
    {"all", NULL, cmd_all_sensors},
    {"test", db_sub_commands, NULL},
    {NULL, NULL, cmd_unknown} // Default case
};