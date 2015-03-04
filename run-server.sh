#!/bin/sh
set -v
export XDG_RUNTIME_DIR=/tmp
# default backend and shell don't work on this baserock machine,
# can't be bothered to fix it.
weston --backend=headless-backend.so --shell="" --modules="server.so"
