#pragma once
#include"../lib.h"
#include "codeblk.h"
#include "xbool.h"

typedef struct xif{
	xexpr super;
	xbool cond;
	codeblk code;
}xif;

inline static void _xif_compile_(const xexpr*oo,toc*tc){
	xif*o=(xif*)oo;
	printf("if ");
	_xbool_compile_((xexpr*)&o->cond,tc);
	printf(" ");
	_codeblock_compile_((xexpr*)&o->code,tc);
}

#define xif_def (xif){{str_def,_xif_compile_,NULL},xbool_def,codeblk_def}

inline static xif*xif_read_next(const char**pp,toc*tc){
	xif*o=malloc(sizeof(xif));
	*o=xif_def;
	xbool_parse(&o->cond,pp,tc);
	codeblock_read_next(&o->code,pp,tc);
	return o;
}
