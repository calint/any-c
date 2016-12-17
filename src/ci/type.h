#pragma once
#include"../lib.h"
#include "codeblock.h"

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
	dynp args;//owns arg*
	codeblk code;
}func;

#define func_def (func){str_def,str_def,dynp_def,codeblk_def}

typedef struct type{
	str name;
	dynp/*owns str*/extends;
	dynp/*owns field*/fields;
	dynp/*owns func*/funcs;
}type;

#define type_def (type){str_def,dynp_def,dynp_def,dynp_def}
