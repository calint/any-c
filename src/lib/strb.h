#pragma once
//-----------------------------------------------------------------------------
// strb dynamic array generated from template ... do not modify
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<unistd.h>
#include"../lib/stacktrace.h"
//----------------------------------------------------------------------config

#define strb_initial_capacity 8
#define strb_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct strb{
	char *data;
	long count;
	long cap;
}strb;
#define strb_def {0,0,0}

//--------------------------------------------------------------------- private

inline static void _strb_insure_free_capcity(strb*o,long n){
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
	o->cap=strb_initial_capacity;
	o->data=malloc(sizeof(char)*(unsigned)o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void strb_add(strb*o,char oo){
	_strb_insure_free_capcity(o,1);
	*(o->data+o->count++)=oo;
}

//-----------------------------------------------------------------------------

inline static char strb_get(const strb*o,long index){
#ifdef strb_bounds_check
	if(index>=o->count){
		fprintf(stderr,"\n   index-out-of-bounds at %s:%u\n",__FILE__,__LINE__);
		fprintf(stderr,"        index: %ld  in strb: %p  size: %ld  capacity: %ld\n\n",
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

inline static char strb_get_last(const strb*o){
	char p=*(o->data+o->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t strb_size_in_bytes(strb const*o){
	return (unsigned)o->count*sizeof(char);
}

//-----------------------------------------------------------------------------

inline static void strb_free(strb*o){
	if(!o->data)
		return;
	free(o->data);
}

//-----------------------------------------------------------------------------

inline static void strb_add_list(strb*o,/*copies*/char const*str,long n){
	//? optimize memcpy
	char const*p=str;
	while(n--){
		_strb_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void strb_add_string(strb*o,/*copies*/char const*str){
	//? optimize
	char const*p=str;
	while(*p){
		_strb_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void strb_write_to_fd(const strb*o,int fd){
	if(!o->data)
		return;
	write(fd,o->data,(size_t)o->count);
}

//-----------------------------------------------------------------------------

inline static strb strb_from_file(const char*path){
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

	const strb s={
		.data=filedata,
		.count=((unsigned)length+1)/sizeof(char),
		.cap=(unsigned)length+1
	};
	return s;
}

//-----------------------------------------------------------------------------

inline static void strb_clear(strb*o){
	o->count=0;
}

//-----------------------------------------------------------------------------

inline static void strb_setz(strb*o,/*copies*/char const*s){
	//? optimize
	char const*p=s;
	o->count=0;
	while(*p){
		_strb_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
	_strb_insure_free_capcity(o,1);
	*(o->data+o->count++)=0;
}

//-----------------------------------------------------------------------------
#define strb_foa(ls,body)strb_foreach_all(ls,({void __fn__ (char o) body __fn__;}))
#define strb_foac(ls,body)strb_foreach_all_count(ls,({void __fn__ (char o,long i) body __fn__;}))
#define strb_fou(ls,body)strb_foreach(ls,({int __fn__ (char o) body __fn__;}))
#define strb_foar(ls,body)strb_foreach_all_rev(ls,({void __fn__ (char o) body __fn__;}))
//-----------------------------------------------------------------------------
inline static void strb_foreach(strb*o,int(*f)(char)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		char oo=o->data[i];
		if(f(oo))
			break;
	}
}
//-----------------------------------------------------------------------------
inline static void strb_foreach_all(strb*o,void(*f)(char)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		char oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
inline static void strb_foreach_all_count(strb*o,void(*f)(char,long)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		char oo=o->data[i];
		f(oo,i);
	}
}
//-----------------------------------------------------------------------------
inline static void strb_foreach_all_rev(strb*o,void(*f)(char)){
	if(!o->count)
		return;
	for(int i=(signed)o->count-1;i!=0;i--){
		char oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
// returns count if not found otherwise index
inline static long strb_find_index(const strb*o,char oo){
	for(long i=0;i<o->count;i++){
		if(strb_get(o,i)==oo)
			return i;
	}
	return o->count;
}
//-----------------------------------------------------------------------------
inline static long strb_has(const strb*o,char oo){
	const long i=strb_find_index(o,oo);
	if(i==o->count)
		return 0;
	return 1;
}
//-----------------------------------------------------------------------------
inline static/*gives*/strb strb_from_string(char const*s){
	strb o=strb_def;
	strb_add_string(&o,s);
	strb_add(&o,0);
	return o;
}
//-----------------------------------------------------------------------------
