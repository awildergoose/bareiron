#ifndef NETINET_IN_H_WINDOWS_COMPAT
#define NETINET_IN_H_WINDOWS_COMPAT

#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>


typedef ADDRESS_FAMILY sa_family_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

#ifndef INADDR_ANY
#define INADDR_ANY ((u_long)0x00000000)
#endif
#ifndef INADDR_BROADCAST
#define INADDR_BROADCAST ((u_long)0xffffffff)
#endif
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK 0x7f000001
#endif

#ifndef IN6ADDR_ANY_INIT
#define IN6ADDR_ANY_INIT {{0}}
#endif
#ifndef IN6ADDR_LOOPBACK_INIT
#define IN6ADDR_LOOPBACK_INIT {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}}
#endif

#ifndef ntohl
#define ntohl(x) _byteswap_ulong(x)
#endif
#ifndef htonl
#define htonl(x) _byteswap_ulong(x)
#endif
#ifndef ntohs
#define ntohs(x) _byteswap_ushort(x)
#endif
#ifndef htons
#define htons(x) _byteswap_ushort(x)
#endif

#endif /* NETINET_IN_H_WINDOWS_COMPAT */
