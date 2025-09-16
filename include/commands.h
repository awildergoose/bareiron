#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

typedef struct {
  char **args;
  size_t count;
} Command;

typedef int (*CommandCallback)(Command cmd, int client_fd);

typedef struct {
  const char *name;
  int arg_count;
  CommandCallback callback;
} RegisteredCommand;

Command parse_command(char *input);
void free_command(Command *cmd);
int handle_command(Command cmd, int client_fd);

int str_to_int(const char *str, int *out);
int str_to_float(const char *str, float *out);
int startswith(const char *str, const char *prefix);
int invalid_arguments(int client_fd);

#endif
