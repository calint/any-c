#pragma once
#include"../lib.h"
#include "expr.h"
#include "toc.h"

//inline static ci_class*ci_find_class_by_name(const char*);

typedef struct ci_expr_ident{
	ci_expr super;
	str name;
	long int cnst;
}ci_expr_ident;

inline static void _ci_expr_ident_free_(struct ci_expr*oo){
	ci_expr_ident*o=(ci_expr_ident*)oo;
	ci_expr_free((ci_expr*)o);
}

inline static void _ci_expr_ident_compile_(const ci_expr*oo,ci_toc*tc){
	const ci_expr_ident*o=(ci_expr_ident*)oo;

	// constant

	// string or character
	if(o->name.data[0]=='"' || o->name.data[0]=='\''){
		printf("%s",o->name.data);
		return;
	}

	// boolean
	if(!strcmp("true",o->name.data)){
		printf("true");
		return;
	}

	if(!strcmp("false",o->name.data)){
		printf("false");
		return;
	}

	char*endptr;

	// float
	strtof(o->name.data,&endptr);
	if(*endptr=='f' && endptr!=o->name.data){
		printf("%s",o->name.data);
		return;
	}

	// int
	strtol(o->name.data,&endptr,10);
	if(endptr==o->name.data+o->name.count-1){
		printf("%s",o->name.data);
		return;
	}

	// hex
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


	const char*p=strpbrk(o->name.data,".");
	if(p){
		str s=str_def;
		str_add_list(&s,o->name.data,p-o->name.data);
		str_add(&s,0);

		const char idtype=ci_toc_find_ident_scope_type(tc,s.data);
		if(idtype){
			if(idtype=='c'){// class member
				printf("%s->%s",s.data,o->name.data);
			}else{
				printf("%s",o->name.data);
			}
//			printf("=%s",o->name.data);
			return;
		}
	}else{
		const char idtype=ci_toc_find_ident_scope_type(tc,o->name.data);
		if(idtype=='c'){// class member
			printf("o->%s",o->name.data);
			return;
		}
		if(idtype){// local identifier
			printf("%s",o->name.data);
			return;
		}
	}


	if(*endptr!=0){
		// not end of string
		printf("\n\n<file> <line:col> '%s' is not found and could not "
				"be parsed to a number\n\n",o->name.data);
		exit(1);

	}

//	str_setz(&o->super.type,"int");
	printf("%s",o->name.data);
}

#define ci_expr_ident_def (ci_expr_ident){\
	{str_def,_ci_expr_ident_compile_,_ci_expr_ident_free_},str_def,0}
