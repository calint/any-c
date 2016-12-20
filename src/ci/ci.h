#pragma once
#include<setjmp.h>
#include"xcall.h"
#include"xident.h"
#include"xife.h"
#include"xloop.h"
#include"xvar.h"
#include"toc.h"
#include"xconst.h"
#include"xexpls.h"
#include"xreturn.h"
#include"xbreak.h"
#include "xcode.h"
#include"xcont.h"
#include "xtype.h"

inline static xtype*toc_get_type_by_name(toc*o,ccharp name){
	for(unsigned i=0;i<o->types.count;i++){
		xtype*c=dynp_get(&o->types,i);
		if(!strcmp(c->name.data,name)){
			return c;
		}
	}
	return NULL;
}
//inline static ccharp toc_get_type_in_context(toc*tc,token tk){
//	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
//		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
//		if(s->type=='c'){
//			return s->name;
//		}
//	}
//	toc_print_source_location(tc,tk,4);
//	printf("error: cannot find current class");
//	printf("\n    %s %d",__FILE__,__LINE__);
//	longjmp(_jmpbufenv,1);
//}
//inline static void toc_print_func_call_for_compile(toc*tc,
//		token tk,ccharp funcnm){
//	// get current class name
//	ccharp typenm=0;
//	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
//		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
//		if(s->type=='c'){
//			typenm=s->name;
//			break;
//		}
//	}
//
//	if(typenm==0){
//		toc_print_source_location(tc,tk,4);
//		printf("could not find class scope");
//		//? print toc
//		longjmp(_jmpbufenv,1);
//	}
//
//	type*tp=toc_get_type_by_name(tc,typenm);
//	if(!tp){
//		toc_print_source_location(tc,tk,4);
//		printf("cannot find class scope");
//		printf("\n    %s %d",__FILE__,__LINE__);
//		longjmp(_jmpbufenv,1);
//	}
//
//	func*fc=type_get_func_by_name(tp,funcnm);
//	if(!fc){
//		toc_print_source_location(tc,tk,4);
//		printf("cannot find function '%s' in '%s'",funcnm,typenm);
//		printf("\n    %s %d",__FILE__,__LINE__);
//		longjmp(_jmpbufenv,1);
//	}
//
//	printf("%s_%s((%s*)&%s",typenm,funcnm,typenm,"o");
//}

inline static void toc_add_type(toc*o,xtype*c){
	dynp_add(&o->types,c);
}

inline static ccharp toc_get_type_for_accessor(toc*tc,
			ccharp accessor,token tk){

	ccharp current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmpbufenv,1);
	}
	ccharp current_class_name=decl->type.data;
	const xtype*current_type=toc_get_type_by_name(tc,current_class_name);
	if(current_type){
		while(1){
			ccharp p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			ccharp lookup=current_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,current_accessor,p-current_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=type_get_field_by_name(current_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						current_accessor,
						current_type->name.data,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmpbufenv,1);
			}
			current_class_name=fld->type.data;
			current_type=toc_get_type_by_name(tc,current_class_name);
			if(!current_type)
				break;
		}
	}
	return current_class_name;
}

