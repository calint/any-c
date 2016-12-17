#pragma once
#include <setjmp.h>
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

inline static void toc_add_type(toc*o,type*c){
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
		longjmp(_jmpbufenv,1);
	}
}

inline static void ci_init(){}

inline static void ci_free(){}

inline static str const_str(const char*s){
	str st=str_def;
	st.data=s;
	st.cap=st.count=strlen(s)+1;
	return st;
}

inline static /*gives*/xexpr*toc_read_next_xexpr(
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
					longjmp(_jmpbufenv,1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					longjmp(_jmpbufenv,1);
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
			e->super.type=str_from_string("ccharp");
			return(xexpr*)e;
		}else if(**pp=='\''){
			(*pp)++;
			(*pp)++;
			if(**pp!='\''){
				printf("<file> <line:col> expected a character\n"
						" example 'a'\n");
				longjmp(_jmpbufenv,1);
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

	// int
	char*endptr;strtol(tks.data,&endptr,10);
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
		xcont*e=xcont_read_next(pp,tc);
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
			token_equals(&tk,"var")||token_equals(&tk,"ccharp")){//const char
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
			longjmp(_jmpbufenv,1);
		}
	}
	codeblk_read_next(&f->code,pp,tc);
	toc_pop_scope(tc);
}

inline static void toc_parse_field(const char**pp,
		toc*tc,type*c,token*type,token*name){

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
		xexpr*e=toc_read_next_xexpr(pp,tc);
		f->initval=e;
		if(strcmp(f->type.data,"var") && !toc_can_assign(tc,
				f->type.data,f->initval->type.data,e->type.data)){

			printf("<file> <line:col> cannot assign '%s' to '%s'\n",
					e->type.data,f->type.data);
			longjmp(_jmpbufenv,1);
		}
		f->type=e->type;//? assert types
	}
	if(**pp==';'){
		(*pp)++;
	}
	return;
}

inline static /*gives*/type*toc_parse_type(
		const char**pp,toc*tc,token name){

	type*c=malloc(sizeof(type));
	*c=type_def;
	toc_add_type(tc,c);
	token_setz(&name,&c->name);
	toc_push_scope(tc,'c',c->name.data);
	if(**pp!='{'){
		printf("<file> <line:col> expected '{' to open class body");
		longjmp(_jmpbufenv,1);
	}
	(*pp)++;
	while(1){
		token type=token_next(pp);
		if(token_is_empty(&type)){
			if(**pp!='}'){
				printf("<file> <line:col> expected '}' to close class body\n");
				longjmp(_jmpbufenv,1);
			}
			(*pp)++;
			break;
		}
		if(**pp=='(' || **pp=='{'){
			toc_parse_func(pp,tc,c,&type);
		}else if(**pp=='=' || **pp==';'){
			token name=token_next(pp);
			toc_parse_field(pp,tc,c,&type,&name);
		}else{
			token nm=token_next(pp);
			if(**pp=='(' || **pp=='{'){
				*pp=nm.content;
				toc_parse_func(pp,tc,c,&type);
			}else{
				toc_parse_field(pp,tc,c,&type,&nm);
			}
		}
	}
	toc_pop_scope(tc);
	return c;
}

inline static void _print_right_aligned_comment(const char*comment){

	const char*line="--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - -- ---";
	const int maxlen=strlen(line);
	const int ln=strlen(comment);
	int start_at=maxlen-ln-4;
	if(start_at<0)start_at=0;
	printf("//");
	printf("%.*s %s\n",start_at,line,comment);
}

inline static void toc_compile_to_c(toc*tc){
	_print_right_aligned_comment("generated c source");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("#include<stdlib.h>\n");
	printf("#include<stdio.h>\n");
	printf("typedef char bool;\n");
	printf("typedef const char* ccharp;\n");
	printf("#define true 1\n");
	printf("#define false 1\n");
	printf("#define char_def 0\n");
	printf("#define int_def 0\n");
	printf("#define float_def 0.0f\n");
	printf("#define bool_def false\n");
	for(unsigned i=0;i<tc->types.count;i++){
		type*c=dynp_get(&tc->types,i);
		toc_push_scope(tc,'c',c->name.data);
		// type
		_print_right_aligned_comment(c->name.data);
		printf("typedef struct %s{",c->name.data);
		// fields
		if(c->fields.count)printf("\n");
		for(unsigned i=0;i<c->fields.count;i++){
			field*f=(field*)dynp_get(&c->fields,i);
			toc_add_ident(tc,f->type.data,f->name.data);
			if(!strcmp(f->type.data,"auto")){
				if(!f->initval){
					printf("<file> <line:col> expected initializer with auto");
					longjmp(_jmpbufenv,1);
				}
				f->initval->compile(f->initval,tc);
				f->type=f->initval->type;
			}
			printf("    %s %s;\n",f->type.data,f->name.data);
		}
		printf("}%s;\n",c->name.data);

		// #define object_def {...}
		printf("#define %s_def (%s){",c->name.data,c->name.data);
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

		// functions
		if(c->funcs.count){
			_print_right_aligned_comment("funcs");
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
		}
		toc_pop_scope(tc);
	}
	printf("\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("int main(int c,char** a){global_main(0,c,a);}\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
}

inline static void toc_compile_file(const char*path){
	int val=setjmp(_jmpbufenv);
	if(val==1){
		printf(" * error occured ");
		return;
	}

	toc tc=toc_def;
	tc.src=str_from_file(path);
	tc.srcp=tc.src.data;
	while(1){
		token nmspc=token_next(&tc.srcp);
		if(token_is_empty(&nmspc))break;
		toc_parse_type(&tc.srcp,&tc,nmspc);
	}

	toc_compile_to_c(&tc);

	// free toc cascading
}


