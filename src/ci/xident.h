#pragma once
#include"../lib.h"
#include"xtype.h"
#include"toc.h"

typedef struct xident{
	xexp super;
	strc ident;
	char incdecbits;
}xident;

#define xident_def (xident){\
	{_xident_compile_,NULL,_xident_print_source_,strc_def,token_def,0,false},\
	strc_def,false\
}

inline static void _xident_print_source_(xexp*oo){
}

inline static void _xident_compile_(const xexp*oo,toc*tc){
	const xident*o=(xident*)oo;
	strc id=o->ident;
	strc p=strpbrk(id,".");
	if(!p){// local var or class member
		const char idtype=toc_get_declaration_scope_type(tc,id);
		if(idtype=='c'){// class member
			printf("o->%s",id);
		}else{
			// local var
			printf("%s",id);
		}
		if(o->incdecbits&1)printf("++");
		if(o->incdecbits&2)printf("--");
		return;
	}
	// ie: e.location.name
	strb s=strb_def;
	strb_add_list(&s,o->ident,p-o->ident);
	strb_add(&s,0);
	const char decl_scope_type=toc_get_declaration_scope_type(tc,s.data);
	const tocdecl*td=toc_get_declaration_for_accessor(tc,s.data);
	s.count--; // prepare for append
	if(td->is_ref){
		strb_add_string(&s,"->");
	}else{
		strb_add_string(&s,".");
	}
	xtype*tp=ci_get_type_for_name_try(tc,td->type);
	while(1){
		id=p+1;
		p=strpbrk(id,".");
		strb si=strb_def;// sub identiy
		if(p){
			strb_add_list(&si,id,p-id);
		}else{
			strb_add_string(&si,id);
		}
		strb_add(&si,0);
		xfield*fld=xtype_get_field_for_name(tp,si.data);
		if(!fld){// arguments
			toc_print_source_location(tc,o->super.token,4);
			printf("could not find field '%s' in type '%s' using identifier '%s'",si.data,tp->name,o->ident);
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
		strb_free(&si);

		strb_add_string(&s,fld->name);
		if(!strpbrk(id,".")){
			strb_add(&s,0);
			break;
		}

		if(fld->is_ref){
			strb_add_string(&s,"->");
		}else{
			strb_add_string(&s,".");
		}
		tp=ci_get_type_for_name_try(tc,fld->type);
	}
	if(decl_scope_type=='c'){// class member
		printf("o->%s",s.data);
	}else{// variable
		printf("%s",s.data);
	}
	strb_free(&s);
	if(o->incdecbits&1)printf("++");
	if(o->incdecbits&2)printf("--");
}
