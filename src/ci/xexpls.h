#pragma once
#include"toc.h"

inline static xexp*ci_read_next_expression(toc*);

typedef struct xexpls{
	xexp super;
	dynp exprs;
	str ops;
}xexpls;

#define xexpls_def (xexpls){{_xexpls_compile_,NULL,str_def,token_def,0},\
							dynp_def,str_def}

inline static void _xexpls_compile_(const xexp*oo,toc*tc){
	const xexpls*o=(xexpls*)oo;
	for(unsigned i=0;i<o->exprs.count;i++){
		const xexp*e=dynp_get(&o->exprs,i);
		const char op=str_get(&o->ops,i);
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
//	xexpls*o=malloc(sizeof(xexpls));
//	*o=xexpls_def;
//	o->super.type=type;//?
	o->super.token=tk;

	if(toc_srcp_is_take(tc,'(')){
		o->super.bits|=4;
	}
	while(1){
		if(!o->exprs.count)
			str_add(&o->ops,'\0');
		xexp*e;
		if(toc_srcp_is(tc,'(')){
			e=(xexp*)xexpls_read_next(tc,tk);
		}else{
			e=ci_read_next_expression(tc);
		}
		if(xexpr_is_empty(e))
			break;
		dynp_add(&o->exprs,e);
		if(toc_srcp_is_take(tc,'+')){str_add(&o->ops,'+');}
		else if(toc_srcp_is_take(tc,'-')){str_add(&o->ops,'-');}
		else if(toc_srcp_is_take(tc,'*')){str_add(&o->ops,'*');}
		else if(toc_srcp_is_take(tc,'/')){str_add(&o->ops,'/');}
		else if(toc_srcp_is_take(tc,'%')){str_add(&o->ops,'%');}
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

	xexp*first=dynp_get(&o->exprs,0);
	o->super.type=first->type;
//	return o;
}


