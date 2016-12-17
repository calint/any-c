#pragma once
#include"../lib.h"
#include"toc.h"

inline static /*gives*/xexpr*toc_read_next_xexpr(toc*o);

typedef struct code{
	xexpr super;
	dynp exprs;
}code;

#define code_def (code){{_code_compile_,NULL,str_def,token_def,1},dynp_def}

inline static void _code_compile_(const xexpr*oo,toc*tc){
	code*o=(code*)oo;
	toc_push_scope(tc,'b',"");
	if(o->super.bits&1){
		printf("{");
	}
	if(o->exprs.count)
		printf("\n");

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
	if(!o->exprs.count)
		printf("\n");
}

inline static void codeblk_read_next(code*o,toc*tc){
	token_skip_empty_space(&tc->srcp);
	toc_push_scope(tc,'b',"");
	if(*tc->srcp=='{'){
		tc->srcp++;
		o->super.bits|=1;
		while(1){
			xexpr*e=toc_read_next_xexpr(tc);
			if(xexpr_is_empty(e))
				break;
			dynp_add(&o->exprs,e);
			if(*tc->srcp==';'){
				tc->srcp++;
				continue;
			}
		}
		if(*tc->srcp!='}'){
			printf("<file> <line:col> expected '}' to end block\n");
			longjmp(_jmpbufenv,1);
		}
		tc->srcp++;
		toc_pop_scope(tc);
		return;
	}
	o->super.bits&=~1;
	xexpr*e=toc_read_next_xexpr(tc);
	dynp_add(&o->exprs,e);
	if(*tc->srcp==';'){
		tc->srcp++;
	}
	toc_pop_scope(tc);
}
