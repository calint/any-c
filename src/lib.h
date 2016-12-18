#pragma once
#include <setjmp.h>
#define bool char
#define true 1
#define false 0

typedef const char* ccharp;
#define ccharp_def ""

#include"lib/str.h"
#include"lib/dynp.h"
#include"lib/token.h"

static jmp_buf _jmpbufenv;

inline static str const_str(ccharp s){
	str st=str_def;
	st.data=s;
	st.cap=st.count=strlen(s)+1;
	return st;
}

