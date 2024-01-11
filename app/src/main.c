#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./commands/command_processor.h"
#include "./utils/utils.h"
int main() {
    setbuf(stdout, NULL);
    sleep(5);
    log(__func__, "Sensor Command Terminal");
    log(__func__, "Enter a command or 'exit' to quit.");
    char command[256];
    while (1) {
        printf("Enter command: ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // Handle EOF or read error
        }
        command[strcspn(command, "\n")] = 0; // Remove newline character
        if (strcmp(command, "exit") == 0) {
            break;
        }
        process_command(command);
    }
    log(__func__, "Exiting program.");
    fflush(stdout); // Flush the output buffer
    return 0;
}
