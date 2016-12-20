#pragma once
#include <setjmp.h>

#define int_def 0
#define char_def 0
#define float_def 0.0f

#define bool char
#define false 0
#define bool_def false
#define true 1

#define cstr const char*
#define cstr_def ""

#define null 0

#include"lib/str.h"
#include"lib/dynp.h"
#include"lib/token.h"

static jmp_buf _jmpbufenv;

inline static str const_str(cstr s){
	str st=str_def;
	st.data=s;
	st.cap=st.count=strlen(s)+1;
	return st;
}

