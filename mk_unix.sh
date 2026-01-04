#!/bin/sh
cflags=$(tr '\n' ' ' < compile_flags.txt)

cc $cflags src/main.c -o wapl.o
ar rcs libwapl.a wapl.o
