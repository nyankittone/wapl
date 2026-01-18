#!/bin/sh
cflags=$(tr '\n' ' ' < compile_flags.txt)

if [ -z "$CC" ]; then
    CC=cc
fi

"$CC" $cflags "$@" -c src/main.c -o wapl.o
ar rcs libwapl.a wapl.o
