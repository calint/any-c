#pragma once
#include"xcode.h"

typedef struct xfield{
	cstr type;
	cstr name;
	xexpls initval;
	token token;
	bool is_ref;
}xfield;

#define xfield_def (xfield){cstr_def,cstr_def,xexpls_def,token_def,false}

inline static void xfield_free(xfield*o){
	_xexpls_free_((xexp*)&o->initval);
}

typedef struct xfuncparam{
	cstr type;
	cstr name;
	token token;
	bool is_ref;
}xfuncparam;

#define xfuncparam_def (xfuncparam){cstr_def,cstr_def,token_def,false}

typedef struct xfunc{
	cstr type;
	cstr name;
	dynp params;
	xcode code;
	token token;
	bool is_ref;
}xfunc;

#define xfunc_def (xfunc){\
	cstr_def,cstr_def,dynp_def,xcode_def,token_def,false}

inline static void xfunc_free(xfunc*o){
	dynp_free(&o->params);
	_xcode_free_((xexp*)&o->code);
}

#include"decouple.h"

typedef struct xtype{
	cstr name;
	dynp fields;
	dynp funcs;
	token token;
	char bits; // 1: needs call to free   2: has _free
	           // 3: needs call to init   4: has _init
}xtype;

#define xtype_def (xtype){cstr_def,dynp_def,dynp_def,token_def,0}

//inline static struct xtype*ci_get_type_by_name(const toc*o,cstr);

inline static void xtype_free(xtype*o){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=(xfield*)dynp_get(&o->fields,i);
		xfield_free(f);
		free(f);
	}
	dynp_free(&o->fields);

	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=(xfunc*)dynp_get(&o->funcs,i);
		xfunc_free(f);
		free(f);
	}
	dynp_free(&o->funcs);
}

inline static xfield*xtype_get_field_for_name(const xtype*o,cstr field_name){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=dynp_get(&o->fields,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xtype_get_func_for_name(const xtype*o,cstr field_name){
	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=dynp_get(&o->funcs,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xfunc_read_next(toc*tc,xtype*c,bool is_ref,
		token type,token name){
	xfunc*f=malloc(sizeof(xfunc));
	*f=xfunc_def;
	f->is_ref=is_ref;
	bool enclosed_args=false;
	if(toc_srcp_is(tc,'{') || toc_srcp_is(tc,'(')){
		if(token_is_empty(&name)){ // global{main{}}
			f->type="void";
			f->name=token_to_new_cstr(&type);
		}else{ // global{int main{}}
			f->type=token_to_new_cstr(&type);
			f->name=token_to_new_cstr(&name);
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
		dynp_add(&f->params,fp);
		*fp=xfuncparam_def;
		if(toc_srcp_is_take(tc,'&'))
			fp->is_ref=true;
		token tkn=toc_next_token(tc);
		fp->type=token_to_new_cstr(&tkt);
		fp->name=token_to_new_cstr(&tkn);
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
	dynp_add(&c->funcs,f);
	xcode_read_next(&f->code,tc);
	toc_pop_scope(tc);
	return f;
}

inline static xfield*xfield_read_next(toc*tc,cstr type,token name,bool is_ref){
	xfield*f=malloc(sizeof(xfield));
	*f=xfield_def;
	f->type=type;
	f->is_ref=is_ref;
	if(toc_srcp_is_take(tc,'&'))
		f->is_ref=true;

	f->name=token_is_empty(&name)?f->type:token_to_new_cstr(&name);
	toc_add_declaration(tc,f->type,f->is_ref,f->name);
	if(toc_srcp_is_take(tc,'=')){
		xexpls_parse_next(&f->initval,tc,name);
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
	return f;
}

inline static xtype*xtype_read_next(toc*tc,token nmtk){
	xtype*c=malloc(sizeof(xtype));
	*c=xtype_def;
	dynp_add(&tc->types,c);
	c->token=nmtk;
	c->name=token_to_new_cstr(&c->token);
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
		}else if(toc_srcp_is(tc,'=')){
			xfield*f=xfield_read_next(tc,"var",tptk,is_ref);
			dynp_add(&c->fields,f);
		}else if(toc_srcp_is(tc,';')){
			xfield*f=xfield_read_next(tc,token_to_new_cstr(&tptk),tptk,is_ref);
			dynp_add(&c->fields,f);
		}else{
			token nm=toc_next_token(tc);
			if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
//				tc->srcp=nm.content;
				xfunc_read_next(tc,c,is_ref,tptk,nm);
			}else{
				xfield*f=xfield_read_next(tc,token_to_new_cstr(&tptk),nm,is_ref);
				dynp_add(&c->fields,f);
			}
		}
	}
	for(unsigned i=0;i<c->funcs.count;i++){
		xfunc*f=(xfunc*)dynp_get(&c->funcs,i);
		if(!strcmp(f->name,"_free")){
			c->bits|=3; // needs free and has _free
			continue;
		}
		if(!strcmp(f->name,"_init"))
			c->bits|=4+8; // needs init and has _init
	}
	for(unsigned i=0;i<c->fields.count;i++){
		xfield*f=(xfield*)dynp_get(&c->fields,i);
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

