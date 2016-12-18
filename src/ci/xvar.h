#pragma once
#include"../lib.h"
#include "toc.h"
#include"xexpls.h"

inline static void toc_assert_can_set(toc*,ccharp,ccharp,token);
inline static xexpls*toc_read_next_xexpls(toc*,token);

typedef struct xset{
	xexpr super;
	str name;
	xexpr*expr;
}xset;

inline static void _xset_compile_(const xexpr*oo,toc*tc){
	xset*o=(xset*)oo;
	toc_compile_for_xset(tc,o->super.token,o->name.data,o->expr->type.data);
	o->expr->compile(o->expr,tc);
}

#define xset_def (xset){{_xset_compile_,NULL,str_def,token_def,0},str_def,0}

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
	xexpr*e=(xexpr*)toc_read_next_xexpls(tc,tk);
	o->expr=e;
//	o->expr=toc_read_next_xexpr(tc);
	toc_assert_can_set(tc,name.data,o->expr->type.data,tk);
	o->super.type=o->expr->type;
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

typedef struct xvar{
	xexpr super;
	str name;
	xset initval;
}xvar;

inline static void _xvar_compile_(const xexpr*oo,toc*tc){
	xvar*o=(xvar*)oo;
	const char idtype=toc_get_declaration_scope_type(tc,o->name.data);
	if(idtype){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' is already declared at ...",o->name.data);
		longjmp(_jmpbufenv,1);
		return;
	}

	printf("%s ",o->super.type.data);
	toc_add_ident(tc,o->super.type.data,o->name.data);
	if(o->initval.super.compile){
		_xset_compile_((xexpr*)&o->initval,tc);
	}else{
		printf("%s=%s_def",o->name.data,o->super.type.data);
	}
}

#define xvar_def (xvar){{_xvar_compile_,NULL,str_def,token_def,0},\
						str_def,xset_def}

inline static xvar*xvar_read_next(toc*tc,str type){
	xvar*o=malloc(sizeof(xvar));
	*o=xvar_def;
	o->super.type=type;
	o->super.token=toc_next_token(tc);
	token_setz(&o->super.token,&o->name);

	if(!strcmp("o",o->name.data)){
		toc_print_source_location(tc,o->super.token,4);
		printf("identifier 'o' is reserved, equivalent of 'this'");
		longjmp(_jmpbufenv,1);
	}

	toc_add_ident(tc,o->super.type.data,o->name.data);
	bool is_var=!strcmp(o->super.type.data,"var");
	if(toc_is_srcp_take(tc,'=')){
		xset_parse_next(&o->initval,tc,o->name,o->super.token);
		if(is_var){
			o->super.type=o->initval.super.type;
			toc_set_declaration_type(tc,o->name.data,o->super.type.data);
		}else{
			if(strcmp(o->super.type.data,o->initval.super.type.data)){
				printf("\n\n<file> <line:col> %s declared as %s but initial"
						" expression is of type %s\n",
						o->initval.name.data,
						o->super.type.data,
						o->initval.super.type.data);
				longjmp(_jmpbufenv,1);
			}
		}
	}else{
		o->initval.super.compile=NULL;
	}
	return o;
}
