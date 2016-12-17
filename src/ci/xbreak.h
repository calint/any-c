#pragma once

typedef struct xbreak{
	xexpr super;
}xbreak;

inline static void _xbreak_compile_(const xexpr*oo,toc*tc){
	printf("break");
}

#define xbreak_def (xbreak){{str_def,_xbreak_compile_,NULL}}

inline static xbreak*xbreak_read_next(const char**pp,toc*tc){
	if(**pp==';')
		(*pp)++;

	xbreak*e=malloc(sizeof(xbreak));
	*e=xbreak_def;
	return e;
}
