#pragma once
#include "xcode.h"

typedef struct xloop{
	xexp super;
	xcode code;
}xloop;

inline static void _xloop_compile_(const xexp*oo,toc*tc){
	xloop*o=(xloop*)oo;
	printf("while(1)");
	o->code.super.compile((xexp*)&o->code,tc);
}

#define xloop_def (xloop){{_xloop_compile_,NULL,cstr_def,token_def,1},\
							xcode_def}

inline static xloop*xloop_read_next(toc*tc){
	toc_push_scope(tc,'l',"");
	xloop*o=malloc(sizeof(xloop));
	*o=xloop_def;
	xcode_read_next(&o->code,tc);
	toc_pop_scope(tc);
	return o;
}

