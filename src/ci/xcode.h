#pragma once
#include"xexp.h"
#include"toc.h"

inline static /*gives*/xexp*ci_read_next_statement(toc*o);

typedef struct xcode{
	xexp super;
	dynp exprs;
}xcode;

#define xcode_def (xcode){{_code_compile_,NULL,str_def,token_def,1},dynp_def}

inline static void _code_compile_(const xexp*oo,toc*tc){
	xcode*o=(xcode*)oo;
	toc_push_scope(tc,'b',"");
	if(o->super.bits&1){
		printf("{");
	}
	if(o->exprs.count)
		printf("\n");

	for(unsigned i=0;i<o->exprs.count;i++){
		xexp*e=dynp_get(&o->exprs,i);
		toc_indent_for_compile(tc);
		e->compile(e,tc);
		if(xexpr_is_block(e))
			continue;
		if(!xexpr_is_encapsulated(e))
			printf(";\n");
		else
			printf("\n");
	}
	toc_pop_scope(tc);
	if(xexpr_is_encapsulated(&o->super)){
		toc_indent_for_compile(tc);
		printf("}");
	}
	if(!o->exprs.count)// typedef struct empty {}
		printf("\n");

}

inline static void code_read_next(xcode*o,toc*tc){
	token_skip_empty_space(&tc->srcp);
	toc_push_scope(tc,'b',"");
	if(*tc->srcp=='{'){
		tc->srcp++;
		o->super.bits|=1;
		while(1){
			xexp*e=ci_read_next_statement(tc);
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
	xexp*e=ci_read_next_statement(tc);
	dynp_add(&o->exprs,e);
	if(*tc->srcp==';'){
		tc->srcp++;
	}
	toc_pop_scope(tc);
}
