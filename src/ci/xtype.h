#pragma once
#include"xexpls.h"
#include"xcode.h"

typedef struct xfield{
	str type;
	str name;
	xexpls initval;
	token token;
}xfield;

#define xfield_def (xfield){str_def,str_def,xexpls_def,token_def}

typedef struct xfuncarg{
	str type;
	str name;
	token token;
}xfuncarg;

#define xfuncarg_def (xfuncarg){str_def,str_def,token_def}

typedef struct xfunc{
	str type;
	str name;
	dynp args;
	xcode code;
	token token;
}xfunc;

#define xfunc_def (xfunc){str_def,str_def,dynp_def,xcode_def,token_def}

typedef struct xtype{
	cstr name;
	dynp fields;
	dynp funcs;
	token token;
}xtype;

#define xtype_def (xtype){cstr_def,dynp_def,dynp_def,token_def}

inline static xfield*xtype_get_field_by_name(const xtype*o,cstr field_name){
	for(unsigned i=0;i<o->fields.count;i++){
		xfield*f=dynp_get(&o->fields,i);
		if(!strcmp(f->name.data,field_name))
				return f;
	}
	return NULL;
}

inline static xfunc*xtype_get_func_by_name(const xtype*o,cstr field_name){
	for(unsigned i=0;i<o->funcs.count;i++){
		xfunc*f=dynp_get(&o->funcs,i);
		if(!strcmp(f->name.data,field_name))
				return f;
	}
	return NULL;
}
