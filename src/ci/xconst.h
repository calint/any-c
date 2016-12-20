#pragma once
#include"xexp.h"

typedef struct xconst{
	xexp super;
	cstr name;
}xconst;

#define xconst_def (xconst){{_xconst_compile_,NULL,cstr_def,token_def,0},\
							cstr_def}

inline static void _xconst_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;
	printf("%s",o->name);
	if(!strcmp(o->super.type,"float"))
		printf("f");
}

