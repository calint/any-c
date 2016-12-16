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

inline static void ci_init(){}

inline static void ci_free(){
	dynp_foa(&ci_classes,{
		ci_class_free((ci_class*)o);
	});
	dynp_free(&ci_classes);
}

inline static /*gives*/ci_expr*ci_expr_new_from_pp(
		const char**pp,ci_toc*tc){

	token tk=token_next(pp);
	if(token_is_empty(&tk)){
		ci_expr*e=malloc(sizeof(ci_expr));
		*e=ci_expr_def;
		return e;
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
	if(token_equals(&tk,"int")){
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

	ci_expr_ident*e=malloc(sizeof(ci_expr_ident));
	*e=ci_expr_ident_def;
	e->name=/*gives*/name;
	return(ci_expr*)e;
}

inline static void _ci_parse_func(const char**pp,ci_toc*tc,ci_class*c,
		token*type,token*name){
	ci_func*f=malloc(sizeof(ci_func));
	*f=ci_func_def;
	token_setz(type,&f->type);
	token_setz(name,&f->name);
	dynp_add(&c->funcs,f);

	ci_toc_push_scope(tc,'f',f->name.data);
	while(1){
		token argtype=token_next(pp);
		if(token_is_empty(&argtype)){
			if(**pp==')'){
				(*pp)++;
				break;
			}
			printf("<file> <line:col> expected function arguments or ')'\n");
			exit(1);
		}
		token argname=token_next(pp);
		ci_func_arg*fa=malloc(sizeof(ci_func_arg));
		*fa=ci_func_arg_def;
		dynp_add(&f->args,fa);
		token_setz(&argtype,&fa->type);
		token_setz(&argname,&fa->name);

		ci_toc_add_ident(tc,fa->name.data);

		if(**pp==','){
			(*pp)++;
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
	if(**pp=='='){
		(*pp)++;
		ci_expr*e=ci_expr_new_from_pp(pp,tc);
		f->initval=e;
	}
	if(**pp!=';'){
		printf("<file> <line:col> expected ';' to finish field declaration\n");
		exit(1);
	}
	(*pp)++;
	ci_toc_add_ident(tc,f->name.data);
}

inline static void _ci_compile_to_c(ci_toc*tc){
//	for(unsigned i=0;i<ci_classes.count;i++){
//		ci_class*c=dynp_get(&ci_classes,i);
//		printf("\n");
//		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
//		printf("static typedef struct %s{\n",c->name.data);
//		for(unsigned j=0;j<c->extends.count;j++){
//			str*e=dynp_get(&c->extends,j);
//			printf("    %s %s;\n",e->data,e->data);
//		}
//		for(unsigned j=0;j<c->fields.count;j++){
//			ci_field*f=dynp_get(&c->fields,j);
//			printf("    %s %s;\n",f->type.data,f->name.data);
//		}
//		printf("}%s;\n",c->name.data);
//
//		printf("\n");
//		// #define object_def {...}
//		printf("#define %s_def {",c->name.data);
//		for(unsigned j=0;j<c->extends.count;j++){
//			str*e=dynp_get(&c->extends,j);
//			printf("%s_def,",e->data);
//		}
//		for(unsigned j=0;j<c->fields.count;j++){
//			ci_field*f=dynp_get(&c->fields,j);
//			printf("    %s %s;\n",f->type.data,f->name.data);
//		}
//		printf("}\n");
//	}
//	return;

	dynp_foa(&ci_classes,{
		ci_class*c=o;
		ci_toc_push_scope(tc,'c',c->name.data);
		printf("\n");
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		printf("static typedef struct %s{\n",c->name.data);
		dynp_foa(&c->extends,{
			str*s=o;
			printf("    %s %s;\n",s->data,s->data);
		});

		// fields
		dynp_foa(&c->fields,{
			ci_field*s=o;
			ci_toc_add_ident(tc,s->name.data);
			if(!strcmp(s->type.data,"auto")){
				if(!s->initval){
					printf("<file> <line:col> expected initializer with auto");
					exit(1);
				}
				s->initval->compile(s->initval,tc);
				s->type=s->initval->type;
			}
			printf("    %s %s;\n",s->type.data,s->name.data);
		});
		printf("}%s;\n",c->name.data);

		printf("\n");

		// #define object_def {...}
		printf("#define %s_def {",c->name.data);
		dynp_foa(&c->extends,{
			str*s=o;
			printf("%s_def,",s->data);
		});
		dynp_foa(&c->fields,{
			ci_field*s=o;
			if(s->initval){
				s->initval->compile(s->initval,tc);
			}else{
				printf("%s_def",s->type.data);
			}
			printf(",");
		});
		printf("}\n");

		// init
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		printf(	"inline static void %s_init(%s*o){\n",
				c->name.data,c->name.data);
		dynp_foa(&c->extends,{
			str*s=o;
			printf(	"	%s_init(&o->%s);\n",s->data,s->data);
		});
		printf("}\n");

		// alloc
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		printf(	"inline static %s*%s_alloc(){\n"
				"	%s*p=malloc(sizeof(%s));\n"
				"	*p=%s_def;\n"
				"	%s_init(p);\n"
				"	return p;\n"
				"}\n",
				c->name.data,c->name.data,
				c->name.data,c->name.data,
				c->name.data,
				c->name.data
		);
		// free
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		printf(	"inline static void %s_free(%s*o){\n"
				,c->name.data,c->name.data);

		dynp_foar(&c->extends,{
			str*s=o;
			printf(	"	%s_free(&o->%s);\n",s->data,s->data);
		});
		printf("	free(o);\n");
		printf("}\n");

		// casts
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		dynp_foa(&c->extends,{
			str*s=o;
			printf(	"inline static %s*%s_as_%s(const %s*o){"
					"return(%s*)&o->%s;"
					"}\n",s->data,c->name.data,s->data,c->name.data,
					s->data,s->data
			);
			printf(	"inline static const %s*%s_as_const_%s(const %s*o){"
					"return(const %s*)&o->%s;"
					"}\n",s->data,c->name.data,s->data,c->name.data,
					s->data,s->data
			);
		});

		// functions
		printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
		dynp_foa(&c->funcs,{
			ci_func*f=o;
			ci_toc_push_scope(tc,'f',f->name.data);
			printf("inline static %s %s_%s(",f->type.data,c->name.data,f->name.data);
			printf("%s*o",c->name.data);
			dynp_foa(&f->args,{
				ci_func_arg*a=o;
				printf(",");
				printf("%s %s",a->type.data,a->name.data);
				ci_toc_add_ident(tc,a->name.data);
			});
			printf(")");
			f->code.super.compile((ci_expr*)&f->code,tc);
			ci_toc_pop_scope(tc);
		});
		ci_toc_pop_scope(tc);
	});
	printf("\n//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("\n");
}

inline static /*gives*/ci_class*_ci_parse_class(const char**pp,ci_toc*tc,
		token type,token name){
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
		token name=token_next(pp);
		if(**pp=='('){
			(*pp)++;
			_ci_parse_func(pp,tc,c,&type,&name);
		}else if(**pp=='=' || **pp==';'){
			_ci_parse_field(pp,tc,c,&type,&name);
		}else{
			printf("<file> <line:col> expected ';'\n");
			exit(1);
		}
	}
	ci_toc_pop_scope(tc);
	return/*gives*/c;
}
inline static void ci_compile_file(const char*path){
	str s=str_from_file(path);
	const char*p=s.data;
	ci_toc toc=ci_toc_def;
	while(1){
		token t=token_next(&p);
		if(token_is_empty(&t))
			break;
		if(!token_equals(&t,"class")){
			printf("expected 'class' declaration\n");
			exit(1);
		}
		token nm=token_next(&p);
		_ci_parse_class(&p,&toc,t,nm);
	}
	_ci_compile_to_c(&toc);
}

