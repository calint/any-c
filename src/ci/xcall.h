#pragma once
#include"../lib.h"
#include "toc.h"
#include"xexpls.h"

//inline static xexpls*toc_read_next_xexpls(toc*,token);
inline static void ci_xcall_compile(toc*,token,ccharp,unsigned);

typedef struct xcall{
	xexp super;
	str name;
	dynp args;
}xcall;

inline static void _xcall_compile_(const xexp*oo,toc*tc){
	xcall*o=(xcall*)oo;
	ci_xcall_compile(tc,o->super.token,o->name.data,o->args.count);
	for(unsigned i=0;i<o->args.count;i++){
		xexp*a=(xexp*)dynp_get(&o->args,i);
		a->compile(a,tc);
		if(i!=o->args.count-1){
			printf(",");
		}
	}
	printf(")");
}

#define xcall_def (xcall){{_xcall_compile_,NULL,str_def,token_def,0},\
	str_def,dynp_def}

inline static xcall*xcall_read_next(toc*tc,token tk,str name){
	xcall*o=malloc(sizeof(xcall));
	*o=xcall_def;
	o->name=name;
	o->super.token=tk;
	if(*tc->srcp!='('){// arguments
		toc_print_source_location(tc,o->super.token,4);
		printf("expected '(' followed by arguments and ')'");
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmpbufenv,1);
	}
	tc->srcp++;
	while(1){
		if(*tc->srcp==')'){
			tc->srcp++;
			break;
		}
		xexp*a=(xexp*)xexpls_read_next(tc,tk);
		if(xexpr_is_empty(a)){
			printf("<file> <line> <col> expected ')' or more arguments");
			longjmp(_jmpbufenv,1);
		}
		dynp_add(&o->args,a);
		if(*tc->srcp==','){
			tc->srcp++;
			continue;
		}
		if(*tc->srcp==')'){
			tc->srcp++;
			break;
		}
		toc_print_source_location(tc,o->super.token,4);
		printf("expected ',' followed by more arguments to function '%s'",
				name.data);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmpbufenv,1);
	}
	return o;
}
