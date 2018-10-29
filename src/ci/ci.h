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

#define ci_identifier_maxlen 1024

typedef struct ci{
	ptrs types;
}ci;
#define ci_def {ptrs_def}

inline static bool ci_is_builtin_type(strc typenm){
	if(!strcmp("var",typenm))return true;
	if(!strcmp("void",typenm))return true;
	if(!strcmp("int",typenm))return true;
	if(!strcmp("str",typenm))return true;
	if(!strcmp("float",typenm))return true;
	if(!strcmp("bool",typenm))return true;
	if(!strcmp("char",typenm))return true;
	if(!strcmp("strc",typenm))return true;
//	if(!strcmp("short",typenm))return true;
//	if(!strcmp("long",typenm))return true;
//	if(!strcmp("double",typenm))return true;
	return false;
}

inline static xtype*ci_get_type_for_name_try(const toc*o,strc name){
	for(unsigned i=0;i<o->types.count;i++){
		xtype*c=ptrs_get(&o->types,i);
		if(!strcmp(c->name,name)){
			return c;
		}
	}
	return NULL;
}

inline static xfunc*toc_get_func_in_context(const toc*tc,token tk){
	strc funcname=null;
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&tc->scopes,(unsigned)j);
		if(s->type!='f')
			continue;
		funcname=s->name;
		break;
	}
	strc typenm=null;
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&tc->scopes,(unsigned)j);
		if(s->type!='c')
			continue;
		typenm=s->name;
		break;
	}
	xtype*tp=ci_get_type_for_name_try(tc,typenm);
	xfunc*fn=xtype_get_func_for_name(tp,funcname);
	return fn;
}

