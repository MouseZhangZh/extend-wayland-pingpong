#!/bin/sh
LIBS="$(pkg-config --libs wayland-client)"
CFLAGS="$(pkg-config --cflags wayland-client)"
gcc $LIBS $CFLAGS -o client client.c pingpong-protocol.c
