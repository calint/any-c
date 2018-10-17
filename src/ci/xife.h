#pragma once
#include"../lib.h"
#include "toc.h"
#include "xbool.h"
#include "xcode.h"

typedef struct xif{
	xexp super;
	xbool cond;
	xcode code;
}xif;

inline static void _xif_compile_(const xexp*oo,toc*tc){
	xif*o=(xif*)oo;
	printf("if (");
	_xbool_compile_((xexp*)&o->cond,tc);
	printf(") ");
	_xcode_compile_((xexp*)&o->code,tc);
}

#define xif_def (xif){\
	{_xif_compile_,NULL,NULL,strc_def,token_def,0,false},\
	xbool_def,xcode_def\
}

inline static xif*xif_read_next(toc*tc,token tk){
	xif*o=malloc(sizeof(xif));
	*o=xif_def;
	toc_srcp_skip_if(tc,'(');
	xbool_parse(&o->cond,tc,tk);
	toc_srcp_skip_if(tc,')');
	xcode_read_next(&o->code,tc);
	return o;
}

typedef struct xife{
	xexp super;
	ptrs ifs;
	xcode elsecode;
}xife;

inline static void _xife_compile_(const xexp*oo,toc*tc){
	xife*o=(xife*)oo;
	for(unsigned i=0;i<o->ifs.count;i++){
		xif*fi=(xif*)ptrs_get(&o->ifs,i);
		if(i>0){
			xif*prvfi=(xif*)ptrs_get(&o->ifs,i-1);
			if(xexp_is_encapsulated(&prvfi->super)){
				printf(" else ");
			}else{
				toc_print_indent_for_compile(tc);
				printf("else ");
			}
		}
		_xif_compile_((xexp*)fi,tc);
	}
	xif*lastif=(xif*)ptrs_get_last(&o->ifs);
	if(o->elsecode.exps.count){
		if(xexp_is_encapsulated(&lastif->code.super))
			printf(" else ");
		else{
			toc_print_indent_for_compile(tc);
			printf("else ");
		}
		_xcode_compile_((xexp*)&o->elsecode,tc);
		if(xexp_is_encapsulated(&o->elsecode.super)){
			printf("\n");
		}
	}else{
		if(xexp_is_encapsulated(&lastif->code.super))
			printf("\n");
	}
}

#define xife_def (xife){\
	{_xife_compile_,NULL,NULL,strc_def,token_def,2,false},\
		ptrs_def,xcode_def\
}

inline static xife*xife_read_next(toc*tc,token tk){
	xife*o=malloc(sizeof(xife));
	*o=xife_def;
	o->super.token=tk;
	while(1){
		xif*i=xif_read_next(tc,tk);
		ptrs_add(&o->ifs,i);
		token t=toc_next_token(tc);
		if(!token_equals(&t,"else")){
			tc->srcp=t.begin;
			return o;
		}
		token t2=toc_next_token(tc);
		if(!token_equals(&t2,"if")){
			tc->srcp=t2.begin;
			xcode_read_next(&o->elsecode,tc);
			return o;
		}
	}
}
