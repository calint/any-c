#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xset{
	xexpr super;
	str name;
	xexpr*expr;
}xset;

inline static void _xset_compile_(const xexpr*oo,toc*tc){
	xset*o=(xset*)oo;
	toc_compile_for_xset(tc,o->name.data,o->expr->type.data);
	o->expr->compile((xexpr*)o->expr,tc);
}

#define xset_def (xset){{_xset_compile_,NULL,str_def,token_def,0},str_def,0}

inline static/*gives*/xset*xset_read_next(toc*tc,/*takes*/str name){
	xset*e=malloc(sizeof(xset));
	*e=xset_def;
	e->name=/*gives*/name;
	e->expr=toc_read_next_xexpr(tc);
	e->super.type=e->expr->type;//? str shares buffer
	return e;
}

inline static void xset_parse_next(xset*o,toc*tc,str name){
	o->name=/*gives*/name;
	o->expr=toc_read_next_xexpr(tc);
	o->super.type=o->expr->type;//? str shares buffer
}


typedef struct xvar{
	xexpr super;
	str name;
	xset initval;
}xvar;

inline static void _xvar_compile_(const xexpr*oo,toc*tc){
	xvar*o=(xvar*)oo;
	const char idtype=toc_find_ident_scope_type(tc,o->name.data);
	if(idtype){
		printf("\n\n<file> <line:col> '%s' is already declared at ..."
				,o->initval.name.data);
		longjmp(_jmpbufenv,1);
		return;
	}

	toc_add_ident(tc,o->super.type.data,o->name.data);

	printf("%s ",o->super.type.data);
	if(o->initval.super.compile){
		_xset_compile_((xexpr*)&o->initval,tc);
	}else{
		printf("%s=%s_def",o->name.data,o->super.type.data);
	}
}

#define xvar_def (xvar){{_xvar_compile_,NULL,str_def,token_def,0},\
						str_def,xset_def}

inline static xvar*xvar_read_next(toc*tc,str type){
	xvar*e=malloc(sizeof(xvar));
	*e=xvar_def;
	e->super.type/*takes*/=type;
	token tk=toc_next_token(tc);
	token_setz(&tk,&e->name);

	if(!strcmp("o",e->name.data)){
		printf("\n\n<file> <line:col> identifier 'o' is reserved");
		longjmp(_jmpbufenv,1);
	}

	bool is_var=!strcmp(e->super.type.data,"var")||
			!strcmp(e->super.type.data,"auto");

	if(*tc->srcp=='='){
		tc->srcp++;
		xset_parse_next(&e->initval,tc,/*shares*/e->name);
		if(is_var){
			e->super.type=e->initval.super.type;
		}else{
			if(strcmp(e->super.type.data,e->initval.super.type.data)){
				printf("\n\n<file> <line:col> %s declared as %s but initial"
						" expression is of type %s\n",
						e->initval.name.data,
						e->super.type.data,
						e->initval.super.type.data);
				longjmp(_jmpbufenv,1);
			}
		}
	}else{
		e->initval.super.compile=NULL;
	}
	return e;
}
