#pragma once

typedef struct xcont{
	xexpr super;
}xcont;

inline static void _xcont_compile_(const xexpr*oo,toc*tc){
	printf("continue");
}

#define xcont_def (xcont){{_xcont_compile_,NULL,str_def,0}}

inline static xcont*xcont_read_next(const char**pp,toc*tc){
	if(**pp==';')
		(*pp)++;
	xcont*e=malloc(sizeof(xcont));
	*e=xcont_def;
	return e;
}
