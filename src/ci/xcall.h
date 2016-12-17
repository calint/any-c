#pragma once
#include"../lib.h"
#include "toc.h"

inline static /*gives*/xexpr*toc_read_next_xexpr(const char**pp,toc*tc);

typedef struct xcall{
	xexpr super;
	str name;
	dynp/*owns ci_expression*/args;
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

#define xcall_def (xcall){{str_def,_xcall_compile_,NULL},str_def,dynp_def}

inline static xcall*xcall_read_next(
		const char**pp,toc*tc,str name){

	xcall*o=malloc(sizeof(xcall));
	*o=xcall_def;
	o->name=name;
	if(**pp!='('){// arguments
		printf("<file> <line> <col> expected '(' followed by arguments and ')'");
		exit(1);
	}
	(*pp)++;
	while(1){
		if(**pp==')'){
			(*pp)++;
			break;
		}
		xexpr*a=toc_read_next_xexpr(pp,tc);
		if(xexpr_is_empty(a)){
			printf("<file> <line> <col> expected ')' or more arguments");
			exit(1);
		}
		dynp_add(&o->args,a);
		if(**pp==','){
			(*pp)++;
			continue;
		}
		if(**pp==')'){
			(*pp)++;
			break;
		}
		printf("<file> <line> <col> expected ',' followed by more arguments");
		exit(1);
	}
	return o;
}
