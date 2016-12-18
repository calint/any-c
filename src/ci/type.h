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

inline static field*type_get_field_by_name(const type*o,ccharp field_name){
	for(unsigned i=0;i<o->fields.count;i++){
		field*f=dynp_get(&o->fields,i);
		if(!strcmp(f->name.data,field_name))
				return f;
	}
	return NULL;
}

inline static func*type_get_func_by_name(const type*o,ccharp field_name){
	for(unsigned i=0;i<o->funcs.count;i++){
		func*f=dynp_get(&o->funcs,i);
		if(!strcmp(f->name.data,field_name))
				return f;
	}
	return NULL;
}
