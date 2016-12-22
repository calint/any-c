#pragma once
#include"xexpls.h"

typedef struct xreturn{
	xexp super;
	xexpls expls;
}xreturn;

inline static void _xreturn_compile_(const xexp*oo,toc*tc){
	xreturn*o=(xreturn*)oo;
	printf("return ");
	if(o->super.is_ref && !o->expls.super.is_ref){
		printf("&");
	}else if(!o->super.is_ref && o->expls.super.is_ref){
		printf("*");
	}
	o->expls.super.compile((xexp*)&o->expls,tc);
}

#define xreturn_def (xreturn){\
	{_xreturn_compile_,NULL,cstr_def,token_def,0,false},\
	xexpls_def\
}

inline static xreturn*xreturn_read_next(toc*tc,token tk){
	toc_srcp_skip_if(tc,';');
	xreturn*o=malloc(sizeof(xreturn));
	*o=xreturn_def;
	o->super.token=tk;
	xexpls_parse_next(&o->expls,tc,tk);
	ci_xreturn_assert(tc,o);
	return o;
}
