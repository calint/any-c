#pragma once
#include"xcode.h"
#include"xset.h"

typedef struct xfield{
	xexp super;
	strc type;
	strc name;
	xexpls initval;
	bool is_ref;
}xfield;


inline static void _xfield_free_(xexp*o){
	xfield*oo=(xfield*)o;
//	if(oo->initval)
	if(oo->initval.super.free)
		oo->initval.super.free((xexp*)&oo->initval);
//	_xexpls_free_((xexp*)&oo->initval);
//	free(oo);
}

inline static void _xfield_print_source_(xexp*e){
	xfield*o=(xfield*)e;
	token_print(&o->super.token);
	printf("=");
	o->initval.super.print_source((xexp*)&o->initval);
	fflush(0);
}

#define xfield_def (xfield){\
	{NULL,_xfield_free_,_xfield_print_source_,strc_def,token_def,0,false}\
	,strc_def,strc_def,xexpls_def,false}

typedef struct xfuncparam{
	xexp super;
	strc type;
	strc name;
	bool is_ref;
}xfuncparam;

#define xfuncparam_def (xfuncparam){xexp_def,strc_def,strc_def,false}

typedef struct xfunc{
	xexp super;
	strc type;
	strc name;
	ptrs params;
	xcode code;
	bool is_ref;
}xfunc;


inline static void _xfunc_free_(xexp*e){
	xfunc*o=(xfunc*)e;
	o->code.super.free((xexp*)&o->code);
	const long n=o->params.count;
	for(long i=0;i<n;i++){
		xfuncparam*fp=(xfuncparam*)ptrs_get(&o->params,i);
//		fp->super.free((xexp*)fp);
		free(fp);
	}
	ptrs_free(&o->params);
}

inline static void _xfunc_print_source_(xexp*e){
	xfunc*o=(xfunc*)e;
	token_print(&o->super.token);
	printf("{");

	printf("}");
}

#define xfunc_def (xfunc){\
	{NULL,_xfunc_free_,_xfunc_print_source_,strc_def,token_def,0,false},\
	strc_def,strc_def,ptrs_def,xcode_def,false}

#include"decouple.h"

typedef struct xtype{
	xexp super;
	strc name;
	ptrs fields;
	ptrs funcs;
	ptrs stmts;
	char bits; // bit 1: needs call to free     bit 2: has _free
	           // bit 3: needs call to init     bit 4: has _init
}xtype;

//? inlined does not compile
bool xtype_is_needs_free(const xtype*o){return (o->bits&1)==1;}
void xtype_set_needs_free(xtype*o){o->bits|=1;}

bool xtype_is_has_free(const xtype*o){return (o->bits&2)==2;}
void xtype_set_has_free(xtype*o){o->bits|=2;}

bool xtype_is_needs_init(const xtype*o){return (o->bits&4)==4;}
void xtype_set_needs_init(xtype*o){o->bits|=4;}

bool xtype_is_has_init(const xtype*o){return (o->bits&8)==8;}
void xtype_set_has_init(xtype*o){o->bits|=8;}

inline static void ci_print_right_aligned_comment(strc comment){
	strc line="--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - -- ---";
	const size_t maxlen=strlen(line);
	const size_t ln=strlen(comment);
	long start_at=(long)(maxlen-ln)-4;
	if(start_at<0)start_at=0;
	printf("//");
	printf("%.*s %s\n",(int)start_at,line,comment);
}

