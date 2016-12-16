#pragma once
#include"../lib.h"
#include"expr.h"

typedef struct ci_field{
	str type;
	str name;
	ci_expr*initval;
}ci_field;

#define ci_field_def (ci_field){str_def,str_def,NULL}

inline static void ci_field_free(ci_field*o){
	if(o->initval->free)
		o->initval->free(o->initval);
	str_free(&o->type);
	str_free(&o->name);
	free(o);
}
