#!/bin/sh

CC='gcc -std=gnu11'
SRC=src/main.c
BIN=any-c

WARNINGS='-Wall -Wextra -Wfatal-errors -Wno-unused-function 
   -Wno-unused-parameter -Wno-incompatible-pointer-types-discards-qualifiers -Wno-discarded-qualifiers'

LIBS=
OPTIMIZATION=-O3

#REMARKS=-Rpass=inline
REMARKS=

#PROFILE=-pg
PROFILE=

$CC $PROFILE $SRC -o $BIN $OPTIMIZATION $CFLAGS $LIBS $WARNINGS &&

ls -la --color $BIN