inline static xtyperef ci_get_typeref_for_accessor(
		const toc*tc,token tk,strc accessor){

	const tocdecl*td=toc_get_declaration_for_accessor(tc,accessor);
	if(!td){
		toc_print_source_location(tc,tk,4);
		xfunc*fn=toc_get_func_in_context(tc,tk);
		printf("'%s' not found in '%s' and is not declared in '%s'",accessor,
				toc_get_typenm_in_context(tc,tk),
				fn->name
				);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	xtype*tp=ci_get_type_for_name_try(tc,td->type);
	strc tpnm=tp?tp->name:td->type;
	bool isref=td->is_ref;

	strc dotixptr=strchr(accessor,'.');
	while(1){ // e2.id
		if(!dotixptr)
			break;
		accessor=dotixptr+1; // id
		dotixptr=strchr(accessor,'.');
		strb s=strb_def;
		if(dotixptr)
			strb_add_list(&s,accessor,(unsigned)(dotixptr-accessor));
		else
			strb_add_string(&s,accessor);
		strb_add(&s,0);
		xfield*fld=xtype_get_field_for_name(tp,s.data);
		if(fld){
			isref=fld->is_ref;
			tp=ci_get_type_for_name_try(tc,fld->type);
			if(tp)
				tpnm=tp->name;
			else
				tpnm=fld->type;
		}else{
			xfunc*fn=xtype_get_func_for_name(tp,s.data);
			if(!fn){
				toc_print_source_location(tc,tk,4);
				printf("'%s' not found in '%s'",accessor,tpnm);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmp_buf,1);
			}
			isref=fn->is_ref;
			tp=ci_get_type_for_name_try(tc,fn->type);
			if(tp)
				tpnm=tp->name;
			else
				tpnm=fn->type;
		}
		strb_free(&s);
		if(!tp)
			break;
	}
	if(!tp && !ci_is_builtin_type(tpnm)){
		toc_print_source_location(tc,tk,4);
		printf("cannot find type '%s'",tpnm);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	struct xtyperef ti={tpnm,isref};
	return ti;
}


inline static bool ci_is_builtin_func(strc funcnamne){
	if(!strcmp("p",funcnamne) ||
		!strcmp("printf",funcnamne)||
		!strcmp("malloc",funcnamne)||
		!strcmp("sizeof",funcnamne)||
		!strcmp("calloc",funcnamne)||
		!strcmp("free",funcnamne))
		return true;
	return false;
}

inline static xfunc*ci_get_func_for_accessor(const toc*tc,
						strc accessor,token tk){

	strc cur_accessor=accessor;
	const tocdecl*decl=toc_get_declaration_for_accessor(tc,cur_accessor);
	if(!decl){// no declaration found, func call to member or builtin or error
		strc tpnm=toc_get_typenm_in_context(tc,tk);
		xtype*tp=ci_get_type_for_name_try(tc,tpnm);
		xfunc*fn=xtype_get_func_for_name(tp,cur_accessor);
		if(fn)
			return fn;
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",cur_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	strc cur_typenm=decl->type;
	const xtype*cur_type=ci_get_type_for_name_try(tc,cur_typenm);
	if(cur_type){
		while(1){
			strc p=strpbrk(cur_accessor,".");
			if(!p)
				break;
			cur_accessor=p+1;
			p=strpbrk(cur_accessor,".");
			if(!p){
				xfunc*fn=xtype_get_func_for_name(cur_type,cur_accessor);
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
			strc lookup=cur_accessor;
			if(p){
				strb s=strb_def;
				strb_add_list(&s,cur_accessor,(unsigned)(p-cur_accessor));
				strb_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_for_name(cur_type,lookup);
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
			cur_type=ci_get_type_for_name_try(tc,cur_typenm);
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
	if(ci_is_builtin_func(o->name))
		return;
	xfunc*fn=ci_get_func_for_accessor(tc,o->name,o->super.token);
	o->super.is_ref=fn->is_ref;
	o->super.type=fn->type;
	if(o->args.count!=fn->params.count){
		printf("function '%s' requires %ld arguments, got %ld'",
				fn->name,fn->params.count,
				o->args.count
		);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
}
//
//inline static xtype*toc_get_type_in_context(const toc*tc,token tk){
//	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
//		tocscope*s=ptrs_get(&tc->scopes,(unsigned)j);
//		if(s->type!='c')
//			continue;
//		xtype*tp=ci_get_type_by_name(tc,s->name);
//		return tp;
//	}
//}

inline static void ci_xreturn_assert(const toc*tc,struct xreturn*o){
	xfunc*fn=toc_get_func_in_context(tc,o->super.token);
	o->super.is_ref=fn->is_ref;
	if(!strcmp(fn->type,o->expls.super.type))
			return;
	toc_print_source_location(tc,o->super.token,4);
	printf("return type '%s' does not match function return type '%s'",
			o->expls.super.type,
			fn->type
	);
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}
inline static bool ci_xvar_needs_init(const toc*tc,strc name){
	xtype*t=ci_get_type_for_name_try(tc,name);
	return (t->bits&4)==4;
//	return xtype_is_needs_call_to_init(t);
}

inline static void ci_xcode_compile_free_current_scope(toc*tc){
	const tocscope*ts=ptrs_get_last(&tc->scopes);
	for(long i=ts->tocdecls.count-1;i>=0;i--){
		const tocdecl*td=(tocdecl*)ptrs_get(&ts->tocdecls,i);
		if(ci_is_builtin_type(td->type))
			continue;
		const xtype*t=ci_get_type_for_name_try(tc,td->type);
		if(t->bits&1){ // needs free
//		if(xtype_is_needs_call_to_free(t)){// needs free
			toc_print_indent_for_compile(tc);
			printf("%s_free(&%s);",t->name,td->name);
		}
	}
}

inline static void ci_xcode_compile_free_current_loop_scope(const toc*tc,
		token tk){
//	bool nl=false;
	for(long j=tc->scopes.count-1;j>=0;j--){
		const tocscope*ts=(const tocscope*)ptrs_get(&tc->scopes,j);
		for(long i=ts->tocdecls.count-1;i>=0;i--){
			const tocdecl*td=(tocdecl*)ptrs_get(&ts->tocdecls,i);
			if(ci_is_builtin_type(td->type))
				continue;
			const xtype*t=ci_get_type_for_name_try(tc,td->type);
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
	for(long j=tc->scopes.count-1;j>=0;j--){
		const tocscope*ts=(const tocscope*)ptrs_get(&tc->scopes,j);
		for(long i=ts->tocdecls.count-1;i>=0;i--){
			const tocdecl*td=(tocdecl*)ptrs_get(&ts->tocdecls,i);
			if(ci_is_builtin_type(td->type))
				continue;
			const xtype*t=ci_get_type_for_name_try(tc,td->type);
			if(!(t->bits&1)) // needs free?
				continue;
			return true;
		}
		if(ts->type=='l') // clear including loop scope
			return false;
	}
	return false;
}

inline static strc ci_get_field_type_for_accessor(const toc*tc,
						strc accessor,token tk){

	strc current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration_for_accessor(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}
	strc current_class_name=decl->type;
	const xtype*current_type=ci_get_type_for_name_try(tc,current_class_name);
	if(current_type){
		while(1){
			strc p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			strc lookup=current_accessor;
			if(p){
				strb s=strb_def;
				strb_add_list(&s,current_accessor,p-current_accessor);
				strb_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_for_name(current_type,lookup);
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
			current_type=ci_get_type_for_name_try(tc,current_class_name);
			if(!current_type)
				break;
		}
	}
	return current_class_name;
}

inline static void ci_xset_assert(const toc*tc,const xset*o){
	strc accessor=o->name;
	strc settype=o->super.type;
	token tk=o->super.token;

	strc current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration_for_accessor(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	if(!strcmp(decl->type,"var"))// if dest is var
		return;

	strc current_class_name=decl->type;
	const xtype*current_type=ci_get_type_for_name_try(tc,current_class_name);
	if(current_type){
		while(1){
			strc p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			strc lookup=current_accessor;
			if(p){
				strb s=strb_def;
				strb_add_list(&s,current_accessor,p-current_accessor);
				strb_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=xtype_get_field_for_name(current_type,lookup);
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
			current_type=ci_get_type_for_name_try(tc,current_class_name);
			if(!current_type)
				break;
		}
	}

	if(!current_type)// builtin
		if(ci_is_builtin_type(current_class_name))
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


// returns NULL if not constant
inline static xexp*ci_read_next_constant_try(toc*tc,token tk){
	if(token_is_empty(&tk)){
		if(toc_srcp_is_take(tc,'"')){ // string
			while(1){
//				const char c=*tc->srcp;
				if(toc_srcp_is_take(tc,0) || toc_srcp_is_take(tc,'\n')){
					toc_print_source_location(tc,tk,4);
					printf("did not find the end of string on the same line");
					printf("\n    %s %d",__FILE__,__LINE__);
					longjmp(_jmp_buf,1);
				}
//				tc->srcp++;
				if(toc_srcp_is_take(tc,'\\')){
					toc_srcp_inc(tc);// skip next character
					continue;
				}
				if(toc_srcp_is_take(tc,'"'))
					break;
				toc_srcp_inc(tc);
			}
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			e->name=token_content_to_new_strc(&tk);
			e->super.type="strc";
			return(xexp*)e;

		}else if(toc_srcp_is_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(!toc_srcp_is(tc,'\'')){
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
			e->name=token_content_to_new_strc(&tk);
			e->super.type="char";
			return(xexp*)e;

		}else{
			return NULL;
		}
	}

	// constant
	strc tks=token_content_to_new_strc(&tk);
	const size_t tkslen=strlen(tks);

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


inline static xexp*ci_read_next_statement(toc*tc){
	token tk=toc_next_token(tc);

	if(token_is_empty(&tk)) // end of stream
		return NULL;

	// try read constant
	xexp*ce=ci_read_next_constant_try(tc,tk);
	if(ce)
		return ce;
	// keywords
	if(token_equals(&tk,"loop"))
		return(xexp*)xloop_read_next(tc,tk);

	if(token_equals(&tk,"break"))
		return(xexp*)xbreak_read_next(tc,tk);

	if(token_equals(&tk,"continue"))
		return(xexp*)xcont_read_next(tc,tk);

	if(token_equals(&tk,"if"))
		return(xexp*)xife_read_next(tc,tk);

	if(token_equals(&tk,"return"))
		return(xexp*)xreturn_read_next(tc,tk);

	// built in types
	strc name=token_content_to_new_strc(&tk);
	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
			token_equals(&tk,"bool")||token_equals(&tk,"char")||
			token_equals(&tk,"var")||token_equals(&tk,"strc"))
		return(xexp*)xvar_read_next(tc,name);

	//  class instance
	xtype*c=ci_get_type_for_name_try(tc,name);
	if(c) // instantiate
		return(xexp*)xvar_read_next(tc,name);

	// function call
	if(toc_srcp_is(tc,'('))
		return(xexp*)xcall_read_next(tc,tk,name);

	// assignment
	if(toc_srcp_is_take(tc,'='))
		return(xexp*)xset_read_next(tc,name,tk);

	// incdecs
	char incdecbits=0;
//	if(toc_srcp_peek_is(tk,"++")) //?
	if(toc_srcp_is_take(tc,'+')){
		if(toc_srcp_is_take(tc,'+')){
			incdecbits|=1;
		}else{
//			toc_srcp_back(2);//?
			tc->srcp--;
			tc->srcp--;
		}
	}else if(toc_srcp_is_take(tc,'-')){
		if(toc_srcp_is_take(tc,'-')){
			incdecbits|=2;
		}else{
//			toc_srcp_back(2);//?
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

inline static xexp*ci_read_next_expression(toc*tc){
	token tk=toc_next_token(tc);

	// try to read constant
	xexp*ce=ci_read_next_constant_try(tc,tk);
	if(ce)
		return ce;

	if(token_is_empty(&tk)){
		toc_print_source_location(tc,tk,4);
		printf("expected expression");
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	strc name=token_content_to_new_strc(&tk);

	// function call
	if(toc_srcp_is(tc,'('))
		return(xexp*)xcall_read_next(tc,tk,name);

	char incdecbits=0;
	// toc_src_peek_is(tc,"++") //?
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

	const xtyperef tr=ci_get_typeref_for_accessor(tc,tk,name);

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=tr.type;
	e->super.is_ref=tr.is_ref;
	return(xexp*)e;
}

inline static void ci_compile_to_c(toc*tc){
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
	for(unsigned i=0;i<tc->types.count;i++){
		xtype*c=ptrs_get(&tc->types,i);
		c->super.compile((xexp*)c,tc);
//		toc_push_scope(tc,'c',c->name);
//		// type
//		ci_print_right_aligned_comment(c->name);
//		printf("typedef struct %s{",c->name);
//		// fields
//		if(c->fields.count)
//			printf("\n");
//		for(unsigned i=0;i<c->fields.count;i++){
//			xfield*f=(xfield*)ptrs_get(&c->fields,i);
//			toc_add_declaration(tc,f->type,f->is_ref,f->name);
//			if(!strcmp(f->type,"var")){
//				if(!f->initval.exps.count){
////					toc_print_source_location(tc,);
//					printf("expected initializer for var '%s'",f->name);
//					printf("\n    %s %d",__FILE__,__LINE__);
//					longjmp(_jmp_buf,1);
//				}
//				f->initval.super.compile((xexp*)&f->initval,tc);
//				f->type=f->initval.super.type;
//			}
//			printf("    ");
//			if(!strcmp(f->type,c->name))
//				printf("struct ");
//			printf("%s",f->type);
//			if(f->is_ref)
//				printf("*");
//			else
//				printf(" ");
//			printf("%s;\n",f->name);
//		}
//		printf("}%s;\n",c->name);
//
//		// #define object_def {...}
//		printf("#define %s_def (%s){",c->name,c->name);
//		for(unsigned i=0;i<c->fields.count;i++){
//			xfield*f=(xfield*)ptrs_get(&c->fields,i);
//			if(f->initval.exps.count){
//				f->initval.super.compile((xexp*)&f->initval,tc);
//			}else{
//				if(f->is_ref)
//					printf("null");
//				else
//					printf("%s_def",f->type);
//			}
//			if(i!=c->fields.count-1)
//				printf(",");
//		}
//		printf("}\n");
//		// functions
//		if(c->funcs.count){
//			ci_print_right_aligned_comment("funcs");
//			for(unsigned i=0;i<c->funcs.count;i++){
//				xfunc*f=(xfunc*)ptrs_get(&c->funcs,i);
//				toc_push_scope(tc,'f',f->name);
//				printf("inline static %s",f->type);
//				if(f->is_ref)
//					printf("*");
//				else
//					printf(" ");
//				printf("%s_%s(%s*o",c->name,f->name,c->name);
//				for(unsigned j=0;j<f->params.count;j++){
//					xfuncparam*fp=ptrs_get(&f->params,j);
//					printf(",");
//					printf("%s",fp->type);
//					if(fp->is_ref)
//						printf("*");
//					else
//						printf(" ");
//					printf("%s",fp->name);
//					toc_add_declaration(tc,fp->type,fp->is_ref,fp->name);
//				}
//				printf(")");
//				f->code.super.compile((xexp*)&f->code,tc);
////				printf("\n");
//				toc_pop_scope(tc);
//			}
//		}
//		// cascading init
//		if((c->bits&4) || !strcmp(c->name,"global")){// needs call to init
//			printf("inline static void %s_init(%s*o){",c->name,c->name);
//			if(c->fields.count)
//				printf("\n");
//			for(unsigned i=0;i<c->fields.count;i++){
//				xfield*f=ptrs_get(&c->fields,i);
//				if(ci_is_builtin_type(f->type))
//					continue;
//				xtype*cc=ci_get_type_for_name_try(tc,f->type);
//				if(cc->bits&4)// has init
//					printf("    %s_init(&o->%s);\n",f->type,f->name);
//			}
//			if(c->bits&8) // has _init
//				printf("    %s__init(o);\n",c->name);
//			printf("}\n");
//		}
//		// cascading free
//		if((c->bits&1) || !strcmp(c->name,"global")){// needs call to free
//			printf("inline static void %s_free(%s*o){",c->name,c->name);
//			if(c->fields.count)
//				printf("\n");
//			if(c->bits&2) // has _free
//				printf("    %s__free(o);\n",c->name);
//			for(long i=c->fields.count-1;i>=0;i--){
//				xfield*f=(xfield*)ptrs_get(&c->fields,i);
//				if(ci_is_builtin_type(f->type))
//					continue;
//				xtype*cc=ci_get_type_for_name_try(tc,f->type);
//				if(cc->bits&1) // needs _free?
//					printf("    %s_free(&o->%s);\n",f->type,f->name);
//			}
//			printf("}\n");
//		}
//		toc_pop_scope(tc);
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

inline static int ci_compile_file(strc path){
	const int ret=setjmp(_jmp_buf);
	if(ret)
		return ret;
	toc tc=toc_def;
	tc.filepth=path;
	strb srcstr=strb_from_file(path);
	tc.srcp=tc.src=srcstr.data;

	xprg*prg=/*takes*/xprg_read_next(&tc);
	prg->super.compile((xexp*)prg,&tc);
//	while(1){
//		token tk=toc_next_token(&tc);
//		if(token_is_empty(&tk))
//			break;
//		xtype_read_next(&tc,tk);
//	}
//	ci_print_source(&tc);
//	prg->super.print_source((xexp*)prg);
//	ci_compile_to_c(&tc);
	prg->super.free((xexp*)prg);
//	for(unsigned i=0;i<tc.types.count;i++){
//		xtype*t=(xtype*)ptrs_get(&tc.types,i);
//		xtype_free(t);
//		free(t);
//	}
	ptrs_free(&tc.types);
	ptrs_free(&tc.scopes);
	token_free();
	strb_free(&srcstr);
	ci_free();
	return 0;
}

inline static void ci_xset_compile(const toc*tc,const xset*o){
	token tk=o->super.token;
	strc id=o->name;
	strc p=strpbrk(id,".");
	if(p){
		strb idstr=strb_def;
		strb_add_list(&idstr,id,p-id);
		strb_add(&idstr,0);

		const tocdecl*i=toc_get_declaration_for_accessor(tc,idstr.data);
		if(!i){
			printf("<file> <line:col> identifier '%s' not found\n",id);
			longjmp(_jmp_buf,1);
		}
		ci_xset_assert(tc,o);
		const char scopetype=toc_get_declaration_scope_type(tc,idstr.data);
		idstr.count--;//? adhock
		if(i->is_ref){
			strb_add_list(&idstr,"->",2);
			strb_add_string(&idstr,p+1);
		}else{
			strb_add_string(&idstr,p);
		}
		strb_add(&idstr,0);
		if(scopetype){
			if(scopetype=='c'){// class member
				printf("o->%s",idstr.data);
			}else{// local identifier
				printf("%s",idstr.data);
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

	toc_print_source_location(tc,tk,4);
	printf("could not find var '%s'\n",id);
	longjmp(_jmp_buf,1);
}

inline static/*gives*/strc ci_get_c_accessor_for_accessor(
		const toc*tc,token tk,strc accessor){
	strb cacc=strb_def;
	strc ap=accessor;
	strc p=strchr(ap,'.');
	while(1){
		if(p)
			strb_add_list(&cacc,ap,p-ap);
		else
			strb_add_string(&cacc,ap);
		if(!p)
			break;
		strb temp=strb_def;
		strb_add_list(&temp,ap,p-ap);
		strb_add(&temp,0);
		const xtyperef tr=ci_get_typeref_for_accessor(tc,tk,temp.data);
		strb_free(&temp);
		if(tr.is_ref)
			strb_add_string(&cacc,"->");
		else
			strb_add_string(&cacc,".");
		ap=p+1;
		p=strchr(ap,'.');
	}
	strb_add(&cacc,0);
	return cacc.data;
}

inline static void ci_xcall_compile(const toc*tc,const struct xcall*c){
	token tk=c->super.token;

	//? builtins
	if(!strcmp("p",c->name) || !strcmp("printf",c->name)){
		printf("printf(");
		return;
	}
	if(!strcmp("malloc",c->name) || !strcmp("calloc",c->name)){
		printf("%s(",c->name);
		return;
	}
	char cb[ci_identifier_maxlen];
	strcpy(cb,c->name);
	const char*pathnm=cb;
	const char*varnm=cb;
	const char*funcnm=strrchr(cb,'.');
	if(funcnm){
		cb[funcnm-cb]=0;
		funcnm++;
		const xtyperef vartr=ci_get_typeref_for_accessor(tc,tk,varnm);
		const char scope=toc_get_declaration_scope_type(tc,varnm);
		const xtyperef pathtr=ci_get_typeref_for_accessor(tc,tk,pathnm);
//		printf("%s_%s((%s*)",vartr.type,funcnm,vartr.type);
		printf("%s_%s(",vartr.type,funcnm);
//		printf("(%s*)",vartr.type);
		if(!pathtr.is_ref)
			printf("&");

		strc cacc=ci_get_c_accessor_for_accessor(tc,tk,pathnm);

		if(scope=='c')
			printf("o->");
		printf("%s",cacc);
		if(c->args.count)
			printf(",");
		return;
	}
	funcnm=cb;
	if(ci_is_builtin_func(funcnm)){
		printf("%s(",funcnm);
		return;
	}
	strc typenm=toc_get_typenm_in_context(tc,tk);
	printf("%s_%s(o",typenm,funcnm);
	if(c->args.count)
		printf(",");
}

inline static bool ci_is_func_param_ref(
		const toc*tc,token tk,strc accessor,unsigned param_index){

	//? builtins
	if(!strcmp("p",accessor) || !strcmp("printf",accessor))
		return false;

	if(!strcmp("free",accessor) && param_index==0)
		return true;


	char cb[ci_identifier_maxlen];
	strcpy(cb,accessor);
	const char*varnm=cb;
	const char*funcnm=strrchr(cb,'.');
	if(funcnm){
		cb[funcnm-cb]=0;
		funcnm++;
		strc vartypenm=ci_get_field_type_for_accessor(tc,varnm,tk);
		const xtype*tp=ci_get_type_for_name_try(tc,vartypenm);
		const xfunc*fn=xtype_get_func_for_name(tp,funcnm);
		const xfuncparam*fna=ptrs_get(&fn->params,param_index);
		return fna->is_ref;
	}
	return false;
}
