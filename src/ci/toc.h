#pragma once
#include"xexp.h"

typedef struct toc{
	dynp types;
	dynp scopes;
	cstr src;
	cstr srcp;
	cstr filepth;
}toc;

#include"decouple.h"
//inline static void ci_assert_set(const toc*,cstr,cstr,token);
//inline static cstr ci_get_type_for_accessor(const toc*,cstr,token);

#define toc_def {dynp_def,dynp_def,cstr_def,NULL,NULL}

typedef struct tocscope{
	char type;
	cstr name;
	dynp tocdecls;
}tocscope;

#define ci_toc_scope_def (tocscope){0,NULL,dynp_def}

typedef struct tocdecl{
	cstr type;
	cstr name;
}tocdecl;

#define toctn_def (tocdecl){cstr_def,cstr_def}


typedef struct tocloc{
	cstr filenm;
	unsigned line;
	unsigned col;
}tocloc;


inline static tocloc toc_get_line_number_from_pp(const toc*o,cstr p,
		unsigned tabsize){

	cstr pt=o->src;
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

inline static void toc_print_source_location(const toc*o,token tk,int tabsize){
	tocloc tl=toc_get_line_number_from_pp(o,tk.content,tabsize);
	printf("\n\n%s:%d:%d: ",tl.filenm,tl.line,tl.col);
}

inline static cstr toc_get_type_in_context(const toc*tc,token tk){
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&tc->scopes,(unsigned)j);
		if(s->type=='c'){
			return s->name;
		}
	}
	toc_print_source_location(tc,tk,4);
	printf("error: cannot find current class");
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}

inline static void tocscope_free(tocscope*o){
	for(unsigned i=0;i<o->tocdecls.count;i++){
		tocdecl*td=dynp_get(&o->tocdecls,i);
		free(td);
	}
	dynp_free(&o->tocdecls);
	free(o);
}

inline static void toc_push_scope(toc*o,char type,cstr name){
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

inline static void toc_print(const toc*o){
	for(unsigned j=0;j<o->scopes.count;j++){
		tocscope*s=dynp_get(&o->scopes,j);
		printf("%c %s\n",s->type,s->name);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)dynp_get(&s->tocdecls,i);
			for(unsigned k=0;k<=j;k++){
				printf("    ");
			}
			printf("%s %s\n",id->type,id->name);
		}
	}
}

inline static void toc_add_declaration(toc*o,cstr type,cstr name){
	tocdecl*id=(tocdecl*)malloc(sizeof(tocdecl));
	*id=toctn_def;
	id->type=type;
	id->name=name;
	tocscope*s=dynp_get_last(&o->scopes);
	dynp_add(&s->tocdecls,id);
//	ci_toc_print(o);
//	printf(" %s  %s  %s\n",s->name,i->type.data,i->name.data);
}

inline static char toc_get_declaration_scope_type(const toc*oo,cstr name){
	for(int j=(signed)oo->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&oo->scopes,(unsigned)j);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)dynp_get(&s->tocdecls,i);
			if(!strcmp(id->name,name))
				return s->type;
		}
	}
	return 0;
}


inline static const tocdecl*toc_get_declaration(const toc*o,cstr name){
	cstr p=strpbrk(name,".");
	cstr variable_name;
	if(p){
		str s=str_def;//? leak
		str_add_list(&s,name,p-name);
		str_add(&s,0);
		variable_name=s.data;
	}else{
		variable_name=name;
	}

	for(int j=(signed)o->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&o->scopes,(unsigned)j);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			const tocdecl*td=(const tocdecl*)dynp_get(&s->tocdecls,i);
			if(!strcmp(td->name,variable_name))
				return td;
		}
	}
	return 0;
}

inline static void toc_set_declaration_type(toc*o,cstr name,cstr type){
	for(int j=(signed)o->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&o->scopes,(unsigned)j);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)dynp_get(&s->tocdecls,i);
			if(!strcmp(id->name,name)){
				id->type=type;
				return;
			}
		}
	}
	//? exit with error
}

inline static void toc_pop_scope(toc*o){
	tocscope*s=dynp_get_last(&o->scopes);
	tocscope_free(s);
	o->scopes.count--;//? pop
}

inline static void toc_print_indent_for_compile(const toc*o){
	for(unsigned i=2;i<o->scopes.count;i++){
		printf("\t");
	}
}

inline static token toc_next_token(toc*o){return token_next(&o->srcp);}
inline static void toc_srcp_inc(toc*o){o->srcp++;}
inline static bool toc_srcp_is(const toc*o,const char ch)
	{return *o->srcp==ch;}
inline static bool toc_srcp_is_take(toc*o,const char ch){
	if(*o->srcp==ch){
		o->srcp++;
		return true;
	}
	return false;
}
inline static void toc_srcp_skip_if(toc*o,const char ch){
	if(toc_srcp_is(o,ch))
		toc_srcp_inc(o);
}
//inline static bool toc_is_type_builtin(const toc*o,cstr typenm){
//	if(!strcmp("int",typenm))return true;
//	if(!strcmp("str",typenm))return true;
//	if(!strcmp("float",typenm))return true;
//	if(!strcmp("bool",typenm))return true;
//	if(!strcmp("char",typenm))return true;
//	if(!strcmp("ccharp",typenm))return true;
////	if(!strcmp("short",typenm))return true;
////	if(!strcmp("long",typenm))return true;
////	if(!strcmp("double",typenm))return true;
//	return false;
//}



