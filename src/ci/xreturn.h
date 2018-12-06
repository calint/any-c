#pragma once
#include"xexpls.h"

typedef struct xreturn{
	xexp super;
	xexpls expls;
}xreturn;

#define xreturn_def (xreturn){\
	{_xreturn_compile_,_xreturn_free_,NULL,strc_def,token_def,0,false},\
	xexpls_def\
}

inline static void _xreturn_compile_(const xexp*oo,toc*tc){
	xreturn*o=(xreturn*)oo;
	printf("return ");
	if(o->super.is_ref && !o->expls.super.is_ref){
		printf("&");
	}else if(!o->super.is_ref && o->expls.super.is_ref){
		printf("*");
	}
	o->expls.super.compile((xexp*)&o->expls,tc);
	//? free scope
}

inline static void _xreturn_free_(xexp*oo){
	xreturn*o=(xreturn*)oo;
	o->expls.super.free((xexp*)&o->expls);
}

inline static xreturn*xreturn_read_next(toc*tc,token tk){
	toc_srcp_skip_if(tc,';');
	xreturn*o=malloc(sizeof(xreturn));
	*o=xreturn_def;
	o->super.token=tk;
	xexpls_parse_next(&o->expls,tc,tk,false,0,NULL);
	ci_xreturn_assert(tc,o);
	return o;
}
