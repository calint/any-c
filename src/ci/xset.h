#pragma once
#include"xexpls.h"

//inline static void ci_assert_can_set(toc*,ccharp,ccharp,token);

typedef struct xset{
	xexp super;
	str name;
	xexpls expls;//? xexpls
}xset;

inline static void _xset_compile_(const xexp*oo,toc*tc){
	xset*o=(xset*)oo;
	toc_compile_for_xset(tc,o->super.token,o->name.data,
	o->expls.super.type.data);

	o->expls.super.compile((xexp*)&o->expls,tc);
}

#define xset_def (xset){{_xset_compile_,NULL,str_def,token_def,0},str_def,\
	xexpls_def}

inline static void _xset_init(toc*tc,xset*o,str name,token tk){
	const tocdecl*d=toc_get_declaration(tc,name.data);
	if(!d){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' not found",o->name.data);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmpbufenv,1);
		return;
	}

	o->super.token=tk;
	o->name=name;
//	xexpr*e=(xexpr*)toc_read_next_xexpls(tc,tk);
//	o->expr=e;
	xexpls_parse_next(&o->expls,tc, tk);
	ci_assert_can_set(tc,name.data,o->expls.super.type.data,tk);
	o->super.type=o->expls.super.type;
}

inline static/*gives*/xset*xset_read_next(toc*tc,str name,token tk){
	xset*o=malloc(sizeof(xset));
	*o=xset_def;
	_xset_init(tc,o,name,tk);
	return o;
}

inline static void xset_parse_next(xset*o,toc*tc,str name,token tk){
	_xset_init(tc,o,name,tk);
}
