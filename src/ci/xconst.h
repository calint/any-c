#pragma once
#include"xexp.h"

typedef struct xconst{
	xexp super;
	strc name;
}xconst;

inline static void _xconst_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;
	printf("%s",o->name);
	if(!strcmp(o->super.type,"float"))
		printf("f");
}

inline static void _xconst_print_(xexp*oo){
	const xident*o=(xident*)oo;
	printf("%s",o->name);
}

#define xconst_def (xconst){\
	{_xconst_compile_,NULL,_xconst_print_,strc_def,token_def,0,false},\
		strc_def\
}
