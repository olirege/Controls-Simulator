#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_functions.h"
#include "commands.h"
#include "../utils/utils.h"
void process_command(char* input) {
    log(__func__, "%s", input);
    if (input == NULL || strlen(input) == 0) {
        cmd_unknown();
        return;
    }

    // Split input into primary_command, sub_command, and args
    char* primary_command = strtok(input, " ");
    char* sub_command = strtok(NULL, " ");
    char* args = strtok(NULL, " ");
    
    for (int i = 0; commands[i].command_name != NULL; i++) {
        if (strcmp(primary_command, commands[i].command_name) == 0) {
            if (commands[i].sub_commands != NULL) {
                // Process sub_command
                for (int j = 0; commands[i].sub_commands[j].sub_command_name != NULL; j++) {
                    if (strcmp(sub_command, commands[i].sub_commands[j].sub_command_name) == 0) {
                        return commands[i].sub_commands[j].sub_command_func();
                        
                    } 
                }
            } else if (commands[i].command_func != NULL) {
                // check if command is for external program
                if(sub_command != NULL) {
                    if(args != NULL) {
                        log(__func__, "args: %s", args);
                        return commands[i].command_func(sub_command, args);
                    } else {
                        log(__func__, "no args");
                        return commands[i].command_func(sub_command);
                    }
                } else {
                    log(__func__, "no sub_command");
                    return commands[i].command_func();
                }
            }
            log(__func__, "Command not found");
            return -1;
        }
    }
    // If command not found, execute cmdUnknown
    return cmd_unknown();
    
}
