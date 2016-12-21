#pragma once
#include"xexpls.h"
#include"decouple.h"
//inline static void ci_assert_set(const toc*,cstr,cstr,token);
//inline static void ci_xset_compile(const toc*,token,cstr,cstr);

typedef struct xset{
	xexp super;
	cstr name;
	xexpls expls;//? xexpls
	bool is_set_ref;
}xset;

inline static void _xset_compile_(const xexp*oo,toc*tc){
	xset*o=(xset*)oo;
	ci_xset_compile(tc,o->super.token,o->name,o->expls.super.type);
	if(o->is_set_ref)
		printf("&");
	o->expls.super.compile((xexp*)&o->expls,tc);
}

#define xset_def (xset){{_xset_compile_,NULL,cstr_def,token_def,0},\
	cstr_def,xexpls_def,false\
}

inline static void _xset_init(toc*tc,xset*o,cstr name,token tk){
	o->super.token=tk;
	o->name=name;
	const tocdecl*d=toc_get_declaration(tc,name);
	if(!d){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' not found",o->name);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
		return;
	}
	xexpls_parse_next(&o->expls,tc, tk);
	ci_assert_set(tc,name,o->expls.super.type,tk);
	o->super.type=o->expls.super.type;
}

inline static/*gives*/xset*xset_read_next(toc*tc,cstr name,token tk){
	xset*o=malloc(sizeof(xset));
	*o=xset_def;
	_xset_init(tc,o,name,tk);
	return o;
}

inline static void xset_parse_next(xset*o,toc*tc,cstr name,token tk){
	_xset_init(tc,o,name,tk);
}
