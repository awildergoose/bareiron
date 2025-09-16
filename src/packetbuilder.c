#include "packetbuilder.h"
#include <stdint.h>
#include <string.h>

PacketBuilder *pb_create(size_t initial_capacity) {
  PacketBuilder *pb = (PacketBuilder *)malloc(sizeof(PacketBuilder));
  pb->length = 0;
  pb->capacity = initial_capacity > 0 ? initial_capacity : 128;
  pb->data = (uint8_t *)malloc(pb->capacity);
  return pb;
}

void pb_free(PacketBuilder *pb) {
  if (!pb)
    return;
  free(pb->data);
  free(pb);
}

void pb_ensure(PacketBuilder *pb, size_t extra) {
  if (pb->length + extra > pb->capacity) {
    while (pb->length + extra > pb->capacity)
      pb->capacity *= 2;
    pb->data = (uint8_t *)realloc(pb->data, pb->capacity);
  }
}

void pb_writeByte(PacketBuilder *pb, uint8_t val) {
  pb_ensure(pb, 1);
  pb->data[pb->length++] = val;
}

void pb_writeVarInt(PacketBuilder *pb, int32_t val) {
  while (1) {
    if ((val & ~SEGMENT_BITS) == 0) {
      pb_writeByte(pb, val);
      return;
    }
    pb_writeByte(pb, (val & SEGMENT_BITS) | CONTINUE_BIT);
    val >>= 7;
  }
}

void pb_writeUint16(PacketBuilder *pb, uint16_t val) {
  pb_ensure(pb, 2);
  pb->data[pb->length++] = (val >> 8) & 0xFF;
  pb->data[pb->length++] = val & 0xFF;
}

void pb_writeUint32(PacketBuilder *pb, uint32_t val) {
  pb_ensure(pb, 4);
  pb->data[pb->length++] = (val >> 24) & 0xFF;
  pb->data[pb->length++] = (val >> 16) & 0xFF;
  pb->data[pb->length++] = (val >> 8) & 0xFF;
  pb->data[pb->length++] = val & 0xFF;
}

void pb_writeUint64(PacketBuilder *pb, uint64_t val) {
  pb_ensure(pb, 8);
  for (int i = 7; i >= 0; i--)
    pb->data[pb->length++] = (val >> (i * 8)) & 0xFF;
}

void pb_writeFloat(PacketBuilder *pb, float val) {
  uint32_t bits;
  memcpy(&bits, &val, 4);
  pb_writeUint32(pb, bits);
}

void pb_writeDouble(PacketBuilder *pb, double val) {
  uint64_t bits;
  memcpy(&bits, &val, 8);
  pb_writeUint64(pb, bits);
}

void pb_writeString(PacketBuilder *pb, const char *str) {
  size_t len = strlen(str);
  pb_writeVarInt(pb, (int32_t)len);
  pb_ensure(pb, len);
  memcpy(pb->data + pb->length, str, len);
  pb->length += len;
}

ssize_t pb_send(PacketBuilder *pb, int client_fd, int packet_id) {
  int packet_id_size = sizeVarInt(packet_id);
  int total_len = packet_id_size + pb->length;
  writeVarInt(client_fd, total_len);
  writeVarInt(client_fd, packet_id);
  return send_all(client_fd, pb->data, pb->length);
}

void pb_reset(PacketBuilder *pb) { pb->length = 0; }
