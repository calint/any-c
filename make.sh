#!/bin/sh

CC='gcc -std=c99'
SRC=src/main.c
BIN=any-c

WARNINGS='-Wall -Wextra -Wpedantic -Wfatal-errors -Wno-unused-parameter'

LIBS=

OPTIMIZATION='-O0 -g'
#OPTIMIZATION='-Os -g'
#OPTIMIZATION=-Os
#OPTIMIZATION=-Ofast

#REMARKS=-Rpass=inline
REMARKS=

#PROFILE=-pg
PROFILE=

rm -f $BIN &&

echo optimization $OPTIMIZATION &&
$CC -o $BIN $SRC $PROFILE $OPTIMIZATION $CFLAGS $LIBS $WARNINGS &&

ls -la --color $BIN
