#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"

typedef struct ci_block{
	ci_expr super;
	int is_encaps;
	dynp/*own expr*/exprs;
	token token_pre_block;
}ci_block;

inline static void _ci_block_free_(ci_expr*oo){
	ci_block*o=(ci_block*)oo;
	dynp_foa(&o->exprs,{
		ci_expr*oo=(ci_expr*)o;
		if(oo->free)
			oo->free(oo);
		else
			ci_expr_free(oo);
	});
	dynp_free(&o->exprs);
}

inline static void toc_indent_for_source(ci_toc*tc){
	for(unsigned i=2;i<tc->scopes.count;i++){
		printf("    ");
	}
}

inline static void _ci_block_compile_(const ci_expr*oo,ci_toc*tc){
	ci_block*o=(ci_block*)oo;
	ci_toc_push_scope(tc,'b',"");
	if(o->is_encaps){
		printf("{\n");
	}
	for(unsigned i=0;i<o->exprs.count;i++){
		ci_expr*e=dynp_get(&o->exprs,i);
		toc_indent_for_source(tc);
		e->compile(e,tc);
		printf(";\n");//? e->is_block
	}
	ci_toc_pop_scope(tc);
	if(o->is_encaps){
		toc_indent_for_source(tc);
		printf("}");
	}
}

#define ci_block_def (ci_block){{str_def,_ci_block_compile_,_ci_block_free_},\
	0,dynp_def,token_def}

//inline static void ci_expr_ident_free(ci_expr_ident*o){
//	ci_expression_free(&o->super);
//	free(o);
//}

inline static /*gives*/ci_expr*ci_expr_new_from_pp(const char**pp,ci_toc*tc);

inline static /*gives*/ci_block*ci_block_parse(ci_block*o,const char**pp,ci_toc*tc){
//	ci_code*c=malloc(sizeof(ci_code));
//	*o=ci_code_def;
	token_skip_empty_space(pp);
//	o->token_pre_block=token_next(pp);//? exclude { from delimiters
	ci_toc_push_scope(tc,'b',"");
	if(**pp=='{'){
		(*pp)++;
		o->is_encaps=1;
	}
	while(1){
		ci_expr*e=ci_expr_new_from_pp(pp,tc);
		if(ci_expr_is_empty(e)){
			break;
		}
		dynp_add(&o->exprs,e);
		if(**pp==';'){
			(*pp)++;
			continue;
		}
//		printf("<file> <line:col> expected ';'\n");
//		exit(1);
	}
	if(o->is_encaps){
		if(**pp!='}'){
			printf("<file> <line:col> expected '}' to end block\n");
			exit(1);
		}
		(*pp)++;
	}
	ci_toc_pop_scope(tc);
	return o;
}
