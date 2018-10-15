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
		if(xexp_is_negated(e))
			printf("-");
		if(xexp_is_parenthesis(e))
			printf("(");
		e->compile(e,tc);
		if(xexp_is_parenthesis(e))
			printf(")");
	}
}

inline static void xexpls_parse_next(xexpls*,toc*,token,bool issubexpr);
inline static xexpls*xexpls_read_next(toc*tc,token tk,bool issubexpr){
	xexpls*o=malloc(sizeof(xexpls));
	*o=xexpls_def;
	xexp_set_is_parenthesis(&o->super,issubexpr);
	xexpls_parse_next(o,tc,tk,issubexpr);
	return o;
}

inline static void xexpls_parse_next(xexpls*o,toc*tc,token tk,bool issubexpr){
	o->super.token=tk;
	while(1){
		if(!o->exps.count)
			strb_add(&o->ops,'\0');
		xexp*e;

		//? ws_pre_negation
		bool is_negated=toc_srcp_is_take(tc,'-');


		if(toc_srcp_is_take(tc,'(')){
			xexpls*els=xexpls_read_next(tc,tk,true);
			//? xexpls_deduce_typeref(&o->exps)
			xexp*first=ptrs_get(&els->exps,0);
			els->super.type=first->type;
			els->super.is_ref=first->is_ref;
			e=(xexp*)els;
		}else{
			e=ci_read_next_expression(tc);
		}
		xexp_set_is_negated(e,is_negated);
		if(!e)
			break;
		ptrs_add(&o->exps,e);
		if(issubexpr)
			if(toc_srcp_is_take(tc,')'))
				return;
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
	//? xexpls_deduce_typeref(&o->exps)
	xexp*first=ptrs_get(&o->exps,0);
	o->super.type=first->type;
	o->super.is_ref=first->is_ref;
}


