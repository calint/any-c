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
	const char idtype=ci_toc_find_ident_scope_type(tc,o->name.data);
	if(idtype){
		printf("\n\n<file> <line:col> '%s' is already declared at ..."
				,o->initval.name.data);
		exit(1);
		return;
	}

	ci_toc_ident*id=(ci_toc_ident*)malloc(sizeof(ci_toc_ident));
	*id=ci_toc_ident_def;
	str_setz(&id->type,o->super.type.data);
	str_setz(&id->name,o->name.data);
	ci_toc_add_ident(tc,id);

	printf("%s ",o->super.type.data);
	if(o->initval.super.compile){
		_ci_expr_assign_compile_((ci_expr*)&o->initval,tc);
	}else{
		printf("%s=%s_def",o->name.data,o->super.type.data);
	}
}

#define ci_expr_var_def (ci_expr_var){\
	{str_def,_ci_expr_var_compile_,_ci_expr_var_free_},\
	str_def,ci_expr_assign_def}

inline static /*gives*/ci_expr_var*ci_expr_var_next(
		const char**pp,ci_toc*tc,/*takes*/str type){

	ci_expr_var*e=malloc(sizeof(ci_expr_var));
	*e=ci_expr_var_def;
	e->super.type/*takes*/=type;
	token tk=token_next(pp);
	token_setz(&tk,&e->name);
	bool is_var=!strcmp(e->super.type.data,"var")||
			!strcmp(e->super.type.data,"auto");
	if(**pp=='='){
		(*pp)++;
		ci_expr_assign_parse(&e->initval,pp,tc,/*shares*/e->name);
		if(is_var){
			e->super.type=e->initval.super.type;
		}else{
			if(strcmp(e->super.type.data,e->initval.super.type.data)){
				printf("\n\n<file> <line:col> %s declared as %s but initial"
						" expression is of type %s\n",
						e->initval.name.data,
						e->super.type.data,
						e->initval.super.type.data);
				exit(1);
			}
		}
	}else{
		e->initval.super.compile=NULL;
	}
	return e;
}
