#pragma once
#include"../lib.h"

typedef struct toc{
	dynp/*owns*/ci_classes;
	dynp/*own ci_toc_scope*/scopes;
	str src;
}toc;

#define toc_def {dynp_def,dynp_def,str_def}

inline static bool toc_is_assignable_from(toc*o,
		const char*dst,const char*path,const char*src);
//inline static bool ci_toc_is_assignable_from(ci_toc*tc,
//		const char*dst,const char*path,const char*src);

typedef struct ci_toc_scope{
	char type;
	const char*name;
	dynp/*const char**/idents;
}ci_toc_scope;

#define ci_toc_scope_def (ci_toc_scope){0,NULL,dynp_def}

typedef struct ci_toc_ident{
	str type;
	str name;
}toctn;

#define toctn_def (toctn){str_def,str_def}

inline static void toc_scope_free(ci_toc_scope*o){
	dynp_free(&o->idents);
	free(o);
}

inline static void toc_push_scope(toc*o,char type,const char*name){
	ci_toc_scope*s=malloc(sizeof(ci_toc_scope));
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
		ci_toc_scope*s=dynp_get(&o->scopes,j);
		printf("%c %s\n",s->type,s->name);
		for(unsigned i=0;i<s->idents.count;i++){
			toctn*id=(toctn*)dynp_get(&s->idents,i);
			for(unsigned k=0;k<=j;k++){
				printf("    ");
			}
			printf("%s %s\n",id->type.data,id->name.data);
		}
	}
}

inline static void toc_add_ident(toc*o,const char*type,const char*name){
	toctn*id=(toctn*)malloc(sizeof(toctn));
	*id=toctn_def;
	str_setz(&id->type,type);
	str_setz(&id->name,name);
	ci_toc_scope*s=dynp_get_last(&o->scopes);
	dynp_add(&s->idents,id);
//	ci_toc_print(o);
//	printf(" %s  %s  %s\n",s->name,i->type.data,i->name.data);
}


inline static char toc_find_ident_scope_type(toc*oo,const char*name){
	for(int j=(signed)oo->scopes.count-1;j>=0;j--){
		ci_toc_scope*s=dynp_get(&oo->scopes,(unsigned)j);
		for(unsigned i=0;i<s->idents.count;i++){
			toctn*id=(toctn*)dynp_get(&s->idents,i);
			if(!strcmp(id->name.data,name))
				return s->type;
		}
	}
	return 0;
}


inline static const toctn*toc_find_ident(toc*oo,const char*name){
	for(int j=(signed)oo->scopes.count-1;j>=0;j--){
		ci_toc_scope*s=dynp_get(&oo->scopes,(unsigned)j);
		for(unsigned i=0;i<s->idents.count;i++){
			const toctn*id=(const toctn*)dynp_get(&s->idents,i);
			if(!strcmp(id->name.data,name))
				return id;
		}
	}
	return 0;
}

inline static void toc_pop_scope(toc*o){
	ci_toc_scope*s=dynp_get_last(&o->scopes);
//	printf("    popped   %s  \n\n\n",s->name);
	toc_scope_free(s);
	o->scopes.count--;//? pop
}

inline static void toc_print_resolved_identifier_for_assignment(
		toc*tc,const char*id,const char*type){

	const char*p=strpbrk(id,".");
	if(p){
		str sid=str_def;
		str_add_list(&sid,id,p-id);
		str_add(&sid,0);

		const toctn*i=toc_find_ident(tc,sid.data);
		if(!i){
			printf("<file> <line:col> identifier '%s' not found\n",id);
			exit(1);
		}
		if(!toc_is_assignable_from(tc,i->type.data,p+1,type)){
//			const char*ft=ci_toc_meta_get_first_field_type_for_class_name(tc,
//					i->type.data);

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
//	const ci_toc_ident*i=ci_toc_find_ident(tc,id);
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

inline static void toc_print_resolved_function_identifier_call(
		toc*tc,const char*id,unsigned argcount){

	const char*p=strpbrk(id,".");
	if(p){
		str sid=str_def;
		str_add_list(&sid,id,p-id);
		str_add(&sid,0);

		const toctn*i=toc_find_ident(tc,sid.data);
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


inline static void toc_indent_for_source(toc*o){
	for(unsigned i=2;i<o->scopes.count;i++){
		printf("\t");
	}
}

