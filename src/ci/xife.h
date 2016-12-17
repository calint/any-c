#pragma once
#include"../lib.h"
#include "codeblock.h"
#include "expr.h"
#include "toc.h"
#include "xbool.h"
#include "xif.h"

typedef struct xife{
	xexpr super;
	dynp/*own&ci_expr_if*/ifs;
	codeblock elsecode;
}xife;

inline static void _xife_compile_(const xexpr*oo,toc*tc){
	xife*o=(xife*)oo;

	for(unsigned i=0;i<o->ifs.count;i++){
		xif*iff=(xif*)dynp_get(&o->ifs,i);
		if(i){
			printf(" else ");
		}
		_xif_compile_((xexpr*)iff,tc);
	}
	if(o->elsecode.exprs.count){
		printf(" else ");
		_codeblock_compile_((xexpr*)&o->elsecode,tc);
	}
}

#define xife_def (xife){{str_def,_xife_compile_,NULL},dynp_def,codeblock_def}

inline static xife*xife_read_next(const char**pp,toc*tc){
	xife*o=malloc(sizeof(xife));
	*o=xife_def;

	while(1){
		xif*i=malloc(sizeof(xif));
		*i=xif_def;
		dynp_add(&o->ifs,i);

		ci_expr_bool_parse(&i->cond,pp,tc);
		codeblock_read_next(&i->code,pp,tc);


		token t=token_next(pp);
		if(!token_equals(&t,"else")){
			(*pp)=t.begin;
			return o;
		}

		token t2=token_next(pp);
		if(!token_equals(&t2,"if")){
			codeblock_read_next(&o->elsecode,pp,tc);
			return o;
		}
	}
}
