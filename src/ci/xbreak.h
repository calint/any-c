#pragma once
#include"xexp.h"

typedef struct xbreak{
	xexp super;
}xbreak;

inline static void _xbreak_compile_(const xexp*oo,toc*tc){
	ci_xcode_compile_free_current_loop_scope(tc,oo->token);
	printf("break");
}

#define xbreak_def (xbreak){{_xbreak_compile_,NULL,cstr_def,token_def,0}}

inline static xbreak*xbreak_read_next(toc*tc,token tk){
	if(*tc->srcp==';')
		tc->srcp++;

	xbreak*e=malloc(sizeof(xbreak));
	*e=xbreak_def;
	e->super.token=tk;

//	if(ci_xcode_needs_compile_free_current_loop_scope(tc,tk))
//		e->super.bits|=1; // will generate statements, make it block

	return e;
}
