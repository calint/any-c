#pragma once
#include"../lib.h"
#include "code.h"

typedef struct field{
	str type;
	str name;
	xexpr*initval;
}field;

#define field_def (field){str_def,str_def,NULL}

typedef struct funcarg{
	str type;
	str name;
}funcarg;

#define funcarg_def (funcarg){str_def,str_def}

typedef struct func{
	str type;
	str name;
	dynp args;
	code code;
}func;

#define func_def (func){str_def,str_def,dynp_def,code_def}

typedef struct type{
	str name;
	dynp fields;
	dynp funcs;
	token token;
}type;

#define type_def (type){str_def,dynp_def,dynp_def,token_def}
