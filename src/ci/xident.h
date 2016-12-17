#pragma once
#include"../lib.h"
#include "toc.h"

typedef struct xident{
	xexpr super;
	str name;
	char incdecbits;
}xident;

#define xident_def (xident){{_xident_compile_,NULL,str_def,0},str_def,false}

inline static void _xident_compile_(const xexpr*oo,toc*tc){
	const xident*o=(xident*)oo;

	// test string or charachter
	if(o->name.data[0]=='"' || o->name.data[0]=='\''){
		printf("%s",o->name.data);
		return;
	}
	// test boolean
	if(!strcmp("true",o->name.data)){
		printf("true");
		return;
	}
	if(!strcmp("false",o->name.data)){
		printf("false");
		return;
	}

	// test number
	char*endptr;
	//   float
	strtof(o->name.data,&endptr);
	if(*endptr=='f' && endptr!=o->name.data){
		printf("%s",o->name.data);
		return;
	}
	//   int
	strtol(o->name.data,&endptr,10);
	if(endptr==o->name.data+o->name.count-1){
		printf("%s",o->name.data);
		return;
	}
	//   hex or binary
	if(o->name.count>1){
		if((o->name.data[0]=='0' && o->name.data[1]=='x')){
			strtol(o->name.data+2,&endptr,16);
			if(endptr==o->name.data+o->name.count-2+1){
				printf("%s",o->name.data);
				return;
			}
		}
		if((o->name.data[0]=='0' && o->name.data[1]=='b')){
			unsigned v=strtol(o->name.data+2,&endptr,2);
			if(endptr==o->name.data+o->name.count-2+1){
				printf("0x%x",v);
				return;
			}
		}
	}

	// identifier ie  p.member  vs  localvar;
	const char*p=strpbrk(o->name.data,".");
	if(p){// p.member
		str s=str_def;
		str_add_list(&s,o->name.data,p-o->name.data);
		str_add(&s,0);

		const char idtype=toc_find_ident_scope_type(tc,s.data);
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
		const char idtype=toc_find_ident_scope_type(tc,o->name.data);
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


	if(*endptr!=0){
		// not end of string
		printf("\n\n<file> <line:col> '%s' is not found and could not "
				"be parsed to a number\n\n",o->name.data);
		longjmp(_jmpbufenv,1);

	}

//	str_setz(&o->super.type,"int");
	printf("%s",o->name.data);
}
