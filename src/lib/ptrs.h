#pragma once
//-----------------------------------------------------------------------------
// ptrs dynamic array generated from template ... do not modify
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<unistd.h>
#include"../lib/stacktrace.h"
//----------------------------------------------------------------------config

#define ptrs_initial_capacity 8
#define ptrs_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct ptrs{
	void* *data;
	long count;
	long cap;
}ptrs;
#define ptrs_def {0,0,0}

//--------------------------------------------------------------------- private

inline static void _ptrs_insure_free_capcity(ptrs*o,long n){
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
	o->cap=ptrs_initial_capacity;
	o->data=malloc(sizeof(void*)*(unsigned)o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void ptrs_add(ptrs*o,void* oo){
	_ptrs_insure_free_capcity(o,1);
	*(o->data+o->count++)=oo;
}

//-----------------------------------------------------------------------------

inline static void* ptrs_get(const ptrs*o,long index){
#ifdef ptrs_bounds_check
	if(index>=o->count){
		fprintf(stderr,"\n   index-out-of-bounds at %s:%u\n",__FILE__,__LINE__);
		fprintf(stderr,"        index: %ld  in ptrs: %p  size: %ld  capacity: %ld\n\n",
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

inline static void* ptrs_get_last(const ptrs*o){
	void* p=*(o->data+o->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t ptrs_size_in_bytes(ptrs const*o){
	return (unsigned)o->count*sizeof(void*);
}

//-----------------------------------------------------------------------------

inline static void ptrs_free(ptrs*o){
	if(!o->data)
		return;
	free(o->data);
}

//-----------------------------------------------------------------------------

inline static void ptrs_add_list(ptrs*o,/*copies*/void* const*str,long n){
	//? optimize memcpy
	void* const*p=str;
	while(n--){
		_ptrs_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void ptrs_add_string(ptrs*o,/*copies*/void* const*str){
	//? optimize
	void* const*p=str;
	while(*p){
		_ptrs_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void ptrs_write_to_fd(const ptrs*o,int fd){
	if(!o->data)
		return;
	write(fd,o->data,(size_t)o->count);
}

//-----------------------------------------------------------------------------

inline static ptrs ptrs_from_file(const char*path){
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

	const ptrs s={
		.data=filedata,
		.count=((unsigned)length+1)/sizeof(char),
		.cap=(unsigned)length+1
	};
	return s;
}

//-----------------------------------------------------------------------------

inline static void ptrs_clear(ptrs*o){
	o->count=0;
}

//-----------------------------------------------------------------------------

inline static void ptrs_setz(ptrs*o,/*copies*/void* const*s){
	//? optimize
	void* const*p=s;
	o->count=0;
	while(*p){
		_ptrs_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
	_ptrs_insure_free_capcity(o,1);
	*(o->data+o->count++)=0;
}

//-----------------------------------------------------------------------------
#define ptrs_foa(ls,body)ptrs_foreach_all(ls,({void __fn__ (void* o) body __fn__;}))
#define ptrs_foac(ls,body)ptrs_foreach_all_count(ls,({void __fn__ (void* o,long i) body __fn__;}))
#define ptrs_fou(ls,body)ptrs_foreach(ls,({int __fn__ (void* o) body __fn__;}))
#define ptrs_foar(ls,body)ptrs_foreach_all_rev(ls,({void __fn__ (void* o) body __fn__;}))
//-----------------------------------------------------------------------------
inline static void ptrs_foreach(ptrs*o,int(*f)(void*)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		void* oo=o->data[i];
		if(f(oo))
			break;
	}
}
//-----------------------------------------------------------------------------
inline static void ptrs_foreach_all(ptrs*o,void(*f)(void*)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		void* oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
inline static void ptrs_foreach_all_count(ptrs*o,void(*f)(void*,long)){
	if(!o->count)
		return;
	for(long i=0;i<o->count;i++){
		void* oo=o->data[i];
		f(oo,i);
	}
}
//-----------------------------------------------------------------------------
inline static void ptrs_foreach_all_rev(ptrs*o,void(*f)(void*)){
	if(!o->count)
		return;
	for(int i=(signed)o->count-1;i!=0;i--){
		void* oo=o->data[i];
		f(oo);
	}
}
//-----------------------------------------------------------------------------
// returns count if not found otherwise index
inline static long ptrs_find_index(const ptrs*o,void* oo){
	for(long i=0;i<o->count;i++){
		if(ptrs_get(o,i)==oo)
			return i;
	}
	return o->count;
}
//-----------------------------------------------------------------------------
inline static long ptrs_has(const ptrs*o,void* oo){
	const long i=ptrs_find_index(o,oo);
	if(i==o->count)
		return 0;
	return 1;
}
//-----------------------------------------------------------------------------
inline static/*gives*/ptrs ptrs_from_string(void* const*s){
	ptrs o=ptrs_def;
	ptrs_add_string(&o,s);
	ptrs_add(&o,0);
	return o;
}
//-----------------------------------------------------------------------------
