#pragma once
#include"toc.h"
#include"decouple.h"
#include"../lib.h"
//inline static xexp*ci_read_next_expression(toc*);

typedef struct xexpls{
	xexp super;
	ptrs exps;
	strb ops;
}xexpls;

#define xexpls_def (xexpls){\
	{_xexpls_compile_,_xexpls_free_,strc_def,token_def,0,false},\
		ptrs_def,strb_def}

inline static void _xexpls_free_(xexp*oo){
	xexpls*o=(xexpls*)oo;
	for(unsigned i=0;i<o->exps.count;i++){
		xexp*e=(xexp*)ptrs_get(&o->exps,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	ptrs_free(&o->exps);
	strb_free(&o->ops);
}

inline static void _xexpls_compile_(const xexp*oo,toc*tc){
	const xexpls*o=(xexpls*)oo;
	for(unsigned i=0;i<o->exps.count;i++){
		const xexp*e=ptrs_get(&o->exps,i);
		const char op=strb_get(&o->ops,i);
		if(i!=0){
			printf("%c",op);
		}
		if(e->bits&4)
			printf("(");
		e->compile(e,tc);
		if(e->bits&4)
			printf(")");
	}
}

inline static void xexpls_parse_next(xexpls*,toc*,token);
inline static xexpls*xexpls_read_next(toc*tc,token tk){
	xexpls*o=malloc(sizeof(xexpls));
	*o=xexpls_def;
	xexpls_parse_next(o,tc,tk);
	return o;
}

inline static void xexpls_parse_next(xexpls*o,toc*tc,token tk){
	o->super.token=tk;
	if(toc_srcp_is_take(tc,'(')){
		o->super.bits|=4;
	}
	while(1){
		if(!o->exps.count)
			strb_add(&o->ops,'\0');
		xexp*e;
		if(toc_srcp_is(tc,'(')){
			e=(xexp*)xexpls_read_next(tc,tk);
		}else{
			e=ci_read_next_expression(tc);
		}
		if(!e)
			break;
//		if(xexpr_is_empty(e))
//			break;
		ptrs_add(&o->exps,e);
		if(toc_srcp_is_take(tc,'+')){strb_add(&o->ops,'+');}
		else if(toc_srcp_is_take(tc,'-')){strb_add(&o->ops,'-');}
		else if(toc_srcp_is_take(tc,'*')){strb_add(&o->ops,'*');}
		else if(toc_srcp_is_take(tc,'/')){strb_add(&o->ops,'/');}
		else if(toc_srcp_is_take(tc,'%')){strb_add(&o->ops,'%');}
		else if(toc_srcp_is_take(tc,'|')){strb_add(&o->ops,'|');}
		else if(toc_srcp_is_take(tc,'&')){strb_add(&o->ops,'&');}
		else if(toc_srcp_is_take(tc,'^')){strb_add(&o->ops,'^');}
		else{
			break;
		}
	}
	if(o->super.bits&4){
		if(!toc_srcp_is_take(tc,')')){
			toc_print_source_location(tc,o->super.token,4);
			printf("expected ')'");
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
	}
	xexp*first=ptrs_get(&o->exps,0);//? morechecks
	o->super.type=first->type;
	o->super.is_ref=first->is_ref;
}


