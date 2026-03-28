# Makefile for Half-Duplex Named Pipe example (Windows)
# Uses MinGW-w64 (gcc) on Windows or cross-compilation on Linux.

CC      = x86_64-w64-mingw32-gcc
CFLAGS  = -Wall -Wextra
LDFLAGS = -lkernel32

all: servidor.exe cliente.exe

servidor.exe: servidor.c
$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

cliente.exe: cliente.c
$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
del /Q servidor.exe cliente.exe 2>nul || rm -f servidor.exe cliente.exe

.PHONY: all clean
