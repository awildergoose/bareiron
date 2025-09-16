#ifndef COMMANDS_H
#define COMMANDS_H

#include "globals.h"
#include <stddef.h>

typedef struct {
  char **args;
  size_t count;
} Command;

Command parse_command(char *input);
void free_command(Command *cmd);
int str_to_int(const char *str, int *out);
int str_to_float(const char *str, float *out);
int handle_command(Command cmd, int client_fd);

#endif
