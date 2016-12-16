#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"expr_assign.h"

typedef struct ci_expr_var{
	ci_expr super;
	str name;
	ci_expr_assign initval;
}ci_expr_var;

inline static void _ci_expr_var_free_(ci_expr*oo){
	ci_expr_var*o=(ci_expr_var*)oo;
	str_free(&o->name);
	_ci_expr_assign_free_((ci_expr*)&o->initval);
}

inline static void _ci_expr_var_compile_(const ci_expr*oo,ci_toc*tc){

	ci_expr_var*o=(ci_expr_var*)oo;
	const char idtype=ci_toc_find_ident_type(tc,o->initval.name.data);
	if(idtype){
		printf("\n\n<file> <line:col> '%s' is already declared at ..."
				,o->initval.name.data);
		exit(1);
		return;
	}
	ci_toc_add_ident(tc,o->initval.name.data);
	printf("%s ",o->super.type.data);
	_ci_expr_assign_compile_((ci_expr*)&o->initval,tc);
//	printf(";\n");
}

#define ci_expr_var_def (ci_expr_var){\
	{str_def,_ci_expr_var_compile_,_ci_expr_var_free_},\
	str_def,ci_expr_assign_def}

inline static /*gives*/ci_expr_var*ci_expr_var_next(
		const char**pp,ci_toc*tc,/*takes*/str type){

	ci_expr_var*e=malloc(sizeof(ci_expr_var));
	*e=ci_expr_var_def;
	e->super.type=/*gives*/type;
	token tk=token_next(pp);
	str name=str_def;
	token_setz(&tk,&name);
	if(**pp=='='){
		(*pp)++;
		ci_expr_assign_parse(&e->initval,pp,tc,/*gives*/name);
	}else{
		e->initval.super.compile=NULL;
	}
	return e;
}