inline static void toc_assert_can_set(toc*tc,
		ccharp accessor,ccharp settype,token tk){

	ccharp current_accessor=accessor;
	const tocdecl*decl=toc_get_declaration(tc,current_accessor);
	if(!decl){
		toc_print_source_location(tc,tk,4);
		printf("'%s' not found",current_accessor);
		printf("\n    %s %d",__FILE__,__LINE__);
		longjmp(_jmpbufenv,1);
	}

	if(!strcmp(decl->type.data,"var"))// if dest is var
		return;

	ccharp current_class_name=decl->type.data;
	const xtype*current_type=toc_get_type_by_name(tc,current_class_name);
	if(current_type){
		while(1){
			ccharp p=strpbrk(current_accessor,".");// p.anim.frame=2   vs  a=2
			if(!p){// a=2
				break;
			}
			current_accessor=p+1; // anim.frame
			p=strpbrk(current_accessor,".");
			ccharp lookup=current_accessor;
			if(p){
				str s=str_def;
				str_add_list(&s,current_accessor,p-current_accessor);
				str_add(&s,0);
				lookup=s.data;//? leak
			}
			const xfield*fld=type_get_field_by_name(current_type,lookup);
			if(!fld){
				toc_print_source_location(tc,tk,4);
				printf("cannot find field '%s' in '%s', using '%s'",
						current_accessor,
						current_type->name.data,
						accessor
				);
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmpbufenv,1);
			}
			current_class_name=fld->type.data;
			current_type=toc_get_type_by_name(tc,current_class_name);
			if(!current_type)
				break;
		}
	}

	if(!current_type)// builtin
		if(toc_is_type_builtin(tc,current_class_name))
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
	longjmp(_jmpbufenv,1);
}
//
//inline static bool toc_can_assign(toc*tc,
//		ccharp to_typenm,
//		ccharp accessor,
//		ccharp from_typenm){
//
//	type*c=toc_get_type_by_name(tc,to_typenm);
//	ccharp endptr=accessor;
//	while(1){
//		ccharp p=strpbrk(endptr,".");
//		str ident=str_def;
//		if(p){
//			str_add_list(&ident,endptr,p-endptr);
//			str_add(&ident,0);
//			endptr=p+1;
//		}else{
//			str_add_list(&ident,endptr,strlen(endptr));
//			str_add(&ident,0);
//		}
//
//		if(!c){// base type or class not found
//			return !strcmp(to_typenm,from_typenm);
//		}
//		bool found=false;
//		for(unsigned i=0;i<c->fields.count;i++){
//			field*fld=(field*)dynp_get(&c->fields,i);
//			if(!strcmp(fld->name.data,ident.data)){
//				c=toc_get_type_by_name(tc,fld->type.data);
//				if(!c){
//					if(!from_typenm)
//						return true;//?
//					return !strcmp(fld->type.data,from_typenm);
//				}
//				found=true;
//				break;
//			}
//		}
//		if(found)continue;
//		printf("<file> <line:col> cannot find '%s' in '%s'\n",
//				accessor,c->name.data);
//		longjmp(_jmpbufenv,1);
//	}
//}

inline static void ci_init(){}

inline static void ci_free(){}

//inline static xexpls*toc_read_next_xexpls(toc*tc,token tk){
//	return xexpls_read_next(tc,tk);
//}

