#pragma once
#include <setjmp.h>
#define bool char
#define true 1
#define false 0

#include"lib/str.h"
#include"lib/dynp.h"
#include"lib/token.h"

static jmp_buf _jmpbufenv;
