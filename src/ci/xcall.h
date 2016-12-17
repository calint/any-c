#pragma once
#include"../lib.h"
#include "toc.h"

inline static /*gives*/xexpr*toc_read_next_xexpr(toc*tc);

typedef struct xcall{
	xexpr super;
	str name;
	dynp args;
}xcall;

inline static void _xcall_compile_(const xexpr*o,toc*tc){
	xcall*e=(xcall*)o;
	toc_compile_for_call(tc,e->name.data,e->args.count);
	for(unsigned i=0;i<e->args.count;i++){
		xexpr*a=(xexpr*)dynp_get(&e->args,i);
		a->compile(a,tc);
		if(i!=e->args.count-1){
			printf(",");
		}
	}
	printf(")");
}

#define xcall_def (xcall){{_xcall_compile_,NULL,str_def,0},str_def,dynp_def}

inline static xcall*xcall_read_next(toc*tc,str name){
	xcall*o=malloc(sizeof(xcall));
	*o=xcall_def;
	o->name=name;
	if(*tc->srcp!='('){// arguments
		printf("<file> <line> <col> expected '(' followed by arguments and ')'");
		longjmp(_jmpbufenv,1);
	}
	tc->srcp++;
	while(1){
		if(*tc->srcp==')'){
			tc->srcp++;
			break;
		}
		xexpr*a=toc_read_next_xexpr(tc);
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
		printf("<file> <line> <col> expected ',' followed by more arguments");
		longjmp(_jmpbufenv,1);
	}
	return o;
}
