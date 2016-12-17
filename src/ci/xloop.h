#pragma once

typedef struct xloop{
	xexpr super;
	code code;
}xloop;

inline static void _xloop_compile_(const xexpr*oo,toc*tc){
	xloop*o=(xloop*)oo;
	printf("while(1)");
	o->code.super.compile((xexpr*)&o->code,tc);
}

#define xloop_def (xloop){{_xloop_compile_,NULL,str_def,0},code_def}

inline static xloop*xloop_read_next(toc*tc){
	toc_push_scope(tc,'l',"");
	xloop*o=malloc(sizeof(xloop));
	*o=xloop_def;
	codeblk_read_next(&o->code,tc);
	toc_pop_scope(tc);
	return o;
}

typedef struct xbreak{
	xexpr super;
}xbreak;

inline static void _xbreak_compile_(const xexpr*oo,toc*tc){
	printf("break");
}

#define xbreak_def (xbreak){{_xbreak_compile_,NULL,str_def,0}}

inline static xbreak*xbreak_read_next(toc*tc){
	if(*tc->srcp==';')
		tc->srcp++;

	xbreak*e=malloc(sizeof(xbreak));
	*e=xbreak_def;
	return e;
}

typedef struct xcont{
	xexpr super;
}xcont;

inline static void _xcont_compile_(const xexpr*oo,toc*tc){
	printf("continue");
}

#define xcont_def (xcont){{_xcont_compile_,NULL,str_def,0}}

inline static xcont*xcont_read_next(toc*tc){
	if(*tc->srcp==';')
		tc->srcp++;
	xcont*e=malloc(sizeof(xcont));
	*e=xcont_def;
	return e;
}
