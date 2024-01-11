#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
void get_current_date_time(char *buffer, size_t buffer_size) {
    time_t rawtime;
    struct tm *timeinfo;

    // Get the current time
    time(&rawtime);
    // Convert it to local time
    timeinfo = localtime(&rawtime);
    // Format the time and store it in the buffer
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timeinfo);
}
void log(const char *func, const char *format, ...) {
    char date_time[20];  // Buffer to hold the date-time string
    char message[1024];
    va_list args;
    // Get the current date-time
    get_current_date_time(date_time, sizeof(date_time));
    // Format the log message
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    // get container name
    char *container_name = getenv("CONTAINER_NAME");
    // Print or save the log message
    printf("[%s][%s][%s]: %s\n", container_name, date_time, func, message);
}