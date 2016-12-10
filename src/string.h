#pragma once
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include"string.h"
//----------------------------------------------------------------------config

#define string_initial_capacity 8
#define string_bounds_check 1

//------------------------------------------------------------------------ def

typedef struct string{
	char *data;
	unsigned count;
	unsigned cap;
}string;
#define string_def (string{0,0,0})

//--------------------------------------------------------------------- private

inline static void _string_insure_free_capcity(string*o,unsigned n){
	const unsigned rem=o->cap-o->count;
	if(rem>=n)
		return;
	if(o->data){
		unsigned new_cap=o->cap*2;
		char *new_data=realloc(o->data,sizeof(char)*new_cap);
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
	o->cap=string_initial_capacity;
	o->data=malloc(sizeof(char)*o->cap);
	if(!o->data){
		fprintf(stderr,"\nout-of-memory");
		fprintf(stderr,"\tfile: '%s'  line: %d\n\n",__FILE__,__LINE__);
		exit(-1);
	}
}

//---------------------------------------------------------------------- public

inline static void string_add(string*o,char obj){
	_string_insure_free_capcity(o,1);
	*(o->data+o->count++)=obj;
}

//-----------------------------------------------------------------------------

inline static char string_get(string*o,unsigned index){
#ifdef string_bounds_check
	if(index>=o->cap){
		fprintf(stderr,"\nindex-out-of-bounds");
		fprintf(stderr,"\t%s\n\n%d  index: %u    capacity: %u\n",
				__FILE__,__LINE__,index,o->cap);
		exit(-1);
	}
#endif
	char p=*(o->data+index);
	return p;
}

//-----------------------------------------------------------------------------

inline static char string_get_last(string*o){
	char p=*(o->data+o->count-1);
	return p;
}

//-----------------------------------------------------------------------------

inline static size_t string_size_in_bytes(string*o){
	return o->count*sizeof(char);
}

//-----------------------------------------------------------------------------

inline static void string_free(string*o){
	if(!o->data)
		return;
	free(o->data);
}

//-----------------------------------------------------------------------------

inline static void string_add_list(string*o,/*copies*/const char*str,int n){
	//? optimize
	const char*p=str;
	while(n--){
		_string_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void string_add_string(string*o,/*copies*/const char*str){
	//? optimize
	const char*p=str;
	while(*p){
		_string_insure_free_capcity(o,1);
		*(o->data+o->count++)=*p++;
	}
}

//-----------------------------------------------------------------------------

inline static void string_write_to_fd(string*o,int fd){
	if(!o->data)
		return;
	write(fd,o->data,o->count);
}

//-----------------------------------------------------------------------------

inline static/*gives*/char string_new(char*def){
	char*o=malloc(sizeof(char));
	if(def)
		*o=*def;
	return o;
}

//-----------------------------------------------------------------------------

static string _string;

