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

inline static/*gives*/strb*strb_new(){
	strb*s=malloc(sizeof(strb));
	*s=strb_def;
	return s;
}

inline static void strb_free_alloc(/*takes*/strb*s){
	strb_free(s);
	free(s);
}

inline static/*gives*/ptrs/*<strb*>*/strc_split(strc s,char split_at_char){
	ptrs strings=ptrs_def;
	strc ptr=s;
	strb*buf=strb_new();
	while(1){ // "p.anim.frame\0"
		if(*ptr==split_at_char||*ptr==0){
			strb_add(buf,0);
			ptrs_add(&strings,buf);
			if(*ptr==0)
				return strings;
			buf=strb_new();
			ptr++;
			continue;
		}
		strb_add(buf,*ptr);
		ptr++;
	}
}

inline static void strc_split_free(/*takes*/ptrs*/*<strb*>*/p){
	for(long i=0;i<p->count;i++){
		strb*sb=(strb*)ptrs_get(p,i);
		strb_free_alloc(sb);
	}
	ptrs_free(p);
}

inline static xtyperef ci_get_typeref_for_accessor(
		const toc*tc,token tk,strc accessor){

	ptrs strbs=strc_split(accessor,'.');
	strb*acc=ptrs_get(&strbs,0);
	const tocdecl*td=toc_get_declaration_for_accessor(tc,acc->data);
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

	for(long i=1;i<strbs.count;i++){
		strb*sb=ptrs_get(&strbs,i);
		xfield*fld=xtype_get_field_for_name(tp,sb->data);
		if(fld){
			isref=fld->is_ref;
			tp=ci_get_type_for_name_try(tc,fld->type);
			if(tp)
				tpnm=tp->name;
			else
				tpnm=fld->type;
		}else{
			xfunc*fn=xtype_get_func_for_name(tp,sb->data);
			if(!fn){
				toc_print_source_location(tc,tk,4);
				printf("field or function '%s' not found in type '%s'",
						sb->data,tpnm);
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
	}
	strc_split_free(&strbs);
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
	if(!strcmp("p",funcnamne)||
		!strcmp("printf",funcnamne))
//		!strcmp("malloc",funcnamne)||
//		!strcmp("sizeof",funcnamne)||
//		!strcmp("calloc",funcnamne)||
//		!strcmp("free",funcnamne))
		return true;
	return false;
}

inline static xfunc*ci_get_func_for_accessor(const toc*tc,
						strc accessor,token tk){

	ptrs strbs=strc_split(accessor,'.');
	strb*acc=ptrs_get(&strbs,0);
	const tocdecl*td=toc_get_declaration_for_accessor(tc,acc->data);
	if(!td){// no declaration found thus func call to member or builtin or error
		strc tpnm=toc_get_typenm_in_context(tc,tk);
		xtype*tp=ci_get_type_for_name_try(tc,tpnm);
		xfunc*fn=xtype_get_func_for_name(tp,acc->data);
		if(fn){
			strc_split_free(&strbs);
			return fn;
		}
		toc_print_source_location(tc,tk,4);
		printf("function '%s' not found in type '%s'",acc->data,tp->name);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	xtype*tp=ci_get_type_for_name_try(tc,td->type);
	for(long i=1;i<strbs.count-1;i++){
		strb*sb=ptrs_get(&strbs,i);
		xfield*fld=xtype_get_field_for_name(tp,sb->data);
		if(!fld){
			toc_print_source_location(tc,tk,4);
			printf("'%s' not found in '%s'",sb->data,accessor);
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
		tp=ci_get_type_for_name_try(tc,fld->type);
	}

	strb*funcnm=ptrs_get_last(&strbs);
	xfunc*fn=xtype_get_func_for_name(tp,funcnm->data);

	if(fn){
		strc_split_free(&strbs);
		return fn;
	}

	toc_print_source_location(tc,tk,4);
	printf("cannot find function '%s' in type '%s', using accessor '%s'",
		funcnm->data,
		tp->name,
		accessor
	);
	printf("\n    %s %d",__FILE__,__LINE__);
	strc_split_free(&strbs);
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
	if(!t)
		return false;
	return xtype_is_needs_init(t);
}

inline static void ci_xcode_compile_free_current_scope(toc*tc){
	const tocscope*ts=ptrs_get_last(&tc->scopes);
	for(long i=ts->tocdecls.count-1;i>=0;i--){
		const tocdecl*td=(tocdecl*)ptrs_get(&ts->tocdecls,i);
		if(ci_is_builtin_type(td->type))
			continue;
		const xtype*t=ci_get_type_for_name_try(tc,td->type);
		if(xtype_is_needs_free(t)){
			toc_print_indent_for_compile(tc);
			printf("%s_free(&%s);",t->name,td->name);
		}
	}
}

inline static void ci_xcode_compile_free_current_loop_scope(const toc*tc,
		token tk){
	for(long j=tc->scopes.count-1;j>=0;j--){
		const tocscope*ts=(const tocscope*)ptrs_get(&tc->scopes,j);
		for(long i=ts->tocdecls.count-1;i>=0;i--){
			const tocdecl*td=(tocdecl*)ptrs_get(&ts->tocdecls,i);
			if(ci_is_builtin_type(td->type))
				continue;
			const xtype*t=ci_get_type_for_name_try(tc,td->type);
			if(!xtype_is_needs_free(t))
				continue;
			toc_print_indent_for_compile(tc);
			printf("%s_free(&%s);\n",t->name,td->name);
		}
		if(ts->type=='l') // clear including loop scope
			break;
	}
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
			if(!xtype_is_needs_free(t)) // needs free?
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

	ptrs strbs=strc_split(accessor,'.');
	strb*acc=ptrs_get(&strbs,0);
	const tocdecl*td=toc_get_declaration_for_accessor(tc,acc->data);
	if(!td){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",acc->data);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	strc typenm=td->type;
	xtype*tp=ci_get_type_for_name_try(tc,td->type);
	for(long i=1;i<strbs.count;i++){
		strb*sb=ptrs_get(&strbs,i);
		xfield*fld=xtype_get_field_for_name(tp,sb->data);
		if(!fld){
			toc_print_source_location(tc,tk,4);
			printf("field '%s' not found in type '%s' using accessor '%s'",
					sb->data,tp->name,accessor);
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
		typenm=fld->type;
		tp=ci_get_type_for_name_try(tc,fld->type);
	}

	strc_split_free(&strbs);

	return typenm;
}

inline static void ci_xset_assert(const toc*tc,const xset*o){
	strc accessor=o->name;
	strc settype=o->setexpls.super.type;
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
			xfield*fld;
			if(p){
				strb s=strb_def;
				strb_add_list(&s,current_accessor,p-current_accessor);
				strb_add(&s,0);
//				lookup=s.data;//? leak
				fld=xtype_get_field_for_name(current_type,s.data);
				strb_free(&s);
			}else{
				fld=xtype_get_field_for_name(current_type,lookup);
			}
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
	printf("cannot set '%s' to '%s' because '%s' is '%s'",
			settype,accessor,accessor,current_class_name
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


	// assignment
	if(toc_srcp_is_take(tc,'='))
		return(xexp*)xset_read_next(tc,name,tk);

	//  class instance
	xtype*c=ci_get_type_for_name_try(tc,name);
	if(c) // instantiate
		return(xexp*)xvar_read_next(tc,name);

	// function call
	if(toc_srcp_is(tc,'('))
		return(xexp*)xcall_read_next(tc,tk,name);

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
	e->ident=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=ci_get_field_type_for_accessor(tc,e->ident,tk);
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
	e->ident=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=tr.type;
	e->super.is_ref=tr.is_ref;
	return(xexp*)e;
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
//	prg->super.print_source((xexp*)prg);
	prg->super.free((xexp*)prg);
	free(prg);

	ptrs_free(&tc.types);
	ptrs_free(&tc.scopes);
	token_free();
	strb_free(&srcstr);
	ci_free();
	return 0;
}
inline static/*gives*/strb ci_get_c_accessor_for_accessor(
		const toc*tc,token tk,strc accessor){//? rewrite

	strb acc_c=strb_def;
	ptrs strbs=strc_split(accessor,'.');
	strb*acc=ptrs_get(&strbs,0);

	const tocdecl*td=toc_get_declaration_for_accessor(tc,acc->data);
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
	if(!tp && !ci_is_builtin_type(td->type)){
		toc_print_source_location(tc,tk,4);
		printf("type '%s' not found",td->type);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmp_buf,1);
	}

	bool isref=td->is_ref;

	strb_add_string(&acc_c,acc->data);

	for(long i=1;i<strbs.count;i++){
		if(isref){
			strb_add_string(&acc_c,"->");
		}else{
			strb_add(&acc_c,'.');
		}
		strb*sb=ptrs_get(&strbs,i);
		xfield*fld=xtype_get_field_for_name(tp,sb->data);
		if(!fld){
			toc_print_source_location(tc,tk,4);
			printf("field '%s' not found in '%s'",sb->data,tp->name);
			printf("\n    %s %d",__FILE__,__LINE__);
			longjmp(_jmp_buf,1);
		}
		strb_add_string(&acc_c,fld->name);
		isref=fld->is_ref;
		tp=ci_get_type_for_name_try(tc,fld->type);
	}
	strb_add(&acc_c,0);

	strc_split_free(&strbs);
	return acc_c;
}


inline static void ci_xset_compile(const toc*tc,const xset*o){
	token tk=o->super.token;
	strc id=o->name;
	strb acc_c/*takes*/=ci_get_c_accessor_for_accessor(tc,tk,id);

	ptrs split/*takes*/=strc_split(id,'.');
	strb*first_id=ptrs_get(&split,0);
	const char scopetype=toc_get_declaration_scope_type(tc,first_id->data);
	strc_split_free(&split);
	if(scopetype=='c'){// class member
		printf("o->%s=",acc_c.data);
		strb_free(&acc_c);
		return;
	}
	if(scopetype){
		printf("%s=",acc_c.data);
		strb_free(&acc_c);
		return;
	}
	strb_free(&acc_c);
	toc_print_source_location(tc,tk,4);
	printf("could not find var '%s'\n",id);
	longjmp(_jmp_buf,1);
}


inline static void ci_xcall_compile(const toc*tc,const struct xcall*c){
	token tk=c->super.token;

	//? builtins
	if(!strcmp("p",c->name) || !strcmp("printf",c->name)){
		printf("printf(");
		return;
	}
//	if(!strcmp("malloc",c->name) || !strcmp("calloc",c->name)){
//		printf("%s(",c->name);
//		return;
//	}
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

		strb/*takes*/cacc=ci_get_c_accessor_for_accessor(tc,tk,pathnm);

		if(scope=='c')
			printf("o->");
		printf("%s",cacc.data);
		if(c->args.count)
			printf(",");

		strb_free(&cacc);
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

//	if(!strcmp("free",accessor) && param_index==0)
//		return true;


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
