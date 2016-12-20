#pragma once
#include<setjmp.h>
#include"xcall.h"
#include"xident.h"
#include"xloop.h"
#include"xvar.h"
#include"xconst.h"
#include"xexpls.h"
#include"xreturn.h"
#include"xbreak.h"
#include"xcode.h"
#include"xcont.h"
#include"xife.h"
#include"xtype.h"

typedef struct ci{
	dynp types;
}ci;
#define ci_def {dynp_def}

inline static xtype*ci_get_type_by_name(const toc*o,cstr name){
	for(unsigned i=0;i<o->types.count;i++){
		xtype*c=dynp_get(&o->types,i);
		if(!strcmp(c->name,name)){
			return c;
		}
	}
	return NULL;
}

inline static cstr ci_get_type_for_accessor(const toc*tc,
						cstr accessor,token tk){

	cstr current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	cstr current_class_name=decl->type;
	const xtype*current_type=ci_get_type_by_name(tc,current_class_name);
	if(current_type){
		while(1){
			cstr p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			cstr lookup=current_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,current_accessor,p-current_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_by_name(current_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						current_accessor,
						current_type->name,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			current_class_name=fld->type;
			current_type=ci_get_type_by_name(tc,current_class_name);
			if(!current_type)
				break;
		}
	}
	return current_class_name;
}

inline static void ci_assert_set(const toc*tc,
		cstr accessor,cstr settype,token tk){

	cstr current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	if(!strcmp(decl->type,"var"))// if dest is var
		return;

	cstr current_class_name=decl->type;
	const xtype*current_type=ci_get_type_by_name(tc,current_class_name);
	if(current_type){
		while(1){
			cstr p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			cstr lookup=current_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,current_accessor,p-current_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_by_name(current_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						current_accessor,
						current_type->name,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			current_class_name=fld->type;
			current_type=ci_get_type_by_name(tc,current_class_name);
			if(!current_type)
				break;
		}
	}

	if(!current_type)// builtin
		if(toc_is_type_builtin(tc,current_class_name))
			if(!strcmp(current_class_name,settype))
				return;

	if(!strcmp(settype,current_class_name))
		return;

	toc_print_source_location(tc,tk,4);
	printf("cannot set '%s' to '%s', %s is %s",
			settype,current_class_name,
			accessor,current_class_name
	);
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}

inline static void ci_init(){}

inline static void ci_free(){}

inline static xexp*ci_read_next_statement(toc*tc){
	token tk=toc_next_token(tc);
	if(token_is_empty(&tk)){
		if(toc_srcp_is_take(tc,'"')){ // string
			while(1){
				const char c=*tc->srcp;
				if(c==0){
					printf("<file> <line:col> did not find the "
							" end of string\n");
					longjmp(_jmp_buf,1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					longjmp(_jmp_buf,1);
				}
				tc->srcp++;
				if(c=='\\'){
					tc->srcp++;
					continue;
				}
				if(c=='"')break;
			}
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);
			e->super.type="cstr";
			return(xexp*)e;

		}else if(toc_srcp_is_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(*tc->srcp!='\''){
				toc_print_source_location(tc,tk,4);
				printf("expected a character, example 'a'");
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			toc_srcp_inc(tc);
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);
			e->super.type="char";
			return(xexp*)e;

		}else{
			return null;
//			xexp*e=malloc(sizeof(xexp));
//			*e=xexp_def;
//			return e;
		}
	}

	// constant
	cstr tks=token_to_new_cstr(&tk);

	// boolean
	if(!strcmp("true",tks) || !strcmp("false",tks)){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="bool";
		return(xexp*)e;
	}

	// int
	char*endptr;strtol(tks,&endptr,10);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="int";
		return(xexp*)e;
	}

	// float
	strtof(tks,&endptr);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="float";
		return(xexp*)e;
	}

	// hex
	const unsigned tkslen=strlen(tks);
	if(tkslen>1){
		if((tks[0]=='0' && tks[1]=='x')){
			strtol(tks+2,&endptr,16);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
		if((tks[0]=='0' && tks[1]=='b')){
			strtol(tks+2,&endptr,2);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
	}

	// keywords
	if(token_equals(&tk,"loop")){
		xloop*e=xloop_read_next(tc);
		e->super.token=tk;
		return (xexp*)e;
	}

	if(token_equals(&tk,"break")){
		xbreak*e=xbreak_read_next(tc);
		e->super.token=tk;
		return (xexp*)e;
	}

	if(token_equals(&tk,"continue")){
		xcont*e=xcont_read_next(tc,tk);
		return (xexp*)e;
	}

	if(token_equals(&tk,"if")){
		xife*e=xife_read_next(tc,tk);
		return (xexp*)e;
	}

	if(token_equals(&tk,"return")){
		xreturn*e=xreturn_read_next(tc,tk);
		return (xexp*)e;
	}

	// built in types
	cstr name=token_to_new_cstr(&tk);//? leak
	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
			token_equals(&tk,"bool")||token_equals(&tk,"char")||
			token_equals(&tk,"var")||token_equals(&tk,"ccharp")){//const char*
		xvar*e=xvar_read_next(tc,name);
		return(xexp*)e;
	}

	//  class instance
	xtype*c=ci_get_type_by_name(tc,name);
	if(c){// instantiate
		xvar*e=xvar_read_next(tc,name);
		return(xexp*)e;
	}

	// function call
	if(*tc->srcp=='('){
		xcall*e=xcall_read_next(tc,tk,/*gives*/name);
		return(xexp*)e;
	}

	// assignment
	if(*tc->srcp=='='){
		tc->srcp++;
		if(*tc->srcp!='='){
			xset*e=xset_read_next(tc,name,tk);
			return(xexp*)e;
		}
		tc->srcp--;
	}

	char incdecbits=0;
	if(*tc->srcp=='+'){
		tc->srcp++;
		if(*tc->srcp=='+'){
			tc->srcp++;
			incdecbits|=1;
		}else{
			tc->srcp--;
			tc->srcp--;
		}
	}else if(*tc->srcp=='-'){
		tc->srcp++;
		if(*tc->srcp=='-'){
			tc->srcp++;
			incdecbits|=2;
		}else{
			tc->srcp--;
			tc->srcp--;
		}
	}

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=ci_get_type_for_accessor(tc,e->name,tk);
	return(xexp*)e;
}

inline static xexp*ci_read_next_expression(toc*tc){
	token tk=toc_next_token(tc);
	if(token_is_empty(&tk)){
		if(toc_srcp_is_take(tc,'"')){ // string
			while(1){
				const char c=*tc->srcp;
				if(c==0){
					printf("<file> <line:col> did not find the "
							" end of string\n");
					longjmp(_jmp_buf,1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					longjmp(_jmp_buf,1);
				}
				tc->srcp++;
				if(c=='\\'){
					tc->srcp++;
					continue;
				}
				if(c=='"')break;
			}
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);//? leak
//			token_setz(&tk,&e->name);
			e->super.type="cstr";
			return(xexp*)e;

		}else if(toc_srcp_is_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(*tc->srcp!='\''){
				toc_print_source_location(tc,tk,4);
				printf("expected a character, example 'a'");
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			toc_srcp_inc(tc);
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);//? leak
//			token_setz(&tk,&e->name);
			e->super.type="char";
			return(xexp*)e;

		}else{
			xexp*e=malloc(sizeof(xexp));
			*e=xexp_def;
			return e;
		}
	}

	// constant
	cstr tks=token_to_new_cstr(&tk);//? leak
	const unsigned tkslen=strlen(tks);

	// boolean
	if(!strcmp("true",tks) || !strcmp("false",tks)){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="bool";
		return(xexp*)e;
	}

	// int
	char*endptr;strtol(tks,&endptr,10);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="int";
		return(xexp*)e;
	}

	// float
	strtof(tks,&endptr);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="float";
		return(xexp*)e;
	}

	// hex
	if(tkslen>1){
		if(tks[0]=='0' && tks[1]=='x'){
			strtol(tks+2,&endptr,16);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
		if(tks[0]=='0' && tks[1]=='b'){
			strtol(tks+2,&endptr,2);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
	}
//
//	// keywords
//	if(token_equals(&tk,"loop")){
//		xloop*e=xloop_read_next(tc);
//		e->super.token=tk;
//		return (xexpr*)e;
//	}
//
//	if(token_equals(&tk,"break")){
//		xbreak*e=xbreak_read_next(tc);
//		e->super.token=tk;
//		return (xexpr*)e;
//	}
//
//	if(token_equals(&tk,"continue")){
//		xcont*e=xcont_read_next(tc,tk);
//		return (xexpr*)e;
//	}
//
//	if(token_equals(&tk,"if")){
//		xife*e=xife_read_next(tc,tk);
//		return (xexpr*)e;
//	}

	// built in types
	cstr name=token_to_new_cstr(&tk); //? leak
//	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
//			token_equals(&tk,"bool")||token_equals(&tk,"char")||
//			token_equals(&tk,"var")||token_equals(&tk,"ccharp")){//const char*
//		xvar*e=xvar_read_next(tc,name);
//		return(xexpr*)e;
//	}

	//  class instance
//	type*c=toc_get_type_by_name(tc,name.data);
//	if(c){// instantiate
//		xvar*e=xvar_read_next(tc,name);
//		return(xexpr*)e;
//	}

	// function call
	if(*tc->srcp=='('){
		xcall*e=xcall_read_next(tc,tk,name);
		return(xexp*)e;
	}

	// assignment
//	if(*tc->srcp=='='){
//		tc->srcp++;
//		if(*tc->srcp!='='){
//			xset*e=xset_read_next(tc,name,tk);
//			return(xexpr*)e;
//		}
//		tc->srcp--;
//	}

	char incdecbits=0;
	if(toc_srcp_is_take(tc,'+')){
		if(toc_srcp_is_take(tc,'+')){
			incdecbits|=1;
		}else{
			tc->srcp--;
		}
	}else if(toc_srcp_is_take(tc,'-')){
		if(toc_srcp_is_take(tc,'-')){
			incdecbits|=2;
		}else{
			tc->srcp--;
		}
	}

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=ci_get_type_for_accessor(tc,e->name,tk);
	return(xexp*)e;
}

inline static void ci_parse_func(toc*tc,xtype*c,token*type){
	xfunc*f=malloc(sizeof(xfunc));
	*f=xfunc_def;
	bool enclosed_args=false;
	if(*tc->srcp=='{' || *tc->srcp=='('){
		f->type="void";
		f->name=token_to_new_cstr(type);//? leak
	}else{
		token tkname=toc_next_token(tc);
		f->type=token_to_new_cstr(type);//? leak
		f->name=token_to_new_cstr(&tkname);//? leak
	}

	dynp_add(&c->funcs,f);

	if(*tc->srcp=='('){
		enclosed_args=true;
		tc->srcp++;
	}

	toc_push_scope(tc,'f',f->name);
	while(1){
		token tkt=toc_next_token(tc);
		if(token_is_empty(&tkt)){
			break;
		}
		xfuncarg*fa=malloc(sizeof(xfuncarg));
		dynp_add(&f->funcargs,fa);
		*fa=xfuncarg_def;
		token tkn=toc_next_token(tc);
		fa->type=token_to_new_cstr(&tkt);//? leak
		fa->name=token_to_new_cstr(&tkn);//? leak

		toc_add_declaration(tc,fa->type,fa->name);

		if(*tc->srcp==','){
			tc->srcp++;
		}
	}
	if(enclosed_args){
		if(*tc->srcp==')'){
			tc->srcp++;
		}else{
			printf("<file> <line:col> expected ')' after arguments\n");
			longjmp(_jmp_buf,1);
		}
	}
	xcode_read_next(&f->code,tc);
	toc_pop_scope(tc);
}

inline static void ci_parse_field(toc*tc,xtype*c,token*type,token*name){
	xfield*f=malloc(sizeof(xfield));
	*f=xfield_def;
	f->type=token_to_new_cstr(type);//? leak
//	token_setz(type,&f->type);
	if(token_is_empty(name)){
		f->name=f->type;
	}else{
		f->name=token_to_new_cstr(name);//? leak
	}
	dynp_add(&c->fields,f);
	toc_add_declaration(tc,f->type,f->name);
	if(*tc->srcp=='='){
		tc->srcp++;
		xexpls_parse_next(&f->initval, tc,*type);
//		f->initval=e;
		if(strcmp(f->type,"var")){
				ci_assert_set(tc,
					f->name,
					f->initval.super.type,
					f->initval.super.token);
		}
		f->type=f->initval.super.type;
	}
	if(*tc->srcp==';'){
		tc->srcp++;
	}
	return;
}

inline static xtype*ci_parse_type(toc*tc,token name){
	xtype*c=malloc(sizeof(xtype));
	*c=xtype_def;
	dynp_add(&tc->types,c);
	c->token=name;
	c->name=token_to_new_cstr(&c->token);//? leak
	toc_push_scope(tc,'c',c->name);
	if(!toc_srcp_is(tc,'{')){
		toc_print_source_location(tc,c->token,4);
		printf("expected '{' to open class body\n");
		longjmp(_jmp_buf,1);
	}
	toc_srcp_inc(tc);
	while(1){
		token type=toc_next_token(tc);
		if(token_is_empty(&type)){
			if(*tc->srcp!='}'){
				printf("<file> <line:col> expected '}' to close class body\n");
				longjmp(_jmp_buf,1);
			}
			tc->srcp++;
			break;
		}
		if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
			ci_parse_func(tc,c,&type);
		}else if(toc_srcp_is(tc,'=') || toc_srcp_is(tc,';')){
			token name=toc_next_token(tc);
			ci_parse_field(tc,c,&type,&name);
		}else{
			token name=toc_next_token(tc);
			if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
				tc->srcp=name.content;
				ci_parse_func(tc,c,&type);
			}else{
				ci_parse_field(tc,c,&type,&name);
			}
		}
	}
	toc_pop_scope(tc);
	return c;
}

