#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"

typedef struct ci_expr_ident{
	ci_expr super;
	str name;
	long int cnst;
}ci_expr_ident;

inline static void _ci_expr_ident_free_(struct ci_expr*oo){
	ci_expr_ident*o=(ci_expr_ident*)oo;
//	str_free(&o->name);
	ci_expr_free((ci_expr*)o);
}

inline static void _ci_expr_ident_compile_(const ci_expr*oo,ci_toc*tc){

	ci_expr_ident*o=(ci_expr_ident*)oo;
	const char idtype=ci_toc_find_ident_type(tc,o->name.data);
	if(idtype=='c'){// class member
		printf("o->%s",o->name.data);
		o->cnst=0;
		return;
	}
	if(idtype){// local identifier
		printf("%s",o->name.data);
		o->cnst=0;
		return;
	}
	// constant
	o->cnst=1;
	char* endptr;
	o->cnst=strtol(o->name.data,&endptr,10);
	if(*endptr!=0){
		// not end of string
		printf("\n\n<file> <line:col> '%s' is not found and could not "
				"be parsed to a number\n\n",o->name.data);
		exit(1);

	}
	str_setz(&o->super.type,"int");
	printf("%s",o->name.data);
}

#define ci_expr_ident_def (ci_expr_ident){\
	{str_def,_ci_expr_ident_compile_,_ci_expr_ident_free_},str_def,0}

//inline static void ci_expr_ident_free(ci_expr_ident*o){
//	ci_expression_free(&o->super);
//	free(o);
//}
