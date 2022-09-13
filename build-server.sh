#!/bin/sh
LIBS="$(pkg-config --libs pixman-1 libweston-9)"
CFLAGS="$(pkg-config --cflags pixman-1 libweston-9)"

gcc $LIBS $CFLAGS -c -fPIC server.c -o server.o
gcc $LIBS $CFLAGS -c -fPIC pingpong-protocol.c -o pingpong-protocol.o
gcc $LIBS $CFLAGS server.o pingpong-protocol.o -shared -o server.so
sudo cp server.so /usr/local/lib/x86_64-linux-gnu/weston/
