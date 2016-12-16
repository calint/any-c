#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
typedef struct ci_expr_break{
	ci_expr super;
}ci_expr_break;

inline static void _ci_expr_break_free_(ci_expr*oo){
	ci_expr_free(oo);
}

inline static void _ci_expr_break_compile_(const ci_expr*oo,ci_toc*tc){
	printf("break");
}

#define ci_expr_break_def (ci_expr_break){\
	{str_def,_ci_expr_break_compile_,_ci_expr_break_free_}}

inline static ci_expr_break*ci_expr_break_next(const char**pp,ci_toc*tc){
	if(**pp!=';'){
		printf("\n\n<file> <line:col> expected ';' after keyword 'break'");
		exit(1);
	}
	(*pp)++;
	ci_expr_break*e=malloc(sizeof(ci_expr_break));
	*e=ci_expr_break_def;
	return e;
}
