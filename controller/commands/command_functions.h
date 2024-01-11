#ifndef COMMAND_FUNCIONS_H
#define COMMAND_FUNCIONS_H

int cmd_add_sensor(char *sensor_name);
int cmd_remove_sensor(char *sensor_name);
int cmd_stop_sensor(char *sensor_name);
int cmd_start_sensor(char *sensor_name);
int cmd_all_sensors(char *command);
int cmd_unknown();
#endif // COMMAND_FUNCIONS_H

