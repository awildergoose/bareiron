#ifndef SYS_TYPES_H_WINDOWS_COMPAT
#define SYS_TYPES_H_WINDOWS_COMPAT

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#if defined(_MSC_VER) || defined(_WIN32)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
typedef long ssize_t;
#endif

typedef int pid_t;
typedef int uid_t;
typedef int gid_t;
typedef long long off_t;
typedef unsigned int mode_t;
typedef unsigned long ino_t;
typedef unsigned short nlink_t;
typedef unsigned int dev_t;
typedef long blksize_t;
typedef long long blkcnt_t;

#endif /* SYS_TYPES_H_WINDOWS_COMPAT */
