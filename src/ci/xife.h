#pragma once
#include"../lib.h"
#include "code.h"
#include "toc.h"
#include "xbool.h"

typedef struct xif{
	xexpr super;
	xbool cond;
	code code;
}xif;

inline static void _xif_compile_(const xexpr*oo,toc*tc){
	xif*o=(xif*)oo;
	printf("if (");
	_xbool_compile_((xexpr*)&o->cond,tc);
	printf(") ");
	_code_compile_((xexpr*)&o->code,tc);
}

#define xif_def (xif){{_xif_compile_,NULL,str_def,token_def,0},xbool_def,code_def}

inline static xif*xif_read_next(toc*tc,token tk){
	xif*o=malloc(sizeof(xif));
	*o=xif_def;
	toc_charp_skip_if(tc,'(');
	xbool_parse(&o->cond,tc,tk);
	toc_charp_skip_if(tc,')');
	code_read_next(&o->code,tc);
	return o;
}

typedef struct xife{
	xexpr super;
	dynp ifs;
	code elsecode;
}xife;

inline static void _xife_compile_(const xexpr*oo,toc*tc){
	xife*o=(xife*)oo;
	for(unsigned i=0;i<o->ifs.count;i++){
		xif*fi=(xif*)dynp_get(&o->ifs,i);
		if(i>0){
			xif*prvfi=(xif*)dynp_get(&o->ifs,i-1);
			if(xexpr_is_encapsulated(&prvfi->super)){
				printf(" else ");
			}else{
				toc_indent_for_compile(tc);
				printf("else ");
			}
		}
		_xif_compile_((xexpr*)fi,tc);
	}
	xif*lastif=(xif*)dynp_get_last(&o->ifs);
	if(o->elsecode.exprs.count){
		if(xexpr_is_encapsulated(&lastif->code.super))
			printf(" else ");
		else{
			toc_indent_for_compile(tc);
			printf("else ");
		}
		_code_compile_((xexpr*)&o->elsecode,tc);
		if(xexpr_is_encapsulated(&o->elsecode.super)){
			printf("\n");
		}
	}else{
		if(xexpr_is_encapsulated(&lastif->code.super))
			printf("\n");
	}
}

#define xife_def (xife){{_xife_compile_,NULL,str_def,token_def,2},\
						dynp_def,code_def}

inline static xife*xife_read_next(toc*tc,token tk){
	xife*o=malloc(sizeof(xife));
	*o=xife_def;
	o->super.token=tk;
	while(1){
		xif*i=xif_read_next(tc,tk);
		dynp_add(&o->ifs,i);
		token t=toc_next_token(tc);
		if(!token_equals(&t,"else")){
			tc->srcp=t.begin;
			return o;
		}
		token t2=toc_next_token(tc);
		if(!token_equals(&t2,"if")){
			tc->srcp=t2.begin;
			code_read_next(&o->elsecode,tc);
			return o;
		}
	}
}
