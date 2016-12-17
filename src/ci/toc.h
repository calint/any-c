#pragma once
#include"../lib.h"

typedef struct toc{
	dynp types;
	dynp scopes;
	str src;
}toc;

#define toc_def {dynp_def,dynp_def,str_def}

inline static bool toc_can_assign(toc*o,const char*dst,const char*path,
		const char*src);

typedef struct tocscope{
	char type;
	const char*name;
	dynp/*const char**/idents;
}tocscope;

#define ci_toc_scope_def (tocscope){0,NULL,dynp_def}

typedef struct tocdecl{
	str type;
	str name;
}tocdecl;

#define toctn_def (tocdecl){str_def,str_def}

inline static void toc_scope_free(tocscope*o){
	dynp_free(&o->idents);
	free(o);
}

inline static void toc_push_scope(toc*o,char type,const char*name){
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

inline static void toc_add_ident(toc*o,const char*type,const char*name){
	tocdecl*id=(tocdecl*)malloc(sizeof(tocdecl));
	*id=toctn_def;
	str_setz(&id->type,type);
	str_setz(&id->name,name);
	tocscope*s=dynp_get_last(&o->scopes);
	dynp_add(&s->idents,id);
//	ci_toc_print(o);
//	printf(" %s  %s  %s\n",s->name,i->type.data,i->name.data);
}


inline static char toc_find_ident_scope_type(toc*oo,const char*name){
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


inline static const tocdecl*toc_find_ident(toc*oo,const char*name){
	for(int j=(signed)oo->scopes.count-1;j>=0;j--){
		tocscope*s=dynp_get(&oo->scopes,(unsigned)j);
		for(unsigned i=0;i<s->idents.count;i++){
			const tocdecl*id=(const tocdecl*)dynp_get(&s->idents,i);
			if(!strcmp(id->name.data,name))
				return id;
		}
	}
	return 0;
}

inline static void toc_pop_scope(toc*o){
	tocscope*s=dynp_get_last(&o->scopes);
//	printf("    popped   %s  \n\n\n",s->name);
	toc_scope_free(s);
	o->scopes.count--;//? pop
}

inline static void toc_compile_for_xset(
		toc*tc,const char*id,const char*type){

	const char*p=strpbrk(id,".");
	if(p){
		str sid=str_def;
		str_add_list(&sid,id,p-id);
		str_add(&sid,0);

		const tocdecl*i=toc_find_ident(tc,sid.data);
		if(!i){
			printf("<file> <line:col> identifier '%s' not found\n",id);
			exit(1);
		}
		if(!toc_can_assign(tc,i->type.data,p+1,type)){
			printf("<file> <line:col> cannot assign '%s' to '%s'\n",
					type,i->type.data);
			exit(1);
		}
		const char scopetype=toc_find_ident_scope_type(tc,sid.data);
		if(scopetype){
			if(scopetype=='c'){// class member
				printf("o->%s",id);
			}else{// local identifier
				printf("%s",id);
			}
			printf("=");
			return;
		}
	}
	const char scopetype=toc_find_ident_scope_type(tc,id);
	if(scopetype=='c'){// class member
		printf("o->%s=",id);
		return;
	}
	if(scopetype){// local identifier
		printf("%s=",id);
		return;
	}

	printf("<file> <line:col> could not find identifier: %s\n",id);
	exit(1);
}

inline static void toc_compile_for_call(
		toc*tc,const char*id,unsigned argcount){

	const char*p=strpbrk(id,".");
	if(p){
		str sid=str_def;
		str_add_list(&sid,id,p-id);
		str_add(&sid,0);

		const tocdecl*i=toc_find_ident(tc,sid.data);
		if(!i){
			printf("<file> <line:col> identifier not found: %s\n",sid.data);
			exit(1);
		}
		printf("%s_%s((%s*)&%s",
				i->type.data,p+1,
				i->type.data,sid.data);
		if(argcount){
			printf(",");
		}
	}else{
		printf("%s(",id);
	}
}


inline static void toc_indent_for_compile(toc*o){
	for(unsigned i=2;i<o->scopes.count;i++){
		printf("\t");
	}
}

typedef struct xexpr{
	void (*compile)(const struct xexpr*,struct toc*);
	void (*free)(struct xexpr*);
	str type;
	char bits;
}xexpr;

#define xexpr_def (xexpr){NULL,NULL,str_def,0}

inline static int xexpr_is_empty(xexpr*o){
	return o->compile==NULL;
}




