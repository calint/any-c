#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
typedef struct ci_expr_continue{
	ci_expr super;
}ci_expr_continue;

inline static void _ci_expr_continue_free_(ci_expr*oo){
	ci_expr_free(oo);
}

inline static void _cci_expr_continue_compile_(const ci_expr*oo,ci_toc*tc){
	printf("continue");
}

#define ci_expr_continue_def (ci_expr_continue){\
	{str_def,_cci_expr_continue_compile_,_ci_expr_continue_free_}}

inline static ci_expr_continue*ci_expr_continue_next(const char**pp,ci_toc*tc){
	if(**pp!=';'){
		printf("\n\n<file> <line:col> expected ';' after keyword 'break'");
		exit(1);
	}
	(*pp)++;
	ci_expr_continue*e=malloc(sizeof(ci_expr_continue));
	*e=ci_expr_continue_def;
	return e;
}
