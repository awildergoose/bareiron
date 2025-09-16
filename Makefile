CC=clang
CFLAGS=-Iinclude -D_CRT_SECURE_NO_WARNINGS -O0 -g0
LDFLAGS=-lws2_32 -fuse-ld=lld

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

all: bareiron.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bareiron.exe: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	del /Q $(OBJS) bareiron.exe
