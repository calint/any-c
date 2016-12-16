#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
typedef struct ci_expr_loop{
	ci_expr super;
	ci_block code;
}ci_expr_loop;

inline static void _ci_expr_loop_free_(ci_expr*oo){
	ci_expr_loop*o=(ci_expr_loop*)oo;
	_ci_block_free_((ci_expr*)&o->code);
	ci_expr_free(&o->super);
}

inline static void _ci_ci_expr_loop_compile_(const ci_expr*oo,ci_toc*tc){
	ci_expr_loop*o=(ci_expr_loop*)oo;
	printf("while(1)");
	o->code.super.compile((ci_expr*)&o->code,tc);
}

#define ci_expr_loop_def (ci_expr_loop){\
	{str_def,_ci_ci_expr_loop_compile_,_ci_expr_loop_free_},ci_block_def}


inline static ci_expr_loop*ci_expr_loop_next(const char**pp,ci_toc*tc){
	ci_toc_push_scope(tc,'l',"");
	ci_expr_loop*o=malloc(sizeof(ci_expr_loop));
	*o=ci_expr_loop_def;
	ci_block_parse(&o->code,pp,tc);
	ci_toc_pop_scope(tc);
	return o;
}
