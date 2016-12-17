#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
typedef struct xloop{
	ci_expr super;
	codeblock code;
}xloop;

inline static void _xloop_free_(ci_expr*oo){
	xloop*o=(xloop*)oo;
	_codeblock_free_((ci_expr*)&o->code);
	ci_expr_free(&o->super);
}

inline static void _xloop_compile_(const ci_expr*oo,toc*tc){
	xloop*o=(xloop*)oo;
	printf("while(1)");
	o->code.super.compile((ci_expr*)&o->code,tc);
}

#define xloop_def (xloop){\
	{str_def,_xloop_compile_,_xloop_free_},codeblock_def}


inline static xloop*xloop_read_next(const char**pp,toc*tc){
	toc_push_scope(tc,'l',"");
	xloop*o=malloc(sizeof(xloop));
	*o=xloop_def;
	codeblock_read_next(&o->code,pp,tc);
	toc_pop_scope(tc);
	return o;
}
