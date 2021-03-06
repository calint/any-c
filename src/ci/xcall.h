#pragma once
#include"xexpls.h"
#include"decouple.h"
#include"../lib.h"

typedef struct xcall{
	xexp super;
	strc name;
	ptrs args;
}xcall;

#define xcall_def (xcall){\
	{_xcall_compile_,_xcall_free_,NULL,strc_def,token_def,0,false},\
	strc_def,ptrs_def\
}

inline static void _xcall_free_(xexp*oo){
	xcall*o=(xcall*)oo;
	for(unsigned i=0;i<o->args.count;i++){
		xexp*e=(xexp*)ptrs_get(&o->args,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	ptrs_free(&o->args);
}

inline static void _xcall_compile_(const xexp*oo,toc*tc){
	const xcall*o=(xcall*)oo;
	ci_xcall_compile(tc,o);
	for(unsigned i=0;i<o->args.count;i++){
		const xexp*e=(xexp*)ptrs_get(&o->args,i);
		const bool func_arg_is_ref=ci_is_func_param_ref(tc,e->token,o->name,i);
		if(func_arg_is_ref && !e->is_ref)
			printf("&");
		else if(!func_arg_is_ref && e->is_ref)
			printf("*");
		e->compile(e,tc);
		if(i!=o->args.count-1)
			printf(",");
	}
	printf(")");
}

inline static xcall*xcall_read_next(toc*tc,token tk,strc name){
	xcall*o=malloc(sizeof(xcall));
	*o=xcall_def;
	o->name=name;
//	if(!strcmp(o->name,"malloc")){
//		o->super.is_ref=true;
//	}
//
	o->super.token=tk;
	if(!toc_srcp_is_take(tc,'(')){// arguments
		toc_print_source_location(tc,o->super.token,4);
		printf("expected '(' followed by arguments and ')'");
		printf("\n    %s %d\n",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
//	toc_srcp_inc(tc);
	while(1){ // arguments passed to function
		if(toc_srcp_is_take(tc,')'))
			break;
		xexp*a=(xexp*)xexpls_read_next(tc,tk,false,0,NULL);
		ptrs_add(&o->args,a);

		if(toc_srcp_is_take(tc,','))
			continue;
		if(toc_srcp_is_take(tc,')'))
			break;

		toc_print_source_location(tc,o->super.token,4);
		printf("expected ',' and more arguments or ')' for function '%s'",
				name);
		printf("\n    %s %d\n",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	ci_xcall_assert(tc,o->super.token,o);
	return o;
}
