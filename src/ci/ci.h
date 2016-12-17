#pragma once
#include"../lib.h"
#include "codeblk.h"
#include "type.h"
#include "xbreak.h"
#include "xcall.h"
#include "xcontinue.h"
#include "xident.h"
#include "xife.h"
#include "xloop.h"
#include "xset.h"
#include "xvar.h"

inline static type*toc_find_class_by_name(toc*o,const char*name){
	for(unsigned i=0;i<o->types.count;i++){
		type*c=dynp_get(&o->types,i);
		if(!strcmp(c->name.data,name)){
			return c;
		}
	}
	return NULL;
}

inline static void toc_add_class(toc*o,type*c){
	dynp_add(&o->types,c);
}

inline static bool toc_can_assign(toc*tc,
		const char*dst,const char*path,const char*src){

	type*c=toc_find_class_by_name(tc,dst);
	const char*endptr=path;
	while(1){
		const char*p=strpbrk(endptr,".");
		str ident=str_def;
		if(p){
			str_add_list(&ident,endptr,p-endptr);
			str_add(&ident,0);
			endptr=p+1;
		}else{
			str_add_list(&ident,endptr,strlen(endptr));
			str_add(&ident,0);
		}

		if(!c){
			return !strcmp(dst,src);
		}

		bool found=false;
		for(unsigned i=0;i<c->extends.count;i++){
			str*ext=(str*)dynp_get(&c->extends,i);
			if(!strcmp(ext->data,ident.data)){
				c=toc_find_class_by_name(tc,ext->data);
				found=true;
				break;
			}
		}
		if(found)continue;
		for(unsigned i=0;i<c->fields.count;i++){
			field*fld=(field*)dynp_get(&c->fields,i);
			if(!strcmp(fld->name.data,ident.data)){
				c=toc_find_class_by_name(tc,fld->type.data);
				if(!c){
					if(!src)
						return true;//?
					return !strcmp(fld->type.data,src);
				}
				found=true;
				break;
			}
		}
		if(found)continue;
		printf("<file> <line:col> cannot find '%s' in '%s'\n",
				path,c->name.data);
		exit(1);
	}
}


inline static void ci_init(){}

inline static void ci_free(){
//	dynp_foa(&ci_classes,{
//		ci_class_free((ci_class*)o);
//	});
//	dynp_free(&ci_classes);
}

inline static str const_str(const char*s){
	str st=str_def;
	st.data=s;
	st.cap=st.count=strlen(s)+1;
	return st;
}

