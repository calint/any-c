#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct ci_expr{
	str type;
	void (*compile)(const struct ci_expr*,struct ci_toc*);
	void (*free)(struct ci_expr*);
}ci_expr;

inline static void ci_expr_free(ci_expr*o){
	str_free(&o->type);
//	free(o);
}

#define ci_expr_def (ci_expr){str_def,NULL,ci_expr_free}

inline static int ci_expr_is_empty(ci_expr*o){
	return o->compile==NULL;
}

