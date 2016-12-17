#pragma once
#include"../lib.h"
#include"toc.h"

typedef struct xset{
	xexpr super;
	str name;
	xexpr*expr;
}xset;

inline static void _xset_compile_(const xexpr*oo,toc*tc){
	xset*o=(xset*)oo;
	toc_compile_for_xset(tc,
			o->name.data,o->expr->type.data);

	o->expr->compile((xexpr*)o->expr,tc);
}

#define xset_def (xset){{str_def,_xset_compile_,NULL},str_def,0}

inline static/*gives*/xset*xset_read_next(
		const char**pp,toc*tc,/*takes*/str name){

	xset*e=malloc(sizeof(xset));
	*e=xset_def;
	e->name=/*gives*/name;
	e->expr=toc_next_expr_from_pp(pp,tc);
	e->super.type=e->expr->type;//? str shares buffer
	return e;
}

inline static void xset_parse_next(xset*o,
		const char**pp,toc*tc,/*takes*/str name){

	o->name=/*gives*/name;
	o->expr=toc_next_expr_from_pp(pp,tc);
	o->super.type=o->expr->type;//? str shares buffer
}