inline static /*gives*/xexpr*toc_next_expr_from_pp(
		const char**pp,toc*tc){

	token tk=token_next(pp);
	if(token_is_empty(&tk)){
		if(**pp=='"'){
			(*pp)++;
			while(1){
				const char c=**pp;
				if(c==0){
					printf("<file> <line:col> did not find the "
							" end of string\n");
					exit(1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					exit(1);
				}
				(*pp)++;
				if(c=='\\'){
					(*pp)++;
					continue;
				}
				if(c=='"')break;
			}
			tk.end=tk.content_end=*pp;
			xident*e=malloc(sizeof(xident));
			*e=xident_def;
			str name=str_def;
			token_setz(&tk,&name);
			e->name=name;
			e->super.type=str_from_string("const char*");
			return(xexpr*)e;
		}else if(**pp=='\''){
			(*pp)++;
			(*pp)++;
			if(**pp!='\''){
				printf("<file> <line:col> expected a character\n"
						" example 'a'\n");
				exit(1);
			}
			(*pp)++;
			tk.end=tk.content_end=*pp;
			xident*e=malloc(sizeof(xident));
			*e=xident_def;
			str name=str_def;
			token_setz(&tk,&name);
			e->name=name;
			e->super.type=str_from_string("char");
			return(xexpr*)e;
		}else{
			xexpr*e=malloc(sizeof(xexpr));
			*e=xexpr_def;
			return e;
		}
	}


	// constant
	str tks=str_def;
	token_setz(&tk,&tks);

	// boolean
	if(!strcmp("true",tks.data) || !strcmp("false",tks.data)){
		xident*e=malloc(sizeof(xident));
		*e=xident_def;
		e->name=tks;
		e->super.type=const_str("bool");
		return(xexpr*)e;
	}


	char*endptr;
	// int
	strtol(tks.data,&endptr,10);
	if(endptr==tks.data+tks.count-1){
		xident*e=malloc(sizeof(xident));
		*e=xident_def;
		e->name=tks;
		e->super.type=const_str("int");
		return(xexpr*)e;
	}

	// float
	strtof(tks.data,&endptr);
	if(*endptr=='f' && tks.count>2){
		xident*e=malloc(sizeof(xident));
		*e=xident_def;
		e->name=tks;
		e->super.type=const_str("float");
		return(xexpr*)e;
	}
//

//
	// hex
	if(tks.count>1){
		if((tks.data[0]=='0' && tks.data[1]=='x')){
			strtol(tks.data+2,&endptr,16);
			if(endptr==tks.data+tks.count-2+1){
				xident*e=malloc(sizeof(xident));
				*e=xident_def;
				e->name=tks;
				e->super.type=const_str("int");
				return(xexpr*)e;
			}
		}
		if((tks.data[0]=='0' && tks.data[1]=='b')){
			strtol(tks.data+2,&endptr,2);
			if(endptr==tks.data+tks.count-2+1){
				xident*e=malloc(sizeof(xident));
				*e=xident_def;
				e->name=tks;
				e->super.type=const_str("int");
				return(xexpr*)e;
			}
		}
	}

	if(token_equals(&tk,"loop")){
		xloop*e=xloop_read_next(pp,tc);
		return (xexpr*)e;
	}

	if(token_equals(&tk,"break")){
		xbreak*e=xbreak_read_next(pp,tc);
		return (xexpr*)e;
	}

	if(token_equals(&tk,"continue")){
		xcontinue*e=xcontinue_read_next(pp,tc);
		return (xexpr*)e;
	}

	if(token_equals(&tk,"if")){
		xife*e=xife_read_next(pp,tc);
		return (xexpr*)e;
	}

	str name=str_def;
	token_setz(&tk,&name);
	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
			token_equals(&tk,"bool")||token_equals(&tk,"char")||
			token_equals(&tk,"auto")||token_equals(&tk,"var")){
		xvar*e=xvar_read_next(pp,tc,name);
		return(xexpr*)e;
	}

	type*c=toc_find_class_by_name(tc,name.data);
	if(c){// instantiate
		xvar*e=xvar_read_next(pp,tc,name);
		return(xexpr*)e;
	}

	if(**pp=='('){// function call
		xcall*e=xcall_read_next(pp,tc,/*gives*/name);
		return(xexpr*)e;
	}

	if(**pp=='='){// assignment
		(*pp)++;
		if(**pp!='='){
			xset*e=/*takes*/xset_read_next(pp,tc,/*gives*/name);
			return(xexpr*)e;
		}
		(*pp)--;
	}

	char incdecbits=0;
	if(**pp=='+'){
		(*pp)++;
		if(**pp=='+'){
			(*pp)++;
			incdecbits|=1;
		}else{
			(*pp)--;
			(*pp)--;
		}
	}else if(**pp=='-'){
		(*pp)++;
		if(**pp=='-'){
			(*pp)++;
			incdecbits|=2;
		}else{
			(*pp)--;
			(*pp)--;
		}
	}

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->incdecbits=incdecbits;
	return(xexpr*)e;
}
//inline static str ci_abbreviate_func_arg_name_for_type(ci_toc*tc,const char*tp){
//	return *tp;
//}
inline static void toc_parse_func(const char**pp,toc*tc,type*c,
		token*type){
	func*f=malloc(sizeof(func));
	*f=func_def;
	bool enclosed_args=false;
	if(**pp=='{' || **pp=='('){
		f->type=const_str("void");
		token_setz(type,&f->name);
	}else{
		token tkname=token_next(pp);
		token_setz(type,&f->type);
		token_setz(&tkname,&f->name);
	}

	dynp_add(&c->funcs,f);

	if(**pp=='('){
		enclosed_args=true;
		(*pp)++;
	}

	toc_push_scope(tc,'f',f->name.data);
	while(1){
		token tkt=token_next(pp);
		if(token_is_empty(&tkt)){
			break;
		}
		funcarg*fa=malloc(sizeof(funcarg));
		dynp_add(&f->args,fa);
		*fa=funcarg_def;
		token tkn=token_next(pp);
		token_setz(&tkt,&fa->type);
		token_setz(&tkn,&fa->name);

		toc_add_ident(tc,fa->type.data,fa->name.data);

		if(**pp==','){
			(*pp)++;
		}
	}
	if(enclosed_args){
		if(**pp==')'){
			(*pp)++;
		}else{
			printf("<file> <line:col> expected ')' after arguments\n");
			exit(1);
		}
	}
	codeblk_read_next(&f->code,pp,tc);
	toc_pop_scope(tc);
}

