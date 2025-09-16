#ifndef ARPA_INET_H_WINDOWS_COMPAT
#define ARPA_INET_H_WINDOWS_COMPAT

#include <stdint.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAVE_INET_PTON_NTOP
static inline int inet_pton(int af, const char *src, void *dst) {
#ifdef InetPton
  return InetPton(af, src, dst);
#else
  if (af == AF_INET) {
    unsigned long a = inet_addr(src);
    if (a == INADDR_NONE && strcmp(src, "255.255.255.255") != 0)
      return 0;
    *(uint32_t *)dst = htonl(a);
    return 1;
  }
  return 0;
#endif
}
static inline const char *inet_ntop(int af, const void *src, char *dst,
                                    size_t size) {
#ifdef InetNtop
  return InetNtop(af, src, dst, (ULONG)size);
#else
  if (af == AF_INET) {
    struct in_addr a;
    a.s_addr = *(const uint32_t *)src;
    const char *s = inet_ntoa(a);
    if (!s)
      return NULL;
    strncpy(dst, s, size);
    dst[size - 1] = 0;
    return dst;
  }
  return NULL;
#endif
}
#endif

#define htonl(x)                                                               \
  (((uint32_t)(x) << 24) | (((uint32_t)(x) & 0x0000FF00) << 8) |               \
   (((uint32_t)(x) & 0x00FF0000) >> 8) | ((uint32_t)(x) >> 24))
#define ntohl(x) htonl(x)
#define htons(x)                                                               \
  ((uint16_t)((((uint16_t)(x) << 8) & 0xff00) |                                \
              (((uint16_t)(x) >> 8) & 0x00ff)))
#define ntohs(x) htons(x)

#ifdef __cplusplus
}
#endif

#endif /* ARPA_INET_H_WINDOWS_COMPAT */
