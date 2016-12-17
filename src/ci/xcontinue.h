#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
typedef struct xcontinue{
	ci_expr super;
}xcontinue;

inline static void _xcontinue_free_(ci_expr*oo){
	ci_expr_free(oo);
}

inline static void _xcontinue_compile_(const ci_expr*oo,toc*tc){
	printf("continue");
}

#define xcontinue_def (xcontinue){\
	{str_def,_xcontinue_compile_,_xcontinue_free_}}

inline static xcontinue*xcontinue_read_next(const char**pp,toc*tc){
	if(**pp!=';'){
		printf("\n\n<file> <line:col> expected ';' after keyword 'break'");
		exit(1);
	}
	(*pp)++;
	xcontinue*e=malloc(sizeof(xcontinue));
	*e=xcontinue_def;
	return e;
}
