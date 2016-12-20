#pragma once
#include"xexpls.h"
#include"xset.h"

typedef struct xvar{
	xexp super;
	cstr name;
	xset initval;
}xvar;

inline static void _xvar_compile_(const xexp*oo,toc*tc){
	xvar*o=(xvar*)oo;
	const char idtype=toc_get_declaration_scope_type(tc,o->name);
	if(idtype){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' is already declared at ...",o->name);
		longjmp(_jmp_buf,1);
		return;
	}

	printf("%s ",o->super.type);
	toc_add_declaration(tc,o->super.type,o->name);
	if(o->initval.super.compile){
		_xset_compile_((xexp*)&o->initval,tc);
	}else{
		printf("%s=%s_def",o->name,o->super.type);
	}
}

#define xvar_def (xvar){{_xvar_compile_,NULL,cstr_def,token_def,0},\
						cstr_def,xset_def}

inline static xvar*xvar_read_next(toc*tc,cstr type){
	xvar*o=malloc(sizeof(xvar));
	*o=xvar_def;

	o->super.type=type;
	o->super.token=toc_next_token(tc);
	o->name=token_to_new_cstr(&o->super.token);//? leak

	if(!strcmp("o",o->name)){
		toc_print_source_location(tc,o->super.token,4);
		printf("identifier 'o' is reserved, equivalent of 'this'");
		longjmp(_jmp_buf,1);
	}

	toc_add_declaration(tc,o->super.type,o->name);
	bool is_var=!strcmp(o->super.type,"var");
	if(toc_srcp_is_take(tc,'=')){
		xset_parse_next(&o->initval,tc,o->name,o->super.token);
		if(is_var){
			o->super.type=o->initval.super.type;
			toc_set_declaration_type(tc,o->name,o->super.type);
		}else{
			if(strcmp(o->super.type,o->initval.super.type)){
				printf("\n\n<file> <line:col> %s declared as %s but initial"
						" expression is of type %s\n",
						o->initval.name,
						o->super.type,
						o->initval.super.type);
				longjmp(_jmp_buf,1);
			}
		}
	}else{
		o->initval.super.compile=NULL;
	}
	return o;
}
