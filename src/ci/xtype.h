#pragma once
#include"xcode.h"

typedef struct xfield{
	strc type;
	strc name;
	xexpls initval;
	token token;
	bool is_ref;
}xfield;

#define xfield_def (xfield){strc_def,strc_def,xexpls_def,token_def,false}

inline static void xfield_free(xfield*o){
	_xexpls_free_((xexp*)&o->initval);
}

typedef struct xfuncparam{
	strc type;
	strc name;
	token token;
	bool is_ref;
}xfuncparam;

#define xfuncparam_def (xfuncparam){strc_def,strc_def,token_def,false}

typedef struct xfunc{
	strc type;
	strc name;
	ptrs params;
	xcode code;
	token token;
	bool is_ref;
}xfunc;

#define xfunc_def (xfunc){\
	strc_def,strc_def,ptrs_def,xcode_def,token_def,false}

inline static void xfunc_free(xfunc*o){
	ptrs_free(&o->params);
	_xcode_free_((xexp*)&o->code);
}

#include"decouple.h"

typedef struct xtype{
	strc name;
	ptrs fields;
	ptrs funcs;
	token token;
	char bits; // 1: needs call to free   2: has _free
	           // 3: needs call to init   4: has _init
}xtype;

#define xtype_def (xtype){strc_def,ptrs_def,ptrs_def,token_def,0}

inline static void xtype_free(xtype*o){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=(xfield*)ptrs_get(&o->fields,i);
		xfield_free(f);
		free(f);
	}
	ptrs_free(&o->fields);

	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=(xfunc*)ptrs_get(&o->funcs,i);
		xfunc_free(f);
		free(f);
	}
	ptrs_free(&o->funcs);
}

inline static xfield*xtype_get_field_for_name(const xtype*o,strc field_name){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=ptrs_get(&o->fields,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xtype_get_func_for_name(const xtype*o,strc field_name){
	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=ptrs_get(&o->funcs,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static void xfunc_read_next(toc*tc,xtype*c,bool is_ref,
		token type,token name){
	xfunc*f=malloc(sizeof(xfunc));
	*f=xfunc_def;
	f->is_ref=is_ref;
	bool enclosed_args=false;
	if(toc_srcp_is(tc,'{') || toc_srcp_is(tc,'(')){
		if(token_is_empty(&name)){ // global{main{}}
			f->type="void";
			f->name=token_to_new_strc(&type);
		}else{ // global{int main{}}
			f->type=token_to_new_strc(&type);
			f->name=token_to_new_strc(&name);
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
		fp->type=token_to_new_strc(&tkt);
		fp->name=token_to_new_strc(&tkn);
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
}

inline static void xfield_read_next(toc*tc,xtype*c,strc tktype,
		token tkname,bool is_ref){

	xfield*f=malloc(sizeof(xfield));
	*f=xfield_def;
	f->token=tkname; //? tktype
	f->type=tktype;
	f->is_ref=is_ref;
	if(token_is_empty(&tkname))
		f->name=f->type;
	else
		f->name=token_to_new_strc(&tkname);


	ptrs_add(&c->fields,f);
	toc_add_declaration(tc,f->type,f->is_ref,f->name);

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
}

inline static xtype*xtype_read_next(toc*tc,token tkname){
	xtype*c=malloc(sizeof(xtype));
	*c=xtype_def;
	c->token=tkname;
	c->name=token_to_new_strc(&c->token);

	ptrs_add(&tc->types,c);
	toc_push_scope(tc,'c',c->name);

	if(!toc_srcp_is(tc,'{')){
		toc_print_source_location(tc,c->token,4);
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

		if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
			xfunc_read_next(tc,c,is_ref,tptk,token_def);
		}else if(toc_srcp_is(tc,'=')){// global{id=1}
			xfield_read_next(tc,c,"var",tptk,is_ref);
		}else if(toc_srcp_is(tc,';')){// global{tokens;}
			xfield_read_next(tc,c,token_to_new_strc(&tptk),tptk,is_ref);
		}else{
			token nm=toc_next_token(tc);
			if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
				xfunc_read_next(tc,c,is_ref,tptk,nm);
			}else{
				xfield_read_next(tc,c,token_to_new_strc(&tptk),nm,is_ref);
			}
		}
	}
	for(unsigned i=0;i<c->funcs.count;i++){
		xfunc*f=(xfunc*)ptrs_get(&c->funcs,i);
		if(!strcmp(f->name,"_free")){
			c->bits|=3; // needs free and has _free
			continue;
		}
		if(!strcmp(f->name,"_init"))
			c->bits|=4+8; // needs init and has _init
	}
	for(unsigned i=0;i<c->fields.count;i++){
		xfield*f=(xfield*)ptrs_get(&c->fields,i);
		if(ci_is_builtin_type(f->type))
			continue;
		xtype*mc=ci_get_type_for_name_try(tc,f->type);
		if(mc->bits&1) // needs free?
			c->bits|=1;
		if(mc->bits&4) // needs init?
			c->bits|=4;
	}
	toc_pop_scope(tc);
	return c;
}

