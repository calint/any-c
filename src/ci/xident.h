#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xident{
	xexp super;
	str name;
	char incdecbits;
}xident;

#define xident_def (xident){{_xident_compile_,NULL,str_def,token_def,0},\
							str_def,false}

inline static void _xident_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;

	// identifier ie  p.member  vs  localvar;
	cstr p=strpbrk(o->name.data,".");
	if(p){// object.type.size
		str s=str_def;
		str_add_list(&s,o->name.data,p-o->name.data);
		str_add(&s,0);

		const char idtype=toc_get_declaration_scope_type(tc,s.data);
		if(idtype){
			if(idtype=='c'){// class member
				printf("%s->%s",s.data,o->name.data);
			}else{
				printf("%s",o->name.data);
			}
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
	}else{// localvar or member
		const char idtype=toc_get_declaration_scope_type(tc,o->name.data);
		if(idtype=='c'){// class member
			printf("o->%s",o->name.data);
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
		if(idtype){// local identifier
			printf("%s",o->name.data);
			if(o->incdecbits&1)printf("++");
			if(o->incdecbits&2)printf("--");
			return;
		}
	}

	printf("%s",o->name.data);
}
