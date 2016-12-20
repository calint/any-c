#!/bin/sh

CC='gcc -std=gnu11'
SRC=src/main.c
BIN=any-c

WARNINGS='-Wall -Wextra -Wfatal-errors -Wno-unused-parameter -Wno-discarded-qualifiers'

LIBS=

#OPTIMIZATION='-O0 -g'
#OPTIMIZATION=-Os
#OPTIMIZATION=-O3
OPTIMIZATION=-Ofast

#REMARKS=-Rpass=inline
REMARKS=

#PROFILE=-pg
PROFILE=

rm -f $BIN &&

$CC $PROFILE $SRC -o $BIN $OPTIMIZATION $CFLAGS $LIBS $WARNINGS &&

ls -la --color $BIN
