#pragma once
#include"../lib.h"
#include"toc.h"

inline static /*gives*/xexpr*toc_next_expr_from_pp(const char**pp,toc*o);

typedef struct codeblk{
	xexpr super;
	int is_encaps;
	dynp/*own expr*/exprs;
}codeblk;

inline static void _codeblk_compile_(const xexpr*oo,toc*tc){
	codeblk*o=(codeblk*)oo;
	toc_push_scope(tc,'b',"");
	if(o->is_encaps){
		printf("{\n");
	}else{
		printf("\n");
	}
	for(unsigned i=0;i<o->exprs.count;i++){
		xexpr*e=dynp_get(&o->exprs,i);
		toc_indent_for_compile(tc);
		e->compile(e,tc);
		printf(";\n");//? e->is_block
	}
	toc_pop_scope(tc);
	if(o->is_encaps){
		toc_indent_for_compile(tc);
		printf("}");
	}
}

#define codeblk_def (codeblk){{str_def,_codeblk_compile_,NULL},0,dynp_def}

inline static void codeblk_read_next(codeblk*o,const char**pp,toc*tc){
	token_skip_empty_space(pp);
	toc_push_scope(tc,'b',"");
	if(**pp=='{'){
		(*pp)++;
		o->is_encaps=1;
	}
	while(1){
		xexpr*e=toc_next_expr_from_pp(pp,tc);
		if(xexpr_is_empty(e)){
			break;
		}
		dynp_add(&o->exprs,e);
		if(**pp==';'){
			(*pp)++;
			continue;
		}
	}
	if(o->is_encaps){
		if(**pp!='}'){
			printf("<file> <line:col> expected '}' to end block\n");
			exit(1);
		}
		(*pp)++;
	}
	toc_pop_scope(tc);
}