inline static void toc_parse_field(const char**pp,toc*tc,type*c,
		token*type,token*name){

	field*f=malloc(sizeof(field));
	*f=field_def;
	token_setz(type,&f->type);
	if(token_is_empty(name)){
		token_setz(type,&f->name);
	}else{
		token_setz(name,&f->name);
	}
	dynp_add(&c->fields,f);
	toc_add_ident(tc,f->type.data,f->name.data);
	if(**pp=='='){
		(*pp)++;
		xexpr*e=toc_next_expr_from_pp(pp,tc);
		f->initval=e;
		if(strcmp(f->type.data,"var") && !toc_can_assign(tc,
				f->type.data,f->initval->type.data,e->type.data)){

			printf("<file> <line:col> cannot assign '%s' to '%s'\n",
					e->type.data,f->type.data);
			exit(1);
		}
		f->type=e->type;//? assert types
	}
	if(**pp==';'){
		(*pp)++;
	}
	return;
}

//inline static void _ci_compile_to_c(ci_toc*tc){
//	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//	printf("#include<stdlib.h>\n");
//	printf("#include<stdio.h>\n");
//	printf("typedef char bool;\n");
//	printf("#define true 1\n");
//	printf("#define false 1\n");
//	printf("#define char_def 0\n");
//	printf("#define int_def 0\n");
//	printf("#define float_def 0.0f\n");
//	printf("#define bool_def false\n");
//	for(unsigned i=0;i<ci_classes.count;i++){
//		ci_class*c=(ci_class*)dynp_get(&ci_classes,i);
//		ci_toc_push_scope(tc,'c',c->name.data);
//		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//		printf("typedef struct %s{",c->name.data);
//		if(c->extends.count||c->fields.count)
//			printf("\n");
//		for(unsigned i=0;i<c->extends.count;i++){
//			str*s=(str*)dynp_get(&c->extends,i);
//			printf("    %s %s;\n",s->data,s->data);
//		}
//		// fields
//		for(unsigned i=0;i<c->fields.count;i++){
//			ci_field*f=(ci_field*)dynp_get(&c->fields,i);
//			ci_toc_add_ident(tc,f->type.data,f->name.data);
//			if(!strcmp(f->type.data,"auto")){
//				if(!f->initval){
//					printf("<file> <line:col> expected initializer with auto");
//					exit(1);
//				}
//				f->initval->compile(f->initval,tc);
//				f->type=f->initval->type;
//			}
//			printf("    %s %s;\n",f->type.data,f->name.data);
//		}
//		printf("}%s;\n",c->name.data);
//
//		// #define object_def {...}
//		printf("#define %s_def (%s){",c->name.data,c->name.data);
//		for(unsigned i=0;i<c->extends.count;i++){
//			str*s=(str*)dynp_get(&c->extends,i);
//			printf("%s_def",s->data);
//			if(i!=c->extends.count-1)
//				printf(",");
//		}
//		for(unsigned i=0;i<c->fields.count;i++){
//			ci_field*s=(ci_field*)dynp_get(&c->fields,i);
//			if(s->initval){
//				s->initval->compile(s->initval,tc);
//			}else{
//				printf("%s_def",s->type.data);
//			}
//			if(i!=c->fields.count-1)
//				printf(",");
//		}
//		printf("}\n");
////
////		// casts
//////		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
////		for(unsigned i=0;i<c->extends.count;i++){
////			str*s=(str*)dynp_get(&c->extends,i);
////			printf(	"inline static %s*%s_as_%s(const %s*o){"
////					"return(%s*)&o->%s;"
////					"}\n",s->data,c->name.data,s->data,c->name.data,
////					s->data,s->data
////			);
////			printf(	"inline static const %s*%s_as_const_%s(const %s*o){"
////					"return(const %s*)&o->%s;"
////					"}\n",s->data,c->name.data,s->data,c->name.data,
////					s->data,s->data
////			);
////		}
//		// functions
//		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//		for(unsigned i=0;i<c->funcs.count;i++){
//			ci_func*f=(ci_func*)dynp_get(&c->funcs,i);
//			ci_toc_push_scope(tc,'f',f->name.data);
//			printf("inline static %s %s_%s(%s*o",
//					f->type.data,c->name.data,f->name.data,c->name.data);
//
//			for(unsigned j=0;j<f->args.count;j++){
//				ci_func_arg*a=(ci_func_arg*)dynp_get(&f->args,j);
//				printf(",");
//				printf("%s %s",a->type.data,a->name.data);
//				ci_toc_add_ident(tc,a->type.data,a->name.data);
//			}
//			printf(")");
//			f->code.super.compile((ci_expr*)&f->code,tc);
//			ci_toc_pop_scope(tc);
//		}
//		ci_toc_pop_scope(tc);
//	}
//	printf("\n//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//	printf("int main(int c,char** a){global_main(0,c,a);}\n");
//	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//}

