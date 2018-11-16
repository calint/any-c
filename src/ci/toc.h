#pragma once
#include"decouple.h"
#include"xexp.h"

typedef struct toc{
	ptrs types;
	ptrs scopes;
	strc src;
	strc srcp;
	strc filepth;
	unsigned indent;
}toc;

#define toc_def {ptrs_def,ptrs_def,strc_def,NULL,NULL,0}

typedef struct tocscope{
	char type;
	strc name;
	ptrs tocdecls;
}tocscope;

#define ci_toc_scope_def (tocscope){0,NULL,ptrs_def}

typedef struct tocdecl{
	token token;
	strc type;
	strc name;
	bool is_ref;
}tocdecl;

#define toctn_def (tocdecl){token_def,strc_def,strc_def,false}


typedef struct tocloc{
	strc filenm;
	int line;
	int col;
}tocloc;


inline static tocloc toc_get_line_number_in_src(const toc*o,strc p,
		int tabsize){//? rewrite

	strc pt=o->src;
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
	tocloc tl=toc_get_line_number_in_src(o,tk.content,tabsize);
	printf("\n\n%s:%d:%d: ",tl.filenm,tl.line,tl.col);
}

inline static strc toc_get_typenm_in_context(const toc*tc,token tk){
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&tc->scopes,(unsigned)j);
		if(s->type=='c'){
			return s->name;
		}
	}
	toc_print_source_location(tc,tk,4);
	printf("error: cannot find current class");
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}

inline static tocscope*toc_get_loop_scope_in_context(const toc*tc,token tk){
	for(int j=(signed)tc->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&tc->scopes,(unsigned)j);
		if(s->type=='l'){
			return s;
		}
	}
	toc_print_source_location(tc,tk,4);
	printf("error: cannot find current loop");
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}

inline static void tocscope_free(tocscope*o){
	for(unsigned i=0;i<o->tocdecls.count;i++){
		tocdecl*td=ptrs_get(&o->tocdecls,i);
		free(td);
	}
	ptrs_free(&o->tocdecls);
	free(o);
}

inline static void toc_push_scope(toc*o,char type,strc name){
	tocscope*s=malloc(sizeof(tocscope));
	*s=ci_toc_scope_def;
	s->type=type;
	s->name=name;
	ptrs_add(&o->scopes,s);
	if(s->type!='l')
		o->indent++;


//	printf("//");
//	for(unsigned i=0;i<o->scopes.count;i++){
//			ci_toc_scope*s=(ci_toc_scope*)ptrs_get(&o->scopes,i);
//			printf(" %s :: ",s->name);
//	}
//	printf("\n");
}

inline static void toc_print(const toc*o){
	for(unsigned j=0;j<o->scopes.count;j++){
		tocscope*s=ptrs_get(&o->scopes,j);
		printf("%c %s\n",s->type,s->name);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)ptrs_get(&s->tocdecls,i);
			for(unsigned k=0;k<=j;k++){
				printf("    ");
			}
			printf("%s %s\n",id->type,id->name);
		}
	}
}

inline static void toc_add_declaration(toc*o,strc type,bool is_ref,strc name){
	tocdecl*id=(tocdecl*)malloc(sizeof(tocdecl));
	*id=toctn_def;
	id->type=type;
	id->name=name;
	id->is_ref=is_ref;
	tocscope*s=ptrs_get_last(&o->scopes);
	ptrs_add(&s->tocdecls,id);
//	ci_toc_print(o);
//	printf(" %s  %s  %s\n",s->name,i->type.data,i->name.data);
}

inline static char toc_get_declaration_scope_type(const toc*oo,token tk,strc name){
	for(long j=oo->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&oo->scopes,j);
		for(long i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)ptrs_get(&s->tocdecls,i);
			if(!strcmp(id->name,name))
				return s->type;
		}
	}
	toc_print_source_location(oo,tk,4);
	printf("\ndeclaration '%s' not found in scope",name);
	printf("\n    %s %d",__FILE__,__LINE__);
	longjmp(_jmp_buf,1);
}

inline static bool toc_is_declared(const toc*oo,strc name){
	for(long j=oo->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&oo->scopes,j);
		for(long i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)ptrs_get(&s->tocdecls,i);
			if(!strcmp(id->name,name))
				return true;
		}
	}
	return false;
}

inline static bool toc_is_declaration_ref(const toc*oo,strc name){
	for(int j=(signed)oo->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&oo->scopes,(unsigned)j);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)ptrs_get(&s->tocdecls,i);
			if(!strcmp(id->name,name))
				return id->is_ref;
		}
	}
//	toc_print_source_location(oo,tk,4);
//	printf("\ncompiler error: declaration '%s' not found in scope",name);
//	printf("\n    %s %d",__FILE__,__LINE__);
//	longjmp(_jmp_buf,1);
	return 0;
}


inline static const tocdecl*toc_get_declaration_for_accessor(
		const toc*o,strc name){
	strc p=strpbrk(name,".");
	strc variable_name;
	if(p){
		strb s=strb_def;//? leak
		strb_add_list(&s,name,p-name);
		strb_add(&s,0);
		variable_name=s.data;
		ptrs_add(&_token_to_new_strc_,s.data);//? adhock
	}else{
		variable_name=name;
	}

	for(long j=o->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&o->scopes,j);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			const tocdecl*td=(const tocdecl*)ptrs_get(&s->tocdecls,i);
			if(!strcmp(td->name,variable_name))
				return td;
		}
	}

//	toc_print_source_location(o,tk,4);
//	printf("declaration for '%s' not found",name);
//	printf("\n    %s %d",__FILE__,__LINE__);
//	longjmp(_jmp_buf,1);

	return NULL;
}

inline static void toc_set_declaration_type(toc*o,strc name,strc type){
	for(int j=(signed)o->scopes.count-1;j>=0;j--){
		tocscope*s=ptrs_get(&o->scopes,(unsigned)j);
		for(unsigned i=0;i<s->tocdecls.count;i++){
			tocdecl*id=(tocdecl*)ptrs_get(&s->tocdecls,i);
			if(!strcmp(id->name,name)){
				id->type=type;
				return;
			}
		}
	}
	//? exit with error
}

inline static void toc_pop_scope(toc*o){
	tocscope*s=ptrs_get_last(&o->scopes);
	if(s->type!='l')
		o->indent--;
	tocscope_free(s);
	o->scopes.count--;//? pop
}

inline static void toc_print_indent_for_compile(const toc*o){
	for(unsigned i=2;i<o->indent;i++){
		printf("    ");
	}
}

inline static token toc_next_token(toc*o){return token_next(&o->srcp);}
inline static void toc_srcp_inc(toc*o){o->srcp++;}
inline static bool toc_srcp_is(const toc*o,const char ch){
	return *o->srcp==ch;
}
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

inline static void toc_read_to_end_of_line(toc*o){
	while(*o->srcp!='\n'){
		o->srcp++;
	}
}

