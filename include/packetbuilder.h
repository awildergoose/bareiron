#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H

#include "tools.h"
#include "varnum.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint8_t *data;
  size_t length;
  size_t capacity;
} PacketBuilder;

PacketBuilder *pb_create(size_t initial_capacity);
void pb_free(PacketBuilder *pb);

void pb_ensure(PacketBuilder *pb, size_t extra);

void pb_writeByte(PacketBuilder *pb, uint8_t val);
void pb_writeVarInt(PacketBuilder *pb, int32_t val);
void pb_writeUint16(PacketBuilder *pb, uint16_t val);
void pb_writeUint32(PacketBuilder *pb, uint32_t val);
void pb_writeUint64(PacketBuilder *pb, uint64_t val);
void pb_writeFloat(PacketBuilder *pb, float val);
void pb_writeDouble(PacketBuilder *pb, double val);
void pb_writeString(PacketBuilder *pb, const char *str);

ssize_t pb_send(PacketBuilder *pb, int client_fd, int packet_id);
void pb_reset(PacketBuilder *pb);

#endif
