#!/bin/sh
wayland-scanner client-header <protocol.xml >pingpong-client-protocol.h
wayland-scanner server-header <protocol.xml >pingpong-server-protocol.h
wayland-scanner private-code <protocol.xml >pingpong-protocol.c

