#pragma once
#include"xexpls.h"
#include"xcode.h"

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

typedef struct xtype{
	cstr name;
	dynp fields;
	dynp funcs;
	token token;
}xtype;

#define xtype_def (xtype){cstr_def,dynp_def,dynp_def,token_def}

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
