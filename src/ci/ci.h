#pragma once
#include"../lib.h"
#include "block.h"
#include "class.h"
#include "expr.h"
#include "expr_call.h"
#include "expr_ident.h"
#include "expr_assign.h"
#include "expr_loop.h"
#include "expr_break.h"
#include "expr_continue.h"
#include "expr_var.h"
#include "expr_ife.h"

dynp/*owns*/ci_classes=dynp_def;

inline static ci_class*ci_find_class_by_name(const char*name){
	for(unsigned i=0;i<ci_classes.count;i++){
		ci_class*c=dynp_get(&ci_classes,i);
		if(!strcmp(c->name.data,name)){
			return c;
		}
	}
	return NULL;
}

//inline static ci_field*ci_class_find_field_for_name(const ci_class*o,
//		const char*nm){
//
//	for(unsigned i=0;i<o->extends.count;i++){
//		str*ext=(str*)dynp_get(&o->extends,i);
//		if(!strcmp(ext->name.data,nm))
//			return ext
//	}
//}

inline static bool ci_is_assignable_from(ci_toc*tc,
		const char*dst,const char*path,const char*src){

	ci_class*c=ci_find_class_by_name(dst);
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
				c=ci_find_class_by_name(ext->data);
				found=true;
				break;
			}
		}
		if(found)continue;
		for(unsigned i=0;i<c->fields.count;i++){
			ci_field*fld=(ci_field*)dynp_get(&c->fields,i);
			if(!strcmp(fld->name.data,ident.data)){
				c=ci_find_class_by_name(fld->type.data);
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

inline static str str_const(const char*s){
	str st=str_def;
	st.data=s;
	st.cap=st.count=strlen(s)+1;
	return st;
}
inline static /*gives*/ci_expr*_ci_expr_new_from_pp(
		const char**pp,ci_toc*tc){

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
			ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
			*e=ci_expr_ident_def;
			str name=str_def;
			token_setz(&tk,&name);
			e->name=name;
			e->super.type=str_from_string("const char*");
			return(ci_expr*)e;
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
			ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
			*e=ci_expr_ident_def;
			str name=str_def;
			token_setz(&tk,&name);
			e->name=name;
			e->super.type=str_from_string("char");
			return(ci_expr*)e;
		}else{
			ci_expr*e=malloc(sizeof(ci_expr));
			*e=ci_expr_def;
			return e;
		}
	}


	// constant
	str tks=str_def;
	token_setz(&tk,&tks);

	// boolean
	if(!strcmp("true",tks.data) || !strcmp("false",tks.data)){
		ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
		*e=ci_expr_ident_def;
		e->name=tks;
		e->super.type=str_const("bool");
		return(ci_expr*)e;
	}


	char*endptr;
	// int
	strtol(tks.data,&endptr,10);
	if(endptr==tks.data+tks.count-1){
		ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
		*e=ci_expr_ident_def;
		e->name=tks;
		e->super.type=str_const("int");
		return(ci_expr*)e;
	}

	// float
	strtof(tks.data,&endptr);
	if(*endptr=='f' && tks.count>2){
		ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
		*e=ci_expr_ident_def;
		e->name=tks;
		e->super.type=str_const("float");
		return(ci_expr*)e;
	}
//

//
	// hex
	if(tks.count>1){
		if((tks.data[0]=='0' && tks.data[1]=='x')){
			strtol(tks.data+2,&endptr,16);
			if(endptr==tks.data+tks.count-2+1){
				ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
				*e=ci_expr_ident_def;
				e->name=tks;
				e->super.type=str_const("int");
				return(ci_expr*)e;
			}
		}
		if((tks.data[0]=='0' && tks.data[1]=='b')){
			strtol(tks.data+2,&endptr,2);
			if(endptr==tks.data+tks.count-2+1){
				ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
				*e=ci_expr_ident_def;
				e->name=tks;
				e->super.type=str_const("int");
				return(ci_expr*)e;
			}
		}
	}

	if(token_equals(&tk,"loop")){
		ci_expr_loop*e=ci_expr_loop_next(pp,tc);
		return (ci_expr*)e;
	}

	if(token_equals(&tk,"break")){
		ci_expr_break*e=ci_expr_break_next(pp,tc);
		return (ci_expr*)e;
	}

	if(token_equals(&tk,"continue")){
		ci_expr_continue*e=ci_expr_continue_next(pp,tc);
		return (ci_expr*)e;
	}

	if(token_equals(&tk,"if")){
		ci_expr_ife*e=ci_expr_ife_next(pp,tc);
		return (ci_expr*)e;
	}

	str name=str_def;
	token_setz(&tk,&name);
	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
			token_equals(&tk,"bool")||token_equals(&tk,"char")||
			token_equals(&tk,"auto")||token_equals(&tk,"var")){
		ci_expr_var*e=ci_expr_var_next(pp,tc,name);
		return(ci_expr*)e;
	}

	ci_class*c=ci_find_class_by_name(name.data);
	if(c){// instantiate
		ci_expr_var*e=ci_expr_var_next(pp,tc,name);
		return(ci_expr*)e;
	}

	if(**pp=='('){// function call
		ci_expr_call*e=ci_expr_call_next(pp,tc,/*gives*/name);
		return(ci_expr*)e;
	}

	if(**pp=='='){// assignment
		(*pp)++;
		if(**pp!='='){
			ci_expr_assign*e=/*takes*/ci_expr_assign_next(pp,tc,/*gives*/name);
			return(ci_expr*)e;
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

	ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
	*e=ci_expr_ident_def;
	e->name=name;
	e->incdecbits=incdecbits;
	return(ci_expr*)e;
}
//inline static str ci_abbreviate_func_arg_name_for_type(ci_toc*tc,const char*tp){
//	return *tp;
//}
inline static void _ci_parse_func(const char**pp,ci_toc*tc,ci_class*c,
		token*type){
	ci_func*f=malloc(sizeof(ci_func));
	*f=ci_func_def;
	bool enclosed_args=false;
	if(**pp=='{' || **pp=='('){
		f->type=str_const("void");
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

	ci_toc_push_scope(tc,'f',f->name.data);
	while(1){
		token tkt=token_next(pp);
		if(token_is_empty(&tkt)){
			break;
		}
		ci_func_arg*fa=malloc(sizeof(ci_func_arg));
		dynp_add(&f->args,fa);
		*fa=ci_func_arg_def;
		token tkn=token_next(pp);
		token_setz(&tkt,&fa->type);
		token_setz(&tkn,&fa->name);

		ci_toc_add_ident(tc,fa->type.data,fa->name.data);

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
	ci_block_parse(&f->code,pp,tc);
	ci_toc_pop_scope(tc);
}

inline static void _ci_parse_field(const char**pp,ci_toc*tc,ci_class*c,
		token*type,token*name){

	ci_field*f=malloc(sizeof(ci_field));
	*f=ci_field_def;
	token_setz(type,&f->type);
	if(token_is_empty(name)){
		token_setz(type,&f->name);
	}else{
		token_setz(name,&f->name);
	}
	dynp_add(&c->fields,f);
	ci_toc_add_ident(tc,f->type.data,f->name.data);
	if(**pp=='='){
		(*pp)++;
		ci_expr*e=_ci_expr_new_from_pp(pp,tc);
		f->initval=e;
		if(strcmp(f->type.data,"var") && !ci_is_assignable_from(tc,
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

inline static void _ci_compile_to_c(ci_toc*tc){
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
	for(unsigned i=0;i<ci_classes.count;i++){
		ci_class*c=(ci_class*)dynp_get(&ci_classes,i);
		ci_toc_push_scope(tc,'c',c->name.data);
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
			ci_field*f=(ci_field*)dynp_get(&c->fields,i);
			ci_toc_add_ident(tc,f->type.data,f->name.data);
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
			printf("%s_def,",s->data);
		}
		for(unsigned i=0;i<c->fields.count;i++){
			ci_field*s=(ci_field*)dynp_get(&c->fields,i);
			if(s->initval){
				s->initval->compile(s->initval,tc);
			}else{
				printf("%s_def",s->type.data);
			}
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
			ci_func*f=(ci_func*)dynp_get(&c->funcs,i);
			ci_toc_push_scope(tc,'f',f->name.data);
			printf("inline static %s %s_%s(%s*o",
					f->type.data,c->name.data,f->name.data,c->name.data);

			for(unsigned j=0;j<f->args.count;j++){
				ci_func_arg*a=(ci_func_arg*)dynp_get(&f->args,j);
				printf(",");
				printf("%s %s",a->type.data,a->name.data);
				ci_toc_add_ident(tc,a->type.data,a->name.data);
			}
			printf(")");
			f->code.super.compile((ci_expr*)&f->code,tc);
			ci_toc_pop_scope(tc);
		}
		ci_toc_pop_scope(tc);
	}
	printf("\n//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("int main(int c,char** a){global_main(0,c,a);}\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
}

inline static /*gives*/ci_class*_ci_parse_class(
		const char**pp,ci_toc*tc,token name){

	ci_class*c=malloc(sizeof(ci_class));
	*c=ci_class_def;
	dynp_add(&ci_classes,c);
	token_setz(&name,&c->name);
	ci_toc_push_scope(tc,'c',c->name.data);
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
			_ci_parse_func(pp,tc,c,&type);
		}else if(**pp=='=' || **pp==';'){
			token name=token_next(pp);
			_ci_parse_field(pp,tc,c,&type,&name);
		}else{
			token nm=token_next(pp);
			if(**pp=='(' || **pp=='{'){//  player{print{puts "hello"}}
				*pp=nm.content;
				_ci_parse_func(pp,tc,c,&type);
			}else{
				_ci_parse_field(pp,tc,c,&type,&nm);
			}
//			printf("<file> <line:col> expected ';'\n");
//			exit(1);
		}
	}
	ci_toc_pop_scope(tc);
	return/*gives*/c;
}
inline static void ci_compile_file(const char*path){
	str s=str_from_file(path);
	const char*p=s.data;
	ci_toc tc=ci_toc_def;
	while(1){
		token nmspc=token_next(&p);
		if(token_is_empty(&nmspc))break;
		_ci_parse_class(&p,&tc,nmspc);
	}
	_ci_compile_to_c(&tc);
}

