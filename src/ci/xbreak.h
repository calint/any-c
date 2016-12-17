#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
typedef struct xbreak{
	ci_expr super;
}xbreak;

inline static void _xbreak_free_(ci_expr*oo){
	ci_expr_free(oo);
}

inline static void _xbreak_compile_(const ci_expr*oo,toc*tc){
	printf("break");
}

#define ci_expr_break_def (xbreak){{str_def,_xbreak_compile_,_xbreak_free_}}

inline static xbreak*xbreak_read_next(const char**pp,toc*tc){
	if(**pp!=';'){
		printf("\n\n<file> <line:col> expected ';' after keyword 'break'");
		exit(1);
	}
	(*pp)++;
	xbreak*e=malloc(sizeof(xbreak));
	*e=ci_expr_break_def;
	return e;
}
