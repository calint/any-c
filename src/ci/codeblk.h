#pragma once
#include"../lib.h"
#include"toc.h"

inline static /*gives*/xexpr*toc_read_next_xexpr(const char**pp,toc*o);

typedef struct codeblk{
	xexpr super;
	dynp/*own expr*/exprs;
}codeblk;

#define codeblk_def (codeblk){{_codeblk_compile_,NULL,str_def,1},dynp_def}

inline static void _codeblk_compile_(const xexpr*oo,toc*tc){
	codeblk*o=(codeblk*)oo;
	toc_push_scope(tc,'b',"");
	if(o->super.bits&1){
		printf("{\n");
	}else{
		printf("\n");
	}
	for(unsigned i=0;i<o->exprs.count;i++){
		xexpr*e=dynp_get(&o->exprs,i);
		toc_indent_for_compile(tc);
		e->compile(e,tc);
		if(!(e->bits&1))
			printf(";\n");
		else
			printf("\n");
	}
	toc_pop_scope(tc);
	if(o->super.bits&1){
		toc_indent_for_compile(tc);
		printf("}");
	}
}

inline static void codeblk_read_next(codeblk*o,const char**pp,toc*tc){
	token_skip_empty_space(pp);
	toc_push_scope(tc,'b',"");
	if(**pp=='{'){
		(*pp)++;
		o->super.bits|=1;
		while(1){
			xexpr*e=toc_read_next_xexpr(pp,tc);
			if(xexpr_is_empty(e))
				break;
			dynp_add(&o->exprs,e);
			if(**pp==';'){
				(*pp)++;
				continue;
			}
		}
		if(**pp!='}'){
			printf("<file> <line:col> expected '}' to end block\n");
			exit(1);
		}
		(*pp)++;
		toc_pop_scope(tc);
		return;
	}
	o->super.bits&=~1;
	xexpr*e=toc_read_next_xexpr(pp,tc);
	dynp_add(&o->exprs,e);
	if(**pp==';'){
		(*pp)++;
	}
	toc_pop_scope(tc);
}
