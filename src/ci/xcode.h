#pragma once
#include"xexp.h"
#include"decouple.h"

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
//		if(xexp_is_block(e))
//			continue;
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
	if(toc_srcp_is_take(tc,'{')){// is block statement
		xexp_set_is_encapsulated(&o->super,true);
		while(1){
			xexp*e=ci_read_next_statement(tc);
			if(!e){
				if(toc_srcp_is_take(tc,'#')){
					toc_read_to_end_of_line(tc);
					continue;
				}
				break;
			}
			ptrs_add(&o->exps,e);
		}
		if(!toc_srcp_is_take(tc,'}')){
			toc_print_source_location2(tc,tc->srcp,4);
			printf("expected '}' to end block");
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
		toc_pop_scope(tc);
		return;
	}
	// is single statement
	xexp_set_is_encapsulated(&o->super,false);
	xexp*e=ci_read_next_statement(tc);
	ptrs_add(&o->exps,e);
	toc_pop_scope(tc);
}
