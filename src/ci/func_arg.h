#pragma once
#include"../lib.h"

typedef struct ci_func_arg{
	str type;
	str name;
}ci_func_arg;

#define ci_func_arg_def (ci_func_arg){str_def,str_def}

inline static void ci_func_arg_free(ci_func_arg*o){
	str_free(&o->name);
	str_free(&o->type);
	free(o);
}

