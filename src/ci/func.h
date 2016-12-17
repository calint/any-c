#pragma once
#include"../lib.h"
#include "codeblock.h"

typedef struct funcarg{
	str type;
	str name;
}funcarg;

#define funcarg_def (funcarg){str_def,str_def}

typedef struct func{
	str type;
	str name;
	dynp args;//owns arg*
	codeblock code;
}func;

#define func_def (func){str_def,str_def,dynp_def,codeblock_def}

