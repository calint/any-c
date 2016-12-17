#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xexpr{
	str type;
	void (*compile)(const struct xexpr*,struct toc*);
	void (*free)(struct xexpr*);
}xexpr;

#define xexpr_def (xexpr){str_def,NULL,NULL}

inline static int expression_is_empty(xexpr*o){
	return o->compile==NULL;
}

