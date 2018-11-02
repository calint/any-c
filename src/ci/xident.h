#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xident{
	xexp super;
	strc name;
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

	strc p=strpbrk(o->name,".");
	// identifier ie  p.member  vs  localvar;
	if(p){//       p.member
		strb s=strb_def;
		strb_add_list(&s,o->name,p-o->name);
		strb_add(&s,0);
		const char decl_scope_type=toc_get_declaration_scope_type(tc,s.data);
		const bool is_ref=toc_is_declaration_ref(tc,s.data);
		s.count--;//? adhock
		if(is_ref){
			strb_add_string(&s,"->");
			strb_add_string(&s,p+1);
		}else{
			strb_add_string(&s,p);
		}
		strb_add(&s,0);
		if(decl_scope_type){
			if(decl_scope_type=='c'){// class member
				printf("o->%s",s.data);
			}else{// variable
				printf("%s",s.data);
			}
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			strb_free(&s);
			return;
		}
		ptrs_add(&_token_to_new_strc_,s.data);//? adhock
//		strb_free(&s);
	}else{// localvar or member
		const char idtype=toc_get_declaration_scope_type(tc,o->name);
		if(idtype=='c'){// class member
			printf("o->%s",o->name);
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
		if(idtype){// local identifier
			printf("%s",o->name);
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
	}

	printf("%s",o->name);
}
