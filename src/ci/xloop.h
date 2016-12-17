#pragma once

typedef struct xloop{
	xexpr super;
	codeblk code;
}xloop;

inline static void _xloop_compile_(const xexpr*oo,toc*tc){
	xloop*o=(xloop*)oo;
	printf("while(1)");
	o->code.super.compile((xexpr*)&o->code,tc);
}

#define xloop_def (xloop){{str_def,_xloop_compile_,NULL},codeblk_def}

inline static xloop*xloop_read_next(const char**pp,toc*tc){
	toc_push_scope(tc,'l',"");
	xloop*o=malloc(sizeof(xloop));
	*o=xloop_def;
	codeblk_read_next(&o->code,pp,tc);
	toc_pop_scope(tc);
	return o;
}
