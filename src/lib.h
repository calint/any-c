#pragma once
#include<setjmp.h>
static jmp_buf _jmp_buf;

#define int_def 0
#define char_def 0
#define float_def 0.0f

typedef char bool;
#define false 0
#define bool_def false
#define true 1

typedef const char* strc;
#define strc_def ""

#define null 0

#include"lib/strb.h"
#include"lib/ptrs.h"
#include"lib/token.h"
