#pragma once
#include"xexpls.h"
#include"xset.h"

typedef struct xvar{
	xexp super;
	strc name;
	xset initval;
	bool is_ref;
}xvar;

inline static void _xvar_compile_(const xexp*oo,toc*tc){
	xvar*o=(xvar*)oo;
	printf("%s",o->super.type);
	if(o->is_ref)
		printf("*");
	else
		printf(" ");
	toc_add_declaration(tc,o->super.type,o->is_ref,o->name);
	if(o->initval.super.compile){
		_xset_compile_((xexp*)&o->initval,tc);
	}else{
		printf("%s=%s_def",o->name,o->super.type);
		if(ci_xvar_needs_init(tc,o->super.type)){
			printf(";\n");
			toc_print_indent_for_compile(tc);
			printf("%s_init(%s)",o->super.type,o->name);
		}
	}

}

inline static void _xvar_free_(xexp*oo){
	xvar*o=(xvar*)oo;
	o->initval.super.free((xexp*)&o->initval);
}

inline static void _xvar_print_source_(xexp*oo){
	xvar*o=(xvar*)oo;
	token_print_including_whitespace(&o->super.token);
}

#define xvar_def (xvar){\
	{_xvar_compile_,_xvar_free_,_xvar_print_source_,strc_def,token_def,0,false},\
		strc_def,xset_def,false\
}

inline static xvar*xvar_read_next(toc*tc,strc type){
	xvar*o=malloc(sizeof(xvar));
	*o=xvar_def;
	o->super.type=type;
	if(toc_srcp_is_take(tc,'&'))
		o->is_ref=true;
	o->super.token=toc_next_token(tc);
	o->name=token_content_to_new_strc(&o->super.token);

	if(!strcmp("o",o->name)){
		toc_print_source_location(tc,o->super.token,4);
		printf("identifier 'o' is reserved, equivalent of 'this'");
		longjmp(_jmp_buf,1);
	}

	if(toc_is_declared(tc,o->name)){
		toc_print_source_location(tc,o->super.token,4);
		printf("'%s' is already declared",o->name);
		longjmp(_jmp_buf,1);
	}

	toc_add_declaration(tc,o->super.type,o->is_ref,o->name);
	bool is_var=!strcmp(o->super.type,"var");
	if(toc_srcp_is_take(tc,'=')){
//		o->initval.is_set_ref=o->is_ref;
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
