#!/bin/bash

set -e

touch dasm.out

objdump -d "$1" | awk '                                                                                                                                                                                                     
  /^[[:space:]]*[0-9A-Fa-f]+:/ {
    addr=$1
    bytes=""
    for (i=2; i<=NF; i++) {
      if ($i ~ /^[0-9A-Fa-f]{2}$/)  # only 2 hex digits
        bytes = bytes (bytes ? " " : "") $i
      else
        break
    }
    printf "%s\t%s\n", addr, bytes
  }' > dasm.out

