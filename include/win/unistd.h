#ifndef WIN_UNISTD_H
#define WIN_UNISTD_H

#include <io.h>
#include <stdint.h>
#include <windows.h>

#undef close
#define close(fd) closesocket((SOCKET)(fd))
#include <process.h>
#include <time.h>

#if defined(_MSC_VER) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
typedef long ssize_t;
#endif

#define read _read
#define write _write
#define getpid _getpid

#if !defined(_INC_TIME)
#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC
struct timespec {
  long tv_sec;
  long tv_nsec;
};
#endif
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif

static inline int clock_gettime(int clk_id, struct timespec *tp) {
  if (!tp)
    return -1;

  if (clk_id == CLOCK_REALTIME) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    uint64_t ns100 = uli.QuadPart;
    uint64_t ns = ns100 * 100ULL;

    tp->tv_sec = (long)(ns / 1000000000ULL);
    tp->tv_nsec = (long)(ns % 1000000000ULL);
    return 0;
  }

  if (clk_id == CLOCK_MONOTONIC) {
    static LARGE_INTEGER freq;
    static BOOL freq_init = FALSE;
    if (!freq_init) {
      QueryPerformanceFrequency(&freq);
      freq_init = TRUE;
    }

    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);

    double sec = (double)count.QuadPart / (double)freq.QuadPart;
    tp->tv_sec = (long)sec;
    tp->tv_nsec = (long)((sec - tp->tv_sec) * 1e9);
    return 0;
  }

  return -1;
}

#endif /* WIN_UNISTD_H */
