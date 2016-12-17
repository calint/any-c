#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include "xassign.h"

typedef struct xvar{
	ci_expr super;
	str name;
	xassign initval;
}xvar;

inline static void _xvar_free_(ci_expr*oo){
	xvar*o=(xvar*)oo;
	str_free(&o->name);
	_xassign_free_((ci_expr*)&o->initval);
}

inline static void _xvar_compile_(const ci_expr*oo,toc*tc){
	xvar*o=(xvar*)oo;
	const char idtype=_ci_toc_find_ident_scope_type(tc,o->name.data);
	if(idtype){
		printf("\n\n<file> <line:col> '%s' is already declared at ..."
				,o->initval.name.data);
		exit(1);
		return;
	}

	_ci_toc_add_ident(tc,o->super.type.data,o->name.data);

	printf("%s ",o->super.type.data);
	if(o->initval.super.compile){
		_xassign_compile_((ci_expr*)&o->initval,tc);
	}else{
		printf("%s=%s_def",o->name.data,o->super.type.data);
	}
}

#define xvar_def (xvar){{str_def,_xvar_compile_,_xvar_free_},str_def,\
	ci_expr_assign_def}

inline static /*gives*/xvar*xvar_read_next(
		const char**pp,toc*tc,/*takes*/str type){

	xvar*e=malloc(sizeof(xvar));
	*e=xvar_def;
	e->super.type/*takes*/=type;
	token tk=token_next(pp);
	token_setz(&tk,&e->name);

	if(!strcmp("o",e->name.data)){
		printf("\n\n<file> <line:col> identifier 'o' is reserved");
		exit(1);
	}

	bool is_var=!strcmp(e->super.type.data,"var")||
			!strcmp(e->super.type.data,"auto");

	if(**pp=='='){
		(*pp)++;
		xassign_parse(&e->initval,pp,tc,/*shares*/e->name);
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
