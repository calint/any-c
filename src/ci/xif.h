#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
#include "xbool.h"
typedef struct xif{
	ci_expr super;
	xbool cond;
	codeblock code;
}xif;

inline static void _xif_compile_(const ci_expr*oo,toc*tc){
	xif*o=(xif*)oo;
	printf("if ");
	_xbool_compile_((ci_expr*)&o->cond,tc);
	printf(" ");
	_codeblock_compile_((ci_expr*)&o->code,tc);
}

inline static void _xif_free_(ci_expr*oo){
	ci_expr_free(oo);
}

#define xif_def (xif){{str_def,_xif_compile_,_xif_free_},xbool_def,\
	codeblock_def}

inline static xif*xif_read_next(const char**pp,toc*tc){
	xif*o=malloc(sizeof(xif));
	*o=xif_def;
	ci_expr_bool_parse(&o->cond,pp,tc);
	codeblock_read_next(&o->code,pp,tc);
	return o;
}
