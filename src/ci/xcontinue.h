#pragma once
#include"../lib.h"
#include "codeblock.h"
#include "toc.h"

typedef struct xcontinue{
	xexpr super;
}xcontinue;

inline static void _xcontinue_compile_(const xexpr*oo,toc*tc){
	printf("continue");
}

#define xcontinue_def (xcontinue){\
	{str_def,_xcontinue_compile_,NULL}}

inline static xcontinue*xcontinue_read_next(const char**pp,toc*tc){
	if(**pp!=';'){
		printf("\n\n<file> <line:col> expected ';' after keyword 'break'");
		exit(1);
	}
	(*pp)++;
	xcontinue*e=malloc(sizeof(xcontinue));
	*e=xcontinue_def;
	return e;
}
