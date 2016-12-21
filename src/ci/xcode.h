#pragma once
#include"xexp.h"
#include"decouple.h"
//inline static /*gives*/xexp*ci_read_next_statement(toc*o);

typedef struct xcode{
	xexp super;
	dynp exps;
}xcode;

#define xcode_def (xcode){\
	{_xcode_compile_,_xcode_free_,cstr_def,token_def,1},\
	dynp_def\
}

inline static void _xcode_free_(xexp*oo){
	xcode*o=(xcode*)oo;
	for(unsigned i=0;i<o->exps.count;i++){
		xexp*e=(xexp*)dynp_get(&o->exps,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	dynp_free(&o->exps);
}

inline static void _xcode_compile_(const xexp*oo,toc*tc){
	xcode*o=(xcode*)oo;
	toc_push_scope(tc,'b',"");
//	if(o->super.bits&1){
//		printf("{");
//	}
//	if(o->exps.count)
//		printf("\n");
	printf("{\n");
	for(unsigned i=0;i<o->exps.count;i++){
		xexp*e=dynp_get(&o->exps,i);
		toc_print_indent_for_compile(tc);
		e->compile(e,tc);
		if(xexpr_is_block(e))
			continue;
		if(!xexpr_is_encapsulated(e))
			printf(";\n");
		else
			printf("\n");
	}
	toc_print_indent_for_compile(tc);
	ci_xcode_compile_free_current_scope(tc);
	toc_pop_scope(tc);
//	if(xexpr_is_encapsulated(&o->super)){
//		toc_print_indent_for_compile(tc);
//		printf("}");
//	}
//	if(!o->exps.count)// typedef struct empty {}
//		printf("\n");
	toc_print_indent_for_compile(tc);
	printf("}\n");
}

inline static void xcode_read_next(xcode*o,toc*tc){
	token_skip_empty_space(&tc->srcp);
	toc_push_scope(tc,'b',"");
	if(*tc->srcp=='{'){
		tc->srcp++;
		o->super.bits|=1;
		while(1){
			xexp*e=ci_read_next_statement(tc);
			if(!e)
				break;
			dynp_add(&o->exps,e);
			if(*tc->srcp==';'){
				tc->srcp++;
				continue;
			}
		}
		if(*tc->srcp!='}'){
			printf("<file> <line:col> expected '}' to end block\n");
			longjmp(_jmp_buf,1);
		}
		tc->srcp++;
		toc_pop_scope(tc);
		return;
	}
	o->super.bits&=~1;
	xexp*e=ci_read_next_statement(tc);
	dynp_add(&o->exps,e);
	if(*tc->srcp==';'){
		tc->srcp++;
	}
	toc_pop_scope(tc);
}
