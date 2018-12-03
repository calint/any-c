#pragma once
#include"xexpls.h"

typedef struct xbool{
	xexp super;

	// leaf
	xexpls lh;
	char op;
	xexpls rh;

	// list
	ptrs lsbools;
	strb lsops;

	// 1: not
	char bits;
}xbool;

#define xbool_def (xbool){\
	{_xbool_compile_,_xbool_free_,NULL,strc_def,token_def,0,false},\
	xexpls_def,0,xexpls_def,ptrs_def,strb_def,0\
}

inline static bool xbool_is_not(const xexp*oo){
	xbool*o=(xbool*)oo;
	return (o->bits&1)==1;
}

inline static void xbool_set_is_not(xexp*oo,bool b){
	xbool*o=(xbool*)oo;
	if(b) o->bits|=1;
	else o->bits&=~1;
}

inline static void _xbool_free_(xexp*oo){
	xbool*o=(xbool*)oo;
	for(unsigned i=0;i<o->lsbools.count;i++){
		xbool*b=(xbool*)ptrs_get(&o->lsbools,i);
		b->super.free((xexp*)b);
		free(b);
	}
	o->lh.super.free((xexp*)&o->lh);
	o->lh.super.free((xexp*)&o->rh);

	ptrs_free(&o->lsbools);
	strb_free(&o->lsops);
}

inline static void _xbool_compile_(const xexp*oo,toc*tc){
	xbool*o=(xbool*)oo;
	if(o->lsbools.count){
		if(xbool_is_not(oo))
			printf("!");
		printf("(");
		const long n=o->lsbools.count;
		for(long i=0;;i++){
			xbool*b=(xbool*)ptrs_get(&o->lsbools,i);
			_xbool_compile_((xexp*)b,tc);

			if(i==n-1)
				break;

			const char op=strb_get(&o->lsops,i);
			if(op=='&')
				printf(" && ");
			else if(op=='|')
				printf(" || ");
		}
		printf(")");
		return;
	}

	if(xbool_is_not(oo))
		printf("!(");
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
		printf("unknown op '%d' ')'\n",o->op);
		printf("\n    %s %d\n",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	o->rh.super.compile((xexp*)&o->rh,tc);
	if(xbool_is_not(oo))
		printf(")");
}


inline static void xbool_parse_next(xbool*o,toc*tc,token tk);
inline static xbool*xbool_read_next(toc*tc,token tk){
	xbool*o=malloc(sizeof(xbool));
	*o=xbool_def;
	xbool_parse_next(o,tc,tk);
	return o;
}

inline static void xbool_parse_next(xbool*o,toc*tc,token tk){
	o->super.type="bool";
	o->super.token=tk;

	token t=toc_next_token(tc);
	if(token_equals(&t,"not")){
		xbool_set_is_not((xexp*)o,true);
	}else{
		toc_push_back_token(tc,t);
	}

	if(toc_srcp_is_take(tc,'(')){
		while(1){
			xbool*e=malloc(sizeof(xbool));
			*e=xbool_def;
			xbool_parse_next(e,tc,tk);
			ptrs_add(&o->lsbools,e);
			if(toc_srcp_is_take(tc,')'))
				return;
			token tkn=toc_next_token(tc);
			if(token_equals(&tkn,"and")){
				strb_add(&o->lsops,'&');
			}else if(token_equals(&tkn,"or")){
				strb_add(&o->lsops,'|');
			}else{
				toc_print_source_location(tc,tkn,4);
				printf("expected 'and' or 'or'");
				printf("\n    %s %d\n",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
		}
	}

	int ret=0;
	xexpls_parse_next(&o->lh,tc,tk,false,&ret,0);
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
			printf("expected '!='");
			printf("\n    %s %d\n",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
	}else{
		toc_print_source_location2(tc,tc->srcp,4);
		printf("expected comparison = > < >= <= !=");
		printf("\n    %s %d\n",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	ret=0;
	xexpls_parse_next(&o->rh,tc,tk,false,&ret,0);
}