inline static void _xtype_compile_(const struct xexp*e,struct toc*tc){
	xtype*c=(xtype*)e;
	toc_push_scope(tc,'c',c->name);
	// type
	ci_print_right_aligned_comment(c->name);
	printf("typedef struct %s{",c->name);
	// fields
	if(c->fields.count)
		printf("\n");
	for(unsigned i=0;i<c->fields.count;i++){
		xfield*f=(xfield*)ptrs_get(&c->fields,i);
		toc_add_declaration(tc,f->type,f->is_ref,f->name);
		if(!strcmp(f->type,"var")){
			if(!f->initval.exps.count){
//					toc_print_source_location(tc,);
				printf("expected initializer for var '%s'",f->name);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			f->initval.super.compile((xexp*)&f->initval,tc);
			f->type=f->initval.super.type;
		}
		printf("    ");
		if(!strcmp(f->type,c->name))
			printf("struct ");
		printf("%s",f->type);
		if(f->is_ref)
			printf("*");
		else
			printf(" ");
		printf("%s;\n",f->name);
	}
	printf("}%s;\n",c->name);

	// #define object_def {...}
	printf("#define %s_def (%s){",c->name,c->name);
	for(unsigned i=0;i<c->fields.count;i++){
		xfield*f=(xfield*)ptrs_get(&c->fields,i);
		if(f->initval.exps.count){
			f->initval.super.compile((xexp*)&f->initval,tc);
		}else{
			if(f->is_ref)
				printf("null");
			else
				printf("%s_def",f->type);
		}
		if(i!=c->fields.count-1)
			printf(",");
	}
	printf("}\n");
	// functions
	if(c->funcs.count){
		ci_print_right_aligned_comment("funcs");
		for(unsigned i=0;i<c->funcs.count;i++){
			xfunc*f=(xfunc*)ptrs_get(&c->funcs,i);
			toc_push_scope(tc,'f',f->name);
			printf("inline static %s",f->type);
			if(f->is_ref)
				printf("*");
			else
				printf(" ");
			printf("%s_%s(%s*o",c->name,f->name,c->name);
			for(unsigned j=0;j<f->params.count;j++){
				xfuncparam*fp=ptrs_get(&f->params,j);
				printf(",");
				printf("%s",fp->type);
				if(fp->is_ref)
					printf("*");
				else
					printf(" ");
				printf("%s",fp->name);
				toc_add_declaration(tc,fp->type,fp->is_ref,fp->name);
			}
			printf(")");
			f->code.super.compile((xexp*)&f->code,tc);
//				printf("\n");
			toc_pop_scope(tc);
		}
	}
	// cascading init
	if(xtype_is_needs_init(c) || !strcmp(c->name,"global")){// needs call to init
		printf("inline static void %s_init(%s*o){",c->name,c->name);
		if(c->fields.count)
			printf("\n");
		for(unsigned i=0;i<c->fields.count;i++){
			xfield*f=ptrs_get(&c->fields,i);
			if(ci_is_builtin_type(f->type))
				continue;
			xtype*cc=ci_get_type_for_name_try(tc,f->type);
			if(xtype_is_needs_init(cc))
				printf("    %s_init(&o->%s);\n",f->type,f->name);
		}
		if(xtype_is_has_init(c))
			printf("    %s__init(o);\n",c->name);
		printf("}\n");
	}
	// cascading free
	if(xtype_is_needs_free(c) || !strcmp(c->name,"global")){// needs call to free
		printf("inline static void %s_free(%s*o){",c->name,c->name);
		if(c->fields.count)
			printf("\n");
		if(xtype_is_has_free(c))
			printf("    %s__free(o);\n",c->name);
		for(long i=c->fields.count-1;i>=0;i--){
			xfield*f=(xfield*)ptrs_get(&c->fields,i);
			if(ci_is_builtin_type(f->type))
				continue;
			xtype*cc=ci_get_type_for_name_try(tc,f->type);
			if(xtype_is_needs_free(cc))
//			if(cc->bits&1) // needs _free?
				printf("    %s_free(&o->%s);\n",f->type,f->name);
		}
		printf("}\n");
	}
	toc_pop_scope(tc);
}

inline static void _xtype_free_(xexp*o){
	xtype*oo=(xtype*)o;
	const long n=oo->stmts.count;
	for(long i=0;i<n;i++){
		xexp*e=(xexp*)ptrs_get(&oo->stmts,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	ptrs_free(&oo->stmts);
	ptrs_free(&oo->funcs);
	ptrs_free(&oo->fields);
}

inline static void _xtype_print_source_(xexp*o){//! TODO
	xtype*oo=(xtype*)o;
	token_print(&oo->super.token);
	printf("{");
	const long n=oo->stmts.count;
	for(long i=0;i<n;i++){
		xexp*e=(xexp*)ptrs_get(&oo->stmts,i);
		if(e->print_source)
			e->print_source(e);
	}
	printf("}");
}

#define xtype_def (xtype){\
	{_xtype_compile_,_xtype_free_,_xtype_print_source_,strc_def,token_def,0,false},\
	strc_def,ptrs_def,ptrs_def,ptrs_def,0}

inline static xfield*xtype_get_field_for_name(const xtype*o,strc field_name){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=ptrs_get(&o->fields,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xtype_get_func_for_name(const xtype*o,strc name){
	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=ptrs_get(&o->funcs,i);
		if(!strcmp(f->name,name))
				return f;
	}
	return NULL;
}

inline static/*gives*/xfunc*xfunc_read_next(toc*tc,xtype*c,bool is_ref,
	token type,token name){
	xfunc*f=malloc(sizeof(xfunc));
	*f=xfunc_def;
	f->is_ref=is_ref;
	bool enclosed_args=false;
	if(toc_srcp_is(tc,'{') || toc_srcp_is(tc,'(')){
		if(token_is_empty(&name)){ // global{main{}}
			f->type="void";
			f->name=token_content_to_new_strc(&type);
			f->super.token=type;
		}else{ // global{int main{}}
			f->type=token_content_to_new_strc(&type);
			f->name=token_content_to_new_strc(&name);
			f->super.token=name;
		}
	}
	if(toc_srcp_is_take(tc,'('))
		enclosed_args=true;
	toc_push_scope(tc,'f',f->name);
	token last_arg_tkn=token_def;
	while(1){
		token tkt=toc_next_token(tc);
		last_arg_tkn=tkt;
		if(token_is_empty(&tkt))
			break;
		xfuncparam*fp=malloc(sizeof(xfuncparam));
		ptrs_add(&f->params,fp);
		*fp=xfuncparam_def;
		if(toc_srcp_is_take(tc,'&'))
			fp->is_ref=true;
		token tkn=toc_next_token(tc);
		fp->type=token_content_to_new_strc(&tkt);
		fp->name=token_content_to_new_strc(&tkn);
		toc_add_declaration(tc,fp->type,fp->is_ref,fp->name);
		toc_srcp_is_take(tc,',');
	}
	if(enclosed_args){
		if(!toc_srcp_is_take(tc,')')){
			toc_print_source_location(tc,last_arg_tkn,4);
			printf("expected ')' after arguments");
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
	}
	ptrs_add(&c->funcs,f);
	xcode_read_next(&f->code,tc);
	toc_pop_scope(tc);
	return f;
}

inline static/*gives*/xfield*xfield_read_next(toc*tc,xtype*c,strc tktype,
	token tkname,bool is_ref){

	xfield*f=malloc(sizeof(xfield));
	*f=xfield_def;
	f->super.token=tkname;
	f->type=tktype;
	f->is_ref=is_ref;
	if(token_is_empty(&tkname))
		f->name=f->type;
	else
		f->name=token_content_to_new_strc(&tkname);

	ptrs_add(&c->fields,f);

	if(toc_srcp_is_take(tc,'=')){
		xexpls_parse_next(&f->initval,tc,tkname,false);
		if(strcmp(f->type,"var")){
			xset tmp=xset_def;
			tmp.name=f->name;
			tmp.super.type=f->initval.super.type;
			tmp.super.token=f->initval.super.token;
			ci_xset_assert(tc,&tmp);
		}
		f->type=f->initval.super.type;
	}
	toc_srcp_is_take(tc,';');
	toc_add_declaration(tc,f->type,f->is_ref,f->name);

	return f;
}

inline static/*gives*/xtype*xtype_read_next(toc*tc,token tk){
	xtype*c=malloc(sizeof(xtype));
	*c=xtype_def;
//	c->token=tk;
	c->super.token=tk;
	c->name=token_content_to_new_strc(&c->super.token);

	ptrs_add(&tc->types,c);
	toc_push_scope(tc,'c',c->name);

	if(!toc_srcp_is(tc,'{')){
		toc_print_source_location(tc,c->super.token,4);
		printf("expected '{' to open class body\n");
		longjmp(_jmp_buf,1);
	}
	toc_srcp_inc(tc);
	while(1){
		token tptk=toc_next_token(tc);
		if(token_is_empty(&tptk)){
			if(!toc_srcp_is(tc,'}')){
				toc_print_source_location(tc,tptk,4);
				printf("expected '}' to close '%s' declared at %d",c->name,0);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			toc_srcp_inc(tc);
			break;
		}

		const bool is_ref=toc_srcp_is_take(tc,'&');

		xexp*e=NULL;
		if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
			e/*takes*/=(xexp*)xfunc_read_next(tc,c,is_ref,tptk,token_def);
		}else if(toc_srcp_is(tc,'=')){// global{id=1}
			e/*takes*/=(xexp*)xfield_read_next(tc,c,"var",tptk,is_ref);
		}else if(toc_srcp_is(tc,';')){// global{tokens;}
			e/*takes*/=(xexp*)xfield_read_next(tc,c,token_content_to_new_strc(&tptk),tptk,is_ref);
		}else{
			token nm=toc_next_token(tc);
			if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
				e/*takes*/=(xexp*)xfunc_read_next(tc,c,is_ref,tptk,nm);
			}else{
				e/*takes*/=(xexp*)xfield_read_next(tc,c,token_content_to_new_strc(&tptk),nm,is_ref);
			}
		}
		ptrs_add(&c->stmts,e);
	}
	for(unsigned i=0;i<c->funcs.count;i++){
		xfunc*f=(xfunc*)ptrs_get(&c->funcs,i);
		if(!strcmp(f->name,"_free")){
			xtype_set_has_free(c);
			xtype_set_needs_free(c);
//			c->bits|=3; // needs free and has _free
			continue;
		}
		if(!strcmp(f->name,"_init")){
			xtype_set_has_init(c);
			xtype_set_needs_init(c);
//			c->bits|=4+8; // needs init and has _init
		}
	}
	for(unsigned i=0;i<c->fields.count;i++){
		xfield*f=(xfield*)ptrs_get(&c->fields,i);
		if(ci_is_builtin_type(f->type))
			continue;
		xtype*t=ci_get_type_for_name_try(tc,f->type);

		if(xtype_is_needs_free(t))
			xtype_set_needs_free(c);

		if(xtype_is_needs_init(t))
			xtype_set_needs_init(c);
//			c->bits|=4;
	}
	toc_pop_scope(tc);
	return c;
}

typedef struct xprg{
	xexp super;
	ptrs stmts;
}xprg;

inline static void _xprg_print_source_(xexp*o){
	xprg*oo=(xprg*)o;
	const long n=oo->stmts.count;
	for(long i=0;i<n;i++){
		xexp*e=(xexp*)ptrs_get(&oo->stmts,i);
		e->print_source(e);
	}
}

inline static void _xprg_compile_(const struct xexp*o,struct toc*tc){
	xprg*oo=(xprg*)o;
	xtype*globl=ci_get_type_for_name_try(tc,"global");
	if(!globl){
//		toc_print_source_location(tc,tk,4);
		printf("expected to find type 'global' containing function 'main' to start the program");
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	xfunc*mainfn=xtype_get_func_for_name(globl,"main");
	if(!mainfn){
//		toc_print_source_location(tc,tk,4);
		printf("expected to find function 'main' in type 'global' to start the program");
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}


	ci_print_right_aligned_comment("tidy salami");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("#include<stdlib.h>\n");
	printf("#include<stdio.h>\n");
	printf("typedef const char*strc;\n");
	printf("typedef char bool;\n");
	printf("#define true 1\n");
	printf("#define false 0\n");
	printf("#define strc_def \"\"\n");
	printf("#define bool_def false\n");
	printf("#define char_def 0\n");
	printf("#define int_def 0\n");
	printf("#define float_def 0.0f\n");
	printf("#define null 0\n");

	const long n=oo->stmts.count;
	for(long i=0;i<n;i++){
		xexp*e=(xexp*)ptrs_get(&oo->stmts,i);
		if(e->compile)
			e->compile(e,tc);
	}

	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("int main(int c,char**a){\n");
	printf("    global g=global_def;\n");
	printf("    global_init(&g);\n");
	printf("    global_main(&g);\n");
	printf("    global_free(&g);\n");
	printf("    return 0;\n");
	printf("}\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");

}

inline static void _xprg_free_(xexp*o){
	xprg*oo=(xprg*)o;
	const long n=oo->stmts.count;
	for(long i=0;i<n;i++){
		xexp*e=(xexp*)ptrs_get(&oo->stmts,i);
		if(e->free)
			e->free(e);
		free(e);
	}
	ptrs_free(&oo->stmts);
//	free(o);
}

#define xprg_def (xprg){\
	{_xprg_compile_,_xprg_free_,_xprg_print_source_,strc_def,token_def,0,false}\
	 ,ptrs_def}

inline static/*gives*/xprg*xprg_read_next(toc*tc){
	xprg*o=malloc(sizeof(xprg));
	*o=xprg_def;
	while(1){
		token tk=toc_next_token(tc);
		if(token_is_empty(&tk))
			break;
		xtype*t/*takes*/=xtype_read_next(tc,tk);
		ptrs_add(&o->stmts,t);
	}
	return o;
}
