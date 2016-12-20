#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xident{
	xexp super;
	cstr name;
	char incdecbits;
}xident;

#define xident_def (xident){{_xident_compile_,NULL,cstr_def,token_def,0},\
							cstr_def,false}

inline static void _xident_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;

	// identifier ie  p.member  vs  localvar;
	cstr p=strpbrk(o->name,".");
	if(p){// object.type.size
		str s=str_def;
		str_add_list(&s,o->name,p-o->name);
		str_add(&s,0);

		const char idtype=toc_get_declaration_scope_type(tc,s.data);
		if(idtype){
			if(idtype=='c'){// class member
				printf("o->%s",o->name);
			}else{
				printf("%s",o->name);
			}
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
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