inline static /*gives*/type*toc_parse_class(
		const char**pp,toc*tc,token name){

	type*c=malloc(sizeof(type));
	*c=type_def;
	toc_add_class(tc,c);
	token_setz(&name,&c->name);
//	token_setz(&name,&c->name);
	toc_push_scope(tc,'c',c->name.data);
	if(**pp==':'){
		(*pp)++;
		while(1){
			token extends_name=token_next(pp);
			dynp_add(&c->extends,/*takes*/token_to_str(&extends_name));
			if(**pp=='{'){
				break;
			}else if(**pp==','){
				(*pp)++;
				continue;
			}
			printf("<file> <line:col> expected '{' or ',' followed by "
					"class name");
			exit(1);

		}
	}
	if(**pp!='{'){
		printf("<file> <line:col> expected '{' to open class body");
		exit(1);
	}
	(*pp)++;
	while(1){
		token type=token_next(pp);
		if(token_is_empty(&type)){
			if(**pp!='}'){
				printf("<file> <line:col> expected '}' to close class body\n");
				exit(1);
			}
			(*pp)++;
			break;
		}
		if(**pp=='(' || **pp=='{'){//  player{print{puts "hello"}}
//			(*pp)++;
			toc_parse_func(pp,tc,c,&type);
		}else if(**pp=='=' || **pp==';'){
			token name=token_next(pp);
			toc_parse_field(pp,tc,c,&type,&name);
		}else{
			token nm=token_next(pp);
			if(**pp=='(' || **pp=='{'){//  player{print{puts "hello"}}
				*pp=nm.content;
				toc_parse_func(pp,tc,c,&type);
			}else{
				toc_parse_field(pp,tc,c,&type,&nm);
			}
//			printf("<file> <line:col> expected ';'\n");
//			exit(1);
		}
	}
	toc_pop_scope(tc);
	return/*gives*/c;
}

