#pragma once
#include"xexpls.h"
#include"xbool.h"
#include"decouple.h"

typedef struct xset{
	xexp super;
	strc name;
	xexp*exp;
}xset;

#define xset_def (xset){\
	{_xset_compile_,_xset_free_,_xset_print_source_,strc_def,token_def,0,false},\
	strc_def,NULL\
}

inline static void _xset_compile_(const xexp*oo,toc*tc){
	xset*o=(xset*)oo;
	ci_xset_compile(tc,o);
	xexp*e=o->exp;
	if(o->super.is_ref && !e->is_ref){
		printf("&");
	}else if(!o->super.is_ref && e->is_ref){
		printf("*");
	}

	o->exp->compile(o->exp,tc);
}

inline static void _xset_free_(xexp*oo){
	xset*o=(xset*)oo;
	o->exp->free(o->exp);
	free(o->exp);
}

inline static void _xset_print_source_(xexp*oo){
	xset*o=(xset*)oo;
	token_print_including_whitespace(&o->super.token);
	printf("=");
	if(o->exp->print_source)
		o->exp->print_source(o->exp);
}

inline static void _xset_parse(xset*o,toc*tc,strc name,token tk){
	o->super.token=tk;
	o->name=name;
	const tocdecl*d=toc_get_declaration_for_accessor(tc,name);
	if(!d){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' not found",o->name);//? better msg
		printf("\n    %s %d\n",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
		return;
	}
	const xtyperef tr=ci_get_typeref_for_accessor(tc,tk,o->name);
	o->super.type=tr.type;
	o->super.is_ref=tr.is_ref;

	int ret=0;
	o->exp=(xexp*)xexpls_read_next(tc,tk,false,1,&ret);// mode=1 try to parse
	if(ret){// could not parse, try bool
		o->exp=(xexp*)xbool_read_next(tc,tk);
	}
//	if(strcmp(o->super.type,"bool")){
//		int ret=0;
//		o->exp=(xexp*)xexpls_read_next(tc,tk,false,&ret,0);
//	}else{
//		o->exp=(xexp*)xbool_read_next(tc,tk);
//	}
//	xexpls_parse_next(&o->setexpls,tc,tk,false);
	ci_xset_assert(tc,o);

	if(!strcmp(o->super.type,"var"))
		o->super.type=o->exp->type;
}

inline static/*gives*/xset*xset_read_next(toc*tc,strc name,token tk){
	xset*o=malloc(sizeof(xset));
	*o=xset_def;
	_xset_parse(o,tc,name,tk);
	return o;
}

inline static void xset_parse_next(xset*o,toc*tc,strc name,token tk){
	_xset_parse(o,tc,name,tk);
}
