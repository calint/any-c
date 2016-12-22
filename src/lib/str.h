#pragma once
//-----------------------------------------------------------------------------
// str dynamic array generated from template ... do not modify
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<unistd.h>
#include"../lib/stacktrace.h"
//----------------------------------------------------------------------config

#define str_initial_capacity 8
#define str_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct str{
	char *data;
	long count;
	long cap;
}str;
#define str_def {0,0,0}

//--------------------------------------------------------------------- private

inline static void _str_insure_free_capcity(str*o,long n){
	const long rem=o->cap-o->count;
	if(rem>=n)
		return;
	if(o->data){
		long new_cap=o->cap*2;
		char *new_data=realloc(o->data,sizeof(char)*(unsigned)new_cap);
		if(!new_data){
			fprintf(stderr,"\nout-of-memory");
			fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
			exit(-1);
		}
		if(new_data!=o->data){
			o->data=new_data;
		}
		o->cap=new_cap;
		return;
	}
	o->cap=str_initial_capacity;
	o->data=malloc(sizeof(char)*(unsigned)o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void str_add(str*o,char oo){
	_str_insure_free_capcity(o,1);
	*(o->data+o->count++)=oo;
}

//-----------------------------------------------------------------------------

inline static char str_get(const str*o,long index){
#ifdef str_bounds_check
	if(index>=o->count){
		fprintf(stderr,"\n   index-out-of-bounds at %s:%u\n",__FILE__,__LINE__);
		fprintf(stderr,"        index: %ld  in str: %p  size: %ld  capacity: %ld\n\n",
				index,(void*)o,o->count,o->cap);
		stacktrace_print(stderr);
		fprintf(stderr,"\n\n");
		exit(-1);
	}
#endif
	char p=*(o->data+index);
	return p;
}

//-----------------------------------------------------------------------------

inline static char str_get_last(const str*o){
	char p=*(o->data+o->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t str_size_in_bytes(str const*o){
	return (unsigned)o->count*sizeof(char);
}

//-----------------------------------------------------------------------------

inline static void str_free(str*o){
	if(!o->data)
		return;
	free(o->data);
}

//-----------------------------------------------------------------------------

inline static void str_add_list(str*o,/*copies*/char const*str,long n){
	//? optimize memcpy
	char const*p=str;
	while(n--){
		_str_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void str_add_string(str*o,/*copies*/char const*str){
	//? optimize
	char const*p=str;
	while(*p){
		_str_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void str_write_to_fd(const str*o,int fd){
	if(!o->data)
		return;
	write(fd,o->data,(size_t)o->count);
}

//-----------------------------------------------------------------------------

inline static str str_from_file(const char*path){
	FILE*f=fopen(path,"rb");
	if(!f){
		perror("\ncannot open");
		fprintf(stderr,"\t%s\n\n%s %d\n",path,__FILE__,__LINE__);
		exit(-1);
	}
	long sk=fseek(f,0,SEEK_END);
	if(sk<0){
		fprintf(stderr,"\nwhile fseek\n");
		fprintf(stderr,"\t\n%s %d\n",__FILE__,__LINE__);
		exit(-1);
	}
	long length=ftell(f);
	if(length<0){
		fprintf(stderr,"\nwhile ftell\n");
		fprintf(stderr,"\t\n%s %d\n",__FILE__,__LINE__);
		exit(-1);
	}
	rewind(f);
	char*filedata=(char*)malloc((unsigned)length+1);
	if(!filedata){
		fprintf(stderr,"\nout-of-memory\n");
		fprintf(stderr,"\t\n%s %d\n",__FILE__,__LINE__);
		exit(-1);
	}
	const size_t n=fread(filedata,1,(unsigned)length+1,f);
	if(n!=(unsigned)length){
		fprintf(stderr,"\nnot-a-full-read\n");
		fprintf(stderr,"\t\n%s %d\n",__FILE__,__LINE__);
		exit(-1);
	}
	fclose(f);
	filedata[length]=0;

	const str s={
		.data=filedata,
		.count=((unsigned)length+1)/sizeof(char),
		.cap=(unsigned)length+1
	};
	return s;
}

//-----------------------------------------------------------------------------

inline static void str_clear(str*o){
	o->count=0;
}

//-----------------------------------------------------------------------------

inline static void str_setz(str*o,/*copies*/char const*s){
	//? optimize
	char const*p=s;
	o->count=0;
	while(*p){
		_str_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
	_str_insure_free_capcity(o,1);
	*(o->data+o->count++)=0;
}

//-----------------------------------------------------------------------------
#define str_foa(ls,body)str_foreach_all(ls,({void __fn__ (char o) body __fn__;}))
#define str_foac(ls,body)str_foreach_all_count(ls,({void __fn__ (char o,long i) body __fn__;}))
#define str_fou(ls,body)str_foreach(ls,({int __fn__ (char o) body __fn__;}))
#define str_foar(ls,body)str_foreach_all_rev(ls,({void __fn__ (char o) body __fn__;}))
//-----------------------------------------------------------------------------
inline static void str_foreach(str*o,int(*f)(char)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		char oo=o->data[i];
		if(f(oo))
			break;
	}
}
//-----------------------------------------------------------------------------
inline static void str_foreach_all(str*o,void(*f)(char)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		char oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
inline static void str_foreach_all_count(str*o,void(*f)(char,long)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		char oo=o->data[i];
		f(oo,i);
	}
}
//-----------------------------------------------------------------------------
inline static void str_foreach_all_rev(str*o,void(*f)(char)){
	if(!o->count)
		return;
	for(int i=(signed)o->count-1;i!=0;i--){
		char oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
// returns count if not found otherwise index
inline static long str_find_index(const str*o,char oo){
	for(long i=0;i<o->count;i++){
		if(str_get(o,i)==oo)
			return i;
	}
	return o->count;
}
//-----------------------------------------------------------------------------
inline static long str_has(const str*o,char oo){
	const long i=str_find_index(o,oo);
	if(i==o->count)
		return 0;
	return 1;
}
//-----------------------------------------------------------------------------
inline static/*gives*/str str_from_string(char const*s){
	str o=str_def;
	str_add_string(&o,s);
	str_add(&o,0);
	return o;
}
//-----------------------------------------------------------------------------
