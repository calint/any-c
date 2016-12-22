#pragma once
#include<setjmp.h>
#include"xcall.h"
#include"xident.h"
#include"xloop.h"
#include"xvar.h"
#include"xconst.h"
#include"xexpls.h"
#include"xreturn.h"
#include"xbreak.h"
#include"xcode.h"
#include"xcont.h"
#include"xife.h"
#include"xtype.h"
#include"decouple.h"

typedef struct ci{
	dynp types;
}ci;
#define ci_def {dynp_def}

inline static bool ci_is_type_builtin(cstr typenm){
	if(!strcmp("var",typenm))return true;
	if(!strcmp("void",typenm))return true;
	if(!strcmp("int",typenm))return true;
	if(!strcmp("str",typenm))return true;
	if(!strcmp("float",typenm))return true;
	if(!strcmp("bool",typenm))return true;
	if(!strcmp("char",typenm))return true;
	if(!strcmp("cstr",typenm))return true;
//	if(!strcmp("short",typenm))return true;
//	if(!strcmp("long",typenm))return true;
//	if(!strcmp("double",typenm))return true;
	return false;
}

inline static xtype*ci_get_type_by_name(const toc*o,cstr name){
	for(unsigned i=0;i<o->types.count;i++){
		xtype*c=dynp_get(&o->types,i);
		if(!strcmp(c->name,name)){
			return c;
		}
	}
	return NULL;
}

