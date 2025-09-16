#include "commands.h"
#include "globals.h"
#include "packetbuilder.h"
#include "packets.h"
#include "procedures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "globals.h"
#include "packetbuilder.h"
#include "packets.h"
#include "procedures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Command parse_command(char *input) {
  Command cmd = {0};
  size_t capacity = 4;
  cmd.args = malloc(capacity * sizeof(char *));
  cmd.count = 0;

  char *token = strtok(input, " ");
  while (token != NULL) {
    if (cmd.count >= capacity) {
      capacity *= 2;
      cmd.args = realloc(cmd.args, capacity * sizeof(char *));
    }
    cmd.args[cmd.count++] = token;
    token = strtok(NULL, " ");
  }

  return cmd;
}

void free_command(Command *cmd) {
  free(cmd->args);
  cmd->args = NULL;
  cmd->count = 0;
}

int str_to_int(const char *str, int *out) {
  char *end;
  long val = strtol(str, &end, 10);
  if (*end != '\0')
    return 0;
  *out = (int)val;
  return 1;
}

int str_to_float(const char *str, float *out) {
  char *end;
  float val = strtof(str, &end);
  if (*end != '\0')
    return 0;
  *out = val;
  return 1;
}

int startswith(const char *str, const char *prefix) {
  if (!str || !prefix)
    return 0;
  size_t len_prefix = strlen(prefix);
  size_t len_str = strlen(str);
  if (len_prefix > len_str)
    return 0;
  return strncmp(str, prefix, len_prefix) == 0;
}

int invalid_arguments(int client_fd) {
  char *s = "Invalid argument count";
  sc_systemChat(client_fd, s, strlen(s));
  return 1;
}

int cmd_gamemode(Command cmd, int client_fd) {
  if (cmd.count != 2)
    return invalid_arguments(client_fd);

  int gamemode = GAMEMODE_SURVIVAL;
  if (startswith(cmd.args[1], "c"))
    gamemode = GAMEMODE_CREATIVE;
  if (startswith(cmd.args[1], "sp"))
    gamemode = GAMEMODE_SPECTATOR;
  if (startswith(cmd.args[1], "a"))
    gamemode = GAMEMODE_ADVENTURE;

  PlayerData *player;
  if (getPlayerData(client_fd, &player))
    return 1;

  sc_gameEvent(client_fd, GAME_EVENT_CHANGE_GAME_MODE, gamemode);
  player->gamemode = gamemode;
  return 0;
}

int cmd_endtest(Command cmd, int client_fd) {
  PacketBuilder *pb = pb_create(128);
  pb_writeByte(pb, GAME_EVENT_BEGIN_RAINING);
  pb_writeFloat(pb, 0);
  pb_send(pb, client_fd, 0x22);

  pb_reset(pb);
  pb_writeByte(pb, GAME_EVENT_RAIN_LEVEL_CHANGE);
  pb_writeFloat(pb, 1);
  pb_send(pb, client_fd, 0x22);

  pb_free(pb);
  return 0;
}

RegisteredCommand commands[] = {{"gamemode", 1, cmd_gamemode},
                                {"endtest", 0, cmd_endtest},
                                {NULL, 0, NULL}};

int handle_command(Command cmd, int client_fd) {
  if (cmd.count == 0)
    return 1;

  for (int i = 0; commands[i].name != NULL; i++) {
    if (strcmp(cmd.args[0], commands[i].name) == 0) {
      int expected = commands[i].arg_count;
      if (expected >= 0 && (int)(cmd.count - 1) != expected)
        return invalid_arguments(client_fd);

      return commands[i].callback(cmd, client_fd);
    }
  }

  printf("Unknown command: %s\n", cmd.args[0]);
  return 1;
}
