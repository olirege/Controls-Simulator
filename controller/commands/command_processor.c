#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_functions.h"
#include "commands.h"
#include "../utils/utils.h"
void process_command(char* input) {
    log(__func__, "processing command: %s", input);
    // Check if input is NULL or empty
    if (input == NULL || strlen(input) == 0) {
        cmd_unknown();
        return;
    }
    
    // Split input into primary_command, sub_command, and args
    char* primary_command = strtok(input, " ");
    char* sub_command = strtok(NULL, " ");
    char* args = strtok(NULL, " ");
    // Process primary_command
    for (int i = 0; commands[i].command_name != NULL; i++) {
        // Check if primary_command is valid
        if (strcmp(primary_command, commands[i].command_name) == 0) {
            if (commands[i].sub_command != NULL) {
                // Process sub_command
                for (int j = 0; commands[i].sub_command[j].sub_command_name != NULL; j++) {
                    if (strcmp(sub_command, commands[i].sub_command[j].sub_command_name) == 0) {
                        if (args != NULL) {
                            if(commands[i].sub_command[j].sub_command_func(args) != 0) {
                                log(__func__, "Error executing subcommand with args");
                            }
                        } else {
                            if(commands[i].sub_command[j].sub_command_func() != 0) {
                                log(__func__, "Error executing subcommand without args");
                            }
                        }
                        return;
                    } 
                }
            } else if (commands[i].command_func != NULL) {
                // Check if sub_command exists
                if(sub_command != NULL) {
                    // If sub_command is not NULL, pass args to command_func if it exists
                    if (args != NULL) {
                        if(commands[i].command_func(sub_command, args) != 0) {
                            log(__func__, "Error executing command with callback and args");
                        }
                    } else {
                        if(commands[i].command_func(sub_command) != 0) {
                            log(__func__, "Error executing command with callback without args");
                        }
                    }
                } else {
                    if(commands[i].command_func() != 0) {
                        log(__func__, "Error executing command without args");
                    }
                }
                return;
            }
            break;
        }
    }
    // If command not found, execute cmd_unknown
    cmd_unknown();
}
