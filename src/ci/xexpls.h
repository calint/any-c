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
	{_xexpls_compile_,_xexpls_free_,_xexpls_print_source_,strc_def,token_def,0,false},\
		ptrs_def,strb_def\
}

inline static void _xexpls_compile_(const xexp*oo,toc*tc){
	const xexpls*o=(xexpls*)oo;
	for(unsigned i=0;i<o->exps.count;i++){
		const xexp*e=ptrs_get(&o->exps,i);
		const char op=strb_get(&o->ops,i);
		if(i!=0)
			printf("%c",op);
		if(xexp_is_negated(e))
			printf("-");
		if(xexp_is_parenthesis(e))
			printf("(");
		e->compile(e,tc);
		if(xexp_is_parenthesis(e))
			printf(")");
	}
}

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
//	free(oo);
}

inline static void _xexpls_print_source_(xexp*oo){
	xexpls*o=(xexpls*)oo;
	const long n=o->exps.count;
	for(int i=0;i<n;i++){
		xexp*e=ptrs_get(&o->exps,i);
		char op=strb_get(&o->ops,i);
		if(op)
			printf("%c",op);
		e->print_source(e);
	}
}

inline static void xexpls_parse_next(xexpls*,toc*,token,bool,int,int*);
// ret: return code, mode: 0 normal, 1 try and return ret code
inline static xexpls*xexpls_read_next(
		toc*tc,token tk,bool issubexpr,int mode,int*ret){
	xexpls*o=malloc(sizeof(xexpls));
	*o=xexpls_def;
	xexp_set_is_parenthesis(&o->super,issubexpr);
	xexpls_parse_next(o,tc,tk,issubexpr,mode,ret);
	return o;
}

inline static void xexpls_parse_next(
		xexpls*o,toc*tc,token tk,bool issubexpr,int mode,int*ret){
	o->super.token=tk;
	xexp*prev_exp=NULL;
	const char*entry_srcp=tc->srcp;
	while(1){
		if(!o->exps.count)
			strb_add(&o->ops,'\0');
		token_skip_empty_space(&tc->srcp); //? lost info
		bool is_negated=toc_srcp_is_take(tc,'-');
		xexp*e;
		if(toc_srcp_is_take(tc,'(')){
			int result=0;
			xexpls*els=xexpls_read_next(tc,tk,true,mode,&result);
			if(mode==1 && result){
				*ret=result;
				tc->srcp=entry_srcp;
				return;
			}
			//? xexpls_deduce_typeref(&o->exps)
			xexp*first=ptrs_get(&els->exps,0);
			els->super.type=first->type;
			els->super.is_ref=first->is_ref;
			e=(xexp*)els;
		}else{
			e=ci_read_next_expression(tc);
		}
		xexp_set_is_negated(e,is_negated);
		if(prev_exp){
			if(strcmp(e->type,prev_exp->type)){
				toc_print_source_location(tc,e->token,4);
				printf("expression type '%s' is not same as previous type '%s'",
						e->type,prev_exp->type);
				printf("\n    %s %d\n",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
		}
		prev_exp=e;
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
		else {
			if(!issubexpr)
				break;
			if(mode==0){
				toc_print_source_location2(tc,tc->srcp,4);
				printf("expected operator + - * / %% | & ^");
				printf("\n    %s %d\n",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			*ret=1;
			tc->srcp=entry_srcp;
			return;
		}
	}
	//? xexpls_deduce_typeref(&o->exps)
	xexp*first=ptrs_get(&o->exps,0);
	o->super.type=first->type;
	o->super.is_ref=first->is_ref;
}


