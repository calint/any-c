#pragma once
#include"xexpls.h"
#include"decouple.h"
//inline static void ci_xcall_compile(toc*,token,cstr,unsigned);

typedef struct xcall{
	xexp super;
	cstr name;
	dynp args;
}xcall;

inline static void _xcall_free_(xexp*oo){
	xcall*o=(xcall*)oo;
	for(unsigned i=0;i<o->args.count;i++){
		xexp*e=(xexp*)dynp_get(&o->args,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	dynp_free(&o->args);
}

inline static void _xcall_compile_(const xexp*oo,toc*tc){
	xcall*o=(xcall*)oo;
	ci_xcall_compile(tc,o->super.token,o->name,o->args.count);
	for(unsigned i=0;i<o->args.count;i++){
		xexp*e=(xexp*)dynp_get(&o->args,i);
		e->compile(e,tc);
		if(i!=o->args.count-1){
			printf(",");
		}
	}
	printf(")");
}

#define xcall_def (xcall){\
	{_xcall_compile_,_xcall_free_,cstr_def,token_def,0},\
	cstr_def,dynp_def\
}

inline static xcall*xcall_read_next(toc*tc,token tk,cstr name){
	xcall*o=malloc(sizeof(xcall));
	*o=xcall_def;
	o->name=name;
	o->super.token=tk;
	if(!toc_srcp_is(tc,'(')){// arguments
		toc_print_source_location(tc,o->super.token,4);
		printf("expected '(' followed by arguments and ')'");
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	toc_srcp_inc(tc);
	while(1){
		if(toc_srcp_is_take(tc,')'))
			break;
		xexp*a=(xexp*)xexpls_read_next(tc,tk);
		if(xexpr_is_empty(a)){
			printf("<file> <line> <col> expected ')' or more arguments");
			longjmp(_jmp_buf,1);
		}
		dynp_add(&o->args,a);
		//? assert can set  ci_assert_xcall_arg_type(name.data,i)
		if(toc_srcp_is_take(tc,','))
			continue;
		if(toc_srcp_is_take(tc,')'))
			break;
		toc_print_source_location(tc,o->super.token,4);
		printf("expected ',' followed by more arguments to function '%s'",
				name);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	return o;
}
