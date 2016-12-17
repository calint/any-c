#pragma once
#include"../lib.h"
#include "code.h"

typedef struct field{
	str type;
	str name;
	xexpr*initval;
	token token;
}field;

#define field_def (field){str_def,str_def,NULL,token_def}

typedef struct funcarg{
	str type;
	str name;
	token token;
}funcarg;

#define funcarg_def (funcarg){str_def,str_def,token_def}

typedef struct func{
	str type;
	str name;
	dynp args;
	code code;
	token token;
}func;

#define func_def (func){str_def,str_def,dynp_def,code_def,token_def}

typedef struct type{
	str name;
	dynp fields;
	dynp funcs;
	token token;
}type;

#define type_def (type){str_def,dynp_def,dynp_def,token_def}
