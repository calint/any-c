#pragma once
#include"xexpls.h"
#include"xcode.h"

//struct xtype;

typedef struct xfield{
	cstr type;
	cstr name;
	xexpls initval;
	token token;
}xfield;

#define xfield_def (xfield){cstr_def,cstr_def,xexpls_def,token_def}

inline static void xfield_free(xfield*o){
	_xexpls_free_((xexp*)&o->initval);
}

typedef struct xfuncarg{
	cstr type;
	cstr name;
	token token;
}xfuncarg;

#define xfuncarg_def (xfuncarg){cstr_def,cstr_def,token_def}

typedef struct xfunc{
	cstr type;
	cstr name;
	dynp funcargs;
	xcode code;
	token token;
}xfunc;

#define xfunc_def (xfunc){cstr_def,cstr_def,dynp_def,xcode_def,token_def}

inline static void xfunc_free(xfunc*o){
	dynp_free(&o->funcargs);
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

inline static xfield*xtype_get_field_by_name(const xtype*o,cstr field_name){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=dynp_get(&o->fields,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xtype_get_func_by_name(const xtype*o,cstr field_name){
	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=dynp_get(&o->funcs,i);
		if(!strcmp(f->name,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xfunc_read_next(toc*tc,token type){
	xfunc*f=malloc(sizeof(xfunc));
	*f=xfunc_def;
	bool enclosed_args=false;
	if(toc_srcp_is(tc,'{') || toc_srcp_is(tc,'(')){
		f->type="void";
		f->name=token_to_new_cstr(&type);
	}else{
		token tkname=toc_next_token(tc);
		f->type=token_to_new_cstr(&type);
		f->name=token_to_new_cstr(&tkname);
	}
	if(toc_srcp_is_take(tc,'(')){
		enclosed_args=true;
//		tc->srcp++;
	}
	toc_push_scope(tc,'f',f->name);
	while(1){
		token tkt=toc_next_token(tc);
		if(token_is_empty(&tkt))
			break;
		xfuncarg*fa=malloc(sizeof(xfuncarg));
		dynp_add(&f->funcargs,fa);
		*fa=xfuncarg_def;
		token tkn=toc_next_token(tc);
		fa->type=token_to_new_cstr(&tkt);
		fa->name=token_to_new_cstr(&tkn);
		toc_add_declaration(tc,fa->type,fa->name);
		toc_srcp_is_take(tc,',');
	}
	if(enclosed_args){
		if(!toc_srcp_is_take(tc,')')){
			//? errormsg
			printf("<file> <line:col> expected ')' after arguments\n");
			longjmp(_jmp_buf,1);
		}
	}
	xcode_read_next(&f->code,tc);
	toc_pop_scope(tc);
	return f;
}

inline static xfield*xfield_read_next(toc*tc,token type,token name){
	xfield*f=malloc(sizeof(xfield));
	*f=xfield_def;
	f->type=token_to_new_cstr(&type);
	f->name=token_is_empty(&name)?f->type:token_to_new_cstr(&name);
	toc_add_declaration(tc,f->type,f->name);
	if(toc_srcp_is_take(tc,'=')){
		xexpls_parse_next(&f->initval,tc,type);
		if(strcmp(f->type,"var")){
				ci_assert_set(tc,
					f->name,
					f->initval.super.type,
					f->initval.super.token);
		}
		f->type=f->initval.super.type;
	}
	toc_srcp_is_take(tc,';');
	return f;
}

inline static xtype*xtype_read_next(toc*tc,token name){
	xtype*c=malloc(sizeof(xtype));
	*c=xtype_def;
	dynp_add(&tc->types,c);
	c->token=name;
	c->name=token_to_new_cstr(&c->token);
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
			xfunc*f=xfunc_read_next(tc,type);
			dynp_add(&c->funcs,f);
		}else if(toc_srcp_is(tc,'=') || toc_srcp_is(tc,';')){
			token name=toc_next_token(tc);
			xfield*f=xfield_read_next(tc,type,name);
			dynp_add(&c->fields,f);
		}else{
			token name=toc_next_token(tc);
			if(toc_srcp_is(tc,'(') || toc_srcp_is(tc,'{')){
				tc->srcp=name.content;
				xfunc*f=xfunc_read_next(tc,type);
				dynp_add(&c->funcs,f);
			}else{
				xfield*f=xfield_read_next(tc,type,name);
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
		if(ci_is_type_builtin(f->type))
			continue;
		xtype*mc=ci_get_type_by_name(tc,f->type);
		if(mc->bits&1) // needs free?
			c->bits|=1;
		if(mc->bits&4) // needs init?
			c->bits|=4;
	}
	toc_pop_scope(tc);
	return c;
}

