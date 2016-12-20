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

typedef const char* cstr;
#define cstr_def ""

#define null 0

#include"lib/str.h"
#include"lib/dynp.h"
#include"lib/token.h"
//
//inline static str const_str(cstr s){
//	str st=str_def;
//	st.data=s;
//	st.cap=st.count=strlen(s)+1;
//	return st;
//}
