#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"
#include"block.h"
#include "expr_bool.h"
#include"expr_if.h"
typedef struct ci_expr_ife{
	ci_expr super;
	dynp/*own&ci_expr_if*/ifs;
	ci_block elsecode;
}ci_expr_ife;

inline static void _ci_expr_ife_compile_(const ci_expr*oo,ci_toc*tc){
	ci_expr_ife*o=(ci_expr_ife*)oo;

	for(unsigned i=0;i<o->ifs.count;i++){
		ci_expr_if*iff=(ci_expr_if*)dynp_get(&o->ifs,i);
		if(i){
			printf(" else ");
		}
		_ci_expr_if_compile_((ci_expr*)iff,tc);
	}
	if(o->elsecode.exprs.count){
		printf(" else ");
		_ci_block_compile_((ci_expr*)&o->elsecode,tc);
	}
}

inline static void _ci_expr_ife_free_(ci_expr*oo){
	ci_expr_free(oo);
}

#define ci_expr_ife_def (ci_expr_ife){\
	{str_def,_ci_expr_ife_compile_,_ci_expr_ife_free_},\
	dynp_def,ci_block_def}

inline static ci_expr_ife*ci_expr_ife_next(const char**pp,ci_toc*tc){
	ci_expr_ife*o=malloc(sizeof(ci_expr_ife));
	*o=ci_expr_ife_def;

	while(1){
		ci_expr_if*i=malloc(sizeof(ci_expr_if));
		*i=ci_expr_if_def;
		dynp_add(&o->ifs,i);

		ci_expr_bool_parse(&i->cond,pp,tc);
		ci_block_parse(&i->code,pp,tc);


		token t=token_next(pp);
		if(!token_equals(&t,"else")){
			(*pp)=t.begin;
			return o;
		}

		token t2=token_next(pp);
		if(!token_equals(&t2,"if")){
			ci_block_parse(&o->elsecode,pp,tc);
			return o;
		}
	}
}
