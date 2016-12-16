#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
inline static /*gives*/ci_expr*_ci_expr_new_from_pp(const char**pp,ci_toc*tc);


typedef struct ci_expr_call{
	ci_expr super;
	str name;
	dynp/*owns ci_expression*/args;
}ci_expr_call;

inline static void _ci_expr_call_compile_(const ci_expr*o,ci_toc*tc){
	ci_expr_call*e=(ci_expr_call*)o;

	const char*p=strpbrk(e->name.data,".");
	if(p){
		str s=str_def;
		str_add_list(&s,e->name.data,p-e->name.data);
		str_add(&s,0);

		const ci_toc_ident*id=ci_toc_find_ident(tc,s.data);
		if(!id){
			printf("<file> <line:col> identifier not found: %s\n",
					s.data);
			exit(1);
		}
		printf("%s_%s((%s*)&%s",
				id->type.data,p+1,
				id->type.data,s.data);
		if(e->args.count){
			printf(",");
		}
	}else{
		printf("%s",e->name.data);
		printf("(");
	}
	dynp_foac(&e->args,{
		ci_expr*a=(ci_expr*)o;
		a->compile(a,tc);
		if(i!=e->args.count-1){
			printf(",");
		}
	});
	printf(")");
}

#define ci_expr_call_def (ci_expr_call){\
	{str_def,_ci_expr_call_compile_,ci_expr_call_free},\
	str_def,dynp_def}

inline static void ci_expr_call_free(ci_expr*o){
	ci_expr_call*oo=(ci_expr_call*)o;
	ci_expr_free(&oo->super);
	dynp_foa(&oo->args,{
			ci_expr*e=(ci_expr*)o;
			if(e->free)
				e->free(e);
			else
				ci_expr_free(e);
	});
	free(o);
}

inline static /*gives*/ci_expr_call*ci_expr_call_next(
		const char**pp,ci_toc*tc,/*takes*/str name){
	ci_expr_call*o=malloc(sizeof(ci_expr_call));
	*o=ci_expr_call_def;
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
		ci_expr*a=_ci_expr_new_from_pp(pp,tc);
		if(ci_expr_is_empty(a)){
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
