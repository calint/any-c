#pragma once
#include"../lib.h"
#include "codeblk.h"
#include "toc.h"
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
	_codeblk_compile_((xexpr*)&o->code,tc);
}

#define xif_def (xif){{_xif_compile_,NULL,str_def,0},xbool_def,codeblk_def}

inline static xif*xif_read_next(const char**pp,toc*tc){
	xif*o=malloc(sizeof(xif));
	*o=xif_def;
	xbool_parse(&o->cond,pp,tc);
	codeblk_read_next(&o->code,pp,tc);
	return o;
}

typedef struct xife{
	xexpr super;
	dynp ifs;
	codeblk elsecode;
}xife;

inline static void _xife_compile_(const xexpr*oo,toc*tc){
	xife*o=(xife*)oo;

	for(unsigned i=0;i<o->ifs.count;i++){
		xif*fi=(xif*)dynp_get(&o->ifs,i);
		if(i){
			printf(" else ");
		}
		_xif_compile_((xexpr*)fi,tc);
	}
	if(o->elsecode.exprs.count){
		printf(" else ");
		_codeblk_compile_((xexpr*)&o->elsecode,tc);
	}
}

#define xife_def (xife){{_xife_compile_,NULL,str_def,0},dynp_def,codeblk_def}

inline static xife*xife_read_next(const char*pp[],toc*tc){
	xife*o=malloc(sizeof(xife));
	*o=xife_def;

	while(1){
		xif*i=malloc(sizeof(xif));
		*i=xif_def;
		dynp_add(&o->ifs,i);

		xbool_parse(&i->cond,pp,tc);
		codeblk_read_next(&i->code,pp,tc);

		token t=token_next(pp);
		if(!token_equals(&t,"else")){
			(*pp)=t.begin;
			return o;
		}

		token t2=token_next(pp);
		if(!token_equals(&t2,"if")){
			codeblk_read_next(&o->elsecode,pp,tc);
			return o;
		}
	}
}
