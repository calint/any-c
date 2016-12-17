#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"

typedef struct xassign{
	xexpr super;
	str name;
	xexpr*expr;
}xassign;

inline static void _xassign_free_(struct expression*oo){
//	ci_expr_assign*o=(ci_expr_assign*)oo;
//	str_free(&o->name);
//	ci_expr_free(o->expr);
}

inline static void _xassign_compile_(const xexpr*oo,toc*tc){
	xassign*o=(xassign*)oo;
	toc_print_resolved_identifier_for_assignment(tc,
			o->name.data,o->expr->type.data);

	o->expr->compile((xexpr*)o->expr,tc);
}

#define ci_expr_assign_def (xassign){\
	{str_def,_xassign_compile_,_xassign_free_},str_def,0}

inline static/*gives*/xassign*xassign_next(
		const char**pp,toc*tc,/*takes*/str name){

	xassign*e=malloc(sizeof(xassign));
	*e=ci_expr_assign_def;
	e->name=/*gives*/name;
	e->expr=toc_next_expr_from_pp(pp,tc);
	e->super.type=e->expr->type;//? str shares buffer
	return e;
}

inline static void xassign_parse(xassign*o,
		const char**pp,toc*tc,/*takes*/str name){

	o->name=/*gives*/name;
	o->expr=toc_next_expr_from_pp(pp,tc);
	o->super.type=o->expr->type;//? str shares buffer
}
