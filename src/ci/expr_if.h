#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
#include "expr_bool.h"
typedef struct ci_expr_if{
	ci_expr super;
	ci_expr_bool cond;
	ci_block code;
}ci_expr_if;

inline static void _ci_expr_if_compile_(const ci_expr*oo,ci_toc*tc){
	ci_expr_if*o=(ci_expr_if*)oo;
	printf("if ");
	_ci_expr_bool_compile_((ci_expr*)&o->cond,tc);
	printf(" ");
	_ci_block_compile_((ci_expr*)&o->code,tc);
}

inline static void _ci_expr_if_free_(ci_expr*oo){
	ci_expr_free(oo);
}

#define ci_expr_if_def (ci_expr_if){\
	{str_def,_ci_expr_if_compile_,_ci_expr_if_free_},\
	ci_expr_bool_def,ci_block_def}

inline static ci_expr_if*ci_expr_if_next(const char**pp,ci_toc*tc){
	ci_expr_if*o=malloc(sizeof(ci_expr_if));
	*o=ci_expr_if_def;
	ci_expr_bool_parse(&o->cond,pp,tc);
	ci_block_parse(&o->code,pp,tc);
	return o;
}
