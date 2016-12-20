#pragma once
#include"../lib.h"
#include"xexpls.h"

typedef struct xreturn{
	xexp super;
	xexpls expls;
}xreturn;

inline static void _xreturn_compile_(const xexp*oo,toc*tc){
	xreturn*o=(xreturn*)oo;
	printf("return ");
	o->expls.super.compile((xexp*)&o->expls,tc);
}

#define xreturn_def (xreturn){{_xreturn_compile_,NULL,str_def,token_def,0},\
	xexpls_def}

inline static xreturn*xreturn_read_next(toc*tc,token tk){
	toc_srcp_skip_if(tc,';');
	xreturn*o=malloc(sizeof(xreturn));
	*o=xreturn_def;
	o->super.token=tk;
	xexpls_parse_next(&o->expls,tc,tk);
	//? assert return value type
	return o;
}
