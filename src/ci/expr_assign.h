#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"

typedef struct ci_expr_assign{
	ci_expr super;
	str name;
	ci_expr*expr;
}ci_expr_assign;

inline static void _ci_expr_assign_free_(struct ci_expr*oo){
	ci_expr_assign*o=(ci_expr_assign*)oo;
//	str_free(&o->name);
	ci_expr_free(o->expr);
}

inline static void _ci_expr_assign_compile_(const ci_expr*oo,ci_toc*tc){
	ci_expr_assign*o=(ci_expr_assign*)oo;
	ci_toc_print_resolved_identifier_for_assignment(tc,
			o->name.data,o->expr->type.data);

	o->expr->compile((ci_expr*)o->expr,tc);
}

#define ci_expr_assign_def (ci_expr_assign){\
	{str_def,_ci_expr_assign_compile_,_ci_expr_assign_free_},str_def,0}

inline static/*gives*/ci_expr_assign*ci_expr_assign_next(
		const char**pp,ci_toc*tc,/*takes*/str name){

	ci_expr_assign*e=malloc(sizeof(ci_expr_assign));
	*e=ci_expr_assign_def;
	e->name=/*gives*/name;
	e->expr=_ci_expr_new_from_pp(pp,tc);
	e->super.type=e->expr->type;//? str shares buffer
	return e;
}

inline static void ci_expr_assign_parse(ci_expr_assign*o,
		const char**pp,ci_toc*tc,/*takes*/str name){

	o->name=/*gives*/name;
	o->expr=_ci_expr_new_from_pp(pp,tc);
	o->super.type=o->expr->type;//? str shares buffer
}
