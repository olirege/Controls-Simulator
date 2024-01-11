#ifndef UTILS_H
#define UTILS_H
#include <time.h>
void get_current_date_time(char *buffer, size_t bufferSize);
void log(const char *func, const char *format, ...);
#endif // UTILS_H