#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xident{
	xexp super;
	cstr name;
	char incdecbits;
}xident;

#define xident_def (xident){{_xident_compile_,NULL,cstr_def,token_def,0,false},\
							cstr_def,false}

inline static void _xident_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;

	// identifier ie  p.member  vs  localvar;
	cstr p=strpbrk(o->name,".");
	if(p){//       p.member
		str s=str_def;
		str_add_list(&s,o->name,p-o->name);
		str_add(&s,0);
		const char decl_scope_type=toc_get_declaration_scope_type(tc,s.data);
		const bool is_ref=toc_is_declaration_ref(tc,s.data);
		s.count--;//? adhock
		if(is_ref){
			str_add_string(&s,"->");
			str_add_string(&s,p+1);
		}else{
			str_add_string(&s,p);
		}
		str_add(&s,0);
		if(decl_scope_type){
			if(decl_scope_type=='c'){// class member
				printf("o->%s",s.data);
			}else{// variable
				printf("%s",s.data);
			}
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
		dynp_add(&_token_to_new_cstr_,s.data);// adhock
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
