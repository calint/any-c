#pragma once
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct token{
	const char*begin;
	const char*content;
	const char*content_end;
	const char*end;
}token;

#define token_def (token){NULL,NULL,NULL,NULL}

inline static size_t token_size_including_whitespace(token*t){
	return (size_t)(t->end-t->begin);
}

inline static void token_print_including_whitespace(token*t){
//	write(1,t->begin,(unsigned)(t->end-t->begin));
	printf("%.*s",(int)token_size_including_whitespace(t),t->begin);
}

inline static void token_print(token*t){
//	write(1,t->content,(unsigned)(t->content_end-t->content));
	printf("%.*s",(int)token_size_including_whitespace(t),t->begin);
}

inline static unsigned token_size(token*t){
	return (unsigned)(t->content_end-t->content);
}

inline static int token_starts_with(token*t,const char*str){
	return strncmp(str,t->content,strlen(str))==0;
}

inline static bool token_equals(token*t,const char*str){
	const char*p=t->content;//? stdlib
	while(1){
		if(p==t->content_end){
			if(!*str)
				return 1;
			return 0;
		}
		if(*p!=*str)
			return 0;
		p++;
		str++;
	}
}
//
//inline static bool token_equals(token*t,const char*str){
//	const char*p=t->content;
//	while(1){
//		if(!*str)return 1;
//		if(p==t->content_end)return 0;
//		if(*p!=*str)return 0;
//		p++;
//		str++;
//	}
//}

inline static float token_get_float(token*t){
	float f=(float)atof(t->content);//? assuming file ends with whitespace
	return f;
}

inline static int token_get_int(token*t){
	int i=atoi(t->content);//?  assuming file ends with whitespace
	return i;
}

inline static unsigned token_get_uint(token*t){
	int i=atoi(t->content);//?  assuming file ends with whitespace, error?
	if(i<0){
		//? break
		exit(-100);
	}
	return(unsigned)i;
}

//inline static token token_next_from_string(const char*s){
//	const char*p=s;
//	token t;
//	t.begin=s;
//	while(1){
//		if(!*p)break;
//		if(!isspace(*p))break;
//		p++;
//	}
//	t.content=p;
//	while(1){
//		if(!*p)break;
//		if(isspace(*p))break;
//		p++;
//	}
//	t.content_end=p;
//	while(1){
//		if(!*p)break;
//		if(!isspace(*p))break;
//		p++;
//	}
//	t.end=p;
//	return t;
//}

inline static token token_next(const char**s){
	const char*p=*s;
	token t;
	t.begin=p;
	while(1){
		if(!*p)break;
		if(!isspace(*p))break;
		p++;
	}
	t.content=p;
	while(1){
		if(!*p)break;
		if(isspace(*p))break;
		if(*p==':')break;
		if(*p=='{')break;
		if(*p=='}')break;
		if(*p==',')break;
		if(*p=='=')break;
		if(*p==';')break;
		if(*p=='(')break;
		if(*p==')')break;
		if(*p=='!')break;
		if(*p=='>')break;
		if(*p=='<')break;
		p++;
	}
	t.content_end=p;
	while(1){
		if(!*p)break;
		if(!isspace(*p))break;
		p++;
	}
	*s=t.end=p;
	return t;
}

inline static token token_next2(const char**s){
	const char*p=*s;
	token t;
	t.begin=p;
	while(1){
		if(!*p)break;
		if(!isspace(*p))break;
		p++;
	}
	t.content=p;
	while(1){
		if(!*p)break;
		if(isspace(*p))break;
		p++;
	}
	t.content_end=p;
	while(1){
		if(!*p)break;
		if(!isspace(*p))break;
		p++;
	}
	*s=t.end=p;
	return t;
}

inline static token token_from_string_additional_delim(
		const char*s,
		char delim)
{
	const char*p=s;
	token t;
	t.begin=s;
	while(1){
		if(!*p)break;
		if(!isspace(*p))break;
		p++;
	}
	t.content=p;
	while(1){
		if(!*p)break;
		if(isspace(*p))break;
		if(*p==delim){
			p++;
			t.end=t.content_end=p;
			return t;
		}
		p++;
	}
	t.content_end=p;
	while(1){
		if(!*p)break;
		if(!isspace(*p))break;
		p++;
	}
	t.end=p;
	return t;
}

inline static const char*scan_to_including_newline(const char*p){
	while(1){
		if(!*p)
			return p;
		if(*p=='\n'){
			p++;
			return p;
		}
		p++;
	}
}

inline static void token_setz(token*o,str*s){
	str_clear(s);
	str_add_list(s,o->content,(unsigned)(o->content_end-o->content));
	str_add(s,0);
}

inline static /*gives*/str token_to_str2(token*o){
	str s=str_def;
	str_add_list(&s,o->content,(unsigned)(o->content_end-o->content));
	str_add(&s,0);
	return s;
}

inline static /*gives*/str*token_to_str(token*o){
	str*s=malloc(sizeof(str));
	*s=str_def;
	str_add_list(s,o->content,(unsigned)(o->content_end-o->content));
	str_add(s,0);
	return s;
}

inline static bool token_is_empty(token*o){
	return o->content_end==o->content;
}

inline static void token_skip_empty_space(const char**pp){
	while(1){
		if(!**pp)break;
		if(!isspace(**pp))break;
		(*pp)++;
	}
}
