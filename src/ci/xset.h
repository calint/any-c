#pragma once
#include"xexpls.h"
#include"decouple.h"

typedef struct xset{
	xexp super;
	strc name;
	xexpls expls;//? xexpls
//	bool is_set_ref;
}xset;

inline static void _xset_compile_(const xexp*oo,toc*tc){
	xset*o=(xset*)oo;
	ci_xset_compile(tc,o);
	xexp*e=ptrs_get(&o->expls.exps,0);
	if(o->super.is_ref && !e->is_ref){
		printf("&");
	}else if(!o->super.is_ref && e->is_ref){
		printf("*");
	}

	o->expls.super.compile((xexp*)&o->expls,tc);
}

#define xset_def (xset){{_xset_compile_,NULL,strc_def,token_def,0,false},\
	strc_def,xexpls_def\
}

inline static void _xset_parse(xset*o,toc*tc,strc name,token tk){
	o->super.token=tk;
	o->name=name;
	const tocdecl*d=toc_get_declaration_for_accessor(tc,name);
	if(!d){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' not found",o->name);//? better msg
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
		return;
	}
	const xtyperef tr=ci_get_typeref_for_accessor(tc,tk,o->name);
	o->super.type=tr.type;
	o->super.is_ref=tr.is_ref;

	xexpls_parse_next(&o->expls,tc,tk,false);
	ci_xset_assert(tc,o);

	if(!strcmp(o->super.type,"var"))
		o->super.type=o->expls.super.type;

//	if(o->super.is_ref==o->super.is_ref)//? alwaystrue
//		return;
//
//	toc_print_source_location(tc,o->super.token,4);
//	printf("cannot set reference to non reference");
//	printf("\n    %s %d",__FILE__,__LINE__);
//	longjmp(_jmp_buf,1);
	return;
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
