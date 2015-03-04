#!/bin/sh
LIBS="$(pkg-config --libs pixman-1)"
CFLAGS="$(pkg-config --cflags pixman-1)"

gcc $LIBS $CFLAGS -c -fPIC server.c -o server.o
gcc $LIBS $CFLAGS -c -fPIC pingpong-protocol.c -o pingpong-protocol.o
gcc $LIBS $CFLAGS server.o pingpong-protocol.o -shared -o server.so
cp server.so /usr/lib/weston/