inline static struct xaccessorinfo ci_get_accessorinfo(
		toc*tc,token tk,cstr accessor){

	const tocdecl*td=toc_get_declaration_for_accessor(tc,accessor);
	if(!td){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	xtype*tp=ci_get_type_by_name(tc,td->type);
	cstr tpnm=tp?tp->name:td->type;
	bool isref=td->is_ref;

	cstr dotixptr=strchr(accessor,'.');
	while(1){ // e2.id
		if(!dotixptr)
			break;
		accessor=dotixptr+1; // id
		dotixptr=strchr(accessor,'.');
		str s=str_def;
		if(dotixptr)
			str_add_list(&s,accessor,dotixptr-accessor);
		else
			str_add_string(&s,accessor);
		str_add(&s,0);
		xfield*fld=xtype_get_field_by_name(tp,s.data);
		if(fld){
			isref=fld->is_ref;
			tp=ci_get_type_by_name(tc,fld->type);
			if(tp)
				tpnm=tp->name;
			else
				tpnm=fld->type;
		}else{
			xfunc*fn=xtype_get_func_by_name(tp,s.data);
			if(!fn){
				toc_print_source_location(tc,tk,4);
				printf("'%s' not found in '%s'",accessor,tpnm);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			isref=fn->return_is_ref;
			tp=ci_get_type_by_name(tc,fn->type);
			if(tp)
				tpnm=tp->name;
			else
				tpnm=fn->type;
		}
		str_free(&s);
		if(!tp)
			break;
	}
	if(!tp && !ci_is_type_builtin(tpnm)){
		toc_print_source_location(tc,tk,4);
		printf("cannot find type '%s'",tpnm);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	struct xaccessorinfo ti={tpnm,isref};
	return ti;
}


inline static bool ci_is_func_builtin(cstr funcnamne){
	if(!strcmp("p",funcnamne) ||
		!strcmp("printf",funcnamne))
		return true;
	return false;
}

inline static xfunc*toc_get_func_in_context(const toc*tc,token tk){
	cstr funcname=null;
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
		if(s->type!='f')
			continue;
		funcname=s->name;
		break;
	}
	cstr typenm=null;
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
		if(s->type!='c')
			continue;
		typenm=s->name;
		break;
	}
	xtype*tp=ci_get_type_by_name(tc,typenm);
	xfunc*fn=xtype_get_func_by_name(tp,funcname);
	return fn;
}

inline static xfunc*ci_get_func_for_accessor(const toc*tc,
						cstr accessor,token tk){

	cstr cur_accessor=accessor;
	const tocdecl*decl=toc_get_declaration_for_accessor(tc,cur_accessor);
	if(!decl){// no declaration found, func call to member or builtin or error
		cstr tpnm=toc_get_type_in_context(tc,tk);
		xtype*tp=ci_get_type_by_name(tc,tpnm);
		xfunc*fn=xtype_get_func_by_name(tp,cur_accessor);
		if(fn)
			return fn;
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",cur_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	cstr cur_typenm=decl->type;
	const xtype*cur_type=ci_get_type_by_name(tc,cur_typenm);
	if(cur_type){
		while(1){
			cstr p=strpbrk(cur_accessor,".");
			if(!p)
				break;
			cur_accessor=p+1;
			p=strpbrk(cur_accessor,".");
			if(!p){
				xfunc*fn=xtype_get_func_by_name(cur_type,cur_accessor);
				if(!fn){
					toc_print_source_location(tc,tk,4);
					printf("cannot find function '%s' in '%s', using '%s'",
							cur_accessor,
							cur_type->name,
							accessor
					);
					printf("\n    %s %d",__FILE__,__LINE__);
					longjmp(_jmp_buf,1);
				}
				return fn;
			}
			cstr lookup=cur_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,cur_accessor,p-cur_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_by_name(cur_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						cur_accessor,
						cur_type->name,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			cur_typenm=fld->type;
			cur_type=ci_get_type_by_name(tc,cur_typenm);
			if(!cur_type)
				break;
		}
	}
	printf("cannot find function '%s' in '%s', using '%s'",
			cur_accessor,
			cur_type->name,
			accessor
	);
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}


inline static void ci_xcall_assert(const toc*tc,xcall*o){
	if(ci_is_func_builtin(o->name))
		return;
	xfunc*fn=ci_get_func_for_accessor(tc,o->name,o->super.token);
	o->super.is_ref=fn->return_is_ref;
	o->super.type=fn->type;
	if(o->args.count!=fn->funcargs.count){
		printf("function '%s' requires %d arguments, got %d'",
				fn->name,fn->funcargs.count,
				o->args.count
		);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
}
//
//inline static xtype*toc_get_type_in_context(const toc*tc,token tk){
//	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
//		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
//		if(s->type!='c')
//			continue;
//		xtype*tp=ci_get_type_by_name(tc,s->name);
//		return tp;
//	}
//}

inline static void ci_xreturn_assert(const toc*tc,struct xreturn*o){
	xfunc*fn=toc_get_func_in_context(tc,o->super.token);
	o->super.is_ref=fn->return_is_ref;
	if(!strcmp(fn->type,o->expls.super.type))
			return;
	printf("return type '%s' does not match function return type '%s'",
			o->expls.super.type,
			fn->type
	);
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}
inline static bool ci_type_needs_init(toc*tc,cstr name){
	xtype*t=ci_get_type_by_name(tc,name);
	return (t->bits&4)==4;
}

inline static void ci_xcode_compile_free_current_scope(toc*tc){
	const tocscope*ts=dynp_get_last(&tc->scopes);
	for(int i=ts->tocdecls.count-1;i>=0;i--){
		const tocdecl*td=(tocdecl*)dynp_get(&ts->tocdecls,i);
		if(ci_is_type_builtin(td->type))
			continue;
		const xtype*t=ci_get_type_by_name(tc,td->type);
		if(t->bits&1){ // needs free
			toc_print_indent_for_compile(tc);
			printf("%s_free(&%s);",t->name,td->name);
		}
	}
}

inline static void ci_xcode_compile_free_current_loop_scope(toc*tc,token tk){
//	bool nl=false;
	for(int j=tc->scopes.count-1;j>=0;j--){
		const tocscope*ts=(const tocscope*)dynp_get(&tc->scopes,j);
		for(int i=ts->tocdecls.count-1;i>=0;i--){
			const tocdecl*td=(tocdecl*)dynp_get(&ts->tocdecls,i);
			if(ci_is_type_builtin(td->type))
				continue;
			const xtype*t=ci_get_type_by_name(tc,td->type);
			if(!(t->bits&1)) // needs free
				continue;
			toc_print_indent_for_compile(tc);
			printf("%s_free(&%s);\n",t->name,td->name);
//			nl=true;
		}
		if(ts->type=='l') // clear including loop scope
			break;
	}
//	if(nl)
//		printf("\n");
}

inline static bool ci_xcode_needs_compile_free_current_loop_scope(toc*tc,
		token tk){
	for(int j=tc->scopes.count-1;j>=0;j--){
		const tocscope*ts=(const tocscope*)dynp_get(&tc->scopes,j);
		for(int i=ts->tocdecls.count-1;i>=0;i--){
			const tocdecl*td=(tocdecl*)dynp_get(&ts->tocdecls,i);
			if(ci_is_type_builtin(td->type))
				continue;
			const xtype*t=ci_get_type_by_name(tc,td->type);
			if(!(t->bits&1)) // needs free?
				continue;
			return true;
		}
		if(ts->type=='l') // clear including loop scope
			return false;
	}
	return false;
}

inline static cstr ci_get_field_type_for_accessor(const toc*tc,
						cstr accessor,token tk){

	cstr current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration_for_accessor(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	cstr current_class_name=decl->type;
	const xtype*current_type=ci_get_type_by_name(tc,current_class_name);
	if(current_type){
		while(1){
			cstr p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			cstr lookup=current_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,current_accessor,p-current_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_by_name(current_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						current_accessor,
						current_type->name,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			current_class_name=fld->type;
			current_type=ci_get_type_by_name(tc,current_class_name);
			if(!current_type)
				break;
		}
	}
	return current_class_name;
}

inline static void ci_xset_assert(const toc*tc,
		cstr accessor,cstr settype,token tk){

	cstr current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration_for_accessor(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	if(!strcmp(decl->type,"var"))// if dest is var
		return;

	cstr current_class_name=decl->type;
	const xtype*current_type=ci_get_type_by_name(tc,current_class_name);
	if(current_type){
		while(1){
			cstr p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			cstr lookup=current_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,current_accessor,p-current_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_by_name(current_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						current_accessor,
						current_type->name,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			current_class_name=fld->type;
			current_type=ci_get_type_by_name(tc,current_class_name);
			if(!current_type)
				break;
		}
	}

	if(!current_type)// builtin
		if(ci_is_type_builtin(current_class_name))
			if(!strcmp(current_class_name,settype))
				return;

	if(!strcmp(settype,current_class_name))
		return;

	toc_print_source_location(tc,tk,4);
	printf("cannot set '%s' to '%s', %s is %s",
			settype,current_class_name,
			accessor,current_class_name
	);
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}

inline static void ci_init(){}

inline static void ci_free(){}

inline static xexp*ci_read_next_statement(toc*tc){
	token tk=toc_next_token(tc);
	if(token_is_empty(&tk)){
		if(toc_srcp_is_take(tc,'"')){ // string
			while(1){
				const char c=*tc->srcp;
				if(c==0){
					printf("<file> <line:col> did not find the "
							" end of string\n");
					longjmp(_jmp_buf,1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					longjmp(_jmp_buf,1);
				}
				tc->srcp++;
				if(c=='\\'){
					tc->srcp++;
					continue;
				}
				if(c=='"')break;
			}
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);
			e->super.type="cstr";
			return(xexp*)e;

		}else if(toc_srcp_is_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(*tc->srcp!='\''){
				toc_print_source_location(tc,tk,4);
				printf("expected a character, example 'a'");
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			toc_srcp_inc(tc);
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);
			e->super.type="char";
			return(xexp*)e;

		}else{
			return null;
//			xexp*e=malloc(sizeof(xexp));
//			*e=xexp_def;
//			return e;
		}
	}

	// constant
	cstr tks=token_to_new_cstr(&tk);

	// boolean
	if(!strcmp("true",tks) || !strcmp("false",tks)){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="bool";
		return(xexp*)e;
	}

	// int
	char*endptr;strtol(tks,&endptr,10);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="int";
		return(xexp*)e;
	}

	// float
	strtof(tks,&endptr);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="float";
		return(xexp*)e;
	}

	// hex
	const unsigned tkslen=strlen(tks);
	if(tkslen>1){
		if((tks[0]=='0' && tks[1]=='x')){
			strtol(tks+2,&endptr,16);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
		if((tks[0]=='0' && tks[1]=='b')){
			strtol(tks+2,&endptr,2);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
	}

	// keywords
	if(token_equals(&tk,"loop")){
		xloop*e=xloop_read_next(tc,tk);
		e->super.token=tk;
		return (xexp*)e;
	}

	if(token_equals(&tk,"break")){
		xbreak*e=xbreak_read_next(tc,tk);
		e->super.token=tk;
		return (xexp*)e;
	}

	if(token_equals(&tk,"continue")){
		xcont*e=xcont_read_next(tc,tk);
		return (xexp*)e;
	}

	if(token_equals(&tk,"if")){
		xife*e=xife_read_next(tc,tk);
		return (xexp*)e;
	}

	if(token_equals(&tk,"return")){
		xreturn*e=xreturn_read_next(tc,tk);
		return (xexp*)e;
	}

	// built in types
	cstr name=token_to_new_cstr(&tk);
	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
			token_equals(&tk,"bool")||token_equals(&tk,"char")||
			token_equals(&tk,"var")||token_equals(&tk,"ccharp")){//const char*
		xvar*e=xvar_read_next(tc,name);
		return(xexp*)e;
	}

	//  class instance
	xtype*c=ci_get_type_by_name(tc,name);
	if(c){// instantiate
		xvar*e=xvar_read_next(tc,name);
		return(xexp*)e;
	}

	// function call
	if(*tc->srcp=='('){
		xcall*e=xcall_read_next(tc,tk,/*gives*/name);
		return(xexp*)e;
	}

	// assignment
	if(*tc->srcp=='='){
		tc->srcp++;
		if(*tc->srcp!='='){
			xset*e=xset_read_next(tc,name,tk);
			return(xexp*)e;
		}
		tc->srcp--;
	}

	char incdecbits=0;
	if(*tc->srcp=='+'){
		tc->srcp++;
		if(*tc->srcp=='+'){
			tc->srcp++;
			incdecbits|=1;
		}else{
			tc->srcp--;
			tc->srcp--;
		}
	}else if(*tc->srcp=='-'){
		tc->srcp++;
		if(*tc->srcp=='-'){
			tc->srcp++;
			incdecbits|=2;
		}else{
			tc->srcp--;
			tc->srcp--;
		}
	}

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=ci_get_field_type_for_accessor(tc,e->name,tk);
	return(xexp*)e;
}


#define ci_identifier_maxlen 1024
inline static bool ci_is_func_return_ref(
		toc*tc,token tk,cstr accessor){

	if(!strcmp("p",accessor) || !strcmp("printf",accessor)){
		return false;
	}

	char cb[ci_identifier_maxlen];
	strcpy(cb,accessor);
//	const char*path_ptr=cb;
	const char*varnm_ptr=cb;
	const char*funcnm_ptr=strrchr(cb,'.');   // g.gl.draw
	if(funcnm_ptr){                           //
		cb[funcnm_ptr-cb]=0;
		funcnm_ptr++;                         // func: print
	}
	cstr vartypestr=ci_get_field_type_for_accessor(tc,varnm_ptr,tk);
	const xtype*tp=ci_get_type_by_name(tc,vartypestr);
	const xfunc*fn=xtype_get_func_by_name(tp,funcnm_ptr);
	return fn->return_is_ref;
}

// returns NULL if not constant
inline static xexp*ci_read_next_constant_try(toc*tc,token tk){
	if(token_is_empty(&tk)){
		if(toc_srcp_is_take(tc,'"')){ // string
			while(1){
				const char c=*tc->srcp;
				if(c==0){
					toc_print_source_location(tc,tk,4);
					printf("did not find the end of string on the same line");
					printf("\n    %s %d",__FILE__,__LINE__);
					longjmp(_jmp_buf,1);
				}
				if(c=='\n'){
					toc_print_source_location(tc,tk,4);
					printf("did not find the end of string on the same line");
					printf("\n    %s %d",__FILE__,__LINE__);
					longjmp(_jmp_buf,1);
				}
				tc->srcp++;
				if(c=='\\'){
					tc->srcp++;
					continue;
				}
				if(c=='"')break;
			}
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);
//			token_setz(&tk,&e->name);
			e->super.type="cstr";
			return(xexp*)e;

		}else if(toc_srcp_is_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(*tc->srcp!='\''){
				toc_print_source_location(tc,tk,4);
				printf("expected a character, example 'a'");
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			toc_srcp_inc(tc);
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_to_new_cstr(&tk);
			e->super.type="char";
			return(xexp*)e;

		}else{
			xexp*e=malloc(sizeof(xexp));
			*e=xexp_def;
			return e;
		}
	}

	// constant
	cstr tks=token_to_new_cstr(&tk);
	const unsigned tkslen=strlen(tks);

	// boolean
	if(!strcmp("true",tks) || !strcmp("false",tks)){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="bool";
		return(xexp*)e;
	}

	// int
	char*endptr;strtol(tks,&endptr,10);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="int";
		return(xexp*)e;
	}

	// float
	strtof(tks,&endptr);
	if(!*endptr){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type="float";
		return(xexp*)e;
	}

	// hex
	if(tkslen>1){
		if(tks[0]=='0' && tks[1]=='x'){
			strtol(tks+2,&endptr,16);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
		if(tks[0]=='0' && tks[1]=='b'){
			strtol(tks+2,&endptr,2);
			if(!*endptr){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type="int";
				return(xexp*)e;
			}
		}
	}
	return NULL;
}

inline static xexp*ci_read_next_expression(toc*tc){
	token tk=toc_next_token(tc);
	xexp*ce=ci_read_next_constant_try(tc,tk);
	if(ce)
		return ce;
	// built in types
	cstr name=token_to_new_cstr(&tk);

	// function call
	if(*tc->srcp=='('){
		xcall*e=xcall_read_next(tc,tk,name);
		return(xexp*)e;
	}

	char incdecbits=0;
	if(toc_srcp_is_take(tc,'+')){
		if(toc_srcp_is_take(tc,'+')){
			incdecbits|=1;
		}else{
			tc->srcp--;
		}
	}else if(toc_srcp_is_take(tc,'-')){
		if(toc_srcp_is_take(tc,'-')){
			incdecbits|=2;
		}else{
			tc->srcp--;
		}
	}

	struct xaccessorinfo ai=ci_get_accessorinfo(tc,tk,name);

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=ai.type;
	e->super.is_ref=ai.is_ref;
	return(xexp*)e;
}

inline static void ci_print_right_aligned_comment(cstr comment){
	cstr line="--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - -- ---";
	const int maxlen=strlen(line);
	const int ln=strlen(comment);
	int start_at=maxlen-ln-4;
	if(start_at<0)start_at=0;
	printf("//");
	printf("%.*s %s\n",start_at,line,comment);
}

inline static void ci_compile_to_c(toc*tc){
	ci_print_right_aligned_comment("generated c source");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("#include<stdlib.h>\n");
	printf("#include<stdio.h>\n");
	printf("typedef const char*cstr;\n");
	printf("typedef char bool;\n");
	printf("#define true 1\n");
	printf("#define false 1\n");
	printf("#define cstr_def \"\"\n");
	printf("#define bool_def false\n");
	printf("#define char_def 0\n");
	printf("#define int_def 0\n");
	printf("#define float_def 0.0f\n");
	printf("#define null 0\n");
	for(unsigned i=0;i<tc->types.count;i++){
		xtype*c=dynp_get(&tc->types,i);
		toc_push_scope(tc,'c',c->name);
		// type
		ci_print_right_aligned_comment(c->name);
		printf("typedef struct %s{",c->name);
		// fields
		if(c->fields.count)printf("\n");
		for(unsigned i=0;i<c->fields.count;i++){
			xfield*f=(xfield*)dynp_get(&c->fields,i);
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
			xfield*f=(xfield*)dynp_get(&c->fields,i);
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
				xfunc*f=(xfunc*)dynp_get(&c->funcs,i);
				toc_push_scope(tc,'f',f->name);
				printf("inline static %s",f->type);
				if(f->return_is_ref)
					printf("*");
				else
					printf(" ");
				printf("%s_%s(%s*o",c->name,f->name,c->name);
				for(unsigned j=0;j<f->funcargs.count;j++){
					xfuncarg*a=(xfuncarg*)dynp_get(&f->funcargs,j);
					printf(",");
					printf("%s",a->type);
					if(a->func_arg_is_ref)
						printf("*");
					else
						printf(" ");
					printf("%s",a->name);
					toc_add_declaration(tc,a->type,a->func_arg_is_ref,a->name);
				}
				printf(")");
				f->code.super.compile((xexp*)&f->code,tc);
				printf("\n");
				toc_pop_scope(tc);
			}
		}
		// cascading init
		if((c->bits&4) || !strcmp(c->name,"global")){// needs call to init
			printf("inline static void %s_init(%s*o){\n",c->name,c->name);
			for(unsigned i=0;i<c->fields.count;i++){
				xfield*f=(xfield*)dynp_get(&c->fields,i);
				if(ci_is_type_builtin(f->type))
					continue;
				xtype*cc=ci_get_type_by_name(tc,f->type);
				if(cc->bits&4)// has init
					printf("    %s_init(&o->%s);\n",f->type,f->name);
			}
			if(c->bits&8) // has _init
				printf("    %s__init(o);\n",c->name);
			printf("}\n");
		}
		// cascading free
		if((c->bits&1) || !strcmp(c->name,"global")){// needs call to free
			printf("inline static void %s_free(%s*o){\n",c->name,c->name);
			if(c->bits&2) // has _free
				printf("    %s__free(o);\n",c->name);
			for(int i=c->fields.count-1;i>=0;i--){
				xfield*f=(xfield*)dynp_get(&c->fields,i);
				if(ci_is_type_builtin(f->type))
					continue;
				xtype*cc=ci_get_type_by_name(tc,f->type);
				if(cc->bits&1) // needs _free?
					printf("    %s_free(&o->%s);\n",f->type,f->name);
			}
			printf("}\n");
		}
		toc_pop_scope(tc);
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

inline static int ci_compile_file(cstr path){
	const int ret=setjmp(_jmp_buf);
	if(ret)
		return ret;
	toc tc=toc_def;
	tc.filepth=path;
	str srcstr=str_from_file(path);
	tc.srcp=tc.src=srcstr.data;
	while(1){
		token typenm=toc_next_token(&tc);
		if(token_is_empty(&typenm))
			break;
		xtype_read_next(&tc,typenm);
	}
	ci_compile_to_c(&tc);
	for(unsigned i=0;i<tc.types.count;i++){
		xtype*t=(xtype*)dynp_get(&tc.types,i);
		xtype_free(t);
		free(t);
	}
	dynp_free(&tc.types);
	dynp_free(&tc.scopes);
	token_free();
	str_free(&srcstr);
	ci_free();
	return 0;
}

inline static void ci_xset_compile(const toc*tc,token tk,
						cstr id,cstr type){
	cstr p=strpbrk(id,".");
	if(p){
		str sid=str_def;
		str_add_list(&sid,id,p-id);
		str_add(&sid,0);

		const tocdecl*i=toc_get_declaration_for_accessor(tc,sid.data);
		if(!i){
			printf("<file> <line:col> identifier '%s' not found\n",id);
			longjmp(_jmp_buf,1);
		}
		ci_xset_assert(tc,id,type,tk);
		const char scopetype=toc_get_declaration_scope_type(tc,sid.data);
		sid.count--;//? adhock
		if(i->is_ref){
			str_add_list(&sid,"->",2);
			str_add_string(&sid,p+1);
		}else{
			str_add_string(&sid,p);
		}
		str_add(&sid,0);
		if(scopetype){
			if(scopetype=='c'){// class member
				printf("o->%s",sid.data);
			}else{// local identifier
				printf("%s",sid.data);
			}
			printf("=");
			return;
		}
	}
	const char scopetype=toc_get_declaration_scope_type(tc,id);
	if(scopetype=='c'){// class member
		printf("o->%s=",id);
		return;
	}
	if(scopetype){// local identifier
		printf("%s=",id);
		return;
	}

//	tocloc tl=toc_get_line_number_from_pp(tc, p)

	toc_print_source_location(tc,tk,4);
	printf("could not find var '%s'\n",id);
	longjmp(_jmp_buf,1);
}

inline static/*gives*/cstr ci_make_c_accessor(toc*tc,token tk,cstr accessor){
	str cacc=str_def;
	cstr ap=accessor;
	cstr p=strchr(ap,'.');
	while(1){
		if(p)
			str_add_list(&cacc,ap,p-ap);
		else
			str_add_string(&cacc,ap);
		if(!p)
			break;
		str temp=str_def;
		str_add_list(&temp,ap,p-ap);
		str_add(&temp,0);
		const struct xaccessorinfo ai=ci_get_accessorinfo(tc,tk,temp.data);
		str_free(&temp);
		if(ai.is_ref)
			str_add_string(&cacc,"->");
		else
			str_add_string(&cacc,".");
		ap=p+1;
		p=strchr(ap,'.');
	}
	str_add(&cacc,0);
	return cacc.data;
}

inline static void ci_xcall_compile(
		toc*tc,token tk,cstr accessor,unsigned argcount){

	if(!strcmp("p",accessor) || !strcmp("printf",accessor)){
		printf("printf(");
		return;
	}

	char cb[ci_identifier_maxlen];
	strcpy(cb,accessor);
	const char*path_ptr=cb;
	const char*varnm_ptr=cb;
	const char*funcnm_ptr=strrchr(cb,'.');   // g.gl.draw
	if(funcnm_ptr){                           //
		cb[funcnm_ptr-cb]=0;
//		*funcnm_ptr=0;                        // path: g.gl
		funcnm_ptr++;                         // func: print
//		cstr target_type=ci_get_field_type_for_accessor(tc,varnm_ptr,tk);
		struct xaccessorinfo fai=ci_get_accessorinfo(tc,tk,varnm_ptr);
		const char scope=toc_get_declaration_scope_type(tc,varnm_ptr);
//		const bool is_arg_ref=toc_is_declaration_ref(tc,varnm_ptr);
		printf("%s_%s((%s*)",fai.type,funcnm_ptr,fai.type);

		struct xaccessorinfo ai=ci_get_accessorinfo(tc,tk,path_ptr);

		if(!ai.is_ref)
			printf("&");

		cstr cacc=ci_make_c_accessor(tc,tk,path_ptr);

		if(scope=='c'){
			printf("o->%s",cacc);
			if(argcount)
				printf(",");
			return;
		}
//		char*first_dot=strchr(cb,'.'); // g.gl
//		if(first_dot){
//			*first_dot=0;                        // var: g
//			path_ptr=first_dot+1;
//			printf("%s.%s",varnm_ptr,path_ptr);
//			if(argcount)
//				printf(",");
//			return;
//		}
//		printf("%s",varnm_ptr);
//		if(argcount)
//			printf(",");
		printf("%s",cacc);
		if(argcount)
			printf(",");
		return;
	}
	funcnm_ptr=cb;       // func: draw
	cstr target_type=toc_get_type_in_context(tc,tk);
//	const bool is_arg_ref=ci_is_func_arg_ref(tc,tk,funcnm_ptr,0);
	printf("%s_%s((%s*)",
		target_type,
		funcnm_ptr,
		target_type
	);
//	if(is_arg_ref){
//		printf("&o");
//	}else{
		printf("o");
//	}
	if(argcount)
		printf(",");
}

inline static bool ci_is_func_arg_ref(
		toc*tc,token tk,cstr accessor,unsigned arg_index){

	if(!strcmp("p",accessor) || !strcmp("printf",accessor)){
		return false;
	}

	char cb[ci_identifier_maxlen];
	strcpy(cb,accessor);
//	const char*path_ptr=cb;
	const char*varnm_ptr=cb;
	const char*funcnm_ptr=strrchr(cb,'.');   // g.gl.draw
	if(funcnm_ptr){                           //
		cb[funcnm_ptr-cb]=0;
		funcnm_ptr++;                         // func: print
		cstr vartypestr=ci_get_field_type_for_accessor(tc,varnm_ptr,tk);
		const xtype*tp=ci_get_type_by_name(tc,vartypestr);
		const xfunc*fn=xtype_get_func_by_name(tp,funcnm_ptr);
		const xfuncarg*fna=(xfuncarg*)dynp_get(&fn->funcargs,arg_index);
		return fna->func_arg_is_ref;
	}
	return false;
//	funcnm_ptr=cb;       // func: draw
//	cstr target_type=toc_get_type_in_context(tc,tk);
//	printf("%s_%s((%s*)&o",
//		target_type,
//		funcnm_ptr,
//		target_type
//	);
//	if(argcount)
//		printf(",");
}
//
//inline static bool ci_is_func_return_ref(
//		toc*tc,token tk,cstr accessor,unsigned arg_index){
//
//	if(!strcmp("p",accessor) || !strcmp("printf",accessor)){
//		return false;
//	}
//
//	char cb[ci_identifier_maxlen];
//	strcpy(cb,accessor);
////	const char*path_ptr=cb;
//	const char*varnm_ptr=cb;
//	const char*funcnm_ptr=strrchr(cb,'.');   // g.gl.draw
//	if(funcnm_ptr){                           //
//		cb[funcnm_ptr-cb]=0;
//		funcnm_ptr++;                         // func: print
//		cstr vartypestr=ci_get_type_for_accessor(tc,varnm_ptr,tk);
//		const xtype*tp=ci_get_type_by_name(tc,vartypestr);
//		const xfunc*fn=xtype_get_func_by_name(tp,funcnm_ptr);
//		const xfuncarg*fna=(xfuncarg*)dynp_get(&fn->funcargs,arg_index);
//		return fna->func_arg_is_ref;
//	}
//	return false;
////	funcnm_ptr=cb;       // func: draw
////	cstr target_type=toc_get_type_in_context(tc,tk);
////	printf("%s_%s((%s*)&o",
////		target_type,
////		funcnm_ptr,
////		target_type
////	);
////	if(argcount)
////		printf(",");
//}

