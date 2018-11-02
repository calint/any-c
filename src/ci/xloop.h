#pragma once
#include "xcode.h"

typedef struct xloop{
	xexp super;
	xcode code;
}xloop;

inline static void _xloop_compile_(const xexp*oo,toc*tc){
	xloop*o=(xloop*)oo;
	toc_push_scope(tc,'l',"");
	printf("while(1)");
	o->code.super.compile((xexp*)&o->code,tc);
	toc_pop_scope(tc);
}

inline static void _xloop_free_(xexp*oo){
	xloop*o=(xloop*)oo;
	o->code.super.free((xexp*)&o->code);
}

#define xloop_def (xloop){\
	{_xloop_compile_,_xloop_free_,NULL,strc_def,token_def,1,false},\
		xcode_def\
}

inline static xloop*xloop_read_next(toc*tc,token tk){
	toc_push_scope(tc,'l',"");
	xloop*o=malloc(sizeof(xloop));
	*o=xloop_def;
	o->super.token=tk;
	xcode_read_next(&o->code,tc);
	toc_pop_scope(tc);
	return o;
}

