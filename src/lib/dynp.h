#pragma once
//-----------------------------------------------------------------------------
// dynp dynamic array generated from template ... do not modify
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<unistd.h>
#include"../lib/stacktrace.h"
//----------------------------------------------------------------------config

#define dynp_initial_capacity 8
#define dynp_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct dynp{
	void* *data;
	long count;
	long cap;
}dynp;
#define dynp_def {0,0,0}

//--------------------------------------------------------------------- private

inline static void _dynp_insure_free_capcity(dynp*o,long n){
	const long rem=o->cap-o->count;
	if(rem>=n)
		return;
	if(o->data){
		long new_cap=o->cap*2;
		void* *new_data=realloc(o->data,sizeof(void*)*(unsigned)new_cap);
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
	o->cap=dynp_initial_capacity;
	o->data=malloc(sizeof(void*)*(unsigned)o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void dynp_add(dynp*o,void* oo){
	_dynp_insure_free_capcity(o,1);
	*(o->data+o->count++)=oo;
}

//-----------------------------------------------------------------------------

inline static void* dynp_get(const dynp*o,long index){
#ifdef dynp_bounds_check
	if(index>=o->count){
		fprintf(stderr,"\n   index-out-of-bounds at %s:%u\n",__FILE__,__LINE__);
		fprintf(stderr,"        index: %ld  in dynp: %p  size: %ld  capacity: %ld\n\n",
				index,(void*)o,o->count,o->cap);
		stacktrace_print(stderr);
		fprintf(stderr,"\n\n");
		exit(-1);
	}
#endif
	void* p=*(o->data+index);
	return p;
}

//-----------------------------------------------------------------------------

inline static void* dynp_get_last(const dynp*o){
	void* p=*(o->data+o->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t dynp_size_in_bytes(dynp const*o){
	return (unsigned)o->count*sizeof(void*);
}

//-----------------------------------------------------------------------------

inline static void dynp_free(dynp*o){
	if(!o->data)
		return;
	free(o->data);
}

//-----------------------------------------------------------------------------

inline static void dynp_add_list(dynp*o,/*copies*/void* const*str,long n){
	//? optimize memcpy
	void* const*p=str;
	while(n--){
		_dynp_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void dynp_add_string(dynp*o,/*copies*/void* const*str){
	//? optimize
	void* const*p=str;
	while(*p){
		_dynp_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void dynp_write_to_fd(const dynp*o,int fd){
	if(!o->data)
		return;
	write(fd,o->data,(size_t)o->count);
}

//-----------------------------------------------------------------------------

inline static dynp dynp_from_file(const char*path){
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
	void**filedata=(void**)malloc((unsigned)length+1);
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

	const dynp s={
		.data=filedata,
		.count=((unsigned)length+1)/sizeof(char),
		.cap=(unsigned)length+1
	};
	return s;
}

//-----------------------------------------------------------------------------

inline static void dynp_clear(dynp*o){
	o->count=0;
}

//-----------------------------------------------------------------------------

inline static void dynp_setz(dynp*o,/*copies*/void* const*s){
	//? optimize
	void* const*p=s;
	o->count=0;
	while(*p){
		_dynp_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
	_dynp_insure_free_capcity(o,1);
	*(o->data+o->count++)=0;
}

//-----------------------------------------------------------------------------
#define dynp_foa(ls,body)dynp_foreach_all(ls,({void __fn__ (void* o) body __fn__;}))
#define dynp_foac(ls,body)dynp_foreach_all_count(ls,({void __fn__ (void* o,long i) body __fn__;}))
#define dynp_fou(ls,body)dynp_foreach(ls,({int __fn__ (void* o) body __fn__;}))
#define dynp_foar(ls,body)dynp_foreach_all_rev(ls,({void __fn__ (void* o) body __fn__;}))
//-----------------------------------------------------------------------------
inline static void dynp_foreach(dynp*o,int(*f)(void*)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		void* oo=o->data[i];
		if(f(oo))
			break;
	}
}
//-----------------------------------------------------------------------------
inline static void dynp_foreach_all(dynp*o,void(*f)(void*)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		void* oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
inline static void dynp_foreach_all_count(dynp*o,void(*f)(void*,long)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		void* oo=o->data[i];
		f(oo,i);
	}
}
//-----------------------------------------------------------------------------
inline static void dynp_foreach_all_rev(dynp*o,void(*f)(void*)){
	if(!o->count)
		return;
	for(int i=(signed)o->count-1;i!=0;i--){
		void* oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
// returns count if not found otherwise index
inline static long dynp_find_index(const dynp*o,void* oo){
	for(long i=0;i<o->count;i++){
		if(dynp_get(o,i)==oo)
			return i;
	}
	return o->count;
}
//-----------------------------------------------------------------------------
inline static long dynp_has(const dynp*o,void* oo){
	const long i=dynp_find_index(o,oo);
	if(i==o->count)
		return 0;
	return 1;
}
//-----------------------------------------------------------------------------
inline static/*gives*/dynp dynp_from_string(void* const*s){
	dynp o=dynp_def;
	dynp_add_string(&o,s);
	dynp_add(&o,0);
	return o;
}
//-----------------------------------------------------------------------------
