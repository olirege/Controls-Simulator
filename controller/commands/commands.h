#ifndef COMMANDS_H
#define COMMANDS_H

typedef int (*CommandFunc)();

typedef struct {
    const char* sub_command_name;
    int (*sub_command_func)();
} SubCommand;

typedef struct {
    const char* command_name;
    SubCommand* sub_command;
    CommandFunc command_func;
} Command;

extern Command commands[]; // Declaration of the commands array

#endif // COMMANDS_H