inline static xexp*toc_read_next_xexpr(toc*tc){
	token tk=toc_next_token(tc);
	if(token_is_empty(&tk)){
		if(toc_is_srcp_take(tc,'"')){ // string
			while(1){
				const char c=*tc->srcp;
				if(c==0){
					printf("<file> <line:col> did not find the "
							" end of string\n");
					longjmp(_jmpbufenv,1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					longjmp(_jmpbufenv,1);
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
			token_setz(&tk,&e->name);
			e->super.type=str_from_string("ccharp");
			return(xexp*)e;

		}else if(toc_is_srcp_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(*tc->srcp!='\''){
				toc_print_source_location(tc,tk,4);
				printf("expected a character, example 'a'");
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmpbufenv,1);
			}
			toc_srcp_inc(tc);
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			token_setz(&tk,&e->name);
			e->super.type=str_from_string("char");
			return(xexp*)e;

		}else{
			xexp*e=malloc(sizeof(xexp));
			*e=xexp_def;
			return e;
		}
	}

	// constant
	str tks=str_def;
	token_setz(&tk,&tks);

	// boolean
	if(!strcmp("true",tks.data) || !strcmp("false",tks.data)){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type=const_str("bool");
		return(xexp*)e;
	}

	// int
	char*endptr;strtol(tks.data,&endptr,10);
	if(endptr==tks.data+tks.count-1){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type=const_str("int");
		return(xexp*)e;
	}

	// float
	strtof(tks.data,&endptr);
	if(*endptr=='f' && tks.count>2){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type=const_str("float");
		return(xexp*)e;
	}

	// hex
	if(tks.count>1){
		if((tks.data[0]=='0' && tks.data[1]=='x')){
			strtol(tks.data+2,&endptr,16);
			if(endptr==tks.data+tks.count-2+1){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type=const_str("int");
				return(xexp*)e;
			}
		}
		if((tks.data[0]=='0' && tks.data[1]=='b')){
			strtol(tks.data+2,&endptr,2);
			if(endptr==tks.data+tks.count-2+1){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type=const_str("int");
				return(xexp*)e;
			}
		}
	}

	// keywords
	if(token_equals(&tk,"loop")){
		xloop*e=xloop_read_next(tc);
		e->super.token=tk;
		return (xexp*)e;
	}

	if(token_equals(&tk,"break")){
		xbreak*e=xbreak_read_next(tc);
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
	str name=str_def;
	token_setz(&tk,&name);
	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
			token_equals(&tk,"bool")||token_equals(&tk,"char")||
			token_equals(&tk,"var")||token_equals(&tk,"ccharp")){//const char*
		xvar*e=xvar_read_next(tc,name);
		return(xexp*)e;
	}

	//  class instance
	xtype*c=toc_get_type_by_name(tc,name.data);
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
	e->super.type=const_str(toc_get_type_for_accessor(tc,e->name.data,tk));
	return(xexp*)e;
}

inline static xexp*toc_read_next_expression(toc*tc){
	token tk=toc_next_token(tc);
	if(token_is_empty(&tk)){
		if(toc_is_srcp_take(tc,'"')){ // string
			while(1){
				const char c=*tc->srcp;
				if(c==0){
					printf("<file> <line:col> did not find the "
							" end of string\n");
					longjmp(_jmpbufenv,1);
				}
				if(c=='\n'){
					printf("<file> <line:col> did not find the "
							" end of string on the same line\n");
					longjmp(_jmpbufenv,1);
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
			token_setz(&tk,&e->name);
			e->super.type=str_from_string("ccharp");
			return(xexp*)e;

		}else if(toc_is_srcp_take(tc,'\'')){
			toc_srcp_inc(tc);
			if(*tc->srcp!='\''){
				toc_print_source_location(tc,tk,4);
				printf("expected a character, example 'a'");
				printf("\n    %s %d",__FILE__,__LINE__);
				longjmp(_jmpbufenv,1);
			}
			toc_srcp_inc(tc);
			tk.end=tk.content_end=tc->srcp;

			xconst*e=malloc(sizeof(xconst));
			*e=xconst_def;
			e->super.token=tk;
			token_setz(&tk,&e->name);
			e->super.type=str_from_string("char");
			return(xexp*)e;

		}else{
			xexp*e=malloc(sizeof(xexp));
			*e=xexp_def;
			return e;
		}
	}

	// constant
	str tks=str_def;
	token_setz(&tk,&tks);

	// boolean
	if(!strcmp("true",tks.data) || !strcmp("false",tks.data)){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type=const_str("bool");
		return(xexp*)e;
	}

	// int
	char*endptr;strtol(tks.data,&endptr,10);
	if(endptr==tks.data+tks.count-1){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type=const_str("int");
		return(xexp*)e;
	}

	// float
	strtof(tks.data,&endptr);
	if(*endptr=='f' && tks.count>2){
		xconst*e=malloc(sizeof(xconst));
		*e=xconst_def;
		e->super.token=tk;
		e->name=tks;
		e->super.type=const_str("float");
		return(xexp*)e;
	}

	// hex
	if(tks.count>1){
		if((tks.data[0]=='0' && tks.data[1]=='x')){
			strtol(tks.data+2,&endptr,16);
			if(endptr==tks.data+tks.count-2+1){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type=const_str("int");
				return(xexp*)e;
			}
		}
		if((tks.data[0]=='0' && tks.data[1]=='b')){
			strtol(tks.data+2,&endptr,2);
			if(endptr==tks.data+tks.count-2+1){
				xconst*e=malloc(sizeof(xconst));
				*e=xconst_def;
				e->super.token=tk;
				e->name=tks;
				e->super.type=const_str("int");
				return(xexp*)e;
			}
		}
	}
//
//	// keywords
//	if(token_equals(&tk,"loop")){
//		xloop*e=xloop_read_next(tc);
//		e->super.token=tk;
//		return (xexpr*)e;
//	}
//
//	if(token_equals(&tk,"break")){
//		xbreak*e=xbreak_read_next(tc);
//		e->super.token=tk;
//		return (xexpr*)e;
//	}
//
//	if(token_equals(&tk,"continue")){
//		xcont*e=xcont_read_next(tc,tk);
//		return (xexpr*)e;
//	}
//
//	if(token_equals(&tk,"if")){
//		xife*e=xife_read_next(tc,tk);
//		return (xexpr*)e;
//	}

	// built in types
	str name=str_def;
	token_setz(&tk,&name);
//	if(token_equals(&tk,"int")||token_equals(&tk,"float")||
//			token_equals(&tk,"bool")||token_equals(&tk,"char")||
//			token_equals(&tk,"var")||token_equals(&tk,"ccharp")){//const char*
//		xvar*e=xvar_read_next(tc,name);
//		return(xexpr*)e;
//	}

	//  class instance
//	type*c=toc_get_type_by_name(tc,name.data);
//	if(c){// instantiate
//		xvar*e=xvar_read_next(tc,name);
//		return(xexpr*)e;
//	}

	// function call
	if(*tc->srcp=='('){
		xcall*e=xcall_read_next(tc,tk,name);
		return(xexp*)e;
	}

	// assignment
//	if(*tc->srcp=='='){
//		tc->srcp++;
//		if(*tc->srcp!='='){
//			xset*e=xset_read_next(tc,name,tk);
//			return(xexpr*)e;
//		}
//		tc->srcp--;
//	}

	char incdecbits=0;
	if(toc_is_srcp_take(tc,'+')){
		if(toc_is_srcp_take(tc,'+')){
			incdecbits|=1;
		}else{
			tc->srcp--;
		}
	}else if(toc_is_srcp_take(tc,'-')){
		if(toc_is_srcp_take(tc,'-')){
			incdecbits|=2;
		}else{
			tc->srcp--;
		}
	}

	xident*e=malloc(sizeof(xident));
	*e=xident_def;
	e->name=name;
	e->super.token=tk;
	e->incdecbits=incdecbits;
	e->super.type=const_str(toc_get_type_for_accessor(tc,e->name.data,tk));
	return(xexp*)e;
}

inline static void toc_parse_func(toc*tc,xtype*c,token*type){
	xfunc*f=malloc(sizeof(xfunc));
	*f=xfunc_def;
	bool enclosed_args=false;
	if(*tc->srcp=='{' || *tc->srcp=='('){
		f->type=const_str("void");
		token_setz(type,&f->name);
	}else{
		token tkname=toc_next_token(tc);
		token_setz(type,&f->type);
		token_setz(&tkname,&f->name);
	}

	dynp_add(&c->funcs,f);

	if(*tc->srcp=='('){
		enclosed_args=true;
		tc->srcp++;
	}

	toc_push_scope(tc,'f',f->name.data);
	while(1){
		token tkt=toc_next_token(tc);
		if(token_is_empty(&tkt)){
			break;
		}
		xfuncarg*fa=malloc(sizeof(xfuncarg));
		dynp_add(&f->args,fa);
		*fa=xfuncarg_def;
		token tkn=toc_next_token(tc);
		token_setz(&tkt,&fa->type);
		token_setz(&tkn,&fa->name);

		toc_add_ident(tc,fa->type.data,fa->name.data);

		if(*tc->srcp==','){
			tc->srcp++;
		}
	}
	if(enclosed_args){
		if(*tc->srcp==')'){
			tc->srcp++;
		}else{
			printf("<file> <line:col> expected ')' after arguments\n");
			longjmp(_jmpbufenv,1);
		}
	}
	code_read_next(&f->code,tc);
	toc_pop_scope(tc);
}

inline static void toc_parse_field(toc*tc,xtype*c,token*type,token*name){
	xfield*f=malloc(sizeof(xfield));
	*f=xfield_def;
	token_setz(type,&f->type);
	if(token_is_empty(name)){
		token_setz(type,&f->name);
	}else{
		token_setz(name,&f->name);
	}
	dynp_add(&c->fields,f);
	toc_add_ident(tc,f->type.data,f->name.data);
	if(*tc->srcp=='='){
		tc->srcp++;
		xexpls_parse_next(&f->initval, tc,*type);
//		f->initval=e;
		if(strcmp(f->type.data,"var")){
				toc_assert_can_set(tc,
					f->name.data,
					f->initval.super.type.data,
					f->initval.super.token);
		}
		f->type=f->initval.super.type;
	}
	if(*tc->srcp==';'){
		tc->srcp++;
	}
	return;
}

inline static xtype*toc_parse_type(toc*tc,token name){
	xtype*c=malloc(sizeof(xtype));
	*c=xtype_def;
	toc_add_type(tc,c);
	c->token=name;
	token_setz(&c->token,&c->name);
	toc_push_scope(tc,'c',c->name.data);
	if(!toc_is_srcp(tc,'{')){
		toc_print_source_location(tc,c->token,4);
		printf("expected '{' to open class body\n");
		longjmp(_jmpbufenv,1);
	}
	toc_srcp_inc(tc);
	while(1){
		token type=toc_next_token(tc);
		if(token_is_empty(&type)){
			if(*tc->srcp!='}'){
				printf("<file> <line:col> expected '}' to close class body\n");
				longjmp(_jmpbufenv,1);
			}
			tc->srcp++;
			break;
		}
		if(toc_is_srcp(tc,'(') || toc_is_srcp(tc,'{')){
			toc_parse_func(tc,c,&type);
		}else if(toc_is_srcp(tc,'=') || toc_is_srcp(tc,';')){
			token name=toc_next_token(tc);
			toc_parse_field(tc,c,&type,&name);
		}else{
			token name=toc_next_token(tc);
			if(toc_is_srcp(tc,'(') || toc_is_srcp(tc,'{')){
				tc->srcp=name.content;
				toc_parse_func(tc,c,&type);
			}else{
				toc_parse_field(tc,c,&type,&name);
			}
		}
	}
	toc_pop_scope(tc);
	return c;
}

inline static void _print_right_aligned_comment(ccharp comment){
	ccharp line="--- - - -------------------  - -- - - - - - - -- - - - -- - - - -- - - -- ---";
	const int maxlen=strlen(line);
	const int ln=strlen(comment);
	int start_at=maxlen-ln-4;
	if(start_at<0)start_at=0;
	printf("//");
	printf("%.*s %s\n",start_at,line,comment);
}

inline static void toc_compile_to_c(toc*tc){
	_print_right_aligned_comment("generated c source");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("#include<stdlib.h>\n");
	printf("#include<stdio.h>\n");
	printf("typedef char bool;\n");
	printf("typedef const char* ccharp;\n");
	printf("#define true 1\n");
	printf("#define false 1\n");
	printf("#define char_def 0\n");
	printf("#define var_def 0\n");
	printf("#define int_def 0\n");
	printf("#define float_def 0.0f\n");
	printf("#define bool_def false\n");
	for(unsigned i=0;i<tc->types.count;i++){
		xtype*c=dynp_get(&tc->types,i);
		toc_push_scope(tc,'c',c->name.data);
		// type
		_print_right_aligned_comment(c->name.data);
		printf("typedef struct %s{",c->name.data);
		// fields
		if(c->fields.count)printf("\n");
		for(unsigned i=0;i<c->fields.count;i++){
			xfield*f=(xfield*)dynp_get(&c->fields,i);
			toc_add_ident(tc,f->type.data,f->name.data);
			if(!strcmp(f->type.data,"var")){
				if(!f->initval.exprs.count){
					printf("<file> <line:col> expected initializer with auto");
					longjmp(_jmpbufenv,1);
				}
				f->initval.super.compile((xexp*)&f->initval,tc);
				f->type=f->initval.super.type;
			}
			printf("    %s %s;\n",f->type.data,f->name.data);
		}
		printf("}%s;\n",c->name.data);

		// #define object_def {...}
		printf("#define %s_def (%s){",c->name.data,c->name.data);
		for(unsigned i=0;i<c->fields.count;i++){
			xfield*s=(xfield*)dynp_get(&c->fields,i);
			if(s->initval.exprs.count){
				s->initval.super.compile((xexp*)&s->initval,tc);
			}else{
				printf("%s_def",s->type.data);
			}
			if(i!=c->fields.count-1)
				printf(",");
		}
		printf("}\n");

		// functions
		if(c->funcs.count){
			_print_right_aligned_comment("funcs");
			for(unsigned i=0;i<c->funcs.count;i++){
				xfunc*f=(xfunc*)dynp_get(&c->funcs,i);
				toc_push_scope(tc,'f',f->name.data);
				printf("inline static %s %s_%s(%s*o",
						f->type.data,c->name.data,f->name.data,c->name.data);
				for(unsigned j=0;j<f->args.count;j++){
					xfuncarg*a=(xfuncarg*)dynp_get(&f->args,j);
					printf(",");
					printf("%s %s",a->type.data,a->name.data);
					toc_add_ident(tc,a->type.data,a->name.data);
				}
				printf(")");
				f->code.super.compile((xexp*)&f->code,tc);
				printf("\n");
				toc_pop_scope(tc);
			}
		}
		toc_pop_scope(tc);
	}
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
	printf("int main(int c,char** a){global_main(0);}\n");
	printf("//--- - - ---------------------  - -- - - - - - - -- - - - -- - - - -- - - -\n");
}

inline static int toc_compile_file(ccharp path){
	int val=setjmp(_jmpbufenv);
	if(val==1){
//		printf(" error occured");
		return -1;
	}

	toc tc=toc_def;
	tc.filepth=path;
	tc.src=str_from_file(path);
	tc.srcp=tc.src.data;
	while(1){
		token typenm=toc_next_token(&tc);
		if(token_is_empty(&typenm))break;
		toc_parse_type(&tc,typenm);
	}

	toc_compile_to_c(&tc);

	return 0;
}


