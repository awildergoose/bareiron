#ifndef SYS_SOCKET_H_WINDOWS_COMPAT
#define SYS_SOCKET_H_WINDOWS_COMPAT

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define F_GETFL 3
#define F_SETFL 4
#define O_NONBLOCK 0x800

static inline int fcntl(int fd, int cmd, long arg) {
  if (cmd == F_GETFL) {
    return 0;
  }
  if (cmd == F_SETFL && (arg & O_NONBLOCK)) {
    u_long mode = 1;
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode);
  }
  if (cmd == F_SETFL) {
    u_long mode = 0;
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode);
  }
  return -1;
}

#if defined(_MSC_VER) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
typedef long ssize_t;
#endif

#ifndef SHUT_RD
#define SHUT_RD SD_RECEIVE
#endif
#ifndef SHUT_WR
#define SHUT_WR SD_SEND
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

static inline int close_socket(int fd) { return closesocket((SOCKET)fd); }

static int __winsock_init_done = 0;
static void __winsock_init(void) {
  if (!__winsock_init_done) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
      fprintf(stderr, "WSAStartup failed\n");
      exit(1);
    }
    __winsock_init_done = 1;
  }
}

static inline int linux_socket(int domain, int type, int protocol) {
  __winsock_init();
  return (int)socket(domain, type, protocol);
}
#define socket(domain, type, protocol) linux_socket(domain, type, protocol)

static inline int __map_wsa_error(int wsa_err) {
  switch (wsa_err) {
  case WSAEWOULDBLOCK:
    return EWOULDBLOCK;
  case WSAEINPROGRESS:
    return EINPROGRESS;
  case WSAEALREADY:
    return EALREADY;
  case WSAENOTSOCK:
    return ENOTSOCK;
  case WSAEDESTADDRREQ:
    return EDESTADDRREQ;
  case WSAEMSGSIZE:
    return EMSGSIZE;
  case WSAEPROTOTYPE:
    return EPROTOTYPE;
  case WSAENOPROTOOPT:
    return ENOPROTOOPT;
  case WSAEPROTONOSUPPORT:
    return EPROTONOSUPPORT;
  case WSAESOCKTNOSUPPORT:
    return EPROTONOSUPPORT;
  case WSAEOPNOTSUPP:
    return EOPNOTSUPP;
  case WSAEAFNOSUPPORT:
    return EAFNOSUPPORT;
  case WSAEADDRINUSE:
    return EADDRINUSE;
  case WSAEADDRNOTAVAIL:
    return EADDRNOTAVAIL;
  case WSAENETDOWN:
    return ENETDOWN;
  case WSAENETUNREACH:
    return ENETUNREACH;
  case WSAENETRESET:
    return ENETRESET;
  case WSAECONNABORTED:
    return ECONNABORTED;
  case WSAECONNRESET:
    return ECONNRESET;
  case WSAENOBUFS:
    return ENOBUFS;
  case WSAEISCONN:
    return EISCONN;
  case WSAENOTCONN:
    return ENOTCONN;
  case WSAETIMEDOUT:
    return ETIMEDOUT;
  case WSAECONNREFUSED:
    return ECONNREFUSED;
  case WSAEHOSTUNREACH:
    return EHOSTUNREACH;
  default:
    return wsa_err;
  }
}

static inline int recv_shim(SOCKET fd, void *buf, int len, int flags) {
  int r = recv(fd, (char *)buf, len, flags);
  if (r < 0)
    errno = __map_wsa_error(WSAGetLastError());
  return r;
}
static inline int send_shim(SOCKET fd, const void *buf, int len, int flags) {
  int r = send(fd, (const char *)buf, len, flags);
  if (r < 0)
    errno = __map_wsa_error(WSAGetLastError());
  return r;
}

#undef recv
#undef send
#define recv recv_shim
#define send send_shim

#endif /* SYS_SOCKET_H_WINDOWS_COMPAT */
