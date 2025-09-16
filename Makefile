CC=clang
CFLAGS=-Iinclude -O0 -g0 -include-pch include/pch.h.pch
LDFLAGS=-lws2_32 -fuse-ld=lld

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

all: bareiron.exe

%.o: %.c include/pch.h.pch
	$(CC) $(CFLAGS) -c $< -o $@

bareiron.exe: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

include/pch.h.pch: include/pch.h
	$(CC) -x c-header include/pch.h -Iinclude -o include/pch.h.pch

clean:
	del /Q $(OBJS) bareiron.exe
	del /Q include/pch.h.pch
