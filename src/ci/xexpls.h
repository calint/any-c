#pragma once
#include"../lib.h"
#include "toc.h"

inline static xexpr*toc_read_next_expression(toc*);

typedef struct xexpls{
	xexpr super;
	dynp exprs;
	str ops;
}xexpls;

#define xexpls_def (xexpls){{_xexpls_compile_,NULL,str_def,token_def,0},\
							dynp_def,str_def}

inline static void _xexpls_compile_(const xexpr*oo,toc*tc){
	const xexpls*o=(xexpls*)oo;
	for(unsigned i=0;i<o->exprs.count;i++){
		const xexpr*e=dynp_get(&o->exprs,i);
		const char op=str_get(&o->ops,i);
		if(i!=0){
			printf("%c",op);
		}
		e->compile(e,tc);
	}
}

inline static xexpls*xexpls_read_next(toc*tc,token tk){
	xexpls*o=malloc(sizeof(xexpls));
	*o=xexpls_def;
//	o->super.type=type;//?
	o->super.token=tk;

	while(1){
		xexpr*e=toc_read_next_expression(tc);
		if(xexpr_is_empty(e))
			break;
		if(!o->exprs.count){
			str_add(&o->ops,'\0');
		}
		dynp_add(&o->exprs,e);
		if(toc_is_char_take(tc,'+')){
			str_add(&o->ops,'+');
		}else{
			break;
		}
	}
	xexpr*first=dynp_get(&o->exprs,0);
	o->super.type=first->type;
	return o;
}