inline static void ci_print_right_aligned_comment(cstr comment){
	cstr line="--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - -- ---";
	const int maxlen=strlen(line);
	const int ln=strlen(comment);
	int start_at=maxlen-ln-4;
	if(start_at<0)start_at=0;
	printf("//");
	printf("%.*s %s\n",start_at,line,comment);
}

inline static void ci_compile_to_c(toc*tc){
	ci_print_right_aligned_comment("generated c source");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("#include<stdlib.h>\n");
	printf("#include<stdio.h>\n");
	printf("typedef char bool;\n");
	printf("typedef const char* cstr;\n");
	printf("#define true 1\n");
	printf("#define false 1\n");
	printf("#define char_def 0\n");
//	printf("#define var_def 0\n");
	printf("#define int_def 0\n");
	printf("#define float_def 0.0f\n");
	printf("#define bool_def false\n");
	for(unsigned i=0;i<tc->types.count;i++){
		xtype*c=dynp_get(&tc->types,i);
		toc_push_scope(tc,'c',c->name);
		// type
		ci_print_right_aligned_comment(c->name);
		printf("typedef struct %s{",c->name);
		// fields
		if(c->fields.count)printf("\n");
		for(unsigned i=0;i<c->fields.count;i++){
			xfield*f=(xfield*)dynp_get(&c->fields,i);
			toc_add_declaration(tc,f->type,f->name);
			if(!strcmp(f->type,"var")){
				if(!f->initval.exps.count){
					printf("<file> <line:col> expected initializer with auto");
					longjmp(_jmp_buf,1);
				}
				f->initval.super.compile((xexp*)&f->initval,tc);
				f->type=f->initval.super.type;
			}
			printf("    %s %s;\n",f->type,f->name);
		}
		printf("}%s;\n",c->name);

		// #define object_def {...}
		printf("#define %s_def (%s){",c->name,c->name);
		for(unsigned i=0;i<c->fields.count;i++){
			xfield*s=(xfield*)dynp_get(&c->fields,i);
			if(s->initval.exps.count){
				s->initval.super.compile((xexp*)&s->initval,tc);
			}else{
				printf("%s_def",s->type);
			}
			if(i!=c->fields.count-1)
				printf(",");
		}
		printf("}\n");

		// functions
		if(c->funcs.count){
			ci_print_right_aligned_comment("funcs");
			for(unsigned i=0;i<c->funcs.count;i++){
				xfunc*f=(xfunc*)dynp_get(&c->funcs,i);
				toc_push_scope(tc,'f',f->name);
				printf("inline static %s %s_%s(%s*o",
						f->type,c->name,f->name,c->name);
				for(unsigned j=0;j<f->funcargs.count;j++){
					xfuncarg*a=(xfuncarg*)dynp_get(&f->funcargs,j);
					printf(",");
					printf("%s %s",a->type,a->name);
					toc_add_declaration(tc,a->type,a->name);
				}
				printf(")");
				f->code.super.compile((xexp*)&f->code,tc);
				printf("\n");
				toc_pop_scope(tc);
			}
		}
		toc_pop_scope(tc);
	}
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("int main(int c,char**a){global g=global_def;global_main(&g);}\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
}

inline static int ci_compile_file(cstr path){
	const int ret=setjmp(_jmp_buf);
	if(ret)
		return ret;
	toc tc=toc_def;
	tc.filepth=path;
	str srcstr=str_from_file(path);//? leak
	tc.srcp=tc.src=srcstr.data;
	while(1){
		token typenm=toc_next_token(&tc);
		if(token_is_empty(&typenm))
			break;
		ci_parse_type(&tc,typenm);
	}
	ci_compile_to_c(&tc);

	//? toc_free
	for(unsigned i=0;i<tc.types.count;i++){
		xtype*t=(xtype*)dynp_get(&tc.types,i);
		xtype_free(t);
		free(t);
	}
	dynp_free(&tc.types);
	dynp_free(&tc.scopes);
	token_free();
	str_free(&srcstr);
	ci_free();
	return 0;
}

inline static void ci_xset_compile(const toc*tc,token tk,cstr id,cstr type){
	cstr p=strpbrk(id,".");
	if(p){
		str sid=str_def;
		str_add_list(&sid,id,p-id);
		str_add(&sid,0);

		const tocdecl*i=toc_get_declaration(tc,sid.data);
		if(!i){
			printf("<file> <line:col> identifier '%s' not found\n",id);
			longjmp(_jmp_buf,1);
		}
		ci_assert_set(tc,id,type,tk);
		const char scopetype=toc_get_declaration_scope_type(tc,sid.data);
		if(scopetype){
			if(scopetype=='c'){// class member
				printf("o->%s",id);
			}else{// local identifier
				printf("%s",id);
			}
			printf("=");
			return;
		}
	}
	const char scopetype=toc_get_declaration_scope_type(tc,id);
	if(scopetype=='c'){// class member
		printf("o->%s=",id);
		return;
	}
	if(scopetype){// local identifier
		printf("%s=",id);
		return;
	}

//	tocloc tl=toc_get_line_number_from_pp(tc, p)

	toc_print_source_location(tc,tk,4);
	printf("could not find var '%s'\n",id);
	longjmp(_jmp_buf,1);
}


#define ci_identifier_maxlen 1024
inline static void ci_xcall_compile(
		toc*tc,token tk,cstr accessor,unsigned argcount){

	if(!strcmp("printf",accessor)){
		printf("%s(",accessor);
		return;
	}
	if(!strcmp("p",accessor)){
		printf("printf(");
		return;
	}
//
	char cb[ci_identifier_maxlen];
	strcpy(cb,accessor);
	const char*path_ptr=cb;
	const char*varnm_ptr=cb;
	const char*funcnm_ptr=strrchr(cb,'.');   // g.gl.draw
	if(funcnm_ptr){                           //
		cb[funcnm_ptr-cb]=0;
//		*funcnm_ptr=0;                        // path: g.gl
		funcnm_ptr++;                         // func: print
		cstr target_type=ci_get_type_for_accessor(tc,varnm_ptr,tk);
		const char scope=toc_get_declaration_scope_type(tc,varnm_ptr);
		printf("%s_%s((%s*)&",target_type,funcnm_ptr,target_type);
		if(scope=='c'){
			printf("o->%s",path_ptr);
			if(argcount)
				printf(",");
			return;
		}
		char*first_dot=strchr(cb,'.'); // g.gl
		if(first_dot){
			*first_dot=0;                        // var: g
			path_ptr=first_dot+1;
			printf("%s.%s",varnm_ptr,path_ptr);
			if(argcount)
				printf(",");
			return;
		}
		printf("%s",varnm_ptr);
		if(argcount)
			printf(",");
		return;
	}
	funcnm_ptr=cb;       // func: draw
	cstr target_type=toc_get_type_in_context(tc,tk);
	printf("%s_%s((%s*)&o",
		target_type,
		funcnm_ptr,
		target_type
	);
	if(argcount)
		printf(",");
}