inline static void toc_compile_to_c(toc*tc){
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("#include<stdlib.h>\n");
	printf("#include<stdio.h>\n");
	printf("typedef char bool;\n");
	printf("#define true 1\n");
	printf("#define false 1\n");
	printf("#define char_def 0\n");
	printf("#define int_def 0\n");
	printf("#define float_def 0.0f\n");
	printf("#define bool_def false\n");
	for(unsigned i=0;i<tc->types.count;i++){
		type*c=dynp_get(&tc->types,i);
		toc_push_scope(tc,'c',c->name.data);
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		printf("typedef struct %s{",c->name.data);
		if(c->extends.count||c->fields.count)
			printf("\n");
		for(unsigned i=0;i<c->extends.count;i++){
			str*s=(str*)dynp_get(&c->extends,i);
			printf("    %s %s;\n",s->data,s->data);
		}
		// fields
		for(unsigned i=0;i<c->fields.count;i++){
			field*f=(field*)dynp_get(&c->fields,i);
			toc_add_ident(tc,f->type.data,f->name.data);
			if(!strcmp(f->type.data,"auto")){
				if(!f->initval){
					printf("<file> <line:col> expected initializer with auto");
					exit(1);
				}
				f->initval->compile(f->initval,tc);
				f->type=f->initval->type;
			}
			printf("    %s %s;\n",f->type.data,f->name.data);
		}
		printf("}%s;\n",c->name.data);

		// #define object_def {...}
		printf("#define %s_def (%s){",c->name.data,c->name.data);
		for(unsigned i=0;i<c->extends.count;i++){
			str*s=(str*)dynp_get(&c->extends,i);
			printf("%s_def",s->data);
			if(i!=c->extends.count-1)
				printf(",");
		}
		for(unsigned i=0;i<c->fields.count;i++){
			field*s=(field*)dynp_get(&c->fields,i);
			if(s->initval){
				s->initval->compile(s->initval,tc);
			}else{
				printf("%s_def",s->type.data);
			}
			if(i!=c->fields.count-1)
				printf(",");
		}
		printf("}\n");
//
//		// casts
////		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//		for(unsigned i=0;i<c->extends.count;i++){
//			str*s=(str*)dynp_get(&c->extends,i);
//			printf(	"inline static %s*%s_as_%s(const %s*o){"
//					"return(%s*)&o->%s;"
//					"}\n",s->data,c->name.data,s->data,c->name.data,
//					s->data,s->data
//			);
//			printf(	"inline static const %s*%s_as_const_%s(const %s*o){"
//					"return(const %s*)&o->%s;"
//					"}\n",s->data,c->name.data,s->data,c->name.data,
//					s->data,s->data
//			);
//		}
		// functions
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		for(unsigned i=0;i<c->funcs.count;i++){
			func*f=(func*)dynp_get(&c->funcs,i);
			toc_push_scope(tc,'f',f->name.data);
			printf("inline static %s %s_%s(%s*o",
					f->type.data,c->name.data,f->name.data,c->name.data);

			for(unsigned j=0;j<f->args.count;j++){
				funcarg*a=(funcarg*)dynp_get(&f->args,j);
				printf(",");
				printf("%s %s",a->type.data,a->name.data);
				toc_add_ident(tc,a->type.data,a->name.data);
			}
			printf(")");
			f->code.super.compile((xexpr*)&f->code,tc);
			toc_pop_scope(tc);
		}
		toc_pop_scope(tc);
	}
	printf("\n//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("int main(int c,char** a){global_main(0,c,a);}\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
}

inline static void toc_compile_file(const char*path){
	str s=str_from_file(path);
	const char*p=s.data;
	toc tc=toc_def;
	while(1){
		token nmspc=token_next(&p);
		if(token_is_empty(&nmspc))break;
		toc_parse_class(&p,&tc,nmspc);
	}
	toc_compile_to_c(&tc);
	str_free(&s);
}


