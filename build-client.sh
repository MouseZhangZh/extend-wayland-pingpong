#!/bin/sh
gcc $LIBS $CFLAGS -o client client.c pingpong-protocol.c -lwayland-client
