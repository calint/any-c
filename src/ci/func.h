#pragma once
#include"../lib.h"
#include "block.h"
#include "expr.h"
#include"block.h"

typedef struct funcarg{
	str type;
	str name;
}funcarg;
#define funcarg_def (funcarg){str_def,str_def}

typedef struct ci_func{
	str type;
	str name;
	dynp/*owns ci_func_arg**/args;
	codeblock code;
}ci_func;

#define ci_func_def (ci_func){str_def,str_def,dynp_def,codeblock_def}

