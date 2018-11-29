#pragma once
#include"xexpls.h"

typedef struct xbool{
	xexp super;

	// leaf
	xexpls lh;
	char op;
	xexpls rh;

	// list
	strb bool_op_list;
	ptrs bool_list;
}xbool;

#define xbool_def (xbool){\
	{_xbool_compile_,_xbool_free_,NULL,strc_def,token_def,0,false},\
	xexpls_def,0,xexpls_def,\
	strb_def,\
	ptrs_def,\
}


inline static void _xbool_free_(xexp*oo){
	xbool*o=(xbool*)oo;
	for(unsigned i=0;i<o->bool_list.count;i++){
		xbool*b=(xbool*)ptrs_get(&o->bool_list,i);
		b->super.free((xexp*)b);
		free(b);
	}
	o->lh.super.free((xexp*)&o->lh);
	o->lh.super.free((xexp*)&o->rh);

	ptrs_free(&o->bool_list);
	strb_free(&o->bool_op_list);
}

inline static void _xbool_compile_(const xexp*oo,toc*tc){
	xbool*o=(xbool*)oo;
	if(o->bool_list.count){
		printf("(");
		const long n=o->bool_list.count;
		for(long i=0;i<n;i++){
			xbool*b=(xbool*)ptrs_get(&o->bool_list,i);
			_xbool_compile_((xexp*)b,tc);

			if(i==n-1) break;
			const char op=strb_get(&o->bool_op_list,i);
			if(op=='&')
				printf(" && ");
			else if(op=='|')
				printf(" || ");
		}
		printf(")");
		return;
	}

	o->lh.super.compile((xexp*)&o->lh,tc);
	if(o->op=='='){
		printf("==");
	}else if(o->op=='!'){
		printf("!=");
	}else if(o->op=='>'){
		printf(">");
	}else if(o->op=='g'){
		printf(">=");
	}else if(o->op=='<'){
		printf("<");
	}else if(o->op=='l'){
		printf("<=");
	}else{
		printf("<file> <line:col> unknown op '%d' ')'\n",o->op);
		longjmp(_jmp_buf,1);
	}
	o->rh.super.compile((xexp*)&o->rh,tc);

}

inline static void xbool_parse(xbool*o,toc*tc,token tk,bool is_sub_expr){
	o->super.type="bool";
	o->super.token=tk;
	if(toc_srcp_is_take(tc,'(')){
		while(1){
			xbool*e=malloc(sizeof(xbool));
			*e=xbool_def;
			xbool_parse(e,tc,tk,true);
			ptrs_add(&o->bool_list,e);
			if(toc_srcp_is_take(tc,')'))
				return;
			token tkn=toc_next_token(tc);
			if(token_equals(&tkn,"and")){
				strb_add(&o->bool_op_list,'&');
			}else if(token_equals(&tkn,"or")){
				strb_add(&o->bool_op_list,'|');
			}else{
				toc_print_source_location(tc,tkn,4);
				printf("expected 'and' or 'or'\n");
				longjmp(_jmp_buf,1);
			}
		}
	}

	xexpls_parse_next(&o->lh,tc,tk,false);
	if(toc_srcp_is_take(tc,'=')){
		o->op='=';
	}else if(toc_srcp_is_take(tc,'>')){
		if(toc_srcp_is_take(tc,'='))
			 o->op='g';
		else
			 o->op='>';
	}else if(toc_srcp_is_take(tc,'<')){
		if(toc_srcp_is_take(tc,'='))
			 o->op='l';
		else
			 o->op='<';
	}else if(toc_srcp_is_take(tc,'!')){
		if(toc_srcp_is_take(tc,'='))
			 o->op='!';
		else{
			toc_print_source_location(tc,tk,4);
			printf("expected '!='\n");
			longjmp(_jmp_buf,1);
		}
	}else{
		toc_print_source_location(tc,tk,4);
		printf("expected = > < >= <= !=\n");
		longjmp(_jmp_buf,1);
	}
	xexpls_parse_next(&o->rh,tc,tk,false);
}
