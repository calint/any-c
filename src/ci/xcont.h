#pragma once
#include"xexp.h"

typedef struct xcont{
	xexp super;
}xcont;

inline static void _xcont_compile_(const xexp*oo,toc*tc){
	printf("continue");
}

#define xcont_def (xcont){{_xcont_compile_,NULL,str_def,token_def,0}}

inline static xcont*xcont_read_next(toc*tc,token tk){
	toc_charp_skip_if(tc,';');
	xcont*o=malloc(sizeof(xcont));
	*o=xcont_def;
	o->super.token=tk;
	return o;
}
