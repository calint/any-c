#pragma once
#include"../lib.h"
#include "codeblock.h"
#include "expr.h"
#include "toc.h"

typedef struct xbreak{
	xexpr super;
}xbreak;

inline static void _xbreak_compile_(const xexpr*oo,toc*tc){
	printf("break");
}

#define xbreak_def (xbreak){{str_def,_xbreak_compile_,NULL}}

inline static xbreak*xbreak_read_next(const char**pp,toc*tc){
	if(**pp!=';'){
		printf("\n\n<file> <line:col> expected ';' after keyword 'break'");
		exit(1);
	}
	(*pp)++;
	xbreak*e=malloc(sizeof(xbreak));
	*e=xbreak_def;
	return e;
}
