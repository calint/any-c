#pragma once
#include"../lib.h"
#include"xtype.h"
#include"toc.h"

typedef struct xident{
	xexp super;
	strc ident;
	char incdecbits;
}xident;

#define xident_def (xident){\
	{_xident_compile_,NULL,_xident_print_source_,strc_def,token_def,0,false},\
	strc_def,false\
}

inline static void _xident_print_source_(xexp*oo){
}

inline static void _xident_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;

	strb/*takes*/acc_c=ci_get_c_accessor_for_accessor(
			tc,o->super.token,o->ident);

	ptrs split=/*takes*/strc_split(o->ident,'.');
	strb*first=(strb*)ptrs_get(&split,0);
	const char decl_scope_type=toc_get_declaration_scope_type(
			tc,o->super.token,first->data);
	strc_split_free(/*gives*/&split);

	if(decl_scope_type=='c'){// class member
		printf("o->%s",acc_c.data);
	}else{// variable
		printf("%s",acc_c.data);
	}
	strb_free(/*gives*/&acc_c);

	if(o->incdecbits&1)printf("++");
	if(o->incdecbits&2)printf("--");
}
