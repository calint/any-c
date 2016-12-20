#pragma once
#include"xexp.h"

typedef struct toc{
	dynp types;
	dynp scopes;
	str src;
	ccharp srcp;
	ccharp filepth;
}toc;

inline static void ci_assert_set(toc*,ccharp,ccharp,token);
inline static ccharp ci_get_type_for_accessor(toc*,ccharp,token);

#define toc_def {dynp_def,dynp_def,str_def,NULL,NULL}

typedef struct tocscope{
	char type;
	ccharp name;
	dynp idents;
}tocscope;

#define ci_toc_scope_def (tocscope){0,NULL,dynp_def}

typedef struct tocdecl{
	str type;
	str name;
}tocdecl;

#define toctn_def (tocdecl){str_def,str_def}


typedef struct tocloc{
	ccharp filenm;
	unsigned line;
	unsigned col;
}tocloc;


inline static tocloc toc_get_line_number_from_pp(toc*o,ccharp p,
		unsigned tabsize){

	ccharp pt=o->src.data;
	int line=1,col=1;
	while(pt<p){
		if(*pt!='\n'){
			if(*pt=='\t')
				col+=tabsize;
			else
				col++;
			pt++;
			continue;
		}
		col=1;
		line++;
		pt++;
	}
	return (tocloc){o->filepth,line,col};
}

inline static void toc_print_source_location(toc*o,token tk,int tabsize){
	tocloc tl=toc_get_line_number_from_pp(o,tk.content,tabsize);
	printf("\n\n%s:%d:%d: ",tl.filenm,tl.line,tl.col);
}

inline static ccharp toc_get_type_in_context(toc*tc,token tk){
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
		if(s->type=='c'){
			return s->name;
		}
	}
	toc_print_source_location(tc,tk,4);
	printf("error: cannot find current class");
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmpbufenv,1);
}

inline static void toc_scope_free(tocscope*o){
	dynp_free(&o->idents);
	free(o);
}

inline static void toc_push_scope(toc*o,char type,ccharp name){
	tocscope*s=malloc(sizeof(tocscope));
	*s=ci_toc_scope_def;
	s->type=type;
	s->name=name;
	dynp_add(&o->scopes,s);

//	printf("//");
//	for(unsigned i=0;i<o->scopes.count;i++){
//			ci_toc_scope*s=(ci_toc_scope*)dynp_get(&o->scopes,i);
//			printf(" %s :: ",s->name);
//	}
//	printf("\n");
}

inline static void toc_print(toc*o){
	for(unsigned j=0;j<o->scopes.count;j++){
		tocscope*s=dynp_get(&o->scopes,j);
		printf("%c %s\n",s->type,s->name);
		for(unsigned i=0;i<s->idents.count;i++){
			tocdecl*id=(tocdecl*)dynp_get(&s->idents,i);
			for(unsigned k=0;k<=j;k++){
				printf("    ");
			}
			printf("%s %s\n",id->type.data,id->name.data);
		}
	}
}

inline static void toc_add_ident(toc*o,ccharp type,ccharp name){
	tocdecl*id=(tocdecl*)malloc(sizeof(tocdecl));
	*id=toctn_def;
	str_setz(&id->type,type);
	str_setz(&id->name,name);
	tocscope*s=dynp_get_last(&o->scopes);
	dynp_add(&s->idents,id);
//	ci_toc_print(o);
//	printf(" %s  %s  %s\n",s->name,i->type.data,i->name.data);
}

inline static char toc_get_declaration_scope_type(toc*oo,ccharp name){
	for(int j=(signed)oo->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&oo->scopes,(unsigned)j);
		for(unsigned i=0;i<s->idents.count;i++){
			tocdecl*id=(tocdecl*)dynp_get(&s->idents,i);
			if(!strcmp(id->name.data,name))
				return s->type;
		}
	}
	return 0;
}


inline static const tocdecl*toc_get_declaration(toc*o,ccharp name){
	ccharp p=strpbrk(name,".");
	ccharp variable_name;
	if(p){
		str s=str_def;//? leakcase
		str_add_list(&s,name,p-name);
		str_add(&s,0);
		variable_name=s.data;
	}else{
		variable_name=name;
	}

	for(int j=(signed)o->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&o->scopes,(unsigned)j);
		for(unsigned i=0;i<s->idents.count;i++){
			const tocdecl*td=(const tocdecl*)dynp_get(&s->idents,i);
			if(!strcmp(td->name.data,variable_name))
				return td;
		}
	}
	return 0;
}

inline static void toc_set_declaration_type(toc*o,ccharp name,ccharp type){
	for(int j=(signed)o->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&o->scopes,(unsigned)j);
		for(unsigned i=0;i<s->idents.count;i++){
			tocdecl*id=(tocdecl*)dynp_get(&s->idents,i);
			if(!strcmp(id->name.data,name)){
				id->type=const_str(type);
				return;
			}
		}
	}
	//? exit with error
}

inline static void toc_pop_scope(toc*o){
	tocscope*s=dynp_get_last(&o->scopes);
	toc_scope_free(s);
	o->scopes.count--;//? pop
}

inline static void toc_indent_for_compile(toc*o){
	for(unsigned i=2;i<o->scopes.count;i++){
		printf("\t");
	}
}

inline static token toc_next_token(toc*o){return token_next(&o->srcp);}
inline static void toc_srcp_inc(toc*o){o->srcp++;}
inline static bool toc_srcp_is(const toc*o,const char ch)
	{return *o->srcp==ch;}
inline static bool toc_srcp_if_is_then_take(toc*o,const char ch){
	if(*o->srcp==ch){
		o->srcp++;
		return true;
	}
	return false;
}
inline static void toc_charp_skip_if(toc*o,const char ch){
	if(toc_srcp_is(o,ch))
		toc_srcp_inc(o);
}
inline static bool toc_is_type_builtin(const toc*o,ccharp typenm){
	if(!strcmp("int",typenm))return true;
	if(!strcmp("str",typenm))return true;
	if(!strcmp("float",typenm))return true;
	if(!strcmp("bool",typenm))return true;
	if(!strcmp("char",typenm))return true;
	if(!strcmp("ccharp",typenm))return true;
//	if(!strcmp("short",typenm))return true;
//	if(!strcmp("long",typenm))return true;
//	if(!strcmp("double",typenm))return true;
	return false;
}



