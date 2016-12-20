#pragma once
#include"xexp.h"

typedef struct xbreak{
	xexp super;
}xbreak;

inline static void _xbreak_compile_(const xexp*oo,toc*tc){
	printf("break");
}

#define xbreak_def (xbreak){{_xbreak_compile_,NULL,cstr_def,token_def,0}}

inline static xbreak*xbreak_read_next(toc*tc){
	if(*tc->srcp==';')
		tc->srcp++;

	xbreak*e=malloc(sizeof(xbreak));
	*e=xbreak_def;
	return e;
}
