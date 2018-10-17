#pragma once
#include"xexp.h"
#include"decouple.h"
//inline static /*gives*/xexp*ci_read_next_statement(toc*o);

typedef struct xcode{
	xexp super;
	ptrs exps;
}xcode;

#define xcode_def (xcode){\
	{_xcode_compile_,_xcode_free_,NULL,strc_def,token_def,1,false},\
	ptrs_def\
}

inline static void _xcode_free_(xexp*oo){
	xcode*o=(xcode*)oo;
	for(unsigned i=0;i<o->exps.count;i++){
		xexp*e=(xexp*)ptrs_get(&o->exps,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	ptrs_free(&o->exps);
}

inline static void _xcode_compile_(const xexp*oo,toc*tc){
	xcode*o=(xcode*)oo;
	toc_push_scope(tc,'b',"");
	printf("{\n");
	for(unsigned i=0;i<o->exps.count;i++){
		xexp*e=ptrs_get(&o->exps,i);
		toc_print_indent_for_compile(tc);
		e->compile(e,tc);
		if(xexp_is_block(e))
			continue;
		if(!xexp_is_encapsulated(e))
			printf(";\n");
		else
			printf("\n");
	}
	ci_xcode_compile_free_current_scope(tc);
	toc_pop_scope(tc);
	toc_print_indent_for_compile(tc);
	printf("}\n");
}

inline static void xcode_read_next(xcode*o,toc*tc){
	token_skip_empty_space(&tc->srcp);
	toc_push_scope(tc,'b',"");
//	if(*tc->srcp=='{'){
//		tc->srcp++;
	if(toc_srcp_is_take(tc,'{')){
		xexp_set_is_encapsulated(&o->super,true);
		while(1){
			xexp*e=ci_read_next_statement(tc);
			if(!e)
				break;
			ptrs_add(&o->exps,e);
//			if(*tc->srcp==';'){
//				tc->srcp++;
//				continue;
//			}
			if(toc_srcp_is_take(tc,';'))
				continue;
		}
		if(*tc->srcp!='}'){
//			toc_print_source_location(tc,tk,4);//! fix
			printf("expected '}' to end block");
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
//		tc->srcp++;
		toc_srcp_inc(tc);
		toc_pop_scope(tc);
		return;
	}
	xexp_set_is_encapsulated(&o->super,false);
	xexp*e=ci_read_next_statement(tc);
	ptrs_add(&o->exps,e);
	if(*tc->srcp==';'){
		tc->srcp++;
	}
	toc_pop_scope(tc);
}
