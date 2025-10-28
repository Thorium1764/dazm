#!/bin/bash

set -e

BUILD=/home/tobias/code/dazm/build/

for f in *.c; do
    [ -f "$f" ] || continue    # skip if no .c files
    obj="$BUILD/${f%.c}.o"
    gcc "$f" -o "$obj" -c
done

gcc -o "$BUILD/dazm" "$BUILD"/*.o


