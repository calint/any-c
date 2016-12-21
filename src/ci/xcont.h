#pragma once
#include"xexp.h"

typedef struct xcont{
	xexp super;
}xcont;

inline static void _xcont_compile_(const xexp*oo,toc*tc){
	ci_xcode_compile_free_current_loop_scope(tc,oo->token);
	toc_print_indent_for_compile(tc);
	printf("continue");
}

#define xcont_def (xcont){{_xcont_compile_,NULL,cstr_def,token_def,0}}

inline static xcont*xcont_read_next(toc*tc,token tk){
	toc_srcp_skip_if(tc,';');
	xcont*o=malloc(sizeof(xcont));
	*o=xcont_def;
	o->super.token=tk;
	return o;
}
